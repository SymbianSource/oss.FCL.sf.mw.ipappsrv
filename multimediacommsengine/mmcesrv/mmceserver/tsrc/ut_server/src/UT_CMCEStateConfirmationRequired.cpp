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
#include "ut_cmcestateconfirmationrequired.h"

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
#include "mcestateconfirmationrequired.h"
#include "mceevent.h"
#include "mceserial.h"
#include "mcemediamanager.h"



// CONSTRUCTION
UT_CMceStateConfirmationRequired* UT_CMceStateConfirmationRequired::NewL()
    {
    UT_CMceStateConfirmationRequired* self = UT_CMceStateConfirmationRequired::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceStateConfirmationRequired* UT_CMceStateConfirmationRequired::NewLC()
    {
    UT_CMceStateConfirmationRequired* self = new( ELeave ) UT_CMceStateConfirmationRequired();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceStateConfirmationRequired::~UT_CMceStateConfirmationRequired()
    {
    }
 
 
// Default constructor
UT_CMceStateConfirmationRequired::UT_CMceStateConfirmationRequired()
    {
    }

// Second phase construct
void UT_CMceStateConfirmationRequired::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CMceStateConfirmationRequired::SetupL(  )
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
    
    iSipSession->SetBodyL();
    
    MCETestHelper::RequestL( iSipSession, SipStrConsts::EUpdate, ETrue, EFalse );
	
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EReserving /*clientState*/, 
                    KMceStateConfirmationRequired /*serverState*/ );
    iState = static_cast<CMceStateConfirmationRequired*>(&iSipSession->CurrentState());
    HBufC8* token = KMediaToken().AllocL();
    iSipSession->SetMediaAuthorizationToken( token );
    MCE_RESET_STUBS();
    }

void UT_CMceStateConfirmationRequired::Teardown()
    {
    CMCETls::Close();
	delete iSession;
    delete iServerCore;

    SdpCodecStringPool::Close();
    }

void UT_CMceStateConfirmationRequired::UT_CMceStateConfirmationRequired_AcceptLL()
    {
    TMceStateTransitionEvent event1( *iSipSession, EMceUpdate );
    EUNIT_ASSERT ( iState->AcceptL( event1 ) );

    TMceStateTransitionEvent event2( *iSipSession, EMceMediaUpdated );
    EUNIT_ASSERT ( iState->AcceptL( event2 ) );

    TMceStateTransitionEvent event3( *iSipSession, EMceResponse );
    EUNIT_ASSERT_SPECIFIC_LEAVE ( iState->AcceptL( event3 ), KErrTotalLossOfPrecision );

    TMceIds ids;
    TMceStateTransitionEvent event4( *iSipSession, EMceItcRejectSession, ids );
    EUNIT_ASSERT ( iState->AcceptL( event4 ) );
    }

