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
#include <MCEInSession.h>
#include <MCEAudioStream.h>
#include <MCEFCMsgExchange.h>
#include <mcertpsource.h>
#include <mcedisplaysink.h>
#include <mcespeakersink.h>
#include <MCEmediaSink.h>
#include <MCEVideoCodec.h>
#include <mcesecuresession.h>
#include <mcecamerasource.h>
#include <mcevideostream.h>
#include <aknutils.h>
#include "MCETestUIEngineDefaultValues.h"

#include "CMCETestUIEngineMultimediaObserver.h"
#include "CMCETestUIEngine.h"
#include "CMCETestUIEngineSession.h"
#include "CMCETestUIEngineInSession.h"

#include "TMCETestUIEngineCmdAcceptSession.h"
#include "TMCETestUIEngineCmdRejectSession.h"
#include "TMCETestUIEngineCmdRingingSession.h"
#include "TMCETestUIEngineCmdDeleteSession.h"
#include "TMCETestUIEngineCmdFloorControlSession.h"
#include "TMCETestUIEngineCmdCryptoHandling.h"

#include "TMCETestUIEngineCmdSRReqRes.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMCETestUIEngineInSession::NewL()
// -----------------------------------------------------------------------------
//
CMCETestUIEngineInSession* CMCETestUIEngineInSession::NewL(
            CMCETestUIEngine& aEngine,
            CMceInSession* aInSession ) 
    {
    CMCETestUIEngineInSession* self = 
        new (ELeave) CMCETestUIEngineInSession( aEngine, aInSession );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;  
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineInSession::CMCETestUIEngineInSession
// -----------------------------------------------------------------------------
//
CMCETestUIEngineInSession::CMCETestUIEngineInSession(
            CMCETestUIEngine& aEngine,
            CMceInSession* aInSession )
    :CMCETestUIEngineSession( aEngine, ENotSpesified, EVideoNotSpesified),
     iInSession( aInSession ),
     iId( 0 )
    {
    }
     

// -----------------------------------------------------------------------------
// CMCETestUIEngineInSession::ConstructL()
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineInSession::ConstructL()
    {
    User::LeaveIfError( iInSession ? KErrNone : KErrArgument );
    
    BaseConstructL();
    
	if(iEngine.IsFCSession()) 
		{
		TRAP_IGNORE(iEngine.FloorControl().InitializeL(
										*iInSession,0, iEngine.MMObserver()));	
		}
    iSession = iInSession;
    
    if( iEngine.iSdp )
        {
        CDesC8Array* array = new (ELeave) CDesC8ArrayFlat(2);
	    array->AppendL(_L8("a=IncomingSession1:20\r\n"));
        array->AppendL(_L8("a=IncomingSession2:40\r\n"));
	    iInSession->SetSessionSDPLinesL( array );
	    AddSdpToStreamsL( iInSession );
        }
        
    AdjustRectSize(iInSession); // Main rect adjusted
    SolveDirection();
    SolveVideoDirection();
    SetAudioRouting( iInSession );
            
    UpdateVideoParametersL( iInSession ); // Possibly secondary rect adjusted
     
    // Get identity
    iId = iEngine.NextInSessionId();
    iType = KMceInSession;
   
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineInSession::AdjustRectSize
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineInSession::AdjustRectSize(CMceInSession* aSession)
	{
	const RPointerArray<CMceMediaStream>& streams = aSession->Streams();

    if ( streams.Count() == 0 )
        {
        User::Leave( KErrNoMemory );
        }
	for ( TInt i = 0; i < streams.Count(); ++i )
		{
		CDesC8Array* array = new (ELeave) CDesC8ArrayFlat(2);
		CMceMediaStream* mediaStream = 
                                    static_cast<CMceMediaStream*>(streams[i]);
                                    
        const RPointerArray<CMceMediaSink>& sinks = streams[i]->Sinks();
        
        for ( TInt j = 0; j < sinks.Count(); ++j )
        	{	
	        	if( sinks[j]->Type() == KMceDisplaySink  )
	            {
	            CMceDisplaySink* dispSink = static_cast<CMceDisplaySink*>(sinks[j]);
	           // TRect rect(iEngine.GetRect());
	            TRect rect(32, 88, 208, 232);
	            AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EMainPane, rect );
	           	dispSink->SetDisplayRectL(rect);
	            }
        	}
        
         }
	}


// -----------------------------------------------------------------------------
// CMCETestUIEngineInSession::UpdateVideoParametersL
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineInSession::UpdateVideoParametersL(CMceInSession* aSession)
    {
    if ( iVideoDirection == EVideoSendRecv )
	    {
	    CMceCameraSource* cameraSource = NULL;
	    const RPointerArray<CMceMediaStream>& streams = aSession->Streams(); 
        for ( TInt j = 0; j < streams.Count() && !cameraSource; ++j )
        	{
        	CMceMediaStream* cameraStream = FindStreamForSourceL( *streams[ j ], KMceCameraSource );
        	
        	if ( cameraStream )
        	    {
        	    cameraSource = static_cast<CMceCameraSource*>( cameraStream->Source() );
        	    } 
        	}
            	
	    if ( iEngine.iInSessionSettings.iCameraSetting == TSessionParams::EMceCameraSecondary &&
	         cameraSource )
	        {
	        if ( cameraSource->CamerasAvailable() > 1 )
	            {
	            cameraSource->SetCameraIndexL( 1 );
	            }
	        }
	        
	    if ( iEngine.iInSessionSettings.videoSinkDisplay == TSessionParams::EMceViewFinderEnabled &&
	         cameraSource )
	        {
	        CMceVideoStream* videoLocalStream = CMceVideoStream::NewL();
            CleanupStack::PushL( videoLocalStream );
            CMceDisplaySink* localDispSink = CMceDisplaySink::NewLC( iEngine.MCEManager() );
        
            TRect localDisplayRect(0, 0, 36, 44);
            AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EStatusPane, localDisplayRect );
            localDispSink->SetDisplayRectL( localDisplayRect );
            localDispSink->SetPriorityL( ECoeWinPriorityAlwaysAtFront );
        	videoLocalStream->AddSinkL( localDispSink );
        	CleanupStack::Pop( localDispSink );
        	
        	videoLocalStream->SetSourceL( cameraSource );
        	
        	aSession->AddStreamL( videoLocalStream );
        	CleanupStack::Pop( videoLocalStream );
	        }
	    }
    
    if ( iVideoDirection == EVideoSendRecv || iVideoDirection == EVideoSendOnly )
        {
        const RPointerArray<CMceMediaStream>& streams = aSession->Streams();
        for ( TInt k = 0; k < streams.Count(); k++ )
            {
           	CMceMediaStream* cameraStream = FindStreamForSourceL( *streams[ k ], KMceCameraSource );
           	if ( cameraStream )
           	    {
           	    CMceVideoStream* videoStream = static_cast<CMceVideoStream*>( cameraStream );
           	    AdjustVideoCodecsL( videoStream->Codecs() );
           	    }
            }
        }
    }
    
