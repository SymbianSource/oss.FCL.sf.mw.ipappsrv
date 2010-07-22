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




#include <e32std.h>
#include <es_sock.h>
#include <e32math.h>
#include "mccfilevideo.h"
#include "mmcccodecinformation.h"
#include "mccperiodicrunner.h"
#include "mccfilesourcelogs.h"
#include "mccfilesourceimpl.h"
#include "mccinternaldef.h"
#include "mccresources.h"


const TUint32 KMccFileSourceParameterSetsIntervalInMicroSeconds = 2000000;

// -----------------------------------------------------------------------------
// CMccFileVideo::NewL
// -----------------------------------------------------------------------------
//
CMccFileVideo* CMccFileVideo::NewL( 
    CMccFileSourceImpl& aSource, 
    MMccResources* aMccResources,
    TUint32 aEndpointId )
	{
	CMccFileVideo* self = 
        new (ELeave) CMccFileVideo( aSource, aMccResources, aEndpointId );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
	return self;
	}

// -----------------------------------------------------------------------------
// CMccFileVideo::ConstructL
// -----------------------------------------------------------------------------
//
void CMccFileVideo::ConstructL()
	{
	CMccFileSourceTypeBase::ConstructL();
	}

// -----------------------------------------------------------------------------
// CMccFileVideo::CMccFileVideo
// -----------------------------------------------------------------------------
//
CMccFileVideo::CMccFileVideo( 
    CMccFileSourceImpl& aSource, MMccResources* aMccResources, TUint32 aEndpointId )
	: CMccFileSourceTypeBase ( aSource, KUidMediaTypeVideo ),
	  iMccResources( aMccResources ),
	  iEndpointId( aEndpointId ),
	  iBufferType ( CCMRMediaBuffer::EVideoH263 )
	{
	}

// -----------------------------------------------------------------------------
// CMccFileVideo::~CMccFileVideo
// -----------------------------------------------------------------------------
//
CMccFileVideo::~CMccFileVideo()
	{
	delete iAVCNALUsBuffer;
	}

// -----------------------------------------------------------------------------
// CMccFileVideo::FillBufferL
// -----------------------------------------------------------------------------
//
void CMccFileVideo::FillBufferL( 
    CMMFBuffer* aBuffer,
    MDataSink* aConsumer )
	{
	CMccFileSourceTypeBase::FillBufferL( aBuffer, aConsumer );
	
	// There might be pending video frames, read the oldest one immediately
	WriteBufferL();
	}
		
// -----------------------------------------------------------------------------
// CMccFileVideo::ParseUpdateAudioDescriptions
// -----------------------------------------------------------------------------
//
TBool CMccFileVideo::ParseUpdateVideoDescriptions( MP4Handle aMP4Handle )
	{
	TBool containVideo = EFalse;
	mp4_u32 timeScale = 0;
	if ( MP4ParseRequestVideoDescription( aMP4Handle,
		 ( mp4_u32* ) &iLength,  &iFrameRate, ( mp4_u32* ) &iType,
	 	 ( mp4_u32* ) &iWidth,  ( mp4_u32* ) &iHeight,
         ( mp4_u32* ) &timeScale ) 
         == MP4_OK )
		{
		if ( iType == MP4_TYPE_H263_PROFILE_0 )
		    {
		    __FILESOURCE_CONTROLL( "CMccFileVideo::ParseUpdateVideoDescriptions \
MP4_TYPE_H263_PROFILE_0" )

		    iBufferType = CCMRMediaBuffer::EVideoH263;
		    containVideo = ETrue;
		    iFourCC = TFourCC( KMccFourCCIdH263 );

            TVideoClipProperties clipProperties;
        	MP4ParseGetVideoClipProperties( aMP4Handle, clipProperties );
        	iLevel = clipProperties.iH263Level;
		    }
		else if ( iType == MP4_TYPE_MPEG4_VIDEO )
		    {
		    __FILESOURCE_CONTROLL( "CMccFileVideo::ParseUpdateVideoDescriptions \
MP4_TYPE_MPEG4_VIDEO!" )

		    iBufferType = CCMRMediaBuffer::EVideoH263;
		    containVideo = ETrue;
		    iFourCC = TFourCC( KMccFourCCIdAVC );
		    }
		else if ( IsAVC() )
		    {
		    __FILESOURCE_CONTROLL( "CMccFileVideo::\
ParseUpdateVideoDescriptions AVC!" )

		    iBufferType = CCMRMediaBuffer::EVideoH263;
		    containVideo = ETrue;
		    iFourCC = TFourCC( KMccFourCCIdAVC );
		    iIsFirstFrame = ETrue;
		    iInsertParameterSets = ETrue;
		    }
		else if ( iType != MP4_TYPE_NONE )
		    {
		    __FILESOURCE_CONTROLL( "CMccFileVideo::\
ParseUpdateVideoDescriptions contains unsupported video" )
		    iFourCC = TFourCC( KMccFourCCUnsupported );
		    }
		else
		    {
		    // NOP
		    }
		}
        
	return containVideo;	
	}

