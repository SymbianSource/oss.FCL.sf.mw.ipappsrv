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
#include "ut_cmcestateestablished.h"

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
#include "mcestateestablished.h"
#include "mcecomaudiostream.h"
#include "mcecomrtpsource.h"
#include "mcecomspeakersink.h"
#include "mcecomrtpsink.h"
#include "mcecommicsource.h"
#include "mcemediamanager.h"

// CONSTRUCTION
UT_CMceStateEstablished* UT_CMceStateEstablished::NewL()
    {
    UT_CMceStateEstablished* self = UT_CMceStateEstablished::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceStateEstablished* UT_CMceStateEstablished::NewLC()
    {
    UT_CMceStateEstablished* self = new( ELeave ) UT_CMceStateEstablished();
    CleanupStack::PushL( self );
    

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceStateEstablished::~UT_CMceStateEstablished()
    {
    }

// Default constructor
UT_CMceStateEstablished::UT_CMceStateEstablished()
    {
    }

// Second phase construct
void UT_CMceStateEstablished::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CMceStateEstablished::SetupL(  )
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

    iSipSession->NextState( KMceStateEstablished );
    iState = static_cast<CMceStateEstablished*>(&iSipSession->CurrentState());
    }

void UT_CMceStateEstablished::Teardown()
    {
    CMCETls::Close();
	delete iSession;
    delete iServerCore;
    SdpCodecStringPool::Close();
    }

void UT_CMceStateEstablished::UT_CMceStateEstablished_StateEntryLL()
	{
	iSipSession->IncrementReceivedSdpCount();  
    EUNIT_ASSERT_EQUALS( 1, iSipSession->iReceivedSdpCount )
    iSipSession->IncrementSentSdpCount();
    EUNIT_ASSERT_EQUALS( 1, iSipSession->iSentSdpCount )
	
	TMceStateTransitionEvent event1( *iSipSession, EMceMediaUpdated );
    iState->StateEntry( event1 );
    
    //Ensure that SDP Count get reseted in once session is in established state.
	EUNIT_ASSERT_EQUALS( 0, iSipSession->iSentSdpCount )
    EUNIT_ASSERT_EQUALS( 0, iSipSession->iReceivedSdpCount )
	}

void UT_CMceStateEstablished::UT_CMceStateEstablished_AcceptLL()
    {
    TMceIds ids;
    CMceMsgBase* msg = NULL;
   
    TMceStateTransitionEvent event3( *iSipSession, EMceItcTerminateSession, ids, *msg );
    EUNIT_ASSERT ( iState->AcceptL( event3 ) );
    TMceStateTransitionEvent event4( *iSipSession, EMceItcUpdate, ids, *msg );
    EUNIT_ASSERT ( iState->AcceptL( event4 ) );
    TMceStateTransitionEvent event7( *iSipSession, EMceItcEnable, ids, *msg );
    EUNIT_ASSERT ( iState->AcceptL( event7 ) );
    TMceStateTransitionEvent event8( *iSipSession, EMceItcDisable, ids, *msg );
    EUNIT_ASSERT ( iState->AcceptL( event8 ) );
    TMceStateTransitionEvent event9( *iSipSession, EMceItcSendRR, ids, *msg );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iState->AcceptL( event9 ), KErrTotalLossOfPrecision );
    TMceStateTransitionEvent event10( *iSipSession, EMceItcSendSR, ids, *msg );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iState->AcceptL( event10 ), KErrTotalLossOfPrecision );
    TMceStateTransitionEvent event11( *iSipSession, EMceMediaSessionStopped );
    TMceStateTransitionEvent event15( *iSipSession, EMceInvite );
    EUNIT_ASSERT ( iState->AcceptL( event15 ) );
    TMceStateTransitionEvent event16( *iSipSession, EMceBye );
    EUNIT_ASSERT ( iState->AcceptL( event16 ) );
    TMceStateTransitionEvent event17( *iSipSession, EMceSessionExpired );
    EUNIT_ASSERT ( iState->AcceptL( event17 ) );
    TMceStateTransitionEvent event18( *iSipSession, EMceSessionRefresh );
    EUNIT_ASSERT ( iState->AcceptL( event18 ) );
    
    CSIPClientTransaction* clitransaction = 
    MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, KMceSipOK, SipStrConsts::EPhraseOk, ETrue );
	iSipSession->SetPendingTransactionL( clitransaction );
    CleanupStack::Pop( clitransaction );
    iSipSession->iResponse = clitransaction;

	
	// Following Events are not accpeted for Known SIP Transaction in Established State
    TMceStateTransitionEvent event1b( *iSipSession, EMceProvisionalResponse );
	TRAPD( e1b, iState->AcceptL( event1b ) );
    EUNIT_ASSERT ( e1b == KErrTotalLossOfPrecision );

    TMceStateTransitionEvent event1c( *iSipSession, EMceRedirectionResponse );
	TRAPD( e1c, iState->AcceptL( event1c ) );
	EUNIT_ASSERT ( e1c == KErrTotalLossOfPrecision );
	
	// Following Events will be accepted for Extenssion Requests
    CSIPClientTransaction* clitransaction2 = 
    MCETestHelper::ClientTransactionLC( SipStrConsts::EInfo, KMceSipOK, SipStrConsts::EPhraseOk, ETrue );
	iSipSession->SetPendingTransactionL( clitransaction2 );
    CleanupStack::Pop( clitransaction2 );
    iSipSession->iResponse = clitransaction2;

    TMceStateTransitionEvent event2a( *iSipSession, EMceResponse );
	TRAPD( e2a, iState->AcceptL( event2a ) );
    EUNIT_ASSERT ( e2a == KErrNone );

    TMceStateTransitionEvent event2b( *iSipSession, EMceProvisionalResponse );
	TRAPD( e2b, iState->AcceptL( event2b ) );
    EUNIT_ASSERT ( e2b == KErrNone );

    TMceStateTransitionEvent event2c( *iSipSession, EMceRedirectionResponse );
	TRAPD( e2c, iState->AcceptL( event2c ) );
	EUNIT_ASSERT ( e2c == KErrNone );

	TMceStateTransitionEvent event2d( *iSipSession, EMceErrorResponse );
	TRAPD( e2d, iState->AcceptL( event2d ) );
	EUNIT_ASSERT ( e2d == KErrNone );
	
	TMceStateTransitionEvent event2e( *iSipSession, EMceItcReplySend, ids, *msg );
	TRAPD( e2e, iState->AcceptL( event2e ) );
	EUNIT_ASSERT ( e2e == KErrNone );
	
	TMceStateTransitionEvent event2f( *iSipSession, EMceRequest );
	TRAPD( e2f, iState->AcceptL( event2f ) );
	EUNIT_ASSERT ( e2f == KErrNone );
	
    }

