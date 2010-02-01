/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
#include "MceMediaManagerTestHelper.h"
#include "mcecomfactory.h"
#include "mcedefs.h"
#include "mceserial.h"
#include "mcemediamanager.h"
#include "mcesrvstream.h"
#include "mcesrvsink.h"
#include "mcesrvsource.h"
#include "mcecomstreambundle.h"
#include "mcefilesource.h"
#include "CMCCController_stub.h"
#include <sdpdocument.h>

_LIT8( KTestFile, "C:\\test.3gp");

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>

#define SRVSTREAM( n ) aSession.MccStreams()[ n ]


CMceComSession* MceMediaManagerTestHelper::CreateInSessionL()
    {
	CMceComSession* session = NULL;
	CMceComSession* inSession = CMceComSession::NewL( CMceComSession::EInSession );

    //CleanupStack::PushL( inSession );
    session = EncodeDecodeSessionL( inSession );
    //CleanupStack::PopAndDestroy( inSession );
    
    return session;
        
    }

CMceComSession* MceMediaManagerTestHelper::CreateAudioOutSessionL( CMceComSession* aSession, 
                                                                   const TDesC8& aFmtp,
                                                                   TBuf8<KMceMaxSdpNameLength> aSdpName )
	{

	TMceComFactory factory;

	CMceComSession* session = NULL;
	CMceComSession* outSession = aSession;
	if ( !outSession )
	    {
    	outSession = CMceComSession::NewL( CMceComSession::EOutSession );
	    CleanupStack::PushL( outSession );
	    }

	CMceComAudioStream* downlinkStream = 
	    static_cast<CMceComAudioStream*>( factory.MediaStreamFactory().CreateLC( KMceAudio ) );
	downlinkStream->iLocalMediaPort = KMceMMTestAudioLocalPort;
	 
	CMceComRtpSource* rtpSource = 
	    static_cast<CMceComRtpSource*>( factory.SourceFactory().CreateLC( KMceRTPSource ) );
	    
	rtpSource->iBufferLength = KMceMMTestDefaultBufferLength;
	rtpSource->iBufferTreshold = KMceMMTestDefaultBufferThreshold;
	
	CMceComSpeakerSink* speakerSink = 
	    static_cast<CMceComSpeakerSink*>( factory.SinkFactory().CreateLC( KMceSpeakerSink ) );
	downlinkStream->AddSinkL( speakerSink );
	CleanupStack::Pop( speakerSink );

	downlinkStream->SetSourceL( rtpSource );
	CleanupStack::Pop( rtpSource );

    CMceComAudioCodec* amr = 
        static_cast<CMceComAudioCodec*>( factory.AudioCodecFactory().CreateLC( aSdpName ) );
	amr->iSamplingFreq = KMceMMTestAudioSamplingFreq;
	amr->iPayloadType = KMceMMTestAudioPayloadType;
	amr->iCodecMode = KMceMMTestAudioCodecMode;
	amr->iAllowedBitrates = KMceMMTestAudioAllowedBitrates;
	amr->iPTime = KMceMMTestAudioPTime;
	amr->iMaxPTime = KMceMMTestAudioMaxPTime;
	if ( aFmtp.Length() > 0 )
	    {
	    amr->SetFmtpAttributeL( aFmtp );
	    }
	downlinkStream->AddCodecL( amr );
	CleanupStack::Pop( amr );

	CMceComAudioStream* uplinkStream = 
	    static_cast<CMceComAudioStream*>( factory.MediaStreamFactory().CreateLC( KMceAudio ) );
	uplinkStream->iLocalMediaPort = KMceMMTestAudioLocalPort;
	 
	CMceComMicSource* micSource = 
	    static_cast<CMceComMicSource*>( factory.SourceFactory().CreateLC( KMceMicSource ) );
	
	CMceComRtpSink* rtpSink = 
	    static_cast<CMceComRtpSink*>( factory.SinkFactory().CreateLC( KMceRTPSink ) );
	uplinkStream->AddSinkL( rtpSink );
	CleanupStack::Pop( rtpSink );

	uplinkStream->SetSourceL( micSource );
	CleanupStack::Pop( micSource );

	
    amr = static_cast<CMceComAudioCodec*>( factory.AudioCodecFactory().CreateLC( aSdpName ) );
	amr->iSamplingFreq = KMceMMTestAudioSamplingFreq;
	amr->iPayloadType = KMceMMTestAudioPayloadType;
	amr->iCodecMode = KMceMMTestAudioCodecMode;
	amr->iAllowedBitrates = KMceMMTestAudioAllowedBitrates;
	amr->iBitrate = KMceMMTestAudioBitrate;
	amr->iPTime = KMceMMTestAudioPTime;
	amr->iMaxPTime = KMceMMTestAudioMaxPTime;
	if ( aFmtp.Length() > 0 )
	    {
	    amr->SetFmtpAttributeL( aFmtp );
	    }
	uplinkStream->AddCodecL( amr );
	CleanupStack::Pop( amr );

    downlinkStream->BindL( uplinkStream );
    CleanupStack::Pop( uplinkStream );
	
    delete downlinkStream->iLocalMediaSDPLines;
    downlinkStream->iLocalMediaSDPLines = NULL;
    downlinkStream->iLocalMediaSDPLines = new (ELeave) CDesC8ArrayFlat( 5 );
    downlinkStream->iLocalMediaSDPLines->AppendL( _L8("a=client:media attribute 1\r\n") );
    downlinkStream->iLocalMediaSDPLines->AppendL( _L8("a=client:media attribute 2\r\n") );

	outSession->AddStreamL( downlinkStream );   
	CleanupStack::Pop( downlinkStream );

	outSession->InitializeL();
    outSession->iSessionModifiers[ KMcePreconditions ] = KMcePreconditionsSupported;
    delete outSession->iLocalSessionSDPLines;
    outSession->iLocalSessionSDPLines = NULL;
    outSession->iLocalSessionSDPLines = new (ELeave) CDesC8ArrayFlat( 5 );
    outSession->iLocalSessionSDPLines->AppendL( _L8("a=client:session attribute 1\r\n") );
    outSession->iLocalSessionSDPLines->AppendL( _L8("a=client:session attribute 2\r\n") );
    
    if ( !aSession )
        {
        if ( aFmtp.Length() == 0 )
            {
            CleanupStack::Pop( outSession );
            session = EncodeDecodeSessionL( outSession );
            }
        else
            {
            CleanupStack::Pop( outSession );
            session = outSession;
            }
        }
    else
        {
        session = outSession;
        }

    session->iUseLocalPreconditions = ETrue;
    return session;

	
	}


