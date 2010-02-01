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
#include "UT_CNATFWClient.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <digia/eunit/EUnitDecorators.h>
#include <e32debug.h>
#include "natfwcandidate.h"
#include "natfwcandidatepair.h"
#include <badesca.h>

//  INTERNAL INCLUDES
#include "natfwclient.h"
#include "natfwconnectivityapidefs.h"
#include "defaultsettings.h"
#include "natfwcredentials.h"
#include "natfwsession.h"
#include "natfwstream.h"
#include "natfwunittestmacros.h"

#ifndef __WINSCW__
const TInt KIapId = 1;
#else
const TInt KIapId = 10;
#endif

_LIT8( KDomain, "isp1v.wipsl.com" );
_LIT8( KTestPlugin, "nokia.test" );

const TInt KQoS = 6;
const TUint KCollectionId = 1;
const TUint KMediaComponentId = 1;
const TUint32 KDestAddr = INET_ADDR( 10,0,0,10 );
const TUint KPort = 50000;
const TUint KStartStopTimerVal = 500;
const TUint KErrorTestStreamId = 99;
const TUint32 KIcmpErrorTestAddress = INET_ADDR( 255, 255, 255, 255 );


// CONSTRUCTION
UT_CNATFWClient* UT_CNATFWClient::NewL()
    {
    UT_CNATFWClient* self = UT_CNATFWClient::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWClient* UT_CNATFWClient::NewLC()
    {
    UT_CNATFWClient* self = new( ELeave ) UT_CNATFWClient();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWClient::~UT_CNATFWClient()
    {
    }

// Default constructor
UT_CNATFWClient::UT_CNATFWClient()
    {
    }

// Second phase construct
void UT_CNATFWClient::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }


// From MNATFWConnectivityObserver

void UT_CNATFWClient::EventOccured( TUint aSessionId, TUint aStreamId,
    TNATFWConnectivityEvent aEvent, TInt aError, TAny* /*aEventData*/ )
    {
    _LIT( KNotifyPrint, 
        "UT_CNATFWClient::Notify, SessionId: %d, StreamId: %d, Event: %d, Err: %d" );
    RDebug::Print( KNotifyPrint, aSessionId, aStreamId, aEvent, aError );
    iLastNotifyEvent = aEvent;
    iLastNotifyError = aError;
    if ( iIsRunning && ELocalCandidateFound != aEvent &&
        ECandidatePairFound != aEvent )
        {
        CActiveScheduler::Stop();    
        iIsRunning = EFalse;
        }
    }

//  METHODS

void UT_CNATFWClient::SetupL(  )
    {
    iIsRunning = EFalse;
    iSettings = CDefaultSettings::NewL( KDomain, KIapId );
    iSettings->EraseL( 0x0, 0x0 );
    iSettings->CreateSettingsL();
    
    iClient = CNATFWClient::NewL();
    iClient->RegisterObserverForEventsL( *this,
        MNATFWConnectivityObserver::EAllEvents );
    }
    

void UT_CNATFWClient::SetupSessionL()
    {
    SetupL();
    EUNIT_ASSERT_NO_LEAVE( iSessionId = iClient->CreateSessionL( 
        KIapId, KDomain ) );
    iIsRunning = ETrue;
    CActiveScheduler::Start();
    }


void UT_CNATFWClient::SetupStreamL()
    {
    SetupSessionL();
    EUNIT_ASSERT_NO_LEAVE( iStreamId = iClient->CreateStreamL( iSessionId, KProtocolInetUdp,
        KQoS ) );
    
    CDesC8Array* array = new(ELeave) CDesC8ArrayFlat( 1 ); 
    CleanupStack::PushL( array );
    
    TBuf8<10> stun( KTestPlugin );
  
    HBufC8* pluginDesc = stun.AllocL();
    CleanupStack::PushL( pluginDesc );
    array->AppendL( *pluginDesc );
    
    TInt pluginId( 0 );
    iClient->LoadPluginL( iSessionId, *array, pluginId );
        
    CleanupStack::PopAndDestroy( pluginDesc );
    CleanupStack::PopAndDestroy( array );
    }


void UT_CNATFWClient::Teardown(  )
    {
    if ( iStreamId )
        {
        iClient->CloseStreamL( iSessionId, iStreamId );
        iStreamId = 0;
        }
    if ( iSessionId )
        {
        iClient->CloseSessionL( iSessionId );
        iSessionId = 0;
        }
        
    iClient->UnregisterObserverForEvents( *this,
        MNATFWConnectivityObserver::EAllEvents);
    
    delete iClient;
    delete iSettings;
    }


void UT_CNATFWClient::UT_CNATFWClient_NewLL(  )
    {
    CNATFWClient* client1;
    CNATFWClient* client2;
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( client1 = CNATFWClient::NewL() );
    CleanupStack::PushL( client1 );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( client2 = CNATFWClient::NewL() );
    
    CleanupStack::PopAndDestroy( client1 );
    delete client2;
    }


void UT_CNATFWClient::UT_CNATFWClient_NewLCL(  )
    {
    CNATFWClient* client1;
    CNATFWClient* client2;
    
    client1 = CNATFWClient::NewLC();
    client2 = CNATFWClient::NewLC();
    
    CleanupStack::PopAndDestroy( client2 );
    CleanupStack::PopAndDestroy( client1 );
    }


void UT_CNATFWClient::UT_CNATFWClient_CreateSessionLL(  )
    {
    TUint sessionId;
    NATFW_EUNIT_ASSERT_NO_LEAVE(
        sessionId = iClient->CreateSessionL( KIapId, KDomain ) );
    iIsRunning = ETrue;
    CActiveScheduler::Start();
    
    TBuf8<10> stun( KTestPlugin );
    
    CDesC8Array* array = new(ELeave) CDesC8ArrayFlat( 1 ); 
    CleanupStack::PushL( array );
    
    HBufC8* pluginDesc = stun.AllocL();
    CleanupStack::PushL( pluginDesc );
    array->AppendL( *pluginDesc );
    
    TInt pluginId( 0 );
    NATFW_EUNIT_ASSERT_NO_LEAVE(
        iClient->LoadPluginL( sessionId, *array, pluginId ) );
    
    CleanupStack::PopAndDestroy( pluginDesc );
    CleanupStack::PopAndDestroy( array );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iClient->CloseSessionL( sessionId ) );
    }


