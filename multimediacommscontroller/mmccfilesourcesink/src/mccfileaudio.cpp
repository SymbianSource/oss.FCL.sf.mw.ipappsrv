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




#include "mccfileaudio.h"
#include "mmcccodecinformation.h"
#include "mccperiodicrunner.h"
#include "mccfilesourcelogs.h"
#include "mccinternaldef.h"


// -----------------------------------------------------------------------------
// CMccFileAudio::NewL
// -----------------------------------------------------------------------------
//
CMccFileAudio* CMccFileAudio::NewL( CMccFileSourceImpl& aSource )
	{
	CMccFileAudio* self = new (ELeave) CMccFileAudio ( aSource );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	
	return self;
	}

// -----------------------------------------------------------------------------
// CMccFileAudio::ConstructL
// -----------------------------------------------------------------------------
//
void CMccFileAudio::ConstructL()
	{
	CMccFileSourceTypeBase::ConstructL();	
	}
	
// -----------------------------------------------------------------------------
// CMccFileAudio::CMccFileAudio
// -----------------------------------------------------------------------------
//
CMccFileAudio::CMccFileAudio( CMccFileSourceImpl& aSource )
	: CMccFileSourceTypeBase ( aSource, KUidMediaTypeAudio ),
	  iBufferType ( CCMRMediaBuffer::EAudioAMRNB )
	{
	}

// -----------------------------------------------------------------------------
// CMccFileAudio::~CMccFileAudio
// -----------------------------------------------------------------------------
//
CMccFileAudio::~CMccFileAudio( )
	{
	}

// -----------------------------------------------------------------------------
// CMccFileAudio::FillBufferL
// -----------------------------------------------------------------------------
//
void CMccFileAudio::FillBufferL( 
    CMMFBuffer* aBuffer,
    MDataSink* aConsumer )
	{
	CMccFileSourceTypeBase::FillBufferL( aBuffer, aConsumer );
	
	// There might be pending audio frames, read the oldest one immediately
	WriteBufferL();
	}
	
// -----------------------------------------------------------------------------
// CMccFileAudio::ParseUpdateAudioDescriptions
// -----------------------------------------------------------------------------
//
TBool CMccFileAudio::ParseUpdateAudioDescriptions( MP4Handle aMP4Handle )
	{
	TBool isAudio = EFalse;
	
	mp4_u32 timeScale = 0;
	if ( MP4ParseRequestAudioDescription( aMP4Handle,
         ( mp4_u32* ) &iLength, ( mp4_u32* ) &iType,
         ( mp4_u8* ) &iFramesPerSample, ( mp4_u32* ) &timeScale,
         ( mp4_u32* ) &iAverageBitRate )
         == MP4_OK )
	    {
        if ( iType == MP4_TYPE_AMR_NB )
	        {
	        __FILESOURCE_CONTROLL( "CMccFileAudio:ParseUpdateAudioDescriptions \
MP4_TYPE_AMR_NB!" )

	        iBufferType = CCMRMediaBuffer::EAudioAMRNB;
	        isAudio = ETrue;
	        iFourCC = TFourCC( KMccFourCCIdAMRNB );
	        }
	    else if ( iType != MP4_TYPE_NONE )
	        {
	        __FILESOURCE_CONTROLL( "CMccFileAudio:ParseUpdateAudioDescriptions \
contains unsupported audio" )

		    iFourCC = TFourCC( KMccFourCCUnsupported );
	        }
	    else
	        {
	        // NOP
	        }
	    }
	return isAudio;	
	}
	
// -----------------------------------------------------------------------------
// CMccFileAudio::DoSetPositionL
// -----------------------------------------------------------------------------
//
void CMccFileAudio::DoSetPositionL( TUint32 aPosition )
	{
	__FILESOURCE_CONTROLL( "CMccFileAudio::DoSetPositionL" )

    __ASSERT_ALWAYS( aPosition <= iLength, User::Leave( KErrArgument ) );

    mp4_u32 videoPos = 0;
    if ( MP4ParseSeek( iMP4Handle, ( mp4_u32 ) aPosition,
         ( mp4_u32* ) &iPosition, ( mp4_u32* ) &videoPos, MP4TRUE )
         != MP4_OK )
        {   
        User::Leave( KErrGeneral );
        }	
	}

