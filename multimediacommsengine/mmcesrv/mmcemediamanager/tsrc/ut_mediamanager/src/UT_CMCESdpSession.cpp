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
#include "UT_cmcesdpsession.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <in_sock.h>


//  INTERNAL INCLUDES
#include <SdpCodecStringConstants.h>
#include <SdpCodecStringPool.h>
#include <sdpdocument.h>
#include <SdpOriginField.h>
#include <SdpConnectionField.h>
#include <SdpMediaField.h>
#include <SdpAttributeField.h>
#include <SdpFmtAttributeField.h>
#include <SdpRtpmapValue.h>
#include <sdpbandwidthfield.h>

#include "mcesdpsession.h"
#include "mcecomsession.h"
#include "mcecomaudiostream.h"
#include "mcecomvideostream.h"
#include "mcecomvideocodec.h"
#include "mcemediamanager.h"

#include "mcecommicsource.h"
#include "mcecomspeakersink.h"
#include "mcecomrtpsource.h"
#include "mcertpsource.h"
#include "mcertpsink.h"
#include "mcecomrtpsink.h"
#include "mcemicsource.h"
#include "mcespeakersink.h"
#include "mcecomamrcodec.h"

#include "mceaudiosdpcodec.h"
#include "mcevideosdpcodec.h"
#include "mcecomcamerasource.h"
#include "mcecomdisplaysink.h"
#include "mcecomh263codec.h"

#include "cleanupresetanddestroy.h"
#include "CMccController_stub.h"
#include "mcemedianegotiated.h"
#include "mcemediamanagertesthelper.h"
#include "mcemediadefs.h"
#include "mcenatpluginmanager.h"
#include "mmcccodecinformation.h"

_LIT8( KMceMMTestSdpMessage_AudioVideo_sendrecv_MT_NOK, "v=0\r\n\
o=mhandley 2890844526 2890842807 IN IP4 126.16.64.4\r\n\
s=SDP	Seminar\n\
i=A Seminar on the session description protocol\r\n\
u=http://www.cs.ucl.ac.uk/staff/M.Handley/sdp.03.ps\r\n\
e=mjh@isi.edu (Mark Handley)\r\n\
e=j.doe@example.com (Jane Doe)\r\n\
p=+44-171-380-7777\r\n\
p=+1 617 555 6011\r\n\
c=IN IP4 510.21.32.148\r\n\
b=X-YZ:128\r\n\
t=0 0\r\n\
r=604800 3600 0 90000\r\n\
t=2873397496 2873404696\r\n\
z=2882844526 -1h 2898848070 0\r\n\
k=uri:http://www.cs.ucl.ac.uk/staff/M.Handley/sdp.03.ps\r\n\
a=sendrecv\r\n\
a=orient:landscape\r\n\
m=application 32416/234 udp wb\r\n\
a=orient:portrait\r\n\
m=audio 49170 RTP/AVP 0 96\r\n\
c=IN IP4 224.2.17.88/127\r\n\
a=rtpmap:96 AMR/8000/1\r\n\
a=fmtp:96 mode-set=0,2; octet-align=0\r\n\
a=sendrecv\r\n\
i=A Seminar on the session description protocol\r\n\
b=X-YZ:128\r\n\
m=video 51372 RTP/AVP 106\r\n\
c=IN IP4 224.2.17.88/127\r\n\
a=rtpmap:106 H263-2000/90000\r\n\
a=fmtp:106 profile=0;level=10\r\n\
a=sendrecv\r\n");


_LIT8( KMceMMTestSdpMessage_MT_Confield_In_MediaLine, "v=0\r\n\
o=- 123 123 IN IP4 10.36.7.239\r\n\
s=-\r\n\
t=0 0\r\n\
m=audio 5000/1 RTP/AVP 96\r\n\
c=IN IP4 10.36.7.239\r\n\
a=rtpmap:96 AMR/8000\r\n");

_LIT8( KMceMMTestSdpMessage_MT_No_rtpmap, "v=0\r\n\
o=- 123 123 IN IP4 10.36.7.239\r\n\
s=-\r\n\
c=IN IP4 10.36.7.239\r\n\
t=0 0\r\n\
m=audio 5000/1 RTP/AVP 96\r\n\
");

_LIT8( KMceTestSdpAMR, "v=0\r\n\
o=userb 1234567890 1234567890 IN IP4 10.20.180.72\r\n\
s=-\r\n\
i=-\r\n\
c=IN IP4 10.20.180.72\r\n\
t=0 0\r\n\
a=ptime:20\r\n\
a=maxptime:400\r\n\
m=audio 5078 RTP/AVP 96\r\n\
a=sendrecv\r\n\
a=curr:qos local sendrecv\r\n\
a=curr:qos remote none\r\n\
a=des:qos mandatory local sendrecv\r\n\
a=des:qos none remote sendrecv\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=ptime:20\r\n\
a=maxptime:400\r\n\
a=fmtp:96 octet-align=0; mode-set=0,1,2,3,4,5,6,7\r\n");


_LIT8( KMceTestSdpAMRWithb, "v=0\r\n\
o=userb 1234567890 1234567890 IN IP4 10.20.180.72\r\n\
s=-\r\n\
i=-\r\n\
c=IN IP4 10.20.180.72\r\n\
b=AS:80\r\n\
t=0 0\r\n\
a=ptime:20\r\n\
a=maxptime:400\r\n\
m=audio 5078 RTP/AVP 96\r\n\
a=sendrecv\r\n\
a=curr:qos local sendrecv\r\n\
a=curr:qos remote none\r\n\
a=des:qos mandatory local sendrecv\r\n\
a=des:qos none remote sendrecv\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=ptime:20\r\n\
a=maxptime:400\r\n\
a=fmtp:96 octet-align=0; mode-set=0,1,2,3,4,5,6,7\r\n");


_LIT8( KOffer, "v=0\r\n\
o=- 123 123 IN IP4 10.36.7.239\r\n\
s=-\r\n\
t=0 0\r\n\
m=audio 5000/1 RTP/AVP 96\r\n\
c=IN IP4 10.36.7.239\r\n\
a=rtpmap:96 AMR/8000\r\n");


_LIT8( KOfferOldSchoolOrigin, "v=0\r\n\
o=- 123 123 IN IP4 0.0.0.0\r\n\
s=-\r\n\
t=0 0\r\n\
m=audio 5000/1 RTP/AVP 96\r\n\
c=IN IP4 10.36.7.239\r\n\
a=rtpmap:96 AMR/8000\r\n");


_LIT8( KMceSdpMediaFieldOld, "v=0\r\n\
o=- 123 123 IN IP4 0.0.0.0\r\n\
s=-\r\n\
t=0 0\r\n\
m=audio 5000/1 RTP/AVP 96\r\n\
c=IN IP4 10.36.7.239\r\n\
a=rtpmap:0 PCMU/8000\r\n");


_LIT8( KMceSdpMediaFieldNew, "v=0\r\n\
o=- 123 123 IN IP4 0.0.0.0\r\n\
s=-\r\n\
t=0 0\r\n\
m=audio 5000/1 RTP/AVP 96\r\n\
c=IN IP4 10.36.7.239\r\n\
a=rtpmap:0 AMR/8000\r\n");


_LIT8( KConnectionField1, "v=0\r\n\
o=- 123 123 IN IP4 0.0.0.0\r\n\
s=-\r\n\
t=0 0\r\n\
m=audio 5000/1 RTP/AVP 96\r\n\
c=IN IP4 10.36.7.239\r\n\
a=rtpmap:96 AMR/8000\r\n\
m=video 0 RTP/AVP 97\r\n\
c=IN IP4 193.65.182.50\r\n");


_LIT8( KConnectionField2, "v=0\r\n\
o=- 123 123 IN IP4 0.0.0.0\r\n\
s=-\r\n\
c=IN IP4 10.36.7.229\r\n\
t=0 0\r\n\
m=audio 5000/1 RTP/AVP 96\r\n\
c=IN IP4 10.36.7.239\r\n\
a=rtpmap:96 AMR/8000\r\n\
m=video 0 RTP/AVP 97\r\n\
c=IN IP4 193.65.182.50\r\n");



#define SDP_STRING_L( stringIndex )\
    SdpCodecStringPool::StringPoolL().StringF( stringIndex, SdpCodecStringPool::StringTableL() )

