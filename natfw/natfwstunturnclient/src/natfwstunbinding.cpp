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




#include "stunassert.h"
#include "natfwstunbinding.h"
#include "cstunbindingimplementation.h"
#include "natfwstunclient.h"
#include "cstunclientimplementation.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSTUNBinding::NewL
// ---------------------------------------------------------------------------
//
EXPORT_C CSTUNBinding* CSTUNBinding::NewL( CSTUNClient& aSTUNClient, 
                                           RSocket& aSocket )
    {
    __ASSERT_ALWAYS( aSTUNClient.IsInitialized(), User::Leave( KErrNotReady ) );
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
    __ASSERT_ALWAYS( aSTUNClient.IsInitialized(), User::Leave( KErrNotReady ) );
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
void CSTUNBinding::ConstructL( CSTUNClient& aSTUNClient, RSocket& aSocket )
    {
    iImplementation =
        CSTUNBindingImplementation::NewL( *this,
                                          aSTUNClient.Implementation(),
                                          aSocket );
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::ConstructL
// ---------------------------------------------------------------------------
//
void CSTUNBinding::ConstructL( CSTUNClient& aSTUNClient,
                               TUint aStreamId,
                               TUint aConnectionId )
    {
    iImplementation =
        CSTUNBindingImplementation::NewL( *this,
                      aSTUNClient.Implementation(),
                      aStreamId,
                      aConnectionId,
                      aSTUNClient.Implementation().MultiplexerInstance() );
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::~CSTUNBinding
// ---------------------------------------------------------------------------
//
EXPORT_C CSTUNBinding::~CSTUNBinding()
    {
    delete iImplementation;
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::SendRequestL
// ---------------------------------------------------------------------------
//
EXPORT_C void CSTUNBinding::SendRequestL()
    {
    Implementation().SendRequestL();
    }


// ---------------------------------------------------------------------------
// CSTUNBinding::SendRequestL - overloaded
// ---------------------------------------------------------------------------
//
EXPORT_C void CSTUNBinding::SendRequestL( const TInetAddr& aDestAddr,
                                          TBool aAddFingerprint,
                                          TUint aRtoValue )
    {
    Implementation().SendRequestL( aDestAddr, aAddFingerprint, aRtoValue );
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::CancelRequest
// ---------------------------------------------------------------------------
//
EXPORT_C void CSTUNBinding::CancelRequest()
    {
    Implementation().CancelRequest();
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::Socket
// ---------------------------------------------------------------------------
//
EXPORT_C const RSocket& CSTUNBinding::Socket() const
    {    
    return Implementation().Socket();
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::AddressResolved
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CSTUNBinding::AddressResolved() const
    {
    return Implementation().AddressResolved();
    }
        
// ---------------------------------------------------------------------------
// CSTUNBinding::PublicAddr
// ---------------------------------------------------------------------------
//
EXPORT_C const TInetAddr& CSTUNBinding::PublicAddr() const
    {    
    return Implementation().PublicAddr();
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::HandleDataL
// ---------------------------------------------------------------------------
//
EXPORT_C HBufC8* CSTUNBinding::HandleDataL( const TDesC8& aData, 
    TBool& aConsumed, TInetAddr& aRemoteAddr )
    {
    return Implementation().HandleDataL( aData, aConsumed, aRemoteAddr );
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::SetICESpecificAttributes
// ---------------------------------------------------------------------------
//
EXPORT_C void CSTUNBinding::SetICESpecificAttributes( 
    const TICEAttributes& aAttributes )
    {
    Implementation().SetICESpecificAttributes( aAttributes );
    }
    
// ---------------------------------------------------------------------------
// CSTUNBinding::StreamId
// ---------------------------------------------------------------------------
//
EXPORT_C TUint CSTUNBinding::StreamId() const
    {
    return Implementation().StreamId();
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::ConnectionId
// ---------------------------------------------------------------------------
//
EXPORT_C TUint CSTUNBinding::ConnectionId() const
    {
    return Implementation().ConnectionId();
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::STUNClient
// ---------------------------------------------------------------------------
//
EXPORT_C const CSTUNClient* CSTUNBinding::STUNClient() const
    {
    return Implementation().STUNClient();
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::AlternateServerAddr
// ---------------------------------------------------------------------------
//
EXPORT_C const TInetAddr& CSTUNBinding::AlternateServerAddr() const
    {    
    return Implementation().AlternateServerAddr();
    }    
    
// ---------------------------------------------------------------------------
// CSTUNBinding::RealmFromResponse
// ---------------------------------------------------------------------------
//
EXPORT_C const HBufC8* CSTUNBinding::RealmFromResponse() const
    {    
    return Implementation().RealmFromResponse();
    }    

// ---------------------------------------------------------------------------
// CSTUNBinding::CancelRetransmission
// ---------------------------------------------------------------------------
//
EXPORT_C void CSTUNBinding::CancelRetransmission()
    {    
    Implementation().CancelRetransmission();
    }
   
// ---------------------------------------------------------------------------
// CSTUNBinding::SendIndicationL
// ---------------------------------------------------------------------------
//
EXPORT_C void CSTUNBinding::SendIndicationL(
    const TInetAddr& aRemoteAddr, const TDesC8& aData, TBool aAddFingerprint )
    {    
    Implementation().SendIndicationL( aRemoteAddr, aData, aAddFingerprint );
    }   

// ---------------------------------------------------------------------------
// CSTUNBinding::Implementation
// ---------------------------------------------------------------------------
//
CBindingImplementation& CSTUNBinding::Implementation()
    {
    __TEST_INVARIANT;

    return *iImplementation;
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::Implementation
// ---------------------------------------------------------------------------
//
const CBindingImplementation& CSTUNBinding::Implementation() const
    {
    __TEST_INVARIANT;

    return *iImplementation;
    }

// -----------------------------------------------------------------------------
// CSTUNBinding::__DbgTestInvariant
// -----------------------------------------------------------------------------
//
void CSTUNBinding::__DbgTestInvariant() const
    {
    #if defined(_DEBUG)
    if ( !iImplementation )
        {
        User::Invariant();
        }
#endif
    }