// -----------------------------------------------------------------------------
// CMccFileAudio::Type
// -----------------------------------------------------------------------------
//
CCMRMediaBuffer::TBufferType CMccFileAudio::Type()
	{
	return iBufferType;
	}
	
// -----------------------------------------------------------------------------
// CMccFileAudio::GetFourCC
// -----------------------------------------------------------------------------
//
TFourCC CMccFileAudio::GetFourCC()
	{
	return iFourCC;
	}

// -----------------------------------------------------------------------------
// CMccFileAudio::TickCallBack
// -----------------------------------------------------------------------------
//
TCallBack CMccFileAudio::TickCallBack()
    {
    return TCallBack( TickAudioL, this );
    }
    
// -----------------------------------------------------------------------------
// CMccFileAudio::PositionL
// -----------------------------------------------------------------------------
//
TUint32 CMccFileAudio::Position()
	{
	return iPosition;	
	}
	
// -----------------------------------------------------------------------------
// CMccFileAudio::DurationL
// -----------------------------------------------------------------------------
//
TUint32 CMccFileAudio::Duration()
	{
	return iLength;	
	}
	
// -----------------------------------------------------------------------------
// CMccFileAudio::AudioBitRateL
// -----------------------------------------------------------------------------
//
TUint32 CMccFileAudio::AudioBitRate()
	{
	return iAverageBitRate;	
	}
	
// -----------------------------------------------------------------------------
// CMccFileAudio::VideoFrameSizeL
// -----------------------------------------------------------------------------
//
TSize CMccFileAudio::VideoFrameSize()
	{
	TSize size;
    return size;
	}
	
// -----------------------------------------------------------------------------
// CMccFileAudio::VideoFrameRateL
// -----------------------------------------------------------------------------
//
TReal CMccFileAudio::VideoFrameRateL()
	{
	return 0;	
	}

// -----------------------------------------------------------------------------
// CMccFileAudio::StartTimerL
// -----------------------------------------------------------------------------
//
void CMccFileAudio::StartTimerL()
	{
	__FILESOURCE_CONTROLL( "CMccFileAudio::StartTimerL" )
	
	if ( IsPaused() )
	    {
	    SetPaused( EFalse );
	    
	    SetPositionL( iPosition, ETrue );
	    }
	else
	    {
	    SetStartTime();
	    }
	
	const TInt KMccFileIntervalMinInterval = 60000;       
	const TInt KMccFileAudioPTime = 20000;
	TInt tickIntervalAudio = 0;
	iReadsPerInterval = 0;
	
	__ASSERT_ALWAYS( iFramesPerSample > 0, User::Leave( KErrArgument ) );
	
	while ( tickIntervalAudio < KMccFileIntervalMinInterval )
        {
        tickIntervalAudio += KMccFileAudioPTime * iFramesPerSample;
        iReadsPerInterval++;
        }
    
   	__FILESOURCE_CONTROLL_INT1( "CMccFileAudio::StartTimer (timer):", 
                            tickIntervalAudio )
                            
    iTimerInterval = tickIntervalAudio;
    iCorrectionInterval = iTimerInterval;
    
	iPeriodicRunner->Start( tickIntervalAudio, TCallBack( TickAudioL, this ) );	
 
	__FILESOURCE_CONTROLL( "CMccFileAudio::StartTimerL, exit" )	
	}

// -----------------------------------------------------------------------------
// CMccFileAudio::TickAudioL
// -----------------------------------------------------------------------------
//
TInt CMccFileAudio::TickAudioL( TAny* aObject )
	{
	__FILESOURCE_CONTROLL( "CMccFileAudio::TickAudioL" )
	
	CMccFileAudio* self = static_cast<CMccFileAudio*>( aObject );

    TBool read( ETrue );
    TInt readsPerInterval( self->iReadsPerInterval );
    for ( TInt i = 0; i < readsPerInterval && read; i++ )
        {
        // Do timing correction only after everything within this cycle has
        // been read
        TBool doTimingCorrection( ( i + 1 ) == readsPerInterval );
        read = self->ReadFrameL( doTimingCorrection );
        }
   
    return KErrNone;	
	}

