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
* Description:    Implementation.
*
*/



#include "TMCETestUIEngineCmdDisableAudioSource.h"
#include "CMCETestUIEngineSource.h"
#include "CMCETestUIEngineVideoSource.h"
#include <MCEMediaSource.h>
#include <MCEMicSource.h>
#include <MCERtpSource.h>
#include "MCEAudioStream.h"
#include "MCEVideoStream.h"

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDisableStream::TMCETestUIEngineCmdDisableStream
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdDisableSource::TMCETestUIEngineCmdDisableSource(
            CMCETestUIEngine& aEngine,
		    CMCETestUIEngineSource& aAudioSource )
    : TMCETestUIEngineCmdBase( aEngine ),
      iAudioSource( aAudioSource )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDisableSource::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdDisableSource::ExecuteL()
	{
	iAudioSource.Source().DisableL();
	iEngine.EngineStateChangedL();
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDisableSource::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdDisableSource::Caption() const
	{
	return KCommandCaptionDisableSource;
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDisableVideoSource::TMCETestUIEngineCmdDisableVideoSource
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdDisableVideoSource::TMCETestUIEngineCmdDisableVideoSource(
            CMCETestUIEngine& aEngine,
		    CMCETestUIEngineVideoSource& aVideoSource )
    : TMCETestUIEngineCmdBase( aEngine ),
      iVideoSource( aVideoSource )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDisableVideoSource::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdDisableVideoSource::ExecuteL()
	{
	iVideoSource.Source().DisableL();
	iEngine.EngineStateChangedL();
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDisableVideoSource::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdDisableVideoSource::Caption() const
	{
	return KCommandCaptionDisableSource;
	}



// End of File
