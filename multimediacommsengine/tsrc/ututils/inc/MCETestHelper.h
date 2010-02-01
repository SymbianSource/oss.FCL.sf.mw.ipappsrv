/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef MCETESTHELPER_H
#define MCETESTHELPER_H


#pragma warn_illtokenpasting off

#include <e32std.h>
#include <bamdesca.h>
#include <siperr.h>
#include <sipaddress.h>
#include <sipextensionheader.h>
#include <sipaddress.h>
#include <sipsupportedheader.h>
#include <sipcontenttypeheader.h>
#include <sipfromheader.h>
#include <siptoheader.h>
#include <sipstrings.h>
#include <sipstrconsts.h>
#include <Uri8.h>
#include <sdpdocument.h>


//  INTERNAL INCLUDES
#include "sip.h"
#include "sipconnection.h"
#include "sipprofile.h"
#include "SIPClientTransaction.h"
#include "SIPDialog.h"
#include "SIPServerTransaction.h"
#include "SIPRequestElements.h"
#include "SIPResponseElements.h"
#include "SIPMessageElements.h"
#include "SIPInviteDialogAssoc.h"
#include "uricontainer.h"

#include "mcecomsession.h"
#include "mcesession.h"
#include "mcecomaudiostream.h"
#include "mcecomrtpsource.h"
#include "mcecomspeakersink.h"
#include "mcecomrtpsink.h"
#include "mcecommicsource.h"
#include "mcecomamrcodec.h"

#include "cmcetls.h"
#include "mcesip.h"
#include "mceclientserver.h"
#include "mceevents.h"
#include "mcecssubsessionreceiver.h"
#include "mcecsreceivequeue.h"


_LIT8(KSDPMessage_OK1, "v=0\r\n\
o=mhandley 2890844526 2890842807 IN IP4 126.16.64.4\r\n\
s=SDP	Seminar\n\
i=A Seminar on the session description protocol\r\n\
u=http://www.cs.ucl.ac.uk/staff/M.Handley/sdp.03.ps\r\n\
e=mjh@isi.edu (Mark Handley)\r\n\
e=j.doe@example.com (Jane Doe)\r\n\
p=+44-171-380-7777\r\n\
p=+1 617 555 6011\r\n\
c=IN IP4 224.2.17.12/127\r\n\
b=X-YZ:128\r\n\
t=0 0\r\n\
r=604800 3600 0 90000\r\n\
t=2873397496 2873404696\r\n\
z=2882844526 -1h 2898848070 0\r\n\
k=uri:http://www.cs.ucl.ac.uk/staff/M.Handley/sdp.03.ps\r\n\
a=recvonly\r\n\
a=orient:landscape\r\n\
m=application 32416/234 udp wb\r\n\
a=orient:portrait\r\n\
m=audio 49170 RTP/AVP 0 96\r\n\
a=rtpmap:0 PCMU/8000\r\n\
a=rtpmap:96 AMR/8000/1\r\n\
i=A Seminar on the session description protocol\r\n\
c=IN IP4 224.2.17.12/127\r\n\
c=IN IP4 224.2.17.12/127\r\n\
b=X-YZ:128\r\n\
k=uri:http://www.cs.ucl.ac.uk/staff/M.Handley/sdp.03.ps\r\n\
a=orient:portrait\r\n\
m=video 51372 RTP/AVP 32\r\n\
a=rtpmap:32 MPV/90000\r\n");


