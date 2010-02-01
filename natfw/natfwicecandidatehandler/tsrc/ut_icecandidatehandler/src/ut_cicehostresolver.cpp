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
* Description:  
*
*/




//  CLASS HEADER
#include "UT_CIceHostResolver.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include "natfwcandidate.h"

//  INTERNAL INCLUDES
#include "cicehostresolver.h"
#include "cncmconnectionmultiplexer_stub.h"
#include "icecandidatehandlertestdefs.h"


// CONSTRUCTION
UT_CIceHostResolver* UT_CIceHostResolver::NewL()
    {
    UT_CIceHostResolver* self = UT_CIceHostResolver::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CIceHostResolver* UT_CIceHostResolver::NewLC()
    {
    UT_CIceHostResolver* self = new( ELeave ) UT_CIceHostResolver();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CIceHostResolver::~UT_CIceHostResolver()
    {
    }

// Default constructor
UT_CIceHostResolver::UT_CIceHostResolver()
    {
    }

// Second phase construct
void UT_CIceHostResolver::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CIceHostResolver::SetupL(  )
    {
    iMultiplexer = CNcmConnectionMultiplexer::NewL( *this );
    iMultiplexer->iConnectionNotifyEnabled = ETrue;
    iSessionId = iMultiplexer->CreateSessionL( KTestIap, 5090, 5500 );
    iStreamId = iMultiplexer->CreateStreamL( iSessionId, 0, KProtocolInetUdp );
    
    CNATFWPluginApi::TNATFWPluginInitParams initParams(
            *this, *iMultiplexer, KTestDomain(), KTestIap );
    
    iResolver = CIceHostResolver::NewL( &initParams );
    }

void UT_CIceHostResolver::Teardown(  )
    {
    delete iResolver;
    iResolver = NULL;
    
    delete iMultiplexer;
    iMultiplexer = NULL;
    iLocalCandidates.ResetAndDestroy();
    }

void UT_CIceHostResolver::UT_CICEHostResolver_HandleCallBackL(  )
    {
    // invalid event
    EUNIT_ASSERT_PANIC(
        iResolver->HandleCallBack( *iResolver, 
        10 ),"USER", 0 );
    }

void UT_CIceHostResolver::UT_CICEHostResolver_LocalAddressL()
    {
    UT_CICEHostResolver_FetchCandidateLL();
    
    for ( TInt i(0); i < iLocalCandidates.Count(); ++i )
        {
        if ( CNATFWCandidate::EHost == iLocalCandidates[i]->Type() )
            {
            TInetAddr address = iResolver->LocalAddress( 
                iLocalCandidates[i]->StreamId(), KAfInet );
            EUNIT_ASSERT( !address.IsUnspecified() );
            }
        else
            {
            TInetAddr address = iResolver->LocalAddress( 
                iLocalCandidates[i]->StreamId(), KAfInet );
            EUNIT_ASSERT( address.IsUnspecified() );
            }
        }
    }

void UT_CIceHostResolver::UT_CICEHostResolver_ConnectServerLL()
    {
    RSocketServ dummyServer;
    TName dummyName;
    iResolver->ConnectServerL( dummyServer, dummyName );
    WaitForEvent( EServerConnected );
    }

void UT_CIceHostResolver::UT_CICEHostResolver_FetchCandidateLL(  )
    {
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        iResolver->FetchCandidateL( KTestStreamIdRtp1, 0, KAfInet );
        iResolver->FetchCandidateL( KTestStreamIdRtcp1, 0, KAfInet );
        WaitForEvent( EFetchingCompleted );
        WaitForEvent( EFetchingCompleted );
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE( 
            iResolver->FetchCandidateL( KTestStreamIdRtp1, 0, KAfInet  ) );
        EUNIT_ASSERT_NO_LEAVE( 
            iResolver->FetchCandidateL( KTestStreamIdRtcp1, 0, KAfInet  ) );
        WaitForEvent( EFetchingCompleted );
        WaitForEvent( EFetchingCompleted );
        }
    }

void UT_CIceHostResolver::UT_CICEHostResolver_SetReceivingStateLL(  )
    {
    UT_CICEHostResolver_FetchCandidateLL();
    
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        if ( 1 == iLocalCandidates.Count() )
            {
            iResolver->SetReceivingStateL( 
                *iLocalCandidates[0], EStreamingStateActive );
            WaitForEvent( MNATFWPluginObserver::EReceivingActivated );
            EUNIT_ASSERT( iEventStreamId == iLocalCandidates[0]->StreamId() );
            }
        
        if ( 2 == iLocalCandidates.Count() )
            {
            iResolver->SetReceivingStateL( 
                *iLocalCandidates[1], EStreamingStateActive );
            
            WaitForEvent( MNATFWPluginObserver::EReceivingActivated );
            EUNIT_ASSERT( iEventStreamId == iLocalCandidates[0]->StreamId() );
            }
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iResolver->SetReceivingStateL( 
            *iLocalCandidates[0], EStreamingStateActive ) );            
        EUNIT_ASSERT_NO_LEAVE( iResolver->SetReceivingStateL( 
            *iLocalCandidates[1], EStreamingStateActive ) );
        WaitForEvent( MNATFWPluginObserver::EReceivingActivated );
        EUNIT_ASSERT( 
            iEventStreamId == KTestStreamIdRtp1
            && KErrNone == iEventErrCode );
        WaitForEvent( MNATFWPluginObserver::EReceivingActivated );
        EUNIT_ASSERT( 
            iEventStreamId == KTestStreamIdRtcp1
            && KErrNone == iEventErrCode );
        
        EUNIT_ASSERT_NO_LEAVE( iResolver->SetReceivingStateL( 
            *iLocalCandidates[0], EStreamingStatePassive ) );
        EUNIT_ASSERT_NO_LEAVE( iResolver->SetReceivingStateL( 
            *iLocalCandidates[1], EStreamingStatePassive ) );
        WaitForEvent( MNATFWPluginObserver::EReceivingDeactivated );
        EUNIT_ASSERT( 
            iEventStreamId == KTestStreamIdRtp1
            && KErrNone == iEventErrCode );
        WaitForEvent( MNATFWPluginObserver::EReceivingDeactivated );
        EUNIT_ASSERT( 
            iEventStreamId == KTestStreamIdRtcp1
            && KErrNone == iEventErrCode );
        
        EUNIT_ASSERT_NO_LEAVE( iResolver->SetReceivingStateL( 
            *iLocalCandidates[0], EStreamingStateActive ) );
        EUNIT_ASSERT_NO_LEAVE( iResolver->SetReceivingStateL( 
            *iLocalCandidates[1], EStreamingStateActive ) );
        WaitForEvent( MNATFWPluginObserver::EReceivingActivated );
        EUNIT_ASSERT( 
            iEventStreamId == KTestStreamIdRtp1
            && KErrNone == iEventErrCode );
        WaitForEvent( MNATFWPluginObserver::EReceivingActivated );
        EUNIT_ASSERT( 
            iEventStreamId == KTestStreamIdRtcp1
            && KErrNone == iEventErrCode );
        
        EUNIT_ASSERT_NO_LEAVE( iResolver->SetReceivingStateL( 
            *iLocalCandidates[0], EStreamingStatePassive ) );
        EUNIT_ASSERT_NO_LEAVE( iResolver->SetReceivingStateL( 
            *iLocalCandidates[1], EStreamingStatePassive ) );
        WaitForEvent( MNATFWPluginObserver::EReceivingDeactivated );
        EUNIT_ASSERT( 
            iEventStreamId == KTestStreamIdRtp1
            && KErrNone == iEventErrCode );
        WaitForEvent( MNATFWPluginObserver::EReceivingDeactivated );
        EUNIT_ASSERT( 
            iEventStreamId == KTestStreamIdRtcp1
            && KErrNone == iEventErrCode );
        }
    }

