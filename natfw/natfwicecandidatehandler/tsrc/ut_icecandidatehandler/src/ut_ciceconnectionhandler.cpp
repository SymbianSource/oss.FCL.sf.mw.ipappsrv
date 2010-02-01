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
#include "ut_ciceconnectionhandler.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include "natfwcandidate.h"
#include "natfwcandidatepair.h"
#include "natfwstunbinding.h"

//  INTERNAL INCLUDES
#include "ciceconnectionhandler.h"
#include "cicesessiondata.h"
#include "cicenatplugincontainer.h"
#include "ticenatplugincontaineriter.h"
#include "icecandidatehandlertestdefs.h"


// CONSTRUCTION
UT_CIceConnectionHandler* UT_CIceConnectionHandler::NewL()
    {
    UT_CIceConnectionHandler* self = UT_CIceConnectionHandler::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CIceConnectionHandler* UT_CIceConnectionHandler::NewLC()
    {
    UT_CIceConnectionHandler* self = new( ELeave ) UT_CIceConnectionHandler();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CIceConnectionHandler::~UT_CIceConnectionHandler()
    {
    delete iConnectionHandler;
    delete iCandStorage;
    delete iMultiplexer;
    delete iIdentification;
    delete iPluginContainer;
    iPluginContainer = NULL;
    iLocalCandidates.ResetAndDestroy();
    iPairs.ResetAndDestroy();
    }

// Default constructor
UT_CIceConnectionHandler::UT_CIceConnectionHandler()
    {
    }

// Second phase construct
void UT_CIceConnectionHandler::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CIceConnectionHandler::SetupL(  )
    {
    iCheckCompletedCalled = EFalse;
    
    iMultiplexer = CNcmConnectionMultiplexer::NewL( *this ); 
    iSessionId = iMultiplexer->CreateSessionL( KTestIap, 5090, 5500 );
    iStreamId = iMultiplexer->CreateStreamL( iSessionId, 0, KProtocolInetUdp );

    iCandStorage = CIceSessionData::NewL( KTestDomain, KTestIap );
    iCandStorage->SetTaTimerValue( KTaTimerValueForGathering );
    iIdentification = CNATFWCredentials::NewL();
    iIdentification->SetStreamId( iStreamId );
    iIdentification->SetDirection( CNATFWCredentials::EOutbound );
    iIdentification->SetUsernameL( KUsername );
    iIdentification->SetPasswordL( KPassword );
    iCandStorage->AddCredentialsL( *iIdentification );
    
    iPluginContainer = CIceNatPluginContainer::NewL( 
        *iCandStorage, *iMultiplexer );
    iPluginContainer->RegObserverForEventL( *this, 
        MIceNatPluginEventObs::ELocalCandidateFound );
    iPluginContainer->RegObserverForEventL( *this, 
        MIceNatPluginEventObs::EFetchingCompleted );
    
    iConnectionHandler = CIceConnectionHandler::NewL(
        *iMultiplexer, *this, *iCandStorage, *iPluginContainer,  *this );
    
    CreateCandidatePairsL();
    }

void UT_CIceConnectionHandler::Teardown(  )
    {
    delete iConnectionHandler;
    iConnectionHandler = NULL;
    delete iCandStorage;
    iCandStorage = NULL;
    delete iIdentification;
    iIdentification = NULL;
    delete iPluginContainer;
    iPluginContainer = NULL;
    delete iMultiplexer;
    iMultiplexer = NULL;
    iLocalCandidates.ResetAndDestroy();
    iPairs.ResetAndDestroy();
    }

// from MNATFWConnectionMultiplexerObserver
void UT_CIceConnectionHandler::Notify( 
        TUint /*aSessionId*/, TUint /*aStreamId*/,
        MNcmConnectionMultiplexerObserver::TNotifyType /*aType*/, TInt /*aError*/ )
    {
    CActiveScheduler::Stop();
    }

void UT_CIceConnectionHandler::IcmpError( 
        TUint /*aSessionId*/, TUint /*aStreamId*/, 
        const TInetAddr& /*aAddress*/ )
    {
    
    }

void UT_CIceConnectionHandler::IncomingMessageL( TUint /*aStreamId*/, 
    const TDesC8& /*aMessage*/, const TInetAddr& /*aLocalAddr*/, 
    const TInetAddr& /*aFromAddr*/, const TInetAddr& /*aPeerRemoteAddr*/, 
    TBool& /*Consumed*/ )
    {
    
    }
      
void UT_CIceConnectionHandler::ConnCheckCompletedL( TInt /*aCompletionCode*/,
    const TInetAddr& /*FromAddr*/, const TInetAddr& /*aDestAddr*/,
    const TInetAddr& /*aMappedAddr*/ )
    {
    iCheckCompletedCalled = ETrue;
    }

void UT_CIceConnectionHandler::StreamClosed( TUint /*aStreamId*/ )
    {
    
    }
    
void UT_CIceConnectionHandler::PluginEventOccured( 
        const CNATFWPluginApi* /*aPlugin*/, 
        TUint aStreamId, TNatPluginEvent aEventCode, 
        TInt aErrorCode, TAny* aEventData, TUint /*aStreamConnectionId*/,
        TUint /*aComponentId*/, TBool /*aIPv6After*/ )
    {
    switch ( aEventCode )
        {
        case MIceNatPluginEventObs::ELocalCandidateFound:
            {
            CNATFWCandidate* candidate =
                reinterpret_cast<CNATFWCandidate*>( aEventData );
            CleanupStack::PushL( candidate );
            iLocalCandidates.AppendL( candidate );
            CleanupStack::Pop( candidate );
            }
            break;
        
        case MIceNatPluginEventObs::EFetchingCompleted:
        case MIceNatPluginEventObs::ESendingActivated:
        case MIceNatPluginEventObs::ESendingDeactivated:
        case MIceNatPluginEventObs::ECandidatePairFound:
        case MIceNatPluginEventObs::EConnChecksCompleted:
            {
            if ( iEventToWait == aEventCode )
                {
                iEventErrCode = aErrorCode;
                iEventStreamId = aStreamId;
                
                CActiveScheduler::Stop();
                }
            }
            break;
        
        case MIceNatPluginEventObs::EGeneralError:
            EUNIT_ASSERT( EFalse );
        
        default:
            EUNIT_ASSERT( EFalse );
        }
    }

void UT_CIceConnectionHandler::CreateCandidatePairsL()
    {
    TIceNatPluginContainerIter iterator 
        = TIceNatPluginContainerIter( *iPluginContainer );
    while ( !iterator.IsDone() )
        {
        CNATFWPluginApi* item = iterator++;
        item->FetchCandidateL( iStreamId, 0, KAfInet );
        WaitForEvent( MIceNatPluginEventObs::EFetchingCompleted );
        }
    
    CNATFWCandidate* remoteCandidate = CNATFWCandidate::NewLC();
    TInetAddr remoteAddr( INET_ADDR( 192, 168, 0, 1 ), 5000 );
    remoteCandidate->SetTransportAddrL( remoteAddr );
    
    for ( TInt i = 0; i < iLocalCandidates.Count(); ++i )
        {
        CNATFWCandidatePair* pair = CNATFWCandidatePair::NewLC( 
            *iLocalCandidates[i], *remoteCandidate );
        iPairs.AppendL( pair );
        CleanupStack::Pop( pair );
        }
    
    CleanupStack::PopAndDestroy( remoteCandidate );
    }

void UT_CIceConnectionHandler::UT_CNATFWConnectionHandler_SetSendingStatusLL()
    {
    TInetAddr addr( KRemoteAddr, KRemotePort );
    addr.SetFamily( KAfInet );
    
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        iConnectionHandler->SetSendingStateL( 
            *iLocalCandidates[0], EStreamingStateActive, addr );
        }
    else
        {
        TInt count = iPluginContainer->Count();
        for ( TInt i( 0 ); i < count; i++ )
            {
            iConnectionHandler->SetSendingStateL( *iLocalCandidates[i],
                EStreamingStateActive, addr );            
            WaitForEvent( MIceNatPluginEventObs::ESendingActivated );
            EUNIT_ASSERT( iEventStreamId == iStreamId &&
                          KErrNone == iEventErrCode );

            iConnectionHandler->SetSendingStateL( *iLocalCandidates[i],
                EStreamingStatePassive, addr );
            WaitForEvent( MIceNatPluginEventObs::ESendingDeactivated );
            EUNIT_ASSERT( iEventStreamId == iStreamId &&
                          KErrNone == iEventErrCode );
            }
        }
    }

void UT_CIceConnectionHandler::UT_CNATFWConnectionHandler_CreateConnectionLL(  )
    {
    TUint conId(0);
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        conId = iConnectionHandler->CreateConnectionL( *iPairs[0] );
        EUNIT_ASSERT( 1 == iConnectionHandler->iBindings.Count() );
        
        conId = iConnectionHandler->CreateConnectionL( *iPairs[1] );
        EUNIT_ASSERT( 2 == iConnectionHandler->iBindings.Count() );
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE( conId = iConnectionHandler->CreateConnectionL( 
            *iPairs[0] ) );
        EUNIT_ASSERT( 1 == iConnectionHandler->iBindings.Count() );
        
        EUNIT_ASSERT_NO_LEAVE( conId = iConnectionHandler->CreateConnectionL( 
            *iPairs[1] ) );
        EUNIT_ASSERT( 2 == iConnectionHandler->iBindings.Count() );
        }
    }

