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




#include <MCESession.h>
#include <MCEMediaStream.h>
#include <MCEVideoStream.h>
#include <MCEMediaSource.h>
#include <MCEMicSource.h>
#include <MCEMediaSink.h>
#include <MCERtpSink.h>
#include <MCERtpSource.h>
#include <MCESpeakerSink.h>
#include <MCEVideoCodec.h>

#include "MCETestUIEngineConstants.h"
#include "CMCETestUIEngineVideoStream.h"
#include "CMCETestUIEngineVideoCodec.h"
#include "CMCETestUIEngineVideoSink.h"
#include "CMCETestUIEngineVideoSource.h"

#include "TMCETestUIEngineCmdBase.h"
#include "TMCETestUIEngineCmdEnableStream.h"
#include "TMCETestUIEngineCmdDisableStream.h"
#include "TMCETestUIEngineCmdRemoveStream.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutSession::NewL()
// -----------------------------------------------------------------------------
//
CMCETestUIEngineVideoStream* CMCETestUIEngineVideoStream::NewL(
    CMCETestUIEngine& aEngine, 
    CMceVideoStream& aVideoStream )
    {

    CMCETestUIEngineVideoStream* self = 
        new( ELeave )CMCETestUIEngineVideoStream( aEngine, aVideoStream );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;  
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineVideoStream::CMCETestUIEngineVideoStream
// -----------------------------------------------------------------------------
//
CMCETestUIEngineVideoStream::CMCETestUIEngineVideoStream( 
                CMCETestUIEngine& aEngine,
                CMceVideoStream& aVideoStream )
    : iEngine( aEngine ),
      iVideoStream( aVideoStream ),
      iOneTimeRTCPFlag(1),
      iVideoDirection( CMCETestUIEngineVideoStream::ENotSpesified )
    {
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineVideoStream::ConstructL()
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineVideoStream::ConstructL()
    {
    SolveVideoDirection();
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineVideoStream::~CMCETestUIEngineVideoStream
// -----------------------------------------------------------------------------
//
EXPORT_C CMCETestUIEngineVideoStream::~CMCETestUIEngineVideoStream()
    {
    iCommands.ResetAndDestroy();
    iSources.ResetAndDestroy();
    iSinks.ResetAndDestroy();
    iCodecs.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineVideoStream::GetCommands
// -----------------------------------------------------------------------------
//
const RPointerArray<TMCETestUIEngineCmdBase>& 
    CMCETestUIEngineVideoStream::GetCommandsL()
    {	
    iCommands.ResetAndDestroy();
    
    if ( iVideoStream.IsEnabled() )
        {
        iCommands.Append( new( ELeave )TMCETestUIEngineCmdDisableVideoStream( 
            iEngine, *this ) );
        }
    else
        {
        iCommands.Append( new( ELeave )TMCETestUIEngineCmdEnableVideoStream( 
            iEngine, *this ) );
        }	  

    iCommands.AppendL( new( ELeave )TMCETestUIEngineCmdRemoveStream( 
        iEngine, iVideoStream ) );
       
    return iCommands; 
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineVideoStream::Direction
// -----------------------------------------------------------------------------
//
EXPORT_C CMCETestUIEngineVideoStream::TVideoDirection 
    CMCETestUIEngineVideoStream::VideoDirection() const
    {
    return iVideoDirection;    
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineVideoStream::TextualDirection
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC16& 
    CMCETestUIEngineVideoStream::TextualVideoDirection() const
    {
    switch ( VideoDirection() )
        {
        case CMCETestUIEngineVideoStream::EUpLink :
            {
            return KStreamDirectionUpLink;
            break;
            }
        case CMCETestUIEngineVideoStream::EDownLink :
            {
            return KStreamDirectionDownLink;
            break;
            }           
        default:
            {
            break;
            }
        }
        
    return KStreamDirectionNotSpesified;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineVideoStream::StreamState
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC16& CMCETestUIEngineVideoStream::StreamState() 
    {
    TInt state = iVideoStream.State();

	switch( state ) 
		{
		case CMceMediaStream::EUninitialized: 
		    return KStreamUninitialized; 
		    break;
		case CMceMediaStream::EInitialized: 
		    return KStreamInitialized; 
		    break;
		case CMceMediaStream::EBuffering: 
		    return KStreamBuffering; 
		    break;
		case CMceMediaStream::EIdle: 
		    return KStreamIdle; 
		    break;
		case CMceMediaStream::EStreaming: 
		    return KStreamStreaming; 
		    break;
		case CMceMediaStream::EDisabled: 
		    return KStreamDisabled; 
		    break;
		case CMceMediaStream::ENoResources: 
		    return KStreamNoResources; 
		    break;
		case CMceMediaStream::ETranscodingRequired: 
		    return KStreamTranscodingRequired; 
		    break;
		case CMceMediaStream::ETranscoding: 
		    return KStreamTranscoding; 
		    break;
		default: 
		    return KSessionStateUnknown; 
		    break;
		}    
    }
     

// -----------------------------------------------------------------------------
// CMCETestUIEngineVideoStream::State
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC16& CMCETestUIEngineVideoStream::State() const
    {
    if ( iVideoStream.IsEnabled() )
        {
        return KStreamStateEnabled;
        }
    return KStreamStateDisabled;
    }
        
// -----------------------------------------------------------------------------
// CMCETestUIEngineVideoStream::AudioStream
// -----------------------------------------------------------------------------
//
CMceVideoStream& CMCETestUIEngineVideoStream::VideoStream()
    {        
    return iVideoStream;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineVideoStream::SolveDirection
// -----------------------------------------------------------------------------
//
CMCETestUIEngineVideoStream::TVideoDirection 
    CMCETestUIEngineVideoStream::SolveVideoDirection()
    {
    TBool isUpLink = EFalse;
    TBool isDownLink = EFalse;
          
    if ( iVideoStream.Source() )
        {
        if ( iVideoStream.Source()->Type() == KMceRTPSource )
            {
            isDownLink = ETrue;
            }
        }
        
    const RPointerArray<CMceMediaSink>& sinks = iVideoStream.Sinks();
    for ( TInt i = 0; i < sinks.Count(); ++i )
        {
        if ( sinks[i]->Type() == KMceRTPSink )
            {
            isUpLink = ETrue;
            }
        }
       
    if ( isUpLink && isDownLink )
        {
        iVideoDirection = CMCETestUIEngineVideoStream::ENotSpesified;
        }
    else if ( isUpLink )
        {
        iVideoDirection = CMCETestUIEngineVideoStream::EUpLink;
        }
    else if ( isDownLink )
        {
        iVideoDirection = CMCETestUIEngineVideoStream::EDownLink;
        }  
    else
        {
        iVideoDirection = CMCETestUIEngineVideoStream::ENotSpesified;
        }
        
    return iVideoDirection; 
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineAudioStream::Sources
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray<CMCETestUIEngineVideoSource>& 
				CMCETestUIEngineVideoStream::SourcesL()
    {
	iSources.ResetAndDestroy();
	CMceMediaSource* source = iVideoStream.Source();
	
	CMCETestUIEngineVideoSource* engineSource = 
	    CMCETestUIEngineVideoSource::NewL(iEngine,*source);
	CleanupStack::PushL(engineSource);
	iSources.AppendL(engineSource);
	CleanupStack::Pop(engineSource);

	return iSources;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineVideoStream::Sinks
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray<CMCETestUIEngineVideoSink>& 
    CMCETestUIEngineVideoStream::SinksL()
    {
    iSinks.ResetAndDestroy();
    const RPointerArray<CMceMediaSink>& sinks = iVideoStream.Sinks();
    
    for ( TInt i = 0; i < sinks.Count(); ++i )
        {
        CMceMediaSink* sink = sinks[i];
        CMCETestUIEngineVideoSink* engineSink =
            CMCETestUIEngineVideoSink::NewL( iEngine, *sink);
        CleanupStack::PushL( engineSink );
        iSinks.AppendL( engineSink );
        CleanupStack::Pop( engineSink );                   
        }
	return iSinks;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineVideoStream::Codecs
// -----------------------------------------------------------------------------
//
EXPORT_C  RPointerArray<CMCETestUIEngineVideoCodec>& 
    CMCETestUIEngineVideoStream::CodecsL()
    {
	iCodecs.ResetAndDestroy();
    const RPointerArray<CMceVideoCodec>& codecs = iVideoStream.Codecs();
    
    for ( TInt i = 0; i < codecs.Count(); ++i )
        {
        CMceVideoCodec* codec = codecs[i];
        CMCETestUIEngineVideoCodec* engineCodec =
            CMCETestUIEngineVideoCodec::NewL( iEngine, *codec);
        CleanupStack::PushL( engineCodec );
        iCodecs.AppendL( engineCodec );
        CleanupStack::Pop( engineCodec );                   
        }
	return iCodecs;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineVideoStream::CurrentCodecs
// -----------------------------------------------------------------------------
//
EXPORT_C RPointerArray<CMCETestUIEngineVideoCodec>& 
    CMCETestUIEngineVideoStream::CurrentCodecs()
    {
	return iCodecs;
    }