CMceComSession* MceMediaManagerTestHelper::CreateVideoOutSessionL( CMceComSession* aSession, TBool aUseAvc )
	{
	
	TMceComFactory factory;

	CMceComSession* session = NULL;
	CMceComSession* outSession = aSession;
	if ( !outSession )
	    {
    	outSession = CMceComSession::NewL( CMceComSession::EOutSession );
	    CleanupStack::PushL( outSession );
	    }
//downlink

	CMceComMediaStream* downlinkStream = CreateVideoDownLinkL( KMceMMTestVideoPayloadType, aUseAvc );
	CleanupStack::PushL( downlinkStream );

//uplink
	CMceComVideoStream* uplinkStream = 
	    static_cast<CMceComVideoStream*>( factory.MediaStreamFactory().CreateLC( KMceVideo ) );
	uplinkStream->iLocalMediaPort = KMceMMTestVideoLocalPort;

	CMceComCameraSource* cameraSource = 
	    static_cast<CMceComCameraSource*>( factory.SourceFactory().CreateLC( KMceCameraSource ) );

	
	CMceComRtpSink* rtpSink = 
	    static_cast<CMceComRtpSink*>( factory.SinkFactory().CreateLC( KMceRTPSink ) );
	uplinkStream->AddSinkL( rtpSink );
	CleanupStack::Pop( rtpSink );

	uplinkStream->SetSourceL( cameraSource );
	CleanupStack::Pop( cameraSource );

    if ( aUseAvc )
        {
        CMceComVideoCodec* avc = 
            static_cast<CMceComVideoCodec*>( factory.VideoCodecFactory().CreateLC( KMceSDPNameH264() ) );
    	avc->iClockRate = KMceMMTestVideoClockrate;
    	avc->iPayloadType = KMceMMTestVideoPayloadType;
    	avc->iCodecMode = 0;
    	avc->iBitrate = KMceMMTestVideoBitrate;
    	uplinkStream->AddCodecL( avc );
    	CleanupStack::Pop( avc );
        }
    else
        {
        CMceComVideoCodec* h263 = 
            static_cast<CMceComVideoCodec*>( factory.VideoCodecFactory().CreateLC( KMceSDPNameH263() ) );
    	h263->iClockRate = KMceMMTestVideoClockrate;
    	h263->iPayloadType = KMceMMTestVideoPayloadType;
    	h263->iCodecMode = KMceMMTestVideoCodecMode;
    	h263->iAllowedBitrates = KMceMMTestVideoAllowedBitrates;
    	h263->iBitrate = KMceMMTestVideoBitrate;
    	uplinkStream->AddCodecL( h263 );
    	CleanupStack::Pop( h263 );
        }

    downlinkStream->BindL( uplinkStream );
    CleanupStack::Pop( uplinkStream );
	
	outSession->AddStreamL( downlinkStream );   
	CleanupStack::Pop( downlinkStream );


//display
	CMceComMediaStream* localStream = CreateLocalVideoStreamL( cameraSource );
	CleanupStack::PushL( localStream );
	outSession->AddStreamL( localStream );   
	CleanupStack::Pop( localStream );
	
	outSession->InitializeL();
	
    if ( !aSession )
        {
        CleanupStack::Pop( outSession );
        session = EncodeDecodeSessionL( outSession );
        }
    else
        {
        session = outSession;
        }
    
    return session;
	
	}	
	