void UT_CIceConnectionHandler::UT_CNATFWConnectionHandler_RemoveConnectionL(  )
    {    
    TUint conId(0);
    conId = iConnectionHandler->CreateConnectionL( *iPairs[0] );
    
    iConnectionHandler->RemoveConnection( 10, conId );
    iConnectionHandler->RemoveConnection( iStreamId, 10 );
    iConnectionHandler->RemoveConnection( iStreamId, conId );
    }

void UT_CIceConnectionHandler::UT_CNATFWConnectionHandler_PerformConnCheckLL(  )
    {
    TInetAddr peerAddr( iPairs[0]->RemoteCandidate().TransportAddr() );
    TUint connection = iConnectionHandler->CreateConnectionL( *iPairs[0] );
    
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        iConnectionHandler->PerformConnCheckL( 
            iStreamId, connection, 0, EFalse, 0, peerAddr, *this );
        }
    else
        {
        // SUCCESFULL RESPONSE
        EUNIT_ASSERT_NO_LEAVE( iConnectionHandler->PerformConnCheckL( 
            iStreamId, connection, 0, EFalse, 0, peerAddr, *this ) );
        TInetAddr localAddr;
        TInetAddr fromAddr = peerAddr;
        _LIT8( Kdummycontent, "dummycontent" );
        TBool consumed( EFalse );
        EUNIT_ASSERT_NO_LEAVE( iConnectionHandler->IncomingMessageL( 
            iStreamId, Kdummycontent, localAddr, fromAddr, peerAddr, consumed ) );
        EUNIT_ASSERT_NO_LEAVE( iConnectionHandler->STUNBindingEventOccurredL( 
            MSTUNClientObserver::EPublicAddressResolved, 
            *( iConnectionHandler->iBindings[0].iBinding ) ) );
        
        // CREDENTIALS REJECTED
        EUNIT_ASSERT_NO_LEAVE( iConnectionHandler->PerformConnCheckL( 
            iStreamId, connection, 0, EFalse, 0, peerAddr, *this ) );
        EUNIT_ASSERT_NO_LEAVE( iConnectionHandler->IncomingMessageL( 
            iStreamId, Kdummycontent, localAddr, fromAddr, peerAddr, consumed ) );
        
        EUNIT_ASSERT_NO_LEAVE( iConnectionHandler->STUNBindingEventOccurredL( 
            MSTUNClientObserver::ECredentialsRejected, 
            *( iConnectionHandler->iBindings[0].iBinding ) ) );
        }
    
    iConnectionHandler->RemoveConnection( iStreamId, connection );
    }


