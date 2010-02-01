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
#include "UT_CMccDtmfUlStream.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "mccdtmfulstream.h"
#include "mccuids.hrh"

#include "Mccrtpmanager.h"
#include "Mcctesteventhandler.h"
#include "mmccinterfacedef.h"
#include "mccresourcepool_stub.h"
#include "mcccodecconfigurator.h"
#include "mccmultiplexer.h"
#include "mccrtpmediaclock.h"

#include "mccunittestmacros.h"

const TUid KRtpDataSink =  { KImplUidRtpDataSink     };
const TUid KMccAnySource  =  { KImplUidMccAnySource }; 

// CONSTRUCTION
UT_CMccDtmfUlStream* UT_CMccDtmfUlStream::NewL()
    {
    UT_CMccDtmfUlStream* self = UT_CMccDtmfUlStream::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccDtmfUlStream* UT_CMccDtmfUlStream::NewLC()
    {
    UT_CMccDtmfUlStream* self = new( ELeave ) UT_CMccDtmfUlStream();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccDtmfUlStream::~UT_CMccDtmfUlStream()
    {
    }

// Default constructor
UT_CMccDtmfUlStream::UT_CMccDtmfUlStream()
    {
    }

// Second phase construct
void UT_CMccDtmfUlStream::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccDtmfUlStream::SetupL(  )
    {
    iResources = CMccResourcePoolStub::NewL();
    
    TFourCC fourCC;
    
    iStreamId = 1;
    iHandler = CMccTestEventHandler::NewL();
    iRtpMan = CMccRtpManager::NewL( *iHandler, *iResources, iMccSessionId );
    
    iSourceStub = MDataSource::NewSourceL( KMccAnySource, KNullDesC8 );
	iSinkStub = MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 );
	iRtpMediaClock = CMccRtpMediaClock::NewL();
	
    iStream = CMccDtmfUlStream::NewLC( iStreamId, iHandler, iResources, iRtpMan, fourCC, 200, *iRtpMediaClock );
    CleanupStack::Pop( iStream );

    iStream->AddSinkAndSourceL( iSinkStub, iSourceStub );
    
    iCInfo.iType = KUidMediaTypeAudio;
    iCInfo.iFourCC = TFourCC( KMccFourCCIdDTMF );
    //iCInfo.iBitrate = KAmrNbBitrate122;
    //iCInfo.iHwFrameTime = 20;
    //iCInfo.iFrameSize = 32;
    iCInfo.iPtime = 20;
    iCInfo.iMaxPtime = 200;
    iCInfo.iPayloadType = KMinDynamicPT;
    iCInfo.iCodecMode = EDTMFModeEvent;
    iCInfo.iPayloadFormatEncoder = KImplUidDTMFPayloadFormatEncode;
    iCInfo.iPayloadFormatDecoder = KImplUidDTMFPayloadFormatDecode;
    _LIT8( KFmtp, "0-15" );
    TBuf8<13> buf( KFmtp() );
    iStream->LoadCodecL( iCInfo, buf );
    
    TUint port;
    iRtpMan->CreateSessionL( port, ETrue, EFalse );
    }

void UT_CMccDtmfUlStream::Teardown(  )
    {
    delete iStream;
    delete iSinkStub;
    delete iSourceStub;
    delete iRtpMan;
    delete iHandler;
    delete iResources;
    delete iRtpMediaClock;
    
    REComSession::FinalClose();
    }

void UT_CMccDtmfUlStream::UT_CMccDtmfUlStream_SetPrioritySettingsLL(  )
    {
    TMMFPrioritySettings priority;
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->SetPrioritySettingsL( priority ) );
    }

void UT_CMccDtmfUlStream::UT_CMccDtmfUlStream_AddSinkAndSourceLL(  )
    {
    // These two will fail because of the NULL
    EUNIT_ASSERT_LEAVE( iStream->AddSinkAndSourceL( reinterpret_cast<MDataSink*>( this ), NULL ) );
    EUNIT_ASSERT_LEAVE( iStream->AddSinkAndSourceL( NULL, reinterpret_cast<MDataSource*>( this ) ) );
    }