CMceComSession* MceMediaManagerTestHelper::CreateVideoOutSessionFileSourceL( CMceComSession* aSession, TBool aUseAvc )
	{
	
	TMceComFactory factory;

	CMceComSession* session = NULL;
	CMceComSession* outSession = aSession;
	if ( !outSession )
	    {
    	outSession = CMceComSession::NewL( CMceComSession::EOutSession );
	    CleanupStack::PushL( outSession );
	    }
//downlink

	CMceComMediaStream* downlinkStream = CreateVideoDownLinkL( KMceMMTestVideoPayloadType, ETrue );
	CleanupStack::PushL( downlinkStream );

//uplink
	CMceComVideoStream* uplinkStream = 
	    static_cast<CMceComVideoStream*>( factory.MediaStreamFactory().CreateLC( KMceVideo ) );
	uplinkStream->iLocalMediaPort = KMceMMTestVideoLocalPort;

	
	CMceComFileSource* fileSource = 
	    static_cast<CMceComFileSource*>( factory.SourceFactory().CreateLC( KMceFileSource ) );
	
	CMceComRtpSink* rtpSink = 
	    static_cast<CMceComRtpSink*>( factory.SinkFactory().CreateLC( KMceRTPSink ) );
	uplinkStream->AddSinkL( rtpSink );
	CleanupStack::Pop( rtpSink );

	uplinkStream->SetSourceL( fileSource );
	CleanupStack::Pop( fileSource );

    if ( aUseAvc )
        {
        CMceComVideoCodec* avc = 
            static_cast<CMceComVideoCodec*>( factory.VideoCodecFactory().CreateLC( KMceSDPNameH264() ) );
    	avc->iClockRate = KMceMMTestVideoClockrate;
    	avc->iPayloadType = KMceMMTestVideoPayloadType;
    	avc->iCodecMode = 0;
    	avc->iBitrate = KMceMMTestVideoBitrate;
    	uplinkStream->AddCodecL( avc );
    	CleanupStack::Pop( avc );
        }
    else
        {
        CMceComVideoCodec* h263 = 
            static_cast<CMceComVideoCodec*>( factory.VideoCodecFactory().CreateLC( KMceSDPNameH263() ) );
    	h263->iClockRate = KMceMMTestVideoClockrate;
    	h263->iPayloadType = KMceMMTestVideoPayloadType;
    	h263->iCodecMode = KMceMMTestVideoCodecMode;
    	h263->iAllowedBitrates = KMceMMTestVideoAllowedBitrates;
    	h263->iBitrate = KMceMMTestVideoBitrate;
    	uplinkStream->AddCodecL( h263 );
    	CleanupStack::Pop( h263 );
        }

    downlinkStream->BindL( uplinkStream );
    CleanupStack::Pop( uplinkStream );
	
	outSession->AddStreamL( downlinkStream );   
	CleanupStack::Pop( downlinkStream );


//display
	CMceComMediaStream* localStream = CreateLocalVideoStreamL( fileSource );
	CleanupStack::PushL( localStream );
	outSession->AddStreamL( localStream );   
	CleanupStack::Pop( localStream );
	
	outSession->InitializeL();
	
    if ( !aSession )
        {
        CleanupStack::Pop( outSession );
        session = EncodeDecodeSessionL( outSession );
        }
    else
        {
        session = outSession;
        }
    
    return session;
	
	}	
	
	