void UT_CIceConnectionHandler::UT_CNATFWConnectionHandler_CancelCheckL()
    {
    TInetAddr peerAddr( iPairs[0]->RemoteCandidate().TransportAddr() );
    TUint connection = iConnectionHandler->CreateConnectionL( *iPairs[0] );
    
    // cancel before mux connection activation
    iConnectionHandler->PerformConnCheckL( 
        iStreamId, connection, 0, EFalse, 0, peerAddr, *this );
    iConnectionHandler->CancelCheck( iStreamId, connection, peerAddr );
    
    // cancel after mux connection activation
    iConnectionHandler->PerformConnCheckL( 
        iStreamId, connection, 0, EFalse, 0, peerAddr, *this );
    iConnectionHandler->ConnectionNotify( iStreamId, connection, 
        MNcmConnectionObserver::EReceivingActivated, KErrNone );
    iConnectionHandler->ConnectionNotify( iStreamId, connection, 
        MNcmConnectionObserver::ESendingActivated, KErrNone );
    iConnectionHandler->CancelCheck( iStreamId, connection, peerAddr );
    iConnectionHandler->RemoveConnection( iStreamId, connection );
    
    EUNIT_ASSERT_PANIC( 
        iConnectionHandler->CancelCheck( iStreamId, 5, peerAddr ), "USER", 0 );
    }


