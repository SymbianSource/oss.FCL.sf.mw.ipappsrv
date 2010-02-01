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
#include "ut_cnatfwstunconnectionhandler.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <digia/eunit/EUnitDecorators.h>

//  INTERNAL INCLUDES
#include "natfwstunconnectionhandler.h"
#include "cncmconnectionmultiplexer.h"
#include "natfwstunbinding.h"
#include "natfwstunclient.h"
#include "natfwcandidate.h"
#include "natfwstunstreamdata.h"
#include "cstunasynccallback.h"
#include <cnatfwsettingsapi.h>
#include "testsettings.h"
#include "natfwunittestmacros.h"

_LIT8( KDomain,"www.domain.fi" );
const TUint KIapId = 6;
const TUint KRtoValue = 500; 
const TUint KQos = 100;

// CONSTRUCTION
UT_CNATFWStunConnectionHandler* UT_CNATFWStunConnectionHandler::NewL()
    {
    UT_CNATFWStunConnectionHandler* self =
        UT_CNATFWStunConnectionHandler::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWStunConnectionHandler* UT_CNATFWStunConnectionHandler::NewLC()
    {
    UT_CNATFWStunConnectionHandler* self =
        new( ELeave ) UT_CNATFWStunConnectionHandler();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWStunConnectionHandler::~UT_CNATFWStunConnectionHandler()
    {
    }

// Default constructor
UT_CNATFWStunConnectionHandler::UT_CNATFWStunConnectionHandler()
    {
    }

// Second phase construct
void UT_CNATFWStunConnectionHandler::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CNATFWStunConnectionHandler::SetupL(  )
    {
    iConnMux = CNcmConnectionMultiplexer::NewL( *this );
    iSessionId = iConnMux->CreateSessionL( KIapId, 0, 0 );
    iStreamId = iConnMux->CreateStreamL( iSessionId, KQos, KProtocolInetUdp );
    iConnHandler = CNATFWStunConnectionHandler::NewL( *iStunPlugin, *this );
    iConnHandler->PluginInitializeL( KIapId, KDomain, *iConnMux );
    iSettings = CTestSettings::NewL( KDomain, KIapId );
    }


void UT_CNATFWStunConnectionHandler::Teardown(  )
    {
    delete iConnHandler;
    iConnHandler = NULL;
    
    delete iConnMux;
    iConnMux = NULL;
    
    delete iSettings;
    iSettings = NULL;
    }
    
// From MNATFWConnectionMultiplexerObserver
void UT_CNATFWStunConnectionHandler::Notify( TUint /*aSessionId*/,
                                             TUint /*aStreamId*/,
                                             TNotifyType /*aType*/,
                                             TInt /*aError*/ )
    {

    }

// From MNATFWConnectionMultiplexerObserver
void UT_CNATFWStunConnectionHandler::IcmpError( TUint /*aSessionId*/,
                                                TUint /*aStreamId*/,
                                                const TInetAddr& /*aAddress*/ )
    {

    }
    
// From MNATFWPluginObserver
void UT_CNATFWStunConnectionHandler::Error( const CNATFWPluginApi& /*aPlugin*/,
                                            TUint /*aStreamId*/,
                                            TInt /*aErrorCode*/ )
    {

    }

// From MNATFWPluginObserver
void UT_CNATFWStunConnectionHandler::Notify( const CNATFWPluginApi& /*aPlugin*/,
                                             TUint /*aStreamId*/,
	                                         TNATFWPluginEvent /*aEvent*/,
	                                         TInt /*aErrCode*/ )
    {
    if ( iAsyncTestCase )
        {
        CActiveScheduler::Stop();
        iAsyncTestCase = EFalse;
        }
    }

// From MNATFWPluginObserver
void UT_CNATFWStunConnectionHandler::NewCandidatePairFound(
    const CNATFWPluginApi& /*aPlugin*/,
    CNATFWCandidatePair* /*aCandidatePair*/ )
    {

    }

// From MNATFWPluginObserver
void UT_CNATFWStunConnectionHandler::NewLocalCandidateFound(
    const CNATFWPluginApi& /*aPlugin*/,
    CNATFWCandidate* aLocalCandidate )
    {
    delete aLocalCandidate;
    }
    
void UT_CNATFWStunConnectionHandler::UT_CNATFWStunConnectionHandler_ConnectServerLL(  )
    {
    RSocketServ* socketServ;
    TName connectionName;
    
    socketServ = iConnMux->GetSessionInfoL( iSessionId, connectionName );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->ConnectServerL( *socketServ, connectionName ) );
    socketServ = NULL;
    }

void UT_CNATFWStunConnectionHandler::UT_CNATFWStunConnectionHandler_TryNextServerLL(  )
    {
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->TryNextServerL() );
    EUNIT_ASSERT_LEAVE( iConnHandler->TryNextServerL() );
    }