void UT_CNATFWClient::UT_CNATFWClient_CreateStreamLL(  )
    {
    TUint streamId1;
    TUint streamId2;

    NATFW_EUNIT_ASSERT_NO_LEAVE( streamId1 = iClient->CreateStreamL( iSessionId,
        KProtocolInetUdp, KQoS ) );
        
    NATFW_EUNIT_ASSERT_NO_LEAVE( streamId2 = iClient->CreateStreamL( iSessionId,
        KProtocolInetUdp, KQoS ) );  

    NATFW_EUNIT_ASSERT_NO_LEAVE( iClient->CloseStreamL( iSessionId, streamId1 ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iClient->CloseStreamL( iSessionId, streamId2 ) );
    }


void UT_CNATFWClient::UT_CNATFWClient_CreateWrapperLL(  )
    {
    TUint KInvalidStreamId = 9999;
    
    MNATFWSocketMediaConnWrapper* wrapperIf;
    NATFW_EUNIT_ASSERT_NO_LEAVE( wrapperIf = &iClient->CreateWrapperL( iSessionId,
        iStreamId ) );
    
    NATFW_EUNIT_ASSERT_SPECIFIC_LEAVE( wrapperIf = &iClient->CreateWrapperL( iSessionId,
            KInvalidStreamId ), KErrNotFound );
    }


void UT_CNATFWClient::UT_CNATFWClient_FetchCandidateLL(  )
    {
    EUNIT_ASSERT_LEAVE( iClient->FetchCandidateL( 0,
            iStreamId, KAfInet ) );
    
    iClient->SessionByIdL( iSessionId )->iServerConnectionState =
        CNATFWSession::EConnectionConnected;
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iClient->FetchCandidateL( iSessionId,
        iStreamId, KAfInet ) );
    
    iIsRunning = ETrue;
    CActiveScheduler::Start();
    
    // put requests in queue
    CNATFWPluginApi* plugin = NULL;

    iClient->SessionByIdL( iSessionId )->iServerConnectionState =
        CNATFWSession::EConnectionInProgress;
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iClient->FetchCandidateL( iSessionId,
            iStreamId, KAfInet ) );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iClient->FetchCandidateL( iSessionId,
            iStreamId, KAfInet ) );
    
    iClient->SessionByIdL( iSessionId )->Notify( *plugin, 0,
        MNATFWPluginObserver::EServerConnected, KErrNone );
    
    iIsRunning = ETrue;
    CActiveScheduler::Start();
    
    iIsRunning = ETrue;
    CActiveScheduler::Start();
    
    // put request in queue which will cause error
    iClient->SessionByIdL( iSessionId )->iServerConnectionState =
        CNATFWSession::EConnectionInProgress;

    NATFW_EUNIT_ASSERT_NO_LEAVE( iClient->FetchCandidateL( iSessionId,
        0, KAfInet ) );

    iClient->SessionByIdL( iSessionId )->Notify( *plugin, 0,
        MNATFWPluginObserver::EServerConnected, KErrNone );
    }


