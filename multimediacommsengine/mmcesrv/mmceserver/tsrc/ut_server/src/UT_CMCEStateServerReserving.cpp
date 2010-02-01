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
#include "ut_cmcestateserverreserving.h"

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
#include "mcestateserverreserving.h"
#include "mceevent.h"
#include "mceserial.h"
#include "mcemediamanager.h"



// CONSTRUCTION
UT_CMceStateServerReserving* UT_CMceStateServerReserving::NewL()
    {
    UT_CMceStateServerReserving* self = UT_CMceStateServerReserving::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceStateServerReserving* UT_CMceStateServerReserving::NewLC()
    {
    UT_CMceStateServerReserving* self = new( ELeave ) UT_CMceStateServerReserving();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceStateServerReserving::~UT_CMceStateServerReserving()
    {
    }
 
 
// Default constructor
UT_CMceStateServerReserving::UT_CMceStateServerReserving()
    {
    }

// Second phase construct
void UT_CMceStateServerReserving::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceStateServerReserving::SetupL(  )
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
    
    MCETestHelper::RequestL( iSipSession, SipStrConsts::EInvite );
	
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EReserving /*clientState*/, 
                    KMceStateServerReserving /*serverState*/ );
    iState = static_cast<CMceStateServerReserving*>(&iSipSession->CurrentState());
    HBufC8* token = KMediaToken().AllocL();
    iSipSession->SetMediaAuthorizationToken( token );
    }

void UT_CMceStateServerReserving::Teardown()
    {
    CMCETls::Close();
	delete iSession;
    delete iServerCore;

    SdpCodecStringPool::Close();
    
    }


void UT_CMceStateServerReserving::UT_CMceStateServerReserving_AcceptLL()
    {
    CSIPClientTransaction* clitransaction = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EUpdate, KMceSipOK, SipStrConsts::EPhraseRinging, EFalse );
	iSipSession->SetPendingTransactionL( clitransaction );
    CleanupStack::Pop( clitransaction );
    
    iSipSession->iResponse = clitransaction;
    
    TMceStateTransitionEvent event1( *iSipSession, EMceUpdate );
    EUNIT_ASSERT ( iState->AcceptL( event1 ) );

    TMceStateTransitionEvent event2( *iSipSession, EMceMediaReserved );
    EUNIT_ASSERT ( iState->AcceptL( event2 ) );

    
    TMceStateTransitionEvent event3( *iSipSession, EMceResponse );
    EUNIT_ASSERT_SPECIFIC_LEAVE ( iState->AcceptL( event3 ), KErrTotalLossOfPrecision );
    }

void UT_CMceStateServerReserving::UT_CMceStateServerReserving_EntryLL()
    {
    MCE_RESET_STUBS();
    // EMceUpdate
    // success
    TMceStateTransitionEvent event1( *iSipSession, EMceUpdate );
    iState->EntryL( event1 );
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                      CMCETls::EDecode /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_EVENT( event1 /*event*/, 
                      EMceUpdate /*code*/, 
                      KErrNone /*status*/ );                      
    
    MCE_RESET_STUBS();
    
    // update fails
    iStorage->iMediaManagerUpdateStatus = KErrGeneral;
    TMceStateTransitionEvent event2( *iSipSession, EMceUpdate );
    iState->EntryL( event2 );
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                      CMCETls::EDecode /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_EVENT( event2 /*event*/, 
                      EMceUpdate /*code*/, 
                      KErrGeneral /*status*/ );                      
    
    MCE_RESET_STUBS();
    
    // EMceMediaReserved
    // need to negotiate, no update received
    TMceStateTransitionEvent event3( *iSipSession, EMceMediaReserved );
    iState->EntryL( event3 );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_EVENT( event3 /*event*/, 
                      EMceMediaReserved /*code*/, 
                      KErrNone /*status*/ );                      
    
    MCE_RESET_STUBS();
    
    // need to negotiate, update received
    MCETestHelper::RequestL( iSipSession, SipStrConsts::EUpdate );
    MCE_RESET_STUBS();
    
    TMceStateTransitionEvent event4( *iSipSession, EMceMediaReserved );
    iState->EntryL( event4 );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipOK /*sentResponse*/);
    MCE_ASSERT_EVENT( event4 /*event*/, 
                      EMceMediaReserved /*code*/, 
                      KErrNone /*status*/ );  
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EProceeding /*clientState*/, 
                       KMceStateUpdated /*serverState*/ );
    
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EReserving /*clientState*/, 
                    KMceStateServerReserving /*serverState*/ );
                                              
    
    MCE_RESET_STUBS();
    
    // no need to negotiate
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    TMceStateTransitionEvent event5( *iSipSession, EMceMediaReserved );
    iState->EntryL( event5 );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_EVENT( event5 /*event*/, 
                      EMceMediaReserved /*code*/, 
                      KErrNone /*status*/ );  
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EProceeding /*clientState*/, 
                       KMceStateUpdated /*serverState*/ );
    
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EReserving /*clientState*/, 
                    KMceStateServerReserving /*serverState*/ );
    MCE_RESET_STUBS();
    
    // default
    TMceStateTransitionEvent event6( *iSipSession, EMceResponse );
    iState->EntryL( event6 );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_EVENT( event6 /*event*/, 
                      EMceResponse /*code*/, 
                      KErrNone /*status*/ );  
                      
    MCE_RESET_STUBS();
                      
    // Enable & disable    
    
    TMceIds ids;
    CMceMsgBase* msg = NULL;
    
    MCE_ASSERT_ENDPOINT_ENABLE_AND_DISABLE();
    
    }
    
