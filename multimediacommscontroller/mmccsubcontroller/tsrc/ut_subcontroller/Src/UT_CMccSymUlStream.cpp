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
#include "UT_CMccSymUlStream.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "Mccsymulstream.h"
#include "Mccrtpmanager.h"
#include "Mcctesteventhandler.h"
#include "MmccEvents.h"
#include "MccRtpDataSink.h"
#include "mccresourcepool_stub.h"
#include "mcccodecconfigurator.h"
#include "mccunittestmacros.h"
#include "mccrtpmediaclock.h"
#include "mmcccryptocontext.h"

#include "UT_CMccSymSubthreadClient.h"

#include "mccunittestmacros.h"


const TUid KRtpDataSink = { KImplUidRtpDataSink };
const TInt KAmrNbBitrate122 = 12200;
const TInt KAmrNbBitrate102 = 10200;
const TInt KG711Bitrate64 = 64000;

// CONSTRUCTION
UT_CMccSymUlStream* UT_CMccSymUlStream::NewL()
    {
    UT_CMccSymUlStream* self = UT_CMccSymUlStream::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccSymUlStream* UT_CMccSymUlStream::NewLC()
    {
    UT_CMccSymUlStream* self = new( ELeave ) UT_CMccSymUlStream();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccSymUlStream::~UT_CMccSymUlStream()
    {
    }

// Default constructor
UT_CMccSymUlStream::UT_CMccSymUlStream()
    {
    }

// Second phase construct
void UT_CMccSymUlStream::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccSymUlStream::SetupL(  )
    {
    iResources = CMccResourcePoolStub::NewL();
    
    TFourCC fourCC;
    
    iStreamId = 1;
    iHandler = CMccTestEventHandler::NewL();
    iRtpMan = CMccRtpManager::NewL( *iHandler, *iResources, iMccSessionId );
    iSourceStub =  MDataSource::NewSourceL( KUidMmfAudioInput, KNullDesC8 );
	iSinkStub = MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 );
	iRtpMediaClock = CMccRtpMediaClock::NewL();
	
    iStream = CMccSymUlStream::NewLC( iStreamId, iHandler, iResources, iRtpMan, fourCC, 200, *iRtpMediaClock );
    CleanupStack::Pop( iStream );

    iStream->AddSinkAndSourceL( iSinkStub, iSourceStub );
    
    iCInfo.iType = KUidMediaTypeAudio;
    iCInfo.iFourCC = TFourCC( KMccFourCCIdAMRNB );
    iCInfo.iBitrate = KAmrNbBitrate122;
    iCInfo.iHwFrameTime = 20;
    iCInfo.iFrameSize = 32;
    iCInfo.iPtime = 20;
    iCInfo.iMaxPtime = 400; // recommended "limit" 200ms
    iCInfo.iPayloadType = KMinDynamicPT;
    iCInfo.iCodecMode = EBandwidthEfficient;
    iCInfo.iPayloadFormatEncoder = KImplUidAmrPayloadFormatEncode;
    iCInfo.iPayloadFormatDecoder = KImplUidAmrPayloadFormatDecode;
    _LIT8( KFmtp, "octet-align=0" );
    TBuf8<13> buf( KFmtp() );
    iStream->LoadCodecL( iCInfo, buf );
    
    TUint port;
    iRtpMan->CreateSessionL( port, ETrue, EFalse );
    }
    
void UT_CMccSymUlStream::Setup2L(  )
    {
    iResources = CMccResourcePoolStub::NewL();
    
    TFourCC fourCC;
    
    iStreamId = 1;
    iHandler = CMccTestEventHandler::NewL();
    iRtpMan = CMccRtpManager::NewL( *iHandler, *iResources, iMccSessionId );
    iSourceStub =  MDataSource::NewSourceL( KUidMmfAudioInput, KNullDesC8 );
	iSinkStub = MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 );
	iRtpMediaClock = CMccRtpMediaClock::NewL();
	
    iStream = CMccSymUlStream::NewLC( iStreamId, iHandler, iResources, iRtpMan, fourCC, 200, *iRtpMediaClock );
    CleanupStack::Pop( iStream );

    iStream->AddSinkAndSourceL( iSinkStub, iSourceStub );
    
    TUint port;
    iRtpMan->CreateSessionL( port, ETrue, EFalse );
    }

