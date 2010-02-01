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
//#include "mmfdatabuffer.h"
#include "UT_CMccFileSource.h"
#include "MmccInterfaceDef.h"
#include "mccinternaldef.h"
#include "MmccCodecInformation.h"
#include "mccresourcepool.h"
#include "mccfilesourceimpl.h"



//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "MccFileSource.h"
#include "Mcculdatapath.h"


// CONSTRUCTION
UT_CMccFileSource* UT_CMccFileSource::NewL()
    {
    UT_CMccFileSource* self = UT_CMccFileSource::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccFileSource* UT_CMccFileSource::NewLC()
    {
    UT_CMccFileSource* self = new( ELeave ) UT_CMccFileSource();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccFileSource::~UT_CMccFileSource()
    {
    }

// Default constructor
UT_CMccFileSource::UT_CMccFileSource()
    {
    }

// Second phase construct
void UT_CMccFileSource::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccFileSource::SetupL(  )
    {
    iBuffer = CMMFDescriptorBuffer::NewL( 1000 );
    iBuffer->Data().Copy(_L("aaaaaaaaaa"));
    TMediaId media = KUidMediaTypeAudio;
    MAsyncEventHandler* eventHandler = NULL;
    CMccResourcePool* iMccResource = NULL;


    iAudioULDPStub = CMccUlDataPath::NewL( eventHandler,iMccResource,  media );
    media = KUidMediaTypeVideo;
    iVideoULDPStub = CMccUlDataPath::NewL( eventHandler, iMccResource, media );
    
    TFileName fileName;
    fileName = _L("c:\\CrazyFrog.3gp");
    TPckgBuf<TFileName> initParam( fileName );
    
    iFileSource = static_cast<CMccFileSource*>( CMccFileSource::NewSourceL( KMccFileSourceUid, initParam ) );
    iFileSource->ConstructSourceL( initParam );
    
    iFileSource->SourcePrimeL();
    }
    
void UT_CMccFileSource::Setup2L(  )
    {
    iBuffer = CMMFDescriptorBuffer::NewL( 1000 );
    iBuffer->Data().Copy(_L("aaaaaaaaaa"));
    TMediaId media = KUidMediaTypeAudio;
    MAsyncEventHandler* eventHandler = NULL;
    CMccResourcePool* iMccResource = NULL;


    iAudioULDPStub = CMccUlDataPath::NewL( eventHandler,iMccResource,  media );
    media = KUidMediaTypeVideo;
    iVideoULDPStub = CMccUlDataPath::NewL( eventHandler, iMccResource, media );
    
    TFileName fileName;
    fileName = _L("c:\\CrazyFrog.3gp");
    TPckgBuf<TFileName> initParam( fileName );
    
    iFileSource = static_cast<CMccFileSource*>( CMccFileSource::NewSourceL( KMccFileSourceUid, initParam ) );
    }


void UT_CMccFileSource::Teardown(  )
    {
    delete iFileSource;
    delete iVideoULDPStub;
    delete iAudioULDPStub;
    delete iBuffer;
    }

void UT_CMccFileSource::UT_CMccFileSource_NewSourceLL(  )
    {
    EUNIT_ASSERT( ETrue );
    }

void UT_CMccFileSource::UT_CMccFileSource_ConstructSourceLL(  )
    {
    EUNIT_ASSERT( ETrue );
    }

void UT_CMccFileSource::UT_CMccFileSource_CMccFileSourceL(  )
    {
    EUNIT_ASSERT( ETrue );
    }

void UT_CMccFileSource::UT_CMccFileSource_OpenFileLL(  )
    {
    iFileSource->CloseFileL();
    
    // Might leave with KErrGeneral if running out of memory
    TRAPD( err, iFileSource->OpenFileL( _L("c:\\test.3gp") ) );
    if ( err == KErrGeneral )
        {
        User::Leave( KErrNoMemory );
        }
    }

void UT_CMccFileSource::UT_CMccFileSource_CloseFileLL(  )
    {
    iFileSource->CloseFileL();
    // Might leave with KErrGeneral if running out of memory
    TRAPD( err, iFileSource->OpenFileL( _L("c:\\test.3gp") ) );
    if ( err == KErrGeneral )
        {
        User::Leave( KErrNoMemory );
        }
    iFileSource->CloseFileL();
    }

void UT_CMccFileSource::UT_CMccFileSource_SetPositionLL(  )
    {
    iFileSource->SetPositionL(10000);
    }

void UT_CMccFileSource::UT_CMccFileSource_VideoFrameRateLL(  )
    {
    TReal32 videoFrameRate = iFileSource->VideoFrameRateL();
    }

void UT_CMccFileSource::UT_CMccFileSource_AudioBitRateLL(  )
    {
    TUint32 audioBitRate = iFileSource->AudioBitRateL();
    }

void UT_CMccFileSource::UT_CMccFileSource_DurationLL(  )
    {
    TTimeIntervalMicroSeconds duration = iFileSource->DurationL();
    }

void UT_CMccFileSource::UT_CMccFileSource_VideoFrameSizeLL(  )
    {
    TSize size = iFileSource->VideoFrameSizeL();
    }

void UT_CMccFileSource::UT_CMccFileSource_VideoBitRateLL(  )
    {
    TUint32 videoBitRate = iFileSource->VideoBitRateL();
    }

void UT_CMccFileSource::UT_CMccFileSource_PositionLL(  )
    {
    TTimeIntervalMicroSeconds pos = iFileSource->PositionL();
    }

void UT_CMccFileSource::UT_CMccFileSource_SourceDataTypeCodeL(  )
    {
    TMediaId mediaId (KUidMediaTypeAudio);
    TFourCC gotFourCC = iFileSource->SourceDataTypeCode(mediaId);
    EUNIT_ASSERT( gotFourCC == TFourCC( KMccFourCCIdAMRNB ) );
    }

void UT_CMccFileSource::UT_CMccFileSource_SetSourceDataTypeCodeL(  )
    { 
    TFourCC fourCC(KMccFourCCIdAMRNB ); 
    TMediaId mediaId (KUidMediaTypeAudio);
    iFileSource->SetSourceDataTypeCode( fourCC,mediaId );
    }

void UT_CMccFileSource::UT_CMccFileSource_BufferEmptiedLL(  )
    {
    CMMFBuffer* buffer = NULL;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFileSource->BufferEmptiedL(buffer) ,KErrNotSupported );
    }

void UT_CMccFileSource::UT_CMccFileSource_CanCreateSourceBufferL(  )
    {
    EUNIT_ASSERT( !iFileSource->CanCreateSourceBuffer() );
    }

void UT_CMccFileSource::UT_CMccFileSource_CreateSourceBufferLL(  )
    {
    TMediaId mediaId;
    TBool bufref = EFalse;
    EUNIT_ASSERT_SPECIFIC_LEAVE(iFileSource->CreateSourceBufferL(mediaId, bufref), KErrNotSupported);
    }

void UT_CMccFileSource::UT_CMccFileSource_SourceThreadLogonL(  )
    {
    MAsyncEventHandler* handler = NULL;
    EUNIT_ASSERT( KErrNone == iFileSource->SourceThreadLogon(*handler) );
    }

void UT_CMccFileSource::UT_CMccFileSource_SourceThreadLogoffL(  )
    {
//    iFileSource->SourceThreadLogoff();
    }

void UT_CMccFileSource::UT_CMccFileSource_SourcePrimeLL(  )
    {
    iFileSource->SourcePrimeL();
    }

void UT_CMccFileSource::UT_CMccFileSource_SourcePlayLL(  )
    {
    iFileSource->SourcePrimeL();
    iFileSource->SourcePlayL();
    }

void UT_CMccFileSource::UT_CMccFileSource_SourcePauseLL(  )
    {
    iFileSource->SourcePrimeL();
    iFileSource->SourcePlayL();
    iFileSource->SourcePauseL();
    }

void UT_CMccFileSource::UT_CMccFileSource_SourceStopLL(  )
    {
    iFileSource->SourcePrimeL();
    iFileSource->SourcePlayL();
    iFileSource->SourcePauseL();
    iFileSource->SourceStopL();

    }

void UT_CMccFileSource::UT_CMccFileSource_FillBufferLL(  )
    {
    TMediaId mediaId(KUidMediaTypeAudio);
    EUNIT_ASSERT_EQUALS(TFourCC( KMccFourCCIdAMRNB ), iFileSource->SourceDataTypeCode(mediaId) );
    iFileSource->FillBufferL(iBuffer, iAudioULDPStub,  mediaId);
    
    mediaId = KUidMediaTypeVideo;
    EUNIT_ASSERT_EQUALS(TFourCC( KMccFourCCIdH263 ), iFileSource->SourceDataTypeCode(mediaId) );
    iFileSource->FillBufferL(iBuffer, iVideoULDPStub,  mediaId);
    }

void UT_CMccFileSource::UT_CMccFileSource_NegotiateSourceLL(  )
    {
    MDataSink* sink = NULL;
    iFileSource->NegotiateSourceL(*sink);
    }
    
void UT_CMccFileSource::UT_CMccFileSource_SetParameterLL()
    {
    TPckgBuf<TInt> packageBuffer = 346;
    iFileSource->SetParameterL( KMccPrerollTime, packageBuffer );
    EUNIT_ASSERT( iFileSource->iSourceImpl->DelayAfterStop() == 346 );
    
    TMccFileSourceSettingBuf settingsBuf;
    settingsBuf().iFileName = _L("c:\\test.3gp");
    
    // Might leave with KErrGeneral if running out of memory
    TRAPD( err, iFileSource->SetParameterL( KMccFileName, settingsBuf ) );
    if ( err == KErrGeneral )
        {
        User::Leave( KErrNoMemory );
        }
    
    settingsBuf().iPosition = 2000;
    iFileSource->SetParameterL( KMccFilePosition, settingsBuf );
    
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iFileSource->SetParameterL( KMccFileVideoFrameRate, settingsBuf ), KErrArgument );
    
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iFileSource->SetParameterL( KMccFileVideoBitRate, settingsBuf ), KErrArgument );
    
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iFileSource->SetParameterL( KMccFileVideoFrameSize, settingsBuf ), KErrArgument );
        
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iFileSource->SetParameterL( KMccFileAudioBitRate, settingsBuf ), KErrArgument );
      
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iFileSource->SetParameterL( KMccFileDuration, settingsBuf ), KErrArgument );

    EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iFileSource->SetParameterL( KMccFileInfo, settingsBuf ), KErrArgument );


    EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iFileSource->SetParameterL( KMccFileAudioFourCC, settingsBuf ), KErrArgument );

    EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iFileSource->SetParameterL( KMccFileVideoFourCC, settingsBuf ), KErrArgument );
     
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iFileSource->SetParameterL( KMccFileFastForward, settingsBuf ), KErrNotSupported );
    
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFileSource->SetParameterL( 1000, settingsBuf ), KErrArgument );         
    }
         
