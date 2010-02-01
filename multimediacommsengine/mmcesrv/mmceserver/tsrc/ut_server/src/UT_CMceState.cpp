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
#include "ut_cmcestate.h"

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
#include "mcestate.h"
#include "mceserial.h"


// CONSTRUCTION
UT_CMceState* UT_CMceState::NewL()
    {
    UT_CMceState* self = UT_CMceState::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceState* UT_CMceState::NewLC()
    {
    UT_CMceState* self = new( ELeave ) UT_CMceState();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceState::~UT_CMceState()
    {
    }
 
 
// Default constructor
UT_CMceState::UT_CMceState()
    {
    }

// Second phase construct
void UT_CMceState::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceState::SetupL(  )
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
    
    // For Constructing iState we need to have certain valid state
    iSipSession->NextState( KMceStateIdle );
    iState = static_cast<CMceState*>(&iSipSession->CurrentState());
    
    
    }

void UT_CMceState::Teardown()
    {
    CMCETls::Close();
	delete iSession;
    delete iServerCore;
    SdpCodecStringPool::Close();
    }




void UT_CMceState::UT_CMceState_EntryReceiveExtensionResponseL()
    {
    TMceIds ids;
    CMceMsgBase* msg = NULL;
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    
    // Test1: Failure Case Leaves with KErrTotalLossOfPrecision if response is received 
    // for Extension Rquest when dialog state is EInit or ETerminated.

    iSipSession->Dialog()->Dialog().iState = CSIPDialog::EInit; 
    
    //EMceProvisionalResponse
	TMceStateTransitionEvent event1( *iSipSession, EMceProvisionalResponse );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iState->HandleExtensionRequestEventL( event1 ),
    													 KErrTotalLossOfPrecision );
    
    //EMceResponse
    TMceStateTransitionEvent event2( *iSipSession, EMceResponse );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iState->HandleExtensionRequestEventL( event2 ),
    													 KErrTotalLossOfPrecision );
    
    //EMceRedirectionResponse
    TMceStateTransitionEvent event3( *iSipSession, EMceRedirectionResponse );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iState->HandleExtensionRequestEventL( event3 ),
    													 KErrTotalLossOfPrecision );
    
    //EMceErrorResponse
    TMceStateTransitionEvent event4( *iSipSession, EMceErrorResponse );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iState->HandleExtensionRequestEventL( event4 ),
    													 KErrTotalLossOfPrecision );    
    
    // Test2: Success Case if response is received 
    // for Extension Rquest when dialog state is not EInit or ETerminated.
    
    iSipSession->Dialog()->Dialog().iState = CSIPDialog::EConfirmed; 
    
    //EMceProvisionalResponse
    CSIPClientTransaction* clitransaction = 
    MCETestHelper::ClientTransactionLC( SipStrConsts::EInfo, KMceSipSessionProgress, 
    									SipStrConsts::EPhraseSessionProgress, ETrue );
    									
	iSipSession->SetPendingTransactionL( clitransaction );
    CleanupStack::Pop( clitransaction );
    iSipSession->iResponse = clitransaction;

    TMceStateTransitionEvent event5( *iSipSession, EMceProvisionalResponse );
    iState->HandleExtensionRequestEventL( event5 );
    
    
    //EMceResponse
    CSIPClientTransaction* clitransaction1 = 
    MCETestHelper::ClientTransactionLC( SipStrConsts::EInfo, KMceSipOK, 
    									SipStrConsts::EPhraseOk, ETrue );
	iSipSession->SetPendingTransactionL( clitransaction1 );
    CleanupStack::Pop( clitransaction1 );
    iSipSession->iResponse = clitransaction1;

    TMceStateTransitionEvent event6( *iSipSession, EMceResponse );
    iState->HandleExtensionRequestEventL( event6 );
    
    //EMceRedirectionResponse
    CSIPClientTransaction* clitransaction2 = 
    MCETestHelper::ClientTransactionLC( SipStrConsts::EInfo, KMceSipMovedTemporarily, 
    									SipStrConsts::EPhraseOk, ETrue );
	iSipSession->SetPendingTransactionL( clitransaction2 );
    CleanupStack::Pop( clitransaction2 );
    iSipSession->iResponse = clitransaction2;
    TMceStateTransitionEvent event7( *iSipSession, EMceRedirectionResponse );
    iState->HandleExtensionRequestEventL( event7 );
    
    //EMceErrorResponse
    CSIPClientTransaction* clitransaction3 = 
    MCETestHelper::ClientTransactionLC( SipStrConsts::EInfo, KMceSipSessionIntervalTooSmall, 
    									SipStrConsts::EPhraseIntervalTooBrief, ETrue );
	iSipSession->SetPendingTransactionL( clitransaction3 );
    CleanupStack::Pop( clitransaction3 );
    iSipSession->iResponse = clitransaction3;
    TMceStateTransitionEvent event8( *iSipSession, EMceErrorResponse );
    iState->HandleExtensionRequestEventL( event8 );
    }




