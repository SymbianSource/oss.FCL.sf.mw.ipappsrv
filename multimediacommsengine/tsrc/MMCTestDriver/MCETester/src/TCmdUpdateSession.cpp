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
#include "TCmdUpdateSession.h"
#include "CTcMCEContext.h"

#include <mcedefs.h>
#include <MCESession.h>
#include <MceMediaStream.h>
#include <MceAudioStream.h>
#include <MceVideoStream.h>

#include <MCERtpSource.h>
#include <MCEMicSource.h>
#include <mcefilesource.h>
#include <MceMediaSink.h>
#include <MCESpeakerSink.h>
#include <mcedisplaysink.h>
#include <mcefilesink.h>
#include <MCEAMRCodec.h>
#include <MceCodec.h>
#include <MceVideoCodec.h>
#include <MceAvcCodec.h>

void TCmdUpdateSession::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------

	// Get session
	CMceSession* session = 
		reinterpret_cast<CMceSession*>(GetObjectForIdL(KSessionId, ETrue));
	const RPointerArray<CMceMediaStream>& streams = session->Streams();

	TInt keepAliveTimer = ExtractIntegerL( KParamKeepAliveValue, 0, EFalse );
	keepAliveTimer = Max( 0, keepAliveTimer );
	keepAliveTimer = Min( KMaxTUint8, keepAliveTimer );
	
	TBool setDtmfPri = ExtractBooleanL( KParamSetDtmfPri, EFalse );	
	
	_LIT8( DTMF, "telephone-event" );	
	//source path for audiofile
	TPtrC8 pathAudioFile = ExtractTextL( KParamAudioFileSource, EFalse );
	
	//get path for media file sink
	TPtrC8 mediaFileSink = ExtractTextL( KParamMediaFileSink, EFalse );
	
	TPtrC8 selectedCodec = ExtractTextL( KParamSelectCodec, EFalse );
	
	//get the inactivity timer value for rtp source
	TInt rtpInactivityTimer = ExtractIntegerL(KParamRTPInactivityTimer, 0, EFalse);
		
	for ( TInt i = 0; i < streams.Count(); ++i )
        {        
        CMceMediaStream* tmpStream;
        tmpStream = streams[i];        
        // Setting the Audio Stream
        if ( streams[i]->Type() == KMceAudio )
            {

            CMceAudioStream* audioStream = 
                                    static_cast<CMceAudioStream*>(streams[i]);                                    
			
            //setting the media file source for the stream
            if (pathAudioFile.Compare( KNullDesC8 ) != 0) 
	            {		            
	           //Setting FileSource if the parameter has been set
		        if ( streams[i]->Source()->Type() == KMceMicSource) 
			        {
			        iSessionHelper.SetMediaFileSourceL( iContext.MCEManager(), *tmpStream, pathAudioFile);
		    	    }
		    	else if ( audioStream->BoundStream() ) 
			    	{
			    	iSessionHelper.SetMediaFileSourceL( iContext.MCEManager(), tmpStream->BoundStreamL(), pathAudioFile);
			    	}
				else
					{
					// Keep pc-lint happy
					}
	            }            
         
          	const RPointerArray<CMceAudioCodec>& codecsMain =  audioStream->Codecs();
    	
	    	for (TInt j = codecsMain.Count()-1; j >= 0;  j--)
				{
				if (setDtmfPri && codecsMain[j]->SdpName().Compare(DTMF))
					{
					iSessionHelper.SetCodecPrioritiesL(*codecsMain[j], iSessionHelper.dtmf);
					}
				if ( audioStream->Source()->Type() == KMceRTPSource )
	          		{
	          		// set priority for downlink
	          		iSessionHelper.SetCodecPrioritiesL(*codecsMain[j], iSessionHelper.downlink);
	          		}
	          	else
	          		{
	          		// set priority for uplink
	          		iSessionHelper.SetCodecPrioritiesL(*codecsMain[j], iSessionHelper.uplink);
	          		
					if ( keepAliveTimer )
						{
						iSessionHelper.SetKeepaliveL(*codecsMain[j], keepAliveTimer );
						}	
	          		}      
				}
       	
	    	//setting priorities for bound stream
           	if (audioStream->BoundStream())
              	{
              	CMceAudioStream* audioStreamBound = 
                                    static_cast<CMceAudioStream*> ( &audioStream->BoundStreamL());
              	
              	const RPointerArray<CMceAudioCodec>& codecs =  audioStreamBound->Codecs();
    	  
    	        for (TInt j = codecs.Count()-1; j >= 0;  j--)
					{
					if (setDtmfPri && codecs[j]->SdpName().Compare(DTMF))
						{
						iSessionHelper.SetCodecPrioritiesL(*codecs[j], iSessionHelper.dtmf);
						}
					if ( audioStream->BoundStreamL().Source()->Type() == KMceRTPSource )
	              		{
	              		// set priority for downlink
	              		iSessionHelper.SetCodecPrioritiesL(*codecs[j], iSessionHelper.downlink);
	              		}
              		else
	              		{
	              		// set priority for uplink
	              		iSessionHelper.SetCodecPrioritiesL(*codecs[j], iSessionHelper.uplink);
	                	if ( keepAliveTimer )
	            			{
	                		iSessionHelper.SetKeepaliveL(*codecs[j], keepAliveTimer );
							}
	              		}
            		}
				}
				
            }
        
        // Setting the Video Stream
        if ( streams[i]->Type() == KMceVideo )
            {

            CMceVideoStream* videoStream = 
                                    static_cast<CMceVideoStream*>(streams[i]);
            
            //Select specified codec
            if (selectedCodec.Compare( KNullDesC8 ) != 0) 
		        {
		        	DoVideoCodecSelectionL( *videoStream, &selectedCodec );
		        }
            
           	const RPointerArray<CMceVideoCodec>& codecsMainVideo =  videoStream->Codecs(); 
	    	for (TInt j = codecsMainVideo.Count()-1; j >= 0;  j--)
				{
				if ( videoStream->Source()->Type() == KMceRTPSource )
	          		{
	          		// set priority for downlink
	          		iSessionHelper.SetCodecPrioritiesL(*codecsMainVideo[j],
	          										   iSessionHelper.downlink);
	          		}
	          	else
	          		{
	          		// set priority for uplink
	          		iSessionHelper.	SetCodecPrioritiesL(*codecsMainVideo[j],
	          											iSessionHelper.uplink);
	            	if ( keepAliveTimer )
	            		{
	            		((CMceCodec*)codecsMainVideo[j])->SetKeepAliveTimerL(
	            			keepAliveTimer );
	            		}
	          		}      
				}
          	
           	if(videoStream->BoundStream())
              	{
              	CMceVideoStream* videoStreamBound = 
                                    static_cast<CMceVideoStream*> ( &videoStream->BoundStreamL());
              	
              	const RPointerArray<CMceVideoCodec>& codecsv =  videoStreamBound->Codecs();  
    	        for (TInt j = codecsv.Count()-1; j >= 0;  j--)
					{
					if ( videoStream->BoundStreamL().Source()->Type() == KMceRTPSource )
	              		{
	              		// set priority for downlink
	              		iSessionHelper.SetCodecPrioritiesL(*codecsv[j], iSessionHelper.downlink);
	              		}
              		else
	              		{
	              		// set priority for uplink
	              		iSessionHelper.SetCodecPrioritiesL(*codecsv[j], iSessionHelper.uplink);
	                	if ( keepAliveTimer )
	            			{
	                		((CMceCodec*)codecsv[j])->SetKeepAliveTimerL( keepAliveTimer );
	              			}
	              		}
            		}
				}
            }
        
        //set Inactivity timer for RTP source for each stream
        if( rtpInactivityTimer != 0 )
        	iSessionHelper.SetRtpInactivityTimerL(*tmpStream, rtpInactivityTimer);
        }

	// ---------- Execution ----------------------------------------------------
   
	//setting the media file sink
	 if (mediaFileSink.Compare( KNullDesC8 ) != 0) 
		{
		SetMediaFileSinkL( *session, &mediaFileSink);
		}
	 
	// Get timeout 
	TInt timeout = ExtractIntegerL( KParamTimeout, 0, EFalse );
    // Timeout is given to API in milliseconds, but is going to change to 
    // seconds, so translate it here instead of TTCN
    //timeout = 1000 * timeout; 
    
	// Get ContentType
	HBufC8* contentType = HBufCParameterL(ExtractTextL(KParamContentType, 
													   EFalse));
	CleanupStack::PushL( contentType );
	
	// Get Headers
	CDesC8Array* headers = ExtractHeadersL( EFalse );
	if (!headers)
		{
		headers = new (ELeave) CDesC8ArrayFlat(1);	
		}
	headers->AppendL(_L8("event: ttcn"));	
	CleanupStack::PushL( headers );
	// Get Content
	HBufC8* body = HBufCParameterL(ExtractTextL(KParamBody, EFalse));
	CleanupStack::PushL( body );
	
	// Get ContentHeaders
	CDesC8Array* contentHeaders = ExtractArrayL(KParamContentHeaders, EFalse );
	CleanupStack::PushL(contentHeaders);
	
	if ( !headers && !body && !contentType &&contentHeaders->Count()<=0 && timeout == 0 )
		{
		CleanupStack::PopAndDestroy( contentHeaders); contentHeaders = NULL;
		CleanupStack::PopAndDestroy( body ); body = NULL;
		CleanupStack::PopAndDestroy( headers ); headers = NULL;
		CleanupStack::PopAndDestroy( contentType ); contentType = NULL;
		session->UpdateL();
		}
	else
		{
		session->UpdateL( timeout, headers, contentType, body, contentHeaders );
		CleanupStack::Pop( contentHeaders);
		CleanupStack::Pop( body );
		CleanupStack::Pop( headers );
		CleanupStack::Pop( contentType );	
		}


	CMceSession::TState state = session->State();	
		
	// ---------- Response creation --------------------------------------------


 	AddIdResponseL( KSessionId, *session );
 
	// Add Session state	
	AddIntegerResponseL( KResponseState, state );
	AddTextualSessionStateL( state );

	}
	