void UT_CNATFWStunConnectionHandler::UT_CNATFWStunConnectionHandler_ConnectionByIdL(  )
    {
    // create STUN Client
    iConnHandler->TryNextServerL();
    // Store stream ID and create connection to array
    iConnHandler->FetchCandidateL( iStreamId, 0, KAFUnspec, KAFUnspec );
    TUint streamIndex( 0 );
    TUint connectionId( 3 );
    
    TConnectionData* connection = iConnHandler->ConnectionById(
        streamIndex, connectionId );
    
    if ( connection )
        {
        EUNIT_ASSERT( EFalse );
        }
    
    connectionId = 2;
    
    connection = iConnHandler->ConnectionById( streamIndex, connectionId );
    
    if ( !connection )
        {
        EUNIT_ASSERT( EFalse );
        }
    }

void UT_CNATFWStunConnectionHandler::UT_CNATFWStunConnectionHandler_FetchCandidateLL(  )
    {
    EUNIT_ASSERT_LEAVE( iConnHandler->FetchCandidateL(
        222, KRtoValue, KAFUnspec, KInetAddrAny ) );
        
    // Must create STUN Client before fetching candidate:
    iConnHandler->TryNextServerL();
        
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->FetchCandidateL(
        222, KRtoValue, KAFUnspec, KInetAddrAny ) );
        
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->FetchCandidateL(
        222, KRtoValue, KAFUnspec, KInetAddrAny ) );
        
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->FetchCandidateL(
        333, KRtoValue, KAFUnspec, KInetAddrAny ) );
    }

void UT_CNATFWStunConnectionHandler::UT_CNATFWStunConnectionHandler_StartStunRefreshL(  )
    { 
    CSTUNClient* dummyClient = NULL;
    CSTUNBinding* dummyBinding = CSTUNBinding::NewLC( *dummyClient, 222, 2 );
    
    TConnectionData connData;
    TStreamData streamData;
    streamData.iStreamId = 222;
    streamData.iRtoValue = 555; // used in relay binding stub for the this test 
    connData.iConnectionId = 2;
    connData.iStunBinding = dummyBinding;
    streamData.iConnArray.AppendL( connData );
    CleanupClosePushL( streamData.iConnArray );
    iConnHandler->iStreamArray.AppendL( streamData );

    CleanupStack::Pop( &streamData.iConnArray );
    CleanupStack::Pop( dummyBinding );
    
    iConnHandler->iStunRefreshInterval = 1000;
    
    iConnHandler->StartStunRefresh( );

    if( !iConnHandler->iStunRefreshStarted )
        {
        EUNIT_ASSERT( EFalse );
        }
    
    CActiveScheduler::Start();
    
    iConnHandler->iStunRefreshInterval = 0;
    iConnHandler->StartStunRefresh( );

    if ( iConnHandler->iStunRefreshStarted )
        {
        EUNIT_ASSERT( EFalse );
        }
    }

void UT_CNATFWStunConnectionHandler::UT_CNATFWStunConnectionHandler_CreateSTUNBindingLL(  )
    {
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->CreateSTUNBindingL( 222, 2 ) );
    
    TConnectionData connData;
    TStreamData streamData;
    streamData.iStreamId = 222;
    
    connData.iConnectionId = 2;
    streamData.iConnArray.AppendL( connData );
    CleanupClosePushL( streamData.iConnArray );
    iConnHandler->iStreamArray.AppendL( streamData );
    CleanupStack::Pop( &streamData.iConnArray );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->CreateSTUNBindingL( 222, 2 ) );
        
    if ( !iConnHandler->iStreamArray[0].iConnArray[0].iStunBinding )
        {
        EUNIT_ASSERT( EFalse );
        }
    }

