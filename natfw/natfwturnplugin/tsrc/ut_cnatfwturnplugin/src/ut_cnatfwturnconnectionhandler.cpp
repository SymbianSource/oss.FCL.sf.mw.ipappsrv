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
#include "ut_cnatfwturnconnectionhandler.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <digia/eunit/CEunitAllocTestCaseDecorator.h>   // for using EUNIT_ALLOC_TEST
#include <badesca.h>

//  INTERNAL INCLUDES
#include "natfwturnconnectionhandler.h"
#include "cncmconnectionmultiplexer.h"
#include "natfwstunrelaybinding.h"
#include "natfwstunclient.h"
#include "natfwcandidate.h"
#include "natfwpluginapi.h"

#include "natfwunsafsendindication.h"
#include "natfwunsafallocaterequest.h"
#include "natfwunsafconnectrequest.h"
#include "natfwunsafsetactivedestinationrequest.h"
#include "natfwunsafbindingrequest.h"
#include "natfwunsafsetactivedestinationerrorresponse.h"
#include "natfwunsafdataindication.h"
#include "natfwunsafremoteaddressattribute.h"
#include "natfwunsafdataattribute.h"
#include "natfwunsafbindingresponse.h"
#include "cturnasynccallback.h"

//#include "tnatserversettings.h"
#include <cnatfwsettingsapi.h>
#include "testsettings.h"
#include "natfwunittestmacros.h"

_LIT8( KDomain,"www.domain.fi" );

const TUint KIapId = 6;
const TUint KRtoValue = 500; 
const TUint KQos = 100;

CActiveSchedulerWait UT_CNATFWTurnConnectionHandler::iActiveSchedulerWait;

// CONSTRUCTION
UT_CNATFWTurnConnectionHandler* UT_CNATFWTurnConnectionHandler::NewL()
    {
    UT_CNATFWTurnConnectionHandler* self =
        UT_CNATFWTurnConnectionHandler::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWTurnConnectionHandler* UT_CNATFWTurnConnectionHandler::NewLC()
    {
    UT_CNATFWTurnConnectionHandler* self =
        new( ELeave ) UT_CNATFWTurnConnectionHandler();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWTurnConnectionHandler::~UT_CNATFWTurnConnectionHandler()
    {
    }

// Default constructor
UT_CNATFWTurnConnectionHandler::UT_CNATFWTurnConnectionHandler()
    {
    }

// Second phase construct
void UT_CNATFWTurnConnectionHandler::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CNATFWTurnConnectionHandler::SetupL(  )
    {
    //__UHEAP_MARK;
    iConnMux = CNcmConnectionMultiplexer::NewL( *this );
    iSessionId = iConnMux->CreateSessionL( KIapId, 0, 0 );
    iStreamId = iConnMux->CreateStreamL( iSessionId, KQos, KProtocolInetUdp );
    iConnHandler = CNATFWTurnConnectionHandler::NewL( *iTurnPlugin, *this );
    iConnHandler->PluginInitializeL( KIapId, KDomain, *iConnMux );
    iSettings = CTestSettings::NewL( KDomain, KIapId );
    }

void UT_CNATFWTurnConnectionHandler::SetupAL(  )
    {
    iAlloc = ETrue;
    
    iConnMux = CNcmConnectionMultiplexer::NewL( *this );
    iSessionId = iConnMux->CreateSessionL( KIapId, 0, 0 );
    iStreamId = iConnMux->CreateStreamL( iSessionId, KQos, KProtocolInetUdp );
    iConnHandler = CNATFWTurnConnectionHandler::NewL( *iTurnPlugin, *this );
    iConnHandler->PluginInitializeL( KIapId, KDomain, *iConnMux );
    iSettings = CTestSettings::NewL( KDomain, KIapId );
    }

void UT_CNATFWTurnConnectionHandler::Teardown(  )
    {
    delete iConnHandler;
    iConnHandler = NULL;
    
    delete iConnMux;
    iConnMux = NULL;
    
    delete iSettings;
    iSettings = NULL;
    
    StopActiveScheduler();
    
    //__UHEAP_MARKEND;
    }
    
// From MNATFWConnectionMultiplexerObserver
void UT_CNATFWTurnConnectionHandler::Notify( TUint /*aSessionId*/,
                                             TUint /*aStreamId*/,
                                             TNotifyType /*aType*/,
                                             TInt /*aError*/ )
    {

    }

// From MNATFWConnectionMultiplexerObserver
void UT_CNATFWTurnConnectionHandler::IcmpError( TUint /*aSessionId*/,
                                                TUint /*aStreamId*/,
                                                const TInetAddr& /*aAddress*/ )
    {

    }
    
// From MNATFWPluginObserver
void UT_CNATFWTurnConnectionHandler::Error( const CNATFWPluginApi& /*aPlugin*/,
                                            TUint /*aStreamId*/,
                                            TInt /*aErrorCode*/ )
    {

    }

// From MNATFWPluginObserver
void UT_CNATFWTurnConnectionHandler::Notify( const CNATFWPluginApi& /*aPlugin*/,
                                             TUint /*aStreamId*/,
	                                         TNATFWPluginEvent /*aEvent*/,
	                                         TInt /*aErrCode*/ )
    {
    if ( iAsyncTestCase )
        {
        StopActiveScheduler();
        iAsyncTestCase = EFalse;
        }
    }

// From MNATFWPluginObserver
void UT_CNATFWTurnConnectionHandler::NewCandidatePairFound( 
    const CNATFWPluginApi& /*aPlugin*/,
    CNATFWCandidatePair* /*aCandidatePair*/ )
    {

    }

// From MNATFWPluginObserver
void UT_CNATFWTurnConnectionHandler::NewLocalCandidateFound(
    const CNATFWPluginApi& /*aPlugin*/,
    CNATFWCandidate* aLocalCandidate )
    {
    delete aLocalCandidate;
    }
    
void UT_CNATFWTurnConnectionHandler::UT_CNATFWTurnConnectionHandler_ConnectServerLL(  )
    {
    TName connectionName;
    RSocketServ* socketServ = iConnMux->GetSessionInfoL( iSessionId, connectionName );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->ConnectServerL( *socketServ, connectionName ) );
    }

void UT_CNATFWTurnConnectionHandler::UT_CNATFWTurnConnectionHandler_TryNextServerLL(  )
    {
    if ( !iAlloc )
        {
        NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->TryNextServerL() );
        
        EUNIT_ASSERT_LEAVE( iConnHandler->TryNextServerL() );
        }
    else
        {
        EUNIT_ASSERT_LEAVE( iConnHandler->TryNextServerL() );
        }
    }

void UT_CNATFWTurnConnectionHandler::UT_CNATFWTurnConnectionHandler_ConnectionByIdL(  )
    {
    // create STUN Client
    iConnHandler->TryNextServerL();
    // Store stream ID and create connection to array
    iConnHandler->FetchCandidateL( iStreamId, 0, KAFUnspec, KAFUnspec );
    TUint streamIndex( 0 );
    TUint connectionId( 3 );
    
    TConnectionData* connection = iConnHandler->ConnectionById( streamIndex, connectionId );
    
    if ( connection )
        {
        EUNIT_ASSERT( EFalse );
        }
    
    connectionId = 1;
    
    connection = iConnHandler->ConnectionById( streamIndex, connectionId );
    
    if ( !connection )
        {
        EUNIT_ASSERT( EFalse );
        }
    }

