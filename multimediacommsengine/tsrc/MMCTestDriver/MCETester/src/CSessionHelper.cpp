/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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



#include "CSessionHelper.h"

#include "MceMediaStream.h"
#include "MceAudioStream.h"
#include "MceVideoStream.h"
#include "MCERtpSource.h"
#include "MceCodec.h"
#include "mcefilesource.h"
#include "mcefilesink.h"
#include <MCEManager.h>

#include "mcedefs.h"

#include <mcefilesource.h>

const TUint KAudioPriorityPocAudioUplink        = 100;//95;
const TUint KAudioPriorityPocAudioDownlink      = 100;//95;
const TUint KAudioPrefPocAudioUplink            = 0x05230001;//0x05130001;
const TUint KAudioPrefPocAudioDownlink          = 0x05210001; //0x05120001; 

const TUint KDtmfAudioPriority					= 45;
const TUint KDtmfAudioPref						= 0x00150001;

void CSessionHelper::SetCodecPrioritiesL(CMceCodec& aCodec, priorities aPri)
	{
	if (aPri == downlink) 
		{
		aCodec.SetMMFPriorityL( KAudioPriorityPocAudioDownlink );
	    aCodec.SetMMFPriorityPreferenceL( KAudioPrefPocAudioDownlink ); 
		}
	else if (aPri == uplink) 
		{
		aCodec.SetMMFPriorityL( KAudioPriorityPocAudioUplink );
	    aCodec.SetMMFPriorityPreferenceL( KAudioPrefPocAudioUplink );
		}
	else if (aPri == dtmf)
		{
		//TODO: set dtmf priorities
		aCodec.SetMMFPriorityL( KDtmfAudioPriority );
	    aCodec.SetMMFPriorityPreferenceL( KDtmfAudioPref );	
		}
	else
		User::LeaveIfError(KErrUnknown);
	}

void
CSessionHelper::SetCodecPriPrefL(CMceCodec& aCodec, TUint aPri, TUint aPref)
	{
	aCodec.SetMMFPriorityL( aPri );
	aCodec.SetMMFPriorityPreferenceL( aPref );
	}
	
/*
The idea here is 
to set the same payload type as the main codec for the keep alive timer 
later on, we can then check with TTCN is MCE using the correct codec.
*/
void CSessionHelper::SetKeepaliveL(CMceCodec& aCodec, TInt aKeepaliveTimeSec)
	{
	aCodec.SetKeepAliveTimerL( aKeepaliveTimeSec );
	if (!aCodec.SdpName().Compare(KMceSDPNameAMR))
		{
		aCodec.SetKeepAlivePayloadTypeL( 96 );
		}
	else if(!aCodec.SdpName().Compare(KMceSDPNameG729))
		{
		aCodec.SetKeepAlivePayloadTypeL( 18 );
		}
	else if(!aCodec.SdpName().Compare(KMceSDPNameiLBC))
		{
		aCodec.SetKeepAlivePayloadTypeL( 97 );
		}
	else if(!aCodec.SdpName().Compare(KMceSDPNameAMRWB))
		{
		aCodec.SetKeepAlivePayloadTypeL( 100 );
		}
	else
		{
		// Keep pc-lint happy
		}
	}

void CSessionHelper::SetRtpInactivityTimerL(CMceMediaStream& aStream, TInt aInactivityTimer)
	{
	
	//for audio
	 if ( aStream.Type() == KMceAudio )
		{	
		CMceAudioStream* audioStream = 
								static_cast<CMceAudioStream*>(&aStream);
								
		if ( audioStream->Source()->Type() == KMceRTPSource )
			{
			//set inactivity timer
			CMceRtpSource* rtpSource = static_cast<CMceRtpSource*>(audioStream->Source());
			rtpSource->EnableInactivityTimerL(aInactivityTimer * 1000);
			}
		const CMceAudioStream* boundAudioStream = NULL;
		if (audioStream->BoundStream()) 
			{
			boundAudioStream = static_cast<CMceAudioStream*>(&audioStream->BoundStreamL());
			if ( boundAudioStream->Source()->Type() == KMceRTPSource )
				{
				//set
				CMceRtpSource* rtpSource = static_cast<CMceRtpSource*>(boundAudioStream->Source());
				rtpSource->EnableInactivityTimerL(aInactivityTimer * 1000);
				}
			}
	
		}
	 
	 //for video
	 if ( aStream.Type() == KMceVideo )
		{	
		CMceVideoStream* videoStream = 
								static_cast<CMceVideoStream*>(&aStream);
								
		if ( videoStream->Source()->Type() == KMceRTPSource )
			{
			//set inactivity timer
			CMceRtpSource* rtpSource = static_cast<CMceRtpSource*>(videoStream->Source());
			rtpSource->EnableInactivityTimerL(aInactivityTimer * 1000);
			}
		const CMceVideoStream* boundVideoStream = NULL;
		if (videoStream->BoundStream()) 
			{
			boundVideoStream = static_cast<CMceVideoStream*>(&videoStream->BoundStreamL());
			if ( boundVideoStream->Source()->Type() == KMceRTPSource )
				{
				//set
				CMceRtpSource* rtpSource = static_cast<CMceRtpSource*>(boundVideoStream->Source());
				rtpSource->EnableInactivityTimerL(aInactivityTimer * 1000);
				}
			}
		}
		
	}

// ----------------------------------------------------------------------------
// CSessionHelper::SetMediaFileSourceL
// ----------------------------------------------------------------------------
void CSessionHelper::SetMediaFileSourceL(CMceManager& aManager, CMceMediaStream& aStream, TPtrC8 aFilename )
	{
	TFileName filename;
	filename.Copy(aFilename);
	CMceFileSource* fileSource = CMceFileSource::NewL( aManager, filename );
	if(!fileSource)
		User::Leave( KErrNotFound );
	CleanupStack::PushL(fileSource);

   	fileSource->SetPositionL( TTimeIntervalMicroSeconds( 1 ) );		
	fileSource->EnableL();
	aStream.SetSourceL( fileSource );
	aStream.SynchronizeWithFileL(*fileSource);

	CleanupStack::Pop(fileSource);
	}

// ----------------------------------------------------------------------------
// CSessionHelper::SetMediaFileSinkL
// ----------------------------------------------------------------------------
void CSessionHelper::SetMediaFileSinkL(CMceMediaStream& aStream, TPtrC8 aFilename )
	{
	TFileName filename;
	filename.Copy(aFilename);
	CMceFileSink* fileSink = CMceFileSink::NewL( filename );
	if(!fileSink)
		User::Leave( KErrNotFound );
	
	CleanupStack::PushL(fileSink);
//	fileSink->EnableL();
	aStream.AddSinkL( fileSink );
	
	CleanupStack::Pop(fileSink);
	}
