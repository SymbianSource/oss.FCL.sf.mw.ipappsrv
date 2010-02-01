/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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




//#include "stunassert.h"
#include "natfwstunbinding.h"
//#include "cstunbindingimplementation.h"
#include "natfwstunclient.h"
//#include "cstunclientimplementation.h"

const TUint KRtoValue = 500;
const TInt KRtoValueUsedInRefreshTest = 555;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSTUNBinding::NewL
// ---------------------------------------------------------------------------
//
EXPORT_C CSTUNBinding* CSTUNBinding::NewL( CSTUNClient& aSTUNClient, 
                                           RSocket& aSocket )
    {
    //__ASSERT_ALWAYS( aSTUNClient.IsInitialized(), User::Leave( KErrNotReady ) );
    CSTUNBinding* self = CSTUNBinding::NewLC( aSTUNClient, aSocket );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::NewL
// ---------------------------------------------------------------------------
//
EXPORT_C CSTUNBinding* CSTUNBinding::NewL( CSTUNClient& aSTUNClient, 
                                           TUint aStreamId, 
                                           TUint aConnectionId )
    {
//    __ASSERT_ALWAYS( aSTUNClient.IsInitialized(), User::Leave( KErrNotReady ) );
    CSTUNBinding* self = CSTUNBinding::NewLC( aSTUNClient, 
                                              aStreamId, 
                                              aConnectionId );
    CleanupStack::Pop( self );
    return self;
    }
    
// ---------------------------------------------------------------------------
// CSTUNBinding::NewLC
// ---------------------------------------------------------------------------
//
EXPORT_C CSTUNBinding* CSTUNBinding::NewLC( CSTUNClient& aSTUNClient, 
                                            RSocket& aSocket )
    {
    CSTUNBinding* self = new ( ELeave ) CSTUNBinding();
    CleanupStack::PushL( self );
    self->ConstructL( aSTUNClient, aSocket );
    return self;
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::NewLC
// ---------------------------------------------------------------------------
//
EXPORT_C CSTUNBinding* CSTUNBinding::NewLC( CSTUNClient& aSTUNClient, 
                                            TUint aStreamId, 
                                            TUint aConnectionId )
    {
    CSTUNBinding* self = new ( ELeave ) CSTUNBinding();
    CleanupStack::PushL( self );
    self->ConstructL( aSTUNClient, aStreamId, aConnectionId );
    return self;
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::CSTUNBinding
// ---------------------------------------------------------------------------
//
CSTUNBinding::CSTUNBinding()
    {
    }


// ---------------------------------------------------------------------------
// CSTUNBinding::CSTUNBinding
// Dummy implementation, as copy constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CSTUNBinding::CSTUNBinding( const CBinding& /*aBinding*/ )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::ConstructL
// ---------------------------------------------------------------------------
//
void CSTUNBinding::ConstructL( CSTUNClient& /*aSTUNClient*/, RSocket& /*aSocket*/ )
    {
/*    iImplementation =
        CSTUNBindingImplementation::NewL( *this,
                                          aSTUNClient.Implementation(),
                                          aSocket );*/
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::ConstructL
// ---------------------------------------------------------------------------
//
void CSTUNBinding::ConstructL( CSTUNClient& /*aSTUNClient*/,
                               TUint /*aStreamId*/,
                               TUint /*aConnectionId*/ )
    {
   
/*    iImplementation =
        CSTUNBindingImplementation::NewL( *this,
                      aSTUNClient.Implementation(),
                      aStreamId,
                      aSubstreamId,
                      aSTUNClient.Implementation().MultiplexerInstance() );*/
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::~CSTUNBinding
// ---------------------------------------------------------------------------
//
EXPORT_C CSTUNBinding::~CSTUNBinding()
    {
//    delete iImplementation;
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::SendRequestL
// ---------------------------------------------------------------------------
//
EXPORT_C void CSTUNBinding::SendRequestL()
    {
//    Implementation().SendRequestL();
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::SendRequestL - overloaded
// ---------------------------------------------------------------------------
//
EXPORT_C void CSTUNBinding::SendRequestL( const TInetAddr& /*aDestAddr*/,
    TBool /*aAddFingerprint*/, TUint aRtoValue )
    {
//    Implementation().SendRequestL( aDestAddr );

   if ( KRtoValueUsedInRefreshTest == aRtoValue )
        {
        CActiveScheduler::Stop();
        }
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::CancelRequest
// ---------------------------------------------------------------------------
//
EXPORT_C void CSTUNBinding::CancelRequest()
    {
//    Implementation().CancelRequest();
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::Socket
// ---------------------------------------------------------------------------
//
EXPORT_C const RSocket& CSTUNBinding::Socket() const
    {
    RSocket* socket = NULL;
//    return Implementation().Socket();
    return *socket;
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::AddressResolved
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CSTUNBinding::AddressResolved() const
    {
    return ETrue;
//    return Implementation().AddressResolved();
    }
        
// ---------------------------------------------------------------------------
// CSTUNBinding::PublicAddr
// ---------------------------------------------------------------------------
//
EXPORT_C const TInetAddr& CSTUNBinding::PublicAddr() const
    {
    return KInetAddrAny;
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::HandleDataL
// ---------------------------------------------------------------------------
//
EXPORT_C HBufC8* CSTUNBinding::HandleDataL( const TDesC8& /*aData*/, 
    TBool& aConsumed, TInetAddr& /*aRemoteAddr*/ )
    {
    aConsumed = ETrue;
    
    HBufC8* buffer = NULL;
//    return Implementation().HandleDataL( aData, aConsumed, aRemoteAddr );
    return buffer;
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::SetICESpecificAttributes
// ---------------------------------------------------------------------------
//
EXPORT_C void CSTUNBinding::SetICESpecificAttributes( 
    const TICEAttributes& /*aAttributes*/ )
    {
//    Implementation().SetICESpecificAttributes( aAttributes );
    }
    
// ---------------------------------------------------------------------------
// CSTUNBinding::StreamId
// ---------------------------------------------------------------------------
//
EXPORT_C TUint CSTUNBinding::StreamId() const
    {
    return 222;
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::ConnectionId
// ---------------------------------------------------------------------------
//
EXPORT_C TUint CSTUNBinding::ConnectionId() const
    {
    return 2;
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::STUNClient
// ---------------------------------------------------------------------------
//
EXPORT_C const CSTUNClient* CSTUNBinding::STUNClient() const
    {
    CSTUNClient* client = NULL;
//    return Implementation().STUNClient();
    return client;
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::AlternateServerAddr
// ---------------------------------------------------------------------------
//
EXPORT_C const TInetAddr& CSTUNBinding::AlternateServerAddr() const
    {
    return TInetAddr();
    }
    
// ---------------------------------------------------------------------------
// CSTUNBinding::Implementation
// ---------------------------------------------------------------------------
//
CBindingImplementation& CSTUNBinding::Implementation()
    {
//    __TEST_INVARIANT;

//    return *iImplementation;

    CBindingImplementation* impl = NULL;
    return *impl;
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::Implementation
// ---------------------------------------------------------------------------
//
const CBindingImplementation& CSTUNBinding::Implementation() const
    {
//    __TEST_INVARIANT;

//    return *iImplementation;
    CBindingImplementation* impl = NULL;
    return *impl;
    }