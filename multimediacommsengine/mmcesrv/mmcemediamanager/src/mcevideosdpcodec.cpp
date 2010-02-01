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




// INCLUDE FILES
#include <e32base.h>
#include <e32std.h>
#include <e32def.h>
#include <stringpool.h>
#include <sdpdocument.h>
#include <sdpcodecstringconstants.h>
#include <sdpcodecstringpool.h>
#include <sdpmediafield.h>
#include <sdpattributefield.h>
#include <sdprtpmapvalue.h>
#include <sdpfmtattributefield.h>
#include <sdporiginfield.h>

#include "mcemediadefs.h"
#include "mcevideosdpcodec.h"
#include "mcevideostream.h"
#include "mcecomsession.h"
#include "mcecomvideostream.h"
#include "mcecomvideocodec.h"
#include "mceh263codec.h"
#include "mcecomfactory.h"
#include "mcecomrtpsource.h"
#include "mcecomrtpsink.h"
#include "mcecomcamerasource.h"
#include "mcecomdisplaysink.h"
#include "mcemmlogs.h"
#include "mcesip.h"



const TInt KFrameSizeLength = KMceSdpNumericAttributeLength * 3;

    
// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CMceVideoSdpCodec::NewL
// -----------------------------------------------------------------------------
//
CMceVideoSdpCodec* CMceVideoSdpCodec::NewL()
    {
    RStringF video = MCE_SDP_STRING_VIDEOL();
        
    CMceVideoSdpCodec* self = new (ELeave) CMceVideoSdpCodec( video );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceVideoSdpCodec::CMceVideoSdpCodec
// -----------------------------------------------------------------------------
//
CMceVideoSdpCodec::CMceVideoSdpCodec( RStringF aMedia )
    : CMceMediaSdpCodec( aMedia )
    {
    }


// -----------------------------------------------------------------------------
// CMceVideoSdpCodec::ConstructL
// -----------------------------------------------------------------------------
//    
void CMceVideoSdpCodec::ConstructL()
    {
    CMceMediaSdpCodec::ConstructL();
    
    iFrameSizeAttr = iStringPool.OpenFStringL( KMceVideoFrameSizeAttribute );
    }
    
// -----------------------------------------------------------------------------
// CMceVideoSdpCodec::~CMceVideoSdpCodec
// -----------------------------------------------------------------------------
//
CMceVideoSdpCodec::~CMceVideoSdpCodec()
    {
    iFrameSizeAttr.Close();
    }


// -----------------------------------------------------------------------------
// CMceVideoSdpCodec::Encodes
// -----------------------------------------------------------------------------
//
TBool CMceVideoSdpCodec::Encodes( const CMceComMediaStream& aStream ) const
    {
    return aStream.iStreamType != CMceComMediaStream::ELocalStream &&
           aStream.iType == KMceVideo;
    }


// -----------------------------------------------------------------------------
// CMceVideoSdpCodec::CodecsL
// -----------------------------------------------------------------------------
//
const RPointerArray<CMceComCodec>& CMceVideoSdpCodec::CodecsL( CMceComMediaStream& aStream ) const
    {
    __ASSERT_ALWAYS( aStream.iType == KMceVideo, User::Leave( KErrArgument ) );
    MCE_DEFINE_VIDEO( stream, aStream );

	return reinterpret_cast< const RPointerArray< CMceComCodec >& >( stream.Codecs() );
    }

// -----------------------------------------------------------------------------
// CMceVideoSdpCodec::EncodeRtpmapAttributeLC
// -----------------------------------------------------------------------------
//
CSdpFmtAttributeField* CMceVideoSdpCodec::EncodeRtpmapAttributeLC( CMceComCodec& aCodec ) const
    {
    MCEMM_DEBUG("CMceVideoSdpCodec::EncodeRtpmapAttributeLC(), Entry ");
    
    MCE_DEFINE_VIDEO_CODEC( codec, aCodec );
    CSdpFmtAttributeField* rtpmapAttribute = NULL;
    
    TUint payloadType = codec.iPayloadType;
    // Get the encoding name
    TPtrC8 codecName = codec.iSdpName; 
    TUint clockRate = codec.iClockRate;      
        
    // create rtpmap field for current codec
    rtpmapAttribute = EncodeRtpMapFieldsL( payloadType, 
                                           codecName, 
                                           clockRate,
                                           KMceSdpOptionalEncodingParam );
    
    
    CleanupStack::PushL( rtpmapAttribute );
    
    MCEMM_DEBUG_SVALUE("encoded rtpmap", rtpmapAttribute->Value() );
    
    MCEMM_DEBUG("CMceVideoSdpCodec::EncodeRtpmapAttributeLC(), Exit ");
    return rtpmapAttribute;
    
    
    }


// -----------------------------------------------------------------------------
// CMceVideoSdpCodec::EncodeMediaAttributesL
// -----------------------------------------------------------------------------
//
void CMceVideoSdpCodec::EncodeMediaAttributesL( CMceComCodec& aCodec, 
                                                CSdpMediaField& aMediaLine, 
                                                CSdpFmtAttributeField& aRtpmap ) const
    {
    MCEMM_DEBUG("CMceVideoSdpCodec::EncodeMediaAttributesL(), Entry ");
    MCE_DEFINE_VIDEO_CODEC( codec, aCodec );
    
    EncodeFrameRateAttributeL( codec, aMediaLine, aRtpmap );
    EncodeFrameSizeAttributeL( codec, aMediaLine, aRtpmap );
        
    MCEMM_DEBUG("CMceVideoSdpCodec::EncodeMediaAttributesL(), Exit ");
    }
    

// -----------------------------------------------------------------------------
// CMceVideoSdpCodec::DecodeMediaAttributesL  
// -----------------------------------------------------------------------------
//

void CMceVideoSdpCodec::DecodeMediaAttributesL( 
                                    CSdpMediaField& aMediaLine,
                                    CMceComCodec& aCodec,
                                    CSdpFmtAttributeField& /*aRtpmap*/ ) const

                                                
    {
    MCEMM_DEBUG("CMceVideoSdpCodec::DecodeMediaAttributesL(), Entry ");
    MCE_DEFINE_VIDEO_CODEC( codec, aCodec );
        
    RPointerArray<CSdpAttributeField>& attrfields = aMediaLine.AttributeFields();
    
    for ( TInt index = 0; index < attrfields.Count(); index++ )
        {
        CSdpAttributeField* attrfield = attrfields[ index ];
        RStringF attribute = attrfield->Attribute();
        
        if ( attribute == SDP_STRING( SdpCodecStringConstants::EAttributeFramerate ) )
            {
            const TDesC8& framerate = attrfield->Value();
            TReal frameRateVal = ConvertDesToRealL( framerate );
            MCEMM_DEBUG_SVALUE("decoded framerate", framerate );
            
            // Do not update frame rate for receive stream, but is anyway good
            // to know at media side what framerate sender is about to use.
            TReal& updateFrameRate = codec.Stream()->SendStream() ? 
                    codec.iFrameRate : codec.iReceiveFrameRate;
            
            updateFrameRate = frameRateVal;
            }
        else if ( codec.Stream()->SendStream() && attribute == iFrameSizeAttr )
            {
            const TDesC8& framesize = attrfield->Value();
            
            TInt ind1 = framesize.Locate( KMceSipBlank );
            TInt ind2 = framesize.Locate( KMceSipDash );
            TInt widthStartPos( ind1 + 1 );
            TInt heightStartPos( ind2 + 1 );  

            if ( ind1 != KErrNotFound && 
                 ind2 != KErrNotFound && 
                 widthStartPos < ind2 && 
                 heightStartPos < framesize.Length() &&
                 ConvertDesToUintL( framesize.Mid( 0, ind1 ) ) == 
                    codec.iPayloadType )
                {
                codec.iResolutionWidth = 
                    ConvertDesToIntL( 
                        framesize.Mid( widthStartPos, ind2 - widthStartPos ) );
                
                codec.iResolutionHeight = 
                    ConvertDesToIntL( framesize.Mid( heightStartPos ) );
                
                MCEMM_DEBUG_SVALUE("decoded framesize", framesize );
            
                }
            }
        else
            {
            // NOP
            }
        }
        
    MCEMM_DEBUG("CMceVideoSdpCodec::DecodeMediaAttributesL(), Exit ");    
    
    }



// -----------------------------------------------------------------------------
// CMceVideoSdpCodec::DecodeSessionMediaAttributesL
// -----------------------------------------------------------------------------
//
void CMceVideoSdpCodec::DecodeSessionMediaAttributesL( CMceComMediaStream& aStream, 
                                                       CSdpDocument& aSdpDocument ) const
    {   
    MCEMM_DEBUG("CMceVideoSdpCodec::DecodeSessionMediaAttributesL(), Entry ");
    MCE_DEFINE_VIDEO( stream, aStream );
    
    RPointerArray<CSdpAttributeField>& attrfields = 
                                            aSdpDocument.AttributeFields();

    const RPointerArray<CMceComVideoCodec>& codecs = stream.Codecs();
    // look for the framerate attribute, go through all
    // attribute fields and apply to all codecs in the stream if found
    for ( TInt index = 0; index < attrfields.Count(); index++ )
        {
        CSdpAttributeField* attrfield = attrfields[ index ];
        RStringF attribute = attrfield->Attribute();
        if ( attribute == SDP_STRING( SdpCodecStringConstants::EAttributeFramerate ) )
            {
            TReal frameRate = ConvertDesToRealL( attrfield->Value() );
            MCEMM_DEBUG_SVALUE("decoded framerate", attrfield->Value() );
            for ( TInt j = 0; j < codecs.Count(); j++ )
                {
                TReal& updateFrameRate = stream.SendStream() ? 
                    codecs[ j ]->iFrameRate : codecs[ j ]->iReceiveFrameRate;
                updateFrameRate = frameRate;
                }
            }
        }
        
    MCEMM_DEBUG("CMceVideoSdpCodec::ParseSessionFrameRateAttributeL(), Exit ");
    
    }
    
    
// -----------------------------------------------------------------------------
// CMceVideoSdpCodec::EncodeFrameRateAttributeL
// -----------------------------------------------------------------------------
//
void CMceVideoSdpCodec::EncodeFrameRateAttributeL( 
    CMceComVideoCodec& aCodec,
    CSdpMediaField& aMedia,
    CSdpFmtAttributeField& aFmtpline ) const
    {   
    MCEMM_DEBUG("CMceVideoSdpCodec::EncodeFrameRateAttributeL(), Entry ");
    
    RStringF attributeFrameRate = 
        SDP_STRING( SdpCodecStringConstants::EAttributeFramerate );
   
    HBufC8* frameRate = ConvertTRealToDesLC( aCodec.iFrameRate );

    MCEMM_DEBUG_SVALUE("encoded framerate", *frameRate );
    
    CSdpAttributeField* frameRateLine = 
        CSdpAttributeField::NewL( attributeFrameRate, *frameRate );
    CleanupStack::PopAndDestroy( frameRate );
    CleanupStack::PushL( frameRateLine ); 
    aMedia.AttributeFields().AppendL( frameRateLine );
    CleanupStack::Pop( frameRateLine ); 
    
    
    // This creates dependency between framerate and payload type
    frameRateLine->AssignTo( aFmtpline );

    MCEMM_DEBUG("CMceVideoSdpCodec::EncodeFrameRateAttributeL(), Exit ");
    }
    
// -----------------------------------------------------------------------------
// CMceVideoSdpCodec::EncodeFrameSizeAttributeL
// -----------------------------------------------------------------------------
//
void CMceVideoSdpCodec::EncodeFrameSizeAttributeL( 
    CMceComVideoCodec& aCodec,
    CSdpMediaField& aMedia,
    CSdpFmtAttributeField& aFmtpline ) const
    {   
    MCEMM_DEBUG("CMceVideoSdpCodec::EncodeFrameSizeAttributeL(), Entry ");

    HBufC8* frameSize = 
        HBufC8::NewLC( KMceVideoFrameSizeFormat().Length() + 
                       KFrameSizeLength );
    
    TPtr8 ptrFrameSize( frameSize->Des() );
    ptrFrameSize.AppendFormat( KMceVideoFrameSizeFormat, 
                               aCodec.iPayloadType, 
                               aCodec.iResolutionWidth, 
                               aCodec.iResolutionHeight );
                                    
    MCEMM_DEBUG_SVALUE("encoded framesize", ptrFrameSize );
    
    CSdpAttributeField* frameSizeLine = 
        CSdpAttributeField::NewL( iFrameSizeAttr, *frameSize );
    CleanupStack::PopAndDestroy( frameSize );
    CleanupStack::PushL( frameSizeLine ); 
    aMedia.AttributeFields().AppendL( frameSizeLine );
    CleanupStack::Pop( frameSizeLine ); 

    frameSizeLine->AssignTo( aFmtpline );

    MCEMM_DEBUG("CMceVideoSdpCodec::EncodeFrameSizeAttributeL(), Exit ");
    }
    
// -----------------------------------------------------------------------------
// CMceVideoSdpCodec::CreateStreamLC
// -----------------------------------------------------------------------------
//
CMceComMediaStream* CMceVideoSdpCodec::CreateStreamLC( TInt aType ) const
    {
    MCEMM_DEBUG("CMceVideoSdpCodec::CreateStreamLC(), Entry ");
    CMceComVideoStream* videostream = NULL;
                
    if( aType == SdpCodecStringConstants::EAttributeSendonly )
        {
        // downlink
        MCEMM_DEBUG( "create downlink" );
        videostream = CreateDownlinkStreamL();
        }
    else if( aType == SdpCodecStringConstants::EAttributeRecvonly )        
        {
        // uplink 
        MCEMM_DEBUG( "create uplink" );
        videostream = CreateUplinkStreamL();
        
        }
    else
        {
        // sendrcv 
        // create two way stream
        // create downlink
        MCEMM_DEBUG( "create two-way stream");
        videostream = CreateDownlinkStreamL();
        CleanupStack::PushL( videostream );
        // create bounded uplink
        CMceComVideoStream *boundUplink = CreateUplinkStreamL();
        CleanupStack::PushL( boundUplink );
        videostream->BindL( boundUplink );
        CleanupStack::Pop( boundUplink ); 
        CleanupStack::Pop( videostream );
        }
    
    CleanupStack::PushL( videostream );
    
    
    MCEMM_DEBUG("CMceVideoSdpCodec::CreateStreamLC(), Exit ");
    return videostream;
    
    }


// -----------------------------------------------------------------------------
// CMceVideoSdpCodec::UpdateStreamL
// -----------------------------------------------------------------------------
//
void CMceVideoSdpCodec::UpdateStreamL( CMceComMediaStream& aStream, TInt aDirection ) const
    {
    
    MCEMM_DEBUG("CMceVideoSdpCodec::UpdateStreamL(), Entry ");
    MCE_DEFINE_VIDEO( stream, aStream );

    CMceComVideoStream* videostream = NULL;
    TInt streamType = stream.SdpStreamType();

    if ( aDirection != SdpCodecStringConstants::EAttributeInactive )
        {
        switch( streamType )
            {
            case SdpCodecStringConstants::EAttributeRecvonly:
                {
                //if direction is changed
                if ( aDirection != SdpCodecStringConstants::EAttributeSendonly )
                    {
                    MCEMM_DEBUG("update from recvonly to send");
                    MCEMM_DEBUG("create uplink");
                    videostream = CreateUplinkStreamL();
                    }
                break;
                }
            case SdpCodecStringConstants::EAttributeSendonly:
                {
                //if direction is changed
                if ( aDirection != SdpCodecStringConstants::EAttributeRecvonly )
                    {
                    MCEMM_DEBUG("update from sendonly to recv");
                    MCEMM_DEBUG("create downlink");
                    videostream = CreateDownlinkStreamL();
                    }
                break;
                }
            case SdpCodecStringConstants::EAttributeSendrecv:
            default:
                {
                break;
                }
            }
        }
        
    if ( videostream )
        {
        CleanupStack::PushL( videostream );

        MCEMM_DEBUG("cloning codecs");
        
        for( TInt index = 0; index < stream.CodecCount(); index++ )
            {
            CMceComVideoCodec* codec = 
                static_cast<CMceComVideoCodec*>( stream.CodecL( index )->CloneL() );
            CleanupStack::PushL( codec );
            MCEMM_DEBUG_SVALUE("cloned codec", codec->iSdpName );
            videostream->AddCodecL( codec );
            CleanupStack::Pop( codec );
            }

        stream.BindL( videostream );
        
        videostream->InitializeL( *aStream.Session() );
        
        MCEMM_DEBUG("binded stream created");
        CleanupStack::Pop( videostream ); 

        
        }
    
    MCEMM_DEBUG("CMceVideoSdpCodec::UpdateStreamL(), Exit ");
    
    }

// -----------------------------------------------------------------------------
// CMceVideoSdpCodec::CreateCodecLC
// -----------------------------------------------------------------------------
//
CMceComCodec* CMceVideoSdpCodec::CreateCodecLC( CSdpFmtAttributeField& aRtpmap ) const
    {
    MCEMM_DEBUG("CMceVideoSdpCodec::CreateCodecLC(), Entry ");
    
    CMceComVideoCodec* codec = NULL;
    
    const TDesC8& payloadType = aRtpmap.Format();
    const TDesC8& attributeValue = aRtpmap.Value();
    TSdpRtpmapValue rtpmapValue = TSdpRtpmapValue::DecodeL( attributeValue );

    TPtrC8 codecName = rtpmapValue.iEncName;
    TPtrC8 clockRate = rtpmapValue.iClockrate;
    
    TUint uintValPT = ConvertDesToUintL( payloadType );
    TUint uintValCR = ConvertDesToUintL( clockRate );
    
    if ( uintValPT > KMceMaxPTValue )
        {
		MCEMM_DEBUG_DVALUE("CMceVideoSdpCodec::CreateCodecLC(), invalid payloadtype value ", uintValPT )
		User::Leave( KErrNotSupported );        	
        }
    if ( IsSupported( codecName ) )
        {
        TMceComVideoCodecFactory factory;
        codec = factory.CreateLC( codecName );
        codec->iClockRate = uintValCR;
        codec->iPayloadType = uintValPT;
        MCEMM_DEBUG_SVALUE("created codec", codecName );
        MCEMM_DEBUG_DVALUE("clock rate", uintValCR  )
        MCEMM_DEBUG_DVALUE("payload", uintValPT  )
        }
    else
        {
        MCEMM_DEBUG_SVALUE("not supported codec", codecName );
        }
        
    MCEMM_DEBUG("CMceVideoSdpCodec::CreateCodecLC(), Exit ");
    
    return codec;
    
    }

// -----------------------------------------------------------------------------
// CMceVideoSdpCodec::CreateCodecLC
// -----------------------------------------------------------------------------
//
CMceComCodec* CMceVideoSdpCodec::CreateCodecLC( TUint aPayload, 
                                                CSdpMediaField& /*aMediaLine*/ ) const
    {
    MCEMM_DEBUG("CMceVideoSdpCodec::CreateCodecLC( payload ), Entry ");
    
    TMceComVideoCodecFactory factory;
    CMceComVideoCodec* codec = NULL;
    
    codec = factory.CreateCodecLC( aPayload );
    if ( codec && !IsSupported( codec->iSdpName ) )
        {
        CleanupStack::PopAndDestroy( codec );
        codec = NULL;
        }

    MCEMM_DEBUG("CMceVideoSdpCodec::CreateCodecLC( payload ), Exit ");
    
    return codec;
    }

    
// -----------------------------------------------------------------------------
// CMceVideoSdpCodec::CreateUplinkStreamL
// -----------------------------------------------------------------------------
//
CMceComVideoStream* CMceVideoSdpCodec::CreateUplinkStreamL() const
    {
    MCEMM_DEBUG("CMceVideoSdpCodec::CreateUplinkStreamL(), Entry ");
    
    CMceComVideoStream* videoStream = CMceComVideoStream::NewL();
    CleanupStack::PushL( videoStream );
    // uplink           
    CMceComCameraSource* cameraSource = CMceComCameraSource::NewL();
    CleanupStack::PushL( cameraSource );
    cameraSource->InitializeL( 
                    static_cast<CMceComVideoStream&> ( *videoStream ) );
    videoStream->SetSourceL( cameraSource );
    CleanupStack::Pop( cameraSource );
    CMceComRtpSink* rtpSink = CMceComRtpSink::NewL();
    CleanupStack::PushL( rtpSink );
    rtpSink->InitializeL( static_cast<CMceComVideoStream&> ( *videoStream ) );
    videoStream->AddSinkL( rtpSink );
    CleanupStack::Pop( rtpSink ); 
    CleanupStack::Pop( videoStream );
    
    MCEMM_DEBUG("CMceVideoSdpCodec::CreateUplinkStreamL(), Exit ");
    
    return videoStream;
    }

// -----------------------------------------------------------------------------
// CMceVideoSdpCodec::CreateDownlinkStreamL
// -----------------------------------------------------------------------------
//
CMceComVideoStream* CMceVideoSdpCodec::CreateDownlinkStreamL() const
    {
    MCEMM_DEBUG("CMceVideoSdpCodec::CreateDownlinkStreamL(), Entry ");

    CMceComVideoStream* videoStream = CMceComVideoStream::NewL();
    CleanupStack::PushL( videoStream ); 
    // downlink         
    CMceComRtpSource* rtpSource = CMceComRtpSource::NewL();
    CleanupStack::PushL( rtpSource );
    rtpSource->InitializeL( static_cast<CMceComVideoStream&> ( *videoStream ) );
    videoStream->SetSourceL( rtpSource );
    CleanupStack::Pop( rtpSource );
    CMceComDisplaySink* display = CMceComDisplaySink::NewL();
    CleanupStack::PushL( display );
    display->InitializeL( static_cast<CMceComVideoStream&> ( *videoStream ) );
    videoStream->AddSinkL( display );
    CleanupStack::Pop( display ); 
    CleanupStack::Pop( videoStream );
    
    MCEMM_DEBUG("CMceVideoSdpCodec::CreateDownlinkStreamL(), Exit ");
    
    return videoStream;
    }
 
    
