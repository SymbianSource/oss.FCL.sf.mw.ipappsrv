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
#include "ut_cmcestateupdated.h"

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
#include "mcestateupdated.h"
#include "mcecomaudiostream.h"
#include "mcecomrtpsource.h"
#include "mcecomspeakersink.h"
#include "mcecomrtpsink.h"
#include "mcecommicsource.h"
#include "mcefcactionset.h"



// CONSTRUCTION
UT_CMceStateUpdated* UT_CMceStateUpdated::NewL()
    {
    UT_CMceStateUpdated* self = UT_CMceStateUpdated::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceStateUpdated* UT_CMceStateUpdated::NewLC()
    {
    UT_CMceStateUpdated* self = new( ELeave ) UT_CMceStateUpdated();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceStateUpdated::~UT_CMceStateUpdated()
    {
    }

// Default constructor
UT_CMceStateUpdated::UT_CMceStateUpdated()
    {
    }

// Second phase construct
void UT_CMceStateUpdated::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CMceStateUpdated::SetupL()
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
    iSipSession = static_cast<CMceSipSession*>((iSession->Implementation().SubSessions())[0]);
    
    iSipSession->NextState( KMceStateUpdated );
    iState = static_cast<CMceStateUpdated*>(&iSipSession->CurrentState());
    }

void UT_CMceStateUpdated::Teardown()
    {
    CMCETls::Close();
	delete iSession;
    delete iServerCore;
    
    SdpCodecStringPool::Close();    
    }

void UT_CMceStateUpdated::UT_CMceStateUpdated_AcceptLL()
    {
    TMceIds ids;
    CMceMsgBase* msg = NULL;
    
    TMceStateTransitionEvent event1( *iSipSession, EMceItcEnable, ids, *msg );
    EUNIT_ASSERT ( iState->AcceptL( event1 ) );
    TMceStateTransitionEvent event2( *iSipSession, EMceItcDisable, ids, *msg );
    EUNIT_ASSERT ( iState->AcceptL( event2 ) );
    TMceStateTransitionEvent event3( *iSipSession, EMceItcAcceptSession, ids, *msg );
    EUNIT_ASSERT ( iState->AcceptL( event3 ) );
    TMceStateTransitionEvent event4( *iSipSession, EMceItcRejectSession, ids, *msg );
    EUNIT_ASSERT ( iState->AcceptL( event4 ) );
    TMceStateTransitionEvent event5( *iSipSession, EMceItcRing, ids, *msg );
    EUNIT_ASSERT ( iState->AcceptL( event5 ) );
    
    TMceStateTransitionEvent event8( *iSipSession, EMceItcUpdate, ids, *msg );
    TRAPD( e1, iState->AcceptL( event8 ) );
    EUNIT_ASSERT ( e1 == KErrTotalLossOfPrecision );


    TMceStateTransitionEvent event9( *iSipSession, EMceResponse );
    TRAPD( e2, iState->AcceptL( event9 ) );
    EUNIT_ASSERT ( e2 == KErrTotalLossOfPrecision );
    }

void UT_CMceStateUpdated::UT_CMceStateUpdated_EntryLL()
    {
    TMceIds ids;
    CMceMsgBase* msg = NULL;
    
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EInSession );

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
    
    HBufC8* offer = KSDPMessage_OK1().AllocLC();
    iSipSession->iOffer = CSdpDocument::DecodeL( *offer );
    CleanupStack::PopAndDestroy( offer );

    
    CSIPServerTransaction* invite = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, ETrue );
    
    CleanupStack::PushL( invite );
    iSipSession->iPendingReceivedRequests.AppendL( invite );
    CleanupStack::Pop(invite);
    
    iStorage->iSipSentMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    iStorage->iSipResponseToMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    iStorage->iMediaManagerAction = CMCETls::ENone;
    iStorage->iMMObject = NULL;

    ids.iAppUID = 10;
    ids.iSessionID = 1;
    ids.iMediaID = inStream->iID;
    ids.iSourceID = rtpSource->iID;

    TMceStateTransitionEvent event1( *iSipSession, EMceItcDisable, ids, *msg );
    iState->EntryL( event1 );
    EUNIT_ASSERT ( iStorage->iMediaManagerAction == CMCETls::EPauseSource );
    EUNIT_ASSERT ( static_cast<const CMceComMediaSource*>(iStorage->iMMObject) == rtpSource );


    iStorage->iSipSentMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    iStorage->iSipResponseToMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    iStorage->iMediaManagerAction = CMCETls::ENone;
    iStorage->iMMObject = NULL;
    
    ids.iAppUID = 10;
    ids.iSessionID = 1;
    ids.iMediaID = inStream->iID;
    ids.iSourceID = rtpSource->iID;

    TMceStateTransitionEvent event2( *iSipSession, EMceItcEnable, ids, *msg );
    iState->EntryL( event2 );
    EUNIT_ASSERT ( iStorage->iMediaManagerAction == CMCETls::EResumeSource );
    EUNIT_ASSERT ( static_cast<const CMceComMediaSource*>(iStorage->iMMObject) == rtpSource );

    iStorage->iSipSentMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    iStorage->iSipResponseToMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    iStorage->iMediaManagerAction = CMCETls::ENone;
    iStorage->iMMObject = NULL;


    iStorage->iSipSentMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    iStorage->iSipResponseToMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    iStorage->iMediaManagerAction = CMCETls::ENone;
    iStorage->iMMObject = NULL;

    TMceStateTransitionEvent event7( *iSipSession, EMceItcAcceptSession, ids, *msg );
    iState->EntryL( event7 );
    EUNIT_ASSERT ( iStorage->iMediaManagerAction == CMCETls::ENone );
    EUNIT_ASSERT ( iStorage->iSipResponseToMethod == SIPStrings::StringF( SipStrConsts::EInvite ) );
    EUNIT_ASSERT ( iStorage->iSipSentResponse->StatusCode() == KMceSipOK );
    
    iStorage->iSipSentMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    iStorage->iSipResponseToMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    iStorage->iMediaManagerAction = CMCETls::ENone;
    iStorage->iMMObject = NULL;
            
    iSipSession->iNatSession = ETrue;
               
    TMceStateTransitionEvent event7_0( *iSipSession, EMceItcAcceptSession, ids, *msg );
    iState->EntryL( event7_0 );
    EUNIT_ASSERT ( iStorage->iMediaManagerAction == CMCETls::ENone );
    EUNIT_ASSERT ( iStorage->iSipResponseToMethod == SIPStrings::StringF( SipStrConsts::EInvite ) );
    EUNIT_ASSERT ( iStorage->iSipSentResponse->StatusCode() == KMceSipOK );
	iSipSession->iNatSession = EFalse;

    //with FC    
    iSipSession->FCActions().InitializeFCL();

    iStorage->iSipSentMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    iStorage->iSipResponseToMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    iStorage->iMediaManagerAction = CMCETls::ENone;
    iStorage->iMMObject = NULL;

    TMceStateTransitionEvent event7_1( *iSipSession, EMceItcAcceptSession, ids, *msg );
    iState->EntryL( event7_1 );
    EUNIT_ASSERT ( iStorage->iMediaManagerAction == CMCETls::ENone );
    EUNIT_ASSERT ( iStorage->iSipResponseToMethod == SIPStrings::StringF( SipStrConsts::EInvite ) );
    EUNIT_ASSERT ( iStorage->iSipSentResponse->StatusCode() == KMceSipOK );

