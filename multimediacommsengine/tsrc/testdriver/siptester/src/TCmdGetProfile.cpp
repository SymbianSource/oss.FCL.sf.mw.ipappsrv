/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Implementation
*
*/

#include "CTcSIPContext.h"
#include "SIPConstants.h"
#include "TCmdGetProfile.h"
#include "CTcSIPProfileContainer.h"

#include <sipmanagedprofile.h>

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	-
 *   IDs:			ProfileId, RegistryId
 *
 * OUTPUT:
 *	 Parameters:	Type, IapId, AORs, PrivateIdentity, Name, SigComp, 
 *					AutoRegistration, SecurityNegotiation, Server,
 *					ServerParameter, ServerExtensionParameter, 
 *					ExtensionParameter, Default, Enabled
 *	 IDs:			-
 */
void TCmdGetProfile::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get selected registry
    CTcSIPProfileContainer& container = SelectProfileL();

	// Get profile from the profile registry
	CSIPManagedProfile& profile = GetManagedProfileL( container );

	// -- Execution -----------------------------------------------------------

	// -- Response creation ---------------------------------------------------
	
	GetProfileParamsL( container, &profile );
	
	}

void TCmdGetProfile::GetProfileParamsL( 
    CTcSIPProfileContainer& aContainer,
    CSIPManagedProfile* aProfile )
	{
	__ASSERT_ALWAYS( aProfile, User::Leave( KErrArgument ) );
	// Type
	TSIPProfileTypeInfo type = aProfile->Type();
	if( type.iSIPProfileName != KNullDesC8 )
		{
		CTcStructure* s1 = AddStructureResponseL( KParamProfileType );
		s1->AddItemL( KParamProfileClass, type.iSIPProfileClass );
		s1->AddItemL( KParamProfileName, type.iSIPProfileName );
		}

	// IapName
	TUint32 iapId( 0 );
	User::LeaveIfError( aProfile->GetParameter( KSIPAccessPointId, iapId ) );
	AddTextResponseL( KParamIAPName, iContext.IAPNameL( iapId ) );

	// AOR
    const TDesC8* aor = NULL;
	aProfile->GetParameter( KSIPUserAor, aor );
	if( aor && *aor != KNullDesC8 )
	    {    
    	AddTextResponseL( KParamProfileAOR, *aor );
	    }
	    
	// Registered AOR(s)
	const MDesC8Array* aors = NULL;
	aProfile->GetParameter( KSIPRegisteredAors, aors );
	if( aors && aors->MdcaCount() > 0 )
	    {    
    	AddArrayResponseL( KParamProfileAORs, *aors );
	    }
	    
	// PrivateIdentity
	const TDesC8* privateIdentity = NULL;
	aProfile->GetParameter( KSIPPrivateIdentity, privateIdentity );
	if( privateIdentity && *privateIdentity != KNullDesC8 )
	    {	    
    	AddTextResponseL( KParamProfilePrivateIdentity, *privateIdentity );
	    }

	// Name
	const TDesC8* name = NULL;
	aProfile->GetParameter( KSIPProviderName, name );
	if( name && *name != KNullDesC8 )
	    {  
    	AddTextResponseL( KParamProfileName, *name );
	    }

	// SigComp
	TBool isSigCompEnabled( EFalse );
	aProfile->GetParameter( KSIPSigComp, isSigCompEnabled );
	AddBooleanResponseL( KParamProfileSigComp, isSigCompEnabled );

	// AutoRegistration
	
	TBool isAutoRegEnabled( EFalse );
	aProfile->GetParameter( KSIPAutoRegistration, isAutoRegEnabled );
	AddBooleanResponseL( KParamProfileAutoRegistration, isAutoRegEnabled );

	// SecurityNegotiation
	TBool isSecNegEnabled( EFalse );
	aProfile->GetParameter( KSIPSecurityNegotiation, isSecNegEnabled );
	AddBooleanResponseL( KParamProfileSecurityNegotiation, isSecNegEnabled );
	
	// NegotiatedSecMechanism
	const TDesC8* secmech = NULL;
	aProfile->GetParameter( KSIPNegotiatedSecurityMechanism, secmech );
	if( secmech && *secmech != KNullDesC8 )
	    {
    	AddTextResponseL( KParamNegotiatedSecMechanism, *secmech );
	    }

	// Server
	const TDesC8* address = NULL;
	
	aProfile->GetParameter( KSIPRegistrar, KSIPServerAddress, address );
	if( address && *address != KNullDesC8 )
		{
		CTcStructure* s1 = AddStructureResponseL( KParamProfileServer );
		s1->AddItemL( KRegistrar, *address );
		}
	else
		{
		aProfile->GetParameter( KSIPOutboundProxy, KSIPServerAddress, address );
		if( address && *address != KNullDesC8 )
			{
			CTcStructure* s1 = AddStructureResponseL( KParamProfileServer );
			s1->AddItemL( KOutboundProxy, *address  );
			}
		}
		
	// ServerParameter
    ServerParameterL( KSIPRegistrar, KSIPDigestRealm, aProfile );
	ServerParameterL( KSIPRegistrar, KSIPDigestUserName, aProfile );
	ServerParameterL( KSIPOutboundProxy, KSIPDigestRealm, aProfile );
	ServerParameterL( KSIPOutboundProxy, KSIPDigestUserName, aProfile );

	// ServerExtensionParameter
	CTcStructure* structure;
	do {
		structure = FindStructureL( KParamProfileServerExtensionParameter, EFalse );
		if( structure )
			{
			TPtrC8 server = structure->ItemL( KParamProfileServer )->Value();
			
			const CTcNameValue* extensionParam = structure->ItemL( 1 );
			
			TUint32 serverType = ServerL( structure );

			const TDesC8* value = NULL;
			aProfile->GetParameter( serverType, extensionParam->NameAsIntL(), value ); 
			if( value && *value != KNullDesC8 )
				{
				CTcStructure* s1 = AddStructureResponseL( KParamProfileServerExtensionParameter );
				s1->AddItemL( KParamProfileServer, server );
				s1->AddItemL( extensionParam->Name(), *value );
				}
			iContext.List().RemoveParameter( KParamProfileServerExtensionParameter );
			}
		} while( structure );

	// ExtensionParameter
	do {
		structure = FindStructureL( KParamProfileExtensionParameter, EFalse );
		if( structure )
			{
			const CTcNameValue* extensionParam = structure->ItemL( 0 );

			const TDesC8* value = NULL;	
			TBool boolVal( EFalse );
			TUint32 intVal( 0 );
			
			if ( aProfile->GetParameter( extensionParam->NameAsIntL(), value ) != KErrNotFound )
			    {
    			if( value && *value != KNullDesC8 )
    				{
    				CTcStructure* s1 = AddStructureResponseL( KParamProfileExtensionParameter );
    				s1->AddItemL( extensionParam->Name(), *value );
    				}
			    }
			else if ( aProfile->GetParameter( extensionParam->NameAsIntL(), boolVal ) != KErrNotFound )
			    {
			    CTcStructure* s1 = AddStructureResponseL( KParamProfileExtensionParameter );
    			s1->AddBooleanItemL( extensionParam->Name(), boolVal );
			    }
			else if ( aProfile->GetParameter( extensionParam->NameAsIntL(), intVal ) != KErrNotFound )
			    {
			    CTcStructure* s1 = AddStructureResponseL( KParamProfileExtensionParameter );
    			s1->AddItemL( extensionParam->Name(), intVal );
			    }
			else
			    {
			    }
			
			iContext.List().RemoveParameter( KParamProfileExtensionParameter );
			}
		} while( structure );

    // Header params
	CTcArray* headerParamsArr = FindArrayL( KParamProfileHeaderParams, EFalse );
	if( headerParamsArr && headerParamsArr->Count() > 0 )
		{
    	TUint32 paramType( 0 );
	    TLex8 lexer( headerParamsArr->Item( 0 ) );
	    User::LeaveIfError( lexer.Val( paramType, EDecimal ) );
	    
    	headerParamsArr->RemoveItem( 0 );
    	const MDesC8Array* arr = 0;
    	aProfile->GetParameter( paramType, arr );
    	if( arr && arr->MdcaCount() > 0 )
    	    {    
        	AddArrayResponseL( KParamProfileHeaderParams, *arr );
    	    }
		}
		
	// Default
	TBool isDefault( EFalse );
	aProfile->GetParameter( KSIPDefaultProfile, isDefault );
	AddBooleanResponseL( KParamProfileDefault, isDefault );
	
	// Enabled status
	AddBooleanResponseL( KParamProfileEnabled, 
	                     aContainer.ProfileRegistry().IsEnabled( *aProfile ) );
	}