CMceComSession* MceMediaManagerTestHelper::CreateMultimediaOutSessionL( TBool aUseAvc )
    {
	CMceComSession* session = NULL;
	CMceComSession* outSession = NULL;
	
	outSession = CMceComSession::NewL( CMceComSession::EOutSession );
    CleanupStack::PushL( outSession );

    CreateAudioOutSessionL( outSession );
    CreateVideoOutSessionL( outSession, aUseAvc );
    
    CleanupStack::Pop( outSession );
    session = EncodeDecodeSessionL( outSession );
    return session;
    
    }

CMceComSession* MceMediaManagerTestHelper::CreateMultimediaBundledOutSessionL()
    {
	TMceComFactory factory;
    
	CMceComSession* session = NULL;
	CMceComSession* outSession = NULL;
	
	outSession = CMceComSession::NewL( CMceComSession::EOutSession );
    CleanupStack::PushL( outSession );

    CreateAudioOutSessionL( outSession );
    CreateVideoOutSessionL( outSession );

	CMceComVideoStream* localVideoStream = 
	    static_cast<CMceComVideoStream*>( factory.MediaStreamFactory().CreateLC( KMceVideo ) );

	CMceComAudioStream* localAudioStream = 
	    static_cast<CMceComAudioStream*>( factory.MediaStreamFactory().CreateLC( KMceAudio ) );


	CMceComSpeakerSink* speakerSink = 
	    static_cast<CMceComSpeakerSink*>( factory.SinkFactory().CreateLC( KMceSpeakerSink ) );

	CMceComDisplaySink* diplaySink = 
	    static_cast<CMceComDisplaySink*>( factory.SinkFactory().CreateLC( KMceDisplaySink ) );
	    
	CMceComFileSource* fileSource = 
	    static_cast<CMceComFileSource*>( factory.SourceFactory().CreateLC( KMceFileSource ) );

    fileSource->iFileInfo.iFileName.Copy( KTestFile );
    fileSource->iFileInfo.iDirection = TMceFileInfo::EReadWrite;
    fileSource->iFileInfo.iAccessRights = TMceFileInfo::EReadWrite;

    TMceFileInfo iFileInfo;
 
    localVideoStream->SetSourceL( fileSource );
    CleanupStack::Pop( fileSource );

    localVideoStream->AddSinkL( diplaySink );
    CleanupStack::Pop( diplaySink );

    localAudioStream->SetSourceL( fileSource );

    localAudioStream->AddSinkL( speakerSink );
    CleanupStack::Pop( speakerSink );

    CMceComVideoCodec* h263 = 
        static_cast<CMceComVideoCodec*>( factory.VideoCodecFactory().CreateLC( KMceSDPNameH263() ) );
	h263->iClockRate = KMceMMTestVideoClockrate;
	h263->iPayloadType = KMceMMTestVideoPayloadType;
	h263->iCodecMode = KMceMMTestVideoCodecMode;
	h263->iAllowedBitrates = KMceMMTestVideoAllowedBitrates;
	h263->iBitrate = KMceMMTestVideoBitrate;
	localVideoStream->AddCodecL( h263 );
	CleanupStack::Pop( h263 );

    CMceComAudioCodec* amr = 
        static_cast<CMceComAudioCodec*>( factory.AudioCodecFactory().CreateLC( KMceSDPNameAMR() ) );
	amr->iSamplingFreq = KMceMMTestAudioSamplingFreq;
	amr->iPayloadType = KMceMMTestAudioPayloadType;
	amr->iCodecMode = KMceMMTestAudioCodecMode;
	amr->iAllowedBitrates = KMceMMTestAudioAllowedBitrates;
	amr->iPTime = KMceMMTestAudioPTime;
	amr->iMaxPTime = KMceMMTestAudioMaxPTime;
	localAudioStream->AddCodecL( amr );
	CleanupStack::Pop( amr );
    
    outSession->AddStreamL( localAudioStream );
    CleanupStack::Pop( localAudioStream );

    outSession->AddStreamL( localVideoStream );
    CleanupStack::Pop( localVideoStream );
    
    CMceComStreamBundle* bundle = CMceComStreamBundle::NewLC( CMceStreamBundle::ELS );
    bundle->AddStreamL( *localAudioStream );
    bundle->AddStreamL( *localVideoStream );
    
    outSession->AddBundleL( bundle );
    CleanupStack::Pop( bundle );
    
    CleanupStack::Pop( outSession );
    session = EncodeDecodeSessionL( outSession );
	
    return session;
    
    }


