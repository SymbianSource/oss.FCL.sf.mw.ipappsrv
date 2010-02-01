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
#include "ut_cmcestateserverestablishing.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>

//  INTERNAL INCLUDES
#include "MCETestHelper.h"

#include "mcesipsession.h"
#include "mcesipstatemachine.h"
#include "mceservercore.h"
#include "mcecssession.h"
#include "mcecssessionimplementation.h"
#include "mcesipconnection.h"
#include "mcecomsession.h"
#include "mcestateserverestablishing.h"
#include "mcemediamanager.h"

const TUint32 KDialogId = 12345;

#define MCE_ENTRYL_POSTCONDITION \
MCE_CHECK_MEMORY_LEAVE( iStorage->iMediaManagerAction == CMCETls::ECloseSession )

#define MCE_ENTRYL_POSTCONDITION_2( responseCode ) \
MCE_CHECK_MEMORY_LEAVE( !iStorage->iSipSentResponse || iStorage->iSipSentResponse->StatusCode() != responseCode )
// CONSTRUCTION
UT_CMceStateServerEstablishing* UT_CMceStateServerEstablishing::NewL()
    {
    UT_CMceStateServerEstablishing* self = UT_CMceStateServerEstablishing::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceStateServerEstablishing* UT_CMceStateServerEstablishing::NewLC()
    {
    UT_CMceStateServerEstablishing* self = new( ELeave ) UT_CMceStateServerEstablishing();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }


// Destructor (virtual by CBase)
UT_CMceStateServerEstablishing::~UT_CMceStateServerEstablishing()
    {
    }

// Default constructor
UT_CMceStateServerEstablishing::UT_CMceStateServerEstablishing()
    {
    }

// Second phase construct
void UT_CMceStateServerEstablishing::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceStateServerEstablishing::SetupL(  )
    {
    CMCETls::OpenL();
    SdpCodecStringPool::OpenL();
    //TMceIds ids;
    iIds.iProfileID = 1;
    iIds.iManagerType = KMceCSSIPSession;
    iIds.iDialogType = KMceDlgTypeInvite;
    iServerCore = CMceServerCore::NewL();
    
    iStorage = CMCETls::Storage();
    
	CDesC8ArrayFlat* params = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
	CleanupStack::PushL( params );
	params->AppendL( _L8("User <user2@host>") );
	params->AppendL( _L8("User <user@host>") );
    
    CMceMsgTextArray* msgParams = new (ELeave) CMceMsgTextArray( *params );
    CleanupStack::PushL( msgParams );
    iStorage->StoreL( iIds, *msgParams );
    CleanupStack::PopAndDestroy( msgParams );
    CleanupStack::PopAndDestroy( params );
    
    iSession = CMceCsSession::NewL(*iServerCore);
    iSession->CreateSubSessionL(iIds, RMessage2());
    iSipSession =  static_cast<CMceSipSession*>((iSession->Implementation().SubSessions())[0]);
    
    // incoming request
    CSIPServerTransaction* invite = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInvite );
    CleanupStack::PushL( invite );
    iSipSession->iPendingReceivedRequests.AppendL( invite );
    CleanupStack::Pop( invite );
    
    iSipSession->NextState( KMceStateServerEstablishing );
    
    iState = static_cast<CMceStateServerEstablishing*>(&iSipSession->CurrentState());
    }

void UT_CMceStateServerEstablishing::Teardown()
    {
    CMCETls::Close();
	delete iSession;
    delete iServerCore;
    SdpCodecStringPool::Close();
    }


void UT_CMceStateServerEstablishing::UT_CMceStateServerEstablishing_AcceptLL()
    {
    TMceIds ids;
    CMceMsgBase* msg = NULL;

    TMceStateTransitionEvent event1( *iSipSession, EMceItcUpdate, ids, *msg );
    EUNIT_ASSERT ( iState->AcceptL( event1 ) );

    TMceStateTransitionEvent event2( *iSipSession, EMceItcRejectSession, ids, *msg  );
    EUNIT_ASSERT ( iState->AcceptL( event2 ) );
 
    TMceStateTransitionEvent event3( *iSipSession, EMceMediaSessionStopped );
    TRAPD( e1, iState->AcceptL( event3 ) );
    EUNIT_ASSERT ( e1 == KErrTotalLossOfPrecision );

    TMceStateTransitionEvent event4( *iSipSession, EMceResponse );
    TRAPD( e2, iState->AcceptL( event4 ) );
    EUNIT_ASSERT ( e2 == KErrTotalLossOfPrecision );
    
    }