// -----------------------------------------------------------------------------
// CMccFileAudio::ReadFrameL
// -----------------------------------------------------------------------------
//
TBool CMccFileAudio::ReadFrameL( TBool aDoTimingCorrection )
	{
	__FILESOURCE_CONTROLL( "CMccFileAudio::ReadFrameL" )	    
	   

	if ( MP4ParseIsFrameAvailable( iMP4Handle, MP4_TYPE_AMR_NB ) != MP4_OK )
        {	
        __FILESOURCE_CONTROLL( "CMccFileAudio::ReadFrameL, no audio" ) 

		SetPaused( ETrue );
		TRAPD( positionErr, SetPositionL( 0 ) );
		iSource.PauseAudioL( positionErr );	
        return EFalse;	
        }
    
	MP4ParseNextFrameSize( iMP4Handle, MP4_TYPE_AMR_NB,
        ( mp4_u32* ) &iBufferSize );

    HBufC8* mediaBuffer = HBufC8::NewLC( iBufferSize );
    TPtr8 mediaSinkBuffer = mediaBuffer->Des();
    mediaSinkBuffer.SetLength( iBufferSize );
    TUint8* bufPtr = CONST_CAST(TUint8*, mediaSinkBuffer.Ptr());

    TUint32 oldPosition = iPosition;
    if ( MP4ParseReadAudioFrames( iMP4Handle,
        ( mp4_u8* )  bufPtr,
        ( mp4_u32 ) iBufferSize, ( mp4_u32* ) &iAudioSize,
        ( mp4_u32* ) &iPosition, ( mp4_u32* ) &iReturnedFrames,
		NULL ) == MP4_OK )
		{
		__FILESOURCE_CONTROLL_INT1( ":ReadFrameL:ReadAudioFrameL, \
read, time position", iPosition ) 	
        __FILESOURCE_CONTROLL_INT1( ":ReadFrameL:ReadAudioFrameL, \
read, returned frames", iReturnedFrames ) 	

        if ( aDoTimingCorrection )
            {
            DoTimingCorrection( iPosition );
            }
	
	    // Update read interval (how much position changes on each frame read)
        iReadInterval = iPosition - oldPosition;
    				
		// Calculate real frame size from sample size
        TInt frameSize = iBufferSize / iReturnedFrames;
        TUint counter = 0;
        TInt addToTimeStamp = 0;
        TInt startPosition = 0;
        TInt mediaBufferLen = mediaBuffer->Length();
        
        while ( ( counter < iReturnedFrames ) && 
                ( startPosition < mediaBufferLen ) )
            {
            counter++;
	
			frameSize = GetFrameLength( mediaBuffer->Mid( startPosition ) );
		    if ( frameSize != 0 && ( startPosition + frameSize <= mediaBufferLen ) )
			    {
			    TTimeIntervalMicroSeconds timeStamp = 
            	    GetTimeStamp( addToTimeStamp );
            	
                CMccFrameItem* item = new (ELeave) CMccFrameItem();
                CleanupStack::PushL( item );
                item->iFrame = HBufC8::NewL( frameSize );
                item->iFrame->Des().Copy( 
                    mediaBuffer->Mid( startPosition, frameSize ) );
                item->iTimeStamp = timeStamp;
                User::LeaveIfError( iFrames.Append( item ) );
                CleanupStack::Pop( item );
			    }
	   	    addToTimeStamp += 20000;
            startPosition += frameSize;				
            }    
		}
	else
		{
		__FILESOURCE_CONTROLL( ":ReadFrameL:ReadAudioFrameL, \
reading frames failed" )
		}
    CleanupStack::PopAndDestroy(mediaBuffer);
	mediaBuffer = NULL; 

	__FILESOURCE_CONTROLL( ":ReadFrameL:ReadAudioFrameL, \
writing buffer, copying" )

    WriteBufferL();
    
    return ETrue;
	}

