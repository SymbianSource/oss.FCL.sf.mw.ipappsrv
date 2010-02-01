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
#include "TCmdCreateSession.h"
#include "CTcMCEContext.h"

#include <sipprofile.h>
#include "TCLog.h"

#include <MCEMicSource.h>
#include <MceRtpSink.h>
#include <mcefilesink.h>
#include <MCEAudioStream.h>
#include <MceRtpSource.h>
#include <MCESpeakerSink.h>
#include <MceOutSession.h>
#include <MCEEvent.h>
#include <MCERefer.h>
#include <MCEAMRCodec.h>	
#include <MCECnCodec.h>
#include <mceredcodec.h>
#include <mcedefs.h>

#include <MCEVideoStream.h>
#include <mcecamerasource.h>
#include <mcedisplaysink.h>
#include <MCEVideoCodec.h>
#include <mceh263codec.h>
#include <mcefilesource.h>

const TUint KAudioPriorityPocAudioUplink                  = 95;
const TUint KAudioPriorityPocAudioDownlink                = 95;
const TUint KAudioPrefPocAudioUplink                      = 0x05130001;
const TUint KAudioPrefPocAudioDownlink                    = 0x05120001;

const TUint KDtmfAudioPriority							  = 45;
const TUint KDtmfAudioPref								  = 0x00150001;

class CMceManager;
class CTcNameValue;
	
_LIT8( CN, "CN" );
_LIT8( red, "red" );
// ----------------------------------------------------------------------------
// TCmdCreateSession::ExecuteL
// ----------------------------------------------------------------------------
//		
void TCmdCreateSession::ExecuteL()
	{
	TcLog::Write( _L8("\nTCmdCreateSession::ExecuteL()") );
	CreateOutSessionL();	
	}