void UT_CIceConnectionHandler::UT_CNATFWConnectionHandler_IncomingMessageLL(  )
    {
    TInetAddr localAddr( KLocalAddr );
    TInetAddr fromAddr( KRemoteAddr );
    TInetAddr peerAddr( KRemoteAddr );
    
    _LIT8( KDummyContent, "dummycontent" );
    TBool consumed( EFalse );
    
    // no connection
    EUNIT_ASSERT_LEAVE( iConnectionHandler->IncomingMessageL( 
        iStreamId, KDummyContent, localAddr, fromAddr, peerAddr, consumed ) );
    
    TUint stunConnection = iConnectionHandler->CreateConnectionL( *iPairs[0] );
    
    // unknown sream
    EUNIT_ASSERT_NO_LEAVE( iConnectionHandler->IncomingMessageL( 
        iStreamId + 1, KDummyContent, 
        localAddr, fromAddr, peerAddr, consumed ) );
    EUNIT_ASSERT( !consumed );
    
    // known stream
    TUint stunConnection2 = iConnectionHandler->CreateConnectionL( *iPairs[0] );
    EUNIT_ASSERT_NO_LEAVE( iConnectionHandler->IncomingMessageL( 
        iStreamId, KDummyContent, localAddr, fromAddr, peerAddr, consumed ) );
    
    iConnectionHandler->RemoveConnection( iStreamId, stunConnection );
    iConnectionHandler->RemoveConnection( iStreamId, stunConnection2 );
    }

void UT_CIceConnectionHandler::UT_CNATFWConnectionHandler_STUNClientInitCompletedL()
    {
    EUNIT_ASSERT_PANIC(
        iConnectionHandler->STUNClientInitCompleted( 
            *iConnectionHandler->iStunClient,
            KErrNone ),
        "USER", 0 );
    }

