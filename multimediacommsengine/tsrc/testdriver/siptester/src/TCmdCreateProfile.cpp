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
#include "TCmdCreateProfile.h"
#include "CTcSIPProfileContainer.h"

#include <sipmanagedprofile.h>
#include <sipmanagedprofileregistry.h>
#include "tclog.h"

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	ProfileType*, IapName*, ProfileAORList*, PrivateIdentity*,
 *					ProfileName*, SigComp*, 
 *					AutoRegistration*, SecurityNegotiation*, Server*,
 *					ServerParameter*, ServerExtensionParameter*, 
 *					ExtensionParameter*, ProfileDefault*
 *   IDs:			RegistryId
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			-
 */
void TCmdCreateProfile::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------
	// Get selected registry
    CTcSIPProfileContainer& container = SelectProfileL();

	// Create a new profile, if type parameter is not found use default 
	// Internet, IETF
	CSIPManagedProfile* profile = NULL;
	CTcStructure* structure = FindStructureL( KParamProfileType, EFalse );
	if( structure )
	    {
	    profile = container.ManagedProfileRegistry().CreateL( ProfileTypeInfoL( structure ) );
	    }
	else
	    {
	    TSIPProfileTypeInfo typeInfo;
	    typeInfo.iSIPProfileClass = TSIPProfileTypeInfo::EInternet;
	    typeInfo.iSIPProfileName = KIETF;
	    profile = container.ManagedProfileRegistry().CreateL( typeInfo );
	    }
	CleanupStack::PushL( profile );
	
	// -- Execution -----------------------------------------------------------
	SetProfileParamsL( profile );
	
	// Saving the profile
	container.ManagedProfileRegistry().SaveL( *profile );

	CleanupStack::Pop( profile );
	
	container.AddManagedProfileL( profile ); //store profile

	// -- Response creation ---------------------------------------------------

	AddProfileIdResponseL( *profile );
	}

