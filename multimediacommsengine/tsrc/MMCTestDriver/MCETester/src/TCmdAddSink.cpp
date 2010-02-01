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
#include "TCmdAddSink.h"
#include "CTcMCEContext.h"
#include <MceMediaStream.h>
#include <MCEAudioStream.h>
#include <MceRtpSink.h>
#include <MCESpeakerSink.h>
#include <MCERtpSource.h>

void TCmdAddSink::ExecuteL()
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
	TPtrC8 sink = ExtractTextL( KParamSink, ETrue );

	//get path for media file sink
	TPtrC8 mediaFileSink = ExtractTextL( KParamMediaFileSink, EFalse );
	
	// ---------- Execution ----------------------------------------------------

	if (sink == KValueSinkRTP )
		{
		//CMceRtpSink* rtpSink = CMceRtpSink::NewLC();
		CMceRtpSink* rtpSink = CMceRtpSink::NewL(EFalse);
		CleanupStack::PushL(rtpSink);
		audioStream->AddSinkL(rtpSink);
		CleanupStack::Pop(rtpSink);	
		AddIdResponseL( KSinkId, *rtpSink );		
		}
	else if (sink == KValueSinkSpeaker )
		{
		CMceSpeakerSink* speaker = CMceSpeakerSink::NewLC();
		audioStream->AddSinkL( speaker );	
		CleanupStack::Pop( speaker );
		AddIdResponseL( KSinkId, *speaker );
		}
	else if ( sink == KValueSinkFile )
		{
		CMceAudioStream* audioOutStreamFile = CMceAudioStream::NewL();
		CleanupStack::PushL( audioOutStreamFile );

		//use the same source which was used for outgoing audio stream	
		if( audioStream->Source()->Type() != KMceRTPSource )
			{
			audioOutStreamFile->SetSourceL( audioStream->Source() );
			}
		else
			{
			if(audioStream->BoundStream())
				if( audioStream->BoundStreamL().Source()->Type() != KMceRTPSource )
					audioOutStreamFile->SetSourceL(audioStream->BoundStreamL().Source());
			}
		
		if( mediaFileSink.Compare( KNullDesC8 ) != 0 )
			{
			iSessionHelper.SetMediaFileSinkL(*audioOutStreamFile, mediaFileSink);
			}
		else
			{
			iSessionHelper.SetMediaFileSinkL(*audioOutStreamFile, TPtrC8( KTestAudioFileName ));
			}
		//add the stream to the session
		audioStream->Session()->AddStreamL(audioOutStreamFile);
		CleanupStack::Pop( audioOutStreamFile );
		}
	else 
		{
		User::Leave( KErrNotSupported );
		}

	// ---------- Response creation --------------------------------------------

	AddIdResponseL( KStreamId, *audioStream ); 
	// Sink added to response during execution
	
	}
	
TBool TCmdAddSink::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("AddSink") );
	}

TTcCommandBase* TCmdAddSink::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdAddSink( aContext );
	}
