/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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




//  CLASS HEADER
#include "UT_CMccFileAudio.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "MccFileAudio.h"
#include "MccFileVideo.h"
#include "MccFileSourceImpl.h"
#include "mmccinterfacedef.h"
#include "mccresourcepool.h"
#include "Mcculdatapath.h"
#include "mccinternaldef.h"

// CONSTRUCTION
UT_CMccFileAudio* UT_CMccFileAudio::NewL()
    {
    UT_CMccFileAudio* self = UT_CMccFileAudio::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccFileAudio* UT_CMccFileAudio::NewLC()
    {
    UT_CMccFileAudio* self = new( ELeave ) UT_CMccFileAudio();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccFileAudio::~UT_CMccFileAudio()
    {
    }

// Default constructor
UT_CMccFileAudio::UT_CMccFileAudio()
    {
    }

// Second phase construct
void UT_CMccFileAudio::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccFileAudio::Setup2L(  )
    {
    iBuffer = CMMFDescriptorBuffer::NewL( 1000 );
    iBuffer->Data().Copy(_L("aaaaaaaaaa"));
    TMediaId media = KUidMediaTypeAudio;

    MAsyncEventHandler* eventHandler = NULL;
    CMccResourcePool* iMccResource = NULL;

    iAudioULDPStub = CMccUlDataPath::NewL( eventHandler,iMccResource,  media );
    media = KUidMediaTypeVideo;
    iVideoULDPStub = CMccUlDataPath::NewL( eventHandler, iMccResource, media );
    
    TMccFileSourceSettingBuf settingBuf;
    
    iFileSourceImpl = CMccFileSourceImpl::NewL( 0 );
    
	iAudio = CMccFileAudio::NewL(*iFileSourceImpl);
	iVideo = CMccFileVideo::NewL(*iFileSourceImpl);
	
#define KFileName TFileName(_L("c:\\test.mp4"));  
    TFileName fileName = KFileName;
    TFileName fullFileName;
    MP4Err error = MP4_OK;
    
    RFs fS;
    User::LeaveIfError( fS.Connect() );
	CleanupClosePushL( fS );
    TParse fp;	
    if ( fS.Parse( fileName, fp ) != KErrNone )
        {
        RDebug::Print(_L("CMccFileSourceImpl::OpenFileL file name parse error!"));
	    User::Leave( KErrGeneral );
        }
	
    fullFileName = fp.FullName();

  	CleanupStack::PopAndDestroy( &fS ); 
  	
  	if (MP4ParseOpen( &iMP4Handle, (wchar_t *) fullFileName.PtrZ() ) != MP4_OK )
	  	{
	  	iMP4Handle = NULL;
	  	User::Leave(KErrGeneral);	
	  	}	
	  	
	iAudio->ParseUpdateAudioDescriptions(iMP4Handle);
	iVideo->ParseUpdateVideoDescriptions(iMP4Handle);
	iAudio->ParseUpdateStreamDescription(iMP4Handle);
	iAudio->SetFileHandle(iMP4Handle);
	iVideo->SetFileHandle(iMP4Handle);
    }

void UT_CMccFileAudio::SetupL(  )
    {
    TMccFileSourceSettingBuf settingBuf;
   
    iBuffer = CMMFDescriptorBuffer::NewL( 1000 );
    iBuffer->Data().Copy(_L("aaaaaaaaaa"));
    TMediaId media = KUidMediaTypeAudio;

    MAsyncEventHandler* eventHandler = NULL;
    CMccResourcePool* iMccResource = NULL;

    iAudioULDPStub = CMccUlDataPath::NewL( eventHandler,iMccResource,  media );
    media = KUidMediaTypeVideo;
    iVideoULDPStub = CMccUlDataPath::NewL( eventHandler, iMccResource, media );
    
    iFileSourceImpl = CMccFileSourceImpl::NewL( 0 );
	iAudio = CMccFileAudio::NewL(*iFileSourceImpl);
	iVideo = CMccFileVideo::NewL(*iFileSourceImpl);
	
#define KFileName2 TFileName(_L("c:\\CrazyFrog.3gp"));  
    TFileName fileName = KFileName2;
    TFileName fullFileName;
    MP4Err error = MP4_OK;
    
    RFs fS;
    User::LeaveIfError( fS.Connect() );
	CleanupClosePushL( fS );
    TParse fp;	
    if ( fS.Parse( fileName, fp ) != KErrNone )
        {
        RDebug::Print(_L("CMccFileSourceImpl::OpenFileL file name parse error!"));
	    User::Leave( KErrGeneral );
        }
	
    fullFileName = fp.FullName();

  	CleanupStack::PopAndDestroy( &fS ); 
  	
  	if (MP4ParseOpen( &iMP4Handle, (wchar_t *) fullFileName.PtrZ() ) != MP4_OK )
	  	{
	  	iMP4Handle = NULL;
	  	User::Leave(KErrGeneral);	
	  	}	
	  	
	iAudio->ParseUpdateAudioDescriptions(iMP4Handle);
	iVideo->ParseUpdateVideoDescriptions(iMP4Handle);
	iAudio->ParseUpdateStreamDescription(iMP4Handle);
	iAudio->SetFileHandle(iMP4Handle);
	iVideo->SetFileHandle(iMP4Handle);
    }

void UT_CMccFileAudio::Setup3L(  )
    {
    TMccFileSourceSettingBuf settingBuf;
   
    iBuffer = CMMFDescriptorBuffer::NewL( 1000 );
    iBuffer->Data().Copy(_L("aaaaaaaaaa"));
    TMediaId media = KUidMediaTypeAudio;

    MAsyncEventHandler* eventHandler = NULL;
    CMccResourcePool* iMccResource = NULL;

    iAudioULDPStub = CMccUlDataPath::NewL( eventHandler,iMccResource,  media );
    media = KUidMediaTypeVideo;
    iVideoULDPStub = CMccUlDataPath::NewL( eventHandler, iMccResource, media );
    
    iFileSourceImpl = CMccFileSourceImpl::NewL( 0 );
	iVideo = CMccFileVideo::NewL(*iFileSourceImpl);
	iAudio = NULL;
	
#define KFileName3 TFileName(_L("c:\\test_avc.3gp"));  
    TFileName fileName = KFileName3;
    TFileName fullFileName;
    MP4Err error = MP4_OK;
    
    RFs fS;
    User::LeaveIfError( fS.Connect() );
	CleanupClosePushL( fS );
    TParse fp;	
    if ( fS.Parse( fileName, fp ) != KErrNone )
        {
        RDebug::Print(_L("CMccFileSourceImpl::OpenFileL file name parse error!"));
	    User::Leave( KErrGeneral );
        }
	
    fullFileName = fp.FullName();

  	CleanupStack::PopAndDestroy( &fS ); 
  	
  	if (MP4ParseOpen( &iMP4Handle, (wchar_t *) fullFileName.PtrZ() ) != MP4_OK )
	  	{
	  	iMP4Handle = NULL;
	  	User::Leave(KErrGeneral);	
	  	}	
	  	;
	iVideo->ParseUpdateVideoDescriptions(iMP4Handle);
	iVideo->SetFileHandle(iMP4Handle);
    }
       
void UT_CMccFileAudio::Teardown(  )
    {
    if ( iMP4Handle )
	    {
	    MP4Err error = MP4ParseClose( iMP4Handle );
	    iMP4Handle = NULL;
	    }
	
	delete iVideo; 
	delete iAudio;
    delete iFileSourceImpl;
    delete iVideoULDPStub;
    delete iAudioULDPStub;
    delete iBuffer;
    }

void UT_CMccFileAudio::UT_CMccFileAudio_NewLL(  )
    {
    EUNIT_ASSERT( ETrue );
    }

void UT_CMccFileAudio::UT_CMccFileAudio_SetPositionLL(  )
    {
    iAudio->SetFileHandle(NULL);
  	iAudio->SetPositionL(100);
  	EUNIT_ASSERT_EQUALS( iAudio->Position(), 100 );
  	
  	iAudio->SetPositionL(200);
  	EUNIT_ASSERT_EQUALS( iAudio->Position(), 200 );

  	// If position is changed while reader is active, position is changed to nearest iframe
	iVideo->SetFileHandle(iMP4Handle);
	iVideo->iIsPaused = EFalse;
  	iVideo->SetPositionL(50);
  	EUNIT_ASSERT( iVideo->Position() != 50 );
  	
  	// If position setting is called just because resuming (pos not changed while paused),
  	// position is exactly the one which was when pause occured
  	iVideo->iPosition = 50;
  	iVideo->SetPositionL(50, ETrue);
  	EUNIT_ASSERT_EQUALS( iVideo->Position(), 50 );
  	
  	// If position was changed while paused, position is changed to nearest iframe
  	iVideo->iIsPaused = ETrue;
  	iVideo->SetPositionL(100);
  	EUNIT_ASSERT_EQUALS( iVideo->Position(), 100 );
  	iVideo->iIsPaused = EFalse;
  	iVideo->SetPositionL(100, ETrue);
  	EUNIT_ASSERT( iVideo->Position() != 100 );
  	
  	// If position is tried to be set over the end, end pos is used
  	iVideo->iIsPaused = EFalse;
  	TUint32 duration = iVideo->Duration();
  	iVideo->SetPositionL(duration + 1000, EFalse);
  	EUNIT_ASSERT( iVideo->Position() <= duration );
    }

void UT_CMccFileAudio::UT_CMccFileAudio_TypeL(  )
    {
    CCMRMediaBuffer::TBufferType bufferType1 = iAudio->Type();
    CCMRMediaBuffer::TBufferType bufferType2 = iVideo->Type();
    }

void UT_CMccFileAudio::UT_CMccFileAudio_PositionLL(  )
    {
    TUint32 pos = iAudio->Position();
    TUint32 pos2 = iVideo->Position();
    }

void UT_CMccFileAudio::UT_CMccFileAudio_DurationLL(  )
    {
    TUint32 duration = iAudio->Duration();
    TUint32 duration2 = iVideo->Duration();
    }

void UT_CMccFileAudio::UT_CMccFileAudio_AudioBitRateLL(  )
    {
    TUint32 audioBitRate = iAudio->AudioBitRate();
    TUint32 audioBitRate2 = iVideo->AudioBitRate();
    }

void UT_CMccFileAudio::UT_CMccFileAudio_VideoFrameSizeLL(  )
    {
    TSize size = iAudio->VideoFrameSize();
    TSize size2 = iVideo->VideoFrameSize();
    }

void UT_CMccFileAudio::UT_CMccFileAudio_VideoFrameRateLL(  )
    {
    TReal videoFrameRate = iAudio->VideoFrameRateL();
    EUNIT_ASSERT_EQUALS( (TInt)videoFrameRate, 0 );
    TReal videoFrameRate2 = iVideo->VideoFrameRateL();
    EUNIT_ASSERT_EQUALS( (TInt)videoFrameRate2, 5 );
    
    // Test that framerate is limited to more interoperable value if value is too high for codec type
    iVideo->iType = MP4_TYPE_H263_PROFILE_0;
    iVideo->iFrameRate = 21;
    videoFrameRate2 = iVideo->VideoFrameRateL();
    EUNIT_ASSERT_EQUALS( (TInt)videoFrameRate2, KMccH263ProfileZeroMaxFramerateIOP );
    
    iVideo->iType = MP4_TYPE_AVC_PROFILE_BASELINE;
    iVideo->iFrameRate = 18;
    videoFrameRate2 = iVideo->VideoFrameRateL();
    EUNIT_ASSERT_EQUALS( (TInt)videoFrameRate2, 18 ); // Value not changed for AVC
    }

void UT_CMccFileAudio::UT_CMccFileAudio_StartTimerL(  )
    {
    // Audio
    iAudio->StartTimerL();
    EUNIT_ASSERT( !iAudio->iIsPaused );
    TInt posMod = iAudio->iPositionModifier;
    iAudio->StopTimer();
    EUNIT_ASSERT( iAudio->iIsPaused );
    
    User::After( 1000 );
    iAudio->StartTimerL();
    EUNIT_ASSERT( posMod != iAudio->iPositionModifier );
    
    // Video
    iVideo->StartTimerL();
    EUNIT_ASSERT( !iVideo->iIsPaused );
    iVideo->StopTimer();
    }

void UT_CMccFileAudio::UT_CMccFileAudio_StopTimerL(  )
    {
    // Audio
    iAudio->StartTimerL();
    EUNIT_ASSERT( !iAudio->iIsPaused );
    iAudio->StopTimer();
    EUNIT_ASSERT( iAudio->iIsPaused );
    
    // Video
    iVideo->StartTimerL();
    EUNIT_ASSERT( !iVideo->iIsPaused );
    iVideo->StopTimer();    
    TTime pauseStarted = iVideo->iPauseStarted;
    EUNIT_ASSERT( iVideo->iIsPaused );
    
    User::After( 1000 );
    iVideo->StopTimer();
    EUNIT_ASSERT( iVideo->iIsPaused );
    EUNIT_ASSERT( pauseStarted == iVideo->iPauseStarted );
    }

void UT_CMccFileAudio::UT_CMccFileAudio_GetFourCCL(  )
    {
    TFourCC fourCC = iAudio->GetFourCC();
    TFourCC fourCC2 = iVideo->GetFourCC();
    }

void UT_CMccFileAudio::UT_CMccFileAudio_ResetTimeL(  )
    {
    iAudio->ResetTimeL();
    iVideo->ResetTimeL();
    }

void UT_CMccFileAudio::UT_CMccFileAudio_TickAudioLL(  )
    {
    iAudio->TickAudioL(iAudio);
    iVideo->TickVideoL(iVideo);
    }

void UT_CMccFileAudio::UT_CMccFileAudio_ReadFrameLL(  )
    {
    // 3GP clip
    TInt audioPos = iAudio->iPosition;
    iAudio->ReadFrameL( EFalse );
    EUNIT_ASSERT( iAudio->iPosition >= audioPos );
    audioPos = iAudio->iPosition;
    
    iAudio->iConsumer = iAudioULDPStub;
	iAudio->iConsumerBuffer = iBuffer;
    iAudio->ReadFrameL( ETrue );
    EUNIT_ASSERT( iAudio->iPosition > audioPos );
    
    TInt videoPos = iVideo->iPosition;
    iVideo->ReadFrameL();
    EUNIT_ASSERT( iVideo->iPosition >= videoPos );
    videoPos = iVideo->iPosition;
    iVideo->iConsumer = iVideoULDPStub;
	iVideo->iConsumerBuffer = iBuffer;
    iVideo->ReadFrameL();
    EUNIT_ASSERT( iVideo->iPosition > videoPos );
    }

void UT_CMccFileAudio::UT_CMccFileAudio_ReadFrameL2L(  )
    {
    // MP4 clip
    TInt audioPos = iAudio->iPosition;
    iAudio->ReadFrameL( EFalse );
    EUNIT_ASSERT( iAudio->iPosition == audioPos ); // No audio
    
    TInt videoPos = iVideo->iPosition;
    iVideo->ReadFrameL();
    EUNIT_ASSERT( iVideo->iPosition >= videoPos );
    videoPos = iVideo->iPosition;
    
    iVideo->ReadFrameL();
    EUNIT_ASSERT( iVideo->iPosition > videoPos );
    }

void UT_CMccFileAudio::UT_CMccFileAudio_ParseUpdateAudioDescriptionsL(  )
    {
    iAudio->ParseUpdateAudioDescriptions(iMP4Handle);
    iVideo->ParseUpdateVideoDescriptions(iMP4Handle);
    }

void UT_CMccFileAudio::UT_CMccFileAudio_GetFrameLengthL()
	{
	TBuf8<3> data;
	data.Append( 42 );
	TUint8 dataVal = dataVal & 0x0;
	data[ 0 ] = dataVal;
	EUNIT_ASSERT_EQUALS( iAudio->GetFrameLength( data ), 13 );
	
	dataVal = dataVal & 0x0 | ( 0x1 << 3 );
	data[ 0 ] = dataVal;
	EUNIT_ASSERT_EQUALS( iAudio->GetFrameLength( data ), 14 );
	
	dataVal = dataVal & 0x0 | ( 0x2 << 3 );
	data[ 0 ] = dataVal;
	EUNIT_ASSERT_EQUALS( iAudio->GetFrameLength( data ), 16 );	
	
	dataVal = dataVal & 0x0 | ( 0x3 << 3 );
	data[ 0 ] = dataVal;
	EUNIT_ASSERT_EQUALS( iAudio->GetFrameLength( data ), 18 );	
	
	dataVal = dataVal & 0x0 | ( 0x4 << 3 );
	data[ 0 ] = dataVal;
	EUNIT_ASSERT_EQUALS( iAudio->GetFrameLength( data ), 20 );	
	
	dataVal = dataVal & 0x0 | ( 0x5 << 3 );
	data[ 0 ] = dataVal;
	EUNIT_ASSERT_EQUALS( iAudio->GetFrameLength( data ), 21 );	
	
	dataVal = dataVal & 0x0 | ( 0x6 << 3 );
	data[ 0 ] = dataVal;
	EUNIT_ASSERT_EQUALS( iAudio->GetFrameLength( data ), 27 );	
	
	dataVal = dataVal & 0x0 | ( 0x7 << 3 );
	data[ 0 ] = dataVal;
	EUNIT_ASSERT_EQUALS( iAudio->GetFrameLength( data ), 32 );	
	
	dataVal = dataVal & 0x0 | ( 0x8 << 3 );
	data[ 0 ] = dataVal;
	EUNIT_ASSERT_EQUALS( iAudio->GetFrameLength( data ), 6 );	
	
	dataVal = dataVal & 0x0 | ( 0xF << 3 );
	data[ 0 ] = dataVal;
	EUNIT_ASSERT_EQUALS( iAudio->GetFrameLength( data ), 1 );	
	
	dataVal = dataVal & 0x0 | ( 0x9 << 3 );
	data[ 0 ] = dataVal;
	EUNIT_ASSERT_EQUALS( iAudio->GetFrameLength( data ), 0 );
	}

void UT_CMccFileAudio::UT_CMccFileAudio_SetStartTimeL()
    {
    static_cast<CMccFileSourceTypeBase*>(iAudio)->SetStartTime();
    }
         
void UT_CMccFileAudio::UT_CMccFileAudio_IsPausedL()
    {
    static_cast<CMccFileSourceTypeBase*>(iAudio)->IsPaused();
    }

void UT_CMccFileAudio::UT_CMccFileSink_GetTimeStampL()
    {
    TTimeIntervalMicroSeconds audioTimestamp = iAudio->GetTimeStamp(100);   
    TTimeIntervalMicroSeconds videoTimestamp = iVideo->GetTimeStamp();
    }

void UT_CMccFileAudio::UT_CMccFileAudio_FillBufferL()
    {
    // Audio
    iAudio->FillBufferL( iBuffer, iAudioULDPStub );
    CMccFrameItem* frame = CreateFrameItemLC( 800 );
    iAudio->iFrames.AppendL( frame );
    CleanupStack::Pop( frame );
    
    iAudio->FillBufferL( iBuffer, iAudioULDPStub );
    EUNIT_ASSERT_EQUALS( iAudio->iFrames.Count(), 0 );
    
    // Video
    iVideo->FillBufferL( iBuffer, iVideoULDPStub );
    frame = CreateFrameItemLC( 800 );
    iVideo->iFrames.AppendL( frame );
    CleanupStack::Pop( frame );
    
    iVideo->FillBufferL( iBuffer, iVideoULDPStub );
    EUNIT_ASSERT_EQUALS( iVideo->iFrames.Count(), 0 );
    }

void UT_CMccFileAudio::UT_CMccFileAudio_WriteBufferL()
    {
    // Audio
    iAudio->WriteBufferL();
    
    iAudio->FillBufferL( iBuffer, iAudioULDPStub );
    CMccFrameItem* frame = CreateFrameItemLC( 800 );
    iAudio->iFrames.AppendL( frame );
    CleanupStack::Pop( frame );
    
    iAudio->WriteBufferL();
    EUNIT_ASSERT_EQUALS( iAudio->iFrames.Count(), 0 );
    
    // Too big frame
    iAudio->FillBufferL( iBuffer, iAudioULDPStub );
    frame = CreateFrameItemLC( 1200 );
    iAudio->iFrames.AppendL( frame );
    CleanupStack::Pop( frame );
    
    iAudio->WriteBufferL();
    EUNIT_ASSERT_EQUALS( iAudio->iFrames.Count(), 0 );
    
    // Video
    iVideo->WriteBufferL();
    
    iVideo->FillBufferL( iBuffer, iVideoULDPStub );
    frame = CreateFrameItemLC( 800 );
    iVideo->iFrames.AppendL( frame );
    CleanupStack::Pop( frame );
    
    iVideo->WriteBufferL();
    EUNIT_ASSERT_EQUALS( iVideo->iFrames.Count(), 0 );
    
    // Too big frame
    iVideo->FillBufferL( iBuffer, iVideoULDPStub );
    frame = CreateFrameItemLC( 1200 );
    iVideo->iFrames.AppendL( frame );
    CleanupStack::Pop( frame );
    
    iVideo->WriteBufferL();
    EUNIT_ASSERT_EQUALS( iVideo->iFrames.Count(), 0 );
    
    // AVC parameter sets exists and are written at first round
    delete iVideo->iAVCNALUsBuffer;
    iVideo->iAVCNALUsBuffer = NULL;
    iVideo->iAVCNALUsBuffer = _L8("somedata").AllocL();
    iVideo->iInsertParameterSets = ETrue;
    iVideo->FillBufferL( iBuffer, iVideoULDPStub );
    frame = CreateFrameItemLC( 800 );
    iVideo->iFrames.AppendL( frame );
    CleanupStack::Pop( frame );

    iVideo->WriteBufferL();
    // real frame wasn't therefore yet written
    EUNIT_ASSERT_EQUALS( iVideo->iFrames.Count(), 1 ); 
    EUNIT_ASSERT_EQUALS( iVideo->iInsertParameterSets, EFalse );
    TInt64 firstParameterSetsTime = iVideo->iInsertParameterSetsTime.Int64();
    EUNIT_ASSERT( firstParameterSetsTime != 0 );
    
    // Next round takes the real frame
    iVideo->FillBufferL( iBuffer, iVideoULDPStub );
    iVideo->WriteBufferL();
    EUNIT_ASSERT_EQUALS( iVideo->iFrames.Count(), 0 ); 
    
    // Once enough time has elapsed, AVC parameter sets are again written
    // const TUint32 KMccFileSourceParameterSetsIntervalInMicroSeconds = 2000000;
    iVideo->iInsertParameterSetsTime = ( iVideo->iInsertParameterSetsTime.Int64() - 2000002 );
    firstParameterSetsTime = iVideo->iInsertParameterSetsTime.Int64();
    
    iVideo->FillBufferL( iBuffer, iVideoULDPStub );
    frame = CreateFrameItemLC( 800 );
    iVideo->iFrames.AppendL( frame );
    CleanupStack::Pop( frame );

    iVideo->WriteBufferL();
    EUNIT_ASSERT_EQUALS( iVideo->iFrames.Count(), 0 ); // param sets were not written yet
    EUNIT_ASSERT_EQUALS( iVideo->iInsertParameterSets, ETrue );

    iVideo->FillBufferL( iBuffer, iVideoULDPStub );
    frame = CreateFrameItemLC( 800 );
    iVideo->iFrames.AppendL( frame );
    CleanupStack::Pop( frame );
    
    iVideo->WriteBufferL();
    EUNIT_ASSERT_EQUALS( iVideo->iFrames.Count(), 1 ); // parameter sets were written
    EUNIT_ASSERT_EQUALS( iVideo->iInsertParameterSets, EFalse );
    EUNIT_ASSERT( iVideo->iInsertParameterSetsTime.Int64() != firstParameterSetsTime );
    
    }

void UT_CMccFileAudio::UT_CMccFileAudio_TimingCorrectionL()
    {
    const TInt KMccTestTimerInterval = 100;
    iAudio->SetStartTime();
    iAudio->iTimerInterval = KMccTestTimerInterval; 
    iAudio->DoTimingCorrection( 0 ); // Not enough time has elapsed
    User::After( 2000100 );
    
    // Enough time has elapsed, correction not needed
    iAudio->DoTimingCorrection( 2000 );
    EUNIT_ASSERT( iAudio->iCorrectionInterval == KMccTestTimerInterval );
    
    // Enough time has elapsed and correction is needed as position is not
    // far enough (speeding the timer)
    iAudio->iPreviousCorrection -= TTimeIntervalMicroSeconds32( 2000000 );
    iAudio->DoTimingCorrection( 1000 );
    
    EUNIT_ASSERT( iAudio->iCorrectionInterval < KMccTestTimerInterval );
    
    // Enough time has elapsed and correction is needed as position is too
    // far (slowing the timer)
    User::After( 2000100 );
    iAudio->DoTimingCorrection( 8000 );
    EUNIT_ASSERT( iAudio->iCorrectionInterval > KMccTestTimerInterval );
    }    

void UT_CMccFileAudio::UT_CMccFileAudio_ConfigKeyL()
    {
    HBufC8* confKey = iVideo->GetConfigKeyL();
    CleanupStack::PushL( confKey );
    EUNIT_ASSERT( confKey && confKey->Length() > 0 );
   
    
    // Test also non-sdp version
    HBufC8* confKey2 = NULL;
    iVideo->ExtractH264ParameterSetNALUsL( &confKey2 );
    CleanupStack::PushL( confKey2 );
    EUNIT_ASSERT( confKey2 && confKey2->Length() > 0 );
    EUNIT_ASSERT( confKey2->Des().CompareF( *confKey ) != 0 );
    
    CleanupStack::PopAndDestroy( confKey2 );
    CleanupStack::PopAndDestroy( confKey );
    }  
    
CMccFrameItem* UT_CMccFileAudio::CreateFrameItemLC( TInt aSize )
    {
    CMccFrameItem* item = new (ELeave) CMccFrameItem();
    CleanupStack::PushL( item );
    item->iFrame = HBufC8::NewL( aSize );
    item->iTimeStamp = 1000;
    return item;
    }
    
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccFileAudio,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMccFileAudio",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileAudio_NewLL, Teardown)

EUNIT_TEST(
    "SetPositionL - test ",
    "CMccFileAudio",
    "SetPositionL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileAudio_SetPositionLL, Teardown)

EUNIT_TEST(
    "Type - test ",
    "CMccFileAudio",
    "Type",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileAudio_TypeL, Teardown)

EUNIT_TEST(
    "PositionL - test ",
    "CMccFileAudio",
    "PositionL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileAudio_PositionLL, Teardown)

EUNIT_TEST(
    "DurationL - test ",
    "CMccFileAudio",
    "DurationL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileAudio_DurationLL, Teardown)

EUNIT_TEST(
    "AudioBitRateL - test ",
    "CMccFileAudio",
    "AudioBitRateL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileAudio_AudioBitRateLL, Teardown)

EUNIT_TEST(
    "VideoFrameSizeL - test ",
    "CMccFileAudio",
    "VideoFrameSizeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileAudio_VideoFrameSizeLL, Teardown)

EUNIT_TEST(
    "VideoFrameRateL - test ",
    "CMccFileAudio",
    "VideoFrameRateL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileAudio_VideoFrameRateLL, Teardown)

EUNIT_TEST(
    "StartTimer - test ",
    "CMccFileAudio",
    "StartTimer",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileAudio_StartTimerL, Teardown)

EUNIT_TEST(
    "StopTimer - test ",
    "CMccFileAudio",
    "StopTimer",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileAudio_StopTimerL, Teardown)

EUNIT_TEST(
    "GetFourCC - test ",
    "CMccFileAudio",
    "GetFourCC",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileAudio_GetFourCCL, Teardown)

EUNIT_TEST(
    "ResetTime - test ",
    "CMccFileAudio",
    "ResetTime",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileAudio_ResetTimeL, Teardown)

EUNIT_TEST(
    "TickAudioL - test ",
    "CMccFileAudio",
    "TickAudioL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileAudio_TickAudioLL, Teardown)

EUNIT_TEST(
    "ReadFrameL - test ",
    "CMccFileAudio",
    "ReadFrameL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileAudio_ReadFrameLL, Teardown)

EUNIT_TEST(
    "ReadFrameL2 - test ",
    "CMccFileAudio",
    "ReadFrameL2",
    "FUNCTIONALITY",
    Setup2L, UT_CMccFileAudio_ReadFrameL2L, Teardown)
    
EUNIT_TEST(
    "ParseUpdateAudioDescriptions - test ",
    "CMccFileAudio",
    "ParseUpdateAudioDescriptions",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileAudio_ParseUpdateAudioDescriptionsL, Teardown)

EUNIT_TEST(
    "GetFrameLengthL - test ",
    "CMccFileAudio",
    "GetFrameLengthL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileAudio_GetFrameLengthL, Teardown)

EUNIT_TEST(
    "SetStartTimeL - test ",
    "CMccFileAudio",
    "SetStartTimeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileAudio_SetStartTimeL, Teardown)

EUNIT_TEST(
    "IsPausedL - test ",
    "CMccFileAudio",
    "IsPausedL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileAudio_IsPausedL, Teardown)
         
EUNIT_TEST(
    "GetTimeStampL - test ",
    "CMccFileAudio",
    "GetTimeStampL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSink_GetTimeStampL, Teardown)
    
EUNIT_TEST(
    "FillBufferL - test ",
    "CMccFileAudio",
    "FillBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileAudio_FillBufferL, Teardown)
    
EUNIT_TEST(
    "WriteBufferL - test ",
    "CMccFileAudio",
    "WriteBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileAudio_WriteBufferL, Teardown)
    
EUNIT_TEST(
    "TimingCorrectionL - test ",
    "CMccFileAudio",
    "TimingCorrectionL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileAudio_TimingCorrectionL, Teardown)

EUNIT_TEST(
    "ConfigKeyL - test ",
    "CMccFileAudio",
    "ConfigKeyL",
    "FUNCTIONALITY",
    Setup3L, UT_CMccFileAudio_ConfigKeyL, Teardown)
        
EUNIT_END_TEST_TABLE

//  END OF FILE