void UT_CMceStateEstablished::UT_CMceStateEstablished_EntryLL()
    {
    
    TMceIds ids;
    CMceMsgBase* msg = NULL;
    
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );

    CMceComAudioStream* inStream = CMceComAudioStream::NewLC();
    CMceComAudioStream* outStream = CMceComAudioStream::NewLC();
    inStream->BindL( outStream );
    CleanupStack::Pop( outStream );
    
    CMceComRtpSource* rtpSource = CMceComRtpSource::NewLC();
    inStream->SetSourceL( rtpSource );
    CleanupStack::Pop( rtpSource );
    
    CMceComSpeakerSink* speaker = CMceComSpeakerSink::NewLC();
    inStream->AddSinkL( speaker );
    CleanupStack::Pop( speaker );
    

    CMceComRtpSink* rtpSink = CMceComRtpSink::NewLC();
    outStream->AddSinkL( rtpSink );
    CleanupStack::Pop( rtpSink );
    
    CMceComMicSource* mic = CMceComMicSource::NewLC();
    outStream->SetSourceL( mic );
    CleanupStack::Pop( mic );
    
    iSipSession->iBody->AddStreamL( inStream );
    CleanupStack::Pop( inStream );
    iSipSession->iBody->InitializeL();

    TUid appUid = TUid::Uid( 10 );
    iSipSession->iBody->iID = 1;
    inStream->iID   = TMceMediaId(appUid,1);
    outStream->iID  = TMceMediaId(appUid,2);
    rtpSource->iID  = TMceMediaId(appUid,3);
    speaker->iID    = TMceMediaId(appUid,4);
    rtpSink->iID    = TMceMediaId(appUid,5);
    mic->iID        = TMceMediaId(appUid,6);


    CSIPClientTransaction* clitransaction = 
       MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, KMceSipOK, SipStrConsts::EPhraseOk, ETrue );
	iSipSession->SetPendingTransactionL( clitransaction );
    CleanupStack::Pop( clitransaction );

    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 1 );
    MCE_RESET_STUBS();
    
    TMceStateTransitionEvent event1( *iSipSession, EMceItcTerminateSession, ids, *msg );
    iState->EntryL( event1 );
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EBye /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 2 );

    MCE_RESET_STUBS();
    
    TMceStateTransitionEvent event2( *iSipSession, EMceSessionExpired );
    iState->EntryL( event2 );
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EBye /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    
    MCE_RESET_STUBS();
    
    TMceStateTransitionEvent event2_1( *iSipSession, EMceRegister );
    iState->EntryL( event2_1 );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 3 );
    MCE_RESET_STUBS();