void UT_CNATFWTurnConnectionHandler::UT_CNATFWTurnConnectionHandler_FetchCandidateLL(  )
    {
    if ( !iAlloc )
        {
        EUNIT_ASSERT_LEAVE( iConnHandler->FetchCandidateL( iStreamId, KRtoValue, KAFUnspec, KInetAddrAny ) );
        
        // Must create STUN Client before fetching candidate:
        iConnHandler->TryNextServerL();
        
        NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->FetchCandidateL( iStreamId, KRtoValue, KAFUnspec, KInetAddrAny ) );
        
        NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->FetchCandidateL( iStreamId, KRtoValue, KAFUnspec, KInetAddrAny ) );
        
        TInt stream2Id = iConnMux->CreateStreamL( iSessionId, KQos, KProtocolInetUdp );
        NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->FetchCandidateL( stream2Id, KRtoValue, KAFUnspec, KInetAddrAny ) );
        }
    else
        {
        EUNIT_ASSERT_LEAVE( iConnHandler->FetchCandidateL( iStreamId, KRtoValue, KAFUnspec, KInetAddrAny ) );
        
        EUNIT_ASSERT_LEAVE( iConnHandler->FetchCandidateL( 333, KRtoValue, KAFUnspec, KInetAddrAny ) );
        }
    }

void UT_CNATFWTurnConnectionHandler::UT_CNATFWTurnConnectionHandler_StartTurnRefreshL(  )
    {
    CSTUNClient* dummyClient = NULL;
    CSTUNRelayBinding* dummyBinding = CSTUNRelayBinding::NewL( *dummyClient, 222, 2 );
    
    CleanupStack::PushL( dummyBinding );
    TConnectionData connData;
    TStreamData streamData;
    streamData.iStreamId = 222;
    streamData.iRtoValue = 555; // used in relay binding stub for the this test 
    connData.iConnectionId = 2;
    connData.iTurnBinding = dummyBinding;
    streamData.iConnArray.AppendL( connData );
    CleanupClosePushL( streamData.iConnArray );
    iConnHandler->iStreamArray.AppendL( streamData );
    CleanupStack::Pop( &streamData.iConnArray );
    CleanupStack::Pop( dummyBinding );
   
    iConnHandler->iTurnRefreshInterval = 1000;
    iConnHandler->StartTurnRefresh();

    if( !iConnHandler->iTurnRefreshStarted )
        {
        EUNIT_ASSERT( EFalse );
        }
        
    StartActiveScheduler(); // stopped in the stun relay binding stub
    
    iConnHandler->iTurnRefreshInterval = 0;
    iConnHandler->StartTurnRefresh();

    if ( iConnHandler->iTurnRefreshStarted )
        {
        EUNIT_ASSERT( EFalse );
        }
    }

void UT_CNATFWTurnConnectionHandler::UT_CNATFWTurnConnectionHandler_CreateTURNBindingLL(  )
    {
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->CreateTURNBindingL( 222, 2 ) );
    
    TConnectionData connData;
    TStreamData streamData;
    streamData.iStreamId = 222;
    
    connData.iConnectionId = 2;
    streamData.iConnArray.AppendL( connData );
    CleanupClosePushL( streamData.iConnArray );
    iConnHandler->iStreamArray.AppendL( streamData );
    CleanupStack::Pop( &streamData.iConnArray );
    
    if ( !iAlloc )
        {
        NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->CreateTURNBindingL( 222, 2 ) );
        
        if ( !iConnHandler->iStreamArray[0].iConnArray[0].iTurnBinding )
            {
            EUNIT_ASSERT( EFalse );
            }
        }
    else
        {
        EUNIT_ASSERT_LEAVE( iConnHandler->CreateTURNBindingL( 222, 2 ) );
        }
    }

void UT_CNATFWTurnConnectionHandler::UT_CNATFWTurnConnectionHandler_SetReceivingStateLL(  )
    {
    CNATFWCandidate* candidate = CNATFWCandidate::NewL();
    CleanupStack::PushL( candidate );
    candidate->SetStreamId( 333 );
    
    iAsyncTestCase = ETrue;
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->SetReceivingStateL( *candidate, EStreamingStateActive ) );
    StartActiveScheduler();
    
    iAsyncTestCase = ETrue;
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->SetReceivingStateL( *candidate, EStreamingStatePassive ) );
    StartActiveScheduler();
    
    CNATFWCandidate* candidate2 = CNATFWCandidate::NewL();
    CleanupStack::PushL( candidate2 );
    candidate2->SetStreamId( 222 );
    candidate2->SetType( CNATFWCandidate::ERelay );
    
    TStreamData streamData;
    streamData.iStreamId = 222;
    TConnectionData connData;
    connData.iConnectionId = 2;
    
    connData.iLocalCandidate = candidate2;
    streamData.iConnArray.AppendL( connData );
    
    CleanupClosePushL( streamData.iConnArray );
    iConnHandler->iStreamArray.AppendL( streamData );
    CleanupStack::Pop( &streamData.iConnArray );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->SetReceivingStateL( *candidate2, EStreamingStateActive ) );
    iConnHandler->iStreamArray[0].iConnArray[0].iReceivingActivated = ETrue;
    
    iAsyncTestCase = ETrue;
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->SetReceivingStateL( *candidate2, EStreamingStateActive ) );
    StartActiveScheduler();
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->SetReceivingStateL( *candidate2, EStreamingStatePassive ) );
    iConnHandler->iStreamArray[0].iConnArray[0].iReceivingActivated = EFalse;
    
    iAsyncTestCase = ETrue;
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->SetReceivingStateL( *candidate2, EStreamingStatePassive ) );
    StartActiveScheduler();
    
    CleanupStack::PopAndDestroy();
    CleanupStack::PopAndDestroy();
    }

void UT_CNATFWTurnConnectionHandler::UT_CNATFWTurnConnectionHandler_SetSendingStateLL(  )
    {
    CNATFWCandidate* candidate = CNATFWCandidate::NewL();
    CleanupStack::PushL( candidate );
    candidate->SetStreamId( 333 );
    
    iAsyncTestCase = ETrue;
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->SetSendingStateL( *candidate, EStreamingStateActive, KInetAddrLoop ) );
    StartActiveScheduler();
    
    iAsyncTestCase = ETrue;
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->SetSendingStateL( *candidate, EStreamingStatePassive, KInetAddrLoop ) );
    StartActiveScheduler();
    
    CSTUNClient* dummyClient = NULL;
    CSTUNRelayBinding* dummyBinding = CSTUNRelayBinding::NewL( *dummyClient, 222, 2 );
    CleanupStack::PushL( dummyBinding );
    CNATFWCandidate* candidate2 = CNATFWCandidate::NewL();
    CleanupStack::PushL( candidate2 );
    candidate2->SetStreamId( 222 );
    candidate2->SetType( CNATFWCandidate::ERelay );
    
    TStreamData streamData;
    streamData.iStreamId = 222;
    TConnectionData connData;
    connData.iConnectionId = 2;
    connData.iTurnBinding = dummyBinding;
    connData.iLocalCandidate = candidate2;
    
    streamData.iConnArray.AppendL( connData );
    CleanupClosePushL( streamData.iConnArray );
    iConnHandler->iStreamArray.AppendL( streamData );
    CleanupStack::Pop( &streamData.iConnArray );
    CleanupStack::Pop( candidate2 );
    CleanupStack::Pop( dummyBinding );
    
    iConnHandler->iStreamArray[0].iTransportProtocol = KProtocolInetIcmp;
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->SetSendingStateL( *candidate2, EStreamingStateActive, KInetAddrLoop ) );
    
    iConnHandler->iStreamArray[0].iTransportProtocol = KProtocolInetUdp;
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->SetSendingStateL( *candidate2, EStreamingStateActive, KInetAddrLoop ) );
    // Destination address must be set here as KInetAddrLoop has no address family
    iConnHandler->iStreamArray[0].iConnArray[0].iCurrentActDest = KInetAddrLoop;
    
    iAsyncTestCase = ETrue;
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->SetSendingStateL( *candidate2, EStreamingStateActive, KInetAddrLoop ) );
    StartActiveScheduler();
    
    iConnHandler->iStreamArray[0].iConnArray[0].iCurrentActDest = KAFUnspec;
    iConnHandler->iStreamArray[0].iTransportProtocol = KProtocolInetTcp;
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->SetSendingStateL( *candidate2, EStreamingStateActive, KInetAddrLoop ) );
    
    iConnHandler->iStreamArray[0].iConnArray[0].iActDestState = TConnectionData::EActDestTransitioning;
    iAsyncTestCase = ETrue;
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->SetSendingStateL( *candidate2, EStreamingStateActive, KInetAddrLoop ) );
    StartActiveScheduler();
    
    iConnHandler->iStreamArray[0].iConnArray[0].iActDestState = TConnectionData::EActDestSet;
    iConnHandler->iStreamArray[0].iConnArray[0].iCurrentActDest = KInetAddrLoop;
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->SetSendingStateL( *candidate2, EStreamingStatePassive, KAFUnspec ) );
  
    CleanupStack::PopAndDestroy( candidate );  
    }

