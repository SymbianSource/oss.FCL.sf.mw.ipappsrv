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
#include "ut_cmcesipevent.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "MCETestHelper.h"
#include "mcesipevent.h"
#include "mcesipstatemachine.h"
#include "mceservercore.h"
#include "mcecssession.h"
#include "mcecssessionimplementation.h"
#include "mcesipconnection.h"
#include "cmcecomevent.h"
#include "cmcetls.h"
#include "mceactionset.h"
#include "sipeventheader.h"


_LIT8(KFmtAttribute,"a=fmtp:TBCP format parameters\r\n");

_LIT8(KFmtAttribute2,"a=fmtp:TestPlugIn format parameters\r\n");



// CONSTRUCTION
UT_CMceSipEvent* UT_CMceSipEvent::NewL()
    {
    UT_CMceSipEvent* self = UT_CMceSipEvent::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceSipEvent* UT_CMceSipEvent::NewLC()
    {
    UT_CMceSipEvent* self = new( ELeave ) UT_CMceSipEvent();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceSipEvent::~UT_CMceSipEvent()
    {
    }

// Default constructor
UT_CMceSipEvent::UT_CMceSipEvent()
    {
    }

// Second phase construct
void UT_CMceSipEvent::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceSipEvent::SetupL()
    {
    CMCETls::OpenL();
    SdpCodecStringPool::OpenL();
    iIds.iProfileID = 1;
    iIds.iManagerType = KMceCSSIPEvent;
    iServerCore = CMceServerCore::NewL(); 
    iStorage = CMCETls::Storage();
    iSession = CMceCsSession::NewL( *iServerCore );
    iSession->SetClientUidL( iIds, RMessage2() );
    }

void UT_CMceSipEvent::SetupMoSubscribeL()
    {
    SetupL();
    iIds.iDialogType = KMceDlgTypeSubscribe;
    CDesC8ArrayFlat* params = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
	CleanupStack::PushL( params );
	params->AppendL( KEvent );
	params->AppendL( KRecipient );
	params->AppendL( KNullDesC8 );
    
    CMceMsgTextArray* msgParams = new (ELeave) CMceMsgTextArray( *params );
    CleanupStack::PushL( msgParams );
    iStorage->StoreL( iIds, *msgParams );
    CleanupStack::PopAndDestroy( msgParams );
    CleanupStack::PopAndDestroy( params );
    iSession->CreateSubSessionL(iIds, RMessage2());
    iSipEvent =  static_cast<CMceSipEvent*>((iSession->Implementation().SubSessions())[0]);
    }
    
void UT_CMceSipEvent::SetupMtSubscribeL()
    {
    SetupL();
    iSipEvent = MCETestHelper::ClientIncomingEventL( iSession, SipStrConsts::ESubscribe );
    EUNIT_ASSERT( iSipEvent->EventStateType() == EMtSubscribe );
    }

void UT_CMceSipEvent::SetupMoReferL()
    {
    SetupL();
    iIds.iDialogType = KMceDlgTypeRefer;
    CDesC8ArrayFlat* params = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
	CleanupStack::PushL( params );
	params->AppendL( KReferTo );
	params->AppendL( KRecipient );
	params->AppendL( KFrom );
    
    CMceMsgTextArray* msgParams = new (ELeave) CMceMsgTextArray( *params );
    CleanupStack::PushL( msgParams );
    iStorage->StoreL( iIds, *msgParams );
    CleanupStack::PopAndDestroy( msgParams );
    CleanupStack::PopAndDestroy( params );
    
    
    iSession->CreateSubSessionL(iIds, RMessage2());
    iSipEvent =  static_cast<CMceSipEvent*>((iSession->Implementation().SubSessions())[0]);
    }

void UT_CMceSipEvent::SetupMtReferL()
    {
    SetupL();
    
    CMceSipManager& manager = iServerCore->Manager();
    TUint32 profileId = 1;
    CSIPProfile& profile = manager.ProfileL( profileId );
    
    iSipEvent = MCETestHelper::ClientIncomingEventL( iSession, SipStrConsts::ERefer );
    //iSipEvent = static_cast<CMceSipEvent*>((iSession->Implementation().SubSessions())[0]);
    iSipEvent->iEvent->SetReferType( CMceRefer::ESuppressed );
    EUNIT_ASSERT( iSipEvent->EventStateType() == EMtRefer );
    }
    
void UT_CMceSipEvent::Teardown()
    {
    CMCETls::Close();
    
	delete iSession;
    delete iServerCore;
    
    SdpCodecStringPool::Close();
    
    }

void UT_CMceSipEvent::UT_CMceSipEvent_NewLL()
    {
    CMceSipManager& manager = iServerCore->Manager();
    TUint32 profileId = 1;
    CSIPProfile& profile = manager.ProfileL( profileId );
    
    CMceSipConnection& connection = manager.ConnectionL( profile );
    
    CMceSipEvent* event = CMceSipEvent::NewL( *iSession, connection, profile );
    delete event;
    }


void UT_CMceSipEvent::UT_CMceSipEvent_DoServiceLL()
    {
    EUNIT_ASSERT_LEAVE( iSipEvent->DoServiceL( iIds, EMceItcSubscribe, KNullDesC8 ) );
 
    }

void UT_CMceSipEvent::UT_CMceSipEvent_DoServiceL2L()
    {
    // MO Subscribe
    // Leave with no iEvent and wrong action;
    CMceMsgBase* failMsg = MCETestHelper::EventMsg( EMceItcEventTypeSubscribe );
    CleanupStack::PushL( failMsg );
    EUNIT_ASSERT_LEAVE( iSipEvent->DoServiceL( iIds,EMceItcSubscribeUpdate,*failMsg ) );
    CleanupStack::PopAndDestroy( failMsg );
    
    // EMceItcSubscribe
    CMceMsgBase* subscrMsg = MCETestHelper::EventMsg( EMceItcEventTypeSubscribe );
    CleanupStack::PushL( subscrMsg );
    iSipEvent->DoServiceL( iIds,EMceItcSubscribe,*subscrMsg );
    CleanupStack::PopAndDestroy( subscrMsg );
    
    EUNIT_ASSERT( iSipEvent->iEvent );
    EUNIT_ASSERT( iSipEvent->EventStateType() == EMoSubscribe );
    
    //EMceItcSubscribeUpdate
    CMceMsgBase* updateMsg = MCETestHelper::EventMsg( EMceItcEventTypeSubscribe );
    CleanupStack::PushL( updateMsg );
    iSipEvent->DoServiceL( iIds,EMceItcSubscribeUpdate,*updateMsg );
    CleanupStack::PopAndDestroy( updateMsg );
    
    //EMceItcTerminateEvent
    // Msg can be defined also
    CMceMsgBase* msg2 = MCETestHelper::EventMsg( EMceItcEventTypeSubscribe );
    CleanupStack::PushL( msg2 );
    iSipEvent->DoServiceL( iIds,EMceItcTerminateEvent,*msg2 );
    CleanupStack::PopAndDestroy( msg2 );
    
    //EMceItcTerminateEvent, successfull
    iSipEvent->DoServiceL( iIds, EMceItcTerminateEvent );
    }

void UT_CMceSipEvent::UT_CMceSipEvent_DoServiceL3L()
    {
    // MO Refer
    // EMceItcReferEvent
    CMceMsgBase* referMsg = MCETestHelper::EventMsg( EMceItcEventTypeRefer );
    CleanupStack::PushL( referMsg );
    
    iSipEvent->DoServiceL( iIds,EMceItcReferEvent,*referMsg );
    CleanupStack::PopAndDestroy( referMsg );
    
    EUNIT_ASSERT( iSipEvent->iEvent );
    EUNIT_ASSERT( iSipEvent->EventStateType() == EMoRefer );
    }

void UT_CMceSipEvent::UT_CMceSipEvent_DoServiceL4L()
    {
    // MT Subscribe
    
    // Leave with totally wrong action
    EUNIT_ASSERT_LEAVE( iSipEvent->DoServiceL( iIds, EMceItcCreateSession ) );
    
    //EMceItcAcceptEvent
    iSipEvent->DoServiceL( iIds, EMceItcAcceptEvent );
    
 	//EMceItcRejectEvent
 	iSipEvent->DoServiceL( iIds, EMceItcRejectEvent );
    
 	//EMceItcTerminateEvent
 	iSipEvent->DoServiceL( iIds, EMceItcTerminateEvent );
    
    //EMceItcNotify
    CMceMsgBase* notifyMsg = MCETestHelper::EventMsg( EMceItcEventTypeEither );
    CleanupStack::PushL( notifyMsg );
    iSipEvent->DoServiceL( iIds,EMceItcNotify,*notifyMsg );
    CleanupStack::PopAndDestroy( notifyMsg );
    
    //EMceItcRespondEvent, 1XX
    CMceMsgBase* provMsg = MCETestHelper::ReplyMsg( 222 );
    CleanupStack::PushL( provMsg );
    iSipEvent->DoServiceL( iIds,EMceItcRespondEvent,*provMsg );
    CleanupStack::PopAndDestroy( provMsg );
    
    //EMceItcRespondEvent, 2XX
    CMceMsgBase* acceptMsg = MCETestHelper::ReplyMsg( 222 );
    CleanupStack::PushL( acceptMsg );
    iSipEvent->DoServiceL( iIds,EMceItcRespondEvent,*acceptMsg );
    CleanupStack::PopAndDestroy( acceptMsg );
    
    //EMceItcRespondEvent, 4XX
    CMceMsgBase* rejectMsg = MCETestHelper::ReplyMsg( 444 );
    CleanupStack::PushL( rejectMsg );
    iSipEvent->DoServiceL( iIds,EMceItcRespondEvent,*rejectMsg );
    CleanupStack::PopAndDestroy( rejectMsg );
    }

void UT_CMceSipEvent::UT_CMceSipEvent_DoServiceL5L()
    {
    // MT Refer
    
    //EMceItcAcceptEvent
    iSipEvent->DoServiceL( iIds, EMceItcAcceptEvent );
    
 	//EMceItcRejectEvent
 	iSipEvent->DoServiceL( iIds, EMceItcRejectEvent );
    
 	//EMceItcRejectEvent
 	iSipEvent->DoServiceL( iIds, EMceItcTerminateEvent );
    
    //EMceItcNotify
    CMceMsgBase* notifyMsg = MCETestHelper::EventMsg( EMceItcEventTypeEither );
    CleanupStack::PushL( notifyMsg );
    iSipEvent->DoServiceL( iIds,EMceItcNotify,*notifyMsg );
    CleanupStack::PopAndDestroy( notifyMsg );
    
    //EMceItcRespondEvent, 1XX
    CMceMsgBase* provMsg = MCETestHelper::ReplyMsg( 111 );
    CleanupStack::PushL( provMsg );
    iSipEvent->DoServiceL( iIds,EMceItcRespondEvent,*provMsg );
    CleanupStack::PopAndDestroy( provMsg );
   
    //EMceItcRespondEvent, 4XX
    CMceMsgBase* rejectMsg = MCETestHelper::ReplyMsg( 444 );
    CleanupStack::PushL( rejectMsg );
    iSipEvent->DoServiceL( iIds,EMceItcRespondEvent,*rejectMsg );
    CleanupStack::PopAndDestroy( rejectMsg );
    }
 
void UT_CMceSipEvent::UT_CMceSipEvent_DoServiceL6L()
    {
    //EMceItcRespondEvent, 2XX
    CMceMsgBase* acceptMsg = MCETestHelper::ReplyMsg( 222 );
    CleanupStack::PushL( acceptMsg );
    iSipEvent->DoServiceL( iIds,EMceItcRespondEvent,*acceptMsg );
    CleanupStack::PopAndDestroy( acceptMsg );
    }
    
    
void UT_CMceSipEvent::UT_CMceSipEvent_AcceptMethodL()
    {
    EUNIT_ASSERT( !iSipEvent->AcceptMethod( SIPStrings::StringF( SipStrConsts::ERegister ) ) );
    EUNIT_ASSERT( !iSipEvent->AcceptMethod( SIPStrings::StringF( SipStrConsts::EInvite ) ) );
    EUNIT_ASSERT( !iSipEvent->AcceptMethod( SIPStrings::StringF( SipStrConsts::ECancel ) ) );
    EUNIT_ASSERT( !iSipEvent->AcceptMethod( SIPStrings::StringF( SipStrConsts::EAck ) ) );
    EUNIT_ASSERT( !iSipEvent->AcceptMethod( SIPStrings::StringF( SipStrConsts::EBye ) ) );
    EUNIT_ASSERT( !iSipEvent->AcceptMethod( SIPStrings::StringF( SipStrConsts::EPrack ) ) );
    EUNIT_ASSERT( !iSipEvent->AcceptMethod( SIPStrings::StringF( SipStrConsts::EUpdate ) ) );
    EUNIT_ASSERT( iSipEvent->AcceptMethod( SIPStrings::StringF( SipStrConsts::ESubscribe ) ) );
    EUNIT_ASSERT( iSipEvent->AcceptMethod( SIPStrings::StringF( SipStrConsts::ENotify ) ) );
    EUNIT_ASSERT( iSipEvent->AcceptMethod( SIPStrings::StringF( SipStrConsts::ERefer ) ) );
    EUNIT_ASSERT( !iSipEvent->AcceptMethod( SIPStrings::StringF( SipStrConsts::EMessage ) ) );
    EUNIT_ASSERT( !iSipEvent->AcceptMethod( SIPStrings::StringF( SipStrConsts::EOptions ) ) );
    EUNIT_ASSERT( !iSipEvent->AcceptMethod( SIPStrings::StringF( SipStrConsts::ERegister ) ) );
    EUNIT_ASSERT( !iSipEvent->AcceptMethod( SIPStrings::StringF( SipStrConsts::EInfo ) ) );
    }
    
void UT_CMceSipEvent::UT_CMceSipEvent_RequestReceivedL()
    {
    // !AcceptDialogTransaction( *aTransaction )
    MCE_RESET_STUBS();
    CSIPServerTransaction* notify = 
        MCETestHelper::ServerTransactionL( SipStrConsts::ENotify );

    TBool consumed = iSipEvent->DialogRequestReceived( notify, iSipEvent->Dialog()->Dialog() );
    EUNIT_ASSERT( !consumed );
    delete notify;
    MCE_RESET_STUBS();   
   
    // acceptable method && iEvent
    CMceMsgBase* subscrMsg = MCETestHelper::EventMsg( EMceItcEventTypeSubscribe );
    CleanupStack::PushL( subscrMsg );
    iSipEvent->DoServiceL( iIds,EMceItcSubscribe,*subscrMsg );
    CleanupStack::PopAndDestroy( subscrMsg );
    EUNIT_ASSERT( iSipEvent->iEvent );
    
    RPointerArray<CSIPHeaderBase> notifyHdrs;
    CSIPHeaderBase::PushLC( &notifyHdrs );
    CSIPEventHeader* eventHdr = CSIPEventHeader::NewLC( KEvent );
    notifyHdrs.AppendL( eventHdr );
    CleanupStack::Pop( eventHdr );
    CSIPServerTransaction* notify2 = 
        MCETestHelper::ServerTransactionL( SipStrConsts::ENotify, notifyHdrs );
    CleanupStack::Pop( &notifyHdrs );
    consumed = iSipEvent->DialogRequestReceived( notify2, iSipEvent->Dialog()->Dialog() );
    if ( !consumed )
        {
        delete notify2;
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT( consumed );

    EUNIT_ASSERT( &iSipEvent->Request() == notify2 );
    MCE_RESET_STUBS();
    
    // acceptable method && !iEvent
    delete iSipEvent->iEvent;
    iSipEvent->iEvent = NULL;
    CSIPServerTransaction* refer = 
        MCETestHelper::ServerTransactionL( SipStrConsts::ERefer );
    CleanupStack::PushL( refer );
    delete iSipEvent->PopRequest();
    iSipEvent->iPendingReceivedRequests.AppendL( refer );
    CleanupStack::Pop( refer );
    iSipEvent->RequestReceived( EFalse, iSipEvent->Dialog()->Dialog() );
    MCE_CHECK_MEMORY_LEAVE( !iStorage->iSipSentResponse || 
                            iStorage->iSipSentResponse->StatusCode() != KMceSipServerInternalError )
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/,
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipServerInternalError /*sentResponse*/);
    
    MCE_RESET_STUBS();
    
    // not acceptable method
    CSIPServerTransaction* invite = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInvite );
        
    CleanupStack::PushL( invite );
    delete iSipEvent->PopRequest();
    iSipEvent->iPendingReceivedRequests.AppendL( invite );
    CleanupStack::Pop( invite );
    iSipEvent->RequestReceived( EFalse, iSipEvent->Dialog()->Dialog() );
    MCE_CHECK_MEMORY_LEAVE( !iStorage->iSipSentResponse || 
                            iStorage->iSipSentResponse->StatusCode() != KMceSipServerInternalError )

    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/,
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipServerInternalError /*sentResponse*/);
    }
    