void UT_CMccDtmfUlStream::UT_CMccDtmfUlStream_LoadCodecLL(  )
    {
    TMccCodecInfo cInfo;
    cInfo.iType = KUidMediaTypeAudio;
    cInfo.iFourCC = TFourCC( KMccFourCCIdDTMF );
    //iCInfo.iBitrate = KAmrNbBitrate122;
    //iCInfo.iHwFrameTime = 20;
    //iCInfo.iFrameSize = 32;
    cInfo.iPtime = 20;
    cInfo.iMaxPtime = 200;
    cInfo.iPayloadType = KMinDynamicPT;
    cInfo.iCodecMode = EDTMFModeEvent;
    cInfo.iPayloadFormatEncoder = KImplUidDTMFPayloadFormatEncode;
    cInfo.iPayloadFormatDecoder = KImplUidDTMFPayloadFormatDecode;
    _LIT8( KFmtp, "0-15" );
    TBuf8<13> buf( KFmtp() );
    
    // Right state, "right" codec
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->LoadCodecL( cInfo, buf ) );
    
    // Right state, wrong codec
    cInfo.iFourCC = TFourCC();
    
    EUNIT_ASSERT_LEAVE( iStream->LoadCodecL( cInfo, buf ) );

    cInfo.iFourCC = KMccFourCCIdAMRNB;

    // Wrong state, right codec
    EUNIT_ASSERT_LEAVE( iStream->LoadCodecL( cInfo, buf ) );
    }

void UT_CMccDtmfUlStream::UT_CMccDtmfUlStream_PrimeLL(  )
    {
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->PrimeL( MCC_ENDPOINT_ID( iSinkStub ) ) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->PrimeL( MCC_ENDPOINT_ID( iSourceStub ) ) );
    }

void UT_CMccDtmfUlStream::UT_CMccDtmfUlStream_PlayLL(  )
    {
    UT_CMccDtmfUlStream_PrimeLL();
    
    TBool streamPaused( EFalse );
    TBool enableRtcp( EFalse );

    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->PlayL( 0, streamPaused, enableRtcp ) );
    }

void UT_CMccDtmfUlStream::UT_CMccDtmfUlStream_PauseLL(  )
    {
    UT_CMccDtmfUlStream_PlayLL();
    
    TBool enableRtcp( EFalse );

    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->PauseL( MCC_ENDPOINT_ID( iSinkStub ), enableRtcp ) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->PauseL( MCC_ENDPOINT_ID( iSourceStub ), enableRtcp ) );
    }

void UT_CMccDtmfUlStream::UT_CMccDtmfUlStream_ResumeLL(  )
    {
    UT_CMccDtmfUlStream_PauseLL();
    
    TBool enableRtcp( EFalse );

    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->ResumeL( MCC_ENDPOINT_ID( iSinkStub ), enableRtcp ) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->ResumeL( MCC_ENDPOINT_ID( iSourceStub ), enableRtcp ) );
    }

void UT_CMccDtmfUlStream::UT_CMccDtmfUlStream_StopLL(  )
    {
    UT_CMccDtmfUlStream_PlayLL();
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->StopL( MCC_ENDPOINT_ID( iSinkStub ) ) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->StopL( MCC_ENDPOINT_ID( iSourceStub ) ) );
    }

void UT_CMccDtmfUlStream::UT_CMccDtmfUlStream_ResetCountersLL(  )
    {
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->ResetCountersL() );
    }

void UT_CMccDtmfUlStream::UT_CMccDtmfUlStream_SetBalanceLL(  )
    {
    TInt left( 0 ), right( 0 );

    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->SetBalanceL( left, right ) );
    }

void UT_CMccDtmfUlStream::UT_CMccDtmfUlStream_GetBalanceLL(  )
    {
    TInt left( 0 ), right( 0 );

    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->GetBalanceL( left, right ) );
    }

void UT_CMccDtmfUlStream::UT_CMccDtmfUlStream_SendMediaSignalLL(  )
    {
    // Manual start
    TMccDtmfEventData dtmfData;
    dtmfData.iDtmfString.Copy( _L8("1") );
    dtmfData.iDtmfEventType = KMccDtmfManualStart;
    TMccDtmfEventDataPackage dtmfDataPackage( dtmfData );

    TMccEvent event( 1, 1, 1, 1, KMccEventCategoryDtmf, KMccDtmfControl,
        KErrNone, dtmfDataPackage );
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->SendMediaSignalL( event ) );

    // Start tone
    dtmfData.iDtmfEventType = KMccDtmfSigStartTone;
    dtmfDataPackage = dtmfData;
    event.iEventData.Copy( dtmfDataPackage );
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->SendMediaSignalL( event ) );
    
    // Stop tone
    dtmfData.iDtmfEventType = KMccDtmfSigStopTone;
    dtmfDataPackage = dtmfData;
    event.iEventData.Copy( dtmfDataPackage );
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->SendMediaSignalL( event ) );
    
    // Send string
    dtmfData.iDtmfEventType = KMccDtmfSigSendString;
    dtmfDataPackage = dtmfData;
    event.iEventData.Copy( dtmfDataPackage );
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->SendMediaSignalL( event ) );
    
    // Cancel sending
    dtmfData.iDtmfEventType = KMccDtmfSigCancelSending;
    dtmfDataPackage = dtmfData;
    event.iEventData.Copy( dtmfDataPackage );
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->SendMediaSignalL( event ) );
    
    // Continue sending
    dtmfData.iDtmfEventType = KMccDtmfSigContinueSending;
    dtmfDataPackage = dtmfData;
    event.iEventData.Copy( dtmfDataPackage );
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->SendMediaSignalL( event ) );
    
    // Unsupported
    TMccRtcpEventData rtcpData;
    rtcpData.iRtcpPacketData.Copy( _L8("foofoo") );
    rtcpData.iRtcpPacketType = KRtcpAnyPacket;
    rtcpData.iSsrc = 2;
    
    TMccRtcpEventDataPackage rtcpDataPackage( rtcpData );
    TMccEvent event2( 1, 1, 1, 1, KMccEventCategoryRtcp, KMccRtcpReceived,
                         KErrNone, rtcpDataPackage );

    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iStream->SendMediaSignalL( event2 ), KErrNotSupported );
    }