void UT_CNATFWTurnConnectionHandler::UT_CNATFWTurnConnectionHandler_GetConnectionIdLL(  )
    {
    // Matching candidate
    CNATFWCandidate* candidate = CNATFWCandidate::NewL();
    CleanupStack::PushL( candidate );
    candidate->SetStreamId( iStreamId );
    candidate->SetType( CNATFWCandidate::ERelay );
    candidate->SetBase( KInetAddrAny );
    candidate->SetTransportAddrL( KInetAddrAny );
    
    // Non-matching candidate
    CNATFWCandidate* candidate2 = CNATFWCandidate::NewL();
    CleanupStack::PushL( candidate2 );
    candidate2->SetStreamId( iStreamId );
    
    // create STUN Client
    iConnHandler->TryNextServerL();
    // Store stream ID and create connection to array
    iConnHandler->FetchCandidateL( iStreamId, 0, KAFUnspec, KAFUnspec );
    iConnHandler->iStreamArray[0].iConnArray[0].iConnectionId = 1;
    iConnHandler->iStreamArray[0].iConnArray[0].iLocalCandidate = candidate;
    
    TUint connectionId( 0 );
    EUNIT_ASSERT_LEAVE( iConnHandler->GetConnectionIdL( *candidate2, iStreamId, connectionId ) );
    
    connectionId = 0;
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->GetConnectionIdL( *candidate, iStreamId, connectionId ) );
    
    CleanupStack::PopAndDestroy();
    CleanupStack::PopAndDestroy();
    }
    
void UT_CNATFWTurnConnectionHandler::UT_CNATFWTurnConnectionHandler_IsRequestOrIndicationLL(  )
    {    
    TNATFWUNSAFTransactionID transactionID;
    TBool booleanVal;
    HBufC8* message = NULL;
    CNATFWUNSAFMessage* msg = NULL;
    CBufBase* temp = NULL;
    TInt length;

    // Test all TURN request types for coverage

    msg = CNATFWUNSAFAllocateRequest::NewL( transactionID );
    CleanupStack::PushL( msg );    
    temp = msg->EncodeL();
    CleanupStack::PushL( temp );
    length = temp->Size();
    message = HBufC8::NewL( length );
    TPtr8 writable( message->Des() );
    temp->Read( 0, writable, length  );
    
    CleanupStack::PopAndDestroy( temp );
    CleanupStack::PopAndDestroy( msg );
    CleanupStack::PushL( message );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( booleanVal = iConnHandler->IsRequestOrIndicationL( *message ) );
    CleanupStack::PopAndDestroy( message );
    if ( !booleanVal )
        {
        EUNIT_ASSERT( EFalse );
        }
    
    msg = CNATFWUNSAFSetActiveDestinationRequest::NewL( transactionID );
    CleanupStack::PushL( msg );    
    temp = msg->EncodeL();
    CleanupStack::PushL( temp );
    length = temp->Size();
    message = HBufC8::NewL( length );
    TPtr8 writable2( message->Des() );
    temp->Read( 0, writable2, length  );
    
    CleanupStack::PopAndDestroy( temp );
    CleanupStack::PopAndDestroy( msg );
    CleanupStack::PushL( message );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( booleanVal = iConnHandler->IsRequestOrIndicationL( *message ) );
    CleanupStack::PopAndDestroy( message );
    if ( !booleanVal )
        {
        EUNIT_ASSERT( EFalse );
        }
    
    msg = CNATFWUNSAFConnectRequest::NewL( transactionID );
    CleanupStack::PushL( msg );    
    temp = msg->EncodeL();
    CleanupStack::PushL( temp );
    length = temp->Size();
    message = HBufC8::NewL( length );
    TPtr8 writable3( message->Des() );
    temp->Read( 0, writable3, length  );
    
    CleanupStack::PopAndDestroy( temp );
    CleanupStack::PopAndDestroy( msg );
    CleanupStack::PushL( message );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( booleanVal = iConnHandler->IsRequestOrIndicationL( *message ) );
    CleanupStack::PopAndDestroy( message );
    if ( !booleanVal )
        {
        EUNIT_ASSERT( EFalse );
        }
    
    msg = CNATFWUNSAFSendIndication::NewL( transactionID );
    CleanupStack::PushL( msg );    
    temp = msg->EncodeL();
    CleanupStack::PushL( temp );
    length = temp->Size();
    message = HBufC8::NewL( length );
    TPtr8 writable4( message->Des() );
    temp->Read( 0, writable4, length  );
    
    CleanupStack::PopAndDestroy( temp );
    CleanupStack::PopAndDestroy( msg );
    CleanupStack::PushL( message );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( booleanVal = iConnHandler->IsRequestOrIndicationL( *message ) );
    CleanupStack::PopAndDestroy( message );
    if ( !booleanVal )
        {
        EUNIT_ASSERT( EFalse );
        }
    
    // Test non-TURN message
    
    msg = CNATFWUNSAFBindingRequest::NewL( transactionID );
    CleanupStack::PushL( msg );    
    temp = msg->EncodeL();
    CleanupStack::PushL( temp );
    length = temp->Size();
    message = HBufC8::NewL( length );
    TPtr8 writable5( message->Des() );
    temp->Read( 0, writable5, length  );
    
    CleanupStack::PopAndDestroy( temp );
    CleanupStack::PopAndDestroy( msg );
    CleanupStack::PushL( message );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( booleanVal = iConnHandler->IsRequestOrIndicationL( *message ) );
    CleanupStack::PopAndDestroy( message );
    if ( booleanVal )
        {
        EUNIT_ASSERT( EFalse );
        }

    // Invalid message
    TBuf8<20> invaMessage;
    invaMessage.Copy( _L8( "invalid_message" ) );
    NATFW_EUNIT_ASSERT_SPECIFIC_LEAVE( iConnHandler->IsRequestOrIndicationL( invaMessage ), KErrArgument );
    }