void UT_CNATFWClient::UT_CNATFWClient_FetchCandidatesLL(  )
    {
    const TInt KLeaveId = 99;
    
    iClient->SessionByIdL( iSessionId )->iServerConnectionState =
        CNATFWSession::EConnectionConnected;
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iClient->FetchCandidatesL( iSessionId,
        iStreamId, KCollectionId, KMediaComponentId, KAFUnspec ) );
    
    iIsRunning = ETrue;
    CActiveScheduler::Start();
    
    // put request in queue
    CNATFWPluginApi* plugin = NULL;

    iClient->SessionByIdL( iSessionId )->iServerConnectionState =
        CNATFWSession::EConnectionInProgress;
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iClient->FetchCandidatesL( iSessionId,
            iStreamId, KCollectionId, KMediaComponentId, KAFUnspec ) );
    
    iClient->SessionByIdL( iSessionId )->Notify( *plugin, 0,
            MNATFWPluginObserver::EServerConnected, KErrNone );
    
    iIsRunning = ETrue;
    CActiveScheduler::Start();
    
    // put request in queue which will cause error
    iClient->SessionByIdL( iSessionId )->iServerConnectionState =
        CNATFWSession::EConnectionInProgress;

    NATFW_EUNIT_ASSERT_NO_LEAVE( iClient->FetchCandidatesL( iSessionId,
        iStreamId, KLeaveId, KMediaComponentId, KAFUnspec ) );

    iClient->SessionByIdL( iSessionId )->Notify( *plugin, 0,
        MNATFWPluginObserver::EServerConnected, KErrNone );

    iIsRunning = ETrue;
    CActiveScheduler::Start();
    }


void UT_CNATFWClient::UT_CNATFWClient_SetReceivingStateLL(  )
    {
    CNATFWCandidate* candidate = CNATFWCandidate::NewLC();
    candidate->SetSessionId( iSessionId );
    candidate->SetStreamId( iStreamId );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iClient->SetReceivingStateL( 
        *candidate, EStreamingStateActive ) );
    
    iIsRunning = ETrue;
    CActiveScheduler::Start();
    
    EUNIT_ASSERT( EReceivingActivated == iLastNotifyEvent && KErrNone == iLastNotifyError );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iClient->SetReceivingStateL( 
        *candidate, EStreamingStatePassive ) );
    
    iIsRunning = ETrue;
    CActiveScheduler::Start();
    
    EUNIT_ASSERT( EReceivingDeactivated == iLastNotifyEvent && KErrNone == iLastNotifyError );
    CleanupStack::PopAndDestroy( candidate );
    }


