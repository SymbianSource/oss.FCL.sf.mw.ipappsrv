/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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



        
// INCLUDE FILES
#include <mmf/devvideo/devvideobase.h>
#include <mmcccodecinformationfactory.h>
#include <mmcccodecinformation.h>
#include "mccvideosourceimpl.h"
#include "mccinternalcodecs.h"
#include "mmcccodecinformation.h"
#include "mmcccodech263.h"
#include "mmcccodecavc.h"
#include "mccvideosourcewrapper.h"
#include "mccvideosourcesinklogs.h"
#include "mccinternalevents.h"
#include "mmccinterfacedef.h"
#include "mccrateadaptationdef.h"
#include "mccdef.h"
#include "mccinternaldef.h"
#include "mccvideosourcekeyretriever.h"
#include "mccresources.h"
    
_LIT8(KVideoCodecFourCC, "video/%S; %S");

const TInt KMccMaxVideoQueueSize = 10;
const TUint KMccBitrateTooLowLimit = 20000;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::NewL
// -----------------------------------------------------------------------------
//
CMccVideoSourceImpl* CMccVideoSourceImpl::NewL( 
    TUint32 aEndpointId, MMccResources* aMccResources, TBool aConfigKeyRetrieval )
    {
    CMccVideoSourceImpl* self = new ( ELeave ) CMccVideoSourceImpl( 
            aEndpointId, aMccResources, aConfigKeyRetrieval );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::ConstructL
// -----------------------------------------------------------------------------
//
void CMccVideoSourceImpl::ConstructL()
    {
	iMediaRecorder = CMccVideoSourceWrapper::NewL(*this, *this);
    iVideoCodec.iFourCC = TFourCC();
    iAudioFourCC = TFourCC();
    iFrameRate = KMccDefaultVideoFrameRate;
    }
    
// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::CMccVideoSourceImpl
// -----------------------------------------------------------------------------
//
CMccVideoSourceImpl::CMccVideoSourceImpl( 
    TUint32 aEndpointId, MMccResources* aMccResources, TBool aConfigKeyRetrieval ) : 
    iAudioEnabled( EFalse ),
    iEndpointId( aEndpointId ),
    iMccResources( aMccResources ),
    iConfigKeyRetrieval( aConfigKeyRetrieval )
	{
    }
        
// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::~CMccVideoSourceImpl 
// -----------------------------------------------------------------------------
//
CMccVideoSourceImpl::~CMccVideoSourceImpl()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::~CMccVideoSourceImpl" )
    
    StopCameraMonitoring();
        
    if ( iMediaRecorder )
        {
        TRAP_IGNORE( iMediaRecorder->StopL() )
        }

	delete iVideoCodecFourCC;
	delete iMediaRecorder;
	iAsyncEventHandler = NULL;
	
	iVideoQueue.ResetAndDestroy();
	
	delete iConfigKey;	
	delete iConfigKeyData;
	
	__V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::~CMccVideoSourceImpl, exit" )
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::SetResources
// -----------------------------------------------------------------------------
//
void CMccVideoSourceImpl::SetResources( MMccResources* aResources )
	{
	__V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::SetResources" )		

	iMccResources = aResources;
	}
	
// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::SetCameraHandler
// -----------------------------------------------------------------------------
//
void CMccVideoSourceImpl::SetCameraHandler( MMccCameraHandler& aCameraHandler )
	{
	__V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::SetCameraHandler" )		

	iCameraHandler = &aCameraHandler;
	}

// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::SourcePrimeL
// -----------------------------------------------------------------------------
//
void CMccVideoSourceImpl::SourcePrimeL( TBool aMultipleCodecs )
	{
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::SourcePrimeL" )
    __ASSERT_ALWAYS( iMediaRecorder, User::Leave( KErrNotReady ) );
    __ASSERT_ALWAYS( iCameraHandler, User::Leave( KErrNotReady ) );

    iMultipleCodecs = aMultipleCodecs;
    
    if ( !iCameraHandler->IsCameraReady() )
        {
        __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::SourcePrimeL, camera not ready" )
        
        HandleCameraReservationL();
        }
    else if ( iMediaRecorder->State() == CCMRMediaRecorder::EStateNone )
        {       
        if ( !FullPrepareAllowed() )
            {
            __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::SourcePrimeL, prepare partially" )
            
            // Cannot prepare recorder yet fully, fake preparing and do it
            // later automatically once recording is started
            SendStreamEventToClient( KMccStreamPrepared, KErrNone, KMccAutomaticEvent );
            }
        else
            {
            __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::SourcePrimeL, prepare fully" )
            
            TUid encoderUid( KNullUid );
            TBool forceEncapsulationType( EFalse );
            DoCodecSpecificConfigurationL( encoderUid, forceEncapsulationType );
            
            __ASSERT_ALWAYS( iVideoCodecFourCC, User::Leave( KErrNotReady ) );
            	
            __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::SourcePrimeL, opening source" )		
        	iMediaRecorder->OpenL( iAudioSource, iCameraHandler->Handle(), 
        		*iVideoCodecFourCC, iAudioFourCC, encoderUid, forceEncapsulationType ) ;
            }
        }
    else
        {
        __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::SourcePrimeL, not priming" )
        }

    __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::SourcePrimeL, exit" )
	}

// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::SourcePlayL()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceImpl::SourcePlayL()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::SourcePlayL" )
    
    TMccEventType event = KMccEventNone;
    TUint32 eventNumData = 0;
    iMultipleCodecs = EFalse;
    
    if ( iMediaRecorder->State() == CCMRMediaRecorder::EStateNone )
        {
        __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::SourcePlayL, \
not prepared, prepare in autoplay mode" )
        SetAutoPlayMode( ETrue );
        SourcePrimeL();
        }
    else if ( iMediaRecorder->State() == CCMRMediaRecorder::EStateReadyToRecord )
        {        
	    iMediaRecorder->PlayL();
	    if ( AutoPlayMode() )
	        {
	        event = KMccStreamResumed;
	        eventNumData = KMccAutomaticEvent;
	        SetAutoPlayMode( EFalse );
	        }
	    else
	        {
	        event = KMccStreamStarted;
	        }
        }
    else
        {
        __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::SourcePlayL, \
already playing or stopping" )
        }
        
    if ( event != KMccEventNone && !AutoPlayMode() )
        {
        SendStreamEventToClient( event, KErrNone, eventNumData );
        }
    
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::SourcePlayL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::SourcePauseL()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceImpl::SourcePauseL()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::SourcePauseL")
    
    iVideoConsumer = NULL;
	iVideoConsumerBuffer = NULL;
	SetAutoPlayMode( EFalse );
	
	StopCameraMonitoring();
	    
    // Always stop the recorder, this will free camera resources
    iMediaRecorder->StopL();
    
    iVideoQueue.ResetAndDestroy();
    
    SendStreamEventToClient( KMccStreamPaused );
    	
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::SourcePauseL, exit")
	}

// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::SourceStopL()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceImpl::SourceStopL()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::SourceStopL" )
    __V_SOURCESINK_MEDIA_INT1("CMccVideoSourceImpl::SourceStopL, thread", \
        RThread().Id().operator TUint());
	iVideoConsumer = NULL;
	iVideoConsumerBuffer = NULL;
	iMultipleCodecs = EFalse;
	iMediaRecorder->StopL();

    iVideoQueue.ResetAndDestroy();
    
    SendStreamEventToClient( KMccStreamStopped );
    
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::SourceStopL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::SourceDataTypeCode()
// -----------------------------------------------------------------------------
//
TFourCC CMccVideoSourceImpl::SourceDataTypeCode( TMediaId aMediaId )
	{
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::SourceDataTypeCode" )

    if ( KUidMediaTypeVideo == aMediaId.iMediaType )
        {
        return iVideoCodec.iFourCC;
        }
    else if ( KUidMediaTypeAudio == aMediaId.iMediaType  )
        {
        return iAudioFourCC;
        }
    else
        {
        return TFourCC( KMMFFourCCCodeNULL );
        }
	}
	
// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::SetSourceDataTypeCode()
// -----------------------------------------------------------------------------
//
TInt CMccVideoSourceImpl::SetSourceDataTypeCode( 
    TFourCC aCodec, 
    TMediaId aMediaId )
	{
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::SetSourceDataTypeCode" )

    if ( KUidMediaTypeVideo == aMediaId.iMediaType &&
    	aCodec == iVideoCodec.iFourCC )
        {
        return KErrNone;
        }
    else if ( KUidMediaTypeAudio == aMediaId.iMediaType &&
    	aCodec == iAudioFourCC )
        {
        return KErrNone;
        }
    else
        {
        return KErrNotSupported;
        }
	}	

// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::BufferEmptiedL()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceImpl::BufferEmptiedL( CMMFBuffer* /*aBuffer*/ )
	{
    __V_SOURCESINK_MEDIA( "CMccVideoSourceImpl::BufferEmptiedL" )
    __ASSERT_ALWAYS( EFalse, User::Leave( KErrNotSupported ) );
	}
	
// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::CanCreateSourceBuffer()
// -----------------------------------------------------------------------------
//
TBool CMccVideoSourceImpl::CanCreateSourceBuffer()
	{
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::CanCreateSourceBuffer, ETrue" )
	return EFalse;
	}	

// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::CreateSourceBufferL()
// -----------------------------------------------------------------------------
//
CMMFBuffer* CMccVideoSourceImpl::CreateSourceBufferL( 
    TMediaId /*aMediaId*/, 
    TBool& /*aReference*/ )
	{
	__V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::CreateSourceBufferL" )
    CMMFBuffer* buffer = NULL;
    __ASSERT_ALWAYS( EFalse, User::Leave( KErrNotSupported ) );

	return buffer;	
	}
	
// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::SourceThreadLogon()
// -----------------------------------------------------------------------------
//
TInt CMccVideoSourceImpl::SourceThreadLogon( MAsyncEventHandler& aEventHandler )
	{
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::SourceThreadLogon" )	
	iAsyncEventHandler = static_cast<MAsyncEventHandler*>( &aEventHandler );
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::SourceThreadLogoff()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceImpl::SourceThreadLogoff()
	{
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::SourceThreadLogoff" )
    iAsyncEventHandler = NULL;
    
    iVideoQueue.ResetAndDestroy();
	}
	
// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::FillBufferL()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceImpl::FillBufferL( 
    CMMFBuffer* aBuffer,
    MDataSink* aConsumer,
    TMediaId aMediaId )
	{
	__V_SOURCESINK_MEDIA( "CMccVideoSourceImpl::FillBufferL" )
    
    // Sink might want to indicate pause/stop by passing NULL buffer
    if ( aBuffer )
        {
        __ASSERT_ALWAYS ( aConsumer, User::Leave( KErrArgument ) );
        }

    if ( aMediaId.iMediaType == KUidMediaTypeVideo )
		{
    	__V_SOURCESINK_MEDIA( "CMccVideoSourceImpl::FillBufferL, video" )
	    iVideoConsumer = aConsumer;
		iVideoConsumerBuffer = aBuffer;	
			
	    WriteFirstBufferL();
		}
	else
		{
    	__V_SOURCESINK_MEDIA_INT1( "CMccVideoSourceImpl::FillBufferL, \
unknown media, type=", aMediaId.iMediaType.iUid ) 
		User::Leave( KErrNotSupported );	
		}
	}	
                  
// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::NegotiateSourceL()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceImpl::NegotiateSourceL( MDataSink& /*aDataSink*/ )
	{
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::NegotiateSourceL" )
	}
	
// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::WriteBufferL()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceImpl::WriteBufferL( CCMRMediaBuffer* aBuffer )
	{
	__V_SOURCESINK_MEDIA( "CMccVideoSourceImpl::WriteBufferL" )
    
    __ASSERT_ALWAYS( aBuffer, User::Leave( KErrArgument ) );

    if ( QueueBufferL( *aBuffer ) )
        {
        WriteFirstBufferL();
        }
        
    __V_SOURCESINK_MEDIA( "CMccVideoSourceImpl::WriteBufferL, exit" )
	}
    
// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::SetVideoFrameSize()
// -----------------------------------------------------------------------------
//
TInt CMccVideoSourceImpl::SetVideoFrameSize( TSize aSize )
	{
    __V_SOURCESINK_CONTROLL_INT2( 
        "CMccVideoSourceImpl::SetVideoFrameSize (from mediarecorder), width", 
        aSize.iWidth,
        " heigth", 
        aSize.iHeight )
        
    iVideoCodec.iVideoHeight = aSize.iHeight;
    iVideoCodec.iVideoWidth = aSize.iWidth;
    				
    return KErrNone;				
	}

// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::SetAverageVideoBitRate()
// -----------------------------------------------------------------------------
//
TInt CMccVideoSourceImpl::SetAverageVideoBitRate( TInt aBitRate )
	{
    __V_SOURCESINK_CONTROLL_INT1( 
        "CMccVideoSourceImpl::SetAverageVideoBitRate (from mediarecorder)", 
        aBitRate )				
    iVideoCodec.iAverageBitrate = (TUint)aBitRate;
    return KErrNone;				
	}

// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::SetMaxVideoBitRate()
// -----------------------------------------------------------------------------
//
TInt CMccVideoSourceImpl::SetMaxVideoBitRate( TInt aBitRate )
	{
    __V_SOURCESINK_CONTROLL_INT1( 
        "CMccVideoSourceImpl::SetMaxVideoBitRate (from mediarecorder)", 
        aBitRate )
                                  				
    // Do not reset max bitrate as mediarecorder will call back with the same
    // value which was given in SetVideoBitrate phase (i.e. value is meaningless)
    aBitRate = aBitRate; 
    return KErrNone;				
	}

// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::SetAverageAudioBitRate()
// -----------------------------------------------------------------------------
//
TInt CMccVideoSourceImpl::SetAverageAudioBitRate( TInt aBitRate )
	{
    __V_SOURCESINK_CONTROLL_INT1( 
        "CMccVideoSourceImpl::SetAverageAudioBitRate (from mediarecorder)", 
        aBitRate )
    iAverageAudioBitRate = aBitRate;
    return KErrNone;				
	}
		
// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::MmroPrepareComplete()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceImpl::MmroPrepareComplete( TInt aError )    
	{
    __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSourceImpl::MmroPrepareComplete=", aError )
	
	if ( !aError )
    	{	
    	aError = SetVideoBitrate( iVideoCodec.iBitrate );  
        if ( !aError )
            {
            TRAP( aError, iMediaRecorder->SetVideoFrameRateL( iVideoCodec.iFramerate ) );
            
            if ( !aError )
                {
                TRateControlOptions options;
                TRAP( aError, iMediaRecorder->GetVideoRateControlOptionsL( options ) );
                if ( !aError )
                    {
                    __V_SOURCESINK_CONTROLL_INT1( 
                        "Ratecontrol options:: control:", options.iControl )
                    __V_SOURCESINK_CONTROLL_INT1( 
                        "Ratecontrol options:: bitrate:", options.iBitrate )
                    __V_SOURCESINK_CONTROLL_INT1( 
                        "Ratecontrol options:: quality:", options.iPictureQuality )
                    __V_SOURCESINK_CONTROLL_REAL( 
                        "Ratecontrol options:: rate:", options.iPictureRate )
                    __V_SOURCESINK_CONTROLL_REAL( 
                        "Ratecontrol options:: qtt:", options.iQualityTemporalTradeoff )
                    __V_SOURCESINK_CONTROLL_REAL( 
                        "Ratecontrol options:: lqt:", options.iLatencyQualityTradeoff )
        
                    // Enable constant bitrate (CBR i.e. more strict bitrate control)
                    options.iLatencyQualityTradeoff = KMccLatencyQyalityTradeoff; 
                    
					__V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::MmroPrepareComplete, set CBR" )
                    TRAP( aError, iMediaRecorder->SetVideoRateControlOptionsL( options ) );
                    }
                }
            }
    	}
    	
    	
	if ( AutoPlayMode() && !aError )
	    {
	    __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::MmroPrepareComplete, autoplay" )
	    
	    TRAPD( err, SourcePlayL() );
	    if ( err )
	        {
            SendStreamEventToClient( KMccStreamPrepared, err, KMccAutomaticEvent );
	        }
	    }
	else
	    { 
        SendStreamEventToClient( KMccStreamPrepared, aError, KMccAutomaticEvent );
	    }
	}

// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::MmroStateChange()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceImpl::MmroStateChange( TInt aState, TInt aError )
	{
    __V_SOURCESINK_CONTROLL_INT2( "CMccVideoSourceImpl::MmroStateChange, \
aState=", aState, "aError=", aError )

	if ( aError )
		{
		SendStreamEventToClient( KMccStreamError, aError );
		}
	else
		{
		switch ( aState )
			{
		    case CCMRMediaRecorder::EStateNone:
		        {	
		        __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSourceImpl::MmroStateChange, \
aState=EStateNone, aError=", aError )
		        }
		        break;
		    case CCMRMediaRecorder::EStateOpen:
		        {
		        __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSourceImpl::MmroStateChange, \
aState=EStateOpen, aError=", aError )	
		        }
		        break;
		    case CCMRMediaRecorder::EStatePreparing:
		        {
		        __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSourceImpl::MmroStateChange, \
aState=EStatePreparing, aError=", aError )			        	
		        }
		        break;
		    case CCMRMediaRecorder::EStateRecording:
		        {
		        __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSourceImpl::MmroStateChange, \
aState=EStateRecording, aError=", aError )		        	
		        }
		        break;
		    case CCMRMediaRecorder::EStatePausing:
		        {
		        __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSourceImpl::MmroStateChange, \
aState=EStatePausing, aError=", aError )			        	
		        }
		        break;
			case CCMRMediaRecorder::EStateStopping:
			    {
		        __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSourceImpl::MmroStateChange, \
aState=EStateStopping, aError=", aError )			        	
		        }
		        break;
		    case CCMRMediaRecorder::EStateReadyToRecord:
		    	{
		        __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSourceImpl::MmroStateChange, \
aState=EStateReadyToRecord, aError=", aError )	
		       	}
		        break;
		    case CCMRMediaRecorder::EStatePaused:
		        {
		        __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSourceImpl::MmroStateChange, \
aState=EStatePaused, aError=", aError )	
		        }
		        break;
		    default:
		        break;
			}	
		}
	}

// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::MmroTemporaryError()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceImpl::MmroTemporaryError( TInt aError )    
	{
    __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSourceImpl::MmroTemporaryError=", 
                                  aError )   
    
    SendStreamEventToClient( KMccStreamError, aError );                         
	}

// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::MmroFatalError()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceImpl::MmroFatalError( TInt aError )
	{
    __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSourceImpl::MmroFatalError=", aError )
    
    SendStreamEventToClient( KMccStreamError, aError );
	}

// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::CameraReady()
// -----------------------------------------------------------------------------
//	
void CMccVideoSourceImpl::CameraReady( TInt aError )
    {
    __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSourceImpl::CameraReady", aError )
    
    if ( !aError )
        {
        TRAP( aError, SourcePrimeL( iMultipleCodecs ) );
        }
    
    if ( aError )
        {
        SendStreamEventToClient( KMccStreamError, aError );
        }
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::SetVideoCodecL()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceImpl::SetVideoCodecL( const TMccCodecInfo& aVideoCodec )
	{
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::SetVideoCodecL " )
    
    iVideoCodec = aVideoCodec;	

    if ( iVideoCodec.iConfigKey.Length() > 0 && !iConfigKey )
        {
        __V_SOURCESINK_CONTROLL( "Setting config key" )
        iConfigKey = iVideoCodec.iConfigKey.AllocL();
        }

    __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSourceImpl::SetVideoCodecL, iVideoHeight ", 
        iVideoCodec.iVideoHeight )
    __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSourceImpl::SetVideoCodecL, iVideoWidth ", 
        iVideoCodec.iVideoWidth )
    __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSourceImpl::SetVideoCodecL, iVideoFourCC ", 
        iVideoCodec.iFourCC.FourCC() )
    __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSourceImpl::SetVideoCodecL, iVideoBitRate ",
        iVideoCodec.iBitrate )
    __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSourceImpl::SetVideoCodecL, iAverageVideoBitRate ", 
        iVideoCodec.iAverageBitrate )
    __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSourceImpl::SetVideoCodecL, iMaxVideoBitRate ", 
        iVideoCodec.iMaxBitrate )
    __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSourceImpl::SetVideoCodecL, iVideoCodecMode ", 
        iVideoCodec.iCodecMode )
    __V_SOURCESINK_CONTROLL_REAL( "CMccVideoSourceImpl::SetVideoCodecL, iFrameRate ", 
        iVideoCodec.iFramerate )
	}
	
// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::GetVideoCodecL()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceImpl::GetVideoCodecL( TMccCodecInfo& aVideoCodec )
	{
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::GetVideoCodecL " )
    
    aVideoCodec = iVideoCodec;
	}

// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::GetSupportedVideoCodecsL()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceImpl::GetSupportedVideoCodecsL( RArray<TFourCC>& /*aVideoTypes*/ )
	{
	__V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::GetSupportedVideoCodecsL " )
	}

// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::SetAudioCodecL()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceImpl::SetAudioCodecL( const TMccCodecInfo& aAudioCodec )
	{
	__V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::SetAudioCodecL" )
	iAudioFourCC = aAudioCodec.iFourCC;
    iAverageAudioBitRate = aAudioCodec.iAverageBitrate;
	}

// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::GetAudioCodecL()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceImpl::GetAudioCodecL( TMccCodecInfo& aAudioCodec )
	{
	__V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::GetAudioCodecL" )

	aAudioCodec.iFourCC = iAudioFourCC;
    aAudioCodec.iAverageBitrate = iAverageAudioBitRate;
	}

// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::GetSupportedAudioCodecsL()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceImpl::GetSupportedAudioCodecsL( RArray<TFourCC>& /*aAudioTypes*/ )
	{
	__V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::GetSupportedAudioCodecsL" )
	}

// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::SetAudioEnabledL()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceImpl::SetAudioEnabledL( TBool aEnabled )
	{
	__V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::SetAudioEnabledL" )
	iAudioEnabled = aEnabled;
	}

// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::AudioEnabledL()
// -----------------------------------------------------------------------------
//
TBool CMccVideoSourceImpl::AudioEnabledL() const
	{
	__V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::AudioEnabledL" )
	return iAudioEnabled;
	}

// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::SetGainL()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceImpl::SetGainL( TInt aGain )
	{
	__V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::SetGainL" )
	iAudioGain = aGain;
	}

// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::GainL()
// -----------------------------------------------------------------------------
//
TInt CMccVideoSourceImpl::GainL() const
	{
	__V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::GainL" )

	return iAudioGain;
	}

// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::MaxGainL()
// -----------------------------------------------------------------------------
//
TInt CMccVideoSourceImpl::MaxGainL() const
	{
	__V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::MaxGainL" )
	return iAudioGain;	
	}

// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::RateAdaptationRequest
// -----------------------------------------------------------------------------
//     
TInt CMccVideoSourceImpl::RateAdaptationRequest( 
    const TMccEvent& aInputData, 
    TMccEvent& aOutputData )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::RateAdaptationRequest, entry" )

    TRAPD( err, RateAdaptationRequestL( aInputData, aOutputData ) );
    
    __V_SOURCESINK_CONTROLL_INT1( 
        "CMccVideoSourceImpl::RateAdaptationRequest, exit with err:", err )
    
    return err;
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::IsStopped()
// -----------------------------------------------------------------------------
//   
TBool CMccVideoSourceImpl::IsStopped() const
    {
    return ( iMediaRecorder->State() == CCMRMediaRecorder::EStateNone );
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::GetConfigKeyL()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceImpl::GetConfigKeyL( 
    const TMccCodecInfo& aVideoCodec, 
    TDes8& aConfigKey,
    TBool aAllowAsyncRetrieval )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::GetConfigKeyL" )
    
    if ( iConfigKey )
        {
        __ASSERT_ALWAYS( iConfigKey->Length() <= aConfigKey.MaxLength(), 
                         User::Leave( KErrArgument ) );
        aConfigKey.Copy( *iConfigKey );
        }
    else
        {
        __ASSERT_ALWAYS( iCameraHandler != NULL, User::Leave( KErrNotReady ) );
        __ASSERT_ALWAYS( aAllowAsyncRetrieval, User::Leave( KErrNotReady ) );
        
        CMccVideoSourceConfigKeyRetriever* retriever = 
            CMccVideoSourceConfigKeyRetriever::NewLC( *iCameraHandler );
        retriever->RetrieveL( aVideoCodec, aConfigKey );
        CleanupStack::PopAndDestroy( retriever );
        
        iConfigKey = aConfigKey.AllocL();  
        }
        
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::GetConfigKeyL, entry" )
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::HasConfigKey()
// -----------------------------------------------------------------------------
//
TBool CMccVideoSourceImpl::HasConfigKey() const
    {
    return ( iConfigKey != NULL );
    }
    
// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::SendStreamEventToClient()
// -----------------------------------------------------------------------------
//	
void CMccVideoSourceImpl::SendStreamEventToClient( 
    TMccEventType aEventType,
    TInt aError,
    TUint32 aEventNumData )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::SendStreamEventToClient" )
    
    if ( iAsyncEventHandler )
	    {
	    aEventType = aError ? KMccStreamError : aEventType;
	    
	    TMccEvent event( 0, 
	                     0, 
	                     0, 
	                     iEndpointId, 
	                     KMccEventCategoryStream, 
	                     aEventType, 
	                     aError, 
	                     KNullDesC8 );
	                     
	    event.iEventNumData = aEventNumData;

		TMccInternalEvent internalEvent( KMccVideoSourceUid, 
		                                 EMccInternalEventNone,
		                                 event );
		                         
		iAsyncEventHandler->SendEventToClient( internalEvent );
	    }
	else
		{
		__V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::SendStreamEventToClient, \
		iAsyncEventHandler=NULL" )
		}
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::SetAutoPlayMode()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceImpl::SetAutoPlayMode( TBool aIsInAutoPlayMode )
    {
    iAutoPlayMode = aIsInAutoPlayMode;
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::AutoPlayMode()
// -----------------------------------------------------------------------------
//       
TBool CMccVideoSourceImpl::AutoPlayMode() const
    {
    return iAutoPlayMode;
    }


// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::CopyBufferData()
// -----------------------------------------------------------------------------
//
TBool CMccVideoSourceImpl::CopyBufferData( 
    CMMFBuffer& aDstBuffer, 
    CCMRMediaBuffer& aSrcBuffer )
    {
    CMMFDataBuffer& dstBuf = 
  	    static_cast<CMMFDataBuffer&>( aDstBuffer ); 		
       
    __V_SOURCESINK_MEDIA_INT2( "CMccVideoSourceImpl::CopyBufferData, \
src buf length =", aSrcBuffer.Data().Length(), " type =", aSrcBuffer.Type() )
        
  	__V_SOURCESINK_MEDIA_INT1( "CMccVideoSourceImpl::CopyBufferData, \
dst buf size =", dstBuf.BufferSize() )

    if ( aSrcBuffer.Type() == CCMRMediaBuffer::EVideoH264NALDecSpecInfo ||
         aSrcBuffer.Type() == CCMRMediaBuffer::EVideoH264BytestreamDecSpecInfo )
        {
        __V_SOURCESINK_CONTROLL_STR8( "Decoder spec info:", aSrcBuffer.Data() ) 
        TRAP_IGNORE( SetConfigKeyL( aSrcBuffer.Data() ) )
        }
        
    TBool copied( EFalse );
    if ( dstBuf.Data().MaxLength() >= aSrcBuffer.Data().Length() )
        {
        dstBuf.Data().Copy( aSrcBuffer.Data() );
        dstBuf.SetTimeToPlay( aSrcBuffer.TimeStamp().Int64() );
        dstBuf.SetLastBuffer( EFalse );
        copied = ETrue;
        }
        
    return copied;
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::CopyBufferData()
// -----------------------------------------------------------------------------
//
TBool CMccVideoSourceImpl::CopyBufferData( 
    CMMFBuffer& aDstBuffer, 
    CMMFDataBuffer& aSrcBuffer )
    {
    CMMFDataBuffer& dstBuf = 
  	    static_cast<CMMFDataBuffer&>( aDstBuffer ); 		
       
    __V_SOURCESINK_MEDIA_INT1( "CMccVideoSourceImpl::CopyBufferData, \
src buf size =", aSrcBuffer.BufferSize() )
        
  	__V_SOURCESINK_MEDIA_INT1( "CMccVideoSourceImpl::CopyBufferData, \
dst buf size =", dstBuf.BufferSize() )

    TBool copied( EFalse );
    if ( dstBuf.Data().MaxLength() >= aSrcBuffer.Data().Length() )
        {
        dstBuf.Data().Copy( aSrcBuffer.Data() );
        dstBuf.SetTimeToPlay( aSrcBuffer.TimeToPlay() );
        dstBuf.SetLastBuffer( EFalse );
        copied = ETrue;
        }
    return copied;
    }
        
// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::QueueBufferL()
// -----------------------------------------------------------------------------
//
TBool CMccVideoSourceImpl::QueueBufferL( CCMRMediaBuffer& aBuffer )
    {
    TBool queued( EFalse );
                               
    if ( iVideoQueue.Count() < KMccMaxVideoQueueSize )
        {
        CMMFDataBuffer* dataBuf = CMMFDataBuffer::NewL( aBuffer.Data().Length() );
        CleanupStack::PushL( dataBuf );
        if ( CopyBufferData( *dataBuf, aBuffer ) )
            {
            // Random access point buffers (IFrames) have special handling
            if ( aBuffer.RandomAccessPoint() )
                {
                AddIFrameToQueueL( dataBuf );
                CleanupStack::Pop( dataBuf );
                }
            else
                {
                iVideoQueue.AppendL( dataBuf );
                CleanupStack::Pop( dataBuf );
                }
            queued = ETrue;
            }
        else
            {
            CleanupStack::PopAndDestroy( dataBuf );
            }
        }
    return queued;
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::FetchQueuedBuffer()
// -----------------------------------------------------------------------------
//
CMMFDataBuffer* CMccVideoSourceImpl::FetchQueuedBuffer()
    {
    CMMFDataBuffer* queuedBuf = NULL;
    if ( iVideoQueue.Count() > 0 )
        {
        queuedBuf = iVideoQueue[ 0 ];
        iVideoQueue.Remove( 0 );
        }
    return queuedBuf;
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::DoBufferFilledL()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceImpl::DoBufferFilledL()
    {
    CMMFBuffer* tempBuf = iVideoConsumerBuffer;
    MDataSink* tempSink = iVideoConsumer;
    iVideoConsumer = NULL;
    iVideoConsumerBuffer = NULL;
    
    if ( tempSink )
        {
        tempSink->BufferFilledL( tempBuf );
        }
    }
    
// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::SetVideoBitrate()
// -----------------------------------------------------------------------------
//
TInt CMccVideoSourceImpl::SetVideoBitrate( TUint& aVideoBitrate )
    {
    // Respect maximum negotiation bitrate and do not allow too low values
    //

    __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::SetVideoBitrate" )
    
    TUint videoBitrate( aVideoBitrate );
    
    if ( videoBitrate > iMaxNegBitRate )
        {
        __V_SOURCESINK_CONTROLL( 
            "CMccVideoSourceImpl:: bitrate higher than negotiated" )
        videoBitrate = iMaxNegBitRate;
        }
        
    if ( videoBitrate > iVideoCodec.iMaxBitrate )
        {
        __V_SOURCESINK_CONTROLL( 
            "CMccVideoSourceImpl:: bitrate higher than maximum set" )
        videoBitrate = iVideoCodec.iMaxBitrate;
        }
    
    if ( videoBitrate < KMccBitrateTooLowLimit )
        {
        __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl:: bitrate too low" )   
        videoBitrate = KMccBitrateTooLowLimit;
        }
            
    TRAPD( err, iMediaRecorder->SetVideoBitrateL( videoBitrate ) );  
    if ( err == KErrNone )
        {
        aVideoBitrate = videoBitrate;
        }
    
    __V_SOURCESINK_CONTROLL_INT1( 
        "CMccVideoSourceImpl::SetVideoBitrate, exit with err", err )
        
    return err;
    }
    
// -----------------------------------------------------------------------------
// CMccVideoSourceImpl::RateAdaptationRequestL
// -----------------------------------------------------------------------------
//     
void CMccVideoSourceImpl::RateAdaptationRequestL( 
    const TMccEvent& aInputData, 
    TMccEvent& aOutputData )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::RateAdaptationRequestL" )
    
    TMccRateAdaptationEventDataPackage inputdatapkg;
	inputdatapkg.Copy(aInputData.iEventData);
	TMccRateAdaptationEventData inputdata = inputdatapkg();
    TMccRateAdaptationEventData outputdata = TMccRateAdaptationEventData();

#ifdef RATECONTROL

    if ( iMediaRecorder && 
       ( iMediaRecorder->State() == CCMRMediaRecorder::EStateRecording ||
         iMediaRecorder->State() == CCMRMediaRecorder::EStatePaused ) )
    	{
    	__V_SOURCESINK_CONTROLL( 
    	"CMccVideoSourceImpl::RateAdaptationRequest EStateRecording or EStatePaused" )

    	TUint bitrate = 0;
		
		bitrate = iMediaRecorder->GetVideoBitrateL();
		
		__V_SOURCESINK_CONTROLL( 
		"CMccVideoSourceImpl::RateAdaptationRequest GetVideoBitRateL done" )

		outputdata.iBitrateOriginal = bitrate;
		
		__V_SOURCESINK_CONTROLL_INT1(
		"CMccVideoSourceImpl::RateAdaptationRequest originalBitrate=", bitrate)	
		TUint mbitrate = TUint( inputdata.iRateAdaptationAdvice * TReal( bitrate ) );
		__V_SOURCESINK_CONTROLL_INT1(
		"CMccVideoSourceImpl::RateAdaptationRequest modifiedBitrate=", mbitrate)	
			
        TInt bitrateErr = SetVideoBitrate( mbitrate );
        if ( bitrateErr == KErrNone )
            {
            __V_SOURCESINK_CONTROLL( 
            "CMccVideoSourceImpl::RateAdaptationRequest SetVideoBitRate ok" )
            iVideoCodec.iBitrate = mbitrate; // This will be used after pause/resume
            outputdata.iBitrateModified = mbitrate;
            }
        else
            {
            __V_SOURCESINK_CONTROLL( 
            "CMccVideoSourceImpl::RateAdaptationRequest SetVideoBitRate nok" )
            outputdata.iBitrateModified = bitrate;
            }		

#ifdef FRAMERATECONTROL 

        TReal frameRate = KMccDefaultVideoFrameRate;
		
		if ( mbitrate > KIPULFrameRate10Limit )
			{
			__V_SOURCESINK_CONTROLL_INT1(
			"CMccVideoSourceImpl::RateAdaptationRequest frameRate=", (TInt) frameRate)	

			frameRate = iFrameRate;
			}
		else if (mbitrate < KIPULFrameRate5Limit )
			{
			__V_SOURCESINK_CONTROLL_INT1(
			"CMccVideoSourceImpl::RateAdaptationRequest frameRate=", (TInt) frameRate)	

			frameRate = Min( 5.0, (TUint) iFrameRate );
			}
		else 
			{
			__V_SOURCESINK_CONTROLL_INT1(
			"CMccVideoSourceImpl::RateAdaptationRequest frameRate=", (TInt) frameRate)	

			frameRate = Min( 10.0, (TUint) iFrameRate );
			}
		
		iMediaRecorder->SetVideoFrameRateL( frameRate );
		
		__V_SOURCESINK_CONTROLL( 
		"CMccVideoSourceImpl::RateAdaptationRequest SetVideoFrameRateL done" )
	
#endif //FRAMERATECONTROL 
	
		TMccRateAdaptationEventDataPackage temp( outputdata );
		aOutputData.iEventData.Copy( temp );
		
		__V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::RateAdaptationRequest end" )
		return;		
    	}
	
#endif //RATECONTROL
    	
    User::Leave( KErrNotSupported );
    }

// ---------------------------------------------------------------------------
// CMccVideoSourceImpl::DoCodecSpecificConfigurationL
// ---------------------------------------------------------------------------
//
void CMccVideoSourceImpl::DoCodecSpecificConfigurationL( 
    TUid& aEncoderUid, TBool& aForceEncapsulationType )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::DoCodecSpecificConfigurationL" )
    
    TBuf8<KMaxSdpNameLength> sdpName = iVideoCodec.iSdpName;
    
    if ( iVideoCodec.iSdpName.FindF( KH263SdpName ) != KErrNotFound )
        {
        __V_SOURCESINK_CONTROLL( "H263" )
        const TUint maxBitRateOne = 64000;
        const TUint maxBitRateTwo = 128000;
        
        if ( iVideoCodec.iMaxBitrate <= maxBitRateOne )
    	    {
            __V_SOURCESINK_CONTROLL( "level set 10" )			
    	    iMaxNegBitRate = maxBitRateOne;
    	    }
    	else if ( iVideoCodec.iMaxBitrate > maxBitRateOne && 
    		 iVideoCodec.iMaxBitrate <= maxBitRateTwo )  
    		{
            __V_SOURCESINK_CONTROLL( "level set 45" )			
    		iMaxNegBitRate = maxBitRateTwo;
    		}
        else
            {
            __V_SOURCESINK_CONTROLL( "max bitrate too high, set level 45" )	
    		iMaxNegBitRate = maxBitRateTwo;
    		iVideoCodec.iMaxBitrate = maxBitRateTwo;
            }

        if ( iVideoCodec.iCodecMode == KH263FormatModeOld )
            {
            __V_SOURCESINK_CONTROLL( "mode set 1998" )		
            }
        
        // Always use H263-2000 mime type as mediarecorder doesn't support 1998 mode    
        sdpName = KH2632000SdpName;
        }
    else if ( iVideoCodec.iSdpName.CompareF( KAVCSdpName ) == 0 )
        {
        __V_SOURCESINK_CONTROLL( "AVC" )
        
        iMaxNegBitRate = iVideoCodec.iMaxBitrate;
        __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSourceImpl::DoCodecSpecificConfigurationL, EncodingDecodingDeviceUid:", 
    		                              iVideoCodec.iEncodingDecodingDevice.iUid )
        aEncoderUid = iVideoCodec.iEncodingDecodingDevice;
        aForceEncapsulationType = ETrue;
        }
    else
        {
        // NOP
        }
    	
    HBufC8* fmtp = GetFmtpLC( iVideoCodec );
    
    delete iVideoCodecFourCC;
    iVideoCodecFourCC = NULL;
    iVideoCodecFourCC = 
        HBufC8::NewL( KVideoCodecFourCC().Length() + KMaxSdpNameLength + fmtp->Length() );
        
    iVideoCodecFourCC->Des().Format( KVideoCodecFourCC, &sdpName, &*fmtp );
    
    CleanupStack::PopAndDestroy( fmtp );
             	
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::DoCodecSpecificConfigurationL, exit" );	
    }
    
// ---------------------------------------------------------------------------
// CMccVideoSourceImpl::GetFmtpLC
// ---------------------------------------------------------------------------
//
HBufC8* CMccVideoSourceImpl::GetFmtpLC( const TMccCodecInfo& aCodecInfo )    
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::GetFmtpLC" )
    
    CMccCodecInformationFactory* factory = CMccCodecInformationFactory::NewL();
    CleanupStack::PushL( factory );
    CMccCodecInformation* codec = 
        factory->CreateCodecInformationL( aCodecInfo.iSdpName );
    CleanupStack::PushL( codec );
    codec->SetValues( aCodecInfo );
    HBufC8* fmtp = codec->GetFmtpL().AllocL();
    CleanupStack::PopAndDestroy( codec );
    CleanupStack::PopAndDestroy( factory );
    CleanupStack::PushL( fmtp );
    
    __V_SOURCESINK_CONTROLL_STR8( "fmtp:", *fmtp )
    
    if ( aCodecInfo.iSdpName.CompareF( KAVCSdpName ) == 0 )
        {
        __V_SOURCESINK_CONTROLL( "avc handling" )
        
        TPtr8 ptrFmtp( fmtp->Des() );
        // Strip useless information off from SDP 
        // (mediarecorder does not accept packetization mode etc.)
        TInt index = ptrFmtp.Find( KTxtProfileLevelId );
        if ( index >= 0 )
            {
            _LIT8( KSemicolon, ";" );
            TInt semicolonIndex = ptrFmtp.Mid( index ).Find( KSemicolon );
            
            if ( semicolonIndex > 0 )
                {
                HBufC8* strippedFmtp = ptrFmtp.Mid( index, semicolonIndex ).AllocL();
                CleanupStack::PopAndDestroy( fmtp );
                fmtp = strippedFmtp;
                CleanupStack::PushL( fmtp );
                __V_SOURCESINK_CONTROLL_STR8( "stripped fmtp:", *fmtp )
                }
            }
        }
    
    return fmtp;
    }

// ---------------------------------------------------------------------------
// CMccVideoSourceImpl::SetConfigKeyL
// Base 64 encoded version of key is more for session negotiation and does
// not need to be updated if it already existis. Raw data config key is
// used inside stream and it will represent current configuration information
// as it will be received from mediarecorded. Therefore keep updating it.
// ---------------------------------------------------------------------------
//            
void CMccVideoSourceImpl::SetConfigKeyL( const TDesC8& aData )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::SetConfigKeyL" )
    
    if ( iConfigKey == NULL )
        {
        TInt bufferProgressIndex( 0 );
        TInt part1StartIndex = 
            TMccCodecInfo::FindAvcNaluStart( bufferProgressIndex, aData );
        __ASSERT_ALWAYS( part1StartIndex != KErrNotFound, User::Leave( KErrNotFound ) );

        TInt part1EndIndex = TMccCodecInfo::FindAvcNaluEnd( bufferProgressIndex, aData );
        __ASSERT_ALWAYS( part1EndIndex != KErrNotFound, User::Leave( KErrNotFound ) );
                         
        TInt part2StartIndex = TMccCodecInfo::FindAvcNaluStart( bufferProgressIndex, aData );
        __ASSERT_ALWAYS( part2StartIndex != KErrNotFound, User::Leave( KErrNotFound ) );

        TInt part2EndIndex = TMccCodecInfo::FindAvcNaluEnd( bufferProgressIndex, aData );
        __ASSERT_ALWAYS( part2EndIndex != KErrNotFound, User::Leave( KErrNotFound ) );
        
        TInt part1Len = part1EndIndex - part1StartIndex;
        HBufC8* confKeyPart1 =
            MccConversionUtility::Base64EncodeLC( aData.Mid( part1StartIndex, part1Len ) );
        
        TInt part2Len = part2EndIndex - part2StartIndex;
        HBufC8* confKeyPart2 =
            MccConversionUtility::Base64EncodeLC( aData.Mid( part2StartIndex, part2Len ) );
            
        _LIT8( KMccVideoSourceNaluSeparator, "," );
        
        iConfigKey = HBufC8::NewL( 
            confKeyPart1->Length() + 
            confKeyPart2->Length() + 
            KMccVideoSourceNaluSeparator().Length() );
        TPtr8 ptrConfigKey( iConfigKey->Des() );    
		ptrConfigKey.Append( *confKeyPart1 );
		ptrConfigKey.Append( KMccVideoSourceNaluSeparator() );
		ptrConfigKey.Append( *confKeyPart2 );

        CleanupStack::PopAndDestroy( confKeyPart2 );
        CleanupStack::PopAndDestroy( confKeyPart1 );
        
        __V_SOURCESINK_CONTROLL_STR8( "base64 encoded:", *iConfigKey ) 
        }
    
    delete iConfigKeyData;
    iConfigKeyData = NULL;
    iConfigKeyData = aData.AllocL();
        
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::SetConfigKeyL, exit" )
    }