CMceComMediaStream* MceMediaManagerTestHelper::CreateVideoDownLinkL( TInt aPayload, TBool aUseAvc )
    {

	TMceComFactory factory;

	CMceComVideoStream* downlinkStream = 
	    static_cast<CMceComVideoStream*>( factory.MediaStreamFactory().CreateLC( KMceVideo ) );
	downlinkStream->iLocalMediaPort = KMceMMTestVideoLocalPort;
	 
	CMceComRtpSource* rtpSource = 
	    static_cast<CMceComRtpSource*>( factory.SourceFactory().CreateLC( KMceRTPSource ) );

	rtpSource->iBufferLength = KMceMMTestDefaultBufferLength;
	rtpSource->iBufferTreshold = KMceMMTestDefaultBufferThreshold;
	
	CMceComDisplaySink* diplaySink = 
	    static_cast<CMceComDisplaySink*>( factory.SinkFactory().CreateLC( KMceDisplaySink ) );
	downlinkStream->AddSinkL( diplaySink );
	CleanupStack::Pop( diplaySink );

	downlinkStream->SetSourceL( rtpSource );
	CleanupStack::Pop( rtpSource );

    if ( aUseAvc )
        {
        CMceComVideoCodec* avc = 
            static_cast<CMceComVideoCodec*>( factory.VideoCodecFactory().CreateLC( KMceSDPNameH264() ) );
    	avc->iClockRate = KMceMMTestVideoClockrate;
    	avc->iPayloadType = KMceMMTestVideoPayloadType;
    	avc->iCodecMode = 0;
    	avc->iBitrate = KMceMMTestVideoBitrate;
    	downlinkStream->AddCodecL( avc );
    	CleanupStack::Pop( avc );
        }
    else
        {
        CMceComVideoCodec* h263 = 
            static_cast<CMceComVideoCodec*>( factory.VideoCodecFactory().CreateLC( KMceSDPNameH263() ) );
    	h263->iClockRate = KMceMMTestVideoClockrate;
    	h263->iPayloadType = aPayload;
    	h263->iCodecMode = KMceMMTestVideoCodecMode;
    	h263->iAllowedBitrates = KMceMMTestVideoAllowedBitrates;
    	h263->iBitrate = KMceMMTestVideoBitrate;
    	
    	downlinkStream->AddCodecL( h263 );
    	CleanupStack::Pop( h263 );
        }
        
    CleanupStack::Pop( downlinkStream );
    return downlinkStream;
    
    }
    
