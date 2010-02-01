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
#include "mceaudiosdpcodec.h"
#include "mcecomaudiostream.h"
#include "mceaudiostream.h"
#include "mcecomaudiocodec.h"
#include "mcemediadefs.h"
#include "mcecomfactory.h"
#include "mcemmlogs.h"
#include "mcecomrtpsource.h"
#include "mcecomspeakersink.h"
#include "mcecommicsource.h"
#include "mcecomrtpsink.h"
#include "mcedefs.h"


// ================= MEMBER FUNCTIONS =======================


// -----------------------------------------------------------------------------
// CMceAudioSdpCodec::NewL
// -----------------------------------------------------------------------------
//
CMceAudioSdpCodec* CMceAudioSdpCodec::NewL()
    {
    RStringF audio = MCE_SDP_STRING_AUDIOL();
        
    CMceAudioSdpCodec* self = new (ELeave) CMceAudioSdpCodec( audio );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceAudioSdpCodec::CMceAudioSdpCodec
// -----------------------------------------------------------------------------
//
CMceAudioSdpCodec::CMceAudioSdpCodec( RStringF aMedia )
    : CMceMediaSdpCodec( aMedia )
    {
    }


// -----------------------------------------------------------------------------
// CMceAudioSdpCodec::~CMceAudioSdpCodec
// -----------------------------------------------------------------------------
//
CMceAudioSdpCodec::~CMceAudioSdpCodec()
    {
    }

// -----------------------------------------------------------------------------
// CMceAudioSdpCodec::Encodes
// -----------------------------------------------------------------------------
//
TBool CMceAudioSdpCodec::Encodes( const CMceComMediaStream& aStream ) const
    {
    return aStream.iStreamType != CMceComMediaStream::ELocalStream &&
           aStream.iType == KMceAudio;
    }


// -----------------------------------------------------------------------------
// CMceAudioSdpCodec::CodecsL
// -----------------------------------------------------------------------------
//
const RPointerArray<CMceComCodec>& CMceAudioSdpCodec::CodecsL( CMceComMediaStream& aStream ) const
    {
    __ASSERT_ALWAYS( aStream.iType == KMceAudio, User::Leave( KErrArgument ) );
    MCE_DEFINE_AUDIO( stream, aStream );

	return reinterpret_cast< const RPointerArray< CMceComCodec >& >( stream.Codecs() );
    }

// -----------------------------------------------------------------------------
// CMceAudioSdpCodec::EncodeRtpmapAttributeLC
// -----------------------------------------------------------------------------
//
CSdpFmtAttributeField* CMceAudioSdpCodec::EncodeRtpmapAttributeLC( CMceComCodec& aCodec ) const
    {
    MCEMM_DEBUG( "CMceAudioSdpCodec::EncodeRtpmapAttributeLC(), Entry" )
    MCE_DEFINE_AUDIO_CODEC( codec, aCodec );
    
    CSdpFmtAttributeField* rtpmapAttribute = NULL;
    
    TUint payloadType = codec.iPayloadType;
    TPtrC8 codecName = codec.iSdpName; 
    TUint samplingFreq = codec.iSamplingFreq;      
        
    // Create rtpmap field for current codec
    rtpmapAttribute = EncodeRtpMapFieldsL( payloadType, 
                                           codecName, 
                                           samplingFreq,
                                           KMceSdpOptionalEncodingParam );
    
    CleanupStack::PushL( rtpmapAttribute );
    
    MCEMM_DEBUG_SVALUE( "encoded rtpmap", rtpmapAttribute->Value() )
    MCEMM_DEBUG( "CMceAudioSdpCodec::EncodeRtpmapAttributeLC(), Exit" )
    
    return rtpmapAttribute;
    
    
    }

// -----------------------------------------------------------------------------
// CMceAudioSdpCodec::EncodeMediaAttributesL
// -----------------------------------------------------------------------------
//
void CMceAudioSdpCodec::EncodeMediaAttributesL( CMceComCodec& aCodec, 
                                                CSdpMediaField& aMediaLine, 
                                                CSdpFmtAttributeField& aRtpmap ) const
    {
    MCEMM_DEBUG( "CMceAudioSdpCodec::EncodeMediaAttributesL(), Entry" )
    MCE_DEFINE_AUDIO_CODEC( codec, aCodec );
    
    if ( aCodec.Stream()->ReceiveStream() )
        {
        if ( codec.iPTime && 
             codec.iMaxPTime &&
             !FindAttribute( SdpCodecStringConstants::EAttributePtime, aMediaLine ) )
            {            
            RStringF attributePtime = SDP_STRING( SdpCodecStringConstants::EAttributePtime );
            RStringF attributeMaxPtime = SDP_STRING( SdpCodecStringConstants::EAttributeMaxptime );
               
            MCE_DEFINE_DECSTR( ptime, codec.iPTime )
            MCE_DEFINE_DECSTR( maxptime, codec.iMaxPTime )
            
            CSdpAttributeField* ptimeLine = 
                CSdpAttributeField::NewLC( attributePtime, ptime );
            CSdpAttributeField* maxptimeLine = 
                CSdpAttributeField::NewLC( attributeMaxPtime, maxptime );

            MCEMM_DEBUG_SVALUE( "encoded ptime", ptime )
            MCEMM_DEBUG_SVALUE( "encoded maxptime", maxptime )

            aMediaLine.AttributeFields().AppendL( ptimeLine );
            // this creates dependency between ptime and payload type
            ptimeLine->AssignTo( aRtpmap );
            CleanupStack::Pop( maxptimeLine ); 
            
            aMediaLine.AttributeFields().AppendL( maxptimeLine );
            // this creates dependency between maxptime and payload type
            maxptimeLine->AssignTo( aRtpmap ); 
            CleanupStack::Pop( ptimeLine ); 
            }
        }
        
    MCEMM_DEBUG( "CMceAudioSdpCodec::EncodeMediaAttributesL(), Exit" )
    }
    
// -----------------------------------------------------------------------------
// CMceAudioSdpCodec::DecodeMediaAttributesL 
// -----------------------------------------------------------------------------
//
void CMceAudioSdpCodec::DecodeMediaAttributesL(
    CSdpMediaField& aMediaLine, 
    CMceComCodec& aCodec,
    CSdpFmtAttributeField& /*aRtpmap*/ ) const
    {
    MCEMM_DEBUG( "CMceAudioSdpCodec::DecodeMediaAttributesL(), Entry ")
    
    MCE_DEFINE_AUDIO_CODEC( codec, aCodec );
    
    if ( aCodec.Stream()->SendStream() )
        {
		RPointerArray<CSdpAttributeField>& attrfields =
		    aMediaLine.AttributeFields();
		
		// Packet times are negotiated at session or media line level and thus 
		// several codecs share common packet time values.
        for ( TInt index = 0; index < attrfields.Count(); index++ )
            {
            CSdpAttributeField* attrfield = attrfields[ index ];
            RStringF attribute;
            attribute = attrfield->Attribute();
            if ( attribute ==
                 SDP_STRING( SdpCodecStringConstants::EAttributePtime ) )
                {
                TUint ptime = ConvertDesToUintL( attrfield->Value() );
                codec.SetPTime( ptime );
                
                MCEMM_DEBUG_DVALUE( "decoded ptime", ptime )
                }
            else if ( attribute ==
                 SDP_STRING( SdpCodecStringConstants::EAttributeMaxptime ) )
                {
                TUint maxptime = ConvertDesToUintL( attrfield->Value() );
                codec.SetMaxPTime( maxptime );
                
                MCEMM_DEBUG_DVALUE( "decoded maxptime", maxptime )
                }
            else
                {
                // do not mind other attributes
                }
            }
        }
    
    MCEMM_DEBUG_DVALUES( "exit ptime", codec.iPTime,
                         "maxptime", codec.iMaxPTime )
    MCEMM_DEBUG("CMceAudioSdpCodec::DecodeMediaAttributesL(), Exit ")
    }


// -----------------------------------------------------------------------------
// CMceAudioSdpCodec::CreateStreamLC
// -----------------------------------------------------------------------------
//
CMceComMediaStream* CMceAudioSdpCodec::CreateStreamLC( TInt aType ) const
    {
    MCEMM_DEBUG( "CMceAudioSdpCodec::CreateStreamLC(), Entry" )
    CMceComAudioStream* audiostream = NULL;
            
    if( aType == SdpCodecStringConstants::EAttributeSendonly )
        {
        // downlink
        MCEMM_DEBUG( "create downlink" )
        audiostream = CreateDownlinkStreamL();
        }
    else if( aType == SdpCodecStringConstants::EAttributeRecvonly )        
        {
        // uplink 
        MCEMM_DEBUG( "create uplink" )
        audiostream = CreateUplinkStreamL();
        }
    else
        {
        // sendrcv 
        // create two way stream
        // create downlink
        MCEMM_DEBUG( "create two-way stream" )
        audiostream = CreateDownlinkStreamL();
        CleanupStack::PushL( audiostream );
        // create bounded uplink
        CMceComAudioStream *boundUplink = CreateUplinkStreamL();
        CleanupStack::PushL( boundUplink );
        audiostream->BindL( boundUplink );
        CleanupStack::Pop( boundUplink ); 
        CleanupStack::Pop( audiostream );
        }
    
    CleanupStack::PushL( audiostream );
    
    MCEMM_DEBUG( "CMceAudioSdpCodec::CreateStreamLC(), Exit" )
    return audiostream;
    
    }


// -----------------------------------------------------------------------------
// CMceAudioSdpCodec::UpdateStreamL
// -----------------------------------------------------------------------------
//
void CMceAudioSdpCodec::UpdateStreamL( CMceComMediaStream& aStream, TInt aDirection ) const
    {
    MCEMM_DEBUG( "CMceAudioSdpCodec::UpdateStreamL(), Entry ")
    MCE_DEFINE_AUDIO( stream, aStream );

    CMceComAudioStream* audiostream = NULL;
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
                    MCEMM_DEBUG( "update from recvonly to send" )
                    MCEMM_DEBUG( "create uplink" )
                    audiostream = CreateUplinkStreamL();
                    }
                break;
                }
            case SdpCodecStringConstants::EAttributeSendonly:
                {
                //if direction is changed
                if ( aDirection != SdpCodecStringConstants::EAttributeRecvonly )
                    {
                    MCEMM_DEBUG( "update from sendonly to recv" )
                    MCEMM_DEBUG( "create downlink" )
                    audiostream = CreateDownlinkStreamL();
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
        
    if ( audiostream )
        {
        CleanupStack::PushL( audiostream );

        MCEMM_DEBUG( "cloning codecs" )
        
        for( TInt index = 0; index < stream.CodecCount(); ++index )
            {
            CMceComAudioCodec* codec = 
                static_cast<CMceComAudioCodec*>( stream.CodecL( index )->CloneL() );
            CleanupStack::PushL( codec );
            
            MCEMM_DEBUG_SVALUE( "cloned codec", codec->iSdpName )
            
            audiostream->AddCodecL( codec );
            CleanupStack::Pop( codec );
            }

        stream.BindL( audiostream );
        audiostream->InitializeL( *aStream.Session() );
        
        MCEMM_DEBUG( "binded stream created" )
        CleanupStack::Pop( audiostream ); 
        }
    
    MCEMM_DEBUG( "CMceAudioSdpCodec::UpdateStreamL(), Exit" )
    }

// -----------------------------------------------------------------------------
// CMceAudioSdpCodec::CreateCodecLC
// -----------------------------------------------------------------------------
//
CMceComCodec* CMceAudioSdpCodec::CreateCodecLC( CSdpFmtAttributeField& aRtpmap ) const
    {
    MCEMM_DEBUG( "CMceAudioSdpCodec::CreateCodecLC(), Entry" )
    
    CMceComAudioCodec* codec = NULL;
    
    const TDesC8& payloadType = aRtpmap.Format();
    const TDesC8& attributeValue = aRtpmap.Value();
    TSdpRtpmapValue rtpmapValue = TSdpRtpmapValue::DecodeL( attributeValue );

    TPtrC8 codecName = rtpmapValue.iEncName;
    TPtrC8 clockRate = rtpmapValue.iClockrate;
    
    TUint uintValPT = ConvertDesToUintL( payloadType );
    TUint uintValCR = ConvertDesToUintL( clockRate );

    if ( uintValPT > KMceMaxPTValue )
        {
		MCEMM_DEBUG_DVALUE("CMceAudioSdpCodec::CreateCodecLC(), invalid payloadtype value ", uintValPT )
		User::Leave( KErrNotSupported );        	
        }
    if ( IsSupported( codecName ) && IsPayloadTypeSupported( codecName, uintValPT ) )
        {
        TMceComAudioCodecFactory factory;
        codec =  factory.CreateLC( codecName );
        codec->iSamplingFreq = uintValCR;
        codec->iPayloadType = uintValPT;
        MCEMM_DEBUG_SVALUE( "created codec", codecName )
        MCEMM_DEBUG_DVALUE( "sampling frequence", uintValCR )
        MCEMM_DEBUG_DVALUE( "payload", uintValPT )
        }
    else
        {
        MCEMM_DEBUG_SVALUE( "not supported codec", codecName )
        }
        
    MCEMM_DEBUG( "CMceAudioSdpCodec::CreateCodecLC(), Exit" )
    
    return codec;
    }


// -----------------------------------------------------------------------------
// CMceAudioSdpCodec::CreateCodecLC
// -----------------------------------------------------------------------------
//
CMceComCodec* CMceAudioSdpCodec::CreateCodecLC( TUint aPayload, 
                                                CSdpMediaField& /*aMediaLine*/ ) const
    {
    MCEMM_DEBUG( "CMceAudioSdpCodec::CreateCodecLC( payload ), Entry" )
    
    TMceComAudioCodecFactory factory;
    CMceComAudioCodec* codec = NULL;
    
    codec = factory.CreateCodecLC( aPayload );
    if ( codec && !IsSupported( codec->iSdpName ) )
        {
        CleanupStack::PopAndDestroy( codec );
        codec = NULL;
        }

    MCEMM_DEBUG( "CMceAudioSdpCodec::CreateCodecLC( payload ), Exit" )
    
    return codec;
    }

    
// -----------------------------------------------------------------------------
// CMceAudioSdpCodec::DecodeSessionMediaAttributesL
// -----------------------------------------------------------------------------
//
void CMceAudioSdpCodec::DecodeSessionMediaAttributesL( CMceComMediaStream& aStream, 
                                                       CSdpDocument& aSdpDocument ) const
    {
    MCEMM_DEBUG( "CMceAudioSdpCodec::DecodeSessionMediaAttributesL(), Entry" )
    MCE_DEFINE_AUDIO( stream, aStream );
    
    if ( stream.SendStream() )
        {
        RPointerArray<CSdpAttributeField>& attrfields = 
                                                aSdpDocument.AttributeFields();

        // look for the ptime and maxptime attributes, go through all
        // attribute fields and apply to all codecs in the stream if found
        for ( TInt index = 0; index < attrfields.Count(); index++ )
            {
            CSdpAttributeField* attrfield = attrfields[ index ];
            RStringF attribute = attrfield->Attribute();
            TInt j = 0;
            if ( attribute == SDP_STRING( SdpCodecStringConstants::EAttributePtime ) )
                {
                MCEMM_DEBUG_SVALUE( "decoded ptime", attrfield->Value() )

                for ( j = 0; j < stream.CodecCount(); j++ )
                    {
                    stream.CodecL( j )->iPTime = ConvertDesToUintL( attrfield->Value() );
                    }
                }
            else if ( attribute == SDP_STRING( SdpCodecStringConstants::EAttributeMaxptime ) )
                {
                MCEMM_DEBUG_SVALUE( "decoded maxptime", attrfield->Value() )

                for ( j = 0; j < stream.CodecCount(); j++ )
                    {
                    stream.CodecL( j )->iMaxPTime = ConvertDesToUintL( attrfield->Value() );
                    }   
                }
            }
        }
        
    MCEMM_DEBUG( "CMceAudioSdpCodec::DecodeSessionMediaAttributesL(), Exit" )
    }
    
// -----------------------------------------------------------------------------
// CMceAudioSdpCodec::CreateUplinkStreamL
// -----------------------------------------------------------------------------
//
CMceComAudioStream* CMceAudioSdpCodec::CreateUplinkStreamL() const
    {
    MCEMM_DEBUG( "CMceAudioSdpCodec::CreateUplinkStreamL(), Entry" )
    
    CMceComAudioStream *audiostream = CMceComAudioStream::NewL();
    CleanupStack::PushL( audiostream );
    // uplink           
    CMceComMicSource* micSource = CMceComMicSource::NewL();
    CleanupStack::PushL( micSource );
    micSource->InitializeL( static_cast<CMceComAudioStream&> ( *audiostream ) );
    audiostream->SetSourceL( micSource );
    CleanupStack::Pop( micSource );
    CMceComRtpSink* rtpSink = CMceComRtpSink::NewL();
    CleanupStack::PushL( rtpSink );
    rtpSink->InitializeL( static_cast<CMceComAudioStream&> ( *audiostream ) );
    audiostream->AddSinkL( rtpSink );
    CleanupStack::Pop( rtpSink ); 
    CleanupStack::Pop( audiostream );
    
    MCEMM_DEBUG( "CMceAudioSdpCodec::CreateUplinkStreamL(), Exit" )
    
    return audiostream;
    }

// -----------------------------------------------------------------------------
// CMceAudioSdpCodec::CreateDownlinkStreamL
// -----------------------------------------------------------------------------
//
CMceComAudioStream* CMceAudioSdpCodec::CreateDownlinkStreamL() const
    {
    MCEMM_DEBUG( "CMceAudioSdpCodec::CreateDownlinkStreamL(), Entry" )

    CMceComAudioStream *audiostream = CMceComAudioStream::NewL();
    CleanupStack::PushL( audiostream ); 
    // downlink         
    CMceComRtpSource* rtpSource = CMceComRtpSource::NewL();
    CleanupStack::PushL( rtpSource );
    rtpSource->InitializeL( static_cast<CMceComAudioStream&> ( *audiostream ) );
    audiostream->SetSourceL( rtpSource );
    CleanupStack::Pop( rtpSource );
    CMceComSpeakerSink* speaker = CMceComSpeakerSink::NewL();
    CleanupStack::PushL( speaker );
    speaker->InitializeL( static_cast<CMceComAudioStream&> ( *audiostream ) );
    audiostream->AddSinkL( speaker );
    CleanupStack::Pop( speaker ); 
    CleanupStack::Pop( audiostream );
    
    MCEMM_DEBUG( "CMceAudioSdpCodec::CreateDownlinkStreamL(), Exit" )
    
    return audiostream;
    }