void UT_CMceStateServerEstablishing::UT_CMceStateServerEstablishing_EntryLL()
    {
    
    CSIPServerTransaction* invite = 
            MCETestHelper::ServerTransactionL( SipStrConsts::EInvite );
    CleanupStack::PushL( invite );
    iSipSession->iPendingReceivedRequests.AppendL( invite );
    CleanupStack::Pop( invite );
    
    TMceIds ids;
    CMceMsgBase* msg = NULL;
    
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EInSession );
    iSipSession->iBody->iID = 1;
    iSipSession->iNewBodyCandidate = CMceComSession::NewL( CMceComSession::EInSession );
    iSipSession->iNewBodyCandidate->iID = 1;
    
    ids.iAppUID = 10;
    ids.iSessionID = 1;
    
    CMceComSession* clientSession = CMceComSession::NewL( CMceComSession::EInSession );
    clientSession->iID = 1;
    ids.iSessionID = clientSession->iID;
    CleanupStack::PushL( clientSession );
    CMceMsgObject<CMceComSession>* clientSessionMsg = 
        new (ELeave) CMceMsgObject<CMceComSession>( *clientSession, EMceItcMsgTypeSession );
    CleanupStack::Pop( clientSession );
    clientSessionMsg->PushL();
    CleanupStack::PushL( clientSessionMsg );
    

    iSipSession->iSubState = CMceSipSession::EAnswering;
    MCE_RESET_STUBS();
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    delete iStorage->iSipSentResponse;
    iStorage->iSipSentResponse = NULL;
    
    TMceStateTransitionEvent event1( *iSipSession, EMceItcUpdate, ids, 
                                      *clientSessionMsg );

    iState->EntryL( event1 );
    MCE_ENTRYL_POSTCONDITION
    
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    
    EUNIT_ASSERT ( event1.Code() == EMceItcUpdate );
    EUNIT_ASSERT ( event1.ParamStatus() == KMceAsync );
    MCE_RESET_STUBS();
    CleanupStack::PopAndDestroy( clientSessionMsg );

// ---

    clientSession = CMceComSession::NewL( CMceComSession::EInSession );
    clientSession->iID = 1;
    ids.iSessionID = clientSession->iID;
    CleanupStack::PushL( clientSession );
    clientSessionMsg = 
        new (ELeave) CMceMsgObject<CMceComSession>( *clientSession, EMceItcMsgTypeSession );
    CleanupStack::Pop( clientSession );
    clientSessionMsg->PushL();
    CleanupStack::PushL( clientSessionMsg );


    iSipSession->iSubState = CMceSipSession::EAnswering;
    iStorage->iMediaManagerUpdateStatus = KErrGeneral;
    delete iStorage->iSipSentResponse;
    iStorage->iSipSentResponse = NULL;
    
    TMceStateTransitionEvent event2( *iSipSession, EMceItcUpdate, ids, 
                                      *clientSessionMsg );

    iState->EntryL( event2 );
    MCE_ENTRYL_POSTCONDITION_2( KMceSipServerInternalError )
    
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipServerInternalError /*sentResponse*/);
    
    EUNIT_ASSERT ( event2.Code() == EMceItcRejectSession );
    EUNIT_ASSERT ( event2.ParamStatus() == KErrGeneral );
    MCE_RESET_STUBS();
    CleanupStack::PopAndDestroy( clientSessionMsg );

// ---

    clientSession = CMceComSession::NewL( CMceComSession::EInSession );
    clientSession->iID = 1000;//wrong
    ids.iSessionID = clientSession->iID;
    CleanupStack::PushL( clientSession );
    clientSessionMsg = 
        new (ELeave) CMceMsgObject<CMceComSession>( *clientSession, EMceItcMsgTypeSession );
    CleanupStack::Pop( clientSession );
    clientSessionMsg->PushL();
    CleanupStack::PushL( clientSessionMsg );


    iSipSession->iSubState = CMceSipSession::EAnswering;
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    delete iStorage->iSipSentResponse;
    iStorage->iSipSentResponse = NULL;
    
    TMceStateTransitionEvent event2_1( *iSipSession, EMceItcUpdate, ids, 
                                      *clientSessionMsg );

    MCE_EUNIT_ASSERT_LEAVE( iState->EntryL( event2_1 ) );
    
    MCE_RESET_STUBS();
    CleanupStack::PopAndDestroy( clientSessionMsg );