void UT_CIceHostResolver::UT_CICEHostResolver_SetSendingStateLL(  )
    {
    TInetAddr addr( KRemoteAddr, KRemotePort );
    addr.SetFamily( KAfInet );
    
    UT_CICEHostResolver_FetchCandidateLL();
    
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        if ( 1 == iLocalCandidates.Count() )
            {
            iResolver->SetSendingStateL( 
                *iLocalCandidates[0], EStreamingStateActive, addr );
            WaitForEvent( MNATFWPluginObserver::ESendingActivated );
            EUNIT_ASSERT( iEventStreamId == iLocalCandidates[0]->StreamId() );
            }
        
        if ( 2 == iLocalCandidates.Count() )
            {
            iResolver->SetSendingStateL( 
                *iLocalCandidates[1], EStreamingStateActive, addr );
            
            WaitForEvent( MNATFWPluginObserver::ESendingActivated );
            EUNIT_ASSERT( iEventStreamId == iLocalCandidates[0]->StreamId() );
            }
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE( iResolver->SetSendingStateL( 
            *iLocalCandidates[0], EStreamingStateActive, addr ) );            
        EUNIT_ASSERT_NO_LEAVE( iResolver->SetSendingStateL( 
            *iLocalCandidates[1], EStreamingStateActive, addr ) );
        WaitForEvent( MNATFWPluginObserver::ESendingActivated );
        EUNIT_ASSERT( 
            iEventStreamId == KTestStreamIdRtp1
            && KErrNone == iEventErrCode );
        WaitForEvent( MNATFWPluginObserver::ESendingActivated );
        EUNIT_ASSERT( 
            iEventStreamId == KTestStreamIdRtcp1
            && KErrNone == iEventErrCode );
        
        EUNIT_ASSERT_NO_LEAVE( iResolver->SetSendingStateL( 
            *iLocalCandidates[0], EStreamingStatePassive, addr ) );
        EUNIT_ASSERT_NO_LEAVE( iResolver->SetSendingStateL( 
            *iLocalCandidates[1], EStreamingStatePassive, addr ) );
        WaitForEvent( MNATFWPluginObserver::ESendingDeactivated );
        EUNIT_ASSERT( 
            iEventStreamId == KTestStreamIdRtp1
            && KErrNone == iEventErrCode );
        WaitForEvent( MNATFWPluginObserver::ESendingDeactivated );
        EUNIT_ASSERT( 
            iEventStreamId == KTestStreamIdRtcp1
            && KErrNone == iEventErrCode );
        
        EUNIT_ASSERT_NO_LEAVE( iResolver->SetSendingStateL( 
            *iLocalCandidates[0], EStreamingStateActive, addr ) );
        EUNIT_ASSERT_NO_LEAVE( iResolver->SetSendingStateL( 
            *iLocalCandidates[1], EStreamingStateActive, addr ) );
        WaitForEvent( MNATFWPluginObserver::ESendingActivated );
        EUNIT_ASSERT( 
            iEventStreamId == KTestStreamIdRtp1
            && KErrNone == iEventErrCode );
        WaitForEvent( MNATFWPluginObserver::ESendingActivated );
        EUNIT_ASSERT( 
            iEventStreamId == KTestStreamIdRtcp1
            && KErrNone == iEventErrCode );
        
        EUNIT_ASSERT_NO_LEAVE( iResolver->SetSendingStateL( 
            *iLocalCandidates[0], EStreamingStatePassive, addr ) );
        EUNIT_ASSERT_NO_LEAVE( iResolver->SetSendingStateL( 
            *iLocalCandidates[1], EStreamingStatePassive, addr ) );
        WaitForEvent( MNATFWPluginObserver::ESendingDeactivated );
        EUNIT_ASSERT( 
            iEventStreamId == KTestStreamIdRtp1
            && KErrNone == iEventErrCode );
        WaitForEvent( MNATFWPluginObserver::ESendingDeactivated );
        EUNIT_ASSERT( 
            iEventStreamId == KTestStreamIdRtcp1
            && KErrNone == iEventErrCode );
        }
    }