void UT_CMceSipEvent::UT_CMceSipEvent_ResponseReceivedL()
    {
    
    CSIPClientTransaction* notify1 = NULL;
    CSIPClientTransaction* notify2 = NULL;
    
    //create  2 NOTIFYs and 
    notify1 = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::ENotify );
	iSipEvent->SetPendingTransactionL( notify1 );
    CleanupStack::Pop( notify1 );
    EUNIT_ASSERT( iSipEvent->PendingTransactions().Count() == 1 );
    MCETestHelper::SetResponseL( *notify1, KMceSipOK, SipStrConsts::EPhraseOk );
    
    notify2 = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::ENotify );
	iSipEvent->SetPendingTransactionL( notify2 );
    CleanupStack::Pop( notify2 );
    EUNIT_ASSERT( iSipEvent->PendingTransactions().Count() == 2 );
    MCETestHelper::SetResponseL( *notify2, KMceSipOK, SipStrConsts::EPhraseOk );
    
	iSipEvent->DialogResponseReceived( *notify2 );
    EUNIT_ASSERT( &iSipEvent->Response() == notify2 );
    EUNIT_ASSERT( iSipEvent->PendingTransactions().Count() == 2 );
	
    notify1->iState = CSIPTransactionBase::ECompleted;
	iSipEvent->DialogResponseReceived( *notify1 );
    EUNIT_ASSERT( iSipEvent->PendingTransactions().Count() == 1 );
    EUNIT_ASSERT( &iSipEvent->Response() == notify1 );

	iSipEvent->DialogResponseReceived( *notify2 );
    EUNIT_ASSERT( iSipEvent->PendingTransactions().Count() == 1 );
    EUNIT_ASSERT( &iSipEvent->Response() == notify2 );

    notify2->iState = CSIPTransactionBase::ECompleted;
    // Cause CMceComEvent::ProceedL() to leave 
    iSipEvent->iEvent->iLeave = ETrue;
	iSipEvent->DialogResponseReceived( *notify2 );
    EUNIT_ASSERT( iSipEvent->PendingTransactions().Count() == 0 );
    EUNIT_ASSERT( &iSipEvent->Response() == notify2 );
    }
    
