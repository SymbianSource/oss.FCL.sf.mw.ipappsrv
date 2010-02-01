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
* Description:    Implementation
*
*/



#include "MCEConstants.h"
#include "TCmdSetSource.h"
#include "CTcMCEContext.h"
#include <MceMediaStream.h>
#include <MCEAudioStream.h>
#include <MceRtpSource.h>
#include <MCEMicSource.h>

void TCmdSetSource::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------

	// Get stream
	CMceMediaStream* mediaStream = reinterpret_cast<CMceMediaStream*>
		(GetObjectForIdL(KStreamId, ETrue));		

	if ( mediaStream->Type() != KMceAudio )
		{
		User::Leave( KErrNotSupported );
		}
	
	CMceAudioStream* audioStream = static_cast<CMceAudioStream*>(mediaStream);

	// Get type
	TPtrC8 source = ExtractTextL( KParamSource, ETrue );
	
	//get the inactivity timer value for rtp source
	TInt rtpInactivityTimer = ExtractIntegerL(KParamRTPInactivityTimer, 0, EFalse);

	// ---------- Execution ----------------------------------------------------

	if (source == KValueSourceRTP )
		{
		CMceRtpSource* rtpSource = NULL;
		
		if(rtpInactivityTimer != 0)
			rtpSource = CMceRtpSource::NewLC(KMceJitterBufferLength, KMceJitterBufferTreshold, rtpInactivityTimer * 1000);
		else	
			rtpSource = CMceRtpSource::NewLC();
		
		audioStream->SetSourceL(rtpSource);
		CleanupStack::Pop(rtpSource);
		AddIdResponseL( KSourceId, *rtpSource );			
		}
	else if (source == KValueSourceMic )
		{
		CMceMicSource* mic = CMceMicSource::NewLC();
		audioStream->SetSourceL(mic);	
		CleanupStack::Pop(mic);
		AddIdResponseL( KSourceId, *mic );
		}
	else 
		{
		User::Leave( KErrNotSupported );
		}

	// ---------- Response creation --------------------------------------------
 
	AddIdResponseL( KStreamId, *audioStream ); 
	// Source added to response during execution

	}
	
TBool TCmdSetSource::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("SetSource") );
	}

TTcCommandBase* TCmdSetSource::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSetSource( aContext );
	}
