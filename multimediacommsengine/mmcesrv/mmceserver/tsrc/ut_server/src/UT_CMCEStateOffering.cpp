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
#include "ut_cmcestateoffering.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <sipmessageelements.h>
#include <sipcseqheader.h>

//  INTERNAL INCLUDES
#include "MCETestHelper.h"

#include "mcesipsession.h"
#include "mcesipstatemachine.h"
#include "mceservercore.h"
#include "mcecssession.h"
#include "mcecssessionimplementation.h"
#include "mcesipconnection.h"
#include "mcecomsession.h"
#include "mcestateoffering.h"
#include "mcemediamanager.h"
#include <sdporiginfield.h>
#include "mcecssubsessionreceiver.h"
#include "mcecsreceivequeue.h"

#define MCE_ENTRYL_POSTCONDITION \
MCE_CHECK_MEMORY_LEAVE( iStorage->iMediaManagerAction == CMCETls::ECloseSession )

#define MCE_ENTRYL_POSTCONDITION_2 \
MCE_CHECK_MEMORY_LEAVE( iStorage->iMediaManagerAction != CMCETls::ECloseSession || \
     iStorage->iSipSentMethod != SIPStrings::StringF( SipStrConsts::EBye ) )

// CONSTRUCTION
UT_CMceStateOffering* UT_CMceStateOffering::NewL()
    {
    UT_CMceStateOffering* self = UT_CMceStateOffering::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceStateOffering* UT_CMceStateOffering::NewLC()
    {
    UT_CMceStateOffering* self = new( ELeave ) UT_CMceStateOffering();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceStateOffering::~UT_CMceStateOffering()
    {
    }

// Default constructor
UT_CMceStateOffering::UT_CMceStateOffering()
    {
    }

// Second phase construct
void UT_CMceStateOffering::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceStateOffering::SetupL(  )
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
    
    CSIPClientTransaction* transaction = 
        MCETestHelper::ClientTransactionLC( 
            SipStrConsts::EInvite, KMceSipOK, SipStrConsts::EPhraseOk, ETrue );
	iSipSession->SetPendingTransactionL( transaction );
    CleanupStack::Pop( transaction );    
    iSipSession->iResponse = transaction;
    
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    imServer = new (ELeave) CMceServerStub();
    iManager = CMceMediaManager::NewL( *imServer );
    iSdpSession = CMceSdpSession::NewL( *iSipSession->iBody, *iManager );

    iSipSession->NextState( KMceStateOffering );  
    iState = static_cast<CMceStateOffering*>(&iSipSession->CurrentState());
    
    iSipSession->iOffer = CSdpDocument::NewL();        
    }

void UT_CMceStateOffering::Setup2L(  )
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
    
    iSipSession->SetBodyL();
        
    MCETestHelper::RequestL( iSipSession, SipStrConsts::EUpdate, ETrue, EFalse );
    	
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EOffering /*clientState*/, 
                    KMceStateOffering /*serverState*/ );
    iState = static_cast<CMceStateOffering*>(&iSipSession->CurrentState());
    HBufC8* token = KMediaToken().AllocL();
    iSipSession->SetMediaAuthorizationToken( token );
    }

void UT_CMceStateOffering::Teardown()
    {
    CMCETls::Close();
    
	delete iSession;
    delete iServerCore;
    if ( imServer )
        {
        delete imServer;
        imServer = NULL;
        }
    if ( iManager )
        {
        delete iManager;
        iManager = NULL;
        }
    if ( iSdpSession )
        {
        delete iSdpSession;
        iSdpSession = NULL;
        }
    
    SdpCodecStringPool::Close();
    
    }


