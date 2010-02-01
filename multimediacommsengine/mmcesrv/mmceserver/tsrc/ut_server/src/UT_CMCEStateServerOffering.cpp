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
#include "ut_cmcestateserveroffering.h"

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
#include "mcestateserveroffering.h"


// CONSTRUCTION
UT_CMceStateServerOffering* UT_CMceStateServerOffering::NewL()
    {
    UT_CMceStateServerOffering* self = UT_CMceStateServerOffering::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceStateServerOffering* UT_CMceStateServerOffering::NewLC()
    {
    UT_CMceStateServerOffering* self = new( ELeave ) UT_CMceStateServerOffering();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceStateServerOffering::~UT_CMceStateServerOffering()
    {
    }

// Default constructor
UT_CMceStateServerOffering::UT_CMceStateServerOffering()
    {
    }

// Second phase construct
void UT_CMceStateServerOffering::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CMceStateServerOffering::SetupL()
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
    
    iSipSession->NextState( KMceStateServerOffering );
    iState = static_cast<CMceStateServerOffering*>(&iSipSession->CurrentState());
    }

void UT_CMceStateServerOffering::Teardown()
    {
    CMCETls::Close();
	delete iSession;
    delete iServerCore;
    SdpCodecStringPool::Close();
    }

void UT_CMceStateServerOffering::UT_CMceStateServerOffering_AcceptLL()
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
   
    TMceStateTransitionEvent event4( *iSipSession, EMceBye );
    EUNIT_ASSERT ( iState->AcceptL( event4 ) );
    
    TMceStateTransitionEvent event5( *iSipSession, EMceItcDisable, ids, *msg  );
    EUNIT_ASSERT ( iState->AcceptL( event5 ) );
    
    TMceStateTransitionEvent event6( *iSipSession, EMceItcEnable, ids, *msg  );
    EUNIT_ASSERT ( iState->AcceptL( event6 ) );
    
    TMceStateTransitionEvent event7( *iSipSession, EMceMediaUpdated );
    EUNIT_ASSERT ( iState->AcceptL( event7 ) );
    }

void UT_CMceStateServerOffering::UT_CMceStateServerOffering_EntryLL()
    {
    CMceComSession* clientSession = CMceComSession::NewL( CMceComSession::EOutSession );
    CleanupStack::PushL( clientSession );
    iSipSession->SetBodyL( clientSession );
    CleanupStack::Pop( clientSession );
   
    CSIPServerTransaction* transaction = MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, ETrue );
    CleanupStack::PushL( transaction );
    iSipSession->iPendingReceivedRequests.AppendL( transaction );
    CleanupStack::Pop( transaction );
 
    iStorage->iSipSentMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    iStorage->iSipResponseToMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
      
    iSipSession->SetSubState( CMceSipSession::EAnswering );
      
    TMceStateTransitionEvent event1( *iSipSession, EMceAck);
    iState->EntryL( event1 );

	MCE_CHECK_MEMORY_LEAVE( iStorage->iMediaManagerAction != CMCETls::EUpdate )
    EUNIT_ASSERT ( iStorage->iMediaManagerAction == CMCETls::EUpdate );
    EUNIT_ASSERT ( iStorage->iSipSentMethod == SIPStrings::StringF( SipStrConsts::EEmpty ) );
    EUNIT_ASSERT ( iStorage->iSipResponseToMethod == SIPStrings::StringF( SipStrConsts::EEmpty ) );
    EUNIT_ASSERT ( iStorage->iSipSentMessage == NULL );
    EUNIT_ASSERT ( iStorage->iSipSentResponse == NULL );
    
    MCE_RESET_STUBS();
                    
    TMceStateTransitionEvent event2( *iSipSession, EMceBye );
    iState->EntryL( event2 );
    
    EUNIT_ASSERT ( iStorage->iMediaManagerAction == CMCETls::ECloseSession );
    EUNIT_ASSERT ( iStorage->iSipSentMethod == SIPStrings::StringF( SipStrConsts::EEmpty ) );

    
    MCE_RESET_STUBS();
      
    // Enable & disable    
    
    TMceIds ids;
    CMceMsgBase* msg = NULL;
    
    MCE_ASSERT_ENDPOINT_ENABLE_AND_DISABLE();
    
    CSIPServerTransaction& request = iSipSession->Request();
    CSIPRequestElements* requestElement = const_cast<CSIPRequestElements*>(request.RequestElements());
    requestElement->MessageElements().SetContent( NULL );
    
    TMceStateTransitionEvent event3( *iSipSession, EMceAck );
    iState->EntryL( event3 );
   
    iSipSession->iBody->Streams().ResetAndDestroy();
    TMceStateTransitionEvent event4( *iSipSession, EMceAck );
    iState->EntryL( event4 );
    }
    
void UT_CMceStateServerOffering::UT_CMceStateServerOffering_ExitLL()
    {
    TMceIds ids;
    CMceMsgBase* msg = NULL;
    
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EInSession );

    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateServerOffering );

    CSIPServerTransaction* srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EBye, ETrue );
    
    CleanupStack::PushL( srvtransaction );
    iSipSession->iPendingReceivedRequests.AppendL( srvtransaction );
    CleanupStack::Pop( srvtransaction );
    
    TMceStateTransitionEvent event1( *iSipSession, EMceBye );
    iState->ExitL( event1 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateTerminated );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::ETerminated );
    
    TMceStateTransitionEvent event2( *iSipSession, EMceMediaUpdated );
    iState->ExitL( event2 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateEstablished );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::EEstablished );
    
    TMceStateTransitionEvent event3( *iSipSession, EMceItcEnable, ids, *msg );
    iState->ExitL( event3 );
   
    TMceStateTransitionEvent event4( *iSipSession, EMceItcDisable, ids, *msg );
    iState->ExitL( event4 );
    
    TMceStateTransitionEvent event5( *iSipSession, EMceItcUpdate, ids, *msg );
    iState->ExitL( event5 );
    }
    
        
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceStateServerOffering,
    "UT_CMceStateServerOffering",
    "UNIT" )

EUNIT_TEST(
    "AcceptL - test ",
    "CMceStateServerOffering",
    "AcceptL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateServerOffering_AcceptLL, Teardown)

EUNIT_TEST(
    "EntryL - test ",
    "CMceStateServerOffering",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateServerOffering_EntryLL, Teardown)

EUNIT_TEST(
    "ExitL - test ",
    "CMceStateServerOffering",
    "ExitL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateServerOffering_ExitLL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