_LIT8(KSDPMessage_OK2, "v=0\r\n\
o=mhandley 2890844526 2890842807 IN IP4 126.16.64.4\r\n\
s=SDP	Seminar\n\
i=UPDATED A Seminar on the session description protocol is UPDATED\r\n\
u=http://www.cs.ucl.ac.uk/staff/M.Handley/sdp.03.ps\r\n\
e=mjh@isi.edu (Mark Handley)\r\n\
e=j.doe@example.com (Jane Doe)\r\n\
p=+44-171-380-7777\r\n\
p=+1 617 555 6011\r\n\
c=IN IP4 224.2.17.12/127\r\n\
b=X-YZ:128\r\n\
t=0 0\r\n\
r=604800 3600 0 90000\r\n\
t=2873397496 2873404696\r\n\
z=2882844526 -1h 2898848070 0\r\n\
k=uri:http://www.cs.ucl.ac.uk/staff/M.Handley/sdp.03.ps\r\n\
a=recvonly\r\n\
a=orient:landscape\r\n\
m=application 32416/234 udp wb\r\n\
a=orient:portrait\r\n\
m=audio 49170 RTP/AVP 0 96\r\n\
a=rtpmap:0 PCMU/8000\r\n\
a=rtpmap:96 AMR/8000/1\r\n\
i=A Seminar on the session description protocol\r\n\
c=IN IP4 224.2.17.12/127\r\n\
c=IN IP4 224.2.17.12/127\r\n\
b=X-YZ:128\r\n\
k=uri:http://www.cs.ucl.ac.uk/staff/M.Handley/sdp.03.ps\r\n\
a=orient:portrait\r\n\
m=video 51372 RTP/AVP 32\r\n\
a=rtpmap:32 MPV/90000\r\n");

_LIT8(KSDPMessage_BAD, "badSDP");

const TInt KMaxLengthOfSDPMessage = 1000;
const TInt KMaxLengthOfSessionExpires = 40;
const TUint KMillisecondToSecond = 1000;
//const TChar KSipSeparator = ':';
const TChar KSipHeaderSeparator = ';';
const TChar KSipEqualsTo = '=';
_LIT8(KSessionExpires, "Session-Expires");
_LIT8(KSessionExpiresCompactForm, "x");
_LIT8(KRequire, "Require");
_LIT8(KMinSE, "Min-SE");
_LIT8(KReresher, "refresher");
_LIT8(KReresherUAC, "uac");
_LIT8(KReresherUAS, "uas");
_LIT8(KRecipient, " <sip:you@yours.com;p=fuu>;q=fuu");
_LIT8(KFrom, "sip:me@here.com");
_LIT8(KTo, "sip:you@here.com");
_LIT8(KEvent, "anevent");
_LIT8(KEvent2, "anotherevent");
_LIT8(KReferTo, "someone@there.com");
_LIT8(KContentType, "text/plain");
_LIT8(KReason, "Can't explain what happened");
_LIT8(KMethod, "FUU");
_LIT8(KHeader, "Header: Value");
_LIT8(KHeader2, "Header2: Value2");
_LIT8(KHeader3, "Header3: Value3");
_LIT8(KHeader4, "Header4: Value4");
_LIT8(KMediaToken, "media authorization token" );
_LIT8(KFeatureTag, "+featuretag");
_LIT8(KParamValue, "param=value" );
_LIT8(KParam, "param" );
_LIT8(KValue, "value" );
_LIT8(KRequestUriUser, "user");
_LIT8( KSIPProfileDefAOR, "sip:user@aa");
_LIT8(KContactValue, "sip:user@localhost");
_LIT8(KContactHeaderValue, "sip:user@172.21.11.216;expires=3600");
_LIT8(KContactValueTls, "<sip:user@localhost;transport=tls>;expires=3600");
_LIT8(KContactValueSips, "<sips:user@localhost>;expires=3600");
_LIT8(KContactValueSipsAndTls, "<sips:user@localhost;transport=tls>;expires=3600");
_LIT8(KRequestUri, "sip:user@1.2.3.4");

_LIT8(KForwarded, "sip:he@there.com" );
const TUint KMceTestLoops = 10;
const TUint32 KRSeq = 1;
const TUint32 KInviteCSeq = 1;

#define MCE_SET_URIL( uri )\
  	uri->SetComponentL( _L8("sip"), EUriScheme );\
  	uri->SetComponentL( _L8("nokia.com"), EUriHost );\
  	uri->SetComponentL( KRequestUriUser, EUriUserinfo )