void UT_CMceState::UT_CMceState_EntryReceiveExtensionRequestL()
    {
    TMceIds ids;
    CMceMsgBase* msg = NULL;
    
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    iSipSession->Dialog()->Dialog().iState = CSIPDialog::EConfirmed; 
    
    //case EMceRequest:

	CSIPServerTransaction* transaction = MCETestHelper::ServerTransactionL( SipStrConsts::EInfo, ETrue );
    CleanupStack::PushL( transaction );
    iSipSession->iPendingReceivedRequests.AppendL( transaction );
    CleanupStack::Pop( transaction );

	 // Handle EMceRequest Event
    
    TMceStateTransitionEvent event1( *iSipSession, EMceRequest );
    iState->HandleExtensionRequestEventL( event1 );
    
    // Ensure Receiving Extenssion Request is Handled, 
    // No Transition to State.
    EUNIT_ASSERT( iSipSession->CurrentState().Id() == KMceStateIdle )

	EUNIT_ASSERT( iSipSession->PendingReceivedRequests().Count() == 1 )    
    }


    
void UT_CMceState::UT_CMceState_EntrySendExtensionResponseL()
    {
   	TUint32 code = 200;
 	TMceIds ids;
    ids.iTransactionID = 5000;
 	iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
	iSipSession->Dialog()->Dialog().iState = CSIPDialog::EConfirmed; 

 	CMceMsgBase* msg = MCETestHelper::ReplyMsg( code );
 	msg->PushL();
    CleanupStack::PushL( msg );
 	EUNIT_ASSERT( iSipSession->PendingTransactions().Count() == 0 )

	//case EMceItcReplySend:
 	TMceStateTransitionEvent event( *iSipSession, EMceItcReplySend, ids, *msg );

    iState->HandleExtensionRequestEventL( event );
    
    // Ensure Sending of Response to the Extension Request is Handled, 
    // No Transition to State.
    EUNIT_ASSERT( iSipSession->CurrentState().Id() == KMceStateIdle )
    CleanupStack::PopAndDestroy( msg );
    }




void UT_CMceState::UT_CMceState_EntrySendExtensionRequestL()
    {
    TMceIds ids;
    CMceMsgBase* msg = NULL;
    _LIT8( KMceMethodINFO, "INFO");
    iSipSession->Dialog()->Dialog().iState = CSIPDialog::EInit; 
    
    //Test: 1 Failure Case, Sending of of Extension Request is not permited if Dialog state
    //is EInit or ETerminated.
    
    TMceStateTransitionEvent event1( *iSipSession, EMceItcRequestSend, ids, *msg );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iState->HandleExtensionRequestEventL( event1 ),
    													 KErrTotalLossOfPrecision );
    													 
	//Test: 2 Success Case Sending of Extension Request Succeed when dilog state is 
    //is not EInit or ETerminate.

    iSipSession->Dialog()->Dialog().iState = CSIPDialog::EConfirmed; 
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
  	
  	CMceMsgBase* msg1 = MCETestHelper::RequestMsg( KMceMethodINFO );
 	msg1->PushL();
    CleanupStack::PushL( msg1 );
 
    // Ensure that no pending transaction exist
    EUNIT_ASSERT( iSipSession->PendingTransactions().Count() == 0 )
    TMceStateTransitionEvent event2( *iSipSession, EMceItcRequestSend, ids, *msg1 );
	iState->HandleExtensionRequestEventL( event2 );
	
	
   // Ensure Send Request is Handled, No Transition to State.
    EUNIT_ASSERT( iSipSession->CurrentState().Id() == KMceStateIdle )

    
    // Ensure request sent as as pending transaction
    EUNIT_ASSERT( iSipSession->PendingTransactions().Count() == 1 )
   
    CleanupStack::PopAndDestroy( msg1 );
    }

    

