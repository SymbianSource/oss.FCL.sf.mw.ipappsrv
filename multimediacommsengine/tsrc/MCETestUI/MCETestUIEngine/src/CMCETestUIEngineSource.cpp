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
#include <MCEFileSource.h>
#include <MCESpeakerSink.h>

#include "CMCETestUIEngine.h"
#include "MCETestUIEngineConstants.h"
#include "CMCETestUIEngineSource.h"

#include "TMCETestUIEngineCmdBase.h"
#include "TMCETestUIEngineCmdEnableAudioSource.h"
#include "TMCETestUIEngineCmdDisableAudioSource.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMCETestUIEngineSource::NewL()
// -----------------------------------------------------------------------------
//
CMCETestUIEngineSource* CMCETestUIEngineSource::NewL(
            CMCETestUIEngine& aEngine, 
            CMceMediaSource& aSource  )
    {

    CMCETestUIEngineSource* self = 
        new (ELeave) CMCETestUIEngineSource( aEngine, 
                                                  aSource );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;  
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineSource::CMCETestUIEngineSource
// -----------------------------------------------------------------------------
//
CMCETestUIEngineSource::CMCETestUIEngineSource( 
                CMCETestUIEngine& aEngine, 
            CMceMediaSource& aSource )
    : iEngine( aEngine ),
      iSource( aSource )
    {
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineSource::ConstructL()
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineSource::ConstructL()
    {
    return;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineAudioStream::~CMCETestUIEngineAudioStream
// -----------------------------------------------------------------------------
//
EXPORT_C CMCETestUIEngineSource::~CMCETestUIEngineSource()
    {
    iCommands.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineSource::GetCommands
// -----------------------------------------------------------------------------
//
const RPointerArray<TMCETestUIEngineCmdBase>& 
            CMCETestUIEngineSource::GetCommandsL()
    {	
    iCommands.ResetAndDestroy();
    
    if ( iSource.IsEnabled() )
        {
        iCommands.Append( new (ELeave) TMCETestUIEngineCmdDisableSource( 
                                iEngine, *this ) );
        }
    else
        {
        iCommands.Append( new (ELeave) TMCETestUIEngineCmdEnableSource( 
                                iEngine, *this ) );
        }
    if(iSource.Type() == KMceRTPSource)    	  
    	{
    	iCommands.Append( new (ELeave) TMCETestUIEngineCmdEnableRtpSourceInActivityTimer( 
                                iEngine, *this ) );
        iCommands.Append( new (ELeave) TMCETestUIEngineCmdSendRR( 
                                iEngine, *this ) );
    	}
    else if(iSource.Type() == KMceFileSource) 
    	{
    	iCommands.Append( new (ELeave) TMCETestUIEngineCmdAudioTranscode( 
                                iEngine, *this ) );
		iCommands.Append( new (ELeave) TMCETestUIEngineCmdAudioCancelTranscode( 
                                iEngine, *this ) );                                
    	}
        
    if( iSource.DtmfAvailable() && !iSource.DtmfActive() )
        {
        iCommands.Append( new (ELeave) TMCETestUIEngineCmdStartDtmfToneL( 
                                iEngine, *this ) );
                                
		iCommands.Append( new (ELeave) TMCETestUIEngineCmdSendDtmfToneL( 
                                iEngine, *this ) );     
        
        iCommands.Append( new (ELeave) TMCETestUIEngineCmdSendDtmfToneSequenceL( 
                                iEngine, *this ) );                          
        }
    else if( iSource.DtmfAvailable() && iSource.DtmfActive() )
        {
        iCommands.Append( new (ELeave) TMCETestUIEngineCmdStopDtmfToneL( 
                                iEngine, *this ) );
        }
        
    return iCommands; 
    }

// -----------------------------------------------------------------------------
// CMCEMediaSource::Type
// -----------------------------------------------------------------------------
//
EXPORT_C TMceSourceType CMCETestUIEngineSource::Type() const
    {
    return iSource.Type();;
    }
    


// -----------------------------------------------------------------------------
// CMCETestUIEngineAudioStream::State
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC16& CMCETestUIEngineSource::State() const
    {
    if ( iSource.IsEnabled() )
        {
        return KSourceStateEnabled;
        }
    return KSourceStateDisabled;
    }
        

// -----------------------------------------------------------------------------
// CMCETestUIEngineSource::Source
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMediaSource& CMCETestUIEngineSource::Source()
    {        
    return iSource;
    }
    