//  FORWARD DECLARATION

class CMceSipSession;
class CMceCsSession;
class CMceSipEvent;

// CLASS DECLARATION

#define MCE_EUNIT_ASSERT( func ) \
if ( !(func) )\
	{\
	static TInt i = 20;\
	--i;\
	if ( i )\
		{\
		User::Leave(KErrNoMemory);\
		}\
	}\
EUNIT_ASSERT( func );

#define MCE_EUNIT_ASSERT_LEAVE( func ) { TRAPD( leaveErr, func );\
if ( leaveErr == KErrNoMemory )\
    {\
    User::Leave( KErrNoMemory );\
    }\
EUNIT_ASSERT( leaveErr != KErrNone );}

#define MCE_EUNIT_ASSERT_EQUALS( func, val ) \
{\
TInt mceUnitTestRetVal = func;\
if ( mceUnitTestRetVal != KErrNoMemory ){\
	EUNIT_ASSERT_EQUALS( mceUnitTestRetVal, val );\
	}\
else{\
	User::Leave( KErrNoMemory );\
	}\
}

#define MCE_EUNIT_ASSERT_SPECIFIC_LEAVE( func, val ) \
{\
TRAPD( mceUnitTestRetVal, func );\
if ( val != KErrNoMemory && mceUnitTestRetVal == KErrNoMemory ){\
	User::Leave( KErrNoMemory );\
	}\
else{\
	EUNIT_ASSERT_EQUALS( mceUnitTestRetVal, val );\
	}\
}

#define MCE_EUNIT_ASSERT_NO_LEAVE( func ) MCE_EUNIT_ASSERT_SPECIFIC_LEAVE( func, KErrNone )

#define MCE_LEAVE_HANDLING( func, oomMappedError ) {\
TRAPD( mceUnitTestLeaveError, func );\
if ( mceUnitTestLeaveError == KErrNoMemory || mceUnitTestLeaveError == oomMappedError )\
    {\
    User::Leave( KErrNoMemory );\
    }\
}

#define MCE_DELETE_SUBSESSION_POSTCODITION( mceUnitTestSession, mceUnitTestSubSession )\
if ( mceUnitTestSession && \
mceUnitTestSession->Implementation().SubSessions().Find( mceUnitTestSubSession ) == KErrNotFound && \
mceUnitTestSession->ServerCore().Manager().iOrphanSubSessions->SubSessions().Find( mceUnitTestSubSession ) == KErrNotFound )\
    {\
    User::Leave( KErrNoMemory );\
    }

#define MCE_RECEIVE_QUEUE_POSTCONDITION( mceUnitTestSipSession ){ \
TInt lastIndex = mceUnitTestSipSession->iReceiver->iReceiveQueue->iReceiveItems.Count() - 1; \
if ( lastIndex >= 0 && mceUnitTestSipSession->iReceiver->iReceiveQueue->iReceiveItems[ lastIndex ].Error() == KErrNoMemory )\
    {\
    User::Leave( KErrNoMemory );\
    }}

#define MCE_TH_SET( instance, value ) delete instance;instance=NULL;instance=value

#define MCE_RESET_STUBS()\
    iStorage->SIPSends( SIPStrings::StringF( SipStrConsts::EEmpty ), (CSIPMessageElements*)NULL );\
    iStorage->SIPSends( SIPStrings::StringF( SipStrConsts::EEmpty ), (CSIPResponseElements*)NULL );\
    iStorage->iAckSent = EFalse;\
    iStorage->iMediaManagerAction = CMCETls::ENone;\
    iStorage->iMediaManagerSdpAction = CMCETls::ENone;\
    iStorage->iMediaManagerUpdateStatus = KMceReady;\
    iStorage->iMediaManagerDecodeStatus = KErrNone;\
    iStorage->iMediaManagerReserveStatus = KMceReady;\
    iStorage->iMediaManagerNeedToNegotiate = ETrue;\
    iStorage->iCompletionCode = KErrNotFound

    