void UT_CNATFWStunConnectionHandler::UT_CNATFWStunConnectionHandler_SetReceivingStateLL(  )
    {
    CNATFWCandidate* candidate = CNATFWCandidate::NewL();
    CleanupStack::PushL( candidate );
    candidate->SetStreamId( 333 );
    
    iAsyncTestCase = ETrue;
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->SetReceivingStateL(
        *candidate, EStreamingStateActive ) );
    CActiveScheduler::Start();
    
    iAsyncTestCase = ETrue;
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->SetReceivingStateL(
        *candidate, EStreamingStatePassive ) );
    CActiveScheduler::Start();
    
    CNATFWCandidate* candidate2 = CNATFWCandidate::NewL();
    CleanupStack::PushL( candidate2 );
    candidate2->SetStreamId( 222 );
    candidate2->SetType( CNATFWCandidate::EServerReflexive );
    
    TStreamData streamData;
    streamData.iStreamId = 222;
    TConnectionData connData;
    connData.iConnectionId = 2;
    
    connData.iLocalCandidate = candidate2;
    streamData.iConnArray.AppendL( connData );
    CleanupClosePushL( streamData.iConnArray );
    iConnHandler->iStreamArray.AppendL( streamData );
    CleanupStack::Pop( &streamData.iConnArray );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->SetReceivingStateL(
        *candidate2, EStreamingStateActive ) );
    iConnHandler->iStreamArray[0].iConnArray[0].iReceivingActivated = ETrue;
    
    iAsyncTestCase = ETrue;
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->SetReceivingStateL(
        *candidate2, EStreamingStateActive ) );
    CActiveScheduler::Start();
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->SetReceivingStateL(
        *candidate2, EStreamingStatePassive ) );
    iConnHandler->iStreamArray[0].iConnArray[0].iReceivingActivated = EFalse;
    
    iAsyncTestCase = ETrue;
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->SetReceivingStateL(
        *candidate2, EStreamingStatePassive ) );
    CActiveScheduler::Start();
    
    CleanupStack::PopAndDestroy( candidate2 );
    CleanupStack::PopAndDestroy( candidate );
    }

void UT_CNATFWStunConnectionHandler::UT_CNATFWStunConnectionHandler_SetSendingStateLL(  )
    {
    CNATFWCandidate* candidate = CNATFWCandidate::NewL();
    CleanupStack::PushL( candidate );
    candidate->SetStreamId( 333 );
    
    iAsyncTestCase = ETrue;
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->SetSendingStateL(
        *candidate, EStreamingStateActive, KInetAddrLoop ) );
    CActiveScheduler::Start();
    
    iAsyncTestCase = ETrue;
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->SetSendingStateL(
        *candidate, EStreamingStatePassive, KInetAddrLoop ) );
    CActiveScheduler::Start();
    
    CNATFWCandidate* candidate2 = CNATFWCandidate::NewL();
    CleanupStack::PushL( candidate2 );
    candidate2->SetStreamId( 222 );
    candidate2->SetType( CNATFWCandidate::EServerReflexive );
    
    TStreamData streamData;
    streamData.iStreamId = 222;
    TConnectionData connData;
    connData.iConnectionId = 2;
    
    connData.iLocalCandidate = candidate2;
    streamData.iConnArray.AppendL( connData );
    CleanupClosePushL( streamData.iConnArray );
    iConnHandler->iStreamArray.AppendL( streamData );
    CleanupStack::Pop( &streamData.iConnArray );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->SetSendingStateL(
        *candidate2, EStreamingStateActive, KInetAddrLoop ) );
    iConnHandler->iStreamArray[0].iConnArray[0].iSendingActivated = ETrue;
    // Destination address must be set here as KInetAddrLoop has no address family
    iConnHandler->iStreamArray[0].iConnArray[0].iDestAddr = KInetAddrLoop;
    
    iAsyncTestCase = ETrue;
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->SetSendingStateL(
        *candidate2, EStreamingStateActive, KInetAddrLoop ) );
    CActiveScheduler::Start();
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->SetSendingStateL(
        *candidate2, EStreamingStateActive, KInetAddrLinkLocalNet ) );
    // Destination address must be set here as KInetAddrLinkLocalNet has no address family
    iConnHandler->iStreamArray[0].iConnArray[0].iDestAddr = KInetAddrLinkLocalNet;
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->SetSendingStateL(
        *candidate2, EStreamingStatePassive, KInetAddrLoop ) );
    iConnHandler->iStreamArray[0].iConnArray[0].iSendingActivated = EFalse;
    
    iAsyncTestCase = ETrue;
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->SetSendingStateL(
        *candidate2, EStreamingStatePassive, KInetAddrLoop ) );
    CActiveScheduler::Start();
    
    CleanupStack::PopAndDestroy();
    CleanupStack::PopAndDestroy();
    }

