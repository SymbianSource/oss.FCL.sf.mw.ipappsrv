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
#include "ut_cmcestateconfirming.h"

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
#include "mcestateconfirming.h"
#include "mceevent.h"
#include "mceserial.h"
#include "mcemediamanager.h"



// CONSTRUCTION
UT_CMceStateConfirming* UT_CMceStateConfirming::NewL()
    {
    UT_CMceStateConfirming* self = UT_CMceStateConfirming::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceStateConfirming* UT_CMceStateConfirming::NewLC()
    {
    UT_CMceStateConfirming* self = new( ELeave ) UT_CMceStateConfirming();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceStateConfirming::~UT_CMceStateConfirming()
    {
    }
 
 
// Default constructor
UT_CMceStateConfirming::UT_CMceStateConfirming()
    {
    }

// Second phase construct
void UT_CMceStateConfirming::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceStateConfirming::SetupL(  )
    {
    CMCETls::OpenL();
    SdpCodecStringPool::OpenL();
    
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
    
    CMceComSession* clientSession = CMceComSession::NewL( CMceComSession::EOutSession );
    CleanupStack::PushL( clientSession );
    iSipSession->SetBodyL( clientSession );
    CleanupStack::Pop( clientSession );
    
    CSIPClientTransaction* invite = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite );
	iSipSession->SetPendingTransactionL( invite );
	CleanupStack::Pop( invite );
    
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EOffering /*clientState*/, 
                    KMceStateConfirming /*serverState*/ );
    iState = static_cast<CMceStateConfirming*>(&iSipSession->CurrentState());
    }

void UT_CMceStateConfirming::Teardown()
    {
    CMCETls::Close();
	delete iSession;
    delete iServerCore;

    SdpCodecStringPool::Close();
    
    }


void UT_CMceStateConfirming::UT_CMceStateConfirming_AcceptLL()
    {
    TMceIds ids;
    CMceMsgBase* msg = NULL;

    CSIPClientTransaction* clitransaction = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EUpdate, KMceSipOK, SipStrConsts::EPhraseRinging, EFalse );
	iSipSession->SetPendingTransactionL( clitransaction );
    CleanupStack::Pop( clitransaction );
    
    iSipSession->iResponse = clitransaction;
    
    TMceStateTransitionEvent event1( *iSipSession, EMceResponse );
    EUNIT_ASSERT ( iState->AcceptL( event1 ) );

    TMceStateTransitionEvent event2( *iSipSession, EMceProvisionalResponse );
    EUNIT_ASSERT ( iState->AcceptL( event2 ) );
    
    TMceStateTransitionEvent event3( *iSipSession, EMceErrorResponse );
    EUNIT_ASSERT ( iState->AcceptL( event3 ) );

    TMceStateTransitionEvent event4( *iSipSession, EMceRedirectionResponse );
    EUNIT_ASSERT ( iState->AcceptL( event4 ) );
    
    TMceStateTransitionEvent event5( *iSipSession, EMceItcCancel, ids, *msg );
    EUNIT_ASSERT ( iState->AcceptL( event5 ) );
    
    TMceStateTransitionEvent event6( *iSipSession, EMceMediaUpdated );
    EUNIT_ASSERT ( iState->AcceptL( event6 ) );
    
    TMceStateTransitionEvent event7( *iSipSession, EMceInvite );
    MCE_EUNIT_ASSERT_SPECIFIC_LEAVE( iState->AcceptL( event7 ), KErrTotalLossOfPrecision );
    }

