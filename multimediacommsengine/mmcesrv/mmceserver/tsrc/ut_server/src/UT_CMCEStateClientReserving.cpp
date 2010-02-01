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
#include "ut_cmcestateclientreserving.h"

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
#include "mcestateclientreserving.h"
#include "mceevent.h"
#include "mceserial.h"
#include "mcemediamanager.h"



// CONSTRUCTION
UT_CMceStateClientReserving* UT_CMceStateClientReserving::NewL()
    {
    UT_CMceStateClientReserving* self = UT_CMceStateClientReserving::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceStateClientReserving* UT_CMceStateClientReserving::NewLC()
    {
    UT_CMceStateClientReserving* self = new( ELeave ) UT_CMceStateClientReserving();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }


// Destructor (virtual by CBase)
UT_CMceStateClientReserving::~UT_CMceStateClientReserving()
    {
    } 

// Default constructor
UT_CMceStateClientReserving::UT_CMceStateClientReserving()
    {
    }

// Second phase construct
void UT_CMceStateClientReserving::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceStateClientReserving::SetupL(  )
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
                    KMceStateClientReserving /*serverState*/ );
    
    iState = static_cast<CMceStateClientReserving*>(&iSipSession->CurrentState());
    }

void UT_CMceStateClientReserving::Teardown()
    {
    CMCETls::Close();
	delete iSession;
    delete iServerCore;

    SdpCodecStringPool::Close();
    
    }


void UT_CMceStateClientReserving::UT_CMceStateClientReserving_AcceptLL()
    {
    TMceIds ids;
    CMceMsgBase* msg = NULL;

    TMceStateTransitionEvent event1( *iSipSession, EMceItcCancel, ids, *msg );
    EUNIT_ASSERT ( iState->AcceptL( event1 ) );

    TMceStateTransitionEvent event2( *iSipSession, EMceMediaReserved );
    EUNIT_ASSERT ( iState->AcceptL( event2 ) );
    
    TMceStateTransitionEvent event3( *iSipSession, EMceResponse );
    EUNIT_ASSERT_SPECIFIC_LEAVE ( iState->AcceptL( event3 ), KErrTotalLossOfPrecision );
    }

void UT_CMceStateClientReserving::UT_CMceStateClientReserving_EntryLL()
    {
    TMceIds ids;
    CMceMsgBase* msg = NULL;
    
    //EMceItcCancel && subState == CMceSipSession::EOffering
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
                       KMceStateClientReserving /*serverState*/ );
    MCE_RESET_STUBS();
    
    //EMceItcCancel && subState == CMceSipSession::EUpdating
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
                       KMceStateClientReserving /*serverState*/ );
    iSipSession->SetSubState( CMceSipSession::EOffering );
    MCE_RESET_STUBS();
        
    // EMceMediaReserved && !invite->StateL() == CSIPTransactionBase::ETerminated
    TMceStateTransitionEvent event3( *iSipSession, EMceMediaReserved );
    iState->EntryL( event3 );
    
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EUpdate /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EOffering /*clientState*/, 
                       KMceStateClientReserving /*serverState*/ );
    MCE_RESET_STUBS();

    // EMceMediaReserved && invite->StateL() == CSIPTransactionBase::ETerminated
    // && !session.Actions().NeedToNegotiate()
    iSipSession->iReceivedSdpCount = 1;
    iSipSession->iSentSdpCount = 1;      
    iSipSession->InviteTransaction()->iState = CSIPTransactionBase::ETerminated;
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    TMceStateTransitionEvent event4( *iSipSession, EMceMediaReserved );
    iState->EntryL( event4 );
    
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EEstablished /*clientState*/, 
                       KMceStateEstablished /*serverState*/ );
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EOffering /*clientState*/, 
                    KMceStateClientReserving /*serverState*/ );
                           
    MCE_RESET_STUBS();
    
    // EMceMediaReserved && invite->StateL() == CSIPTransactionBase::ETerminated
    // && session.Actions().NeedToNegotiate()
    TMceStateTransitionEvent event5( *iSipSession, EMceMediaReserved );
    iState->EntryL( event5 );
    
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EInvite /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EOffering /*clientState*/, 
                       KMceStateClientReserving /*serverState*/ );
    MCE_RESET_STUBS();
    
    
    // default
    iSipSession->NextState( KMceStateClientReserving );
    TMceStateTransitionEvent event6( *iSipSession, EMceResponse );
    iState->EntryL( event6 );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EOffering /*clientState*/, 
                       KMceStateClientReserving /*serverState*/ );
                       
    MCE_RESET_STUBS();
                      
    // Enable & disable    
    
    MCE_ASSERT_ENDPOINT_ENABLE_AND_DISABLE();
    
    }
    
void UT_CMceStateClientReserving::UT_CMceStateClientReserving_ExitLL()
    {
    TMceIds ids;
    CMceMsgBase* msg = NULL;
    
    TMceStateTransitionEvent event1( *iSipSession, EMceItcCancel, ids, *msg );
    iState->ExitL( event1 );
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::ECancelling /*clientState*/, 
                       KMceStateCanceled /*serverState*/ );
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EOffering /*clientState*/, 
                    KMceStateClientReserving /*serverState*/ );
    // !update    
    TMceStateTransitionEvent event2( *iSipSession, EMceMediaReserved );
    iState->ExitL( event2 );
    
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EOffering /*clientState*/, 
                       KMceStateOffering /*serverState*/ );
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EOffering /*clientState*/, 
                    KMceStateClientReserving /*serverState*/ );
    
    // update
    CSIPClientTransaction* update = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EUpdate );
	iSipSession->SetPendingTransactionL( update );
	CleanupStack::Pop( update );
    
    TMceStateTransitionEvent event3( *iSipSession, EMceMediaReserved );
    iState->ExitL( event3 );
    
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EOffering /*clientState*/, 
                       KMceStateConfirming /*serverState*/ );
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EOffering /*clientState*/, 
                    KMceStateClientReserving /*serverState*/ );
    
    // default
    TMceStateTransitionEvent event4( *iSipSession, EMceResponse );
    iState->ExitL( event4 );
    
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EOffering /*clientState*/, 
                       KMceStateClientReserving /*serverState*/ );
    
    }
    
    
        
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceStateClientReserving,
    "UT_CMceStateClientReserving",
    "UNIT" )

EUNIT_TEST(
    "AcceptL - test ",
    "CMceStateClientReserving",
    "AcceptL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateClientReserving_AcceptLL, Teardown)

EUNIT_TEST(
    "EntryL - test ",
    "CMceStateClientReserving",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateClientReserving_EntryLL, Teardown)

EUNIT_TEST(
    "ExitL - test ",
    "CMceStateClientReserving",
    "ExitL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateClientReserving_ExitLL, Teardown)
    



EUNIT_END_TEST_TABLE

//  END OF FILE
