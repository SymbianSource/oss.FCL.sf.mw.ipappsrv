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
#include "UT_CMccSymDlStream.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "Mccsymdlstream.h"
#include "Mccrtpmanager.h"
#include "Mcctesteventhandler.h"
#include "mmccinterfacedef.h"
#include "mccresourcepool_stub.h"
#include "mcccodecconfigurator.h"
#include "mccmultiplexer.h"
#include "mmcccryptocontext.h"
#include "mccjitterbuffer.h"
#include "mccrtpmediaclock.h"

#include "mccunittestmacros.h"

const TUid KRtpDataSource = { KImplUidRtpDataSource }; 
const TInt KAmrNbBitrate122 = 12200;
const TInt KIlbcBitrate20 = 15200;
const TInt KIlbcBitrate30 = 13330;

// CONSTRUCTION
UT_CMccSymDlStream* UT_CMccSymDlStream::NewL()
    {
    UT_CMccSymDlStream* self = UT_CMccSymDlStream::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccSymDlStream* UT_CMccSymDlStream::NewLC()
    {
    UT_CMccSymDlStream* self = new( ELeave ) UT_CMccSymDlStream();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccSymDlStream::~UT_CMccSymDlStream()
    {
    }

// Default constructor
UT_CMccSymDlStream::UT_CMccSymDlStream()
    {
    }

// Second phase construct
void UT_CMccSymDlStream::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS
void UT_CMccSymDlStream::SetupL()
    {
    iRtpMediaClock = CMccRtpMediaClock::NewL();
    
    iResources = CMccResourcePoolStub::NewL();
    iStreamId = 1;
    iHandler = CMccTestEventHandler::NewL();
    iRtpMan = CMccRtpManager::NewL( *iHandler, *iResources, iMccSessionId );
    iStream = CMccSymDlStream::NewLC( iStreamId, iHandler, iResources, iRtpMan, 201, *iRtpMediaClock );
    CleanupStack::Pop( iStream );
    
    TPckgBuf<TInt> params( 30000 );
    iSourceStub = MDataSource::NewSourceL( KRtpDataSource, params );
    iSinkStub = MDataSink::NewSinkL( KUidMmfAudioOutput, KNullDesC8 );

    iStream->AddSinkAndSourceL( iSinkStub, iSourceStub );
    
    TMccCodecInfo cInfo;
    cInfo.iFourCC = TFourCC( KMccFourCCIdAMRNB );
    cInfo.iBitrate = KAmrNbBitrate122;
    cInfo.iHwFrameTime = 20;
    cInfo.iFrameSize = 32;
    cInfo.iPtime = 20;
    cInfo.iMaxPtime = 400; // recommended "limit" 200ms
    cInfo.iJitterBufBufferLength = 100;
    cInfo.iPayloadType = KMinDynamicPT;
    cInfo.iCodecMode = EBandwidthEfficient;
    cInfo.iPayloadFormatEncoder = KImplUidAmrPayloadFormatEncode;
    cInfo.iPayloadFormatDecoder = KImplUidAmrPayloadFormatDecode;
    _LIT8( KFmtp, "octet-align=0" );
    TBuf8<13> buf( KFmtp() );
    iStream->LoadCodecL( cInfo, buf );
    
    TUint port;
    iRtpMan->CreateSessionL( port, ETrue, EFalse );
    }
    
void UT_CMccSymDlStream::Setup2L()
    {
    iRtpMediaClock = CMccRtpMediaClock::NewL();
    
    iResources = CMccResourcePoolStub::NewL();
    iStreamId = 1;
    iHandler = CMccTestEventHandler::NewL();
    iRtpMan = CMccRtpManager::NewL( *iHandler, *iResources, iMccSessionId );
    iStream = CMccSymDlStream::NewLC( iStreamId, iHandler, iResources, iRtpMan, 201, *iRtpMediaClock );
    CleanupStack::Pop( iStream );
    
    TPckgBuf<TInt> params( 30000 );
    iSourceStub = MDataSource::NewSourceL( KRtpDataSource, params );
    iSinkStub = MDataSink::NewSinkL( KUidMmfAudioOutput, KNullDesC8 );

    iStream->AddSinkAndSourceL( iSinkStub, iSourceStub );
    }

void UT_CMccSymDlStream::SetupIlbcL()
    {
    iRtpMediaClock = CMccRtpMediaClock::NewL();
    
    iResources = CMccResourcePoolStub::NewL();
    iStreamId = 1;
    iHandler = CMccTestEventHandler::NewL();
    iRtpMan = CMccRtpManager::NewL( *iHandler, *iResources,iMccSessionId );
    iStream = CMccSymDlStream::NewLC( iStreamId, iHandler, iResources, iRtpMan, 201, *iRtpMediaClock );
    CleanupStack::Pop( iStream );
    
    TPckgBuf<TInt> params( 30000 );
    iSourceStub = MDataSource::NewSourceL( KRtpDataSource, params );
    iSinkStub = MDataSink::NewSinkL( KUidMmfAudioOutput, KNullDesC8 );
    
    iStream->AddSinkAndSourceL( iSinkStub, iSourceStub );
    
    TMccCodecInfo cInfo;
    cInfo.iFourCC = TFourCC( KMccFourCCIdILBC );
    cInfo.iBitrate = KIlbcBitrate20;
    cInfo.iHwFrameTime = 20;
    cInfo.iFrameSize = 0;
    cInfo.iPtime = 20;
    cInfo.iMaxPtime = 200;
    cInfo.iJitterBufBufferLength = 100;
    cInfo.iPayloadType = KMinDynamicPT;
    cInfo.iPayloadFormatEncoder = KImplUidiLBCPayloadFormatEncode;
    cInfo.iPayloadFormatDecoder = KImplUidiLBCPayloadFormatDecode;
    
    iStream->LoadCodecL( cInfo, KNullDesC8 );
    
    TUint port;
    iRtpMan->CreateSessionL( port, ETrue, EFalse );
    }
    
void UT_CMccSymDlStream::Teardown()
    {
    if ( iStream )
        {
        delete iStream;
        }
    
    if ( iRtpMan )
        {
        delete iRtpMan;
        }
    
    if ( iHandler )
        {
        delete iHandler;
        }
    
    delete iSourceStub;
    delete iSinkStub;
    delete iResources;
    
    if ( iRtpMediaClock )
        {
        delete iRtpMediaClock;
        iRtpMediaClock = NULL;
        }
    }

void UT_CMccSymDlStream::UT_CMccSymDlStream_SetPrioritySettingsLL(  )
    {
    TMMFPrioritySettings priority;

    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->SetPrioritySettingsL( priority ) );
    
    iStream->iDatasink = NULL;

    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iStream->SetPrioritySettingsL( priority ), KErrNotReady );
    }
    