//update (fails)
    delete iStorage->iSipSentResponse;
    iStorage->iSipSentResponse = NULL;
    delete iSipSession->iNewBodyCandidate;
    iSipSession->iNewBodyCandidate = NULL;
    delete iSipSession->PopRequest();
    CSIPServerTransaction* transaction = MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, EFalse );
    CleanupStack::PushL( transaction );
    iSipSession->iPendingReceivedRequests.AppendL( transaction );
    CleanupStack::Pop( transaction );

    TMceStateTransitionEvent event3( *iSipSession, EMceInvite );
    iState->EntryL( event3 );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);

    MCE_RESET_STUBS();
    delete iStorage->iSipSentResponse;
    iStorage->iSipSentResponse = NULL;
    delete iSipSession->iNewBodyCandidate;
    iSipSession->iNewBodyCandidate = NULL;
    delete iSipSession->PopRequest();
    transaction = MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, ETrue );
    CleanupStack::PushL( transaction );
    iSipSession->iPendingReceivedRequests.AppendL( transaction );
    CleanupStack::Pop( transaction );

    TMceStateTransitionEvent event3_1( *iSipSession, EMceInvite );
    iState->EntryL( event3_1 );
    
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                      CMCETls::EDecode /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);

    MCE_RESET_STUBS();
    delete iStorage->iSipSentResponse;
    iStorage->iSipSentResponse = NULL;
    delete iSipSession->iNewBodyCandidate;
    iSipSession->iNewBodyCandidate = NULL;
    delete iSipSession->PopRequest();
    transaction = MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, ETrue );
    CleanupStack::PushL( transaction );
    iSipSession->iPendingReceivedRequests.AppendL( transaction );
    CleanupStack::Pop( transaction );

    iStorage->iMediaManagerUpdateStatus = KMceReady;
    iStorage->iMediaManagerNeedToNegotiate = EFalse;

    TMceStateTransitionEvent event3_2( *iSipSession, EMceInvite );
    iState->EntryL( event3_2 );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipOK /*sentResponse*/);

    MCE_RESET_STUBS();
    iStorage->iMediaManagerNeedToNegotiate = ETrue;
    delete iStorage->iSipSentResponse;
    iStorage->iSipSentResponse = NULL;
    delete iSipSession->iNewBodyCandidate;
    iSipSession->iNewBodyCandidate = NULL;
    delete iSipSession->PopRequest();
    transaction = MCETestHelper::ServerTransactionL( SipStrConsts::EBye, EFalse );
    CleanupStack::PushL( transaction );
    iSipSession->iPendingReceivedRequests.AppendL( transaction );
    CleanupStack::Pop( transaction );
    
    TMceStateTransitionEvent event4( *iSipSession, EMceBye );
    iState->EntryL( event4 );
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipOK /*sentResponse*/);

    MCE_RESET_STUBS();
    
    TMceStateTransitionEvent event5( *iSipSession, EMceSessionRefresh );
    iState->EntryL( event5 );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EInvite /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);


    MCE_RESET_STUBS();
    
    iStorage->iMediaManagerNeedToNegotiate = ETrue;
    delete iStorage->iSipSentResponse;
    iStorage->iSipSentResponse = NULL;
    delete iSipSession->iNewBodyCandidate;
    iSipSession->iNewBodyCandidate = NULL;
    delete iSipSession->PopRequest();
    transaction = MCETestHelper::ServerTransactionL( SipStrConsts::EUpdate, ETrue );
    CleanupStack::PushL( transaction );
    iSipSession->iPendingReceivedRequests.AppendL( transaction );
    CleanupStack::Pop( transaction );

    TMceStateTransitionEvent event6( *iSipSession, EMceUpdate );
    iState->EntryL( event6 );
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                      CMCETls::EDecode /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);

    MCE_RESET_STUBS();
    
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    delete iStorage->iSipSentResponse;
    iStorage->iSipSentResponse = NULL;
    delete iSipSession->iNewBodyCandidate;
    iSipSession->iNewBodyCandidate = NULL;
    delete iSipSession->PopRequest();
    transaction = MCETestHelper::ServerTransactionL( SipStrConsts::EUpdate, ETrue );
    CleanupStack::PushL( transaction );
    iSipSession->iPendingReceivedRequests.AppendL( transaction );
    CleanupStack::Pop( transaction );

    TMceStateTransitionEvent event6_1( *iSipSession, EMceUpdate );
    iState->EntryL( event6_1 );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipOK /*sentResponse*/);

    MCE_RESET_STUBS();
    
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    delete iStorage->iSipSentResponse;
    iStorage->iSipSentResponse = NULL;
    delete iSipSession->iNewBodyCandidate;
    iSipSession->iNewBodyCandidate = NULL;
    delete iSipSession->PopRequest();
    transaction = MCETestHelper::ServerTransactionL( SipStrConsts::EUpdate, EFalse );
    CleanupStack::PushL( transaction );
    iSipSession->iPendingReceivedRequests.AppendL( transaction );
    CleanupStack::Pop( transaction );

    TMceStateTransitionEvent event6_2( *iSipSession, EMceUpdate );
    iState->EntryL( event6_2 );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipOK /*sentResponse*/);

    MCE_RESET_STUBS();
    iStorage->iMediaManagerNeedToNegotiate = ETrue;
    delete iStorage->iSipSentResponse;
    iStorage->iSipSentResponse = NULL;
    delete iSipSession->iNewBodyCandidate;
    iSipSession->iNewBodyCandidate = NULL;
    delete iSipSession->PopRequest();
    
    TMceStateTransitionEvent event9( *iSipSession, EMceMediaSessionStopped );
    iState->EntryL( event9 );

    MCE_RESET_STUBS();
    
    TMceStateTransitionEvent event10( *iSipSession, EMceMediaError );
    iState->EntryL( event10 );


    MCE_RESET_STUBS();
    iStorage->iMMObject = NULL;

    ids.iAppUID = 10;
    ids.iSessionID = 1;
    ids.iMediaID = inStream->iID;
    ids.iSourceID = rtpSource->iID;

    TMceStateTransitionEvent event11( *iSipSession, EMceItcDisable, ids, *msg );
    iState->EntryL( event11 );
    EUNIT_ASSERT ( iStorage->iMediaManagerAction == CMCETls::EPauseSource );
    EUNIT_ASSERT ( static_cast<const CMceComMediaSource*>(iStorage->iMMObject) == rtpSource );


    MCE_RESET_STUBS();
    iStorage->iMMObject = NULL;
    
    ids.iAppUID = 10;
    ids.iSessionID = 1;
    ids.iMediaID = inStream->iID;
    ids.iSourceID = rtpSource->iID;

    TMceStateTransitionEvent event12( *iSipSession, EMceItcEnable, ids, *msg );
    iState->EntryL( event12 );
    EUNIT_ASSERT ( iStorage->iMediaManagerAction == CMCETls::EResumeSource );
    EUNIT_ASSERT ( static_cast<const CMceComMediaSource*>(iStorage->iMMObject) == rtpSource );


    MCE_RESET_STUBS();
    iStorage->iMMObject = NULL;
    
    ids.iAppUID = 10;
    ids.iSessionID = 1;
    ids.iMediaID = outStream->iID;
    ids.iSourceID = KMceMediaIdNotAssigned;
    ids.iSinkID = rtpSink->iID;

    TMceStateTransitionEvent event13( *iSipSession, EMceItcSendRR, ids, *msg );
    iState->EntryL( event13 );
    //EUNIT_ASSERT ( iStorage->iMediaManagerAction == CMCETls::EResumeSource );
    //EUNIT_ASSERT ( static_cast<CMceComMediaSink*>(iStorage->iMMObject) == rtpSource );

    MCE_RESET_STUBS();
    iStorage->iMMObject = NULL;


    TMceStateTransitionEvent event14( *iSipSession, EMceItcSendSR, ids, *msg );
    iState->EntryL( event14 );
    //EUNIT_ASSERT ( iStorage->iMediaManagerAction == CMCETls::EResumeSource );
    //EUNIT_ASSERT ( static_cast<CMceComMediaSink*>(iStorage->iMMObject) == rtpSource );


 	
    MCE_RESET_STUBS();    
    iStorage->iMMObject = NULL;
    
    
    CMceComSession* clientSession = CMceComSession::NewL( CMceComSession::EOutSession );
    clientSession->iID = 1;
    CleanupStack::PushL( clientSession );
    CMceMsgObject<CMceComSession>* clientSessionMsg = 
        new (ELeave) CMceMsgObject<CMceComSession>( *clientSession, EMceItcMsgTypeSession );
    CleanupStack::PushL( clientSessionMsg );
    
    TMceIds ids1;
    ids1.iAppUID = 10;
    ids1.iSessionID = 1;
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    TMceStateTransitionEvent event19( *iSipSession, EMceItcUpdate, ids1, 
                                     *clientSessionMsg );

    iState->EntryL( event19 );
    CleanupStack::PopAndDestroy( clientSessionMsg );
    CleanupStack::Pop( clientSession );
    
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);

    }