void UT_CMceStateOffering::UT_CMceStateOffering_AcceptLL()
    {
    TMceIds ids;
    CMceMsgBase* msg = NULL;

    TMceStateTransitionEvent event0( *iSipSession, EMceUpdate );
    EUNIT_ASSERT ( iState->AcceptL( event0 ) );

    TMceStateTransitionEvent event1( *iSipSession, EMceItcCancel, ids, *msg );
    EUNIT_ASSERT ( iState->AcceptL( event1 ) );

    TMceStateTransitionEvent event2( *iSipSession, EMceResponse );
    EUNIT_ASSERT ( iState->AcceptL( event2 ) );

    TMceStateTransitionEvent event3( *iSipSession, EMceProvisionalResponse );
    EUNIT_ASSERT ( iState->AcceptL( event3 ) );
    
    TMceStateTransitionEvent event4( *iSipSession, EMceErrorResponse );
    EUNIT_ASSERT ( iState->AcceptL( event4 ) );

    TMceStateTransitionEvent event5( *iSipSession, EMceMediaUpdated );
    EUNIT_ASSERT ( iState->AcceptL( event5 ) );

    TMceStateTransitionEvent event6( *iSipSession, EMceRedirectionResponse );
    EUNIT_ASSERT ( iState->AcceptL( event6 ) );
    
    TMceStateTransitionEvent event7( *iSipSession, EMceMediaSessionStopped );
    TRAPD( e1, iState->AcceptL( event7 ) );
    EUNIT_ASSERT ( e1 == KErrTotalLossOfPrecision );

	// Following Events will be accepted for Extenssion Requests
    CSIPClientTransaction* clitransaction2 = 
    MCETestHelper::ClientTransactionLC( SipStrConsts::EInfo, KMceSipSessionProgress, 
    									SipStrConsts::EPhraseSessionProgress, ETrue );
	iSipSession->SetPendingTransactionL( clitransaction2 );
    CleanupStack::Pop( clitransaction2 );
    iSipSession->iResponse = clitransaction2;

    TMceStateTransitionEvent event2a( *iSipSession, EMceResponse );
	TRAPD( e2a, iState->AcceptL( event2a ) );
    EUNIT_ASSERT ( e2a == KErrNone );

    TMceStateTransitionEvent event2b( *iSipSession, EMceProvisionalResponse );
	TRAPD( e2b, iState->AcceptL( event2b ) );
    EUNIT_ASSERT ( e2b == KErrNone );

    TMceStateTransitionEvent event2c( *iSipSession, EMceRedirectionResponse );
	TRAPD( e2c, iState->AcceptL( event2c ) );
	EUNIT_ASSERT ( e2c == KErrNone );

	TMceStateTransitionEvent event2d( *iSipSession, EMceErrorResponse );
	TRAPD( e2d, iState->AcceptL( event2d ) );
	EUNIT_ASSERT ( e2d == KErrNone );
	
	TMceStateTransitionEvent event2e( *iSipSession, EMceItcReplySend, ids, *msg );
	TRAPD( e2e, iState->AcceptL( event2e ) );
	EUNIT_ASSERT ( e2e == KErrNone );
	
	TMceStateTransitionEvent event2f( *iSipSession, EMceRequest );
	TRAPD( e2f, iState->AcceptL( event2f ) );
	EUNIT_ASSERT ( e2f == KErrNone );
    }

void UT_CMceStateOffering::UT_CMceStateOffering_EntryL_WithUpdateL()
    {	
    MCE_RESET_STUBS();
    // EMceUpdate
    // update fails
    TMceStateTransitionEvent event1( *iSipSession, EMceUpdate);
    iStorage->iMediaManagerUpdateStatus = KErrGeneral;
    iState->EntryL(event1);
    MCE_ASSERT_STUBS(CMCETls::EUpdate /*mmaction*/,
            CMCETls::EDecode /*mmsdpaction*/,
            SipStrConsts::EEmpty /*sentMethod*/, KErrNotFound /*sentResponse*/);
    MCE_ASSERT_EVENT(event1 /*event*/, EMceUpdate /*code*/, KErrGeneral /*status*/);

    MCE_RESET_STUBS();
    
    // async
    TMceStateTransitionEvent event2( *iSipSession, EMceUpdate);
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    iState->EntryL(event2);
    MCE_ASSERT_STUBS(CMCETls::EUpdate /*mmaction*/,
            CMCETls::EDecode /*mmsdpaction*/,
            SipStrConsts::EEmpty /*sentMethod*/, KErrNotFound /*sentResponse*/);
    MCE_ASSERT_EVENT(event2 /*event*/, EMceUpdate /*code*/, KMceAsync /*status*/);

    MCE_RESET_STUBS();
	
    // ready, reserve async
	    
    TMceStateTransitionEvent event3( *iSipSession, EMceUpdate);
    iStorage->iMediaManagerUpdateStatus = KMceReady;
    iStorage->iMediaManagerReserveStatus = KMceAsync;
    iState->EntryL(event3);
    MCE_ASSERT_STUBS(CMCETls::EReserve /*mmaction*/,
            CMCETls::EDecode /*mmsdpaction*/,
            SipStrConsts::EEmpty /*sentMethod*/, KErrNotFound /*sentResponse*/);
    MCE_ASSERT_EVENT(event3 /*event*/, EMceMediaUpdated /*code*/, KMceAsync /*status*/);
	
    // ready, reserve ready
	    
    TMceStateTransitionEvent event4( *iSipSession, EMceUpdate);
    iStorage->iMediaManagerUpdateStatus = KMceReady;
    iStorage->iMediaManagerReserveStatus = KMceReady;
    iState->EntryL(event4);
    MCE_ASSERT_STUBS(CMCETls::EReserve /*mmaction*/,
            CMCETls::EEncode /*mmsdpaction*/,
            SipStrConsts::EEmpty /*sentMethod*/, KMceSipOK /*sentResponse*/);
    MCE_ASSERT_EVENT(event4 /*event*/, EMceMediaUpdated /*code*/, KMceReady /*status*/);

    MCE_RESET_STUBS();
    }