void UT_CNATFWTurnConnectionHandler::UT_CNATFWTurnConnectionHandler_IsTurnResponseLL(  )
    {
    // Wrong message type
    TNATFWUNSAFTransactionID transactionID;
    CNATFWUNSAFMessage* msg = NULL;
    TBool booleanVal;
    
    msg = CNATFWUNSAFAllocateRequest::NewL( transactionID );
    CleanupStack::PushL( msg );    
    CBufBase* temp = msg->EncodeL();
    CleanupStack::PushL( temp );
    TInt length = temp->Size();
    HBufC8* message = HBufC8::NewL( temp->Size() );
    TPtr8 writable( message->Des() );
    temp->Read( 0, writable, length  );
    
    CleanupStack::PopAndDestroy( temp );
    CleanupStack::PushL( message );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( booleanVal = iConnHandler->IsTurnResponseL( *message ) );
    CleanupStack::PopAndDestroy( message );
    CleanupStack::PopAndDestroy( msg );
    EUNIT_ASSERT( !booleanVal );
    
    // Right message type cannot be tested as these UNSAF Responses' NewL methods are not exported
    
    // Invalid message
    TBuf8<20> invaMessage;
    invaMessage.Copy( _L8( "invalid_message" ) );
    NATFW_EUNIT_ASSERT_SPECIFIC_LEAVE( iConnHandler->IsTurnResponseL( invaMessage ), KErrArgument );

    }

void UT_CNATFWTurnConnectionHandler::UT_CNATFWTurnConnectionHandler_STUNClientInitCompletedL(  )
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
    TTurnPluginCallbackInfo* lastCallback = iConnHandler->iAsyncCallback->iPendingCallbacks.Last();
    iConnHandler->STUNClientInitCompleted( *iConnHandler->iStunClient, KErrNone );
    TTurnPluginCallbackInfo* lastCallback2 = iConnHandler->iAsyncCallback->iPendingCallbacks.Last();
    // Check whether callback wasn't created because of silent out-of-memory handling
    if ( lastCallback == lastCallback2 || lastCallback2 == NULL )
        {
        User::Leave( KErrNoMemory );
        }
    StartActiveScheduler();
    
    iConnHandler->STUNClientInitCompleted( *iConnHandler->iStunClient, KErrArgument );
    }

void UT_CNATFWTurnConnectionHandler::UT_CNATFWTurnConnectionHandler_STUNBindingEventOccurredLL(  )
    {
    // Create STUN Client
    iConnHandler->TryNextServerL();
    CSTUNRelayBinding* dummyBinding = CSTUNRelayBinding::NewL( *iConnHandler->iStunClient, 222, 2 );
    CleanupStack::PushL( dummyBinding );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->STUNBindingEventOccurredL(
        MSTUNClientObserver::EPublicAddressResolved, *dummyBinding ) );

    // Store stream and create connection to array
    TStreamData streamData;
    TConnectionData connData;
    streamData.iStreamId = 222;
    connData.iConnectionId = 2;
    streamData.iConnArray.AppendL( connData );
    CleanupClosePushL( streamData.iConnArray );
    iConnHandler->iStreamArray.AppendL( streamData );
    CleanupStack::Pop( &streamData.iConnArray );

    iConnHandler->iStreamArray[0].iConnArray[0].iTurnBinding = dummyBinding;
    CleanupStack::Pop( dummyBinding );
    
    iAsyncTestCase = ETrue;
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->STUNBindingEventOccurredL(
        MSTUNClientObserver::EPublicAddressResolved, *dummyBinding ) );
    StartActiveScheduler();
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->STUNBindingEventOccurredL(
        MSTUNClientObserver::EPublicAddressResolved, *dummyBinding ) );
    
    iAsyncTestCase = ETrue;
    delete iConnHandler->iStreamArray[0].iConnArray[0].iLocalCandidate;
    iConnHandler->iStreamArray[0].iConnArray[0].iLocalCandidate = NULL;
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->STUNBindingEventOccurredL(
        MSTUNClientObserver::EPublicAddressResolved, *dummyBinding ) );
    StartActiveScheduler();
    
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->STUNBindingEventOccurredL(
        MSTUNClientObserver::ETCPConnectOk, *dummyBinding ) );
    
    iConnHandler->iStreamArray[0].iConnArray[0].iPeerAddr.SetAddress( KInetAddrLoop );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->STUNBindingEventOccurredL(
        MSTUNClientObserver::EActiveDestinationSet, *dummyBinding ) );
    
    // Case where Active Destination was already set on server and was then reseted
    iConnHandler->iStreamArray[0].iConnArray[0].iActDestAlreadySetOnServer = ETrue;
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->STUNBindingEventOccurredL(
        MSTUNClientObserver::EActiveDestinationSet, *dummyBinding ) );
    iConnHandler->iStreamArray[0].iConnArray[0].iActDestAlreadySetOnServer = EFalse;
    
        
    iConnHandler->iStreamArray[0].iConnArray[0].iActDestState = TConnectionData::EActDestSet;
    
    iConnHandler->iStreamArray[0].iConnArray[0].iCurrentActDest.SetAddress( KInetAddrLoop );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->STUNBindingEventOccurredL(
        MSTUNClientObserver::EActiveDestinationSet, *dummyBinding ) );
    
    iConnHandler->iStreamArray[0].iConnArray[0].iActDestState = TConnectionData::EActDestSet;

    iConnHandler->iStreamArray[0].iConnArray[0].iPeerAddr.SetAddress( KInetAddrLinkLocalNet );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->STUNBindingEventOccurredL(
        MSTUNClientObserver::EActiveDestinationSet, *dummyBinding ) );
    
    iConnHandler->iStreamArray[0].iConnArray[0].iActDestState = TConnectionData::EActDestSet;
    
    iConnHandler->iStreamArray[0].iConnArray[0].iPeerAddr.SetAddress( KAFUnspec );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->STUNBindingEventOccurredL(
        MSTUNClientObserver::EActiveDestinationSet, *dummyBinding ) );
    
    iConnHandler->iStreamArray[0].iConnArray[0].iPeerAddr.SetAddress( KAFUnspec );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->STUNBindingEventOccurredL(
        MSTUNClientObserver::EActiveDestinationSet, *dummyBinding ) );
    
    iConnHandler->iStreamArray[0].iConnArray[0].iPeerAddr.SetAddress( KInetAddrLinkLocalNet );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->STUNBindingEventOccurredL(
        MSTUNClientObserver::EActiveDestinationSet, *dummyBinding ) );
    
    iAsyncTestCase = ETrue;
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->STUNBindingEventOccurredL(
        MSTUNClientObserver::ECredentialsRejected, *dummyBinding ) );
    StartActiveScheduler();
    }

