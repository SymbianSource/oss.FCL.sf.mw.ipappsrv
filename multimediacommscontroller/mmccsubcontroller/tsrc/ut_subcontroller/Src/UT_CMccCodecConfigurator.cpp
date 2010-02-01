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
#include "UT_CMccCodecConfigurator.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <mmf/server/mmfaudioinput.h>
#include <mmf/plugin/mmfaudioiointerfaceuids.hrh>

//  INTERNAL INCLUDES
#include "mcccodecconfigurator.h"
#include "Mccinternalcodecs.h"
#include "FormatStateMachine.h"
#include <mmf/server/mmfformat.h>
#include "mccrtpmediaclock.h"
#include "mmccinterfacedef.h"
#include "mccunittestmacros.h"
#include "mccinternaldef.h"
#include "mccgenericdatasink_STUB.h"
#include "mccgenericdatasource_STUB.h"
#include "mccfilesink.h"

// CONSTRUCTION
UT_CMccCodecConfigurator* UT_CMccCodecConfigurator::NewL()
    {
    UT_CMccCodecConfigurator* self = UT_CMccCodecConfigurator::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccCodecConfigurator* UT_CMccCodecConfigurator::NewLC()
    {
    UT_CMccCodecConfigurator* self = new( ELeave ) UT_CMccCodecConfigurator();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccCodecConfigurator::~UT_CMccCodecConfigurator()
    {
    }

// Default constructor
UT_CMccCodecConfigurator::UT_CMccCodecConfigurator()
    {
    }

// Second phase construct
void UT_CMccCodecConfigurator::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CMccCodecConfigurator::SetupL()
    {
    iRtpMediaClock = CMccRtpMediaClock::NewL();
    }

void UT_CMccCodecConfigurator::Teardown()
    {
    delete iRtpMediaClock;
    }

void UT_CMccCodecConfigurator::UT_CMccCodecConfigurator_ConfigureCodecLL()
    {
    TMccCodecInfo cInfo;

    // These two will fail because of the NULL    
    //EUNIT_ASSERT_LEAVE( CMccCodecConfigurator::ConfigureCodecL( NULL, reinterpret_cast<MDataSink&>( *this ), cInfo, *iRtpMediaClock ) );
    //EUNIT_ASSERT_LEAVE( CMccCodecConfigurator::ConfigureCodecL( reinterpret_cast<MDataSource&>( *this ), NULL, cInfo, *iRtpMediaClock ) );
    }

void UT_CMccCodecConfigurator::UT_CMccCodecConfigurator_DoConfigureAmrNbLL()
    {
    // aPlaybackDev == NULL
    /*EUNIT_ASSERT_LEAVE( CMccCodecConfigurator::DoConfigureAmrNbL(
        static_cast<MMMFAudioOutput&>( NULL ), KAmrNbBitrate475, EFalse ) );*/

    // AMR-NB
    TMccCodecInfo codecInfo;
    codecInfo.iFourCC = KMccFourCCIdAMRNB;
    codecInfo.iBitrate = 42;
    // Wrong bitrate
    EUNIT_ASSERT_LEAVE( MccCodecConfigurator::DoConfigureAmrL(
        reinterpret_cast<MMMFAudioOutput&>( *this ), codecInfo ) );
    
    // AMR-WB    
    codecInfo.iFourCC = KMccFourCCIdAMRWB;
    // Wrong bitrate
    EUNIT_ASSERT_LEAVE( MccCodecConfigurator::DoConfigureAmrL(
        reinterpret_cast<MMMFAudioOutput&>( *this ), codecInfo ) );
    }

void UT_CMccCodecConfigurator::UT_CMccCodecConfigurator_DoConfigureAmrNbL2L()
    {
    // aRecordDev == NULL
    /*EUNIT_ASSERT_LEAVE( CMccCodecConfigurator::DoConfigureAmrNbL(
        static_cast<MMMFAudioInput&>( NULL ), KAmrNbBitrate475, EFalse ) );*/
    }

void UT_CMccCodecConfigurator::UT_CMccCodecConfigurator_DoConfigureUplinkStreamLL()
    {
    TMccCodecInfo codec;
    
    // aRecordDevice == NULL
    /*EUNIT_ASSERT_LEAVE( CMccCodecConfigurator::DoConfigureLiveAudioUplinkStreamL(
        static_cast<MMMFAudioInput&>( NULL ),
        NULL,
        codec ) );*/

    // aFormatter == NULL
    /*EUNIT_ASSERT_LEAVE( CMccCodecConfigurator::DoConfigureLiveAudioUplinkStreamL(
        reinterpret_cast<MMMFAudioInput*>( this ),
        NULL,
        codec ) );*/

    // wrong codec
    EUNIT_ASSERT_LEAVE( MccCodecConfigurator::DoConfigureLiveAudioUplinkStreamL(
        reinterpret_cast<MMMFAudioInput&>( *this ),
        reinterpret_cast<CPayloadFormatWrite&>( *this ),
        codec,
        *iRtpMediaClock,
        CMccSymStreamBase::EStateCodecLoaded ) );
    }

void UT_CMccCodecConfigurator::UT_CMccCodecConfigurator_DoConfigureDownlinkStreamLL()
    {
    TMccCodecInfo codec;
    
    // aPlayDevice == NULL
    /*EUNIT_ASSERT_LEAVE( CMccCodecConfigurator::DoConfigureLiveAudioDownlinkStreamL(
        static_cast<MMMFAudioOutput*>( NULL ),
        NULL,
        codec ) );*/

    // aFormatter == NULL
    /*EUNIT_ASSERT_LEAVE( CMccCodecConfigurator::DoConfigureLiveAudioDownlinkStreamL(
        reinterpret_cast<MMMFAudioOutput*>( this ),
        NULL,
        codec ) );*/

    // wrong codec
    EUNIT_ASSERT_LEAVE( MccCodecConfigurator::DoConfigureLiveAudioDownlinkStreamL(
        reinterpret_cast<MMMFAudioOutput&>( *this ),
        reinterpret_cast<CPayloadFormatRead&>( *this ),
        codec,
        CMccSymStreamBase::EStateCodecLoaded ) );
    }

void UT_CMccCodecConfigurator::UT_CMccCodecConfigurator_DoGetClipConfigurationLL()
    {
    TMccFileSourceSetting settings;
    
    // File not found
    settings.iFileName = _L("c:\\testfilecannotbefound.3gp");
    TMccFileSourceSettingBuf settingsBuf( settings );
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( 
        MccCodecConfigurator::DoGetClipConfigurationL( settingsBuf ), KErrNotFound );
    
    // File contains audio and video
    settings.iFileName = _L("c:\\CrazyFrog.3gp");
    settingsBuf = settings;
    
    TRAPD( err, MccCodecConfigurator::DoGetClipConfigurationL( settingsBuf ));
    if ( err == KErrNotFound )
        {
    	User::Leave( KErrNoMemory ); // MP4 lib gives file errors when out of mem
        }
    else if ( err != KErrNone )
        {
        User::Leave( err );
        }
    
    settings = settingsBuf();
    
    // FourCC value is not set when running out of memory.    
    if ( settings.iAudioFourCC == TFourCC() ||
         settings.iVideoFourCC == TFourCC() )
        {
        return;
        }   
    
    EUNIT_ASSERT( settings.iAudioFourCC == TFourCC( KMccFourCCIdAMRNB ) );
    EUNIT_ASSERT( settings.iVideoFourCC == TFourCC( KMccFourCCIdH263 ) );
    
    TMccFileSourceSetting settings2;
    settings2.iFileName = _L("c:\\test_avc.3gp");
    settingsBuf = settings2;
    
    TRAP( err, MccCodecConfigurator::DoGetClipConfigurationL( settingsBuf ));
    if ( err == KErrNotFound )
        {
    	User::Leave( KErrNoMemory ); // MP4 lib gives file errors when out of mem
        }
    else if ( err != KErrNone )
        {
        User::Leave( err );
        }
    
    settings2 = settingsBuf();
    
    // FourCC value is not set when running out of memory.    
    if ( settings2.iVideoFourCC == TFourCC() )
        {
        return;
        }   
    
    EUNIT_ASSERT( settings2.iVideoFourCC == TFourCC( KMccFourCCIdAVC ) );
    }

void UT_CMccCodecConfigurator::UT_CMccCodecConfigurator_PassPrerollFromSinkToSourceLL()
    {
    CMccGenericDataSource* dataSource = new ( ELeave ) CMccGenericDataSource( KMccFileSourceUid );
    CleanupStack::PushL( dataSource );
    CMccGenericDataSink* dataSink = new ( ELeave ) CMccGenericDataSink( KMccVideoSinkUid );
    CleanupStack::PushL( dataSink );

//    CMccFileSource* dataSource = new CMccFileSource();
//    dataSource->iSourceImpl = CMccFileSourceImpl::NewL( 0 );

    // Should return immediately
    MccCodecConfigurator::PassPrerollFromSinkToSourceL(
        reinterpret_cast<MDataSink*>( NULL ), (MDataSource *)dataSource );
    // Should return immediately
    MccCodecConfigurator::PassPrerollFromSinkToSourceL(
        (MDataSink *)dataSink, reinterpret_cast<MDataSource*>( NULL ) );

    TInt preroll = 346;
    TPckgBuf<TInt> packageBuffer = preroll;
    dataSink->SetParameterL( KMccPrerollTime, packageBuffer );

    MccCodecConfigurator::PassPrerollFromSinkToSourceL(
        (MDataSink *)dataSink, (MDataSource *)dataSource );
        
//    EUNIT_ASSERT( dataSource->iSourceImpl->DelayAfterStop() == KMccMinPreroll ); // KMccMinPreroll = 2000
    packageBuffer = 0;
    dataSource->GetParameterL( KMccPrerollTime, packageBuffer );
    EUNIT_ASSERT( packageBuffer() == 346 );

    CleanupStack::Pop( dataSink );
    CleanupStack::Pop( dataSource );
    delete dataSink;
    delete dataSource;
    }


void UT_CMccCodecConfigurator::UT_MccCodecConfigurator_IlbcNeedsStopResumeL()
    {
    TInt origBr = 13300;
    TInt newBr = 15200;
    TFourCC fcc;
    EUNIT_ASSERT( MccCodecConfigurator::IlbcNeedsStopResume( origBr, newBr, fcc ) == EFalse );
    
    fcc = KMccFourCCIdILBC;
    EUNIT_ASSERT( MccCodecConfigurator::IlbcNeedsStopResume( origBr, newBr, fcc ) == ETrue );
    EUNIT_ASSERT( MccCodecConfigurator::IlbcNeedsStopResume( origBr, origBr, fcc ) == EFalse );
    EUNIT_ASSERT( MccCodecConfigurator::IlbcNeedsStopResume( newBr, newBr, fcc ) == EFalse );
    
    fcc = KMccFourCCIdAMRNB;
    EUNIT_ASSERT( MccCodecConfigurator::IlbcNeedsStopResume( origBr, newBr, fcc ) == EFalse );
    EUNIT_ASSERT( MccCodecConfigurator::IlbcNeedsStopResume( origBr, origBr, fcc ) == EFalse );
    EUNIT_ASSERT( MccCodecConfigurator::IlbcNeedsStopResume( newBr, newBr, fcc ) == EFalse );
    }
    
void UT_CMccCodecConfigurator::UT_MccCodecConfigurator_VadWithoutComfortNoiseL()
    {
    // Actually this test does not really work because CI's are not implemented
    // in WINSCW. But, basic skeleton case implemented here so that it can be
    // easily brought in action in case of working WINSCW CI's.
    TUid inputUid = TUid::Uid( KMmfUidAudioInputInterface );
    MDataSource* recordDevice = MDataSource::NewSourceL( inputUid, KNullDesC8 );
    CleanupDeletePushL( recordDevice );
    recordDevice->SourceThreadLogon( *this );
    
    MMMFAudioInput* tmp = static_cast<MMMFAudioInput*>( recordDevice );
    
    TMccCodecInfo info;
    info.iFourCC = KMccFourCCIdILBC;
    info.iComfortNoiseGenerationPt = KPayloadTypeUndefined;
    info.iEnableDTX = ETrue;
    
    MCC_EUNIT_ASSERT_NO_LEAVE( 
        MccCodecConfigurator::DoConfigureIlbcL( *tmp, info, CMccSymStreamBase::EStateCodecPrepared ) );
    
    info.iComfortNoiseGenerationPt = 13;
    
    MCC_EUNIT_ASSERT_NO_LEAVE( 
        MccCodecConfigurator::DoConfigureIlbcL( *tmp, info, CMccSymStreamBase::EStateCodecPrepared ) );
    
    info.iFourCC = KMccFourCCIdG711;
    info.iComfortNoiseGenerationPt = KPayloadTypeUndefined;
    info.iEnableDTX = ETrue;
    
    recordDevice->SourceThreadLogoff();
    User::LeaveIfError( recordDevice->SourceThreadLogon( *this ) );
    
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( 
        MccCodecConfigurator::DoConfigureG711L( *tmp, info, CMccSymStreamBase::EStateCodecPrepared ),
        KErrNone ); // Was KErrNotFound, but stub return KErrNone. Not sure about hw.
    
    recordDevice->SourceThreadLogoff();
    CleanupStack::PopAndDestroy( recordDevice );
    }

void UT_CMccCodecConfigurator::UT_MccCodecConfigurator_DoConfigureFileSinkL()
    {
    CMccFileSink* fileSink = static_cast<CMccFileSink*>( CMccFileSink::NewSinkL( KNullUid, KNullDesC8 ) );
    CleanupStack::PushL( fileSink );
    TMccCodecInfo videoCodec;
    videoCodec.iType = KUidMediaTypeVideo;
    videoCodec.iFourCC = KMccFourCCIdAVC;
    MccCodecConfigurator::DoConfigureFileSinkL( *fileSink, videoCodec );
    EUNIT_ASSERT( fileSink->SinkDataTypeCode( KUidMediaTypeVideo ) == KMccFourCCIdAVC );
    
    TMccCodecInfo audioCodec;
    audioCodec.iType = KUidMediaTypeAudio;
    audioCodec.iFourCC = KMccFourCCIdG711;
    MccCodecConfigurator::DoConfigureFileSinkL( *fileSink, audioCodec );
    EUNIT_ASSERT( fileSink->SinkDataTypeCode( KUidMediaTypeAudio ) == KMccFourCCIdG711 );
    CleanupStack::PopAndDestroy( fileSink );
    }
    
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccCodecConfigurator,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "ConfigureCodecL - test ",
    "CMccCodecConfigurator",
    "ConfigureCodecL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecConfigurator_ConfigureCodecLL, Teardown)

EUNIT_TEST(
    "DoConfigureAmrNbL - test ",
    "CMccCodecConfigurator",
    "DoConfigureAmrNbL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecConfigurator_DoConfigureAmrNbLL, Teardown)

EUNIT_TEST(
    "DoConfigureAmrNbL2 - test ",
    "CMccCodecConfigurator",
    "DoConfigureAmrNbL2",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecConfigurator_DoConfigureAmrNbL2L, Teardown)

EUNIT_TEST(
    "DoConfigureUplinkStreamL - test ",
    "CMccCodecConfigurator",
    "DoConfigureUplinkStreamL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecConfigurator_DoConfigureUplinkStreamLL, Teardown)

EUNIT_TEST(
    "DoConfigureDownlinkStreamL - test ",
    "CMccCodecConfigurator",
    "DoConfigureDownlinkStreamL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecConfigurator_DoConfigureDownlinkStreamLL, Teardown)

EUNIT_TEST(
    "DoGetClipConfigurationL - test ",
    "CMccCodecConfigurator",
    "DoGetClipConfigurationL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecConfigurator_DoGetClipConfigurationLL, Teardown)
    
EUNIT_TEST(
    "PassPrerollFromSinkToSourceL - test ",
    "CMccCodecConfigurator",
    "PassPrerollFromSinkToSourceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecConfigurator_PassPrerollFromSinkToSourceLL, Teardown)

EUNIT_TEST(
    "IlbcNeedsStopResume - test ",
    "CMccCodecConfigurator",
    "IlbcNeedsStopResume",
    "FUNCTIONALITY",
    SetupL, UT_MccCodecConfigurator_IlbcNeedsStopResumeL, Teardown)

EUNIT_TEST(
    "VadWithoutComfortNoiser - test ",
    "CMccCodecConfigurator",
    "VadWithoutComfortNoise",
    "FUNCTIONALITY",
    SetupL, UT_MccCodecConfigurator_VadWithoutComfortNoiseL, Teardown)

EUNIT_TEST(
    "DoConfigureFileSinkL - test ",
    "CMccCodecConfigurator",
    "DoConfigureFileSinkL",
    "FUNCTIONALITY",
    SetupL, UT_MccCodecConfigurator_DoConfigureFileSinkL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
