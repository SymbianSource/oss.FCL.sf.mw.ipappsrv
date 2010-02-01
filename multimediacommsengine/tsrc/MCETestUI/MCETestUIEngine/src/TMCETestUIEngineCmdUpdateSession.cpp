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




// INCLUDES

#include "TMCETestUIEngineCmdUpdateSession.h"
#include "CMCETestUIQuestionSingleSelection.h"
#include "CMCETestUIQuestionDataQuery.h"
#include "CMCETestUIEngineSession.h"
#include "CMCETestUIEngineCodec.h"
#include "CMCETestUIEngineAudioStream.h"
#include "CMCETestUIEngineVideoCodec.h"
#include "CMCETestUIEngineVideoStream.h"
#include <MCESession.h>
#include <MceAudioStream.h>
#include <MCEAudioCodec.h>
#include <MCEFCMsgExchange.h>
#include <MceVideoStream.h>
#include <MCEVideoCodec.h>
#include <mcertpsource.h>
#include <mcemicsource.h>
#include <mcespeakersink.h>
#include <sipstrings.h> 
#include <sipstrconsts.h>


// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdUpdateSession::TMCETestUIEngineCmdUpdateSession
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdUpdateSession::TMCETestUIEngineCmdUpdateSession( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineSession& aSession )
    : TMCETestUIEngineCmdBase( aEngine ),
      iSession( aSession )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdUpdateSession::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdUpdateSession::ExecuteL()
	{
	if(iEngine.iMultiCodecSupport == EFalse)
		{
		DeleteCodecs();	
		} // delete this if you want to delete all codecs manually
	iSession.Session().UpdateL();
	iEngine.EngineStateChangedL();    
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdUpdateSession::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdUpdateSession::Caption() const
	{
	return KCommandCaptionUpdateSession;
	}
	
	
EXPORT_C void TMCETestUIEngineCmdUpdateSession:: DeleteCodecs()
	{
	const RPointerArray<CMCETestUIEngineAudioStream>& audioStreams = 
            iSession.AudioStreamsL();
            
         for ( TInt i = 0; i < audioStreams.Count(); ++i )
        	{
        	 RPointerArray<CMCETestUIEngineCodec>& codecs = 
            	audioStreams[i]->CodecsL();
            	TInt count = codecs.Count();
            //delete all codecs except first one	
         	for ( TInt j = count-1; j > 0; --j )
				{
				 audioStreams[i]->AudioStream().RemoveCodecL(codecs[j]->Codec());
		         delete codecs[j];
		         codecs.Remove( j );
		         codecs.Compress();
		         iEngine.EngineStateChangedL();
		        }
        	}
     const RPointerArray<CMCETestUIEngineVideoStream>& videoStreams = 
            iSession.VideoStreamsL();
            
         for ( TInt i = 0; i < videoStreams.Count(); ++i )
        	{
        	 RPointerArray<CMCETestUIEngineVideoCodec>& codecs = 
            	videoStreams[i]->CodecsL();
            	TInt count = codecs.Count();
            //delete all codecs except first one	
         	for ( TInt j = count-1; j > 0; --j )
				{
				 videoStreams[i]->VideoStream().RemoveCodecL(codecs[j]->Codec());
		         delete codecs[j];
		         codecs.Remove( j );
		         codecs.Compress();
		         iEngine.EngineStateChangedL();
		        }
        	}
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdUpdateSessionWithValues::TMCETestUIEngineCmdUpdateSessionWithValues
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdUpdateSessionWithValues::TMCETestUIEngineCmdUpdateSessionWithValues( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineSession& aSession )
    : TMCETestUIEngineCmdBase( aEngine ),
      iSession( aSession )
    {
    }
    



// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdUpdateSessionWithValues::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdUpdateSessionWithValues::ExecuteL()
	{
	if(iEngine.iMultiCodecSupport == EFalse)
		{
		DeleteCodecs();	
		} // delete this if you want to delete all codecs manually
	if(iEngine.IsFCSession())// for PoC Session update , may be new update can be added for PoC
		{
	//	iEngine.FloorControl().InitializeL(iSession.Session(),0,iEngine.MMObserver());
		CDesC8Array* array = new (ELeave) CDesC8ArrayFlat(1);	
		array->AppendL(_L8("a=fmtp:TBCP queuing=0; tb_priority=1; timestamp=0; tb_granted=0; poc_sess_priority=1; poc_lock=0\r\n"));
		iEngine.FloorControl().UpdateL(iSession.Session(), array);	
	
		CDesC8Array* sipHeaders = new ( ELeave ) CDesC8ArrayFlat(1);
		CleanupStack::PushL( sipHeaders );	//1
	//	sipHeaders->AppendL(_L8("Accept-Contact: *;+mckoppa"));					
		CDesC8Array* contHeaders = new ( ELeave ) CDesC8ArrayFlat(1);
		CleanupStack::PushL( contHeaders );							// 2
		
		// empty content type
		HBufC8* contentType = HBufC8::NewLC( 1 );					// 3
		HBufC8* sdpcontent = HBufC8::NewLC( 1 ); 					// 4
		// server - header
		sipHeaders->AppendL( (_L8("Server: PoC-client/OMA1.0 Nokia-S60/v1.0")) );
		// privacy - header
		sipHeaders->AppendL(_L8("Privacy: id;user;header"));
		// p-preferred identity header
						// 4
		sipHeaders->AppendL( (_L8("P-Preferred-Identity: <sip:someone@somewhere.com> ")) );
		iSession.Session().UpdateL( 36000, 
							  sipHeaders,
							  contentType,
							  sdpcontent,
							  contHeaders );
	    CleanupStack::Pop( sdpcontent );
	    CleanupStack::Pop( contentType );
	    CleanupStack::Pop( contHeaders );
	    CleanupStack::Pop( sipHeaders );	
		iEngine.EngineStateChangedL();    
		}
	else
		{
		TSessionEstablishValues sessionValues;
    	if(iEngine.SessionEstablishValues( sessionValues))
    	{
    	// change the values to the format needed by api 
    	CDesC8Array* headers;
    	if(sessionValues.headers.Compare(KNone))
    		{
    		CMCETestUIQuestionDataQuery* question = 
	    			CMCETestUIQuestionDataQuery::NewLC();
    		question->SetCaptionL( KUserQuestionInputRecipient );
			question->SetDefaultValueL(sessionValues.headers  );
			question->SetAnswerL( sessionValues.headers );
			TBufC8<100> headersbuffer( question->Answer8() );
			CleanupStack::PopAndDestroy( question );
			headers = new (ELeave) CDesC8ArrayFlat( 1 );
			CleanupStack::PushL( headers );
			headers->AppendL( headersbuffer );
    		}
    	else
    		{
    		headers = new (ELeave) CDesC8ArrayFlat( 1 );
			CleanupStack::PushL( headers );
		//	headers->AppendL(_L8("Accept-Contact: *;+mckoppa"));
		//	headers->AppendL( KAcceptContactMCETestUI );	
    		}
    
			CleanupStack::Pop( headers );
			iSession.Session().SetServiceTypeL(sessionValues.iServiceType);
			iSession.Session().UpdateL(sessionValues.timeOut, headers, NULL, NULL, NULL );
			iEngine.EngineStateChangedL();
	       	}
		
		}
 	
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdUpdateSessionWithValues::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdUpdateSessionWithValues::Caption() const
	{
	return KUserQuestionOptionUpdateWithValues;
	}



// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdUpdateSessionWithValues::DeleteCodecs
// -----------------------------------------------------------------------------
//

EXPORT_C void TMCETestUIEngineCmdUpdateSessionWithValues:: DeleteCodecs()
	{
	const RPointerArray<CMCETestUIEngineAudioStream>& audioStreams = 
            iSession.CurrentAudioStreams();
            
         for ( TInt i = 0; i < audioStreams.Count(); ++i )
        	{
        	 RPointerArray<CMCETestUIEngineCodec>& codecs = 
            	audioStreams[i]->CurrentCodecs();
            //delete all codecs except first one	
         	for ( TInt j = 1; j < codecs.Count(); ++j )
				{
				 audioStreams[i]->AudioStream().RemoveCodecL(codecs[j]->Codec());
		         delete codecs[j];
		         codecs.Remove( j );
		         codecs.Compress();
		        }
        	}
      const RPointerArray<CMCETestUIEngineVideoStream>& videoStreams = 
            iSession.VideoStreamsL();
            
         for ( TInt i = 0; i < videoStreams.Count(); ++i )
        	{
        	 RPointerArray<CMCETestUIEngineVideoCodec>& codecs = 
            	videoStreams[i]->CodecsL();
            	TInt count = codecs.Count();
            //delete all codecs except first one	
         	for ( TInt j = count-1; j > 0; --j )
				{
				 videoStreams[i]->VideoStream().RemoveCodecL(codecs[j]->Codec());
		         delete codecs[j];
		         codecs.Remove( j );
		         codecs.Compress();
		         iEngine.EngineStateChangedL();
		        }
        	}
	}






// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdUpdateSession::TMCETestUIEngineCmdUpdateSession
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdHoldCall::TMCETestUIEngineCmdHoldCall( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineSession& aSession )
    : TMCETestUIEngineCmdBase( aEngine ),
      iSession( aSession )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdUpdateSession::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdHoldCall::ExecuteL()
	{
	const RPointerArray<CMceMediaStream>& streams = iSession.Session().Streams();
	CMceMediaStream& mainstream = *streams[0];
	CMceMediaStream& boundStream = mainstream.BoundStreamL();
	
    if( !iSession.iHold )
        {
        HandleLocalHoldingL( mainstream, boundStream );
        DisableAudioL();
        iSession.iHold = ETrue;
        }
    else
        {
        HandleLocalResumingL( mainstream, boundStream );
        iSession.iHold = EFalse;
        iSession.iHoldInProgress = ETrue;
        }

	iSession.Session().UpdateL();
	iEngine.EngineStateChangedL();    
	}

// ---------------------------------------------------------------------------
// CSVPHoldMediaHandler::HandleLocalHoldingL - copied from VoIP
// ---------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdHoldCall::
HandleLocalHoldingL( CMceMediaStream& aMediaStream,
                     CMceMediaStream& aBoundStream )
    {
    
    // In case of local hold RTP source must be disabled
    if ( KMceRTPSource == aMediaStream.Source()->Type() )
        {
        aMediaStream.DisableL();
        }
        
    else if ( KMceRTPSource == aBoundStream.Source()->Type() )
        {
        aBoundStream.DisableL();
        }
        
    else
        {
        User::Leave( KErrNotFound );
        }
        
    }

// ---------------------------------------------------------------------------
// CSVPHoldMediaHandler::DisableAudioL
// ---------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdHoldCall::DisableAudioL()
    {
    // Disable speaker & microphone
    CMceSession* session = &iSession.Session();
    const RPointerArray< CMceMediaStream >& streams = session->Streams();
    TInt streamCount = streams.Count();
    for ( TInt i = 0; i < streamCount; i++ )
        {
        CMceMediaStream* mediaStream = streams[ i ];
        TMceMediaType mediaType = mediaStream->Type();
        if ( KMceAudio == mediaType )
            {
            CMceMediaStream& boundStream = mediaStream->BoundStreamL();
            DisableAudioStreamL( boundStream );
            DisableAudioStreamL( *mediaStream );
            }
        } 
    }
 
// ---------------------------------------------------------------------------
// CSVPHoldMediaHandler::DisableAudioStreamL
// ---------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdHoldCall::DisableAudioStreamL( CMceMediaStream& aAudioStream )
    {
    if ( aAudioStream.Source() &&
         KMceMicSource == aAudioStream.Source()->Type() )
        {
        CMceMicSource* micSource = 
                    ( CMceMicSource* ) aAudioStream.Source();
        if ( micSource->IsEnabled() )
            {
            // mute speaker sink
            micSource->DisableL();
            }
        } 
        
     if ( aAudioStream.Sinks().Count()  )
        {
        const RPointerArray< CMceMediaSink >& sinks = 
            aAudioStream.Sinks();
        for ( TInt z = 0; z < sinks.Count(); z++ )
            {
            if ( sinks[ z ]->Type() == KMceSpeakerSink )
                {
                CMceSpeakerSink* speakerSink = 
                    ( CMceSpeakerSink* )  sinks[ z ];
                if ( speakerSink->IsEnabled() )
                    {
                    // mute speaker sink
                    speakerSink->DisableL();                    
                    }
                }
            } 
        }                              
    
    } 
    

// ---------------------------------------------------------------------------
// CSVPHoldMediaHandler::HandleLocalResumingL
// ---------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdHoldCall::
HandleLocalResumingL( CMceMediaStream& aMediaStream,
                      CMceMediaStream& aBoundStream )
    {
    if ( !aBoundStream.IsEnabled() )
        {
        aBoundStream.EnableL();
        }
        
    if ( !aMediaStream.IsEnabled() )
        {
        aMediaStream.EnableL();
        }

    }




    
      
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdUpdateSession::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdHoldCall::Caption() const
	{
	return KCommandCaptionHoldSession;
	}
	


// End of File
