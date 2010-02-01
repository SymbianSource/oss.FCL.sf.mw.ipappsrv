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


//#include <MCEManager.h>
#include <MCESession.h>
#include <MCEMediaStream.h>
//#include <MCEOneWayStream.h>
#include <MCEVideoStream.h>
#include <MCEMediaSource.h>
#include <MCEMicSource.h>
#include <MCEMediaSink.h>
#include <MCERtpSink.h>
#include <MCERtpSource.h>
#include <MCESpeakerSink.h>
#include <MCEcodec.h>
#include <MCEvideocodec.h>


#include "CMCETestUIEngine.h"
#include "MCETestUIEngineConstants.h"
#include "CMCETestUIEngineVideoCodec.h"

//#include "TMCETestUIEngineCmdBase.h"
#include "TMCETestUIEngineCmdDeleteAudioCodec.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMCETestUIEngineVideoCodec::NewL()
// -----------------------------------------------------------------------------
//
CMCETestUIEngineVideoCodec* CMCETestUIEngineVideoCodec::NewL(
            CMCETestUIEngine& aEngine, 
            CMceVideoCodec& aCodec )
    {

    CMCETestUIEngineVideoCodec* self = 
        new (ELeave) CMCETestUIEngineVideoCodec( aEngine, 
                                                  aCodec );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;  
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineVideoCodec::CMCETestUIEngineVideoCodec
// -----------------------------------------------------------------------------
//
CMCETestUIEngineVideoCodec::CMCETestUIEngineVideoCodec( 
                CMCETestUIEngine& aEngine, 
            	CMceVideoCodec& aCodec  )
    : iEngine( aEngine ),
      iCodec( aCodec )
    {
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineVideoCodec::ConstructL()
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineVideoCodec::ConstructL()
    {
      return;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineVideoCodec::~CMCETestUIEngineVideoCodec
// -----------------------------------------------------------------------------
//
EXPORT_C CMCETestUIEngineVideoCodec::~CMCETestUIEngineVideoCodec()
    {
    iCommands.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineCodec::GetCommands
// -----------------------------------------------------------------------------
//
const RPointerArray<TMCETestUIEngineCmdBase>& 
            CMCETestUIEngineVideoCodec::GetCommandsL()
    {	
    iCommands.ResetAndDestroy();
    if ( iCodec.State() == CMceCodec::EDisabled )
        {
        iCommands.Append( new (ELeave) TMCETestUIEngineCmdEnableVideoCodec( 
                                iEngine, *this ) );
        iCommands.Append( new (ELeave) TMCETestUIEngineCmdStandByVideoCodec( 
                                iEngine, *this ) );
        }
    else if(iCodec.State() == CMceCodec::EEnabled)
        {
        iCommands.Append( new (ELeave) TMCETestUIEngineCmdDisableVideoCodec( 
                                iEngine, *this ) );
        iCommands.Append( new (ELeave) TMCETestUIEngineCmdStandByVideoCodec( 
                                iEngine, *this ) );
        }
    else if(iCodec.State() == CMceCodec::EStandby)
    	{
        iCommands.Append( new (ELeave) TMCETestUIEngineCmdEnableVideoCodec( 
                                iEngine, *this ) );
        iCommands.Append( new (ELeave) TMCETestUIEngineCmdDisableVideoCodec( 
                                iEngine, *this ) );
        }  
        
    iCommands.Append( new (ELeave) TMCETestUIEngineCmdDeleteVideoCodec( 
                                iEngine, *this ) );
    return iCommands; 
    }
// -----------------------------------------------------------------------------
// CMCETestUIEngineVideoCodec::Codec
// -----------------------------------------------------------------------------
//
EXPORT_C CMceVideoCodec& CMCETestUIEngineVideoCodec::Codec()
	{
      return iCodec;
    }
// -----------------------------------------------------------------------------
// CMCETestUIEngineVideoSink::State
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC16& CMCETestUIEngineVideoCodec::State() const
    {
    if(iCodec.State() == CMceCodec::EEnabled) 
        {
        return KCodecStateEnabled;
        }
    else if(iCodec.State() == CMceCodec::EDisabled) 
        {
        return KCodecStateDisabled;
        }
    return KCodecStateStandBy;
    }
        


// -----------------------------------------------------------------------------
// CMCETestUIEngineVideoSink::AudioStream
// -----------------------------------------------------------------------------
//

EXPORT_C TMceCodecType CMCETestUIEngineVideoCodec::Type()
{
	return iCodec.Type();
}