// ---

    clientSession = CMceComSession::NewL( CMceComSession::EInSession );
    clientSession->iID = 1;
    ids.iSessionID = clientSession->iID;
    CleanupStack::PushL( clientSession );
    clientSessionMsg = 
        new (ELeave) CMceMsgObject<CMceComSession>( *clientSession, EMceItcMsgTypeSession );
    CleanupStack::Pop( clientSession );
    clientSessionMsg->PushL();
    CleanupStack::PushL( clientSessionMsg );

    iSipSession->iSubState = CMceSipSession::EUpdating;
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    delete iStorage->iSipSentResponse;
    iStorage->iSipSentResponse = NULL;
    
    TMceStateTransitionEvent event3( *iSipSession, EMceItcUpdate, ids, 
                                      *clientSessionMsg );

    iState->EntryL( event3 );
    MCE_ENTRYL_POSTCONDITION
    
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    
    EUNIT_ASSERT ( iStorage->iSipSentResponse == NULL );
    EUNIT_ASSERT ( event3.Code() == EMceItcUpdate );
    EUNIT_ASSERT ( event3.ParamStatus() == KMceAsync );
    MCE_RESET_STUBS();
    CleanupStack::PopAndDestroy( clientSessionMsg );

// ---
    // EMceItcUpdate, update ready, directly 200
    clientSession = CMceComSession::NewL( CMceComSession::EInSession );
    clientSession->iID = 1;
    ids.iSessionID = clientSession->iID;
    CleanupStack::PushL( clientSession );
    clientSessionMsg = 
        new (ELeave) CMceMsgObject<CMceComSession>( *clientSession, EMceItcMsgTypeSession );
    CleanupStack::Pop( clientSession );
    clientSessionMsg->PushL();
    CleanupStack::PushL( clientSessionMsg );

    iSipSession->iSubState = CMceSipSession::EUpdating;
    iStorage->iMediaManagerUpdateStatus = KMceReady;
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    delete iStorage->iSipSentResponse;
    iStorage->iSipSentResponse = NULL;
    
    TMceStateTransitionEvent event4( *iSipSession, EMceItcUpdate, ids, 
                                      *clientSessionMsg );

    iState->EntryL( event4 );
    MCE_ENTRYL_POSTCONDITION_2( KMceSipOK )
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipOK /*sentResponse*/);
    
    EUNIT_ASSERT ( event4.Code() == EMceItcUpdate );
    EUNIT_ASSERT ( event4.ParamStatus() == KMceReady );
    EUNIT_ASSERT ( iSipSession->BodyCandidate() == NULL );
    MCE_RESET_STUBS();
    CleanupStack::PopAndDestroy( clientSessionMsg );

    iSipSession->iNewBodyCandidate = CMceComSession::NewL( CMceComSession::EInSession );
    iSipSession->iNewBodyCandidate->iID = 1;

// ---
    // EMceItcUpdate, update ready, need 183
    clientSession = CMceComSession::NewL( CMceComSession::EInSession );
    clientSession->iID = 1;
    ids.iSessionID = clientSession->iID;
    CleanupStack::PushL( clientSession );
    clientSessionMsg = 
        new (ELeave) CMceMsgObject<CMceComSession>( *clientSession, EMceItcMsgTypeSession );
    CleanupStack::Pop( clientSession );
    clientSessionMsg->PushL();
    CleanupStack::PushL( clientSessionMsg );

    iSipSession->iSubState = CMceSipSession::EUpdating;
    iStorage->iMediaManagerUpdateStatus = KMceReady;
    iStorage->iMediaManagerNeedToNegotiate = ETrue;
    delete iStorage->iSipSentResponse;
    iStorage->iSipSentResponse = NULL;
    iSipSession->Extensions().SetRemote( CMceSipExtensions::E100rel, CMceSipExtensions::ESupported );
    TMceStateTransitionEvent event4_2( *iSipSession, EMceItcUpdate, ids, 
                                      *clientSessionMsg );

    iState->EntryL( event4_2 );
    MCE_ENTRYL_POSTCONDITION_2( KMceSipSessionProgress )
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipSessionProgress /*sentResponse*/);
    
    EUNIT_ASSERT ( event4_2.Code() == EMceItcUpdate );
    EUNIT_ASSERT ( event4_2.ParamStatus() == KMceReady );
    EUNIT_ASSERT ( iSipSession->BodyCandidate() != NULL );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateAcknowledgementRequired );
    
    MCE_RESET_STUBS();
    CleanupStack::PopAndDestroy( clientSessionMsg );
    MCE_DELETE( iSipSession->iNewBodyCandidate );
    iSipSession->iNewBodyCandidate = CMceComSession::NewL( CMceComSession::EInSession );
    iSipSession->iNewBodyCandidate->iID = 1;