// ----------------------------------------------------------------------------
// TCmdCreateSession::CreateOutSessionL
// ----------------------------------------------------------------------------
//	
void TCmdCreateSession::CreateOutSessionL()
	{
	TcLog::Write( _L8("\nTCmdCreateSession::CreateOutSessionL()") );
	// ---- Setup --------------------------------------------------------------
	// Extract parameters
	
	TPtrC8 sessionType = ExtractTextL( KParamSessionType, EFalse );	
	
	TBool suppressRtcp = ExtractBooleanL( KParamSuppressRtcp, EFalse );	
		
	// Get profile
	CSIPProfile* profile = 
		reinterpret_cast<CSIPProfile*>(GetObjectForIdL(KProfileObj, EFalse));

	// Get event
	CMceEvent* event = 
	    reinterpret_cast<CMceEvent*>(GetObjectForIdL(KEventId, EFalse));			

	// Get refer
	CMceRefer* refer = 
	    reinterpret_cast<CMceRefer*>(GetObjectForIdL(KReferId, EFalse));	
	   
    // Get alternative mainStream
	TBool mainStreamAudioOut = ExtractBooleanL( KParamMainStreamAudioOut, EFalse );	
	
	//get path for media file source
	TPtrC8 pathMediaFile = ExtractTextL( KParamAudioFileSource, EFalse );
	
	//get path for media file sink
	TPtrC8 mediaFileSink = ExtractTextL( KParamMediaFileSink, EFalse );
	
	//extract Array of codecs
	CDesC8Array* codecsList = ExtractArrayL(KArrayCodecsList, EFalse);
	
	//get the inactivity timer value for rtp source
	TInt rtpInactivityTimer = ExtractIntegerL(KParamRTPInactivityTimer, 0, EFalse);
 		
    TInt keepAliveTimer = ExtractIntegerL( KParamKeepAliveValue, 0, EFalse );
	keepAliveTimer = Max( 0, keepAliveTimer );
	keepAliveTimer = Min( KMaxTUint8, keepAliveTimer );
	// ---- Execution ----------------------------------------------------------

	// Contruct session	

	CMceSession* session = NULL;
	
	if ( profile ) 
		{
		
		///---------------------------------------------------------------
		// Next is used only during API change, use only first row after change
		TPtrC8 recipient = ExtractTextL( KParamRecipient, EFalse );
		TPtrC8 toHeader = ExtractTextL( KParamToHeader, EFalse );		

		TPtrC8 recipientOrToHeader;
		
		if ( recipient != KNullDesC8 )
		    {
		    recipientOrToHeader.Set( recipient );
		    }
		else
		    {
		    recipientOrToHeader.Set( toHeader );
		    }
		
		//------------------------------------------------------------------
		
        HBufC8* originator = HBufCParameterL(ExtractTextL( KParamOriginator, 
                                                           EFalse));
                                                           
	    CleanupStack::PushL( originator );
			
		session = CreateSessionL( iContext.MCEManager(),	
								  *profile, 
								  recipientOrToHeader,
								  originator );    
								                             
		CleanupStack::Pop( originator );
		}
	else if ( event ) 
		{
		session = CreateSessionL( *event );
		}
	else if ( refer )
	    {
	    session = CreateSessionL( *refer );
	    }
    else
    	{
		User::Leave( KErrNotFound );
		}	
	
	CleanupStack::PushL(session);
		
	// Construct and bind streams
	if ( (sessionType == KValueSessionTypeAudio) || (sessionType == KValueSessionTypeAudioVideo ))
		{
		CMceAudioStream* audioInStream = CMceAudioStream::NewLC();
		
		CMceRtpSource* rtpSource = NULL;
		if(rtpInactivityTimer != 0)
			rtpSource = CMceRtpSource::NewLC(KMceJitterBufferLength, KMceJitterBufferTreshold, rtpInactivityTimer * 1000);
		else
			rtpSource = CMceRtpSource::NewLC();
		
		audioInStream->SetSourceL(rtpSource);
		CleanupStack::Pop(rtpSource);
			
		CMceSpeakerSink* speaker = CMceSpeakerSink::NewLC();	
		audioInStream->AddSinkL(speaker);
		CleanupStack::Pop(speaker);

		
		CMceAudioStream* audioOutStream = CMceAudioStream::NewLC();
		
		//use file stream as a source
		if( pathMediaFile.Compare( KNullDesC8 ) != 0 )
			{
			// If source path was defined use the file audio specified in path
			iSessionHelper.SetMediaFileSourceL( iContext.MCEManager(), *audioOutStream, pathMediaFile );
			}
		else
			{
			CMceMicSource* mic = CMceMicSource::NewLC();
			audioOutStream->SetSourceL(mic);	
			CleanupStack::Pop(mic);	
			}
		
		CMceRtpSink* rtpSink = CMceRtpSink::NewL(suppressRtcp);
		CleanupStack::PushL(rtpSink);
		audioOutStream->AddSinkL(rtpSink);
		CleanupStack::Pop(rtpSink);

        if ( mainStreamAudioOut )
        	{
        	audioOutStream->BindL( audioInStream );	
        	// Add bound streams to session 
			session->AddStreamL( audioOutStream );
        	}
        else
        	{
        	audioInStream->BindL( audioOutStream );
        	// Add bound streams to session
			session->AddStreamL( audioInStream );
        	}
        
		//use file as a media sink for recording the sending media
		if( mediaFileSink.Compare( KNullDesC8 ) != 0 )
			{
			CMceAudioStream* audioOutStreamFile = CMceAudioStream::NewL();
			CleanupStack::PushL( audioOutStreamFile );
			
			//use the same source which was used for outgoing audio stream	
			audioOutStreamFile->SetSourceL( audioOutStream->Source() );	
			
			iSessionHelper.SetMediaFileSinkL(*audioOutStreamFile, mediaFileSink);
			session->AddStreamL( audioOutStreamFile );
			CleanupStack::Pop( audioOutStreamFile );
		
			if(codecsList)
				{
					RemoveMediaCodecsL(audioOutStreamFile, audioOutStreamFile, codecsList, EStreamAudio);
				}
			}
		
		CleanupStack::Pop( audioOutStream );			
		CleanupStack::Pop( audioInStream );
		
		const RPointerArray<CMceAudioCodec>& codecsIn =  audioInStream->Codecs();
		const RPointerArray<CMceAudioCodec>& codecsOut =  audioOutStream->Codecs();

		TUint audioPriorityUplink = ExtractIntegerL(KParamSessionPriorityUplink, KAudioPriorityPocAudioUplink ,EFalse);
		TUint audioPriorityDownlink = ExtractIntegerL(KParamSessionPriorityDownlink, KAudioPriorityPocAudioDownlink, EFalse );
		TUint audioPrefUplink = ExtractIntegerL(KParamAudioPrefUplink, KAudioPrefPocAudioUplink, EFalse);
		TUint audioPrefDownlink = ExtractIntegerL(KParamAudioPrefDownlink, KAudioPrefPocAudioDownlink, EFalse);

 	    //removing selected codecs from streams
 		if(codecsList)
 		{
 			//remove codecs which are not presented in the given list
 			RemoveMediaCodecsL(audioOutStream, audioInStream, codecsList, EStreamAudio);
 		
 		}
		
		//defined here because KMceSDPNameDtmf hasn't been defined in mce2
		_LIT8( DTMF, "telephone-event" );
		//deleting pcm codecs. bug work-around until selected codecs can be passed from the test case
		for (TInt i = codecsOut.Count()-1; i >= 0;  i --)
			{
		 	if ( keepAliveTimer )
 				{
 				iSessionHelper.SetKeepaliveL(*codecsOut[i], keepAliveTimer);
 				}

			if (!codecsOut[i]->SdpName().Compare(/*KMceSDPNameDtmf*/DTMF))
				{
				iSessionHelper.SetCodecPrioritiesL(*codecsOut[i], iSessionHelper.dtmf);
				}
			else
				{
				iSessionHelper.SetCodecPriPrefL(*codecsOut[i],audioPriorityUplink,audioPrefUplink);
				}	
			}
		for (TInt i = 0; i < codecsIn.Count(); i ++)
			{
			if (!codecsIn[i]->SdpName().Compare(DTMF))
				{
				iSessionHelper.SetCodecPrioritiesL(*codecsIn[i], iSessionHelper.dtmf);
		
				}
			else
				{
				iSessionHelper.SetCodecPriPrefL(*codecsIn[i],audioPriorityDownlink,audioPrefDownlink);
				}		
			}
	    
		AddIdResponseL( KStreamId, *audioOutStream );
		
		}
	// Initializing Video stream
	if ( (sessionType == KValueSessionTypeVideo) || (sessionType == KValueSessionTypeAudioVideo ))
		{
	    
	    //TODO
	    CMceVideoStream* videoInStream = CMceVideoStream::NewLC();
		
		CMceRtpSource* rtpSource = CMceRtpSource::NewLC(200, 100, 10);
		videoInStream->SetSourceL(rtpSource);
		CleanupStack::Pop(rtpSource);
			
		CMceDisplaySink* display = CMceDisplaySink::NewLC(iContext.MCEManager());
		videoInStream->AddSinkL(display);
		CleanupStack::Pop(display);
		
		//creating outgoing video stream
		CMceVideoStream* videoOutStream = CMceVideoStream::NewLC();
				
		//use file stream as a source
		if( pathMediaFile.Compare( KNullDesC8 ) != 0 )
			{			
			// If source path was defined use the file audio specified in path
			iSessionHelper.SetMediaFileSourceL( iContext.MCEManager(), *videoOutStream, pathMediaFile );
			}
		else
			{
			CMceCameraSource* cam = CMceCameraSource::NewLC(iContext.MCEManager());
			videoOutStream->SetSourceL(cam);	
			CleanupStack::Pop(cam);	
			}

		
		CMceRtpSink* rtpSink = CMceRtpSink::NewL(EFalse);
		CleanupStack::PushL(rtpSink);
		videoOutStream->AddSinkL(rtpSink);
		CleanupStack::Pop(rtpSink);

        if ( mainStreamAudioOut )
        	{
        	videoOutStream->BindL( videoInStream );	
        	// Add bound streams to session 
			session->AddStreamL( videoOutStream );
        	}
        else
        	{
        	videoInStream->BindL( videoOutStream );
        	// Add bound streams to session
			session->AddStreamL( videoInStream );
        	}
        
		//use file as a media sink for recording the sending media
		if( mediaFileSink.Compare( KNullDesC8 ) != 0 )
			{
			CMceVideoStream* videoOutStreamFile = CMceVideoStream::NewL();
			CleanupStack::PushL( videoOutStreamFile );
			
			//use the same source which was used for outgoing video stream	
			videoOutStreamFile->SetSourceL( videoOutStream->Source() );	
			
			iSessionHelper.SetMediaFileSinkL(*videoOutStreamFile, mediaFileSink);
			session->AddStreamL( videoOutStreamFile );
			CleanupStack::Pop( videoOutStreamFile );
		
			if(codecsList)
				{
					RemoveMediaCodecsL(videoOutStreamFile, videoOutStreamFile, codecsList, EStreamVideo);
				}
			}
        	
		CleanupStack::Pop( videoOutStream );			
		CleanupStack::Pop( videoInStream );

		//remove selected codecs from the streams
		if(codecsList)
		{
			RemoveMediaCodecsL(videoOutStream, videoInStream, codecsList, EStreamVideo);
		}
 		
		AddIdResponseL( KStreamId, *videoOutStream );
			
	//	AddIdResponseL(KSessionId, session); // uses CleanupStack internally 
		}    
	else
	    {
	    // NOP   
	    }
	    
	    
	CTcStructure* structure;
	do
		{
		structure = FindStructureL( KParamSessionModifier, EFalse );
		if( structure )
			{
            //structure contains a list of name-value pairs
			for (int i=0; i < structure->Count(); i++)
				{
				CTcNameValue* item = structure->ItemL( i );
				session->SetModifierL( item->NameAsIntL(), item->ValueAsIntL() );
				}
			iContext.List().RemoveParameter( KParamSessionModifier );
			}
	} while( structure );

	CleanupStack::Pop( session );
	
	
	// ---- Response creation --------------------------------------------------
 
 	// Session must be added by pointer
	AddIdResponseL( KSessionId, session ); // uses CleanupStack internally
	}