void UT_CIceConnectionHandler::UT_CNATFWConnectionHandler_ConnectionNotifyL()
    {
    TInetAddr peerAddr( iPairs[0]->RemoteCandidate().TransportAddr() );
    TUint stunConnection = iConnectionHandler->CreateConnectionL( *iPairs[0] );
    
    // unknown connection
    const TUint KUnknownStream( 10 );
    const TUint KUnknownConnection( 10 );
    iConnectionHandler->ConnectionNotify( KUnknownStream, KUnknownConnection,
        MNcmConnectionObserver::EConnectionRemoved, KErrNone );
    EUNIT_ASSERT( 0 != iConnectionHandler->iBindings.Count() );
    
    // known connection
    // MNcmConnectionObserver::EReceivingActivated
    iConnectionHandler->ConnectionNotify( iStreamId, stunConnection,
        MNcmConnectionObserver::EReceivingActivated, KErrNone );
    EUNIT_ASSERT( EStreamingStatePassive 
        == iConnectionHandler->iBindings[0].iStreamingStatus );
    
    // MNcmConnectionObserver::EReceivingDeactivated
    iConnectionHandler->ConnectionNotify( iStreamId, stunConnection,
        MNcmConnectionObserver::EReceivingDeactivated, KErrNone );
    EUNIT_ASSERT( EStreamingStatePassive 
        == iConnectionHandler->iBindings[0].iStreamingStatus );
    
    // MNcmConnectionObserver::ESendingActivated
    iConnectionHandler->ConnectionNotify( iStreamId, stunConnection,
        MNcmConnectionObserver::ESendingActivated, KErrNone );
    EUNIT_ASSERT( EStreamingStateActive 
        == iConnectionHandler->iBindings[0].iStreamingStatus );
    
    // MNcmConnectionObserver::EConnectionRemoved
    iConnectionHandler->ConnectionNotify( iStreamId, stunConnection,
        MNcmConnectionObserver::EConnectionRemoved, KErrNone );
    EUNIT_ASSERT( 0 == iConnectionHandler->iBindings.Count() );
    
    // MNcmConnectionObserver::EConnectionError
    stunConnection = iConnectionHandler->CreateConnectionL( *iPairs[0] );
    iConnectionHandler->ConnectionNotify( iStreamId, stunConnection,
        MNcmConnectionObserver::EConnectionError, KErrUnknown );
    EUNIT_ASSERT( 0 == iConnectionHandler->iBindings.Count() );
    
    // events for relay connections must be discarded
    for ( TInt i = 0; i < iPairs.Count(); ++i )
        {
        if ( CNATFWCandidate::ERelay == iPairs[i]->LocalCandidate().Type() )
            {
            TUint relayConnection 
                = iConnectionHandler->CreateConnectionL( *iPairs[i] );
            iConnectionHandler->ConnectionNotify( iStreamId, relayConnection,
                MNcmConnectionObserver::EConnectionRemoved, KErrNone );
            EUNIT_ASSERT( 0 != iConnectionHandler->iBindings.Count() );
            }
        }
    
    // unknown event
    stunConnection = iConnectionHandler->CreateConnectionL( *iPairs[0] );
    EUNIT_ASSERT_PANIC(
        iConnectionHandler->ConnectionNotify( iStreamId, stunConnection,
            static_cast<MNcmConnectionObserver::TConnectionNotifyType>( -1 ), 
            KErrNone ), "USER", 0 );
    }


void UT_CIceConnectionHandler::UT_CNATFWConnectionHandler_PluginEventOccuredL()
    {
    const CNATFWPluginApi* plugin 
        = reinterpret_cast<const CNATFWPluginApi*>( this );
    
    TUint streamConnectionId( 0 );
    TUint componentId( 0 );
    TBool ipv6After( EFalse );
    
    // test handling of unknown event
    EUNIT_ASSERT_PANIC(
        iConnectionHandler->PluginEventOccured( plugin, iStreamId,
            MIceNatPluginEventObs::EGeneralError, KErrNone, NULL, streamConnectionId,
            componentId, ipv6After ),
            "USER", 0 );
    }


void UT_CIceConnectionHandler::UT_CNATFWConnectionHandler_STUNBindingEventOccurredLL()
    {
    // not ready case
    EUNIT_ASSERT_LEAVE( iConnectionHandler->STUNBindingEventOccurredL( 
        MSTUNClientObserver::EPublicAddressResolved, 
        reinterpret_cast<const CBinding&>( *this ) ) );
    EUNIT_ASSERT( !iCheckCompletedCalled );
    
    // ready cases
    TInetAddr peerAddr( KRemoteAddr, KRemotePort );
    peerAddr.SetFamily( KAfInet );
    const_cast<CNATFWCandidate&>
        ( iPairs[0]->RemoteCandidate() ).SetTransportAddrL( peerAddr );
    
    TUint stunConnection = iConnectionHandler->CreateConnectionL( *iPairs[0] );
    
    iConnectionHandler->PerformConnCheckL( 
        iStreamId, stunConnection, 0, 0, 0, peerAddr, *this );
    
    EUNIT_ASSERT_NO_LEAVE( iConnectionHandler->STUNBindingEventOccurredL( 
        MSTUNClientObserver::EPublicAddressResolved, 
        *( iConnectionHandler->iBindings[0].iBinding ) ) );
    EUNIT_ASSERT( iCheckCompletedCalled );
    
    EUNIT_ASSERT_NO_LEAVE( iConnectionHandler->STUNBindingEventOccurredL( 
        MSTUNClientObserver::ECredentialsRejected, 
        *( iConnectionHandler->iBindings[0].iBinding ) ) );
    EUNIT_ASSERT( iCheckCompletedCalled );
    
    // UNKNOWN EVENT
    EUNIT_ASSERT_PANIC( 
        iConnectionHandler->STUNBindingEventOccurredL( 
            MSTUNClientObserver::EActiveDestinationSet, 
            *( iConnectionHandler->iBindings[0].iBinding ) ), "USER", 0 );
    }

