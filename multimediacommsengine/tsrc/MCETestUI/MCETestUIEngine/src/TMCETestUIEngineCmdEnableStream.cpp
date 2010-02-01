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



#include "TMCETestUIEngineCmdEnableStream.h"
#include "CMCETestUIEngineAudioStream.h"
#include "MCEAudioStream.h"
#include "CMCETestUIEngineVideoStream.h"
#include "MCEVideoStream.h"

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEnableStream::TMCETestUIEngineCmdEnableStream
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdEnableStream::TMCETestUIEngineCmdEnableStream(
            CMCETestUIEngine& aEngine,
		    CMCETestUIEngineAudioStream& aAudioStream )
    : TMCETestUIEngineCmdBase( aEngine ),
      iAudioStream( aAudioStream )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEnableStream::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdEnableStream::ExecuteL()
	{
	iAudioStream.AudioStream().EnableL();
	iEngine.EngineStateChangedL();
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEnableStream::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdEnableStream::Caption() const
	{
	return KCommandCaptionEnableStream;
	}


// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEnableVideoStream::TMCETestUIEngineCmdEnableVideoStream
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdEnableVideoStream::TMCETestUIEngineCmdEnableVideoStream(
            CMCETestUIEngine& aEngine,
		    CMCETestUIEngineVideoStream& aVideoStream )
    : TMCETestUIEngineCmdBase( aEngine ),
      iVideoStream( aVideoStream )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEnableVideoStream::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdEnableVideoStream::ExecuteL()
	{
	iVideoStream.VideoStream().EnableL();
	iEngine.EngineStateChangedL();
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEnableVideoStream::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdEnableVideoStream::Caption() const
	{
	return KCommandCaptionEnableStream;
	}

// End of File