void UT_CNATFWStunConnectionHandler::UT_CNATFWStunConnectionHandler_GetConnectionIdLL(  )
    {
    // Matching candidate
    CNATFWCandidate* candidate = CNATFWCandidate::NewL();
    CleanupStack::PushL( candidate );
    candidate->SetStreamId( 222 );
    candidate->SetType( CNATFWCandidate::EServerReflexive );
    candidate->SetBase( KInetAddrAny );
    candidate->SetTransportAddrL( KInetAddrAny );
    
    // Non-matching candidate
    CNATFWCandidate* candidate2 = CNATFWCandidate::NewL();
    CleanupStack::PushL( candidate2 );
    candidate2->SetStreamId( 222 );
    
    // create STUN Client
    iConnHandler->TryNextServerL();
    // Store stream ID and create connection to array
    iConnHandler->FetchCandidateL( iStreamId, 0, KAFUnspec, KAFUnspec );
    iConnHandler->iStreamArray[0].iConnArray[0].iConnectionId = 2;
    iConnHandler->iStreamArray[0].iConnArray[0].iLocalCandidate = candidate;
    
    TUint connectionId( 0 );
    EUNIT_ASSERT_LEAVE( iConnHandler->GetConnectionIdL( *candidate2, 222, connectionId ) );
    
    connectionId = 0;
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->GetConnectionIdL(
        *candidate, 222, connectionId ) );
    
    CleanupStack::PopAndDestroy();
    CleanupStack::PopAndDestroy();
    }

void UT_CNATFWStunConnectionHandler::
    UT_CNATFWStunConnectionHandler_STUNClientInitCompletedL(  )
    {
    TStreamData streamData;
    streamData.iStreamId = 222;
    TConnectionData connData;
    connData.iConnectionId = 2;
    streamData.iConnArray.AppendL( connData );
    CleanupClosePushL( streamData.iConnArray );
    iConnHandler->iStreamArray.AppendL( streamData );
    CleanupStack::Pop( &streamData.iConnArray );
    iAsyncTestCase = ETrue;
    iConnHandler->STUNClientInitCompleted( *iConnHandler->iStunClient, KErrNone );
    CActiveScheduler::Start();
    
    iConnHandler->STUNClientInitCompleted( *iConnHandler->iStunClient, KErrArgument );
    }

void UT_CNATFWStunConnectionHandler::
    UT_CNATFWStunConnectionHandler_STUNClientInitCompleted_AllocL(  )
    {
    TStreamData streamData;
    streamData.iStreamId = 222;
    TConnectionData connData;
    connData.iConnectionId = 2;
    streamData.iConnArray.AppendL( connData );
    CleanupClosePushL( streamData.iConnArray );
    iConnHandler->iStreamArray.AppendL( streamData );
    CleanupStack::Pop( &streamData.iConnArray );
    iAsyncTestCase = ETrue;
    iConnHandler->STUNClientInitCompleted( *iConnHandler->iStunClient, KErrNone );
    
    iConnHandler->STUNClientInitCompleted( *iConnHandler->iStunClient, KErrArgument );
    }