// CONSTRUCTION
UT_CMceSdpSession* UT_CMceSdpSession::NewL()
    {
    UT_CMceSdpSession* self = UT_CMceSdpSession::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceSdpSession* UT_CMceSdpSession::NewLC()
    {
    UT_CMceSdpSession* self = new( ELeave ) UT_CMceSdpSession();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceSdpSession::~UT_CMceSdpSession()
    {
    }

// Default constructor
UT_CMceSdpSession::UT_CMceSdpSession()
    {
    }

// Second phase construct
void UT_CMceSdpSession::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceSdpSession::SetupL(  )
    {
    SIPStrings::OpenL();
    
    iMcc = CMccControllerStub::Stub();
    iServer = new (ELeave) CMceServerStub();
    
    iManager = CMceMediaManager::NewL( *this );
    // Make a dummy call to NatEnabled() to cause plugin to be loaded.
    // When the actual test case function
    // (e.g. UT_CMceSdpSession_DecodeOfferLL_CreateAnswer_OK_1) uses the plugin,
    // it is already loaded. Because if the test function would load the plugin,
    // then EUnit reports a memory leak, since the plugin is unloaded in
    // Teardown().
    iManager->NatPluginManager().NatEnabledL();

    RPointerArray<CMceComAudioCodec> audioCapabilities;
    RPointerArray<CMceComVideoCodec> videoCapabilities;
    MceCleanupResetAndDestroyPushL( audioCapabilities );
    MceCleanupResetAndDestroyPushL( videoCapabilities );
    
    iManager->GetCapabilitiesL( audioCapabilities );
    iManager->GetCapabilitiesL( videoCapabilities );
    
    CleanupStack::PopAndDestroy( 2 );//capas
    
    __ASSERT_ALWAYS( iManager->iSdpCodecs.Count() >= 2, User::Leave( KErrNotFound ) );
    iAudioSdpCodec = static_cast<CMceMediaSdpCodec*>( iManager->iSdpCodecs[ 0 ] );
    iVideoSdpCodec = static_cast<CMceMediaSdpCodec*>( iManager->iSdpCodecs[ 1 ] );

    iAudio = SdpCodecStringPool::StringPoolL().StringF( 
    											SdpCodecStringConstants::EMediaAudio, 
												SdpCodecStringPool::StringTableL());
												
    iVideo = SdpCodecStringPool::StringPoolL().StringF( 
    											SdpCodecStringConstants::EMediaVideo, 
												SdpCodecStringPool::StringTableL());
												
	iProtocol = SdpCodecStringPool::StringPoolL().StringF( 
												SdpCodecStringConstants::EProtocolRtpAvp, 
												SdpCodecStringPool::StringTableL() );
												
	iAudioOutSession = CreateSessionWithDowlinkL();
	iVideoOutSession = CreateSessionWithVideoDowlinkL();
    iInSession = CMceComSession::NewL(CMceComSession::EInSession);
    iAudioOutSession->SetObserver( *iServer );
    iVideoOutSession->SetObserver( *iServer );
    iInSession->SetObserver( *iServer );
    
    iSdpSession = CMceSdpSession::NewL( *iAudioOutSession, *iManager ); 
    iAudioOutSession->AttachSDPSessionL( *iSdpSession );
    } 

void UT_CMceSdpSession::Teardown(  )
    {
    CMceComSession::Delete( iAudioOutSession, *iManager );
    CMceComSession::Delete( iVideoOutSession, *iManager );
    CMceComSession::Delete( iInSession, *iManager );
	delete iServer;
	iServer = NULL;
    delete iSdpSession;
    iSdpSession = NULL;
    delete iManager;
	iManager = NULL;
	CMccControllerStub::Close();
	iMcc = NULL;
    
    SIPStrings::Close();
    }

// ---------------------------------------------------------
// Test offers and answers containing audio
// ---------------------------------------------------------    
//
void UT_CMceSdpSession::UT_CMceSdpSession_MatchingStreamsToMLinesL()
    {
    RPointerArray<CMceComMediaStream>& streams = iAudioOutSession->Streams();

    CMceComAMRCodec *amr = CMceComAMRCodec::NewL(_L8("PCMU"));
	CleanupStack::PushL( amr );
	amr->iSamplingFreq = 6000;
	amr->iPayloadType = 99;
	
	CMceComAudioStream* audio = static_cast <CMceComAudioStream*> (streams[0]);
	
    audio->AddCodecL( static_cast <CMceComAudioCodec*> ( amr ));
    CleanupStack::Pop( amr );

    CSdpDocument* sdpDocument = CSdpDocument::NewLC();
    RPointerArray< CSdpMediaField >& mediaLines = sdpDocument->MediaFields();

    CSdpMediaField* mediaLine = iSdpSession->CreateMediaLineLC( *audio );
    mediaLines.AppendL( mediaLine );
    CleanupStack::Pop( mediaLine );
                
    iSdpSession->MatchingStreamsToMLinesL(*sdpDocument, *iAudioOutSession);
    
    CleanupStack::PopAndDestroy( sdpDocument );   
    sdpDocument = NULL;
    mediaLine = NULL;
    /////
    
    ///// one stream and two media line
    sdpDocument = CSdpDocument::NewLC();
    mediaLines = sdpDocument->MediaFields();

    RPointerArray<CMceComMediaStream>& videoStreams = iVideoOutSession->Streams();

    CMceComVideoStream* video = static_cast <CMceComVideoStream*> (videoStreams[0]);
   
    mediaLine = iSdpSession->CreateMediaLineLC( *video );
    mediaLines.AppendL( mediaLine );
    CleanupStack::Pop( mediaLine );
    mediaLine = NULL;
    
    mediaLine = iSdpSession->CreateMediaLineLC( *audio );
    mediaLines.AppendL( mediaLine );
    CleanupStack::Pop( mediaLine );
    mediaLine = NULL;
    
    iSdpSession->MatchingStreamsToMLinesL(*sdpDocument, *iAudioOutSession);
    CleanupStack::PopAndDestroy( sdpDocument );   
    sdpDocument = NULL;
    }

void UT_CMceSdpSession::UT_CMceSdpSession_DecodePullModeUpdateMediaLineL()
    {    
    RPointerArray<CMceComMediaStream>& streams = iAudioOutSession->Streams();

    CMceComAMRCodec *amr = CMceComAMRCodec::NewL(_L8("PCMU"));
	CleanupStack::PushL( amr );
	amr->iSamplingFreq = 6000;
	amr->iPayloadType = 99;
	
    CMceComAudioStream* audio = static_cast <CMceComAudioStream*> (streams[0]);
	audio->SdpIndex() = 1;
	
    audio->AddCodecL( static_cast <CMceComAudioCodec*> ( amr ));
    CleanupStack::Pop( amr );
  
    CSdpDocument* sdpDocument = CSdpDocument::NewLC();
    
    RPointerArray< CSdpMediaField >& mediaLines = sdpDocument->MediaFields();

    RPointerArray<CMceComMediaStream>& videoStreams = iVideoOutSession->Streams();

    CMceComVideoStream* video = static_cast <CMceComVideoStream*> (videoStreams[0]);
   
    CSdpMediaField* mediaLine1 = iSdpSession->CreateMediaLineLC( *video );
    mediaLines.InsertL( mediaLine1, 0 );
    CleanupStack::Pop( mediaLine1 );
    
    CSdpMediaField* mediaLine2 = iSdpSession->CreateMediaLineLC( *audio );
    
    mediaLines.AppendL( mediaLine2 );
    CleanupStack::Pop( mediaLine2 );
    
    CMceSdpCodec* sdpCodec1 = iSdpSession->Manager().SdpCodec( *mediaLine1 );
    CMceSdpCodec* sdpCodec2 = iSdpSession->Manager().SdpCodec( *mediaLine2 );

    EUNIT_ASSERT_SPECIFIC_LEAVE( iSdpSession->DecodePullModeUpdateMediaLineL( 
        *sdpDocument, *iAudioOutSession, *mediaLine1, 0, *sdpCodec1 ), KErrNotFound );
    
    mediaLine1->SetPortL(0);
    
    EUNIT_ASSERT_NO_LEAVE(iSdpSession->DecodePullModeUpdateMediaLineL( 
        *sdpDocument, *iAudioOutSession, *mediaLine1, 0, *sdpCodec1 ));
    
    mediaLine2->KeepFormatL(_L8("97"));
    EncodeDirectionAttrL( *mediaLine2, SdpCodecStringConstants::EAttributeSendonly );

    EUNIT_ASSERT_LEAVE(iSdpSession->DecodePullModeUpdateMediaLineL( 
        *sdpDocument, *iAudioOutSession, *mediaLine2, 1, *sdpCodec2 ) );
    
    CleanupStack::PopAndDestroy( sdpDocument );
    }

void UT_CMceSdpSession::UT_CMceSdpSession_CreateOfferLL_OK_1(  )
    { 
    CSdpDocument* sdp = iSdpSession->CreateOfferL( *iAudioOutSession );
    CleanupStack::PushL( sdp );
    
    CSdpOriginField* origin = sdp->OriginField();
    const TDesC8& username = origin->UserName();

    TInt64 sessionid = origin->SessionId(); 
    RStringF attr = origin->NetType();
    RStringF addr = origin->AddressType();
    
    EUNIT_ASSERT( attr == SdpCodecStringPool::StringPoolL().StringF( 
  									SdpCodecStringConstants::ENetType, 
         							SdpCodecStringPool::StringTableL() ));
	EUNIT_ASSERT( addr == SdpCodecStringPool::StringPoolL().StringF( 
  									SdpCodecStringConstants::EAddressTypeIP4, 
         							SdpCodecStringPool::StringTableL() ));
         							
    const TDesC8& inet = origin->Address();
    
    const TDesC8& sessionname = sdp->SessionName();
    CSdpConnectionField* conn = sdp->ConnectionField();
    RStringF attr2 = conn->NetType();
    RStringF addr2 = conn->AddressType();
    
    EUNIT_ASSERT( attr2 == SdpCodecStringPool::StringPoolL().StringF( 
  									SdpCodecStringConstants::ENetType, 
         							SdpCodecStringPool::StringTableL() ));
	EUNIT_ASSERT( addr2 == SdpCodecStringPool::StringPoolL().StringF( 
  									SdpCodecStringConstants::EAddressTypeIP4, 
         							SdpCodecStringPool::StringTableL() ));
         							
    const TDesC8& inet2 = conn->Address();
    
    RPointerArray<CSdpMediaField>& mediafields = sdp->MediaFields();
    
    
    
    EUNIT_ASSERT( mediafields.Count() == 1 );
    
    CleanupStack::PopAndDestroy( sdp );
    }

void UT_CMceSdpSession::UT_CMceSdpSession_CreateOfferLL_REFRESH_OK_1(  )
	{
	// Create the SDP document with a different port 
	// than in the iAudioOutSession
	CSdpDocument* originalSdpDocument = 
	    CSdpDocument::DecodeLC( KMceTestSdpAMR );
	iSdpSession->iSdpDocument = originalSdpDocument;
	EUNIT_ASSERT( originalSdpDocument->MediaFields()[0]->IsValid() )
	originalSdpDocument->MediaFields()[0]->SetPortL( 
	    iAudioOutSession->Streams()[0]->LocalMediaPort() + 1 );
	
	iSdpSession->iOldLocalMediaPort = iAudioOutSession->Streams()[0]->LocalMediaPort();
	//assign the offer type to refresh, and get the new sdp
	CSdpDocument* offeredSdp = 
	    iSdpSession->CreateOfferL( *iAudioOutSession, CMceSdpSession::ERefresh );
	EUNIT_ASSERT( offeredSdp == originalSdpDocument )
	//test that whether the port is changed
	EUNIT_ASSERT( offeredSdp->MediaFields()[0]->IsValid());
	EUNIT_ASSERT_EQUALS( 
	    iAudioOutSession->Streams()[0]->LocalMediaPort(), 
	    offeredSdp->MediaFields()[0]->Port() )
	
	CleanupStack::PopAndDestroy( originalSdpDocument );
	}

void UT_CMceSdpSession::UT_CMceSdpSession_CreateOfferLL_NOK_1(  )
    {
	CMceComSession* session = CMceComSession::NewL(CMceComSession::EOutSession);
	CleanupStack::PushL( session );
	session->AddStreamL( NULL );
    iSdpSession->SetMediaSession( session );
    
    EUNIT_ASSERT_LEAVE( CSdpDocument* sdp = iSdpSession->CreateOfferL( *session ));
    
    delete iSdpSession->iSdpDocument;
    CleanupStack::Pop( session );
    CMceComSession::Delete( session, *iManager );
    }


void UT_CMceSdpSession::UT_CMceSdpSession_DecodeAnswerLL_OK_1(  )
    {
    RPointerArray<CMceComMediaStream>& streams = iAudioOutSession->Streams();
    
    CMceComAMRCodec *amr = CMceComAMRCodec::NewL(_L8("PCMU"));
	CleanupStack::PushL( amr );
	amr->iSamplingFreq = 6000;
	amr->iPayloadType = 99;
	amr->InitializeL( *streams[0] );
	
	CMceComAudioStream* audio = static_cast <CMceComAudioStream*> (streams[0]);
	
    audio->AddCodecL( static_cast <CMceComAudioCodec*> ( amr ));
    CleanupStack::Pop( amr );
    
    
    CSdpDocument* sdp = iSdpSession->CreateOfferL( *iAudioOutSession );
    CleanupStack::PushL( sdp );
    
    CSdpOriginField* origin = sdp->OriginField();
    const TDesC8& username = origin->UserName();
    
    // construct the answer to the offer with one selected codec
    CSdpConnectionField* conn = sdp->ConnectionField();
    
    TInetAddr addr;
    
    const TUint32 KInetAddr2 = INET_ADDR(19,19,19,19);
    
    addr.SetAddress( KInetAddr2 );
    
    conn->SetInetAddressL( addr );
    
    RPointerArray<CSdpMediaField>& mediafields = sdp->MediaFields();
    
    CSdpMediaField* mediaLine = mediafields[0];
    
    mediaLine->KeepFormatL(_L8("97"));//remove 99
    mediaLine->SetPortL( 8000 );
    EncodeDirectionAttrL( *mediaLine, SdpCodecStringConstants::EAttributeSendonly );
    
    iSdpSession->DecodeAnswerL( *sdp, *iAudioOutSession );
        
    EUNIT_ASSERT( iAudioOutSession->iRemoteIpAddress.Match(addr) );
    
    EUNIT_ASSERT( streams.Count() == 1 );
    
    CMceComAudioStream* selectedaudio = static_cast<CMceComAudioStream*>( streams[0] );
    const RPointerArray<CMceComAudioCodec>& codecs = selectedaudio->Codecs();
    
    
    EUNIT_ASSERT( codecs.Count() == 2 );
    EUNIT_ASSERT( codecs[0]->iIsNegotiated );
    EUNIT_ASSERT( !codecs[1]->iIsNegotiated );
    
    CMceComAudioCodec* codec = codecs[0];
    EUNIT_ASSERT( codec->iSamplingFreq == 8000 );
    
    EUNIT_ASSERT( codec->iSdpName == _L8("AMR") )
    
    EUNIT_ASSERT( codec->iPayloadType == 97 );
    
    EUNIT_ASSERT( selectedaudio->iRemoteMediaPort == 8000);
    
    CleanupStack::PopAndDestroy( sdp );
 
    }

void UT_CMceSdpSession::UT_CMceSdpSession_DecodeAnswerLL_OK_2(  )
    {
    RPointerArray<CMceComMediaStream>& streams = iAudioOutSession->Streams();
    
    CMceComAMRCodec *amr = CMceComAMRCodec::NewL(_L8("PCMU"));
	CleanupStack::PushL( amr );
	amr->iSamplingFreq = 6000;
	amr->iPayloadType = 99;
	
	CMceComAudioStream* audio = static_cast <CMceComAudioStream*> (streams[0]);
	
    audio->AddCodecL( static_cast <CMceComAudioCodec*> ( amr ));
    CleanupStack::Pop( amr );

	CMceComAudioStream* uplinkStream = CMceComAudioStream::NewL();
	CleanupStack::PushL( uplinkStream );
	uplinkStream->iLocalMediaPort = 6000;
	
	
	// uplink/sendonly in sdp 		
	CMceComMicSource* micSource = CMceComMicSource::NewL();
	CleanupStack::PushL( micSource );
    uplinkStream->SetSourceL( micSource );
	CleanupStack::Pop( micSource );
	CMceComRtpSink* rtpSink = CMceComRtpSink::NewL();
	CleanupStack::PushL( rtpSink );
	uplinkStream->AddSinkL( rtpSink );
	CleanupStack::Pop( rtpSink );
	
	CMceComAMRCodec *amr3 = CMceComAMRCodec::NewL(_L8("PCMU"));
	CleanupStack::PushL( amr3 );
	amr3->iSamplingFreq = 6000;
	amr3->iPayloadType = 0;
	uplinkStream->AddCodecL( static_cast <CMceComAudioCodec*> ( amr3 ));
	CleanupStack::Pop( amr3 );
	
	CMceComAMRCodec *amr4 = CMceComAMRCodec::NewL(_L8("AMR"));
	CleanupStack::PushL( amr4 );
	amr4->iSamplingFreq = 5000;
	amr4->iPayloadType = 99;
	uplinkStream->AddCodecL( static_cast <CMceComAudioCodec*> ( amr4 ));
    CleanupStack::Pop( amr4 );
    
	iAudioOutSession->AddStreamL( uplinkStream ); 
	CleanupStack::Pop( uplinkStream );
    iAudioOutSession->InitializeL();

    
    CSdpDocument* sdp = iSdpSession->CreateOfferL( *iAudioOutSession );
    CleanupStack::PushL( sdp );
    
    // construct the answer to the offer with one selected codec
    CSdpConnectionField* conn = sdp->ConnectionField();
    
    TInetAddr addr;
    
    const TUint32 KInetAddr2 = INET_ADDR(19,19,19,19);
    
    addr.SetAddress( KInetAddr2 );
    
    conn->SetInetAddressL( addr );
    
    RPointerArray<CSdpMediaField>& mediafields = sdp->MediaFields();
    
    mediafields[0]->KeepFormatL(_L8("97"));
    mediafields[0]->SetPortL( 8000 );
    EncodeDirectionAttrL( *mediafields[0], SdpCodecStringConstants::EAttributeSendonly );
    
    mediafields[1]->KeepFormatL(_L8("99"));
    mediafields[1]->SetPortL( 6000 );
    EncodeDirectionAttrL( *mediafields[1], SdpCodecStringConstants::EAttributeRecvonly );
    
    RStringF application = SdpCodecStringPool::StringPoolL().StringF
    			( SdpCodecStringConstants::EMediaApplication, SdpCodecStringPool::StringTableL() );
    			
    CSdpMediaField* othermedia = CSdpMediaField::NewLC( application, 5000, iProtocol, _L8("97") );
	mediafields.AppendL( othermedia );
	CleanupStack::Pop( othermedia );
	
    iSdpSession->DecodeAnswerL( *sdp, *iAudioOutSession );
        
    EUNIT_ASSERT( iAudioOutSession->iRemoteIpAddress.Match(addr) );
    
    EUNIT_ASSERT( streams.Count() == 2 );
    
    CMceComAudioStream* selectedaudio1 = static_cast<CMceComAudioStream*>( streams[0] );
    const RPointerArray<CMceComAudioCodec>& codecs1 = selectedaudio1->Codecs();
    
    CMceComAudioStream* selectedaudio2 = static_cast<CMceComAudioStream*>( streams[1] );
    const RPointerArray<CMceComAudioCodec>& codecs2 = selectedaudio2->Codecs();
    
    EUNIT_ASSERT( codecs1.Count() == 2 );
    EUNIT_ASSERT( codecs2.Count() == 2 );
    EUNIT_ASSERT( codecs1[0]->iIsNegotiated );
    EUNIT_ASSERT( !codecs1[1]->iIsNegotiated );
    EUNIT_ASSERT( !codecs2[0]->iIsNegotiated );
    EUNIT_ASSERT( codecs2[1]->iIsNegotiated );
    
    CMceComAudioCodec* codec1 = codecs1[0];
    CMceComAudioCodec* codec2 = codecs2[1];
    EUNIT_ASSERT( codec1->iSamplingFreq == 8000 );
    
    EUNIT_ASSERT( codec1->iSdpName == _L8("AMR") )
    
    EUNIT_ASSERT( codec1->iPayloadType == 97 );
    
    EUNIT_ASSERT( codec2->iSamplingFreq == 5000 );
    
    EUNIT_ASSERT( codec2->iSdpName == _L8("AMR") )
    
    EUNIT_ASSERT( codec2->iPayloadType == 99 );
    
    EUNIT_ASSERT( selectedaudio1->iRemoteMediaPort == 8000);
    EUNIT_ASSERT( selectedaudio2->iRemoteMediaPort == 6000);
    
    CleanupStack::PopAndDestroy( sdp );
 
    }

void UT_CMceSdpSession::UT_CMceSdpSession_DecodeAnswerLL_NOK_1(  )
    {
    RPointerArray<CMceComMediaStream>& streams = iAudioOutSession->Streams();
    
    CMceComAMRCodec *amr = CMceComAMRCodec::NewL(_L8("PCMU"));
	CleanupStack::PushL( amr );
	amr->iSamplingFreq = 6000;
	amr->iPayloadType = 99;
	
	CMceComAudioStream* audio = static_cast <CMceComAudioStream*> (streams[0]);
    audio->AddCodecL( static_cast <CMceComAudioCodec*> ( amr ));
    CleanupStack::Pop( amr );
    
    
	CMceComAudioStream* uplinkStream = CMceComAudioStream::NewL();
	CleanupStack::PushL( uplinkStream );
	uplinkStream->iLocalMediaPort = 6000;
	
	
	// uplink/sendonly in sdp 		
	CMceComMicSource* micSource = CMceComMicSource::NewL();
	CleanupStack::PushL( micSource );
	uplinkStream->SetSourceL( micSource );
	CleanupStack::Pop( micSource );
	CMceComRtpSink* rtpSink = CMceComRtpSink::NewL();
	CleanupStack::PushL( rtpSink );
	uplinkStream->AddSinkL( rtpSink );
	CleanupStack::Pop( rtpSink );
	
	CMceComAMRCodec *amr3 = CMceComAMRCodec::NewL(_L8("PCMU"));
	CleanupStack::PushL( amr3 );
	amr3->iSamplingFreq = 6000;
	amr3->iPayloadType = 0;
	uplinkStream->AddCodecL( static_cast <CMceComAudioCodec*> ( amr3 ));
	CleanupStack::Pop( amr3 );
	
	CMceComAMRCodec *amr4 = CMceComAMRCodec::NewL(_L8("AMR"));
	CleanupStack::PushL( amr4 );
	amr4->iSamplingFreq = 5000;
	amr4->iPayloadType = 94;
	uplinkStream->AddCodecL( static_cast <CMceComAudioCodec*> ( amr4 ));
	CleanupStack::Pop( amr4 );
	
	iAudioOutSession->AddStreamL( uplinkStream ); 
	CleanupStack::Pop( 1 );
    iAudioOutSession->InitializeL();

    
    CSdpDocument* sdp = iSdpSession->CreateOfferL( *iAudioOutSession );
    CleanupStack::PushL( sdp );
    
    // construct the answer to the offer with one selected codec
    CSdpConnectionField* conn = sdp->ConnectionField();
    
    TInetAddr addr;
    
    const TUint32 KInetAddr2 = INET_ADDR(19,19,19,19);
    
    addr.SetAddress( KInetAddr2 );
    
    conn->SetInetAddressL( addr );
    
    RPointerArray<CSdpMediaField>& mediafields = sdp->MediaFields();
    
    delete ( mediafields[0] );
    mediafields.Remove( 0 );
    
    
    EUNIT_ASSERT_LEAVE( iSdpSession->DecodeAnswerL( *sdp, *iAudioOutSession ) );

    CleanupStack::PopAndDestroy( sdp );
    sdp = NULL;
    
    sdp = CSdpDocument::DecodeL( KMceMMTestSdpMessage_AudioVideo_sendrecv_MT_NOK );
    CleanupStack::PushL( sdp );
    EUNIT_ASSERT_LEAVE( iSdpSession->DecodeAnswerL( *sdp, *iAudioOutSession ) );
      
    CleanupStack::PopAndDestroy( sdp );
    }

void UT_CMceSdpSession::UT_CMceSdpSession_DecodeOfferLL_CreateAnswer_OK_1(  )
    {
    iSdpSession->SetMediaSession( iInSession );
    iInSession->AttachSDPSessionL( *iSdpSession );

    const TUint32 KInetAddr2 = INET_ADDR(12,12,12,12);
    iInSession->iLocalIpAddress.SetAddress(KInetAddr2);
    
    // Create media line: m= This should correspond to the OFFER received!
    
    CSdpDocument* offer = CSdpDocument::NewLC();
	CSdpMediaField* media = CSdpMediaField::NewLC( iAudio, 5000, iProtocol, _L8("97") );
	
	// Note the sendrcv, this should create bound stream as well since it's two way.
	RStringF attribute = SdpCodecStringPool::StringPoolL().StringF
    			( SdpCodecStringConstants::EAttributeSendrecv, SdpCodecStringPool::StringTableL() );
	CSdpAttributeField* mediaAttr = CSdpAttributeField::NewLC(attribute, _L8(""));
	User::LeaveIfError( ( media->AttributeFields()).Append( mediaAttr ) );
    CleanupStack::Pop( mediaAttr );
    mediaAttr = NULL;
    TUint payloadType = 97;
	// Get the encoding name
	TPtrC8 codecName = _L8("AMR");	
	TUint samplingFreq = 8000;		
		
	// create rtpmap field for current codec
	CSdpFmtAttributeField* rtpmapAttribute = 
	    iAudioSdpCodec->EncodeRtpMapFieldsL( payloadType, 
											 codecName, 
											 samplingFreq,
										     KMceSdpOptionalEncodingParam );
	CleanupStack::PushL( rtpmapAttribute );
	User::LeaveIfError( media->FormatAttributeFields().Append ( rtpmapAttribute ) );
	CleanupStack::Pop( rtpmapAttribute );
	rtpmapAttribute = NULL;
	
	CSdpMediaField* media2 = CSdpMediaField::NewLC ( iAudio, 6000, iProtocol, _L8("99") );
	
	attribute = SdpCodecStringPool::StringPoolL().StringF
    			( SdpCodecStringConstants::EAttributeRecvonly, SdpCodecStringPool::StringTableL() );
	mediaAttr = CSdpAttributeField::NewLC(attribute, _L8(""));
	User::LeaveIfError( ( media2->AttributeFields()).Append( mediaAttr ) );
    CleanupStack::Pop( mediaAttr );
    
	payloadType = 99;
	// Get the encoding name
    TPtrC8 codecName2 = _L8("AMR");	
    samplingFreq = 8000;		
		
	// create rtpmap field for current codec
	rtpmapAttribute = 
	    iAudioSdpCodec->EncodeRtpMapFieldsL( payloadType, 
										     codecName2, 
											 samplingFreq,
									         KMceSdpOptionalEncodingParam );
	CleanupStack::PushL( rtpmapAttribute );
	User::LeaveIfError( media2->FormatAttributeFields().Append ( rtpmapAttribute ) );
	CleanupStack::Pop( rtpmapAttribute );
	
	RPointerArray<CSdpMediaField>& mediafields = offer->MediaFields();
    mediafields.AppendL( media );
    CleanupStack::Pop( media2 );
    CleanupStack::Pop( media );
    CleanupStack::PushL( media2 );
    mediafields.AppendL( media2 );
    CleanupStack::Pop( media2 );

    RStringF application = SdpCodecStringPool::StringPoolL().StringF
    			( SdpCodecStringConstants::EMediaApplication, SdpCodecStringPool::StringTableL() );
    // this line should be ignored			
    CSdpMediaField* othermedia = CSdpMediaField::NewLC ( application, 5000, iProtocol, _L8("97") );
	mediafields.AppendL( othermedia );
	CleanupStack::Pop( othermedia );
	
     // Set session name: s=
    offer->SetSessionNameL( _L8("") );

    const TUint32 KInetAddr = INET_ADDR(10,10,10,10);
	TInetAddr localIpAddress;
	localIpAddress.SetAddress(KInetAddr);
	
    CSdpOriginField* originField = CSdpOriginField::NewL( _L8("username"),
                                                          55555,
                                                          55555,
                                                          localIpAddress );
    offer->SetOriginField( originField );
    
    // Create connection field: c=
    CSdpConnectionField* connectionField = CSdpConnectionField::NewL( localIpAddress );
    offer->SetConnectionField( connectionField );
    
    User::LeaveIfError (offer->IsValid());
    
    iSdpSession->DecodeOfferL( *offer, *iInSession );

    RPointerArray<CMceComMediaStream>& streams = iInSession->Streams();   
    
    
    EUNIT_ASSERT( streams.Count() == 2 );
    
    CMceComAudioStream* audioStream1 = static_cast<CMceComAudioStream*>( streams[0] );
    audioStream1->iLocalMediaPort = 5000;
    EUNIT_ASSERT( audioStream1->iRemoteMediaPort == 5000 );
    EUNIT_ASSERT( audioStream1->BoundStream() );
    
	// get all the codecs to be offered for this media stream.
	const RPointerArray<CMceComAudioCodec>& codecs = audioStream1->Codecs();
	
	EUNIT_ASSERT( codecs.Count() == 1 );
	
	EUNIT_ASSERT( codecs[0]->iSdpName == _L8("AMR") )
    
    EUNIT_ASSERT( codecs[0]->iPayloadType == 97 );
    
    //EUNIT_ASSERT( codecs[0]->iMccState == CMceComAudioCodec::EMccStreamCreated );

	CMceComAudioStream* audioStream2 = static_cast<CMceComAudioStream*>( streams[1] );
	audioStream2->iLocalMediaPort = 6000;
	EUNIT_ASSERT( audioStream2->BoundStream() == EFalse );
	EUNIT_ASSERT( audioStream2->iRemoteMediaPort == 6000 );
	// get all the codecs to be offered for this media stream.
	const RPointerArray<CMceComAudioCodec>& codecs2 = audioStream2->Codecs();
	
	EUNIT_ASSERT( codecs2.Count() == 1 );
	
	EUNIT_ASSERT( codecs2[0]->iSdpName == _L8("AMR") )
    
    EUNIT_ASSERT( codecs2[0]->iPayloadType == 99 );
	
	/***
	* Now create answer
	***/
	//both media lines have one codec so select those
	
	iSdpSession->CreateAnswerL( *iInSession );
	
	// first media line in the anwer, this should be valid
	const TDesC8& fmtlist = mediafields[0]->FormatList();
	EUNIT_ASSERT(fmtlist == _L8("97"));
	
	EUNIT_ASSERT( mediafields[0]->IsValid());
	TUint port = mediafields[0]->Port();
  	EUNIT_ASSERT( mediafields[0]->Port() == 5000 );
  	
  	EUNIT_ASSERT( mediafields[0]->Media() == SdpCodecStringPool::StringPoolL().StringF( 
  									SdpCodecStringConstants::EMediaAudio, 
         							SdpCodecStringPool::StringTableL() ));
	EUNIT_ASSERT( mediafields[0]->Protocol() == SdpCodecStringPool::StringPoolL().StringF( 
  									SdpCodecStringConstants::EProtocolRtpAvp, 
         							SdpCodecStringPool::StringTableL() ));
         							
	RPointerArray<CSdpFmtAttributeField>& rtpmaps = mediafields[0]->FormatAttributeFields();
	
	
	EUNIT_ASSERT(rtpmaps.Count() == 2);
	EUNIT_ASSERT(rtpmaps[0]->Attribute() == SdpCodecStringPool::StringPoolL().StringF( 
								SdpCodecStringConstants::EAttributeRtpmap, 
     							SdpCodecStringPool::StringTableL() ));
    EUNIT_ASSERT(rtpmaps[0]->Format() == _L8("97") );				
     	
 	const TDesC8& attributeValue = rtpmaps[0]->Value();
 	TSdpRtpmapValue rtpmapValue = TSdpRtpmapValue::DecodeL( attributeValue );
 	EUNIT_ASSERT(rtpmapValue.iEncName == _L8("AMR"));
 	EUNIT_ASSERT(rtpmapValue.iClockrate == _L8("8000"));
     	
   	// second media line in the answe, this should be valid
   	
   	
   	const TDesC8& fmtlist2 = mediafields[1]->FormatList();
	EUNIT_ASSERT(fmtlist2 == _L8("99"));
	
	EUNIT_ASSERT( mediafields[1]->IsValid());
	TUint port2 = mediafields[1]->Port();
  	EUNIT_ASSERT( mediafields[1]->Port() == 6000 );
  	
  	EUNIT_ASSERT( mediafields[1]->Media() == SdpCodecStringPool::StringPoolL().StringF( 
  									SdpCodecStringConstants::EMediaAudio, 
         							SdpCodecStringPool::StringTableL() ));
	EUNIT_ASSERT( mediafields[1]->Protocol() == SdpCodecStringPool::StringPoolL().StringF( 
  									SdpCodecStringConstants::EProtocolRtpAvp, 
         							SdpCodecStringPool::StringTableL() ));
         							
	RPointerArray<CSdpFmtAttributeField>& rtpmaps2 = mediafields[1]->FormatAttributeFields();
	
	
	EUNIT_ASSERT(rtpmaps2.Count() == 2);
	EUNIT_ASSERT(rtpmaps2[0]->Attribute() == SdpCodecStringPool::StringPoolL().StringF( 
								SdpCodecStringConstants::EAttributeRtpmap, 
     							SdpCodecStringPool::StringTableL() ));
    EUNIT_ASSERT(rtpmaps2[0]->Format() == _L8("99") );				
     	
 	const TDesC8& attributeValue2 = rtpmaps2[0]->Value();
 	TSdpRtpmapValue rtpmapValue2 = TSdpRtpmapValue::DecodeL( attributeValue );
 	EUNIT_ASSERT(rtpmapValue2.iEncName == _L8("AMR"));
 	EUNIT_ASSERT(rtpmapValue2.iClockrate == _L8("8000"));     
	
	RPointerArray<CSdpAttributeField>& attributes = mediafields[0]->AttributeFields();
	

	TInt attributeCount = 3;
	if ( iManager->NatPluginManager().NatEnabledL() )
		{
		attributeCount = 4;
		}
	EUNIT_ASSERT( attributes.Count() == attributeCount );

	for ( TInt i = 0; i < attributes.Count(); i++ )
		{
		CSdpAttributeField* attribute = attributes[i];
		if(attribute->Attribute() == SdpCodecStringPool::StringPoolL().StringF( 
  									SdpCodecStringConstants::EAttributePtime, 
         							SdpCodecStringPool::StringTableL() ))
         	{
         	EUNIT_ASSERT(attribute->Value() == _L8("20") );
         	}
        else if(attribute->Attribute() == SdpCodecStringPool::StringPoolL().StringF( 
  									SdpCodecStringConstants::EAttributeMaxptime, 
         							SdpCodecStringPool::StringTableL() ))
         	{
         	EUNIT_ASSERT(attribute->Value() == _L8("400") );
         	}
        else if(attribute->Attribute() == SdpCodecStringPool::StringPoolL().StringF( 
  									SdpCodecStringConstants::EAttributeSendrecv, 
         							SdpCodecStringPool::StringTableL() ))
         	{
         	}
         else if( !attribute->Attribute().DesC().CompareF(_L8("rtcp") ) )
         	{
         	} 	
        else 
         	{
         	EUNIT_FAIL_TEST("extra attrbutes in the media");
         	}
		}
		
	CleanupStack::PopAndDestroy( offer );
    }


void UT_CMceSdpSession::UT_CMceSdpSession_DecodeOfferL_1L()
    {
    iSdpSession->SetMediaSession( iInSession );
    iInSession->AttachSDPSessionL( *iSdpSession );
    
    // this function failes the offer and creates rejected answer
    const TUint32 KInetAddr2 = INET_ADDR(12,12,12,12);
    iInSession->iLocalIpAddress.SetAddress(KInetAddr2);
    TInetAddr addr;
    EUNIT_ASSERT( iInSession->iRemoteIpAddress.CmpAddr( addr ) );
    
    addr.SetAddress( INET_ADDR(10,36,7,239) );
	CSdpDocument* offer = CSdpDocument::DecodeL( KMceMMTestSdpMessage_MT_Confield_In_MediaLine );
	CleanupStack::PushL( offer );
	
    TInt err = iSdpSession->DecodeOfferL( *offer, *iInSession );
    EUNIT_ASSERT( err == KErrNone );
    EUNIT_ASSERT( iInSession->iRemoteIpAddress.Match( addr ) );
    
    CleanupStack::PopAndDestroy( offer );   
    }


void UT_CMceSdpSession::UT_CMceSdpSession_DecodeOfferL_2L()
    {
    iSdpSession->SetMediaSession( iInSession );
    iInSession->AttachSDPSessionL( *iSdpSession );
    
    const TUint32 KInetAddr2 = INET_ADDR(12,12,12,12);
    iInSession->iLocalIpAddress.SetAddress(KInetAddr2);

	CSdpDocument* offer = CSdpDocument::DecodeL( KMceMMTestSdpMessage_MT_No_rtpmap );
	CleanupStack::PushL( offer );
	
    TInt err = iSdpSession->DecodeOfferL( *offer, *iInSession );
    EUNIT_ASSERT( err == KMceSipBadRequest );
    
    CleanupStack::PopAndDestroy( offer );
    }


void UT_CMceSdpSession::UT_CMceSdpSession_DecodeOfferL_3L(  )
    {
    iSdpSession->SetMediaSession( iInSession );
    iInSession->AttachSDPSessionL( *iSdpSession );

    const TUint32 KInetAddr2 = INET_ADDR(12,12,12,12);
    iInSession->iLocalIpAddress.SetAddress(KInetAddr2);
    
    // Create media line: m= This should correspond to the OFFER received!
    
    CSdpDocument* offer = CSdpDocument::NewL();
	CleanupStack::PushL( offer );
	
	CSdpMediaField* media = CSdpMediaField::NewLC ( iAudio, 5000, iProtocol, _L8("97") );
	
	// Note the sendrcv, this should create bound stream as well since it's two way.
	RStringF attribute = SdpCodecStringPool::StringPoolL().StringF
    			( SdpCodecStringConstants::EAttributeSendrecv, SdpCodecStringPool::StringTableL() );
	CSdpAttributeField* mediaAttr = CSdpAttributeField::NewLC(attribute, _L8(""));
	User::LeaveIfError( ( media->AttributeFields()).Append( mediaAttr ) );
    CleanupStack::Pop(); //mediaAttr
    TUint payloadType = 97;
	// Get the encoding name
	TPtrC8 codecName = _L8("AMR");	
	TUint samplingFreq = 8000;		
		
	// create rtpmap field for current codec
	CSdpFmtAttributeField* rtpmapAttribute = 
	    iAudioSdpCodec->EncodeRtpMapFieldsL( payloadType, 
											 codecName, 
											 samplingFreq,
										     KMceSdpOptionalEncodingParam );
	CleanupStack::PushL( rtpmapAttribute );
	User::LeaveIfError( media->FormatAttributeFields().Append ( rtpmapAttribute ) );
	CleanupStack::Pop(); // rtpmapAttribut
	
	CSdpMediaField* media2 = CSdpMediaField::NewLC ( iAudio, 6000, iProtocol, _L8("99") );
	
	attribute = SdpCodecStringPool::StringPoolL().StringF
    			( SdpCodecStringConstants::EAttributeRecvonly, SdpCodecStringPool::StringTableL() );
	CSdpAttributeField* mediaAttr2 = CSdpAttributeField::NewLC(attribute, _L8(""));
	User::LeaveIfError( ( media2->AttributeFields()).Append( mediaAttr2 ) );
    CleanupStack::Pop(); //mediaAttr
    
	payloadType = 99;
	// Get the encoding name
    TPtrC8 codecName2 = _L8("AMR");	
    samplingFreq = 8000;		
		
	// create rtpmap field for current codec
	CSdpFmtAttributeField* rtpmapAttribute2 = 
	    iAudioSdpCodec->EncodeRtpMapFieldsL( payloadType, 
										     codecName2, 
											 samplingFreq,
									         KMceSdpOptionalEncodingParam );
	CleanupStack::PushL( rtpmapAttribute2 );
	User::LeaveIfError( media2->FormatAttributeFields().Append ( rtpmapAttribute2 ) );
	CleanupStack::Pop(); // rtpmapAttribut2
	
	RPointerArray<CSdpMediaField>& mediafields = offer->MediaFields();
    mediafields.AppendL( media );
    CleanupStack::Pop( media2 );
    CleanupStack::Pop( media );
    CleanupStack::PushL( media2 );
    mediafields.AppendL( media2 );
    CleanupStack::Pop( media2 );
    
    RStringF application = SdpCodecStringPool::StringPoolL().StringF
    			( SdpCodecStringConstants::EMediaApplication, SdpCodecStringPool::StringTableL() );
    // this line should be ignored			
    CSdpMediaField* othermedia = CSdpMediaField::NewLC ( application, 5000, iProtocol, _L8("97") );
	mediafields.AppendL( othermedia );
	CleanupStack::Pop( othermedia );
	
     // Set session name: s=
    offer->SetSessionNameL( _L8("") );

    const TUint32 KInetAddr = INET_ADDR(10,10,10,10);
	TInetAddr localIpAddress;
	localIpAddress.SetAddress(KInetAddr);
	
    CSdpOriginField* originField = CSdpOriginField::NewL( _L8("username"),
                                                          55555,
                                                          55555,
                                                          localIpAddress );
    offer->SetOriginField( originField );
    
    // Create connection field: c=
    CSdpConnectionField* connectionField = CSdpConnectionField::NewL( localIpAddress );
    offer->SetConnectionField( connectionField );
    
    User::LeaveIfError (offer->IsValid());
    
    iSdpSession->DecodeOfferL( *offer, *iInSession );
    
    RPointerArray<CMceComMediaStream>& streams = iInSession->Streams();   
    
    
    EUNIT_ASSERT( streams.Count() == 2 );
    
    CMceComAudioStream* audioStream1 = static_cast<CMceComAudioStream*>( streams[0] );
    audioStream1->iLocalMediaPort = 5000;
    EUNIT_ASSERT( audioStream1->iRemoteMediaPort == 5000 );
    EUNIT_ASSERT( audioStream1->BoundStream() );
    
	// get all the codecs to be offered for this media stream.
	const RPointerArray<CMceComAudioCodec>& codecs = audioStream1->Codecs();
	
	EUNIT_ASSERT( codecs.Count() == 1 );
	
	EUNIT_ASSERT( codecs[0]->iSdpName == _L8("AMR") )
    
    EUNIT_ASSERT( codecs[0]->iPayloadType == 97 );
    
    //EUNIT_ASSERT( codecs[0]->iMccState == CMceComAudioCodec::EMccStreamCreated );
	
	CMceComAudioStream* audioStream2 = static_cast<CMceComAudioStream*>( streams[1] );
	audioStream2->iLocalMediaPort = 6000;
	EUNIT_ASSERT( audioStream2->BoundStream() == EFalse );
	EUNIT_ASSERT( audioStream2->iRemoteMediaPort == 6000 );
	// get all the codecs to be offered for this media stream.
	const RPointerArray<CMceComAudioCodec>& codecs2 = audioStream2->Codecs();
	
	EUNIT_ASSERT( codecs2.Count() == 1 );
	
	EUNIT_ASSERT( codecs2[0]->iSdpName == _L8("AMR") )
    
    EUNIT_ASSERT( codecs2[0]->iPayloadType == 99 );
	CleanupStack::PopAndDestroy( offer );	
	
	/****re-invite offer****/
	// to test if remove one m line
	
    iInSession->iLocalIpAddress.SetAddress(KInetAddr2);
    
    // Create media line: m= This should correspond to the OFFER received!
    
    CSdpDocument* offer2 = CSdpDocument::NewL();
	CleanupStack::PushL( offer2 );
	
	CSdpMediaField* media3 = CSdpMediaField::NewLC ( iAudio, 5000, iProtocol, _L8("97") );
	
	// Note the sendrcv, this should create bound stream as well since it's two way.
	RStringF attribute2 = SdpCodecStringPool::StringPoolL().StringF
    			( SdpCodecStringConstants::EAttributeSendrecv, SdpCodecStringPool::StringTableL() );
	CSdpAttributeField* mediaAttr3 = CSdpAttributeField::NewLC(attribute2, _L8(""));
	User::LeaveIfError( ( media3->AttributeFields()).Append( mediaAttr3 ) );
    CleanupStack::Pop(); //mediaAttr3
    payloadType = 97;
	// Get the encoding name
	TPtrC8 codecName3 = _L8("AMR");	
	samplingFreq = 8000;		
		
	// create rtpmap field for current codec
	CSdpFmtAttributeField* rtpmapAttribute3 = 
	    iAudioSdpCodec->EncodeRtpMapFieldsL( payloadType, 
											 codecName3, 
											 samplingFreq,
										     KMceSdpOptionalEncodingParam );
	CleanupStack::PushL( rtpmapAttribute3 );
	User::LeaveIfError( media3->FormatAttributeFields().Append ( rtpmapAttribute3 ) );
	CleanupStack::Pop(); // rtpmapAttribut3
	

	
	RPointerArray<CSdpMediaField>& mediafields2 = offer2->MediaFields();
    mediafields2.AppendL( media3 );
    
    CleanupStack::Pop( media3 );
    
    RStringF application2 = SdpCodecStringPool::StringPoolL().StringF
    			( SdpCodecStringConstants::EMediaApplication, SdpCodecStringPool::StringTableL() );
    // this line should be ignored			
    CSdpMediaField* othermedia2 = CSdpMediaField::NewLC ( application2, 5000, iProtocol, _L8("97") );
	mediafields2.AppendL( othermedia2 );
	CleanupStack::Pop( othermedia2 );
	
     // Set session name: s=
    offer->SetSessionNameL( _L8("") );

	localIpAddress.SetAddress(KInetAddr);
	
    CSdpOriginField* originField2 = CSdpOriginField::NewL( _L8("username"),
                                                          55555,
                                                          55555,
                                                          localIpAddress );
    offer2->SetOriginField( originField2 );
    
    // Create connection field: c=
    CSdpConnectionField* connectionField2 = CSdpConnectionField::NewL( localIpAddress );
    offer->SetConnectionField( connectionField2 );
    
    User::LeaveIfError (offer2->IsValid());
    
    iSdpSession->DecodeOfferL( *offer2, *iInSession );
		
	CleanupStack::PopAndDestroy( offer2 );	
	
	
		/****re-invite offer 3****/
	// to test if remove one m line
	
    iInSession->iLocalIpAddress.SetAddress(KInetAddr2);
    
    // Create media line: m= This should correspond to the OFFER received!
    
    CSdpDocument* offer3 = CSdpDocument::NewL();
	CleanupStack::PushL( offer3 );
	
	CSdpMediaField* media4 = CSdpMediaField::NewLC ( iAudio, 5000, iProtocol, _L8("97") );
	
	// Note the sendrcv, this should create bound stream as well since it's two way.
	RStringF attribute3 = SdpCodecStringPool::StringPoolL().StringF
    			( SdpCodecStringConstants::EAttributeSendrecv, SdpCodecStringPool::StringTableL() );
	CSdpAttributeField* mediaAttr4 = CSdpAttributeField::NewLC(attribute3, _L8(""));
	User::LeaveIfError( ( media4->AttributeFields()).Append( mediaAttr4 ) );
    CleanupStack::Pop(); //mediaAttr4
    payloadType = 97;
	// Get the encoding name
	TPtrC8 codecName4 = _L8("AMR");	
	samplingFreq = 8000;		
		
	// create rtpmap field for current codec
	CSdpFmtAttributeField* rtpmapAttribute4 = 
	    iAudioSdpCodec->EncodeRtpMapFieldsL( payloadType, 
											 codecName4, 
											 samplingFreq,
										     KMceSdpOptionalEncodingParam );
	CleanupStack::PushL( rtpmapAttribute4 );
	User::LeaveIfError( media4->FormatAttributeFields().Append ( rtpmapAttribute4 ) );
	CleanupStack::Pop(); // rtpmapAttribut4
	

	
	RPointerArray<CSdpMediaField>& mediafields3 = offer3->MediaFields();
    mediafields3.AppendL( media4 );
    
    CleanupStack::Pop( media4 );
    
    
     // Set session name: s=
    offer3->SetSessionNameL( _L8("") );

	localIpAddress.SetAddress(KInetAddr);
	
    CSdpOriginField* originField3 = CSdpOriginField::NewL( _L8("username"),
                                                          55555,
                                                          55555,
                                                          localIpAddress );
    offer3->SetOriginField( originField3 );
    
    // Create connection field: c=
    CSdpConnectionField* connectionField3 = CSdpConnectionField::NewL( localIpAddress );
    offer3->SetConnectionField( connectionField3 );
    
    User::LeaveIfError (offer3->IsValid());
    
    EUNIT_ASSERT( KMceSipNotAcceptableHere == iSdpSession->DecodeOfferL( *offer3, *iInSession ));
		
	CleanupStack::PopAndDestroy( offer3 );						
    }    
	
void UT_CMceSdpSession::UT_CMceSdpSession_DecodeOfferL_4L()
    {	
    iSdpSession->SetMediaSession( iInSession );
    iInSession->AttachSDPSessionL( *iSdpSession );
    
    TInetAddr remoteIP;
    remoteIP.SetAddress( INET_ADDR( 10,10,10,10 ) );
    
    _LIT8( KMyUserName, "username" );
    const TInt64 KSessionId( 1234 );
    const TInt64 KSessionVersion( 5678 );    
    CSdpOriginField* remoteorigin = 
            CSdpOriginField::NewL( KMyUserName, KSessionId, KSessionVersion, remoteIP );
    iSdpSession->iRemoteOrigin = remoteorigin;

    iSdpSession->iStoreRemoteOrigin = EFalse;
    CSdpDocument* offer = CSdpDocument::DecodeL( KMceMMTestSdpMessage_MT_Confield_In_MediaLine );
    CleanupStack::PushL( offer );
    TInt err = iSdpSession->DecodeOfferL( *offer, *iInSession );
    EUNIT_ASSERT( err == KErrNone );
    EUNIT_ASSERT( iSdpSession->iRemoteOrigin->InetAddress()->CmpAddr( remoteIP ) );

    iSdpSession->iStoreRemoteOrigin = ETrue;
    err = iSdpSession->DecodeOfferL( *offer, *iInSession );
    EUNIT_ASSERT( err == KErrNone );
    EUNIT_ASSERT( !iSdpSession->iRemoteOrigin->InetAddress()->CmpAddr( remoteIP ) );
    CleanupStack::PopAndDestroy( offer ); 
    }

void UT_CMceSdpSession::UT_CMceSdpSession_DecodeOfferLL_CreateAnswer_NOK_1(  )
    {
    iSdpSession->SetMediaSession( iInSession );
    iInSession->AttachSDPSessionL( *iSdpSession );
    
    // this function failes the offer and creates rejected answer
    const TUint32 KInetAddr2 = INET_ADDR(12,12,12,12);
    iInSession->iLocalIpAddress.SetAddress(KInetAddr2);
    
    // Create media line: m= This should correspond to the OFFER received!
    
	
	CSdpMediaField* media = CSdpMediaField::NewLC ( iAudio, 8000, iProtocol, _L8("97") );
	
	// Note the sendrcv, this should create bound stream as well since it's two way.
	RStringF attribute = SdpCodecStringPool::StringPoolL().StringF
    			( SdpCodecStringConstants::EAttributeSendrecv, SdpCodecStringPool::StringTableL() );
	CSdpAttributeField* mediaAttr = CSdpAttributeField::NewLC(attribute, _L8(""));
	media->AttributeFields().AppendL( mediaAttr );
    CleanupStack::Pop( mediaAttr );
    TUint payloadType = 97;
	// Get the encoding name
	TPtrC8 codecName = _L8("HMMH");	
	TUint samplingFreq = 8000;		
		
	// create rtpmap field for current codec
	CSdpFmtAttributeField* rtpmapAttribute = 
	    iAudioSdpCodec->EncodeRtpMapFieldsL( payloadType, 
											 codecName, 
										     samplingFreq,
										     KMceSdpOptionalEncodingParam );
	CleanupStack::PushL( rtpmapAttribute );
	media->FormatAttributeFields().AppendL( rtpmapAttribute );
	CleanupStack::Pop( rtpmapAttribute );

	
	CSdpDocument* offer = CSdpDocument::NewL();
	CleanupStack::PushL( offer );
	
	RPointerArray<CSdpMediaField>& mediafields = offer->MediaFields();
    mediafields.AppendL( media );
    CleanupStack::Pop( offer );
    CleanupStack::Pop( media );
    CleanupStack::PushL( offer );
    
     // Set iInSession name: s=
    offer->SetSessionNameL( _L8("") );

    const TUint32 KInetAddr = INET_ADDR(10,10,10,10);
	TInetAddr localIpAddress;
	localIpAddress.SetAddress(KInetAddr);
	
    CSdpOriginField* originField = CSdpOriginField::NewL( _L8("username"),
                                                          55555,
                                                          55555,
                                                          localIpAddress );
    offer->SetOriginField( originField );
    
    // Create connection field: c=
    CSdpConnectionField* connectionField = CSdpConnectionField::NewL( localIpAddress );
    offer->SetConnectionField( connectionField );
    
    User::LeaveIfError (offer->IsValid());
    
    TInt err = iSdpSession->DecodeOfferL(*offer, *iInSession );
    EUNIT_ASSERT(err == KMceSipWarnMediaTypeNotAvailable );
    
    iSdpSession->CreateAnswerL( *iInSession );
    
    
    
    
    CSdpOriginField* origin = offer->OriginField();
    const TDesC8& username = origin->UserName();
    EUNIT_ASSERT( _L8("-") == username );
    TInt64 sessionid = origin->SessionId(); 
    RStringF attr = origin->NetType();
    RStringF addr = origin->AddressType();
    
    EUNIT_ASSERT( attr == SdpCodecStringPool::StringPoolL().StringF( 
  									SdpCodecStringConstants::ENetType, 
         							SdpCodecStringPool::StringTableL() ));
	EUNIT_ASSERT( addr == SdpCodecStringPool::StringPoolL().StringF( 
  									SdpCodecStringConstants::EAddressTypeIP4, 
         							SdpCodecStringPool::StringTableL() ));
         							
    const TDesC8& inet = origin->Address();
    
    const TDesC8& sessionname = offer->SessionName();
    CSdpConnectionField* conn = offer->ConnectionField();
    RStringF attr2 = conn->NetType();
    RStringF addr2 = conn->AddressType();
    
    EUNIT_ASSERT( attr2 == SdpCodecStringPool::StringPoolL().StringF( 
  									SdpCodecStringConstants::ENetType, 
         							SdpCodecStringPool::StringTableL() ));
	EUNIT_ASSERT( addr2 == SdpCodecStringPool::StringPoolL().StringF( 
  									SdpCodecStringConstants::EAddressTypeIP4, 
         							SdpCodecStringPool::StringTableL() ));
         							
    const TDesC8& inet2 = conn->Address();
        
    
    EUNIT_ASSERT( mediafields.Count() == 1 );
    EUNIT_ASSERT( mediafields[0]->Port() == 0);
	
	CleanupStack::PopAndDestroy( offer );
	
    offer = CSdpDocument::DecodeL( KMceMMTestSdpMessage_AudioVideo_sendrecv_MT_NOK );
    CleanupStack::PushL( offer );
    EUNIT_ASSERT( iSdpSession->DecodeOfferL( *offer, *iInSession ) == KMceSipWarnIncompatibleNetworkAddressFormat );

	CleanupStack::PopAndDestroy( offer );
    }


void UT_CMceSdpSession::UT_CMceSdpSession_CreateOfferLL_Video_OK_1()
    { 
    iSdpSession->SetMediaSession( iVideoOutSession );
    iVideoOutSession->AttachSDPSessionL( *iSdpSession );
    
    CSdpDocument* sdp = iSdpSession->CreateOfferL( *iVideoOutSession );
    CleanupStack::PushL( sdp );
    
    CSdpOriginField* origin = sdp->OriginField();
    const TDesC8& username = origin->UserName();

    TInt64 sessionid = origin->SessionId(); 
    RStringF attr = origin->NetType();
    RStringF addr = origin->AddressType();
    
    EUNIT_ASSERT( attr == SdpCodecStringPool::StringPoolL().StringF( 
  									SdpCodecStringConstants::ENetType, 
         							SdpCodecStringPool::StringTableL() ));
	EUNIT_ASSERT( addr == SdpCodecStringPool::StringPoolL().StringF( 
  									SdpCodecStringConstants::EAddressTypeIP4, 
         							SdpCodecStringPool::StringTableL() ));
         							
    const TDesC8& inet = origin->Address();
    
    const TDesC8& sessionname = sdp->SessionName();
    CSdpConnectionField* conn = sdp->ConnectionField();
    RStringF attr2 = conn->NetType();
    RStringF addr2 = conn->AddressType();
    
    EUNIT_ASSERT( attr2 == SdpCodecStringPool::StringPoolL().StringF( 
  									SdpCodecStringConstants::ENetType, 
         							SdpCodecStringPool::StringTableL() ));
	EUNIT_ASSERT( addr2 == SdpCodecStringPool::StringPoolL().StringF( 
  									SdpCodecStringConstants::EAddressTypeIP4, 
         							SdpCodecStringPool::StringTableL() ));
         							
    const TDesC8& inet2 = conn->Address();
    
    RPointerArray<CSdpMediaField>& mediafields = sdp->MediaFields();
    
    EUNIT_ASSERT( mediafields.Count() == 1 );
    
    CleanupStack::PopAndDestroy( sdp );
    }


void UT_CMceSdpSession::UT_CMceSdpSession_DecodeAnswerLL_Video_OK_1()
    {
    iSdpSession->SetMediaSession( iVideoOutSession );
    iVideoOutSession->AttachSDPSessionL( *iSdpSession );
   
    RPointerArray<CMceComMediaStream>& streams = iVideoOutSession->Streams();
	
	CMceComVideoStream* video = static_cast<CMceComVideoStream*>(streams[0]);
	
	AddVideoCodecL( *video, 99, TSize( 200, 200 ) ); 
    
    CSdpDocument* sdp = iSdpSession->CreateOfferL( *iVideoOutSession );
    CleanupStack::PushL( sdp );
    
    CSdpOriginField* origin = sdp->OriginField();
    const TDesC8& username = origin->UserName();
    
    // construct the answer to the offer with one selected codec
    CSdpConnectionField* conn = sdp->ConnectionField();
    
    TInetAddr addr;
    
    const TUint32 KInetAddr2 = INET_ADDR(19,19,19,19);
    
    addr.SetAddress( KInetAddr2 );
    
    conn->SetInetAddressL( addr );
    
    RPointerArray<CSdpMediaField>& mediafields = sdp->MediaFields();
    
    mediafields[0]->KeepFormatL(_L8("96"));
    mediafields[0]->SetPortL( 8000 );
    EncodeDirectionAttrL( *mediafields[0], SdpCodecStringConstants::EAttributeSendonly );
    
    CMceSdpCodec* sdpCodec = NULL;
    sdpCodec = iManager->SdpCodec( *mediafields[0] );
    sdpCodec->AddCapabilityL( KH263SdpName );
    sdpCodec->AddCapabilityPayloadTypeL(KMinDynamicPT);
    
    iSdpSession->DecodeAnswerL( *sdp, *iVideoOutSession );
        
    EUNIT_ASSERT( iVideoOutSession->iRemoteIpAddress.Match(addr) );
    
    EUNIT_ASSERT( streams.Count() == 1 );
    
    CMceComVideoStream* selectedVideo = static_cast<CMceComVideoStream*>( streams[0] );
    const RPointerArray<CMceComVideoCodec>& codecs = selectedVideo->Codecs();

    
    EUNIT_ASSERT( codecs.Count() == 2 );
    EUNIT_ASSERT( codecs[0]->iIsNegotiated );
    EUNIT_ASSERT( !codecs[1]->iIsNegotiated );
    
    CMceComVideoCodec* codec = codecs[0];
    EUNIT_ASSERT( codec->iClockRate == 90000 );
    
    EUNIT_ASSERT( codec->iSdpName == _L8("H263") )
    
    EUNIT_ASSERT( codec->iPayloadType == 96 );
    
    EUNIT_ASSERT( codec->iFrameRate == 14.97 );
    
    EUNIT_ASSERT( codec->iResolutionWidth == 120 );
    
    EUNIT_ASSERT( codec->iResolutionHeight == 160 );
    
    EUNIT_ASSERT( selectedVideo->iRemoteMediaPort == 8000);
    
    CleanupStack::PopAndDestroy( sdp );
 
    }

void UT_CMceSdpSession::UT_CMceSdpSession_DecodeAnswerLL_Video_OK_2()
    {  
    iSdpSession->SetMediaSession( iVideoOutSession );
    iVideoOutSession->AttachSDPSessionL( *iSdpSession );
    
    RPointerArray<CMceComMediaStream>& streams = iVideoOutSession->Streams();
	
	CMceComVideoStream* video = static_cast <CMceComVideoStream*> (streams[0]);
	
    AddVideoCodecL( *video, 99, TSize( 200, 200 ) );

    AddVideoUplinkL( *iVideoOutSession );     
    
    CSdpDocument* sdp = iSdpSession->CreateOfferL( *iVideoOutSession );
    CleanupStack::PushL( sdp );
    
    // construct the answer to the offer with one selected codec
    CSdpConnectionField* conn = sdp->ConnectionField();
    
    TInetAddr addr;
    
    const TUint32 KInetAddr2 = INET_ADDR(19,19,19,19);
    
    addr.SetAddress( KInetAddr2 );
    
    conn->SetInetAddressL( addr );
    
    RPointerArray<CSdpMediaField>& mediafields = sdp->MediaFields();
    
    mediafields[0]->KeepFormatL(_L8("96"));
    mediafields[0]->SetPortL( 8000 );
    EncodeDirectionAttrL( *mediafields[0], SdpCodecStringConstants::EAttributeSendonly );
	
    mediafields[1]->KeepFormatL(_L8("96"));
    mediafields[1]->SetPortL( 6000 );
    EncodeDirectionAttrL( *mediafields[1], SdpCodecStringConstants::EAttributeRecvonly );
    
    TUint payloadType = 96;
	// Get the encoding name
	TPtrC8 codecName = _L8("H263");	
	TUint samplingFreq = 5000;	
    CSdpFmtAttributeField* rtpmapAttribute = 
	    iVideoSdpCodec->EncodeRtpMapFieldsL( payloadType, 
											 codecName, 
											 samplingFreq,
										     KMceSdpOptionalEncodingParam );
	CleanupStack::PushL( rtpmapAttribute );
	mediafields[1]->FormatAttributeFields().AppendL( rtpmapAttribute );
	CleanupStack::Pop( rtpmapAttribute );
    rtpmapAttribute = NULL;
    
    RStringF application = SdpCodecStringPool::StringPoolL().StringF
    			( SdpCodecStringConstants::EMediaApplication, SdpCodecStringPool::StringTableL() );
    			
    CSdpMediaField* othermedia = CSdpMediaField::NewLC( application, 5000, iProtocol, _L8("97") );
	mediafields.AppendL( othermedia );
	CleanupStack::Pop( othermedia );
	
	payloadType = 97;
	// Get the encoding name
    TPtrC8 codecNameH263 = _L8("H263");	
	samplingFreq = 5000;	
    rtpmapAttribute = 
	    iVideoSdpCodec->EncodeRtpMapFieldsL( payloadType, 
											 codecNameH263, 
											 samplingFreq,
										     KMceSdpOptionalEncodingParam );
	CleanupStack::PushL( rtpmapAttribute );
	othermedia->FormatAttributeFields().AppendL( rtpmapAttribute );
	CleanupStack::Pop( rtpmapAttribute );
	
	CMceSdpCodec* sdpCodec = NULL;
    sdpCodec = iManager->SdpCodec( *mediafields[0] );
    sdpCodec->AddCapabilityL( KH263SdpName );
    sdpCodec->AddCapabilityPayloadTypeL(KMinDynamicPT);
	
    iSdpSession->DecodeAnswerL( *sdp, *iVideoOutSession );
        
    EUNIT_ASSERT( iVideoOutSession->iRemoteIpAddress.Match(addr) );
    
    EUNIT_ASSERT( streams.Count() == 2 );
    
    CMceComVideoStream* selectedvideo1 = static_cast<CMceComVideoStream*>( streams[0] );
    const RPointerArray<CMceComVideoCodec>& codecs1 = selectedvideo1->Codecs();
    
    CMceComVideoStream* selectedvideo2 = static_cast<CMceComVideoStream*>( streams[1] );
    const RPointerArray<CMceComVideoCodec>& codecs2 = selectedvideo2->Codecs();
    
    EUNIT_ASSERT( codecs1.Count() == 2 );
    EUNIT_ASSERT( codecs2.Count() == 2 );
    EUNIT_ASSERT( codecs1[0]->iIsNegotiated );
    EUNIT_ASSERT( !codecs1[1]->iIsNegotiated );
    EUNIT_ASSERT( codecs2[0]->iIsNegotiated );
    EUNIT_ASSERT( !codecs2[1]->iIsNegotiated );
    
    CMceComVideoCodec* codec1 = codecs1[0];
    CMceComVideoCodec* codec2 = codecs2[0];
    
    EUNIT_ASSERT( codec1->iSdpName == _L8("H263") )
    
    EUNIT_ASSERT( codec2->iSdpName == _L8("H263") )

    CleanupStack::PopAndDestroy( sdp ); 
    }


void UT_CMceSdpSession::UT_CMceSdpSession_DecodeAnswerLL_Video_NOK_1(  )
    {
    iSdpSession->SetMediaSession( iVideoOutSession );
    iVideoOutSession->AttachSDPSessionL( *iSdpSession );

    RPointerArray<CMceComMediaStream>& streams = iVideoOutSession->Streams();
	CMceComVideoStream* video = static_cast <CMceComVideoStream*> (streams[0]);
	
	AddVideoCodecL( *video, 99, TSize( 200, 200 ) );
     
	AddVideoUplinkL( *iVideoOutSession );

    CSdpDocument* sdp = iSdpSession->CreateOfferL( *iVideoOutSession );
    CleanupStack::PushL( sdp );
    
    // construct the answer to the offer with one selected codec
    CSdpConnectionField* conn = sdp->ConnectionField();
    
    TInetAddr addr;
    
    const TUint32 KInetAddr2 = INET_ADDR(19,19,19,19);
    
    addr.SetAddress( KInetAddr2 );
    
    conn->SetInetAddressL( addr );
    
    RPointerArray<CSdpMediaField>& mediafields = sdp->MediaFields();
    
    delete ( mediafields[0] );
    mediafields.Remove( 0 );   
    
    EUNIT_ASSERT_LEAVE( iSdpSession->DecodeAnswerL( *sdp, *iVideoOutSession ) );
      
    CleanupStack::PopAndDestroy( sdp );
 
    }
    
void UT_CMceSdpSession::UT_CMceSdpSession_DecodeOfferLL_CreateAnswer_Video_OK_1(  )
    {
    iSdpSession->SetMediaSession( iInSession );
    iInSession->AttachSDPSessionL( *iSdpSession );
    
    const TUint32 KInetAddr2 = INET_ADDR(12,12,12,12);
    iInSession->iLocalIpAddress.SetAddress(KInetAddr2);
    
    // Create media line: m= This should correspond to the OFFER received!
    
    CSdpDocument* offer = CSdpDocument::NewL();
	CleanupStack::PushL( offer );
	
	CSdpMediaField* media = CSdpMediaField::NewLC ( iVideo, 5000, iProtocol, _L8("96") );
	
	// Note the sendrcv, this should create bound stream as well since it's two way.
	RStringF attribute = SdpCodecStringPool::StringPoolL().StringF
    			( SdpCodecStringConstants::EAttributeSendrecv, SdpCodecStringPool::StringTableL() );
	CSdpAttributeField* mediaAttr = CSdpAttributeField::NewLC(attribute, _L8(""));
	User::LeaveIfError( ( media->AttributeFields()).Append( mediaAttr ) );
    CleanupStack::Pop(); //mediaAttr
    TUint payloadType = 96;
	// Get the encoding name
	TPtrC8 codecName = _L8("H263-2000");	
	TUint clockRate = 90000;		
		
	// create rtpmap field for current codec
	CSdpFmtAttributeField* rtpmapAttribute = 
	    iVideoSdpCodec->EncodeRtpMapFieldsL( payloadType, 
											 codecName, 
											 clockRate,
										     KMceSdpOptionalEncodingParam );
	CleanupStack::PushL( rtpmapAttribute );
	media->FormatAttributeFields().AppendL( rtpmapAttribute );
	CleanupStack::Pop( rtpmapAttribute );
	
	CSdpAttributeField* frameSizeAttr = CSdpAttributeField::DecodeLC( _L8("a=framesize:96 120-160\r\n") );
	media->AttributeFields().AppendL( frameSizeAttr );
	CleanupStack::Pop( frameSizeAttr );
	
	CSdpAttributeField* frameRateAttr = CSdpAttributeField::DecodeLC( _L8("a=framerate:14.97\r\n") );
	media->AttributeFields().AppendL( frameRateAttr );
	CleanupStack::Pop( frameRateAttr );

	
	CSdpMediaField* media2 = CSdpMediaField::NewLC ( iVideo, 6000, iProtocol, _L8("99") );
	
	attribute = SdpCodecStringPool::StringPoolL().StringF
    			( SdpCodecStringConstants::EAttributeRecvonly, SdpCodecStringPool::StringTableL() );
	CSdpAttributeField* mediaAttr2 = CSdpAttributeField::NewLC(attribute, _L8(""));
	User::LeaveIfError( ( media2->AttributeFields()).Append( mediaAttr2 ) );
    CleanupStack::Pop(); //mediaAttr
    
	payloadType = 99;
	// Get the encoding name
    TPtrC8 codecName2 = _L8("H263-2000");	
    clockRate = 90000;		
		
	// create rtpmap field for current codec
	CSdpFmtAttributeField* rtpmapAttribute2 = 
	    iVideoSdpCodec->EncodeRtpMapFieldsL( payloadType, 
										     codecName2, 
											 clockRate,
									         KMceSdpOptionalEncodingParam );
	CleanupStack::PushL( rtpmapAttribute2 );
	User::LeaveIfError( media2->FormatAttributeFields().Append ( rtpmapAttribute2 ) );
	CleanupStack::Pop(); // rtpmapAttribut2
	
	frameSizeAttr = CSdpAttributeField::DecodeLC( _L8("a=framesize:99 200-200\r\n") );
	media2->AttributeFields().AppendL( frameSizeAttr );
	CleanupStack::Pop( frameSizeAttr );
	
	frameRateAttr = CSdpAttributeField::DecodeLC( _L8("a=framerate:14.97\r\n") );
	media2->AttributeFields().AppendL( frameRateAttr );
	CleanupStack::Pop( frameRateAttr );
	
	RPointerArray<CSdpMediaField>& mediafields = offer->MediaFields();
    mediafields.AppendL( media );
    CleanupStack::Pop( media2 );
    CleanupStack::Pop( media );
    CleanupStack::PushL( media2 );
    mediafields.AppendL( media2 );
    CleanupStack::Pop( media2 );
    
    RStringF application = SdpCodecStringPool::StringPoolL().StringF
    			( SdpCodecStringConstants::EMediaApplication, SdpCodecStringPool::StringTableL() );
    // this line should be ignored			
    CSdpMediaField* othermedia = CSdpMediaField::NewLC ( application, 5000, iProtocol, _L8("97") );
	mediafields.AppendL( othermedia );
	CleanupStack::Pop( othermedia );
	
     // Set iInSession name: s=
    offer->SetSessionNameL( _L8("") );

    const TUint32 KInetAddr = INET_ADDR(10,10,10,10);
	TInetAddr localIpAddress;
	localIpAddress.SetAddress(KInetAddr);
	
    CSdpOriginField* originField = CSdpOriginField::NewL( _L8("username"),
                                                          55555,
                                                          55555,
                                                          localIpAddress );
    offer->SetOriginField( originField );
    
    // Create connection field: c=
    CSdpConnectionField* connectionField = CSdpConnectionField::NewL( localIpAddress );
    offer->SetConnectionField( connectionField );
    
    User::LeaveIfError (offer->IsValid());
    
    iSdpSession->DecodeOfferL( *offer, *iInSession );
    
    RPointerArray<CMceComMediaStream>& streams = iInSession->Streams();   
    
    
    EUNIT_ASSERT( streams.Count() == 2 );
    
    CMceComVideoStream* videoStream1 = static_cast<CMceComVideoStream*>( streams[0] );
    videoStream1->iLocalMediaPort = 5000;
    EUNIT_ASSERT( videoStream1->iRemoteMediaPort == 5000 );
    EUNIT_ASSERT( videoStream1->BoundStream() );
    
	// get all the codecs to be offered for this media stream.
	const RPointerArray<CMceComVideoCodec>& codecs = videoStream1->Codecs();
	
	EUNIT_ASSERT( codecs.Count() == 1 );
	
	EUNIT_ASSERT( codecs[0]->iSdpName == _L8("H263-2000") )
    
    EUNIT_ASSERT( codecs[0]->iPayloadType == 96 );
    
    EUNIT_ASSERT( codecs[0]->iFrameRate == 14.97 );
    
    EUNIT_ASSERT( codecs[0]->iResolutionWidth == 120 );
    
    EUNIT_ASSERT( codecs[0]->iResolutionHeight == 160 );
	
	CMceComVideoStream* videoStream2 = static_cast<CMceComVideoStream*>( streams[1] );
	videoStream2->iLocalMediaPort = 6000;
	EUNIT_ASSERT( videoStream2->BoundStream() == EFalse );
	EUNIT_ASSERT( videoStream2->iRemoteMediaPort == 6000 );
	// get all the codecs to be offered for this media stream.
	const RPointerArray<CMceComVideoCodec>& codecs2 = videoStream2->Codecs();
	
	EUNIT_ASSERT( codecs2.Count() == 1 );
	
	EUNIT_ASSERT( codecs2[0]->iSdpName == _L8("H263-2000") )
    
    EUNIT_ASSERT( codecs2[0]->iPayloadType == 99 );

    EUNIT_ASSERT( codecs2[0]->iFrameRate == 14.97 );
    
    EUNIT_ASSERT( codecs2[0]->iResolutionWidth == 200 );
    
    EUNIT_ASSERT( codecs2[0]->iResolutionHeight == 200 );
	
	/***
	* Now create answer
	***/
	//both media lines have one codec so select those
	
	iSdpSession->CreateAnswerL( *iInSession );
	
	
	// first media line in the anwer, this should be valid
	const TDesC8& fmtlist = mediafields[0]->FormatList();
	EUNIT_ASSERT(fmtlist == _L8("96"));
	
	EUNIT_ASSERT( mediafields[0]->IsValid());
	TUint port = mediafields[0]->Port();
  	EUNIT_ASSERT( mediafields[0]->Port() == 5000 );
  	
  	EUNIT_ASSERT( mediafields[0]->Media() == SdpCodecStringPool::StringPoolL().StringF( 
  									SdpCodecStringConstants::EMediaVideo, 
         							SdpCodecStringPool::StringTableL() ));
	EUNIT_ASSERT( mediafields[0]->Protocol() == SdpCodecStringPool::StringPoolL().StringF( 
  									SdpCodecStringConstants::EProtocolRtpAvp, 
         							SdpCodecStringPool::StringTableL() ));
         							
	RPointerArray<CSdpFmtAttributeField>& rtpmaps = mediafields[0]->FormatAttributeFields();
	
	
	EUNIT_ASSERT(rtpmaps.Count() == 1);
	EUNIT_ASSERT(rtpmaps[0]->Attribute() == SdpCodecStringPool::StringPoolL().StringF( 
								SdpCodecStringConstants::EAttributeRtpmap, 
     							SdpCodecStringPool::StringTableL() ));
    EUNIT_ASSERT(rtpmaps[0]->Format() == _L8("96") );				
     	
 	const TDesC8& attributeValue = rtpmaps[0]->Value();
 	TSdpRtpmapValue rtpmapValue = TSdpRtpmapValue::DecodeL( attributeValue );
 	EUNIT_ASSERT(rtpmapValue.iEncName == _L8("H263-2000"));
 	EUNIT_ASSERT(rtpmapValue.iClockrate == _L8("90000"));
     	
   	// second media line in the answe, this should be valid
   	
   	
   	const TDesC8& fmtlist2 = mediafields[1]->FormatList();
	EUNIT_ASSERT(fmtlist2 == _L8("99"));
	
	EUNIT_ASSERT( mediafields[1]->IsValid());
	TUint port2 = mediafields[1]->Port();
  	EUNIT_ASSERT( mediafields[1]->Port() == 6000 );
  	
  	EUNIT_ASSERT( mediafields[1]->Media() == SdpCodecStringPool::StringPoolL().StringF( 
  									SdpCodecStringConstants::EMediaVideo, 
         							SdpCodecStringPool::StringTableL() ));
	EUNIT_ASSERT( mediafields[1]->Protocol() == SdpCodecStringPool::StringPoolL().StringF( 
  									SdpCodecStringConstants::EProtocolRtpAvp, 
         							SdpCodecStringPool::StringTableL() ));
         							
	RPointerArray<CSdpFmtAttributeField>& rtpmaps2 = mediafields[1]->FormatAttributeFields();
	
	
	EUNIT_ASSERT(rtpmaps2.Count() == 1);
	EUNIT_ASSERT(rtpmaps2[0]->Attribute() == SdpCodecStringPool::StringPoolL().StringF( 
								SdpCodecStringConstants::EAttributeRtpmap, 
     							SdpCodecStringPool::StringTableL() ));
    EUNIT_ASSERT(rtpmaps2[0]->Format() == _L8("99") );				
     	
 	const TDesC8& attributeValue2 = rtpmaps2[0]->Value();
 	TSdpRtpmapValue rtpmapValue2 = TSdpRtpmapValue::DecodeL( attributeValue );
 	EUNIT_ASSERT(rtpmapValue2.iEncName == _L8("H263-2000"));
 	EUNIT_ASSERT(rtpmapValue2.iClockrate == _L8("90000"));
		
	CleanupStack::PopAndDestroy( offer );				
    }


void UT_CMceSdpSession::UT_CMceSdpSession_DecodeOfferLL_CreateAnswer_Video_NOK_1(  )
    {
    // this function failes the offer and creates rejected answer
    iSdpSession->SetMediaSession( iInSession );
    iInSession->AttachSDPSessionL( *iSdpSession );
    
    const TUint32 KInetAddr2 = INET_ADDR(12,12,12,12);
    iInSession->iLocalIpAddress.SetAddress(KInetAddr2);
    
    // Create media line: m= This should correspond to the OFFER received!
    
	
	CSdpMediaField* media = CSdpMediaField::NewLC ( iVideo, 8000, iProtocol, _L8("96") );
	
	// Note the sendrcv, this should create bound stream as well since it's two way.
	RStringF attribute = SdpCodecStringPool::StringPoolL().StringF
    			( SdpCodecStringConstants::EAttributeSendrecv, SdpCodecStringPool::StringTableL() );
	CSdpAttributeField* mediaAttr = CSdpAttributeField::NewLC(attribute, _L8(""));
	media->AttributeFields().AppendL( mediaAttr );
    CleanupStack::Pop( mediaAttr );
    TUint payloadType = 96;
	// Get the encoding name
	TPtrC8 codecName = _L8("HMMH");	
	TUint clockRate = 90000;		
		
	// create rtpmap field for current codec
	CSdpFmtAttributeField* rtpmapAttribute = 
	    iVideoSdpCodec->EncodeRtpMapFieldsL( payloadType, 
											 codecName, 
										     clockRate,
										     KMceSdpOptionalEncodingParam );
	CleanupStack::PushL( rtpmapAttribute );
	media->FormatAttributeFields().AppendL( rtpmapAttribute );
	CleanupStack::Pop( rtpmapAttribute );

	
	CSdpDocument* offer = CSdpDocument::NewL();
	CleanupStack::PushL( offer );
	
	RPointerArray<CSdpMediaField>& mediafields = offer->MediaFields();
    mediafields.AppendL( media );
    CleanupStack::Pop( offer );
    CleanupStack::Pop( media );
    CleanupStack::PushL( offer );
    
     // Set iInSession name: s=
    offer->SetSessionNameL( _L8("") );

    const TUint32 KInetAddr = INET_ADDR(10,10,10,10);
	TInetAddr localIpAddress;
	localIpAddress.SetAddress(KInetAddr);
	
    CSdpOriginField* originField = CSdpOriginField::NewL( _L8("username"),
                                                          55555,
                                                          55555,
                                                          localIpAddress );
    offer->SetOriginField( originField );
    
    // Create connection field: c=
    CSdpConnectionField* connectionField = CSdpConnectionField::NewL( localIpAddress );
    offer->SetConnectionField( connectionField );
    
    User::LeaveIfError (offer->IsValid());
    
    TInt err = iSdpSession->DecodeOfferL(*offer, *iInSession );
    EUNIT_ASSERT(err == KMceSipWarnMediaTypeNotAvailable);
    
    iSdpSession->CreateAnswerL( *iInSession );
    
    
    
    
    CSdpOriginField* origin = offer->OriginField();
    const TDesC8& username = origin->UserName();
    EUNIT_ASSERT( _L8("-") == username );
    TInt64 sessionid = origin->SessionId(); 
    RStringF attr = origin->NetType();
    RStringF addr = origin->AddressType();
    
    EUNIT_ASSERT( attr == SdpCodecStringPool::StringPoolL().StringF( 
  									SdpCodecStringConstants::ENetType, 
         							SdpCodecStringPool::StringTableL() ));
	EUNIT_ASSERT( addr == SdpCodecStringPool::StringPoolL().StringF( 
  									SdpCodecStringConstants::EAddressTypeIP4, 
         							SdpCodecStringPool::StringTableL() ));
         							
    const TDesC8& inet = origin->Address();
    
    const TDesC8& sessionname = offer->SessionName();
    CSdpConnectionField* conn = offer->ConnectionField();
    RStringF attr2 = conn->NetType();
    RStringF addr2 = conn->AddressType();
    
    EUNIT_ASSERT( attr2 == SdpCodecStringPool::StringPoolL().StringF( 
  									SdpCodecStringConstants::ENetType, 
         							SdpCodecStringPool::StringTableL() ));
	EUNIT_ASSERT( addr2 == SdpCodecStringPool::StringPoolL().StringF( 
  									SdpCodecStringConstants::EAddressTypeIP4, 
         							SdpCodecStringPool::StringTableL() ));
         							
    const TDesC8& inet2 = conn->Address();
        
    
    EUNIT_ASSERT( mediafields.Count() == 1 );
    EUNIT_ASSERT( mediafields[0]->Port() == 0);
	
	CleanupStack::PopAndDestroy( offer );				
    }

// ---------------------------------------------------------
// Test generic
// ---------------------------------------------------------    
//
     
void UT_CMceSdpSession::UT_CMceSdpSession_EncodeSessionParamsL()
	{
    CSdpDocument* sdp = iSdpSession->CreateOfferL( *iAudioOutSession );
    CleanupStack::PushL( sdp );
    
    iSdpSession->EncodeSessionParamsL( *iAudioOutSession, *sdp );
    EUNIT_ASSERT( iSdpSession->iUserName->Des() == KMceSdpDefaultUserName );
    
    delete iAudioOutSession->iOriginator;iAudioOutSession->iOriginator = NULL;
    iAudioOutSession->iOriginator = HBufC8::NewL( 50 );
    
    iAudioOutSession->iOriginator->Des().Copy(_L("sip:username@blaa.com"));
    iSdpSession->EncodeSessionParamsL( *iAudioOutSession, *sdp );
    EUNIT_ASSERT( iSdpSession->iUserName->Des() == _L8("username"));
    
    iAudioOutSession->iOriginator->Des().Copy(_L("sip:username@blaah"));
    iSdpSession->EncodeSessionParamsL( *iAudioOutSession, *sdp );
    EUNIT_ASSERT( iSdpSession->iUserName->Des() == _L8("username"));
    
    iAudioOutSession->iOriginator->Des().Copy(_L("\"username\"<sip:username@blaah>"));
    iSdpSession->EncodeSessionParamsL( *iAudioOutSession, *sdp );
    EUNIT_ASSERT( iSdpSession->iUserName->Des() == _L8("username"));

    iAudioOutSession->iType = CMceComSession::EInSession;
    delete iAudioOutSession->iRecipient;iAudioOutSession->iRecipient = NULL;
    iAudioOutSession->iRecipient = HBufC8::NewL( 50 );

    iAudioOutSession->iRecipient->Des().Copy(_L("sip:zaikka@blaah"));
    iSdpSession->EncodeSessionParamsL( *iAudioOutSession, *sdp );
    EUNIT_ASSERT( iSdpSession->iUserName->Des() == _L8("zaikka"));
    
    iAudioOutSession->iRecipient->Des().Copy(_L("\"zaikka@blaah\"<sip:zaikka@blaah>"));
    iSdpSession->EncodeSessionParamsL( *iAudioOutSession, *sdp );
    EUNIT_ASSERT( iSdpSession->iUserName->Des() == _L8("zaikka"));
    
    iAudioOutSession->iRecipient->Des().Copy( _L("tel:+86123456789") );
    iSdpSession->EncodeSessionParamsL( *iAudioOutSession, *sdp );
    EUNIT_ASSERT( iSdpSession->iUserName->Des() == _L8("+86123456789") );

    iAudioOutSession->iRecipient->Des().Copy( _L("tel:123456789") );
    iSdpSession->EncodeSessionParamsL( *iAudioOutSession, *sdp );
    EUNIT_ASSERT( iSdpSession->iUserName->Des() == _L8("123456789") );

    iAudioOutSession->iRecipient->Des().Copy( _L("123456789") );
    iSdpSession->EncodeSessionParamsL( *iAudioOutSession, *sdp );
    EUNIT_ASSERT( iSdpSession->iUserName->Des() == _L8("123456789") );

    CleanupStack::PopAndDestroy( sdp );
	}
	
void UT_CMceSdpSession::UT_CMceSdpSession_EncodeConnectionFieldL()
    {
    // set the local ip address
    const TUint32 KInetAddr = INET_ADDR( 10, 10, 10, 10 );
    iAudioOutSession->iLocalIpAddress.SetAddress(KInetAddr);
    
    CSdpDocument* sdp = CSdpDocument::NewLC();

    iSdpSession->iIOldSchool = 1;
    CSdpConnectionField* nullAddress = 
                           CSdpConnectionField::DecodeLC( KMceSipNullAddressA );
    iSdpSession->EncodeConnectionFieldL( *iAudioOutSession, *sdp );
    EUNIT_ASSERT( sdp->ConnectionField()->Address() == nullAddress->Address() );
    CleanupStack::PopAndDestroy( nullAddress );

    iSdpSession->iIOldSchool = 0;
    iSdpSession->EncodeConnectionFieldL( *iAudioOutSession, *sdp );
    EUNIT_ASSERT( *( sdp->ConnectionField()->InetAddress() )
                      == iAudioOutSession->iLocalIpAddress );

    // set the session level c= line
    const TUint32 KInetAddr0 = INET_ADDR( 11, 11, 11, 11 );
    TInetAddr addr0;
    addr0.SetAddress( KInetAddr0 );
    CSdpConnectionField* connectionField = CSdpConnectionField::NewLC( addr0 );
    sdp->SetConnectionField( connectionField );
    CleanupStack::Pop( connectionField );

    CSdpMediaField* media = CSdpMediaField::NewLC( iAudio, 5000, iProtocol, _L8("97") );

    //set the first media level c= line
    const TUint32 KInetAddr1 = INET_ADDR( 12, 12, 12, 12 );
    TInetAddr addr1;
    addr1.SetAddress( KInetAddr1 );
    CSdpConnectionField* connField1 = CSdpConnectionField::NewLC( addr1 );
    User::LeaveIfError( ( media->ConnectionFields() ).Append( connField1 ) );
    CleanupStack::Pop( connField1 );

    CSdpMediaField* media2 = CSdpMediaField::NewLC ( iAudio, 6000, iProtocol, _L8("99") );

    RPointerArray<CSdpMediaField>& mediafields = sdp->MediaFields();
    mediafields.AppendL( media );
    mediafields.AppendL( media2 );

    CleanupStack::Pop( media2 );
    CleanupStack::Pop( media );

    iSdpSession->EncodeConnectionFieldL( *iAudioOutSession, *sdp );
    EUNIT_ASSERT( *( sdp->ConnectionField()->InetAddress() )
                      == iAudioOutSession->iLocalIpAddress );

    RPointerArray <CSdpMediaField>& mediaLines = sdp->MediaFields();

    EUNIT_ASSERT( *( mediaLines[0]->ConnectionFields()[0]->InetAddress() )
                                    == iAudioOutSession->iLocalIpAddress );
    EUNIT_ASSERT( ( mediaLines[0]->ConnectionFields() ).Count() == 1 );                                
    EUNIT_ASSERT( ( mediaLines[1]->ConnectionFields() ).Count() == 0 );

    // set the session level c= line
    connectionField = CSdpConnectionField::NewLC( addr0 );
    sdp->SetConnectionField( connectionField );
    CleanupStack::Pop( connectionField );

    // set the second media level c= line
   
    const TUint32 KInetAddr2 = INET_ADDR( 13, 13, 13, 13 );
    TInetAddr addr2;
    addr2.SetAddress( KInetAddr2 );
    CSdpConnectionField* connField2 = CSdpConnectionField::NewLC( addr2 );
    User::LeaveIfError( ( media2->ConnectionFields() ).Append( connField2 ) );
    CleanupStack::Pop( connField2 );
    

    iSdpSession->EncodeConnectionFieldL( *iAudioOutSession, *sdp );
    EUNIT_ASSERT( *( sdp->ConnectionField()->InetAddress() ) 
		                   == iAudioOutSession->iLocalIpAddress );
    EUNIT_ASSERT( *( mediaLines[0]->ConnectionFields()[0]->InetAddress() )
                                    == iAudioOutSession->iLocalIpAddress );
    EUNIT_ASSERT( *( mediaLines[1]->ConnectionFields()[0]->InetAddress() )
                                    == iAudioOutSession->iLocalIpAddress );
    // set the session level c= line
    sdp->SetConnectionField( NULL );
    

    iSdpSession->EncodeConnectionFieldL( *iAudioOutSession, *sdp );
    EUNIT_ASSERT( sdp->ConnectionField() == NULL );

    EUNIT_ASSERT( *( mediaLines[0]->ConnectionFields()[0]->InetAddress() )
                                                      == iAudioOutSession->iLocalIpAddress );
    EUNIT_ASSERT( *( mediaLines[1]->ConnectionFields()[0]->InetAddress() ) 
                                                     == iAudioOutSession->iLocalIpAddress );

    CleanupStack::PopAndDestroy( sdp );
   
	}
	
void UT_CMceSdpSession::UT_CMceSdpSession_EncodeClientAttributesLL()
    {

	CMceComSession* session = CreateSessionWithDowlinkL();
    CleanupStack::PushL( session );
    
    CMceSdpSession* sdpsession = CMceSdpSession::NewL( *session, *iManager );
    CleanupStack::PushL( sdpsession );

    CSdpDocument* sdp = CSdpDocument::DecodeL( KMceTestSdpAMR );
    CleanupStack::PushL( sdp );
    
    sdp->AttributeFields().ResetAndDestroy();
    sdp->BandwidthFields().ResetAndDestroy();
    EUNIT_ASSERT( sdp->AttributeFields().Count() == 0 );
    EUNIT_ASSERT( sdp->BandwidthFields().Count() == 0 );
    
    MCE_DELETE( session->iLocalSessionSDPLines );

    EUNIT_ASSERT( session->iLocalSessionSDPLines == NULL );
    EUNIT_ASSERT_LEAVE( sdpsession->EncodeClientAttributesL( *session, *sdp ) );
    EUNIT_ASSERT( sdp->AttributeFields().Count() == 0 );
    EUNIT_ASSERT( sdp->BandwidthFields().Count() == 0 );
    
    session->iLocalSessionSDPLines = new (ELeave) CDesC8ArrayFlat( 5 );
    sdpsession->EncodeClientAttributesL( *session, *sdp );
    EUNIT_ASSERT( sdp->AttributeFields().Count() == 0 );
    EUNIT_ASSERT( sdp->BandwidthFields().Count() == 0 );
    
    sdp->AttributeFields().ResetAndDestroy();
    sdp->BandwidthFields().ResetAndDestroy();
    MCE_DELETE( session->iLocalSessionSDPLines );
    session->iLocalSessionSDPLines = new (ELeave) CDesC8ArrayFlat( 5 );
    session->iLocalSessionSDPLines->AppendL( _L8("a=curr:qos local none\r\n") );
    sdpsession->EncodeClientAttributesL( *session, *sdp );
    EUNIT_ASSERT( sdp->AttributeFields().Count() == 1 );
    EUNIT_ASSERT( sdp->BandwidthFields().Count() == 0 );
    
    sdp->AttributeFields().ResetAndDestroy();
    sdp->BandwidthFields().ResetAndDestroy();
    MCE_DELETE( session->iLocalSessionSDPLines );
    session->iLocalSessionSDPLines = new (ELeave) CDesC8ArrayFlat( 5 );
    session->iLocalSessionSDPLines->AppendL( _L8("b=AS:17\r\n") );
    sdpsession->EncodeClientAttributesL( *session, *sdp );
    EUNIT_ASSERT( sdp->AttributeFields().Count() == 0 );
    EUNIT_ASSERT( sdp->BandwidthFields().Count() == 1 );
    sdpsession->CleanSessionParams(*sdp);
    EUNIT_ASSERT( sdp->AttributeFields().Count() == 0 );
    EUNIT_ASSERT( sdp->BandwidthFields().Count() == 0 );
  
    MCE_DELETE( session->iLocalSessionSDPLines );
    session->iLocalSessionSDPLines = new (ELeave) CDesC8ArrayFlat( 5 );
    session->iLocalSessionSDPLines->AppendL( _L8("x=foo:faa\r\n") );
    EUNIT_ASSERT_LEAVE( sdpsession->EncodeClientAttributesL( *session, *sdp ) );
    EUNIT_ASSERT( sdp->AttributeFields().Count() == 0 );
    EUNIT_ASSERT( sdp->BandwidthFields().Count() == 0 );
    
    CleanupStack::PopAndDestroy( sdp );
    CleanupStack::PopAndDestroy( 2 );
    
    
    }

void UT_CMceSdpSession::UT_CMceSdpSession_DecodeClientAttributesLL()
    {
	CMceComSession* session = CreateSessionWithDowlinkL();
    CleanupStack::PushL( session );
    
    CMceSdpSession* sdpsession = CMceSdpSession::NewL( *session, *iManager );
    CleanupStack::PushL( sdpsession );
    
    CSdpDocument* sdp = CSdpDocument::DecodeL( KMceTestSdpAMRWithb );
    CleanupStack::PushL( sdp );
    
    EUNIT_ASSERT( session->iRemoteSessionSDPLines->Count() == 0 );
    
    sdpsession->DecodeClientAttributesL( *sdp, *session );
    EUNIT_ASSERT( session->iRemoteSessionSDPLines->Count() == 3 );
    
    CleanupStack::PopAndDestroy( sdp );

    MCE_DELETE( session->iRemoteSessionSDPLines );
    session->iRemoteSessionSDPLines = new (ELeave) CDesC8ArrayFlat( 5 );
    EUNIT_ASSERT( session->iRemoteSessionSDPLines->Count() == 0 );

    sdp = CSdpDocument::DecodeL( KMceTestSdpAMR );
    CleanupStack::PushL( sdp );

    sdpsession->DecodeClientAttributesL( *sdp, *session );
    EUNIT_ASSERT( session->iRemoteSessionSDPLines->Count() == 2 );
    
    CleanupStack::PopAndDestroy( sdp );
    CleanupStack::PopAndDestroy( 2 );
    
    }


void UT_CMceSdpSession::UT_CMceSdpSession_ForkLL()
    {
    EUNIT_ASSERT_LEAVE( iSdpSession->ForkL() );

    iSdpSession->iIsMaster = EFalse;
    EUNIT_ASSERT_LEAVE( iSdpSession->ForkL() );

    iSdpSession->iIsMaster = ETrue;
    iSdpSession->iBackup = iAudioOutSession;
    EUNIT_ASSERT_LEAVE( iSdpSession->ForkL() );

    iSdpSession->iIsMaster = ETrue;
    iSdpSession->iBackup = NULL;
    EUNIT_ASSERT_LEAVE( iSdpSession->ForkL() );
  
    iSdpSession->iInitialSession = iAudioOutSession->CloneL();
    TMceMediaNegotiated::TMceEndState::SetAsCurrentStateL( *iAudioOutSession );
 
    CMceSdpSession& anew = iSdpSession->ForkL();
    
    EUNIT_ASSERT( anew.MediaSession() != NULL );    
    }


void UT_CMceSdpSession::UT_CMceSdpSession_ConsumesL()
    {
    EUNIT_ASSERT( !iSdpSession->Consumes( 1 ) );
    }


void UT_CMceSdpSession::UT_CMceSdpSession_ContextSwitchL()
    {
    TMceMediaNegotiated::TMceEndState::SetAsCurrentStateL( *iAudioOutSession );
    
    iSdpSession->iInitialSession = iAudioOutSession->CloneL();
    EUNIT_ASSERT( iSdpSession->IsMaster() );

    iSdpSession->ContextSwitch();
    EUNIT_ASSERT( iSdpSession->IsMaster() );

    iSdpSession->iIsMaster = EFalse;
    iSdpSession->ContextSwitch();
    EUNIT_ASSERT( !iSdpSession->IsMaster() );

    iSdpSession->iIsMaster = ETrue;
    CMceSdpSession& anew = iSdpSession->ForkL();

    iSdpSession->ContextSwitch( &anew );
    EUNIT_ASSERT( !iSdpSession->IsMaster() );
    EUNIT_ASSERT( anew.IsMaster() );
    }

void UT_CMceSdpSession::UT_CMceSdpSession_AttachContextL()
    {
    CSIPDialogAssocBase* a = (CSIPDialogAssocBase*)1;
    CSIPDialogAssocBase* b = (CSIPDialogAssocBase*)2;
    
    iAudioOutSession->AttachSDPSessionL( *iSdpSession );
    
    iSdpSession->AttachContext( NULL );
    EUNIT_ASSERT( iSdpSession->Context() == NULL );
    EUNIT_ASSERT( iSdpSession->iRequestedContext == NULL );

    iSdpSession->iContext = a;
    iSdpSession->iRequestedContext = NULL;
    iSdpSession->AttachContext( b );
    EUNIT_ASSERT( iSdpSession->Context() == a );
    EUNIT_ASSERT( iSdpSession->iRequestedContext == b );

    iSdpSession->iRequestedContext = NULL;
    iSdpSession->AttachContext( a );
    EUNIT_ASSERT( iSdpSession->Context() == a );
    EUNIT_ASSERT( iSdpSession->iRequestedContext == NULL );
    
    
    
    }


void UT_CMceSdpSession::UT_CMceSdpSession_ContextSwitchRequestedL()
    {
    CSIPDialogAssocBase* a = (CSIPDialogAssocBase*)1;
    CSIPDialogAssocBase* b = (CSIPDialogAssocBase*)2;
    
    EUNIT_ASSERT( !iSdpSession->ContextSwitchRequested() );

    iSdpSession->iContext = a;
    EUNIT_ASSERT( !iSdpSession->ContextSwitchRequested() );

    iSdpSession->iRequestedContext = b;
    EUNIT_ASSERT( iSdpSession->ContextSwitchRequested() );

    iSdpSession->iRequestedContext = a;
    EUNIT_ASSERT( !iSdpSession->ContextSwitchRequested() );
    
    
    }

// ---------------------------------------------------------
// Helpers
// ---------------------------------------------------------    
//

CMceComSession* UT_CMceSdpSession::CreateSessionWithDowlinkL()
	{
	CMceComSession* session = CMceComSession::NewL( CMceComSession::EOutSession );
	CleanupStack::PushL( session );
	
	CMceComAudioStream* downlinkStream = CMceComAudioStream::NewL();
	CleanupStack::PushL( downlinkStream );
	downlinkStream->iLocalMediaPort = 5000;
	
	// downlink/recvonly in sdp 		
	CMceComRtpSource* rtpSource = CMceComRtpSource::NewL();
	CleanupStack::PushL( rtpSource );
	downlinkStream->SetSourceL( rtpSource );
    CleanupStack::Pop( rtpSource );
	CMceComSpeakerSink* speakerSink = CMceComSpeakerSink::NewL();
	CleanupStack::PushL( speakerSink );
	downlinkStream->AddSinkL( speakerSink );
	CleanupStack::Pop( speakerSink );
    
    CMceComAMRCodec *amr = CMceComAMRCodec::NewL(_L8("AMR"));
	CleanupStack::PushL( amr );
	amr->iSamplingFreq = 8000;
	amr->iPayloadType = 97;

	downlinkStream->AddCodecL( amr );
	CleanupStack::Pop( amr ); 
	
	session->AddStreamL( downlinkStream ); 
	CleanupStack::Pop( downlinkStream ); 
    
	 
	const TUint32 KInetAddr = INET_ADDR(12,12,12,12);
    session->iLocalIpAddress.SetAddress(KInetAddr);
    session->InitializeL();
    
	CleanupStack::Pop( session ); 
	return session; 
	
	}

void UT_CMceSdpSession::EncodeDirectionAttrL( CSdpMediaField& aMediaLine, 
                                              TInt aDirection )
    {
    
    for( TInt i = 0;i<aMediaLine.AttributeFields().Count();i++)
        {
        if ( aMediaLine.AttributeFields()[ i ]->Attribute() == 
             SDP_STRING_L( SdpCodecStringConstants::EAttributeRecvonly ) ||
             aMediaLine.AttributeFields()[ i ]->Attribute() == 
             SDP_STRING_L( SdpCodecStringConstants::EAttributeSendrecv ) ||
             aMediaLine.AttributeFields()[ i ]->Attribute() == 
             SDP_STRING_L( SdpCodecStringConstants::EAttributeSendonly ) ||
             aMediaLine.AttributeFields()[ i ]->Attribute() == 
             SDP_STRING_L( SdpCodecStringConstants::EAttributeInactive ) )
            {
            delete aMediaLine.AttributeFields()[ i ];
            aMediaLine.AttributeFields().Remove( i-- );
            }
             
        }
    
    RStringF direction = SDP_STRING_L( aDirection );

    CSdpAttributeField* mediaAttr = 
        CSdpAttributeField::NewLC( direction, KNullDesC8 );
        
    aMediaLine.AttributeFields().AppendL( mediaAttr );
    CleanupStack::Pop( mediaAttr );
    
    }
	

	
CMceComSession* UT_CMceSdpSession::CreateSessionWithVideoDowlinkL()
	{

	CMceComSession* session = CMceComSession::NewL( CMceComSession::EOutSession );
	CleanupStack::PushL( session );
	
	CMceComVideoStream* downlinkStream = CMceComVideoStream::NewL();
	CleanupStack::PushL( downlinkStream );
	downlinkStream->iLocalMediaPort = 5000;
	
	// downlink/recvonly in sdp 		
	CMceComRtpSource* rtpSource = CMceComRtpSource::NewL();
	CleanupStack::PushL( rtpSource );
	downlinkStream->SetSourceL( rtpSource );
    CleanupStack::Pop( rtpSource );
	CMceComDisplaySink* displaySink = CMceComDisplaySink::NewL();
	CleanupStack::PushL( displaySink );
	downlinkStream->AddSinkL( displaySink );
	CleanupStack::Pop( displaySink );
	  
	AddVideoCodecL( *downlinkStream, 96, TSize( 120, 160 ) );
	
	session->AddStreamL( downlinkStream ); 
	CleanupStack::Pop( downlinkStream );
	 
	const TUint32 KInetAddr = INET_ADDR(12,12,12,12);
    session->iLocalIpAddress.SetAddress(KInetAddr);
    session->InitializeL();
    
	CleanupStack::Pop( session ); 
	return session; 
	}

void UT_CMceSdpSession::AddVideoUplinkL( CMceComSession& aSession )
    {
    CMceComVideoStream* uplinkStream = CMceComVideoStream::NewL();
	CleanupStack::PushL( uplinkStream );
	uplinkStream->iLocalMediaPort = 6000;
		
	// uplink/sendonly in sdp 		
	CMceComCameraSource* cameraSource = CMceComCameraSource::NewL();
	CleanupStack::PushL( cameraSource );
    uplinkStream->SetSourceL( cameraSource );
	CleanupStack::Pop( cameraSource );
	
	CMceComRtpSink* rtpSink = CMceComRtpSink::NewL();
	CleanupStack::PushL( rtpSink );
	uplinkStream->AddSinkL( rtpSink );
	CleanupStack::Pop( rtpSink );
	
	AddVideoCodecL( *uplinkStream, 96, TSize( 120, 160 ) );
	
	AddVideoCodecL( *uplinkStream, 97, TSize( 300, 300 ) );
  
	aSession.AddStreamL( uplinkStream ); 
	CleanupStack::Pop( uplinkStream );
	aSession.InitializeL();
	
    }

void UT_CMceSdpSession::AddVideoCodecL( CMceComVideoStream& aStream, TUint aPayloadType, TSize aResolution )
    {
    CMceComH263Codec* h263 = CMceComH263Codec::NewL(_L8("H263"));
	CleanupStack::PushL( h263 );
	h263->iPayloadType = aPayloadType;
	h263->iFrameRate = 14.97;
	h263->iResolutionWidth = aResolution.iWidth;
	h263->iResolutionHeight = aResolution.iHeight;
	h263->InitializeL( aStream );
    aStream.AddCodecL( h263 );
    CleanupStack::Pop( h263 );
    }
	

void UT_CMceSdpSession::UT_CMceSdpSession_StoreRemoteOriginL()
    {    		
	CSdpDocument* offer = CSdpDocument::NewLC();
    iSdpSession->iSdpDocument = offer;    
	TInetAddr remoteIP;
	remoteIP.SetAddress( INET_ADDR( 10,10,10,10 ) );

    // Origin field is NULL
    EUNIT_ASSERT_LEAVE( iSdpSession->StoreRemoteOriginL() )

    // Origin address is IP address, but not 0.0.0.0
	_LIT8( KMyUserName, "username" );
	const TInt64 KSessionId( 1234 );
	const TInt64 KSessionVersion( 5678 );    
    CSdpOriginField* originField = 
        CSdpOriginField::NewL( KMyUserName, KSessionId, KSessionVersion, remoteIP );
    offer->SetOriginField( originField );  
    EUNIT_ASSERT( iSdpSession->iRemoteOrigin == NULL )
    iSdpSession->StoreRemoteOriginL();
    EUNIT_ASSERT( *iSdpSession->iRemoteOrigin->InetAddress() == remoteIP )

    // Origin address is IP address 0.0.0.0, check that cloning it is possible
	TInetAddr zeroIP;
	zeroIP.SetAddress( INET_ADDR( 0,0,0,0 ) );
    originField->SetInetAddress( zeroIP );
    iSdpSession->StoreRemoteOriginL();
    EUNIT_ASSERT( *iSdpSession->iRemoteOrigin->InetAddress() == zeroIP )
    
    // Origin address is not an IP address
    _LIT8( KTextHost, "valid.address.com" );
    originField->SetAddressL( 
        KTextHost, originField->NetType(), originField->AddressType() );
    iSdpSession->StoreRemoteOriginL();
    EUNIT_ASSERT( iSdpSession->iRemoteOrigin->InetAddress() == NULL )
    EUNIT_ASSERT_EQUALS( KTextHost(), iSdpSession->iRemoteOrigin->Address() )
    
    CleanupStack::PopAndDestroy( offer );
    }
	

void UT_CMceSdpSession::UT_CMceSdpSession_StoreRemoteMediaFieldsL()
    {    		
	CSdpDocument* mediaSdpOld = CSdpDocument::DecodeLC( KMceSdpMediaFieldOld() );
    iSdpSession->iSdpDocument = mediaSdpOld;
    
    // Test1: Ensure remote media fields are handled.
    iSdpSession->StoreRemoteMediaFieldsL();
    EUNIT_ASSERT( iSdpSession->iSdpRemoteMediaFields.Count() == 1 )
    EUNIT_ASSERT( *(iSdpSession->iSdpRemoteMediaFields[0]) == *(mediaSdpOld->MediaFields()[0]) )
    
    // Test2: Ensure old remote media fields is replaced by the new one.
	CSdpDocument* mediaSdpNew = CSdpDocument::DecodeLC(KMceSdpMediaFieldNew());
    iSdpSession->iSdpDocument = mediaSdpNew;
    iSdpSession->StoreRemoteMediaFieldsL();
    EUNIT_ASSERT( *(iSdpSession->iSdpRemoteMediaFields[0]) == *(mediaSdpNew->MediaFields()[0]) )
    
    CleanupStack::PopAndDestroy( 2 ); // mediaSdpOld, mediaSdpNew
    }
	

void UT_CMceSdpSession::UT_CMceSdpSession_CompareMediaLinesL()
    {    		
    RPointerArray<CSdpMediaField> src;
    RPointerArray<CSdpMediaField> des;
    CleanupClosePushL( src );
    CleanupClosePushL( des );
    EUNIT_ASSERT( iSdpSession->CompareMediaLines(src, des) == ETrue )
    
    RStringF audio = MCE_SDP_STRING_AUDIOL();
    RStringF protocol = SDP_STRINGL( SdpCodecStringConstants::EProtocolRtpAvp );
    CSdpMediaField* audioLine = 
            CSdpMediaField::NewLC( audio, 5005, protocol, _L8("0") );    
    CSdpMediaField* audioLine2 = 
            CSdpMediaField::NewLC( audio, 5105, protocol, _L8("2") );
    CSdpMediaField* audioLine3 = 
            CSdpMediaField::NewLC( audio, 5305, protocol, _L8("5") );
    
    src.AppendL( audioLine );
    des.AppendL( audioLine );
    des.AppendL( audioLine2 );
    EUNIT_ASSERT( iSdpSession->CompareMediaLines(src, des) == EFalse )
    
    src.Reset();
    des.Reset();
    src.AppendL( audioLine );
    src.AppendL( audioLine2 );
    des.AppendL( audioLine );
    des.AppendL( audioLine2 );
    EUNIT_ASSERT( iSdpSession->CompareMediaLines(src, des) == ETrue )

    src.Reset();
    des.Reset();
    src.AppendL( audioLine );
    src.AppendL( audioLine3 );
    des.AppendL( audioLine );
    des.AppendL( audioLine2 );
    EUNIT_ASSERT( iSdpSession->CompareMediaLines(src, des) == EFalse )

    src.Reset();
    des.Reset();
    src.AppendL( audioLine );
    src.AppendL( audioLine3 );
    src.AppendL( audioLine2 );
    des.AppendL( audioLine3 );
    des.AppendL( audioLine );
    des.AppendL( audioLine2 );
    EUNIT_ASSERT( iSdpSession->CompareMediaLines(src, des) == ETrue )

    CleanupStack::PopAndDestroy( 3 ); // audioLine, audioLine2, audioLine3
    CleanupStack::Pop(2);
    src.Reset();
    des.Reset();
    }
    	


void UT_CMceSdpSession::UT_CMceSdpSession_SetRemoteIpAddressL()
    {
    TInetAddr remoteIP;
    remoteIP.SetAddress( INET_ADDR( 10,36,7,239 ) );
    
    // Empty SDP document, Without "c=" line
    CSdpDocument* offer = CSdpDocument::NewLC();   
    EUNIT_ASSERT( iSdpSession->SetRemoteIpAddressL( *iInSession, *offer ) 
        == KMceSipWarnIncompatibleNetworkAddressFormat )
    CleanupStack::PopAndDestroy( offer );
    
    // Only 1 audio c-line
    offer = CSdpDocument::DecodeLC( KOfferOldSchoolOrigin );
    EUNIT_ASSERT( iSdpSession->SetRemoteIpAddressL( *iInSession, *offer ) == KErrNone );
    EUNIT_ASSERT( iInSession->iRemoteIpAddress.CmpAddr( remoteIP ) );
    CleanupStack::PopAndDestroy( offer );
    
    // without session level c-line, 1 audio c-line, and 1 invalid video c-line,
    offer = CSdpDocument::DecodeLC( KConnectionField1 );
    EUNIT_ASSERT( iSdpSession->SetRemoteIpAddressL( *iInSession, *offer ) == KErrNone );
    EUNIT_ASSERT( iInSession->iRemoteIpAddress.CmpAddr( remoteIP ) );
    CleanupStack::PopAndDestroy( offer );
    
    // 1 session level c-line, 1 audio c-line, and 1 invalid video c-line
    offer = CSdpDocument::DecodeLC( KConnectionField2 );
    EUNIT_ASSERT( iSdpSession->SetRemoteIpAddressL( *iInSession, *offer ) == KErrNone );
    EUNIT_ASSERT( iInSession->iRemoteIpAddress.CmpAddr( remoteIP ) );
    CleanupStack::PopAndDestroy( offer );
    }
    	
	
	
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceSdpSession,
    "SDP session tests",
    "UNIT" )

EUNIT_TEST(
    "MatchingStreamsToMLinesL - test",
    "CMceSdpSession",
    "MatchingStreamsToMLinesL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpSession_MatchingStreamsToMLinesL, Teardown)    

EUNIT_TEST(
    "DecodePullModeUpdateMediaLineL - test",
    "CMceSdpSession",
    "DecodePullModeUpdateMediaLineL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpSession_DecodePullModeUpdateMediaLineL, Teardown)    


EUNIT_TEST(
    "CreateOfferL(a) :)test ",
    "CMceSdpSession",
    "OK_1",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpSession_CreateOfferLL_OK_1, Teardown)


EUNIT_TEST(
    "CreateOfferL(a) :)test ",
    "CMceSdpSession",
    "REFRESH_OK_1",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpSession_CreateOfferLL_REFRESH_OK_1, Teardown)


EUNIT_TEST(
    "CreateOfferL(a) (:test ",
    "CMceSdpSession",
    "NOK_1",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpSession_CreateOfferLL_NOK_1, Teardown)

EUNIT_TEST(
    "DecodeAnswerL(a) :)test ",
    "CMceSdpSession",
    "OK_1",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpSession_DecodeAnswerLL_OK_1, Teardown)

EUNIT_TEST(
    "DecodeAnswerL(a) :)test ",
    "CMceSdpSession",
    "OK_2",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpSession_DecodeAnswerLL_OK_2, Teardown)

EUNIT_TEST(
    "DecodeAnswerL(a) :(test ",
    "CMceSdpSession",
    "NOK_1",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpSession_DecodeAnswerLL_NOK_1, Teardown)
                       
EUNIT_TEST(
    "DecodeOfferL(a) :)test ",
    "CMceSdpSession",
    "OK_1",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpSession_DecodeOfferLL_CreateAnswer_OK_1, Teardown)

EUNIT_TEST(
    "DecodeOfferL(a) :(test ",
    "CMceSdpSession",
    "NOK_1",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpSession_DecodeOfferLL_CreateAnswer_NOK_1, Teardown)
    

EUNIT_TEST(
    "CreateOfferL(v) :)test ",
    "CMceSdpSession",
    "Video_OK_1",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpSession_CreateOfferLL_Video_OK_1, Teardown)

EUNIT_TEST(
    "DecodeAnswerL(v) :)test ",
    "CMceSdpSession",
    "OK_1",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpSession_DecodeAnswerLL_Video_OK_1, Teardown)

EUNIT_TEST(
    "DecodeAnswerL(v) :)test ",
    "CMceSdpSession",
    "Video_OK_2",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpSession_DecodeAnswerLL_Video_OK_2, Teardown)

EUNIT_TEST(
    "DecodeAnswerL(v) :(test ",
    "CMceSdpSession",
    "Video_NOK_1",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpSession_DecodeAnswerLL_Video_NOK_1, Teardown)
                     
EUNIT_TEST(
    "DecodeOfferL(v) :)test ",
    "CMceSdpSession",
    "Video_OK_1",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpSession_DecodeOfferLL_CreateAnswer_Video_OK_1, Teardown)

EUNIT_TEST(
    "DecodeOfferL(v) :(test ",
    "CMceSdpSession",
    "Video_NOK_1",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpSession_DecodeOfferLL_CreateAnswer_Video_NOK_1, Teardown)

EUNIT_TEST(
    "EncodeSessionParamsL - test ",
    "CMceSdpSession",
    "OK_1",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpSession_EncodeSessionParamsL, Teardown)    

EUNIT_TEST(
    "EncodeConnectionFieldL - test ",
    "CMceSdpSession",
    "OK_1",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpSession_EncodeConnectionFieldL, Teardown)    

EUNIT_TEST(
    "c-field inside m-line - test",
    "CMceSdpSession",
    "DecodeOfferL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpSession_DecodeOfferL_1L, Teardown)    

EUNIT_TEST(
    "No rtpmap lines - test",
    "CMceSdpSession",
    "DecodeOfferL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpSession_DecodeOfferL_2L, Teardown)    

EUNIT_TEST(
    "remove media lines - test",
    "CMceSdpSession",
    "DecodeOffer3L",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpSession_DecodeOfferL_3L, Teardown)    
    
EUNIT_TEST(
    "iStoreRemoteOrigin - test",
    "CMceSdpSession",
    "DecodeOffer4L",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpSession_DecodeOfferL_4L, Teardown) 

EUNIT_TEST(
    "ForkL - test",
    "CMceSdpSession",
    "ForkL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpSession_ForkLL, Teardown)    


EUNIT_TEST(
    "Consumes - test",
    "CMceSdpSession",
    "Consumes",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpSession_ConsumesL, Teardown)    

EUNIT_TEST(
    "ContextSwitch - test",
    "CMceSdpSession",
    "ContextSwitch",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpSession_ContextSwitchL, Teardown)    

EUNIT_TEST(
    "AttachContext - test",
    "CMceSdpSession",
    "AttachContext",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpSession_AttachContextL, Teardown)    

EUNIT_TEST(
    "ContextSwitchRequested - test",
    "CMceSdpSession",
    "ContextSwitchRequested",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpSession_ContextSwitchRequestedL, Teardown)    

EUNIT_TEST(
    "EncodeClientAttributesL - test ",
    "CMceSdpSession",
    "EncodeClientAttributesL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpSession_EncodeClientAttributesLL, Teardown)    

EUNIT_TEST(
    "DecodeClientAttributesL - test ",
    "CMceSdpSession",
    "DecodeClientAttributesL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpSession_DecodeClientAttributesLL, Teardown)    

EUNIT_TEST(
    "StoreRemoteOriginL - test ",
    "CMceSdpSession",
    "StoreRemoteOriginL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpSession_StoreRemoteOriginL, Teardown)    

EUNIT_TEST(
	"StoreRemoteMediaFieldsL - test ",
    "CMceSdpSession",
    "StoreRemoteMediaFieldsL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpSession_StoreRemoteMediaFieldsL, Teardown)  

EUNIT_TEST(
	"CompareMediaLines - test ",
    "CMceSdpSession",
    "CompareMediaLines",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpSession_CompareMediaLinesL, Teardown)  

EUNIT_TEST(
    "SetRemoteIpAddressL - test ",
    "CMceSdpSession",
    "SetRemoteIpAddressL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpSession_SetRemoteIpAddressL, Teardown)    


EUNIT_END_TEST_TABLE

//  END OF FILE