void UT_CMceStateOffering::UT_CMceStateOffering_EntryL_WithProvisionalResponsesL()
    {
    MCE_RESET_STUBS();
    CMceCsSubSessionReceiver* receiver = iSipSession->iReceiver;
    TInt receiveCount = receiver->iReceiveQueue->iReceiveItems.Count();
    iSipSession->iSubState = CMceSipSession::EOffering;

    MCETestHelper::SetResponseL( 
        *iSipSession->iResponse, 
        KMceSipRinging, 
        SipStrConsts::EPhraseRinging, 
        EFalse, ETrue, 1 );
    
    // first response received
    TMceStateTransitionEvent event15( *iSipSession, EMceProvisionalResponse );
    iState->EntryL( event15 );
    iState->ExitL( event15 );
    // MCE client must be informed once about response
    EUNIT_ASSERT( ++receiveCount
        == receiver->iReceiveQueue->iReceiveItems.Count() )
    
    // retransmission received
    iState->EntryL( event15 );
    iState->ExitL( event15 );
    EUNIT_ASSERT( receiveCount
        == receiver->iReceiveQueue->iReceiveItems.Count() )
    }


void UT_CMceStateOffering::UT_CMceStateOffering_EntryL_With2XXResponsesL()
    {
    TMceIds ids;
    CMceMsgBase* msg = NULL;

    // 200 OK with answer decoding ready
    MCE_RESET_STUBS();
    iStorage->iMediaManagerUpdateStatus = KMceReady;
    iSipSession->iSubState = CMceSipSession::EOffering;
    CMceComSession& body = iSipSession->ActiveBody();
    body.iSdpSession = iSdpSession;
    body.SecureSessionL();
    
    TMceStateTransitionEvent event1( *iSipSession, EMceResponse );
    iState->EntryL( event1 );
    MCE_ENTRYL_POSTCONDITION
    
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                      CMCETls::EDecode /*mmsdpaction*/, 
                      SipStrConsts::EAck /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    
    EUNIT_ASSERT ( iStorage->iAckSent );
    EUNIT_ASSERT ( event1.Code() == EMceMediaUpdated );
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 1 );
    EUNIT_ASSERT ( !iSipSession->WaitingMediaCallback() );
	EUNIT_ASSERT ( body.iSipContactAddrSecure == CMceSession::EControlPathUnsecure );
	EUNIT_ASSERT ( body.SecureSession()->iLSReadyToBind == ETrue );

    iSipSession->iSubState = CMceSipSession::EOffering;
    MCE_RESET_STUBS();
   
    // 200 OK with answer decoding not ready
    iSipSession->iSubState = CMceSipSession::EOffering;
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    
    TMceStateTransitionEvent event2( *iSipSession, EMceResponse );
    iState->EntryL( event2 );
    MCE_ENTRYL_POSTCONDITION
    
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                      CMCETls::EDecode /*mmsdpaction*/, 
                      SipStrConsts::EAck /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    
    EUNIT_ASSERT ( iStorage->iAckSent );
    EUNIT_ASSERT ( event2.Code() == EMceResponse );
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 1 );
    EUNIT_ASSERT ( iSipSession->WaitingMediaCallback() );
    MCE_RESET_STUBS();
    
    // 200 OK while waiting for media callback
    iSipSession->iSubState = CMceSipSession::EOffering;
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    
    TMceStateTransitionEvent event2_1( *iSipSession, EMceResponse );
    iState->EntryL( event2_1 );
    MCE_ENTRYL_POSTCONDITION
    
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EAck /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    
    EUNIT_ASSERT ( iStorage->iAckSent );
    EUNIT_ASSERT ( event2_1.Code() == EMceResponse );
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 1 );
    EUNIT_ASSERT ( iSipSession->WaitingMediaCallback() );
    MCE_RESET_STUBS();
    
    // 200 OK with wrong SDP
    iSipSession->iSubState = CMceSipSession::EOffering;
    iSipSession->SetWaitingMediaCallback( EFalse );
    iStorage->iMediaManagerUpdateStatus = KErrGeneral;
    
    iSipSession->NextState( KMceStateOffering );
    //use handlesipevent
    iSipSession->HandleSIPEvent( EMceResponse, iSipSession->Dialog()->Dialog() );
    MCE_CHECK_MEMORY_LEAVE( iStorage->iMediaManagerSdpAction != CMCETls::EDecode || 
         iStorage->iSipSentMethod != SIPStrings::StringF( SipStrConsts::EBye ) )
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::EDecode /*mmsdpaction*/, 
                      SipStrConsts::EBye /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    
    EUNIT_ASSERT ( iStorage->iAckSent );
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 2 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateTerminating );
    MCE_RESET_STUBS();
    }