void UT_CMccFileSource::UT_CMccFileSource_GetParameterLL()
    {
    TMccFileSourceSettingBuf settingsBuf;
    settingsBuf().iFileName = _L("");
    
    iFileSource->GetParameterL( KMccFileName, settingsBuf );
    EUNIT_ASSERT( settingsBuf().iFileName == _L("c:\\CrazyFrog.3gp") );
    
    settingsBuf().iPosition = 100;
    iFileSource->GetParameterL( KMccFilePosition, settingsBuf );
    // Haven't had any time to proceed
    EUNIT_ASSERT( settingsBuf().iPosition == 0 );
    
    iFileSource->GetParameterL( KMccFileInfo, settingsBuf );
    iFileSource->GetParameterL( KMccFileDuration, settingsBuf );
    iFileSource->GetParameterL( KMccFileVideoFrameRate, settingsBuf );
    iFileSource->GetParameterL( KMccFileVideoBitRate, settingsBuf );
    iFileSource->GetParameterL( KMccFileVideoFrameSize, settingsBuf );
    iFileSource->GetParameterL( KMccFileAudioBitRate, settingsBuf );
    iFileSource->GetParameterL( KMccFileAudioFourCC, settingsBuf );
    iFileSource->GetParameterL( KMccFileVideoFourCC, settingsBuf );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFileSource->GetParameterL( KMccFileFastForward, settingsBuf ), KErrNotSupported );

    EUNIT_ASSERT_SPECIFIC_LEAVE( iFileSource->GetParameterL( 999, settingsBuf ), KErrArgument );
    
    // Config key (will fail with some error inside implementation class as AVC clip is not opened)
    TBuf8<KMaxConfigKeyLen> confKey;
    TRAPD( err, iFileSource->GetParameterL( KMccConfigKey, confKey ) );
    
    CMccFileSourceImpl* impl = iFileSource->iSourceImpl;
    CleanupStack::PushL( impl );
    iFileSource->iSourceImpl = NULL;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFileSource->GetParameterL( KMccConfigKey, confKey ), KErrArgument );
    iFileSource->iSourceImpl = impl;
    CleanupStack::Pop( impl );
    
    }