CMceComMediaStream* MceMediaManagerTestHelper::CreateLocalVideoStreamL( CMceComMediaSource* aCameraSource )
    {
    CleanupStack::PushL( aCameraSource );
	TMceComFactory factory;
    
	CMceComVideoStream* localStream = 
	    static_cast<CMceComVideoStream*>( factory.MediaStreamFactory().CreateLC( KMceVideo ) );
	
    
    localStream->SetSourceL( aCameraSource );
	
	CleanupStack::Pop( localStream );
    CleanupStack::Pop( aCameraSource );
	CleanupStack::PushL( localStream );

	CMceComDisplaySink* diplaySink = 
	    static_cast<CMceComDisplaySink*>( factory.SinkFactory().CreateLC( KMceDisplaySink ) );
	localStream->AddSinkL( diplaySink );
	CleanupStack::Pop( diplaySink );

    CMceComVideoCodec* h263 = 
        static_cast<CMceComVideoCodec*>( factory.VideoCodecFactory().CreateLC( KMceSDPNameH263() ) );
	localStream->AddCodecL( h263 );
	CleanupStack::Pop( h263 );
	
	CleanupStack::Pop( localStream );
	return localStream;
	
    
    }
    

CMceComSession* MceMediaManagerTestHelper::EncodeDecodeSessionL( CMceComSession* aSession )
	{
	CleanupStack::PushL( aSession );
	
    CMceComSession* session = NULL;
	const TUint32 KInetAddr = INET_ADDR(12,12,12,12);
    
    CMceMsgObject<CMceComSession>* msg = 
        new (ELeave) CMceMsgObject<CMceComSession>( *aSession, EMceItcMsgTypeSession );
    CleanupStack::PushL( msg );
    
    msg->EncodeL();
    CleanupStack::Pop( msg );
    CleanupStack::Pop( aSession ); // msg will take ownership temporarily at decoding phase
    CleanupStack::PushL( msg );
    msg->DecodeL( msg->EncodeBuffer().Ptr( 0 ) );

    session = msg->Object();
    
    CleanupStack::PopAndDestroy( msg );
    delete aSession;
    
    session->iLocalIpAddress.SetAddress( KInetAddr );
    return session;
    
	}
	

void MceMediaManagerTestHelper::LinkCreated( CMceMediaManager& aManager,
                                              TUint32 aLinkId, 
                                              TUint32 aSessionId, 
                                              TUint aPort, 
                                              TUint32 aInetAddr )
    {

    if ( !aInetAddr )
        {
    	aInetAddr = INET_ADDR(12,12,12,12);
        }
    if ( aPort == KMaxTUint32 )
        {
        aPort = KMceMMTestAudioLocalPort;
        }

    TMccEvent linkCreated;
    linkCreated.iSessionId = aSessionId;
    linkCreated.iEventCategory = KMccEventCategoryStream;
    linkCreated.iEventType = KMccLinkCreated;
    linkCreated.iLinkId = aLinkId;
    
    TMccNetSettings netSettings;
    netSettings.iLocalAddress.SetAddress( aInetAddr );
    netSettings.iLocalAddress.SetPort( aPort );
    TPckgBuf<TMccNetSettings> netSettingsBuf( netSettings );
    linkCreated.iEventData.Copy( netSettingsBuf );
	
    aManager.MccEventReceived( linkCreated );
    }