// -----------------------------------------------------------------------------
// CMccFileVideo::IsAVC
// -----------------------------------------------------------------------------
//
TBool CMccFileVideo::IsAVC()
	{
	if ( ( iType == MP4_TYPE_AVC_PROFILE_BASELINE ) ||
         ( iType == MP4_TYPE_AVC_PROFILE_MAIN )	 ||
         ( iType == MP4_TYPE_AVC_PROFILE_EXTENDED )
       )
	    {
	   	return ETrue;
	    }
    
    return EFalse;	
	}
	
// -----------------------------------------------------------------------------
// CMccFileVideo::DoSetPositionL
// -----------------------------------------------------------------------------
//
void CMccFileVideo::DoSetPositionL( TUint32 aPosition )
	{
	__FILESOURCE_CONTROLL( "CMccFileVideo::DoSetPositionL" )

    __ASSERT_ALWAYS( aPosition <= iLength, User::Leave( KErrArgument ) );
    mp4_u32 audioPos = 0;
    // if retrun value is MP4_OK, iPosition is updated
    if ( MP4ParseSeek( iMP4Handle, ( mp4_u32 ) aPosition,
         ( mp4_u32* )&audioPos, ( mp4_u32* ) &iPosition, MP4TRUE )
        != MP4_OK )
	    {
	   	User::Leave( KErrGeneral );
	    }
	}

// -----------------------------------------------------------------------------
// CMccFileVideo::Type
// -----------------------------------------------------------------------------
//
CCMRMediaBuffer::TBufferType CMccFileVideo::Type()
	{
	return iBufferType;	
	}

// -----------------------------------------------------------------------------
// CMccFileVideo::GetFourCC
// -----------------------------------------------------------------------------
//
TFourCC CMccFileVideo::GetFourCC()
	{
	return iFourCC;
	}

// -----------------------------------------------------------------------------
// CMccFileVideo::TickCallBack
// -----------------------------------------------------------------------------
//
TCallBack CMccFileVideo::TickCallBack()
    {
    return TCallBack( TickVideoL, this );
    }

// -----------------------------------------------------------------------------
// CMccFileVideo::GetConfigKeyL
// -----------------------------------------------------------------------------
//
HBufC8* CMccFileVideo::GetConfigKeyL()
    {
    __ASSERT_ALWAYS( IsAVC(), User::Leave( KErrNotSupported ) );
    HBufC8* configKey = NULL;
    ExtractH264ParameterSetNALUsL( &configKey, ETrue );
    return configKey;
    }

// -----------------------------------------------------------------------------
// CMccFileVideo::VideoType
// -----------------------------------------------------------------------------
//
TUint32 CMccFileVideo::VideoType() const
    {
    return iType;
    }

// -----------------------------------------------------------------------------
// CMccFileVideo::Position
// -----------------------------------------------------------------------------
//
TUint32 CMccFileVideo::Position()
	{
	return iPosition;
	}
	
