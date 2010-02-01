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
#include "ut_ciceconnection.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include "natfwcandidate.h"

//  INTERNAL INCLUDES
#include "ciceconnection.h"
#include "cncmconnectionmultiplexer_stub.h"
#include "icecandidatehandlertestdefs.h"


// CONSTRUCTION
UT_CIceConnection* UT_CIceConnection::NewL()
    {
    UT_CIceConnection* self = UT_CIceConnection::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CIceConnection* UT_CIceConnection::NewLC()
    {
    UT_CIceConnection* self = new( ELeave ) UT_CIceConnection();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CIceConnection::~UT_CIceConnection()
    {
    delete iConnection;
    delete iMultiplexer;
    }

// Default constructor
UT_CIceConnection::UT_CIceConnection()
    {
    }

// Second phase construct
void UT_CIceConnection::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS
void UT_CIceConnection::SetupL(  )
    {
    iMultiplexer = CNcmConnectionMultiplexer::NewL( *this );
    iMultiplexer->iConnectionNotifyEnabled = EFalse;
    iSessionId = iMultiplexer->CreateSessionL( KTestIap, 5090, 5500 );
    iStreamId = iMultiplexer->CreateStreamL( iSessionId, 0, KProtocolInetUdp );
    
    iConnection = CIceConnection::NewL( 
        iStreamId, KAfInet, *iMultiplexer, *this );
    }

void UT_CIceConnection::Teardown(  )
    {
    delete iConnection;
    iConnection = NULL;
    
    delete iMultiplexer;
    iMultiplexer = NULL;
    }


void UT_CIceConnection::UT_CIceConnection_StreamIdL(  )
    {
    EUNIT_ASSERT( iConnection->StreamId() == iStreamId );
    }

void UT_CIceConnection::UT_CIceConnection_StateL(  )
    {
    EUNIT_ASSERT( iConnection->State() 
        == CIceConnection::EConnectionStateUninitialized );
    }

void UT_CIceConnection::UT_CIceConnection_TransportProtocolL(  )
    {
    EUNIT_ASSERT( KProtocolInetUdp == iConnection->TransportProtocol() );
    }

void UT_CIceConnection::UT_CIceConnection_LocalAddressLL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iConnection->LocalAddress() );
    }

void UT_CIceConnection::UT_CIceConnection_SetReceivingStateLL(  )
    {
    iMultiplexer->iConnectionNotifyEnabled = ETrue;
    
    iConnection->SetReceivingStateL( EStreamingStateActive, 
        CIceConnection::ENotifyStateEnabled );
    CActiveScheduler::Start();
    EUNIT_ASSERT( 
        MIceConnectionObserver::ENotifyRecvActivated == iLatestNotifyType
        && KErrNone == iLatestError );
    
    iConnection->SetReceivingStateL( EStreamingStatePassive,
        CIceConnection::ENotifyStateEnabled );
    CActiveScheduler::Start();
    EUNIT_ASSERT( 
        MIceConnectionObserver::ENotifyRecvDeactivated == iLatestNotifyType
        && KErrNone == iLatestError );
    }

void UT_CIceConnection::UT_CIceConnection_SetSendingStateLL(  )
    {
    iMultiplexer->iConnectionNotifyEnabled = ETrue;
    
    TInetAddr unspecAddress;
    iConnection->SetSendingStateL( EStreamingStateActive, 
        CIceConnection::ENotifyStateEnabled, unspecAddress );
    CActiveScheduler::Start();
    EUNIT_ASSERT( 
        MIceConnectionObserver::ENotifySendActivated == iLatestNotifyType
        && KErrNone == iLatestError );
    
    iConnection->SetSendingStateL( EStreamingStatePassive, 
        CIceConnection::ENotifyStateEnabled, unspecAddress );
    CActiveScheduler::Start();
    EUNIT_ASSERT( 
        MIceConnectionObserver::ENotifySendDeactivated == iLatestNotifyType
        && KErrNone == iLatestError );
    }

void UT_CIceConnection::UT_CIceConnection_BindWithCandidateLL()
    {
    CNATFWCandidate* candidate = CNATFWCandidate::NewLC();
    candidate->SetType( CNATFWCandidate::EHost );
    TInetAddr addr( INET_ADDR( 192, 168, 0, 1 ), 5000 );
    candidate->SetTransportAddrL( addr );
    candidate->SetBase( addr );
    
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        iConnection->BindWithCandidateL( *candidate );
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE( 
            iConnection->BindWithCandidateL( *candidate ) );
        }
    
    CleanupStack::PopAndDestroy( candidate );
    }