void UT_CMceStateOffering::UT_CMceStateOffering_EntryL_WithErrorResponsesL()
    {
    TMceIds ids;
    CMceMsgBase* msg = NULL;

    // 302 Redirect Response
    MCE_RESET_STUBS();
    iSipSession->iSubState = CMceSipSession::EOffering;
    iSipSession->iResponse = iSipSession->PendingTransactions()[0];
    MCETestHelper::SetResponseL( 
        *iSipSession->iResponse, 
        KMceSipMovedTemporarily, 
        SipStrConsts::EPhraseOk, ETrue );
    
    TMceStateTransitionEvent event11( *iSipSession, EMceRedirectionResponse );
    iState->EntryL( event11 );
    
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EInvite /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    
                     
    MCE_RESET_STUBS();
    
    // 302 response for updating
    iSipSession->iSubState = CMceSipSession::EUpdating;
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    
    MCETestHelper::ChangeResponseTypeL( 
        *iSipSession->iResponse->ResponseElements(), 
        KMceSipMovedTemporarily, 
        SipStrConsts::EPhraseOk );
    
    TMceStateTransitionEvent event12( *iSipSession, EMceRedirectionResponse );
    iState->EntryL( event12 );
    
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    
    MCE_RESET_STUBS();
    
    // 300 Redirect Response
    MCE_RESET_STUBS();
    iSipSession->iSubState = CMceSipSession::EOffering;
    iSipSession->iResponse = iSipSession->PendingTransactions()[0];
    iSipSession->iSentSdpCount = 2;
    iSipSession->iReceivedSdpCount = 1;   
    MCETestHelper::SetResponseL( 
        *iSipSession->iResponse,
        KMceSipMultipleChoices,
        SipStrConsts::EPhraseOk, ETrue );
    
    TMceStateTransitionEvent event13( *iSipSession, EMceRedirectionResponse );
    iState->EntryL( event13 );
    // error response resets the received and sent SDP counts
    EUNIT_ASSERT_EQUALS( 0, iSipSession->iSentSdpCount )
    EUNIT_ASSERT_EQUALS( 0, iSipSession->iReceivedSdpCount )    
    
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    
     
    MCE_RESET_STUBS();
    
    // 300 response for updating
    iSipSession->iSubState = CMceSipSession::EUpdating;
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    
    MCETestHelper::ChangeResponseTypeL( 
        *iSipSession->iResponse->ResponseElements(), 
        KMceSipMovedTemporarily, 
        SipStrConsts::EPhraseOk );
    
    TMceStateTransitionEvent event14( *iSipSession, EMceRedirectionResponse );
    iState->EntryL( event14 );
    
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);    
    
    // KMceSipDecline
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    iSipSession->iSentSdpCount = 2;
    iSipSession->iReceivedSdpCount = 1;
    
    MCETestHelper::ChangeResponseTypeL( 
        *iSipSession->iResponse->ResponseElements(), 
        KMceSipDecline, 
        SipStrConsts::EPhraseDecline );
    
    TMceStateTransitionEvent event3( *iSipSession, EMceErrorResponse );
    iState->EntryL( event3 );
    // error response resets the received and sent SDP counts
    EUNIT_ASSERT_EQUALS( 0, iSipSession->iSentSdpCount )
    EUNIT_ASSERT_EQUALS( 0, iSipSession->iReceivedSdpCount )
    
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    
    EUNIT_ASSERT ( event3.Code() == EMceErrorResponse );
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 2 );
    MCE_RESET_STUBS();
    
    // KMceSipDecline updating
    iSipSession->iSubState = CMceSipSession::EUpdating;
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    
    MCETestHelper::ChangeResponseTypeL( 
        *iSipSession->iResponse->ResponseElements(), 
        KMceSipDecline, 
        SipStrConsts::EPhraseDecline );
    
    TMceStateTransitionEvent event3_1( *iSipSession, EMceErrorResponse );
    iState->EntryL( event3_1 );
    
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    
    EUNIT_ASSERT ( event3_1.Code() == EMceErrorResponse );
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 2 );
    MCE_RESET_STUBS();
    
    // KMceSipCallOrTransactionDoesNotExist updating
    iSipSession->iSubState = CMceSipSession::EUpdating;
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    
    MCETestHelper::ChangeResponseTypeL( 
        *iSipSession->iResponse->ResponseElements(),
        KMceSipCallOrTransactionDoesNotExist,
        SipStrConsts::EPhraseCallTransactionDoesNotExist );
    
    TMceStateTransitionEvent event3_2( *iSipSession, EMceErrorResponse );
    iState->EntryL( event3_2 );
    MCE_ENTRYL_POSTCONDITION_2
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EBye /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    
    EUNIT_ASSERT ( event3_2.Code() == EMceErrorResponse );
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 3 );
    MCE_RESET_STUBS();
    
    // KMceSipRequestTimeout updating
    iSipSession->iSubState = CMceSipSession::EUpdating;
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    
    MCETestHelper::ChangeResponseTypeL( 
        *iSipSession->iResponse->ResponseElements(),
        KMceSipRequestTimeout,
        SipStrConsts::EPhraseRequestTimeout );
    
    TMceStateTransitionEvent event3_3( *iSipSession, EMceErrorResponse );
    iState->EntryL( event3_3 );
    MCE_ENTRYL_POSTCONDITION_2
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EBye /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    
    EUNIT_ASSERT ( event3_2.Code() == EMceErrorResponse );
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 4 );
    MCE_RESET_STUBS();
    
    // KMceSipSessionIntervalTooSmall && dialog state ETerminated
    iSipSession->Dialog()->Dialog().iState = CSIPDialog::ETerminated;
    iSipSession->iSubState = CMceSipSession::EOffering;
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    
    MCETestHelper::ChangeResponseTypeL( 
        *iSipSession->iResponse->ResponseElements(),
        KMceSipSessionIntervalTooSmall,
        SipStrConsts::EPhraseIntervalTooBrief );
    
    TMceStateTransitionEvent event4_2( *iSipSession, EMceErrorResponse );
    iState->EntryL( event4_2 );
    
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EInvite /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 5 );
    EUNIT_ASSERT ( event4_2.Code() == EMceErrorResponse );
    MCE_RESET_STUBS();

    // KMceSipSessionIntervalTooSmall && dialog state EConfirmed
    iSipSession->Dialog()->Dialog().iState = CSIPDialog::EConfirmed;
    iSipSession->iSubState = CMceSipSession::EOffering;
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    
    MCETestHelper::ChangeResponseTypeL(
        *iSipSession->iResponse->ResponseElements(),
        KMceSipSessionIntervalTooSmall,
        SipStrConsts::EPhraseIntervalTooBrief );
    
    TMceStateTransitionEvent event4_3( *iSipSession, EMceErrorResponse );
    iState->EntryL( event4_3 );
    
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EInvite /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 6 );
    EUNIT_ASSERT ( event4_3.Code() == EMceErrorResponse );
    MCE_RESET_STUBS();
    }


