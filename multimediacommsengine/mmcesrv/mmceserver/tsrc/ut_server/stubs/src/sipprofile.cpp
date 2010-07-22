/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    implementation
*
*/




//  INCLUDE FILES
#include "sipprofile.h"
#include "sipmanagedprofile.h"
#include "sipprofileregistry.h"
#include "mcetesthelper.h"
#include "cmcetls.h"

_LIT8( KSIPProfileExtenstionParameter, "%u");
const TUint KProfileBufferSize = 25;

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CSIPProfile::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSIPProfile* CSIPProfile::NewL(CSIPProfileRegistry* aSIPRegistry)
	{
    CSIPProfile* self = CSIPProfile::NewLC (aSIPRegistry);
    CleanupStack::Pop();
    return self;
	}

// -----------------------------------------------------------------------------
// CSIPProfile::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSIPProfile* CSIPProfile::NewLC(CSIPProfileRegistry* aSIPRegistry)
	{
	CSIPProfile* self = new(ELeave)CSIPProfile(aSIPRegistry);
    CleanupStack::PushL (self);
    self->ConstructL ();
    return self;
	}

// -----------------------------------------------------------------------------
// CSIPProfile::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSIPProfile* CSIPProfile::NewL()
	{
    CSIPProfile* self = new(ELeave)CSIPProfile();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
	}

// -----------------------------------------------------------------------------
// CSIPProfile::~CSIPProfile
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPProfile::~CSIPProfile()
	{
	delete iAORs;
	delete iAor;
    delete iContactParams;
    delete iDigestRealm;
    delete iPrivateIdentity;
    delete iDigestUsername;
    delete iDigestPassword;
    delete iRequestUriUser;
    delete iContactHeader;
	}

// -----------------------------------------------------------------------------
// CSIPProfile::GetParameter
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CSIPProfile::GetParameter(TUint32 aParam, TDesC8 const *& aVal ) const
	{
	CMCETls* tls = CMCETls::Storage();
	TInt ret = KErrNone;
	if ( tls )
	    {
	    ret = tls->LeaveError( aParam );
	    if ( !ret )
	        {
	        switch ( aParam )
	            {
	            case KSIPUserAor:
	                {	                
	                aVal = iAor;
	                break;
	                }
	            case KSIPDigestRealm:
	                {
	                aVal = iDigestRealm;
	                break;
	                }
	            case KSIPDigestUserName:
	                {
	                aVal = iDigestUsername;
	                break;
	                }
	            case KSIPDigestPassword:
	                {
	                aVal = iDigestPassword;
	                break;
	                }
	            case KSIPPrivateIdentity:
	                {
	                aVal = iPrivateIdentity;
	                break;
	                }
    	         case KSIPContactHeaderUser:
                    {
                    aVal = iRequestUriUser;
                    break;            
                    }
	            case KSIPRegisteredContact:
	                {
	                aVal = iContactHeader;
	                break;
	                }

               default:
	                {
	                ret = KErrNotFound;
	                break;
	                }
	            }
	        }
	    }
	    
	if ( !ret && !aVal )
	    {
	    ret = KErrNotFound;
	    }
	return ret;
	}	


// -----------------------------------------------------------------------------
// CSIPProfile::GetParameter
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CSIPProfile::GetParameter(TUint32 /*aServerType*/, TUint32 aParam, TDesC8 const *& aVal) const
    {
    return GetParameter( aParam, aVal );
    }



// -----------------------------------------------------------------------------
// CSIPProfile::GetParameter
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CSIPProfile::GetParameter(TUint32 aParam, TUint32& aVal) const
	{
	CMCETls* tls = CMCETls::Storage();
	TInt ret = KErrNone;
	if ( tls )
	    {
	    ret = tls->LeaveError( aParam );
	    if ( !ret )
	        {
	        switch ( aParam )
	            {
	            default:
	                {
	                aVal = 1;
	                break;
	                }
	            }
	        }
	    }
	if( aParam == KSIPProfileId )
		{
		aVal = ContextId();
		}
	return ret;
	}	