void UT_CNATFWTurnConnectionHandler::UT_CNATFWTurnConnectionHandler_STUNBindingErrorOccurredL(  )
    {
    // Create STUN Client
    iConnHandler->TryNextServerL();
    CSTUNRelayBinding* dummyBinding = CSTUNRelayBinding::NewL( *iConnHandler->iStunClient, 222, 2 );
    CleanupStack::PushL( dummyBinding );
    CNATFWCandidate* candidate = CNATFWCandidate::NewL();
    CleanupStack::PushL( candidate );
    candidate->SetStreamId( 222 );
    
    iConnHandler->STUNBindingErrorOccurred( *dummyBinding, KErrNotFound );
    
    
    // Store stream and create connection to array
    TStreamData streamData;
    TConnectionData connData;
    streamData.iStreamId = 222;
    connData.iConnectionId = 2;
    connData.iTurnBinding = dummyBinding;
    connData.iLocalCandidate = candidate;
    
    streamData.iConnArray.AppendL( connData );
    CleanupClosePushL( streamData.iConnArray );
    iConnHandler->iStreamArray.AppendL( streamData );
    CleanupStack::Pop( &streamData.iConnArray );
    CleanupStack::Pop( candidate );
    CleanupStack::Pop( dummyBinding );
    
    // KNoBinding (437) cases:
    
    // First with candidate
    iAsyncTestCase = ETrue;
    TTurnPluginCallbackInfo* lastCallback = iConnHandler->iAsyncCallback->iPendingCallbacks.Last();
    iConnHandler->STUNBindingErrorOccurred( *dummyBinding, 437 );
    TTurnPluginCallbackInfo* lastCallback2 = iConnHandler->iAsyncCallback->iPendingCallbacks.Last();
    // Check whether callback wasn't created because of silent out-of-memory handling
    if ( lastCallback == lastCallback2 || lastCallback2 == NULL )
        {
        User::Leave( KErrNoMemory );
        }
    StartActiveScheduler();
    
    // Then without candidate
    
    // Store stream ID and create connection to array
    dummyBinding = CSTUNRelayBinding::NewL( *iConnHandler->iStunClient, 222, 2 );
    CleanupStack::PushL( dummyBinding );
    
    TStreamData streamData2;
    TConnectionData connData2;
    streamData2.iStreamId = 222;
    connData2.iConnectionId = 2;
    connData2.iTurnBinding = dummyBinding;
    
    streamData2.iConnArray.AppendL( connData2 );
    CleanupClosePushL( streamData2.iConnArray );
    iConnHandler->iStreamArray.AppendL( streamData2 );
    CleanupStack::Pop( &streamData2.iConnArray );
    CleanupStack::Pop( dummyBinding );
    
    iAsyncTestCase = ETrue;
    lastCallback = iConnHandler->iAsyncCallback->iPendingCallbacks.Last();
    iConnHandler->STUNBindingErrorOccurred( *dummyBinding, 437 );
    lastCallback2 = iConnHandler->iAsyncCallback->iPendingCallbacks.Last();
    // Check whether callback wasn't created because of silent out-of-memory handling
    if ( lastCallback == lastCallback2 || lastCallback2 == NULL )
        {
        User::Leave( KErrNoMemory );
        }
    StartActiveScheduler();
    
    // KServerTransitioning (439) cases:
    
    // Store stream ID and create connection to array
    dummyBinding = CSTUNRelayBinding::NewL( *iConnHandler->iStunClient, 222, 2 );
    CleanupStack::PushL( dummyBinding );
    candidate = CNATFWCandidate::NewL();
    CleanupStack::PushL( candidate );
    candidate->SetStreamId( 222 );
    
    TStreamData streamData3;
    TConnectionData connData3;
    streamData3.iStreamId = 222;
    connData3.iConnectionId = 2;
    connData3.iTurnBinding = dummyBinding;
    connData3.iLocalCandidate = candidate;
    streamData3.iConnArray.AppendL( connData3 );
    CleanupClosePushL( streamData3.iConnArray );
    iConnHandler->iStreamArray.AppendL( streamData3 );
    CleanupStack::Pop( &streamData3.iConnArray );
    CleanupStack::Pop( candidate );
    CleanupStack::Pop( dummyBinding );
    
    
    iConnHandler->iStreamArray[0].iConnArray[0].iActDestState = TConnectionData::EActDestNoneSet;
    iConnHandler->STUNBindingErrorOccurred( *dummyBinding, 439 );
    
    iConnHandler->iStreamArray[0].iConnArray[0].iActDestState = TConnectionData::EActDestSet;
    iConnHandler->STUNBindingErrorOccurred( *dummyBinding, 439 );
    
    iConnHandler->iStreamArray[0].iConnArray[0].iActDestState = TConnectionData::EActDestTransitioning;
    iConnHandler->STUNBindingErrorOccurred( *dummyBinding, 439 );
    
    delete iConnHandler->iStreamArray[0].iConnArray[0].iLocalCandidate;
    iConnHandler->iStreamArray[0].iConnArray[0].iLocalCandidate = NULL;
    
    iConnHandler->iStreamArray[0].iConnArray[0].iActDestState = TConnectionData::EActDestNoneSet;
    iConnHandler->STUNBindingErrorOccurred( *dummyBinding, 439 );
    
    
    // Alternate server case:
    
    // Store stream ID and create connection to array
    dummyBinding = CSTUNRelayBinding::NewL( *iConnHandler->iStunClient, 222, 2 );
    CleanupStack::PushL( dummyBinding );
    
    TStreamData streamData4;
    TConnectionData connData4;
    streamData4.iStreamId = 222;
    connData4.iConnectionId = 2;
    connData4.iTurnBinding = dummyBinding;
    streamData4.iConnArray.AppendL( connData4 );
    CleanupClosePushL( streamData4.iConnArray );
    iConnHandler->iStreamArray.AppendL( streamData4 );
    CleanupStack::Pop( &streamData4.iConnArray );
    CleanupStack::Pop( dummyBinding );
    
    iConnHandler->STUNBindingErrorOccurred( *dummyBinding, 300 );
    
    // Other error cases:
    
    dummyBinding = CSTUNRelayBinding::NewL( *iConnHandler->iStunClient, 222, 2 );
    iConnHandler->iStreamArray[0].iConnArray[0].iTurnBinding = dummyBinding;
    iAsyncTestCase = ETrue;
    iConnHandler->STUNBindingErrorOccurred( *dummyBinding, KErrNotFound );
    StartActiveScheduler();
    
    // Error after candidate fetching is completed
    dummyBinding = CSTUNRelayBinding::NewL( *iConnHandler->iStunClient, iStreamId, 2 );
    CleanupStack::PushL( dummyBinding );
    candidate = CNATFWCandidate::NewL();
    CleanupStack::PushL( candidate );
    candidate->SetStreamId( 222 );
    
    TStreamData streamData5;
    TConnectionData connData5;
    streamData5.iStreamId = 222;
    connData5.iConnectionId = 2;
    connData5.iTurnBinding = dummyBinding;
    connData5.iLocalCandidate = candidate;
    streamData5.iConnArray.AppendL( connData5 );
    CleanupClosePushL( streamData5.iConnArray );
    iConnHandler->iStreamArray.AppendL( streamData5 );
    CleanupStack::Pop( &streamData5.iConnArray );
    CleanupStack::Pop( candidate );
    CleanupStack::Pop( dummyBinding );
    
    iConnHandler->STUNBindingErrorOccurred( *dummyBinding, KErrNotFound );
    }

