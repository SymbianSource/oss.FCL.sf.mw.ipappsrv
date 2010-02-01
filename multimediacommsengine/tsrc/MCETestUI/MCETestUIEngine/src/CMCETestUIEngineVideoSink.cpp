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




// MCE API



#include <MCESession.h>
#include <MCEMediaStream.h>
//#include <MCEOneWayStream.h>
#include <MCEVideoStream.h>
#include <MCEMediaSource.h>
#include <MCEMicSource.h>

#include <MCERtpSink.h>
#include <MCEMediaSink.h>
#include <MCESpeakerSink.h>



#include "CMCETestUIEngine.h"
#include "MCETestUIEngineConstants.h"
#include "CMCETestUIEngineVideoSink.h"
#include "TMCETestUIEngineCmdBase.h"
#include "TMCETestUIEngineCmdEnableAudioSink.h"
#include "TMCETestUIEngineCmdDisableAudioSink.h"
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMCETestUIEngineVideoSink::NewL()
// -----------------------------------------------------------------------------
//
CMCETestUIEngineVideoSink* CMCETestUIEngineVideoSink::NewL(
            CMCETestUIEngine& aEngine, 
            CMceMediaSink& aSink )
    {

    CMCETestUIEngineVideoSink* self = 
        new (ELeave) CMCETestUIEngineVideoSink( aEngine, 
                                                  aSink );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;  
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineVideoSink::CMCETestUIEngineVideoSink
// -----------------------------------------------------------------------------
//
CMCETestUIEngineVideoSink::CMCETestUIEngineVideoSink( 
                CMCETestUIEngine& aEngine, 
            	CMceMediaSink& aSink )
    : iEngine( aEngine ),
      iSink( aSink )
    {
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineSink::ConstructL()
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineVideoSink::ConstructL()
    {
	return; //solve direction function is availble in engineaudiostream.h 
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineSink::~CMCETestUIEngineSink
// -----------------------------------------------------------------------------
//
EXPORT_C CMCETestUIEngineVideoSink::~CMCETestUIEngineVideoSink()
    {
    iCommands.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineVideoSink::GetCommands
// -----------------------------------------------------------------------------
//
const RPointerArray<TMCETestUIEngineCmdBase>& 
            CMCETestUIEngineVideoSink::GetCommandsL()
    {	
    iCommands.ResetAndDestroy();
    
    if ( iSink.IsEnabled() )
        {
        iCommands.Append( new (ELeave) TMCETestUIEngineCmdDisableVideoSink( 
                                iEngine, *this ) );
        }
    else
        {
        iCommands.Append( new (ELeave) TMCETestUIEngineCmdEnableVideoSink( 
                                iEngine, *this ) );
        }	  
        
    return iCommands; 
    }


// -----------------------------------------------------------------------------
// CMCETestUIEngineVideoSink::State
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC16& CMCETestUIEngineVideoSink::State() const
    {
    if ( iSink.IsEnabled() )
        {
        return KSinkStateEnabled;
        }
    return KSinkStateDisabled;
    }
        

// -----------------------------------------------------------------------------
// CMCETestUIEngineVideoSink::AudioStream
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMediaSink& CMCETestUIEngineVideoSink::Sink()
    {        
    return iSink;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineVideoSink::AudioStream
// -----------------------------------------------------------------------------
//

EXPORT_C TMceSinkType CMCETestUIEngineVideoSink::Type()
{
	return iSink.Type();
}

// -----------------------------------------------------------------------------
// CMCETestUIEngineVideoSink::AudioStream
// -----------------------------------------------------------------------------
//

void CMCETestUIEngineVideoSink::EnableL() 
{
	iSink.EnableL();
}

// -----------------------------------------------------------------------------
// CMCETestUIEngineVideoSink::AudioStream
// -----------------------------------------------------------------------------
//

void CMCETestUIEngineVideoSink::DisableL() 
{
	iSink.DisableL();
}
