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
#include "UT_CMccFileSourceImpl.h"
#include "mmcccodecinformation.h"
#include "mccresourcepool.h"
#include "mmccinterfacedef.h"
#include "mccinternaldef.h"
#include "mccunittestmacros.h"


//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "mccfilesourceimpl.h"

#include "mcculdatapath.h"
#include "mccfilevideo.h"
#include "mccfileaudio.h"

// CONSTRUCTION
UT_CMccFileSourceImpl* UT_CMccFileSourceImpl::NewL()
    {
    UT_CMccFileSourceImpl* self = UT_CMccFileSourceImpl::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccFileSourceImpl* UT_CMccFileSourceImpl::NewLC()
    {
    UT_CMccFileSourceImpl* self = new( ELeave ) UT_CMccFileSourceImpl();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccFileSourceImpl::~UT_CMccFileSourceImpl()
    {
    }

// Default constructor
UT_CMccFileSourceImpl::UT_CMccFileSourceImpl()
    {
    }

// Second phase construct
void UT_CMccFileSourceImpl::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_CMccFileSourceImpl::SetupL(  )
    {
    // No local play
    //
    iBuffer = CMMFDescriptorBuffer::NewL( 1000 );
    iBuffer->Data().Copy(_L("aaaaaaaaaa")); 
    
    TMediaId media = KUidMediaTypeAudio;
    MAsyncEventHandler* eventHandler = NULL;
    CMccResourcePool* iMccResource = NULL;

    iAudioULDPStub = CMccUlDataPath::NewL( eventHandler, iMccResource, media );
    media = KUidMediaTypeVideo;
    iVideoULDPStub = CMccUlDataPath::NewL( eventHandler, iMccResource, media );
    
  
    TMccFileSourceSettingBuf settingBuf;
    TFileName fileName;
    fileName.Copy(_L("c:\\CrazyFrog.3gp"));
    settingBuf().iFileName = fileName;
    
    iSourceImp = CMccFileSourceImpl::NewL( 0 );
    iSourceImp->OpenFileL( settingBuf().iFileName );
    
    iSourceImp->SourcePrimeL();
    }


void UT_CMccFileSourceImpl::Setup2L( )
    {
    iBuffer = CMMFDescriptorBuffer::NewL( 1000 );
    iBuffer->Data().Copy(_L("aaaaaaaaaa")); 
    
    TMediaId media = KUidMediaTypeAudio;
    MAsyncEventHandler* eventHandler = NULL;
    CMccResourcePool* iMccResource = NULL;

    iAudioULDPStub = CMccUlDataPath::NewL( eventHandler, iMccResource, media );
    media = KUidMediaTypeVideo;
    iVideoULDPStub = CMccUlDataPath::NewL( eventHandler, iMccResource, media );
    
  
    TMccFileSourceSettingBuf settingBuf;
    TFileName fileName;
    fileName.Copy(_L("c:\\test_avc.3gp"));
    settingBuf().iFileName = fileName;
    
    iSourceImp = CMccFileSourceImpl::NewL( 0 );
    iSourceImp->OpenFileL( settingBuf().iFileName );
    
    iSourceImp->SourcePrimeL();
    }
        
void UT_CMccFileSourceImpl::Teardown(  )
    {
    iSourceImp->CloseFileL();

	delete iSourceImp;
	delete iAudioULDPStub;
	delete iVideoULDPStub;
	delete iBuffer;
    }

void UT_CMccFileSourceImpl::UT_CMccFileSourceImpl_NewLL(  )
    {
  //  CMccFileSourceImpl* sourceImpl = CMccFileSourceImpl::NewL(0);
  //  delete sourceImpl;
    }

void UT_CMccFileSourceImpl::UT_CMccFileSourceImpl_CMccFileSourceImplL(  )
    {
    EUNIT_ASSERT( ETrue );
    }

void UT_CMccFileSourceImpl::UT_CMccFileSourceImpl_AddDataSinkL(  )
    {
    MCMRMediaSink* sink = NULL;
    iSourceImp->AddDataSink(sink);
    }

void UT_CMccFileSourceImpl::UT_CMccFileSourceImpl_OpenFileLL(  )
    {
    iSourceImp->CloseFileL(); // close file CrazyFrog.3gp

    // Might leave with KErrGeneral if running out of memory
    TRAPD( err, iSourceImp->OpenFileL( _L("c:\\test.3gp") ) );
    if ( err == KErrGeneral || err == KErrNoMemory )
        {
        User::Leave( KErrNoMemory );
        }
    
    iSourceImp->CloseFileL(); // close file test.3gp

    EUNIT_ASSERT_LEAVE(iSourceImp->OpenFileL( _L("c:\test.3gp") ));
 
    // neither audio nor video
    EUNIT_ASSERT_LEAVE(iSourceImp->OpenFileL( _L("c:\\dummytest.txt") ));    	
    	
    }

void UT_CMccFileSourceImpl::UT_CMccFileSourceImpl_CloseFileLL(  )
    {
    iSourceImp->CloseFileL(); // close file CrazyFrog.3gp

    // Might leave with KErrGeneral if running out of memory
    TRAPD( err, iSourceImp->OpenFileL( _L("c:\\test.mp4") ) );
    if ( err == KErrGeneral || err == KErrNoMemory )
        {
        User::Leave( KErrNoMemory );
        }
    }

void UT_CMccFileSourceImpl::UT_CMccFileSourceImpl_SetPositionLL(  )
    {
    iSourceImp->SetPositionL(  TTimeIntervalMicroSeconds ( 0 ) );
    iSourceImp->SetPositionL(  TTimeIntervalMicroSeconds ( 10 ) );
    TMediaId mediaId;
    mediaId.iMediaType = KUidMediaTypeVideo;
    EUNIT_ASSERT_EQUALS(TFourCC( KMccFourCCIdH263 ), iSourceImp->SourceDataTypeCode(mediaId) );
    iSourceImp->SetPositionL(  TTimeIntervalMicroSeconds ( 100 ) );  
    
    iSourceImp->iIsVideo = EFalse;
    
    mediaId.iMediaType = KUidMediaTypeAudio;
    EUNIT_ASSERT_EQUALS(TFourCC( KMccFourCCIdAMRNB ), iSourceImp->SourceDataTypeCode(mediaId) );
    iSourceImp->SetPositionL(  TTimeIntervalMicroSeconds ( 1000 ) );  
    iSourceImp->iCurrentState = CMccFileSourceImpl::EStopped;
    iSourceImp->SetPositionL(  TTimeIntervalMicroSeconds ( 10000 ) );  
    }

void UT_CMccFileSourceImpl::UT_CMccFileSourceImpl_VideoTypeLL(  )
    {
    EUNIT_ASSERT(iSourceImp->VideoTypeL() == CCMRMediaBuffer::EVideoH263); 	
    }

void UT_CMccFileSourceImpl::UT_CMccFileSourceImpl_AudioTypeLL(  )
    {
    EUNIT_ASSERT(iSourceImp->AudioTypeL() == CCMRMediaBuffer::EAudioAMRNB); 	
    }

void UT_CMccFileSourceImpl::UT_CMccFileSourceImpl_VideoFrameRateLL(  )
    {

    TReal32 videoFrameRate = iSourceImp->VideoFrameRateL();
    }

void UT_CMccFileSourceImpl::UT_CMccFileSourceImpl_AudioBitRateLL(  )
    {
    TUint32 audioBitRate = iSourceImp->AudioBitRateL();
    }

void UT_CMccFileSourceImpl::UT_CMccFileSourceImpl_DurationLL(  )
    {
    TMediaId mediaId;
    mediaId.iMediaType = KUidMediaTypeVideo;
    EUNIT_ASSERT_EQUALS(TFourCC( KMccFourCCIdH263 ), iSourceImp->SourceDataTypeCode(mediaId) );
    
    TTimeIntervalMicroSeconds duration = iSourceImp->DurationL();
    
    iSourceImp->iIsVideo = EFalse;
    
    mediaId.iMediaType = KUidMediaTypeAudio;
    EUNIT_ASSERT_EQUALS(TFourCC( KMccFourCCIdAMRNB ), iSourceImp->SourceDataTypeCode(mediaId) );
      
    duration = iSourceImp->DurationL();
    }

void UT_CMccFileSourceImpl::UT_CMccFileSourceImpl_VideoFrameSizeLL(  )
    {
    TSize size = iSourceImp->VideoFrameSizeL();
    }

void UT_CMccFileSourceImpl::UT_CMccFileSourceImpl_VideoBitRateLL(  )
    {
    TUint32 videoBitRate = iSourceImp->VideoBitRateL();
    EUNIT_ASSERT_EQUALS(videoBitRate, KMccH263ProfileZeroMinBitRateIOP); // Low bitrate increased to more interoperable
    
    // Test situation where no need to make iop modification for bitrate
    iSourceImp->iFileVideo->iStreamAverageBitRate = 100000;
    videoBitRate = iSourceImp->VideoBitRateL();
    EUNIT_ASSERT_EQUALS(videoBitRate, iSourceImp->iFileVideo->iStreamAverageBitRate - iSourceImp->iFileAudio->iAverageBitRate);
    }

void UT_CMccFileSourceImpl::UT_CMccFileSourceImpl_PositionLL(  )
    {
    TMediaId mediaId;
    mediaId.iMediaType = KUidMediaTypeVideo;
    EUNIT_ASSERT_EQUALS(TFourCC( KMccFourCCIdH263 ), iSourceImp->SourceDataTypeCode(mediaId) );
    iSourceImp->iIsAudio = ETrue;
    iSourceImp->iIsVideo = ETrue;
    
    // If both media types are used, smaller position is returned
    iSourceImp->iFileAudio->iPosition = 1000;
    iSourceImp->iFileVideo->iPosition = 2000;
    TTimeIntervalMicroSeconds pos = iSourceImp->PositionL();
    EUNIT_ASSERT_EQUALS( pos.Int64(), 1000000 );
    
    // If other media type is already at end (pos zero) other media types position is used
    iSourceImp->iFileAudio->iPosition = 0;
    iSourceImp->iFileVideo->iPosition = 2000;
    pos = iSourceImp->PositionL();
    EUNIT_ASSERT_EQUALS( pos.Int64(), 2000000 );
    
    iSourceImp->iIsVideo = EFalse;
    
    mediaId.iMediaType = KUidMediaTypeAudio;
    iSourceImp->iFileAudio->iPosition = 3000;
    EUNIT_ASSERT_EQUALS(TFourCC( KMccFourCCIdAMRNB ), iSourceImp->SourceDataTypeCode(mediaId) );
      
    pos = iSourceImp->PositionL();
    EUNIT_ASSERT_EQUALS( pos.Int64(), 3000000 );
    }

void UT_CMccFileSourceImpl::UT_CMccFileSourceImpl_SourceDataTypeCodeL(  )
    {
    TMediaId mediaId;
    EUNIT_ASSERT_EQUALS(TFourCC( NULL ), iSourceImp->SourceDataTypeCode(mediaId) );

    mediaId.iMediaType = KUidMediaTypeVideo;
    EUNIT_ASSERT_EQUALS(TFourCC( KMccFourCCIdH263 ), iSourceImp->SourceDataTypeCode(mediaId) );

    mediaId.iMediaType = KUidMediaTypeAudio;
    EUNIT_ASSERT_EQUALS(TFourCC( KMccFourCCIdAMRNB ), iSourceImp->SourceDataTypeCode(mediaId) );
    }

void UT_CMccFileSourceImpl::UT_CMccFileSourceImpl_SetSourceDataTypeCodeL(  )
    {
    TMediaId mediaId;
    EUNIT_ASSERT_EQUALS( KErrNotSupported, iSourceImp->SetSourceDataTypeCode( TFourCC( ' ','A','N','Y' ), mediaId ) );
    
    mediaId.iMediaType = KUidMediaTypeVideo;
    EUNIT_ASSERT_EQUALS(TFourCC( KMccFourCCIdH263 ), iSourceImp->SourceDataTypeCode(mediaId) );
    EUNIT_ASSERT_EQUALS( KErrNone, iSourceImp->SetSourceDataTypeCode( TFourCC(KMccFourCCIdH263), mediaId ) );
    
    mediaId.iMediaType = KUidMediaTypeAudio;
    EUNIT_ASSERT_EQUALS(TFourCC( KMccFourCCIdAMRNB ), iSourceImp->SourceDataTypeCode(mediaId) );

    EUNIT_ASSERT_EQUALS( KErrNone, iSourceImp->SetSourceDataTypeCode( TFourCC(KMccFourCCIdAMRNB), mediaId ) );
    }

void UT_CMccFileSourceImpl::UT_CMccFileSourceImpl_SourcePrimeLL(  )
    {
    iSourceImp->SourcePrimeL();
    }

void UT_CMccFileSourceImpl::UT_CMccFileSourceImpl_SourcePlayLL(  )
    {
    iSourceImp->SourcePrimeL();
    iSourceImp->SourcePlayL();
    iSourceImp->SourcePlayL(); // return if is in playing state
    iSourceImp->CloseFileL();
    
    // Might leave with KErrGeneral if running out of memory
    TRAPD( err, iSourceImp->OpenFileL( _L("c:\\CrazyFrog.3gp") ) );
    if ( err == KErrGeneral || err == KErrNoMemory )
        {
        User::Leave( KErrNoMemory );
        }
    }
void UT_CMccFileSourceImpl::UT_CMccFileSourceImpl_SourcePlayL2L()
    {
    iSourceImp->SourcePrimeL();
    iSourceImp->iIsVideo = ETrue;
    iSourceImp->iIsAudio = ETrue;  
    iSourceImp->SourcePlayL( EFalse, EFalse );
    iSourceImp->SourcePauseL( );   
    iSourceImp->SourcePlayL( EFalse, EFalse );

    }
void UT_CMccFileSourceImpl::UT_CMccFileSourceImpl_SourcePauseLL(  )
    {
    iSourceImp->SourcePauseL(); // return if not in playing state

    iSourceImp->SourcePrimeL();
    iSourceImp->SourcePlayL();
    iSourceImp->iIsVideo = ETrue;
    iSourceImp->iIsAudio = ETrue;
    iSourceImp->SourcePauseL();
  
    }

void UT_CMccFileSourceImpl::UT_CMccFileSourceImpl_SourceStopLL(  )
    {
    iSourceImp->SourceStopL(); // return if state is in EConstructed

    iSourceImp->SourcePrimeL();
    iSourceImp->SourcePlayL();
    iSourceImp->SourcePauseL();
    iSourceImp->iIsVideo = ETrue;
    iSourceImp->SourceStopL(EFalse, EFalse);
    
    iSourceImp->SourcePrimeL();
    iSourceImp->SourceStopL();

    iSourceImp->SourcePrimeL();
    iSourceImp->SourcePlayL();
    iSourceImp->iIsAudio = ETrue;

    iSourceImp->SourceStopL(EFalse, EFalse);
    }

void UT_CMccFileSourceImpl::UT_CMccFileSourceImpl_FillBufferLL(  )
    {
    TMediaId media(KUidMediaTypeAudio); 
    EUNIT_ASSERT_EQUALS(TFourCC( KMccFourCCIdAMRNB ), iSourceImp->SourceDataTypeCode(media) );
    iSourceImp->FillBufferL(iBuffer, iAudioULDPStub, media);

    media = KUidMediaTypeVideo;
    EUNIT_ASSERT_EQUALS(TFourCC( KMccFourCCIdH263 ), iSourceImp->SourceDataTypeCode(media) );
    iSourceImp->FillBufferL(iBuffer, iVideoULDPStub, media);
    
    media = KUidMediaTypeMidi; // neither video nor audio
    EUNIT_ASSERT_SPECIFIC_LEAVE(
    	iSourceImp->FillBufferL(iBuffer, iVideoULDPStub, media), KErrNotSupported );
    }

void UT_CMccFileSourceImpl::UT_CMccFileSourceImpl_PauseAudioL()
    {
    iSourceImp->PauseAudioL(); // return, iCurrentState != EPlaying 
    iSourceImp->iIsVideo = ETrue;
    iSourceImp->iCurrentState = CMccFileSourceImpl::EPlaying;
    iSourceImp->PauseAudioL( KErrGeneral ); 
    iSourceImp->iIsVideo = EFalse;
    iSourceImp->PauseAudioL( );    
    }

void UT_CMccFileSourceImpl::UT_CMccFileSourceImpl_PauseVideoL()
    {
    iSourceImp->PauseVideoL(); // return, iCurrentState != EPlaying 
    iSourceImp->iIsAudio = ETrue;
    iSourceImp->iCurrentState = CMccFileSourceImpl::EPlaying;
    iSourceImp->PauseVideoL( KErrGeneral ); 
    iSourceImp->iIsVideo = EFalse;
    iSourceImp->PauseVideoL( ); 
    }
void UT_CMccFileSourceImpl::UT_CMccFileSourceImpl_SourceThreadLogonL()
    {
    MAsyncEventHandler* eventHandler = NULL;
    iSourceImp->SourceThreadLogon( *eventHandler );
    }

void UT_CMccFileSourceImpl::UT_CMccFileSourceImpl_SourceThreadLogOffL()
    {
    MAsyncEventHandler* eventHandler = NULL;
    iSourceImp->SourceThreadLogon( *eventHandler );

    iSourceImp->SourceThreadLogoff();   
    }

void UT_CMccFileSourceImpl::UT_CMccFileSourceImpl_MilliToMicroSecondsL()
    {
    TTimeIntervalMicroSeconds microSec = iSourceImp->MilliToMicroSeconds( 1000 );
    EUNIT_ASSERT_EQUALS( microSec.Int64(), 1000000 );
    
    TUint32 milliSec = iSourceImp->MicroToMilliSeconds( microSec );
    EUNIT_ASSERT_EQUALS( milliSec, 1000);

    }

void UT_CMccFileSourceImpl::UT_CMccFileSourceImpl_SetDelayAfterStopL()
    {
    iSourceImp->SetDelayAfterStop( 1000 );
    EUNIT_ASSERT( iSourceImp->DelayAfterStop() == 1000 );
    iSourceImp->SetDelayAfterStop( 2000 );
    EUNIT_ASSERT( iSourceImp->DelayAfterStop() == 2000 );
    }

void UT_CMccFileSourceImpl::UT_CMccFileSourceImpl_EofTimerExpiredLL()
    {
    iSourceImp->iCurrentState = CMccFileSourceImpl::EConstructed;
    iSourceImp->EofTimerExpiredL();
    EUNIT_ASSERT( iSourceImp->iCurrentState == CMccFileSourceImpl::EWaitingPause );
    }

void UT_CMccFileSourceImpl::UT_CMccFileSourceImpl_GetConfigKeyLL()
    {
    // Not initialized fully
    //
    TBuf8<KMaxConfigKeyLen> confKey;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iSourceImp->GetConfigKeyL( confKey ), KErrNotReady );
    
    // Not avc clip
    //
    TMediaId mediaId;
    mediaId.iMediaType = KUidMediaTypeVideo;
    iSourceImp->SourceDataTypeCode( mediaId );
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iSourceImp->GetConfigKeyL( confKey ), KErrNotSupported );
    }
    