void UT_CNATFWTurnConnectionHandler::UT_CNATFWTurnConnectionHandler_IncomingMessageLL(  )
    {
    TInetAddr dummyAddr;
    const TInetAddr dummyAddr2;
    HBufC8* dummyMessage = NULL;
    TBool consumed( EFalse );
    
    // Create STUN Client
    iConnHandler->TryNextServerL();
    
    // Create connections to array
    iConnHandler->FetchCandidateL( iStreamId, 0, KAFUnspec, KAFUnspec );
    CSTUNRelayBinding* dummyBinding = CSTUNRelayBinding::NewL( *iConnHandler->iStunClient, iStreamId, 2 );
    iConnHandler->iStreamArray[0].iConnArray[0].iTurnBinding = dummyBinding;
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
/*    
    iConnHandler->iStreamArray[0].iConnArray[0].iAwaitingDataFromSrv = ETrue;
    
    TNATFWUNSAFTransactionID transactionID;
    CNATFWUNSAFMessage* msg = CNATFWUNSAFAllocateRequest::NewL( transactionID );
    CleanupStack::PushL( msg );    
    CBufBase* temp = msg->EncodeL();
    TInt length = temp->Size();
    HBufC8* message = HBufC8::NewLC( temp->Size() );
    TPtr8 writable( message->Des() );
    temp->Read( 0, writable, length  );
    
    delete temp;
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->IncomingMessageL( 222,
                                                           *message,
                                                           dummyAddr2,
                                                           dummyAddr2,
                                                           dummyAddr,
                                                           consumed ) );
    CleanupStack::PopAndDestroy( message );
    CleanupStack::PopAndDestroy( msg );*/
    }

void UT_CNATFWTurnConnectionHandler::UT_CNATFWTurnConnectionHandler_OutgoingMessageLL(  )
    {
    const TInetAddr dummyAddr;
    _LIT8( KTestMessage, "testmesg" );
    TBool consumed( EFalse );
    
    // Create STUN Client
    iConnHandler->TryNextServerL();
    
    TNATFWUNSAFTransactionID transactionID;
    HBufC8* message = NULL;
    CNATFWUNSAFMessage* msg = NULL;
    CBufBase* temp = NULL;
    TInt length;
    
    msg = CNATFWUNSAFAllocateRequest::NewL( transactionID );
    CleanupStack::PushL( msg );    
    temp = msg->EncodeL();
    CleanupStack::PushL( temp );
    length = temp->Size();
    message = HBufC8::NewL( length );
    TPtr8 writable( message->Des() );
    temp->Read( 0, writable, length  );
    
    CleanupStack::PopAndDestroy( temp );
    CleanupStack::PopAndDestroy( msg );
    CleanupStack::PushL( message );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->OutgoingMessageL( 222,
                           2,
                           dummyAddr,
                           *message,
                           consumed ) );
    CleanupStack::PopAndDestroy( message );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->OutgoingMessageL( 222,
                           2,
                           dummyAddr,
                           KTestMessage,
                           consumed ) );
    
    // Create a connection to array
    iConnHandler->FetchCandidateL( iStreamId, 0, KAFUnspec, KAFUnspec );
    CSTUNRelayBinding* dummyBinding = CSTUNRelayBinding::NewL( *iConnHandler->iStunClient, iStreamId, 2 );
    iConnHandler->iStreamArray[0].iConnArray[0].iTurnBinding = dummyBinding;
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->OutgoingMessageL( 222, 2, dummyAddr, KTestMessage, consumed ) );
    
    iConnHandler->iStreamArray[0].iConnArray[0].iActDestState = TConnectionData::EActDestTransitioning;
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->OutgoingMessageL( 222, 2, dummyAddr, KTestMessage, consumed ) );
    
    iConnHandler->iStreamArray[0].iConnArray[0].iActDestState = TConnectionData::EActDestSet;
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->OutgoingMessageL( 222, 2, dummyAddr, KTestMessage, consumed ) );
    
    iConnHandler->iStreamArray[0].iTransportProtocol = KProtocolInetTcp;

    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->OutgoingMessageL( 222, 2, dummyAddr, KTestMessage, consumed ) );
    
    TInetAddr peerAddress;
    peerAddress.SetAddress( KInetAddrLoop );
    
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->OutgoingMessageL( 222, 2, peerAddress, KTestMessage, consumed ) );
    }

void UT_CNATFWTurnConnectionHandler::UT_CNATFWTurnConnectionHandler_IncomingMessageNotifyL(  )
    {
    TUint streamId( 222 );
    const TInetAddr dummyLocalAddr;
    const TInetAddr dummyFromAddr;
    TInetAddr dummyPeerAddr;
    TBuf8<20> dummyData;
    dummyData.Copy( _L8( "djjksaosdosajkojsdks" ) );
    
    TNATFWUNSAFTransactionID transactionID;
    HBufC8* message = NULL;
    CNATFWUNSAFMessage* msg = NULL;
    CBufBase* temp = NULL;
    TInt length;
    HBufC8* modifiedMsg( NULL );
    
    // Create STUN Client
    iConnHandler->TryNextServerL();
    // Create connection to array
    iConnHandler->FetchCandidateL( iStreamId, 0, KAFUnspec, KAFUnspec );
    
    modifiedMsg = iConnHandler->IncomingMessageNotify( streamId, dummyData, dummyLocalAddr, dummyFromAddr, dummyPeerAddr );
    
    iConnHandler->iStreamArray[0].iConnArray[0].iActDestState = TConnectionData::EActDestSet;
    
    modifiedMsg = iConnHandler->IncomingMessageNotify( streamId, dummyData, dummyLocalAddr, dummyFromAddr, dummyPeerAddr );
    
    msg = CNATFWUNSAFDataIndication::NewL( transactionID );
    CleanupStack::PushL( msg );    
    msg->AddAttributeL( CNATFWUNSAFDataAttribute::NewLC( dummyData ) );
    CleanupStack::Pop(); // CNATFWUNSAFDataAttribute
    
    temp = msg->EncodeL();
    CleanupStack::PushL( temp );
    length = temp->Size();
    message = HBufC8::NewL( length );
    TPtr8 writable( message->Des() );
    temp->Read( 0, writable, length  );
    
    CleanupStack::PopAndDestroy( temp );
    CleanupStack::PopAndDestroy( msg );
    CleanupStack::PushL( message );
    
    modifiedMsg = iConnHandler->IncomingMessageNotify( streamId, *message, dummyLocalAddr, dummyFromAddr, dummyPeerAddr );
    
    if ( !modifiedMsg )
        {
        User::Leave( KErrNoMemory );
        }
    
    delete modifiedMsg;
    CleanupStack::PopAndDestroy( message );
    
    msg = CNATFWUNSAFDataIndication::NewL( transactionID );
    CleanupStack::PushL( msg );
    
    msg->AddAttributeL( CNATFWUNSAFDataAttribute::NewLC( dummyData ) );
    CleanupStack::Pop(); // CNATFWUNSAFDataAttribute
    
    msg->AddAttributeL( CNATFWUNSAFRemoteAddressAttribute::NewLC( dummyPeerAddr ) );
    CleanupStack::Pop(); // CNATFWUNSAFRemoteAddressAttribute
    
    temp = msg->EncodeL();
    CleanupStack::PushL( temp );
    length = temp->Size();
    message = HBufC8::NewL( length );
    TPtr8 writable2( message->Des() );
    temp->Read( 0, writable2, length  );
    
    CleanupStack::PopAndDestroy( temp );
    CleanupStack::PopAndDestroy( msg );
    CleanupStack::PushL( message );
    
    modifiedMsg = iConnHandler->IncomingMessageNotify( streamId, *message, dummyLocalAddr, dummyFromAddr, dummyPeerAddr );
    
    if ( !modifiedMsg )
        {
        User::Leave( KErrNoMemory );
        }
    
    delete modifiedMsg;
    CleanupStack::PopAndDestroy( message );
    
    
    msg = CNATFWUNSAFBindingResponse::NewL( transactionID );
    CleanupStack::PushL( msg );
    
    temp = msg->EncodeL();
    CleanupStack::PushL( temp );
    length = temp->Size();
    message = HBufC8::NewL( length );
    TPtr8 writable3( message->Des() );
    temp->Read( 0, writable3, length  );
    
    CleanupStack::PopAndDestroy( temp );
    CleanupStack::PopAndDestroy( msg );
    CleanupStack::PushL( message );
    
    modifiedMsg = iConnHandler->IncomingMessageNotify( streamId, *message, dummyLocalAddr, dummyFromAddr, dummyPeerAddr );
    
    if ( modifiedMsg )
        {
        User::Leave( KErrNoMemory );
        }
    
    CleanupStack::PopAndDestroy( message );
    }

