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
#include "ut_cmcestateidle.h"

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
#include "mcestateidle.h"
#include "mceevent.h"
#include "mceserial.h"
#include "mcemediamanager.h"
#include "mcenatsipsession.h"



// CONSTRUCTION
UT_CMceStateIdle* UT_CMceStateIdle::NewL()
    {
    UT_CMceStateIdle* self = UT_CMceStateIdle::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceStateIdle* UT_CMceStateIdle::NewLC()
    {
    UT_CMceStateIdle* self = new( ELeave ) UT_CMceStateIdle();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceStateIdle::~UT_CMceStateIdle()
    {
    }

// Default constructor
UT_CMceStateIdle::UT_CMceStateIdle()
    {
    }

// Second phase construct
void UT_CMceStateIdle::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CMceStateIdle::SetupL()
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
    iSipSession->NextState( KMceStateIdle );
    iState = static_cast<CMceStateIdle*>(&iSipSession->CurrentState());
    }

void UT_CMceStateIdle::Teardown()
    {
    CMCETls::Close();
	delete iSession;
    delete iServerCore;

    SdpCodecStringPool::Close();
    }

void UT_CMceStateIdle::UT_CMceStateIdle_AcceptLL()
    {
    TMceIds ids;
    CMceMsgBase* msg = NULL;
    
    TMceStateTransitionEvent event1( *iSipSession, EMceItcEstablishSession, ids, *msg );
    EUNIT_ASSERT ( iState->AcceptL( event1 ) );

    TMceStateTransitionEvent event2( *iSipSession, EMceInvite );
    EUNIT_ASSERT ( iState->AcceptL( event2 ) );
    
    TMceStateTransitionEvent event3( *iSipSession, EMceMediaUpdated );
    TRAPD( e1, iState->AcceptL( event3 ) );
    EUNIT_ASSERT ( e1 == KErrTotalLossOfPrecision );
    
    TMceStateTransitionEvent event4( *iSipSession, EMceItcUpdate, ids, *msg );
    TRAPD( e2, iState->AcceptL( event4 ) );
    EUNIT_ASSERT ( e2 == KErrTotalLossOfPrecision );
    
    TMceStateTransitionEvent event5( *iSipSession, EMceCancel, KErrNotFound );
    TRAPD( e3, iState->AcceptL( event5 ) );
    EUNIT_ASSERT ( e3 == KErrTotalLossOfPrecision );


    TMceStateTransitionEvent event8( *iSipSession, EMceMediaUpdated );
    TRAPD( e6, iState->AcceptL( event8 ) );
    EUNIT_ASSERT ( e6 == KErrTotalLossOfPrecision );    
    }

void UT_CMceStateIdle::UT_CMceStateIdle_EntryLL()
    {
//Invite

    CMceComSession* clientSession = CMceComSession::NewL( CMceComSession::EOutSession );
    CleanupStack::PushL( clientSession );
    CMceMsgObject<CMceComSession>* clientSessionMsg = 
        new (ELeave) CMceMsgObject<CMceComSession>( *clientSession, EMceItcMsgTypeSession );
    CleanupStack::Pop( clientSession );
    clientSessionMsg->PushL();
    CleanupStack::PushL( clientSessionMsg );
    
    TMceIds ids;
    
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    TMceStateTransitionEvent event1( *iSipSession, EMceItcEstablishSession, ids, 
                                     *clientSessionMsg );

    iState->EntryL( event1 );
    CleanupStack::PopAndDestroy( clientSessionMsg );
   
    EUNIT_ASSERT ( iStorage->iMediaManagerAction == CMCETls::EUpdate );
    EUNIT_ASSERT ( iStorage->iMediaManagerSdpAction == CMCETls::ENone );
    EUNIT_ASSERT ( iSipSession->Body() == clientSession );
    EUNIT_ASSERT ( iSipSession->Body()->iSIPContent == NULL );
    EUNIT_ASSERT ( iSipSession->Offer() == NULL );

    delete iSipSession->iBody;
    iSipSession->iBody = NULL;

    clientSession = CMceComSession::NewL( CMceComSession::EOutSession );
    CleanupStack::PushL( clientSession );
    clientSessionMsg = 
        new (ELeave) CMceMsgObject<CMceComSession>( *clientSession, EMceItcMsgTypeSession );
    CleanupStack::Pop( clientSession );
    clientSessionMsg->PushL();
    CleanupStack::PushL( clientSessionMsg );

    iStorage->iMediaManagerUpdateStatus = KMceReady;
    TMceStateTransitionEvent event1_1( *iSipSession, EMceItcEstablishSession, ids, 
                                       *clientSessionMsg );

    iState->EntryL( event1_1 );
    CleanupStack::PopAndDestroy( clientSessionMsg );
    
    EUNIT_ASSERT ( iStorage->iMediaManagerAction == CMCETls::EUpdate );
    EUNIT_ASSERT ( iStorage->iMediaManagerSdpAction == CMCETls::EEncode );
    EUNIT_ASSERT ( iSipSession->Body() == clientSession );
    EUNIT_ASSERT ( iSipSession->Body()->iSIPContent == NULL );
    EUNIT_ASSERT ( iSipSession->Offer() != NULL );
    
    
    delete iSipSession->iOffer;
    iSipSession->iOffer = NULL;
    
    
//Invited    

    iSipSession->Body()->iType = CMceComSession::EInSession;

    RPointerArray<CSIPHeaderBase> sipHeaders;
    CSIPHeaderBase::PushLC( &sipHeaders );

    // Supported: timer 
    RStringF timerKey = SIPStrings::Pool().OpenFStringL( KMceSipTimer() );
	CleanupClosePushL( timerKey );
    
    CSIPSupportedHeader* supportedHeader = CSIPSupportedHeader::NewLC( timerKey );
    
    User::LeaveIfError( sipHeaders.Append( supportedHeader ) );
    CleanupStack::Pop( supportedHeader );
    CleanupStack::PopAndDestroy();//timerKey
    
    // Session-Expires: 10
    				
    CSIPExtensionHeader* sessionExpiresHeader = 
    						CSIPExtensionHeader::NewLC( KSessionExpires(), _L8("10") );
    User::LeaveIfError( sipHeaders.Append( sessionExpiresHeader ) );
    CleanupStack::Pop( sessionExpiresHeader );

    CSIPServerTransaction* srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, sipHeaders, ETrue );
    CleanupStack::PopAndDestroy( &sipHeaders );
    CleanupStack::PushL( srvtransaction );
    iSipSession->iPendingReceivedRequests.AppendL( srvtransaction );
    CleanupStack::Pop( srvtransaction );
    
    TMceStateTransitionEvent event2( *iSipSession, EMceInvite );
    iState->EntryL( event2 );

    EUNIT_ASSERT ( iStorage->iMediaManagerAction == CMCETls::EUpdate );
    
    iStorage->iMediaManagerAction = CMCETls::ENone;
    iStorage->iMediaManagerDecodeStatus = KMceSipWarnMediaTypeNotAvailable;
    
    TMceStateTransitionEvent event3( *iSipSession, EMceInvite );
    iState->EntryL( event3 );


    EUNIT_ASSERT ( iStorage->iMediaManagerAction == CMCETls::ENone );
    EUNIT_ASSERT ( iStorage->iSipSentResponse->StatusCode() == KMceSipNotAcceptableHere );
    EUNIT_ASSERT ( iSipSession->Body() != NULL );
    EUNIT_ASSERT ( iSipSession->BodyCandidate() == NULL );
    EUNIT_ASSERT ( iSipSession->Body()->iType == CMceComSession::EInSession );
    EUNIT_ASSERT ( iSipSession->Body()->iSIPContent == NULL );
    

    TMceStateTransitionEvent event4( *iSipSession, EMceCancel );
    iState->EntryL( event4 );
    
    CSIPServerTransaction& request = iSipSession->Request();
    CSIPRequestElements* requestElement = const_cast<CSIPRequestElements*>(request.RequestElements());
    requestElement->MessageElements().SetContent( NULL );
    
    TMceStateTransitionEvent event2_2( *iSipSession, EMceInvite );
    iState->EntryL( event2_2 );
    }
    