// ---------------------------------------------------------------------------
// CMccVideoSourceImpl::FullPrepareAllowed
// ---------------------------------------------------------------------------
//
TBool CMccVideoSourceImpl::FullPrepareAllowed()
    {
    TBool allowed( !iMultipleCodecs );
    if ( iVideoCodec.iSdpName.CompareF( KAVCSdpName ) == 0 )
        {
        // Config key needs to exists before full prepare is allowed for AVC
        allowed &= ( iConfigKey != NULL || iConfigKeyRetrieval );
        }
        
    return allowed;
    }

// ---------------------------------------------------------------------------
// CMccVideoSourceImpl::WriteFirstBufferL
// ---------------------------------------------------------------------------
//
void CMccVideoSourceImpl::WriteFirstBufferL()
    {
    if ( iVideoConsumerBuffer )
        {
        __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::WriteFirstBufferL" )
        
    	CMMFDataBuffer* queuedBuf = FetchQueuedBuffer();
    	CleanupStack::PushL( queuedBuf );
    	if ( queuedBuf && CopyBufferData( *iVideoConsumerBuffer, *queuedBuf ) )
    	    {
    	    DoBufferFilledL();
    	    }
    	CleanupStack::PopAndDestroy( queuedBuf );
    	
    	__V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::WriteFirstBufferL, exit" )
        }
    }