void UT_CMceState::UT_CMceState_IsExtensionRequestEventL()
    {
    TMceIds ids;
    CMceMsgBase* msg = NULL;
    
    // Following Events are always accepted:
    TMceStateTransitionEvent event2( *iSipSession, EMceItcRequestSend, ids, *msg );
    EUNIT_ASSERT ( iState->IsExtensionRequestEvent( event2 ) == ETrue );
    
    TMceStateTransitionEvent event3( *iSipSession, EMceItcReplySend, ids, *msg );
    EUNIT_ASSERT ( iState->IsExtensionRequestEvent( event3 ) == ETrue );
    
    
    TMceStateTransitionEvent event4( *iSipSession, EMceRequest );
    EUNIT_ASSERT ( iState->IsExtensionRequestEvent( event4 ) == ETrue );


	// Following Events are accepted if the response is for the Extension Request:
	// EMceResponse:EMceProvisionalResponse:EMceRedirectionResponse: EMceErrorResponse:
	// Test-1: Fail Case i.e if the Responses are for the Normal Requests. 
	CSIPClientTransaction* clitransaction1 = 
    MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, KMceSipSessionProgress, 
    									SipStrConsts::EPhraseSessionProgress, ETrue );
    									
	iSipSession->SetPendingTransactionL( clitransaction1 );
    CleanupStack::Pop( clitransaction1 );
    iSipSession->iResponse = clitransaction1;


    TMceStateTransitionEvent event5( *iSipSession, EMceProvisionalResponse );
    EUNIT_ASSERT ( iState->IsExtensionRequestEvent( event5 ) == EFalse );
    
    TMceStateTransitionEvent event6( *iSipSession, EMceResponse );
    EUNIT_ASSERT ( iState->IsExtensionRequestEvent( event6 ) == EFalse);
    
    TMceStateTransitionEvent event7( *iSipSession, EMceRedirectionResponse );
    EUNIT_ASSERT ( iState->IsExtensionRequestEvent( event7 ) == EFalse );
    
    TMceStateTransitionEvent event8( *iSipSession, EMceErrorResponse );
    EUNIT_ASSERT ( iState->IsExtensionRequestEvent( event8 ) == EFalse );

	
	// Test-2: Success Case if the Responses are for Extension Requests. 
    CSIPClientTransaction* clitransaction2 = 
    MCETestHelper::ClientTransactionLC( SipStrConsts::EInfo, KMceSipSessionProgress, 
    									SipStrConsts::EPhraseSessionProgress, ETrue );
    									
	iSipSession->SetPendingTransactionL( clitransaction2 );
    CleanupStack::Pop( clitransaction2 );
    iSipSession->iResponse = clitransaction2;

    TMceStateTransitionEvent event10( *iSipSession, EMceProvisionalResponse );
    EUNIT_ASSERT ( iState->IsExtensionRequestEvent( event10 ) == ETrue );
    
    TMceStateTransitionEvent event9( *iSipSession, EMceResponse );
    EUNIT_ASSERT ( iState->IsExtensionRequestEvent( event9 ) == ETrue );
    
    TMceStateTransitionEvent event11( *iSipSession, EMceRedirectionResponse );
    EUNIT_ASSERT ( iState->IsExtensionRequestEvent( event11 ) == ETrue );
    
    TMceStateTransitionEvent event12( *iSipSession, EMceErrorResponse );
    EUNIT_ASSERT ( iState->IsExtensionRequestEvent( event12 ) == ETrue );
    }

    
        
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceState,
    "UT_CMceState",
    "UNIT" )

EUNIT_TEST(
    "EntryReceiveExtensionResponseL - test ",
    "EntryReceiveExtensionResponseL",
    "AcceptL",
    "FUNCTIONALITY",
    SetupL, UT_CMceState_EntryReceiveExtensionResponseL, Teardown)

EUNIT_TEST(
    "EntryReceiveExtensionRequestL - test ",
    "EntryReceiveExtensionRequestL",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceState_EntryReceiveExtensionRequestL, Teardown)

EUNIT_TEST(
    "EntrySendExtensionResponseL - test ",
    "EntrySendExtensionResponseL",
    "ExitL",
    "FUNCTIONALITY",
    SetupL, UT_CMceState_EntrySendExtensionResponseL, Teardown)

EUNIT_TEST(
    "EntrySendExtensionRequestL - test ",
    "EntrySendExtensionRequestL",
    "AcceptL",
    "FUNCTIONALITY",
    SetupL, UT_CMceState_EntrySendExtensionRequestL, Teardown)


EUNIT_TEST(
    "IsExtensionRequestEventL - test ",
    "IsExtensionRequestEventL",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceState_IsExtensionRequestEventL, Teardown)
    



EUNIT_END_TEST_TABLE

//  END OF FILE
