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
#include "UT_cmceaudiosdpcodec.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <sdpdocument.h>
#include <sdpcodecstringpool.h>
#include <sdpcodecstringconstants.h>
#include <sdpmediafield.h>
#include <sdpfmtattributefield.h>
#include <sdprtpmapvalue.h>
#include <sdpattributefield.h>

#include "mcemediamanager.h"
#define RESET()\
    iServer->Reset()
    
#include "mceaudiosdpcodec.h"
#include "mcemediaidle.h"
#include "mcecomamrcodec.h"
#include "mmcccodecinformation.h"

_LIT8( KMceTestSdpAMR, "v=0\r\n\
o=userb 1234567890 1234567890 IN IP4 10.20.180.72\r\n\
s=-\r\n\
i=-\r\n\
c=IN IP4 10.20.180.72\r\n\
t=0 0\r\n\
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

_LIT8( KMceTestSdpAMRSessionLevel, "v=0\r\n\
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
a=fmtp:96 octet-align=0; mode-set=0,1,2,3,4,5,6,7\r\n");




//  INTERNAL INCLUDES

// CONSTRUCTION
UT_CMceAudioSdpCodec* UT_CMceAudioSdpCodec::NewL()
    {
    UT_CMceAudioSdpCodec* self = UT_CMceAudioSdpCodec::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceAudioSdpCodec* UT_CMceAudioSdpCodec::NewLC()
    {
    UT_CMceAudioSdpCodec* self = new( ELeave ) UT_CMceAudioSdpCodec();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceAudioSdpCodec::~UT_CMceAudioSdpCodec()
    {
    }

// Default constructor
UT_CMceAudioSdpCodec::UT_CMceAudioSdpCodec()
    {
    }

// Second phase construct
void UT_CMceAudioSdpCodec::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_CMceAudioSdpCodec::SetupL()
    {
    iServer = new (ELeave) CMceServerStub();
    iManager = CMceMediaManager::NewL( *iServer );
    iSession = MceMediaManagerTestHelper::CreateMultimediaOutSessionL();
    iSdpCodec = CMceAudioSdpCodec::NewL();
    TMceMediaIdle::SetAsCurrentStateL( *iSession );
    } 

void UT_CMceAudioSdpCodec::Teardown()
    {
	CMceComSession::Delete( iSession, *iManager );
	delete iSdpCodec;
	iSdpCodec = NULL;
	delete iManager;
	iManager = NULL;
	delete iServer;
	iServer = NULL;
    }


void UT_CMceAudioSdpCodec::UT_CMceAudioSdpCodec_EncodesL()
    {
    EUNIT_ASSERT( !iSdpCodec->Encodes( *iSession->Streams()[2] ) );
    EUNIT_ASSERT( !iSdpCodec->Encodes( *iSession->Streams()[1] ) );
    EUNIT_ASSERT( iSdpCodec->Encodes( *iSession->Streams()[0] ) );
    }

void UT_CMceAudioSdpCodec::UT_CMceAudioSdpCodec_CodecsLL()
    {
    
    EUNIT_ASSERT( iSdpCodec->CodecsL( *iSession->Streams()[0] ).Count() > 0 );
    EUNIT_ASSERT_LEAVE( iSdpCodec->CodecsL( *iSession->Streams()[1] ) );
    
    }

void UT_CMceAudioSdpCodec::UT_CMceAudioSdpCodec_EncodeRtpmapAttributeLCL()
    {
    CSdpFmtAttributeField* field = NULL;
    CMceComMediaStream* mediaStream = iSession->Streams()[0];
    const RPointerArray<CMceComCodec>& codecs = iSdpCodec->CodecsL( *mediaStream );
    
    field = iSdpCodec->EncodeRtpmapAttributeLC( *codecs[0] );
    
    EUNIT_ASSERT( field != NULL );
    
    CleanupStack::PopAndDestroy( field );
    }

void UT_CMceAudioSdpCodec::UT_CMceAudioSdpCodec_EncodeMediaAttributesLL()
    {
    CMceComMediaStream* mediaStream = iSession->Streams()[0];
    CMceComMediaStream* mediaBoundStream = &mediaStream->BoundStreamL();
    
    const RPointerArray<CMceComCodec>& codecs = iSdpCodec->CodecsL( *mediaStream );
    const RPointerArray<CMceComCodec>& boundCodecs = iSdpCodec->CodecsL( *mediaBoundStream );
    
    RStringF rtpmapStr = SDP_STRINGL( SdpCodecStringConstants::EAttributeRtpmap );
    MCE_DEFINE_DECSTR( payload, 96 );
    
    TSdpRtpmapValue rtpmapValue = TSdpRtpmapValue::DecodeL( _L8("AMR/8000") );
    HBufC8* rtpmapBuf = rtpmapValue.EncodeL();
    CleanupStack::PushL( rtpmapBuf );
    CSdpFmtAttributeField* rtpmapAttribute = 
        CSdpFmtAttributeField::NewL( rtpmapStr, payload, *rtpmapBuf );
    CleanupStack::PopAndDestroy( rtpmapBuf ); 
    CleanupStack::PushL( rtpmapAttribute ); 

    RStringF audio = MCE_SDP_STRING_AUDIOL();
    RStringF protocol = SDP_STRINGL( SdpCodecStringConstants::EProtocolRtpAvp );
    
    CSdpMediaField* audioLine = 
        CSdpMediaField::NewLC( audio, 5005, protocol, _L8("0") );
    
    EUNIT_ASSERT( audioLine->AttributeFields().Count() == 0 );
    
    iSdpCodec->EncodeMediaAttributesL( *codecs[0], *audioLine, *rtpmapAttribute );
    
    EUNIT_ASSERT( audioLine->AttributeFields().Count() > 0 );
    audioLine->AttributeFields().ResetAndDestroy();
    
    iSdpCodec->EncodeMediaAttributesL( *boundCodecs[0], *audioLine, *rtpmapAttribute );
    
    EUNIT_ASSERT( audioLine->AttributeFields().Count() > 0 );

    audioLine->AttributeFields().ResetAndDestroy();
    mediaStream->iStreamType = CMceComMediaStream::ESendOnlyStream;
    mediaBoundStream->iStreamType = CMceComMediaStream::ESendOnlyStream;
    iSdpCodec->EncodeMediaAttributesL( *boundCodecs[0], *audioLine, *rtpmapAttribute );

    EUNIT_ASSERT( audioLine->AttributeFields().Count() == 0 );

    
    mediaStream->iStreamType = CMceComMediaStream::EReceiveStream;
    mediaBoundStream->iStreamType = CMceComMediaStream::ESendStream;
    audioLine->AttributeFields().ResetAndDestroy();
    static_cast<CMceComAudioCodec*>( codecs[0] )->iPTime = 120;
    static_cast<CMceComAudioCodec*>( codecs[0] )->iMaxPTime = 0;

    iSdpCodec->EncodeMediaAttributesL( *codecs[0], *audioLine, *rtpmapAttribute );
    
    EUNIT_ASSERT( audioLine->AttributeFields().Count() == 0 );

    audioLine->AttributeFields().ResetAndDestroy();
    static_cast<CMceComAudioCodec*>( codecs[0] )->iPTime = 0;
    static_cast<CMceComAudioCodec*>( codecs[0] )->iMaxPTime = 0;

    iSdpCodec->EncodeMediaAttributesL( *codecs[0], *audioLine, *rtpmapAttribute );
    
    EUNIT_ASSERT( audioLine->AttributeFields().Count() == 0 );
    
    CleanupStack::PopAndDestroy( audioLine );
    CleanupStack::PopAndDestroy( rtpmapAttribute );
    
    }
    

void UT_CMceAudioSdpCodec::UT_CMceAudioSdpCodec_DecodeMediaAttributesLL()
    {
    
    CSdpDocument* sdp = CSdpDocument::DecodeL( KMceTestSdpAMR );
    CleanupStack::PushL( sdp );
    
    CSdpMediaField* audioLine = sdp->MediaFields()[ 0 ];
    CMceComMediaStream* mediaStream = iSession->Streams()[ 0 ];
    CMceComMediaStream* mediaBoundStream = &mediaStream->BoundStreamL();
     
    const RPointerArray<CMceComCodec>& codecs = iSdpCodec->CodecsL( *mediaStream );
    
    CSdpFmtAttributeField* rtpmap = NULL;
    MCE_MMTEST_RTP_MAP_L( rtpmap, audioLine );
    iSdpCodec->DecodeMediaAttributesL( *audioLine, *codecs[0], *rtpmap );

    EUNIT_ASSERT( static_cast<CMceComAudioCodec*>(codecs[0])->iPTime == 20 );
    EUNIT_ASSERT( static_cast<CMceComAudioCodec*>(codecs[0])->iMaxPTime == 400 );

    static_cast<CMceComAudioCodec*>( codecs[0] )->iPTime = 0;
    static_cast<CMceComAudioCodec*>( codecs[0] )->iMaxPTime = 0;
    mediaStream->iStreamType = CMceComMediaStream::EReceiveStream;
    mediaBoundStream->iStreamType = CMceComMediaStream::EReceiveStream;
    
    iSdpCodec->DecodeMediaAttributesL( *audioLine, *codecs[0], *rtpmap );
    
    EUNIT_ASSERT( static_cast<CMceComAudioCodec*>(codecs[0])->iPTime == 0 );
    EUNIT_ASSERT( static_cast<CMceComAudioCodec*>(codecs[0])->iMaxPTime == 0 );


    static_cast<CMceComAudioCodec*>( codecs[0] )->iPTime = 0;
    static_cast<CMceComAudioCodec*>( codecs[0] )->iMaxPTime = 0;
    mediaStream->iStreamType = CMceComMediaStream::EReceiveStream;
    mediaBoundStream->iStreamType = CMceComMediaStream::ESendStream;
    audioLine->AttributeFields().ResetAndDestroy();
    
    iSdpCodec->DecodeMediaAttributesL( *audioLine, *codecs[0], *rtpmap );
    
    EUNIT_ASSERT( static_cast<CMceComAudioCodec*>(codecs[0])->iPTime == 0 );
    EUNIT_ASSERT( static_cast<CMceComAudioCodec*>(codecs[0])->iMaxPTime == 0 );
    
    
    CleanupStack::PopAndDestroy( sdp );
    
    }

void UT_CMceAudioSdpCodec::UT_CMceAudioSdpCodec_CreateStreamLCL()
    {
    
    CMceComMediaStream* mediaStream = NULL;
    
    mediaStream = iSdpCodec->CreateStreamLC( KErrNotFound );
    mediaStream->InitializeL( *iSession );
    EUNIT_ASSERT( mediaStream->iType == KMceAudio );
    EUNIT_ASSERT( mediaStream->BoundStream() );
    EUNIT_ASSERT( mediaStream->iStreamType == CMceComMediaStream::EReceiveStream );
    
    CleanupStack::PopAndDestroy( mediaStream );

    mediaStream = iSdpCodec->CreateStreamLC( SdpCodecStringConstants::EAttributeSendrecv );
    mediaStream->InitializeL( *iSession );
    EUNIT_ASSERT( mediaStream->iType == KMceAudio );
    EUNIT_ASSERT( mediaStream->BoundStream() );
    EUNIT_ASSERT( mediaStream->iStreamType == CMceComMediaStream::EReceiveStream );
    CleanupStack::PopAndDestroy( mediaStream );

    mediaStream = iSdpCodec->CreateStreamLC( SdpCodecStringConstants::EAttributeInactive );
    mediaStream->InitializeL( *iSession );
    EUNIT_ASSERT( mediaStream->iType == KMceAudio );
    EUNIT_ASSERT( mediaStream->BoundStream() );
    EUNIT_ASSERT( mediaStream->iStreamType == CMceComMediaStream::EReceiveStream );
    CleanupStack::PopAndDestroy( mediaStream );

    mediaStream = iSdpCodec->CreateStreamLC( SdpCodecStringConstants::EAttributeSendonly );
    mediaStream->InitializeL( *iSession );
    EUNIT_ASSERT( mediaStream->iType == KMceAudio );
    EUNIT_ASSERT( !mediaStream->BoundStream() );
    EUNIT_ASSERT( mediaStream->iStreamType == CMceComMediaStream::EReceiveOnlyStream );
    CleanupStack::PopAndDestroy( mediaStream );

    mediaStream = iSdpCodec->CreateStreamLC( SdpCodecStringConstants::EAttributeRecvonly );
    mediaStream->InitializeL( *iSession );
    EUNIT_ASSERT( mediaStream->iType == KMceAudio );
    EUNIT_ASSERT( !mediaStream->BoundStream() );
    EUNIT_ASSERT( mediaStream->iStreamType == CMceComMediaStream::ESendOnlyStream );
    CleanupStack::PopAndDestroy( mediaStream );
    
    
    }

void UT_CMceAudioSdpCodec::UT_CMceAudioSdpCodec_UpdateStreamLL()
    {
    CMceComMediaStream* mediaStream = NULL;
    mediaStream = iSdpCodec->CreateStreamLC( SdpCodecStringConstants::EAttributeSendrecv );
    mediaStream->InitializeL( *iSession );
    
    EUNIT_ASSERT( mediaStream->Direction() == SdpCodecStringConstants::EAttributeSendrecv );

    iSdpCodec->UpdateStreamL( *mediaStream, SdpCodecStringConstants::EAttributeRecvonly );
    EUNIT_ASSERT( mediaStream->Direction() == SdpCodecStringConstants::EAttributeSendrecv );
  
    mediaStream->iStreamType = CMceComMediaStream::EReceiveOnlyStream;
    iSdpCodec->UpdateStreamL( *mediaStream, SdpCodecStringConstants::EAttributeRecvonly );
    iSdpCodec->UpdateStreamL( *mediaStream, SdpCodecStringConstants::EAttributeSendonly );

    const RPointerArray<CMceComCodec>& codecs = iSdpCodec->CodecsL( *mediaStream );
    CMceComAMRCodec* codec = CMceComAMRCodec::NewLC( KMceSDPNameAMR() );
    mediaStream->AddCodecL( codec );
    CleanupStack::Pop( codec );

    mediaStream->iStreamType = CMceComMediaStream::ESendOnlyStream;
    iSdpCodec->UpdateStreamL( *mediaStream, SdpCodecStringConstants::EAttributeRecvonly );
    iSdpCodec->UpdateStreamL( *mediaStream, SdpCodecStringConstants::EAttributeSendonly );

    mediaStream->iStreamType = CMceComMediaStream::EReceiveStream;
    iSdpCodec->UpdateStreamL( *mediaStream, SdpCodecStringConstants::EAttributeSendrecv );
    iSdpCodec->UpdateStreamL( *mediaStream, SdpCodecStringConstants::EAttributeSendonly );
    
    CleanupStack::PopAndDestroy( mediaStream );
    
    }

void UT_CMceAudioSdpCodec::UT_CMceAudioSdpCodec_CreateCodecLCL()
    {
    iSdpCodec->AddCapabilityL( KMceSDPNameAMR );
    iSdpCodec->AddCapabilityPayloadTypeL(KDefaultAmrNbPT);
    
    RStringF rtpmapStr = SDP_STRINGL( SdpCodecStringConstants::EAttributeRtpmap );

    MCE_DEFINE_DECSTR( payload, 96 );
    TSdpRtpmapValue rtpmapValue = TSdpRtpmapValue::DecodeL( _L8("AMR/8000") );
    HBufC8* rtpmapBuf = rtpmapValue.EncodeL();
    CleanupStack::PushL( rtpmapBuf );
    CSdpFmtAttributeField* rtpmapAttribute = 
        CSdpFmtAttributeField::NewL( rtpmapStr, payload, *rtpmapBuf );
    CleanupStack::PopAndDestroy( rtpmapBuf ); 
    CleanupStack::PushL( rtpmapAttribute ); 
    
    CMceComCodec* codec = NULL;
    codec = iSdpCodec->CreateCodecLC( *rtpmapAttribute );
    
    EUNIT_ASSERT( codec != NULL );
    EUNIT_ASSERT( codec->iPayloadType == 96 );
    
    CleanupStack::PopAndDestroy( codec ); 
    CleanupStack::PopAndDestroy( rtpmapAttribute ); 
    

    MCE_DEFINE_DECSTR( illegalPayload, KMceMaxPTValue + 1 );
    TSdpRtpmapValue rtpmapValue1 = TSdpRtpmapValue::DecodeL( _L8("AMR/8000") );
    rtpmapBuf = rtpmapValue1.EncodeL();
    CleanupStack::PushL( rtpmapBuf );
    rtpmapAttribute = 
        CSdpFmtAttributeField::NewL( rtpmapStr, illegalPayload, *rtpmapBuf );
    CleanupStack::PopAndDestroy( rtpmapBuf ); 
    CleanupStack::PushL( rtpmapAttribute ); 
    
    EUNIT_ASSERT_LEAVE( iSdpCodec->CreateCodecLC( *rtpmapAttribute ) );

    CleanupStack::PopAndDestroy( rtpmapAttribute ); 

    TSdpRtpmapValue rtpmapValue2 = TSdpRtpmapValue::DecodeL( _L8("FOOBAR/8000") );
    rtpmapBuf = rtpmapValue2.EncodeL();
    CleanupStack::PushL( rtpmapBuf );
    rtpmapAttribute = 
        CSdpFmtAttributeField::NewL( rtpmapStr, payload, *rtpmapBuf );
    CleanupStack::PopAndDestroy( rtpmapBuf ); 
    CleanupStack::PushL( rtpmapAttribute ); 

    codec = iSdpCodec->CreateCodecLC( *rtpmapAttribute );
    
    EUNIT_ASSERT( codec == NULL );

    rtpmapAttribute->SetL( rtpmapStr, _L8("96"), _L8("AMR/foo") );

    EUNIT_ASSERT_LEAVE( iSdpCodec->CreateCodecLC( *rtpmapAttribute ) );

    rtpmapAttribute->SetL( rtpmapStr, _L8("aa"), _L8("AMR/8000") );

    EUNIT_ASSERT_LEAVE( iSdpCodec->CreateCodecLC( *rtpmapAttribute ) );
    
    CleanupStack::PopAndDestroy( rtpmapAttribute ); 

    }

void UT_CMceAudioSdpCodec::UT_CMceAudioSdpCodec_DecodeSessionMediaAttributesLL()
    {
    CSdpDocument* sdp = CSdpDocument::DecodeL( KMceTestSdpAMRSessionLevel );
    CleanupStack::PushL( sdp );
    
    CMceComMediaStream* mediaStream = iSession->Streams()[ 0 ];
    CMceComMediaStream* mediaBoundStream = &mediaStream->BoundStreamL();
    
    const RPointerArray<CMceComCodec>& codecs = iSdpCodec->CodecsL( *mediaStream );
    
    iSdpCodec->DecodeSessionMediaAttributesL( *mediaStream, *sdp );

    EUNIT_ASSERT( static_cast<CMceComAudioCodec*>(codecs[0])->iPTime == 20 );
    EUNIT_ASSERT( static_cast<CMceComAudioCodec*>(codecs[0])->iMaxPTime == 400 );

    static_cast<CMceComAudioCodec*>( codecs[0] )->iPTime = 0;
    static_cast<CMceComAudioCodec*>( codecs[0] )->iMaxPTime = 0;
    mediaStream->iStreamType = CMceComMediaStream::EReceiveStream;
    mediaBoundStream->iStreamType = CMceComMediaStream::EReceiveStream;
    
    iSdpCodec->DecodeSessionMediaAttributesL( *mediaStream, *sdp );
    
    EUNIT_ASSERT( static_cast<CMceComAudioCodec*>(codecs[0])->iPTime == 0 );
    EUNIT_ASSERT( static_cast<CMceComAudioCodec*>(codecs[0])->iMaxPTime == 0 );

    mediaStream->iStreamType = CMceComMediaStream::EReceiveStream;
    mediaBoundStream->iStreamType = CMceComMediaStream::ESendStream;
    static_cast< CMceComAudioStream* >( mediaStream )->DestroyCodecs();
    iSdpCodec->DecodeSessionMediaAttributesL( *mediaStream, *sdp );

    sdp->AttributeFields().ResetAndDestroy();
    iSdpCodec->DecodeSessionMediaAttributesL( *mediaStream, *sdp );
    
    CleanupStack::PopAndDestroy( sdp );

    }


	
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceAudioSdpCodec,
    "CMceAudioSdpCodec tests",
    "UNIT" )

EUNIT_TEST (
    "Encodes test",
    "CMceAudioSdpCodec",
    "Encodes",
    "FUNCTIONALITY",
    SetupL, UT_CMceAudioSdpCodec_EncodesL, Teardown)

EUNIT_TEST (
    "CodecsL test",
    "CMceAudioSdpCodec",
    "CodecsL",
    "FUNCTIONALITY",
    SetupL, UT_CMceAudioSdpCodec_CodecsLL, Teardown)

EUNIT_TEST (
    "EncodeRtpmapAttributeLC test",
    "CMceAudioSdpCodec",
    "EncodeRtpmapAttributeLC",
    "FUNCTIONALITY",
    SetupL, UT_CMceAudioSdpCodec_EncodeRtpmapAttributeLCL, Teardown)

EUNIT_TEST (
    "EncodeMediaAttributes test",
    "CMceAudioSdpCodec",
    "EncodeMediaAttributesL",
    "FUNCTIONALITY",
    SetupL, UT_CMceAudioSdpCodec_EncodeMediaAttributesLL, Teardown)

EUNIT_TEST (
    "DecodeMediaAttributesL test",
    "CMceAudioSdpCodec",
    "DecodeMediaAttributesL",
    "FUNCTIONALITY",
    SetupL, UT_CMceAudioSdpCodec_DecodeMediaAttributesLL, Teardown)

EUNIT_TEST (
    "CreateStreamLC test",
    "CMceAudioSdpCodec",
    "CreateStreamLC",
    "FUNCTIONALITY",
    SetupL, UT_CMceAudioSdpCodec_CreateStreamLCL, Teardown)

EUNIT_TEST (
    "UpdateStreamL test",
    "CMceAudioSdpCodec",
    "UpdateStreamL",
    "FUNCTIONALITY",
    SetupL, UT_CMceAudioSdpCodec_UpdateStreamLL, Teardown)

EUNIT_TEST (
    "CreateCodecLC test",
    "CMceAudioSdpCodec",
    "CreateCodecLC",
    "FUNCTIONALITY",
    SetupL, UT_CMceAudioSdpCodec_CreateCodecLCL, Teardown)

EUNIT_TEST (
    "DecodeSessionMediaAttributesL test",
    "CMceAudioSdpCodec",
    "DecodeSessionMediaAttributesL",
    "FUNCTIONALITY",
    SetupL, UT_CMceAudioSdpCodec_DecodeSessionMediaAttributesLL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