// ---------------------------------------------------------------------------
// CMccVideoSourceImpl::AddIFrameToQueueL
// In case of AVC is used, config data is sent before each iframe.
// Config data is not sent before first iframe or if queue doesn't have
// room for config data.
// ---------------------------------------------------------------------------
//
void CMccVideoSourceImpl::AddIFrameToQueueL( CMMFDataBuffer* aBuffer )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::AddIFrameToQueueL" )
    
    __ASSERT_ALWAYS( aBuffer, User::Leave( KErrArgument ) );
    
    if ( iConfigKeyData && 
       ( iVideoCodec.iSdpName.CompareF( KAVCSdpName ) == 0 ) &&
       ( iVideoQueue.Count() < ( KMccMaxVideoQueueSize - 1 ) ) &&
         iFirstIFrameQueued )
        {
        __V_SOURCESINK_CONTROLL( 
            "CMccVideoSourceImpl::AddIFrameToQueueL, adding config data" )
            
        CMMFDataBuffer* configDataBuf = 
            CMMFDataBuffer::NewL( iConfigKeyData->Length() );
        CleanupStack::PushL( configDataBuf );
        configDataBuf->Data().Copy( *iConfigKeyData );
        configDataBuf->SetTimeToPlay( aBuffer->TimeToPlay() );
        configDataBuf->SetLastBuffer( EFalse );
        iVideoQueue.AppendL( configDataBuf );
        CleanupStack::Pop( configDataBuf );
        }
    
    if ( iMccResources )
        {
        __V_SOURCESINK_CONTROLL( 
                "CMccVideoSourceImpl::AddIFrameToQueueL, store iframe info" )
        iMccResources->StoreKeyFrameInfoL( iEndpointId, *aBuffer );
        
        __V_SOURCESINK_CONTROLL( 
                "CMccVideoSourceImpl::AddIFrameToQueueL, info stored" )
        }
    
    iVideoQueue.AppendL( aBuffer );
    
    iFirstIFrameQueued = ETrue;
    
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::AddIFrameToQueueL, exit" )
    }