// -----------------------------------------------------------------------------
// CMccFileVideo::DurationL
// -----------------------------------------------------------------------------
//
TUint32 CMccFileVideo::Duration()
	{
	return iLength;	
	}
	
// -----------------------------------------------------------------------------
// CMccFileVideo::AudioBitRateL
// -----------------------------------------------------------------------------
//
TUint32 CMccFileVideo::AudioBitRate()
	{
    return 0;
	}


// -----------------------------------------------------------------------------
// CMccFileVideo::VideoFrameSizeL
// -----------------------------------------------------------------------------
//
TSize CMccFileVideo::VideoFrameSize()
	{
	TSize size;
	size.iWidth = iWidth;
	size. iHeight = iHeight;
	
	return size;	
	}

// -----------------------------------------------------------------------------
// CMccFileVideo::VideoFrameRateL
// -----------------------------------------------------------------------------
//
TReal CMccFileVideo::VideoFrameRateL()
	{
    TReal modFrameRate;
    MccConversionUtility::FrameRateSanitizeL( modFrameRate, iFrameRate, iType );
    return modFrameRate;
	}
 
// -----------------------------------------------------------------------------
// CMccFileVideo::StartTimerL
// -----------------------------------------------------------------------------
//                     
void CMccFileVideo::StartTimerL()        
	{
	__FILESOURCE_CONTROLL( "CMccFileVideo::StartTimerL" )

	if ( IsPaused() )
	    {
	    SetPaused( EFalse );
	    
	    SetPositionL( iPosition, ETrue );
	    }
	else
	    {
	    SetStartTime();
	    }
	    
	const TReal KMccSecondInMicroSeconds = 1000000;
	TInt tickIntervalVideo = (TInt)KMccSecondInMicroSeconds;
    if ( iFrameRate != 0 )
        {
        // Set interval for video
        tickIntervalVideo = (TInt)( KMccSecondInMicroSeconds / iFrameRate ); 
        
        // Increase tick rate 22%
        const TReal KMccTickModifier = 0.78;
        tickIntervalVideo =(TInt) (tickIntervalVideo * KMccTickModifier);
        }
	
	__FILESOURCE_CONTROLL_INT1( "CMccFileVideo::StartTimer (timer):", 
                                tickIntervalVideo )
    
    iTimerInterval = tickIntervalVideo;
    iCorrectionInterval = iTimerInterval;
                            
	iPeriodicRunner->Start( tickIntervalVideo, TCallBack( TickVideoL, this ));	
	
	__FILESOURCE_CONTROLL( "CMccFileVideo::StartTimerL, exit" )
	}