void TCmdCreateProfile::SetProfileParamsL( CSIPManagedProfile* aProfile )
	{
	// Type	
	TcLog::WriteFormat( _L8("-- Get optional structure \"ProfileType\"") );
	CTcStructure* structure = FindStructureL( KParamProfileType, EFalse );
	if( structure )
		{
		TcLog::Write( KLogOk );
	    aProfile->SetType( ProfileTypeInfoL( structure ) );
		}
	else
		{
		TcLog::Write( KLogNotFound );
		}
	
	// IapId
	TPtrC8 iapname = ExtractTextL( KParamIAPName, EFalse );
	if( iapname != KNullDesC8 )
		{
		TBuf8< KTcMaxTypeName > objectName( iapname );
		TcLog::WriteFormat( _L8("-- Fetching IAP Id for name \"%s\""), objectName.PtrZ() );
		TUint32 iapId = iContext.IAPIdL( iapname );
		User::LeaveIfError( aProfile->SetParameter( KSIPAccessPointId, iapId ) );
		TcLog::Write( KLogOk );
		}

	// AORs
	TcLog::WriteFormat( _L8("-- Get optional array \"ProfileAORList\"") );
	CTcArray* aorsArr = FindArrayL( KParamProfileAORs, EFalse );
	if( aorsArr )
		{
		// Use first item of aorsArr
		TPtrC8 aor = aorsArr->AsMDesCArray().MdcaPoint( 0 );
		if( aor != KNullDesC8 )
		    {
    		TcLog::Write( KLogOk );
    		User::LeaveIfError( aProfile->SetParameter( KSIPUserAor, aor ) );
		    }
		}
	else
		{
		TcLog::Write( KLogNotFound );
		}
		
    // Headers
	TcLog::WriteFormat( _L8("-- Get optional array \"ProfileHeadersList\"") );
	CTcArray* headersArr = FindArrayL( KParamProfileHeaders, EFalse );
	if( headersArr )
		{
    	TcLog::Write( KLogOk );
    	User::LeaveIfError( aProfile->SetParameter( KSIPHeaders, headersArr->AsMDesCArray() ) );
		}
	else
		{
		TcLog::Write( KLogNotFound );
		}
		
	// Header params
	TcLog::WriteFormat( _L8("-- Get optional array \"ProfileHeaderParamsList\"") );
	CTcArray* headerParamsArr = FindArrayL( KParamProfileHeaderParams, EFalse );
	if( headerParamsArr && headerParamsArr->Count() > 0 )
		{
    	TcLog::Write( KLogOk );
    	
    	TUint32 paramType( 0 );
	    TLex8 lexer( headerParamsArr->Item( 0 ) );
	    User::LeaveIfError( lexer.Val( paramType, EDecimal ) );
	    
    	headerParamsArr->RemoveItem( 0 );
    	User::LeaveIfError( aProfile->SetParameter( paramType, headerParamsArr->AsMDesCArray() ) );
		}
	else
		{
		TcLog::Write( KLogNotFound );
		}

	// PrivateIdentity
	TPtrC8 privateIdentity = ExtractTextL( KParamProfilePrivateIdentity, EFalse );
	if( privateIdentity != KNullDesC8 )
		{
		User::LeaveIfError( aProfile->SetParameter( KSIPPrivateIdentity, privateIdentity ) );
		}

	// Name
	TPtrC8 name = ExtractTextL( KParamProfileName, EFalse );
	if( name != KNullDesC8 )
		{
		User::LeaveIfError( aProfile->SetParameter( KSIPProviderName, name ) );
		}

	// SigComp	
	if( HasBooleanL( KParamProfileSigComp ) )
	    {
    	User::LeaveIfError( aProfile->SetParameter( KSIPSigComp, 
    	                                            ExtractBooleanL( KParamProfileSigComp, EFalse ) ) );
	    }

	// AutoRegistration
	if( HasBooleanL( KParamProfileAutoRegistration ) )
	    {
	    User::LeaveIfError( aProfile->SetParameter( KSIPAutoRegistration, 
	                                                ExtractBooleanL( KParamProfileAutoRegistration, EFalse ) ) );
	    }
	    
	// SecurityNegotiation
	if( HasBooleanL( KParamProfileSecurityNegotiation ) )
	    {
    	User::LeaveIfError( aProfile->SetParameter( KSIPSecurityNegotiation, 
    	                                            ExtractBooleanL( KParamProfileSecurityNegotiation, EFalse ) ) );
	    }

	// Server
	do
		{
		structure = FindStructureL( KParamProfileServer, EFalse );
		if( structure )
			{
            User::LeaveIfError( aProfile->SetParameter( 
			                        ServerL( structure ), 
			                        KSIPServerAddress,
			                        structure->ItemL( KParamProfileAddress )->Value() ) );
            
			iContext.List().RemoveParameter( KParamProfileServer );
			}
		} while( structure );


	// ServerParameter
	do
		{
		structure = FindStructureL( KParamProfileServerParameter, EFalse );
		if( structure )
			{
			User::LeaveIfError( aProfile->SetParameter( ServerL( structure ),
										   ServerParameterL( structure ),
										   structure->ItemL( 1 )->Value() ) );
			iContext.List().RemoveParameter( KParamProfileServerParameter );
			}
		} while( structure );

	// ServerExtensionParameter
	do
		{
		structure = FindStructureL( KParamProfileServerExtensionParameter, EFalse );
		if( structure )
			{
			CTcNameValue* item = structure->ItemL( 1 );
			User::LeaveIfError( aProfile->SetParameter( ServerL( structure ),
												        item->NameAsIntL(), item->Value() ) );
			iContext.List().RemoveParameter( KParamProfileServerExtensionParameter );
			}
		} while( structure );

	// ExtensionParameter
	do
		{
		structure = FindStructureL( KParamProfileExtensionParameter, EFalse );
		if( structure )
			{
			const CTcNameValue* item = structure->ItemL( 0 );
			
			TInt extInt( 0 );
			TRAPD( err, extInt = item->ValueAsIntL() );
			if ( !err )
			    {
			    User::LeaveIfError( 
			        aProfile->SetParameter( item->NameAsIntL(), 
			                                static_cast<TUint32>( extInt ) ) );
			    }
			else
			    {
			    TBool extBool( EFalse );
			    TRAPD( err2, extBool = item->ValueAsBooleanL() );
			    if ( !err2 )
			        {
			        User::LeaveIfError( aProfile->SetParameter( item->NameAsIntL(), extBool ) );
			        }
			    else
    			    {
    			    User::LeaveIfError( aProfile->SetParameter( item->NameAsIntL(), item->Value() ) );
    			    }
			    }
			    
			iContext.List().RemoveParameter( KParamProfileExtensionParameter );
			}
		} while( structure );
	
	// Default
	if( ExtractBooleanL( KParamProfileDefault, EFalse ) )
	    {
	    // Can set only to true
	    User::LeaveIfError( aProfile->SetParameter( KSIPDefaultProfile, ETrue ) );
	    }
	    
	}

