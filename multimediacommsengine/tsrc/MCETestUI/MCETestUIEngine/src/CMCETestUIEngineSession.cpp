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
* Description:    
*
*/




// MCE API

#include <MCEManager.h>
#include <MCESession.h>
#include <MCEMediaStream.h>
//#include <MCEOneWayStream.h>
#include <MCEAudioStream.h>
#include <MCEVideoStream.h>
#include <MCEMediaSource.h>
#include <MCEMicSource.h>
#include <MCEMediaSink.h>
#include <MCERtpSink.h>
#include <MCERtpSource.h>
#include <MCESpeakerSink.h>
#include <mcesecuresession.h>

#include "MCETestUIEngineDefaultValues.h"

#include "CMCETestUIEngine.h"
#include "CMCETestUIEngineSession.h"

#include "TMCETestUIEngineCmdTerminateSession.h"
#include "TMCETestUIEngineCmdUpdateSession.h"
#include "TMCETestUIEngineCmdDeleteSession.h"
#include "TMCETestUIEngineCmdFloorControlSession.h"
#include "TMCETestUIEngineCmdWriteSdpToFile.h"
#include "TMCETestUIEngineCmdCryptoHandling.h"
#include "TMCETestUIEngineCmdCreateEvent.h"
#include "TMCETestUIEngineCmdSRReqRes.h"
#include "CMCETestUIEngineAudioStream.h"
#include "CMCETestUIEngineVideoStream.h"


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMCETestUIEngineSession::CMCETestUIEngineSession
// -----------------------------------------------------------------------------
//
CMCETestUIEngineSession::CMCETestUIEngineSession( 
                CMCETestUIEngine& aEngine,
                TDirection aDirection,
                TVideoDirection aVideoDirection )
    : iEngine( aEngine ),
      iDirection( aDirection ),
      iVideoDirection( aVideoDirection ),
      iHold( EFalse ),
      iHoldInProgress( EFalse )
    {
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineSession::BaseConstructL()
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineSession::BaseConstructL()
    {
    }


// -----------------------------------------------------------------------------
// CMCETestUIEngineSession::~CMCETestUIEngineSession
// -----------------------------------------------------------------------------
//
EXPORT_C CMCETestUIEngineSession::~CMCETestUIEngineSession()
    {
    iCommands.ResetAndDestroy();
    iAudioStreams.ResetAndDestroy();
    iVideoStreams.ResetAndDestroy();
    }
/*
// -----------------------------------------------------------------------------
// CMCETestUIEngineSession::StatusCode
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMCETestUIEngineSession::StatusCode()
	{
	return iStatusCode;
	}
*/	
// -----------------------------------------------------------------------------
// CMCETestUIEngineSession::GetCommands
// -----------------------------------------------------------------------------
//
const RPointerArray<TMCETestUIEngineCmdBase>& 
            CMCETestUIEngineSession::GetCommandsL()
    {
    User::LeaveIfError( iSession ? KErrNone : KErrNotReady );

    TInt state = iSession->State();
    

	switch( state ) 
		{
		case CMceSession::EIdle: 
		    {	
		         
		    break;
		    }
		    
		case CMceSession::EAnswering: 
		    {	
		    iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdDeleteSession( iEngine, *this ) );	    
			
			iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdSendInfo( iEngine, *this ) );
			iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdDTMFInfo( iEngine, *this ) );
		    iCommands.Append( new ( ELeave )
		    	TMCETestUIEngineCmdSendInfoResponse( iEngine, *this ) );		        
		   
		    break;
		    }
		    
		case CMceSession::ETerminating: 
		    {	
		    iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdDeleteSession( iEngine, *this ) );	    
		    break;
		    }
	    case CMceSession::EIncoming:
	        {	            
		    iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdUpdateSession( iEngine, *this ) );
		    iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdUpdateSessionWithValues( iEngine, *this ) );

			iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdSendInfo( iEngine, *this ) );
			iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdDTMFInfo( iEngine, *this ) );
		    iCommands.Append( new ( ELeave )
		    	TMCETestUIEngineCmdSendInfoResponse( iEngine, *this ) );		        
		    break;
	        }
		case CMceSession::EEstablished:
		    {		         
		    iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdUpdateSession( iEngine, *this ) );
		    iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdUpdateSessionWithValues( iEngine, *this ) );
		    iCommands.Append( new (ELeave) TMCETestUIEngineCmdFCSendSession( 
                                iEngine, *this ) );
		    iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdTerminateSession( iEngine, *this ) );
		    iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdDeleteSession( iEngine, *this ) ); 
		    iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdHoldCall( iEngine, *this ) );     
		    iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdCreateReferSession( iEngine, *this ) );
		    iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdWriteSdpToFile( iEngine, *this ) );
		    
			iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdSendInfo( iEngine, *this ) );
			iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdDTMFInfo( iEngine, *this ) );
		    iCommands.Append( new ( ELeave )
		    	TMCETestUIEngineCmdSendInfoResponse( iEngine, *this ) );		        
		    		    	        
		    if(iSession->Type() == KMceSessionSecure) 
		    	{
		    	iCommands.Append( new (ELeave) 
		        	TMCETestUIEngineCmdCurrentCryptoContext( iEngine, *this ) );
		        iCommands.Append( new (ELeave) 
		        	TMCETestUIEngineCmdSupportedCryptoContext( iEngine, *this ) );
		        iCommands.Append( new (ELeave) 
		        	TMCETestUIEngineCmdSetCryptoContext( iEngine, *this ) );
		       	}
		     
		    break;
		    }
		case CMceSession::ETerminated: 
		    {	
		    iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdDeleteSession( iEngine, *this ) );
		    if(iSession->Type() == KMceSessionSecure) 
		    	{
		    	iCommands.Append( new (ELeave) 
		        	TMCETestUIEngineCmdCurrentCryptoContext( iEngine, *this ) );
		        iCommands.Append( new (ELeave) 
		        	TMCETestUIEngineCmdSupportedCryptoContext( iEngine, *this ) );
		        break;
		    	}
		     	    
		    break;
		    }
		default: 
		    {    
		    break;
		    }
		}  
		  
    return iCommands; 
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineSession::State
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& CMCETestUIEngineSession::State() 
    {
    if ( !iSession ) 
        {
        User::Panic( _L16("CMCETestUIEngine"), 0 );
        }
        
    TInt state = iSession->State();

	switch( state ) 
		{
		case CMceSession::EIdle: 
		    return KSessionStateIdle; 
		    break;
		case CMceSession::EOffering: 
		    return KSessionStateOffering; 
		    break;
		case CMceSession::EIncoming: 
		    return KSessionStateIncoming; 
		    break;
		case CMceSession::EReserving: 
		    return KSessionStateReserving; 
		    break;
		case CMceSession::EAnswering: 
		    return KSessionStateAnswering; 
		    break;
		case CMceSession::EProceeding: 
		    return KSessionStateProceeding; 
		    break;
		case CMceSession::EEstablished: 
		    return KSessionStateEstablished; 
		    break;
		case CMceSession::ECancelling: 
		    return KSessionStateCancelling; 
		    break;
		case CMceSession::ETerminating: 
		    return KSessionStateTerminating; 
		    break;
		case CMceSession::ETerminated: 
		    return KSessionStateTerminated; 
		    break;
		default: 
		    return KSessionStateUnknown; 
		    break;
		}    
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineSession::Direction
// -----------------------------------------------------------------------------
//
EXPORT_C TDirection CMCETestUIEngineSession::Direction() const
    {
    return iDirection;    
    }
// -----------------------------------------------------------------------------
// CMCETestUIEngineSession::VideoDirection
// -----------------------------------------------------------------------------
//
EXPORT_C TVideoDirection CMCETestUIEngineSession::VideoDirection() const
    {
    return iVideoDirection;    
    }
// -----------------------------------------------------------------------------
// CMCETestUIEngineSession::TextualDirection
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC16& CMCETestUIEngineSession::TextualDirection() const
    {
    switch ( Direction() )
        {
        case ESendRecv :
            {
            return KSessionDirectionSendRecv;
            break;
            }
        case ERecvOnly :
            {
            return KSessionDirectionRecvOnly;
            break;
            }
        case ESendOnly :
            {
            return KSessionDirectionSendOnly;
            break;
            }                        
        default:
            {
            break;
            }
        }
        
    return KSessionDirectionNotSpesified;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineSession::TextualVideoDirection
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC16& CMCETestUIEngineSession::TextualVideoDirection() const
    {
    switch ( VideoDirection() )
        {
        case EVideoSendRecv :
            {
            return KSessionDirectionSendRecv;
            break;
            }
        case EVideoRecvOnly :
            {
            return KSessionDirectionRecvOnly;
            break;
            }
        case EVideoSendOnly :
            {
            return KSessionDirectionSendOnly;
            break;
            }                        
        default:
            {
            break;
            }
        }
        
    return KSessionDirectionNotSpesified;
    }
// -----------------------------------------------------------------------------
// CMCETestUIEngineSession::AudioStreamsL
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray<CMCETestUIEngineAudioStream>& 
                CMCETestUIEngineSession::AudioStreamsL()
    {
    // Wrapped audiostreams are owned by session so we have to reconstruct 
    // audiostream wrappers to be sure that they are in valid state 
    iAudioStreams.ResetAndDestroy();
    
    const RPointerArray<CMceMediaStream>& streams = iSession->Streams();
    
    for ( TInt i = 0; i < streams.Count(); ++i )
        {
        if ( streams[i]->Type() == KMceAudio )
            {
            CMceAudioStream* audioStream = 
                                    static_cast<CMceAudioStream*>(streams[i]);
            CMCETestUIEngineAudioStream* engineAudioStream =
                CMCETestUIEngineAudioStream::NewL( iEngine, *audioStream );
            CleanupStack::PushL( engineAudioStream );
            iAudioStreams.Append( engineAudioStream );
            CleanupStack::Pop( engineAudioStream );
            //// code changed for MCE 2.0
            if(streams[i]->BoundStream())
            	{
            	if ( streams[i]->Type() == KMceAudio )
            		{
            		CMceAudioStream* boundStream = 
                                    static_cast<CMceAudioStream*>(&streams[i]->BoundStreamL());
		            CMCETestUIEngineAudioStream* engineBoundStream =
		                CMCETestUIEngineAudioStream::NewL( iEngine, *boundStream );
		            CleanupStack::PushL( engineBoundStream );
		            iAudioStreams.Append( engineBoundStream );
		            CleanupStack::Pop( engineBoundStream );
            		}
            	}
            //// code changed for MCE 2.0	
          /*  if ( audioStream->BoundStream() )
                {
                CMCEOneWayStream& oneWayStream = audioStream->BoundStreamL();
                if ( oneWayStream.Type() == KMCEAudio )
                    {
                    CMCEAudioStream& boundStream =
                            static_cast<CMCEAudioStream&>(oneWayStream); 
                    CMCETestUIEngineAudioStream* engineBoundStream =
                            CMCETestUIEngineAudioStream::NewL( iEngine, 
                                                               boundStream );
                    CleanupStack::PushL( engineBoundStream );
                    iAudioStreams.Append( engineBoundStream );
                    CleanupStack::Pop( engineBoundStream );        
                    }
                }
            */}
        }
    
    return iAudioStreams;
    }                
 
 // -----------------------------------------------------------------------------
// CMCETestUIEngineSession::VideoStreamsL
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray<CMCETestUIEngineVideoStream>& 
                CMCETestUIEngineSession::VideoStreamsL()
    {
    // Wrapped audiostreams are owned by session so we have to reconstruct 
    // audiostream wrappers to be sure that they are in valid state 
    iVideoStreams.ResetAndDestroy();
    
    const RPointerArray<CMceMediaStream>& streams = iSession->Streams();
    
    for ( TInt i = 0; i < streams.Count(); ++i )
        {
        if ( streams[i]->Type() == KMceVideo )
            {
            CMceVideoStream* videoStream = 
                                    static_cast<CMceVideoStream*>(streams[i]);
            CMCETestUIEngineVideoStream* engineVideoStream =
                CMCETestUIEngineVideoStream::NewL( iEngine, *videoStream );
            CleanupStack::PushL( engineVideoStream );
            iVideoStreams.Append( engineVideoStream );
            CleanupStack::Pop( engineVideoStream );
            //// code changed for MCE 2.0
            if(streams[i]->BoundStream())
            	{
            	if ( streams[i]->Type() == KMceVideo )
            		{
            		CMceVideoStream* boundStream = 
                                    static_cast<CMceVideoStream*>(&streams[i]->BoundStreamL());
		            CMCETestUIEngineVideoStream* engineBoundStream =
		                CMCETestUIEngineVideoStream::NewL( iEngine, *boundStream );
		            CleanupStack::PushL( engineBoundStream );
		            iVideoStreams.Append( engineBoundStream );
		            CleanupStack::Pop( engineBoundStream );
            		}
            	}
            //// code changed for MCE 2.0	
          /*  if ( audioStream->BoundStream() )
                {
                CMCEOneWayStream& oneWayStream = audioStream->BoundStreamL();
                if ( oneWayStream.Type() == KMCEAudio )
                    {
                    CMCEAudioStream& boundStream =
                            static_cast<CMCEAudioStream&>(oneWayStream); 
                    CMCETestUIEngineAudioStream* engineBoundStream =
                            CMCETestUIEngineAudioStream::NewL( iEngine, 
                                                               boundStream );
                    CleanupStack::PushL( engineBoundStream );
                    iAudioStreams.Append( engineBoundStream );
                    CleanupStack::Pop( engineBoundStream );        
                    }
                }
            */}
        }
    
    return iVideoStreams;
    }                
                        
                      
// -----------------------------------------------------------------------------
// CMCETestUIEngineSession::Session
// -----------------------------------------------------------------------------
//
CMceSession& CMCETestUIEngineSession::Session()
    {
    return *iSession;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineSession::ProfileId()
// -----------------------------------------------------------------------------
//
EXPORT_C TUint32 CMCETestUIEngineSession::ProfileId()
    {
    return iSession->Profile();
    }
// -----------------------------------------------------------------------------
// CMCETestUIEngineSession::SolveDirection
// -----------------------------------------------------------------------------
//
TDirection CMCETestUIEngineSession::SolveDirection()
    {
    
    /////////////////////
    //
    // TODO: This function should be refactored to use stream wrappers. 
    //       That will also simplify the function quite much.
    //
    //////////////////////
    
    TBool hasUpLink = EFalse;
    TBool hasDownLink = EFalse;
    
    const RPointerArray<CMceMediaStream>& streams = iSession->Streams();

    for ( TInt i = 0; i < streams.Count(); ++i )
        {        
        if ( streams[i]->Type() == KMceAudio )
            {
            CMceAudioStream* audioStream = 
                                    static_cast<CMceAudioStream*>(streams[i]);
            if ( audioStream->Source() )
                {
                if ( audioStream->Source()->Type() == KMceRTPSource )
                    {
                    hasDownLink = ETrue;
                    }
                }
            const RPointerArray<CMceMediaSink>& sinks = audioStream->Sinks();
            for ( TInt j = 0; j < sinks.Count(); ++j )
                {
                if ( sinks[j]->Type() == KMceRTPSink )
                    {
                    hasUpLink = ETrue;
                    }
                }
              //// code changed for MCE 2.0  
              if(audioStream->BoundStream())
              	{
              	CMceMediaStream& boundStream = audioStream->BoundStreamL();
              	 if ( boundStream.Source() )
                    {
                    if ( boundStream.Source()->Type() == KMceRTPSource )
                        {
                        hasDownLink = ETrue;
                        }
                    }
                  const RPointerArray<CMceMediaSink>& boundStreamSinks = 
                                                        boundStream.Sinks();
                  for ( TInt x = 0; x < boundStreamSinks.Count(); ++x )
                    {
                    if ( boundStreamSinks[x]->Type() == KMceRTPSink )
                        {
                        hasUpLink = ETrue;
                        }
                    }
                                                        
              	}
              //// code changed for MCE 2.0	
    /*        if ( audioStream->BoundStream() )
                {
                CMCEOneWayStream& boundStream = audioStream->BoundStreamL();
                if ( boundStream.Source() )
                    {
                    if ( boundStream.Source()->Type() == KMCERTPSource )
                        {
                        hasDownLink = ETrue;
                        }
                    }
                const RPointerArray<CMCEMediaSink>& boundStreamSinks = 
                                                        boundStream.Sinks();
                                                        
                for ( TInt x = 0; x < boundStreamSinks.Count(); ++x )
                    {
                    if ( boundStreamSinks[x]->Type() == KMCERTPSink )
                        {
                        hasUpLink = ETrue;
                        }
                    }
                }*/
            }
        }  
        
    if ( hasUpLink && hasDownLink )
        {
        iDirection = ESendRecv;
        }
    else if ( hasUpLink )
        {
        iDirection = ESendOnly;
        }
    else if ( hasDownLink )
        {
        iDirection = ERecvOnly;
        }  
    else
        {
        iDirection = ENotSpesified;
        }
        
    return iDirection;   
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineSession::CurrentAudioStreams
// -----------------------------------------------------------------------------

EXPORT_C RPointerArray<CMCETestUIEngineAudioStream>&
		CMCETestUIEngineSession::CurrentAudioStreams()
	{
	return iAudioStreams;	
	}
	

// -----------------------------------------------------------------------------
// CMCETestUIEngineSession::CurrentVideoStreams
// -----------------------------------------------------------------------------

EXPORT_C RPointerArray<CMCETestUIEngineVideoStream>&
		CMCETestUIEngineSession::CurrentVideoStreams()
	{
	return iVideoStreams;	
	}
	
// -----------------------------------------------------------------------------
// CMCETestUIEngineSession::SolveVideoDirection
// -----------------------------------------------------------------------------
//
TVideoDirection CMCETestUIEngineSession::SolveVideoDirection()
    {
    
    /////////////////////
    //
    // TODO: This function should be refactored to use stream wrappers. 
    //       That will also simplify the function quite much.
    //
    //////////////////////
    
    TBool hasUpLink = EFalse;
    TBool hasDownLink = EFalse;
    
    const RPointerArray<CMceMediaStream>& streams = iSession->Streams();

    for ( TInt i = 0; i < streams.Count(); ++i )
        {        
        if ( streams[i]->Type() == KMceVideo )
            {
            CMceVideoStream* videoStream = 
                                    static_cast<CMceVideoStream*>(streams[i]);
            if ( videoStream->Source() )
                {
                if ( videoStream->Source()->Type() == KMceRTPSource )
                    {
                    hasDownLink = ETrue;
                    }
                }
            const RPointerArray<CMceMediaSink>& sinks = videoStream->Sinks();
            for ( TInt j = 0; j < sinks.Count(); ++j )
                {
                if ( sinks[j]->Type() == KMceRTPSink )
                    {
                    hasUpLink = ETrue;
                    }
                }
              //// code changed for MCE 2.0  
              if(videoStream->BoundStream())
              	{
              	CMceMediaStream& boundStream = videoStream->BoundStreamL();
              	 if ( boundStream.Source() )
                    {
                    if ( boundStream.Source()->Type() == KMceRTPSource )
                        {
                        hasDownLink = ETrue;
                        }
                    }
                  const RPointerArray<CMceMediaSink>& boundStreamSinks = 
                                                        boundStream.Sinks();
                  for ( TInt x = 0; x < boundStreamSinks.Count(); ++x )
                    {
                    if ( boundStreamSinks[x]->Type() == KMceRTPSink )
                        {
                        hasUpLink = ETrue;
                        }
                    }
                                                        
              	}
              //// code changed for MCE 2.0	
    /*        if ( audioStream->BoundStream() )
                {
                CMCEOneWayStream& boundStream = audioStream->BoundStreamL();
                if ( boundStream.Source() )
                    {
                    if ( boundStream.Source()->Type() == KMCERTPSource )
                        {
                        hasDownLink = ETrue;
                        }
                    }
                const RPointerArray<CMCEMediaSink>& boundStreamSinks = 
                                                        boundStream.Sinks();
                                                        
                for ( TInt x = 0; x < boundStreamSinks.Count(); ++x )
                    {
                    if ( boundStreamSinks[x]->Type() == KMCERTPSink )
                        {
                        hasUpLink = ETrue;
                        }
                    }
                }*/
           }
        }  
        
    if ( hasUpLink && hasDownLink )
        {
        iVideoDirection = EVideoSendRecv;
        }
    else if ( hasUpLink )
        {
        iVideoDirection = EVideoSendOnly;
        }
    else if ( hasDownLink )
        {
        iVideoDirection = EVideoRecvOnly;
        }  
    else
        {
        iVideoDirection = EVideoNotSpesified;
        }
        
    return iVideoDirection;   
    }
    
 // ---------------------------------------------------------------------------
// CSVPHoldMediaHandler::EnableAudioL
// ---------------------------------------------------------------------------
//
 void CMCETestUIEngineSession::EnableAudioL( )
    {
    const RPointerArray< CMceMediaStream >& streams = iSession->Streams();
    TInt streamCount = streams.Count();
    for ( TInt i = 0; i < streamCount; i++ )
        {
        CMceMediaStream* mediaStream = streams[ i ];
        TMceMediaType mediaType = mediaStream->Type();
        if ( KMceAudio == mediaType )
            {
            // Now enable the audio streams.
            EnableAudioStreamL( mediaStream->BoundStreamL() );
            EnableAudioStreamL( *mediaStream );
            }
        }
    }

// ---------------------------------------------------------------------------
// CSVPHoldMediaHandler::EnableAudioStreamL
// ---------------------------------------------------------------------------
//
 void CMCETestUIEngineSession::EnableAudioStreamL( CMceMediaStream& aAudioStream )
    {
    
    if ( aAudioStream.Source() &&
         ( KMceMicSource == aAudioStream.Source()->Type() ||
           KMceRTPSource == aAudioStream.Source()->Type() ) )
        {
        CMceMediaSource* source = aAudioStream.Source();
        if ( !source->IsEnabled() )
            {
            // Enable source
            source->EnableL();
            }
        }
        
    if ( aAudioStream.Sinks().Count()  )
        {
        const RPointerArray< CMceMediaSink >& sinks =
             aAudioStream.Sinks();
        for ( TInt z = 0; z < sinks.Count(); z++ )
            {
            if ( KMceSpeakerSink == sinks[ z ]->Type() )
                {
                CMceSpeakerSink* speakerSink = 
                    ( CMceSpeakerSink* )  sinks[ z ];
                if ( !speakerSink->IsEnabled() )
                    {
                    // unmute speaker sink
                    speakerSink->EnableL();
                    }
                }
            } 
        }                                        

    }	

// ---------------------------------------------------------------------------
// CMCETestUIEngineSession::AdjustVideoCodecsL
// ---------------------------------------------------------------------------
//    
void CMCETestUIEngineSession::AdjustVideoCodecsL( 
    const RPointerArray<CMceVideoCodec>& aCodecs )
    {
    for ( TInt i = 0; i < aCodecs.Count(); i++ )
        {
        if ( aCodecs[ i ]->SdpName().Compare( KMceSDPNameH264 ) == 0 )
            {
            TUid encDecUid;
            encDecUid.iUid = KMceTestUiAvcEncoderUid;
            // Trapped as method is not supported at the moment
            TRAP_IGNORE( aCodecs[ i ]->SetPreferredEncodingDecodingDeviceL( encDecUid ) )
            }
        }
    }

// ---------------------------------------------------------------------------
// CMCETestUIEngineSession::AdjustVideoCodecsL
// ---------------------------------------------------------------------------
// 
CMceMediaStream* CMCETestUIEngineSession::FindStreamForSourceL(
    CMceMediaStream& aStream, 
    TMceSourceType aSourceType )
    {
    CMceMediaStream* streamForSource = NULL;
    
    CMceMediaSource* source = aStream.Source();
    if ( source && source->Type() == aSourceType )
	    {
	    streamForSource = &aStream;
	    }
	else
	    {
	    CMceMediaSource* boundStreamSource = 
	        aStream.BoundStream() ? aStream.BoundStreamL().Source() : NULL;
    	if ( boundStreamSource && boundStreamSource->Type() == aSourceType )
    	    {
    	    streamForSource = &aStream.BoundStreamL();
    	    }
	    }
	return streamForSource;
    }
    
// End of file

