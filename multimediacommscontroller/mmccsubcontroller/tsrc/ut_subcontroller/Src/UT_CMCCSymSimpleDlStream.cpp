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
#include "UT_CMccSymSimpleDlStream.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "Mccsymsimpledlstream.h"
#include "MMccInterfaceDef.h"
#include "Mccrtpmanager.h"
#include "MccRtpDatasource.h"
#include "MccRtpDataSink.h"
#include "Mcctesteventhandler.h"
#include "mccresourcepool_stub.h"
#include "mcccodecconfigurator.h"
#include "mccfilesink.h"
#include "mccrtpmediaclock.h"

#include "mccunittestmacros.h"

const TUid KRtpDataSource = { KImplUidRtpDataSource }; 
const TUid KRtpDataSink = { KImplUidRtpDataSink };
const TInt KAmrNbBitrate122 = 12200;

// CONSTRUCTION
UT_CMccSymSimpleDlStream* UT_CMccSymSimpleDlStream::NewL()
    {
    UT_CMccSymSimpleDlStream* self = UT_CMccSymSimpleDlStream::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccSymSimpleDlStream* UT_CMccSymSimpleDlStream::NewLC()
    {
    UT_CMccSymSimpleDlStream* self = new( ELeave ) UT_CMccSymSimpleDlStream();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccSymSimpleDlStream::~UT_CMccSymSimpleDlStream()
    {
    }

// Default constructor
UT_CMccSymSimpleDlStream::UT_CMccSymSimpleDlStream()
    {
    }

// Second phase construct
void UT_CMccSymSimpleDlStream::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccSymSimpleDlStream::SetupL(  )
    {
    iRtpMediaClock = CMccRtpMediaClock::NewL();
    
    iResources = CMccResourcePoolStub::NewL();
    iEventHandler = CMccTestEventHandler::NewL();
    iRtpManager = CMccRtpManager::NewL(*iEventHandler, *iResources, iMccSessionId );
    iStreamId = 1;
    
    CMccSymSimpleDlStream* stream = CMccSymSimpleDlStream::NewLC(
    	iStreamId, this, iResources, iRtpManager, TFourCC(),  KMccVideoDownlinkStream, *iRtpMediaClock );
    CleanupStack::Pop( stream );
    iSimpleDlStream = stream;
    stream = NULL;
    
    TPckgBuf<TInt> params( 30000 );
    iSourceStub = MDataSource::NewSourceL( KRtpDataSource, params );
    iSinkStub = CMccFileSink::NewSinkL( KRtpDataSink, KNullDesC8 );

    iSimpleDlStream->AddSinkAndSourceL( iSinkStub, iSourceStub );
    
    TMccCodecInfo cInfo;
    cInfo.iFourCC = TFourCC( KMccFourCCIdAMRNB );
    cInfo.iBitrate = KAmrNbBitrate122;
    cInfo.iHwFrameTime = 20;
    cInfo.iFrameSize = 32;
    cInfo.iPtime = 20;
    cInfo.iMaxPtime = 400; // recommended "limit" 200ms
    cInfo.iPayloadType = KMinDynamicPT;
    cInfo.iCodecMode = EBandwidthEfficient;
    cInfo.iPayloadFormatEncoder = KImplUidAmrPayloadFormatEncode;
    cInfo.iPayloadFormatDecoder = KImplUidAmrPayloadFormatDecode;
    _LIT8( KFmtp, "octet-align=0" );
    TBuf8<13> buf( KFmtp() );
    iSimpleDlStream->LoadCodecL( cInfo, buf );
    
    TUint port;
    iRtpManager->CreateSessionL( port, ETrue, EFalse );
    }

void UT_CMccSymSimpleDlStream::Teardown(  )
    {
    delete iSimpleDlStream;
    delete iSinkStub;
    delete iSourceStub;
    delete iRtpManager;
    delete iEventHandler;
    delete iResources;
    REComSession::FinalClose();
    
    if ( iRtpMediaClock )
        {
        delete iRtpMediaClock;
        iRtpMediaClock = NULL;
        }
    }

void UT_CMccSymSimpleDlStream::UT_CMccSymSimpleDlStream_CreatePayloadFormatDecoderLL(  )
    {   
    MCC_EUNIT_ASSERT_NO_LEAVE( iSimpleDlStream->CreatePayloadFormatDecoderL() );
    }

void UT_CMccSymSimpleDlStream::UT_CMccSymSimpleDlStream_SetPrioritySettingsLL(  )
    {
    TMMFPrioritySettings priority;
    iSimpleDlStream->SetPrioritySettingsL( priority );
    }

void UT_CMccSymSimpleDlStream::UT_CMccSymSimpleDlStream_AddSinkAndSourceLL(  )
    {
    MDataSink* sink = NULL;
    MDataSource* source = NULL;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iSimpleDlStream->AddSinkAndSourceL( sink, source ), KErrArgument );
    iSimpleDlStream->AddSinkAndSourceL(iSinkStub, iSourceStub);
    }

void UT_CMccSymSimpleDlStream::UT_CMccSymSimpleDlStream_LoadCodecLL(  )
    {  
    TMccCodecInfo cInfo;
    cInfo.iFourCC = TFourCC( KMccFourCCIdAMRNB );
    cInfo.iBitrate = KAmrNbBitrate122;
    cInfo.iHwFrameTime = 20;
    cInfo.iFrameSize = 32;
    cInfo.iPtime = 20;
    cInfo.iMaxPtime = 400; // recommended "limit" 200ms
    cInfo.iPayloadType = KMinDynamicPT;
    cInfo.iCodecMode = EBandwidthEfficient;
    cInfo.iPayloadFormatEncoder = KImplUidAmrPayloadFormatEncode;
    cInfo.iPayloadFormatDecoder = KImplUidAmrPayloadFormatDecode;
    _LIT8( KFmtp, "octet-align=0" );
    TBuf8<13> buf( KFmtp() );
    
    EUNIT_ASSERT( iSimpleDlStream->CurrentCodecState() == CMccSymStreamBase::EStateCodecNegotiated );
    MCC_EUNIT_ASSERT_NO_LEAVE( iSimpleDlStream->LoadCodecL( cInfo, buf ) );
    
    EUNIT_ASSERT( iSimpleDlStream->CurrentCodecState() == CMccSymStreamBase::EStateCodecPrepared );
    MCC_EUNIT_ASSERT_NO_LEAVE( iSimpleDlStream->LoadCodecL( cInfo, buf ) );

    EUNIT_ASSERT( iSimpleDlStream->CurrentCodecState() == CMccSymStreamBase::EStateCodecPrepared );
    iSimpleDlStream->UpdateCodecLoadingPermission( ETrue );
    MCC_EUNIT_ASSERT_NO_LEAVE( iSimpleDlStream->LoadCodecL( cInfo, buf ) );
    
    EUNIT_ASSERT( iSimpleDlStream->CurrentCodecState() == CMccSymStreamBase::EStateCodecLoaded );
    MCC_EUNIT_ASSERT_NO_LEAVE( iSimpleDlStream->LoadCodecL( cInfo, buf ) );
    }

void UT_CMccSymSimpleDlStream::UT_CMccSymSimpleDlStream_UpdateCodecInformationLL(  )
    {
    TMccCodecInfo cInfo;
    cInfo.iFourCC = TFourCC( KMccFourCCIdAMRNB );
    cInfo.iBitrate = KAmrNbBitrate122;
    cInfo.iHwFrameTime = 20;
    cInfo.iFrameSize = 32;
    cInfo.iPtime = 40;
    cInfo.iMaxPtime = 400; // recommended "limit" 200ms
    cInfo.iPayloadType = KMinDynamicPT;
    cInfo.iCodecMode = EBandwidthEfficient;
    cInfo.iPayloadFormatEncoder = KImplUidAmrPayloadFormatEncode;
    cInfo.iPayloadFormatDecoder = KImplUidAmrPayloadFormatDecode;
    _LIT8( KFmtp, "octet-align=0" );
    TBuf8<13> buf( KFmtp() );
    
    MCC_EUNIT_ASSERT_NO_LEAVE(iSimpleDlStream->UpdateCodecInformationL( cInfo ));
    }

void UT_CMccSymSimpleDlStream::UT_CMccSymSimpleDlStream_PrimeLL(  )
    {
    MCC_EUNIT_ASSERT_NO_LEAVE(iSimpleDlStream->PrimeL(5));
    }

void UT_CMccSymSimpleDlStream::UT_CMccSymSimpleDlStream_PlayLL(  )
    {
    UT_CMccSymSimpleDlStream_PrimeLL();
    MCC_EUNIT_ASSERT_NO_LEAVE(iSimpleDlStream->PlayL(5, ETrue, EFalse));
    }

void UT_CMccSymSimpleDlStream::UT_CMccSymSimpleDlStream_PauseLL(  )
    {
    UT_CMccSymSimpleDlStream_PlayLL();
    MCC_EUNIT_ASSERT_NO_LEAVE(iSimpleDlStream->PauseL(5, EFalse));
    }

void UT_CMccSymSimpleDlStream::UT_CMccSymSimpleDlStream_ResumeLL(  )
    {
    UT_CMccSymSimpleDlStream_PauseLL();
    MCC_EUNIT_ASSERT_NO_LEAVE(iSimpleDlStream->ResumeL(5, EFalse));
    }

void UT_CMccSymSimpleDlStream::UT_CMccSymSimpleDlStream_StopLL(  )
    {
    UT_CMccSymSimpleDlStream_PlayLL();
    MCC_EUNIT_ASSERT_NO_LEAVE(iSimpleDlStream->StopL(5));
    }

void UT_CMccSymSimpleDlStream::UT_CMccSymSimpleDlStream_SetBalanceLL(  )
    {
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(iSimpleDlStream->SetBalanceL(5,10),KErrNotReady);
    }

void UT_CMccSymSimpleDlStream::UT_CMccSymSimpleDlStream_GetBalanceLL(  )
    {
    TInt left = 0;
    TInt right = 0;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(iSimpleDlStream->GetBalanceL(left,right),KErrNotReady);
    }

void UT_CMccSymSimpleDlStream::UT_CMccSymSimpleDlStream_GetSSRCLL(  )
    {
    TUint32 ssrcValue;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(iSimpleDlStream->GetSSRCL(ssrcValue),KErrNotSupported);
    }

void UT_CMccSymSimpleDlStream::UT_CMccSymSimpleDlStream_ResetCountersLL(  )
    {
	iSimpleDlStream->ResetCountersL();
    }

void UT_CMccSymSimpleDlStream::UT_CMccSymSimpleDlStream_SendRTCPReceiverReportLL(  )
    {
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(iSimpleDlStream->SendRTCPReceiverReportL(), KErrNotReady );
    
    iSimpleDlStream->PrimeL( 0 );
    MCC_EUNIT_ASSERT_NO_LEAVE(iSimpleDlStream->SendRTCPReceiverReportL() );
    
    iSimpleDlStream->iDatasource = NULL;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(iSimpleDlStream->SendRTCPReceiverReportL(), KErrArgument );
    }

void UT_CMccSymSimpleDlStream::UT_CMccSymSimpleDlStream_SendRTCPSenderReportLL(  )
    {
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(iSimpleDlStream->SendRTCPSenderReportL(), KErrNotSupported);

    }

void UT_CMccSymSimpleDlStream::UT_CMccSymSimpleDlStream_SendRTCPReceiverReportL(  )
    {
    MCC_EUNIT_ASSERT_NO_LEAVE(iSimpleDlStream->SendRTCPReceiverReportL() );
    }

void UT_CMccSymSimpleDlStream::UT_CMccSymSimpleDlStream_StartInactivityTimerLL(  )
    {
    iSimpleDlStream->StartInactivityTimerL(80000);    
    }

void UT_CMccSymSimpleDlStream::UT_CMccSymSimpleDlStream_StopInactivityTimerLL(  )
    {
    iSimpleDlStream->StopInactivityTimerL();     
    }

void UT_CMccSymSimpleDlStream::UT_CMccSymSimpleDlStream_IsUplinkL(  )
    {
    EUNIT_ASSERT( !iSimpleDlStream->IsUplink() );
    }

void UT_CMccSymSimpleDlStream::UT_CMccSymSimpleDlStream_StandByLL(  )
    {
    iSimpleDlStream->StandByL();    
    }

void UT_CMccSymSimpleDlStream::UT_CMccSymSimpleDlStream_JitterBufferErrorL(  )
    {
    iSimpleDlStream->JitterBufferError(3);
    }

void UT_CMccSymSimpleDlStream::UT_CMccSymSimpleDlStream_SendEventToClientL()
    {
    TMMFEvent mmfEvent;
    iSimpleDlStream->SendEventToClient( mmfEvent );
    
    TMccEvent event;
    TMccInternalEvent internalEvent;
    internalEvent.iEventType = KUidMediaTypeAudio;
    internalEvent.iMccEvent = &event;
    iSimpleDlStream->SendEventToClient( internalEvent );
    }
    
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccSymSimpleDlStream,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "CreatePayloadFormatDecoderL - test ",
    "CMccSymSimpleDlStream",
    "CreatePayloadFormatDecoderL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSimpleDlStream_CreatePayloadFormatDecoderLL, Teardown)

EUNIT_TEST(
    "SetPrioritySettingsL - test ",
    "CMccSymSimpleDlStream",
    "SetPrioritySettingsL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSimpleDlStream_SetPrioritySettingsLL, Teardown)

EUNIT_TEST(
    "AddSinkAndSourceL - test ",
    "CMccSymSimpleDlStream",
    "AddSinkAndSourceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSimpleDlStream_AddSinkAndSourceLL, Teardown)

EUNIT_TEST(
    "LoadCodecL - test ",
    "CMccSymSimpleDlStream",
    "LoadCodecL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSimpleDlStream_LoadCodecLL, Teardown)

EUNIT_TEST(
    "UpdateCodecInformationL - test ",
    "CMccSymSimpleDlStream",
    "UpdateCodecInformationL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSimpleDlStream_UpdateCodecInformationLL, Teardown)

EUNIT_TEST(
    "PrimeL - test ",
    "CMccSymSimpleDlStream",
    "PrimeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSimpleDlStream_PrimeLL, Teardown)

EUNIT_TEST(
    "PlayL - test ",
    "CMccSymSimpleDlStream",
    "PlayL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSimpleDlStream_PlayLL, Teardown)

EUNIT_TEST(
    "PauseL - test ",
    "CMccSymSimpleDlStream",
    "PauseL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSimpleDlStream_PauseLL, Teardown)

EUNIT_TEST(
    "ResumeL - test ",
    "CMccSymSimpleDlStream",
    "ResumeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSimpleDlStream_ResumeLL, Teardown)

EUNIT_TEST(
    "StopL - test ",
    "CMccSymSimpleDlStream",
    "StopL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSimpleDlStream_StopLL, Teardown)

EUNIT_TEST(
    "SetBalanceL - test ",
    "CMccSymSimpleDlStream",
    "SetBalanceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSimpleDlStream_SetBalanceLL, Teardown)

EUNIT_TEST(
    "GetBalanceL - test ",
    "CMccSymSimpleDlStream",
    "GetBalanceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSimpleDlStream_GetBalanceLL, Teardown)

EUNIT_TEST(
    "GetSSRCL - test ",
    "CMccSymSimpleDlStream",
    "GetSSRCL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSimpleDlStream_GetSSRCLL, Teardown)

EUNIT_TEST(
    "ResetCountersL - test ",
    "CMccSymSimpleDlStream",
    "ResetCountersL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSimpleDlStream_ResetCountersLL, Teardown)

EUNIT_TEST(
    "SendRTCPReceiverReportL - test ",
    "CMccSymSimpleDlStream",
    "SendRTCPReceiverReportL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSimpleDlStream_SendRTCPReceiverReportLL, Teardown)

EUNIT_TEST(
    "SendRTCPSenderReportL - test ",
    "CMccSymSimpleDlStream",
    "SendRTCPSenderReportL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSimpleDlStream_SendRTCPSenderReportLL, Teardown)


EUNIT_TEST(
    "StartInactivityTimerL - test ",
    "CMccSymSimpleDlStream",
    "StartInactivityTimerL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSimpleDlStream_StartInactivityTimerLL, Teardown)

EUNIT_TEST(
    "StopInactivityTimerL - test ",
    "CMccSymSimpleDlStream",
    "StopInactivityTimerL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSimpleDlStream_StopInactivityTimerLL, Teardown)

EUNIT_TEST(
    "IsUplink - test ",
    "CMccSymSimpleDlStream",
    "IsUplink",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSimpleDlStream_IsUplinkL, Teardown)

EUNIT_TEST(
    "StandByL - test ",
    "CMccSymSimpleDlStream",
    "StandByL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSimpleDlStream_StandByLL, Teardown)

EUNIT_TEST(
    "JitterBufferError - test ",
    "CMccSymSimpleDlStream",
    "JitterBufferError",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSimpleDlStream_JitterBufferErrorL, Teardown)

EUNIT_TEST(
    "SendEventToClient - test ",
    "CMccSymSimpleDlStream",
    "SendEventToClient",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSimpleDlStream_SendEventToClientL, Teardown)
    

EUNIT_END_TEST_TABLE

//  END OF FILE