void UT_CNATFWStunConnectionHandler::
    UT_CNATFWStunConnectionHandler_STUNBindingEventOccurredLL(  )
    {
    // Create STUN Client
    iConnHandler->TryNextServerL();
    CSTUNBinding* dummyBinding = CSTUNBinding::NewLC(
        *iConnHandler->iStunClient, iStreamId, 2 );

    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->STUNBindingEventOccurredL(
        MSTUNClientObserver::EPublicAddressResolved, *dummyBinding ) );

    // Store stream and create connection to array
    iConnHandler->FetchCandidateL( iStreamId, 0, KAFUnspec, KAFUnspec );
    iConnHandler->iStreamArray[0].iConnArray[0].iStunBinding = dummyBinding;
    CleanupStack::Pop( dummyBinding );
    
    iAsyncTestCase = ETrue;
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->STUNBindingEventOccurredL(
        MSTUNClientObserver::EPublicAddressResolved, *dummyBinding ) );
    CActiveScheduler::Start();
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->STUNBindingEventOccurredL(
        MSTUNClientObserver::EPublicAddressResolved, *dummyBinding ) );
    
    iAsyncTestCase = ETrue;
    delete iConnHandler->iStreamArray[0].iConnArray[0].iLocalCandidate;
    iConnHandler->iStreamArray[0].iConnArray[0].iLocalCandidate = NULL;
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->STUNBindingEventOccurredL(
        MSTUNClientObserver::EPublicAddressResolved, *dummyBinding ) );
    CActiveScheduler::Start();
    
    iAsyncTestCase = ETrue;
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->STUNBindingEventOccurredL(
        MSTUNClientObserver::ECredentialsRejected, *dummyBinding ) );
    CActiveScheduler::Start();
    }

void UT_CNATFWStunConnectionHandler::
    UT_CNATFWStunConnectionHandler_STUNBindingErrorOccurredL(  )
    {
    // Create STUN Client
    iConnHandler->TryNextServerL();
    CSTUNBinding* dummyBinding = CSTUNBinding::NewLC(
        *iConnHandler->iStunClient, iStreamId, 2 );
    
    iConnHandler->STUNBindingErrorOccurred( *dummyBinding, KErrNotFound );
    
    // Store stream ID and create connection to array
    iConnHandler->FetchCandidateL( iStreamId, 0, KAFUnspec, KAFUnspec );
    iConnHandler->iStreamArray[0].iConnArray[0].iStunBinding = dummyBinding;
    CleanupStack::Pop( dummyBinding );
    
    // Alternate server case
    iConnHandler->STUNBindingErrorOccurred( *dummyBinding, 300 );
    
    // Error during candidate fetching case
    dummyBinding = CSTUNBinding::NewL( *iConnHandler->iStunClient, iStreamId, 2 );
    iConnHandler->iStreamArray[0].iConnArray[0].iStunBinding = dummyBinding;
    iAsyncTestCase = ETrue;
    iConnHandler->STUNBindingErrorOccurred( *dummyBinding, KErrNotFound );
    CActiveScheduler::Start();
    
    // Error after candidate fetching is completed
    dummyBinding = CSTUNBinding::NewL( *iConnHandler->iStunClient, iStreamId, 2 );
    iConnHandler->FetchCandidateL( iStreamId, 0, KAFUnspec, KAFUnspec );
    
    iConnHandler->iStreamArray[0].iConnArray[0].iStunBinding = dummyBinding;
    CNATFWCandidate* candidate = CNATFWCandidate::NewL();
    CleanupStack::PushL( candidate );
    candidate->SetStreamId( iStreamId );
    iConnHandler->iStreamArray[0].iConnArray[0].iLocalCandidate = candidate;
    CleanupStack::Pop( candidate );
    
    iConnHandler->STUNBindingErrorOccurred( *dummyBinding, KErrNotFound );
    }


