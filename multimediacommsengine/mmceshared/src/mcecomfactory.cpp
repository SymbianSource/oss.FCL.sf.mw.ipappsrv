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




#include <s32buf.h>

#include "mceaudiostream.h"
#include "mcevideostream.h"
#include "mcertpsource.h"
#include "mcemicsource.h"
#include "mcecamerasource.h"
#include "mcefilesource.h"
#include "mcefilesink.h"
#include "mcertpsink.h"
#include "mcespeakersink.h"
#include "mcedisplaysink.h"
#include "mceavsink.h"
#include "mcecomfactory.h"
#include "mcecomsession.h"
#include "mcecomaudiostream.h"
#include "mcecomvideostream.h"
#include "mcecomrtpsource.h"
#include "mcecommicsource.h"
#include "mcecomcamerasource.h"
#include "mcecomfilesource.h"
#include "mcecomfilesink.h"
#include "mcecomrtpsink.h"
#include "mcecomspeakersink.h"
#include "mcecomdisplaysink.h"
#include "mcecomavsink.h"
#include "mcecomaudiocodec.h"
#include "mcecomamrcodec.h"
#include "mcecomg711codec.h"
#include "mceg711codec.h"
#include "mcecomg729codec.h"
#include "mceg729codec.h"
#include "mcecncodec.h"
#include "mcecomilbccodec.h"   
#include "mcecomdtmfcodec.h"
#include "mcecomcncodec.h"
#include "mcecomredcodec.h"
#include "mcecomvideocodec.h"
#include "mcecomh263codec.h"
#include "mcecomstreambundle.h"
#include "mcecomavccodec.h"
#include "mcecomamrwbcodec.h"
#include "mceserial.h"



// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// TMceComFactory::CreateLC
// -----------------------------------------------------------------------------
//
CMceComSession* TMceComFactory::CreateLC( MMceComSerializationContext& aSerCtx )
    {
    return CMceComSession::NewLC( aSerCtx );
    }


// -----------------------------------------------------------------------------
// TMceComFactory::CreateL
// -----------------------------------------------------------------------------
//
CMceComStreamBundle* TMceComFactory::CreateBundleLC( CMceComSession& aSession, 
                                                     MMceComSerializationContext& aSerCtx )
    {
    RReadStream& readStream = aSerCtx.ReadStream();
    
    MStreamBuf* streamBuf = readStream.Source();
    TStreamPos pos = streamBuf->TellL( MStreamBuf::ERead );
    CMceStreamBundle::TMceStreamBundleType type =
    	static_cast<CMceStreamBundle::TMceStreamBundleType>( readStream.ReadUint8L() );
    streamBuf->SeekL( MStreamBuf::ERead, pos );
    
    CMceComStreamBundle* bundle = CMceComStreamBundle::NewLC( type );
    bundle->InitializeL( aSession );
    bundle->InternalizeL( aSerCtx );
    
    return bundle;
    
    }

// -----------------------------------------------------------------------------
// TMceComFactory::CreateL
// -----------------------------------------------------------------------------
//
CMceMsgBase* TMceComFactory::CreateL( TMceItcDataType aType,
                                      RReadStream& aReadStream )
    {
    MMceComSerializationContext serCtx( aReadStream );
    CleanupClosePushL( serCtx );
    
    CMceMsgBase* object = CreateL( aType, serCtx );
    
    CleanupStack::PopAndDestroy();//serCtx
    
    return object;
    
    }
    