void UT_CMccSymUlStream::Teardown(  )
    {    
    if ( iStream )
        {
        delete iStream;
        }
        
    delete iSinkStub;
    delete iSourceStub;
    
    if ( iRtpMan )
        {
        delete iRtpMan;
        }
    if ( iHandler )
        {
        delete iHandler;
        }
    delete iResources;
    delete iRtpMediaClock;
    REComSession::FinalClose();
    }

void UT_CMccSymUlStream::UT_CMccSymUlStream_SetPrioritySettingsLL(  )
    {
    TMMFPrioritySettings priority;
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->SetPrioritySettingsL( priority ) );
    }

void UT_CMccSymUlStream::UT_CMccSymUlStream_AddSinkAndSourceLL(  )
    {
    // These two will fail because of the NULL    
    EUNIT_ASSERT_LEAVE( iStream->AddSinkAndSourceL( reinterpret_cast<MDataSink*>( this ), NULL ) );
    EUNIT_ASSERT_LEAVE( iStream->AddSinkAndSourceL( NULL, reinterpret_cast<MDataSource*>( this ) ) );
    }

void UT_CMccSymUlStream::UT_CMccSymUlStream_LoadCodecLL(  )
    {
    TMccCodecInfo cInfo;
    cInfo.iType = KUidMediaTypeAudio;
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

    // Right state, right codec
    EUNIT_ASSERT( iStream->CurrentCodecState() == CMccSymStreamBase::EStateCodecNone );  
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->LoadCodecL( cInfo, buf ) );
    
    EUNIT_ASSERT( iStream->CurrentCodecState() == CMccSymStreamBase::EStateCodecNegotiated );
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->LoadCodecL( cInfo, buf ) );
    
    EUNIT_ASSERT( iStream->CurrentCodecState() == CMccSymStreamBase::EStateCodecPrepared );
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->LoadCodecL( cInfo, buf ) );
    EUNIT_ASSERT( iStream->CurrentCodecState() == CMccSymStreamBase::EStateCodecPrepared );

    EUNIT_ASSERT( iStream->CurrentCodecState() == CMccSymStreamBase::EStateCodecPrepared );
    iStream->UpdateCodecLoadingPermission( ETrue );
    iStream->LoadCodecL( cInfo, buf );
    
    EUNIT_ASSERT( iStream->CurrentCodecState() == CMccSymStreamBase::EStateCodecLoaded );
    iStream->LoadCodecL( cInfo, buf );
    
    cInfo.iKeepalivePT = 96; 
    iRtpMan->CreateTransmitStreamL( *iSinkStub, cInfo );
    iStream->LoadCodecL( cInfo, buf );
    }

void UT_CMccSymUlStream::UT_CMccSymUlStream_UpdateCodecInformationLL(  )
    {

    // iFormatDecode == NULL
    iStream->PrimeL( 0 );
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->UpdateCodecInformationL( iCInfo ) );
    }

void UT_CMccSymUlStream::UT_CMccSymUlStream_PrimeLL(  )
    {
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->PrimeL( 0 ) );
    }

void UT_CMccSymUlStream::UT_CMccSymUlStream_PlayLL(  )
    {
    UT_CMccSymUlStream_PrimeLL();
    
    TBool streamPaused( EFalse );
    TBool enableRtcp( EFalse );

    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->PlayL( 0, streamPaused, enableRtcp ) );
    }

void UT_CMccSymUlStream::UT_CMccSymUlStream_PauseLL(  )
    {
    UT_CMccSymUlStream_PlayLL();
    
    TBool enableRtcp( EFalse );

    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->PauseL( 0, enableRtcp ) );
    }

void UT_CMccSymUlStream::UT_CMccSymUlStream_ResumeLL(  )
    {
    UT_CMccSymUlStream_PauseLL();
    
    TBool enableRtcp( EFalse );

    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->ResumeL( 0, enableRtcp ) );
    }

void UT_CMccSymUlStream::UT_CMccSymUlStream_StopLL(  )
    {
    UT_CMccSymUlStream_PlayLL();
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->StopL( 0 ) );
    }

void UT_CMccSymUlStream::UT_CMccSymUlStream_SetBalanceLL(  )
    {
    TInt left( 0 ), right( 0 );

    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->SetBalanceL( left, right ) );
    }