void UT_CNATFWStunConnectionHandler::
    UT_CNATFWStunConnectionHandler_STUNBindingErrorOccurred_AllocL()
    {
    // Create STUN Client
    iConnHandler->TryNextServerL();
    CSTUNBinding* dummyBinding = CSTUNBinding::NewLC(
        *iConnHandler->iStunClient, iStreamId, 2 );
    
    iConnHandler->STUNBindingErrorOccurred( *dummyBinding, KErrNotFound );
    
    // Store stream ID and create connection to array
    iConnHandler->FetchCandidateL( iStreamId, 0, KAFUnspec, KAFUnspec );
    iConnHandler->iStreamArray[0].iConnArray[0].iStunBinding = dummyBinding;
    CleanupStack::Pop( dummyBinding );
    
    // Alternate server case
    iConnHandler->STUNBindingErrorOccurred( *dummyBinding, 300 );
    
    // Error during candidate fetching case
    dummyBinding = CSTUNBinding::NewL( *iConnHandler->iStunClient, iStreamId, 2 );
    iConnHandler->iStreamArray[0].iConnArray[0].iStunBinding = dummyBinding;
    iAsyncTestCase = ETrue;
    iConnHandler->STUNBindingErrorOccurred( *dummyBinding, KErrNotFound );
    
    // Error after candidate fetching is completed
    dummyBinding = CSTUNBinding::NewL( *iConnHandler->iStunClient, iStreamId, 2 );
    iConnHandler->FetchCandidateL( iStreamId, 0, KAFUnspec, KAFUnspec );
    
    iConnHandler->iStreamArray[0].iConnArray[0].iStunBinding = dummyBinding;
    CNATFWCandidate* candidate = CNATFWCandidate::NewL();
    CleanupStack::PushL( candidate );
    candidate->SetStreamId( iStreamId );
    iConnHandler->iStreamArray[0].iConnArray[0].iLocalCandidate = candidate;
    CleanupStack::Pop( candidate );
    
    iConnHandler->STUNBindingErrorOccurred( *dummyBinding, KErrNotFound );
    }


void UT_CNATFWStunConnectionHandler::
    UT_CNATFWStunConnectionHandler_IncomingMessageLL(  )
    {
    TInetAddr dummyAddr;
    const TInetAddr dummyAddr2;
    HBufC8* dummyMessage = NULL;
    TBool consumed( EFalse );
    
    // Create STUN Client
    iConnHandler->TryNextServerL();
    
    // Create connections to array
    iConnHandler->FetchCandidateL( iStreamId, 0, KAFUnspec, KAFUnspec );
    CSTUNBinding* dummyBinding = CSTUNBinding::NewL(
        *iConnHandler->iStunClient, iStreamId, 2 );
    iConnHandler->iStreamArray[0].iConnArray[0].iStunBinding = dummyBinding;
    iConnHandler->FetchCandidateL( iStreamId, 0, KAFUnspec, KAFUnspec );

    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->IncomingMessageL( 333,
                                                           *dummyMessage,
                                                           dummyAddr2,
                                                           dummyAddr2,
                                                           dummyAddr,
                                                           consumed ) );

    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->IncomingMessageL( 222,
                                                           *dummyMessage,
                                                           dummyAddr2,
                                                           dummyAddr2,
                                                           dummyAddr,
                                                           consumed ) );
    }