void UT_CIceHostResolver::UT_CICEHostResolver_GetConnectionIdLL()
    {
    UT_CICEHostResolver_FetchCandidateLL();
    
    // test with host, stun & relay candidates
    TInt hostCandidateInd( KErrNotFound );
    TUint connectionId( 0 );
    for ( TInt i(0); i < iLocalCandidates.Count(); ++i )
        {
        if ( TIceTestUtils::AllocTestEnabled() )
            {
            iResolver->GetConnectionIdL( 
                *iLocalCandidates[i], connectionId );
            }
        else
            {
            if ( CNATFWCandidate::EHost == iLocalCandidates[i]->Type() )
                {
                hostCandidateInd = i;
                EUNIT_ASSERT_NO_LEAVE( iResolver->GetConnectionIdL( 
                    *iLocalCandidates[i], connectionId ) );
                }
            else
                {
                EUNIT_ASSERT_SPECIFIC_LEAVE( iResolver->GetConnectionIdL( 
                    *iLocalCandidates[i], connectionId ), KErrNotFound );
                }
            }
        }
    
    // test with peer reflexive candidate
    if ( KErrNotFound != hostCandidateInd )
        {
        CNATFWCandidate* peerCandidate 
            = CNATFWCandidate::NewLC( *iLocalCandidates[hostCandidateInd] );
        peerCandidate->SetType( CNATFWCandidate::EPeerReflexive );
        
        if ( TIceTestUtils::AllocTestEnabled() )
            {
            iResolver->GetConnectionIdL( *peerCandidate,
                connectionId );
            }
        else
            {
            EUNIT_ASSERT_NO_LEAVE(
                iResolver->GetConnectionIdL( *peerCandidate, connectionId ) );
            }
        
        CleanupStack::PopAndDestroy( peerCandidate );
        }
    }