void UT_CMccSymUlStream::UT_CMccSymUlStream_GetBalanceLL(  )
    {
    TInt left( 0 ), right( 0 );

    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->GetBalanceL( left, right ) );
    }

void UT_CMccSymUlStream::UT_CMccSymUlStream_SendMediaSignalLL(  )
    {
    TMccDtmfEventData dtmfData;
    dtmfData.iDtmfString.Copy( _L8("foofoo") );
    dtmfData.iDtmfEventType = KMccDtmfManualStart;
    
    TMccDtmfEventDataPackage dtmfDataPackage( dtmfData );
    
    //Create a media signal and send it 
    TMccEvent event( 1, 1, 1, 1, KMccEventCategoryDtmf, KMccDtmfControl,
                         KErrNone, dtmfDataPackage );
    
    // SendMediaSignal
    iStream->SendMediaSignalL( event );
    
    TMccRtcpEventData rtcpData;
    rtcpData.iRtcpPacketData.Copy( _L8("foofoo") );
    rtcpData.iRtcpPacketType = KRtcpAnyPacket;
    rtcpData.iSsrc = 2;
    
    TMccRtcpEventDataPackage rtcpDataPackage( rtcpData );
    
    //Create a media signal and send it 
    TMccEvent event2( 1, 1, 1, 1, KMccEventCategoryRtcp, KMccRtcpReceived,
                         KErrNone, rtcpDataPackage );

    iStream->SendMediaSignalL( event2 );
    }

void UT_CMccSymUlStream::UT_CMccSymUlStream_DoChangeBitrateLL()
    {
    iStream->iCodecInfo.iFourCC = TFourCC();
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(
        iStream->DoChangeBitrateL( KAmrNbBitrate102 ), KErrNotSupported );
    iStream->iCodecInfo.iFourCC = TFourCC( KMccFourCCIdAMRNB );
    
    CMMFDevSound* soundDevice = iStream->iSoundDevice;
    iStream->iSoundDevice = NULL;
    EUNIT_ASSERT_LEAVE( iStream->DoChangeBitrateL( KAmrNbBitrate102 ) );
    iStream->iSoundDevice = soundDevice;
    
    // SoundEncoderConfig is stubbed, so this works also in emulator
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->DoChangeBitrateL( KAmrNbBitrate102 ) );
    }

void UT_CMccSymUlStream::UT_CMccSymUlStream_SendRTCPReceiverReportLL(  )
    {
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iStream->SendRTCPReceiverReportL(), KErrNotSupported );
    }

void UT_CMccSymUlStream::UT_CMccSymUlStream_ResetCountersLL(  )
    {
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->ResetCountersL() );
    }

void UT_CMccSymUlStream::UT_CMccSymUlStream_SendRTCPSenderReportLL(  )
    {
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iStream->SendRTCPSenderReportL(), KErrNotReady );
    iStream->PrimeL( 0 );
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->SendRTCPSenderReportL() );
    
    iStream->iDatasink = NULL;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iStream->SendRTCPSenderReportL(), KErrNotReady );
    }

void UT_CMccSymUlStream::UT_CMccSymUlStream_SendRTCPDataLL(  )
    {
    TBuf8<5> buf;
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->SendRTCPDataL( buf ) );
    }