// -----------------------------------------------------------------------------
// CMCETestUIEngineInSession::AddSdpToStreamsL
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineInSession::SetAudioRouting(CMceInSession* aSession)
	{
	const RPointerArray<CMceMediaStream>& streams = aSession->Streams();

    if ( streams.Count() == 0 )
        {
        User::Leave( KErrNoMemory );
        }
	for ( TInt i = 0; i < streams.Count(); ++i )
		{
		CMceMediaStream* mediaStream = 
                                    static_cast<CMceMediaStream*>(streams[i]);
        
        if( mediaStream->Sinks()[0]->Type() == KMceSpeakerSink  )
            {
            CMceSpeakerSink* speakerSink = ( CMceSpeakerSink* ) mediaStream->Sinks()[0];
            speakerSink->SetRoutingL( KMceAudioRoutePrivate );
            }

        if( mediaStream->BoundStream() )
            {
            CMceMediaStream* boundStream = &mediaStream->BoundStreamL();
            if( boundStream->Sinks()[0]->Type() == KMceSpeakerSink  )
                {
                CMceSpeakerSink* speakerSink = ( CMceSpeakerSink* ) boundStream->Sinks()[0];
                speakerSink->SetRoutingL( KMceAudioRoutePrivate );
              
                }

            }
		}
    }
    