void UT_CMceStateConfirming::UT_CMceStateConfirming_EntryLL()
    {
    TMceIds ids;
    CMceMsgBase* msg = NULL;

    MCE_RESET_STUBS();
    // EMceResponse
    // 2xx to INVITE, no SDP
    TInt inviteInd = iSipSession->iPendingTransactions.Find( iSipSession->InviteTransaction() );
    MCETestHelper::ResponseL( iSipSession, 
                              KMceSipOK, 
                              SipStrConsts::EPhraseOk, 
                              inviteInd,
                              EFalse );
    MCE_RESET_STUBS();
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EOffering /*clientState*/, 
                    KMceStateClientReserving /*serverState*/ );
    
    TMceStateTransitionEvent event1( *iSipSession, EMceResponse );
    iState->EntryL( event1 );
    
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EAck /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    
    MCE_RESET_STUBS();
    
    // 2xx to INVITE, SDP, no PRACK nor UPDATE pending -> leave
    iSipSession->iReceivedSdpCount = 0;
    iSipSession->iSentSdpCount = 1;    
    CSIPClientTransaction* invite = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, KMceSipOK, 
        SipStrConsts::EPhraseOk, ETrue );
    iSipSession->SetResponse( *invite );
    CleanupStack::Pop( invite );
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EOffering /*clientState*/, 
                    KMceStateClientReserving /*serverState*/ );
    
    TMceStateTransitionEvent event1_2( *iSipSession, EMceResponse );
    TRAPD( err, iState->EntryL( event1_2 ) );
    EUNIT_ASSERT( err );
    if ( err != KErrNoMemory )
        {
        MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EAck /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);    
        }
    else
    	{
    	User::Leave( KErrNoMemory );
    	}
    MCE_RESET_STUBS();
    
    // empty prack pending, 2xx to INVITE received, no need to negotiate
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    iSipSession->iReceivedSdpCount = 0;
    iSipSession->iSentSdpCount = 1;    
    TInt pendingCount = iSipSession->PendingTransactions().Count();
    CSIPClientTransaction* prack = 
        CSIPClientTransaction::NewL( SIPStrings::StringF( SipStrConsts::EPrack ) );
    CleanupStack::PushL( prack );
    iSipSession->SetPendingTransactionL( prack );
    CleanupStack::Pop( prack );
    EUNIT_ASSERT( iSipSession->PendingTransactions().Count() == pendingCount + 1 );
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EOffering /*clientState*/, 
                    KMceStateClientReserving /*serverState*/ );
    
    TMceStateTransitionEvent event1_3( *iSipSession, EMceResponse );
    iState->EntryL( event1_3 );
    EUNIT_ASSERT( iSipSession->PendingTransactions().Count() == pendingCount );
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                      CMCETls::EDecode /*mmsdpaction*/, 
                      SipStrConsts::EAck /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_EVENT( event1_3 /*event*/, 
                      EMceMediaUpdated /*code*/, 
                      KErrNone /*status*/ );                      
    
    MCE_RESET_STUBS();

    // update pending, 2xx to INVITE received, need to negotiate
    iStorage->iMediaManagerNeedToNegotiate = ETrue;
    iSipSession->iReceivedSdpCount = 0;
    iSipSession->iSentSdpCount = 1;    
    pendingCount = iSipSession->PendingTransactions().Count();
    CSIPClientTransaction* update = 
        CSIPClientTransaction::NewL( SIPStrings::StringF( SipStrConsts::EUpdate ) );
    CleanupStack::PushL( update );
    iSipSession->SetPendingTransactionL( update );
    CleanupStack::Pop( update );
    EUNIT_ASSERT( iSipSession->PendingTransactions().Count() == pendingCount + 1 );
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EOffering /*clientState*/, 
                    KMceStateClientReserving /*serverState*/ );
    
    TMceStateTransitionEvent event1_4( *iSipSession, EMceResponse );
    iState->EntryL( event1_4 );
    EUNIT_ASSERT( iSipSession->PendingTransactions().Count() == pendingCount + 1 );
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EInvite /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_EVENT( event1_4 /*event*/, 
                      EMceMediaUpdated /*code*/, 
                      KErrNone /*status*/ );                      
    
    MCE_RESET_STUBS();
    
    // update pending, 2xx to INVITE received, update async
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    pendingCount = iSipSession->PendingTransactions().Count();
    update = CSIPClientTransaction::NewL( SIPStrings::StringF( SipStrConsts::EUpdate ) );
    CleanupStack::PushL( update );
    iSipSession->SetPendingTransactionL( update );
    CleanupStack::Pop( update );
    EUNIT_ASSERT( iSipSession->PendingTransactions().Count() == pendingCount + 1 );
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EOffering /*clientState*/, 
                    KMceStateClientReserving /*serverState*/ );
    
    TMceStateTransitionEvent event1_5( *iSipSession, EMceResponse );
    iState->EntryL( event1_5 );
    EUNIT_ASSERT( iSipSession->PendingTransactions().Count() == pendingCount );
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                      CMCETls::EDecode /*mmsdpaction*/, 
                      SipStrConsts::EAck /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_EVENT( event1_5 /*event*/, 
                      EMceResponse /*code*/, 
                      KErrNone /*status*/ );                      
    
    MCE_RESET_STUBS();
    
    // update pending, 2xx to INVITE received, update async
    iStorage->iMediaManagerUpdateStatus = KErrGeneral;
    pendingCount = iSipSession->PendingTransactions().Count();
    update = CSIPClientTransaction::NewL( SIPStrings::StringF( SipStrConsts::EUpdate ) );
    CleanupStack::PushL( update );
    iSipSession->SetPendingTransactionL( update );
    CleanupStack::Pop( update );
    EUNIT_ASSERT( iSipSession->PendingTransactions().Count() == pendingCount + 1 );
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EOffering /*clientState*/, 
                    KMceStateClientReserving /*serverState*/ );
    
    
    TMceStateTransitionEvent event1_6( *iSipSession, EMceResponse );
    TRAPD( err2, iState->EntryL( event1_6 ) );
    EUNIT_ASSERT( err2 );
    if ( err2 != KErrNoMemory )
        {
        EUNIT_ASSERT( iSipSession->PendingTransactions().Count() == pendingCount );
        MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                          CMCETls::EDecode /*mmsdpaction*/, 
                          SipStrConsts::EAck /*sentMethod*/, 
                          KErrNotFound /*sentResponse*/);
        }
	else
    	{
    	User::Leave( KErrNoMemory );
    	}
    MCE_RESET_STUBS();

    // 2xx to PRACK with no SDP, update ready, need to negotiate
    prack = MCETestHelper::ClientTransactionLC( SipStrConsts::EPrack );
	iSipSession->SetPendingTransactionL( prack );
    CleanupStack::Pop( prack );
    TInt prackInd = iSipSession->iPendingTransactions.Find( prack );
    MCETestHelper::ResponseL( iSipSession,
                              KMceSipOK,
                              SipStrConsts::EPhraseOk,
                              prackInd,
                              EFalse,
                              EFalse,
                              1,
                              EFalse );
    MCE_RESET_STUBS();                       
    
    TMceStateTransitionEvent event2( *iSipSession, EMceResponse );
    iState->EntryL( event2 );
    
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_EVENT( event2 /*event*/, 
                      EMceMediaUpdated /*code*/, 
                      KErrNone /*status*/ );                      
    MCE_RESET_STUBS();
    
    // 2xx to UPDATE with SDP, update ready, no need to negotiate
    update = CSIPClientTransaction::NewL( SIPStrings::StringF( SipStrConsts::EUpdate ) );
    CleanupStack::PushL( update );
    iSipSession->SetPendingTransactionL( update );
    CleanupStack::Pop( update );
    TInt updateInd = iSipSession->iPendingTransactions.Count() - 1;
    MCETestHelper::ResponseL( iSipSession,
                              KMceSipOK,
                              SipStrConsts::EPhraseOk,
                              updateInd,
                              ETrue );
    MCE_RESET_STUBS();
    iStorage->iMediaManagerNeedToNegotiate = EFalse;         
    iSipSession->iReceivedSdpCount = 0;
    iSipSession->iSentSdpCount = 1;
    
    TMceStateTransitionEvent event3( *iSipSession, EMceResponse );
    iState->EntryL( event3 );
    
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_EVENT( event3 /*event*/, 
                      EMceMediaUpdated /*code*/, 
                      KErrNone /*status*/ );                      
    MCE_RESET_STUBS();
                              
    // 2xx to UPDATE with SDP, update not ready.
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    
    TMceStateTransitionEvent event4( *iSipSession, EMceResponse );
    iState->EntryL( event4 );
    
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                      CMCETls::EDecode /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_EVENT( event4 /*event*/, 
                      EMceResponse /*code*/, 
                      KMceAsync /*status*/ );                      
    MCE_RESET_STUBS();
    
    // 2xx to UPDATE, media manager update fails
    iStorage->iMediaManagerUpdateStatus = KErrGeneral;
    
    TMceStateTransitionEvent event5( *iSipSession, EMceResponse );
    EUNIT_ASSERT_LEAVE( iState->EntryL( event5 ) );
    
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                      CMCETls::EDecode /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_EVENT( event5 /*event*/, 
                      EMceResponse /*code*/, 
                      KErrNone /*status*/ );                      
    MCE_RESET_STUBS();
    
    // 2xx to BYE
    CSIPClientTransaction* bye = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EBye );
	iSipSession->SetPendingTransactionL( bye );
    CleanupStack::Pop( bye );
    TInt byeInd = iSipSession->iPendingTransactions.Find( bye );
    MCETestHelper::ResponseL( iSipSession,
                              KMceSipOK,
                              SipStrConsts::EPhraseOk,
                              byeInd );
    MCE_RESET_STUBS();                       
    
    TMceStateTransitionEvent event6( *iSipSession, EMceResponse );
    MCE_EUNIT_ASSERT_SPECIFIC_LEAVE( iState->EntryL( event6 ), KErrTotalLossOfPrecision );
    
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_EVENT( event6 /*event*/, 
                      EMceResponse /*code*/, 
                      KErrNone /*status*/ );     
    
    MCE_RESET_STUBS();
                      
    // Enable & disable    
    
    MCE_ASSERT_ENDPOINT_ENABLE_AND_DISABLE();     
           
    }