void UT_CMccSymUlStream::UT_CMccSymUlStream_GetSSRCLL(  )
    {
    TUint32 ssrc;
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->GetSSRCL( ssrc ) );
    }

 void UT_CMccSymUlStream::UT_CMccSymUlStreamBase_SetGainL()
	 {
	 iStream->CMccSymStreamBase::SetGainL( 5 );	
	 }
 void UT_CMccSymUlStream::UT_CMccSymUlStreamBase_SetVolumeL()
	 {
	 iStream->CMccSymStreamBase::SetVolumeL( 5 );	
	 }
 void UT_CMccSymUlStream::UT_CMccSymUlStreamBase_MaxVolumeL()
	 {
	 iStream->CMccSymStreamBase::MaxVolumeL();
	 }
 void UT_CMccSymUlStream::UT_CMccSymUlStreamBase_MaxGainL()
	 {
     iStream->CMccSymStreamBase::MaxGainL();
	 }
 
 void UT_CMccSymUlStream::UT_CMccSymUlStreamBase_VolumeL()
	 {
     iStream->CMccSymStreamBase::VolumeL();
	 }
	 
 void UT_CMccSymUlStream::UT_CMccSymUlStreamBase_GainL()
	 {
	 MCC_EUNIT_ASSERT_NO_LEAVE(iStream->CMccSymStreamBase::GainL());
	 }
 void UT_CMccSymUlStream::UT_CMccSymUlStreamBase_SendMediaSignalL()
	 {
	 TMccEvent event;
     UT_CMccSymSubthreadClient::CreateRtcpEvent( event );

	 MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(
	 	iStream->CMccSymStreamBase::SendMediaSignalL( event ), 
	 	KErrNotSupported);
	 }
 void UT_CMccSymUlStream::UT_CMccSymUlStreamBase_SendRTCPDataL()
	 {
	 HBufC8* buf = HBufC8::NewLC(1);
	 TPtr8 bufPtr = buf->Des();
	 MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(
	 	iStream->CMccSymStreamBase::SendRTCPDataL( bufPtr ), 
	 	KErrNotSupported);
	 CleanupStack::PopAndDestroy( buf );
	 }
 void UT_CMccSymUlStream::UT_CMccSymUlStreamBase_StartInactivityTimerL()
	 {
	 MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(
	 	iStream->CMccSymStreamBase::StartInactivityTimerL(5), 
	 	KErrNotSupported);	
	 }
 
 void UT_CMccSymUlStream::UT_CMccSymUlStreamBase_StopInactivityTimerL()
	 {
	 MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(
	 	iStream->CMccSymStreamBase::StopInactivityTimerL(), 
	 	KErrNotSupported);	
	 }
 
 void UT_CMccSymUlStream::UT_CMccSymUlStreamBase_GetFmtpL()
	 {
	 HBufC8* buf = HBufC8::NewLC( 20 );
	 TPtr8 bufPtr = buf->Des();
	 MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(
	  	iStream->CMccSymStreamBase::GetFmtpL( bufPtr ),
	 	KErrArgument ); 

    if (iStream->iFmtpAttr)
	    {
	    delete iStream->iFmtpAttr;
	    iStream->iFmtpAttr = NULL;
	    }
    
    iStream->iFmtpAttr = HBufC8::NewL(10);
    MCC_EUNIT_ASSERT_NO_LEAVE( iStream->CMccSymStreamBase::GetFmtpL( bufPtr )); 
	CleanupStack::PopAndDestroy( buf );
	}
 
 void UT_CMccSymUlStream::UT_CMccSymUlStreamBase_GetCodecL()
	 {
	 TMccCodecInfo codec;
	 iStream->CMccSymStreamBase::GetCodecL( codec );
	 }
 
 void UT_CMccSymUlStream::UT_CMccSymUlStreamBase_ParsePayloadTypesLL()
    {
    RArray<TUint> payloadTypes;
    TMccCodecInfo codecInfo;
    codecInfo.iPayloadType = 8;
    codecInfo.iRedundantPayload = 100;
    iStream->CMccSymStreamBase::ParsePayloadTypesL( payloadTypes, codecInfo );
    EUNIT_ASSERT( payloadTypes.Count() == 2 );
    payloadTypes.Close();
    }
    
 void UT_CMccSymUlStream::UT_CMccSymUlStreamBase_ChangeBitrateL()
    {
    // SoundEncoderConfig is stubbed, so this works also in emulator
    TInt err = iStream->CMccSymStreamBase::ChangeBitrate( KAmrNbBitrate102 );
    EUNIT_ASSERT( err == KErrNone || err == KErrNoMemory );
    }
    
 void UT_CMccSymUlStream::UT_CMccSymUlStreamBase_DoChangeBitrateLL()
    {
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iStream->CMccSymStreamBase::DoChangeBitrateL( KG711Bitrate64 ), KErrNotSupported );
    }
    
 void UT_CMccSymUlStream::UT_CMccSymUlStreamBase_BindContextIntoStreamLL()
    {
    TMccCryptoContext defContext;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iStream->CMccSymStreamBase::BindContextIntoStreamL( 0, defContext ),
        KErrNotSupported );
    }
    
 void UT_CMccSymUlStream::UT_CMccSymUlStreamBase_RemoveContextL()
    {
    TMccCryptoContext defContext;
    
    EUNIT_ASSERT( 
        KErrNone == iStream->CMccSymStreamBase::RemoveContext( MCC_ENDPOINT_ID( iSinkStub ) ) );
    }

