/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    
*
*/




#include <sipprofile.h>
#include "mcesecureoutsession.h"
#include "mcemanager.h"
#include "mceevent.h"
#include "mcecomsession.h"
#include "mceitcsender.h"
#include "mceserial.h"
#include "mceclilogs.h"


#define _FLAT_DATA static_cast<CMceComSession*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceSecureOutSession::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceSecureOutSession* CMceSecureOutSession::NewL(
                        CMceManager& aManager,
                        CSIPProfile& aProfile,
                        const TDesC8& aRecipient,
						HBufC8* aOriginator )
    {
    TUint32 profileId(0);
    if ( aProfile.GetParameter( KSIPProfileId, profileId ) != KErrNone )
        {
        User::Leave(KErrNotFound);
        }

    CMceSecureOutSession* self =
    	new ( ELeave ) CMceSecureOutSession( &aManager, profileId );
    CleanupStack::PushL( self );
    self->ConstructL( aRecipient, aOriginator );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceSecureOutSession::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceSecureOutSession* CMceSecureOutSession::NewL( CMceEvent& aEvent )
    {
    CMceSecureOutSession* self =
    	new ( ELeave ) CMceSecureOutSession( &aEvent.Manager(),
                                             aEvent.ProfileId() );
    CleanupStack::PushL( self );
    self->ConstructL( aEvent.Recipient(), 
                      aEvent.Originator(), 
                      aEvent.DialogId() );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceSecureOutSession::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceSecureOutSession* CMceSecureOutSession::NewL( CMceRefer& aRefer )
    {
    CMceSecureOutSession* self =
    	new ( ELeave ) CMceSecureOutSession( &aRefer.Manager(),
                                             aRefer.ProfileId() );
    CleanupStack::PushL( self );
    self->ConstructL( aRefer.Recipient(), 
                      aRefer.Originator(),
                      aRefer.DialogId() );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceSecureOutSession::~CMceSecureOutSession
// -----------------------------------------------------------------------------
//
EXPORT_C CMceSecureOutSession::~CMceSecureOutSession()
    {
    iTemp.Close();
    }

// -----------------------------------------------------------------------------
// CMceSecureOutSession::Type
// -----------------------------------------------------------------------------
//
TMceSessionType CMceSecureOutSession::Type() const
	{
	return KMceSessionSecure;
	}

// -----------------------------------------------------------------------------
// CMceSecureOutSession::SupportedCryptoContextsL
// -----------------------------------------------------------------------------
//
const RArray<TMceCryptoContext>&
CMceSecureOutSession::SupportedCryptoContextsL() const
	{
	return iTemp;	
	}		             

// -----------------------------------------------------------------------------
// CMceSecureOutSession::CryptoContexts
// -----------------------------------------------------------------------------
//
const RArray<TMceCryptoContext>& CMceSecureOutSession::CryptoContexts() const
	{
	return FLAT_DATA( iClientCryptoSuites );	
	}		             

// -----------------------------------------------------------------------------
// CMceSecureOutSession::SetCryptoContextsL
// -----------------------------------------------------------------------------
//
void CMceSecureOutSession::SetCryptoContextsL(
	const RArray<TMceCryptoContext>& aArray )
	{	
	MCECLI_DEBUG( "CMceSecureOutSession::SetCryptoContextsL, Entry" )
	TBool equal( ETrue );
	TInt index = 0;
	TInt count = aArray.Count();
	if ( count != FLAT_DATA( iClientCryptoSuites ).Count())
		{
		equal = !equal;
		}
	else
		{
		for ( index = 0; index < count; index ++)
			{
			if (aArray[ index ] != FLAT_DATA( iClientCryptoSuites )[ index ])
				{
				equal = !equal;
				}
			}
		}
	if ( !equal )
	    {
	    MCECLI_DEBUG( "CMceSecureOutSession::SetCryptoContextsL, setting" )
    	
    	FLAT_DATA( iClientCryptoSuites ).Reset(); 
    	for ( index = 0; index < count; index++)
    		{
	       	FLAT_DATA( iClientCryptoSuites ).InsertL(aArray[index], index );
        	}
	    }
	    
	MCECLI_DEBUG( "CMceSecureOutSession::SetCryptoContextsL, Exit" )
	}		             

// -----------------------------------------------------------------------------
// CMceSecureOutSession::NewL
// -----------------------------------------------------------------------------
//
CMceSecureOutSession* CMceSecureOutSession::NewL()
    {
    CMceSecureOutSession* self =
    	new ( ELeave ) CMceSecureOutSession( NULL, NULL );
    CleanupStack::PushL( self );
    self->ConstructL( KNullDesC8, NULL );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceSecureOutSession::CMceSecureOutSession
// -----------------------------------------------------------------------------
//
CMceSecureOutSession::CMceSecureOutSession( CMceManager* aManager,
											TUint32 aProfileId ) :
    CMceOutSession( aManager, aProfileId )
    {
    //NOP
    }

// -----------------------------------------------------------------------------
// CMceSecureOutSession::ConstructL
// -----------------------------------------------------------------------------
//
void CMceSecureOutSession::ConstructL( const TDesC8& aRecipient,
									   HBufC8* aOriginator )
	{
	// CMceOutSession::ConstructL takes ownership of aOriginator. But as leave
	// may occur after that, create a copy of aOriginator.
	HBufC8* originator( NULL );
	if ( aOriginator )
		{
		originator = aOriginator->AllocL();
		}
	CleanupStack::PushL( originator );
	CMceOutSession::ConstructL( aRecipient, originator );
	CleanupStack::Pop( originator );

	FLAT_DATA( iClientCryptoSuites ).InsertL( EAES_CM_128_HMAC_SHA1_80, 0 );  
    FLAT_DATA( iClientCryptoSuites ).InsertL( EAES_CM_128_HMAC_SHA1_32, 1 );

    iTemp.Reset();
	User::LeaveIfError( iTemp.Append( EAES_CM_128_HMAC_SHA1_80 ) );
    User::LeaveIfError( iTemp.Append( EAES_CM_128_HMAC_SHA1_32 ) );

	// Take ownership when leave can't happen
    delete aOriginator;
	}

// -----------------------------------------------------------------------------
// CMceSecureOutSession::ConstructL
// -----------------------------------------------------------------------------
//
void CMceSecureOutSession::ConstructL( const TDesC8& aRecipient, 
                                 	   const TDesC8& aOriginator,
                                 	   TUint32 aDialogId )
    {
    CMceOutSession::ConstructL( aRecipient, aOriginator, aDialogId );
   	FLAT_DATA( iClientCryptoSuites ).InsertL( EAES_CM_128_HMAC_SHA1_80, 0 );
    FLAT_DATA( iClientCryptoSuites ).InsertL( EAES_CM_128_HMAC_SHA1_32, 1 );

    iTemp.Reset();
	User::LeaveIfError( iTemp.Append( EAES_CM_128_HMAC_SHA1_80 ) );
    User::LeaveIfError( iTemp.Append( EAES_CM_128_HMAC_SHA1_32 ) );
    }