TBool TCmdCreateProfile::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("CreateProfile") );
	}

TTcCommandBase* TCmdCreateProfile::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdCreateProfile( aContext );
	}

TUint32 TCmdCreateProfile::ServerL( CTcStructure* aStruct )
	{
	TUint32 server( KSIPRegistrar );
	TPtrC8 value = aStruct->ItemL( KParamProfileServer )->Value();

	if( value == KRegistrar )
		{
		server = KSIPRegistrar;
		}
	else if( value == KOutboundProxy )
		{
		server = KSIPOutboundProxy;
		}
	else
		{
		User::Leave( KTcErrMandatoryStructureItemNotFound );
		}

	return server;
	}

TUint32 TCmdCreateProfile::ServerParameterL( CTcStructure* aStruct )
	{
	TUint32 serverParameter = KSIPDigestRealm;
	TPtrC8 value = aStruct->ItemL( 1 )->Name();

	if( value == KDigestRealm )
		{
		serverParameter = KSIPDigestRealm;
		}
	else if( value == KDigestUsername )
		{
		serverParameter = KSIPDigestUserName;
		}
	else if( value == KDigestPassword )
	    {
	    serverParameter = KSIPDigestPassword;
	    }
	else
		{
		User::Leave( KTcErrMandatoryStructureItemNotFound );
		}

	return serverParameter;
	}
	
TSIPProfileTypeInfo TCmdCreateProfile::ProfileTypeInfoL( CTcStructure* aStruct )
    {
	TSIPProfileTypeInfo typeInfo;
	TcLog::WriteFormat( _L8("-- Get mandatory structure item \"ProfileName\"") );
	typeInfo.iSIPProfileName = aStruct->ItemL( KParamProfileName )->Value();
	TcLog::Write( KLogOk );
	
	typeInfo.iSIPProfileClass = ProfileTypeClassL( aStruct );



    return typeInfo;
    }
    
TSIPProfileTypeInfo::TSIPProfileClass 
    TCmdCreateProfile::ProfileTypeClassL( CTcStructure* aStruct )
	{
	TcLog::WriteFormat( _L8("-- Get mandatory structure item \"Class\"") );
	
	TSIPProfileTypeInfo::TSIPProfileClass typeClass = TSIPProfileTypeInfo::EInternet;
	TPtrC8 value = aStruct->ItemL( KParamProfileClass )->Value();

	if( value == KInternet )
		{
		typeClass = TSIPProfileTypeInfo::EInternet;
		}
	else if( value == KIms )
		{
		typeClass =  TSIPProfileTypeInfo::EIms;
		}
	else if( value == KOther )
		{
		typeClass =  TSIPProfileTypeInfo::EOther;
		}
	
	else
		{
		User::Leave( KTcErrMandatoryStructureItemNotFound );
		}
	TcLog::Write( KLogOk );

	return typeClass;
	}
