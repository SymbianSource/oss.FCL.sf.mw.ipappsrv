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
#include "UT_cmcevideosdpcodec.h"

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
    
#include "mcevideosdpcodec.h"
#include "mcemediaidle.h"
#include "mcecomh263codec.h"

_LIT8( KMceTestSdpH263, "v=0\r\n\
o=userb 1234567890 1234567890 IN IP4 10.20.180.72\r\n\
s=-\r\n\
i=-\r\n\
c=IN IP4 10.20.180.72\r\n\
t=0 0\r\n\
m=video 5078 RTP/AVP 96\r\n\
a=sendrecv\r\n\
a=curr:qos local sendrecv\r\n\
a=curr:qos remote none\r\n\
a=des:qos mandatory local sendrecv\r\n\
a=des:qos none remote sendrecv\r\n\
a=rtpmap:96 H263-2000/90000\r\n\
a=framerate:15\r\n\
a=framesize:176-144\r\n\
a=fmtp:96 profile=0;level=10\r\n");

_LIT8( KMceTestSdpH263SessionLevel, "v=0\r\n\
o=userb 1234567890 1234567890 IN IP4 10.20.180.72\r\n\
s=-\r\n\
i=-\r\n\
c=IN IP4 10.20.180.72\r\n\
t=0 0\r\n\
a=framerate:15\r\n\
a=framesize:176-144\r\n\
m=audio 5078 RTP/AVP 96\r\n\
a=sendrecv\r\n\
a=curr:qos local sendrecv\r\n\
a=curr:qos remote none\r\n\
a=des:qos mandatory local sendrecv\r\n\
a=des:qos none remote sendrecv\r\n\
a=rtpmap:96 H263-2000/90000\r\n\
a=fmtp:96 profile=0;level=10\r\n");


//  INTERNAL INCLUDES

