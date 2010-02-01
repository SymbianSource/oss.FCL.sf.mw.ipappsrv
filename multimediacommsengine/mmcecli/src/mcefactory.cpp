/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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

#include "mceinsession.h"
#include "mceoutsession.h"
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
#include "mceamrcodec.h"
#include "mceamrwbcodec.h"
#include "mceg711codec.h"
#include "mceg729codec.h"
#include "mceilbccodec.h"
#include "mcedtmfcodec.h"
#include "mcecncodec.h"
#include "mceredcodec.h"
#include "mceh263codec.h"
#include "mcefactory.h"
#include "mceserial.h"
#include "mcedefs.h"
#include "mcevideocodec.h"
#include "mcestreambundle.h"
#include "mceavsink.h"
#include "mceavccodec.h"

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// TMceFactory::CreateLC
// -----------------------------------------------------------------------------
//
CMceSession* TMceFactory::CreateLC( CMceComSession::TType aType )
	{
    CMceSession* session = NULL;
    switch ( aType )
        {
        case CMceComSession::EInSession:
            {
            session = CMceInSession::NewL();
            CleanupStack::PushL( session );
            break;
            }
        case CMceComSession::EOutSession:
            {
            session = CMceOutSession::NewL();
            CleanupStack::PushL( session );
            break;
            }
        default:
            {
            break;
            }
        }

    if ( !session )
        {
        User::Leave( KErrNotSupported );
        }
        
    return session;    
	}

// -----------------------------------------------------------------------------
// TMceFactory::CreateLC
// -----------------------------------------------------------------------------
//
CMceSession* TMceFactory::CreateLC( MMceComSerializationContext& aSerCtx )
    {
    RReadStream& readStream = aSerCtx.ReadStream();
    
    MStreamBuf* streamBuf = readStream.Source();
    TStreamPos pos = streamBuf->TellL( MStreamBuf::ERead );
    CMceComSession::TType type =
    	static_cast<CMceComSession::TType>( readStream.ReadUint8L() );
    streamBuf->SeekL( MStreamBuf::ERead, pos );
    
    CMceSession* session = CreateLC( type );
    session->InternalizeL( aSerCtx );
    return session;    
    }


// -----------------------------------------------------------------------------
// TMceFactory::CreateL
// -----------------------------------------------------------------------------
//
CMceMsgBase* TMceFactory::CreateL( TMceItcDataType aType,
                                   RReadStream& aReadStream )
    {
    MMceComSerializationContext serCtx( aReadStream );
    CleanupClosePushL( serCtx );
    
    CMceMsgBase* object = CreateL( aType, serCtx );
    
    CleanupStack::PopAndDestroy();//serCtx
    
    return object;
    
    }


