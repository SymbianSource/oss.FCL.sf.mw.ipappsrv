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
#include "TCmdAddVideoStream.h"
#include "CTcMCEContext.h"

#include <badesca.h>
#include <MceInSession.h>
#include <MCEVideoStream.h>
#include <mcecamerasource.h>
#include <MceRtpSink.h>
#include <MceRtpSource.h>
#include <mcedisplaysink.h>
#include <MCEVideoCodec.h>
#include <mceh263codec.h>
#include <mcefilesource.h>

void TCmdAddVideoStream::ExecuteL()
	{	

	// ---------- Setup --------------------------------------------------------

	// Get session
	CMceSession* session = reinterpret_cast<CMceSession*>
		(GetObjectForIdL(KSessionId, ETrue));	

	// Get existing stream
	CMceMediaStream* mainStream = reinterpret_cast<CMceMediaStream*>
		(GetObjectForIdL(KStreamId, EFalse));	

	//source path for audiofile
	TPtrC8 pathAudioFile = ExtractTextL( KParamAudioFileSource, EFalse );
	
	//get path for media file sink
	TPtrC8 mediaFileSink = ExtractTextL( KParamMediaFileSink, EFalse );
	// ---------- Execution ----------------------------------------------------

	CMceVideoStream* videoStream = CMceVideoStream::NewLC();
	
	
	// Add source
	TPtrC8 source = ExtractTextL( KParamSource, EFalse );
	if (source == KValueSourceRTP )
		{
		CMceRtpSource* rtpSource = CMceRtpSource::NewLC();
		videoStream->SetSourceL(rtpSource);
		CleanupStack::Pop(rtpSource);			
		}
	else if (source == KValueSourceCamera )
		{
		CMceCameraSource* camera = CMceCameraSource::NewLC( iContext.MCEManager() );
		videoStream->SetSourceL(camera);	
		CleanupStack::Pop(camera);
		}
	else if (pathAudioFile.Compare( KNullDesC8 ) != 0)
		{

		// If source path was defined use the file audio specified in path
		iSessionHelper.SetMediaFileSourceL( iContext.MCEManager(), *videoStream, pathAudioFile );		
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
				CMceRtpSink* rtpSink = CMceRtpSink::NewL(EFalse);
				CleanupStack::PushL(rtpSink);
				videoStream->AddSinkL(rtpSink);
				CleanupStack::Pop(rtpSink);
				}
			else if ( (*sinks)[i] == KValueSinkSpeaker )
				{
				CMceDisplaySink* display = CMceDisplaySink::NewLC( iContext.MCEManager() );	
				videoStream->AddSinkL(display);
				CleanupStack::Pop(display);				
				}
			else if ( (*sinks)[i] == KValueSinkFile )
				{				
				const RPointerArray<CMceMediaStream>& streams = session->Streams();				
				for ( TInt i = 0; i < streams.Count(); ++i )
			        {        
			        CMceMediaStream* tmpStream;
			        tmpStream = streams[i];        
					if ( tmpStream->Type() == KMceVideo )
						{
						//use the same source which was used for outgoing video stream	
						if( tmpStream->Source()->Type() != KMceRTPSource )
							{
							videoStream->SetSourceL( tmpStream->Source() );
							}
						else
							{
							if( tmpStream->BoundStream() )
								if( tmpStream->BoundStreamL().Source()->Type() != KMceRTPSource )
									videoStream->SetSourceL(tmpStream->BoundStreamL().Source());
							}			
						break;	//only one file sink can be added for testing
						}			        	
			        }
				if( mediaFileSink.Compare( KNullDesC8 ) != 0 )
					{
					iSessionHelper.SetMediaFileSinkL(*videoStream, mediaFileSink);
					}
				else
					{
					iSessionHelper.SetMediaFileSinkL(*videoStream, TPtrC8( KTestAudioFileName ));
					}
				session->AddStreamL(videoStream);
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
			const RPointerArray<CMceVideoCodec>& streamCodecs = 
			    videoStream->Codecs();
			
			if ( (*codecs)[0] == KValueCodecNone )
				{
			    // Remove all codecs from stream
                for (TInt i = 0; i < streamCodecs.Count(); i++ )
                    {
                    // No implementation yet in MMCE
                    videoStream->RemoveCodecL( *(streamCodecs[i]) );
                    }
				}
				
			if ( (*codecs)[0] != KValueCodecNone &&
	 		   	 (*codecs)[0] != KValueCodecAll )
				{	
				for (TInt i = 0; i < codecs->Count(); i++ )
					{
					videoStream->AddCodecL( 
					    GetVideoCodecCloneL( (*codecs)[i] ) );
					}
				
				}
			}
		CleanupStack::PopAndDestroy( codecs );
		}
	
	// Add streams
	if (mainStream)
		{
		mainStream->BindL( videoStream );
		//if there are no codecs in the stream already, all the codecs will be
		//added to stream by mce.
		//Note: streams needs to be bound before adding stream to session
		session->AddStreamL( mainStream );
		}
	else
		{
		//NOOP	
		}
	
	CleanupStack::Pop( videoStream );
	
	// ---------- Response creation --------------------------------------------
 
	AddIdResponseL(KSessionId, session); // uses CleanupStack internally
	AddIdResponseL(KStreamId, *videoStream); 
	
	}
	
TBool TCmdAddVideoStream::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("AddVideoStream") );
	}

TTcCommandBase* TCmdAddVideoStream::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdAddVideoStream( aContext );
	}
	
	
CMceVideoCodec* TCmdAddVideoStream::GetVideoCodecCloneL( const TDesC8& aParamCodecName )
    {
    
    const RPointerArray<const CMceVideoCodec>& managerVideoCodecs =
        iContext.MCEManager().SupportedVideoCodecs();
    for ( TInt i = 0; i < managerVideoCodecs.Count(); i++ )
        {
        const CMceVideoCodec* managerVideoCodec = managerVideoCodecs[i];
        
        if ( managerVideoCodec->SdpName() == aParamCodecName )
            {
            return managerVideoCodec->CloneL();
            }
        
        }
    
    // If the requested codec is not supported
    User::Leave( KErrNotSupported );
    return NULL; // To keep compiler happy
    }
