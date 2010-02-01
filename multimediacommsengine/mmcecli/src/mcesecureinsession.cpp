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




#include "mcesecureinsession.h"
#include "mcemanager.h"
#include "mceclientserver.h"
#include "mceserial.h"
#include "mceclilogs.h"

#include "mcecomsession.h"
#include "mceitcsender.h"

#define _FLAT_DATA static_cast<CMceComSession*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceSecureInSession::NewL
// -----------------------------------------------------------------------------
//
CMceSecureInSession* CMceSecureInSession::NewL( RReadStream& aReadStream,
            						CMceManager& aManager,
            						TUint32 aProfileId,
            						TBool aUpdateSession )
    {
    CMceSecureInSession* self =
    	new ( ELeave ) CMceSecureInSession( &aManager, aProfileId );
    CleanupStack::PushL( self );

    MMceComSerializationContext serCtx( aReadStream );
    CleanupClosePushL( serCtx );
    
    self->InitializeInSessionL( serCtx, aUpdateSession );
    CleanupStack::PopAndDestroy(); // serCtx
    
    self->InitializeL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceSecureInSession::NewL
// -----------------------------------------------------------------------------
//

CMceSecureInSession* CMceSecureInSession::NewL()
    {
    CMceSecureInSession* self = new (ELeave) CMceSecureInSession( NULL, NULL );
    CleanupStack::PushL( self );
    self->ConstructL( KNullDesC8, KNullDesC8 );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceSecureInSession::~CMceSecureInSession
// -----------------------------------------------------------------------------
//
EXPORT_C CMceSecureInSession::~CMceSecureInSession()
    {
    iTemp.Close();
    }

// -----------------------------------------------------------------------------
// CMceSecureInSession::Type
// -----------------------------------------------------------------------------
//
TMceSessionType CMceSecureInSession::Type() const
	{
	return KMceSessionSecure;
	}

// -----------------------------------------------------------------------------
// CMceSecureInSession::SupportedCryptoContextsL
// -----------------------------------------------------------------------------
//
const RArray<TMceCryptoContext>&
CMceSecureInSession::SupportedCryptoContextsL() const
	{
	return iTemp;
	}

// -----------------------------------------------------------------------------
// CMceSecureInSession::CryptoContexts
// -----------------------------------------------------------------------------
//
const RArray<TMceCryptoContext>& CMceSecureInSession::CryptoContexts() const
	{
	return FLAT_DATA( iClientCryptoSuites );	
	}		             

// -----------------------------------------------------------------------------
// CMceSecureInSession::SetCryptoContextsL
// -----------------------------------------------------------------------------
//
void CMceSecureInSession::SetCryptoContextsL(
	const RArray<TMceCryptoContext>& aArray )
	{
	MCECLI_DEBUG( "CMceSecureInSession::SetCryptoContextsL, Entry" )
	TBool equal( ETrue );
	TInt index = 0;
	TInt count = aArray.Count();
	if ( count != FLAT_DATA( iClientCryptoSuites ).Count() )
		{
		equal = !equal;
		}
	else
		{
		for ( index = 0; index < count; index ++ )
			{
			if ( aArray[ index ] != FLAT_DATA( iClientCryptoSuites )[ index ] )
				{
				equal = !equal;
				}
			}
		}
	if ( !equal )
	    {
	    MCECLI_DEBUG( "CMceSecureInSession::SetCryptoContextsL, setting" )

    	FLAT_DATA( iClientCryptoSuites ).Reset(); 
    	for ( index = 0; index < count; index++ )
    		{
        	FLAT_DATA( iClientCryptoSuites ).InsertL( aArray[ index ], index );
        	}
	    }
	    
	MCECLI_DEBUG( "CMceSecureInSession::SetCryptoContextsL, Exit" )
	}		             

// -----------------------------------------------------------------------------
// CMceSecureInSession::CMceSecureInSession
// -----------------------------------------------------------------------------
//
CMceSecureInSession::CMceSecureInSession( CMceManager* aManager,
										  TUint32 aProfileId ) :
    CMceInSession( aManager, aProfileId )
    {
    }

// -----------------------------------------------------------------------------
// CMceSecureInSession::ConstructL
// -----------------------------------------------------------------------------
//
void CMceSecureInSession::ConstructL( const TDesC8& aOriginator,
									  const TDesC8& aRecipient )
    {
    CMceInSession::ConstructL( aOriginator, aRecipient );
   	FLAT_DATA( iClientCryptoSuites ).InsertL( EAES_CM_128_HMAC_SHA1_80, 0 );      
    FLAT_DATA( iClientCryptoSuites ).InsertL( EAES_CM_128_HMAC_SHA1_32, 1 );
    iTemp.Reset();
	
	User::LeaveIfError( iTemp.Append(EAES_CM_128_HMAC_SHA1_80 ) );
	User::LeaveIfError( iTemp.Append(EAES_CM_128_HMAC_SHA1_32 ) );
    }

// -----------------------------------------------------------------------------
// CMceSecureInSession::InitializeInSessionL
// -----------------------------------------------------------------------------
//
void CMceSecureInSession:: InitializeInSessionL(
	MMceComSerializationContext& aSerCtx,
	TBool aUpdateSession )
	{
	CMceInSession::InitializeInSessionL( aSerCtx, aUpdateSession );
	iTemp.Reset();
	User::LeaveIfError( iTemp.Append( EAES_CM_128_HMAC_SHA1_80 ) );
	User::LeaveIfError( iTemp.Append( EAES_CM_128_HMAC_SHA1_32 ) );
    }