void UT_CIceConnectionHandler::UT_CNATFWConnectionHandler_STUNBindingErrorOccurredL()
    {
    // ready cases
    TInetAddr peerAddr( iPairs[0]->RemoteCandidate().TransportAddr() );
    TUint stunConnection = iConnectionHandler->CreateConnectionL( *iPairs[0] );
    
    iConnectionHandler->PerformConnCheckL( 
        iStreamId, stunConnection, 0, 0, 0, peerAddr, *this );
    
    iConnectionHandler->STUNBindingErrorOccurred( 
        *( iConnectionHandler->iBindings[0].iBinding ), KErrNotReady );
    EUNIT_ASSERT( iCheckCompletedCalled );
    iConnectionHandler->RemoveConnection( iStreamId, stunConnection );
    
    // not ready case
    EUNIT_ASSERT_PANIC(
        iConnectionHandler->STUNBindingErrorOccurred( 
            reinterpret_cast<const CBinding&>( *this ), KErrNotReady ), 
            "USER", 0 );
    EUNIT_ASSERT( !iCheckCompletedCalled );
    }

void UT_CIceConnectionHandler::WaitForEvent( 
        MIceNatPluginEventObs::TNatPluginEvent aEvent )
    {
    iEventToWait = aEvent;
    CActiveScheduler::Start();
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CIceConnectionHandler,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "SetSendingStateL - test ",
    "CIceConnectionHandler",
    "SetSendingStateL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWConnectionHandler_SetSendingStatusLL, Teardown)

EUNIT_TEST(
    "CreateConnectionL - test ",
    "CIceConnectionHandler",
    "CreateConnectionL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWConnectionHandler_CreateConnectionLL, Teardown)

EUNIT_TEST(
    "RemoveConnection - test ",
    "CIceConnectionHandler",
    "RemoveConnection",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWConnectionHandler_RemoveConnectionL, Teardown)

EUNIT_TEST(
    "PerformConnCheckL - test ",
    "CIceConnectionHandler",
    "PerformConnCheckL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWConnectionHandler_PerformConnCheckLL, Teardown)

EUNIT_TEST(
    "CancelCheck - test ",
    "CIceConnectionHandler",
    "CancelCheck",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWConnectionHandler_CancelCheckL, Teardown)

EUNIT_TEST(
    "IncomingMessageL - test ",
    "CIceConnectionHandler",
    "IncomingMessageL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWConnectionHandler_IncomingMessageLL, Teardown)

EUNIT_TEST(
    "STUNClientInitCompletedL - test ",
    "CIceConnectionHandler",
    "STUNClientInitCompletedL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWConnectionHandler_STUNClientInitCompletedL, Teardown)

EUNIT_TEST(
    "ConnectionNotify - test ",
    "CIceConnectionHandler",
    "ConnectionNotify",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWConnectionHandler_ConnectionNotifyL, Teardown)

EUNIT_TEST(
    "PluginEventOccured - test ",
    "CIceConnectionHandler",
    "PluginEventOccured",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWConnectionHandler_PluginEventOccuredL, Teardown)

EUNIT_TEST(
    "STUNBindingEventOccuredL - test ",
    "CIceConnectionHandler",
    "STUNBindingEventOccuredL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWConnectionHandler_STUNBindingEventOccurredLL, Teardown)

EUNIT_TEST(
    "STUNBindingErrorOccured - test ",
    "CIceConnectionHandler",
    "STUNBindingErrorOccured",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWConnectionHandler_STUNBindingErrorOccurredL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
