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
#include "UT_cmcemediasdpcodec.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <digia/eunit/CEUnitAllocTestCaseDecorator.h>
#include <mmcccodecinformation.h>
#include "mcemediamanager.h"

#define RESET()\
    iServer->Reset()
    
#include <sdpdocument.h>
#include <sdpcodecstringpool.h>
#include <sdpcodecstringconstants.h>
#include <sdpmediafield.h>
#include <sdpfmtattributefield.h>
#include <sdprtpmapvalue.h>
#include <sdpattributefield.h>
#include "mceaudiosdpcodec.h"
#include "mcevideosdpcodec.h"
#include "cleanupresetanddestroy.h"
#include "mcemediaidle.h"
#include "mcenatpluginmanager.h"
#include <sdpbandwidthfield.h>
#include "mcetesthelper.h"
_LIT8( KMceTestSdpAMR, "v=0\r\n\
o=userb 1234567890 1234567890 IN IP4 10.20.180.72\r\n\
s=-\r\n\
i=-\r\n\
c=IN IP4 10.20.180.72\r\n\
t=0 0\r\n\
m=audio 5078 RTP/AVP 0 96\r\n\
a=sendrecv\r\n\
a=curr:qos local sendrecv\r\n\
a=curr:qos remote none\r\n\
a=des:qos mandatory local sendrecv\r\n\
a=des:qos none remote sendrecv\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=ptime:20\r\n\
a=maxptime:400\r\n\
a=fmtp:96 octet-align=0; mode-set=0,1,2,3,4,5,6,7\r\n");