void UT_CNATFWStunConnectionHandler::UT_CNATFWStunConnectionHandler_ConnectionNotifyL(  )
    {
    iConnHandler->TryNextServerL();
    iConnHandler->FetchCandidateL( iStreamId, 0, KAFUnspec, KAFUnspec );
    
    iConnHandler->ConnectionNotify( 333,
                                    3,
                                    MNcmConnectionObserver::ESendingActivated,
                                    KErrNone );
    
    // Events during candidate fetching
    iConnHandler->ConnectionNotify( iStreamId,
                                    2,
                                    MNcmConnectionObserver::ESendingActivated,
                                    KErrNone );

    iConnHandler->ConnectionNotify( iStreamId,
                                    2,
                                    MNcmConnectionObserver::ESendingActivated,
                                    KErrNone );
    iConnHandler->ConnectionNotify( iStreamId,
                                    2,
                                    MNcmConnectionObserver::EReceivingActivated,
                                    KErrNone );
    iConnHandler->ConnectionNotify( iStreamId,
                                    2,
                                    MNcmConnectionObserver::ESendingDeactivated,
                                    KErrNone );
    iConnHandler->ConnectionNotify( iStreamId,
                                    2,
                                    MNcmConnectionObserver::EReceivingDeactivated,
                                    KErrNone );

    // Events after candidate has been fetched
    CNATFWCandidate* candidate = CNATFWCandidate::NewL();
    CleanupStack::PushL( candidate );
    candidate->SetStreamId( 222 );
    iConnHandler->iStreamArray[0].iConnArray[0].iLocalCandidate = candidate;
    CleanupStack::Pop( candidate );
    
    
    iConnHandler->ConnectionNotify( iStreamId,
                                    2,
                                    MNcmConnectionObserver::ESendingActivated,
                                    KErrNone );

    iConnHandler->ConnectionNotify( iStreamId,
                                    2,
                                    MNcmConnectionObserver::EReceivingActivated,
                                    KErrNone );
    iConnHandler->ConnectionNotify( iStreamId,
                                    2,
                                    MNcmConnectionObserver::ESendingDeactivated,
                                    KErrNone );
    iConnHandler->ConnectionNotify( iStreamId,
                                    2,
                                    MNcmConnectionObserver::EReceivingDeactivated,
                                    KErrNone );
    
    iConnHandler->ConnectionNotify( iStreamId,
                                    2,
                                    MNcmConnectionObserver::EFirstMediaPacketSent,
                                    KErrNone );
    
    iConnHandler->ConnectionNotify( iStreamId,
                                    2,
                                    MNcmConnectionObserver::EConnectionRemoved,
                                    KErrNone );
    
    // Error events
    iConnHandler->FetchCandidateL( iStreamId, 0, KAFUnspec, KAFUnspec );
    
    iConnHandler->ConnectionNotify( iStreamId,
                                    2,
                                    MNcmConnectionObserver::EConnectionError,
                                    KErrCouldNotConnect );
    
    iConnHandler->FetchCandidateL( iStreamId, 0, KAFUnspec, KAFUnspec );
    iConnHandler->iStreamArray[0].iConnArray[0].iLocalCandidate = candidate;
    
    iConnHandler->ConnectionNotify( iStreamId,
                                    2,
                                    MNcmConnectionObserver::EConnectionError,
                                    KErrCouldNotConnect );
    }

void UT_CNATFWStunConnectionHandler::UT_CNATFWStunConnectionHandler_BindingRefreshLL(  )
    {
    CSTUNClient* dummyClient = NULL;
    CSTUNBinding* dummyBinding = CSTUNBinding::NewLC( *dummyClient, iStreamId, 2 );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->BindingRefreshL() );
    
    TConnectionData connData;
    TStreamData streamData;
    streamData.iStreamId = iStreamId;
    connData.iConnectionId = 2;
    connData.iStunBinding = dummyBinding;
    streamData.iConnArray.AppendL( connData );
    CleanupClosePushL( streamData.iConnArray );
    iConnHandler->iStreamArray.AppendL( streamData );
    CleanupStack::Pop( &streamData.iConnArray );
    CleanupStack::Pop( dummyBinding );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->BindingRefreshL() );
    }
    