TBool TCmdUpdateSession::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("UpdateSession") );
	}

TTcCommandBase* TCmdUpdateSession::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdUpdateSession( aContext );
	}
	
void TCmdUpdateSession::DoVideoCodecSelectionL( CMceVideoStream& aVideoStream, TPtrC8* aCodec  )	
	{	
	const RPointerArray<CMceVideoCodec>& codecs = aVideoStream.Codecs();
    
    if(aCodec->FindF(KMceSDPNameH264())!= KErrNotFound )
	    {
	    //Select only the H264 codec	   
	    const CMceVideoCodec* avcSingleNal = NULL;
	    const CMceVideoCodec* avcNonInterleaved = NULL;
	    
	    for ( TInt codecIndex = 0; codecIndex < codecs.Count(); ++codecIndex )
	        {
	        if ( codecs[codecIndex]->SdpName() == KMceSDPNameH264() )
	            { 
	            if ( codecs[codecIndex]->CodecMode() == KMceAvcModeSingleNal )
	                {
	                // Store a pointer to the Single NAL codec with best bitrate
	                if ( !avcSingleNal || 
	                     ( avcSingleNal && codecs[codecIndex]->MaxBitRate() > 
	                                       avcSingleNal->MaxBitRate() ) )
	                    {
	                    avcSingleNal = codecs[codecIndex];
	                    }
	                }  
	            else if ( codecs[codecIndex]->CodecMode() == 
	                                                KMceAvcModeNonInterleaved )
	                {
	                // Store a pointer to the Non-Interleaved codec with best bitrate
	                if ( !avcNonInterleaved || 
	                     ( avcNonInterleaved && codecs[codecIndex]->MaxBitRate() > 
	                                            avcNonInterleaved->MaxBitRate() ) )
	                    {
	                    avcNonInterleaved = codecs[codecIndex];
	                    }
	                }
	            else 
	                {
	                // NOP, we do not care about interleaved AVC
	                }
	            }
	        }
	    
	    const CMceVideoCodec* selectedCodec = NULL;
	    
	    if ( avcNonInterleaved )
	        {
	        selectedCodec = avcNonInterleaved;
	        }
	    else
	        {
	        selectedCodec = avcSingleNal;
	        }
	   
	    if ( selectedCodec )
	        {
	        // Remove all other codecs
	        for ( TInt codecIndex = 0; codecIndex < codecs.Count(); ++codecIndex )
	            {
	            if ( codecs[codecIndex] != selectedCodec )
	                {
	                aVideoStream.RemoveCodecL( *codecs[codecIndex] );
	                // Since succesfull removal of a codec has decreased the amount
	                // of codecs in array by one, we have to reset the index
	                codecIndex = 0;
	                }
	            }   
	        }
	    }
	    else
	    {
	   	//select others codecs
	    TBool codecModeBasedRemovalNeeded( EFalse );

		//check first if the codec is present
	    for ( TInt codecIndex = 0; codecIndex < codecs.Count(); ++codecIndex )
	        {
	        const CMceVideoCodec& currentCodec = *codecs[codecIndex];
	        if ( aCodec->FindF(  currentCodec.SdpName()) != KErrNotFound )
	           	codecModeBasedRemovalNeeded = ETrue; 	            		            		      
			}
	    
	    //remove codecs
	    if( codecModeBasedRemovalNeeded )
	    	{	    	    
		    for ( TInt codecIndex = 0; codecIndex < codecs.Count(); ++codecIndex )
		        {
		        const CMceVideoCodec& currentCodec = *codecs[codecIndex];
		        if ( aCodec->FindF( currentCodec.SdpName()) == KErrNotFound )
		           	aVideoStream.RemoveCodecL( *codecs[codecIndex] );    		            		      
				}
	    	}
	    }

	}