void UT_CMceStateConfirmationRequired::UT_CMceStateConfirmationRequired_EntryLL()
    {
    // EMceUpdate
    // update fails
    TMceStateTransitionEvent event1( *iSipSession, EMceUpdate );
    iStorage->iMediaManagerUpdateStatus = KErrGeneral;
    iState->EntryL( event1 );
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                      CMCETls::EDecode /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_EVENT( event1 /*event*/, 
                      EMceUpdate /*code*/, 
                      KErrGeneral /*status*/ );                      
    
    MCE_RESET_STUBS();
    
    // async
    TMceStateTransitionEvent event2( *iSipSession, EMceUpdate );
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    iState->EntryL( event2 );
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                      CMCETls::EDecode /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_EVENT( event2 /*event*/, 
                      EMceUpdate /*code*/, 
                      KMceAsync /*status*/ );                      
    
    MCE_RESET_STUBS();
    
    // ready, reserve async
    
    TMceStateTransitionEvent event3( *iSipSession, EMceUpdate );
    iStorage->iMediaManagerUpdateStatus = KMceReady;
    iStorage->iMediaManagerReserveStatus = KMceAsync;
    iState->EntryL( event3 );
    MCE_ASSERT_STUBS( CMCETls::EReserve /*mmaction*/, 
                      CMCETls::EDecode /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_EVENT( event3 /*event*/, 
                      EMceMediaUpdated /*code*/, 
                      KMceAsync /*status*/ );                      
    
    MCE_RESET_STUBS();
    
    // EMceMediaUpdated
    // reserve fails
    TMceStateTransitionEvent event4( *iSipSession, EMceMediaUpdated );
    iStorage->iMediaManagerReserveStatus = KErrGeneral;
    iState->EntryL( event4 );
    MCE_ASSERT_STUBS( CMCETls::EReserve /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_EVENT( event4 /*event*/, 
                      EMceMediaUpdated /*code*/, 
                      KErrGeneral /*status*/ );                      
    
    MCE_RESET_STUBS();
        
    // reserve async
    TMceStateTransitionEvent event5( *iSipSession, EMceMediaUpdated );
    iStorage->iMediaManagerReserveStatus = KMceAsync;
    iState->EntryL( event5 );
    MCE_ASSERT_STUBS( CMCETls::EReserve /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_EVENT( event5 /*event*/, 
                      EMceMediaUpdated /*code*/, 
                      KMceAsync /*status*/ );                      
    
    MCE_RESET_STUBS();
    
    // reserve ready, no update
    MCETestHelper::RequestL( iSipSession, SipStrConsts::EPrack );
    TMceStateTransitionEvent event6( *iSipSession, EMceMediaUpdated );
    iStorage->iMediaManagerReserveStatus = KMceReady;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iState->EntryL( event6 ), KErrTotalLossOfPrecision );
    MCE_ASSERT_STUBS( CMCETls::EReserve /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_EVENT( event6 /*event*/, 
                      EMceMediaUpdated /*code*/, 
                      KMceReady /*status*/ );                      
    
    MCE_RESET_STUBS();
    
    // reserve ready, update received
    MCETestHelper::RequestL( iSipSession, SipStrConsts::EUpdate, ETrue );     
    TMceStateTransitionEvent event7( *iSipSession, EMceMediaUpdated );
    iStorage->iMediaManagerReserveStatus = KMceReady;
    iState->EntryL( event7 );
    MCE_ASSERT_STUBS( CMCETls::EReserve /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipOK /*sentResponse*/);
    MCE_ASSERT_EVENT( event7 /*event*/, 
                      EMceMediaUpdated /*code*/, 
                      KMceReady /*status*/ );                      
    
    MCE_RESET_STUBS();
    
    // default
    TMceStateTransitionEvent event8( *iSipSession, EMceResponse );
    iState->EntryL( event8 );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_EVENT( event8 /*event*/, 
                      EMceResponse /*code*/, 
                      KMceReady /*status*/ );                      
    
    MCE_RESET_STUBS();

    //EMceItcRejectSession
    TMceIds ids;
    TMceStateTransitionEvent event9( *iSipSession, EMceItcRejectSession, ids );
    iStorage->iMediaManagerUpdateStatus = KMceReady;
    iState->EntryL( event9 );
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipDecline /*sentResponse*/);
    MCE_ASSERT_EVENT( event9 /*event*/, 
                      EMceItcRejectSession /*code*/, 
                      KMceReady /*status*/ );

    MCE_RESET_STUBS();
                      
    // Enable & disable    
    
//    TMceIds ids;
    CMceMsgBase* msg = NULL;
    
    MCE_ASSERT_ENDPOINT_ENABLE_AND_DISABLE();
    }
    
void UT_CMceStateConfirmationRequired::UT_CMceStateConfirmationRequired_ExitLL()
    {
    // EMceUpdate
    // error
    TMceStateTransitionEvent event1( *iSipSession, EMceUpdate );
    event1.ParamStatus() = KErrGeneral;
    iState->ExitL( event1 );
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EReserving /*clientState*/, 
                       KMceStateConfirmationRequired /*serverState*/ );
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EReserving /*clientState*/, 
                    KMceStateConfirmationRequired /*serverState*/ );
    
    MCE_RESET_STUBS();
    
    // no error, async
    TMceStateTransitionEvent event2( *iSipSession, EMceUpdate );
    iState->ExitL( event2 );
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EReserving /*clientState*/, 
                       KMceStateConfirmationRequired /*serverState*/ );
    MCE_RESET_STUBS();
    
    // EMceMediaUpdated
    // error
    TMceStateTransitionEvent event3( *iSipSession, EMceMediaUpdated );
    event3.ParamStatus() = KErrGeneral;
    iState->ExitL( event3 );
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EReserving /*clientState*/, 
                       KMceStateConfirmationRequired /*serverState*/ );
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EReserving /*clientState*/, 
                    KMceStateConfirmationRequired /*serverState*/ );
    
    MCE_RESET_STUBS();
    
    // async
    TMceStateTransitionEvent event4( *iSipSession, EMceMediaUpdated );
    event4.ParamStatus() = KMceAsync;
    iState->ExitL( event4 );
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EReserving /*clientState*/, 
                       KMceStateServerReserving /*serverState*/ );
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EReserving /*clientState*/, 
                    KMceStateConfirmationRequired /*serverState*/ );
    
    MCE_RESET_STUBS();
    
    // ready
    TMceStateTransitionEvent event5( *iSipSession, EMceMediaUpdated );
    event5.ParamStatus() = KMceReady;
    iState->ExitL( event5 );
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EProceeding /*clientState*/, 
                       KMceStateUpdated /*serverState*/ );
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EReserving /*clientState*/, 
                    KMceStateConfirmationRequired /*serverState*/ );
    
    MCE_RESET_STUBS();

    //EMceItcRejectSession
    TMceIds ids;
    TMceStateTransitionEvent event7( *iSipSession, EMceItcRejectSession, ids );
    event7.ParamStatus() = KErrGeneral;
    iState->ExitL( event7 );
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::ETerminated /*clientState*/, 
                       KMceStateTerminated /*serverState*/ );
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EReserving /*clientState*/, 
                    KMceStateConfirmationRequired /*serverState*/ );

    MCE_RESET_STUBS();
    
    // default
    TMceStateTransitionEvent event6( *iSipSession, EMceResponse );
    iState->ExitL( event6 );
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EReserving /*clientState*/, 
                       KMceStateConfirmationRequired /*serverState*/ );
    }    
        
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceStateConfirmationRequired,
    "UT_CMceStateConfirmationRequired",
    "UNIT" )

EUNIT_TEST(
    "AcceptL - test ",
    "CMceStateConfirmationRequired",
    "AcceptL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateConfirmationRequired_AcceptLL, Teardown)

EUNIT_TEST(
    "EntryL - test ",
    "CMceStateConfirmationRequired",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateConfirmationRequired_EntryLL, Teardown)

EUNIT_TEST(
    "ExitL - test ",
    "CMceStateConfirmationRequired",
    "ExitL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateConfirmationRequired_ExitLL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
