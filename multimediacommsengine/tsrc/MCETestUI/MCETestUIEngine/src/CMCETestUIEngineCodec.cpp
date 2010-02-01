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
#include <MCEAudioStream.h>
#include <MCEMediaSource.h>
#include <MCEMicSource.h>
#include <MCEMediaSink.h>
#include <MCERtpSink.h>
#include <MCERtpSource.h>
#include <MCESpeakerSink.h>
#include <MCEcodec.h>
#include <MCEaudiocodec.h>

#include "CMCETestUIEngine.h"
#include "MCETestUIEngineConstants.h"
#include "CMCETestUIEngineCodec.h"

#include "TMCETestUIEngineCmdBase.h"
#include "TMCETestUIEngineCmdDeleteAudioCodec.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMCETestUIEngineCodec::NewL()
// -----------------------------------------------------------------------------
//
CMCETestUIEngineCodec* CMCETestUIEngineCodec::NewL(
            CMCETestUIEngine& aEngine, 
            CMceAudioCodec& aCodec )
    {

    CMCETestUIEngineCodec* self = 
        new (ELeave) CMCETestUIEngineCodec( aEngine, 
                                                  aCodec );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;  
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineCodec::CMCETestUIEngineCodec
// -----------------------------------------------------------------------------
//
CMCETestUIEngineCodec::CMCETestUIEngineCodec( 
                CMCETestUIEngine& aEngine, 
            	CMceAudioCodec& aCodec  )
    : iEngine( aEngine ),
      iCodec( aCodec )
    {
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineCodec::ConstructL()
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineCodec::ConstructL()
    {
      return;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineCodec::~CMCETestUIEngineCodec
// -----------------------------------------------------------------------------
//
EXPORT_C CMCETestUIEngineCodec::~CMCETestUIEngineCodec()
    {
    iCommands.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineCodec::GetCommands
// -----------------------------------------------------------------------------
//
const RPointerArray<TMCETestUIEngineCmdBase>& 
            CMCETestUIEngineCodec::GetCommandsL()
    {	
    iCommands.ResetAndDestroy();
    if ( iCodec.State() == CMceCodec::EDisabled )
        {
        iCommands.Append( new (ELeave) TMCETestUIEngineCmdEnableAudioCodec( 
                                iEngine, *this ) );
        iCommands.Append( new (ELeave) TMCETestUIEngineCmdStandByAudioCodec( 
                                iEngine, *this ) );
        }
    else if(iCodec.State() == CMceCodec::EEnabled)
        {
        iCommands.Append( new (ELeave) TMCETestUIEngineCmdDisableAudioCodec( 
                                iEngine, *this ) );
        iCommands.Append( new (ELeave) TMCETestUIEngineCmdStandByAudioCodec( 
                                iEngine, *this ) );
        }
    else if(iCodec.State() == CMceCodec::EStandby)
    	{
        iCommands.Append( new (ELeave) TMCETestUIEngineCmdEnableAudioCodec( 
                                iEngine, *this ) );
        iCommands.Append( new (ELeave) TMCETestUIEngineCmdDisableAudioCodec( 
                                iEngine, *this ) );
        }  
    iCommands.Append( new (ELeave) TMCETestUIEngineCmdDeleteAudioCodec( 
                                iEngine, *this ) );
    return iCommands; 
    }
// -----------------------------------------------------------------------------
// CMCETestUIEngineCodec::Codec
// -----------------------------------------------------------------------------
//
EXPORT_C CMceAudioCodec& CMCETestUIEngineCodec::Codec()
	{
      return iCodec;
    }
// -----------------------------------------------------------------------------
// CMCETestUIEngineCodec::State
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC16& CMCETestUIEngineCodec::State() const
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
// CMCETestUIEngineCodec::Type
// -----------------------------------------------------------------------------
//

EXPORT_C TMceCodecType CMCETestUIEngineCodec::Type()
{
	return iCodec.Type();
}