// CONSTRUCTION
UT_CMceVideoSdpCodec* UT_CMceVideoSdpCodec::NewL()
    {
    UT_CMceVideoSdpCodec* self = UT_CMceVideoSdpCodec::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceVideoSdpCodec* UT_CMceVideoSdpCodec::NewLC()
    {
    UT_CMceVideoSdpCodec* self = new( ELeave ) UT_CMceVideoSdpCodec();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceVideoSdpCodec::~UT_CMceVideoSdpCodec()
    {
    }

// Default constructor
UT_CMceVideoSdpCodec::UT_CMceVideoSdpCodec()
    {
    }

// Second phase construct
void UT_CMceVideoSdpCodec::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_CMceVideoSdpCodec::SetupL()
    {
    iServer = new (ELeave) CMceServerStub();
    iManager = CMceMediaManager::NewL( *iServer );
    iSession = MceMediaManagerTestHelper::CreateMultimediaOutSessionL();
    iSdpCodec = CMceVideoSdpCodec::NewL();
    TMceMediaIdle::SetAsCurrentStateL( *iSession );
    } 

void UT_CMceVideoSdpCodec::Teardown()
    {
	CMceComSession::Delete( iSession, *iManager );
	delete iSdpCodec;
	iSdpCodec = NULL;
	delete iManager;
	iManager = NULL;
	delete iServer;
	iServer = NULL;
    }

void UT_CMceVideoSdpCodec::UT_CMceVideoSdpCodec_EncodesL()
    {
    EUNIT_ASSERT( !iSdpCodec->Encodes( *iSession->Streams()[2] ) );
    EUNIT_ASSERT( !iSdpCodec->Encodes( *iSession->Streams()[0] ) );
    EUNIT_ASSERT( iSdpCodec->Encodes( *iSession->Streams()[1] ) );
    }

void UT_CMceVideoSdpCodec::UT_CMceVideoSdpCodec_CodecsLL()
    {
    
    EUNIT_ASSERT( iSdpCodec->CodecsL( *iSession->Streams()[1] ).Count() > 0 );
    EUNIT_ASSERT_LEAVE( iSdpCodec->CodecsL( *iSession->Streams()[0] ) );
    
    }

void UT_CMceVideoSdpCodec::UT_CMceVideoSdpCodec_EncodeRtpmapAttributeLCL()
    {
    CSdpFmtAttributeField* field = NULL;
    CMceComMediaStream* mediaStream = iSession->Streams()[1];
    const RPointerArray<CMceComCodec>& codecs = iSdpCodec->CodecsL( *mediaStream );
    
    field = iSdpCodec->EncodeRtpmapAttributeLC( *codecs[0] );
    
    EUNIT_ASSERT( field != NULL );
    
    CleanupStack::PopAndDestroy( field );
    }

void UT_CMceVideoSdpCodec::UT_CMceVideoSdpCodec_EncodeMediaAttributesLL()
    {
    CMceComMediaStream* mediaStream = iSession->Streams()[1];
    CMceComMediaStream* mediaBoundStream = &mediaStream->BoundStreamL();
    
    const RPointerArray<CMceComCodec>& codecs = iSdpCodec->CodecsL( *mediaStream );
    const RPointerArray<CMceComCodec>& boundCodecs = iSdpCodec->CodecsL( *mediaBoundStream );
    
    RStringF rtpmapStr = SDP_STRINGL( SdpCodecStringConstants::EAttributeRtpmap );
    MCE_DEFINE_DECSTR( payload, 96 );
    
    TSdpRtpmapValue rtpmapValue = TSdpRtpmapValue::DecodeL( _L8("H263-2000/90000") );
    HBufC8* rtpmapBuf = rtpmapValue.EncodeL();
    CleanupStack::PushL( rtpmapBuf );
    CSdpFmtAttributeField* rtpmapAttribute = 
        CSdpFmtAttributeField::NewL( rtpmapStr, payload, *rtpmapBuf );
    CleanupStack::PopAndDestroy( rtpmapBuf ); 
    CleanupStack::PushL( rtpmapAttribute ); 

    RStringF video = MCE_SDP_STRING_VIDEOL();
    RStringF protocol = SDP_STRINGL( SdpCodecStringConstants::EProtocolRtpAvp );
    
    CSdpMediaField* videoLine = 
        CSdpMediaField::NewLC( video, 5005, protocol, _L8("0") );
    
    EUNIT_ASSERT( videoLine->AttributeFields().Count() == 0 );
    
    iSdpCodec->EncodeMediaAttributesL( *codecs[0], *videoLine, *rtpmapAttribute );
    
    EUNIT_ASSERT( videoLine->AttributeFields().Count() > 0 );
    videoLine->AttributeFields().ResetAndDestroy();
    
    iSdpCodec->EncodeMediaAttributesL( *boundCodecs[0], *videoLine, *rtpmapAttribute );
    
    EUNIT_ASSERT( videoLine->AttributeFields().Count() > 0 );

    videoLine->AttributeFields().ResetAndDestroy();
    mediaStream->iStreamType = CMceComMediaStream::ESendOnlyStream;
    mediaBoundStream->iStreamType = CMceComMediaStream::ESendOnlyStream;
    iSdpCodec->EncodeMediaAttributesL( *boundCodecs[0], *videoLine, *rtpmapAttribute );

    EUNIT_ASSERT( videoLine->AttributeFields().Count() > 0 );

    
    mediaStream->iStreamType = CMceComMediaStream::EReceiveStream;
    mediaBoundStream->iStreamType = CMceComMediaStream::ESendStream;
    videoLine->AttributeFields().ResetAndDestroy();

    iSdpCodec->EncodeMediaAttributesL( *codecs[0], *videoLine, *rtpmapAttribute );
    
    EUNIT_ASSERT( videoLine->AttributeFields().Count() > 0 );

    videoLine->AttributeFields().ResetAndDestroy();

    iSdpCodec->EncodeMediaAttributesL( *codecs[0], *videoLine, *rtpmapAttribute );
    
    EUNIT_ASSERT( videoLine->AttributeFields().Count() > 0 );
    
    CleanupStack::PopAndDestroy( videoLine );
    CleanupStack::PopAndDestroy( rtpmapAttribute );
    
    }
    

void UT_CMceVideoSdpCodec::UT_CMceVideoSdpCodec_DecodeMediaAttributesLL()
    {
    
    CSdpDocument* sdp = CSdpDocument::DecodeL( KMceTestSdpH263 );
    CleanupStack::PushL( sdp );
    
    CSdpMediaField* videoLine = sdp->MediaFields()[ 0 ];
    CMceComMediaStream* mediaStream = iSession->Streams()[ 1 ];
    CMceComMediaStream* mediaBoundStream = &mediaStream->BoundStreamL();
    
    const RPointerArray<CMceComCodec>& codecs = iSdpCodec->CodecsL( *mediaStream );
    
    CSdpFmtAttributeField* rtpmap = NULL;
    MCE_MMTEST_RTP_MAP_L( rtpmap, videoLine );
    iSdpCodec->DecodeMediaAttributesL( *videoLine, *codecs[0], *rtpmap );

    EUNIT_ASSERT( static_cast<CMceComVideoCodec*>(codecs[0])->iFrameRate == 15.0 );
    EUNIT_ASSERT( static_cast<CMceComVideoCodec*>(codecs[0])->iResolutionWidth == 144 );
    EUNIT_ASSERT( static_cast<CMceComVideoCodec*>(codecs[0])->iResolutionHeight == 176 );

    static_cast<CMceComVideoCodec*>( codecs[0] )->iFrameRate = 0.0;
    static_cast<CMceComVideoCodec*>( codecs[0] )->iResolutionWidth = 0;
    static_cast<CMceComVideoCodec*>( codecs[0] )->iResolutionHeight = 0;
    static_cast<CMceComVideoCodec*>( codecs[0] )->iReceiveFrameRate = 0.0;
    mediaStream->iStreamType = CMceComMediaStream::EReceiveStream;
    mediaBoundStream->iStreamType = CMceComMediaStream::EReceiveStream;
    
    iSdpCodec->DecodeMediaAttributesL( *videoLine, *codecs[0], *rtpmap );
    
    EUNIT_ASSERT( static_cast<CMceComVideoCodec*>(codecs[0])->iFrameRate == 0.0 );
    EUNIT_ASSERT( static_cast<CMceComVideoCodec*>(codecs[0])->iResolutionWidth == 0 );
    EUNIT_ASSERT( static_cast<CMceComVideoCodec*>(codecs[0])->iResolutionHeight == 0 );
    EUNIT_ASSERT( static_cast<CMceComVideoCodec*>(codecs[0])->iReceiveFrameRate == 15.0 );

    static_cast<CMceComVideoCodec*>( codecs[0] )->iFrameRate = 0.0;
    static_cast<CMceComVideoCodec*>( codecs[0] )->iResolutionWidth = 0;
    static_cast<CMceComVideoCodec*>( codecs[0] )->iResolutionHeight = 0;
    mediaStream->iStreamType = CMceComMediaStream::EReceiveStream;
    mediaBoundStream->iStreamType = CMceComMediaStream::ESendStream;
    videoLine->AttributeFields().ResetAndDestroy();
    
    iSdpCodec->DecodeMediaAttributesL( *videoLine, *codecs[0], *rtpmap );
    
    EUNIT_ASSERT( static_cast<CMceComVideoCodec*>(codecs[0])->iFrameRate == 0.0 );
    EUNIT_ASSERT( static_cast<CMceComVideoCodec*>(codecs[0])->iResolutionWidth == 0 );
    EUNIT_ASSERT( static_cast<CMceComVideoCodec*>(codecs[0])->iResolutionHeight == 0 );
    
    
    CleanupStack::PopAndDestroy( sdp );
    
    }

void UT_CMceVideoSdpCodec::UT_CMceVideoSdpCodec_CreateStreamLCL()
    {
    
    CMceComMediaStream* mediaStream = NULL;
    
    mediaStream = iSdpCodec->CreateStreamLC( KErrNotFound );
    mediaStream->InitializeL( *iSession );
    EUNIT_ASSERT( mediaStream->iType == KMceVideo );
    EUNIT_ASSERT( mediaStream->BoundStream() );
    EUNIT_ASSERT( mediaStream->iStreamType == CMceComMediaStream::EReceiveStream );
    
    CleanupStack::PopAndDestroy( mediaStream );

    mediaStream = iSdpCodec->CreateStreamLC( SdpCodecStringConstants::EAttributeSendrecv );
    mediaStream->InitializeL( *iSession );
    EUNIT_ASSERT( mediaStream->iType == KMceVideo );
    EUNIT_ASSERT( mediaStream->BoundStream() );
    EUNIT_ASSERT( mediaStream->iStreamType == CMceComMediaStream::EReceiveStream );
    CleanupStack::PopAndDestroy( mediaStream );

    mediaStream = iSdpCodec->CreateStreamLC( SdpCodecStringConstants::EAttributeInactive );
    mediaStream->InitializeL( *iSession );
    EUNIT_ASSERT( mediaStream->iType == KMceVideo );
    EUNIT_ASSERT( mediaStream->BoundStream() );
    EUNIT_ASSERT( mediaStream->iStreamType == CMceComMediaStream::EReceiveStream );
    CleanupStack::PopAndDestroy( mediaStream );

    mediaStream = iSdpCodec->CreateStreamLC( SdpCodecStringConstants::EAttributeSendonly );
    mediaStream->InitializeL( *iSession );
    EUNIT_ASSERT( mediaStream->iType == KMceVideo );
    EUNIT_ASSERT( !mediaStream->BoundStream() );
    EUNIT_ASSERT( mediaStream->iStreamType == CMceComMediaStream::EReceiveOnlyStream );
    CleanupStack::PopAndDestroy( mediaStream );

    mediaStream = iSdpCodec->CreateStreamLC( SdpCodecStringConstants::EAttributeRecvonly );
    mediaStream->InitializeL( *iSession );
    EUNIT_ASSERT( mediaStream->iType == KMceVideo );
    EUNIT_ASSERT( !mediaStream->BoundStream() );
    EUNIT_ASSERT( mediaStream->iStreamType == CMceComMediaStream::ESendOnlyStream );
    CleanupStack::PopAndDestroy( mediaStream );
    
    
    }

void UT_CMceVideoSdpCodec::UT_CMceVideoSdpCodec_UpdateStreamLL()
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

void UT_CMceVideoSdpCodec::UT_CMceVideoSdpCodec_CreateCodecLCL()
    {
    iSdpCodec->AddCapabilityL( KMceSDPNameH2632000 );
    
    RStringF rtpmapStr = SDP_STRINGL( SdpCodecStringConstants::EAttributeRtpmap );

    MCE_DEFINE_DECSTR( payload, 96 );
    TSdpRtpmapValue rtpmapValue = TSdpRtpmapValue::DecodeL( _L8("H263-2000/90000") );
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
    TSdpRtpmapValue rtpmapValue1 = TSdpRtpmapValue::DecodeL( _L8("H263-2000/90000") );
    rtpmapBuf = rtpmapValue1.EncodeL();
    CleanupStack::PushL( rtpmapBuf );
    rtpmapAttribute = 
        CSdpFmtAttributeField::NewL( rtpmapStr, illegalPayload, *rtpmapBuf );
    CleanupStack::PopAndDestroy( rtpmapBuf ); 
    CleanupStack::PushL( rtpmapAttribute ); 
    
    EUNIT_ASSERT_LEAVE( iSdpCodec->CreateCodecLC( *rtpmapAttribute ) );

    CleanupStack::PopAndDestroy( rtpmapAttribute ); 

    TSdpRtpmapValue rtpmapValue2 = TSdpRtpmapValue::DecodeL( _L8("FOOBAR/90000") );
    rtpmapBuf = rtpmapValue2.EncodeL();
    CleanupStack::PushL( rtpmapBuf );
    rtpmapAttribute = 
        CSdpFmtAttributeField::NewL( rtpmapStr, payload, *rtpmapBuf );
    CleanupStack::PopAndDestroy( rtpmapBuf ); 
    CleanupStack::PushL( rtpmapAttribute ); 

    codec = iSdpCodec->CreateCodecLC( *rtpmapAttribute );
    
    EUNIT_ASSERT( codec == NULL );

    rtpmapAttribute->SetL( rtpmapStr, _L8("96"), _L8("H263-2000/foo") );

    EUNIT_ASSERT_LEAVE( iSdpCodec->CreateCodecLC( *rtpmapAttribute ) );

    rtpmapAttribute->SetL( rtpmapStr, _L8("aa"), _L8("H263-2000/90000") );

    EUNIT_ASSERT_LEAVE( iSdpCodec->CreateCodecLC( *rtpmapAttribute ) );
    
    CleanupStack::PopAndDestroy( rtpmapAttribute ); 

    }

void UT_CMceVideoSdpCodec::UT_CMceVideoSdpCodec_DecodeSessionMediaAttributesLL()
    {
    CSdpDocument* sdp = CSdpDocument::DecodeL( KMceTestSdpH263SessionLevel );
    CleanupStack::PushL( sdp );
    
    CMceComMediaStream* mediaStream = iSession->Streams()[ 1 ];
    CMceComMediaStream* mediaBoundStream = &mediaStream->BoundStreamL();
    
    const RPointerArray<CMceComCodec>& codecs = iSdpCodec->CodecsL( *mediaStream );
    
    iSdpCodec->DecodeSessionMediaAttributesL( *mediaStream, *sdp );

    EUNIT_ASSERT( static_cast<CMceComVideoCodec*>(codecs[0])->iFrameRate == 15.0 );
    EUNIT_ASSERT( static_cast<CMceComVideoCodec*>(codecs[0])->iResolutionWidth == 144 );
    EUNIT_ASSERT( static_cast<CMceComVideoCodec*>(codecs[0])->iResolutionHeight == 176 );


    static_cast<CMceComVideoCodec*>( codecs[0] )->iFrameRate = 0.0;
    static_cast<CMceComVideoCodec*>( codecs[0] )->iResolutionWidth = 0;
    static_cast<CMceComVideoCodec*>( codecs[0] )->iResolutionHeight = 0;
    static_cast<CMceComVideoCodec*>( codecs[0] )->iReceiveFrameRate = 0.0;
    mediaStream->iStreamType = CMceComMediaStream::EReceiveStream;
    mediaBoundStream->iStreamType = CMceComMediaStream::EReceiveStream;
    
    iSdpCodec->DecodeSessionMediaAttributesL( *mediaStream, *sdp );
    
    EUNIT_ASSERT( static_cast<CMceComVideoCodec*>(codecs[0])->iFrameRate == 0.0 );
    EUNIT_ASSERT( static_cast<CMceComVideoCodec*>(codecs[0])->iResolutionWidth == 0 );
    EUNIT_ASSERT( static_cast<CMceComVideoCodec*>(codecs[0])->iResolutionHeight == 0 );
    EUNIT_ASSERT( static_cast<CMceComVideoCodec*>(codecs[0])->iReceiveFrameRate == 15.0 );

    mediaStream->iStreamType = CMceComMediaStream::EReceiveStream;
    mediaBoundStream->iStreamType = CMceComMediaStream::ESendStream;
    static_cast< CMceComVideoStream* >( mediaStream )->DestroyCodecs();
    iSdpCodec->DecodeSessionMediaAttributesL( *mediaStream, *sdp );

    sdp->AttributeFields().ResetAndDestroy();
    iSdpCodec->DecodeSessionMediaAttributesL( *mediaStream, *sdp );
    
    CleanupStack::PopAndDestroy( sdp );

    }

	
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceVideoSdpCodec,
    "CMceVideoSdpCodec tests",
    "UNIT" )

EUNIT_TEST (
    "Encodes test",
    "CMceVideoSdpCodec",
    "Encodes",
    "FUNCTIONALITY",
    SetupL, UT_CMceVideoSdpCodec_EncodesL, Teardown)

EUNIT_TEST (
    "CodecsL test",
    "CMceVideoSdpCodec",
    "CodecsL",
    "FUNCTIONALITY",
    SetupL, UT_CMceVideoSdpCodec_CodecsLL, Teardown)

EUNIT_TEST (
    "EncodeRtpmapAttributeLC test",
    "CMceVideoSdpCodec",
    "EncodeRtpmapAttributeLC",
    "FUNCTIONALITY",
    SetupL, UT_CMceVideoSdpCodec_EncodeRtpmapAttributeLCL, Teardown)

EUNIT_TEST (
    "EncodeMediaAttributes test",
    "CMceVideoSdpCodec",
    "EncodeMediaAttributesL",
    "FUNCTIONALITY",
    SetupL, UT_CMceVideoSdpCodec_EncodeMediaAttributesLL, Teardown)

EUNIT_TEST (
    "DecodeMediaAttributesL test",
    "CMceVideoSdpCodec",
    "DecodeMediaAttributesL",
    "FUNCTIONALITY",
    SetupL, UT_CMceVideoSdpCodec_DecodeMediaAttributesLL, Teardown)

EUNIT_TEST (
    "CreateStreamLC test",
    "CMceVideoSdpCodec",
    "CreateStreamLC",
    "FUNCTIONALITY",
    SetupL, UT_CMceVideoSdpCodec_CreateStreamLCL, Teardown)

EUNIT_TEST (
    "UpdateStreamL test",
    "CMceVideoSdpCodec",
    "UpdateStreamL",
    "FUNCTIONALITY",
    SetupL, UT_CMceVideoSdpCodec_UpdateStreamLL, Teardown)

EUNIT_TEST (
    "CreateCodecLC test",
    "CMceVideoSdpCodec",
    "CreateCodecLC",
    "FUNCTIONALITY",
    SetupL, UT_CMceVideoSdpCodec_CreateCodecLCL, Teardown)

EUNIT_TEST (
    "DecodeSessionMediaAttributesL test",
    "CMceVideoSdpCodec",
    "DecodeSessionMediaAttributesL",
    "FUNCTIONALITY",
    SetupL, UT_CMceVideoSdpCodec_DecodeSessionMediaAttributesLL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