// -----------------------------------------------------------------------------
// TMceComFactory::CreateL
// -----------------------------------------------------------------------------
//
CMceMsgBase* TMceComFactory::CreateL( TMceItcDataType aType,
                                      MMceComSerializationContext& aSerCtx )
    {
    
    CMceMsgBase* message = NULL;
    switch ( aType )
        {
        case EMceItcMsgTypeSession:
            {
            message = new (ELeave) CMceMsgObject<CMceComSession>();
        	break;
            }
        case EMceItcMsgTypeStream:
            {
            message = new (ELeave) CMceMsgObject<CMceComMediaStream>();
        	break;
            }
        case EMceItcMsgTypeCodec:
            {
            message = new (ELeave) CMceMsgObject<CMceComAudioCodec>();
        	break;
            }
        case EMceItcMsgTypeSink:
            {
            message = new (ELeave) CMceMsgObject<CMceComMediaSink>();
        	break;
            }
        case EMceItcMsgTypeSource:
            {
            message = new (ELeave) CMceMsgObject<CMceComMediaSource>();
        	break;
            }
        case EMceItcMsgTypeSIPData:
            {
            message = new (ELeave) CMceMsgSIPData();
        	break;
            }
        case EMceItcMsgTypeSIPReply:
            {
            message = new (ELeave) CMceMsgSIPReply();
        	break;
            }
       	case EMceItcMsgTypeSIPRequest:
            {
            message = new (ELeave) CMceMsgSIPRequest();
        	break;
            }    
            
        case EMceItcMsgTypeEvent:
            {
            message = new (ELeave) CMceMsgSIPEvent();
        	break;
            }
        case EMceItcMsgTypeTextArray:
            {
            message = new (ELeave) CMceMsgTextArray();
        	break;
            }
        case EMceItcMsgTypeMessageArray:
            {
            message = new (ELeave) CMceMsgArray<TMceComFactory>( *this );
        	break;
            }
	    default:
	        {
	        }
        }
        
    if ( message )
        {
    	CleanupStack::PushL( message );
    	message->PushL();
        message->DoDecodeL( aSerCtx );
    	message->Pop();
        CleanupStack::Pop( message );
        }

    return message;
            
    }

// -----------------------------------------------------------------------------
// TMceComFactory::CreateLC
// -----------------------------------------------------------------------------
//
void TMceComFactory::CreateLC( MMceComSerializable*& aObject, MMceComSerializationContext& aSerCtx )
    {

    RReadStream& readStream = aSerCtx.ReadStream();
    
    MStreamBuf* streamBuf = readStream.Source();
    TStreamPos pos = streamBuf->TellL( MStreamBuf::ERead );
    TUint type = readStream.ReadUint8L();
    streamBuf->SeekL( MStreamBuf::ERead, pos );
    
    switch ( type )
        {
        case KMceAvSink:
            {
            aObject = CMceComAvSink::NewLC();
            break;
            }
        default:
            {
            break;
            }
        }
    
    if ( !aObject )
        {
        User::Leave( KErrNotSupported );
        }
        
    aObject->InternalizeL( aSerCtx );
    
    }



// -----------------------------------------------------------------------------
// TMceComMediaStreamFactory::CreateLC
// -----------------------------------------------------------------------------
//
CMceComMediaStream* TMceComMediaStreamFactory::CreateLC( TMceMediaType aType )
	{
    
    CMceComMediaStream* stream = NULL;
    switch ( aType )
        {
        case KMceAudio:
            {
            stream = CMceComAudioStream::NewLC();
            break;
            }
        case KMceVideo:
            {
            stream = CMceComVideoStream::NewLC();
            break;
            }
        default:
            {
            break;
            }
        }
    
    if ( !stream )
        {
        User::Leave( KErrNotSupported );
        }
        
    return stream;
    
	}
	
	

// -----------------------------------------------------------------------------
// TMceComMediaStreamFactory::CreateLC
// -----------------------------------------------------------------------------
//
CMceComMediaStream* TMceComMediaStreamFactory::CreateLC( 
                                        MMceComSerializationContext& aSerCtx )
    {
    RReadStream& readStream = aSerCtx.ReadStream();
    
    MStreamBuf* streamBuf = readStream.Source();
    TStreamPos pos = streamBuf->TellL( MStreamBuf::ERead );
    TMceMediaType type = static_cast<TMceMediaType>( readStream.ReadUint8L() );
    streamBuf->SeekL( MStreamBuf::ERead, pos );
    
    CMceComMediaStream* stream = CreateLC( type );
    stream->InternalizeL( aSerCtx );
    return stream;
    
    }

	
// -----------------------------------------------------------------------------
// TMceComSourceFactory::CreateLC
// -----------------------------------------------------------------------------
//
CMceComMediaSource* TMceComSourceFactory::CreateLC( TMceSourceType aType )
    {
    CMceComMediaSource* source = NULL;

    switch ( aType )
        {
        case KMceRTPSource:
            {
            source = CMceComRtpSource::NewLC();
            break;
            }
        case KMceMicSource:
            {
            source = CMceComMicSource::NewLC();
            break;
            }
        case KMceCameraSource:
            {
            source = CMceComCameraSource::NewLC();
            break;
            }
        case KMceFileSource:
            {
            source = CMceComFileSource::NewLC();
            break;
            }
        default:
            {
            break;
            }
        }

    if ( !source )
        {
        User::Leave( KErrNotSupported );
        }
        
    return source;
    
    }