// ---

    clientSession = CMceComSession::NewL( CMceComSession::EInSession );
    clientSession->iID = 1;
    ids.iSessionID = clientSession->iID;
    CleanupStack::PushL( clientSession );
    clientSessionMsg = 
        new (ELeave) CMceMsgObject<CMceComSession>( *clientSession, EMceItcMsgTypeSession );
    CleanupStack::Pop( clientSession );
    clientSessionMsg->PushL();
    CleanupStack::PushL( clientSessionMsg );

    iSipSession->iSubState = CMceSipSession::EUpdating;
    iStorage->iMediaManagerUpdateStatus = KErrGeneral;
    delete iStorage->iSipSentResponse;
    iStorage->iSipSentResponse = NULL;
    
    TMceStateTransitionEvent event5( *iSipSession, EMceItcUpdate, ids, 
                                      *clientSessionMsg );

    iState->EntryL( event5 );
    MCE_ENTRYL_POSTCONDITION_2( KMceSipServerInternalError )
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipServerInternalError /*sentResponse*/);
    
    EUNIT_ASSERT ( event5.Code() == EMceItcUpdate );
    EUNIT_ASSERT ( event5.ParamStatus() == KErrGeneral );
    EUNIT_ASSERT ( iSipSession->BodyCandidate() == NULL );
    MCE_RESET_STUBS();
    CleanupStack::PopAndDestroy( clientSessionMsg );
    
    iSipSession->iNewBodyCandidate = CMceComSession::NewL( CMceComSession::EInSession );
    iSipSession->iNewBodyCandidate->iID = 1;
    

// ---

    clientSession = CMceComSession::NewL( CMceComSession::EInSession );
    clientSession->iID = 1000;//wrong
    ids.iSessionID = clientSession->iID;
    CleanupStack::PushL( clientSession );
    clientSessionMsg = 
        new (ELeave) CMceMsgObject<CMceComSession>( *clientSession, EMceItcMsgTypeSession );
    CleanupStack::Pop( clientSession );
    clientSessionMsg->PushL();
    CleanupStack::PushL( clientSessionMsg );

    iSipSession->iSubState = CMceSipSession::EUpdating;
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    delete iStorage->iSipSentResponse;
    iStorage->iSipSentResponse = NULL;
    
    TMceStateTransitionEvent event5_1( *iSipSession, EMceItcUpdate, ids, 
                                      *clientSessionMsg );

    MCE_EUNIT_ASSERT_LEAVE( iState->EntryL( event5_1 ) );
    
    MCE_RESET_STUBS();
    CleanupStack::PopAndDestroy( clientSessionMsg );


// --- reject

    iSipSession->iSubState = CMceSipSession::EOffering;
    iStorage->iMediaManagerAction = CMCETls::ENone;
    iStorage->iSipSentMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    delete iStorage->iSipSentResponse;
    iStorage->iSipSentResponse = NULL;

    TMceStateTransitionEvent event6( *iSipSession, EMceItcRejectSession, ids );

    iState->EntryL( event6 );
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipDecline /*sentResponse*/);
    
    EUNIT_ASSERT ( event6.Code() == EMceItcRejectSession );
    EUNIT_ASSERT ( event6.ParamStatus() == KErrNone );
    MCE_RESET_STUBS();
    
// --- reject

    iSipSession->iSubState = CMceSipSession::EUpdating;
    iStorage->iMediaManagerAction = CMCETls::ENone;
    iStorage->iSipSentMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    delete iStorage->iSipSentResponse;
    iStorage->iSipSentResponse = NULL;

    TMceStateTransitionEvent event7( *iSipSession, EMceItcRejectSession, ids );

    iState->EntryL( event7 );
    
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipDecline /*sentResponse*/);
    
    EUNIT_ASSERT ( event7.Code() == EMceItcRejectSession );
    EUNIT_ASSERT ( event7.ParamStatus() == KErrNone );


// --- default

    TMceStateTransitionEvent event8( *iSipSession, EMceItcRing, ids );

    iState->EntryL( event8 );
    
    MCE_RESET_STUBS();
    
                      