void UT_CMccSymDlStream::UT_CMccSymDlStream_CreatePayloadFormatDecoderL()
    {
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iStream->CreatePayloadFormatDecoderL(), KErrAlreadyExists );
    
    TMccCodecInfo cInfo;
    cInfo.iFourCC = TFourCC( KMccFourCCIdAMRNB );
    cInfo.iBitrate = KAmrNbBitrate122;
    cInfo.iAlgoUsed = EGenRedUsed;
    cInfo.iHwFrameTime = 20;
    cInfo.iFrameSize = 32;
    cInfo.iPtime = 20;
    cInfo.iMaxPtime = 400; // recommended "limit" 200ms
    cInfo.iJitterBufBufferLength = 100;
    cInfo.iPayloadType = KMinDynamicPT;
    cInfo.iRedundantPayload = KMinDynamicPT;
    cInfo.iCodecMode = EBandwidthEfficient;
    cInfo.iPayloadFormatEncoder = KImplUidAmrPayloadFormatEncode;
    cInfo.iPayloadFormatDecoder = KImplUidAmrPayloadFormatDecode;
    _LIT8( KFmtp, "octet-align=0" );
    TBuf8<13> buf( KFmtp() );

    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->LoadCodecL( cInfo, buf ) );
    
    delete iStream->iFormatDecode;
    iStream->iFormatDecode = NULL;
    delete iStream->iRedFormatDecode;
    iStream->iRedFormatDecode = NULL;
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->CreatePayloadFormatDecoderL() );
    
    cInfo.iAlgoUsed = ENoAlgoUsed;
    iStream->iCodecInfo = cInfo;
    
    delete iStream->iFormatDecode;
    iStream->iFormatDecode = NULL;
    delete iStream->iRedFormatDecode;
    iStream->iRedFormatDecode = NULL;
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->CreatePayloadFormatDecoderL() );

    }