void UT_CIceConnection::UT_CIceConnection_IsBoundToCandidateL()
    {
    CNATFWCandidate* candidate = CNATFWCandidate::NewLC();
    candidate->SetType( CNATFWCandidate::EHost );
    TInetAddr addr( INET_ADDR( 192, 168, 0, 1 ), 5000 );
    candidate->SetTransportAddrL( addr );
    candidate->SetBase( addr );
    
    EUNIT_ASSERT( !iConnection->IsBoundToCandidate( *candidate ) );
    
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        iConnection->BindWithCandidateL( *candidate );
        EUNIT_ASSERT( iConnection->IsBoundToCandidate( *candidate ) );
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE( 
            iConnection->BindWithCandidateL( *candidate ) );
        
        candidate->SetType( CNATFWCandidate::ERelay );
        EUNIT_ASSERT( !iConnection->IsBoundToCandidate( *candidate ) );
        candidate->SetType( CNATFWCandidate::EHost );
        
        TInetAddr wrongAddr( addr );
        wrongAddr.SetPort( 5001 );
        candidate->SetTransportAddrL( wrongAddr );
        EUNIT_ASSERT( !iConnection->IsBoundToCandidate( *candidate ) );
        candidate->SetTransportAddrL( addr );
        
        candidate->SetBase( wrongAddr );
        EUNIT_ASSERT( !iConnection->IsBoundToCandidate( *candidate ) );
        candidate->SetBase( addr );
        
        EUNIT_ASSERT( iConnection->IsBoundToCandidate( *candidate ) );
        }
    
    CleanupStack::PopAndDestroy( candidate );
    }

// from MNATFWConnectionMultiplexerObserver
void UT_CIceConnection::Notify( 
        TUint /*aSessionId*/, TUint /*aStreamId*/,
        MNcmConnectionMultiplexerObserver::TNotifyType /*aType*/, 
        TInt /*aError*/ )
    {
    
    }


// From MIceConnectionObserver
void UT_CIceConnection::ConnectionNotify( CIceConnection& /*aConnection*/,
        MIceConnectionObserver::TNotifyType aType, TInt aError )
    {
    iLatestNotifyType = aType;
    iLatestError = aError;
    CActiveScheduler::Stop();
    }


//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    UT_CIceConnection,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "StreamId - test ",
    "CIceConnection",
    "StreamId",
    "FUNCTIONALITY",
    SetupL, UT_CIceConnection_StreamIdL, Teardown)

EUNIT_TEST(
    "State - test ",
    "CIceConnection",
    "State",
    "FUNCTIONALITY",
    SetupL, UT_CIceConnection_StateL, Teardown)

EUNIT_TEST(
    "TransportProtocol - test ",
    "CIceConnection",
    "TransportProtocol",
    "FUNCTIONALITY",
    SetupL, UT_CIceConnection_TransportProtocolL, Teardown)

EUNIT_TEST(
    "LocalAddressL - test ",
    "CIceConnection",
    "LocalAddressL",
    "FUNCTIONALITY",
    SetupL, UT_CIceConnection_LocalAddressLL, Teardown)

EUNIT_TEST(
    "SetReceivingStateL - test ",
    "CIceConnection",
    "SetReceivingStateL",
    "FUNCTIONALITY",
    SetupL, UT_CIceConnection_SetReceivingStateLL, Teardown)

EUNIT_TEST(
    "SetSendingStateL - test ",
    "CIceConnection",
    "SetSendingStateL",
    "FUNCTIONALITY",
    SetupL, UT_CIceConnection_SetSendingStateLL, Teardown)

EUNIT_TEST(
    "BindWithCandidateL - test ",
    "CIceConnection",
    "BindWithCandidateL",
    "FUNCTIONALITY",
    SetupL, UT_CIceConnection_BindWithCandidateLL, Teardown)
    
EUNIT_TEST(
    "IsBoundToCandidate - test ",
    "CIceConnection",
    "IsBoundToCandidate",
    "FUNCTIONALITY",
    SetupL, UT_CIceConnection_IsBoundToCandidateL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
