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
#include "TCmdGetSinkStatus.h"
#include "CTcMCEContext.h"
#include <MceMediaStream.h>
#include <MceAudioStream.h>
#include <MceVideoStream.h>
#include <MceRtpSink.h>
#include <MceSpeakerSink.h>
#include <MceDisplaySink.h>

void TCmdGetSinkStatus::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------

	// Get stream
	CMceMediaStream* mediaStream = reinterpret_cast<CMceMediaStream*>
		(GetObjectForIdL(KStreamId, ETrue));		
	
	CMceMediaStream* stream = NULL;
	
	if ( mediaStream->Type() == KMceAudio )
		{
		stream = dynamic_cast<CMceAudioStream*>(mediaStream);
		}
	else if( mediaStream->Type() == KMceVideo )
		{
		stream = dynamic_cast<CMceVideoStream*>(mediaStream);		
		}
	else
		{
		User::Leave( KErrNotSupported );	
		}
			

	// ---------- Execution ----------------------------------------------------
	AddIdResponseL( KStreamId, *mediaStream );
	
	const RPointerArray<CMceMediaSink>& mediaSinks = 
			    	stream->Sinks();

	for( TInt i = mediaSinks.Count()-1; i >= 0; i-- )
		{
		const CMceMediaSink *sink = mediaSinks[i];
		if( sink->Type() == KMceDisplaySink)
			{
			TBool sinkState = sink->IsEnabled();
			_LIT8( KValueDisplaySink, "Display" );
			AddBooleanResponseL( KValueDisplaySink, sinkState);
			}
		else if( sink->Type() == KMceRTPSink)
			{
			TBool sinkState = sink->IsEnabled();
			AddBooleanResponseL( KValueSinkRTP, sinkState);
			}
		else if( sink->Type() == KMceSpeakerSink)
			{
			TBool sinkState = sink->IsEnabled();
			AddBooleanResponseL( KValueSinkSpeaker,sinkState);
			}
		else
			{
			// Keep pc-lint happy
			}
		}

	// ---------- Response creation --------------------------------------------

	AddIdResponseL( KStreamId, *stream ); 
	// Sink added to response during execution
	
	}
	
TBool TCmdGetSinkStatus::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("GetSinkStatus") );
	}

TTcCommandBase* TCmdGetSinkStatus::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetSinkStatus( aContext );
	}