void UT_CMceStateServerReserving::UT_CMceStateServerReserving_ExitLL()
    {
    // EMceUpdate
    // error
    TMceStateTransitionEvent event1( *iSipSession, EMceUpdate );
    event1.ParamStatus() = KErrGeneral;
    iState->ExitL( event1 );
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::ETerminated /*clientState*/, 
                       KMceStateTerminated /*serverState*/ );
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EReserving /*clientState*/, 
                    KMceStateServerReserving /*serverState*/ );
    
    MCE_RESET_STUBS();
    
    // async
    TMceStateTransitionEvent event2( *iSipSession, EMceUpdate );
    event2.ParamStatus() = KMceAsync;
    iState->ExitL( event2 );
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EReserving /*clientState*/, 
                       KMceStateConfirmationRequired /*serverState*/ );
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EReserving /*clientState*/, 
                    KMceStateServerReserving /*serverState*/ );
    
    MCE_RESET_STUBS();
    
    // ready
    TMceStateTransitionEvent event3( *iSipSession, EMceUpdate );
    event3.ParamStatus() = KMceReady;
    iState->ExitL( event3 );
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EReserving /*clientState*/, 
                       KMceStateServerReserving /*serverState*/ );
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EReserving /*clientState*/, 
                    KMceStateServerReserving /*serverState*/ );
    
    MCE_RESET_STUBS();
    
    // EMceMediaReserved
    // ready
    TMceStateTransitionEvent event4( *iSipSession, EMceMediaReserved );
    iState->ExitL( event4 );
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EReserving /*clientState*/, 
                       KMceStateConfirmationRequired /*serverState*/ );
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EReserving /*clientState*/, 
                    KMceStateServerReserving /*serverState*/ );
    
    MCE_RESET_STUBS();
    
    // default
    TMceStateTransitionEvent event5( *iSipSession, EMceResponse );
    iState->ExitL( event5 );
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EReserving /*clientState*/, 
                       KMceStateServerReserving /*serverState*/ );
    MCE_RESET_STUBS();
    }
    
    
        
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceStateServerReserving,
    "UT_CMceStateServerReserving",
    "UNIT" )

EUNIT_TEST(
    "AcceptL - test ",
    "CMceStateServerReserving",
    "AcceptL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateServerReserving_AcceptLL, Teardown)

EUNIT_TEST(
    "EntryL - test ",
    "CMceStateServerReserving",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateServerReserving_EntryLL, Teardown)

EUNIT_TEST(
    "ExitL - test ",
    "CMceStateServerReserving",
    "ExitL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateServerReserving_ExitLL, Teardown)
    



EUNIT_END_TEST_TABLE

//  END OF FILE