void UT_CMccSymDlStream::UT_CMccSymDlStream_DoDynamicFrameSizeChangeL()
    {
      
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iStream->DoDynamicFrameSizeChangeL( 20 ), KErrNotReady ); 
 /*   
    TMccCodecInfo cInfo;
    cInfo.iFourCC = TFourCC( KMccFourCCIdAMRNB );
    cInfo.iBitrate = KAmrNbBitrate122;
    cInfo.iAlgoUsed = EGenRedUsed;
    cInfo.iHwFrameTime = 20;
    cInfo.iFrameSize = 32;
    cInfo.iPtime = 20;
    cInfo.iMaxPtime = 400; // recommended "limit" 200ms
    cInfo.iJitterBufBufferLength = 100;
    cInfo.iPayloadType = KMinDynamicPT;
    cInfo.iRedundantPayload = KMinDynamicPT;
    cInfo.iCodecMode = EBandwidthEfficient;
    cInfo.iPayloadFormatEncoder = KImplUidAmrPayloadFormatEncode;
    cInfo.iPayloadFormatDecoder = KImplUidAmrPayloadFormatDecode;
    _LIT8( KFmtp, "octet-align=0" );
    TBuf8<13> buf( KFmtp() );

    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->LoadCodecL( cInfo, buf ) );
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->DoDynamicFrameSizeChangeL( 20 )  ); 
    */
    }

void UT_CMccSymDlStream::UT_CMccSymDlStream_MultiplexerSink()
    {
    iStream->iCodecInfo.iAlgoUsed = EGenRedUsed;
    MDataSink* sink = iStream->MultiplexerSink();
    EUNIT_ASSERT( !sink );
    iStream->iCodecInfo.iAlgoUsed = ENoAlgoUsed;
    sink = iStream->MultiplexerSink();
    EUNIT_ASSERT( sink );
    }

void UT_CMccSymDlStream::UT_CMccSymDlStream_AddSinkAndSourceLL(  )
    {
    // These two will fail because of the NULL    
    EUNIT_ASSERT_LEAVE( iStream->AddSinkAndSourceL( reinterpret_cast<MDataSink*>( this ), NULL ) );
    EUNIT_ASSERT_LEAVE( iStream->AddSinkAndSourceL( NULL, reinterpret_cast<MDataSource*>( this ) ) );
    }

void UT_CMccSymDlStream::UT_CMccSymDlStream_LoadCodecLL(  )
    {
    TMccCodecInfo cInfo;
    cInfo.iFourCC = TFourCC( KMccFourCCIdAMRNB );
    cInfo.iBitrate = KAmrNbBitrate122;
    cInfo.iHwFrameTime = 20;
    cInfo.iFrameSize = 32;
    cInfo.iPtime = 20;
    cInfo.iMaxPtime = 400; // recommended "limit" 200ms
    cInfo.iJitterBufBufferLength = 100;
    cInfo.iPayloadType = KMinDynamicPT;
    cInfo.iCodecMode = EBandwidthEfficient;
    cInfo.iPayloadFormatEncoder = KImplUidAmrPayloadFormatEncode;
    cInfo.iPayloadFormatDecoder = KImplUidAmrPayloadFormatDecode;
    _LIT8( KFmtp, "octet-align=0" );
    TBuf8<13> buf( KFmtp() );

    EUNIT_ASSERT( iStream->CurrentCodecState() == CMccSymStreamBase::EStateCodecNone );
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->LoadCodecL( cInfo, buf ) );
    
    EUNIT_ASSERT( iStream->CurrentCodecState() == CMccSymStreamBase::EStateCodecNegotiated );
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->LoadCodecL( cInfo, buf ) );
    
    EUNIT_ASSERT( iStream->CurrentCodecState() == CMccSymStreamBase::EStateCodecPrepared );
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->LoadCodecL( cInfo, buf ) );

    // It seems that loading the real codec causes memory leak
    EUNIT_ASSERT( iStream->CurrentCodecState() == CMccSymStreamBase::EStateCodecPrepared );
    iStream->UpdateCodecLoadingPermission( ETrue );
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->LoadCodecL( cInfo, buf ) );
    
    EUNIT_ASSERT( iStream->CurrentCodecState() == CMccSymStreamBase::EStateCodecLoaded );
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->LoadCodecL( cInfo, buf ) );
    }