// -----------------------------------------------------------------------------
// CMCETestUIEngineInSession::AddSdpToStreamsL
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineInSession::AddSdpToStreamsL(CMceInSession* aSession)
	{
	const RPointerArray<CMceMediaStream>& streams = aSession->Streams();

    if ( streams.Count() == 0 )
        {
        User::Leave( KErrNoMemory );
        }
	for ( TInt i = 0; i < streams.Count(); ++i )
		{
		CDesC8Array* array = new (ELeave) CDesC8ArrayFlat(2);
		CMceMediaStream* mediaStream = 
                                    static_cast<CMceMediaStream*>(streams[i]);
        
        if( mediaStream->Source()->Type() == KMceRTPSource  )
            {
            array->AppendL(_L8("a=IncomingStream1DL:20\r\n"));
            array->AppendL(_L8("a=MainStream:40\r\n"));
            }
        else
            {
            array->AppendL(_L8("a=IncomingStream1UL:20\r\n"));
            array->AppendL(_L8("a=MainStream:40\r\n"));
            }
            
        mediaStream->SetMediaAttributeLinesL( array );    
        
        CDesC8Array* array2 = new (ELeave) CDesC8ArrayFlat(2);
        
        if( mediaStream->BoundStream() )
            {
            CMceMediaStream* boundStream = &mediaStream->BoundStreamL();
            if( boundStream->Source()->Type() == KMceRTPSource  )
                {
                array2->AppendL(_L8("a=IncomingStream1DL:20\r\n"));
                array2->AppendL(_L8("a=BoundStream:40\r\n"));
                }
            else
                {
                array2->AppendL(_L8("a=IncomingStream1UL:20\r\n"));
                array2->AppendL(_L8("a=BoundStream:40\r\n"));
                }
            boundStream->SetMediaAttributeLinesL( array2 ); 
            }
      	}
    }
    
    
    

// -----------------------------------------------------------------------------
// CMCETestUIEngineInSession::~CMCETestUIEngineInSession
// -----------------------------------------------------------------------------
//
EXPORT_C CMCETestUIEngineInSession::~CMCETestUIEngineInSession()
    {
    delete iInSession;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineInSession::GetCommands
// -----------------------------------------------------------------------------
//
const RPointerArray<TMCETestUIEngineCmdBase>& 
            CMCETestUIEngineInSession::GetCommandsL()
    {
    iCommands.ResetAndDestroy();

    // Get base class commands
    CMCETestUIEngineSession::GetCommandsL();

    // Get own commands
    TInt state = iInSession->State();

	switch( state ) 
		{
		case CMceSession::EIncoming: 
		    {
		    iCommands.Append( new (ELeave) TMCETestUIEngineCmdRejectSession( 
                                iEngine, *this ) );	
            iCommands.Append( new (ELeave) TMCETestUIEngineCmdFCSendSession( 
                                iEngine, *this ) );
            iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdDeleteSession( iEngine, *this ) );  
		        
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
		case CMceSession::EProceeding:
		    {
		    iCommands.Append( new (ELeave) TMCETestUIEngineCmdAcceptSession( 
                                iEngine, *this ) );
		    iCommands.Append( new (ELeave) TMCETestUIEngineCmdRejectSession( 
                                iEngine, *this ) );	
		    iCommands.Append( new (ELeave) TMCETestUIEngineCmdRingingSession( 
                                iEngine, *this ) );
		    iCommands.Append( new (ELeave) TMCETestUIEngineCmdRespondSession( 
                                iEngine, *this ) );
            iCommands.Append( new (ELeave) TMCETestUIEngineCmdFCSendSession( 
                                iEngine, *this ) );	    
		    iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdDeleteSession( iEngine, *this ) ); 

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
		       	}
		                        
            break;
		    }
		case CMceSession::EOffering: 
		    {
		    iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdDeleteSession( iEngine, *this ) );  
		        
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
		       	}
		                                
		    break;	
		    }
	    case CMceSession::EEstablished:
			{
			if(VideoDirection() != EVideoNotSpesified)    
		    	{
		    	if(iEngine.IsVideoEnabled())
		    	{
		    	iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdDisableReceivingVideoSession( iEngine, *this ) );
		    	}
		    	else
		    	{
		    	iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdEnableReceivingVideoSession( iEngine, *this ) );     		        
		    	}
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
// CMCETestUIEngineInSession::Id
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMCETestUIEngineInSession::Id() const
    {
    return iId;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineInSession::InSession
// -----------------------------------------------------------------------------
//
CMceInSession& CMCETestUIEngineInSession::InSession()
    {
    return *iInSession;
    }