void UT_CNATFWClient::UT_CNATFWClient_SetReceivingStateL_ActiveL( )
    {
    CNATFWCandidate* candidate = CNATFWCandidate::NewLC();
    candidate->SetSessionId( iSessionId );
    candidate->SetStreamId( iStreamId );

    EUNIT_TRAP_EXCEPT_ALLOC_D( leavecode, iClient->SetReceivingStateL( 
        *candidate, EStreamingStateActive ); );
    EUNIT_ASSERT_EQUALS( leavecode, KErrNone );

    CleanupStack::PopAndDestroy( candidate );
    }


void UT_CNATFWClient::UT_CNATFWClient_SetReceivingStateL_PassiveL( )
    {
    CNATFWCandidate* candidate = CNATFWCandidate::NewLC();
    candidate->SetSessionId( iSessionId );
    candidate->SetStreamId( iStreamId );

    EUNIT_TRAP_EXCEPT_ALLOC_D( leavecode, iClient->SetReceivingStateL( 
        *candidate, EStreamingStatePassive ); );
    EUNIT_ASSERT_EQUALS( leavecode, KErrNone );

    CleanupStack::PopAndDestroy( candidate );
    }


void UT_CNATFWClient::UT_CNATFWClient_SetSendingStateLL(  )
    {
    _LIT8( KAddress,"192.168.0.10" );
    CNATFWCandidate* candidate = CNATFWCandidate::NewLC();
    candidate->SetSessionId( iSessionId );
    candidate->SetStreamId( iStreamId );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iClient->SetSendingStateL( 
        *candidate, EStreamingStateActive, KDestAddr ) );
        
    iIsRunning = ETrue;
    CActiveScheduler::Start();

    NATFW_EUNIT_ASSERT_NO_LEAVE( iClient->SetSendingStateL( 
        *candidate, EStreamingStatePassive, KDestAddr ) );

    iIsRunning = ETrue;
    CActiveScheduler::Start();

    // resolve ip address by domain
    NATFW_EUNIT_ASSERT_NO_LEAVE( iClient->SetSendingStateL( 
            *candidate, EStreamingStateActive, 
            KDomain, KPort ) );
    
    iIsRunning = ETrue;
    CActiveScheduler::Start();
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iClient->SetSendingStateL( 
            *candidate, EStreamingStatePassive, 
            KAddress, KPort ) );
    
    iIsRunning = ETrue;
    CActiveScheduler::Start();
    
    // Causes IcmpError notify call
    NATFW_EUNIT_ASSERT_NO_LEAVE( iClient->SetSendingStateL( 
        *candidate, EStreamingStatePassive, 
        TInetAddr( KIcmpErrorTestAddress, KPort ) ) );

    iIsRunning = ETrue;
    CActiveScheduler::Start();
    
    CleanupStack::PopAndDestroy( candidate );
    }


void UT_CNATFWClient::UT_CNATFWClient_SetSendingStateL_ActiveL(  )
    {
    //_LIT8( KAddress,"192.168.0.10" );
    CNATFWCandidate* candidate = CNATFWCandidate::NewLC();
    candidate->SetSessionId( iSessionId );
    candidate->SetStreamId( iStreamId );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iClient->SetSendingStateL( 
        *candidate, EStreamingStateActive,  KDomain, KPort ) );

    CleanupStack::PopAndDestroy( candidate );
    }


void UT_CNATFWClient::UT_CNATFWClient_SetSendingStateL_PassiveL(  )
    {
    _LIT8( KAddress,"192.168.0.10" );
    CNATFWCandidate* candidate = CNATFWCandidate::NewLC();
    candidate->SetSessionId( iSessionId );
    candidate->SetStreamId( iStreamId );

    NATFW_EUNIT_ASSERT_NO_LEAVE( iClient->SetSendingStateL( 
        *candidate, EStreamingStatePassive, KDomain, KPort ) );

    CleanupStack::PopAndDestroy( candidate );
    }


void UT_CNATFWClient::UT_CNATFWClient_SetOperationModeLL(  )
    {
    NATFW_EUNIT_ASSERT_NO_LEAVE( iClient->SetRoleL( iSessionId,
        EIceRoleControlling ) );
    }


