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



#include "TMCETestUIEngineCmdEnableAudioSource.h"
#include "CMCETestUIEngineSource.h"
#include "CMCETestUIEngineVideoSource.h"
#include "CMCETestUIQuestionDataQuery.h"
#include <MCEMediaSource.h>
#include <MCEMicSource.h>
#include <MCERtpSource.h>
#include <MCEFileSource.h>
#include "MCEAudioStream.h"
#include "MCEVideoStream.h"

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEnableStream::TMCETestUIEngineCmdEnableStream
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdEnableSource::TMCETestUIEngineCmdEnableSource(
            CMCETestUIEngine& aEngine,
		    CMCETestUIEngineSource& aAudioSource )
    : TMCETestUIEngineCmdBase( aEngine ),
      iAudioSource( aAudioSource )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEnableStream::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdEnableSource::ExecuteL()
	{
	iAudioSource.Source().EnableL();
	iEngine.EngineStateChangedL();
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEnableStream::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdEnableSource::Caption() const
	{
	return KCommandCaptionEnableSource;
	}


// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEnableVideoStream::TMCETestUIEngineCmdEnableVideoStream
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdEnableVideoSource::TMCETestUIEngineCmdEnableVideoSource(
            CMCETestUIEngine& aEngine,
		    CMCETestUIEngineVideoSource& aVideoSource )
    : TMCETestUIEngineCmdBase( aEngine ),
      iVideoSource( aVideoSource )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEnableVideoSource::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdEnableVideoSource::ExecuteL()
	{
	iVideoSource.Source().EnableL();
	iEngine.EngineStateChangedL();
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEnableVideoSource::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdEnableVideoSource::Caption() const
	{
	return KCommandCaptionEnableSource;
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEnableRtpSourceInActivityTimer::TMCETestUIEngineCmdEnableRtpSourceInActivityTimer
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdEnableRtpSourceInActivityTimer::TMCETestUIEngineCmdEnableRtpSourceInActivityTimer(
            CMCETestUIEngine& aEngine,
		    CMCETestUIEngineSource& aAudioSource )
    : TMCETestUIEngineCmdBase( aEngine ),
      iAudioSource( aAudioSource )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEnableRtpSourceInActivityTimer::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdEnableRtpSourceInActivityTimer::ExecuteL()
	{
	CMceRtpSource& rtpSource = ( CMceRtpSource& ) iAudioSource.Source();
	rtpSource.EnableInactivityTimerL(20000);
	iEngine.EngineStateChangedL();
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEnableRtpSourceInActivityTimer::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdEnableRtpSourceInActivityTimer::Caption() const
	{
	return KCommandCaptionEnableRtpSourceInActivityTimer;
	}
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdSendRR::TMCETestUIEngineCmdSendRR
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdSendRR::TMCETestUIEngineCmdSendRR(
            CMCETestUIEngine& aEngine,
		    CMCETestUIEngineSource& aAudioSource )
    : TMCETestUIEngineCmdBase( aEngine ),
      iAudioSource( aAudioSource )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdSendRR::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdSendRR::ExecuteL()
	{
	CMceRtpSource& rtpSource = ( CMceRtpSource& ) iAudioSource.Source();
	rtpSource.SendRRL();
	iEngine.EngineStateChangedL();
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdSendRR::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdSendRR::Caption() const
	{
	return KCommandCaptionSendRR;
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdAudioTranscode::TMCETestUIEngineCmdAudioTranscode
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdAudioTranscode::TMCETestUIEngineCmdAudioTranscode(
            CMCETestUIEngine& aEngine,
		    CMCETestUIEngineSource& aAudioSource )
    : TMCETestUIEngineCmdBase( aEngine ),
      iAudioSource( aAudioSource )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdAudioTranscode::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdAudioTranscode::ExecuteL()
	{
	CMceFileSource& fileSource = ( CMceFileSource& ) iAudioSource.Source();
	 // Solve recipient
   	CMCETestUIQuestionDataQuery* question = 
	    CMCETestUIQuestionDataQuery::NewLC();
	
	question->SetCaptionL( KUserQuestionInputTranscodeFileName );
	question->SetDefaultValueL(KDefaultTranscodeFile);
//	iEngine.AskQuestionL( *question );
//	TBufC<256> filename( question->Answer() );
	TBuf<256> filename;
	filename.Copy(_L("c:\\TranscodedFile.3gp"));
	CleanupStack::PopAndDestroy( question );
	fileSource.TranscodeL(filename);
	iEngine.EngineStateChangedL();
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdAudioTranscode::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdAudioTranscode::Caption() const
	{
	return KCommandCaptionTranscode;
	}
	
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdAudioCancelTranscode::TMCETestUIEngineCmdAudioCancelTranscode
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdAudioCancelTranscode::TMCETestUIEngineCmdAudioCancelTranscode(
            CMCETestUIEngine& aEngine,
		    CMCETestUIEngineSource& aAudioSource )
    : TMCETestUIEngineCmdBase( aEngine ),
      iAudioSource( aAudioSource )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdAudioCancelTranscode::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdAudioCancelTranscode::ExecuteL()
	{
	CMceFileSource& fileSource = ( CMceFileSource& ) iAudioSource.Source();
	fileSource.CancelTranscodeL();
	iEngine.EngineStateChangedL();
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCancelAudioTranscode::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdAudioCancelTranscode::Caption() const
	{
	return KCommandCaptionCancelTranscode;
	}
	
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdVideoTranscode::TMCETestUIEngineCmdVideoTranscode
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdVideoTranscode::TMCETestUIEngineCmdVideoTranscode(
            CMCETestUIEngine& aEngine,
		    CMCETestUIEngineVideoSource& aVideoSource )
    : TMCETestUIEngineCmdBase( aEngine ),
      iVideoSource( aVideoSource )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdVideoTranscode::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdVideoTranscode::ExecuteL()
	{
	CMceFileSource& fileSource = ( CMceFileSource& ) iVideoSource.Source();
	 // Solve recipient
	   CMCETestUIQuestionDataQuery* question = 
		    CMCETestUIQuestionDataQuery::NewLC();
		    
	question->SetCaptionL( KUserQuestionInputTranscodeFileName );
	question->SetDefaultValueL(KDefaultTranscodeFile);
//	iEngine.AskQuestionL( *question );
//	TBufC<256> filename( question->Answer() );
	TBuf<256> filename;
	filename.Copy(_L("c:\\TranscodedFile.3gp"));
	CleanupStack::PopAndDestroy( question );
	fileSource.TranscodeL(filename);
	iEngine.EngineStateChangedL();
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdVideoTranscode::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdVideoTranscode::Caption() const
	{
	return KCommandCaptionTranscode;
	}
	
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdVideoCancelTranscode::TMCETestUIEngineCmdVideoCancelTranscode
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdVideoCancelTranscode::TMCETestUIEngineCmdVideoCancelTranscode(
            CMCETestUIEngine& aEngine,
		    CMCETestUIEngineVideoSource& aVideoSource )
    : TMCETestUIEngineCmdBase( aEngine ),
      iVideoSource( aVideoSource )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdVideoCancelTranscode::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdVideoCancelTranscode::ExecuteL()
	{
	CMceFileSource& fileSource = ( CMceFileSource& ) iVideoSource.Source();
	fileSource.CancelTranscodeL();
	iEngine.EngineStateChangedL();
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdVideoCancelTranscode::Caption
// ----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdVideoCancelTranscode::Caption() const
	{
	return KCommandCaptionCancelTranscode;
	}
	
/*****************************DTMF********************************************/

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdStartDtmfToneL::TMCETestUIEngineCmdStartDtmfToneL
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdStartDtmfToneL::TMCETestUIEngineCmdStartDtmfToneL(
            CMCETestUIEngine& aEngine,
		    CMCETestUIEngineSource& aMediaSource )
    : TMCETestUIEngineCmdBase( aEngine ),
      iMediaSource( aMediaSource )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdVideoCancelTranscode::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdStartDtmfToneL::ExecuteL()
	{
	TChar tone;
	iEngine.StartDTMFToneL( tone );
	TRAPD( err, iMediaSource.Source().StartDtmfToneL( tone ) );
	if( err != KErrNone )
	    {
	    iEngine.NotifyUser( KNotifyDTMFFailed );
	    }
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdVideoCancelTranscode::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdStartDtmfToneL::Caption() const
	{
	return KCommandCaptionDTMFStart;
	}
	
/*************************************************************************/

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdVideoCancelTranscode::TMCETestUIEngineCmdVideoCancelTranscode
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdStopDtmfToneL::TMCETestUIEngineCmdStopDtmfToneL(
            CMCETestUIEngine& aEngine,
		    CMCETestUIEngineSource& aMediaSource )
    : TMCETestUIEngineCmdBase( aEngine ),
      iMediaSource( aMediaSource )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdVideoCancelTranscode::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdStopDtmfToneL::ExecuteL()
	{
	TRAPD( err, iMediaSource.Source().StopDtmfToneL());
	if( err != KErrNone )
	    {
	    iEngine.NotifyUser( KNotifyDTMFFailed );
	    }
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdVideoCancelTranscode::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdStopDtmfToneL::Caption() const
	{
	return KCommandCaptionDTMFStop;
	}

/*************************************************************************/

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdSendDtmfToneL::TMCETestUIEngineCmdVideoCancelTranscode
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdSendDtmfToneL::TMCETestUIEngineCmdSendDtmfToneL(
            CMCETestUIEngine& aEngine,
		    CMCETestUIEngineSource& aMediaSource )
    : TMCETestUIEngineCmdBase( aEngine ),
      iMediaSource( aMediaSource )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdSendDtmfToneL::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdSendDtmfToneL::ExecuteL()
	{
	TChar tone;
	TInt interval;
	iEngine.SendDTMFToneL( tone, interval );
	TTimeIntervalMicroSeconds32 msinterval = interval * 1000;
	
	TRAPD( err, iMediaSource.Source().SendDtmfToneL( tone, msinterval));
	if( err != KErrNone )
	    {
	    iEngine.NotifyUser( KNotifyDTMFFailed );
	    }
	    
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdSendDtmfToneL::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdSendDtmfToneL::Caption() const
	{
	return KCommandCaptionDTMFSendTone;
	}

/*************************************************************************/


// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdSendDtmfToneSequenceL::TMCETestUIEngineCmdSendDtmfToneSequenceL
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdSendDtmfToneSequenceL::TMCETestUIEngineCmdSendDtmfToneSequenceL(
            CMCETestUIEngine& aEngine,
		    CMCETestUIEngineSource& aMediaSource )
    : TMCETestUIEngineCmdBase( aEngine ),
      iMediaSource( aMediaSource )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdVideoCancelTranscode::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdSendDtmfToneSequenceL::ExecuteL()
	{
    TBuf<50> buf;
	iEngine.SendDTMFToneSequenceL( buf );
	
	TRAPD( err, iMediaSource.Source().SendDtmfToneSequenceL( buf ));
	if( err != KErrNone )
	    {
	    iEngine.NotifyUser( KNotifyDTMFFailed );
	    }
	    
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdVideoCancelTranscode::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdSendDtmfToneSequenceL::Caption() const
	{
	return KCommandCaptionDTMFSendSequence;
	}
	



// End of File