// ----------------------------------------------------------------------------
// TCmdCreateSession::CreateSessionL
// ----------------------------------------------------------------------------
//	
CMceSession* TCmdCreateSession::CreateSessionL( CMceManager& aManager,
                        						CSIPProfile& aProfile,
                        						const TDesC8& aRecipient,
												HBufC8* aOriginator  )
	{
 	return CMceOutSession::NewL( aManager, aProfile, 
							   	 aRecipient, aOriginator );	
	}

// ----------------------------------------------------------------------------
// TCmdCreateSession::CreateSessionL
// ----------------------------------------------------------------------------
//	
CMceSession* TCmdCreateSession::CreateSessionL( CMceEvent& aEvent )
	{
 	return CMceOutSession::NewL( aEvent );	
	}

// ----------------------------------------------------------------------------
// TCmdCreateSession::CreateSessionL
// ----------------------------------------------------------------------------
//	
CMceSession* TCmdCreateSession::CreateSessionL( CMceRefer& aRefer )
	{
 	return CMceOutSession::NewL( aRefer );	    
	}

// ----------------------------------------------------------------------------
// TCmdCreateSession::Match
// ----------------------------------------------------------------------------
//	
TBool TCmdCreateSession::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("CreateSession") );
	}

// ----------------------------------------------------------------------------
// TCmdCreateSession::CreateL
// ----------------------------------------------------------------------------
//	
TTcCommandBase* TCmdCreateSession::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdCreateSession( aContext );
	}