void UT_CMceStateConfirming::UT_CMceStateConfirming_EntryL2L()
    {
    // EMceProvisionalResponse
    TInt inviteInd = iSipSession->iPendingTransactions.Find( iSipSession->InviteTransaction() );
    MCETestHelper::ResponseL( iSipSession,
                              KMceSipRinging,
                              SipStrConsts::EPhraseRinging,
                              inviteInd );
    MCE_RESET_STUBS();
    
    TMceStateTransitionEvent event1( *iSipSession, EMceProvisionalResponse );
    iState->EntryL( event1 );
    
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_EVENT( event1 /*event*/, 
                      EMceProvisionalResponse /*code*/, 
                      KErrNone /*status*/ );                      
    }

void UT_CMceStateConfirming::UT_CMceStateConfirming_EntryL3L()
    {
    // EMceErrorResponse
    // Error response to first INVITE
    TInt inviteInd = iSipSession->iPendingTransactions.Find( iSipSession->InviteTransaction() );
    MCETestHelper::ResponseL( iSipSession,
                              KMceSipBadRequest,
                              SipStrConsts::EPhraseBadRequest,
                              inviteInd );
    MCE_RESET_STUBS();
                                  
    TMceStateTransitionEvent event1( *iSipSession, EMceErrorResponse );
    iState->EntryL( event1 );
    
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_EVENT( event1 /*event*/, 
                      EMceErrorResponse /*code*/, 
                      KErrNone /*status*/ );
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::ETerminated /*clientState*/, 
                       KMceStateTerminated /*serverState*/ );
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EOffering /*clientState*/, 
                    KMceStateConfirming /*serverState*/ );
    MCE_RESET_STUBS();
    
    // Error response to re-INVITE
    iSipSession->SetSubState( CMceSipSession::EUpdating );
    TMceStateTransitionEvent event2( *iSipSession, EMceErrorResponse );
    iState->EntryL( event2 );
    
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_EVENT( event2 /*event*/, 
                      EMceErrorResponse /*code*/, 
                      KErrNone /*status*/ );
    iSipSession->SetSubState( CMceSipSession::EOffering );                      
    }