_LIT8( KMceTestSdpPCMU, "v=0\r\n\
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
a=rtpmap:96 PCMU/8000\r\n\
a=ptime:20\r\n\
a=maxptime:400\r\n\
a=fmtp:96 octet-align=0; mode-set=0,1,2,3,4,5,6,7\r\n");

_LIT8( KMceTestSdpPortZero, "v=0\r\n\
o=userb 1234567890 1234567890 IN IP4 10.20.180.72\r\n\
s=-\r\n\
i=-\r\n\
c=IN IP4 10.20.180.72\r\n\
t=0 0\r\n\
m=audio 0 RTP/AVP 96\r\n\
a=sendrecv\r\n\
a=curr:qos local sendrecv\r\n\
a=curr:qos remote none\r\n\
a=des:qos mandatory local sendrecv\r\n\
a=des:qos none remote sendrecv\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=ptime:20\r\n\
a=maxptime:400\r\n\
a=fmtp:96 octet-align=0; mode-set=0,1,2,3,4,5,6,7\r\n");


_LIT8( KMceTestSdpPCMA, "v=0\r\n\
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
a=rtpmap:96 PCMA/8000\r\n\
a=ptime:20\r\n\
a=maxptime:400\r\n\
a=fmtp:96 octet-align=0; mode-set=0,1,2,3,4,5,6,7\r\n");

_LIT8( KMceTestSdpAVC, "v=0\r\n\
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
a=rtpmap:96 H264/90000\r\n\
a=fmtp:96 profile-level-id=42800A\r\n");

_LIT8( KMceTestSdpAVC2, "v=0\r\n\
o=userb 1234567890 1234567890 IN IP4 10.20.180.72\r\n\
s=-\r\n\
i=-\r\n\
c=IN IP4 10.20.180.72\r\n\
t=0 0\r\n\
m=video 5078 RTP/AVP 96 97\r\n\
a=sendrecv\r\n\
a=curr:qos local sendrecv\r\n\
a=curr:qos remote none\r\n\
a=des:qos mandatory local sendrecv\r\n\
a=des:qos none remote sendrecv\r\n\
a=rtpmap:96 H264/90000\r\n\
a=fmtp:96 profile-level-id=42900B\r\n\
a=rtpmap:97 H264/90000\r\n\
a=fmtp:97 profile-level-id=42900B\r\n");

_LIT8( KMceTestSdpMulti, "v=0\r\n\
o=userb 1234567890 1234567890 IN IP4 10.20.180.72\r\n\
s=-\r\n\
i=-\r\n\
c=IN IP4 10.20.180.72\r\n\
t=0 0\r\n\
m=audio 5078 RTP/AVP 96 97\r\n\
a=sendrecv\r\n\
a=curr:qos local sendrecv\r\n\
a=curr:qos remote none\r\n\
a=des:qos mandatory local sendrecv\r\n\
a=des:qos none remote sendrecv\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=fmtp:96 octet-align=0; mode-set=0,1,2,3,4,5,6,7\r\n\
a=rtpmap:97 PCMA/8000\r\n\
a=fmtp:97 octet-align=0;\r\n\
a=fmtp:97 mode-set=0,1,2,3,4,5,6,7\r\n");

_LIT8( KMceTestSdpMultiMissingPtFromMediaLine, "v=0\r\n\
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
a=fmtp:96 octet-align=0; mode-set=0,1,2,3,4,5,6,7\r\n\
a=rtpmap:97 PCMA/8000\r\n\
a=fmtp:97 octet-align=0;\r\n\
a=fmtp:97 mode-set=0,1,2,3,4,5,6,7\r\n");

_LIT( KNullIpAddress, "0.0.0.0");


_LIT8( KMceTestSdpAMRWithb, "v=0\r\n\
o=userb 1234567890 1234567890 IN IP4 10.20.180.72\r\n\
s=-\r\n\
i=-\r\n\
c=IN IP4 10.20.180.72\r\n\
t=0 0\r\n\
m=audio 5078 RTP/AVP 96\r\n\
a=sendrecv\r\n\
b=AS:80\r\n\
a=curr:qos local sendrecv\r\n\
a=curr:qos remote none\r\n\
a=des:qos mandatory local sendrecv\r\n\
a=des:qos none remote sendrecv\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=ptime:20\r\n\
a=maxptime:400\r\n\
a=fmtp:96 octet-align=0; mode-set=0,1,2,3,4,5,6,7\r\n");


_LIT8( KMceTestSdpAMRWithOutDirection, "v=0\r\n\
o=userb 1234567890 1234567890 IN IP4 10.20.180.72\r\n\
s=-\r\n\
i=-\r\n\
c=IN IP4 10.20.180.72\r\n\
t=0 0\r\n\
m=audio 5078 RTP/AVP 0 96\r\n\
a=curr:qos local sendrecv\r\n\
a=curr:qos remote none\r\n\
a=des:qos mandatory local sendrecv\r\n\
a=des:qos none remote sendrecv\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=ptime:20\r\n\
a=maxptime:400\r\n\
a=fmtp:96 octet-align=0; mode-set=0,1,2,3,4,5,6,7\r\n");


_LIT8( KMceTestSdpHold, "v=0\r\n\
o=userb 1234567890 1234567890 IN IP4 10.20.180.72\r\n\
s=-\r\n\
i=-\r\n\
c=IN IP4 10.20.180.72\r\n\
t=0 0\r\n\
m=audio 5078 RTP/AVP 0 96\r\n\
a=sendonly\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=ptime:20\r\n\
a=maxptime:400\r\n\
a=fmtp:96 octet-align=0; mode-set=0,1,2,3,4,5,6,7\r\n");




//  INTERNAL INCLUDES

// CONSTRUCTION
UT_CMceMediaSdpCodec* UT_CMceMediaSdpCodec::NewL()
    {
    UT_CMceMediaSdpCodec* self = UT_CMceMediaSdpCodec::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceMediaSdpCodec* UT_CMceMediaSdpCodec::NewLC()
    {
    UT_CMceMediaSdpCodec* self = new( ELeave ) UT_CMceMediaSdpCodec();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceMediaSdpCodec::~UT_CMceMediaSdpCodec()
    {
    }

// Default constructor
UT_CMceMediaSdpCodec::UT_CMceMediaSdpCodec()
    {
    }

// Second phase construct
void UT_CMceMediaSdpCodec::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_CMceMediaSdpCodec::SetupL()
    {
    iServer = new (ELeave) CMceServerStub();
    iManager = CMceMediaManager::NewL( *iServer );
    
    // Make a dummy call to NatEnabled() to cause plugin to be loaded.
    // When the actual test case function uses the plugin, the plugin is already
    // loaded. Because if the test function would load the plugin, then EUnit
    // reports a memory leak, since the plugin is unloaded in Teardown().
    iManager->NatPluginManager().NatEnabledL();

    iSession = MceMediaManagerTestHelper::CreateMultimediaOutSessionL();
    iManager->CreateSessionL( *iSession );
    iSdpCodec = CMceAudioSdpCodec::NewL();
    } 


void UT_CMceMediaSdpCodec::Setup2L()
    {
    iServer = new (ELeave) CMceServerStub();
    iManager = CMceMediaManager::NewL( *iServer );
    
    // Make a dummy call to NatEnabled() to cause plugin to be loaded.
    // When the actual test case function uses the plugin, the plugin is already
    // loaded. Because if the test function would load the plugin, then EUnit
    // reports a memory leak, since the plugin is unloaded in Teardown().
    iManager->NatPluginManager().NatEnabledL();

    iSession = MceMediaManagerTestHelper::CreateMultimediaOutSessionL( ETrue ); // Avc
    iManager->CreateSessionL( *iSession );
    iSdpCodec = CMceAudioSdpCodec::NewL();
    } 
    
void UT_CMceMediaSdpCodec::Teardown()
    {
	CMceComSession::Delete( iSession, *iManager );
	delete iManager;
	iManager = NULL;
	delete iServer;
	iServer = NULL;
	delete iSdpCodec;
	iSdpCodec = NULL;
	
    }


void UT_CMceMediaSdpCodec::UT_CMceMediaSdpCodec_EncodeMediaOfferLL()
    {
    RStringF audio = MCE_SDP_STRING_AUDIOL();
    RStringF protocol = SDP_STRINGL( SdpCodecStringConstants::EProtocolRtpAvp );
    CSdpDocument* sdp = CSdpDocument::NewLC();
    
    CSdpMediaField* audioLine = 
        CSdpMediaField::NewLC( audio, 5005, protocol, _L8("0") );
    
    EUNIT_ASSERT( audioLine->FormatList() == _L8("0") );
    
    iSdpCodec->EncodeMediaOfferL( *iSession->Streams()[ 0 ], *audioLine, *sdp );
    
    EUNIT_ASSERT( audioLine->FormatList() == _L8("96") );
    
    iSession->iUseLocalPreconditions = ETrue;

    iSdpCodec->EncodeMediaOfferL( *iSession->Streams()[ 0 ], *audioLine, *sdp );

    EUNIT_ASSERT( audioLine->FormatList() == _L8("96") );
    
    CleanupStack::PopAndDestroy( audioLine );
    
    CleanupStack::PopAndDestroy( sdp );
    }

void UT_CMceMediaSdpCodec::UT_CMceMediaSdpCodec_DecodeMediaAnswerLL()
    {
    //Null tested
    
    CSdpDocument* sdp = CSdpDocument::DecodeL( KMceTestSdpAMR );
    CleanupStack::PushL( sdp );
    
    CSdpMediaField* audioLine = sdp->MediaFields()[ 0 ];
    CMceComMediaStream* mediaStream = iSession->Streams()[ 0 ];
    
    iSdpCodec->AddCapabilityL( KMceSDPNameAMR );
    iSdpCodec->AddCapabilityPayloadTypeL(KDefaultAmrNbPT);
    iSdpCodec->DecodeMediaAnswerL( *audioLine,
                                   *mediaStream,
                                   *sdp );
                                   

    iSession->iUseLocalPreconditions = ETrue;

    iSdpCodec->DecodeMediaAnswerL( *audioLine,
                                   *mediaStream,
                                   *sdp );

    CleanupStack::PopAndDestroy( sdp );
    
    //wrong codec
    sdp = CSdpDocument::DecodeL( KMceTestSdpPCMU );
    CleanupStack::PushL( sdp );
    audioLine = sdp->MediaFields()[ 0 ];
    mediaStream = iSession->Streams()[ 0 ];

    EUNIT_ASSERT_LEAVE( iSdpCodec->DecodeMediaAnswerL( *audioLine,
                                                       *mediaStream,
                                                       *sdp ) );

    CleanupStack::PopAndDestroy( sdp );
    
    //port 0
    sdp = CSdpDocument::DecodeL( KMceTestSdpPortZero );
    CleanupStack::PushL( sdp );
    audioLine = sdp->MediaFields()[ 0 ];
    mediaStream = iSession->Streams()[ 0 ];

    EUNIT_ASSERT_LEAVE( iSdpCodec->DecodeMediaAnswerL( *audioLine,
                                                       *mediaStream,
                                                       *sdp ) );

    CleanupStack::PopAndDestroy( sdp );

    // unused rtmap line
    sdp = CSdpDocument::DecodeL( KMceTestSdpMultiMissingPtFromMediaLine );
    CleanupStack::PushL( sdp );
    audioLine = sdp->MediaFields()[ 0 ];
    mediaStream = iSession->Streams()[ 0 ];

    iSdpCodec->DecodeMediaAnswerL( *audioLine, *mediaStream, *sdp );

    EUNIT_ASSERT( 
        static_cast<CMceComAudioStream*>( iSession->Streams()[ 0 ] )->Codecs().Count() == 1 );
    CleanupStack::PopAndDestroy( sdp );

    //no codecs
    sdp = CSdpDocument::DecodeL( KMceTestSdpAMR );
    CleanupStack::PushL( sdp );
    audioLine = sdp->MediaFields()[ 0 ];
    mediaStream = iSession->Streams()[ 0 ];
    
    mediaStream->DestroyCodecs();
    
    EUNIT_ASSERT_LEAVE( iSdpCodec->DecodeMediaAnswerL( *audioLine,
                                                       *mediaStream,
                                                       *sdp ) );

    CleanupStack::PopAndDestroy( sdp );
                                   
    }

void UT_CMceMediaSdpCodec::UT_CMceMediaSdpCodec_DecodeMediaOfferLL()
    {

	CMceComSession::Delete( iSession, *iManager );
    iSession = CMceComSession::NewL( CMceComSession::EInSession );
    iManager->CreateSessionL( *iSession );
    
    EUNIT_ASSERT( !iSdpCodec->IsSupported( KMceSDPNameAMR ) );
    EUNIT_ASSERT( iSession->Streams().Count() == 0 );

    CSdpDocument* sdp = CSdpDocument::DecodeL( KMceTestSdpAMR );
    CleanupStack::PushL( sdp );
    
    CSdpMediaField* audioLine = sdp->MediaFields()[ 0 ];
    CMceComMediaStream* mediaStream = NULL;

    EUNIT_ASSERT(     
        iSdpCodec->DecodeMediaOfferL( *audioLine, mediaStream, *iSession, *sdp ) 
        != KErrNone );
        
    EUNIT_ASSERT( mediaStream == NULL );
    EUNIT_ASSERT( iSession->Streams().Count() == 0 );
    
    CleanupStack::PopAndDestroy( sdp );
    
    iSdpCodec->AddCapabilityL( KMceSDPNameAMR );
    iSdpCodec->AddCapabilityL( KMceSDPNamePCMU );
    
    EUNIT_ASSERT( iSdpCodec->IsSupported( KMceSDPNameAMR ) );

    sdp = CSdpDocument::DecodeL( KMceTestSdpAMR );
    CleanupStack::PushL( sdp );
    
    audioLine = sdp->MediaFields()[ 0 ];
    mediaStream = NULL;

    EUNIT_ASSERT(     
        iSdpCodec->DecodeMediaOfferL( *audioLine, mediaStream, *iSession, *sdp ) 
        == KErrNone );

    EUNIT_ASSERT( mediaStream );
    EUNIT_ASSERT( mediaStream->iRemoteMediaPort == 5078 );
    EUNIT_ASSERT( iSession->Streams().Count() == 1 );
    
    CleanupStack::PopAndDestroy( sdp );


    iSession->iUseRemotePreconditions = ETrue;

    EUNIT_ASSERT( iSdpCodec->IsSupported( KMceSDPNameAMR ) );

    sdp = CSdpDocument::DecodeL( KMceTestSdpAMR );
    CleanupStack::PushL( sdp );
    
    audioLine = sdp->MediaFields()[ 0 ];
    mediaStream = NULL;

    EUNIT_ASSERT(     
        iSdpCodec->DecodeMediaOfferL( *audioLine, mediaStream, *iSession, *sdp ) 
        == KErrNone );

    EUNIT_ASSERT( mediaStream );
    EUNIT_ASSERT( mediaStream->iRemoteMediaPort == 5078 );
    EUNIT_ASSERT( iSession->Streams().Count() == 2 );

    CleanupStack::PopAndDestroy( sdp );
    
    sdp = CSdpDocument::DecodeL( KMceTestSdpMultiMissingPtFromMediaLine );
    CleanupStack::PushL( sdp );
    
    audioLine = sdp->MediaFields()[ 0 ];
    mediaStream = NULL;

    EUNIT_ASSERT(     
        iSdpCodec->DecodeMediaOfferL( *audioLine, mediaStream, *iSession, *sdp ) 
        != KErrNone );

//    EUNIT_ASSERT( mediaStream );
//    EUNIT_ASSERT( mediaStream->iRemoteMediaPort == 5078 );
//    EUNIT_ASSERT( iSession->Streams().Count() == 3 );
//    EUNIT_ASSERT( 
//        static_cast<CMceComAudioStream*>( iSession->Streams()[ 2 ] )->Codecs().Count() == 1 );
    
    CleanupStack::PopAndDestroy( sdp );
    
    }

void UT_CMceMediaSdpCodec::UT_CMceMediaSdpCodec_DecodeMediaUpdateLL()
    {

    iSdpCodec->AddCapabilityL( KMceSDPNameAMR );
    iSdpCodec->AddCapabilityPayloadTypeL(KDefaultAmrNbPT);
    iSdpCodec->AddCapabilityL( KMceSDPNamePCMU );
    iSdpCodec->AddCapabilityPayloadTypeL(KPcmuPayloadType);
    EUNIT_ASSERT( iSdpCodec->IsSupported( KMceSDPNameAMR ) );
    EUNIT_ASSERT( iSession->Streams().Count() == 3 );

    CSdpDocument* sdp = CSdpDocument::DecodeL( KMceTestSdpAMR );
    CleanupStack::PushL( sdp );
    
    CSdpMediaField* audioLine = sdp->MediaFields()[ 0 ];
    CMceComMediaStream* mediaStream = iSession->Streams()[0];
    const RPointerArray<CMceComCodec>& codecs = iSdpCodec->CodecsL( *mediaStream );
    
    EUNIT_ASSERT( codecs.Count() == 1 );
    EUNIT_ASSERT( codecs[0]->iSdpName == KMceSDPNameAMR );

    iSdpCodec->PrepareForDecodeL( *audioLine, mediaStream );
    EUNIT_ASSERT( !codecs[0]->iIsNegotiated  );
    
    EUNIT_ASSERT(     
        iSdpCodec->DecodeMediaUpdateL( *audioLine, *mediaStream, *sdp ) 
        == KErrNone );
        
    EUNIT_ASSERT( mediaStream->iRemoteMediaPort == 5078 );
    EUNIT_ASSERT( iSession->Streams().Count() == 3 );
    EUNIT_ASSERT( codecs.Count() == 2 );
    EUNIT_ASSERT( codecs[0]->iSdpName == KMceSDPNameAMR );
    EUNIT_ASSERT( codecs[0]->iIsNegotiated );
    EUNIT_ASSERT( codecs[1]->iSdpName == KMceSDPNamePCMU );
    EUNIT_ASSERT( codecs[1]->iIsNegotiated );
    
    CleanupStack::PopAndDestroy( sdp );
    
    iSession->iUseRemotePreconditions = ETrue;

    sdp = CSdpDocument::DecodeL( KMceTestSdpAMR );
    CleanupStack::PushL( sdp );
    
    audioLine = sdp->MediaFields()[ 0 ];
    iSdpCodec->PrepareForDecodeL( *audioLine, mediaStream );
    EUNIT_ASSERT( !codecs[0]->iIsNegotiated );

    EUNIT_ASSERT(     
        iSdpCodec->DecodeMediaUpdateL( *audioLine, *mediaStream, *sdp ) 
        == KErrNone );

    EUNIT_ASSERT( mediaStream );
    EUNIT_ASSERT( mediaStream->iRemoteMediaPort == 5078 );
    EUNIT_ASSERT( iSession->Streams().Count() == 3 );
    EUNIT_ASSERT( codecs.Count() == 2 );
    EUNIT_ASSERT( codecs[0]->iSdpName == KMceSDPNameAMR );
    EUNIT_ASSERT( codecs[0]->iIsNegotiated );
    EUNIT_ASSERT( codecs[1]->iSdpName == KMceSDPNamePCMU );
    EUNIT_ASSERT( codecs[1]->iIsNegotiated );
    codecs[1]->iPayloadType = 96;

    CleanupStack::PopAndDestroy( sdp );

    EUNIT_ASSERT( iSdpCodec->IsSupported( KMceSDPNamePCMU ) );

    sdp = CSdpDocument::DecodeL( KMceTestSdpPCMU );
    CleanupStack::PushL( sdp );
    
    audioLine = sdp->MediaFields()[ 0 ];
    iSdpCodec->PrepareForDecodeL( *audioLine, mediaStream );
    EUNIT_ASSERT( !codecs[0]->iIsNegotiated );

	// Payload type not supported 
    EUNIT_ASSERT(     
        iSdpCodec->DecodeMediaUpdateL( *audioLine, *mediaStream, *sdp ) 
        != KErrNone );

    EUNIT_ASSERT( mediaStream->iRemoteMediaPort == 5078 );
    EUNIT_ASSERT( iSession->Streams().Count() == 3 );
    EUNIT_ASSERT( codecs.Count() == 2 );
    EUNIT_ASSERT( codecs[0]->iSdpName == KMceSDPNameAMR );
    EUNIT_ASSERT( codecs[1]->iSdpName == KMceSDPNamePCMU );
    EUNIT_ASSERT( !codecs[0]->iIsNegotiated );
    EUNIT_ASSERT( !codecs[1]->iIsNegotiated );
    
    CleanupStack::PopAndDestroy( sdp );

    EUNIT_ASSERT( !iSdpCodec->IsSupported( KMceSDPNamePCMA ) );

    sdp = CSdpDocument::DecodeL( KMceTestSdpPCMA );
    CleanupStack::PushL( sdp );
    
    audioLine = sdp->MediaFields()[ 0 ];
    iSdpCodec->PrepareForDecodeL( *audioLine, mediaStream );
    EUNIT_ASSERT( !codecs[0]->iIsNegotiated );
    EUNIT_ASSERT( !codecs[1]->iIsNegotiated );

    EUNIT_ASSERT(     
        iSdpCodec->DecodeMediaUpdateL( *audioLine, *mediaStream, *sdp ) 
        != KErrNone );

    EUNIT_ASSERT( iSession->Streams().Count() == 3 );
    EUNIT_ASSERT( codecs.Count() == 2 );
    EUNIT_ASSERT( codecs[0]->iSdpName == KMceSDPNameAMR );
    EUNIT_ASSERT( codecs[1]->iSdpName == KMceSDPNamePCMU );
    EUNIT_ASSERT( !codecs[0]->iIsNegotiated );
    EUNIT_ASSERT( !codecs[1]->iIsNegotiated );
    
    CleanupStack::PopAndDestroy( sdp );
    
    }

void UT_CMceMediaSdpCodec::UT_CMceMediaSdpCodec_EncodeMediaAnswerLL()
    {
    CSdpDocument* sdp = CSdpDocument::DecodeL( KMceTestSdpAMR );
    CleanupStack::PushL( sdp );
    
    CSdpMediaField* audioLine = sdp->MediaFields()[ 0 ];
    CMceComMediaStream* mediaStream = iSession->Streams()[0];
    
    iSdpCodec->EncodeMediaAnswerL( *mediaStream, *audioLine, *sdp );

    EUNIT_ASSERT( audioLine->Port() != 0 );
    
    CleanupStack::PopAndDestroy( sdp );

    iSession->iUseRemotePreconditions = ETrue;

    sdp = CSdpDocument::DecodeL( KMceTestSdpAMR );
    CleanupStack::PushL( sdp );
    
    audioLine = sdp->MediaFields()[ 0 ];
    mediaStream = iSession->Streams()[0];
    
    iSdpCodec->EncodeMediaAnswerL( *mediaStream, *audioLine, *sdp );

    EUNIT_ASSERT( audioLine->Port() != 0 );
    
    CleanupStack::PopAndDestroy( sdp );
    
    mediaStream->DestroyCodecs();

    sdp = CSdpDocument::DecodeL( KMceTestSdpAMR );
    CleanupStack::PushL( sdp );
    
    audioLine = sdp->MediaFields()[ 0 ];
    mediaStream = iSession->Streams()[0];
    
    iSdpCodec->EncodeMediaAnswerL( *mediaStream, *audioLine, *sdp );
    
    EUNIT_ASSERT( audioLine->Port() == 0 );
    
    CleanupStack::PopAndDestroy( sdp );
    
    }

void UT_CMceMediaSdpCodec::UT_CMceMediaSdpCodec_CleanAttributesL()
    {
    _LIT8( KAttrValue, "120");
    RStringF audio = MCE_SDP_STRING_AUDIOL();
    RStringF protocol = SDP_STRINGL( SdpCodecStringConstants::EProtocolRtpAvp );
    CSdpAttributeField* attr = NULL;

    CSdpMediaField* audioLine = 
        CSdpMediaField::NewLC( audio, 5005, protocol, _L8("0") );

    attr = CSdpAttributeField::NewLC( SDP_STRINGL( SdpCodecStringConstants::EAttributeSendonly ), KNullDesC8 );
    audioLine->AttributeFields().AppendL( attr );
    CleanupStack::Pop( attr );
    
    attr = CSdpAttributeField::CSdpAttributeField::NewLC( SDP_STRINGL( SdpCodecStringConstants::EAttributePtime ), KAttrValue );
    audioLine->AttributeFields().AppendL( attr );
    CleanupStack::Pop( attr );

    EUNIT_ASSERT( audioLine->AttributeFields().Count() > 0 );
    iSdpCodec->CleanAttributes( *audioLine );
    

    EUNIT_ASSERT( audioLine->AttributeFields().Count() == 0 );
    
    CleanupStack::PopAndDestroy( audioLine );
    
    }

void UT_CMceMediaSdpCodec::UT_CMceMediaSdpCodec_PrepareForDecodeLL()
    {
    CSdpDocument* sdp = CSdpDocument::DecodeL( KMceTestSdpAMR );
    CleanupStack::PushL( sdp );
    
    CSdpMediaField* audioLine = sdp->MediaFields()[ 0 ];
    CMceComMediaStream* mediaStream = iSession->Streams()[0];
    const RPointerArray<CMceComCodec>& codecs = iSdpCodec->CodecsL( *mediaStream );
    
    codecs[0]->iIsNegotiated = ETrue;
    
    EUNIT_ASSERT( codecs.Count() == 1 );
    EUNIT_ASSERT( codecs[0]->iIsNegotiated );
    
    iSdpCodec->PrepareForDecodeL( *audioLine, mediaStream );
    
    EUNIT_ASSERT( !codecs[0]->iIsNegotiated );
    
    
    codecs[0]->iIsNegotiated = ETrue;
    iSdpCodec->PrepareForDecodeL( *audioLine, NULL );
    
    EUNIT_ASSERT( codecs[0]->iIsNegotiated );

    mediaStream->DestroyCodecs();
    iSdpCodec->PrepareForDecodeL( *audioLine, mediaStream );
    

    CleanupStack::PopAndDestroy( sdp );
    
    
    }

void UT_CMceMediaSdpCodec::UT_CMceMediaSdpCodec_EncodeClientAttributesLL()
    {

    CSdpDocument* sdp = CSdpDocument::DecodeL( KMceTestSdpAMR );
    CleanupStack::PushL( sdp );
    
    CSdpMediaField* audioLine = sdp->MediaFields()[ 0 ];
    CMceComMediaStream* mediaStream = iSession->Streams()[0];
    audioLine->AttributeFields().ResetAndDestroy();
    audioLine->BandwidthFields().ResetAndDestroy();
    EUNIT_ASSERT( audioLine->AttributeFields().Count() == 0 );
    EUNIT_ASSERT( audioLine->BandwidthFields().Count() == 0 );

    delete mediaStream->iLocalMediaSDPLines;
    mediaStream->iLocalMediaSDPLines = NULL;

    EUNIT_ASSERT( mediaStream->iLocalMediaSDPLines == NULL );
    EUNIT_ASSERT_LEAVE( iSdpCodec->EncodeClientAttributesL( *mediaStream, *audioLine ) );
    EUNIT_ASSERT( audioLine->AttributeFields().Count() == 0 );
    EUNIT_ASSERT( audioLine->BandwidthFields().Count() == 0 );
    
    mediaStream->iLocalMediaSDPLines = new (ELeave) CDesC8ArrayFlat( 5 );

    iSdpCodec->EncodeClientAttributesL( *mediaStream, *audioLine );
    EUNIT_ASSERT( audioLine->AttributeFields().Count() == 0 );
    EUNIT_ASSERT( audioLine->BandwidthFields().Count() == 0 );
    
    audioLine->AttributeFields().ResetAndDestroy();
    audioLine->BandwidthFields().ResetAndDestroy();
    MCE_DELETE( mediaStream->iLocalMediaSDPLines );
    mediaStream->iLocalMediaSDPLines = new (ELeave) CDesC8ArrayFlat( 5 );
    mediaStream->iLocalMediaSDPLines->AppendL( _L8("a=curr:qos local none\r\n") );
    
    iSdpCodec->EncodeClientAttributesL( *mediaStream, *audioLine );
    EUNIT_ASSERT( audioLine->AttributeFields().Count() == 1 );
    EUNIT_ASSERT( audioLine->BandwidthFields().Count() == 0 );
    
    audioLine->AttributeFields().ResetAndDestroy();
    audioLine->BandwidthFields().ResetAndDestroy();
    MCE_DELETE( mediaStream->iLocalMediaSDPLines );
    mediaStream->iLocalMediaSDPLines = new (ELeave) CDesC8ArrayFlat( 5 );
    mediaStream->iLocalMediaSDPLines->AppendL( _L8("b=AS:17\r\n") );
    iSdpCodec->EncodeClientAttributesL( *mediaStream, *audioLine );
    EUNIT_ASSERT( audioLine->AttributeFields().Count() == 0 );
    EUNIT_ASSERT( audioLine->BandwidthFields().Count() == 1 );
    
	iSdpCodec->CleanAttributes(*audioLine);
	EUNIT_ASSERT( audioLine->AttributeFields().Count() == 0 );
    EUNIT_ASSERT( audioLine->BandwidthFields().Count() == 0 );
    MCE_DELETE( mediaStream->iLocalMediaSDPLines );
    mediaStream->iLocalMediaSDPLines = new (ELeave) CDesC8ArrayFlat( 5 );
    mediaStream->iLocalMediaSDPLines->AppendL( _L8("A=curr:qos local none\r\n") );
    EUNIT_ASSERT_LEAVE( iSdpCodec->EncodeClientAttributesL( *mediaStream, *audioLine ) );
    EUNIT_ASSERT( audioLine->AttributeFields().Count() == 0 );
    EUNIT_ASSERT( audioLine->BandwidthFields().Count() == 0 );
    

    audioLine->AttributeFields().ResetAndDestroy();
    audioLine->BandwidthFields().ResetAndDestroy();
    MCE_DELETE( mediaStream->iLocalMediaSDPLines );
    mediaStream->iLocalMediaSDPLines = new (ELeave) CDesC8ArrayFlat( 5 );
    mediaStream->iLocalMediaSDPLines->AppendL( _L8("B=AS:17\r\n") );
    EUNIT_ASSERT_LEAVE( iSdpCodec->EncodeClientAttributesL( *mediaStream, *audioLine ) );
    EUNIT_ASSERT( audioLine->AttributeFields().Count() == 0 );
    EUNIT_ASSERT( audioLine->BandwidthFields().Count() == 0 );
    

    audioLine->AttributeFields().ResetAndDestroy();
    audioLine->BandwidthFields().ResetAndDestroy();
    MCE_DELETE( mediaStream->iLocalMediaSDPLines );
    mediaStream->iLocalMediaSDPLines = new (ELeave) CDesC8ArrayFlat( 5 );
    mediaStream->iLocalMediaSDPLines->AppendL( _L8("x=foo:faa\r\n") );
    EUNIT_ASSERT_LEAVE( iSdpCodec->EncodeClientAttributesL( *mediaStream, *audioLine ) );
    EUNIT_ASSERT( audioLine->AttributeFields().Count() == 0 );
    EUNIT_ASSERT( audioLine->BandwidthFields().Count() == 0 );
    CleanupStack::PopAndDestroy( sdp );
    
    }

void UT_CMceMediaSdpCodec::UT_CMceMediaSdpCodec_DecodeClientAttributesLL()
    {
    CSdpDocument* sdp = CSdpDocument::DecodeL( KMceTestSdpAMRWithb );
    CleanupStack::PushL( sdp );
    
    CSdpMediaField* audioLine = sdp->MediaFields()[ 0 ];
    CMceComMediaStream* mediaStream = iSession->Streams()[0];
    
    EUNIT_ASSERT( mediaStream->iRemoteMediaSDPLines->Count() == 0 );
    
    iSdpCodec->DecodeClientAttributesL( *audioLine, *mediaStream );
    EUNIT_ASSERT( mediaStream->iRemoteMediaSDPLines->Count() == 8 );
    EUNIT_ASSERT( mediaStream->BoundStreamL().iRemoteMediaSDPLines->Count() == 8 );
    
    CleanupStack::PopAndDestroy( sdp );

    MCE_DELETE( mediaStream->iRemoteMediaSDPLines );
    mediaStream->iRemoteMediaSDPLines = new (ELeave) CDesC8ArrayFlat( 5 );
    EUNIT_ASSERT( mediaStream->iRemoteMediaSDPLines->Count() == 0 );

    sdp = CSdpDocument::DecodeL( KMceTestSdpAMR );
    CleanupStack::PushL( sdp );

    audioLine = sdp->MediaFields()[ 0 ];
    iSdpCodec->DecodeClientAttributesL( *audioLine, *mediaStream );
    EUNIT_ASSERT( mediaStream->iRemoteMediaSDPLines->Count() == 7 );
    EUNIT_ASSERT( mediaStream->BoundStreamL().iRemoteMediaSDPLines->Count() == 7 );
    
    CleanupStack::PopAndDestroy( sdp );

    mediaStream->BindL( NULL );

    sdp = CSdpDocument::DecodeL( KMceTestSdpAMR );
    CleanupStack::PushL( sdp );

    audioLine = sdp->MediaFields()[ 0 ];
    iSdpCodec->DecodeClientAttributesL( *audioLine, *mediaStream );
    EUNIT_ASSERT( mediaStream->iRemoteMediaSDPLines->Count() == 7 );
    EUNIT_ASSERT( !mediaStream->BoundStream() )
    
    CleanupStack::PopAndDestroy( sdp );
    
    }
    
void UT_CMceMediaSdpCodec::UT_CMceMediaSdpCodec_DecodeRemoteRtcpFieldLL()
    {
    CSdpDocument* sdp = CSdpDocument::DecodeLC( KMceTestSdpAMR );
    CSdpMediaField* audioLine = sdp->MediaFields()[ 0 ];
    CMceComMediaStream* mediaStream = iSession->Streams()[0];

    // Valid rtcp attributes
    
    // No RCTP attribute
    iSdpCodec->DecodeRemoteRtcpFieldL( *audioLine, *mediaStream );
    EUNIT_ASSERT_EQUALS( 0, mediaStream->iRemoteRtcpPort )
    EUNIT_ASSERT( mediaStream->iRemoteRtcpAddress.IsUnspecified() )
    
    // RTCP attribute with both IP address and port present
    CSdpAttributeField* rtcp = 
        CSdpAttributeField::DecodeLC( _L8( "a=rtcp:53020 IN IP4 1.2.3.4\r\n" ) );
    audioLine->AttributeFields().AppendL( rtcp );
    CleanupStack::Pop( rtcp );
    iSdpCodec->DecodeRemoteRtcpFieldL( *audioLine, *mediaStream );
    EUNIT_ASSERT_EQUALS( 53020, mediaStream->iRemoteRtcpPort )
    EUNIT_ASSERT_EQUALS( INET_ADDR( 1,2,3,4 ), mediaStream->iRemoteRtcpAddress.Address() ) 
    audioLine->AttributeFields().ResetAndDestroy();

    // RTCP attribute with both IP address and port present
    rtcp = CSdpAttributeField::DecodeLC( _L8( "a=rtcp:53000\r\n" ) );  
    audioLine->AttributeFields().AppendL( rtcp );
    CleanupStack::Pop( rtcp );
    mediaStream->iRemoteRtcpAddress.Init( KAfInet );
    iSdpCodec->DecodeRemoteRtcpFieldL( *audioLine, *mediaStream );
    EUNIT_ASSERT_EQUALS( 53000, mediaStream->iRemoteRtcpPort )
    EUNIT_ASSERT( mediaStream->iRemoteRtcpAddress.IsUnspecified() )
    audioLine->AttributeFields().ResetAndDestroy();
    
    // Attributes that look similar to rtcp, but are not
        
    // Used by some video implementations
    rtcp = CSdpAttributeField::DecodeLC( _L8( "a=rtcp-fb:* nack pli\r\n" ) );
    audioLine->AttributeFields().AppendL( rtcp );
    CleanupStack::Pop( rtcp );    
    mediaStream->iRemoteRtcpAddress.Init( KAfInet );
    mediaStream->iRemoteRtcpPort = 0;
    iSdpCodec->DecodeRemoteRtcpFieldL( *audioLine, *mediaStream );
    EUNIT_ASSERT_EQUALS( 0, mediaStream->iRemoteRtcpPort )
    EUNIT_ASSERT( mediaStream->iRemoteRtcpAddress.IsUnspecified() )
    audioLine->AttributeFields().ResetAndDestroy();    
    
    // Used by some VoIP implementations
    rtcp = CSdpAttributeField::DecodeLC( _L8( "a=rtcpping:T:14\r\n" ) );
    audioLine->AttributeFields().AppendL( rtcp );
    CleanupStack::Pop( rtcp );
    iSdpCodec->DecodeRemoteRtcpFieldL( *audioLine, *mediaStream );
    EUNIT_ASSERT_EQUALS( 0, mediaStream->iRemoteRtcpPort )
    EUNIT_ASSERT( mediaStream->iRemoteRtcpAddress.IsUnspecified() )
    audioLine->AttributeFields().ResetAndDestroy();
    
    // Rtcp attribute has incorrectly our local address, it should not be used
    mediaStream->Session()->iLocalIpAddress.Input( _L("10.20.30.40") );
    rtcp = CSdpAttributeField::DecodeLC( _L8( "a=rtcp:5020 IN IP4 10.20.30.40\r\n" ) );
    audioLine->AttributeFields().AppendL( rtcp );
    CleanupStack::Pop( rtcp );
    iSdpCodec->DecodeRemoteRtcpFieldL( *audioLine, *mediaStream );
    EUNIT_ASSERT_EQUALS( 5020, mediaStream->iRemoteRtcpPort )
    EUNIT_ASSERT( mediaStream->iRemoteRtcpAddress.IsUnspecified() )  // Not changed
    audioLine->AttributeFields().ResetAndDestroy();

    // Rtcp attrubute is local loopback address, ok to use
    rtcp = CSdpAttributeField::DecodeLC( _L8( "a=rtcp:5020 IN IP4 127.0.0.1\r\n" ) );
    audioLine->AttributeFields().AppendL( rtcp );
    CleanupStack::Pop( rtcp );
    iSdpCodec->DecodeRemoteRtcpFieldL( *audioLine, *mediaStream );
    EUNIT_ASSERT_EQUALS( 5020, mediaStream->iRemoteRtcpPort )
    EUNIT_ASSERT_EQUALS( INET_ADDR( 127,0,0,1 ), mediaStream->iRemoteRtcpAddress.Address() )
    audioLine->AttributeFields().ResetAndDestroy();
    
    // Incorrect white space usage
    rtcp = CSdpAttributeField::DecodeLC( _L8( "a=rtcp: 5050  IN IP4 2.2.2.2 \r\n" ) );
    audioLine->AttributeFields().AppendL( rtcp );
    CleanupStack::Pop( rtcp );
    iSdpCodec->DecodeRemoteRtcpFieldL( *audioLine, *mediaStream );
    EUNIT_ASSERT_EQUALS( 5050, mediaStream->iRemoteRtcpPort )
    EUNIT_ASSERT_EQUALS( INET_ADDR( 2,2,2,2 ), mediaStream->iRemoteRtcpAddress.Address() )
    audioLine->AttributeFields().ResetAndDestroy();
    
    rtcp = CSdpAttributeField::DecodeLC( _L8( "a=rtcp:  50506\r\n" ) );
    audioLine->AttributeFields().AppendL( rtcp );
    CleanupStack::Pop( rtcp );
    iSdpCodec->DecodeRemoteRtcpFieldL( *audioLine, *mediaStream );
    EUNIT_ASSERT_EQUALS( 50506, mediaStream->iRemoteRtcpPort )
    EUNIT_ASSERT_EQUALS( INET_ADDR( 2,2,2,2 ), mediaStream->iRemoteRtcpAddress.Address() )
    audioLine->AttributeFields().ResetAndDestroy();
       
    CleanupStack::PopAndDestroy( sdp );
    }
    
void UT_CMceMediaSdpCodec::UT_CMceMediaSdpCodec_EncodelocalRtcpAttrLL()
    {
	_LIT8( KMatchRtcp,"*rtcp*" );
    CSdpDocument* sdp = CSdpDocument::DecodeL( KMceTestSdpAMR );
    CleanupStack::PushL( sdp );
    
    CSdpMediaField* audioLine = sdp->MediaFields()[ 0 ];
    CMceComMediaStream* mediaStream = iSession->Streams()[0];

	mediaStream->iLocalMediaPort = 0;//rtp port
	mediaStream->Session()->iLocalIpAddress.Input(_L("")) ;
    iSdpCodec->EncodelocalRtcpAttrL( *audioLine, *mediaStream );
    //check the encode value
    TBool enable = EFalse;

	if ( !iManager->NatPluginManager().NatEnabledL() )
		{
	    iSdpCodec->DecodeRemoteRtcpFieldL( *audioLine, *mediaStream );
		EUNIT_ASSERT( mediaStream->iRemoteRtcpPort == 0 );
		}
	
	//now enable nat
	//iManager->NatPluginManager()->iNatEnable
	// remove sdp lines
	RPointerArray<CSdpAttributeField>& attrfields = audioLine->AttributeFields();
    for ( TInt i = 0; i < attrfields.Count(); i++ )
        {
        CSdpAttributeField* attrfield = attrfields[i];
        RStringF attribute;
        attribute = attrfield->Attribute();
        
        if ( attribute.DesC().Match ( KMatchRtcp ) != KErrNotFound ) 
            {
            attrfields.Remove( i );
            delete attrfield;
            }
        }
    
	
	/*case 2*/ 
    mediaStream->iLocalMediaPort = 53020;
    mediaStream->Session()->iLocalIpAddress.Input(_L("126.16.64.4")) ;
    if ( enable )
    	{
    	
	    iSdpCodec->EncodelocalRtcpAttrL( *audioLine, *mediaStream );
		//check the encode value
	    iSdpCodec->DecodeRemoteRtcpFieldL( *audioLine, *mediaStream );
		//rtcp port = rtp port + 1
		EUNIT_ASSERT( mediaStream->iRemoteRtcpPort == 53021 );
		EUNIT_ASSERT( mediaStream->iRemoteRtcpAddress 
						== mediaStream->Session()->iLocalIpAddress);
		
			// remove sdp lines
		attrfields = audioLine->AttributeFields();
	    for ( TInt i = 0; i < attrfields.Count(); i++ )
	        {
	        CSdpAttributeField* attrfield = attrfields[i];
	        RStringF attribute;
	        attribute = attrfield->Attribute();
	        
	        if ( attribute.DesC().Match ( KMatchRtcp ) != KErrNotFound ) 
	            {
	            attrfields.Remove( i );
	            delete attrfield;
	            }
	        }
	    //case 3 
	    mediaStream->iLocalMediaPort = 53020;
	    mediaStream->Session()->iLocalIpAddress.Input( KNullIpAddress ) ;
	    iSdpCodec->EncodelocalRtcpAttrL( *audioLine, *mediaStream );
		//check the encode value
	    iSdpCodec->DecodeRemoteRtcpFieldL( *audioLine, *mediaStream );
		//rtcp port = rtp port + 1
		EUNIT_ASSERT( mediaStream->iRemoteRtcpPort == 53021 );
		EUNIT_ASSERT( mediaStream->iRemoteRtcpAddress 
						!= mediaStream->Session()->iLocalIpAddress);
		mediaStream->Session()->iLocalIpAddress.Input(_L("126.16.64.4")) ;
		EUNIT_ASSERT( mediaStream->iRemoteRtcpAddress 
						== mediaStream->Session()->iLocalIpAddress);
    	}
    CleanupStack::PopAndDestroy( sdp );
    
    
    }    

void UT_CMceMediaSdpCodec::UT_CMceMediaSdpCodec_TypeL()
    {
    
    RStringF audio = MCE_SDP_STRING_AUDIOL();
    RStringF protocol = SDP_STRINGL( SdpCodecStringConstants::EProtocolRtpAvp );
    CSdpAttributeField* direction = NULL;
    RPointerArray<CSdpAttributeField> sessionAttributes;
    MceCleanupResetAndDestroyPushL( sessionAttributes );

    CSdpMediaField* audioLine = 
        CSdpMediaField::NewLC( audio, 5005, protocol, _L8("0") );

    EUNIT_ASSERT( iSdpCodec->Type( *audioLine, sessionAttributes ) == SdpCodecStringConstants::EAttributeSendrecv );

    direction = CSdpAttributeField::NewLC( SDP_STRINGL( SdpCodecStringConstants::EAttributeSendonly ), KNullDesC8 );
    audioLine->AttributeFields().AppendL( direction );
    CleanupStack::Pop( direction );

    EUNIT_ASSERT( iSdpCodec->Type( *audioLine, sessionAttributes ) == SdpCodecStringConstants::EAttributeSendonly );
    
    CleanupStack::PopAndDestroy( audioLine );
    CleanupStack::PopAndDestroy();//sessionAttributes
    
    }

void UT_CMceMediaSdpCodec::UT_CMceMediaSdpCodec_DecodePreconditionsLL()
    {
    RStringF audio = MCE_SDP_STRING_AUDIOL();
    RStringF protocol = SDP_STRINGL( SdpCodecStringConstants::EProtocolRtpAvp );
    
    CMceComMediaStream* mediaStream = iSession->Streams()[0];
    CSdpMediaField* audioLine = 
        CSdpMediaField::NewLC( audio, 5005, protocol, _L8("0") );

    iSession->iSessionModifiers[ KMcePreconditions ] = KMcePreconditionsNotUsed;
    EUNIT_ASSERT( !mediaStream->Preconditions().Count() );
    
    iSdpCodec->DecodePreconditionsL( *audioLine, *mediaStream, EMceRoleOfferer );
    EUNIT_ASSERT( !mediaStream->Preconditions().Count() );
    
    
    iSession->iSessionModifiers[ KMcePreconditions ] = KMcePreconditionsSupported;
    EUNIT_ASSERT( !iSession->iUseLocalPreconditions );
    EUNIT_ASSERT( !iSession->iUseRemotePreconditions );
    
    iSdpCodec->DecodePreconditionsL( *audioLine, *mediaStream, EMceRoleOfferer );
    EUNIT_ASSERT( !mediaStream->Preconditions().Count() );
    
    iSession->iUseRemotePreconditions = ETrue;
    
    iSdpCodec->DecodePreconditionsL( *audioLine, *mediaStream, EMceRoleOfferer );
    EUNIT_ASSERT( !mediaStream->Preconditions().Count() );

    iSession->iUseLocalPreconditions = EFalse;
    iSession->iUseRemotePreconditions = EFalse;
    iSdpCodec->DecodePreconditionsL( *audioLine, *mediaStream, EMceRoleAnswerer );
    EUNIT_ASSERT( !mediaStream->Preconditions().Count() );
    
    iSession->iUseLocalPreconditions = ETrue;
    
    iSdpCodec->DecodePreconditionsL( *audioLine, *mediaStream, EMceRoleAnswerer );
    EUNIT_ASSERT( !mediaStream->Preconditions().Count() );

    
    iSession->iUseLocalPreconditions = ETrue;
    iSdpCodec->DecodePreconditionsL( *audioLine, *mediaStream, EMceRoleOfferer );
    EUNIT_ASSERT( mediaStream->Preconditions().Count() );

    iSession->iUseRemotePreconditions = ETrue;
    iSdpCodec->DecodePreconditionsL( *audioLine, *mediaStream, EMceRoleAnswerer );
    EUNIT_ASSERT( mediaStream->Preconditions().Count() );
    
    //secure precondition
    iSession->iSessionModifiers[ KMcePreconditions ] = KMcePreconditionsNotUsed;
    EUNIT_ASSERT( mediaStream->Preconditions().Count() == 1 ); //qos precondition
    
    /*
    
    if ((aRole == EMceRoleOfferer && 
     	aStream.Session()->iClientCryptoSuites.Count()
     	&& aStream.Session()->Modifier( KMceSecPreconditions ) 
     	== KMcePreconditionsE2ESupported )
    */
    iSdpCodec->DecodePreconditionsL( *audioLine, *mediaStream, EMceRoleOfferer );
    EUNIT_ASSERT( mediaStream->Preconditions().Count() ==1 );
    
    iSession->iSessionModifiers[ KMceSecPreconditions ] = KMcePreconditionsE2ESupported;
    iSdpCodec->DecodePreconditionsL( *audioLine, *mediaStream, EMceRoleOfferer );
    EUNIT_ASSERT( mediaStream->Preconditions().Count() ==1 );
    
    mediaStream->Session()->iClientCryptoSuites.Append( EAES_CM_128_HMAC_SHA1_32 );
    iSdpCodec->DecodePreconditionsL( *audioLine, *mediaStream, EMceRoleOfferer );
    EUNIT_ASSERT( mediaStream->Preconditions().Count() == 1 );
    
    /*
     aRole ==EMceRoleAnswerer && (
     		aStream.Session()->iRemoteSecPreconditionsRequired ||
     		 aStream.Session()->iClientCryptoSuites.Count() )))
    
    */
    iSdpCodec->DecodePreconditionsL( *audioLine, *mediaStream, EMceRoleAnswerer );
    EUNIT_ASSERT( mediaStream->Preconditions().Count() ==1 );
    
    mediaStream->Session()->iRemoteSecPreconditionsRequired = ETrue;
    mediaStream->Session()->iClientCryptoSuites.Reset();
    iSdpCodec->DecodePreconditionsL( *audioLine, *mediaStream, EMceRoleAnswerer );
    EUNIT_ASSERT( mediaStream->Preconditions().Count() ==1 );
    
    mediaStream->Session()->iClientCryptoSuites.Append( EAES_CM_128_HMAC_SHA1_32 );
    iSdpCodec->DecodePreconditionsL( *audioLine, *mediaStream, EMceRoleAnswerer );
    EUNIT_ASSERT( mediaStream->Preconditions().Count() ==1 );
    
    mediaStream->Session()->iRemoteSecPreconditionsRequired = EFalse;
    //add audio line
    CSdpAttributeField* preconds = CSdpAttributeField::DecodeL( _L8("a=curr:sec e2e sendrecv\r\n") );
    CleanupStack::PushL( preconds );
    
    audioLine->AttributeFields().AppendL( preconds );
    CleanupStack::Pop( preconds );
    iSdpCodec->DecodePreconditionsL( *audioLine, *mediaStream, EMceRoleOfferer );
    EUNIT_ASSERT( mediaStream->Preconditions().Count() == 2 );
    
    
    
    mediaStream->Session()->iRemoteSecPreconditionsRequired = ETrue;
    iSdpCodec->DecodePreconditionsL( *audioLine, *mediaStream, EMceRoleAnswerer );
    EUNIT_ASSERT( mediaStream->Preconditions().Count() == 2 );
    
    CleanupStack::PopAndDestroy( audioLine );
    
    
    }

void UT_CMceMediaSdpCodec::UT_CMceMediaSdpCodec_EncodePreconditionsLL()
    {

    RStringF audio = MCE_SDP_STRING_AUDIOL();
    RStringF protocol = SDP_STRINGL( SdpCodecStringConstants::EProtocolRtpAvp );
    
    CMceComMediaStream* mediaStream = iSession->Streams()[0];
    CSdpMediaField* audioLine = 
        CSdpMediaField::NewLC( audio, 5005, protocol, _L8("0") );
        
    iSession->iSessionModifiers[ KMcePreconditions ] = KMcePreconditionsNotUsed;
    EUNIT_ASSERT( !mediaStream->Preconditions().Count() );
    
    iSdpCodec->EncodePreconditionsL( *mediaStream, *audioLine,  EMceRoleOfferer );
    EUNIT_ASSERT( !mediaStream->Preconditions().Count() );
    
    
    iSession->iSessionModifiers[ KMcePreconditions ] = KMcePreconditionsSupported;
    EUNIT_ASSERT( !iSession->iUseLocalPreconditions );
    EUNIT_ASSERT( !iSession->iUseRemotePreconditions );
    
    iSdpCodec->EncodePreconditionsL( *mediaStream, *audioLine, EMceRoleOfferer );
    EUNIT_ASSERT( !mediaStream->Preconditions().Count() );
    
    iSession->iUseRemotePreconditions = ETrue;
    
    iSdpCodec->EncodePreconditionsL( *mediaStream, *audioLine, EMceRoleOfferer );
    EUNIT_ASSERT( !mediaStream->Preconditions().Count() );

    iSession->iUseLocalPreconditions = EFalse;
    iSession->iUseRemotePreconditions = EFalse;
    iSdpCodec->EncodePreconditionsL( *mediaStream, *audioLine, EMceRoleAnswerer );
    EUNIT_ASSERT( !mediaStream->Preconditions().Count() );
    
    iSession->iUseLocalPreconditions = ETrue;
    
    iSdpCodec->EncodePreconditionsL( *mediaStream, *audioLine, EMceRoleAnswerer );
    EUNIT_ASSERT( !mediaStream->Preconditions().Count() );

    
    iSession->iUseLocalPreconditions = ETrue;
    iSdpCodec->EncodePreconditionsL( *mediaStream, *audioLine, EMceRoleOfferer );
    EUNIT_ASSERT( mediaStream->Preconditions().Count() );

    iSession->iUseRemotePreconditions = ETrue;
    iSdpCodec->EncodePreconditionsL( *mediaStream, *audioLine, EMceRoleAnswerer );
    EUNIT_ASSERT( mediaStream->Preconditions().Count() );
    
    //secure precondition
    
   
    mediaStream->Session()->Modifier( KMceSecPreconditions ) = KMcePreconditionsNotUsed;
    iSdpCodec->EncodePreconditionsL( *mediaStream, *audioLine, EMceRoleOfferer );
    EUNIT_ASSERT( mediaStream->Preconditions().Count() == 1 );
    iSdpCodec->EncodePreconditionsL( *mediaStream, *audioLine, EMceRoleAnswerer );
    EUNIT_ASSERT( mediaStream->Preconditions().Count() == 1 );
    
    
    mediaStream->Session()->Modifier( KMceSecPreconditions ) = KMcePreconditionsE2ESupported;
    iSdpCodec->EncodePreconditionsL( *mediaStream, *audioLine, EMceRoleAnswerer );
    EUNIT_ASSERT( mediaStream->Preconditions().Count() == 1 );
    
    iSdpCodec->EncodePreconditionsL( *mediaStream, *audioLine, EMceRoleOfferer );
    EUNIT_ASSERT( mediaStream->Preconditions().Count() == 1 );
    
    //added 
    EUNIT_DISABLE_ALLOC_DECORATOR;
    mediaStream->Session()->iClientCryptoSuites.Append( EAES_CM_128_HMAC_SHA1_32 );
    EUNIT_ENABLE_ALLOC_DECORATOR;
    
    iSdpCodec->EncodePreconditionsL( *mediaStream, *audioLine, EMceRoleAnswerer );
    EUNIT_ASSERT( mediaStream->Preconditions().Count() == 2 );
    
    mediaStream->Session()->SecureSessionL();
    
    iSdpCodec->EncodePreconditionsL( *mediaStream, *audioLine, EMceRoleAnswerer );
    EUNIT_ASSERT( mediaStream->Preconditions().Count() == 2 );
    
    CleanupStack::PopAndDestroy( audioLine );
    
    }

void UT_CMceMediaSdpCodec::UT_CMceMediaSdpCodec_EncodeFmtpAttributeLL()
    {
        
    _LIT8( KFmtp, "octet-align=0; mode-set=7");
    
    CMceComMediaStream* mediaStream = iSession->Streams()[0];
    const RPointerArray<CMceComCodec>& codecs = iSdpCodec->CodecsL( *mediaStream );
    RStringF audio = MCE_SDP_STRING_AUDIOL();
    RStringF protocol = SDP_STRINGL( SdpCodecStringConstants::EProtocolRtpAvp );
    
    CSdpMediaField* audioLine = 
        CSdpMediaField::NewLC( audio, 5005, protocol, _L8("0") );
        
    EUNIT_ASSERT( audioLine->FormatAttributeFields().Count() == 0 );        

    delete codecs[0]->iFmtpAttr;
    codecs[0]->iFmtpAttr = NULL;
    codecs[0]->iFmtpAttr = KFmtp().AllocL();
    
    iSdpCodec->EncodeFmtpAttributeL( *codecs[0], *audioLine );
    EUNIT_ASSERT( audioLine->FormatAttributeFields().Count() == 1 );
    
    delete codecs[0]->iFmtpAttr;
    codecs[0]->iFmtpAttr = NULL;
    codecs[0]->iFmtpAttr = KNullDesC8().AllocL();
    
    iSdpCodec->EncodeFmtpAttributeL( *codecs[0], *audioLine );
    EUNIT_ASSERT( audioLine->FormatAttributeFields().Count() == 1 );

    delete codecs[0]->iFmtpAttr;
    codecs[0]->iFmtpAttr = NULL;
    
    iSdpCodec->EncodeFmtpAttributeL( *codecs[0], *audioLine );
    EUNIT_ASSERT( audioLine->FormatAttributeFields().Count() == 1 );
    
    CleanupStack::PopAndDestroy( audioLine );
    
    
    }

void UT_CMceMediaSdpCodec::UT_CMceMediaSdpCodec_DecodeFmtpLinesLL()
    {
/*    CMceComMediaStream* mediaStream = iSession->Streams()[0];
    const RPointerArray<CMceComCodec>& codecs = iSdpCodec->CodecsL( *mediaStream );
    CMceComCodec::TIterator iterator( codecs );
    
    CMceComCodec* codec = codecs[0];

    CSdpDocument* sdp = CSdpDocument::DecodeL( KMceTestSdpMulti );
    CleanupStack::PushL( sdp );
    
    CSdpMediaField* audioLine = sdp->MediaFields()[ 0 ];
    
    codec->iIsNegotiated = ETrue;
    iSdpCodec->DecodeFmtpLinesL( *audioLine, iterator );
    EUNIT_ASSERT( codec->iFmtpAttr->Length() > 0 );

    codec->iIsNegotiated = EFalse;
    delete codec->iFmtpAttr;
    codec->iFmtpAttr = NULL;
    codec->iFmtpAttr = KNullDesC8().AllocL();
    
    iSdpCodec->DecodeFmtpLinesL( *audioLine, iterator );
    EUNIT_ASSERT( codec->iFmtpAttr->Length() == 0 );
    iterator.Reset();
    
    codec->iIsNegotiated = ETrue;
    codec->iPayloadType = 300;
    delete codec->iFmtpAttr;
    codec->iFmtpAttr = NULL;
    codec->iFmtpAttr = KNullDesC8().AllocL();
    
    iSdpCodec->DecodeFmtpLinesL( *audioLine, iterator );
    EUNIT_ASSERT( codec->iFmtpAttr->Length() > 0 );
        
    mediaStream->RemoveCodecFromListL( 0 );
    CleanupStack::PushL( codec );
    
    iSdpCodec->DecodeFmtpLinesL( *audioLine, iterator );
    iterator.Reset();

    codec->iIsNegotiated = ETrue;
    codec->iPayloadType = 96;
    delete codec->iFmtpAttr;
    codec->iFmtpAttr = NULL;
    codec->iFmtpAttr = KNullDesC8().AllocL();

    mediaStream->AddCodecL( codec );
    CleanupStack::Pop( codec );
    
    codec = CMceComG711Codec::NewLC( KMceSDPNamePCMU() );
    codec->iIsNegotiated = ETrue;
    codec->iPayloadType = 97;
    mediaStream->AddCodecL( codec );
    CleanupStack::Pop( codec );

    iSdpCodec->DecodeFmtpLinesL( *audioLine, iterator );
    EUNIT_ASSERT( codecs[0]->iFmtpAttr->Length() > 0 );
    EUNIT_ASSERT( codecs[1]->iFmtpAttr->Length() > 0 );
    iterator.Reset();

    
    CleanupStack::PopAndDestroy( sdp );
*/    
    }


void UT_CMceMediaSdpCodec::UT_CMceMediaSdpCodec_ValidateSdpL()
    {

    CSdpDocument* sdp = CSdpDocument::DecodeL( KMceTestSdpAMR );
    CleanupStack::PushL( sdp );
    
    RStringF audio = MCE_SDP_STRING_AUDIOL();
    RStringF protocol = SDP_STRINGL( SdpCodecStringConstants::EProtocolRtpAvp );
    
    CSdpMediaField* audioLine = 
        CSdpMediaField::NewLC( audio, 5005, protocol, _L8("0") );
        
    EUNIT_ASSERT( iSdpCodec->ValidateSdpL( *audioLine, *sdp ) == KErrNone );

    CleanupStack::PopAndDestroy( audioLine );

    audioLine = 
        CSdpMediaField::NewLC( audio, 5005, protocol, _L8("96") );
        
    EUNIT_ASSERT( iSdpCodec->ValidateSdpL( *audioLine, *sdp ) == KMceSipBadRequest );

    CleanupStack::PopAndDestroy( audioLine );
    
    audioLine = sdp->MediaFields()[ 0 ];
    
    EUNIT_ASSERT( iSdpCodec->ValidateSdpL( *audioLine, *sdp ) == KErrNone );
    
    CleanupStack::PopAndDestroy( sdp );

    }


void UT_CMceMediaSdpCodec::UT_CMceMediaSdpCodec_DecodeDirectionLL()
	{
	CSdpDocument* sdp = CSdpDocument::DecodeL( KMceTestSdpAMR );
        CleanupStack::PushL( sdp );
        iSession->SdpSession().iOOldSchool = 1; // OldSchool Tried Already
	CSdpMediaField* audioLine = sdp->MediaFields()[ 0 ];
        CMceComMediaStream* mediaStream = iSession->Streams()[ 0 ];

	TMceNegotiationRole aRole = EMceRoleOfferer;
	iSdpCodec->DecodeDirectionL(*audioLine, *mediaStream, *sdp, aRole );
	
	// EUNIT_ASSERT( iSession->SdpSession().iOOldSchool == 0 ); iOOldSchool may not be modified in DecodeDirectionL
	EUNIT_ASSERT( mediaStream->Direction() == SdpCodecStringConstants::EAttributeSendrecv );
	CleanupStack::PopAndDestroy( sdp );
	
	
	sdp = CSdpDocument::DecodeL( KMceTestSdpAMR );
        CleanupStack::PushL( sdp );
        iSession->SdpSession().iOOldSchool = 0; // OldSchool Tried Already
	audioLine = sdp->MediaFields()[ 0 ];
        mediaStream = iSession->Streams()[ 0 ];

	aRole = EMceRoleOfferer;
	iSdpCodec->UpdateDirectionL(*audioLine, *mediaStream, *sdp );
	
	EUNIT_ASSERT( iSession->SdpSession().iOOldSchool == 0 );
	EUNIT_ASSERT( mediaStream->Direction() == SdpCodecStringConstants::EAttributeSendrecv );
	CleanupStack::PopAndDestroy( sdp );
	
	
	sdp = CSdpDocument::DecodeL( KMceTestSdpAMRWithOutDirection );
        CleanupStack::PushL( sdp );
        iSession->SdpSession().iOOldSchool = 0; // OldSchool Tried Already
	audioLine = sdp->MediaFields()[ 0 ];
        mediaStream = iSession->Streams()[ 0 ];

	aRole = EMceRoleOfferer;
	
	//Check that Direction is not changed if it's not offered.
	iSdpCodec->DecodeDirectionL(*audioLine, *mediaStream, *sdp, aRole );
	
	EUNIT_ASSERT( iSession->SdpSession().iOOldSchool == 0 );
	EUNIT_ASSERT( mediaStream->Direction() == SdpCodecStringConstants::EAttributeSendrecv );
	CleanupStack::PopAndDestroy( sdp );
	
	sdp = CSdpDocument::DecodeL( KMceTestSdpAMRWithOutDirection );
        CleanupStack::PushL( sdp );
        iSession->SdpSession().iOOldSchool = 0; // OldSchool Tried Already
	audioLine = sdp->MediaFields()[ 0 ];
        mediaStream = iSession->Streams()[ 0 ];

	aRole = EMceRoleOfferer;
	iSdpCodec->UpdateDirectionL(*audioLine, *mediaStream, *sdp);
	
	EUNIT_ASSERT( iSession->SdpSession().iOOldSchool == 0 );
	EUNIT_ASSERT( mediaStream->Direction() == SdpCodecStringConstants::EAttributeSendrecv );
	CleanupStack::PopAndDestroy( sdp );
	
	// Check if we offer Sendonly and receive Sendonly  and oldschool is allowed
	// We try with Oldschool hold and assume that other 
	// party did not understand the hold request
	
	// SDP Answer to be decoded	contains sendonly direction attribute
	sdp = CSdpDocument::DecodeL( KMceTestSdpHold );
        CleanupStack::PushL( sdp );
        
	audioLine = sdp->MediaFields()[ 0 ];
	mediaStream = iSession->Streams()[ 0 ];
	
	// Set Local Streams as Sendonly
	mediaStream->SetDirection( SdpCodecStringConstants::EAttributeRecvonly );

	aRole = EMceRoleOfferer;

	// Set the flage that OldSchool is supported	
	iSession->Modifier( KMceMediaDirection ) = KMceMediaDirectionWithAddress;
	
	MCE_EUNIT_ASSERT_LEAVE( iSdpCodec->DecodeDirectionL(*audioLine, *mediaStream, *sdp, aRole ));
			
	// Ensure old school to be used next		
	EUNIT_ASSERT( iSession->SdpSession().iOOldSchool == 1 );

    CleanupStack::PopAndDestroy( sdp );

    sdp = CSdpDocument::DecodeL( KMceTestSdpAMRWithOutDirection );
    CleanupStack::PushL( sdp );
    iSession->SdpSession().iOOldSchool = 1; // OldSchool Tried Already
    audioLine = sdp->MediaFields()[ 0 ];
    mediaStream = iSession->Streams()[ 0 ];
    mediaStream->SetDirection( SdpCodecStringConstants::EAttributeSendrecv );

    aRole = EMceRoleOfferer;

    // Check if we offer Sendrecv to unhold the call and the other party
    // is just only accept oldschool hold.
    iSdpCodec->DecodeDirectionL(*audioLine, *mediaStream, *sdp, aRole );

    EUNIT_ASSERT( iSession->SdpSession().iOOldSchool == 1 );
    EUNIT_ASSERT( mediaStream->Direction() == SdpCodecStringConstants::EAttributeSendrecv );
    CleanupStack::PopAndDestroy( sdp );
    
	}
	

void UT_CMceMediaSdpCodec::UT_CMceMediaSdpCodec_DecodeRtpmapLinesLL() 
	{
	TMceNegotiationRole role = EMceRoleOfferer;
	CSdpDocument* sdp = CSdpDocument::DecodeL( KMceTestSdpPCMU );
    CleanupStack::PushL( sdp );
    
    CSdpMediaField* audioLine = sdp->MediaFields()[ 0 ];
    CMceComMediaStream* mediaStream = iSession->Streams()[0];
    const RPointerArray<CMceComCodec>& codecs = iSdpCodec->CodecsL( *mediaStream );
    codecs[0]->iSdpName = KPCMUSdpName;
    
    EUNIT_ASSERT(iSdpCodec->DecodeRtpmapLinesL( *audioLine, *mediaStream, role ) == 0);
    CleanupStack::PopAndDestroy( sdp );
    
    // Test avc decoding
    CMceVideoSdpCodec* videoSdpCodec = CMceVideoSdpCodec::NewL();
    CleanupStack::PushL( videoSdpCodec );
    videoSdpCodec->AddCapabilityL( KMceSDPNameH264 );
    videoSdpCodec->AddCapabilityPayloadTypeL( 100 );
    sdp = NULL;
	sdp = CSdpDocument::DecodeL( KMceTestSdpAVC );
    CleanupStack::PushL( sdp );
    
    CSdpMediaField* videoLine = sdp->MediaFields()[ 0 ];
    mediaStream = iSession->Streams()[1]; // video
    
    EUNIT_ASSERT(videoSdpCodec->DecodeRtpmapLinesL( *videoLine, *mediaStream, role ) == 1);
    CleanupStack::PopAndDestroy( sdp );
    const RPointerArray<CMceComCodec>& codecs1 = videoSdpCodec->CodecsL( *mediaStream );
    codecs1[0]->iIsNegotiated = 0;

    // No exact match for AVC codec in aswer, offered codec is still used as at
    // least SDP name matches
    sdp = NULL;
	sdp = CSdpDocument::DecodeL( KMceTestSdpAVC2 );
    CleanupStack::PushL( sdp );
    
    videoLine = sdp->MediaFields()[ 0 ];
    EUNIT_ASSERT(videoSdpCodec->DecodeRtpmapLinesL( *videoLine, *mediaStream, role ) == 2);
    EUNIT_ASSERT(codecs1[0]->iPayloadType == 96);
    CleanupStack::PopAndDestroy( sdp );
    
    CleanupStack::PopAndDestroy( videoSdpCodec );
    sdp = CSdpDocument::DecodeL( KMceTestSdpMulti );
    CleanupStack::PushL( sdp );
    
    TMceComFactory factory;
    audioLine = sdp->MediaFields()[ 0 ];
    mediaStream = iSession->Streams()[0];
    
    CMceComAudioCodec* pcma = 
        static_cast<CMceComAudioCodec*>(factory.AudioCodecFactory().CreateLC( KPCMASdpName() ));
    
    pcma->InitializeL(*mediaStream);
    pcma->iPayloadType = 93;
    pcma->iIsNegotiated = 0;
    mediaStream->AddCodecL( pcma );
    CleanupStack::Pop( pcma );
    iSdpCodec->AddCapabilityL( KAMRSdpName );
    iSdpCodec->AddCapabilityPayloadTypeL( 100 );
    iSdpCodec->AddCapabilityL( KPCMASdpName );
    iSdpCodec->AddCapabilityPayloadTypeL( 100 );

    const RPointerArray<CMceComCodec>& codecs2 = iSdpCodec->CodecsL( *mediaStream );
    codecs2[0]->iSdpName = KAMRSdpName;
    codecs2[1]->iSdpName = KPCMASdpName;
     
    EUNIT_ASSERT(iSdpCodec->DecodeRtpmapLinesL( *audioLine, *mediaStream, role ) == 2);
    EUNIT_ASSERT(codecs2[1]->iPayloadType == 97);
    
    CleanupStack::PopAndDestroy( sdp );
    
	}
   
	
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceMediaSdpCodec,
    "CMceMediaSdpCodec tests",
    "UNIT" )

EUNIT_TEST (
    "EncodeMediaOfferL test",
    "CMceMediaSdpCodec",
    "EncodeMediaOfferL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaSdpCodec_EncodeMediaOfferLL, Teardown)

EUNIT_TEST (
    "DecodeMediaAnswerL test",
    "CMceMediaSdpCodec",
    "DecodeMediaAnswerL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaSdpCodec_DecodeMediaAnswerLL, Teardown)

EUNIT_TEST (
    "DecodeMediaOfferL test",
    "CMceMediaSdpCodec",
    "DecodeMediaOfferL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaSdpCodec_DecodeMediaOfferLL, Teardown)

EUNIT_TEST (
    "DecodeMediaUpdateL test",
    "CMceMediaSdpCodec",
    "DecodeMediaUpdateL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaSdpCodec_DecodeMediaUpdateLL, Teardown)

EUNIT_TEST (
    "EncodeMediaAnswerL test",
    "CMceMediaSdpCodec",
    "EncodeMediaAnswerL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaSdpCodec_EncodeMediaAnswerLL, Teardown)

EUNIT_TEST (
    "CleanAttributes test",
    "CMceMediaSdpCodec",
    "CleanAttributes",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaSdpCodec_CleanAttributesL, Teardown)

EUNIT_TEST (
    "PrepareForDecodeL test",
    "CMceMediaSdpCodec",
    "PrepareForDecodeL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaSdpCodec_PrepareForDecodeLL, Teardown)

EUNIT_TEST (
    "EncodeClientAttributesL test",
    "CMceMediaSdpCodec",
    "EncodeClientAttributesL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaSdpCodec_EncodeClientAttributesLL, Teardown)

EUNIT_TEST (
    "DecodeClientAttributesL test",
    "CMceMediaSdpCodec",
    "DecodeClientAttributesL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaSdpCodec_DecodeClientAttributesLL, Teardown)


EUNIT_TEST (
    "DecodeRemoteRtcpFieldL test",
    "CMceMediaSdpCodec",
    "DecodeRemoteRtcpFieldL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaSdpCodec_DecodeRemoteRtcpFieldLL, Teardown)

EUNIT_TEST (
    "TypeL test",
    "CMceMediaSdpCodec",
    "Type",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaSdpCodec_TypeL, Teardown)

EUNIT_TEST (
    "DecodePreconditionsL test",
    "CMceMediaSdpCodec",
    "DecodePreconditionsL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaSdpCodec_DecodePreconditionsLL, Teardown)

EUNIT_TEST (
    "EncodePreconditionsL test",
    "CMceMediaSdpCodec",
    "EncodePreconditionsL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaSdpCodec_EncodePreconditionsLL, Teardown)

EUNIT_TEST (
    "EncodeFmtpAttributeL test",
    "CMceMediaSdpCodec",
    "EncodeFmtpAttributeL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaSdpCodec_EncodeFmtpAttributeLL, Teardown)

EUNIT_TEST (
    "DecodeFmtpLinesL test",
    "CMceMediaSdpCodec",
    "DecodeFmtpLinesL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaSdpCodec_DecodeFmtpLinesLL, Teardown)

EUNIT_TEST (
    "ValidateSdp test",
    "CMceMediaSdpCodec",
    "ValidateSdp",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaSdpCodec_ValidateSdpL, Teardown)

EUNIT_TEST (
    "Encode rtcp sdp attr  test",
    "CMceMediaSdpCodec",
    "EncodelocalRtcpAttr",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaSdpCodec_EncodelocalRtcpAttrLL, Teardown)
    

EUNIT_TEST (
    "DecodeDirection test",
    "CMceMediaSdpCodec",
    "DecodeDirectionAttribute",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaSdpCodec_DecodeDirectionLL, Teardown)        
    
EUNIT_TEST (
    "DecodeRtpmapLines test",
    "CMceMediaSdpCodec",
    "DecodeRtpmapLines",
    "FUNCTIONALITY",
    Setup2L, UT_CMceMediaSdpCodec_DecodeRtpmapLinesLL, Teardown)    


EUNIT_END_TEST_TABLE



//  END OF FILE
