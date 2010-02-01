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
#include <MCEAudioStream.h>
#include <MCEMediaSource.h>
#include <MCEMicSource.h>

#include <MCERtpSink.h>
#include <MCEMediaSink.h>
#include <MCESpeakerSink.h>



#include "CMCETestUIEngine.h"
#include "MCETestUIEngineConstants.h"
#include "CMCETestUIEngineSink.h"

#include "TMCETestUIEngineCmdBase.h"
#include "TMCETestUIEngineCmdEnableAudioSink.h"
#include "TMCETestUIEngineCmdDisableAudioSink.h"
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMCETestUIEngineSink::NewL()
// -----------------------------------------------------------------------------
//
CMCETestUIEngineSink* CMCETestUIEngineSink::NewL(
            CMCETestUIEngine& aEngine, 
            CMceMediaSink& aSink )
    {

    CMCETestUIEngineSink* self = 
        new (ELeave) CMCETestUIEngineSink( aEngine, 
                                                  aSink );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;  
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineSink::CMCETestUIEngineSink
// -----------------------------------------------------------------------------
//
CMCETestUIEngineSink::CMCETestUIEngineSink( 
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
void CMCETestUIEngineSink::ConstructL()
    {
	return; //solve direction function is availble in engineaudiostream.h 
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineSink::~CMCETestUIEngineSink
// -----------------------------------------------------------------------------
//
EXPORT_C CMCETestUIEngineSink::~CMCETestUIEngineSink()
    {
    iCommands.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineSink::GetCommands
// -----------------------------------------------------------------------------
//
const RPointerArray<TMCETestUIEngineCmdBase>& 
            CMCETestUIEngineSink::GetCommandsL()
    {	
    iCommands.ResetAndDestroy();
    
    if ( iSink.IsEnabled() )
        {
        iCommands.Append( new (ELeave) TMCETestUIEngineCmdDisableSink( 
                                iEngine, *this ) );
        }
    else
        {
        iCommands.Append( new (ELeave) TMCETestUIEngineCmdEnableSink( 
                                iEngine, *this ) );
        }	  
    if(iSink.Type() == KMceRTPSink)    	  
    	{
    	iCommands.Append( new (ELeave) TMCETestUIEngineCmdSendSR( 
                                iEngine, *this ) );
    	}   
     
    return iCommands; 
    }


// -----------------------------------------------------------------------------
// CMCETestUIEngineSink::State
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC16& CMCETestUIEngineSink::State() const
    {
    if ( iSink.IsEnabled() )
        {
        return KSinkStateEnabled;
        }
    return KSinkStateDisabled;
    }
        

// -----------------------------------------------------------------------------
// CMCETestUIEngineSink::AudioStream
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMediaSink& CMCETestUIEngineSink::Sink()
    {        
    return iSink;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineSink::AudioStream
// -----------------------------------------------------------------------------
//

EXPORT_C const TMceSinkType CMCETestUIEngineSink::Type() const
{
	return iSink.Type();
}