void TCmdUpdateSession::SetMediaFileSinkL( CMceSession& aSession, TPtrC8* aFile )
	{
	TBool video(EFalse);
	TBool audio(EFalse);
	if( aFile->Find(_L8(".mp4")) != KErrNotFound )
		video = ETrue;
	if( aFile->Find(_L8(".3gp")) != KErrNotFound )
		audio = ETrue;
	
	const RPointerArray<CMceMediaStream>& streams = aSession.Streams();
	
	for ( TInt i = 0; i < streams.Count(); ++i )
        {        
        CMceMediaStream* tmpStream;
        tmpStream = streams[i];        
        if( video )
        	{
			if ( tmpStream->Type() == KMceVideo )
				{
				CMceVideoStream* videoOutStreamFile = CMceVideoStream::NewL();
				CleanupStack::PushL( videoOutStreamFile );
				
				//use the same source which was used for outgoing video stream	
				if( tmpStream->Source()->Type() != KMceRTPSource )
					{
					videoOutStreamFile->SetSourceL( tmpStream->Source() );
					}
				else
					{
					if( tmpStream->BoundStream() )
						if( tmpStream->BoundStreamL().Source()->Type() != KMceRTPSource )
							videoOutStreamFile->SetSourceL(tmpStream->BoundStreamL().Source());
					}
				
				iSessionHelper.SetMediaFileSinkL(*videoOutStreamFile, *aFile);
				aSession.AddStreamL( videoOutStreamFile );
				CleanupStack::Pop( videoOutStreamFile );				
				break;	//only one file sink can be added for testing
				}
        	}
        if( audio )
			{
			if ( tmpStream->Type() == KMceAudio )
				{
				CMceAudioStream* audioOutStreamFile = CMceAudioStream::NewL();
				CleanupStack::PushL( audioOutStreamFile );
				
				//use the same source which was used for outgoing audio stream	
				if( tmpStream->Source()->Type() != KMceRTPSource )
					{
					audioOutStreamFile->SetSourceL( tmpStream->Source() );
					}
				else
					{
					if(tmpStream->BoundStream())
						if( tmpStream->BoundStreamL().Source()->Type() != KMceRTPSource )
							audioOutStreamFile->SetSourceL(tmpStream->BoundStreamL().Source());
					}
				
				iSessionHelper.SetMediaFileSinkL(*audioOutStreamFile, *aFile);
				aSession.AddStreamL( audioOutStreamFile );
				CleanupStack::Pop( audioOutStreamFile );				
				break;
				}
			}
        }
	
	}