void UT_CMceStateOffering::UT_CMceStateOffering_EntryL_WithOtherEventsL()
    {
    TMceIds ids;
    CMceMsgBase* msg = NULL;   
    
    // user cancel (not ready)
    iSipSession->iSubState = CMceSipSession::EOffering;
    iSipSession->PendingTransactions()[0]->iState = CSIPTransactionBase::ETrying;
    TMceStateTransitionEvent cancelEvent( *iSipSession, EMceItcCancel, ids, *msg );
    iState->EntryL( cancelEvent );
   
    MCE_ASSERT_STUBS( CMCETls::ECloseSession, //mmaction 
                      CMCETls::ENone, //mmsdpaction 
                      SipStrConsts::EEmpty, //sentMethod 
                      KErrNotFound ); //sentResponse
    
    EUNIT_ASSERT_EQUALS( 1, iSipSession->PendingTransactions().Count() );
    EUNIT_ASSERT_EQUALS( EMceItcCancel , cancelEvent.Code() );
    MCE_RESET_STUBS();

    // user cancel (ready)
    iSipSession->iSubState = CMceSipSession::EOffering;
    iSipSession->InviteTransaction()->iState = CSIPTransactionBase::EProceeding;
    TMceStateTransitionEvent cancelEvent2( *iSipSession, EMceItcCancel, ids, *msg );
    iState->EntryL( cancelEvent2 );
    
    MCE_ASSERT_STUBS( CMCETls::ECloseSession, //mmaction 
                      CMCETls::ENone, //mmsdpaction
                      SipStrConsts::ECancel, //sentMethod 
                      KErrNotFound ); //sentResponse
    
    EUNIT_ASSERT_EQUALS( 2, iSipSession->PendingTransactions().Count() )
    MCE_RESET_STUBS();

    iSipSession->iSubState = CMceSipSession::EOffering;
    TMceStateTransitionEvent updatedEvent( *iSipSession, EMceMediaUpdated );
    iState->EntryL( updatedEvent );
    
    MCE_ASSERT_STUBS( CMCETls::ENone, //mmaction 
                      CMCETls::ENone, //mmsdpaction 
                      SipStrConsts::EEmpty, //sentMethod, 
                      KErrNotFound ); //sentResponse
    
    EUNIT_ASSERT_EQUALS( 2, iSipSession->PendingTransactions().Count() )

    iSipSession->iSubState = CMceSipSession::EOffering;
    
    updatedEvent.ParamStatus() = KErrGeneral;
    iState->EntryL( updatedEvent );// NOP
    
    MCE_ASSERT_STUBS( CMCETls::ENone, //mmaction 
                      CMCETls::ENone, //mmsdpaction 
                      SipStrConsts::EEmpty, //sentMethod 
                      KErrNotFound );  //sentResponse
    
    EUNIT_ASSERT_EQUALS( 2, iSipSession->PendingTransactions().Count() )
    MCE_RESET_STUBS();

    // update and ready
    iStorage->iMediaManagerUpdateStatus = KMceReady;
    iSipSession->iSubState = CMceSipSession::EUpdating;
    iSipSession->iNewBodyCandidate = iSipSession->iBody->CloneL();
    iSipSession->iResponse = iSipSession->PendingTransactions()[0];
    MCETestHelper::SetResponseL( 
        *iSipSession->iResponse, 
        KMceSipOK, SipStrConsts::EPhraseOk, ETrue );
      
    TMceStateTransitionEvent okResponseEvent( *iSipSession, EMceResponse );
    iState->EntryL( okResponseEvent );
    
    MCE_CHECK_MEMORY_LEAVE( iStorage->iMediaManagerSdpAction != CMCETls::EDecode )
    MCE_ASSERT_STUBS( CMCETls::ECloseSession, //mmaction 
                      CMCETls::EDecode, //mmsdpaction 
                      SipStrConsts::EAck, //sentMethod 
                      KErrNotFound ); //sentResponse
    
    EUNIT_ASSERT ( iStorage->iAckSent );
    EUNIT_ASSERT_EQUALS( 2, iSipSession->PendingTransactions().Count() )
    EUNIT_ASSERT ( okResponseEvent.Code() == EMceMediaUpdated );

    // refresing with bad SDP
    iSipSession->iSubState = CMceSipSession::ERefreshing;
    MCE_RESET_STUBS();
    iSipSession->iResponse = iSipSession->PendingTransactions()[0];
    MCETestHelper::SetResponseBADL( *iSipSession->iResponse, KMceSipOK, SipStrConsts::EPhraseOk );
    
    TMceStateTransitionEvent badResponseEvent( *iSipSession, EMceResponse );
    iState->EntryL( badResponseEvent );
    
    EUNIT_ASSERT ( iStorage->iMediaManagerAction == CMCETls::ENone );
    EUNIT_ASSERT ( iStorage->iSipSentMethod == SIPStrings::StringF( SipStrConsts::EAck ) );
    EUNIT_ASSERT ( iStorage->iAckSent );
    EUNIT_ASSERT_EQUALS( 2, iSipSession->PendingTransactions().Count() )   
    }

    