void UT_CMccFileSource::UT_CMccFileSource_ImplNotReadyL()
    {
    TMediaId media = KUidMediaTypeAudio;
    TFileName fileName;
    fileName.Copy( _L("plaa") );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFileSource->OpenFileL( fileName ), KErrArgument );

    EUNIT_ASSERT_SPECIFIC_LEAVE( iFileSource->CloseFileL(), KErrArgument );

    EUNIT_ASSERT_SPECIFIC_LEAVE( iFileSource->FileNameL(), KErrArgument );
        
	EUNIT_ASSERT_SPECIFIC_LEAVE( iFileSource->SetPositionL( TTimeIntervalMicroSeconds( 0 ) ), KErrArgument );
	    
	EUNIT_ASSERT_SPECIFIC_LEAVE( iFileSource->VideoFrameRateL(), KErrArgument );

	EUNIT_ASSERT_SPECIFIC_LEAVE( iFileSource->AudioBitRateL(), KErrArgument );

	EUNIT_ASSERT_SPECIFIC_LEAVE( iFileSource->DurationL(), KErrArgument );

	EUNIT_ASSERT_SPECIFIC_LEAVE( iFileSource->VideoFrameSizeL(), KErrArgument );

	EUNIT_ASSERT_SPECIFIC_LEAVE( iFileSource->VideoBitRateL(), KErrArgument );

	EUNIT_ASSERT_SPECIFIC_LEAVE( iFileSource->PositionL(), KErrArgument );
	    
	EUNIT_ASSERT_SPECIFIC_LEAVE( iFileSource->AudioFourCCL(), KErrArgument );
	    
	EUNIT_ASSERT_SPECIFIC_LEAVE( iFileSource->VideoFourCCL(), KErrArgument );
	
	iFileSource->SourceDataTypeCode( media );

    EUNIT_ASSERT_EQUALS( iFileSource->SetSourceDataTypeCode( TFourCC( 0 ), media ), KErrNotFound );

    EUNIT_ASSERT_EQUALS( iFileSource->SourceThreadLogon( *(MAsyncEventHandler*)NULL ), KErrNotFound );

    EUNIT_ASSERT_SPECIFIC_LEAVE( iFileSource->SourcePrimeL(), KErrArgument );
        
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFileSource->SourcePlayL(), KErrArgument );
        
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFileSource->SourcePauseL(), KErrArgument );
        
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFileSource->SourceStopL(), KErrArgument );

    EUNIT_ASSERT_SPECIFIC_LEAVE( iFileSource->FillBufferL( NULL, NULL, media ), KErrArgument );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccFileSource,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NewSourceL - test ",
    "CMccFileSource",
    "NewSourceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSource_NewSourceLL, Teardown)

