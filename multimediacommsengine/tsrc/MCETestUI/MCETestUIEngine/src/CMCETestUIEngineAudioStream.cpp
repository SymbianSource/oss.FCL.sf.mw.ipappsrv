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
#include <MCEAudioStream.h>
#include <MCEMediaSource.h>
#include <MCEMicSource.h>
#include <MCEMediaSink.h>
#include <MCERtpSink.h>
#include <MCERtpSource.h>
#include <MCESpeakerSink.h>
#include <MCEAudioCodec.h>

#include "MCETestUIEngineConstants.h"
#include "CMCETestUIEngineAudioStream.h"
#include "CMCETestUIEngineCodec.h"
#include "CMCETestUIEngineSink.h"
#include "CMCETestUIEngineSource.h"

#include "TMCETestUIEngineCmdBase.h"
#include "TMCETestUIEngineCmdEnableStream.h"
#include "TMCETestUIEngineCmdDisableStream.h"
#include "TMCETestUIEngineCmdDeleteAudioCodec.h"
#include "TMCETestUIEngineCmdRemoveStream.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutSession::NewL()
// -----------------------------------------------------------------------------
//
CMCETestUIEngineAudioStream* CMCETestUIEngineAudioStream::NewL(
    CMCETestUIEngine& aEngine, 
    CMceAudioStream& aAudioStream )
    {
    CMCETestUIEngineAudioStream* self = 
        new( ELeave )CMCETestUIEngineAudioStream( aEngine, aAudioStream );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;  
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineAudioStream::CMCETestUIEngineAudioStream
// -----------------------------------------------------------------------------
//
CMCETestUIEngineAudioStream::CMCETestUIEngineAudioStream( 
                CMCETestUIEngine& aEngine,
                CMceAudioStream& aAudioStream )
    : iEngine( aEngine ),
      iAudioStream( aAudioStream ),
      iOneTimeRTCPFlag(1),
      iDirection( CMCETestUIEngineAudioStream::ENotSpesified )
    {
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineAudioStream::ConstructL()
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineAudioStream::ConstructL()
    {
    SolveDirection();
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineAudioStream::~CMCETestUIEngineAudioStream
// -----------------------------------------------------------------------------
//
EXPORT_C CMCETestUIEngineAudioStream::~CMCETestUIEngineAudioStream()
    {
    iCommands.ResetAndDestroy();
    iSources.ResetAndDestroy();
    iSinks.ResetAndDestroy();
    iCodecs.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineAudioStream::GetCommands
// -----------------------------------------------------------------------------
//
const RPointerArray<TMCETestUIEngineCmdBase>& 
    CMCETestUIEngineAudioStream::GetCommandsL()
    {    
    iCommands.ResetAndDestroy();
    
    if ( iAudioStream.IsEnabled() )
        {
        iCommands.AppendL( new( ELeave )TMCETestUIEngineCmdDisableStream( 
            iEngine, *this ) );
        }
    else
        {
        iCommands.AppendL( new( ELeave )TMCETestUIEngineCmdEnableStream( 
            iEngine, *this ) );
        }	  
    iCommands.AppendL( new( ELeave )TMCETestUIEngineCmdAddCodec( 
        iEngine, *this ) );
    
    iCommands.AppendL( new( ELeave )TMCETestUIEngineCmdRemoveStream( 
        iEngine, iAudioStream ) );
                                
    return iCommands; 
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineAudioStream::Direction
// -----------------------------------------------------------------------------
//
EXPORT_C CMCETestUIEngineAudioStream::TDirection 
    CMCETestUIEngineAudioStream::Direction() const
    {
    return iDirection;    
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineAudioStream::TextualDirection
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC16& CMCETestUIEngineAudioStream::TextualDirection() const
    {
    switch ( Direction() )
        {
        case CMCETestUIEngineAudioStream::EUpLink :
            {
            return KStreamDirectionUpLink;
            break;
            }
        case CMCETestUIEngineAudioStream::EDownLink :
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
// CMCETestUIEngineAudioStream::StreamState
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC16& CMCETestUIEngineAudioStream::StreamState() 
    {
    TInt state = iAudioStream.State();

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
// CMCETestUIEngineAudioStream::State
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC16& CMCETestUIEngineAudioStream::State() const
    {
    if ( iAudioStream.IsEnabled() )
        {
        return KStreamStateEnabled;
        }
    return KStreamStateDisabled;
    }
        

// -----------------------------------------------------------------------------
// CMCETestUIEngineAudioStream::AudioStream
// -----------------------------------------------------------------------------
//
CMceAudioStream& CMCETestUIEngineAudioStream::AudioStream()
    {        
    return iAudioStream;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineAudioStream::SolveDirection
// -----------------------------------------------------------------------------
//
CMCETestUIEngineAudioStream::TDirection CMCETestUIEngineAudioStream::SolveDirection()
    {
    TBool isUpLink = EFalse;
    TBool isDownLink = EFalse;
          
    if ( iAudioStream.Source() )
        {
        if ( iAudioStream.Source()->Type() == KMceRTPSource )
            {
            isDownLink = ETrue;
            }
        }
        
    const RPointerArray<CMceMediaSink>& sinks = iAudioStream.Sinks();
    for ( TInt i = 0; i < sinks.Count(); ++i )
        {
        if ( sinks[i]->Type() == KMceRTPSink )
            {
            isUpLink = ETrue;
            }
        }
       
    if ( isUpLink && isDownLink )
        {
        iDirection = CMCETestUIEngineAudioStream::ENotSpesified;
        }
    else if ( isUpLink )
        {
        iDirection = CMCETestUIEngineAudioStream::EUpLink;
        }
    else if ( isDownLink )
        {
        iDirection = CMCETestUIEngineAudioStream::EDownLink;
        }  
    else
        {
        iDirection = CMCETestUIEngineAudioStream::ENotSpesified;
        }
        
    return iDirection; 
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineAudioStream::Sources
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray<CMCETestUIEngineSource>& 
				CMCETestUIEngineAudioStream::SourcesL()
    {
	iSources.ResetAndDestroy();
	CMceMediaSource* source = iAudioStream.Source();
	
	CMCETestUIEngineSource* engineSource = CMCETestUIEngineSource::NewL(iEngine,*source);
	CleanupStack::PushL(engineSource);
	iSources.Append(engineSource);
	CleanupStack::Pop(engineSource);

	return iSources;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineAudioStream::Sinks
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray<CMCETestUIEngineSink>& 
    CMCETestUIEngineAudioStream::SinksL()
    {
    iSinks.ResetAndDestroy();
    const RPointerArray<CMceMediaSink>& sinks = iAudioStream.Sinks();
    
    for ( TInt i = 0; i < sinks.Count(); ++i )
        {
        CMceMediaSink* sink = sinks[i];
        CMCETestUIEngineSink* engineSink =
            CMCETestUIEngineSink::NewL( iEngine, *sink );
        CleanupStack::PushL( engineSink );
        iSinks.AppendL( engineSink );
        CleanupStack::Pop( engineSink );               
        }
	return iSinks;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineAudioStream::Codecs
// -----------------------------------------------------------------------------
//
EXPORT_C  RPointerArray<CMCETestUIEngineCodec>& 
    CMCETestUIEngineAudioStream::CodecsL()
    {
	iCodecs.ResetAndDestroy();
    const RPointerArray<CMceAudioCodec>& codecs = iAudioStream.Codecs();
    
    for ( TInt i = 0; i < codecs.Count(); ++i )
        {
        CMceAudioCodec* codec = codecs[i];
        CMCETestUIEngineCodec* engineCodec =
            CMCETestUIEngineCodec::NewL( iEngine, *codec );
        CleanupStack::PushL( engineCodec );
        iCodecs.AppendL( engineCodec );
        CleanupStack::Pop( engineCodec );
        }
	return iCodecs;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineAudioStream::CurrentCodecs
// -----------------------------------------------------------------------------
//
EXPORT_C  RPointerArray<CMCETestUIEngineCodec>& 
	CMCETestUIEngineAudioStream::CurrentCodecs()
    {
	return iCodecs;
    }