void UT_CMceStateOffering::UT_CMceStateOffering_ExitLL()
    {
    CMceMsgBase* msg = NULL;
    
    iSipSession->iSubState = CMceSipSession::EOffering;
    
    TMceIds ids;
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateOffering );

    
    TMceStateTransitionEvent event1( *iSipSession, EMceMediaUpdated );
    iState->ExitL( event1 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateEstablished );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::EEstablished );
    

    iSipSession->NextState( KMceStateOffering );
    iSipSession->iSubState = CMceSipSession::EOffering;
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateOffering );

    TMceStateTransitionEvent event2( *iSipSession, EMceItcCancel, ids, *msg );
    iState->ExitL( event2 );

    //Ringing Case 1: Not Reliable
    iSipSession->NextState( KMceStateOffering );
    iSipSession->iSubState = CMceSipSession::EOffering;
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateOffering );
    
    MCETestHelper::ChangeResponseTypeL( 
        *iSipSession->iResponse->ResponseElements(), 
        KMceSipRinging, 
        SipStrConsts::EPhraseRinging );
    
    TMceStateTransitionEvent event3( *iSipSession, EMceProvisionalResponse );
    iState->ExitL( event3 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateOffering );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::EOffering );
    
    
    
	//Ringing Case 2: Reliable Provisional Response
    iSipSession->NextState( KMceStateOffering );
    iSipSession->iSubState = CMceSipSession::EOffering;
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateOffering );
            
    MCETestHelper::SetResponseL( 
        *iSipSession->iResponse, 
        KMceSipRinging, SipStrConsts::EPhraseRinging,
    	EFalse, ETrue, 500);
    
    TMceStateTransitionEvent event3_1( *iSipSession, EMceProvisionalResponse );
    iState->ExitL( event3_1 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateOffering );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::EOffering );
    EUNIT_ASSERT ( iSipSession->RSeq() == 500 );
    
        

    iSipSession->NextState( KMceStateOffering );
    iSipSession->iSubState = CMceSipSession::EOffering;
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateOffering );

    MCETestHelper::ChangeResponseTypeL( 
        *iSipSession->iResponse->ResponseElements(), 
        KMceSipQueued, 
        SipStrConsts::EEmpty );

    TMceStateTransitionEvent event4( *iSipSession, EMceProvisionalResponse );
    iState->ExitL( event4 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateOffering );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::EOffering );


    iSipSession->NextState( KMceStateOffering );
    iSipSession->iSubState = CMceSipSession::EOffering;
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateOffering );

    TMceStateTransitionEvent event5( *iSipSession, EMceErrorResponse );
    iState->ExitL( event5 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateTerminated );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::ETerminated );


    iSipSession->NextState( KMceStateOffering );
    iSipSession->iSubState = CMceSipSession::EOffering;
    iSipSession->Dialog()->Dialog().iState = CSIPDialog::ETerminated;
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateOffering );

    MCETestHelper::ChangeResponseTypeL( 
        *iSipSession->iResponse->ResponseElements(), 
        KMceSipSessionIntervalTooSmall, 
        SipStrConsts::EPhraseIntervalTooBrief );

    TMceStateTransitionEvent event6( *iSipSession, EMceErrorResponse );
    iState->ExitL( event6 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateOffering );
      

    iSipSession->NextState( KMceStateOffering );
    iSipSession->iSubState = CMceSipSession::EOffering;
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateOffering );

    MCETestHelper::ChangeResponseTypeL( 
        *iSipSession->iResponse->ResponseElements(), 
        KMceSipBadRequest, 
        SipStrConsts::EPhraseBadRequest );
    

    TMceStateTransitionEvent event7( *iSipSession, EMceErrorResponse );
    iState->ExitL( event7 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateTerminated );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::ETerminated );

    iSipSession->NextState( KMceStateOffering );
    iSipSession->iSubState = CMceSipSession::EUpdating;
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateOffering );

    MCETestHelper::ChangeResponseTypeL( 
        *iSipSession->iResponse->ResponseElements(), 
        KMceSipBadRequest, 
        SipStrConsts::EPhraseBadRequest );
    

    TMceStateTransitionEvent event7_1( *iSipSession, EMceErrorResponse );
    iState->ExitL( event7_1 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateEstablished );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::EEstablished );


    iSipSession->NextState( KMceStateOffering );
    iSipSession->iSubState = CMceSipSession::EUpdating;
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateOffering );

    MCETestHelper::ChangeResponseTypeL( 
        *iSipSession->iResponse->ResponseElements(), 
        KMceSipCallOrTransactionDoesNotExist, 
        SipStrConsts::EPhraseCallTransactionDoesNotExist );
    

    TMceStateTransitionEvent event7_2( *iSipSession, EMceErrorResponse );
    iState->ExitL( event7_2 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateTerminating );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::ETerminating );

    iSipSession->NextState( KMceStateOffering );
    iSipSession->iSubState = CMceSipSession::EUpdating;
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateOffering );

    MCETestHelper::ChangeResponseTypeL( 
        *iSipSession->iResponse->ResponseElements(), 
        KMceSipRequestTimeout, 
        SipStrConsts::EPhraseRequestTimeout );

    TMceStateTransitionEvent event7_3( *iSipSession, EMceErrorResponse );
    iState->ExitL( event7_3 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateTerminating );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::ETerminating );


    iSipSession->NextState( KMceStateOffering );
    iSipSession->iSubState = CMceSipSession::EOffering;
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateOffering );
    
    iSipSession->NextState( KMceStateOffering );
    iSipSession->iSubState = CMceSipSession::EOffering;
    MCETestHelper::ChangeResponseTypeL(
        *iSipSession->iResponse->ResponseElements(),
        KMceSipMovedTemporarily,
        SipStrConsts::EPhraseOk );
    
    TMceStateTransitionEvent event8( *iSipSession, EMceRedirectionResponse );
    iState->ExitL( event8 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateOffering );
    
    iSipSession->NextState( KMceStateOffering );
    iSipSession->iSubState = CMceSipSession::EUpdating;
    TMceStateTransitionEvent event9( *iSipSession, EMceRedirectionResponse );
    iState->ExitL( event9 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateServerEstablishing );
    
    iSipSession->NextState( KMceStateOffering );
    iSipSession->iSubState = CMceSipSession::EOffering;
    MCETestHelper::ChangeResponseTypeL( 
        *iSipSession->iResponse->ResponseElements(),
        KMceSipMultipleChoices,
        SipStrConsts::EPhraseOk );

    TMceStateTransitionEvent event10( *iSipSession, EMceRedirectionResponse );
    iState->ExitL( event10 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateTerminated );
    
    iSipSession->NextState( KMceStateOffering );
    iSipSession->iSubState = CMceSipSession::EAnswering;
    TMceStateTransitionEvent event11( *iSipSession, EMceRedirectionResponse );
    iState->ExitL( event11 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateOffering );   

	// Hanlding of Extension Request / Response does not change the session state    

	//EMceItcReplySend
	TMceStateTransitionEvent event12( *iSipSession, EMceItcReplySend, ids, *msg );
    iState->ExitL( event12 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateOffering );   
    
    //EMceRequest
	TMceStateTransitionEvent event13( *iSipSession, EMceRequest );
    iState->ExitL( event13 );
	EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateOffering );   
	
	//EMceProvisionalResponse
	CSIPClientTransaction* clitransaction = 
    MCETestHelper::ClientTransactionLC( SipStrConsts::EInfo, KMceSipSessionProgress, 
    									SipStrConsts::EPhraseSessionProgress, ETrue );
	iSipSession->SetPendingTransactionL( clitransaction );
    CleanupStack::Pop( clitransaction );
    iSipSession->iResponse = clitransaction;

	TMceStateTransitionEvent event14( *iSipSession, EMceProvisionalResponse );
    iState->ExitL( event14 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateOffering );   

	//EMceResponse
	CSIPClientTransaction* clitransaction1 = 
    MCETestHelper::ClientTransactionLC( SipStrConsts::EInfo, KMceSipOK, 
    									SipStrConsts::EPhraseOk, ETrue );
	iSipSession->SetPendingTransactionL( clitransaction1 );
    CleanupStack::Pop( clitransaction1 );
    iSipSession->iResponse = clitransaction1;
	
	TMceStateTransitionEvent event15( *iSipSession, EMceResponse );
    iState->ExitL( event15 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateOffering );   


	//EMceRedirectionResponse
	CSIPClientTransaction* clitransaction2 = 
    MCETestHelper::ClientTransactionLC( SipStrConsts::EInfo, KMceSipMovedTemporarily, 
    									SipStrConsts::EPhraseOk, ETrue );
	iSipSession->SetPendingTransactionL( clitransaction2 );
    CleanupStack::Pop( clitransaction2 );
    iSipSession->iResponse = clitransaction2;
	
	TMceStateTransitionEvent event16( *iSipSession, EMceRedirectionResponse );
    iState->ExitL( event16 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateOffering );   
	

	//EMceErrorResponse
	CSIPClientTransaction* clitransaction3 = 
    MCETestHelper::ClientTransactionLC( SipStrConsts::EInfo, KMceSipBadExtension, 
    									SipStrConsts::EPhraseBadExtension, ETrue );
	iSipSession->SetPendingTransactionL( clitransaction3 );
    CleanupStack::Pop( clitransaction3 );
    iSipSession->iResponse = clitransaction3;
	
	TMceStateTransitionEvent event17( *iSipSession, EMceErrorResponse );
    iState->ExitL( event17 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateOffering );
    
    //EMceMediaUpdated, IMS flow
    TMceStateTransitionEvent event1_1( *iSipSession, EMceMediaUpdated );
    CSIPClientTransaction* response = &iSipSession->Response();
    CSIPCSeqHeader* cSeqHeader = CSIPCSeqHeader::DecodeL( _L8("1 UPDATE") );
    CSIPMessageElements* messageElements =
        const_cast<CSIPMessageElements*>( &response->ResponseElements()->MessageElements() );
    messageElements->AddHeaderL( cSeqHeader );
    iState->ExitL( event1_1 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateOffering );
    }
    
    
        
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceStateOffering,
    "UT_CMceStateOffering",
    "UNIT" )

EUNIT_TEST(
    "AcceptL - test ",
    "CMceStateOffering",
    "AcceptL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateOffering_AcceptLL, Teardown)

EUNIT_TEST(
    "EntryL with update",
    "CMceStateOffering",
    "EntryL",
    "FUNCTIONALITY",
    Setup2L, UT_CMceStateOffering_EntryL_WithUpdateL, Teardown)

EUNIT_TEST(
    "EntryL with provisional responses",
    "CMceStateOffering",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateOffering_EntryL_WithProvisionalResponsesL, Teardown)

EUNIT_TEST(
    "EntryL with 2XX responses",
    "CMceStateOffering",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateOffering_EntryL_With2XXResponsesL, Teardown)
    
EUNIT_TEST(
    "EntryL with error responses",
    "CMceStateOffering",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateOffering_EntryL_WithErrorResponsesL, Teardown)
    
EUNIT_TEST(
    "EntryL with other events",
    "CMceStateOffering",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateOffering_EntryL_WithOtherEventsL, Teardown) 

EUNIT_TEST(
    "ExitL - test ",
    "CMceStateOffering",
    "ExitL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateOffering_ExitLL, Teardown)
    



EUNIT_END_TEST_TABLE

//  END OF FILE
