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
#include <ES_SOCK.H>
#include <e32std.h>
#include "MccFileSourceImpl.h"
#include "MmccCodecInformation.h"
#include "MccFileSourceLogs.h"
#include "MccTickVideoObserver.h"
#include "MccTickAudioObserver.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::NewL
// Static constructor.
// -----------------------------------------------------------------------------
//
CMccFileSourceImpl* CMccFileSourceImpl::NewL()
    {
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::NewL" )
    
    CMccFileSourceImpl* self = new ( ELeave ) CMccFileSourceImpl();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::ConstructL
// -----------------------------------------------------------------------------
//
void CMccFileSourceImpl::ConstructL()
    {
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::ConstructL" )
    }
    
// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
//
CMccFileSourceImpl::CMccFileSourceImpl()
    {
    iObserver = 0;
    iVideoLength = 0;
    iFrameRate = 0;
    iVideoType = 0;
    iVideoWidth = 0;
    iVideoHeight = 0;
    iVideoTimeScale = 0;
    iAudioLength = 0;
    iAudioType = 0;
    iFramesPerSample = 0;
    iAudioTimeScale = 0;
    iAverageBitRate = 0;
    iStreamSize = 0;
    iStreamAverageBitRate = 0;
    iAudioPosition = 0;
    iVideoPosition = 0;
    iPosition = 0;
    iCurrentVideoType = CCMRMediaBuffer::EVideoH263;
    iCurrentAudioType = CCMRMediaBuffer::EAudioAMRNB;
    iBufferSize = 0;
    iAudioSize = 0;
    iReturnedFrames = 0;
    iNextFrameType = 0;
    iKeyFrame = 0;
    iVideoFrameSize = 0;
    iLastPosition = 0;
    iFrameSize.iWidth = 0;
    iFrameSize.iHeight = 0;
    iCurrentState = EConstructed;
    iMP4Handle = NULL;
    iIsAudioLeft = EFalse;
    iIsVideoLeft = EFalse;
    iIsAudio = EFalse;
    iIsVideo = EFalse;
	iRecursiveCalls = 0;
	iIsFirstVideoFrame = EFalse;

	iAudioConsumer = 0;
	iAudioConsumerBuffer = 0;
	iVideoConsumer = 0;
	iVideoConsumerBuffer = 0;    
    }
        
// -----------------------------------------------------------------------------
// CMccFileSourceImpl::~CMccFileSourceImpl.
// -----------------------------------------------------------------------------
//
CMccFileSourceImpl::~CMccFileSourceImpl()
    {
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::~CMccFileSourceImpl" )
  /*
    delete iPeriodicVideo;
    delete iPeriodicVideo;
   */
    if ( iMP4Handle )
	    {
	    MP4Err error = MP4ParseClose( iMP4Handle );
	    }
  
    iObserver = NULL;
    iVideoConsumer = NULL;
    iAudioConsumer = NULL;
    
    iAudioFrames.Reset();
    iAudioFrames.Close();
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
void CMccFileSourceImpl::OpenFileL( TFileName aFileName )
    {
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::OpenFileL" )
    
    __ASSERT_ALWAYS( iCurrentState == EConstructed, User::Leave( KErrNotReady ) );

    TFileName fullFileName;
    MP4Err error = MP4_OK;
    
    RFs fS;
    User::LeaveIfError( fS.Connect() );
	CleanupClosePushL( fS );
    TParse fp;	
    if ( fS.Parse( aFileName, fp ) != KErrNone )
        {
        RDebug::Print(_L("CMccFileSourceImpl::OpenFileL file name parse error!"));
	    User::Leave( KErrGeneral );
        }
	
    fullFileName = fp.FullName();

  	CleanupStack::PopAndDestroy( &fS ); 
  	
  	error = MP4ParseOpen( &iMP4Handle, (wchar_t *) fullFileName.PtrZ() );

    if ( error != MP4_OK )
	    {
	    RDebug::Print(_L("CMccFileSourceImpl::OpenFileL parse open error!"));
        RDebug::Print(_L("CMccFileSourceImpl::OpenFileL parse open error=%d"), error);

	    FileOpenErrorL( error );	
	    }
	
 	ParseUpdateVideoDescriptions();
 	
 	ParseUpdateAudioDescriptions();

    if ( ( !iIsAudio ) && ( !iIsVideo ) )
        {
        User::Leave( KErrNotSupported );
        }
    
    error = MP4ParseRequestStreamDescription( iMP4Handle,
        ( mp4_u32* ) &iStreamSize, ( mp4_u32* ) &iStreamAverageBitRate );
       
    __FILESOURCE_CONTROLL_INT1("CMccFileSourceImpl::OpenFileL, \
MP4ParseRequestStreamDescription error=",error)
    
  
	#ifdef TIMESTAMP
		iReplayTimeStamp = 0;
		iTimeDifference = 0;
		iPausedTime = 0;
	#endif

    iCurrentState = EReady;
    }

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::CloseFileL()
//
// Closes the 3gp file
// -----------------------------------------------------------------------------
//
void CMccFileSourceImpl::CloseFileL()
    {
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::CloseFileL" )

    __ASSERT_ALWAYS( iCurrentState != EConstructed, User::Leave( KErrNotReady ) );

    if ( iCurrentState == EPlaying )
        {
        TRAP_IGNORE( SourceStopL() );
        }

    MP4Err error;
    error = MP4ParseClose( iMP4Handle );

    if ( error != MP4_OK )
        {
        User::Leave( KErrGeneral );
        }

    iCurrentState = EConstructed;
    iMP4Handle = NULL;
    }

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::SetPositionL(TTimeIntervalMicroSeconds aPosition)
// 
// Sets a new streaming position
// -----------------------------------------------------------------------------
//
void CMccFileSourceImpl::SetPositionL( TTimeIntervalMicroSeconds aPosition )
    {
    __FILESOURCE_CONTROLL_INT1( "CMccFileSourceImpl::SetPositionL", Int64() )
    
    __ASSERT_ALWAYS( iCurrentState != EConstructed, User::Leave( KErrNotReady ) );

    if ( iCurrentState == EStopped ) 
    	{
    	return;
    	}

    MP4Err error;

    TInt64 tempPosition = aPosition.Int64();
    TUint tempPosition2 = tempPosition; 
    tempPosition2 = tempPosition2 / 1000; 
    iLastPosition = iVideoLength;

    TRAP_IGNORE( UpdatePositionL());
       
    if ( iLastPosition < iAudioLength )
        {
        iLastPosition = iAudioLength;
        }

    if ( tempPosition2 > iLastPosition )
        {
        User::Leave( KErrArgument );
        }

    error = MP4ParseSeek( iMP4Handle, ( mp4_u32 ) tempPosition2,
        ( mp4_u32* ) &iAudioPosition, ( mp4_u32* ) &iVideoPosition, MP4TRUE );
	
    if ( error != MP4_OK )
        {   
        User::Leave( KErrGeneral );
        }

    #ifdef TIMESTAMP
		if ( iIsAudioLeft == EFalse && iIsVideoLeft == EFalse)
			{
			iPosition = 0;
			}
        iTimeDifference = iTimeDifference + iPosition - iVideoPosition;
        iPosition = iVideoPosition;
    #endif
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

    return iCurrentVideoType;
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
    
    return iCurrentAudioType;
    }
    
// -----------------------------------------------------------------------------
// CMccFileSourceImpl::VideoFrameRateL()
// 
// Returns current video frame rate
// -----------------------------------------------------------------------------
//
TReal32 CMccFileSourceImpl::VideoFrameRateL()
    {
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::VideoFrameRateL" )

    __ASSERT_ALWAYS( iCurrentState != EConstructed, User::Leave( KErrNotReady ) );

    return TReal32 ( iFrameRate );
    }

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::AudioBitRateL()
// 
// Returns current audio bit rate
// -----------------------------------------------------------------------------
//
TUint CMccFileSourceImpl::AudioBitRateL()
    {
    __FILESOURCE_CONTROLL_INT1( "CMccFileSourceImpl::AudioBitRateL", iAverageBitRate )

    __ASSERT_ALWAYS( iCurrentState != EConstructed, User::Leave( KErrNotReady ) );
    
    return iAverageBitRate;
    }

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::DurationL()
// 
// Returns duration of current media clip
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CMccFileSourceImpl::DurationL()
    {
    __FILESOURCE_CONTROLL_INT1( "CMccFileSourceImpl::DurationL", 
                                iVideoLength * 1000 )

    __ASSERT_ALWAYS( iCurrentState != EConstructed, User::Leave( KErrNotReady ) );
    
    return TTimeIntervalMicroSeconds( iVideoLength * 1000 );
    }

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::VideoFrameSizeL()
// 
// Returns current video frame size
// -----------------------------------------------------------------------------
//
TSize CMccFileSourceImpl::VideoFrameSizeL()
    {        
    __FILESOURCE_CONTROLL_INT1( "CMccFileSourceImpl::VideoFrameSizeL iWidth", 
                                iFrameSize.iWidth )
	__FILESOURCE_CONTROLL_INT1("CMccFileSourceImpl::VideoFrameSizeL iHeight",
								iFrameSize.iHeight )                            
    
    __ASSERT_ALWAYS( iCurrentState != EConstructed, User::Leave( KErrNotReady ) );
    
    return iFrameSize;
    }
// -----------------------------------------------------------------------------
// CMccFileSourceImpl::VideoBitRateL()
// 
// Returns current video bitrate
// -----------------------------------------------------------------------------
//
TUint CMccFileSourceImpl::VideoBitRateL()
    {
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::VideoBitRateL, videoBitRate" )
    
    __ASSERT_ALWAYS( iCurrentState != EConstructed, User::Leave( KErrNotReady ) );
    
    TUint videoBitRate = iStreamAverageBitRate - iAverageBitRate;
   
    __FILESOURCE_CONTROLL_INT1( "CMccFileSourceImpl::VideoBitRateL, videoBitRate", 
                                videoBitRate )
    return videoBitRate;
    }

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::PositionL()
// 
// Returns current streaming position
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CMccFileSourceImpl::PositionL()
    {
    __FILESOURCE_CONTROLL_INT1( "CMccFileSourceImpl::PositionL", 
                                iPosition * 1000 )

    __ASSERT_ALWAYS( iCurrentState != EConstructed, User::Leave( KErrNotReady ) );

    return TTimeIntervalMicroSeconds( iPosition * 1000 ); 

    }
// -----------------------------------------------------------------------------
// CMccFileSourceImpl::UpdatePositionL()
// 
// Updates the position parameter
// -----------------------------------------------------------------------------
//
void CMccFileSourceImpl::UpdatePositionL()
	{
	__FILESOURCE_CONTROLL( "CMccFileSourceImpl::UpdatePosition" )
	MP4Err error;
	
    error=MP4ParseGetNextVideoTimestamp( iMP4Handle,
        ( mp4_u32* ) &iPosition, NULL );

    if ( error != MP4_OK )
        {
        User::Leave( KErrGeneral );
        }

    error = MP4ParseSeek( iMP4Handle, ( mp4_u32 ) iPosition,
        ( mp4_u32* ) &iAudioPosition, ( mp4_u32* ) &iVideoPosition, MP4FALSE );
	
     if ( error != MP4_OK )
        {
        User::Leave( KErrGeneral );
        }

	}

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::ReadAudioFrameL()
// 
// Reads the next audio frame from 3gp file
// -----------------------------------------------------------------------------
//
void CMccFileSourceImpl::ReadAudioFrameL()
    {
	__FILESOURCE_CONTROLL( "CMccFileSourceImpl::ReadAudioFrameL" )
	
     
    TBool brokenFrame = EFalse;
    
  	if ( iAudioConsumer == NULL)
  		{
  		__FILESOURCE_CONTROLL( "CMccFileSourceImpl::ReadAudioFrameL, \
source not available" )
  		return;
  		}
  	else
  		{
  		__FILESOURCE_CONTROLL( "CMccFileSourceImpl::ReadAudioFrameL, \
writing buffer" )  		
  		}	

	if (!iAudioFrames.Count())
		{		
	    MP4Err error;

	    error = MP4ParseIsFrameAvailable( iMP4Handle, MP4_TYPE_AMR_NB );
	    
	    if ( error != MP4_OK )
	        {
	  		__FILESOURCE_CONTROLL( "CMccFileSourceImpl::ReadAudioFrameL, \
no audio" ) 
			iIsAudioLeft = EFalse;
	        }
	    else 
	    	{
	  		__FILESOURCE_CONTROLL( "CMccFileSourceImpl::ReadAudioFrameL, \
	audio OK" )
	    	iIsAudioLeft = ETrue;
	    	}
	    	
	    if ( iIsAudioLeft == EFalse && iIsVideoLeft == EFalse )
	        {
	  		__FILESOURCE_CONTROLL( "CMccFileSourceImpl::ReadAudioFrameL, \
no video & audio available" ) 		
	        		
			#ifdef TIMESTAMP
				iReplayTimeStamp = iReplayTimeStamp + iPosition + 10;
			#endif

			SetPositionL(  TTimeIntervalMicroSeconds ( 0 ) );
			SourcePauseL();

	        if ( iObserver )
	            {
	            iObserver->EndOfFile( KErrNone );
	            }
	        return;
		    }

		// Timing correction code
		TTimeIntervalMicroSeconds elapsedTime;
	    TTime currentTime;
	    currentTime.HomeTime();
	    elapsedTime = currentTime.MicroSecondsFrom( iStartTime );
	    TInt64 tempTime = elapsedTime.Int64();
	    TInt timeNow = tempTime  / 1000;

		if ( ( timeNow + 500 ) < ( ( TInt ) iAudioPosition + iTimeDifference + iReplayTimeStamp + ( iPausedTime / 1000 ) ) )
			{
	  		__FILESOURCE_CONTROLL( "CMccFileSourceImpl::ReadAudioFrameL, \
Position check, returning" ) 
	  		__FILESOURCE_CONTROLL( "CMccFileSourceImpl::ReadAudioFrameL, IGNORING" ) 
	  		//return    				
			}

		MP4ParseNextFrameSize( iMP4Handle, MP4_TYPE_AMR_NB,
	        ( mp4_u32* ) &iBufferSize );

	    HBufC8* iMediaBuffer = HBufC8::NewL( iBufferSize );
	    TPtr8 iMediaSinkBuffer = iMediaBuffer->Des();

	    iMediaSinkBuffer.SetLength( iBufferSize );

	    if ( MP4ParseReadAudioFrames( iMP4Handle,
	        ( mp4_u8* ) iMediaSinkBuffer.Ptr(),
	        ( mp4_u32 ) iBufferSize, ( mp4_u32* ) &iAudioSize,
	        ( mp4_u32* ) &iPosition, ( mp4_u32* ) &iReturnedFrames,
			NULL ) == MP4_OK )
			{
			__FILESOURCE_CONTROLL_INT1( "CMccFileSourceImpl::ReadAudioFrameL, \
read, time position", iPosition ) 		

			iAudioPosition = iPosition ;
			brokenFrame = EFalse;
			
			TBool areFramesNormal = EFalse;
		
			// If brokenFrame = ETrue don't send the frame forward!
					
			// Calculate real frame size from sample size
	        TInt FrameSize = iBufferSize / iReturnedFrames;
	        TUint Counter = 0;
	        TInt AddToTimeStamp = 0;
	        TInt startPosition = 0;
	        
	        while (Counter < iReturnedFrames)
	            {
	            Counter++;

				if ( areFramesNormal == EFalse )
					{
					FrameSize = GetFrameLength( iMediaBuffer->Mid( startPosition ) ); 
					if ( FrameSize == 0 )
						{
						brokenFrame = ETrue;
						}
					else 
						{
						brokenFrame = EFalse;
						}
					}

				
				TTimeIntervalMicroSeconds timeStamp(0);
					
	            // Sets a different timestamp than in file (forward/rewind functionality)
	            #ifdef TIMESTAMP
				timeStamp = TTimeIntervalMicroSeconds( ( (iPosition + iTimeDifference + iReplayTimeStamp ) * 1000 ) 
							+ AddToTimeStamp 
				#ifdef TIMESTAMPPAUSE			
							+  iPausedTime 
				#endif //TIMESTAMPPAUSE
							); 
            	#else //TIMESTAMP   
					timeStamp =TTimeIntervalMicroSeconds( ( iPosition * 1000 ) + AddToTimeStamp ) ;
            	#endif //TIMESTAMP

				TAudioFrameItem item;
				item.iFrame = HBufC8::NewL( FrameSize );
    			item.iFrame->Des().Copy( iMediaBuffer->Mid( startPosition , FrameSize ) );
    			item.iTimeStamp = timeStamp;
    			User::LeaveIfError( iAudioFrames.Append(item) );
	            
			    AddToTimeStamp += 20000;
	            startPosition += FrameSize;
	            }    
		    delete iMediaBuffer;
		    iMediaBuffer = NULL; 

			if ( ( timeNow - 500 ) < ( ( TInt ) iAudioPosition + iTimeDifference + iReplayTimeStamp + ( iPausedTime / 1000 ) ) )
				{
				__FILESOURCE_CONTROLL( "CMccFileSourceImpl::ReadAudioFrameL, \
Position check, returning" )
				__FILESOURCE_CONTROLL( "CMccFileSourceImpl::ReadAudioFrameL, \
IGNORING" )
				//return;
				}

			}
		else
			{
			__FILESOURCE_CONTROLL( "CMccFileSourceImpl::ReadAudioFrameL, \
reading frames failed" )
			}
    	}
    else
    	{
		__FILESOURCE_CONTROLL_INT1( "CMccFileSourceImpl::ReadAudioFrameL, \
existing frames, count", iAudioFrames.Count() )    		
    	}

	__FILESOURCE_CONTROLL( "CMccFileSourceImpl::ReadAudioFrameL, \
writing buffer, copying" ) 
	CMMFDataBuffer* buf = static_cast<CMMFDataBuffer*>(iAudioConsumerBuffer); 		
	__FILESOURCE_CONTROLL_INT1( "CMccFileSourceImpl::ReadAudioFrameL, \
writing buffer, len", buf->BufferSize() )
	HBufC8* audioFrame = iAudioFrames[0].iFrame; 
    buf->Data().Copy( *audioFrame );
 	iAudioConsumerBuffer->SetTimeToPlay(iAudioFrames[0].iTimeStamp);
	iAudioConsumerBuffer->SetLastBuffer(EFalse);
    iAudioFrames.Remove(0);
    delete audioFrame;
      	
	__FILESOURCE_CONTROLL( "CMccFileSourceImpl::ReadAudioFrameL, \
writing buffer, complete" ) 

	__FILESOURCE_CONTROLL( "CMccFileSourceImpl::ReadAudioFrameL, \
writing buffer to sink" )   		
	iAudioConsumer->BufferFilledL( iAudioConsumerBuffer );
	iAudioConsumer = 0;
	iAudioConsumerBuffer = 0;
	__FILESOURCE_CONTROLL( "CMccFileSourceImpl::ReadAudioFrameL, \
writing buffer to sink complete" )  	
	}
// -----------------------------------------------------------------------------
// CMccFileSourceImpl::ReadVideoFrameL()
// 
// Reads the next video frame from 3gp file
// -----------------------------------------------------------------------------
//
void CMccFileSourceImpl::ReadVideoFrameL()
    {
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::ReadVideoFrameL" )

    MP4Err error;
    
  	if ( iVideoConsumer == NULL)
  		{
  		__FILESOURCE_CONTROLL( "CMccFileSourceImpl::ReadVideoFrameL, \
source not available" ) 
  		return;
  		}
  
  	__FILESOURCE_CONTROLL( "CMccFileSourceImpl::ReadVideoFrameL, \
writing buffer" ) 
  			
    
    error = MP4ParseIsFrameAvailable( iMP4Handle, iVideoType );
    error = MP4ParseIsFrameAvailable( iMP4Handle, MP4_TYPE_AVC_PROFILE_BASELINE );
    error = MP4ParseIsFrameAvailable( iMP4Handle, iVideoType );
    error = MP4ParseIsFrameAvailable( iMP4Handle, MP4_TYPE_AVC_PROFILE_MAIN );
    error = MP4ParseIsFrameAvailable( iMP4Handle, MP4_TYPE_AVC_PROFILE_EXTENDED );
    error = MP4ParseIsFrameAvailable( iMP4Handle, iVideoType );


	if ( error != MP4_OK )
        {
        iIsVideoLeft = EFalse;
  		__FILESOURCE_CONTROLL( "CMccFileSourceImpl::ReadVideoFrameL, \
no video" )  		
        }
    else 
    	{
    	iIsVideoLeft = ETrue;
  		__FILESOURCE_CONTROLL( "CMccFileSourceImpl::ReadVideoFrameL, \
video available" ) 		
    	}

    // If no more frames available inform observer and stop playing
    if ( iIsAudioLeft == EFalse && iIsVideoLeft == EFalse )
        {
  		__FILESOURCE_CONTROLL( "CMccFileSourceImpl::ReadVideoFrameL, \
no video & audio available" )  		

		#ifdef TIMESTAMP
			iReplayTimeStamp = iReplayTimeStamp + iPosition + 10;
		#endif
		
		SetPositionL(  TTimeIntervalMicroSeconds ( 0 ) );
		SourcePauseL();

        if ( iObserver ) 
        	{
  			__FILESOURCE_CONTROLL( "CMccFileSourceImpl::ReadVideoFrameL, \
  	sending EOF" ) 
        	iObserver->EndOfFile( KErrNone );
        	}
        return;
        }

	// Timing correction code
	TTimeIntervalMicroSeconds elapsedTime;
    TTime currentTime;
    currentTime.HomeTime();
    elapsedTime = currentTime.MicroSecondsFrom( iStartTime );
    TInt64 tempTime = elapsedTime.Int64();
    TInt timeNow = tempTime /*.GetTInt()*/ / 1000;

	if ( ( timeNow + 500 ) < ( ( TInt ) iVideoPosition + iTimeDifference + iReplayTimeStamp + ( iPausedTime / 1000 ) ) )
		{
  		__FILESOURCE_CONTROLL( "CMccFileSourceImpl::ReadVideoFrameL, Position check, returning" ) 
  		__FILESOURCE_CONTROLL_INT1( "CMccFileSourceImpl::ReadVideoFrameL", (timeNow + 500))
  		__FILESOURCE_CONTROLL_INT1( "CMccFileSourceImpl::ReadVideoFrameL", 
  			(( TInt ) iVideoPosition + iTimeDifference + iReplayTimeStamp + ( iPausedTime / 1000 ) ) )  				
  		__FILESOURCE_CONTROLL_INT4( "CMccFileSourceImpl::ReadVideoFrameL, videoPosition", iVideoPosition, 
  			                        "TimeDifference", iTimeDifference, 
  			                        "ReplyTimeStamp", iReplayTimeStamp, 
  			                        "PausedTime", iPausedTime )  				
  		__FILESOURCE_CONTROLL( "CMccFileSourceImpl::ReadVideoFrameL, IGNORING" )   				
		//return; 
		}

    // Read the size of next video frame
  	__FILESOURCE_CONTROLL( "CMccFileSourceImpl::ReadVideoFrameL, reading frame" ) 
    MP4ParseNextFrameSize( iMP4Handle, iVideoType,
        ( mp4_u32* ) &iBufferSize );
    
    HBufC8* tempBuff = NULL;
    if ( IsAVCVideo() && iIsFirstVideoFrame )
    	{
    	ExtractH264ParameterSetNALUsL( &tempBuff );
    	if ( NULL != tempBuff )
    		{
    		CleanupStack::PushL( tempBuff );
    		}
    	iIsFirstVideoFrame = EFalse;
    	}
    // Allocate a buffer big enough for the next video frame
  	__FILESOURCE_CONTROLL_INT1( "CMccFileSourceImpl::ReadVideoFrameL, reading frame, size", iBufferSize )
    HBufC8* iMediaBuffer = HBufC8::NewL( iBufferSize );
    // Pointer descriptor to the buffer	
    const TUint8* iPtrMediaSinkBuffer = iMediaBuffer->Des().Ptr();
	   
    if ( MP4ParseReadVideoFrame( iMP4Handle,
        ( mp4_u8* ) iPtrMediaSinkBuffer, ( mp4_u32 ) iBufferSize,
        ( mp4_u32* ) &iVideoFrameSize, ( mp4_u32* ) &iPosition,
        ( mp4_bool* ) &iKeyFrame, NULL ) == MP4_OK )
		{
  		__FILESOURCE_CONTROLL( "CMccFileSourceImpl::ReadVideoFrameL, reading frame, OK" )   		
        TPtr8 iMediaSinkBuffer = iMediaBuffer->Des();

		iVideoPosition = iPosition ;
		

        iMediaSinkBuffer.SetLength( iBufferSize ); // Sets buffer length
        if ( IsAVCVideo() )
        {
        MarkWithNALUDelimiters( iBufferSize, iMediaSinkBuffer );
    	}

		TTimeIntervalMicroSeconds timeStamp( 0 );
		
        #ifdef TIMESTAMP
			timeStamp = TTimeIntervalMicroSeconds( ( iPosition + iTimeDifference + iReplayTimeStamp ) * 1000 
			#ifdef TIMESTAMPPAUSE
						 + iPausedTime  
			#endif //TIMESTAMPPAUSE
				); 
        	#else //TIMESTAMP
				timeStamp = TTimeIntervalMicroSeconds ( iPosition * 1000 );
        	#endif //TIMESTAMP
   
            //buffer = new ( ELeave ) CCMRMediaBuffer(
            //iMediaSinkBuffer, CCMRMediaBuffer::EVideoH263, iBufferSize,
            //FALSE,  timeStamp );
             
  		__FILESOURCE_CONTROLL( "CMccFileSourceImpl::ReadVideoFrameL, writing buffer, copying" ) 
  		CMMFDataBuffer* buf = static_cast<CMMFDataBuffer*>(iVideoConsumerBuffer); 		
  		__FILESOURCE_CONTROLL_INT1( "CMccFileSourceImpl::ReadVideoFrameL, writing buffer, len", buf->BufferSize() )
  		if( NULL != tempBuff )
  			{
  			buf->Data().Copy( *tempBuff );
  			CleanupStack::PopAndDestroy( tempBuff );
        	tempBuff = NULL;
  			}
  		else
  			{
  			buf->Data().Delete( 0, buf->BufferSize() );
  			}
        buf->Data().Append( *iMediaBuffer );
           	
	 	iVideoConsumerBuffer->SetTimeToPlay(timeStamp);
		iVideoConsumerBuffer->SetLastBuffer(EFalse);
  		__FILESOURCE_CONTROLL( "CMccFileSourceImpl::ReadVideoFrameL, writing buffer, complete" ) 

        TInt err = KErrNone;
		if ( err == KErrNone )
            {

  			__FILESOURCE_CONTROLL( "CMccFileSourceImpl::ReadVideoFrameL, writing buffer to sink" ) 
  			iVideoConsumer->BufferFilledL( iVideoConsumerBuffer );
  			iVideoConsumer = 0;
  			iVideoConsumerBuffer = 0;
  			__FILESOURCE_CONTROLL( "CMccFileSourceImpl::ReadVideoFrameL, writing buffer to sink complete" )   		
  			   
			if ( err != KErrNone )
            	{
            	delete iMediaBuffer;
            	iMediaBuffer = NULL;
            	User::Leave( KErrGeneral );
            	}
            }
        }
        else
	        {
	        __FILESOURCE_CONTROLL( "CMccFileSourceImpl::ReadVideoFrameL, reading frame, NOT OK" )
	        }
   
    delete iMediaBuffer;
    iMediaBuffer = NULL;

	if ( ( timeNow - 500 ) < ( ( TInt ) iVideoPosition + iTimeDifference + iReplayTimeStamp + ( iPausedTime / 1000 ) ) )
		{
  		__FILESOURCE_CONTROLL( "CMccFileSourceImpl::ReadVideoFrameL, Time Check 2, IGNORING" )    						//return;
		}

  	__FILESOURCE_CONTROLL( "CMccFileSourceImpl::ReadVideoFrameL, writing buffer to sink complete" ) 		
    }

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::PlayL()
// 
// Starts streaming from 3gp file
// -----------------------------------------------------------------------------
//
void CMccFileSourceImpl::SourcePlayL()
    {
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::SourcePlayL" )
    
    __ASSERT_ALWAYS( iCurrentState != EConstructed, User::Leave( KErrNotReady ) );
    			
    if ( iCurrentState == EPlaying )
        {
        return;
        }

    #ifdef TIMESTAMP
        if ( iCurrentState == EPaused ) 
            {
            iPauseStopped.HomeTime();
//#ifdef TIMESTAMPPAUSE
            TTimeIntervalMicroSeconds differenceTemp1 = 
            	iPauseStopped.MicroSecondsFrom(iPauseStarted);
            TInt64 differenceTemp2 = differenceTemp1.Int64();
            iPausedTime = iPausedTime + differenceTemp2 /*.GetTInt()*/;
//#endif //TIMESTAMPPAUSE
            }
		else
			{
			 iStartTime.HomeTime(); // Set start time to current time
			}
    #endif
 /*   

    // Variables for periodic timers
    // temporary flagged hack for WINS to get timing right
    TInt iTickIntervalVideo = 1000000;
    TInt iTickIntervalAudio = 10000;

    if ( iFrameRate != 0 )
        {
        // Set interval for video
        iTickIntervalVideo = ( 1000000 / ( TInt ) iFrameRate  ) ; 
        }
        

    TInt ticks = 10000;

	// ROP plays faster so this code was added to help syncronisation
//	iTickIntervalAudio = TInt (iTickIntervalAudio *  ( ( 100.0 - KPlaybackSync ) / 100.0 )) ;
	iTickIntervalVideo = TInt (iTickIntervalVideo *  ( ( 100.0 - KPlaybackSync ) / 100.0 )) ;
    	
    // Creation and starting of periodic timers
	 
    if ( iIsAudio )
        {
        if ( !iPeriodicAudio )
	        {
	        iPeriodicAudio = CMccPeriodicRunner::NewL( *this );
	        }
        
        iPeriodicAudio->Start( iTickIntervalAudio, 
                               TCallBack( TickAudioL, this ) );
        }
    if ( iIsVideo )
        {
        if ( !iPeriodicVideo )
	        {
	        iPeriodicVideo = CMccPeriodicRunner::NewL( *this );
	
	        }
        iPeriodicVideo->Start( iTickIntervalVideo,
                               TCallBack( TickVideoL, this ) );
        }		
       
       */
    iCurrentState = EPlaying;
    }
    
// -----------------------------------------------------------------------------
// CMccFileSourceImpl::TickAudioL
// 
// -----------------------------------------------------------------------------
//
TInt CMccFileSourceImpl::TickAudioL(  TAny* aObject  )
	{
	__FILESOURCE_CONTROLL( "CMccFileSourceImpl::TickAudioL" )

    if ( ( ( CMccFileSourceImpl* ) aObject )->iFramesPerSample <= 2 )
        {
        //Reads five frames in one tick 
        for ( int x = 0 ; x < 5; x++ )
            {
            ( ( CMccFileSourceImpl* ) aObject )->ReadAudioFrameL();
            }
        }

    if ( ( ( CMccFileSourceImpl* ) aObject )->iFramesPerSample == 3 ||
         ( ( CMccFileSourceImpl* ) aObject )->iFramesPerSample == 4 )
         {
               
        //Reads two frames in one tick 
        for ( int x = 0 ; x < 2; x++ )
            {
            ( ( CMccFileSourceImpl* ) aObject )->ReadAudioFrameL() ;
            }
        }

    if ( ( ( CMccFileSourceImpl* ) aObject )->iFramesPerSample >= 5 ) 
        {   
        ( ( CMccFileSourceImpl* ) aObject )->ReadAudioFrameL();
        }
        
    return KErrNone;
	}
    
// -----------------------------------------------------------------------------
// CMccFileSourceImpl::TickVideoL
// 
// -----------------------------------------------------------------------------
//
TInt CMccFileSourceImpl::TickVideoL( TAny* aObject )
	{
	( ( CMccFileSourceImpl* ) aObject )->ReadVideoFrameL();
	
	return KErrNone;
	}
        
// -----------------------------------------------------------------------------
// CMccFileSourceImpl::PauseL()
// 
// Pauses streaming by cancelling timers
// -----------------------------------------------------------------------------
//
void CMccFileSourceImpl::SourcePauseL()
    {
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::SourcePauseL" )

    if ( iCurrentState != EPlaying ) 
    	{
    	return;
    	}
    /*
    __ASSERT_ALWAYS( iPeriodicAudio, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iPeriodicAudio, User::Leave( KErrArgument ) );

    iPeriodicAudio->Cancel();
    iPeriodicVideo->Cancel();
      */  
    iCurrentState = EPaused;
	}

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::StopL()
// 
// Stops streaming
// -----------------------------------------------------------------------------
//
void CMccFileSourceImpl::SourceStopL()
    {
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::SourceStopL" )

    if ( iCurrentState == EConstructed )
        {
        return;
        }
/*
    if ( iCurrentState == EPlaying )
        {   
        __ASSERT_ALWAYS( iPeriodicAudio, User::Leave( KErrArgument ) );
        __ASSERT_ALWAYS( iPeriodicAudio, User::Leave( KErrArgument ) );

		iPeriodicAudio->Cancel();
		iPeriodicVideo->Cancel();
        }
*/	
    SetPositionL(  TTimeIntervalMicroSeconds ( 0 ) );

    iCurrentState = EStopped;

    #ifdef TIMESTAMP
       
	if ( iIsAudioLeft == TRUE  && iIsVideoLeft == TRUE  )
		{
		iTimeDifference = 0;
		iPausedTime=0;
		}
		
    iPauseStarted = 0;
    iPauseStopped = 0;
        
    #endif

    }

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::GetFrameLenght()
// 
// Gets audio frame lenght from given buffer
// -----------------------------------------------------------------------------
//
TUint CMccFileSourceImpl::GetFrameLength( TPtrC8 aData )
	{
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::GetFrameLength" )

	const TUint8 *ptrData=aData.Ptr();
	//read FT from data
	TUint8 fT = (TUint8) ( *ptrData & 0x78 );
	fT = TUint8 ( fT >> 3 );
	//return frame length
	switch ( fT )
		{
			case 0: //amr 4.75
			{
                return 13;
            }
            case 1: //amr 5.15
			{
                return 14;
            }
			case 2: //amr 5.9
			{
				return 16;
            }
 			case 3: //amr 6.7
            {
				return 18;
            }
			case 4: //amr 7.4
			{
				return 20;
            }
            case 5: //amr 7.95
            {
			    return 21;
            }
            case 6: //amr 10.2
			{
                return 27;
            }
            case 7: //amr 12.2
            {
                return 32;
            }
			case 8: //amr SID
			{
				return 6;
			}

			case 15: //no data
				{
				return 1;
				}
            default: //not arm narrowband
            {
				return 0;
            }
        }
}

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::SourceDataTypeCode()
// -----------------------------------------------------------------------------
//
TFourCC CMccFileSourceImpl::SourceDataTypeCode( TMediaId aMediaId )
	{
	__FILESOURCE_CONTROLL( "CMccFileSourceImpl::SourceDataTypeCode" )

    if ( KUidMediaTypeVideo == aMediaId.iMediaType )
        {
        return iVideoFourCC;
        }
    else if ( KUidMediaTypeAudio == aMediaId.iMediaType  )
        {
        return iAudioFourCC;
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
	
    if ( ( KUidMediaTypeVideo == aMediaId.iMediaType &&
    	aCodec == iVideoFourCC ) ||
    	 ( KUidMediaTypeAudio == aMediaId.iMediaType &&
    	aCodec == iAudioFourCC ) )
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

	if (aMediaId.iMediaType == KUidMediaTypeAudio)
		{
    	__FILESOURCE_MEDIA( "CMccFileSourceImpl::FillBufferL, audio" )
	    iAudioConsumer = aConsumer;
		iAudioConsumerBuffer = aBuffer;		
		}
	else if (aMediaId.iMediaType == KUidMediaTypeVideo )
		{
    	__FILESOURCE_MEDIA( "CMccFileSourceImpl::FillBufferL, video" )    
	    iVideoConsumer = aConsumer;
		iVideoConsumerBuffer = aBuffer;		
		}
	else
		{
    	__FILESOURCE_MEDIA( "CMccFileSourceImpl::FillBufferL, unknown media " ) 
		User::Leave( KErrNotSupported );	
		}

	}	
	
// -----------------------------------------------------------------------------
// CMccFileSourceImpl::MarkWithNALUDelimiters()
// -----------------------------------------------------------------------------
//
void CMccFileSourceImpl::MarkWithNALUDelimiters( 
	const TInt aAccessUnitSize, TDes8& aBuffer )
	{
    __FILESOURCE_CONTROLL( "CMccFileSourceImpl::MarkWithNALUDelimiters" )
    
    TUint8* ptrByte = NULL;
    TInt indx = 0;
    TUint32 size;
    const TUint32 size_field_len = 4;
    
    do
	    {
    	ptrByte = const_cast<TUint8*> ( aBuffer.Mid( indx ).Ptr() );	// get starting point of NALU size
    	size = BigEndian::Get32(ptrByte);	// get NALU size
    	aBuffer[indx+0] = 0x00;
    	aBuffer[indx+1] = 0x00;
    	aBuffer[indx+2] = 0x00;
    	aBuffer[indx+3] = 0x01;
    	indx = indx + size + size_field_len;
    	
	    }while(indx < aAccessUnitSize);
    
	}

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::ExtractH264ParameterSetNALUsL()
// -----------------------------------------------------------------------------
//

void CMccFileSourceImpl::ExtractH264ParameterSetNALUsL( HBufC8** aBufferOut )
	{
	__FILESOURCE_CONTROLL( "CMccFileSourceImpl::ExtractH264ParameterSetNALUsL" )
	
	*aBufferOut = NULL;
	
	MP4Err error = MP4_OK;
	HBufC8* buffer = NULL;
	HBufC8* bufferOut = NULL;
	TUint32 decspecinfosize;
	TUint8* ptrByte;
	
	// Query for size
	error = MP4ParseReadVideoDecoderSpecificInfo( iMP4Handle,
												  ( mp4_u8* ) NULL,
												  0,
												  ( mp4_u32* ) &decspecinfosize
											 	);
	if( ( MP4_OK != error ) && ( MP4_BUFFER_TOO_SMALL != error ) )
		{
		User::Leave(KErrGeneral);
		}
	
	buffer = HBufC8::NewLC(decspecinfosize);
	ptrByte = const_cast<TUint8*> ( buffer->Des().Ptr() );
	
	error = MP4ParseReadVideoDecoderSpecificInfo( iMP4Handle,
												  ( mp4_u8* ) ptrByte,
												  ( mp4_u32 ) buffer->Des().MaxSize(),
												  ( mp4_u32* ) &decspecinfosize
											 	);
	if(MP4_OK != error)
		{
		User::Leave(KErrGeneral);
		}
	else
		{
		buffer->Des().SetLength( decspecinfosize );
		}
	
	bufferOut = HBufC8::NewLC( buffer->Size() * 2 );
	TPtr8 ptrBufferOut = bufferOut->Des();
	TPtr8 ptrBuffer = buffer->Des();
	
	// find NALUs in decoder info and put in buffer by marking with delimiters
	
	TUint8 tmpByte;
	TUint16 size16;
	TInt indx = 5;		// skip other info in the beginning
	TInt numNALUs = 0;
	TInt lp_indx;
	
	// get number of seq. parameter set NALUs
	tmpByte = ptrBuffer[indx++];
	numNALUs = tmpByte & 0x1F;		// get rid of reserved '111' bits
	
	// extract seq. parameter set NALUs
	
	for ( lp_indx = 0; lp_indx < numNALUs; lp_indx++ )
		{
		size16 = BigEndian::Get16( ptrBuffer.Mid( indx ).Ptr() );
		indx += 2;
		
		// insert delimiter
		tmpByte = 0x00;
		ptrBufferOut.Append( &tmpByte, 1 );
		ptrBufferOut.Append( &tmpByte, 1 );
		ptrBufferOut.Append( &tmpByte, 1 );
		tmpByte = 0x01;
		ptrBufferOut.Append( &tmpByte, 1 );
		
		ptrBufferOut.Append( ptrBuffer.Mid( indx, size16 ) );
		
		indx  += size16;
		}
	
	// get number of pic. parameter set NALUs
	tmpByte = ptrBuffer[indx++];
	numNALUs = tmpByte;
	
	// extract pic. parameter set NALUs
	
	for ( lp_indx = 0; lp_indx < numNALUs; lp_indx++ )
		{
		size16 = BigEndian::Get16( ptrBuffer.Mid( indx ).Ptr() );
		indx += 2;
		
		// insert delimiter
		tmpByte = 0x00;
		ptrBufferOut.Append( &tmpByte, 1 );
		ptrBufferOut.Append( &tmpByte, 1 );
		ptrBufferOut.Append( &tmpByte, 1 );
		tmpByte = 0x01;
		ptrBufferOut.Append( &tmpByte, 1 );
		
		ptrBufferOut.Append( ptrBuffer.Mid( indx, size16 ) );
		
		indx  += size16;
		}
	
	CleanupStack::Pop( bufferOut );	
	CleanupStack::PopAndDestroy( buffer );
	
	*aBufferOut = bufferOut;
	
	}
	

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::IsAVCVideo()
// -----------------------------------------------------------------------------
//

TBool CMccFileSourceImpl::IsAVCVideo()
	{
	if ( ( iVideoType == MP4_TYPE_AVC_PROFILE_BASELINE ) ||
         ( iVideoType == MP4_TYPE_AVC_PROFILE_MAIN )	 ||
         ( iVideoType == MP4_TYPE_AVC_PROFILE_EXTENDED )
       )
	    {
	   	return ETrue;
	    }
    
    return EFalse;
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
// CMccFileSourceImpl::ParseUpdateVideoDescriptions
// -----------------------------------------------------------------------------
//
void CMccFileSourceImpl::ParseUpdateVideoDescriptions( )
	{		
	if ( MP4ParseRequestVideoDescription( iMP4Handle,
		 ( mp4_u32* ) &iVideoLength,  &iFrameRate, ( mp4_u32* ) &iVideoType,
	 	 ( mp4_u32* ) &iVideoWidth,  ( mp4_u32* ) &iVideoHeight,
         ( mp4_u32* ) &iVideoTimeScale ) 
         == MP4_OK )
		{
		iFrameSize.iWidth = iVideoWidth;
		iFrameSize.iHeight = iVideoHeight;
		  
		if ( iVideoType == MP4_TYPE_H263_PROFILE_0 )
		    {
		    RDebug::Print(_L("CMccFileSourceImpl::OpenFileL MP4_TYPE_H263_PROFILE_0"));

		    iCurrentVideoType = CCMRMediaBuffer::EVideoH263;
		    iIsVideo = ETrue;
		    iIsVideoLeft = ETrue;
		 //   iVideoFourCC = TFourCC( KMccFourCCIdAVC );
		    iVideoFourCC = TFourCC( KMccFourCCIdH263 );
		    }
		if ( iVideoType == MP4_TYPE_MPEG4_VIDEO )
		    {
		    RDebug::Print(_L("CMccFileSourceImpl::OpenFileL MP4_TYPE_MPEG4_VIDEO!"));

		    iCurrentVideoType = CCMRMediaBuffer::EVideoH263;
		    iIsVideo = ETrue;
		    iIsVideoLeft = ETrue;
		    iVideoFourCC = TFourCC( KMccFourCCIdAVC );
		    }
		if ( IsAVCVideo() )
		    {
		    RDebug::Print(_L("CMccFileSourceImpl::OpenFileL AVC!"));

		    iCurrentVideoType = CCMRMediaBuffer::EVideoH263;
		    iIsVideo = ETrue;
		    iIsVideoLeft = ETrue;
		    iVideoFourCC = TFourCC( KMccFourCCIdAVC );
		    iIsFirstVideoFrame = ETrue;
		    }
		}
	}

// -----------------------------------------------------------------------------
// CMccFileSourceImpl::ParseUpdateVideoDescriptions
// -----------------------------------------------------------------------------
//	    
void CMccFileSourceImpl::ParseUpdateAudioDescriptions( )
	{
    if ( MP4ParseRequestAudioDescription( iMP4Handle,
         ( mp4_u32* ) &iAudioLength, ( mp4_u32* ) &iAudioType,
         ( mp4_u8* ) &iFramesPerSample, ( mp4_u32* ) &iAudioTimeScale,
         ( mp4_u32* ) &iAverageBitRate )
         == MP4_OK )
	    {
        if ( iAudioType == MP4_TYPE_AMR_NB )
	        {
	        RDebug::Print(_L("CMccFileSourceImpl::OpenFileL MP4_TYPE_AMR_NB!"));

	        iCurrentAudioType = CCMRMediaBuffer::EAudioAMRNB;
	        iIsAudio = ETrue;
	        iIsAudioLeft = ETrue;
	        iAudioFourCC = TFourCC( KMccFourCCIdAMRNB );
	        }	
	    }
	}

#ifndef EKA2
// DLL interface code
EXPORT_C TInt E32Dll( TDllReason )
    {
    return KErrNone;
    }
#endif