void UT_CMccSymUlStream::UT_CMccSymUlStreamBase_InformStreamStateChange()
    {
    iStream->InformStreamStateChange( KMccStreamPrepared, 1 );
    iStream->InformStreamStateChange( KMccStreamPrepared,  MCC_ENDPOINT_ID( iStream->iDatasink ) );
    iStream->InformStreamStateChange( KMccStreamPrepared,  MCC_ENDPOINT_ID( iStream->iDatasource ) );

    }
 
void UT_CMccSymUlStream::UT_CMccSymUlStreamBase_HandleStandby()
    {
    // TBD: Are the comparisons wanted, if changed to assignment then this test
    // case fails.
    iStream->iType == KMccDtmfStream; // ???
    iStream->HandleStandby( MCC_ENDPOINT_ID( iStream->iDatasink ), ETrue );

    iStream->iType == KMccAudioDownlinkStandbyStream; // ???
    iStream->HandleStandby( MCC_ENDPOINT_ID( iStream->iDatasink ), ETrue );
    iStream->HandleStandby( MCC_ENDPOINT_ID( iStream->iDatasink ), EFalse );

    User::LeaveIfError( iStream->iControlledEndpoints.Append( MCC_ENDPOINT_ID( iStream->iDatasink ) ) );
    iStream->HandleStandby( MCC_ENDPOINT_ID( iStream->iDatasink ), ETrue );
    iStream->HandleStandby( MCC_ENDPOINT_ID( iStream->iDatasink ), EFalse );

    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccSymUlStream,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "SetPrioritySettingsL - test ",
    "CMccSymUlStream",
    "SetPrioritySettingsL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymUlStream_SetPrioritySettingsLL, Teardown)

EUNIT_TEST(
    "AddSinkAndSourceL - test ",
    "CMccSymUlStream",
    "AddSinkAndSourceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymUlStream_AddSinkAndSourceLL, Teardown)

EUNIT_TEST(
    "LoadCodecL - test ",
    "CMccSymUlStream",
    "LoadCodecL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccSymUlStream_LoadCodecLL, Teardown)

EUNIT_TEST(
    "UpdateCodecInformationL - test ",
    "CMccSymUlStream",
    "UpdateCodecInformationL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymUlStream_UpdateCodecInformationLL, Teardown)

EUNIT_TEST(
    "PrimeL - test ",
    "CMccSymUlStream",
    "PrimeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymUlStream_PrimeLL, Teardown)

EUNIT_TEST(
    "PlayL - test ",
    "CMccSymUlStream",
    "PlayL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymUlStream_PlayLL, Teardown)

EUNIT_TEST(
    "PauseL - test ",
    "CMccSymUlStream",
    "PauseL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymUlStream_PauseLL, Teardown)

EUNIT_TEST(
    "ResumeL - test ",
    "CMccSymUlStream",
    "ResumeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymUlStream_ResumeLL, Teardown)

EUNIT_TEST(
    "StopL - test ",
    "CMccSymUlStream",
    "StopL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymUlStream_StopLL, Teardown)

EUNIT_TEST(
    "SetBalanceL - test ",
    "CMccSymUlStream",
    "SetBalanceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymUlStream_SetBalanceLL, Teardown)

EUNIT_TEST(
    "GetBalanceL - test ",
    "CMccSymUlStream",
    "GetBalanceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymUlStream_GetBalanceLL, Teardown)

EUNIT_TEST(
    "SendMediaSignalL - test ",
    "CMccSymUlStream",
    "SendMediaSignalL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymUlStream_SendMediaSignalLL, Teardown)

EUNIT_TEST(
    "DoChangeBitrateL - test ",
    "CMccSymUlStream",
    "DoChangeBitrateL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymUlStream_DoChangeBitrateLL, Teardown )

EUNIT_TEST(
    "SendRTCPReceiverReportL - test ",
    "CMccSymUlStream",
    "SendRTCPReceiverReportL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymUlStream_SendRTCPReceiverReportLL, Teardown)

EUNIT_TEST(
    "ResetCountersL - test ",
    "CMccSymUlStream",
    "ResetCountersL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymUlStream_ResetCountersLL, Teardown)

EUNIT_TEST(
    "SendRTCPSenderReportL - test ",
    "CMccSymUlStream",
    "SendRTCPSenderReportL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymUlStream_SendRTCPSenderReportLL, Teardown)

EUNIT_TEST(
    "SendRTCPDataL - test ",
    "CMccSymUlStream",
    "SendRTCPDataL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymUlStream_SendRTCPDataLL, Teardown)

EUNIT_TEST(
    "GetSSRCL - test ",
    "CMccSymUlStream",
    "GetSSRCL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymUlStream_GetSSRCLL, Teardown)

// base class test cases

        
EUNIT_TEST(
    "SetGainL - test ",
    "CMccSymStreamBase",
    "SetGainL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymUlStreamBase_SetGainL, Teardown)
      
EUNIT_TEST(
    "SetVolumeL - test ",
    "CMccSymStreamBase",
    "SetVolumeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymUlStreamBase_SetVolumeL, Teardown)
             
EUNIT_TEST(
    "MaxVolumeL - test ",
    "CMccSymStreamBase",
    "MaxVolumeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymUlStreamBase_MaxVolumeL, Teardown)
             
EUNIT_TEST(
    "MaxGainL - test ",
    "CMccSymStreamBase",
    "MaxGainL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymUlStreamBase_MaxGainL, Teardown)
             
EUNIT_TEST(
    "VolumeL - test ",
    "CMccSymStreamBase",
    "VolumeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymUlStreamBase_VolumeL, Teardown)
             
EUNIT_TEST(
    "GainL - test ",
    "CMccSymStreamBase",
    "GainL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymUlStreamBase_GainL, Teardown)
                      
EUNIT_TEST(
    "SendMediaSignalL - test ",
    "CMccSymStreamBase",
    "SendMediaSignalL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymUlStreamBase_SendMediaSignalL, Teardown)
             
EUNIT_TEST(
    "SendRTCPDataL - test ",
    "CMccSymStreamBase",
    "SendRTCPDataL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymUlStreamBase_SendRTCPDataL, Teardown)
             
EUNIT_TEST(
    "StartInactivityTimerL - test ",
    "CMccSymStreamBase",
    "StartInactivityTimerL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymUlStreamBase_StartInactivityTimerL, Teardown)
                      
EUNIT_TEST(
    "StopInactivityTimerL - test ",
    "CMccSymStreamBase",
    "StopInactivityTimerL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymUlStreamBase_StopInactivityTimerL, Teardown)
             
EUNIT_TEST(
    "GetFmtpL - test ",
    "CMccSymStreamBase",
    "GetFmtpL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccSymUlStreamBase_GetFmtpL, Teardown)
             
EUNIT_TEST(
    "GetCodecL - test ",
    "CMccSymStreamBase",
    "GetCodecL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymUlStreamBase_GetCodecL, Teardown)

EUNIT_TEST(
    "ParsePayloadTypesL - test ",
    "CMccSymStreamBase",
    "ParsePayloadTypesL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymUlStreamBase_ParsePayloadTypesLL, Teardown )

EUNIT_TEST(
    "ChangeBitrate - test ",
    "CMccSymStreamBase",
    "ChangeBitrate",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymUlStreamBase_ChangeBitrateL, Teardown )

EUNIT_TEST(
    "DoChangeBitrateL - test ",
    "CMccSymStreamBase",
    "DoChangeBitrateL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymUlStreamBase_DoChangeBitrateLL, Teardown )

EUNIT_TEST(
    "BindContextIntoStreamL - test ",
    "CMccSymStreamBase",
    "BindContextIntoStreamL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymUlStreamBase_BindContextIntoStreamLL, Teardown )

EUNIT_TEST(
    "RemoveContext - test ",
    "CMccSymStreamBase",
    "RemoveContext",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymUlStreamBase_RemoveContextL, Teardown )

EUNIT_TEST(
    "InformStreamStateChange - test ",
    "CMccSymStreamBase",
    "InformStreamStateChange",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymUlStreamBase_InformStreamStateChange, Teardown )

EUNIT_TEST(
    "HandleStandby - test ",
    "CMccSymStreamBase",
    "HandleStandby",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymUlStreamBase_HandleStandby, Teardown )


EUNIT_END_TEST_TABLE

//  END OF FILE