void UT_CMccFileSourceImpl::UT_CMccFileSourceImpl_GetConfigKeyL2L()
    {
    // Avc clip
    //
    TMediaId mediaId;
    mediaId.iMediaType = KUidMediaTypeVideo;
    TBuf8<KMaxConfigKeyLen> confKey;
    iSourceImp->SourceDataTypeCode( mediaId );
    iSourceImp->GetConfigKeyL( confKey );
    EUNIT_ASSERT( confKey != KNullDesC8 );
    
    // Avc clip, input param too short
    TBuf8<1> confKey2;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iSourceImp->GetConfigKeyL( confKey2 ), KErrOverflow );
    }
    
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccFileSourceImpl,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMccFileSourceImpl",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSourceImpl_NewLL, Teardown)

EUNIT_TEST(
    "CMccFileSourceImpl - test ",
    "CMccFileSourceImpl",
    "CMccFileSourceImpl",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSourceImpl_CMccFileSourceImplL, Teardown)

EUNIT_TEST(
    "AddDataSink - test ",
    "CMccFileSourceImpl",
    "AddDataSink",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSourceImpl_AddDataSinkL, Teardown)

EUNIT_TEST(
    "OpenFileL - test ",
    "CMccFileSourceImpl",
    "OpenFileL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSourceImpl_OpenFileLL, Teardown)

EUNIT_TEST(
    "CloseFileL - test ",
    "CMccFileSourceImpl",
    "CloseFileL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSourceImpl_CloseFileLL, Teardown)

EUNIT_TEST(
    "SetPositionL - test ",
    "CMccFileSourceImpl",
    "SetPositionL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSourceImpl_SetPositionLL, Teardown)

EUNIT_TEST(
    "VideoTypeL - test ",
    "CMccFileSourceImpl",
    "VideoTypeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSourceImpl_VideoTypeLL, Teardown)

EUNIT_TEST(
    "AudioTypeL - test ",
    "CMccFileSourceImpl",
    "AudioTypeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSourceImpl_AudioTypeLL, Teardown)

EUNIT_TEST(
    "VideoFrameRateL - test ",
    "CMccFileSourceImpl",
    "VideoFrameRateL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSourceImpl_VideoFrameRateLL, Teardown)

EUNIT_TEST(
    "AudioBitRateL - test ",
    "CMccFileSourceImpl",
    "AudioBitRateL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSourceImpl_AudioBitRateLL, Teardown)

EUNIT_TEST(
    "DurationL - test ",
    "CMccFileSourceImpl",
    "DurationL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSourceImpl_DurationLL, Teardown)

EUNIT_TEST(
    "VideoFrameSizeL - test ",
    "CMccFileSourceImpl",
    "VideoFrameSizeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSourceImpl_VideoFrameSizeLL, Teardown)

EUNIT_TEST(
    "VideoBitRateL - test ",
    "CMccFileSourceImpl",
    "VideoBitRateL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSourceImpl_VideoBitRateLL, Teardown)

EUNIT_TEST(
    "PositionL - test ",
    "CMccFileSourceImpl",
    "PositionL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSourceImpl_PositionLL, Teardown)

EUNIT_TEST(
    "SourceDataTypeCode - test ",
    "CMccFileSourceImpl",
    "SourceDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSourceImpl_SourceDataTypeCodeL, Teardown)

EUNIT_TEST(
    "SetSourceDataTypeCode - test ",
    "CMccFileSourceImpl",
    "SetSourceDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSourceImpl_SetSourceDataTypeCodeL, Teardown)

EUNIT_TEST(
    "SourcePrimeL - test ",
    "CMccFileSourceImpl",
    "SourcePrimeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSourceImpl_SourcePrimeLL, Teardown)

EUNIT_TEST(
    "SourcePlayL - test ",
    "CMccFileSourceImpl",
    "SourcePlayL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSourceImpl_SourcePlayLL, Teardown)
EUNIT_TEST(
    "SourcePlay2L - test ",
    "CMccFileSourceImpl",
    "SourcePlay2L",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSourceImpl_SourcePlayL2L, Teardown)

EUNIT_TEST(
    "SourcePauseL - test ",
    "CMccFileSourceImpl",
    "SourcePauseL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSourceImpl_SourcePauseLL, Teardown)

EUNIT_TEST(
    "SourceStopL - test ",
    "CMccFileSourceImpl",
    "SourceStopL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSourceImpl_SourceStopLL, Teardown)

EUNIT_TEST(
    "FillBufferL - test ",
    "CMccFileSourceImpl",
    "FillBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSourceImpl_FillBufferLL, Teardown)
EUNIT_TEST(
    "PauseAudioL - test ",
    "CMccFileSourceImpl",
    "PauseAudioL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSourceImpl_PauseAudioL, Teardown)
    
EUNIT_TEST(
    "PauseVideoL - test ",
    "CMccFileSourceImpl",
    "PauseVideoL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSourceImpl_PauseVideoL, Teardown)
        
EUNIT_TEST(
    "SourceThreadLogonL - test ",
    "CMccFileSourceImpl",
    "SourceThreadLogonL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSourceImpl_SourceThreadLogonL, Teardown)
    
EUNIT_TEST(
    "SourceThreadLogOffL - test ",
    "CMccFileSourceImpl",
    "SourceThreadLogOffL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSourceImpl_SourceThreadLogOffL, Teardown)
    
EUNIT_TEST(
    "MilliToMicroSecondsL - test ",
    "CMccFileSourceImpl",
    "MilliToMicroSecondsL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSourceImpl_MilliToMicroSecondsL, Teardown)

EUNIT_TEST(
    "SetDelayAfterStop - test ",
    "CMccFileSourceImpl",
    "SetDelayAfterStop",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSourceImpl_SetDelayAfterStopL, Teardown)    
    
EUNIT_TEST(
    "EofTimerExpiredL - test ",
    "CMccFileSourceImpl",
    "EofTimerExpiredL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSourceImpl_EofTimerExpiredLL, Teardown)    

EUNIT_TEST(
    "GetConfigKeyL - test ",
    "CMccFileSourceImpl",
    "GetConfigKeyL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSourceImpl_GetConfigKeyLL, Teardown) 
    
EUNIT_TEST(
    "GetConfigKeyL 2 - test ",
    "CMccFileSourceImpl",
    "GetConfigKeyL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccFileSourceImpl_GetConfigKeyL2L, Teardown)  

EUNIT_END_TEST_TABLE

//  END OF FILE
