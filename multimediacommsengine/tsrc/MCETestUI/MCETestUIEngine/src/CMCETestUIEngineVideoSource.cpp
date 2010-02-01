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
#include <MCEFileSource.h>
#include <MCESpeakerSink.h>

#include "CMCETestUIEngine.h"
#include "MCETestUIEngineConstants.h"
#include "CMCETestUIEngineVideoSource.h"

#include "TMCETestUIEngineCmdBase.h"
#include "TMCETestUIEngineCmdEnableAudioSource.h"
#include "TMCETestUIEngineCmdDisableAudioSource.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMCETestUIEngineVideoSource::NewL()
// -----------------------------------------------------------------------------
//
CMCETestUIEngineVideoSource* CMCETestUIEngineVideoSource::NewL(
            CMCETestUIEngine& aEngine, 
            CMceMediaSource& aSource  )
    {

    CMCETestUIEngineVideoSource* self = 
        new (ELeave) CMCETestUIEngineVideoSource( aEngine, 
                                                  aSource );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;  
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineVideoSource::CMCETestUIEngineVideoSource
// -----------------------------------------------------------------------------
//
CMCETestUIEngineVideoSource::CMCETestUIEngineVideoSource( 
                CMCETestUIEngine& aEngine, 
            CMceMediaSource& aSource )
    : iEngine( aEngine ),
      iSource( aSource )
    {
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineVideoSource::ConstructL()
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineVideoSource::ConstructL()
    {
    return;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineVideoSource::~CMCETestUIEngineVideoSource
// -----------------------------------------------------------------------------
//
EXPORT_C CMCETestUIEngineVideoSource::~CMCETestUIEngineVideoSource()
    {
    iCommands.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineVideoSource::GetCommands
// -----------------------------------------------------------------------------
//
const RPointerArray<TMCETestUIEngineCmdBase>& 
            CMCETestUIEngineVideoSource::GetCommandsL()
    {	
    iCommands.ResetAndDestroy();
    
    if ( iSource.IsEnabled() )
        {
        iCommands.Append( new (ELeave) TMCETestUIEngineCmdDisableVideoSource( 
                                iEngine, *this ) );
        }
    else
        {
        iCommands.Append( new (ELeave) TMCETestUIEngineCmdEnableVideoSource( 
                                iEngine, *this ) );
        }	  
    if(iSource.Type() == KMceFileSource) 
    	{
    	iCommands.Append( new (ELeave) TMCETestUIEngineCmdVideoTranscode( 
                                iEngine, *this ) );
		iCommands.Append( new (ELeave) TMCETestUIEngineCmdVideoCancelTranscode( 
                                iEngine, *this ) );                                
    	}
    return iCommands; 
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineVideoSource::Type
// -----------------------------------------------------------------------------
//
EXPORT_C TMceSourceType CMCETestUIEngineVideoSource::Type() const
    {
    return iSource.Type();;
    }
    


// -----------------------------------------------------------------------------
// CMCETestUIEngineVideoSource::State
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC16& CMCETestUIEngineVideoSource::State() const
    {
    if ( iSource.IsEnabled() )
        {
        return KSourceStateEnabled;
        }
    return KSourceStateDisabled;
    }
        

// -----------------------------------------------------------------------------
// CMCETestUIEngineVideoSource::Source
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMediaSource& CMCETestUIEngineVideoSource::Source()
    {        
    return iSource;
    }