void UT_CMceSipEvent::UT_CMceSipEvent_ProvisionalResponseReceivedL()
    {
    CSIPClientTransaction* notify = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::ENotify );
	iSipEvent->SetPendingTransactionL( notify );
    CleanupStack::Pop( notify );
    EUNIT_ASSERT( iSipEvent->PendingTransactions().Count() == 1 );
    MCETestHelper::SetResponseL( *notify, KMceSipRinging, SipStrConsts::EPhraseRinging );
    iSipEvent->DialogResponseReceived( *notify );
    EUNIT_ASSERT( &iSipEvent->Response() == notify );
    }
    
void UT_CMceSipEvent::UT_CMceSipEvent_RedirectionResponseReceivedL()
    {
    CSIPClientTransaction* notify = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::ENotify );
	iSipEvent->SetPendingTransactionL( notify );
    CleanupStack::Pop( notify );
    EUNIT_ASSERT( iSipEvent->PendingTransactions().Count() == 1 );
    MCETestHelper::SetResponseL( *notify, KMceSipUseProxy, SipStrConsts::EPhraseRinging );
    
    iSipEvent->DialogResponseReceived( *notify );
    EUNIT_ASSERT( &iSipEvent->Response() == notify );
    }
    
void UT_CMceSipEvent::UT_CMceSipEvent_ErrorResponseReceivedL()
    {
    CSIPClientTransaction* notify = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::ENotify );
    
	iSipEvent->SetPendingTransactionL( notify );
    CleanupStack::Pop( notify );
    
    //400
    MCETestHelper::SetResponseL( *notify, KMceSipRequestTimeout, SipStrConsts::EPhraseRequestTimeout );
    
	iSipEvent->DialogResponseReceived( *notify );
    EUNIT_ASSERT( &iSipEvent->Response() == notify );
    
    //500
    MCETestHelper::SetResponseL( *notify, KMceSipServerInternalError, SipStrConsts::EPhraseServerInternalError );
    
	iSipEvent->DialogResponseReceived( *notify );
    EUNIT_ASSERT( &iSipEvent->Response() == notify );
    
    //600
    MCETestHelper::SetResponseL( *notify, KMceSipDecline, SipStrConsts::EPhraseDecline );
    
	iSipEvent->DialogResponseReceived( *notify );
    EUNIT_ASSERT( &iSipEvent->Response() == notify );
    }
    
    
