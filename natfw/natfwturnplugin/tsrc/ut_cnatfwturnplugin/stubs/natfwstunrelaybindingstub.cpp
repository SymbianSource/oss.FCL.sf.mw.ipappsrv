/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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




#include "stunassert.h"
#include "natfwstunrelaybinding.h"
#include "cstunrelaybindingimplementation.h"
#include "natfwstunclient.h"
#include "cstunclientimplementation.h"
#include "ut_cnatfwturnconnectionhandler.h"


const TInt KRtoValueUsedInRefreshTest = 555;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSTUNRelayBinding::NewL
// ---------------------------------------------------------------------------
//
EXPORT_C CSTUNRelayBinding* CSTUNRelayBinding::NewL( CSTUNClient& aSTUNClient, 
                                                     TUint aStreamId, 
                                                     TUint aConnectionId )
    {
    __ASSERT_ALWAYS( aSTUNClient.IsInitialized(), User::Leave( KErrNotReady ) );
    CSTUNRelayBinding* self = CSTUNRelayBinding::NewLC( aSTUNClient,
                                                        aStreamId,
                                                        aConnectionId );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBinding::NewLC
// ---------------------------------------------------------------------------
//
EXPORT_C CSTUNRelayBinding* CSTUNRelayBinding::NewLC( CSTUNClient& aSTUNClient, 
                                                      TUint aStreamId, 
                                                      TUint aConnectionId )
    {
    CSTUNRelayBinding* self = new ( ELeave ) CSTUNRelayBinding();
    CleanupStack::PushL( self );
    self->ConstructL( aSTUNClient, aStreamId, aConnectionId );
    return self;
    }
    
// ---------------------------------------------------------------------------
// CSTUNRelayBinding::CSTUNRelayBinding
// ---------------------------------------------------------------------------
//
CSTUNRelayBinding::CSTUNRelayBinding()
    {
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBinding::CSTUNRelayBinding
// Dummy implementation, as copy constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CSTUNRelayBinding::CSTUNRelayBinding( const CBinding& /*aBinding*/ )
    {
    }


// ---------------------------------------------------------------------------
// CSTUNRelayBinding::ConstructL - overloaded
// ---------------------------------------------------------------------------
//
void CSTUNRelayBinding::ConstructL( CSTUNClient& /*aSTUNClient*/,
                                    TUint /*aStreamId*/,
                                    TUint /*aConnectionId*/ ) 
    {
/*    __ASSERT_ALWAYS( aSTUNClient.Implementation().MultiplexerInstance(), 
                     User::Leave( KErrBadHandle ) );
    iImplementation =
        CSTUNRelayBindingImplementation::NewL( *this,
                       aSTUNClient.Implementation(),
                       aStreamId,
                       aConnectionId,
                       aSTUNClient.Implementation().MultiplexerInstance() );*/
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBinding::ConstructL
// ---------------------------------------------------------------------------
//
void CSTUNRelayBinding::ConstructL( CSTUNClient& /*aSTUNClient*/,
                                    RSocket& /*aSocket*/ )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBinding::~CSTUNRelayBinding
// ---------------------------------------------------------------------------
//
EXPORT_C CSTUNRelayBinding::~CSTUNRelayBinding()
    {
//    delete iImplementation;
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBinding::CancelRequest
// ---------------------------------------------------------------------------
//
EXPORT_C void CSTUNRelayBinding::CancelRequest()
    {
//    Implementation().CancelRequest();
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBinding::AllocateRequestL
// ---------------------------------------------------------------------------
//
EXPORT_C void CSTUNRelayBinding::AllocateRequestL( TUint aRtoValue )
    {
//    Implementation().AllocateRequestL();

    if ( KRtoValueUsedInRefreshTest == aRtoValue )
        {
        if ( UT_CNATFWTurnConnectionHandler::iActiveSchedulerWait.IsStarted() )
            {
            UT_CNATFWTurnConnectionHandler::iActiveSchedulerWait.AsyncStop();
            }
        }
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBinding::AddressResolved
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CSTUNRelayBinding::AddressResolved() const
    {
    return ETrue;
//    return Implementation().AddressResolved();
    }
        
// ---------------------------------------------------------------------------
// CSTUNRelayBinding::PublicAddr
// ---------------------------------------------------------------------------
//
EXPORT_C const TInetAddr& CSTUNRelayBinding::PublicAddr() const
    {    
//    return Implementation().PublicAddr();
    return KInetAddrAny;
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBinding::RelayAddr
// ---------------------------------------------------------------------------
//
EXPORT_C const TInetAddr& CSTUNRelayBinding::RelayAddr() const
    {
//    return Implementation().RelayAddr();
    return KInetAddrAny;
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBinding::HandleDataL
// ---------------------------------------------------------------------------
//
EXPORT_C HBufC8* CSTUNRelayBinding::HandleDataL( const TDesC8& /*aData*/, 
    TBool& /*aConsumed*/, TInetAddr& /*aRemoteAddr*/ )
    {
    HBufC8* buffer = NULL;
//    return Implementation().HandleDataL( aData, aConsumed, aRemoteAddr );
    return buffer;
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBinding::SetICESpecificAttributes
// ---------------------------------------------------------------------------
//
EXPORT_C void CSTUNRelayBinding::SetICESpecificAttributes( 
    const TICEAttributes& /*aAttributes*/ )
    {
//    Implementation().SetICESpecificAttributes( aAttributes );
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBinding::SetActiveDestinationRequestL
// ---------------------------------------------------------------------------
//
EXPORT_C void CSTUNRelayBinding::SetActiveDestinationRequestL( 
    const TInetAddr& /*aRemoteAddr*/, TUint32& /*aTimerValue*/ )
    {
//    Implementation().SetActiveDestinationRequestL( aRemoteAddr, aTimerValue );
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBinding::SendIndicationL
// ---------------------------------------------------------------------------
//
EXPORT_C void CSTUNRelayBinding::SendIndicationL( 
    const TInetAddr& /*aRemoteAddr*/, const TDesC8& /*aData*/, TBool /*aAddFingerprint*/ )
    {
//    Implementation().SendIndicationL( aRemoteAddr, aData );
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBinding::ConnectRequestL
// ---------------------------------------------------------------------------
//
EXPORT_C void CSTUNRelayBinding::ConnectRequestL( const TInetAddr& /*aRemoteAddr*/ )
    {
//    Implementation().ConnectRequestL( aRemoteAddr );
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBinding::StreamId
// ---------------------------------------------------------------------------
//
EXPORT_C TUint CSTUNRelayBinding::StreamId() const
    {
//    return Implementation().StreamId();
    return 222;
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBinding::ConnectionId
// ---------------------------------------------------------------------------
//
EXPORT_C TUint CSTUNRelayBinding::ConnectionId() const
    {
    return 2;
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBinding::STUNClient
// ---------------------------------------------------------------------------
//
EXPORT_C const CSTUNClient* CSTUNRelayBinding::STUNClient() const
    {
    CSTUNClient* client = NULL;
//    return Implementation().STUNClient();
    return client;
    }    

// ---------------------------------------------------------------------------
// CSTUNRelayBinding::AlternateServerAddr
// ---------------------------------------------------------------------------
//
EXPORT_C const TInetAddr& CSTUNRelayBinding::AlternateServerAddr() const
    {    
    return TInetAddr();
    }
    
// ---------------------------------------------------------------------------
// CSTUNRelayBinding::RealmFromResponse
// ---------------------------------------------------------------------------
//
EXPORT_C const HBufC8* CSTUNRelayBinding::RealmFromResponse() const
    {    
//    return Implementation().RealmFromResponse();
    return NULL;
    }
    
// ---------------------------------------------------------------------------
// CSTUNRelayBinding::Implementation
// ---------------------------------------------------------------------------
//
CBindingImplementation& CSTUNRelayBinding::Implementation()
    {
//    __TEST_INVARIANT;

//    return *iImplementation;
    CBindingImplementation* impl = NULL;
    return *impl;
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBinding::Implementation
// ---------------------------------------------------------------------------
//
const CBindingImplementation& CSTUNRelayBinding::Implementation() const
    {
//    __TEST_INVARIANT;

//    return *iImplementation;
    CBindingImplementation* impl = NULL;
    return *impl;
    }

