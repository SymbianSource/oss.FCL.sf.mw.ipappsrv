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


#include "natfwstunbinding.h"
#include "natfwstunclient.h"

// ======== MEMBER FUNCTIONS ========

_LIT(KPublicAddr, "1.2.3.4");

/*
* Helper class that contains all the needed data members.
*/
class CSTUNBindingMembers : public CBase
    {
    public: // Data

        static CSTUNBindingMembers* NewL(RSocket& aSocket)
            {
            CSTUNBindingMembers* self = new(ELeave)CSTUNBindingMembers;
            CleanupStack::PushL(self);
            User::LeaveIfError(self->iAddr.Input(KPublicAddr));
            self->iSocket = aSocket;
            CleanupStack::Pop(self);
            return self;
            }

        ~CSTUNBindingMembers()
            {
            }

        TInetAddr iAddr;
        RSocket iSocket;
    };

// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// CSTUNBinding::NewL
// ---------------------------------------------------------------------------
//
EXPORT_C CSTUNBinding* CSTUNBinding::NewL( CSTUNClient& aSTUNClient,
                                           RSocket& aSocket )
    {
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
                                           TUint aSubstreamId )
    {
    __ASSERT_ALWAYS( aSTUNClient.IsInitialized(), User::Leave( KErrNotReady ) );
    CSTUNBinding* self = CSTUNBinding::NewLC( aSTUNClient,
                                              aStreamId,
                                              aSubstreamId );
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
                                            TUint aSubstreamId )
    {
    CSTUNBinding* self = new ( ELeave ) CSTUNBinding();
    CleanupStack::PushL( self );
    self->ConstructL( aSTUNClient, aStreamId, aSubstreamId );
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
void CSTUNBinding::ConstructL( CSTUNClient& /*aSTUNClient*/, RSocket& aSocket )
    {
    iImplementation =
        reinterpret_cast<CBindingImplementation*>(
            CSTUNBindingMembers::NewL(aSocket));
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::ConstructL
// ---------------------------------------------------------------------------
//
void CSTUNBinding::ConstructL( CSTUNClient& /*aSTUNClient*/,
                               TUint /*aStreamId*/,
                               TUint /*aSubstreamId*/ )
    {
    User::Leave( KErrNotSupported );
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::~CSTUNBinding
// ---------------------------------------------------------------------------
//
EXPORT_C CSTUNBinding::~CSTUNBinding()
    {
    CSTUNBindingMembers* members =
        reinterpret_cast<CSTUNBindingMembers*>(iImplementation);
    delete members;
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::SendRequestL
// ---------------------------------------------------------------------------
//
EXPORT_C void CSTUNBinding::SendRequestL()
    {
    // Allocate memory to simulate allocation failures
    HBufC8* tmp = KNullDesC8().AllocL();
    delete tmp;
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::SendRequestL - overloaded
// ---------------------------------------------------------------------------
//
EXPORT_C void CSTUNBinding::SendRequestL( const TInetAddr& /*aDestAddr*/ )
    {
    // Allocate memory to simulate allocation failures
    HBufC8* tmp = KNullDesC8().AllocL();
    delete tmp;
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::CancelRequest
// ---------------------------------------------------------------------------
//
EXPORT_C void CSTUNBinding::CancelRequest()
    {
    // Access member variable, to cause crash if this object is accessed after
    // it has been deleted.
    CSTUNBindingMembers* members =
        reinterpret_cast<CSTUNBindingMembers*>(iImplementation);
    members->iAddr.Port();
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::Socket
// ---------------------------------------------------------------------------
//
EXPORT_C const RSocket& CSTUNBinding::Socket() const
    {
    return reinterpret_cast<CSTUNBindingMembers*>(iImplementation)->iSocket;
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::AddressResolved
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CSTUNBinding::AddressResolved() const
    {
    return ETrue;
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::PublicAddr
// ---------------------------------------------------------------------------
//
EXPORT_C const TInetAddr& CSTUNBinding::PublicAddr() const
    {
    return reinterpret_cast<CSTUNBindingMembers*>(iImplementation)->iAddr;
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::HandleDataL
// ---------------------------------------------------------------------------
//
EXPORT_C HBufC8* CSTUNBinding::HandleDataL( const TDesC8& aData,
    TBool& aConsumed, TInetAddr& /*aRemoteAddr*/ )
    {
    // Allocate memory to simulate allocation failures
    HBufC8* tmp = aData.AllocL();
    delete tmp;
    aConsumed = ETrue;
    return NULL;
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::SetICESpecificAttributes
// ---------------------------------------------------------------------------
//
EXPORT_C void CSTUNBinding::SetICESpecificAttributes(
    const TICEAttributes& /*aAttributes*/ )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::StreamId
// ---------------------------------------------------------------------------
//
EXPORT_C const TUint& CSTUNBinding::StreamId() const
    {
    return 0;
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::ExecuteImmediateRetransmitL
// ---------------------------------------------------------------------------
//
EXPORT_C void CSTUNBinding::ExecuteImmediateRetransmitL()
    {
    }

// ---------------------------------------------------------------------------
// CSTUNBinding::STUNClient
// ---------------------------------------------------------------------------
//
EXPORT_C const CSTUNClient* CSTUNBinding::STUNClient() const
    {
    return NULL;
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