void UT_CMceSipEvent::UT_CMceSipEvent_StandAloneRequestReceivedL(  )
    {
    CSIPServerTransaction* srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EMessage );
    
	EUNIT_ASSERT( iSipEvent->MessageRequestReceived( srvtransaction ) );
    EUNIT_ASSERT( &iSipEvent->Request() == srvtransaction );
    }

void UT_CMceSipEvent::UT_CMceSipEvent_StandAloneResponseReceivedL(  )
    {
    CSIPClientTransaction* clitransaction = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::ECancel );
        
	iSipEvent->SetPendingTransactionL( clitransaction );
    CleanupStack::Pop( clitransaction );
    
    MCETestHelper::SetResponseL( *clitransaction, KMceSipOK, SipStrConsts::EPhraseOk );
    
	iSipEvent->MessageResponseReceived( *clitransaction );
    EUNIT_ASSERT( &iSipEvent->Response() == clitransaction );
    
    clitransaction->iState = CSIPTransactionBase::ECompleted;
    
    CSIPClientTransaction* clitransaction1 = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EInfo );
        
	iSipEvent->SetPendingTransactionL( clitransaction1 );
    CleanupStack::Pop( clitransaction1 );


    MCETestHelper::SetResponseL( *clitransaction1, KMceSipOK, SipStrConsts::EPhraseOk );
    
	iSipEvent->MessageResponseReceived( *clitransaction1 );
    EUNIT_ASSERT( &iSipEvent->Response() == clitransaction1 );
    }
    
void UT_CMceSipEvent::UT_CMceSipEvent_DoInitializeIncomingDialogLL(  )
    {
    // Cover the branches not covered in setups 
    CSIPServerTransaction* subscribe = 
        MCETestHelper::ServerTransactionL( SipStrConsts::ESubscribe );
    CleanupStack::PushL( subscribe );
    
    // Wrong dialog type, subscribe dialog created in setup.
    EUNIT_ASSERT_LEAVE( iSipEvent->DoInitializeIncomingDialogL( *subscribe ) );
    
    // No dialog
    delete iSipEvent->iDialog;
    iSipEvent->iDialog = NULL;
    
    EUNIT_ASSERT_LEAVE( iSipEvent->DoInitializeIncomingDialogL( *subscribe ) );
    CleanupStack::PopAndDestroy( subscribe );
    }

    
void UT_CMceSipEvent::UT_CMceSipEvent_CanDisposeL()
    {
    // iEvent && !terminated
    EUNIT_ASSERT( !iSipEvent->CanDispose() );
    
    // iEvent && terminated 
    iSipEvent->iEvent->EventContext().iCurrentState = KMceTerminatedEventStateIndex;
    EUNIT_ASSERT( iSipEvent->CanDispose() );
    
    // !iEvent
    delete iSipEvent->iEvent;
    iSipEvent->iEvent = NULL;
    EUNIT_ASSERT( iSipEvent->CanDispose() );
    }
    
    
void UT_CMceSipEvent::UT_CMceSipEvent_ConnectionStateChangedL()
    {
    iSipEvent->ConnectionStateChanged( CSIPConnection::ESuspended );
    iSipEvent->ConnectionStateChanged( CSIPConnection::EInactive );
    iSipEvent->ConnectionStateChanged( CSIPConnection::EUnavailable );
    iSipEvent->ConnectionStateChanged( CSIPConnection::EActive );
    
    // !iEvent
    delete iSipEvent->iEvent;
    iSipEvent->iEvent = NULL;
    iSipEvent->ConnectionStateChanged( CSIPConnection::ESuspended );
    }

