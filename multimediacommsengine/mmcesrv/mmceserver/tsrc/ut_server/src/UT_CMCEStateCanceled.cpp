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
#include "ut_cmcestatecanceled.h"

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
#include "mcestatecanceled.h"


// CONSTRUCTION
UT_CMceStateCanceled* UT_CMceStateCanceled::NewL()
    {
    UT_CMceStateCanceled* self = UT_CMceStateCanceled::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceStateCanceled* UT_CMceStateCanceled::NewLC()
    {
    UT_CMceStateCanceled* self = new( ELeave ) UT_CMceStateCanceled();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceStateCanceled::~UT_CMceStateCanceled()
    {
    }
 
 
// Default constructor
UT_CMceStateCanceled::UT_CMceStateCanceled()
    {
    }

// Second phase construct
void UT_CMceStateCanceled::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceStateCanceled::SetupL(  )
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
    
    iSipSession->NextState( KMceStateCanceled );
    iState = static_cast<CMceStateCanceled*>(&iSipSession->CurrentState());
    
    }

void UT_CMceStateCanceled::Teardown()
    {
    CMCETls::Close();
	delete iSession;
    delete iServerCore;
    
    }


void UT_CMceStateCanceled::UT_CMceStateCanceled_AcceptLL()
    {
        
    TMceIds ids;
    CMceMsgBase* msg = NULL;
    
    CSIPClientTransaction* clitransaction = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, KMceSipRinging, SipStrConsts::EPhraseRinging );

	iSipSession->SetPendingTransactionL( clitransaction );
    CleanupStack::Pop( clitransaction );
    
    iSipSession->iResponse = clitransaction;
    
    
    TMceStateTransitionEvent event1( *iSipSession, EMceItcCancel, ids, *msg );
    TRAPD( e1, iState->AcceptL( event1 ) );
    EUNIT_ASSERT ( e1 == KErrTotalLossOfPrecision );
    
    TMceStateTransitionEvent event2( *iSipSession, EMceResponse );
    EUNIT_ASSERT ( iState->AcceptL( event2 ) );

    TMceStateTransitionEvent event3( *iSipSession, EMceMediaSessionStopped );
    EUNIT_ASSERT ( !iState->AcceptL( event3 ) );

    TMceStateTransitionEvent event4( *iSipSession, EMceProvisionalResponse );
    EUNIT_ASSERT( iState->AcceptL( event4 ) );

    TMceStateTransitionEvent event5( *iSipSession, EMceRedirectionResponse );
    EUNIT_ASSERT( !iState->AcceptL( event5 ) );
            
    MCETestHelper::SetResponseL( *clitransaction, KMceSipTrying, SipStrConsts::EPhraseTrying );
    TMceStateTransitionEvent event6( *iSipSession, EMceProvisionalResponse );
    EUNIT_ASSERT( iState->AcceptL( event6 ) );

    MCETestHelper::SetResponseL( *clitransaction, KMceSipRinging, SipStrConsts::EPhraseRinging );
    TMceStateTransitionEvent event7( *iSipSession, EMceProvisionalResponse );
    EUNIT_ASSERT( iState->AcceptL( event7 ) );
    
            
    }

void UT_CMceStateCanceled::UT_CMceStateCanceled_EntryLL()
    {
    
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    iSipSession->SetSubState( CMceSipSession::EOffering );

    //Invite
    CSIPClientTransaction* clitransaction = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, KMceSipTrying, SipStrConsts::EPhraseTrying );
	iSipSession->SetPendingTransactionL( clitransaction );
    CleanupStack::Pop( clitransaction );
    
    iSipSession->iResponse = clitransaction;

    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 1 );


//canceled before 100 Trying
    iStorage->iSipSentMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    iStorage->iSipResponseToMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    iStorage->iMediaManagerAction = CMCETls::ENone;
    iStorage->iAckSent = EFalse;
    clitransaction->iState = CSIPTransactionBase::EProceeding;

    TMceStateTransitionEvent event5( *iSipSession, EMceProvisionalResponse );
    iState->EntryL( event5 );
    EUNIT_ASSERT ( iStorage->iMediaManagerAction == CMCETls::ENone );
    EUNIT_ASSERT ( !iStorage->iAckSent );
    EUNIT_ASSERT ( iStorage->iSipSentMethod == SIPStrings::StringF( SipStrConsts::ECancel ) );
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 2 );


//180
    iStorage->iSipSentMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    iStorage->iSipResponseToMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    iStorage->iMediaManagerAction = CMCETls::ENone;
    iStorage->iAckSent = EFalse;
    clitransaction->iState = CSIPTransactionBase::EProceeding;

    TMceStateTransitionEvent event5_1( *iSipSession, EMceProvisionalResponse );
    iState->EntryL( event5_1 );
    EUNIT_ASSERT ( iStorage->iMediaManagerAction == CMCETls::ENone );
    EUNIT_ASSERT ( !iStorage->iAckSent );
    EUNIT_ASSERT ( iStorage->iSipSentMethod == SIPStrings::StringF( SipStrConsts::EEmpty ) );
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 2 );