#define MCE_ASSERT_STUBS( mmaction, mmsdpaction, sentMethod, sentResponse )\
    EUNIT_ASSERT_EQUALS( mmaction, iStorage->iMediaManagerAction )\
    EUNIT_ASSERT_EQUALS( mmsdpaction, iStorage->iMediaManagerSdpAction )\
    EUNIT_ASSERT_EQUALS( SIPStrings::StringF( sentMethod ).DesC(), iStorage->iSipSentMethod.DesC() )\
    if ( sentResponse > 0 )\
        {\
        EUNIT_ASSERT( iStorage->iSipSentResponse != NULL )\
        EUNIT_ASSERT_EQUALS( sentResponse, iStorage->iSipSentResponse->StatusCode() )\
        }    

#define MCE_ASSERT_STATES( session, clientState, serverState )\
    EUNIT_ASSERT ( session->CurrentState().Id() == serverState );\
    EUNIT_ASSERT ( session->iBody->iState == clientState )

#define MCE_SET_STATES( session, clientState, serverState )\
    session->NextState( serverState );\
    session->iBody->iState = clientState

#define MCE_ASSERT_EVENT( event, code, status )\
    EUNIT_ASSERT( event.Code() == code );\
    EUNIT_ASSERT( event.ParamStatus() == status )

#define MCE_CLEAR_LEAVE()\
    iStorage->SetLeave( KErrNone )    
    
#define MCE_CHECK_MEMORY_LEAVE( conditions ) \
if ( conditions )\
    {\
    User::Leave( KErrNoMemory );\
    }

#define MCE_ASSERT_ENDPOINT_ENABLE_AND_DISABLE() \
    CMceComAudioStream* inStream = CMceComAudioStream::NewLC();\
    CMceComRtpSource* rtpSource = CMceComRtpSource::NewLC();\
    inStream->SetSourceL( rtpSource );\
    CleanupStack::Pop( rtpSource );\
    CMceComSpeakerSink* speaker = CMceComSpeakerSink::NewLC();\
    inStream->AddSinkL( speaker );\
    CleanupStack::Pop( speaker );\
    iSipSession->iBody->AddStreamL( inStream );\
    CleanupStack::Pop( inStream );\
    iSipSession->iBody->InitializeL();\
    TUid appUid = TUid::Uid( 10 );\
    iSipSession->iBody->iID = 1;\
    inStream->iID   = TMceMediaId(appUid,1);\
    rtpSource->iID  = TMceMediaId(appUid,2);\
    speaker->iID    = TMceMediaId(appUid,3);\
    iStorage->iMMObject = NULL;\
    ids.iAppUID = 10;\
    ids.iSessionID = 1;\
    ids.iMediaID = inStream->iID;\
    ids.iSourceID = rtpSource->iID;\
    TMceStateTransitionEvent eventDisable( *iSipSession, EMceItcDisable, ids, *msg );\
    iState->EntryL( eventDisable );\
    EUNIT_ASSERT ( iStorage->iMediaManagerAction == CMCETls::EPauseSource );\
    EUNIT_ASSERT ( static_cast<const CMceComMediaSource*>(iStorage->iMMObject) == rtpSource );\
    MCE_RESET_STUBS();\
    iStorage->iMMObject = NULL;\
    ids.iAppUID = 10;\
    ids.iSessionID = 1;\
    ids.iMediaID = inStream->iID;\
    ids.iSourceID = rtpSource->iID;\
    TMceStateTransitionEvent eventEnable( *iSipSession, EMceItcEnable, ids, *msg );\
    iState->EntryL( eventEnable );\
    EUNIT_ASSERT ( iStorage->iMediaManagerAction == CMCETls::EResumeSource );\
    EUNIT_ASSERT ( static_cast<const CMceComMediaSource*>(iStorage->iMMObject) == rtpSource );   