void UT_CMceSipEvent::UT_CMceSipEvent_CreateOutgoingDialogLL()
    {
    // Cover the branches not covered in setups 
    CDesC8Array* params = new ( ELeave ) CDesC8ArrayFlat( KMceArrayGranularity );
    CSIPDialogAssocBase* dlg = NULL;
    EUNIT_ASSERT_LEAVE(  dlg = iSipEvent->CreateOutgoingDialogL( KMceDlgTypeInvite, *params ) );
    EUNIT_ASSERT( !dlg );
    delete params;
    }

void UT_CMceSipEvent::UT_CMceSipEvent_CreateOutgoingDialogL2L()
    {
    // Inside dialog
    CSIPDialog* existing = CSIPDialog::NewL();
    CleanupStack::PushL( existing );
    CSIPDialogAssocBase* dlg = NULL;
    
    // Leaving with wrong type of dialog
    CDesC8Array* params = new ( ELeave ) CDesC8ArrayFlat( KMceArrayGranularity );
    EUNIT_ASSERT_LEAVE( dlg = iSipEvent->CreateOutgoingDialogL( KMceDlgTypeInvite, *params, *existing ) );
    EUNIT_ASSERT( !dlg );
    delete params;
    
    // refer
    CDesC8Array* refParams = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
    CleanupStack::PushL( refParams );
	refParams->AppendL( KReferTo );
	dlg = iSipEvent->CreateOutgoingDialogL( KMceDlgTypeRefer, *refParams, *existing );
	CleanupStack::PopAndDestroy( refParams );
    EUNIT_ASSERT( dlg );
    EUNIT_ASSERT( dlg->Type() == SIPStrings::StringF( SipStrConsts::ERefer ) );
    
    delete dlg;
    dlg = NULL;
    
    // subscribe
    CDesC8Array* subscrParams = new ( ELeave ) CDesC8ArrayFlat( KMceArrayGranularity );
    CleanupStack::PushL( subscrParams );
    subscrParams->AppendL( KEvent );
	
	dlg = iSipEvent->CreateOutgoingDialogL( KMceDlgTypeSubscribe, *subscrParams, *existing );
	CleanupStack::PopAndDestroy( subscrParams );
    
    
    EUNIT_ASSERT( dlg );
    EUNIT_ASSERT( dlg->Type() == SIPStrings::StringF( SipStrConsts::ESubscribe ) );
    CleanupStack::PopAndDestroy( existing ); 
    delete dlg;
    }

void UT_CMceSipEvent::UT_CMceSipEvent_CreateIncomingDialogLL()
    {
    // Cases that are not covered in setups
    // Wrong transaction type
    CSIPServerTransaction* staWrongType = CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EBye ) );
    CSIPDialogAssocBase* dlg = NULL;
    EUNIT_ASSERT_LEAVE( dlg = iSipEvent->CreateIncomingDialogL( *staWrongType ) );
    EUNIT_ASSERT( !dlg );
    delete staWrongType;
    
    // With no request elements
    CSIPServerTransaction* staNoReq = CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::ESubscribe ) );
    EUNIT_ASSERT_LEAVE( dlg = iSipEvent->CreateIncomingDialogL( *staNoReq ) );
    EUNIT_ASSERT( !dlg );
    delete staNoReq;
    
    // no event header
    CSIPServerTransaction* noEventSta = MCETestHelper::ServerTransactionL( SipStrConsts::ESubscribe );
    EUNIT_ASSERT_LEAVE( dlg = iSipEvent->CreateIncomingDialogL( *noEventSta ) ); 
    EUNIT_ASSERT( !dlg );
    delete noEventSta;
    
    }

void UT_CMceSipEvent::UT_CMceSipEvent_AcceptDialogTransactionL()
    {
    // Notify dialog assoc and all the miscellaneous cases
    // no request elements
    CSIPServerTransaction* noRqstElemSta = 
        CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::ESubscribe  )); 
    EUNIT_ASSERT( !iSipEvent->AcceptDialogTransaction( *noRqstElemSta ) ); 
  	delete noRqstElemSta;
  	
    // no event header
    CSIPServerTransaction* noEventSta = MCETestHelper::ServerTransactionL( SipStrConsts::ESubscribe );
    EUNIT_ASSERT( !iSipEvent->AcceptDialogTransaction( *noEventSta ) ); 
    delete noEventSta;
    
    // wrong transaction type
    RPointerArray< CSIPHeaderBase > hdrsWrongType;
    CSIPHeaderBase::PushLC( &hdrsWrongType );
    CSIPEventHeader* eveWrongType = CSIPEventHeader::DecodeL( KEvent );
    CleanupStack::PushL( eveWrongType  );
    hdrsWrongType .AppendL( eveWrongType  );
    CleanupStack::Pop( eveWrongType );
    CSIPServerTransaction* staWrongType = MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, hdrsWrongType  );
    CleanupStack::PopAndDestroy( &hdrsWrongType );
    EUNIT_ASSERT( !iSipEvent->AcceptDialogTransaction( *staWrongType ) ); 
    delete staWrongType;
    
    // different event
    RPointerArray< CSIPHeaderBase > hdrsWrongEvent;
    CSIPHeaderBase::PushLC( &hdrsWrongEvent );
    CSIPEventHeader* eveWrongEvent = CSIPEventHeader::DecodeL( KEvent2 );
    CleanupStack::PushL( eveWrongEvent );
    hdrsWrongEvent.AppendL( eveWrongEvent );
    CleanupStack::Pop( eveWrongEvent );
    CSIPServerTransaction* staWrongEvent = MCETestHelper::ServerTransactionL( SipStrConsts::ESubscribe, hdrsWrongEvent );
    CleanupStack::PopAndDestroy( &hdrsWrongEvent );
    EUNIT_ASSERT( !iSipEvent->AcceptDialogTransaction( *staWrongEvent ) ); 
    delete staWrongEvent;
    
    // success
    RPointerArray< CSIPHeaderBase > hdrsSuccess;
    CSIPHeaderBase::PushLC( &hdrsSuccess );
    CSIPEventHeader* eveSuccess = CSIPEventHeader::DecodeL( KEvent );
    CleanupStack::PushL( eveSuccess );
    hdrsSuccess.AppendL( eveSuccess );
    CleanupStack::Pop( eveSuccess );
    CSIPServerTransaction* staSuccess = MCETestHelper::ServerTransactionL( SipStrConsts::ESubscribe, hdrsSuccess );
    CleanupStack::PopAndDestroy( &hdrsSuccess );
    EUNIT_ASSERT( iSipEvent->AcceptDialogTransaction( *staSuccess ) ); 
    
    
    // no event
    delete iSipEvent->iEvent;
    iSipEvent->iEvent = NULL;
    EUNIT_ASSERT( !iSipEvent->AcceptDialogTransaction( *staSuccess ) ); 
    
    // no dialog
    delete iSipEvent->iDialog;
    iSipEvent->iDialog = NULL;
    EUNIT_ASSERT( !iSipEvent->AcceptDialogTransaction( *staSuccess ) ); 
    delete staSuccess;
    }