void UT_CIceHostResolver::WaitForEvent( TNATFWPluginEvent aEvent )
    {
    iEventToWait = aEvent;
    CActiveScheduler::Start();
    }
    
// From observer interface
void UT_CIceHostResolver::Error( 
        const CNATFWPluginApi& /*aPlugin*/,
        TUint aStreamId, TInt aErrorCode )
    {
    iEventStreamId = aStreamId;
    iEventErrCode = aErrorCode;
    };

void UT_CIceHostResolver::Notify( 
        const CNATFWPluginApi& /*aPlugin*/,
        TUint aStreamId, TNATFWPluginEvent aEvent, TInt aErrCode )
    {
    if ( iEventToWait == aEvent )
        {
        iEventErrCode = aErrCode;
        iEventStreamId = aStreamId;
        
        CActiveScheduler::Stop();
        }
    }
    
void UT_CIceHostResolver::NewCandidatePairFound( 
        const CNATFWPluginApi& /*aPlugin*/,
        CNATFWCandidatePair* /*aCandidatePair*/ )
    {
    
    }

void UT_CIceHostResolver::NewLocalCandidateFound( 
        const CNATFWPluginApi& /*aPlugin*/,
        CNATFWCandidate* aLocalCandidate )
    {
    EUNIT_ASSERT( 
        aLocalCandidate->StreamId() == KTestStreamIdRtp1
        || aLocalCandidate->StreamId() == KTestStreamIdRtcp1 );
    
    TInt err = iLocalCandidates.Append( aLocalCandidate );
    if ( err )
        {
        delete aLocalCandidate;
        }
    }
    
// from MNATFWConnectionMultiplexerObserver
void UT_CIceHostResolver::Notify( 
        TUint /*aSessionId*/, TUint /*aStreamId*/,
        TNotifyType /*aType*/, TInt /*aError*/ )
    {
    
    }

void UT_CIceHostResolver::IcmpError( 
        TUint /*aSessionId*/, TUint /*aStreamId*/, 
        const TInetAddr& /*aAddress*/ )
    {
    
    }

HBufC8* UT_CIceHostResolver::IncomingMessageL( 
    TUint /*aStreamId*/, const TDesC8& /*aMessage*/, 
    const TInetAddr& /*aLocalAddr*/, const TInetAddr& /*aFromAddr*/,
    TInetAddr& /*aPeerRemoteAddr*/, TBool& /*aConsumed*/ )
    {
    return NULL;
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CIceHostResolver,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "HandleCallBack - test ",
    "CIceHostResolver",
    "HandleCallBack",
    "FUNCTIONALITY",
    SetupL, UT_CICEHostResolver_HandleCallBackL, Teardown)

EUNIT_TEST(
    "LocalAddress - test ",
    "CIceHostResolver",
    "LocalAddress",
    "FUNCTIONALITY",
    SetupL, UT_CICEHostResolver_LocalAddressL, Teardown)

EUNIT_TEST(
    "ConnectServerL - test ",
    "CIceHostResolver",
    "ConnectServerL",
    "FUNCTIONALITY",
    SetupL, UT_CICEHostResolver_ConnectServerLL, Teardown)

EUNIT_TEST(
    "FetchCandidateL - test ",
    "CIceHostResolver",
    "FetchCandidateL",
    "FUNCTIONALITY",
    SetupL, UT_CICEHostResolver_FetchCandidateLL, Teardown)

EUNIT_TEST(
    "SetReceivingStateL - test ",
    "CIceHostResolver",
    "SetReceivingStateL ",
    "FUNCTIONALITY",
    SetupL, UT_CICEHostResolver_SetReceivingStateLL, Teardown)

EUNIT_TEST(
    "SetSendingStateL - test ",
    "CIceHostResolver",
    "SetSendingStateL",
    "FUNCTIONALITY",
    SetupL, UT_CICEHostResolver_SetSendingStateLL, Teardown)

EUNIT_TEST(
    "GetConnectionIdL - test ",
    "CIceHostResolver",
    "GetConnectionIdL",
    "FUNCTIONALITY",
    SetupL, UT_CICEHostResolver_GetConnectionIdLL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