// ----------------------------------------------------------------------------
// TCmdCreateSession::RemoveMediaCodecsL
// ----------------------------------------------------------------------------	
TInt TCmdCreateSession::RemoveMediaCodecsL(CMceMediaStream* aStreamOut, CMceMediaStream* aStreamIn,
				CDesC8Array* aCodecsList, TStreamType aType)
	{
 	
	CleanupStack::PushL(aCodecsList);
	const TInt codecsListCount = aCodecsList->Count();
	
	if(codecsListCount>0)
	{
					
		if( aType == EStreamAudio )
		{
 			CMceAudioStream* audioOutStream = dynamic_cast<CMceAudioStream*>(aStreamOut);
 			CMceAudioStream* audioInStream = dynamic_cast<CMceAudioStream*>(aStreamIn);
 			
			const RPointerArray<CMceAudioCodec>& streamCodecsOut = 
		    	audioOutStream->Codecs();
			const RPointerArray<CMceAudioCodec>& streamCodecsIn = 
		    	audioInStream->Codecs();
						
			if ( (*aCodecsList)[0] != KValueCodecAll )
			{							
				TBool found = EFalse;
				//removing from audioOutStream
				for(TInt iCount = streamCodecsOut.Count()-1; iCount >= 0;  iCount-- )
				{
					CMceAudioCodec* codecOut = streamCodecsOut[iCount];
					found = EFalse;
					
					for(TInt i = 0; i < codecsListCount; i++)
					{													
						if(codecOut->SdpName() == (*aCodecsList)[i] ) 
						{
							//CN codec will get enabled
							if( codecOut->SdpName() == CN ) 
							{
								codecOut->SetStateL(CMceCodec::EEnabled);
							}
							else if( codecOut->SdpName() == red ) 
							{
								codecOut->SetStateL(CMceCodec::EEnabled);
							}
							//setting preference
							codecOut->SetPreferenceL( i );
							
							found = ETrue;
						}
									
					}
					
					if(found == EFalse)
						audioOutStream->RemoveCodecL(*streamCodecsOut[iCount]);
				}
			
				audioOutStream->ReorderCodecsByPreferenceL(EFalse);	
        		
				found = EFalse;
				//removing from audioInStream
				for(TInt iCount = streamCodecsIn.Count()-1; iCount >= 0;  iCount-- )
				{
					CMceAudioCodec* codecIn = streamCodecsIn[iCount];
					found = EFalse;
					
					for(TInt i = 0; i < codecsListCount; i++)
					{
						if( codecIn->SdpName() == (*aCodecsList)[i] ) 
						{
							found = ETrue;
							//CN codec will get enabled
							if( codecIn->SdpName() == CN ) 
							{
								codecIn->SetStateL(CMceCodec::EEnabled);
							}
							else if( codecIn->SdpName() == red ) 
							{
								codecIn->SetStateL(CMceCodec::EEnabled);
							}
							//setting preference according to the order in list
							codecIn->SetPreferenceL( i );
						}
            			
					}
					
					
					if(found == EFalse)
						audioInStream->RemoveCodecL(*streamCodecsIn[iCount]);		
				}
				
				audioInStream->ReorderCodecsByPreferenceL( EFalse );
				
			}

		} //end of audio stream
		if( aType == EStreamVideo )
		{
			CMceVideoStream* videoOutStream = dynamic_cast<CMceVideoStream*>(aStreamOut);
 			CMceVideoStream* videoInStream = dynamic_cast<CMceVideoStream*>(aStreamIn);
 					
			const RPointerArray<CMceVideoCodec>& streamCodecsOut = 
		    	videoOutStream->Codecs();
			const RPointerArray<CMceVideoCodec>& streamCodecsIn = 
		    	videoInStream->Codecs();
		
			if ( (*aCodecsList)[0] != KValueCodecAll )
			{		
				
				//removing from videoOutStream
				for(TInt iCount = streamCodecsOut.Count()-1; iCount >= 0;  iCount-- )
				{
					CMceVideoCodec* codecOut = streamCodecsOut[iCount];
					
					TBool found = EFalse;
					
					for(TInt i = 0; i < codecsListCount; i++)
					{													
						if(codecOut->SdpName() == (*aCodecsList)[i] )
						{
							found = ETrue;
							codecOut->SetPreferenceL( i );
						}
            					
					}
					
					if(found == EFalse)
						videoOutStream->RemoveCodecL(*streamCodecsOut[iCount]);

				}
				videoOutStream->ReorderCodecsByPreferenceL(EFalse);

				//removing from videoInStream
				for(TInt iCount = streamCodecsIn.Count()-1; iCount >= 0;  iCount-- )
				{
					CMceVideoCodec* codecIn = streamCodecsIn[iCount];
					
					TBool found = EFalse;
					
					for(TInt i = 0; i < codecsListCount; i++)
					{
						if( codecIn->SdpName() == (*aCodecsList)[i] )			
						{
							found = ETrue;
							codecIn->SetPreferenceL( i );
						}
            			
					}
					
					if(found == EFalse)
						videoInStream->RemoveCodecL(*streamCodecsIn[iCount]);
				}
				videoInStream->ReorderCodecsByPreferenceL(EFalse);
			}
		}// end of video stream
	} 
	
	CleanupStack::Pop(aCodecsList);						     
	return 0;
	}