//reject no reason
    iStorage->iSipSentMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    iStorage->iSipResponseToMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    iStorage->iMediaManagerAction = CMCETls::ENone;
    iStorage->iMMObject = NULL;

    TMceStateTransitionEvent event8( *iSipSession, EMceItcRejectSession, ids, *msg );
    iState->EntryL( event8 );
    EUNIT_ASSERT ( iStorage->iMediaManagerAction == CMCETls::ECloseSession );
    EUNIT_ASSERT ( iStorage->iSipResponseToMethod == SIPStrings::StringF( SipStrConsts::EInvite ) );
    EUNIT_ASSERT ( iStorage->iSipSentResponse->StatusCode() == KMceSipDecline );
    
//reject with reason
    iStorage->iSipSentMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    iStorage->iSipResponseToMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    iStorage->iMediaManagerAction = CMCETls::ENone;
    iStorage->iMMObject = NULL;

    HBufC8* reason = SIPStrings::StringF( SipStrConsts::EPhraseDecline ).DesC().AllocLC();
    CDesC8Array* headers = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
    CleanupStack::PushL( headers );
    HBufC8* ctype = KNullDesC8().AllocLC();
    CMceMsgSIPReply* reply = new (ELeave) CMceMsgSIPReply( *reason, KMceSipDecline, *headers, *ctype );
    CleanupStack::PushL( reply );
    
    
    TMceStateTransitionEvent event9( *iSipSession, EMceItcRejectSession, ids, *reply );
    iState->EntryL( event9 );
    CleanupStack::PopAndDestroy( reply );
    CleanupStack::Pop( ctype );
    CleanupStack::Pop( headers );
    CleanupStack::Pop( reason );
    
    EUNIT_ASSERT ( iStorage->iMediaManagerAction == CMCETls::ECloseSession );
    EUNIT_ASSERT ( iStorage->iSipResponseToMethod == SIPStrings::StringF( SipStrConsts::EInvite ) );
    EUNIT_ASSERT ( iStorage->iSipSentResponse->StatusCode() == KMceSipDecline );
    