TBool TCmdGetProfile::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("GetProfile") );
	}

TTcCommandBase* TCmdGetProfile::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetProfile( aContext );
	}

TPtrC8 TCmdGetProfile::ServerToString( TUint32 aServer )
	{
	TPtrC8 value;
	if( aServer == KSIPRegistrar )
		{
		value.Set( KRegistrar );
		}
	else if( aServer == KSIPOutboundProxy )
		{
		value.Set( KOutboundProxy );
		}

	return value;
	}

void TCmdGetProfile::ServerParameterL( TUint32 aServer,
									   TUint32 aServerParameter,
									   const CSIPProfile* aSipProfile )
	{
	const TDesC8* value = NULL;
	aSipProfile->GetParameter( aServer, aServerParameter, value );

	if( value && *value != KNullDesC8 )
		{
		CTcStructure* s1 = AddStructureResponseL( KParamProfileServerParameter );

		// Set server type
		TPtrC8 string = ServerToString( aServer );
		if( string != KNullDesC8 )
			{
			s1->AddItemL( KParamProfileServer, string );
			}

		// Set server parameter
		if( aServerParameter == KSIPDigestRealm )
			{
			string.Set( KDigestRealm );
			}
		else if( aServerParameter == KSIPDigestUserName )
			{
			string.Set( KDigestUsername );
			}

		s1->AddItemL( KParamProfileParameter, string );
		s1->AddItemL( KParamProfileValue, *value );
		}
	}

