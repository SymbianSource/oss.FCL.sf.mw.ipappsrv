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
#include "ut_cmcestateupdating.h"

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
#include "mcestateupdating.h"
#include "mcemediamanager.h"

#define MCE_ENTRYL_POSTCONDITION( responseCode ) \
MCE_CHECK_MEMORY_LEAVE( !iStorage->iSipSentResponse || iStorage->iSipSentResponse->StatusCode() != responseCode )
    
// CONSTRUCTION
UT_CMceStateUpdating* UT_CMceStateUpdating::NewL()
    {
    UT_CMceStateUpdating* self = UT_CMceStateUpdating::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceStateUpdating* UT_CMceStateUpdating::NewLC()
    {
    UT_CMceStateUpdating* self = new( ELeave ) UT_CMceStateUpdating();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceStateUpdating::~UT_CMceStateUpdating()
    {
    }

// Default constructor
UT_CMceStateUpdating::UT_CMceStateUpdating()
    {
    }

// Second phase construct
void UT_CMceStateUpdating::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_CMceStateUpdating::SetupL()
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
    
    iSipSession->NextState( KMceStateUpdating );
    iState = static_cast<CMceStateUpdating*>(&iSipSession->CurrentState());
    }

void UT_CMceStateUpdating::Teardown()
    {
    CMCETls::Close();
    
    // when case UT_CMceStateUpdating_ExitLL leaves, crash occurs in next line
	delete iSession;
	iSession = NULL;
    delete iServerCore;
    iServerCore = NULL;
    SdpCodecStringPool::Close();
    }

void UT_CMceStateUpdating::UT_CMceStateUpdating_AcceptLL()
    {
    TMceIds ids;
    CMceMsgBase* msg = NULL;

    TMceStateTransitionEvent event1( *iSipSession, EMceMediaUpdated );
    EUNIT_ASSERT ( iState->AcceptL( event1 ) );

    TMceStateTransitionEvent event4( *iSipSession, EMceItcUpdate, ids, *msg );
    TRAPD( e1, iState->AcceptL( event4 ) );
    EUNIT_ASSERT ( e1 == KErrTotalLossOfPrecision );

    TMceStateTransitionEvent event5( *iSipSession, EMceResponse );
    TRAPD( e2, iState->AcceptL( event5 ) );
    EUNIT_ASSERT ( e2 == KErrTotalLossOfPrecision );
    }

void UT_CMceStateUpdating::UT_CMceStateUpdating_EntryLL()
    {
    HBufC8* offer = KSDPMessage_OK1().AllocLC();
    delete iSipSession->iOffer;
    iSipSession->iOffer = NULL;
    iSipSession->iOffer = CSdpDocument::DecodeL( *offer );
    CleanupStack::PopAndDestroy( offer );

    iSipSession->iBody = MCETestHelper::ComSessionL( 1 );
    
    CSIPServerTransaction* invite = 
            MCETestHelper::ServerTransactionL( SipStrConsts::EInvite );
    CleanupStack::PushL( invite );
    iSipSession->iPendingReceivedRequests.AppendL( invite );
    CleanupStack::Pop( invite );
    
    MCE_RESET_STUBS();
    
    TMceStateTransitionEvent event1( *iSipSession, EMceMediaUpdated );
    iState->EntryL( event1 );
    MCE_ENTRYL_POSTCONDITION( KMceSipSessionProgress )
    
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipSessionProgress /*sentResponse*/);
    
    EUNIT_ASSERT ( iStorage->iSipSentMessage == NULL );
    EUNIT_ASSERT ( iStorage->iSipSentResponse  );

    MCE_RESET_STUBS();
    delete iStorage->iSipSentResponse;
    iStorage->iSipSentResponse = NULL;
    
   
    //iSipSession->ReliableSender()->

    TMceStateTransitionEvent event2( *iSipSession, EMceMediaUpdated );
    event2.ParamStatus() = KErrGeneral;
    iState->EntryL( event2 );
    MCE_ENTRYL_POSTCONDITION( KMceSipServerInternalError )
    
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipServerInternalError /*sentResponse*/);
    
    MCE_RESET_STUBS();
    delete iStorage->iSipSentResponse;
    iStorage->iSipSentResponse = NULL;
   
   
   	/*                       
    TMceStateTransitionEvent event5( *iSipSession, TMceMediaEventCode ( KErrNotFound ) );                     
    iState->EntryL( event5 );
    
    MCE_ASSERT_STUBS( CMCETls::ECloseSession, 
                      CMCETls::ENone, 
                      SipStrConsts::EEmpty, 
                      KMceSipServerInternalError );
                      
     */                 
    MCE_RESET_STUBS();
                      
    // Enable & disable    
    
    TMceIds ids;
    CMceMsgBase* msg = NULL;
    
    MCE_ASSERT_ENDPOINT_ENABLE_AND_DISABLE();
    }

// Test FORCED SDP Sending Functionality.