void UT_CMceSipEvent::UT_CMceSipEvent_AcceptDialogTransaction2L()
    {
    // mo subscribe
    CMceMsgBase* subscrMsg = MCETestHelper::EventMsg( EMceItcEventTypeSubscribe );
    CleanupStack::PushL( subscrMsg );
    iSipEvent->DoServiceL( iIds,EMceItcSubscribe,*subscrMsg );
    CleanupStack::PopAndDestroy( subscrMsg );
    EUNIT_ASSERT( iSipEvent->iEvent );
    
    // success
    RPointerArray< CSIPHeaderBase > hdrsSuccess;
    CSIPHeaderBase::PushLC( &hdrsSuccess );
    CSIPEventHeader* eveSuccess = CSIPEventHeader::DecodeL( KEvent );
    CleanupStack::PushL( eveSuccess );
    hdrsSuccess.AppendL( eveSuccess );
    CleanupStack::Pop( eveSuccess );
    CSIPServerTransaction* staSuccess = MCETestHelper::ServerTransactionL( SipStrConsts::ENotify, hdrsSuccess );
    CleanupStack::PopAndDestroy( &hdrsSuccess );
    CleanupStack::PushL( staSuccess );
    EUNIT_ASSERT( iSipEvent->AcceptDialogTransaction( *staSuccess ) );
    CleanupStack::PopAndDestroy( staSuccess );
    
    // different event
    RPointerArray< CSIPHeaderBase > hdrsWrongEvent;
    CSIPHeaderBase::PushLC( &hdrsWrongEvent );
    CSIPEventHeader* eveWrongEvent = CSIPEventHeader::DecodeL( KEvent2 );
    CleanupStack::PushL( eveWrongEvent );
    hdrsWrongEvent.AppendL( eveWrongEvent );
    CleanupStack::Pop( eveWrongEvent );
    CSIPServerTransaction* staWrongEvent = MCETestHelper::ServerTransactionL( SipStrConsts::ESubscribe, hdrsWrongEvent );
    CleanupStack::PopAndDestroy( &hdrsWrongEvent );
    CleanupStack::PushL( staWrongEvent );
    EUNIT_ASSERT( !iSipEvent->AcceptDialogTransaction( *staWrongEvent ) ); 
    CleanupStack::PopAndDestroy( staWrongEvent );
        
    }

void UT_CMceSipEvent::UT_CMceSipEvent_AcceptDialogTransaction3L()
    {
    // mo refer
    CMceMsgBase* referMsg = MCETestHelper::EventMsg( EMceItcEventTypeRefer );
    CleanupStack::PushL( referMsg );
    iSipEvent->DoServiceL( iIds,EMceItcReferEvent,*referMsg );
    CleanupStack::PopAndDestroy( referMsg );
    EUNIT_ASSERT( iSipEvent->iEvent );
    
    // REFER dialog but iStateType == EMoSuscribe
    iSipEvent->iStateType = EMoSubscribe;
    RPointerArray< CSIPHeaderBase > hdrsWrongDlgType;
    CSIPHeaderBase::PushLC( &hdrsWrongDlgType );
    CSIPEventHeader* eveWrongDlgType = CSIPEventHeader::DecodeL( KMceSipEventPackageRefer );
    CleanupStack::PushL( eveWrongDlgType );
    hdrsWrongDlgType.AppendL( eveWrongDlgType );
    CleanupStack::Pop( eveWrongDlgType );
    CSIPServerTransaction* staWrongDlgType = MCETestHelper::ServerTransactionL( SipStrConsts::ENotify, hdrsWrongDlgType );
    CleanupStack::PopAndDestroy( &hdrsWrongDlgType );
    CleanupStack::PushL( staWrongDlgType );
    EUNIT_ASSERT( !iSipEvent->AcceptDialogTransaction( *staWrongDlgType ) );
    CleanupStack::PopAndDestroy( staWrongDlgType );
    
    // NOTIFY came, but iStateType == EMtRefer
    iSipEvent->iStateType = EMtRefer;
    RPointerArray< CSIPHeaderBase > hdrsWrongStateType;
    CSIPHeaderBase::PushLC( &hdrsWrongStateType );
    CSIPEventHeader* eveWrongStateType = CSIPEventHeader::DecodeL( KMceSipEventPackageRefer );
    CleanupStack::PushL( eveWrongStateType );
    hdrsWrongStateType.AppendL( eveWrongStateType );
    CleanupStack::Pop( eveWrongStateType );
    CSIPServerTransaction* staWrongStateType = MCETestHelper::ServerTransactionL( SipStrConsts::ENotify, hdrsWrongStateType );
    CleanupStack::PopAndDestroy( &hdrsWrongStateType );
    CleanupStack::PushL( staWrongStateType );
    EUNIT_ASSERT( !iSipEvent->AcceptDialogTransaction( *staWrongStateType ) );
    CleanupStack::PopAndDestroy( staWrongStateType );
    
    iSipEvent->iStateType = EMoRefer;
    
    // success
    RPointerArray< CSIPHeaderBase > hdrsSuccess;
    CSIPHeaderBase::PushLC( &hdrsSuccess );
    CSIPEventHeader* eveSuccess = CSIPEventHeader::DecodeL( KMceSipEventPackageRefer );
    CleanupStack::PushL( eveSuccess );
    hdrsSuccess.AppendL( eveSuccess );
    CleanupStack::Pop( eveSuccess );
    CSIPServerTransaction* staSuccess = MCETestHelper::ServerTransactionL( SipStrConsts::ENotify, hdrsSuccess );
    CleanupStack::PopAndDestroy( &hdrsSuccess );
    CleanupStack::PushL( staSuccess );
    EUNIT_ASSERT( iSipEvent->AcceptDialogTransaction( *staSuccess ) );
    CleanupStack::PopAndDestroy( staSuccess );
    
    // different event
    RPointerArray< CSIPHeaderBase > hdrsWrongEvent;
    CSIPHeaderBase::PushLC( &hdrsWrongEvent );
    CSIPEventHeader* eveWrongEvent = CSIPEventHeader::DecodeL( KEvent2 );
    CleanupStack::PushL( eveWrongEvent );
    hdrsWrongEvent.AppendL( eveWrongEvent );
    CleanupStack::Pop( eveWrongEvent );
    CSIPServerTransaction* staWrongEvent = MCETestHelper::ServerTransactionL( SipStrConsts::ESubscribe, hdrsWrongEvent );
    CleanupStack::PopAndDestroy( &hdrsWrongEvent );
    CleanupStack::PushL( staWrongEvent );
    EUNIT_ASSERT( !iSipEvent->AcceptDialogTransaction( *staWrongEvent ) ); 
    CleanupStack::PopAndDestroy( staWrongEvent );
    }