// MCE memory leave asserting can be used to help on cases where memory leave
// which occured deep inside function, cannot be determined from function return
// values (i.e. memory leave was ignored and execution did not go as expected)
#define MCE_MEMORY_LEAVE_CHECKING_ON

#ifdef MCE_MEMORY_LEAVE_CHECKING_ON
#define MCE_CHECK_MEMORY_LEAVE( conditions ) \
if ( conditions )\
    {\
    User::Leave( KErrNoMemory );\
    }

#else
#define MCE_CHECK_MEMORY_LEAVE( conditions )
#endif

/**
*  MCE SIP state machine
*
*  @lib 
*  @since
*/
class MCETestHelper 
	{
	
public:


    static void ManagerReadyToReceiveL( CMceCsSession* aClient );
    
    static CMceSipSession* ClientCreateSessionL( CMceCsSession* aClient, 
                                    const TDesC8& aParam1, 
                                    const TDesC8& aParam2 );

    static CMceSipSession* ClientIncomingSessionL( CMceCsSession* aClient, 
                                                   TBool aInvalid = EFalse,
                                                   TBool aSupported100rel = EFalse );

    static CMceSipEvent* ClientIncomingEventL( CMceCsSession* aClient, 
                                               TInt aMethod );

    
    static void ClientReadyToReceiveL( CMceSipSession* aSession );
    static CMceMsgBase* ClientReceiveLC( CMceSipSession* aSession, TMceIds& aIds );

    static CMceComSession* ClientEstablishL( CMceSipSession* aSession,
                                             const TDesC8& aHeader = KNullDesC8,
                                             const TDesC8& aContentType = KNullDesC8,
                                             const TDesC8& aContent = KNullDesC8,
                                             TBool aRequire100rel = EFalse );
                                             
    static void ClientTerminateL( CMceSipSession* aSession ,
                                  const TDesC8& aHeader = KNullDesC8,
                                  const TDesC8& aContentType = KNullDesC8,
                                  const TDesC8& aContent = KNullDesC8 );
                                  
    static void ClientAcceptL( CMceSipSession* aSession,
                                const TDesC8& aReason = KNullDesC8,
                                TInt aReasonCode = -1 );
    
    static void ClientDeclineL( CMceSipSession* aSession, 
                                const TDesC8& aHeader = KNullDesC8,
                                const TDesC8& aContent = KNullDesC8,
                                const TDesC8& aContentType = KNullDesC8,
                                const TDesC8& aReason = KNullDesC8,
                                TInt aReasonCode = -1 );
                                
    static void ClientRingL( CMceSipSession* aSession, 
                             const TDesC8& aHeader = KNullDesC8,
                             const TDesC8& aContent = KNullDesC8,
                             const TDesC8& aContentType = KNullDesC8 );

    static void ClientCancelL( CMceSipSession* aSession );
 
    static void ClientUpdateSessionL( CMceSipSession* aSession,
                                      CMceComSession& aUpdate,
                                      TUint32 aTimeout = 180,
                                      const TDesC8& aHeader = KNullDesC8, 
                                      const TDesC8& aContentType = KNullDesC8,
                                      const TDesC8& aContent = KNullDesC8,
                                      const TDesC8& aContentHeader = KNullDesC8 );

    static CMceComSession* ClientGetSessionL( CMceSipSession* aSession );

    static void MediaManagerCallBack( CMceSipSession* aSession, 
                                      TMceMediaEventCode aEvent,
                                      TInt aError = KErrNone );
    
    static void MediaManagerSinkCallBack( CMceSipSession* aSession, 
                                          TMceItcFunctions aEventTo,
                                          TInt aStream,
                                          TInt aSink );
                                          
    static void MediaManagerSourceCallBack( CMceSipSession* aSession, 
                                            TMceItcFunctions aEventTo,
                                            TInt aStream );
                                      
    
    static CSIPClientTransaction& ResponseL( CMceSipSession* aSession,
                                  TUint aReasonCode, 
                                  TInt aReasonPhrase, 
                                  TInt aTrx = 0,
                                  TBool aSDPContent = EFalse,
                                  TBool aRequire100rel = EFalse, 
                                  TUint aRSeqV = 1,
                                  TBool aExpectOkStateChange = ETrue );

    static CSIPClientTransaction& ForkedResponseL( 
                               CMceSipSession* aSession,
                               CSIPInviteDialogAssoc* dialog,
                               TUint aReasonCode, 
                               TInt aReasonPhrase, 
                               TInt aTrx = 0,
                               TBool aSDPContent = EFalse );

    static CSIPServerTransaction& RequestL( CMceSipSession* aSession, 
                                            TInt aRequest,
                                            TInt aSDPContent = 0,
                                            TBool aExpectOkStateChange = ETrue );

    static CSIPClientTransaction* ClientTransactionLC( TInt aToMethod, 
                                                       TUint aResponse = 0, 
                                                       TInt aPhrase = SipStrConsts::EEmpty,
                                                       TBool aContent = EFalse );

    static CSIPClientTransaction* ClientTransactionLC( TInt aToMethod, 
                                                       RPointerArray<CSIPHeaderBase>& aSipHeaders, 
                                                       TUint aResponse = 0, 
                                                       TInt aPhrase = SipStrConsts::EEmpty,
                                                       TBool aContent = EFalse );
                                                       
                                                        
                                                        
    static CSIPServerTransaction* ServerTransactionL( TInt aMethod, 
                                                      TBool aContent = EFalse,
                                                      TBool aToAndFromHeaders = ETrue );

    static CSIPServerTransaction* ServerTransaction2L( TInt aMethod );
                                                      
    static CSIPServerTransaction* ServerTransactionBadSDPL( TInt aMethod );
                                                      
    static CSIPServerTransaction* ServerTransactionL( TInt aMethod,
                                                      RPointerArray<CSIPHeaderBase>& aSipHeaders,
                                                      TBool aContent = EFalse );
                                                      

    static CSIPResponseElements* SetResponseL( CSIPClientTransaction& aTrx, 
                                               TUint aResponse, 
                                               TInt aPhrase,
                                               TBool aContent = EFalse,
                                               TBool aRequire100rel = EFalse, 
                                               TUint aRSeqValue = 1 );
                             

    static CSIPResponseElements* SetResponse2L( CSIPClientTransaction& aTrx, TUint aResponse, TInt aPhrase );
                                               
    static CSIPResponseElements* SetResponseBADL( CSIPClientTransaction& aTrx, TUint aResponse, TInt aPhrase );
    
    static void SetMessageElementsL( CSIPMessageElements& aElements, TInt sdpId = 1 );
    
    
    
    static CMceComSession* ComSessionL( TUint32 aSessionId, TBool aRequire100rel = EFalse );
    
    static CSdpDocument* OfferL();
    
    static CMceMsgBase* EventMsg( TInt aEventType );
    static CMceMsgBase* ReplyMsg( TUint aReasonCode );
    
    static CMceMsgBase* RequestMsg( const TDesC8& aMethod );
    static CMceComMediaSink* Sink( CMceSipSession* aSession, TInt aStream = 0, TInt aSink = 0 );

    static CMceComMediaSource* Source( CMceSipSession* aSession, TInt aStream = 0 );
    
    static void SinkEvent( TMceMccComEvent& aEvent, CMceSipSession* aSession, TInt aStream = 0, TInt aSink = 0 );
    static void SourceEvent( TMceMccComEvent& aEvent, CMceSipSession* aSession, TInt aStream = 0 );
    static void AppendContactToUserHeaders( RPointerArray<CSIPHeaderBase>& aSipHeaders, 
									const TDesC8& aContact );
    
    static void ChangeResponseTypeL( const CSIPResponseElements& aResponse, 
                                     TUint aResponseCode,
                                     TInt aReasonPhrase ); 
    };
	
#endif //MCETESTHELPER_H
