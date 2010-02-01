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
#include "TCmdGetSourceStatus.h"
#include "CTcMCEContext.h"
#include <MceMediaStream.h>
#include <MceAudioStream.h>
#include <MceVideoStream.h>

#include <MceMediaSource.h>
#include <MceCameraSource.h>
#include <MceFileSource.h>
#include <MceMicSource.h>
#include <MceRtpSource.h>

void TCmdGetSourceStatus::ExecuteL()
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
	
	// Get source
	const CMceMediaSource* source = stream->Source();
	
	TBool sourceState = EFalse;
	
	if( source->Type() == KMceCameraSource)
	{
		sourceState = source->IsEnabled();
		AddBooleanResponseL( KValueSourceCamera, sourceState);
	}
	else if( source->Type() == KMceFileSource)
	{
	    sourceState = source->IsEnabled();
		AddBooleanResponseL( KValueSourceFile, sourceState);
	}
	else if( source->Type() == KMceMicSource)
	{
		sourceState = source->IsEnabled();
		AddBooleanResponseL( KValueSourceMic,sourceState);
	}
	else if( source->Type() == KMceRTPSource)
	{
		sourceState = source->IsEnabled();
		AddBooleanResponseL( KValueSourceRTP,sourceState);
	}
	else
	{
		
		AddBooleanResponseL( KValueSourceUnknown,sourceState);
	}


	// ---------- Response creation --------------------------------------------

	AddIdResponseL( KStreamId, *stream ); 
	// Sink added to response during execution
	
	}
	
TBool TCmdGetSourceStatus::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("GetSourceStatus") );
	}

TTcCommandBase* TCmdGetSourceStatus::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetSourceStatus( aContext );
	}