//200OK to INVITE    
    MCETestHelper::SetResponseL( *clitransaction, KMceSipOK, SipStrConsts::EPhraseOk );
    
    iStorage->iSipSentMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    iStorage->iSipResponseToMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    iStorage->iMediaManagerAction = CMCETls::ENone;
    
    TMceStateTransitionEvent event1( *iSipSession, EMceResponse );
    iState->EntryL( event1 );
    EUNIT_ASSERT ( iStorage->iMediaManagerAction == CMCETls::ENone );
    EUNIT_ASSERT ( iStorage->iAckSent );
    EUNIT_ASSERT ( iStorage->iSipSentMethod == SIPStrings::StringF( SipStrConsts::EBye ) );
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 3 );


//200OK to CANCEL

    MCETestHelper::SetResponseL( *iSipSession->PendingTransactions()[1], KMceSipOK, SipStrConsts::EPhraseOk );
    
    iSipSession->iResponse = iSipSession->PendingTransactions()[1];
    
    iStorage->iSipSentMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    iStorage->iSipResponseToMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    iStorage->iMediaManagerAction = CMCETls::ENone;
    iStorage->iAckSent = EFalse;

    TMceStateTransitionEvent event2( *iSipSession, EMceResponse );
    iState->EntryL( event2 );
    EUNIT_ASSERT ( iStorage->iMediaManagerAction == CMCETls::ENone );
    EUNIT_ASSERT ( !iStorage->iAckSent );
    EUNIT_ASSERT ( iStorage->iSipSentMethod == SIPStrings::StringF( SipStrConsts::EEmpty ) );
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 3 );


    iStorage->iSipSentMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    iStorage->iSipResponseToMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    iStorage->iMediaManagerAction = CMCETls::ENone;
    iStorage->iAckSent = EFalse;

    TMceStateTransitionEvent event3( *iSipSession, EMceErrorResponse );
    iState->EntryL( event3 );
    EUNIT_ASSERT ( iStorage->iMediaManagerAction == CMCETls::ENone );
    EUNIT_ASSERT ( !iStorage->iAckSent );
    EUNIT_ASSERT ( iStorage->iSipSentMethod == SIPStrings::StringF( SipStrConsts::EEmpty ) );
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 3 );

    iStorage->iSipSentMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    iStorage->iSipResponseToMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    iStorage->iMediaManagerAction = CMCETls::ENone;
    iStorage->iAckSent = EFalse;

    TMceStateTransitionEvent event4( *iSipSession, EMceMediaSessionStopped );
    iState->EntryL( event4 );
    EUNIT_ASSERT ( iStorage->iMediaManagerAction == CMCETls::ENone );
    EUNIT_ASSERT ( !iStorage->iAckSent );
    EUNIT_ASSERT ( iStorage->iSipSentMethod == SIPStrings::StringF( SipStrConsts::EEmpty ) );
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 3 );
   
    }
    
void UT_CMceStateCanceled::UT_CMceStateCanceled_ExitLL()
    {
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    iSipSession->SetSubState( CMceSipSession::EOffering );
    

    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateCanceled );

//200OK to INVITE    
    CSIPClientTransaction* clitransaction = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, KMceSipOK, SipStrConsts::EPhraseOk );
    iSipSession->SetPendingTransactionL( clitransaction );
    CleanupStack::Pop( clitransaction );
    
    iSipSession->iResponse = clitransaction;
    
    TMceStateTransitionEvent event1( *iSipSession, EMceResponse );
    iState->ExitL( event1 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateTerminating );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::ETerminating );

    iSipSession->NextState( KMceStateCanceled );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateCanceled );
    


//200OK to CANCEL

    clitransaction = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::ECancel, KMceSipOK, SipStrConsts::EPhraseOk );
    iSipSession->SetPendingTransactionL( clitransaction );
    CleanupStack::Pop( clitransaction );
    
    iSipSession->iResponse = clitransaction;

    TMceStateTransitionEvent event2( *iSipSession, EMceResponse );
    iState->ExitL( event2 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateTerminated );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::ETerminated );

    iSipSession->NextState( KMceStateCanceled );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateCanceled );

    TMceStateTransitionEvent event3( *iSipSession, EMceErrorResponse );
    iState->ExitL( event3 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateTerminated );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::ETerminated );
    }
    
    
        
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceStateCanceled,
    "UT_CMceStateCanceled",
    "UNIT" )

EUNIT_TEST(
    "AcceptL - test ",
    "CMceStateCanceled",
    "AcceptL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateCanceled_AcceptLL, Teardown)

EUNIT_TEST(
    "EntryL - test ",
    "CMceStateCanceled",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateCanceled_EntryLL, Teardown)

EUNIT_TEST(
    "ExitL - test ",
    "CMceStateCanceled",
    "ExitL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateCanceled_ExitLL, Teardown)
    



EUNIT_END_TEST_TABLE

//  END OF FILE