void UT_CNATFWTurnConnectionHandler::UT_CNATFWTurnConnectionHandler_OutgoingMessageNotifyL(  )
    {
    // Only for coverage
    HBufC8* dummyMsg = NULL;
    iConnHandler->OutgoingMessageNotify( 0, 0, KAFUnspec, *dummyMsg );
    }

void UT_CNATFWTurnConnectionHandler::UT_CNATFWTurnConnectionHandler_ConnectionNotifyL(  )
    {
    iConnHandler->TryNextServerL();
    iConnHandler->FetchCandidateL( iStreamId, 0, KAFUnspec, KAFUnspec );
    
    iConnHandler->ConnectionNotify( 333,
                                    3,
                                    MNcmConnectionObserver::ESendingActivated,
                                    KErrNone );
    
    // Events during candidate fetching
    iConnHandler->ConnectionNotify( iStreamId,
                                    1,
                                    MNcmConnectionObserver::ESendingActivated,
                                    KErrNone );

    iConnHandler->ConnectionNotify( iStreamId,
                                    1,
                                    MNcmConnectionObserver::ESendingActivated,
                                    KErrNone );
    iConnHandler->ConnectionNotify( iStreamId,
                                    1,
                                    MNcmConnectionObserver::EReceivingActivated,
                                    KErrNone );
    iConnHandler->ConnectionNotify( iStreamId,
                                    1,
                                    MNcmConnectionObserver::ESendingDeactivated,
                                    KErrNone );
    iConnHandler->ConnectionNotify( iStreamId,
                                    1,
                                    MNcmConnectionObserver::EReceivingDeactivated,
                                    KErrNone );

    // Events after candidate has been fetched
    CNATFWCandidate* candidate = CNATFWCandidate::NewL();
    candidate->SetStreamId( 222 );
    iConnHandler->iStreamArray[0].iConnArray[0].iLocalCandidate = candidate;
    
    
    iConnHandler->ConnectionNotify( iStreamId,
                                    1,
                                    MNcmConnectionObserver::ESendingActivated,
                                    KErrNone );

    iConnHandler->ConnectionNotify( iStreamId,
                                    1,
                                    MNcmConnectionObserver::EReceivingActivated,
                                    KErrNone );
    iConnHandler->ConnectionNotify( iStreamId,
                                    1,
                                    MNcmConnectionObserver::ESendingDeactivated,
                                    KErrNone );
    iConnHandler->ConnectionNotify( iStreamId,
                                    1,
                                    MNcmConnectionObserver::EReceivingDeactivated,
                                    KErrNone );
    
    iConnHandler->ConnectionNotify( iStreamId,
                                    1,
                                    MNcmConnectionObserver::EFirstMediaPacketSent,
                                    KErrNone );
    
    iConnHandler->ConnectionNotify( iStreamId,
                                    1,
                                    MNcmConnectionObserver::EConnectionRemoved,
                                    KErrNone );
    
    // Error events
    iConnHandler->FetchCandidateL( iStreamId, 0, KAFUnspec, KAFUnspec );
    
    iConnHandler->ConnectionNotify( iStreamId,
                                    1,
                                    MNcmConnectionObserver::EConnectionError,
                                    KErrCouldNotConnect );
    
    iConnHandler->FetchCandidateL( iStreamId, 0, KAFUnspec, KAFUnspec );
    iConnHandler->iStreamArray[0].iConnArray[0].iLocalCandidate = candidate;
    
    iConnHandler->ConnectionNotify( iStreamId,
                                    1,
                                    MNcmConnectionObserver::EConnectionError,
                                    KErrCouldNotConnect );
    }

void UT_CNATFWTurnConnectionHandler::UT_CNATFWTurnConnectionHandler_BindingRefreshLL(  )
    {
    CSTUNClient* dummyClient = NULL;
    CSTUNRelayBinding* dummyBinding = CSTUNRelayBinding::NewL( *dummyClient, 222, 1 );
    CleanupStack::PushL( dummyBinding );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->BindingRefreshL() );
    
    TStreamData streamData;
    streamData.iStreamId = iStreamId;
    TConnectionData connData;
    connData.iConnectionId = 1;
    connData.iTurnBinding = dummyBinding;
    streamData.iConnArray.AppendL( connData );
    CleanupClosePushL( streamData.iConnArray );
    iConnHandler->iStreamArray.AppendL( streamData );
    CleanupStack::Pop( &streamData.iConnArray );
    CleanupStack::Pop( dummyBinding );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->BindingRefreshL() );
    }

void UT_CNATFWTurnConnectionHandler::UT_CNATFWTurnConnectionHandler_TimerTriggeredLL(  )
    {
    CSTUNClient* dummyClient = NULL;
    CSTUNRelayBinding* dummyBinding = CSTUNRelayBinding::NewL( *dummyClient, iStreamId, 1 );
    CleanupStack::PushL( dummyBinding );
    
    // Create STUN Client
    iConnHandler->TryNextServerL();
    // Create connection to array
    iConnHandler->FetchCandidateL( iStreamId, 0, KAFUnspec, KAFUnspec );
    iConnHandler->iStreamArray[0].iConnArray[0].iTurnBinding = dummyBinding;
    CleanupStack::Pop( dummyBinding );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->TimerTriggeredL( 333, 1 ) );
    
    // EActDestNoneSet cases:
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->TimerTriggeredL( iStreamId, 1 ) );
    
    iConnHandler->iStreamArray[0].iConnArray[0].iActDestAlreadySetOnServer = ETrue;
    iConnHandler->iStreamArray[0].iConnArray[0].iActDestState = TConnectionData::EActDestNoneSet;
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->TimerTriggeredL( iStreamId, 1 ) );
    iConnHandler->iStreamArray[0].iConnArray[0].iActDestAlreadySetOnServer = EFalse;
    
    // EActDestTransitioning cases:
    
    iConnHandler->iStreamArray[0].iConnArray[0].iPeerAddr = KAFUnspec;
    iConnHandler->iStreamArray[0].iConnArray[0].iActDestState = TConnectionData::EActDestTransitioning;
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->TimerTriggeredL( iStreamId, 1 ) );
    
    iConnHandler->iStreamArray[0].iConnArray[0].iActDestState = TConnectionData::EActDestTransitioning;
    iConnHandler->iStreamArray[0].iConnArray[0].iPeerAddr.SetAddress( KInetAddrLoop );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->TimerTriggeredL( iStreamId, 1 ) );
    
    // EActDestNoneSet cases:
    
    iConnHandler->iStreamArray[0].iConnArray[0].iActDestState = TConnectionData::EActDestSet;
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->TimerTriggeredL( iStreamId, 1 ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iConnHandler->TimerTriggeredL( iStreamId, 1 ) );
    }
    
