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
#include "UT_CMccDtmfDlStream.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "mccdtmfdlstream.h"
#include "mccuids.hrh"

#include "Mccrtpmanager.h"
#include "Mcctesteventhandler.h"
#include "mmccinterfacedef.h"
#include "mccresourcepool_stub.h"
#include "mcccodecconfigurator.h"
#include "mccmultiplexer.h"
#include "mccrtpmediaclock.h"

#include "mccunittestmacros.h"

const TUid KMccAnySink =  { KImplUidMccAnySink     };
const TUid KRtpDataSource  =  { KImplUidRtpDataSource }; 

// CONSTRUCTION
UT_CMccDtmfDlStream* UT_CMccDtmfDlStream::NewL()
    {
    UT_CMccDtmfDlStream* self = UT_CMccDtmfDlStream::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccDtmfDlStream* UT_CMccDtmfDlStream::NewLC()
    {
    UT_CMccDtmfDlStream* self = new( ELeave ) UT_CMccDtmfDlStream();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccDtmfDlStream::~UT_CMccDtmfDlStream()
    {
    }

// Default constructor
UT_CMccDtmfDlStream::UT_CMccDtmfDlStream()
    {
    }

// Second phase construct
void UT_CMccDtmfDlStream::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccDtmfDlStream::SetupL(  )
    {
    iResources = CMccResourcePoolStub::NewL();
    
    TFourCC fourCC;
    
    iStreamId = 1;
    iHandler = CMccTestEventHandler::NewL();
    iRtpMan = CMccRtpManager::NewL( *iHandler, *iResources, iMccSessionId );
    
    iSourceStub = MDataSource::NewSourceL( KRtpDataSource, KNullDesC8 );
	iSinkStub = MDataSink::NewSinkL( KMccAnySink, KNullDesC8 );
	iRtpMediaClock = CMccRtpMediaClock::NewL();
	
    iStream = CMccDtmfDlStream::NewLC( iStreamId, iHandler, iResources, iRtpMan, fourCC, 200, *iRtpMediaClock );
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

void UT_CMccDtmfDlStream::Teardown(  )
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

void UT_CMccDtmfDlStream::UT_CMccDtmfDlStream_SetPrioritySettingsLL(  )
    {
    TMMFPrioritySettings priority;
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->SetPrioritySettingsL( priority ) );
    }

void UT_CMccDtmfDlStream::UT_CMccDtmfDlStream_AddSinkAndSourceLL(  )
    {
    // These two will fail because of the NULL
    EUNIT_ASSERT_LEAVE( iStream->AddSinkAndSourceL( reinterpret_cast<MDataSink*>( this ), NULL ) );
    EUNIT_ASSERT_LEAVE( iStream->AddSinkAndSourceL( NULL, reinterpret_cast<MDataSource*>( this ) ) );
    }

void UT_CMccDtmfDlStream::UT_CMccDtmfDlStream_LoadCodecLL(  )
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

void UT_CMccDtmfDlStream::UT_CMccDtmfDlStream_PrimeLL(  )
    {
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->PrimeL( MCC_ENDPOINT_ID( iSourceStub ) ) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->PrimeL( MCC_ENDPOINT_ID( iSinkStub ) ) );
    }

void UT_CMccDtmfDlStream::UT_CMccDtmfDlStream_PlayLL(  )
    {
    UT_CMccDtmfDlStream_PrimeLL();
    
    TBool streamPaused( EFalse );
    TBool enableRtcp( EFalse );

    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->PlayL( MCC_ENDPOINT_ID( iSourceStub ), streamPaused, enableRtcp ) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->PlayL( MCC_ENDPOINT_ID( iSinkStub ), streamPaused, enableRtcp ) );
    }

void UT_CMccDtmfDlStream::UT_CMccDtmfDlStream_PauseLL(  )
    {
    UT_CMccDtmfDlStream_PlayLL();
    
    TBool enableRtcp( EFalse );

    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->PauseL( MCC_ENDPOINT_ID( iSourceStub ), enableRtcp ) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->PauseL( MCC_ENDPOINT_ID( iSinkStub ), enableRtcp ) );
    }

void UT_CMccDtmfDlStream::UT_CMccDtmfDlStream_ResumeLL(  )
    {
    UT_CMccDtmfDlStream_PauseLL();
    
    TBool enableRtcp( EFalse );

    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->ResumeL( MCC_ENDPOINT_ID( iSourceStub ), enableRtcp ) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->ResumeL( MCC_ENDPOINT_ID( iSinkStub ), enableRtcp ) );
    }

void UT_CMccDtmfDlStream::UT_CMccDtmfDlStream_StopLL(  )
    {
    UT_CMccDtmfDlStream_PlayLL();
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->StopL( MCC_ENDPOINT_ID( iSourceStub ) ) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->StopL( MCC_ENDPOINT_ID( iSinkStub ) ) );
    }

void UT_CMccDtmfDlStream::UT_CMccDtmfDlStream_ResetCountersLL(  )
    {
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->ResetCountersL() );
    }

void UT_CMccDtmfDlStream::UT_CMccDtmfDlStream_SetBalanceLL(  )
    {
    TInt left( 0 ), right( 0 );

    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->SetBalanceL( left, right ) );
    }