void UT_CMceStateConfirming::UT_CMceStateConfirming_EntryL4L()
    {    
    // Error response to PRACK
    CSIPClientTransaction* prack = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EPrack );
	iSipSession->SetPendingTransactionL( prack );
    CleanupStack::Pop( prack );
    TInt prackInd = iSipSession->iPendingTransactions.Find( prack );
    MCETestHelper::ResponseL( iSipSession,
                              KMceSipBadRequest,
                              SipStrConsts::EPhraseBadRequest,
                              prackInd );
    CSIPClientTransaction* invite = iSipSession->InviteTransaction();
    invite->iState = CSIPTransactionBase::EProceeding;
    MCE_RESET_STUBS();                       
    
    TMceStateTransitionEvent event1( *iSipSession, EMceErrorResponse );
    iState->EntryL( event1 );
    
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::ECancel /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_EVENT( event1 /*event*/, 
                      EMceErrorResponse /*code*/, 
                      KErrNone /*status*/ );                      
    MCE_RESET_STUBS();
    
    // Redirection response to UPDATE
    CSIPClientTransaction* update = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EUpdate );
	iSipSession->SetPendingTransactionL( update );
    CleanupStack::Pop( update );
    TInt updateInd = iSipSession->iPendingTransactions.Find( update );
    MCETestHelper::ResponseL( iSipSession,
                              KMceSipMultipleChoices,
                              SipStrConsts::EPhraseBadRequest,
                              updateInd );
    MCE_RESET_STUBS();                       
    
    TMceStateTransitionEvent event2( *iSipSession, EMceRedirectionResponse );
    iState->EntryL( event2 );
    
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::ECancel /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_EVENT( event2 /*event*/, 
                      EMceRedirectionResponse /*code*/, 
                      KErrNone /*status*/ );                      
    MCE_RESET_STUBS();
    
    // Error response to BYE
    CSIPClientTransaction* bye = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EBye );
	iSipSession->SetPendingTransactionL( bye );
    CleanupStack::Pop( bye );
    TInt byeInd = iSipSession->iPendingTransactions.Find( bye );
    MCETestHelper::ResponseL( iSipSession,
                              KMceSipBadRequest,
                              SipStrConsts::EPhraseBadRequest,
                              byeInd );
    MCE_RESET_STUBS();                       
    
    TMceStateTransitionEvent event3( *iSipSession, EMceErrorResponse );
    MCE_EUNIT_ASSERT_SPECIFIC_LEAVE( iState->EntryL( event3 ), KErrTotalLossOfPrecision );
    
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_EVENT( event3 /*event*/, 
                      EMceErrorResponse /*code*/, 
                      KErrNone /*status*/ );                      
    }