// -----------------------------------------------------------------------------
// TMceComSourceFactory::CreateLC
// -----------------------------------------------------------------------------
//
CMceComMediaSource* TMceComSourceFactory::CreateLC( MMceComSerializationContext& aSerCtx )
    {
    RReadStream& readStream = aSerCtx.ReadStream();
    
    MStreamBuf* streamBuf = readStream.Source();
    TStreamPos pos = streamBuf->TellL( MStreamBuf::ERead );
    TMceSourceType type = static_cast<TMceSourceType>( readStream.ReadUint8L() );
    streamBuf->SeekL( MStreamBuf::ERead, pos );

    CMceComMediaSource* source = CreateLC( type );
    source->InternalizeL( aSerCtx );
    return source;
    
    }

// -----------------------------------------------------------------------------
// TMceComSinkFactory::CreateLC
// -----------------------------------------------------------------------------
//
CMceComMediaSink* TMceComSinkFactory::CreateLC( TMceSinkType aType )
    {
    CMceComMediaSink* sink = NULL;
    
    switch ( aType )
        {
        case KMceRTPSink:
            {
            sink = CMceComRtpSink::NewLC();
            break;
            }
        case KMceSpeakerSink:
            {
            sink = CMceComSpeakerSink::NewLC();
            break;
            }
        case KMceDisplaySink:
            {
            sink = CMceComDisplaySink::NewLC();
            break;
            }
        case KMceFileSink:
            {
            sink = CMceComFileSink::NewLC();
            break;
            }
        default:
            {
            break;
            }
        }

    if ( !sink )
        {
        User::Leave( KErrNotSupported );
        }
        
    return sink;
    
    }

// -----------------------------------------------------------------------------
// TMceComSinkFactory::CreateLC
// -----------------------------------------------------------------------------
//
CMceComMediaSink* TMceComSinkFactory::CreateLC( MMceComSerializationContext& aSerCtx )
    {
    RReadStream& readStream = aSerCtx.ReadStream();
    
    MStreamBuf* streamBuf = readStream.Source();
    TStreamPos pos = streamBuf->TellL( MStreamBuf::ERead );
    TMceSinkType type = static_cast<TMceSinkType>( readStream.ReadUint8L() );
    streamBuf->SeekL( MStreamBuf::ERead, pos );
    
    CMceComMediaSink* sink = CreateLC( type );
    sink->InternalizeL( aSerCtx );
    return sink;
    
    }

// -----------------------------------------------------------------------------
// TMceComAudioCodecFactory::CreateLC 
// -----------------------------------------------------------------------------
//
CMceComAudioCodec* TMceComAudioCodecFactory::CreateLC( const TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComAudioCodec* codec = CreateCodecLC( aSdpName );
    __ASSERT_ALWAYS( codec, User::Leave( KErrNotSupported ) );
    
    return codec;
    
    }

// -----------------------------------------------------------------------------
// TMceComAudioCodecFactory::CreateCodecLC 
// -----------------------------------------------------------------------------
//
CMceComAudioCodec* TMceComAudioCodecFactory::CreateCodecLC( const TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComAudioCodec* codec = NULL;
    
    if ( !aSdpName.CompareF(KMceSDPNameAMR) )
        {
        codec = CMceComAMRCodec::NewLC( aSdpName );
        }
    else if ( !aSdpName.CompareF(KMceSDPNamePCMU) ||
              !aSdpName.CompareF(KMceSDPNamePCMA) )
        {
        codec = CMceComG711Codec::NewLC( aSdpName );
        }
    else if ( !aSdpName.CompareF(KMceSDPNameG729) )    
        {
        codec = CMceComG729Codec::NewLC( aSdpName );
        }
    else if ( !aSdpName.CompareF(KMceSDPNameiLBC) )
        {
        codec = CMceComiLBCCodec::NewLC( aSdpName );    
        }
    else if ( !aSdpName.CompareF(KMceSDPNameDtmf) )
        {
        codec = CMceComDtmfCodec::NewLC( aSdpName );    
        }
    else if ( !aSdpName.CompareF(KMceSDPNameRed) )
        {
        codec = CMceComRedCodec::NewLC( aSdpName );    
        }
    else if ( !aSdpName.CompareF(KMceSDPNameCn) )
        {
        codec = CMceComCnCodec::NewLC( aSdpName );    
        }
        
    else if ( !aSdpName.CompareF(KMceSDPNameAMRWB) )
        {
        codec = CMceComAMRWbCodec::NewLC( aSdpName );    
        }
        
    else
        {
        codec = NULL;
        }
    
    return codec;
    
    }