// -----------------------------------------------------------------------------
// CMccFileVideo::TickVideoL
// -----------------------------------------------------------------------------
//
TInt CMccFileVideo::TickVideoL( TAny* aObject )
	{
	__FILESOURCE_CONTROLL( "CMccFileVideo::TickVideoL" )
	
	static_cast<CMccFileVideo*>(aObject)->ReadFrameL();
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CMccFileVideo::ReadFrameL
// -----------------------------------------------------------------------------
// 
void CMccFileVideo::ReadFrameL()
	{
	__FILESOURCE_CONTROLL( "CMccFileVideo::ReadFrameL" )
	
        
    MP4Err retval = MP4ParseIsFrameAvailable( iMP4Handle, iType );
    
  	if (  retval != MP4_OK )
	  	{
	    __FILESOURCE_CONTROLL_INT1( "CMccFileVideo::ReadFrameL, no video", retval )

		SetPaused( ETrue );
		TRAPD( positionErr, SetPositionL( 0 ) );
		iSource.PauseVideoL( positionErr );	
        return;
	  	}
    
    // Read the size of next video frame
  	__FILESOURCE_CONTROLL( "CMccFileVideo::ReadFrameL, reading frame" ) 
   
    if ( MP4ParseNextFrameSize( iMP4Handle, iType,
        ( mp4_u32* ) &iBufferSize ) != MP4_OK )
	    {
	    __FILESOURCE_CONTROLL( "CMccFileVideo::ReadFrameL, \
MP4ParseNextFrameSize not Ok" ) 
        return;
	    }
    
    TInt aVCNALUsBufferLen = 0;
    if ( IsAVC() && iIsFirstFrame )
    	{
    	delete iAVCNALUsBuffer;
	    iAVCNALUsBuffer = NULL;	

    	ExtractH264ParameterSetNALUsL( &iAVCNALUsBuffer );
    	if( iAVCNALUsBuffer )
	    	{
	    	aVCNALUsBufferLen += iAVCNALUsBuffer->Length();	
	    	}
    	iIsFirstFrame = EFalse;
    	}
    	
    // Allocate a buffer big enough for the next video frame
  	__FILESOURCE_CONTROLL_INT1( "CMccFileVideo::ReadFrameL, reading frame, size", iBufferSize )
    HBufC8* mediaBuffer = HBufC8::NewLC( iBufferSize + aVCNALUsBufferLen );
    TPtr8 mediaSinkBuffer = mediaBuffer->Des();
    mediaSinkBuffer.SetLength( iBufferSize + aVCNALUsBufferLen );
    TUint8* ptrMediaBuffer = CONST_CAST(TUint8*, mediaBuffer->Ptr());

    TBool keyFrame = EFalse;
    
    TUint32 oldPosition = iPosition;
    if ( MP4ParseReadVideoFrame( iMP4Handle,
        ( mp4_u8* ) ptrMediaBuffer, ( mp4_u32 ) iBufferSize,
        ( mp4_u32* ) &iFrameSize, ( mp4_u32* ) &iPosition,
        ( mp4_bool* ) &keyFrame, NULL ) != MP4_OK )
		{
		__FILESOURCE_CONTROLL( "CMccFileVideo::ReadFrameL, reading frame, NOT OK" )
		CleanupStack::PopAndDestroy( mediaBuffer );
	  	if( !iSendReadFrameFailEvent )
	  	    {
	  	    iReadFrameFailCount++;
	  	    if( iReadFrameFailCount >= KMccReadFrameFailCountMax )
	  	    	{
	  	    	iSendReadFrameFailEvent = ETrue;
	  	    	iSource.InvalidVideoFrame( ETrue );
	  	    	}
	  	    }
        }
    else
    	{
 		__FILESOURCE_CONTROLL_INT1( "CMccFileVideo::ReadFrameL, reading frame, OK, position", 
 		                            iPosition )   		
 		                            
	    if( iSendReadFrameFailEvent )
		    {
		    iSendReadFrameFailEvent = EFalse;
		    iReadFrameFailCount = 0;
		    iSource.InvalidVideoFrame( EFalse );
		    }

        DoTimingCorrection( iPosition );
               
        // Update read interval (how much position changes on each frame read)
        iReadInterval = iPosition - oldPosition;
               
        if ( IsAVC() )
	        {
	        MarkWithNALUDelimiters( iBufferSize, mediaSinkBuffer );
	    	}

        TTimeIntervalMicroSeconds timeStamp = GetTimeStamp();
        
        CMccFrameItem* item = new (ELeave) CMccFrameItem();
        item->iFrame = mediaBuffer;
        CleanupStack::Pop( mediaBuffer );
        CleanupStack::PushL( item );
        item->iTimeStamp = timeStamp;
        item->iKeyFrame = keyFrame;
        iFrames.AppendL( item );
        CleanupStack::Pop( item );
        
        WriteBufferL();
		}
    
    __FILESOURCE_CONTROLL( "CMccFileVideo::ReadFrameL, Time Check 2" )    		

  	__FILESOURCE_CONTROLL( "CMccFileVideo::ReadFrameL, \
writing buffer to sink complete" ) 		
	}

// -----------------------------------------------------------------------------
// CMccFileVideo::MarkWithNALUDelimiters()
// -----------------------------------------------------------------------------
//
void CMccFileVideo::MarkWithNALUDelimiters( 
	const TInt aAccessUnitSize, TDes8& aBuffer )
	{
    __FILESOURCE_CONTROLL( "CMccFileVideo::MarkWithNALUDelimiters" )
    
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
// CMccFileVideo::ExtractH264ParameterSetNALUsL()
// -----------------------------------------------------------------------------
//
void CMccFileVideo::ExtractH264ParameterSetNALUsL( 
    HBufC8** aBufferOut, 
    TBool aSdpFormat )
	{
	__FILESOURCE_CONTROLL( "CMccFileVideo::ExtractH264ParameterSetNALUsL" )
	
	*aBufferOut = NULL;
	
	HBufC8* buffer = NULL;
	HBufC8* bufferOut = NULL;
	TUint32 decspecinfosize;
	TUint8* ptrByte;
	
	// Query for size, buffer too small error is ok as we give zero length buffer
	MP4Err retval = MP4ParseReadVideoDecoderSpecificInfo( 
	    iMP4Handle, ( mp4_u8* ) NULL, 0, ( mp4_u32* ) &decspecinfosize );
	    
	if ( retval != MP4_OK && retval != MP4_BUFFER_TOO_SMALL )
		{
		User::Leave(KErrGeneral);
		}
	
	buffer = HBufC8::NewLC(decspecinfosize);
	ptrByte = const_cast<TUint8*> ( buffer->Des().Ptr() );
	
	if( MP4ParseReadVideoDecoderSpecificInfo( iMP4Handle,
											  ( mp4_u8* ) ptrByte,
											  ( mp4_u32 ) buffer->Des().MaxSize(),
											  ( mp4_u32* ) &decspecinfosize
											 ) != MP4_OK )
		{
		User::Leave(KErrGeneral);
		}
	else
		{
		buffer->Des().SetLength( decspecinfosize );
		}
	    
	const TUint two = 2;
	bufferOut = HBufC8::NewLC( buffer->Size() * two );
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
		
		if ( aSdpFormat )
		    {
		    // TBD: take possibility of multiple nalus into account
		    HBufC8* base64Encoded = 
		        MccConversionUtility::Base64EncodeLC( ptrBuffer.Mid( indx, size16 ) ); 
		    ptrBufferOut.Append( *base64Encoded );
		    CleanupStack::PopAndDestroy( base64Encoded );
    		}
        else
            {
            // insert delimiter
    		tmpByte = 0x00;
    		ptrBufferOut.Append( &tmpByte, 1 );
    		ptrBufferOut.Append( &tmpByte, 1 );
    		ptrBufferOut.Append( &tmpByte, 1 );
    		tmpByte = 0x01;
    		ptrBufferOut.Append( &tmpByte, 1 );

    		ptrBufferOut.Append( ptrBuffer.Mid( indx, size16 ) );
	    	}
	    	
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
		
		if ( aSdpFormat )
		    {
		    // TBD: take possibility of multiple nalus into account
		    _LIT8( KMccFileSinkNaluSeparator, "," );
		    ptrBufferOut.Append( KMccFileSinkNaluSeparator );
		    HBufC8* base64Encoded = 
		        MccConversionUtility::Base64EncodeLC( ptrBuffer.Mid( indx, size16 ) ); 
		    ptrBufferOut.Append( *base64Encoded );
		    CleanupStack::PopAndDestroy( base64Encoded );
    		}
        else
            {
    		// insert delimiter
    		tmpByte = 0x00;
    		ptrBufferOut.Append( &tmpByte, 1 );
    		ptrBufferOut.Append( &tmpByte, 1 );
    		ptrBufferOut.Append( &tmpByte, 1 );
    		tmpByte = 0x01;
    		ptrBufferOut.Append( &tmpByte, 1 );
    		
    		ptrBufferOut.Append( ptrBuffer.Mid( indx, size16 ) );
            }
		
		indx  += size16;
		}
	
	CleanupStack::Pop( bufferOut );	
	CleanupStack::PopAndDestroy( buffer );
	
	*aBufferOut = bufferOut;
	
	__FILESOURCE_CONTROLL( "CMccFileVideo::ExtractH264ParameterSetNALUsL, exit" )
	}

// -----------------------------------------------------------------------------
// CMccFileVideo::InsertParameterSetNALUs
// -----------------------------------------------------------------------------
//
TBool CMccFileVideo::InsertParameterSetNALUs()
    {
    TBool insertParameterSets( iInsertParameterSets );
    if ( insertParameterSets )
        {
        iInsertParameterSets = EFalse;
        iInsertParameterSetsTime.HomeTime();
        }
    else
        {
        TTime currentTime;
        currentTime.HomeTime();
        if ( currentTime.MicroSecondsFrom( iInsertParameterSetsTime ).Int64() > 
                KMccFileSourceParameterSetsIntervalInMicroSeconds )
            {
            // Enough time elapsed from previous parameter set NALUs,
            // insert them at next round.
            iInsertParameterSets = ETrue;
            }
        }
    return insertParameterSets;
    }

// -----------------------------------------------------------------------------
// CMccFileVideo::GetTimeStamp
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CMccFileVideo::GetTimeStamp()
	{
    TInt64 position = iPosition + iPositionModifier;
    TUint32 position2 = ( position < 0 ) ? 0 : (TUint32)position;
	TTimeIntervalMicroSeconds timeStamp = 
	    TTimeIntervalMicroSeconds ( (TInt64) position2 * KMccMicroToMilliConst );
   	
   	__FILESOURCE_CONTROLL_INT1( "CMccFileVideo::GetTimeStamp timestamp (without clock frequency):", 
                                timeStamp.Int64() )
   	return timeStamp;
	}

// -----------------------------------------------------------------------------
// CMccFileVideo::WriteBufferL
// If AVC NALUS buffer exists, it is passed always before keyframe and it uses
// timestamp of that frame. It is also passed always at beginning even if first
// frame wouldn't be keyframe.
// -----------------------------------------------------------------------------
//
void CMccFileVideo::WriteBufferL()
    {
    __FILESOURCE_CONTROLL("CMccFileVideo::WriteBufferL")
    
    if ( iFrames.Count() && iConsumer && iConsumerBuffer )
	    {
	    TBool removeCurrentFrame( ETrue );
	    
        CMMFDataBuffer* buf = static_cast<CMMFDataBuffer*>(iConsumerBuffer); 		

        CMccFrameItem* item = iFrames[0];
        HBufC8* videoFrame = item->iFrame;
        
        if ( iAVCNALUsBuffer && InsertParameterSetNALUs() )
            {
            videoFrame = iAVCNALUsBuffer;
            removeCurrentFrame = EFalse;
            }
        
	    if ( buf->Data().MaxLength() >= videoFrame->Des().Length() )
		    {   
		    buf->Data().Delete( 0, buf->BufferSize() );
            	
			buf->Data().Append( *videoFrame );	
			
			iConsumerBuffer->SetTimeToPlay(item->iTimeStamp);
			iConsumerBuffer->SetLastBuffer(EFalse);
			iConsumerBuffer->SetFrameNumber( iSequenceNum );
			
			if ( removeCurrentFrame )
			    {
		        if ( iMccResources && item->iKeyFrame )
		            {
		            iMccResources->StoreKeyFrameInfoL( iEndpointId, *iConsumerBuffer );
		            }
		         
			    delete item;
    			iFrames.Remove(0);
			    }

			videoFrame = NULL;
		
			iConsumer->BufferFilledL( iConsumerBuffer );
			iConsumer = NULL;
			iConsumerBuffer = NULL;
			iSequenceNum++;
			__FILESOURCE_CONTROLL("CMccFileVideo::WriteBufferL, done")
		    }
		else
			{
			__FILESOURCE_CONTROLL("CMccFileVideo::WriteBufferL, drop frame")
			delete iFrames[0];
			iFrames.Remove(0);
			videoFrame = NULL;
			}
	    }
	else
	    {
	    __FILESOURCE_CONTROLL("CMccFileVideo::WriteBufferL, writing ignored")
	    }
    }