void UT_CNATFWClient::UT_CNATFWClient_SetCredentialsLL(  )
    {
    CNATFWCredentials* natId = CNATFWCredentials::NewLC();
    CNATFWCandidate* candidate = CNATFWCandidate::NewLC();
    
    NATFW_EUNIT_ASSERT_SPECIFIC_LEAVE( iClient->SetCredentialsL( *candidate,
        *natId ), KErrNotFound );
    
    candidate->SetSessionId( iSessionId );
    candidate->SetStreamId( iStreamId );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iClient->SetCredentialsL(
        *candidate, *natId ) );
    
    // Try resolving from FQDN
    TInetAddr addr;
    addr.SetFamily( KAFUnspec );
    candidate->SetTransportAddrL( addr );
    candidate->SetTransportDomainAddrL( KDomain, KPort );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iClient->SetCredentialsL(
        *candidate, *natId ) );

    CleanupStack::PopAndDestroy( candidate );
    CleanupStack::PopAndDestroy( natId ); 
    }


void UT_CNATFWClient::UT_CNATFWClient_PerformConnectivityChecksLL(  )
    {
    RPointerArray<CNATFWCandidate> candiArray;
    CleanupClosePushL( candiArray );
    
    CNATFWCandidate* candi = CNATFWCandidate::NewLC(); 
    candiArray.AppendL( candi );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iClient->PerformConnectivityChecksL( iSessionId,
        candiArray ) );
    
    iIsRunning = ETrue;
    CActiveScheduler::Start();
    
    // Try resolving from FQDN
    TInetAddr addr;
    addr.SetFamily( KAFUnspec );
    candi->SetTransportAddrL( addr );
    candi->SetTransportDomainAddrL( KDomain, KPort );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iClient->PerformConnectivityChecksL( iSessionId,
        candiArray ) );
    
    iIsRunning = ETrue;
    CActiveScheduler::Start();
    
    CleanupStack::PopAndDestroy( candi );
    CleanupStack::PopAndDestroy( &candiArray );
    }


void UT_CNATFWClient::UT_CNATFWClient_UpdateIceProcessingLL()
    {
    RPointerArray<CNATFWCandidatePair> candPairs;
    
    CleanupClosePushL( candPairs );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iClient->UpdateIceProcessingL(
        iSessionId, candPairs ) );
    
    candPairs.ResetAndDestroy();
    CleanupStack::Pop( &candPairs );
    }


void UT_CNATFWClient::UT_CNATFWClient_UpdateIceProcessingL2L()
    {
    RPointerArray<CNATFWCandidate> remoteCands;
    TInetAddr addr( KDestAddr, KPort );
    
    CleanupClosePushL( remoteCands );
    
    CNATFWCandidate* candi = CNATFWCandidate::NewL();
    CleanupStack::PushL( candi );
    
    candi->SetTransportAddrL( addr );

    remoteCands.AppendL( candi );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iClient->UpdateIceProcessingL(
        iSessionId, remoteCands ) );
    
    addr.SetFamily( KAFUnspec );
    remoteCands[0]->SetTransportAddrL( addr );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iClient->UpdateIceProcessingL(
        iSessionId, remoteCands ) );
    
    remoteCands.ResetAndDestroy();
    
    CleanupStack::Pop( candi );
    CleanupStack::Pop( &remoteCands );
    }


void UT_CNATFWClient::UT_CNATFWClient_FindStreamByIdL()
    {
    TUint KInvalidStreamId = 99999;
    
    CNATFWStream* stream = NULL;
    
    stream = iClient->FindStreamById( iStreamId );
    EUNIT_ASSERT( stream != NULL );
    EUNIT_ASSERT_EQUALS( stream->StreamId(), iStreamId );
    
    stream = NULL;
    stream = iClient->FindStreamById( KInvalidStreamId ); 
    
    EUNIT_ASSERT( stream == NULL );
    }


//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWClient,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CNATFWClient",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWClient_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CNATFWClient",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWClient_NewLCL, Teardown)