void UT_CNATFWTurnConnectionHandler::UT_CNATFWTurnConnectionHandler_DeleteStreamL(  )
    {
    // create STUN Client
    iConnHandler->TryNextServerL();
    // Store stream ID and create connections to array
    iConnHandler->FetchCandidateL( iStreamId, 0, KAFUnspec, KAFUnspec );
    iConnHandler->FetchCandidateL( iStreamId, 0, KAFUnspec, KAFUnspec );
    CSTUNRelayBinding* dummyBinding = CSTUNRelayBinding::NewL( *iConnHandler->iStunClient, iStreamId, 2 );
    iConnHandler->iStreamArray[0].iConnArray[1].iTurnBinding = dummyBinding;
    
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

void UT_CNATFWTurnConnectionHandler::StartActiveScheduler()
    {
    //iActiveSchedulerStarted = ETrue;
    //CActiveScheduler::Start();
    StopActiveScheduler();
    UT_CNATFWTurnConnectionHandler::iActiveSchedulerWait.Start();
    }

void UT_CNATFWTurnConnectionHandler::StopActiveScheduler()
    {
    /*
    if ( iActiveSchedulerStarted )
        {
        iActiveSchedulerStarted = EFalse;
        CActiveScheduler::Stop();
        }
        */
    if ( UT_CNATFWTurnConnectionHandler::iActiveSchedulerWait.IsStarted() )
        {
        UT_CNATFWTurnConnectionHandler::iActiveSchedulerWait.AsyncStop();
        }
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWTurnConnectionHandler,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "ConnectServerL - test ",
    "CNATFWTurnConnectionHandler",
    "ConnectServerL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWTurnConnectionHandler_ConnectServerLL, Teardown)

EUNIT_TEST(
    "TryNextServerL - test ",
    "CNATFWTurnConnectionHandler",
    "TryNextServerL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWTurnConnectionHandler_TryNextServerLL, Teardown)

EUNIT_TEST(
    "ConnectionById - test ",
    "CNATFWTurnConnectionHandler",
    "ConnectionById",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWTurnConnectionHandler_ConnectionByIdL, Teardown)
     
EUNIT_TEST(
    "FetchCandidateL - test ",
    "CNATFWTurnConnectionHandler",
    "FetchCandidateLL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWTurnConnectionHandler_FetchCandidateLL, Teardown)

EUNIT_TEST(
    "StartTurnRefresh - test ",
    "CNATFWTurnConnectionHandler",
    "StartTurnRefresh",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWTurnConnectionHandler_StartTurnRefreshL, Teardown)

EUNIT_TEST(
    "CreateTURNBindingL - test ",
    "CNATFWTurnConnectionHandler",
    "CreateTURNBindingL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWTurnConnectionHandler_CreateTURNBindingLL, Teardown)
    
EUNIT_TEST(
    "SetReceivingStateL - test ",
    "CNATFWTurnConnectionHandler",
    "SetReceivingStateL",
    "FUNCTIONALITY",    
    SetupL, UT_CNATFWTurnConnectionHandler_SetReceivingStateLL, Teardown)
    
EUNIT_TEST(
    "SetSendingStateL - test ",
    "CNATFWTurnConnectionHandler",
    "SetSendingStateL",
    "FUNCTIONALITY",    
    SetupL, UT_CNATFWTurnConnectionHandler_SetSendingStateLL, Teardown)
    
EUNIT_TEST(
    "GetConnectionIdL - test ",
    "CNATFWTurnConnectionHandler",
    "GetConnectionIdL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWTurnConnectionHandler_GetConnectionIdLL, Teardown)
    
EUNIT_TEST(
    "IsRequestOrIndicationL - test ",
    "CNATFWTurnConnectionHandler",
    "IsRequestOrIndicationL",
    "FUNCTIONALITY",    
    SetupL, UT_CNATFWTurnConnectionHandler_IsRequestOrIndicationLL, Teardown)

EUNIT_TEST(
    "IsTurnResponseL - test ",
    "CNATFWTurnConnectionHandler",
    "IsTurnResponseL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWTurnConnectionHandler_IsTurnResponseLL, Teardown)

EUNIT_TEST(
    "STUNClientInitCompleted - test ",
    "CNATFWTurnConnectionHandler",
    "STUNClientInitCompleted",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWTurnConnectionHandler_STUNClientInitCompletedL, Teardown)

EUNIT_TEST(
    "STUNBindingEventOccurredL - test ",
    "CNATFWTurnConnectionHandler",
    "STUNBindingEventOccurredL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWTurnConnectionHandler_STUNBindingEventOccurredLL, Teardown)

EUNIT_TEST(
    "STUNBindingErrorOccurred - test ",
    "CNATFWTurnConnectionHandler",
    "STUNBindingErrorOccurred",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWTurnConnectionHandler_STUNBindingErrorOccurredL, Teardown)

EUNIT_TEST(
    "IncomingMessageL - test ",
    "CNATFWTurnConnectionHandler",
    "IncomingMessageL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWTurnConnectionHandler_IncomingMessageLL, Teardown)  

EUNIT_TEST(
    "OutgoingMessageL - test ",
    "CNATFWTurnConnectionHandler",
    "OutgoingMessageL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWTurnConnectionHandler_OutgoingMessageLL, Teardown )
    
EUNIT_TEST(
    "IncomingMessageNotify - test ",
    "CNATFWTurnConnectionHandler",
    "IncomingMessageNotify",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWTurnConnectionHandler_IncomingMessageNotifyL, Teardown)

EUNIT_TEST(
    "OutgoingMessageNotify - test ",
    "CNATFWTurnConnectionHandler",
    "OutgoingMessageNotify",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWTurnConnectionHandler_OutgoingMessageNotifyL, Teardown)

EUNIT_TEST(
    "ConnectionNotify - test ",
    "CNATFWTurnConnectionHandler",
    "ConnectionNotify",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWTurnConnectionHandler_ConnectionNotifyL, Teardown)

EUNIT_TEST(
    "BindingRefreshL - test ",
    "CNATFWTurnConnectionHandler",
    "BindingRefreshL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWTurnConnectionHandler_BindingRefreshLL, Teardown)
    
EUNIT_TEST(
    "TimerTriggeredL - test ",
    "CNATFWTurnConnectionHandler",
    "TimerTriggeredL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWTurnConnectionHandler_TimerTriggeredLL, Teardown)

EUNIT_TEST(
    "DeleteStream - test ",
    "CNATFWTurnConnectionHandler",
    "DeleteStream",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWTurnConnectionHandler_DeleteStreamL, Teardown)

// ALLOC TESTS

#ifdef ALLOC_TEST_ON

EUNIT_ALLOC_TEST(
    "FetchCandidateL - alloc test ",
    "CNATFWTurnConnectionHandler",
    "FetchCandidateL",
    "ERRORHANDLING",
    SetupAL, UT_CNATFWTurnConnectionHandler_FetchCandidateLL, Teardown)
/*
EUNIT_ALLOC_TEST(
    "CreateTURNBindingL - alloc test ",
    "CNATFWTurnConnectionHandler",
    "CreateTURNBindingL",
    "ERRORHANDLING",
    SetupAL, UT_CNATFWTurnConnectionHandler_CreateTURNBindingLL, Teardown)*/

#endif // ALLOC_TEST

EUNIT_END_TEST_TABLE

//  END OF FILE