void UT_CMceStateConfirming::UT_CMceStateConfirming_EntryL5L()
    {
    // EMceItcCancel
    TMceIds ids;
    CMceMsgBase* msg = NULL;
    
    //subState == CMceSipSession::EOffering
    iSipSession->SetSubState( CMceSipSession::EOffering );
    iSipSession->InviteTransaction()->iState = CSIPTransactionBase::EProceeding;
    TMceStateTransitionEvent event1( *iSipSession, EMceItcCancel, ids, *msg );
    iState->EntryL( event1 );
    
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::ECancel /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EOffering /*clientState*/, 
                       KMceStateConfirming /*serverState*/ );
    MCE_ASSERT_EVENT( event1 /*event*/, 
                      EMceItcCancel /*code*/, 
                      KErrNone /*status*/ );                                             
    MCE_RESET_STUBS();
    
    //subState == CMceSipSession::EUpdating
    iSipSession->iNewBodyCandidate = iSipSession->Body()->CloneL();
    
    iSipSession->SetSubState( CMceSipSession::EUpdating );
    TMceStateTransitionEvent event2( *iSipSession, EMceItcCancel, ids, *msg );
    iState->EntryL( event2 );
    
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::ECancel /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EOffering /*clientState*/, 
                       KMceStateConfirming /*serverState*/ );
    MCE_ASSERT_EVENT( event2 /*event*/, 
                      EMceItcCancel /*code*/, 
                      KErrNone /*status*/ );                      
                           
    iSipSession->SetSubState( CMceSipSession::EOffering );
    MCE_RESET_STUBS();
    
    // default
    TMceStateTransitionEvent event3( *iSipSession, EMceInvite );
    iState->EntryL( event3 );
    
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EOffering /*clientState*/, 
                       KMceStateConfirming /*serverState*/ );
    MCE_ASSERT_EVENT( event3 /*event*/, 
                      EMceInvite /*code*/, 
                      KErrNone /*status*/ );
    }

void UT_CMceStateConfirming::UT_CMceStateConfirming_EntryL6L()
    {
    // EMceMediaUpdated
    MCE_RESET_STUBS();
    // !session.Actions().NeedToNegotiate()
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    
    TMceStateTransitionEvent event1( *iSipSession, EMceMediaUpdated );
    iState->EntryL( event1 );
    
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_EVENT( event1 /*event*/, 
                      EMceMediaUpdated /*code*/, 
                      KErrNone /*status*/ );                      
    
                          
    MCE_RESET_STUBS();
    
    // session.Actions().NeedToNegotiate() && 
    // session.Dialog()->Dialog().State() != CSIPDialog::EEarly 
    iStorage->iMediaManagerNeedToNegotiate = ETrue;
    TMceStateTransitionEvent event2( *iSipSession, EMceMediaUpdated );
    iState->EntryL( event2 );
    
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EInvite /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_EVENT( event2 /*event*/, 
                      EMceMediaUpdated /*code*/, 
                      KErrNone /*status*/ );                      
    
    // session.Actions().NeedToNegotiate() && 
    // session.Dialog()->Dialog().State() == CSIPDialog::EEarly 
    iStorage->iMediaManagerNeedToNegotiate = ETrue;
    iSipSession->Dialog()->Dialog().iState = CSIPDialog::EEarly;
    TMceStateTransitionEvent event3( *iSipSession, EMceMediaUpdated );
    iState->EntryL( event3 );
    
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EUpdate /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_EVENT( event3 /*event*/, 
                      EMceMediaUpdated /*code*/, 
                      KErrNone /*status*/ );
    }
    