void UT_CMceStateUpdating::UT_CMceStateUpdating_EntryLL2()
    {
    
    HBufC8* offer = KSDPMessage_OK1().AllocLC();
    iSipSession->iOffer = CSdpDocument::DecodeL( *offer );
    CleanupStack::PopAndDestroy( offer );

    iSipSession->iBody = MCETestHelper::ComSessionL( 1 );
    
    CSIPServerTransaction* invite = 
            MCETestHelper::ServerTransactionL( SipStrConsts::EInvite );
    CleanupStack::PushL( invite );
    iSipSession->iPendingReceivedRequests.AppendL( invite );
    CleanupStack::Pop( invite );
    
    MCE_RESET_STUBS();
    
    iSipSession->iForceSDPSending = 1;
    
    TMceStateTransitionEvent event2( *iSipSession, EMceMediaUpdated );
    iState->EntryL( event2 );
    
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipSessionProgress /*sentResponse*/);
    
    EUNIT_ASSERT ( iStorage->iSipSentMessage == NULL );
    EUNIT_ASSERT ( iStorage->iSipSentResponse  );

    MCE_RESET_STUBS();
    
    delete iStorage->iSipSentResponse;
    iStorage->iSipSentResponse = NULL;
    }
    
void UT_CMceStateUpdating::UT_CMceStateUpdating_ExitLL()
    {
    HBufC8* offer = KSDPMessage_OK1().AllocLC();
    iSipSession->iOffer = CSdpDocument::DecodeL( *offer );
    CleanupStack::PopAndDestroy( offer );

    iSipSession->iBody = MCETestHelper::ComSessionL( 1 );
    
     CSIPServerTransaction* invite = 
            MCETestHelper::ServerTransactionL( SipStrConsts::EInvite );
    CleanupStack::PushL( invite );
    iSipSession->iPendingReceivedRequests.AppendL( invite );
    CleanupStack::Pop( invite );
    
    
    TMceIds ids;
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateUpdating );
    
    TMceStateTransitionEvent event1( *iSipSession, EMceMediaUpdated );
//EUnit error access violation in this fn with alloc:
// seems to crash in Teardown() 
    iState->ExitL( event1 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateUpdated );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::EProceeding );
    
    TMceStateTransitionEvent event2( *iSipSession, EMceMediaUpdated );
    event2.ParamStatus() = KErrGeneral;
    iState->ExitL( event2 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateTerminated  );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::ETerminated );

    TMceStateTransitionEvent event3( *iSipSession, EMceMediaUpdated );
    iSipSession->iSubState = CMceSipSession::EOffering;
    iState->ExitL( event3 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateUpdated  );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::EProceeding );

    TMceStateTransitionEvent event4( *iSipSession, EMceMediaUpdated );
    iSipSession->iSubState = CMceSipSession::EOffering;
    event4.ParamStatus() = KErrGeneral;
    iState->ExitL( event4 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateTerminated );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::ETerminated );

    TMceStateTransitionEvent event5( *iSipSession, EMceMediaUpdated );
    iSipSession->iSubState = CMceSipSession::EUpdating;
    iState->ExitL( event5 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateServerOffering  );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::EProceeding );
    
    CSIPServerTransaction* update = MCETestHelper::ServerTransactionL( SipStrConsts::EUpdate, ETrue );
    CleanupStack::PushL( update );
    iSipSession->iPendingReceivedRequests.AppendL( update );
    CleanupStack::Pop( update );

    TMceStateTransitionEvent event5_2( *iSipSession, EMceMediaUpdated );
    iSipSession->iSubState = CMceSipSession::EUpdating;
    iState->ExitL( event5_2 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateEstablished  );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::EEstablished );
    delete iSipSession->PopRequest();

    CSIPServerTransaction* invite2 = MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, ETrue );
    CleanupStack::PushL( invite2 );
    iSipSession->iPendingReceivedRequests.AppendL( invite2 );
    CleanupStack::Pop( invite2 );
    
    TMceStateTransitionEvent event5_3( *iSipSession, EMceMediaUpdated );
    iSipSession->iSubState = CMceSipSession::EUpdating;
    iState->ExitL( event5_3 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateAnswering  );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::EProceeding );
    delete iSipSession->PopRequest();

    TMceStateTransitionEvent event6( *iSipSession, EMceMediaUpdated );
    iSipSession->iSubState = CMceSipSession::EUpdating;
    event6.ParamStatus() = KErrGeneral;
    iState->ExitL( event6 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateEstablished );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::EEstablished );
  
            
    TMceStateTransitionEvent event8( *iSipSession, TMceMediaEventCode ( KErrNotFound ) );
    iState->ExitL( event8 );        
    }
        
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceStateUpdating,
    "UT_CMceStateUpdating",
    "UNIT" )

EUNIT_TEST(
    "AcceptL - test ",
    "CMceStateUpdating",
    "AcceptL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateUpdating_AcceptLL, Teardown)

EUNIT_TEST(
    "EntryL - test ",
    "CMceStateUpdating",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateUpdating_EntryLL, Teardown)

EUNIT_TEST(
    "EntryL - test ",
    "CMceStateUpdating",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateUpdating_EntryLL2, Teardown)

EUNIT_TEST(
    "ExitL - test ",
    "CMceStateUpdating",
    "ExitL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateUpdating_ExitLL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