void UT_CMccDtmfDlStream::UT_CMccDtmfDlStream_GetBalanceLL(  )
    {
    TInt left( 0 ), right( 0 );

    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->GetBalanceL( left, right ) );
    }

void UT_CMccDtmfDlStream::UT_CMccDtmfDlStream_SendMediaSignalLL(  )
    {
    TMccDtmfEventData dtmfData;
    dtmfData.iDtmfString.Copy( _L8("1") );
    dtmfData.iDtmfEventType = KMccDtmfManualStart;
    
    TMccDtmfEventDataPackage dtmfDataPackage( dtmfData );
    
    // Create a media signal and send it 
    TMccEvent event( 1, 1, 1, 1, KMccEventCategoryDtmf, KMccDtmfControl,
        KErrNone, dtmfDataPackage );
    
    // Sending not possible via dtmf receive stream
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iStream->SendMediaSignalL( event ), KErrNotSupported );
    }

void UT_CMccDtmfDlStream::UT_CMccDtmfDlStream_StartInactivityTimerLL(  )
    {
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->StartInactivityTimerL( 5 ) );
    }

void UT_CMccDtmfDlStream::UT_CMccDtmfDlStream_StopInactivityTimerLL(  )
    {
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->StopInactivityTimerL( ) );
    }

void UT_CMccDtmfDlStream::UT_CMccDtmfDlStream_GetSSRCLL(  )
    {
    TUint32 ssrc;
    
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iStream->GetSSRCL( ssrc ), KErrNotSupported );
    }

void UT_CMccDtmfDlStream::UT_CMccDtmfDlStream_IsUplinkL(  )
    {
    EUNIT_ASSERT( iStream->IsUplink() == EFalse );
    }

void UT_CMccDtmfDlStream::UT_CMccDtmfDlStream_StandByLL(  )
    {
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->StandByL() );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccDtmfDlStream,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "SetPrioritySettingsL - test ",
    "CMccDtmfDlStream",
    "SetPrioritySettingsL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDtmfDlStream_SetPrioritySettingsLL, Teardown)

EUNIT_TEST(
    "AddSinkAndSourceL - test ",
    "CMccDtmfDlStream",
    "AddSinkAndSourceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDtmfDlStream_AddSinkAndSourceLL, Teardown)

EUNIT_TEST(
    "LoadCodecL - test ",
    "CMccDtmfDlStream",
    "LoadCodecL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDtmfDlStream_LoadCodecLL, Teardown)

EUNIT_TEST(
    "PrimeL - test ",
    "CMccDtmfDlStream",
    "PrimeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDtmfDlStream_PrimeLL, Teardown)

EUNIT_TEST(
    "PlayL - test ",
    "CMccDtmfDlStream",
    "PlayL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDtmfDlStream_PlayLL, Teardown)

EUNIT_TEST(
    "PauseL - test ",
    "CMccDtmfDlStream",
    "PauseL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDtmfDlStream_PauseLL, Teardown)

EUNIT_TEST(
    "ResumeL - test ",
    "CMccDtmfDlStream",
    "ResumeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDtmfDlStream_ResumeLL, Teardown)

EUNIT_TEST(
    "StopL - test ",
    "CMccDtmfDlStream",
    "StopL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDtmfDlStream_StopLL, Teardown)

EUNIT_TEST(
    "ResetCountersL - test ",
    "CMccDtmfDlStream",
    "ResetCountersL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDtmfDlStream_ResetCountersLL, Teardown)

EUNIT_TEST(
    "SetBalanceL - test ",
    "CMccDtmfDlStream",
    "SetBalanceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDtmfDlStream_SetBalanceLL, Teardown)

EUNIT_TEST(
    "GetBalanceL - test ",
    "CMccDtmfDlStream",
    "GetBalanceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDtmfDlStream_GetBalanceLL, Teardown)

EUNIT_TEST(
    "SendMediaSignalL - test ",
    "CMccDtmfDlStream",
    "SendMediaSignalL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDtmfDlStream_SendMediaSignalLL, Teardown)

EUNIT_TEST(
    "StartInactivityTimerL - test ",
    "CMccDtmfDlStream",
    "StartInactivityTimerL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDtmfDlStream_StartInactivityTimerLL, Teardown)

EUNIT_TEST(
    "StopInactivityTimerL - test ",
    "CMccDtmfDlStream",
    "StopInactivityTimerL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDtmfDlStream_StopInactivityTimerLL, Teardown)

EUNIT_TEST(
    "GetSSRCL - test ",
    "CMccDtmfDlStream",
    "GetSSRCL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDtmfDlStream_GetSSRCLL, Teardown)

EUNIT_TEST(
    "IsUplink - test ",
    "CMccDtmfDlStream",
    "IsUplink",
    "FUNCTIONALITY",
    SetupL, UT_CMccDtmfDlStream_IsUplinkL, Teardown)

EUNIT_TEST(
    "StandByL - test ",
    "CMccDtmfDlStream",
    "StandByL",
    "FUNCTIONALITY",
    SetupL, UT_CMccDtmfDlStream_StandByLL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
