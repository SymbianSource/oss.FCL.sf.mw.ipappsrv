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



#include "TMCETestUIEngineCmdDisableStream.h"
#include "CMCETestUIEngineAudioStream.h"
#include "MCEAudioStream.h"
#include "CMCETestUIEngineVideoStream.h"
#include "MCEVideoStream.h"
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDisableStream::TMCETestUIEngineCmdDisableStream
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdDisableStream::TMCETestUIEngineCmdDisableStream(
            CMCETestUIEngine& aEngine,
		    CMCETestUIEngineAudioStream& aAudioStream )
    : TMCETestUIEngineCmdBase( aEngine ),
      iAudioStream( aAudioStream )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDisableStream::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdDisableStream::ExecuteL()
	{
	iAudioStream.AudioStream().DisableL();
	iEngine.EngineStateChangedL();
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDisableStream::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdDisableStream::Caption() const
	{
	return KCommandCaptionDisableStream;
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDisableVideoStream::TMCETestUIEngineCmdDisableVideoStream
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdDisableVideoStream::TMCETestUIEngineCmdDisableVideoStream(
            CMCETestUIEngine& aEngine,
		    CMCETestUIEngineVideoStream& aVideoStream )
    : TMCETestUIEngineCmdBase( aEngine ),
      iVideoStream( aVideoStream )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDisableVideoStream::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdDisableVideoStream::ExecuteL()
	{
	iVideoStream.VideoStream().DisableL();
	iEngine.EngineStateChangedL();
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDisableVideoStream::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdDisableVideoStream::Caption() const
	{
	return KCommandCaptionDisableStream;
	}


// End of File