// ---  Enable & disable    
    
    MCE_ASSERT_ENDPOINT_ENABLE_AND_DISABLE();
    
    }
    
void UT_CMceStateServerEstablishing::UT_CMceStateServerEstablishing_ExitLL()
    {
    CMceMsgBase* msg = NULL;
    
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EInSession );
    iSipSession->iSubState = CMceSipSession::EAnswering;
    
    
    
    TMceIds ids;
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateServerEstablishing );
    
    
    ids.iState = KMceNotAssigned;
    TMceStateTransitionEvent event1( *iSipSession, EMceItcUpdate, ids, *msg );
    
    iSipSession->iSubState = CMceSipSession::EAnswering;
    event1.ParamStatus() = KMceReady;
    
    iState->ExitL( event1 );
	EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateUpdated );
	EUNIT_ASSERT ( ids.iState == CMceSession::EReserving );
	EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::EProceeding );

    TMceStateTransitionEvent event1_2( *iSipSession, EMceItcUpdate, ids, *msg );
    
    iSipSession->iSubState = CMceSipSession::EUpdating;
    event1.ParamStatus() = KMceReady;

    iState->ExitL( event1_2 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateAnswering );
    EUNIT_ASSERT ( ids.iState == CMceSession::EAnswering );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::EAnswering );

    TMceStateTransitionEvent event1_3( *iSipSession, EMceItcUpdate, ids, *msg );
    
    iSipSession->iSubState = CMceSipSession::EUpdating;
    event1.ParamStatus() = KMceReady;
    
    CSIPServerTransaction* update = MCETestHelper::ServerTransactionL( SipStrConsts::EUpdate );
    CleanupStack::PushL( update );
    iSipSession->iPendingReceivedRequests.AppendL( update );
    CleanupStack::Pop( update );

    iState->ExitL( event1_3 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateEstablished );
    EUNIT_ASSERT ( ids.iState == CMceSession::EEstablished );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::EEstablished );
    delete iSipSession->PopRequest();

    ids.iState = KMceNotAssigned;
    TMceStateTransitionEvent event2( *iSipSession, EMceItcUpdate, ids, *msg );
    
    iSipSession->iSubState = CMceSipSession::EAnswering;
    event2.ParamStatus() = KMceAsync;
    
    iState->ExitL( event2 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateUpdating );
    EUNIT_ASSERT ( ids.iState == CMceSession::EReserving );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::EReserving );

    ids.iState = KMceNotAssigned;
    TMceStateTransitionEvent event3( *iSipSession, EMceItcUpdate, ids, *msg );
    
    iSipSession->iSubState = CMceSipSession::EUpdating;
    event3.ParamStatus() = KErrGeneral;
    
    iState->ExitL( event3 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateEstablished );
    EUNIT_ASSERT ( ids.iState == CMceSession::EEstablished );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::EEstablished );

 
    ids.iState = KMceNotAssigned;
    TMceStateTransitionEvent event4( *iSipSession, EMceItcRejectSession, ids );
    
    iSipSession->iSubState = CMceSipSession::EAnswering;
    
    iState->ExitL( event4 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateTerminated );
    EUNIT_ASSERT ( ids.iState == CMceSession::ETerminated );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::ETerminated );

    ids.iState = KMceNotAssigned;
    TMceStateTransitionEvent event5( *iSipSession, EMceItcRejectSession, ids );
    
    iSipSession->iSubState = CMceSipSession::EUpdating;
    
    iState->ExitL( event5 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateEstablished );
    EUNIT_ASSERT ( ids.iState == CMceSession::EEstablished );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::EEstablished );


    TMceStateTransitionEvent event6( *iSipSession, EMceItcRing, ids );
    iState->ExitL( event6 );

    
    }
    
    
        
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceStateServerEstablishing,
    "UT_CMceStateServerEstablishing",
    "UNIT" )

EUNIT_TEST(
    "AcceptL - test ",
    "CMceStateServerEstablishing",
    "AcceptL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateServerEstablishing_AcceptLL, Teardown)

EUNIT_TEST(
    "EntryL - test ",
    "CMceStateServerEstablishing",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateServerEstablishing_EntryLL, Teardown)

EUNIT_TEST(
    "ExitL - test ",
    "CMceStateServerEstablishing",
    "ExitL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateServerEstablishing_ExitLL, Teardown)
    



EUNIT_END_TEST_TABLE

//  END OF FILE