// -----------------------------------------------------------------------------
// CMccFileAudio::WriteBufferL
// -----------------------------------------------------------------------------
//
void CMccFileAudio::WriteBufferL()
	{
	__FILESOURCE_CONTROLL("CMccFileAudio::WriteBufferL")
	
    if ( iFrames.Count() && iConsumer && iConsumerBuffer )
	    {
	    CMMFDataBuffer* buf = static_cast<CMMFDataBuffer*>(iConsumerBuffer); 
	    
	    HBufC8* audioFrame = (iFrames[0])->iFrame;
	    // to be sure the source buffer length is greater or equal
	    if ( buf->Data().MaxLength() >= audioFrame->Des().Length() )
		    {   
			buf->Data().Copy( *audioFrame );
			iConsumerBuffer->SetTimeToPlay((iFrames[0])->iTimeStamp);
			iConsumerBuffer->SetLastBuffer(EFalse);
			iConsumerBuffer->SetFrameNumber( iSequenceNum );
			delete iFrames[0];
			iFrames.Remove(0);
			audioFrame = NULL;
			iConsumer->BufferFilledL( iConsumerBuffer );
			iConsumer = NULL;
			iConsumerBuffer = NULL;
			iSequenceNum++;
			__FILESOURCE_CONTROLL("CMccFileAudio::WriteBufferL, done")	
		    }
		else
			{
			__FILESOURCE_CONTROLL("CMccFileAudio::WriteBufferL, frame dropped")
			
			delete iFrames[0];
			iFrames.Remove(0);
			audioFrame = NULL;
			}
	    }
	else
		{
		__FILESOURCE_CONTROLL("CMccFileAudio::WriteBufferL, writing ignored")
		}	
	}

// -----------------------------------------------------------------------------
// CMccFileAudio::GetFrameLength
// -----------------------------------------------------------------------------
//	
TUint32 CMccFileAudio::GetFrameLength( TPtrC8 aData )
	{
	const TUint8 *ptrData=aData.Ptr();
	//read FT from data
	TUint8 fT = (TUint8) ( *ptrData & 0x78 );
	fT = TUint8 ( fT >> 3 );
	//return frame length
	switch ( fT )
		{
		case 0: //amr 4.75
			{
			const TUint32 number13 = 13;
	        return number13;
	        }
        case 1: //amr 5.15
			{
			const TUint32 number14 = 14;
	        return number14;
	        }
		case 2: //amr 5.9
			{
			const TUint32 number16 = 16;

			return number16;
	        }
		case 3: //amr 6.7
	        {
	       	const TUint32 number18 = 18;
			return number18;
	        }
		case 4: //amr 7.4
			{
			const TUint32 number20 = 20;
			return number20;
	        }
        case 5: //amr 7.95
	        {
	        const TUint32 number21 = 21;
			return number21;
	        }
        case 6: //amr 10.2
			{
			const TUint32 number27 = 27;
	        return number27;
	        }
        case 7: //amr 12.2
	        {
	       	const TUint32 number32 = 32;
	        return number32;
	        }
		case 8: //amr SID
			{
	       	const TUint32 number6 = 6;
			return number6;
			}
		case 15: //no data
			{
	       	const TUint32 number1 = 1;
			return number1;
			}
        default: //not arm narrowband
	        {
	        const TUint32 number0 = 0;
			return number0;
	        }
        }	 
	}

// -----------------------------------------------------------------------------
// CMccFileAudio::GetTimeStamp
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CMccFileAudio::GetTimeStamp( TUint32 aAddToTimeStamp )
	{                       
	TInt64 position = iPosition + iPositionModifier;
	TUint32 position2 = ( position < 0 ) ? 0 : (TUint32)position;
	TTimeIntervalMicroSeconds timeStamp = 
	    TTimeIntervalMicroSeconds( ( (TInt64) position2 * KMccMicroToMilliConst ) + aAddToTimeStamp );

   	__FILESOURCE_CONTROLL_INT1( "CMccFileAudio::GetTimeStamp timestamp (without clock frequency):", 
                                timeStamp.Int64() )
                                
	return timeStamp;
	}