void UT_CMceStateConfirming::UT_CMceStateConfirming_ExitLL()
    {
    TMceIds ids;
    CMceMsgBase* msg = NULL;

    // EMceMediaUpdated
    // !update && !invite->StateL() == CSIPTransactionBase::ETerminated
    TMceStateTransitionEvent event1( *iSipSession, EMceMediaUpdated );
    iState->ExitL( event1 );
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EOffering /*clientState*/, 
                       KMceStateOffering /*serverState*/ );
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EOffering /*clientState*/, 
                    KMceStateConfirming /*serverState*/ );
    
    // !update && invite->StateL() == CSIPTransactionBase::ETerminated
    CSIPClientTransaction* invite = iSipSession->InviteTransaction();
    invite->iState = CSIPTransactionBase::ETerminated;
    MCETestHelper::ResponseL( iSipSession,
                              KMceSipOK,
                              SipStrConsts::EPhraseOk,
                              iSipSession->iPendingTransactions.Find( invite ) );
    
    TMceStateTransitionEvent event2( *iSipSession, EMceMediaUpdated );
    iState->ExitL( event2 );
    
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EEstablished /*clientState*/, 
                       KMceStateEstablished /*serverState*/ );
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EOffering /*clientState*/, 
                    KMceStateConfirming /*serverState*/ );
    
    // update
    CSIPClientTransaction* update = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EUpdate );
	iSipSession->SetPendingTransactionL( update );
    CleanupStack::Pop( update );
    
    TMceStateTransitionEvent event3( *iSipSession, EMceMediaUpdated );
    iState->ExitL( event3 );
    
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EOffering /*clientState*/, 
                       KMceStateConfirming /*serverState*/ );
    
    // EMceItcCancel
    TMceStateTransitionEvent event4( *iSipSession, EMceItcCancel, ids, *msg );
    iState->ExitL( event4 );
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::ECancelling /*clientState*/, 
                       KMceStateCanceled /*serverState*/ );
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EOffering /*clientState*/, 
                    KMceStateConfirming /*serverState*/ );
    
    // EMceErrorResponse
    TMceStateTransitionEvent event5( *iSipSession, EMceErrorResponse );
    iState->ExitL( event5 );
           
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::ECancelling /*clientState*/, 
                       KMceStateCanceled /* serverState*/ );
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EOffering /*clientState*/, 
                    KMceStateConfirming /*serverState*/ );
    
    // EMceRedirectionResponse
    TMceStateTransitionEvent event6( *iSipSession, EMceRedirectionResponse );
    iState->ExitL( event6 );
    
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::ECancelling /*clientState*/, 
                       KMceStateCanceled /*serverState*/ );
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EOffering /*clientState*/, 
                    KMceStateConfirming /*serverState*/ );
    
    // default
    TMceStateTransitionEvent event7( *iSipSession, EMceInvite );
    iState->ExitL( event7 );
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EOffering /*clientState*/, 
                       KMceStateConfirming /*serverState*/ );
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EOffering /*clientState*/, 
                    KMceStateConfirming /*serverState*/ );
    
    }
    
    
        
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceStateConfirming,
    "UT_CMceStateConfirming",
    "UNIT" )

EUNIT_TEST(
    "AcceptL - test ",
    "CMceStateConfirming",
    "AcceptL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateConfirming_AcceptLL, Teardown)

EUNIT_TEST(
    "EntryL - test ",
    "CMceStateConfirming",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateConfirming_EntryLL, Teardown)

EUNIT_TEST(
    "EntryL - provisional - test ",
    "CMceStateConfirming",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateConfirming_EntryL2L, Teardown)

EUNIT_TEST(
    "EntryL - error to INVITE - test ",
    "CMceStateConfirming",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateConfirming_EntryL3L, Teardown)

EUNIT_TEST(
    "EntryL - error to PRACK/UPDATE - test ",
    "CMceStateConfirming",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateConfirming_EntryL4L, Teardown)

EUNIT_TEST(
    "EntryL - cancel - test ",
    "CMceStateConfirming",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateConfirming_EntryL5L, Teardown)

EUNIT_TEST(
    "EntryL - updated - test ",
    "CMceStateConfirming",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateConfirming_EntryL6L, Teardown)


EUNIT_TEST(
    "ExitL - test ",
    "CMceStateConfirming",
    "ExitL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateConfirming_ExitLL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