void UT_CNATFWStunConnectionHandler::UT_CNATFWStunConnectionHandler_DeleteStreamL(  )
    {
    // create STUN Client
    iConnHandler->TryNextServerL();
    // Store stream ID and create connections to array
    iConnHandler->FetchCandidateL( iStreamId, 0, KAFUnspec, KAFUnspec );
    iConnHandler->FetchCandidateL( iStreamId, 0, KAFUnspec, KAFUnspec );
    CSTUNBinding* dummyBinding = CSTUNBinding::NewL( *iConnHandler->iStunClient, iStreamId, 2 );
    iConnHandler->iStreamArray[0].iConnArray[1].iStunBinding = dummyBinding;
    
    TUint streamIndex( 0 );
    
    iConnHandler->DeleteStream( streamIndex, ETrue );
    
    if ( iConnHandler->iStreamArray.Count() )
        {
        EUNIT_ASSERT( EFalse );
        }
    
    iConnHandler->FetchCandidateL( iStreamId, 0, KAFUnspec, KAFUnspec );
    
    iConnHandler->DeleteStream( streamIndex, EFalse );
    
    if ( iConnHandler->iStreamArray.Count() )
        {
        EUNIT_ASSERT( EFalse );
        }
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWStunConnectionHandler,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "ConnectServerL - test ",
    "CNATFWStunConnectionHandler",
    "ConnectServerL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWStunConnectionHandler_ConnectServerLL, Teardown)

EUNIT_TEST(
    "TryNextServerL - test ",
    "CNATFWStunConnectionHandler",
    "TryNextServerL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWStunConnectionHandler_TryNextServerLL, Teardown)
    
EUNIT_TEST(
    "ConnectionById - test ",
    "CNATFWStunConnectionHandler",
    "ConnectionById",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWStunConnectionHandler_ConnectionByIdL, Teardown)

EUNIT_TEST(
    "FetchCandidateL - test ",
    "CNATFWStunConnectionHandler",
    "FetchCandidateL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWStunConnectionHandler_FetchCandidateLL, Teardown)

EUNIT_TEST(
    "StartStunRefresh - test ",
    "CNATFWStunConnectionHandler",
    "StartStunRefresh",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWStunConnectionHandler_StartStunRefreshL, Teardown)

EUNIT_TEST(
    "CreateSTUNBindingL - test ",
    "CNATFWStunConnectionHandler",
    "CreateSTUNBindingL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWStunConnectionHandler_CreateSTUNBindingLL, Teardown)

EUNIT_TEST(
    "SetReceivingStateL - test ",
    "CNATFWStunConnectionHandler",
    "SetReceivingStateL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWStunConnectionHandler_SetReceivingStateLL, Teardown)

EUNIT_TEST(
    "SetSendingStateL - test ",
    "CNATFWStunConnectionHandler",
    "SetSendingStateL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWStunConnectionHandler_SetSendingStateLL, Teardown)

EUNIT_TEST(
    "GetConnectionIdL - test ",
    "CNATFWStunConnectionHandler",
    "GetConnectionIdL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWStunConnectionHandler_GetConnectionIdLL, Teardown)

EUNIT_ALLOC_TEST(
    "STUNClientInitCompleted - alloc test ",
    "CNATFWStunConnectionHandler",
    "STUNClientInitCompleted",
    "ERROHANDLING",
    SetupL, UT_CNATFWStunConnectionHandler_STUNClientInitCompleted_AllocL, Teardown)
    
EUNIT_NOT_DECORATED_TEST(
    "STUNClientInitCompleted - test ",
    "CNATFWStunConnectionHandler",
    "STUNClientInitCompleted",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWStunConnectionHandler_STUNClientInitCompletedL, Teardown)

EUNIT_TEST(
    "STUNBindingEventOccurredL - test ",
    "CNATFWStunConnectionHandler",
    "STUNBindingEventOccurredL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWStunConnectionHandler_STUNBindingEventOccurredLL, Teardown)

EUNIT_ALLOC_TEST(
    "STUNBindingErrorOccurred - alloc test ",
    "CNATFWStunConnectionHandler",
    "STUNBindingErrorOccurred",
    "ERROHANDLING",
    SetupL, UT_CNATFWStunConnectionHandler_STUNBindingErrorOccurred_AllocL, Teardown)    
    
EUNIT_NOT_DECORATED_TEST(
    "STUNBindingErrorOccurred - test ",
    "CNATFWStunConnectionHandler",
    "STUNBindingErrorOccurred",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWStunConnectionHandler_STUNBindingErrorOccurredL, Teardown)

EUNIT_TEST(
    "IncomingMessageL - test ",
    "CNATFWStunConnectionHandler",
    "IncomingMessageL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWStunConnectionHandler_IncomingMessageLL, Teardown)

EUNIT_TEST(
    "ConnectionNotify - test ",
    "CNATFWStunConnectionHandler",
    "ConnectionNotify",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWStunConnectionHandler_ConnectionNotifyL, Teardown)

EUNIT_TEST(
    "BindingRefreshL - test ",
    "CNATFWStunConnectionHandler",
    "BindingRefreshL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWStunConnectionHandler_BindingRefreshLL, Teardown)

EUNIT_TEST(
    "DeleteStream - test ",
    "CNATFWStunConnectionHandler",
    "DeleteStream",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWStunConnectionHandler_DeleteStreamL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