EUNIT_TEST(
    "CreateSessionL - test ",
    "CNATFWClient",
    "CreateSessionL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWClient_CreateSessionLL, Teardown )
    
EUNIT_TEST(
    "CreateStreamL - test ",
    "CNATFWClient",
    "CreateStreamL",
    "FUNCTIONALITY",
    SetupSessionL, UT_CNATFWClient_CreateStreamLL, Teardown)

EUNIT_TEST(
    "CreateWrapperL - test ",
    "CNATFWClient",
    "CreateWrapperL",
    "FUNCTIONALITY",
    SetupStreamL, UT_CNATFWClient_CreateWrapperLL, Teardown)

EUNIT_TEST(
    "FetchCandidateL - test ",
    "CNATFWClient",
    "FetchCandidateL",
    "FUNCTIONALITY",
    SetupStreamL, UT_CNATFWClient_FetchCandidateLL, Teardown)

EUNIT_TEST(
    "FetchCandidatesL - test ",
    "CNATFWClient",
    "FetchCandidatesL",
    "FUNCTIONALITY",
    SetupStreamL, UT_CNATFWClient_FetchCandidatesLL, Teardown)

EUNIT_NOT_DECORATED_TEST(
    "SetReceivingStateL - test ",
    "CNATFWClient",
    "SetReceivingStateL",
    "FUNCTIONALITY",
    SetupStreamL, UT_CNATFWClient_SetReceivingStateLL, Teardown)

EUNIT_NOT_DECORATED_TEST(
    "SetSendingStateL - test ",
    "CNATFWClient",
    "SetSendingStateL",
    "FUNCTIONALITY",
    SetupStreamL, UT_CNATFWClient_SetSendingStateLL, Teardown)

EUNIT_TEST(
    "SetReceivingState Active - test ",
    "CNATFWClient",
    "SetReceivingStateL",
    "FUNCTIONALITY",
    SetupStreamL, UT_CNATFWClient_SetReceivingStateL_ActiveL, Teardown )

EUNIT_TEST(
    "SetReceivingState Passive - test ",
    "CNATFWClient",
    "SetReceivingStateL",
    "FUNCTIONALITY",
    SetupStreamL, UT_CNATFWClient_SetReceivingStateL_PassiveL, Teardown )
        
EUNIT_TEST(
    "SetSendingState Active - test ",
    "CNATFWClient",
    "SetSendingStateL",
    "FUNCTIONALITY",
    SetupStreamL, UT_CNATFWClient_SetSendingStateL_ActiveL, Teardown )

EUNIT_TEST(
    "SetSendingState Passive - test ",
    "CNATFWClient",
    "SetSendingStateL",
    "FUNCTIONALITY",
    SetupStreamL, UT_CNATFWClient_SetSendingStateL_PassiveL, Teardown )
            
EUNIT_TEST(
    "SetRoleL - test ",
    "CNATFWClient",
    "SetRoleL",
    "FUNCTIONALITY",
    SetupStreamL, UT_CNATFWClient_SetOperationModeLL, Teardown)

EUNIT_TEST(
    "SetCredentialsL - test ",
    "CNATFWClient",
    "SetCredentialsL",
    "FUNCTIONALITY",
    SetupStreamL, UT_CNATFWClient_SetCredentialsLL, Teardown)

EUNIT_TEST(
    "PerformCandidateChecksL - test ",
    "CNATFWClient",
    "PerformCandidateChecksL",
    "FUNCTIONALITY",
    SetupStreamL, UT_CNATFWClient_PerformConnectivityChecksLL, Teardown)
    
EUNIT_TEST(
    "UpdateIceProcessingL - test ",
    "CNATFWClient",
    "UpdateIceProcessingL",
    "FUNCTIONALITY",
    SetupStreamL, UT_CNATFWClient_UpdateIceProcessingLL, Teardown)
        
EUNIT_TEST(
    "UpdateIceProcessingL - test 2 ",
    "CNATFWClient",
    "UpdateIceProcessingL",
    "FUNCTIONALITY",
    SetupStreamL, UT_CNATFWClient_UpdateIceProcessingL2L, Teardown)
    
EUNIT_TEST(
    "FindStreamById - test",
    "CNATFWClient",
    "FindStreamById",
    "FUNCTIONALITY",
    SetupStreamL, UT_CNATFWClient_FindStreamByIdL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