void UT_CMccSymDlStream::UT_CMccSymDlStream_UpdateCodecInformationLL(  )
    {
    TMccCodecInfo cInfo;

    // iFormatDecode == NULL
    EUNIT_ASSERT_LEAVE( iStream->UpdateCodecInformationL( iStream->iCodecInfo, cInfo ) );
    
    cInfo.iFourCC = TFourCC( KMccFourCCIdAMRNB );
    cInfo.iBitrate = KAmrNbBitrate122;
    cInfo.iHwFrameTime = 20;
    cInfo.iFrameSize = 32;
    cInfo.iPtime = 20;
    cInfo.iMaxPtime = 400; // recommended "limit" 200ms
    cInfo.iJitterBufBufferLength = 100;
    cInfo.iPayloadType = KMinDynamicPT;
    cInfo.iCodecMode = EBandwidthEfficient;
    cInfo.iPayloadFormatEncoder = KImplUidAmrPayloadFormatEncode;
    cInfo.iPayloadFormatDecoder = KImplUidAmrPayloadFormatDecode;
    _LIT8( KFmtp, "octet-align=0" );
    TBuf8<13> buf( KFmtp() );
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->LoadCodecL( cInfo, buf ) );
//    TUint port;
//    iRtpMan->CreateSessionL( port, ETrue, EFalse );
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->UpdateCodecInformationL( iStream->iCodecInfo, cInfo ) );
    
    iStream->iDatasink = NULL;
    
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iStream->UpdateCodecInformationL( 
        iStream->iCodecInfo, cInfo ), KErrNotReady );
    }

void UT_CMccSymDlStream::IlbcModeChangeTestL()
    {
    RDebug::Print( _L("UT_CMccSymDlStream::IlbcModeChangeTestL IN") );
    
    iStream->PrimeL( 0 );
    
    TMccCodecInfo cInfo;
    cInfo.iFourCC = TFourCC( KMccFourCCIdILBC );
    cInfo.iBitrate = KIlbcBitrate20;
    cInfo.iHwFrameTime = 20;
    cInfo.iFrameSize = 0;
    cInfo.iPtime = 20;
    cInfo.iMaxPtime = 200;
    cInfo.iJitterBufBufferLength = 100;
    cInfo.iPayloadType = KMinDynamicPT;
    cInfo.iPayloadFormatEncoder = KImplUidiLBCPayloadFormatEncode;
    cInfo.iPayloadFormatDecoder = KImplUidiLBCPayloadFormatDecode;
    
    iStream->iState = CMccSymStreamBase::EStateStreaming;
    
    // First update ptime
    cInfo.iPtime = 40;
    MCC_EUNIT_ASSERT_NO_LEAVE(
        iStream->UpdateCodecInformationL( iStream->iCodecInfo, cInfo ) );
    
    // Then update bitrate which requires also ptime adjustment
    cInfo.iBitrate = KIlbcBitrate30;
    cInfo.iPtime = 30;
    cInfo.iMaxPtime = 180;
    MCC_EUNIT_ASSERT_NO_LEAVE(
        iStream->UpdateCodecInformationL( iStream->iCodecInfo, cInfo ) );
    
    RDebug::Print( _L("UT_CMccSymDlStream::IlbcModeChangeTestL OUT") );
    }
    
void UT_CMccSymDlStream::UT_CMccSymDlStream_PrimeLL(  )
    {
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->PrimeL(0) );
    
    // Can be called twice
    iResources->iControlled = EFalse;
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->PrimeL(0) );
    }

void UT_CMccSymDlStream::UT_CMccSymDlStream_PlayLL(  )
    {
    UT_CMccSymDlStream_PrimeLL();
    
    TBool streamPaused( EFalse );
    TBool enableRtcp( EFalse );
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->PlayL( 0, streamPaused, enableRtcp ) );
    }

void UT_CMccSymDlStream::UT_CMccSymDlStream_PauseLL(  )
    {
    UT_CMccSymDlStream_PlayLL();
    
    TBool enableRtcp( EFalse );

    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->PauseL( 0, enableRtcp ) );
    }

void UT_CMccSymDlStream::UT_CMccSymDlStream_ResumeLL(  )
    {
    UT_CMccSymDlStream_PauseLL();
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->ResumeL( 0, ETrue ) );
    }

void UT_CMccSymDlStream::UT_CMccSymDlStream_StopLL(  )
    {
    UT_CMccSymDlStream_PlayLL();
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->StopL( 0 ) );
    }

void UT_CMccSymDlStream::UT_CMccSymDlStream_SetBalanceLL(  )
    {
    TInt left( 0 ), right( 0 );

    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->SetBalanceL( left, right ) );
    }