// -----------------------------------------------------------------------------
// TMceFactory::CreateL
// -----------------------------------------------------------------------------
//
CMceMsgBase* TMceFactory::CreateL( TMceItcDataType aType,
                                   MMceComSerializationContext& aSerCtx )
    {
    
    CMceMsgBase* message = NULL;
    switch ( aType )
        {
        case EMceItcMsgTypeSession:
            {
            message = new (ELeave) CMceMsgObject<CMceSession>();
        	break;
            }
        case EMceItcMsgTypeStream:
            {
            message = new (ELeave) CMceMsgObject<CMceMediaStream>();
        	break;
            }
        case EMceItcMsgTypeCodec:
            {
            message = new (ELeave) CMceMsgObject<CMceAudioCodec>();
        	break;
            }
        case EMceItcMsgTypeSink:
            {
            message = new (ELeave) CMceMsgObject<CMceMediaSink>();
        	break;
            }
        case EMceItcMsgTypeSource:
            {
            message = new (ELeave) CMceMsgObject<CMceMediaSource>();
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
            message = new (ELeave) CMceMsgArray<TMceFactory>( *this );
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
// TMceFactory::CreateL
// -----------------------------------------------------------------------------
//
CMceStreamBundle* TMceFactory::CreateBundleLC( CMceSession& aSession, 
                                               MMceComSerializationContext& aSerCtx )
    {
    RReadStream& readStream = aSerCtx.ReadStream();
    
    MStreamBuf* streamBuf = readStream.Source();
    TStreamPos pos = streamBuf->TellL( MStreamBuf::ERead );
    CMceStreamBundle::TMceStreamBundleType type =
    	static_cast<CMceStreamBundle::TMceStreamBundleType>( readStream.ReadUint8L() );
    streamBuf->SeekL( MStreamBuf::ERead, pos );
    
    CMceStreamBundle* bundle = CMceStreamBundle::NewLC( type );
    bundle->InitializeL( aSession );
    bundle->InternalizeL( aSerCtx );
    
    return bundle;
    
    }


// -----------------------------------------------------------------------------
// TMceMediaStreamFactory::CreateLC
// -----------------------------------------------------------------------------
//
CMceMediaStream* TMceMediaStreamFactory::CreateLC( TMceMediaType aType )
	{
	
    CMceMediaStream* stream = NULL;
    switch ( aType )
        {
        case KMceAudio:
            {
            stream = CMceAudioStream::NewLC();
            break;
            }
        case KMceVideo:
            {
            stream = CMceVideoStream::NewLC();
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
// TMceMediaStreamFactory::CreateLC
// -----------------------------------------------------------------------------
//
CMceMediaStream* TMceMediaStreamFactory::CreateLC(
                                        MMceComSerializationContext& aSerCtx )
    {
    RReadStream& readStream = aSerCtx.ReadStream();
    
    MStreamBuf* streamBuf = readStream.Source();
    TStreamPos pos = streamBuf->TellL( MStreamBuf::ERead );
    TMceMediaType type = static_cast<TMceMediaType>( readStream.ReadUint8L() );
    streamBuf->SeekL( MStreamBuf::ERead, pos );
    
    CMceMediaStream* stream = CreateLC( type );
    stream->InternalizeL( aSerCtx );
    return stream;
    
    }



	
// -----------------------------------------------------------------------------
// TMceSourceFactory::CreateLC
// -----------------------------------------------------------------------------
//
CMceMediaSource* TMceSourceFactory::CreateLC( TMceSourceType aType )
    {
    CMceMediaSource* source = NULL;

    switch ( aType )
        {
        case KMceRTPSource:
            {
            source = CMceRtpSource::NewLC();
            break;
            }
        case KMceMicSource:
            {
            source = CMceMicSource::NewLC();
            break;
            }
        case KMceCameraSource:
            {
            source = CMceCameraSource::NewLC();
            break;
            }
        case KMceFileSource:
            {
            source = CMceFileSource::NewLC();
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
// TMceSourceFactory::CreateLC
// -----------------------------------------------------------------------------
//
CMceMediaSource* TMceSourceFactory::CreateLC( MMceComSerializationContext& aSerCtx )
    {
    RReadStream& readStream = aSerCtx.ReadStream();
    
    MStreamBuf* streamBuf = readStream.Source();
    TStreamPos pos = streamBuf->TellL( MStreamBuf::ERead );
    TMceSourceType type = static_cast<TMceSourceType>( readStream.ReadUint8L() );
    streamBuf->SeekL( MStreamBuf::ERead, pos );

    CMceMediaSource* source = CreateLC( type );
    source->InternalizeL( aSerCtx );
    return source;
    
    }

// -----------------------------------------------------------------------------
// TMceSinkFactory::CreateLC
// -----------------------------------------------------------------------------
//
CMceMediaSink* TMceSinkFactory::CreateLC( TMceSinkType aType )
    {
    CMceMediaSink* sink = NULL;
    switch ( aType )
        {
        case KMceRTPSink:
            {
            sink = CMceRtpSink::NewLC();
            break;
            }
        case KMceSpeakerSink:
            {
            sink = CMceSpeakerSink::NewLC();
            break;
            }
        case KMceDisplaySink:
            {
            sink = CMceDisplaySink::NewLC();
            break;
            }
        case KMceFileSink:
            {
            sink = CMceFileSink::NewLC();
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
// TMceSinkFactory::CreateLC
// -----------------------------------------------------------------------------
//
CMceMediaSink* TMceSinkFactory::CreateLC( MMceComSerializationContext& aSerCtx )
    {
    RReadStream& readStream = aSerCtx.ReadStream();
    
    MStreamBuf* streamBuf = readStream.Source();
    TStreamPos pos = streamBuf->TellL( MStreamBuf::ERead );
    TMceSinkType type = static_cast<TMceSinkType>( readStream.ReadUint8L() );
    streamBuf->SeekL( MStreamBuf::ERead, pos );
    
    CMceMediaSink* sink = CreateLC( type );
    sink->InternalizeL( aSerCtx );
    return sink;
    
    }
    
// -----------------------------------------------------------------------------
// TMceSinkFactory::CreateLC
// -----------------------------------------------------------------------------
//
void TMceSinkFactory::CreateLC( MMceComSerializable*& aSink, MMceComSerializationContext& aSerCtx )
    {

    RReadStream& readStream = aSerCtx.ReadStream();
    
    MStreamBuf* streamBuf = readStream.Source();
    TStreamPos pos = streamBuf->TellL( MStreamBuf::ERead );
    TMceSinkType type = static_cast<TMceSinkType>( readStream.ReadUint8L() );
    streamBuf->SeekL( MStreamBuf::ERead, pos );
    
    switch ( type )
        {
        case KMceAvSink:
            {
            aSink = CMceAvSink::NewLC();
            break;
            }
        default:
            {
            break;
            }
        }
    
    if ( !aSink )
        {
        User::Leave( KErrNotSupported );
        }
        
    aSink->InternalizeL( aSerCtx );
    
    }

    

// -----------------------------------------------------------------------------
// TMceAudioCodecFactory::CreateLC
// -----------------------------------------------------------------------------
//
CMceAudioCodec* TMceAudioCodecFactory::CreateLC( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceAudioCodec* codec = CreateL( aSdpName );
    CleanupStack::PushL( codec );
	return codec;     
    }
    
// -----------------------------------------------------------------------------
// TMceAudioCodecFactory::CreateL
// -----------------------------------------------------------------------------
//
CMceAudioCodec* TMceAudioCodecFactory::CreateL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceAudioCodec* codec = NULL;
    if( !aSdpName.CompareF( KMceSDPNameAMR ) )
        {
        codec = CMceAmrCodec::NewL( aSdpName );
        }
    else if ( !aSdpName.CompareF( KMceSDPNameAMRWB ) )
    	{
		codec = CMceAmrWbCodec::NewL( aSdpName );
    	}
    else if ( !aSdpName.CompareF( KMceSDPNamePCMU ) ||
              !aSdpName.CompareF( KMceSDPNamePCMA ) )
        {
        codec = CMceG711Codec::NewL( aSdpName );
        }
    else if ( !aSdpName.CompareF( KMceSDPNameG729 ) )        
        {
        codec = CMceG729Codec::NewL( aSdpName );   
        }     
    else if ( !aSdpName.CompareF( KMceSDPNameiLBC ) )        
        {
        codec = CMceiLBCCodec::NewL( aSdpName );
        }
    else if ( !aSdpName.CompareF( KMceSDPNameDtmf ) )
        {
        codec = CMceDtmfCodec::NewL( aSdpName );
        }
    else if ( !aSdpName.CompareF( KMceSDPNameCn ) )
        {
        codec = CMceCnCodec::NewL( aSdpName );
        }
    else if ( !aSdpName.CompareF( KMceSDPNameRed ) )
        {
        codec = CMceRedCodec::NewL( aSdpName );
        }
    else
        {
        User::Leave( KErrNotSupported );
        }
	return codec;
    }

// -----------------------------------------------------------------------------
// TMceAudioCodecFactory::CreateLC
// -----------------------------------------------------------------------------
//
CMceAudioCodec* TMceAudioCodecFactory::CreateLC( MMceComSerializationContext& aSerCtx )
    {
    RReadStream& readStream = aSerCtx.ReadStream();
    
    MStreamBuf* streamBuf = readStream.Source();
    TStreamPos pos = streamBuf->TellL( MStreamBuf::ERead );
    TBuf8<KMceMaxSdpNameLength> sdpName;
    MceSerial::DecodeL( sdpName, readStream );
    
    streamBuf->SeekL( MStreamBuf::ERead, pos );
    
    CMceAudioCodec* codec = CreateL( sdpName );
    CleanupStack::PushL( codec );
    codec->InternalizeL( aSerCtx );
    return codec;    
    }


// -----------------------------------------------------------------------------
// TMceVideoCodecFactory::CreateLC
// -----------------------------------------------------------------------------
//
CMceVideoCodec* TMceVideoCodecFactory::CreateLC( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceVideoCodec* codec = CreateL( aSdpName );
    CleanupStack::PushL( codec );
    return codec;       
    }
    
// -----------------------------------------------------------------------------
// TMceVideoCodecFactory::CreateL
// -----------------------------------------------------------------------------
//
CMceVideoCodec* TMceVideoCodecFactory::CreateL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceVideoCodec* codec = NULL;
    
    if( aSdpName.CompareF(KMceSDPNameH263) == 0 || 
        aSdpName.CompareF(KMceSDPNameH2632000) == 0 ||
        aSdpName.CompareF(KMceSDPNameH2631998) == 0)
        {
        codec = CMceH263Codec::NewL( aSdpName );
        }
    else if ( aSdpName.CompareF(KMceSDPNameH264) == 0 )
        {
        codec = CMceAvcCodec::NewL( aSdpName );
        }
    else
        {
        User::Leave( KErrNotSupported );
        }
	return codec;
	        
    }

// -----------------------------------------------------------------------------
// TMceVideoCodecFactory::CreateLC
// -----------------------------------------------------------------------------
//
CMceVideoCodec* TMceVideoCodecFactory::CreateLC( MMceComSerializationContext& aSerCtx )
    {
    RReadStream& readStream = aSerCtx.ReadStream();
    
    MStreamBuf* streamBuf = readStream.Source();
    TStreamPos pos = streamBuf->TellL( MStreamBuf::ERead );
    TBuf8<KMceMaxSdpNameLength> sdpName;
    MceSerial::DecodeL( sdpName, readStream );
    
    streamBuf->SeekL( MStreamBuf::ERead, pos );
    
    CMceVideoCodec* codec = CreateL( sdpName );
    CleanupStack::PushL( codec );
    codec->InternalizeL( aSerCtx );
    return codec;    
    }

