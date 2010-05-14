/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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


#include <CCMRMediaRecorder.h>
#include <CCMRMediaSink.h>
#include <mmf/devvideo/devvideobase.h>


static TInt MccTestRecorderBehavior = KMccTestRecorderDefaultBehavior;
static TInt MccTestRecorderBehaviorError = KErrNone;

CCMRMediaRecorder* CCMRMediaRecorder::NewL()
    {
    CCMRMediaRecorderStub* stub = new ( ELeave ) CCMRMediaRecorderStub;
    return stub;
    }


        
CCMRMediaRecorderStub::CCMRMediaRecorderStub()
    {
    iCurrentState = CCMRMediaRecorderStub::EStateNone;
    iVideoBitrate = 64000;
    iEncapsulation = EDuGenericPayload;
    }

CCMRMediaRecorderStub::~CCMRMediaRecorderStub()
    {
    }
        
void CCMRMediaRecorderStub::OpenL(MCMRMediaRecorderObserver *aObserver, 
                       MDataSource* aAudioSource, 
                       MCMRMediaSink *aSink, 
                       TInt aCameraHandle,
                       const TDesC8& aVideoMimeType ,
                       TFourCC aAudioType )
    {
    SetCurrentState( CCMRMediaRecorder::EStateOpen );
    iObserver = aObserver;
    iSink = aSink;
    }

CCMRMediaRecorderStub::TRecorderState CCMRMediaRecorderStub::State()
    {
    return iCurrentState;
    }
    
void CCMRMediaRecorderStub::SetVideoCodecL( const TDesC8& aVideoMimeType )
    {
    }

void CCMRMediaRecorderStub::GetVideoCodecL( TDes8& aVideoMimeType )
    {
    }

void CCMRMediaRecorderStub::GetSupportedVideoCodecsL( CDesC8Array& aVideoMimeTypes )
    {
    }

void CCMRMediaRecorderStub::SetAudioCodecL( const TFourCC& aAudioType )
    {
    }

void CCMRMediaRecorderStub::GetAudioCodecL( TFourCC& aAudioType )
    {
    }

void CCMRMediaRecorderStub::GetSupportedAudioCodecsL( RArray<TFourCC>& aAudioTypes )
    {
    }

void CCMRMediaRecorderStub::SetMaxTotalBitRateL(TInt aBitRate)
    {
    }

TInt CCMRMediaRecorderStub::MaxTotalBitRateL() const
    {
    return 128000;
    }

void CCMRMediaRecorderStub::SetVideoFrameSizeL(const TSize& aSize)
    {
    }

void CCMRMediaRecorderStub::GetVideoFrameSizeL(TSize& aSize) const
    {
    }

void CCMRMediaRecorderStub::SetVideoFrameRateL(TReal32 aFrameRate)
    {
    }

TReal32 CCMRMediaRecorderStub::VideoFrameRateL()
    {
    return 15;
    }        

void CCMRMediaRecorderStub::SetVideoBitRateL(TInt aBitRate)
    {
    iVideoBitrate = aBitRate;
    }

TInt CCMRMediaRecorderStub::VideoBitRateL()
    {
    return iVideoBitrate;
    }

void CCMRMediaRecorderStub::SetVideoCodingOptionsL(const TCCMRVideoCodingOptions& aOptions)
    {
    }

void CCMRMediaRecorderStub::SetAudioBitRateL(TInt aBitRate)
    {
    }

TInt CCMRMediaRecorderStub::AudioBitRateL()
    {
    return 12200;
    }

void CCMRMediaRecorderStub::SetAudioEnabledL(TBool aEnabled)
    {
    }

TBool CCMRMediaRecorderStub::AudioEnabledL() const
    {
    return EFalse;
    }
    
void CCMRMediaRecorderStub::SetGainL(TInt aGain)
    {
    }

TInt CCMRMediaRecorderStub::GainL() const
    {
    return 50;
    }

TInt CCMRMediaRecorderStub::MaxGainL() const
    {
    return 100;
    }   
    
void CCMRMediaRecorderStub::SetAudioPriorityL(const TMMFPrioritySettings& aPrioritySettings)
    {
    }

void CCMRMediaRecorderStub::PrepareL()
    {
    iLastFunctionCall = ETestPrepare;
    
    if ( MccTestRecorderBehavior == KMccTestRecorderKeyRetrievalBehavior && iObserver )
        {
        SetCurrentState( CCMRMediaRecorder::EStateReadyToRecord );
        iObserver->MmroPrepareComplete( MccTestRecorderBehaviorError );
        }
    }

void CCMRMediaRecorderStub::RecordL()
    {
    iLastFunctionCall = ETestRecord;
    if ( MccTestRecorderBehavior == KMccTestRecorderKeyRetrievalBehavior && iSink )
        {
        // Make two part configkey (delimiters are important, other data does not matter )
        TBuf8<80> confKey;
        confKey.SetLength( 30 );
        confKey[ 0 ] = 0x00;
        confKey[ 1 ] = 0x00;
        confKey[ 2 ] = 0x01;
        confKey[ 25 ] = 0x00;
        confKey[ 26 ] = 0x00;
        confKey[ 27 ] = 0x01;
        
        TTimeIntervalMicroSeconds timestamp = 0;
        CCMRMediaBuffer* buf = 
            new ( ELeave ) CCMRMediaBuffer( confKey, 
                                            CCMRMediaBuffer::EVideoH264BytestreamDecSpecInfo, 
                                            confKey.Length(), 
                                            EFalse, 
                                            timestamp );
        CleanupStack::PushL( buf );
        iSink->WriteBufferL( buf );
        CleanupStack::PopAndDestroy( buf );
        }
    }

void CCMRMediaRecorderStub::StopL()
    {
    iLastFunctionCall = ETestStop;
    }

void CCMRMediaRecorderStub::PauseL()
    {
    iLastFunctionCall = ETestPause;
    }

void CCMRMediaRecorderStub::ResumeL()
    {
    iLastFunctionCall = ETestResume;
    }

void CCMRMediaRecorderStub::SetVideoRateControlOptionsL(const TRateControlOptions& aOptions)
    {
    iLastFunctionCall = ETestSetRateControl;
    iRateControlOptions = aOptions;
    }
        
void CCMRMediaRecorderStub::GetVideoRateControlOptionsL(TRateControlOptions& aOptions)
    {
    aOptions.iQualityTemporalTradeoff = 0.8;
    iLastFunctionCall = ETestRateControl;
    }

void CCMRMediaRecorderStub::SetPreferredVideoEncoderL(TUid& aEncoder)
    {
    iLastFunctionCall = ETestPreferredEncoder;
    iEncoderUid = aEncoder;
    }

void CCMRMediaRecorderStub::SetPreferredVideoEncapsulationL(TVideoDataUnitEncapsulation aCapsulation)
    {
    iLastFunctionCall = ETestPreferredEncapsulation;
    iEncapsulation = aCapsulation;
    }
        
// STUB STUFF

void CCMRMediaRecorderStub::SetCurrentState( TRecorderState aState )
    {
    iCurrentState = aState;
    }
    
void CCMRMediaRecorderStub::SetBehavior( TInt aBehavior, TInt aError )
    {
    MccTestRecorderBehavior = aBehavior;
    MccTestRecorderBehaviorError = aError;
    }