// ---------------------------------------------------------------------------
// CMccVideoSourceImpl::HandleCameraReservationL
// Camera is needed only for real once autoplay is enabled or config key
// is retrieved. If retrieving config key, we need to force camera creation.
// ---------------------------------------------------------------------------
//
void CMccVideoSourceImpl::HandleCameraReservationL()
    {
    if ( AutoPlayMode() || iConfigKeyRetrieval )
        {
        TBool forceImmediateCameraCreation( iConfigKeyRetrieval );
        
        __V_SOURCESINK_CONTROLL_INT1( 
                "CMccVideoSourceImpl::HandleCameraReservationL, force", 
                forceImmediateCameraCreation )
        
        User::LeaveIfError( iCameraHandler->MonitorCameraReservation( 
                *this, forceImmediateCameraCreation ) );
        }
    else
        {
        __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::HandleCameraReservationL, nop" )
        
        SendStreamEventToClient( KMccStreamPrepared, KErrNone, KMccAutomaticEvent );
        }
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceImpl::HandleCameraReservationL, exit" )
    }

// ---------------------------------------------------------------------------
// CMccVideoSourceImpl::StopCameraMonitoring
// ---------------------------------------------------------------------------
//
void CMccVideoSourceImpl::StopCameraMonitoring()
    {
    if ( iCameraHandler )
        {
        // Unregister, ignore error as we might not be even monitoring
        // Can release resource as if it was created just for temporary purpose
        TBool forceImmediateCameraRelease( iConfigKeyRetrieval );
        iCameraHandler->StopCameraMonitoring( 
                *this, forceImmediateCameraRelease );
        }
    }

#ifndef EKA2
// DLL interface code
EXPORT_C TInt E32Dll( TDllReason )
    {
    return KErrNone;
    }
#endif