CSdpDocument* MceMediaManagerTestHelper::EstablishOutSessionL( 
                                                CMceComSession& aSession,
                                                CMceMediaManager& aManager,
                                                const TDesC8& aAnswer,
                                                CMceServerStub* aServer,
                                                CSdpDocument* aUpdatedSdp )
    {
    
    CMceServerStub* iServer = aServer; // For MCE_MCC_EVENT_POSTCONDITION_L macro
    
    if ( aUpdatedSdp )
        {
        CleanupStack::PushL( aUpdatedSdp );
        }
    
//update
    TMceReturnStatus status;
    MCE_LEAVE_HANDLING( status = aManager.UpdateL( aSession ), KErrArgument );

//1. link created
    LinkCreated( aManager, SRVSTREAM(0)->LinkId(), aSession.iMccID );
                                 
//1. prepared
    aManager.MccMediaPrepared( aSession.iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L
    aManager.MccMediaPrepared( aSession.iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L
                                
//2. prepared
    aManager.MccMediaPrepared( aSession.iMccID,
                                SRVSTREAM(1)->LinkId(),
                                SRVSTREAM(1)->Id(),
                                SRVSTREAM(1)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L
    aManager.MccMediaPrepared( aSession.iMccID,
                                SRVSTREAM(1)->LinkId(),
                                SRVSTREAM(1)->Id(),
                                SRVSTREAM(1)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L
//1. started
    aManager.MccMediaStarted( aSession.iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L
    aManager.MccMediaStarted( aSession.iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L
//encode 
    CSdpDocument* sdpDocument = aManager.EncodeL( aSession );
    delete sdpDocument; 
    if ( aUpdatedSdp )
        {
        if ( aUpdatedSdp == sdpDocument )
            {
            // Was deleted above
            CleanupStack::Pop( aUpdatedSdp );
            }
        else
            {
            CleanupStack::PopAndDestroy( aUpdatedSdp );
            }
        } 
    sdpDocument = NULL;
    
    sdpDocument = CSdpDocument::DecodeL( aAnswer );

    CleanupStack::PushL( sdpDocument );
    
//decode answer    
    aSession.AnswerType() = KMceNegotiationAnswerTypeFinal;
    TMceSipWarningCode decodeStatus = aManager.DecodeL( aSession, *sdpDocument );
//update
    status = aManager.UpdateL( aSession );
    
//2. started
    aManager.MccMediaStarted( aSession.iMccID,
                               SRVSTREAM(1)->LinkId(),
                               SRVSTREAM(1)->Id(),
                               KNoEndpointId );
    MCE_MCC_EVENT_POSTCONDITION_L

    CleanupStack::Pop( sdpDocument );
    return sdpDocument;
    
    
    }


CSdpDocument* MceMediaManagerTestHelper::EstablishOutMultimediaSessionL( 
                                                CMceComSession& aSession,
                                                CMceMediaManager& aManager,
                                                const TDesC8& aAnswer,
                                                CMceServerStub* aServer )
    {
    CMceServerStub* iServer = aServer; // For MCE_MCC_EVENT_POSTCONDITION_L macro

//update
    TMceReturnStatus status = aManager.UpdateL( aSession );
    EUNIT_ASSERT( status == KMceAsync );
    User::LeaveIfError( aSession.MccStreams().Count() == 5 ? KErrNone : KErrArgument );
    
//1. link created
    LinkCreated( aManager, SRVSTREAM(0)->LinkId(), aSession.iMccID );

//2. link created
    LinkCreated( aManager, SRVSTREAM(2)->LinkId(), aSession.iMccID );
    
//1. prepared
    aManager.MccMediaPrepared( aSession.iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L
    aManager.MccMediaPrepared( aSession.iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L
//2. prepared
    aManager.MccMediaPrepared( aSession.iMccID,
                                SRVSTREAM(1)->LinkId(),
                                SRVSTREAM(1)->Id(),
                                SRVSTREAM(1)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L
    aManager.MccMediaPrepared( aSession.iMccID,
                                SRVSTREAM(1)->LinkId(),
                                SRVSTREAM(1)->Id(),
                                SRVSTREAM(1)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L

//3. prepared
    aManager.MccMediaPrepared( aSession.iMccID,
                                SRVSTREAM(2)->LinkId(),
                                SRVSTREAM(2)->Id(),
                                SRVSTREAM(2)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L
    aManager.MccMediaPrepared( aSession.iMccID,
                                SRVSTREAM(2)->LinkId(),
                                SRVSTREAM(2)->Id(),
                                SRVSTREAM(2)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L
//4. prepared
    aManager.MccMediaPrepared( aSession.iMccID,
                                SRVSTREAM(3)->LinkId(),
                                SRVSTREAM(3)->Id(),
                                SRVSTREAM(3)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L
    aManager.MccMediaPrepared( aSession.iMccID,
                                SRVSTREAM(3)->LinkId(),
                                SRVSTREAM(3)->Id(),
                                SRVSTREAM(3)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L

//1. started
    aManager.MccMediaStarted( aSession.iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L
    aManager.MccMediaStarted( aSession.iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L
                                
//3. started
    aManager.MccMediaStarted( aSession.iMccID,
                                SRVSTREAM(2)->LinkId(),
                                SRVSTREAM(2)->Id(),
                                SRVSTREAM(2)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L
    aManager.MccMediaStarted( aSession.iMccID,
                                SRVSTREAM(2)->LinkId(),
                                SRVSTREAM(2)->Id(),
                                SRVSTREAM(2)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L

    //srvstream asserts
    User::LeaveIfError( SRVSTREAM(0)->State() == CMceSrvStream::EStarted ? KErrNone : KErrArgument );
    User::LeaveIfError( SRVSTREAM(1)->State() == CMceSrvStream::EPrepared ? KErrNone : KErrArgument );
    User::LeaveIfError( SRVSTREAM(2)->State() == CMceSrvStream::EStarted ? KErrNone : KErrArgument );
    User::LeaveIfError( SRVSTREAM(3)->State() == CMceSrvStream::EPrepared ? KErrNone : KErrArgument );
    User::LeaveIfError( SRVSTREAM(4)->State() == CMceSrvStream::EStarted ? KErrNone : KErrArgument );
    
//encode 
    CSdpDocument* sdpDocument = aManager.EncodeL( aSession );
    delete sdpDocument;
    sdpDocument = NULL;
    
    sdpDocument = CSdpDocument::DecodeL( aAnswer );
    CleanupStack::PushL( sdpDocument );

//decode answer    
    aSession.AnswerType() = KMceNegotiationAnswerTypeFinal;
    TMceSipWarningCode decodeStatus = aManager.DecodeL( aSession, *sdpDocument );
    User::LeaveIfError( decodeStatus == KErrNone ? KErrNone : KErrArgument );
    
//update
    status = aManager.UpdateL( aSession );
    User::LeaveIfError( status == KMceAsync ? KErrNone : KErrArgument );
    
//2. started
    aManager.MccMediaStarted( aSession.iMccID,
                                SRVSTREAM(1)->LinkId(),
                                SRVSTREAM(1)->Id(),
                                SRVSTREAM(1)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L
    aManager.MccMediaStarted( aSession.iMccID,
                                SRVSTREAM(1)->LinkId(),
                                SRVSTREAM(1)->Id(),
                                SRVSTREAM(1)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L

//4. started
    aManager.MccMediaStarted( aSession.iMccID,
                                SRVSTREAM(3)->LinkId(),
                                SRVSTREAM(3)->Id(),
                                SRVSTREAM(3)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L
    aManager.MccMediaStarted( aSession.iMccID,
                                SRVSTREAM(3)->LinkId(),
                                SRVSTREAM(3)->Id(),
                                SRVSTREAM(3)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L
    
    //srvstream asserts
    User::LeaveIfError( SRVSTREAM(0)->State() == CMceSrvStream::EStarted ? KErrNone : KErrArgument );
    User::LeaveIfError( SRVSTREAM(1)->State() == CMceSrvStream::EStarted ? KErrNone : KErrArgument );
    User::LeaveIfError( SRVSTREAM(2)->State() == CMceSrvStream::EStarted ? KErrNone : KErrArgument );
    User::LeaveIfError( SRVSTREAM(3)->State() == CMceSrvStream::EStarted ? KErrNone : KErrArgument );
    User::LeaveIfError( SRVSTREAM(4)->State() == CMceSrvStream::EStarted ? KErrNone : KErrArgument );

    CleanupStack::Pop( sdpDocument );
    return sdpDocument;
   
    }

HBufC8* MceMediaManagerTestHelper::ToTextLC( CSdpDocument& aDocument )
    {
    
    CBufFlat* encBuf = CBufFlat::NewL( 1000 );
    CleanupStack::PushL( encBuf );
    RBufWriteStream writeStream;
    CleanupClosePushL( writeStream );
    writeStream.Open( *encBuf ); 
    aDocument.EncodeL( writeStream );
    CleanupStack::PopAndDestroy( &writeStream );
    TPtr8 encBufPtr = encBuf->Ptr(0);
    HBufC8* sdpBuf = sdpBuf = encBufPtr.AllocL();
    CleanupStack::PopAndDestroy( encBuf );
    CleanupStack::PushL( sdpBuf );
    
    return sdpBuf;
    }

	
// End of file