void UT_CMccSymDlStream::UT_CMccSymDlStream_GetBalanceLL(  )
    {
    TInt left( 0 ), right( 0 );

    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->GetBalanceL( left, right ) );
    }

void UT_CMccSymDlStream::UT_CMccSymDlStream_GetSSRCLL(  )
    {
    TUint32 ssrc;
    
    // KErrNotSupported
    EUNIT_ASSERT_LEAVE( iStream->GetSSRCL( ssrc ) );
    }

void UT_CMccSymDlStream::UT_CMccSymDlStream_ResetCountersLL(  )
    {
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->ResetCountersL() );
    }

void UT_CMccSymDlStream::UT_CMccSymDlStream_SendMediaSignalL()
    {
    TMccEvent mmcevent;
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->SendMediaSignalL(mmcevent) );
    
    iStream->iDatasource = NULL;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iStream->SendMediaSignalL(mmcevent), KErrNotReady  );

    }

void UT_CMccSymDlStream::UT_CMccSymDlStream_SendRTCPReceiverReportLL(  )
    {
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iStream->SendRTCPReceiverReportL(), KErrNotReady );
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->PrimeL(0) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->SendRTCPReceiverReportL() );
    iStream->iDatasource = NULL;
    EUNIT_ASSERT_LEAVE( iStream->SendRTCPReceiverReportL() );
    }

void UT_CMccSymDlStream::UT_CMccSymDlStream_SendRTCPSenderReportLL(  )
    {
    EUNIT_ASSERT_LEAVE( iStream->SendRTCPSenderReportL() );
    }

void UT_CMccSymDlStream::UT_CMccSymDlStream_StartInactivityTimerLL(  )
    {
    TUint32 timeoutTime( 5000 );

    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->StartInactivityTimerL( timeoutTime ) );
    }

void UT_CMccSymDlStream::UT_CMccSymDlStream_StopInactivityTimerLL(  )
    {
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->StopInactivityTimerL() );
    }
    
void UT_CMccSymDlStream::UT_CMccSymDlStream_AudioRoutingLL()
    {
    TUint32 route;
    
    // This actually works currently in emulator.
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->SetAudioRouteL( KMccAudioToPublic ) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->GetAudioRouteL( route ) );
    EUNIT_ASSERT( KMccAudioToPublic == route );
    
    // KMccAudioToPrivate
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->SetAudioRouteL( KMccAudioToPrivate ) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->GetAudioRouteL( route ) );
    EUNIT_ASSERT( KMccAudioToPrivate == route );
    }

void UT_CMccSymDlStream::UT_CMccSymDlStream_IsUplink()
    {
    EUNIT_ASSERT( !iStream->IsUplink() );
    }
    
void UT_CMccSymDlStream::UT_CMccSymDlStream_StandByL()
    {
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->StandByL() );
    iStream->iDatasource = NULL;
    EUNIT_ASSERT_LEAVE( iStream->StandByL() );

    }
    
void UT_CMccSymDlStream::UT_CMccSymDlStream_JitterBufferError()
    {
    TInt err = MJitterBufferObserver::EBufferOverflow;
    iStream->JitterBufferError( err );
    
    err = MJitterBufferObserver::EBufferUnderflow;
    iStream->JitterBufferError( err );
    }
    
void UT_CMccSymDlStream::UT_CMccSymDlStream_BindContextIntoStreamL()
    {
       
    TMccCryptoContext context;
    TUint32 endpointId = 0;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iStream->BindContextIntoStreamL( endpointId, context ), KErrNotFound );
    endpointId = (TUint32)( iStream->iDatasource );
    
    // Key ok (stubbed)
    iStream->BindContextIntoStreamL( endpointId, context );
    }
    
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccSymDlStream,
    "CMccSymDlStream",
    "UNIT" )