void UT_CMceStateEstablished::UT_CMceStateEstablished_ExitLL()
    {
    CMceMsgBase* msg = NULL;
    
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    
    TMceIds ids;
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateEstablished );

    CSIPServerTransaction* srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EBye, EFalse );
    CleanupStack::PushL( srvtransaction );
    iSipSession->iPendingReceivedRequests.AppendL( srvtransaction );
    CleanupStack::Pop( srvtransaction );

    TMceStateTransitionEvent event1( *iSipSession, EMceBye );
    iState->ExitL( event1 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateTerminated );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::ETerminated );
    

    
    iSipSession->NextState( KMceStateEstablished );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateEstablished );

    TMceStateTransitionEvent event2( *iSipSession, EMceItcTerminateSession, ids, *msg );
    iState->ExitL( event2 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateTerminating );
    EUNIT_ASSERT ( ids.iState == CMceSession::ETerminating );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::ETerminating );

    iSipSession->NextState( KMceStateEstablished );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateEstablished );

    TMceStateTransitionEvent event3( *iSipSession, EMceSessionExpired );
    iState->ExitL( event3 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateTerminating );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::ETerminating );

    
    iSipSession->NextState( KMceStateEstablished );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateEstablished );

    TMceStateTransitionEvent event4( *iSipSession, EMceSessionRefresh );
    iState->ExitL( event4 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateOffering );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::EOffering );
    
    iSipSession->NextState( KMceStateEstablished );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateEstablished );

    
    TMceStateTransitionEvent event5( *iSipSession, EMceItcUpdate, ids, *msg );
    event5.ParamStatus() = KErrGeneral;
    iState->ExitL( event5 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateEstablished );
    EUNIT_ASSERT ( ids.iState == CMceSession::EEstablished );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::EEstablished );
    
    event5.ParamStatus() = KMceAsync;
    iState->ExitL( event5 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateClientEstablishing );
    EUNIT_ASSERT ( ids.iState == CMceSession::EOffering );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::EOffering );
    
    event5.ParamStatus() = KMceReady;
    iState->ExitL( event5 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateEstablished );
    EUNIT_ASSERT ( ids.iState == CMceSession::EEstablished );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::EEstablished );