void UT_CMccDtmfUlStream::UT_CMccDtmfUlStream_StartInactivityTimerLL(  )
    {
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->StartInactivityTimerL( 5 ) );
    }

void UT_CMccDtmfUlStream::UT_CMccDtmfUlStream_StopInactivityTimerLL(  )
    {
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->StopInactivityTimerL( ) );
    }

void UT_CMccDtmfUlStream::UT_CMccDtmfUlStream_GetSSRCLL(  )
    {
    TUint32 ssrc;
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->GetSSRCL( ssrc ) );
    }

void UT_CMccDtmfUlStream::UT_CMccDtmfUlStream_IsUplinkL(  )
    {
    EUNIT_ASSERT( iStream->IsUplink() == ETrue );
    }

void UT_CMccDtmfUlStream::UT_CMccDtmfUlStream_StandByLL(  )
    {
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->StandByL() );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccDtmfUlStream,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "SetPrioritySettingsL - test ",
    "CMccDtmfUlStream",
    "SetPrioritySettingsL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDtmfUlStream_SetPrioritySettingsLL, Teardown)

EUNIT_TEST(
    "AddSinkAndSourceL - test ",
    "CMccDtmfUlStream",
    "AddSinkAndSourceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDtmfUlStream_AddSinkAndSourceLL, Teardown)

EUNIT_TEST(
    "LoadCodecL - test ",
    "CMccDtmfUlStream",
    "LoadCodecL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDtmfUlStream_LoadCodecLL, Teardown)

EUNIT_TEST(
    "PrimeL - test ",
    "CMccDtmfUlStream",
    "PrimeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDtmfUlStream_PrimeLL, Teardown)

EUNIT_TEST(
    "PlayL - test ",
    "CMccDtmfUlStream",
    "PlayL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDtmfUlStream_PlayLL, Teardown)

EUNIT_TEST(
    "PauseL - test ",
    "CMccDtmfUlStream",
    "PauseL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDtmfUlStream_PauseLL, Teardown)

EUNIT_TEST(
    "ResumeL - test ",
    "CMccDtmfUlStream",
    "ResumeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDtmfUlStream_ResumeLL, Teardown)

EUNIT_TEST(
    "StopL - test ",
    "CMccDtmfUlStream",
    "StopL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDtmfUlStream_StopLL, Teardown)

EUNIT_TEST(
    "ResetCountersL - test ",
    "CMccDtmfUlStream",
    "ResetCountersL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDtmfUlStream_ResetCountersLL, Teardown)

EUNIT_TEST(
    "SetBalanceL - test ",
    "CMccDtmfUlStream",
    "SetBalanceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDtmfUlStream_SetBalanceLL, Teardown)

EUNIT_TEST(
    "GetBalanceL - test ",
    "CMccDtmfUlStream",
    "GetBalanceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDtmfUlStream_GetBalanceLL, Teardown)

EUNIT_TEST(
    "SendMediaSignalL - test ",
    "CMccDtmfUlStream",
    "SendMediaSignalL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDtmfUlStream_SendMediaSignalLL, Teardown)

EUNIT_TEST(
    "StartInactivityTimerL - test ",
    "CMccDtmfUlStream",
    "StartInactivityTimerL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDtmfUlStream_StartInactivityTimerLL, Teardown)

EUNIT_TEST(
    "StopInactivityTimerL - test ",
    "CMccDtmfUlStream",
    "StopInactivityTimerL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDtmfUlStream_StopInactivityTimerLL, Teardown)

EUNIT_TEST(
    "GetSSRCL - test ",
    "CMccDtmfUlStream",
    "GetSSRCL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDtmfUlStream_GetSSRCLL, Teardown)

EUNIT_TEST(
    "IsUplink - test ",
    "CMccDtmfUlStream",
    "IsUplink",
    "FUNCTIONALITY",
    SetupL, UT_CMccDtmfUlStream_IsUplinkL, Teardown)

EUNIT_TEST(
    "StandByL - test ",
    "CMccDtmfUlStream",
    "StandByL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDtmfUlStream_StandByLL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