void UT_CMceSipEvent::UT_CMceSipEvent_CanceledL()
    {
    iSipEvent->Canceled();
    }

void UT_CMceSipEvent::UT_CMceSipEvent_IncomingReferLL()
    {
    HBufC8* referTo = KReferTo().AllocLC();
	CDesC8Array* hdrs = new( ELeave ) CDesC8ArrayFlat( KMceArrayGranularity );
	CleanupStack::PushL( hdrs );
	HBufC8* ct = KContentType().AllocLC();
	HBufC8* body = KSDPMessage_OK1().AllocLC();
	
	iSipEvent->IncomingReferL( referTo, hdrs, ct, body );
		
	CleanupStack::Pop( body );
	CleanupStack::Pop( ct );
	CleanupStack::Pop( hdrs );
	CleanupStack::Pop( referTo );
    }

void UT_CMceSipEvent::UT_CMceSipEvent_IncomingSubscribeLL()
    {
    CDesC8Array* hdrs = new( ELeave ) CDesC8ArrayFlat( KMceArrayGranularity );
	CleanupStack::PushL( hdrs );
	HBufC8* ct = KContentType().AllocLC();
	HBufC8* body = KSDPMessage_OK1().AllocLC();
	iSipEvent->IncomingSubscribeL( hdrs, ct, body );
		
	CleanupStack::Pop( body );
	CleanupStack::Pop( ct );
	CleanupStack::Pop( hdrs );
	}

void UT_CMceSipEvent::UT_CMceSipEvent_NotifyReceivedLL()
    {
    CDesC8Array* hdrs = new( ELeave ) CDesC8ArrayFlat( KMceArrayGranularity );
	CleanupStack::PushL( hdrs );
	HBufC8* ct = KContentType().AllocLC();
	HBufC8* body = KSDPMessage_OK1().AllocLC();
	
	iSipEvent->NotifyReceivedL( hdrs, ct, body );
		
	CleanupStack::Pop( body );
	CleanupStack::Pop( ct );
	CleanupStack::Pop( hdrs );
    }

void UT_CMceSipEvent::UT_CMceSipEvent_ClientStateChangedLL()
    {
    // Subscribe, no response
    iSipEvent->ClientStateChangedL( CMceEvent::ETerminated, EFalse );
        
    // Refer, no response
    iSipEvent->ClientStateChangedL( CMceRefer::ETerminated, EFalse );
        
    // Send Notify and receive response
    RPointerArray< CSIPHeaderBase > hdrs;
    CSIPHeaderBase::PushLC( &hdrs );
    CSIPEventHeader* eve = CSIPEventHeader::DecodeL( KEvent );
    CleanupStack::PushL( eve  );
    hdrs .AppendL( eve  );
    CleanupStack::Pop( eve );
    
    CSIPClientTransaction* notify = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::ENotify, hdrs, KMceSipOK, SipStrConsts::EPhraseOk );
    iSipEvent->SetPendingTransactionL( notify );
    iSipEvent->DialogResponseReceived( *notify );
    CleanupStack::Pop( notify );
    CleanupStack::Pop( &hdrs );
    EUNIT_ASSERT( iSipEvent->PendingTransactions().Count() == 1 );
    
    // Subscribe, response
    iSipEvent->ClientStateChangedL( CMceEvent::ETerminated, ETrue );
        
    // Refer, response
    iSipEvent->ClientStateChangedL( CMceRefer::ETerminated, ETrue );
    }

void UT_CMceSipEvent::UT_CMceSipEvent_IdleL()
    {
    CMceSipEvent::Idle( iSipEvent );
    }

    

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceSipEvent,
    "UT_CMceSipEvent",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMceSipEvent",
    "NewL",
    "FUNCTIONALITY",
    SetupMoSubscribeL, UT_CMceSipEvent_NewLL, Teardown)

EUNIT_TEST(
    "DoServiceL - not supported test ",
    "CMceSipEvent",
    "DoServiceL",
    "FUNCTIONALITY",
    SetupMoSubscribeL, UT_CMceSipEvent_DoServiceLL, Teardown)

EUNIT_TEST(
    "DoServiceL - MO Subscribe",
    "CMceSipEvent",
    "DoServiceL",
    "FUNCTIONALITY",
    SetupMoSubscribeL, UT_CMceSipEvent_DoServiceL2L, Teardown)


EUNIT_TEST(
    "DoServiceL - MO Refer",
    "CMceSipEvent",
    "DoServiceL",
    "FUNCTIONALITY",
    SetupMoReferL, UT_CMceSipEvent_DoServiceL3L, Teardown)

EUNIT_TEST(
    "DoServiceL - MT Subscribe",
    "CMceSipEvent",
    "DoServiceL",
    "FUNCTIONALITY",
    SetupMtSubscribeL, UT_CMceSipEvent_DoServiceL4L, Teardown)


EUNIT_TEST(
    "DoServiceL - MT Refer",
    "CMceSipEvent",
    "DoServiceL",
    "FUNCTIONALITY",
    SetupMtReferL, UT_CMceSipEvent_DoServiceL5L, Teardown)

EUNIT_TEST(
    "DoServiceL - MT Refer 2",
    "CMceSipEvent",
    "DoServiceL",
    "FUNCTIONALITY",
    SetupMtReferL, UT_CMceSipEvent_DoServiceL6L, Teardown)


