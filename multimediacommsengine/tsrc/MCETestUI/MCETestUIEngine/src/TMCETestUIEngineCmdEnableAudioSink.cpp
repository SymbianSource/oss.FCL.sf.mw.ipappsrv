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



#include "TMCETestUIEngineCmdEnableAudioSink.h"
#include "CMCETestUIEngineSink.h"
#include "CMCETestUIEngineVideoSink.h"
#include "MCEAudioStream.h"
#include "MCEVideoStream.h"
#include <MCERtpSink.h>
#include <MCEMediaSink.h>
#include <MCESpeakerSink.h>

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEnableStream::TMCETestUIEngineCmdEnableStream
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdEnableSink::TMCETestUIEngineCmdEnableSink(
            CMCETestUIEngine& aEngine,
		    CMCETestUIEngineSink& aAudioSink )
    : TMCETestUIEngineCmdBase( aEngine ),
      iAudioSink( aAudioSink )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEnableSink::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdEnableSink::ExecuteL()
	{
	iAudioSink.Sink().EnableL();
	iEngine.EngineStateChangedL();
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEnableSink::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdEnableSink::Caption() const
	{
	return KCommandCaptionEnableSink;
	}


// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEnableVideoSink::TMCETestUIEngineCmdEnableVideoSink
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdEnableVideoSink::TMCETestUIEngineCmdEnableVideoSink(
            CMCETestUIEngine& aEngine,
		    CMCETestUIEngineVideoSink& aVideoSink )
    : TMCETestUIEngineCmdBase( aEngine ),
      iVideoSink( aVideoSink )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEnableVideoSink::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdEnableVideoSink::ExecuteL()
	{
	iVideoSink.Sink().EnableL();
	iEngine.EngineStateChangedL();
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEnableVideoSink::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdEnableVideoSink::Caption() const
	{
	return KCommandCaptionEnableSink;
	}
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdSendSR::TMCETestUIEngineCmdSendSR
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdSendSR::TMCETestUIEngineCmdSendSR(
  							CMCETestUIEngine& aEngine,
		    CMCETestUIEngineSink& aAudioSink )
    : TMCETestUIEngineCmdBase( aEngine ),
      iAudioSink( aAudioSink )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdSendSR::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdSendSR::ExecuteL()
	{
	CMceRtpSink& rtpSink = ( CMceRtpSink& ) iAudioSink.Sink();
	rtpSink.SendSRL();
	iEngine.EngineStateChangedL();
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdSendSR::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdSendSR::Caption() const
	{
	return KCommandCaptionSendSR;
	}



// End of File