// -----------------------------------------------------------------------------
// CSIPProfile::GetParameter
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CSIPProfile::GetParameter(TUint32 aParam, TBool& aVal) const
	{
	CMCETls* tls = CMCETls::Storage();
	TInt ret = KErrNone;
	if ( tls )
	    {
	    ret = tls->LeaveError( aParam );
	    if ( !ret )
	        {
	        switch ( aParam )
	            {
	            default:
	                {
	                aVal = ETrue;
	                break;
	                }
	            }
	        }
	    }
	return ret;
	}

// -----------------------------------------------------------------------------
// CSIPProfile::GetParameter
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CSIPProfile::GetParameter(TUint32 aParam, 
                                        MDesC8Array const *& aVal ) const
	{
	CMCETls* tls = CMCETls::Storage();
	TInt ret = KErrNone;
	if ( tls )
	    {
	    ret = tls->LeaveError( aParam );
	    if ( !ret )
	        {
	        switch ( aParam )
                {
                case KSIPRegisteredAors:
                    {
                    aVal = iAORs;
                    break;
                    }
                case KSIPContactHeaderParams:
                    {
                    aVal = iContactParams;
                    break;
                    }
                default:
                    {
                    ret = KErrNotFound;
                    break;
                    }
                }
	        }
	    }
	
	if ( !ret && !aVal )
	    {
	    ret = KErrNotFound;
	    }
	
	return ret;
	}
	
// -----------------------------------------------------------------------------
// CSIPProfile::CSIPProfile
// -----------------------------------------------------------------------------
//	
CSIPProfile::CSIPProfile(CSIPProfileRegistryBase* aRegistry):
	iSIPProfileRegistry(aRegistry), iEnabled(EFalse)
	{
	}

// -----------------------------------------------------------------------------
// CSIPProfile::CSIPProfile
// -----------------------------------------------------------------------------
//	
CSIPProfile::CSIPProfile():
	iEnabled(EFalse), iContextId(1)
	{
	}

// -----------------------------------------------------------------------------
// CSIPProfile::ConstructL
// -----------------------------------------------------------------------------
//	
void CSIPProfile::ConstructL()
	{
    iAORs = new( ELeave ) CDesC8ArrayFlat( 1 );
    iAORs->AppendL( KSIPProfileDefAOR );
    iAor = KSIPProfileDefAOR().AllocL();
    iContactParams = new( ELeave ) CDesC8ArrayFlat( 1 );
    iContactParams->AppendL( KFeatureTag );
    iContactParams->AppendL( KParamValue );
    iContactHeader = KContactHeaderValue().AllocL();
    iRequestUriUser = KRequestUriUser().AllocL();
	}

// -----------------------------------------------------------------------------
// CSIPProfile::IsContextActive
// -----------------------------------------------------------------------------
//	
TBool CSIPProfile::IsContextActive() const
	{
	return ETrue;
	}

void CSIPProfile::SetContextId(TInt aContextId)
	{
	iContextId = aContextId;
	}
	
// -----------------------------------------------------------------------------
// CSIPProfile::ContextId
// -----------------------------------------------------------------------------
//	
TUint32 CSIPProfile::ContextId() const
	{
	return iContextId;
	}

// -----------------------------------------------------------------------------
// CSIPProfile::SetParam
// -----------------------------------------------------------------------------
//	
void CSIPProfile::SetParam( TUint aParam, HBufC8* aValue )
    {
    switch ( aParam )
        {
        case KSIPDigestRealm:
            {
            delete iDigestRealm;
            iDigestRealm = aValue;
            break;
            }
        case KSIPDigestPassword:
            {
            delete iDigestPassword;
            iDigestPassword = aValue;
            break;
            }
        case KSIPDigestUserName:
            {
            delete iDigestUsername;
            iDigestUsername = aValue;
            break;
            }
        case KSIPPrivateIdentity:
            {
            delete iPrivateIdentity;
            iPrivateIdentity = aValue;
            break;            
            }
        case KSIPRegisteredContact:
        	{
        	delete iContactHeader;
        	iContactHeader = aValue;
        	break;
        	}
        default:
            {
            break;
            }
        }
    }