EUNIT_TEST(
    "AcceptMethod - test ",
    "CMceSipEvent",
    "AcceptMethod",
    "FUNCTIONALITY",
    SetupMoSubscribeL, UT_CMceSipEvent_AcceptMethodL, Teardown)

    
EUNIT_TEST(
    "RequestReceived - test ",
    "CMceSipEvent",
    "RequestReceived",
    "FUNCTIONALITY",
    SetupMoSubscribeL, UT_CMceSipEvent_RequestReceivedL, Teardown)
    
    
EUNIT_TEST(
    "ResponseReceived - test ",
    "CMceSipEvent",
    "ResponseReceived",
    "FUNCTIONALITY",
    SetupMtSubscribeL, UT_CMceSipEvent_ResponseReceivedL, Teardown)

EUNIT_TEST(
    "ProvisionalResponseReceived - test ",
    "CMceSipEvent",
    "ProvisionalResponseReceived",
    "FUNCTIONALITY",
    SetupMtReferL, UT_CMceSipEvent_ProvisionalResponseReceivedL, Teardown)
    
EUNIT_TEST(
    "RedirectionResponseReceived - test ",
    "CMceSipEvent",
    "RedirectionResponseReceived",
    "FUNCTIONALITY",
    SetupMtSubscribeL, UT_CMceSipEvent_RedirectionResponseReceivedL, Teardown)

EUNIT_TEST(
    "ErrorResponseReceived - test ",
    "CMceSipEvent",
    "ErrorResponseReceived",
    "FUNCTIONALITY",
    SetupMtReferL, UT_CMceSipEvent_ErrorResponseReceivedL, Teardown)
    
EUNIT_TEST(
    "StandAloneRequestReceived - test ",
    "CMceSipEvent",
    "StandAloneRequestReceived",
    "FUNCTIONALITY",
    SetupMtSubscribeL, UT_CMceSipEvent_StandAloneRequestReceivedL, Teardown)

EUNIT_TEST(
    "StandAloneResponseReceived - test ",
    "CMceSipEvent",
    "StandAloneResponseReceived",
    "FUNCTIONALITY",
    SetupMtReferL, UT_CMceSipEvent_StandAloneResponseReceivedL, Teardown)

EUNIT_TEST(
    "DoInitializeIncomingDialogL - test ",
    "CMceSipEvent",
    "DoInitializeIncomingDialogL",
    "FUNCTIONALITY",
    SetupMoSubscribeL, UT_CMceSipEvent_DoInitializeIncomingDialogLL, Teardown)
    
EUNIT_TEST(
    "CanDispose - test ",
    "CMceSipEvent",
    "CanDispose",
    "FUNCTIONALITY",
    SetupMtSubscribeL, UT_CMceSipEvent_CanDisposeL, Teardown)
        

EUNIT_TEST(
    "ConnectionStateChanged - test ",
    "CMceSipEvent",
    "ConnectionStateChanged",
    "FUNCTIONALITY",
    SetupMtSubscribeL, UT_CMceSipEvent_ConnectionStateChangedL, Teardown)


EUNIT_TEST(
    "CreateOutgoingDialog - stand alone ",
    "CMceSipEvent",
    "CreateOutgoingDialog",
    "FUNCTIONALITY",
    SetupMoSubscribeL, UT_CMceSipEvent_CreateOutgoingDialogLL, Teardown)
    
EUNIT_TEST(
    "CreateOutgoingDialog - in existing dialog",
    "CMceSipEvent",
    "CreateOutgoingDialog",
    "FUNCTIONALITY",
    SetupMoSubscribeL, UT_CMceSipEvent_CreateOutgoingDialogL2L, Teardown)

EUNIT_TEST(
    "CreateIncomingDialog - test",
    "CMceSipEvent",
    "CreateIncomingDialog",
    "FUNCTIONALITY",
    SetupMoSubscribeL, UT_CMceSipEvent_CreateIncomingDialogLL, Teardown)


EUNIT_TEST(
    "AcceptDialogTransaction - mt subscribe/refer",
    "CMceSipEvent",
    "AcceptDialogTransaction",
    "FUNCTIONALITY",
    SetupMtSubscribeL, UT_CMceSipEvent_AcceptDialogTransactionL, Teardown)

EUNIT_TEST(
    "AcceptDialogTransaction - mo subscribe",
    "CMceSipEvent",
    "AcceptDialogTransaction",
    "FUNCTIONALITY",
    SetupMoSubscribeL, UT_CMceSipEvent_AcceptDialogTransaction2L, Teardown)
    
EUNIT_TEST(
    "AcceptDialogTransaction - mo refer",
    "CMceSipEvent",
    "AcceptDialogTransaction",
    "FUNCTIONALITY",
    SetupMoReferL, UT_CMceSipEvent_AcceptDialogTransaction3L, Teardown)

EUNIT_TEST(
    "Canceled - test",
    "CMceSipEvent",
    "Canceled",
    "FUNCTIONALITY",
    SetupMtReferL, UT_CMceSipEvent_CanceledL, Teardown)

EUNIT_TEST(
    "IncomingReferL - test",
    "CMceSipEvent",
    "IncomingReferL",
    "FUNCTIONALITY",
    SetupMtReferL, UT_CMceSipEvent_IncomingReferLL, Teardown)

EUNIT_TEST(
    "IncomingSubscribeL - test",
    "CMceSipEvent",
    "IncomingSubscribeL",
    "FUNCTIONALITY",
    SetupMtSubscribeL, UT_CMceSipEvent_IncomingSubscribeLL, Teardown)

EUNIT_TEST(
    "NotifyReceivedL - test",
    "CMceSipEvent",
    "NotifyReceivedL",
    "FUNCTIONALITY",
    SetupMtSubscribeL, UT_CMceSipEvent_NotifyReceivedLL, Teardown)

EUNIT_TEST(
    "ClientStateChangedL - test",
    "CMceSipEvent",
    "ClientStateChangedL",
    "FUNCTIONALITY",
    SetupMtSubscribeL, UT_CMceSipEvent_ClientStateChangedLL, Teardown)
            
EUNIT_TEST(
    "Idle - test",
    "CMceSipEvent",
    "Idle",
    "FUNCTIONALITY",
    SetupMtReferL, UT_CMceSipEvent_IdleL, Teardown)
    
EUNIT_END_TEST_TABLE

//  END OF FILE