//INVITE
    
    delete iSipSession->PopRequest();
    CSIPServerTransaction* transaction = MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, ETrue );
    
    CleanupStack::PushL( transaction );
    iSipSession->iPendingReceivedRequests.AppendL( transaction );
    CleanupStack::Pop( transaction );
    
    TMceStateTransitionEvent event6( *iSipSession, EMceInvite );
    
    event6.ParamStatus() = KErrNone;
    iState->ExitL( event6 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateServerEstablishing  );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::EEstablished );

    delete iSipSession->PopRequest();
    transaction = MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, ETrue );
    CleanupStack::PushL( transaction );
    iSipSession->iPendingReceivedRequests.AppendL( transaction );
    CleanupStack::Pop( transaction );
    
    
    event6.ParamStatus() = KMceReady;
    iState->ExitL( event6 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateServerEstablishing  );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::EEstablished );

    delete iSipSession->PopRequest();
    transaction = MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, ETrue );
    CleanupStack::PushL( transaction );
    iSipSession->iPendingReceivedRequests.AppendL( transaction );
    CleanupStack::Pop( transaction );
    
    event6.ParamStatus() = KErrGeneral;
    iState->ExitL( event6 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateEstablished );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::EEstablished );
    
// UPDATE
    
    delete iSipSession->PopRequest();
    transaction = MCETestHelper::ServerTransactionL( SipStrConsts::EUpdate, ETrue );
        
    CleanupStack::PushL( transaction );
    iSipSession->iPendingReceivedRequests.AppendL( transaction );
    CleanupStack::Pop( transaction );
        
    TMceStateTransitionEvent event7( *iSipSession, EMceUpdate );
        
    event7.ParamStatus() = KErrNone;
    iState->ExitL( event7 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateServerEstablishing  );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::EEstablished );
    
    delete iSipSession->PopRequest();
    iSipSession->SetSubState( CMceSipSession::ERefreshing );
    transaction = MCETestHelper::ServerTransactionL( SipStrConsts::EUpdate, ETrue );
        
    CleanupStack::PushL( transaction );
    iSipSession->iPendingReceivedRequests.AppendL( transaction );
    CleanupStack::Pop( transaction );
        
    TMceStateTransitionEvent event8( *iSipSession, EMceUpdate );
        
    event8.ParamStatus() = KErrNone;
    iState->ExitL( event8 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateEstablished  );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::EEstablished );
    iSipSession->SetSubState( CMceSipSession::ENone );
    
    //Ensure Extension Request / Response Event does not change the session state
    
   	//EMceItcReplySend
	TMceStateTransitionEvent event12( *iSipSession, EMceItcReplySend, ids, *msg );
    iState->ExitL( event12 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateEstablished );
    
    //EMceRequest
	TMceStateTransitionEvent event13( *iSipSession, EMceRequest );
    iState->ExitL( event13 );
	EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateEstablished );
	
	//EMceProvisionalResponse
	CSIPClientTransaction* clitransaction = 
    MCETestHelper::ClientTransactionLC( SipStrConsts::EInfo, KMceSipSessionProgress, 
    									SipStrConsts::EPhraseSessionProgress, ETrue );
	iSipSession->SetPendingTransactionL( clitransaction );
    CleanupStack::Pop( clitransaction );
    iSipSession->iResponse = clitransaction;

	TMceStateTransitionEvent event14( *iSipSession, EMceProvisionalResponse );
    iState->ExitL( event14 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateEstablished );

	//EMceResponse
	CSIPClientTransaction* clitransaction1 = 
    MCETestHelper::ClientTransactionLC( SipStrConsts::EInfo, KMceSipOK, 
    									SipStrConsts::EPhraseOk, ETrue );
	iSipSession->SetPendingTransactionL( clitransaction1 );
    CleanupStack::Pop( clitransaction1 );
    iSipSession->iResponse = clitransaction1;
	
	TMceStateTransitionEvent event15( *iSipSession, EMceResponse );
    iState->ExitL( event15 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateEstablished );


	//EMceRedirectionResponse
	CSIPClientTransaction* clitransaction2 = 
    MCETestHelper::ClientTransactionLC( SipStrConsts::EInfo, KMceSipMovedTemporarily, 
    									SipStrConsts::EPhraseOk, ETrue );
	iSipSession->SetPendingTransactionL( clitransaction2 );
    CleanupStack::Pop( clitransaction2 );
    iSipSession->iResponse = clitransaction2;
	
	TMceStateTransitionEvent event16( *iSipSession, EMceRedirectionResponse );
    iState->ExitL( event16 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateEstablished );
	

	//EMceErrorResponse
	CSIPClientTransaction* clitransaction3 = 
    MCETestHelper::ClientTransactionLC( SipStrConsts::EInfo, KMceSipBadExtension, 
    									SipStrConsts::EPhraseBadExtension, ETrue );
	iSipSession->SetPendingTransactionL( clitransaction3 );
    CleanupStack::Pop( clitransaction3 );
    iSipSession->iResponse = clitransaction3;
	
	TMceStateTransitionEvent event17( *iSipSession, EMceErrorResponse );
    iState->ExitL( event17 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateEstablished );
    }
    
    
 void UT_CMceStateEstablished::UT_CMceStateEstablished_MediaErrorOccured()
 	{
 	TMceStateTransitionEvent event( *iSipSession, EMceMediaError );
    iState->MediaErrorOccured( event );	
    
    // Media Error is Handled, No Transition to State.
    EUNIT_ASSERT( iSipSession->CurrentState().Id() == KMceStateEstablished )
  	}


 void UT_CMceStateEstablished::UT_CMceStateEstablished_EntrySendRequestL()
 	{
  	_LIT8( KMceMethodINFO, "INFO");
  	TMceIds ids;
    ids.iTransactionID = 5000;
    
  	iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
  	
  	CMceMsgBase* msg = MCETestHelper::RequestMsg( KMceMethodINFO );
 	msg->PushL();
    CleanupStack::PushL( msg );
 
    // Ensure that no pending transaction exist
    EUNIT_ASSERT( iSipSession->PendingTransactions().Count() == 0 )
    
    iSipSession->Dialog()->Dialog().iState = CSIPDialog::EConfirmed; 
 	TMceStateTransitionEvent event( *iSipSession, EMceItcRequestSend, ids, *msg );
 	 	
    iState->EntryL( event );
    
    // Send Request is Handled, No Transition to State.
    EUNIT_ASSERT( iSipSession->CurrentState().Id() == KMceStateEstablished )
    
    // Ensure request sent as as pending transaction
    EUNIT_ASSERT( iSipSession->PendingTransactions().Count() == 1 )
   
    CleanupStack::PopAndDestroy( msg );
  	}


 void UT_CMceStateEstablished::UT_CMceStateEstablished_EntrySendResponseL()
 	{
 	//TBD: Create a pending request and send a response for that. 
 	//for the better code coverage
 	TUint32 code = 200;
 	TMceIds ids;
    ids.iTransactionID = 5000;
 	iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
 	
 	CMceMsgBase* msg = MCETestHelper::ReplyMsg( code );
 	msg->PushL();
    CleanupStack::PushL( msg );

 	EUNIT_ASSERT( iSipSession->PendingTransactions().Count() == 0 )
 	 	
    iSipSession->Dialog()->Dialog().iState = CSIPDialog::EConfirmed; 
 	TMceStateTransitionEvent event( *iSipSession, EMceItcReplySend, ids, *msg );
 	 	
    iState->EntryL( event );
    
    // Send Request is Handled, No Transition to State.
    EUNIT_ASSERT( iSipSession->CurrentState().Id() == KMceStateEstablished )
       
    CleanupStack::PopAndDestroy( msg );
  	}


 void UT_CMceStateEstablished::UT_CMceStateEstablished_EntryResponseL()
 	{
 	iSipSession->Dialog()->Dialog().iState = CSIPDialog::EConfirmed; 
 	iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    TMceIds ids;
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateEstablished );
    
    CSIPClientTransaction* clitransaction = 
    MCETestHelper::ClientTransactionLC( SipStrConsts::EInfo, KMceSipOK, 
    									SipStrConsts::EPhraseOk, ETrue );
	iSipSession->SetPendingTransactionL( clitransaction );
    CleanupStack::Pop( clitransaction );
    iSipSession->iResponse = clitransaction;
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 1 );
  	TMceStateTransitionEvent event( *iSipSession, EMceResponse );
  	
  	// Handle Responses Event within Established Session
  	iState->EntryL( event );
	
    // Response is Handled, No Transition to State.
    EUNIT_ASSERT( iSipSession->CurrentState().Id() == KMceStateEstablished )
  	
  	MCE_RESET_STUBS();
  	}


 void UT_CMceStateEstablished::UT_CMceStateEstablished_EntryRequestL()
 	{
 	iSipSession->Dialog()->Dialog().iState = CSIPDialog::EConfirmed; 
	iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    TMceIds ids;
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateEstablished );
 
  	CSIPServerTransaction* transaction = MCETestHelper::ServerTransactionL( SipStrConsts::EInfo, ETrue );
    
    CleanupStack::PushL( transaction );
    iSipSession->iPendingReceivedRequests.AppendL( transaction );
    
    CleanupStack::Pop( transaction );
    
 	// Handle Request Event within Established Session
 	TMceStateTransitionEvent event( *iSipSession, EMceRequest );
    iState->EntryL( event );
    
    // Info Request inside session is Handled,
    // No Transition to State.
    EUNIT_ASSERT( iSipSession->CurrentState().Id() == KMceStateEstablished )
  	}
    

 void UT_CMceStateEstablished::UT_CMceStateEstablished_EMceProvisionalResponse()
 	{
 	iSipSession->Dialog()->Dialog().iState = CSIPDialog::EConfirmed; 
 	iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    TMceIds ids;
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateEstablished );
    
    CSIPClientTransaction* clitransaction = 
    MCETestHelper::ClientTransactionLC( SipStrConsts::EInfo, KMceSipSessionProgress, 
    									SipStrConsts::EPhraseSessionProgress, ETrue );
    									
	iSipSession->SetPendingTransactionL( clitransaction );
    CleanupStack::Pop( clitransaction );
    iSipSession->iResponse = clitransaction;
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 1 );
  	TMceStateTransitionEvent event( *iSipSession, EMceResponse );
  	
  	// Handle Session in progress Event within Established Session
  	
  	iState->EntryL( event );

    // Provisional Response is Handled, No Transition to State.
    EUNIT_ASSERT( iSipSession->CurrentState().Id() == KMceStateEstablished )
  	}


 void UT_CMceStateEstablished::UT_CMceStateEstablished_EntryErrorResponseL()
 	{
 	// Test 1: Error response received for the INVITE Transaction in
 	// established Session. 
 	
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    TMceIds ids;
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateEstablished );
    
    CSIPClientTransaction* clitransaction = 
    MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, KMceSipDecline, 
    									SipStrConsts::EPhraseOk, ETrue );
	iSipSession->SetPendingTransactionL( clitransaction );
    CleanupStack::Pop( clitransaction );
    iSipSession->iResponse = clitransaction;
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 1 );
  	TMceStateTransitionEvent event( *iSipSession, EMceErrorResponse );
  	iState->EntryL( event );
  	MCE_RESET_STUBS();


	// Test 2: Do not leave, an error message is passed to the client if 
	// error message is for the sent request within a session. 
	
	iSipSession->Dialog()->Dialog().iState = CSIPDialog::EConfirmed; 
	CSIPClientTransaction* clitransaction2 = 
    MCETestHelper::ClientTransactionLC( SipStrConsts::EInfo, KMceSipDecline, 
    									SipStrConsts::EPhraseOk, ETrue );
	iSipSession->SetPendingTransactionL( clitransaction2 );
    CleanupStack::Pop( clitransaction2 );
    iSipSession->iResponse = clitransaction2;
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 2 );
  	TMceStateTransitionEvent event2( *iSipSession, EMceErrorResponse );
  	iState->EntryL( event2 );
  	
  	MCE_RESET_STUBS();
  	}
  	
        
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceStateEstablished,
    "UT_CMceStateEstablished",
    "UNIT" )