// -----------------------------------------------------------------------------
// TMceComAudioCodecFactory::CreateCodecLC 
// -----------------------------------------------------------------------------
//
CMceComAudioCodec* TMceComAudioCodecFactory::CreateCodecLC( TUint aPayload )
    {
    CMceComAudioCodec* codec = NULL;
    
    switch( aPayload )
        {
        case KMcePCMUPayloadType:
            {
            codec = CMceComG711Codec::NewLC( KMceSDPNamePCMU() );
            break;
            }
        case KMcePCMAPayloadType:
            {
            codec = CMceComG711Codec::NewLC( KMceSDPNamePCMA() );
            break;
            }
        case KMceDefaultG729PayloadType:
            {
            codec = CMceComG729Codec::NewLC( KMceSDPNameG729() );
            break;
            }
        case KMceDefaultCnPayloadType:
        case KMceDefaultCnPayloadType2:
            {
            codec = CMceComCnCodec::NewLC( KMceSDPNameCn() );
            break;
            }
        default:
            {
            //NOP
            break;
            }
        }
    
    if ( codec )
        {
        codec->iPayloadType = aPayload;
        }
        
    return codec;
    
    }



// -----------------------------------------------------------------------------
// TMceComAudioCodecFactory::CreateLC
// -----------------------------------------------------------------------------
//
CMceComAudioCodec* TMceComAudioCodecFactory::CreateLC( MMceComSerializationContext& aSerCtx )
    {
    RReadStream& readStream = aSerCtx.ReadStream();
    
    MStreamBuf* streamBuf = readStream.Source();
    TStreamPos pos = streamBuf->TellL( MStreamBuf::ERead );
    TBuf8<KMceMaxSdpNameLength> sdpName;
    MceSerial::DecodeL( sdpName, readStream );
    streamBuf->SeekL( MStreamBuf::ERead, pos );
    
    CMceComAudioCodec* codec = CreateLC( sdpName );
    codec->InternalizeL( aSerCtx );
    return codec;
    
    }


// -----------------------------------------------------------------------------
// TMceComVideoCodecFactory::CreateLC
// -----------------------------------------------------------------------------
//
CMceComVideoCodec* TMceComVideoCodecFactory::CreateLC( const TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComVideoCodec* codec = CreateCodecLC( aSdpName );
    __ASSERT_ALWAYS( codec, User::Leave( KErrNotSupported ) );
    
    return codec;
    }


// -----------------------------------------------------------------------------
// TMceComVideoCodecFactory::CreateCodecLC
// -----------------------------------------------------------------------------
//

CMceComVideoCodec* TMceComVideoCodecFactory::CreateCodecLC( const TBuf8<KMceMaxSdpNameLength> aSdpName )

    {
    CMceComVideoCodec* codec = NULL;
    
    if ( aSdpName.CompareF(KMceSDPNameH263) == 0 
         || aSdpName.CompareF(KMceSDPNameH2632000) == 0
         || aSdpName.CompareF(KMceSDPNameH2631998) == 0 )
        {
        codec = CMceComH263Codec::NewLC( aSdpName );
        }
    else if ( aSdpName.CompareF( KMceSDPNameH264 ) == 0 )
        {
        codec = CMceComAvcCodec::NewLC( aSdpName );
        }
    else
        {
        codec = NULL;
        }
    
    return codec;    
    }

// -----------------------------------------------------------------------------
// TMceComVideoCodecFactory::CreateCodecLC 
// -----------------------------------------------------------------------------
//
CMceComVideoCodec* TMceComVideoCodecFactory::CreateCodecLC( TUint /*aPayload*/ )
    {
    CMceComVideoCodec* codec = NULL;
    return codec;
    
    }


// -----------------------------------------------------------------------------
// TMceComVideoCodecFactory::CreateLC
// -----------------------------------------------------------------------------
//
CMceComVideoCodec* TMceComVideoCodecFactory::CreateLC( MMceComSerializationContext& aSerCtx )
    {
    RReadStream& readStream = aSerCtx.ReadStream();
    
    MStreamBuf* streamBuf = readStream.Source();
    TStreamPos pos = streamBuf->TellL( MStreamBuf::ERead );
    TBuf8<KMceMaxSdpNameLength> sdpName;
    MceSerial::DecodeL( sdpName, readStream );
    streamBuf->SeekL( MStreamBuf::ERead, pos );
    
    CMceComVideoCodec* codec = CreateLC( sdpName );
    codec->InternalizeL( aSerCtx );
    return codec;
    
    }
