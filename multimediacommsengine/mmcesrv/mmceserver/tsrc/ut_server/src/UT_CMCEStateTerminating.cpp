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
#include "ut_cmcestateterminating.h"

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
#include "mcestateterminating.h"


// CONSTRUCTION
UT_CMceStateTerminating* UT_CMceStateTerminating::NewL()
    {
    UT_CMceStateTerminating* self = UT_CMceStateTerminating::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceStateTerminating* UT_CMceStateTerminating::NewLC()
    {
    UT_CMceStateTerminating* self = new( ELeave ) UT_CMceStateTerminating();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceStateTerminating::~UT_CMceStateTerminating()
    {
    }
 
 
// Default constructor
UT_CMceStateTerminating::UT_CMceStateTerminating()
    {
    }

// Second phase construct
void UT_CMceStateTerminating::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceStateTerminating::SetupL(  )
    {
    CMCETls::OpenL();
    
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

    iSipSession->NextState( KMceStateTerminating );
    iState = static_cast<CMceStateTerminating*>(&iSipSession->CurrentState());
    
    }

void UT_CMceStateTerminating::Teardown()
    {
    CMCETls::Close();
	delete iSession;
    delete iServerCore;
    
    }


void UT_CMceStateTerminating::UT_CMceStateTerminating_AcceptLL()
    {
    
    TMceIds ids;
    CMceMsgBase* msg = NULL;
    
    CSIPClientTransaction* clitransaction = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EBye, KMceSipOK, SipStrConsts::EPhraseOk );
	iSipSession->SetPendingTransactionL( clitransaction );
    CleanupStack::Pop( clitransaction );
    
    iSipSession->iResponse = clitransaction;
    
    
    TMceStateTransitionEvent event1( *iSipSession, EMceItcTerminateSession, ids, *msg );
    TRAPD( e1, iState->AcceptL( event1 ) );
    EUNIT_ASSERT ( e1 == KErrTotalLossOfPrecision );

    TMceStateTransitionEvent event2( *iSipSession, EMceAck );
    EUNIT_ASSERT ( !iState->AcceptL( event2 ) );
    TMceStateTransitionEvent event3( *iSipSession, EMceResponse );
    EUNIT_ASSERT ( iState->AcceptL( event3 ) );
    TMceStateTransitionEvent event4( *iSipSession, EMceErrorResponse );
    EUNIT_ASSERT ( iState->AcceptL( event4 ) );
    TMceStateTransitionEvent event5( *iSipSession, EMceMediaSessionStopped );
    EUNIT_ASSERT ( !iState->AcceptL( event5 ) );
    
    TMceStateTransitionEvent event6( *iSipSession, EMceMediaUpdated );
    TRAPD( e2, iState->AcceptL( event6 ) );
    EUNIT_ASSERT ( e2 == KErrTotalLossOfPrecision );
    
    }

void UT_CMceStateTerminating::UT_CMceStateTerminating_EntryLL()
    {
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    
    iStorage->iSipSentMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    iStorage->iSipResponseToMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    
    CSIPClientTransaction* clitransaction = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EBye, KMceSipOK, SipStrConsts::EPhraseOk );
	iSipSession->SetPendingTransactionL( clitransaction );
    CleanupStack::Pop( clitransaction );
    
    iSipSession->iResponse = clitransaction;
    
    TMceStateTransitionEvent event1( *iSipSession, EMceResponse );
    iState->EntryL( event1 );
    
    EUNIT_ASSERT ( iStorage->iMediaManagerAction == CMCETls::ENone );
    EUNIT_ASSERT ( iStorage->iSipSentMethod == SIPStrings::StringF( SipStrConsts::EEmpty ) );
    EUNIT_ASSERT ( iStorage->iSipResponseToMethod == SIPStrings::StringF( SipStrConsts::EEmpty ) );
    EUNIT_ASSERT ( iStorage->iSipSentMessage == NULL );
    EUNIT_ASSERT ( iStorage->iSipSentResponse == NULL );
    
    MCETestHelper::SetResponseL( *iSipSession->iResponse, KMceSipBusyHere, SipStrConsts::EPhraseBusyHere );
    
    TMceStateTransitionEvent event2( *iSipSession, EMceErrorResponse );
    iState->EntryL( event2 );
    
    EUNIT_ASSERT ( iStorage->iMediaManagerAction == CMCETls::ENone );
    EUNIT_ASSERT ( iStorage->iSipSentMethod == SIPStrings::StringF( SipStrConsts::EEmpty ) );
    EUNIT_ASSERT ( iStorage->iSipResponseToMethod == SIPStrings::StringF( SipStrConsts::EEmpty ) );
    EUNIT_ASSERT ( iStorage->iSipSentMessage == NULL );
    EUNIT_ASSERT ( iStorage->iSipSentResponse == NULL );

    TMceStateTransitionEvent event3( *iSipSession, EMceMediaSessionStopped );
    iState->EntryL( event3 );
    
    EUNIT_ASSERT ( iStorage->iMediaManagerAction == CMCETls::ENone );
    EUNIT_ASSERT ( iStorage->iSipSentMethod == SIPStrings::StringF( SipStrConsts::EEmpty ) );
    EUNIT_ASSERT ( iStorage->iSipResponseToMethod == SIPStrings::StringF( SipStrConsts::EEmpty ) );
    EUNIT_ASSERT ( iStorage->iSipSentMessage == NULL );
    EUNIT_ASSERT ( iStorage->iSipSentResponse == NULL );
    
    CSIPServerTransaction* transaction = MCETestHelper::ServerTransactionL( SipStrConsts::EBye, EFalse );
    CleanupStack::PushL( transaction );
    iSipSession->iPendingReceivedRequests.AppendL( transaction );
    CleanupStack::Pop( transaction );
    
    
    TMceStateTransitionEvent event4( *iSipSession, EMceBye );
    iState->EntryL( event4 );
    
    EUNIT_ASSERT ( iStorage->iMediaManagerAction == CMCETls::ECloseSession );
    EUNIT_ASSERT ( iStorage->iSipSentMethod == SIPStrings::StringF( SipStrConsts::EEmpty ) );
    EUNIT_ASSERT ( iStorage->iSipResponseToMethod == SIPStrings::StringF( SipStrConsts::EBye ) );
    EUNIT_ASSERT ( iStorage->iSipSentMessage == NULL );
    EUNIT_ASSERT ( iStorage->iSipSentResponse != NULL );

    
    }
    
void UT_CMceStateTerminating::UT_CMceStateTerminating_ExitLL()
    {
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateTerminating );
    
    CSIPClientTransaction* clitransaction = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EBye, KMceSipOK, SipStrConsts::EPhraseOk );
	iSipSession->SetPendingTransactionL( clitransaction );
    CleanupStack::Pop( clitransaction );
    
    iSipSession->iResponse = clitransaction;
    
    TMceStateTransitionEvent event1( *iSipSession, EMceResponse );
    iState->ExitL( event1 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateTerminated );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::ETerminated );
    

    }
    
    
        
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceStateTerminating,
    "UT_CMceStateTerminating",
    "UNIT" )


EUNIT_TEST("AcceptL - test ",
    "CMceStateTerminating",
    "AcceptL",
    "FUNCTIONALITY", 
    SetupL, UT_CMceStateTerminating_AcceptLL, Teardown)
 
EUNIT_TEST(
    "EntryL - test ",
    "CMceStateTerminating",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateTerminating_EntryLL, Teardown)

EUNIT_TEST(
    "ExitL - test ",
    "CMceStateTerminating",
    "ExitL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateTerminating_ExitLL, Teardown)
    



EUNIT_END_TEST_TABLE

//  END OF FILE
