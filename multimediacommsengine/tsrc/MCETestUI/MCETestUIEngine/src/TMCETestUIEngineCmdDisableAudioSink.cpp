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



#include "TMCETestUIEngineCmdDisableAudioSink.h"
#include "CMCETestUIEngineSink.h"
#include "CMCETestUIEngineVideoSink.h"
#include "MCEAudioStream.h"
#include "MCEVideoStream.h"
#include <MCERtpSink.h>
#include <MCEMediaSink.h>
#include <MCESpeakerSink.h>

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDisableStream::TMCETestUIEngineCmdDisableStream
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdDisableSink::TMCETestUIEngineCmdDisableSink(
            CMCETestUIEngine& aEngine,
		    CMCETestUIEngineSink& aAudioSink )
    : TMCETestUIEngineCmdBase( aEngine ),
      iAudioSink( aAudioSink )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDisableSink::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdDisableSink::ExecuteL()
	{
	iAudioSink.Sink().DisableL();
	iEngine.EngineStateChangedL();
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDisableSink::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdDisableSink::Caption() const
	{
	return KCommandCaptionDisableSink;
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDisableVideoSink::TMCETestUIEngineCmdDisableVideoSink
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdDisableVideoSink::TMCETestUIEngineCmdDisableVideoSink(
            CMCETestUIEngine& aEngine,
		    CMCETestUIEngineVideoSink& aVideoSink )
    : TMCETestUIEngineCmdBase( aEngine ),
      iVideoSink( aVideoSink )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDisableVideoSink::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdDisableVideoSink::ExecuteL()
	{
	iVideoSink.Sink().DisableL();
	iEngine.EngineStateChangedL();
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDisableVideoSink::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdDisableVideoSink::Caption() const
	{
	return KCommandCaptionDisableSink;
	}



// End of File
