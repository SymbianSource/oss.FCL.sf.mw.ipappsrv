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
#include "ut_cmcestateanswering.h"

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
#include "mcestateanswering.h"


// CONSTRUCTION
UT_CMceStateAnswering* UT_CMceStateAnswering::NewL()
    {
    UT_CMceStateAnswering* self = UT_CMceStateAnswering::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceStateAnswering* UT_CMceStateAnswering::NewLC()
    {
    UT_CMceStateAnswering* self = new( ELeave ) UT_CMceStateAnswering();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceStateAnswering::~UT_CMceStateAnswering()
    {
    }
 
 
// Default constructor
UT_CMceStateAnswering::UT_CMceStateAnswering()
    {
    }

// Second phase construct
void UT_CMceStateAnswering::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceStateAnswering::SetupL(  )
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
    
    iSipSession->NextState( KMceStateAnswering );
    iState = static_cast<CMceStateAnswering*>(&iSipSession->CurrentState());
    }

void UT_CMceStateAnswering::Teardown()
    {
    CMCETls::Close();
	delete iSession;
    delete iServerCore;
    
    }


void UT_CMceStateAnswering::UT_CMceStateAnswering_AcceptLL()
    {
    TMceIds ids;
    CMceMsgBase* msg = NULL;
    
    TMceStateTransitionEvent event1( *iSipSession, EMceAck );
    EUNIT_ASSERT ( iState->AcceptL( event1 ) );

    TMceStateTransitionEvent event2( *iSipSession, EMceItcCancel, ids, *msg );
    TRAPD( e1, iState->AcceptL( event2 ) );
    EUNIT_ASSERT ( e1 == KErrTotalLossOfPrecision );

    TMceStateTransitionEvent event3( *iSipSession, EMceMediaSessionStopped );
    TRAPD( e2, iState->AcceptL( event3 ) );
    EUNIT_ASSERT ( e2 == KErrTotalLossOfPrecision );
    
    
    }

void UT_CMceStateAnswering::UT_CMceStateAnswering_EntryLL()
    {
    CMceComSession* clientSession = CMceComSession::NewL( CMceComSession::EOutSession );
    CleanupStack::PushL( clientSession );
    iSipSession->SetBodyL( clientSession );
    CleanupStack::Pop( clientSession );
    
    CSIPClientTransaction* invite = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite );
	iSipSession->SetPendingTransactionL( invite );
	CleanupStack::Pop( invite );
	
    iStorage->iSipSentMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    iStorage->iSipResponseToMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    
    TMceStateTransitionEvent event1( *iSipSession, EMceAck );
    iState->EntryL( event1 );
    
    EUNIT_ASSERT ( iStorage->iMediaManagerAction == CMCETls::ENone );
    EUNIT_ASSERT ( iStorage->iSipSentMethod == SIPStrings::StringF( SipStrConsts::EEmpty ) );
    EUNIT_ASSERT ( iStorage->iSipResponseToMethod == SIPStrings::StringF( SipStrConsts::EEmpty ) );
    EUNIT_ASSERT ( iStorage->iSipSentMessage == NULL );
    EUNIT_ASSERT ( iStorage->iSipSentResponse == NULL );
    
    MCE_RESET_STUBS();
                      
    // Enable & disable    
    
    TMceIds ids;
    CMceMsgBase* msg = NULL;
    
    MCE_ASSERT_ENDPOINT_ENABLE_AND_DISABLE();
    }
    
void UT_CMceStateAnswering::UT_CMceStateAnswering_ExitLL()
    {
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EInSession );

    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateAnswering );

    CSIPServerTransaction* srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EAck, EFalse );
    
    CleanupStack::PushL( srvtransaction );
    iSipSession->iPendingReceivedRequests.AppendL( srvtransaction );
    CleanupStack::Pop( srvtransaction );
    
    TMceStateTransitionEvent event1( *iSipSession, EMceAck );
    iState->ExitL( event1 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateEstablished );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::EEstablished );
    
    
    }
    
    
        
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceStateAnswering,
    "UT_CMceStateAnswering",
    "UNIT" )

EUNIT_TEST(
    "AcceptL - test ",
    "CMceStateAnswering",
    "AcceptL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateAnswering_AcceptLL, Teardown)

EUNIT_TEST(
    "EntryL - test ",
    "CMceStateAnswering",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateAnswering_EntryLL, Teardown)

EUNIT_TEST(
    "ExitL - test ",
    "CMceStateAnswering",
    "ExitL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateAnswering_ExitLL, Teardown)
    



EUNIT_END_TEST_TABLE

//  END OF FILE
