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
#include "TCmdAddAudioStream.h"
#include "CTcMCEContext.h"

#include <badesca.h>
#include <MceInSession.h>
#include <MCEAudioStream.h>
#include <MCEMicSource.h>
#include <MceRtpSink.h>
#include <MceRtpSource.h>
#include <MCESpeakerSink.h>
#include <MCEAudioCodec.h>
#include <MCEAMRCodec.h>

#include <mcefilesource.h>
void TCmdAddAudioStream::ExecuteL()
	{	

	// ---------- Setup --------------------------------------------------------

	// Get session
	CMceSession* session = reinterpret_cast<CMceSession*>
		(GetObjectForIdL(KSessionId, ETrue));	

	// Get existing stream
	CMceMediaStream* mainStream = reinterpret_cast<CMceMediaStream*>
		(GetObjectForIdL(KStreamId, EFalse));	

    TBool suppressRtcp = ExtractBooleanL( KParamSuppressRtcp, EFalse );
		
	//source path for audiofile
	TPtrC8 pathAudioFile = ExtractTextL( KParamAudioFileSource, EFalse );
	
	//get path for media file sink
	TPtrC8 mediaFileSink = ExtractTextL( KParamMediaFileSink, EFalse );
	// ---------- Execution ----------------------------------------------------

	CMceAudioStream* audioStream = CMceAudioStream::NewLC();
	
	
	// Add source
	TPtrC8 source = ExtractTextL( KParamSource, EFalse );
	if (source == KValueSourceRTP )
		{
		CMceRtpSource* rtpSource = CMceRtpSource::NewLC();
		audioStream->SetSourceL(rtpSource);
		CleanupStack::Pop(rtpSource);			
		}
	else if (source == KValueSourceMic )
		{
		CMceMicSource* mic = CMceMicSource::NewLC();
		audioStream->SetSourceL(mic);	
		CleanupStack::Pop(mic);
		}
	else if (pathAudioFile.Compare( KNullDesC8 ) != 0)
		{
		// If source path was defined use the file audio specified in path
		iSessionHelper.SetMediaFileSourceL( iContext.MCEManager(), *audioStream, pathAudioFile );
		}
    else
	    {
	    if( mediaFileSink.Compare( KNullDesC8 ) == 0 )
	    	User::Leave( KErrNotSupported );
	    }	

		
	// Add sinks
	CDesC8Array* sinks = ExtractArrayL( KParamSinks, EFalse );

	if ( sinks )
		{	
		CleanupStack::PushL( sinks );
		for ( TInt i = 0; i < sinks->Count(); i++ )
			{
			if ( (*sinks)[i] == KValueSinkRTP )
				{
				//CMceRtpSink* rtpSink = CMceRtpSink::NewLC();
				//enable all RtpSink with RTCP
				CMceRtpSink* rtpSink = CMceRtpSink::NewL(suppressRtcp);
				CleanupStack::PushL(rtpSink);
				audioStream->AddSinkL(rtpSink);
				CleanupStack::Pop(rtpSink);
				}
			else if ( (*sinks)[i] == KValueSinkSpeaker )
				{
				CMceSpeakerSink* speaker = CMceSpeakerSink::NewLC();	
				audioStream->AddSinkL(speaker);
				CleanupStack::Pop(speaker);				
				}
			else if ( (*sinks)[i] == KValueSinkFile )
				{
				const RPointerArray<CMceMediaStream>& streams = session->Streams();
				for ( TInt i = 0; i < streams.Count(); ++i )
					{        
					CMceMediaStream* tmpStream;
					tmpStream = streams[i];  
					if ( tmpStream->Type() == KMceAudio )
						{						
						//use the same source which was used for outgoing audio stream	
						if( tmpStream->Source()->Type() != KMceRTPSource )
							{
							audioStream->SetSourceL( tmpStream->Source() );
							}
						else
							{
							if(tmpStream->BoundStream())
								if( tmpStream->BoundStreamL().Source()->Type() != KMceRTPSource )
									audioStream->SetSourceL(tmpStream->BoundStreamL().Source());
							}			
						break;
						}
					}
				if( mediaFileSink.Compare( KNullDesC8 ) != 0 )
					{
					iSessionHelper.SetMediaFileSinkL(*audioStream, mediaFileSink);
					}
				else
					{
					iSessionHelper.SetMediaFileSinkL(*audioStream, TPtrC8( KTestAudioFileName ));
					}
				session->AddStreamL(audioStream);
				}
	        else
	            {
	            User::Leave( KErrNotSupported );
	            }
			}
		CleanupStack::PopAndDestroy( sinks );
		}

		
	// Modify codecs
	CDesC8Array* codecs = ExtractArrayL( KParamCodecs, EFalse );
	
	if ( codecs )
		{
		CleanupStack::PushL( codecs );
		if ( codecs->Count() > 0)
			{
			const RPointerArray<CMceAudioCodec>& streamCodecs = 
			    audioStream->Codecs();
			
			if ( (*codecs)[0] == KValueCodecNone )
				{
			    // Remove all codecs from stream
                for (TInt i = 0; i < streamCodecs.Count(); i++ )
                    {
                    // No implementation yet in MMCE
                    audioStream->RemoveCodecL( *(streamCodecs[i]) );
                    }
				}
				
			if ( (*codecs)[0] != KValueCodecNone &&
	 		   	 (*codecs)[0] != KValueCodecAll )
				{	
				for (TInt i = 0; i < codecs->Count(); i++ )
					{
					audioStream->AddCodecL( 
					    GetAudioCodecCloneL( (*codecs)[i] ) );
					}
				
				}
			}
		CleanupStack::PopAndDestroy( codecs );
		}
	
	// Add streams
	if (mainStream)
		{
		mainStream->BindL( audioStream );
		//if there are no codecs in the stream already, all the codecs will be
		//added to stream by mce.
		//Note: streams needs to be bound before adding stream to session
		session->AddStreamL( mainStream );
		}
	else
		{
		//NOOP	
		}
	
	CleanupStack::Pop( audioStream );
	
	// ---------- Response creation --------------------------------------------
 
	AddIdResponseL(KSessionId, session); // uses CleanupStack internally
	AddIdResponseL(KStreamId, *audioStream); 
	
	}
	
TBool TCmdAddAudioStream::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("AddAudioStream") );
	}

TTcCommandBase* TCmdAddAudioStream::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdAddAudioStream( aContext );
	}
	
	
CMceAudioCodec* TCmdAddAudioStream::GetAudioCodecCloneL( const TDesC8& aParamCodecName )
    {
    
    const RPointerArray<const CMceAudioCodec>& managerAudioCodecs =
        iContext.MCEManager().SupportedAudioCodecs();
    for ( TInt i = 0; i < managerAudioCodecs.Count(); i++ )
        {
        const CMceAudioCodec* managerAudioCodec = managerAudioCodecs[i];
        
        if ( managerAudioCodec->SdpName() == aParamCodecName )
            {
            return managerAudioCodec->CloneL();
            }
        
        }
    
    // If the requested codec is not supported
    User::Leave( KErrNotSupported );
    return NULL; // To keep compiler happy
    }