//ringing
    iStorage->iSipSentMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    iStorage->iSipResponseToMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    iStorage->iMediaManagerAction = CMCETls::ENone;
    iStorage->iMMObject = NULL;

    TMceStateTransitionEvent event10( *iSipSession, EMceItcRing, ids, *msg );
    iState->EntryL( event10 );
    EUNIT_ASSERT ( iStorage->iMediaManagerAction == CMCETls::ENone );
    EUNIT_ASSERT ( iStorage->iSipResponseToMethod == SIPStrings::StringF( SipStrConsts::EInvite ) );
    EUNIT_ASSERT ( iStorage->iSipSentResponse->StatusCode() == KMceSipRinging );

    TMceStateTransitionEvent event11( *iSipSession, EMceMediaError );
    iState->EntryL( event11 );


// pull mode accept invite
    CSIPServerTransaction& request = iSipSession->Request();
    CSIPRequestElements* requestElement = const_cast<CSIPRequestElements*>(request.RequestElements());
    requestElement->MessageElements().SetContent( NULL );
    
    TMceStateTransitionEvent event12( *iSipSession, EMceItcAcceptSession, ids, *msg );
    iState->EntryL( event12 );
    EUNIT_ASSERT ( iStorage->iMediaManagerAction == CMCETls::ENone );
    EUNIT_ASSERT ( iStorage->iSipResponseToMethod == SIPStrings::StringF( SipStrConsts::EInvite ) );
    EUNIT_ASSERT ( iStorage->iSipSentResponse->StatusCode() == KMceSipOK );
    }
    
void UT_CMceStateUpdated::UT_CMceStateUpdated_ExitLL()
    {
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EInSession );
    
    TMceIds ids;
    CMceMsgBase* msg = NULL;
  
    CSIPServerTransaction* trx = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, EFalse );
    CleanupStack::PushL(trx);
    EUNIT_ASSERT( &iSipSession->Request() == NULL );
    iSipSession->iPendingReceivedRequests.AppendL( trx );
    CleanupStack::Pop(trx);
    EUNIT_ASSERT( &iSipSession->Request() == trx );
    
    // pull mode
    TMceStateTransitionEvent event( *iSipSession, EMceItcAcceptSession, ids, *msg );
    iState->ExitL( event );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateServerOffering );
    EUNIT_ASSERT ( ids.iState == CMceSession::EAnswering );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::EAnswering );
    

    CUri8* uri = CUri8::NewLC(); //OK 
    MCE_SET_URIL( uri );
  	
    CSIPRequestElements* requestelements = CSIPRequestElements::NewL( uri ); 
    CleanupStack::Pop( uri );
    CleanupStack::PushL( requestelements );

    CSIPFromHeader* fromheader = CSIPFromHeader::DecodeL(_L8("User <user2@host>")); 
    CleanupStack::PushL( fromheader );
    
    requestelements->SetFromHeaderL(fromheader); 
    CleanupStack::Pop( fromheader );
            
    MCETestHelper::SetMessageElementsL( requestelements->MessageElements(), 1 );
    
    requestelements->SetMethodL( SIPStrings::StringF( SipStrConsts::EInvite ) );
    trx->SetRequestElements( requestelements );
    CleanupStack::Pop( requestelements );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateServerOffering );
    
    TMceStateTransitionEvent event1( *iSipSession, EMceItcAcceptSession, ids, *msg );
    iState->ExitL( event1 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateAnswering );
    EUNIT_ASSERT ( ids.iState == CMceSession::EAnswering );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::EAnswering );
    
    iSipSession->NextState( KMceStateUpdated );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateUpdated );

    TMceStateTransitionEvent event2( *iSipSession, EMceItcRejectSession, ids, *msg );
    iState->ExitL( event2 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateTerminated );
    EUNIT_ASSERT ( ids.iState == CMceSession::ETerminated );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::ETerminated );


    iSipSession->NextState( KMceStateUpdated );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateUpdated );

    TMceStateTransitionEvent event3( *iSipSession, EMceItcRing, ids, *msg );
    iState->ExitL( event3 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateUpdated );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::ETerminated );    
    }
    
        
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceStateUpdated,
    "UT_CMceStateUpdated",
    "UNIT" )

EUNIT_TEST(
    "AcceptL - test ",
    "CMceStateUpdated",
    "AcceptL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateUpdated_AcceptLL, Teardown)

EUNIT_TEST(
    "EntryL - test ",
    "CMceStateUpdated",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateUpdated_EntryLL, Teardown)

EUNIT_TEST(
    "ExitL - test ",
    "CMceStateUpdated",
    "ExitL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateUpdated_ExitLL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