void UT_CMceStateIdle::UT_CMceStateIdle_ExitLL()
    {
    TMceIds ids;
    CMceMsgBase* msg = NULL;
    
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    
    TMceStateTransitionEvent event1( *iSipSession, EMceItcEstablishSession, ids, *msg );
    event1.ParamStatus() = KMceAsync;
    
    iState->ExitL( event1 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateClientEstablishing );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::EOffering );
    EUNIT_ASSERT ( ids.iState == CMceSession::EOffering );
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EIdle /*clientState*/, 
                    KMceStateIdle /*serverState*/ );
    
    
    TMceStateTransitionEvent event1_1( *iSipSession, EMceItcEstablishSession, ids, *msg );
    event1_1.ParamStatus() = KMceReady;
    
    iState->ExitL( event1_1 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateOffering );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::EOffering );
    EUNIT_ASSERT ( ids.iState == CMceSession::EOffering );
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EIdle /*clientState*/, 
                    KMceStateIdle /*serverState*/ );
    
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateIdle );
    MCE_TH_SET( iSipSession->iBody, NULL );
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EInSession );
    
    TMceStateTransitionEvent event2( *iSipSession, EMceInvite );
    iState->ExitL( event2 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateServerEstablishing );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::EIncoming );

    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EIdle /*clientState*/, 
                    KMceStateIdle /*serverState*/ );
    
    TMceStateTransitionEvent event3( *iSipSession, EMceCancel );
    iState->ExitL( event3 );

    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EIdle /*clientState*/, 
                    KMceStateIdle /*serverState*/ );
    }