EUNIT_TEST(
    "StateEntryL - test ",
    "CMceStateEstablished",
    "StateEntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateEstablished_StateEntryLL, Teardown)

EUNIT_TEST(
    "AcceptL - test ",
    "CMceStateEstablished",
    "AcceptL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateEstablished_AcceptLL, Teardown)

EUNIT_TEST(
    "EntryL - test ",
    "CMceStateEstablished",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateEstablished_EntryLL, Teardown)

EUNIT_TEST(
    "ExitL - test ",
    "CMceStateEstablished",
    "ExitL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateEstablished_ExitLL, Teardown)

EUNIT_TEST(
    "MediaErrorOccured - test ",
    "CMceStateEstablished",
    "ExitL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateEstablished_MediaErrorOccured, Teardown)    
EUNIT_TEST(
    "EntrySendRequestL - test ",
    "CMceStateEstablished",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateEstablished_EntrySendRequestL, Teardown)    

EUNIT_TEST(
    "EntrySendResponse - test ",
    "CMceStateEstablished",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateEstablished_EntrySendResponseL, Teardown)    

EUNIT_TEST(
    "EntryResponse - test ",
    "CMceStateEstablished",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateEstablished_EntryResponseL, Teardown)    

EUNIT_TEST(
    "EntryRequest - test ",
    "CMceStateEstablished",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateEstablished_EntryRequestL, Teardown)    

EUNIT_TEST(
    "EntryProvisionalResponse - test ",
    "CMceStateEstablished",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateEstablished_EMceProvisionalResponse, Teardown)    

EUNIT_TEST(
    "EntryErrorResponseL - test ",
    "CMceStateEstablished",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateEstablished_EntryErrorResponseL, Teardown)    
    

EUNIT_END_TEST_TABLE

//  END OF FILE
