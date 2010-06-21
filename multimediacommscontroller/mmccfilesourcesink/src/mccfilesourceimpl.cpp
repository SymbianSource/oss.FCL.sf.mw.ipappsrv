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
#include <es_sock.h>
#include <e32std.h>
#include "mccfilesourceimpl.h"
#include "mmcccodecinformation.h"
#include "mccfilesourcelogs.h"
#include "mccfileaudio.h"
#include "mccfilevideo.h"
#include "mccinternalevents.h"
#include "mmccinterfacedef.h"
#include "mccinternaldef.h"



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::NewL
// Static constructor.
// -----------------------------------------------------------------------------
//
CMccFileSourceImpl* CMccFileSourceImpl::NewL( 
    TUint32 aEndpointId, 
    MMccResources* aMccResources )
    {
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::NewL" )
    
    CMccFileSourceImpl* self = new ( ELeave ) CMccFileSourceImpl( aEndpointId );
    CleanupStack::PushL(self);
    self->ConstructL( aMccResources );
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::ConstructL
// -----------------------------------------------------------------------------
//
void CMccFileSourceImpl::ConstructL( MMccResources* aMccResources )
    {
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::ConstructL" )
    
    iFileAudio = CMccFileAudio::NewL( *this );
    iFileVideo = CMccFileVideo::NewL( *this, aMccResources, iEndpointId );
    }
    
// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
//
CMccFileSourceImpl::CMccFileSourceImpl( TUint32 aEndpointId )
	: iCurrentState( EConstructed ),
	  iIsAudio ( EFalse ),
	  iIsVideo ( EFalse ),
	  iEndpointId( aEndpointId ),
	  iDelayAfterStop( 0 ),
	  iEofTimer( 0 )
    {
    }
        
// -----------------------------------------------------------------------------
// CMccFileSourceImpl::~CMccFileSourceImpl.
// -----------------------------------------------------------------------------
//
CMccFileSourceImpl::~CMccFileSourceImpl()
    {
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::~CMccFileSourceImpl" )
 
    StopEofTimer();

    if ( iMP4Handle )
	    {
        MP4ParseClose( iMP4Handle );
	    iMP4Handle = NULL;
	    }

    delete iFileAudio;
	delete iFileVideo;  

	iAsyncEventHandler = NULL;
    }

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::AddDataSink(MCMRMediaSink* aSink)
//
// -----------------------------------------------------------------------------
//
void CMccFileSourceImpl::AddDataSink( MCMRMediaSink* /*aSink*/ )
    {
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::AddDataSink" )
    }

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::OpenFileL(TFileName aFileName)
//
// Opens a 3gp file for streaming and reads media descriptions into
// member variables
// -----------------------------------------------------------------------------
//
void CMccFileSourceImpl::OpenFileL( TFileName aFileName, TBool aFileChangeMode )
    {
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::OpenFileL" )
    
    TCurrentState oldState = iCurrentState;
    
    // Close old file (if exists)
    CloseFileL( aFileChangeMode );
  
    aFileName.ZeroTerminate();

    TRAPD( err, OpenFileForStreamingL( aFileName ) );
    if ( err )
        {
        if ( aFileChangeMode )
            {
            // Have to stop timers etc, since old file was closed
            // and the new file is unusable
            DoCleanupL( EFalse );
            }
        User::Leave( err );
        }
		
	this->SetFileName( aFileName );
	
	if ( oldState == EPlaying )
	    {
	    // Continue playing with new file
	    iCurrentState = EPrimed;
	    SourcePlayL( ETrue, aFileChangeMode );
	    }
	else
	    {
	    // Keep old state
	    iCurrentState = oldState;
	    }

    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::OpenFile, exit" )
    }

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::CloseFileL()
//
// Closes the 3gp file
// -----------------------------------------------------------------------------
//
void CMccFileSourceImpl::CloseFileL( TBool aFileChangeMode )
    {
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::CloseFileL" )
    
    if ( iCurrentState == EPlaying )
        {
        SourceStopL( ETrue, aFileChangeMode );
        }
    
    if ( iMP4Handle ) 
	    {
	    if ( MP4ParseClose( iMP4Handle ) != MP4_OK )
	        {
	        User::Leave( KErrGeneral );
	        }
    	iMP4Handle = NULL;
    	iFileAudio->SetFileHandle( iMP4Handle );
	    iFileVideo->SetFileHandle( iMP4Handle );		
	    }
    
    iCurrentState = EConstructed;
    
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::CloseFileL, file closed" )
    }

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::FileNameL()
// -----------------------------------------------------------------------------
//   
TFileName& CMccFileSourceImpl::FileNameL()
    {
    return iFileName;
    }

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::SetPositionL(TTimeIntervalMicroSeconds aPosition)
// 
// Sets a new streaming position
// -----------------------------------------------------------------------------
//
void CMccFileSourceImpl::SetPositionL( TTimeIntervalMicroSeconds aPosition )
    {
    __FILESOURCE_CONTROLL_INT1( "CMccFileSourceImpl::SetPositionL (us):", 
                                aPosition.Int64() )
 	
 	if ( iCurrentState == EStopped ) 
    	{
    	return;
    	}

    TUint32 position = MicroToMilliSeconds( aPosition );
    
    if ( VideoInUse() ) 
	    {
	    iFileVideo->SetPositionL( position );
	    
	    // Position might not end up to what was requested as it is set to
	    // nearest IFrame.  With audio there's no such limitation and
	    // therefore it is good to use actual video position for audio.
	    position = iFileVideo->Position();
	    
	    __FILESOURCE_CONTROLL_INT1( "Pos modified to nearest IFrame (ms):", 
                                    position )
	    }
	    
	if ( AudioInUse() )
		{
		iFileAudio->SetPositionL( position );	
		}
	}

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::VideoTypeL()
// 
// Returns current video type
// -----------------------------------------------------------------------------
//
CCMRMediaBuffer::TBufferType CMccFileSourceImpl::VideoTypeL()
    {
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::VideoTypeL" )

    __ASSERT_ALWAYS( iCurrentState != EConstructed, User::Leave( KErrNotReady ) );
    
    return iFileVideo->Type();
    }

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::AudioTypeL()
// 
// Returns current audio type
// -----------------------------------------------------------------------------
//
CCMRMediaBuffer::TBufferType CMccFileSourceImpl::AudioTypeL()
    {
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::AudioTypeL" )

    __ASSERT_ALWAYS( iCurrentState != EConstructed, User::Leave( KErrNotReady ) );

    return iFileAudio->Type();
    }
    
// -----------------------------------------------------------------------------
// CMccFileSourceImpl::VideoFrameRateL()
// 
// Returns current video frame rate
// -----------------------------------------------------------------------------
//
TReal CMccFileSourceImpl::VideoFrameRateL()
    {
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::VideoFrameRateL" )

    __ASSERT_ALWAYS( iCurrentState != EConstructed, User::Leave( KErrNotReady ) );
        
    return iFileVideo->VideoFrameRateL();    
    }

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::AudioBitRateL()
// 
// Returns current audio bit rate
// -----------------------------------------------------------------------------
//
TUint32 CMccFileSourceImpl::AudioBitRateL()
    {
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::AudioBitRateL" )
    
    __ASSERT_ALWAYS( iCurrentState != EConstructed, User::Leave( KErrNotReady ) );

    TUint32 audioBitRate = iFileAudio->AudioBitRate();
    
    __FILESOURCE_CONTROLL_INT1( "CMccFileSourceImpl::AudioBitRateL", audioBitRate )
    
    return audioBitRate;
    }

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::DurationL()
// 
// Returns duration of current media clip
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CMccFileSourceImpl::DurationL()
    {
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::DurationL" )

    __ASSERT_ALWAYS( iCurrentState != EConstructed, User::Leave( KErrNotReady ) );

    // Audio and video might have different duration, take longer one
    //
    
    TTimeIntervalMicroSeconds duration = 0;
    
    if ( AudioInUse() )
	    {
	    duration = MilliToMicroSeconds( iFileAudio->Duration() );
	    }
	if ( VideoInUse() )
		{
		TTimeIntervalMicroSeconds videoDuration = 
		    MilliToMicroSeconds( iFileVideo->Duration() );
		duration = videoDuration > duration ? videoDuration : duration;
		}
		
    __FILESOURCE_CONTROLL_INT1("CMccFileSourceImpl::DurationL in micro=", 
    	duration.Int64())
    return duration;
    }

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::VideoFrameSizeL()
// 
// Returns current video frame size
// -----------------------------------------------------------------------------
//
TSize CMccFileSourceImpl::VideoFrameSizeL()
    {        
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::VideoFrameSizeL" )
       
    __ASSERT_ALWAYS( iCurrentState != EConstructed, User::Leave( KErrNotReady ) );
    
    TSize size = iFileVideo->VideoFrameSize();
    
    __FILESOURCE_CONTROLL_INT1( "CMccFileSourceImpl::VideoFrameSizeL iWidth", 
                                size.iWidth )
	__FILESOURCE_CONTROLL_INT1("CMccFileSourceImpl::VideoFrameSizeL iHeight",
								size.iHeight )   
								
    return size;
    }
    
// -----------------------------------------------------------------------------
// CMccFileSourceImpl::VideoBitRateL()
// 
// Returns current video bitrate
// -----------------------------------------------------------------------------
//
TUint32 CMccFileSourceImpl::VideoBitRateL()
    {    
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::VideoBitRateL" )

    __ASSERT_ALWAYS( iCurrentState != EConstructed, User::Leave( KErrNotReady ) );

    TUint32 audioBitRate = iFileAudio->AudioBitRate();
    
    // Average bitrate is for the whole stream, 
    // subtract audio average to get video average
    TUint32 videoBitRate = iFileVideo->StreamAverageBitRate() - audioBitRate;
    MccConversionUtility::BitRateSanitize( 
        videoBitRate, videoBitRate, iFileVideo->VideoType() );
    
    __FILESOURCE_CONTROLL_INT1( "CMccFileSourceImpl::VideoBitRateL, videoBitRate", 
        videoBitRate )

    return videoBitRate;
    }

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::PositionL()
// 
// Returns current streaming position. If other stream is already at end
// (zero pos) other ones position is used. Otherwise always less progressed
// streams position is returned.
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CMccFileSourceImpl::PositionL()
    {
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::PositionL" )

    __ASSERT_ALWAYS( iCurrentState != EConstructed, User::Leave( KErrNotReady ) );

    TTimeIntervalMicroSeconds audioPosition = 0;
    TTimeIntervalMicroSeconds videoPosition = 0;
 
    if ( AudioInUse() )
	    {
	    audioPosition = MilliToMicroSeconds( iFileAudio->Position() );
	    }
	if ( VideoInUse() )
		{
		videoPosition = MilliToMicroSeconds( iFileVideo->Position() );
		}
		
    __FILESOURCE_CONTROLL_INT2( "CMccFileSourceImpl::PositionL in micro, audio:", 
                                audioPosition.Int64(), 
                                " video:", videoPosition.Int64() )

    if ( videoPosition.Int64() == 0 || audioPosition.Int64() == 0 )
        {
        return Max( videoPosition, audioPosition );
        }
    return Min( videoPosition, audioPosition );
    }

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::AudioFourCCL()
// -----------------------------------------------------------------------------
//    
TFourCC CMccFileSourceImpl::AudioFourCCL()
    {
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::AudioFourCCL" )

    __ASSERT_ALWAYS( iCurrentState != EConstructed, User::Leave( KErrNotReady ) );
    
    return iFileAudio->GetFourCC();
    }

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::VideoFourCCL()
// -----------------------------------------------------------------------------
//	    
TFourCC CMccFileSourceImpl::VideoFourCCL()
    {
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::VideoFourCCL" )

    __ASSERT_ALWAYS( iCurrentState != EConstructed, User::Leave( KErrNotReady ) );
    
    return iFileVideo->GetFourCC();
    }

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::GetConfigKeyL()
// -----------------------------------------------------------------------------
//
void CMccFileSourceImpl::GetConfigKeyL( TDes8& aConfigKey )
    {
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::GetConfigKeyL" )
    
    __ASSERT_ALWAYS( iMP4Handle && VideoInUse(), User::Leave( KErrNotReady ) );
    
    HBufC8* configKey = iFileVideo->GetConfigKeyL();
    CleanupStack::PushL( configKey );
    __ASSERT_ALWAYS( configKey, User::Leave( KErrNotFound ) );
    __ASSERT_ALWAYS( configKey->Length() <= aConfigKey.MaxLength(), 
                     User::Leave( KErrOverflow ) );
    aConfigKey.Copy( *configKey );
    CleanupStack::PopAndDestroy( configKey );
    
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::GetConfigKeyL, exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccFileSourceImpl::PlayL()
// 
// Starts streaming from 3gp file
// -----------------------------------------------------------------------------
//
void CMccFileSourceImpl::SourcePlayL( TBool aIgnoreEvents, TBool aFileChangeMode )
    {
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::SourcePlayL" )
    
    __ASSERT_ALWAYS( iCurrentState != EConstructed, User::Leave( KErrNotReady ) );

    if ( iCurrentState == EPlaying )
        {
        return;
        }
    
    // Start decision for both needs to be done before starting either one
    TBool videoStartNeeded = StartNeededL( *iFileVideo );
    TBool audioStartNeeded = StartNeededL( *iFileAudio );
    
	if ( videoStartNeeded && !aFileChangeMode  )
    	{
    	iFileVideo->StartTimerL();
    	}
    	
	if ( audioStartNeeded && !aFileChangeMode )
	    {
    	iFileAudio->StartTimerL();	  
	    }
	
	if ( !aIgnoreEvents )
	    {   
    	if ( iCurrentState == EPaused )
    	    {
    	    SendStreamEventToClient( KMccEventCategoryStream, KMccStreamResumed );
    	    }
    	else
    	    {
    	    SendStreamEventToClient( KMccEventCategoryStream, KMccStreamStarted );
    	    }
	    }
	    
    iCurrentState = EPlaying;
    
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::SourcePlayL, exit" )
    }
        
// -----------------------------------------------------------------------------
// CMccFileSourceImpl::SourcePauseL()
// 
// Pauses streaming by cancelling timers
// -----------------------------------------------------------------------------
//
void CMccFileSourceImpl::SourcePauseL()
    {
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::SourcePauseL" )
    
    if ( iCurrentState != EPlaying && iCurrentState != EWaitingPause ) 
    	{
    	return;
    	}
    	
 	if ( VideoInUse() )
    	{
    	iFileVideo->StopTimer();	
    	iFileVideo->Cleanup();
    	}
	if ( AudioInUse() )
	    {
	    iFileAudio->StopTimer();	
    	iFileAudio->Cleanup();	    
	    }
	    
    StopEofTimer();	    
	
	// If pause was caused by eof, paused event is passed to all filesource users    
	TUint32 eventNumData = 
	    ( iCurrentState == EWaitingPause ) ? KMccAutomaticEvent : 0;
	    
	SendStreamEventToClient( KMccEventCategoryStream, 
	                         KMccStreamPaused, 
	                         KErrNone, 
	                         eventNumData );
	
    iCurrentState = EPaused;
    
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::SourcePauseL, exit" )
	}
	
// -----------------------------------------------------------------------------
// CMccFileSourceImpl::PauseAudioL()
// 
// pause audio only
// -----------------------------------------------------------------------------
//
void CMccFileSourceImpl::PauseAudioL( TInt aError )
	{
	__FILESOURCE_CONTROLL( "CMccFileSourceImpl::PauseAudio" )
    
    if ( iCurrentState != EPlaying ) 
    	{
    	return;
    	}
    	
    if ( AudioInUse() )
	    {
	    iFileAudio->StopTimer();	
    	iFileAudio->Cleanup();	    
	    }
	    
	if ( aError )
        {
        SendStreamEventToClient( KMccEventCategoryStream, 
                                 KMccStreamPaused, 
                                 aError, 
                                 KMccAutomaticEvent );
        }
	else if ( !VideoInUse() || iFileVideo->IsPaused() )
		{
		// Initiate pause sequence only if all used media types are paused
		
        StartEofTimerL();
		}
    else
        {
        // NOP
        }
		
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::PauseAudio, exit" )
	}
	
// -----------------------------------------------------------------------------
// CMccFileSourceImpl::PauseVideoL()
// 
// pause video only
// -----------------------------------------------------------------------------
//
void CMccFileSourceImpl::PauseVideoL( TInt aError )
	{
	__FILESOURCE_CONTROLL( "CMccFileSourceImpl::PauseVideo" )
    
    if ( iCurrentState != EPlaying ) 
    	{
    	return;
    	}
    	
 	if ( VideoInUse() )
    	{
    	iFileVideo->StopTimer();	
    	iFileVideo->Cleanup();
    	}	
    
    if ( aError )
        {
        SendStreamEventToClient( KMccEventCategoryStream, 
                                 KMccStreamPaused, 
                                 aError,
                                 KMccAutomaticEvent );
        }
    else if ( !AudioInUse() || iFileAudio->IsPaused() )
		{ 
		// Initiate pause sequence only if all used media types are paused
		
        StartEofTimerL();
		}
    else
        {
        // NOP
        }
		
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::PauseVideo, exit" )
	}

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::StopL()
// 
// Stops streaming
// -----------------------------------------------------------------------------
//
void CMccFileSourceImpl::SourceStopL( TBool aIgnoreEvents, TBool aFileChangeMode )
    {
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::SourceStopL" )
  
    if ( iCurrentState == EConstructed )
        {
        return;
        }

    DoCleanupL( aFileChangeMode );
	
	if ( !aIgnoreEvents )
	    {
    	SendStreamEventToClient( KMccEventCategoryStream, KMccStreamStopped );
	    }
		
    iCurrentState = EStopped;
    
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::SourceStopL, exit" )
    }


// -----------------------------------------------------------------------------
// CMccFileSourceImpl::SourceDataTypeCode()
// -----------------------------------------------------------------------------
//
TFourCC CMccFileSourceImpl::SourceDataTypeCode( TMediaId aMediaId )
	{
	__FILESOURCE_CONTROLL( "CMccFileSourceImpl::SourceDataTypeCode" )

    if ( KUidMediaTypeVideo == aMediaId.iMediaType &&
         iFileVideo )
        {
        iIsVideo = ETrue;
        return iFileVideo->GetFourCC(); 
        }
    else if ( KUidMediaTypeAudio == aMediaId.iMediaType &&
        iFileAudio )
        {
        iIsAudio = ETrue;
        return iFileAudio->GetFourCC();
        }
    else
        {
        return TFourCC( NULL );
        }
	}
	
// -----------------------------------------------------------------------------
// CMccFileSourceImpl::SetSourceDataTypeCode()
// -----------------------------------------------------------------------------
//
TInt CMccFileSourceImpl::SetSourceDataTypeCode( 
	TFourCC aCodec, 
    TMediaId aMediaId )
	{
	__FILESOURCE_CONTROLL( "CMccFileSourceImpl::SetSourceDataTypeCode" )
	
    if ( KUidMediaTypeVideo == aMediaId.iMediaType
    	&& VideoInUse()
        && aCodec == iFileVideo->GetFourCC() )
        {   
        return KErrNone;
        }
    else if ( KUidMediaTypeAudio == aMediaId.iMediaType 
    	&& AudioInUse()
        && aCodec == iFileAudio->GetFourCC() )
        {  
        return KErrNone;
        }
    else
	    {
	    return KErrNotSupported;
	    } 
	}	

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::SourcePrimeL()
// -----------------------------------------------------------------------------
//
void CMccFileSourceImpl::SourcePrimeL()
	{
	__FILESOURCE_CONTROLL( "CMccFileSourceImpl::SourcePrimeL" )
   
   	iCurrentState = EPrimed;	
	SendStreamEventToClient( KMccEventCategoryStream, KMccStreamPrepared );
	
	__FILESOURCE_CONTROLL( "CMccFileSourceImpl::SourcePrimeL, exit" )
	}
	    
// -----------------------------------------------------------------------------
// CMccFileSourceImpl::FillBufferL()
// -----------------------------------------------------------------------------
//
void CMccFileSourceImpl::FillBufferL( CMMFBuffer* aBuffer,
						              MDataSink*  aConsumer,
						              TMediaId    aMediaId )
	{
	__FILESOURCE_MEDIA( "CMccFileSourceImpl::FillBufferL" )

	if ( aMediaId.iMediaType == KUidMediaTypeAudio &&
	     AudioInUse() )
		{
    	__FILESOURCE_MEDIA( "CMccFileSourceImpl::FillBufferL, audio" )
    
    	iFileAudio->FillBufferL( aBuffer, aConsumer );
		}
	else if (aMediaId.iMediaType == KUidMediaTypeVideo &&
	     VideoInUse() )
		{
    	__FILESOURCE_MEDIA( "CMccFileSourceImpl::FillBufferL, video" )    
       
		iFileVideo->FillBufferL( aBuffer, aConsumer );
		}
	else
		{
    	__FILESOURCE_MEDIA( "CMccFileSourceImpl::FillBufferL, unknown media " ) 
		User::Leave( KErrNotSupported );	
		}
	}

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::SourceThreadLogon
// -----------------------------------------------------------------------------
//
TInt CMccFileSourceImpl::SourceThreadLogon( MAsyncEventHandler& aEventHandler )
	{
	__FILESOURCE_CONTROLL( "CMccFileSourceImpl::SourceThreadLogon" )

	iAsyncEventHandler = static_cast<MAsyncEventHandler*>( &aEventHandler );
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::SourceThreadLogoff
// -----------------------------------------------------------------------------
//		
void CMccFileSourceImpl::SourceThreadLogoff()
	{
	__FILESOURCE_CONTROLL( "CMccFileSourceImpl::SourceThreadLogoff" )

	iAsyncEventHandler = NULL;
	}
		
// -----------------------------------------------------------------------------
// CMccFileSourceImpl::FileOpenErrorL
// -----------------------------------------------------------------------------
//	
void CMccFileSourceImpl::FileOpenErrorL( MP4Err error )
	{
	iMP4Handle = NULL;
	if ( error == MP4_OUT_OF_MEMORY )
		{
		User::Leave( KErrNoMemory );	
		}
	else
		{
		User::Leave( KErrGeneral );	
		}
	}
	
// -----------------------------------------------------------------------------
// CMccFileSourceImpl::MilliToMicroSeconds
// -----------------------------------------------------------------------------
//	  	
TTimeIntervalMicroSeconds CMccFileSourceImpl::MilliToMicroSeconds( 
    TUint32 aMilliSeconds )
	{
	return TTimeIntervalMicroSeconds( (TInt64) aMilliSeconds * KMccMicroToMilliConst );
	}
	
// -----------------------------------------------------------------------------
// CMccFileSourceImpl::MicroToMilliSeconds
// -----------------------------------------------------------------------------
//	  		
TUint32 CMccFileSourceImpl::MicroToMilliSeconds( 
    TTimeIntervalMicroSeconds aMicroSeconds )
	{
	return ( aMicroSeconds.Int64() / KMccMicroToMilliConst );
	}

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::SetFileName()
// -----------------------------------------------------------------------------
//
void CMccFileSourceImpl::SetFileName( const TFileName& aFileName )
	{
	__FILESOURCE_CONTROLL( "CMccFileSourceImpl::SetFileName" )

	iFileName.Copy( aFileName );
	}

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::OpenFileForStreamingL()
// -----------------------------------------------------------------------------
//	
void CMccFileSourceImpl::OpenFileForStreamingL( TFileName aFileName )
	{
	__FILESOURCE_CONTROLL( "CMccFileSourceImpl::OpenFileForStreamingL" )

    TFileName fullFileName;
    MP4Err error = MP4_OK;
    
    RFs fS;
    User::LeaveIfError( fS.Connect() );
	CleanupClosePushL( fS );
    TParse fp;
    TInt err = fS.Parse( aFileName, fp );
    
    if ( err )
        {
        __FILESOURCE_CONTROLL_INT1( 
            "CMccFileSourceImpl::OpenFileForStreamingL file name parse error:", 
            err )
	    User::Leave( err );
        }
	
    fullFileName = fp.FullName();
  	CleanupStack::PopAndDestroy( &fS ); 
	
	this->SetFileName( aFileName );
	
	const TInt KMccZeroTerminatorLen = 1;
	__ASSERT_ALWAYS( 
	    fullFileName.Length() + KMccZeroTerminatorLen <= fullFileName.MaxLength(),
	    User::Leave( KErrOverflow ) );
	
  	error = MP4ParseOpen( &iMP4Handle, (wchar_t *) fullFileName.PtrZ() );

    if ( error != MP4_OK )
	    {
        __FILESOURCE_CONTROLL_INT1( 
            "CMccFileSourceImpl::OpenFileForStreamingL parse open error:", 
            error )

	    iMP4Handle = NULL;
	   	User::Leave( KErrGeneral );
	    }
	
    TBool containAudio = iFileAudio->ParseUpdateAudioDescriptions( iMP4Handle );
    TBool containVideo = iFileVideo->ParseUpdateVideoDescriptions( iMP4Handle );
			
	if ( !containAudio && !containVideo )
	    {
	    __FILESOURCE_CONTROLL( 
	        "CMccFileSourceImpl::OpenFileForStreamingL no audio/video" )
	    
	    MP4ParseClose( iMP4Handle );
	    iMP4Handle = NULL;
	   	User::Leave( KErrNotSupported );
	    }
	    
	iFileAudio->SetFileHandle( iMP4Handle );
	iFileVideo->SetFileHandle( iMP4Handle );	

	iFileAudio->ParseUpdateStreamDescription( iMP4Handle );
	iFileVideo->ParseUpdateStreamDescription( iMP4Handle );
	
	// If filecontainers are reused (i.e. filename is changed), 
	// position has to be zeroed
	SetPositionL( 0 );
	
	__FILESOURCE_CONTROLL( "CMccFileSourceImpl::OpenFileForStreamingL, exit" )
	}

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::SendStreamEventToClient()
// -----------------------------------------------------------------------------
//	
void CMccFileSourceImpl::SendStreamEventToClient( 
    TMccEventCategory aEventCategory,
    TMccEventType aEventType,
    TInt aError,
    TUint32 aEventNumData )
    {
    __FILESOURCE_MEDIA( "CMccFileSourceImpl::SendStreamEventToClient" )
    
    if ( iAsyncEventHandler )
	    {
	    ClearEvent();
	    
	    iMccEvent.iEndpointId = iEndpointId;
	    iMccEvent.iEventCategory = aEventCategory;
	    iMccEvent.iEventType = aEventType;
	    iMccEvent.iErrorCode = aError;
	    iMccEvent.iEventNumData = aEventNumData;

		TMccInternalEvent internalEvent( KMccFileSourceUid, 
		                                 EMccInternalEventNone,
		                                 iMccEvent );
		                         
		iAsyncEventHandler->SendEventToClient( internalEvent );
	    }
	else
		{
		__FILESOURCE_MEDIA( "CMccFileSourceImpl::SendStreamEventToClient, \
		iAsyncEventHandler=NULL" )
		}
    }	

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::AudioInUse()
// -----------------------------------------------------------------------------
//
TBool CMccFileSourceImpl::AudioInUse() const
    {
    return ( iFileAudio && iIsAudio );
    }
    
// -----------------------------------------------------------------------------
// CMccFileSourceImpl::VideoInUse()
// -----------------------------------------------------------------------------
//
TBool CMccFileSourceImpl::VideoInUse() const
    {
    return ( iFileVideo && iIsVideo );
    }

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::DoCleanupL()
// -----------------------------------------------------------------------------
//   
void CMccFileSourceImpl::DoCleanupL( TBool aFileChangeMode )
    {
    if ( VideoInUse() && !aFileChangeMode )
    	{
    	iFileVideo->StopTimer();
    	iFileVideo->ResetTimeL();
    	// clean the iConsumerBuffer and iConsumer,
    	// to prevent still send data after Stoped	
    	iFileVideo->Cleanup();
    	}
	if ( AudioInUse() && !aFileChangeMode )
        {
	    iFileAudio->StopTimer();
	    iFileAudio->ResetTimeL();	
    	iFileAudio->Cleanup();	    
	    }
    }

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::StartEofTimerL()
// -----------------------------------------------------------------------------
//
void CMccFileSourceImpl::StartEofTimerL()
    {
    StopEofTimer();
    
    TInt delayAfterStop( KMccMicroToMilliConst * DelayAfterStop() );
    
    __FILESOURCE_CONTROLL_INT1( "CMccFileSourceImpl::StartEofTimerL, microsecs:",
                                delayAfterStop )
    
    iEofTimer = CMccEofTimer::NewL( this );
    
    iEofTimer->After( delayAfterStop );
    }

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::EofTimerExpiredL()
// -----------------------------------------------------------------------------
//
void CMccFileSourceImpl::EofTimerExpiredL()
    {
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::EofTimerExpiredL" );

    iCurrentState = EWaitingPause;
		
    SendStreamEventToClient( KMccEventCategoryStreamControl, KMccStreamPaused );
    }

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::StopEofTimer()
// -----------------------------------------------------------------------------
//
void CMccFileSourceImpl::StopEofTimer()
	{
    if ( iEofTimer )
        {
        __FILESOURCE_CONTROLL( 
            "CMccFileSourceImpl::StopEofTimer, cancel iEofTimer" )
        iEofTimer->Cancel();
        delete iEofTimer;
        iEofTimer = NULL;
        }
	}
	        
// -----------------------------------------------------------------------------
// CMccFileSourceImpl::SetDelayAfterStop()
// -----------------------------------------------------------------------------
//
void CMccFileSourceImpl::SetDelayAfterStop( TInt aDelay )
    {
    __FILESOURCE_CONTROLL_INT1( "CMccFileSourceImpl::SetDelayAfterStop aDelay:", 
                                aDelay )
    iDelayAfterStop = aDelay;    
    }
    
// -----------------------------------------------------------------------------
// CMccFileSourceImpl::DelayAfterStop()
// -----------------------------------------------------------------------------
//
TInt CMccFileSourceImpl::DelayAfterStop( ) const
    {
    return iDelayAfterStop;
    }

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::ClearEvent()
// -----------------------------------------------------------------------------
//
void CMccFileSourceImpl::ClearEvent()
    {
    iMccEvent = TMccEvent();
    }

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::StartNeededL()
// Checks current file position and determines based on that whether starting
// of this media type is needed. If this media type is already at end (at zero
// pos) but other is not yet, this media type does not need starting.
// -----------------------------------------------------------------------------
//
TBool CMccFileSourceImpl::StartNeededL( CMccFileSourceTypeBase& aFileSource )
    {
    TBool startNeeded( ETrue );
    TTimeIntervalMicroSeconds currentPosition = PositionL();
    
    if ( aFileSource.MediaType().iMediaType == KUidMediaTypeAudio )
        {
        startNeeded = AudioInUse();
        }
    if ( aFileSource.MediaType().iMediaType == KUidMediaTypeVideo )
        {
        startNeeded = VideoInUse();
        }
        
    if ( startNeeded && 
         currentPosition.Int64() > 0 && 
         aFileSource.Position() == 0 )
        {
        startNeeded = EFalse;
        }
    
    return startNeeded;
    }

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::InvalidVideoFrame()
// -----------------------------------------------------------------------------
//
void CMccFileSourceImpl::InvalidVideoFrame( TBool aInvalid )
	{
	__FILESOURCE_CONTROLL( 
	            "CMccFileSourceImpl::InvalidVideoFrame" )
    if( aInvalid )
	    {
	    SendStreamEventToClient( KMccEventCategoryStream, 
		    	                 KMccStreamIdle );
		}
	else
	    {
	    SendStreamEventToClient( KMccEventCategoryStream, 
		    	                 KMccStreamPlaying );
	    }
	}

    
#ifndef EKA2
// DLL interface code
EXPORT_C TInt E32Dll( TDllReason )
    {
    return KErrNone;
    }
#endif