EUNIT_TEST(
    "LoadCodecL - test ",
    "CMccSymDlStream",
    "LoadCodecL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccSymDlStream_LoadCodecLL, Teardown)
    
EUNIT_TEST(
    "SetPrioritySettingsL - test ",
    "CMccSymDlStream",
    "SetPrioritySettingsL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymDlStream_SetPrioritySettingsLL, Teardown)

EUNIT_TEST(
    "CreatePayloadFormatDecoderL - test ",
    "CMccSymDlStream",
    "CreatePayloadFormatDecoderL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymDlStream_CreatePayloadFormatDecoderL, Teardown)
    
EUNIT_TEST(
    "DoDynamicFrameSizeChangeL - test ",
    "CMccSymDlStream",
    "DoDynamicFrameSizeChangeL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccSymDlStream_DoDynamicFrameSizeChangeL, Teardown)
    

EUNIT_TEST(
    "MultiplexerSink - test ",
    "CMccSymDlStream",
    "MultiplexerSink",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymDlStream_MultiplexerSink, Teardown)
    

EUNIT_TEST(
    "AddSinkAndSourceL - test ",
    "CMccSymDlStream",
    "AddSinkAndSourceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymDlStream_AddSinkAndSourceLL, Teardown)

EUNIT_TEST(
    "UpdateCodecInformationL - test ",
    "CMccSymDlStream",
    "UpdateCodecInformationL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccSymDlStream_UpdateCodecInformationLL, Teardown)

EUNIT_TEST(
    "IlbcModeChange - test ",
    "CMccSymDlStream",
    "IlbcModeChangeTestL",
    "FUNCTIONALITY",
    SetupIlbcL, IlbcModeChangeTestL, Teardown)
    
EUNIT_TEST(
    "PrimeL - test ",
    "CMccSymDlStream",
    "PrimeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymDlStream_PrimeLL, Teardown)

EUNIT_TEST(
    "PlayL - test ",
    "CMccSymDlStream",
    "PlayL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymDlStream_PlayLL, Teardown)

EUNIT_TEST(
    "PauseL - test ",
    "CMccSymDlStream",
    "PauseL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymDlStream_PauseLL, Teardown)

EUNIT_TEST(
    "ResumeL - test ",
    "CMccSymDlStream",
    "ResumeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymDlStream_ResumeLL, Teardown)

EUNIT_TEST(
    "StopL - test ",
    "CMccSymDlStream",
    "StopL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymDlStream_StopLL, Teardown)

EUNIT_TEST(
    "SetBalanceL - test ",
    "CMccSymDlStream",
    "SetBalanceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymDlStream_SetBalanceLL, Teardown)

EUNIT_TEST(
    "GetBalanceL - test ",
    "CMccSymDlStream",
    "GetBalanceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymDlStream_GetBalanceLL, Teardown)

EUNIT_TEST(
    "GetSSRCL - test ",
    "CMccSymDlStream",
    "GetSSRCL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymDlStream_GetSSRCLL, Teardown)

EUNIT_TEST(
    "ResetCountersL - test ",
    "CMccSymDlStream",
    "ResetCountersL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymDlStream_ResetCountersLL, Teardown)

EUNIT_TEST(
    "SendMediaSignalL - test ",
    "CMccSymDlStream",
    "SendMediaSignalL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymDlStream_SendMediaSignalL, Teardown)


EUNIT_TEST(
    "SendRTCPReceiverReportL - test ",
    "CMccSymDlStream",
    "SendRTCPReceiverReportL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymDlStream_SendRTCPReceiverReportLL, Teardown)

EUNIT_TEST(
    "SendRTCPSenderReportL - test ",
    "CMccSymDlStream",
    "SendRTCPSenderReportL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymDlStream_SendRTCPSenderReportLL, Teardown)

EUNIT_TEST(
    "StartInactivityTimerL - test ",
    "CMccSymDlStream",
    "StartInactivityTimerL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymDlStream_StartInactivityTimerLL, Teardown)

EUNIT_TEST(
    "StopInactivityTimerL - test ",
    "CMccSymDlStream",
    "StopInactivityTimerL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymDlStream_StopInactivityTimerLL, Teardown)

EUNIT_TEST(
    "AudioRoutingL - test ",
    "CMccSymDlStream",
    "AudioRoutingL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymDlStream_AudioRoutingLL, Teardown)
    
EUNIT_TEST(
    "IsUplink - test ",
    "CMccSymDlStream",
    "IsUplink",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymDlStream_IsUplink, Teardown)
    
EUNIT_TEST(
    "StandByL - test ",
    "CMccSymDlStream",
    "StandByL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymDlStream_StandByL, Teardown)
    
EUNIT_TEST(
    "JitterBufferError - test ",
    "CMccSymDlStream",
    "JitterBufferError",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymDlStream_JitterBufferError, Teardown)
    
EUNIT_TEST(
    "BindContextIntoStreamL - test ",
    "CMccSymDlStream",
    "BindContextIntoStreamL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymDlStream_BindContextIntoStreamL, Teardown)
    



EUNIT_END_TEST_TABLE

//  END OF FILE