// INVITE received for a NAT session
void UT_CMceStateIdle::UT_CMceStateIdle_ExitL_NatSessionL()
	{
	
	CMceSipManager& manager = iServerCore->Manager();
    TUint32 profileId = 1;
    CSIPProfile& profile = manager.ProfileL( profileId );
	
	// Syncronous case
	CMceNatSipSession* natSession =
		CMceNatSipSession::NewLC( *iSession,
								  iSipSession->SIPConnection(),
								  *iSipSession->iStateMachine,
								  profile,
								  NULL );
									  
	natSession->iBody = CMceComSession::NewL( CMceComSession::EInSession );
    
    TMceIds ids;
    CMceMsgBase* msg = NULL;
  
    CSIPServerTransaction* trx = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, ETrue );
    CleanupStack::PushL(trx);
    EUNIT_ASSERT( &natSession->Request() == NULL );
    natSession->iPendingReceivedRequests.AppendL( trx );
    CleanupStack::Pop(trx);
    EUNIT_ASSERT( &natSession->Request() == trx );
        
    // TODO: remove natSession->iComSession = natSession->iBody;
    natSession->iType = CMceCsSubSession::EInSession; // make this MT case
									  
								  
	TMceStateTransitionEvent event( *natSession, EMceInvite );
	event.ParamStatus() = KMceReady;

	iState->ExitL( event );

	EUNIT_ASSERT( natSession->CurrentState().Id() ==
				  KMceStateServerInitializing );


	// Asyncronous case
	natSession->NextState( KMceStateIdle );
    event.ParamStatus() = KMceAsync;

    iState->ExitL( event );
	EUNIT_ASSERT( natSession->CurrentState().Id() ==
				  KMceStateServerInitializing );


	// Error case
	natSession->NextState( KMceStateIdle );
    event.ParamStatus() = KErrGeneral;

    iState->ExitL( event );

	EUNIT_ASSERT( natSession->CurrentState().Id() == KMceStateTerminated );
	       
	CleanupStack::PopAndDestroy( natSession );
	}
	

// Pull mode INVITE received for a NAT session
void UT_CMceStateIdle::UT_CMceStateIdle_ExitL_NatSession1L()
	{
	CMceSipManager& manager = iServerCore->Manager();
    TUint32 profileId = 1;
    CSIPProfile& profile = manager.ProfileL( profileId );
    
	// Syncronous case
	CMceNatSipSession* natSession =
		CMceNatSipSession::NewLC( *iSession,
								  iSipSession->SIPConnection(),
								  *iSipSession->iStateMachine,
								  profile,
								  NULL );
	TMceStateTransitionEvent event( *natSession, EMceInvite );
	event.ParamStatus() = KMceReady;

	iState->ExitL( event );

	EUNIT_ASSERT( natSession->CurrentState().Id() ==
				  KMceStateServerEstablishing );

	// Error case
	natSession->NextState( KMceStateIdle );
    event.ParamStatus() = KErrGeneral;

    iState->ExitL( event );

	EUNIT_ASSERT( natSession->CurrentState().Id() == KMceStateTerminated );
	       
	CleanupStack::PopAndDestroy( natSession );
	}

	
	
        
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceStateIdle,
    "UT_CMceStateIdle",
    "UNIT" )

EUNIT_TEST(
    "AcceptL - test ",
    "CMceStateIdle",
    "AcceptL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateIdle_AcceptLL, Teardown)

EUNIT_TEST(
    "EntryL - test ",
    "CMceStateIdle",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateIdle_EntryLL, Teardown)

EUNIT_TEST(
    "ExitL - test ",
    "CMceStateIdle",
    "ExitL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateIdle_ExitLL, Teardown)

EUNIT_TEST(
    "ExitL - NAT session",
    "CMceStateIdle",
    "ExitL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateIdle_ExitL_NatSessionL, Teardown)
    
EUNIT_TEST(
    "ExitL - NAT Pull Mode",
    "CMceStateIdle",
    "ExitL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateIdle_ExitL_NatSession1L, Teardown)
        

EUNIT_END_TEST_TABLE

//  END OF FILE