EUNIT_TEST(
    "ConstructSourceL - test ",
    "CMccFileSource",
    "ConstructSourceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSource_ConstructSourceLL, Teardown)

EUNIT_TEST(
    "CMccFileSource - test ",
    "CMccFileSource",
    "CMccFileSource",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSource_CMccFileSourceL, Teardown)

EUNIT_TEST(
    "OpenFileL - test ",
    "CMccFileSource",
    "OpenFileL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSource_OpenFileLL, Teardown)

EUNIT_TEST(
    "CloseFileL - test ",
    "CMccFileSource",
    "CloseFileL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSource_CloseFileLL, Teardown)

EUNIT_TEST(
    "SetPositionL - test ",
    "CMccFileSource",
    "SetPositionL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSource_SetPositionLL, Teardown)

EUNIT_TEST(
    "VideoFrameRateL - test ",
    "CMccFileSource",
    "VideoFrameRateL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSource_VideoFrameRateLL, Teardown)

EUNIT_TEST(
    "AudioBitRateL - test ",
    "CMccFileSource",
    "AudioBitRateL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSource_AudioBitRateLL, Teardown)

EUNIT_TEST(
    "DurationL - test ",
    "CMccFileSource",
    "DurationL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSource_DurationLL, Teardown)

EUNIT_TEST(
    "VideoFrameSizeL - test ",
    "CMccFileSource",
    "VideoFrameSizeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSource_VideoFrameSizeLL, Teardown)

EUNIT_TEST(
    "VideoBitRateL - test ",
    "CMccFileSource",
    "VideoBitRateL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSource_VideoBitRateLL, Teardown)

EUNIT_TEST(
    "PositionL - test ",
    "CMccFileSource",
    "PositionL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSource_PositionLL, Teardown)

EUNIT_TEST(
    "SourceDataTypeCode - test ",
    "CMccFileSource",
    "SourceDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSource_SourceDataTypeCodeL, Teardown)

EUNIT_TEST(
    "SetSourceDataTypeCode - test ",
    "CMccFileSource",
    "SetSourceDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSource_SetSourceDataTypeCodeL, Teardown)

EUNIT_TEST(
    "BufferEmptiedL - test ",
    "CMccFileSource",
    "BufferEmptiedL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSource_BufferEmptiedLL, Teardown)

EUNIT_TEST(
    "CanCreateSourceBuffer - test ",
    "CMccFileSource",
    "CanCreateSourceBuffer",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSource_CanCreateSourceBufferL, Teardown)

EUNIT_TEST(
    "CreateSourceBufferL - test ",
    "CMccFileSource",
    "CreateSourceBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSource_CreateSourceBufferLL, Teardown)

EUNIT_TEST(
    "SourceThreadLogon - test ",
    "CMccFileSource",
    "SourceThreadLogon",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSource_SourceThreadLogonL, Teardown)

EUNIT_TEST(
    "SourceThreadLogoff - test ",
    "CMccFileSource",
    "SourceThreadLogoff",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSource_SourceThreadLogoffL, Teardown)

EUNIT_TEST(
    "SourcePrimeL - test ",
    "CMccFileSource",
    "SourcePrimeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSource_SourcePrimeLL, Teardown)

EUNIT_TEST(
    "SourcePlayL - test ",
    "CMccFileSource",
    "SourcePlayL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSource_SourcePlayLL, Teardown)

EUNIT_TEST(
    "SourcePauseL - test ",
    "CMccFileSource",
    "SourcePauseL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSource_SourcePauseLL, Teardown)

EUNIT_TEST(
    "SourceStopL - test ",
    "CMccFileSource",
    "SourceStopL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSource_SourceStopLL, Teardown)

EUNIT_TEST(
    "FillBufferL - test ",
    "CMccFileSource",
    "FillBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSource_FillBufferLL, Teardown)

EUNIT_TEST(
    "NegotiateSourceL - test ",
    "CMccFileSource",
    "NegotiateSourceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSource_NegotiateSourceLL, Teardown)

EUNIT_TEST(
    "SetParameterL - test ",
    "CMccFileSource",
    "SetParameterL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSource_SetParameterLL, Teardown)
    
EUNIT_TEST(
    "GetParameterL - test ",
    "CMccFileSource",
    "GetParameterL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSource_GetParameterLL, Teardown)

EUNIT_TEST(
    "ImplNotReadyL - test ",
    "CMccFileSource",
    "ImplNotReadyL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccFileSource_ImplNotReadyL, Teardown)
    
EUNIT_END_TEST_TABLE

//  END OF FILE
