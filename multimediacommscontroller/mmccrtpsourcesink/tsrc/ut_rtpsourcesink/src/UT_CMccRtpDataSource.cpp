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
#include "UT_CMccRtpDataSource.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "MccRtpDataSource.h"
#include "MmccEvents.h"
#include "AmrPayloadFormatRead.h"
#include "mccrtpkeepalivecontainer.h"
#include "mcctesteventhandler.h"
#include <srtpcryptocontext.h>
#include <srtpmastersalt.h>
#include <srtpstreamin.h>
#include "UT_CMccRtpInterface.h"
#include "mccunittestmacros.h"
#include "mccrtpmediaclock.h"

// CONSTANTS
_LIT8(KRFC3711_TestMasterKey128bits,    "E1F97A0D3E018BE0D64FA32C06DE4139");
_LIT8(KRFC3711_TestMasterSalt112bits,   "0EC675AD498AFEEBB6960B3AABE6");	
_LIT8(KTestMKI128Bits,      "ABCDEF1234567890ABCDEF1234567890");

// CONSTRUCTION
UT_CMccRtpDataSource* UT_CMccRtpDataSource::NewL()
    {
    UT_CMccRtpDataSource* self = UT_CMccRtpDataSource::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMccRtpDataSource* UT_CMccRtpDataSource::NewLC()
    {
    UT_CMccRtpDataSource* self = new( ELeave ) UT_CMccRtpDataSource();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccRtpDataSource::~UT_CMccRtpDataSource()
    {
    }

// Default constructor
UT_CMccRtpDataSource::UT_CMccRtpDataSource()
    : MDataSink( KUidMediaTypeAudio )
    {
    }

// Second phase construct
void UT_CMccRtpDataSource::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_CMccRtpDataSource::SetupL()
    {
    iEventHandler = CMccTestEventHandler::NewL();
    iRtpApi = CRtpAPI::NewL( *iEventHandler );
    TUid dummyUid( TUid::Uid( 42 ) ); 
    TBuf8<5> dummyBuf( _L8( "foo" ) );
    iSource = static_cast<CMccRtpDataSource*>(
        CMccRtpDataSource::NewSourceL( dummyUid, dummyBuf ) );
    
    TMccRtpSourceSetting setting;
    setting.iStandByTimerValue = 3000;   
    TMccRtpSourceSettingBuf settingBuf( setting );
    iSource->ConstructSourceL( settingBuf );
    
    MAsyncEventHandler* eventHandler = NULL;
    TRtpId rtpSessionId(0);
    iRtpKeepaliveMechanism = 
        CMccRtpKeepaliveContainer::NewL( *eventHandler, 
                                         *iRtpApi, 
                                         rtpSessionId ); 
      
    iRtpMediaClock = CMccRtpMediaClock::NewL();                                    
    } 

void UT_CMccRtpDataSource::Teardown()
    {
    delete iRtpKeepaliveMechanism;
    
    if ( iSource )
        {
        delete iSource;
        iSource = NULL;
        }
    if ( iSecSession)
    	{
    	delete iSecSession;
		iSecSession = NULL;
    	}

    if ( iRtpApi )
        {
        delete iRtpApi;
        iRtpApi = NULL;
        }
        
    delete iEventHandler;
    
    if ( iRtpMediaClock )
        {
        delete iRtpMediaClock;
        iRtpMediaClock = NULL;
        }
    }


// HELPER FUNCTIONS

void UT_CMccRtpDataSource::InitializeL( TBool aCreateStream, TBool aEnableRtcp )
    {
    iSource->SourceThreadLogon( *iEventHandler );
    
    RArray<TUint> payloads;
    CleanupClosePushL( payloads );
    payloads.AppendL( KDefaultAmrNbPT );
    payloads.AppendL( KPcmuPayloadType );
    iSource->RegisterPayloadTypesL( payloads );
    CleanupStack::PopAndDestroy( &payloads );

    TMccRtpSessionParams params;
    params.iRtpAPI = iRtpApi;
    params.iSessionId = iSession;
    params.iEnableRTCP = aEnableRtcp;
    params.iRtpKeepalive = iRtpKeepaliveMechanism;
    EUNIT_ASSERT_NO_LEAVE( iSource->SetSessionParamsL( params ) );

    if ( aCreateStream )
        {
        CreateStreamL();
        }
    }

void UT_CMccRtpDataSource::CreateStreamL()
    {
    TMccCodecInfo cInfo;
    cInfo.iType = KUidMediaTypeAudio;
    cInfo.iFourCC = TFourCC( ' ','A','M','R' );
    cInfo.iBitrate = 8000;
    cInfo.iPayloadType = KDefaultAmrNbPT;
    TMccCodecInfoBuffer cInfoBuf( cInfo );
    EUNIT_ASSERT_NO_LEAVE( iSource->ConfigureL( cInfoBuf, iRtpMediaClock ) );
    }
    
// TEST FUNCTIONS

void UT_CMccRtpDataSource::UT_CMccRtpDataSource_RtpPacketReceivedL()
    {
    TRtpId stream( 0 );
    TRtpId wrongStream( 99 );
    TRtpRecvHeader header;
    TBuf8<5> data;
    data.Format( _L8( "foo42" ) );
    iSource->SourceThreadLogon( *iEventHandler );

    CMMFBuffer* buffer = CMMFDescriptorBuffer::NewL(10);
    CleanupStack::PushL( buffer );    
    CAmrPayloadFormatRead* amrPfRead = CAmrPayloadFormatRead::NewL( NULL );
    CleanupStack::PushL( amrPfRead );
    
    header.iPayloadType = KPcmuPayloadType;    
    RArray<TUint> payloads;
    CleanupClosePushL( payloads );
    payloads.AppendL( KDefaultAmrNbPT );
    payloads.AppendL( KPcmuPayloadType );
    iSource->RegisterPayloadTypesL( payloads );
    CleanupStack::PopAndDestroy( &payloads );
    
    // Wrong stream
    iSource->FillBufferL( buffer, amrPfRead, TMediaId() );
    iSource->RtpPacketReceived( wrongStream, header, data );

    TMccRtpSessionParams params;
    params.iRtpAPI = iRtpApi;
    params.iSessionId = iSession;
    params.iEnableRTCP = EFalse;
    params.iRtpKeepalive = iRtpKeepaliveMechanism;
    EUNIT_ASSERT_NO_LEAVE( iSource->SetSessionParamsL( params ) );

    TMccCodecInfo cInfo;
    cInfo.iFourCC = TFourCC( ' ','A','M','R' );
    cInfo.iBitrate = 8000;
    cInfo.iPayloadType = KDefaultAmrNbPT;
    TMccCodecInfoBuffer cInfoBuf( cInfo );
    
    EUNIT_ASSERT_NO_LEAVE( iSource->ConfigureL( cInfoBuf, iRtpMediaClock ) );
    //iSource->NegotiateSourceL( *iEventHandler );
    EUNIT_ASSERT_NO_LEAVE( iSource->NegotiateSourceL( *amrPfRead ) );
    EUNIT_ASSERT_NO_LEAVE( iSource->SourcePrimeL() );
    EUNIT_ASSERT_NO_LEAVE( iSource->SourcePlayL() );

    stream = 2; // This is what the RtpAPI stub gives
    header.iPayloadType = KDefaultAmrNbPT;
    
    EUNIT_ASSERT_EQUALS( iSource->iTimeoutTime, 0 );
    iSource->FillBufferL( buffer, amrPfRead, TMediaId() );
    iSource->RtpPacketReceived( stream, header, data );
    EUNIT_ASSERT_EQUALS( iSource->iTimeoutTime, 0 );
    EUNIT_ASSERT_EQUALS( iSource->iInactivityTimerId, KMaxTUint32 );
    
    // Inactivity timer is activated when receiving packet
    MCC_EUNIT_ASSERT_NO_LEAVE( iSource->StartInactivityTimerL( 500000 ) );
    iSource->RtpPacketReceived( stream, header, data );
    EUNIT_ASSERT_EQUALS( iSource->iTimeoutTime, 500000 );
    EUNIT_ASSERT( iSource->iInactivityTimerId != KMaxTUint32 );

    // Jitter observing is active
    iSource->iJitCalc->iMediaQualityObservingStarted = ETrue;
    iSource->RtpPacketReceived( stream, header, data );
    
    // Sink not ready, error ignored silently
    iEventHandler->iLastEvent.iEventType = KMccEventNone;
    iSource->iFillBufferRequester = NULL;
    iSource->RtpPacketReceived( stream, header, data );
    EUNIT_ASSERT_EQUALS( iEventHandler->iLastEvent.iEventType, KMccEventNone );
    
    // secure key expired, error ignored silently
    iSource->iSecureKeyExpired = ETrue;
    iSource->RtpPacketReceived( stream, header, data );
    EUNIT_ASSERT_EQUALS( iEventHandler->iLastEvent.iEventType, KMccEventNone );
    
    CleanupStack::PopAndDestroy( amrPfRead );
    CleanupStack::PopAndDestroy( buffer );
    iSource->SourceThreadLogoff();
    }

void UT_CMccRtpDataSource::UT_CMccRtpDataSource_StartInactivityTimerLL()
    {
    TUint32 time( 0 );
    
    // Invalid time
    EUNIT_ASSERT_SPECIFIC_LEAVE( iSource->StartInactivityTimerL( time ), KErrArgument );

    time = 5000;

    // Ok
    MCC_EUNIT_ASSERT_NO_LEAVE( iSource->StartInactivityTimerL( time ) );
    }

void UT_CMccRtpDataSource::UT_CMccRtpDataSource_StopInactivityTimerLL()
    {
    // No timer exists
    EUNIT_ASSERT_NO_LEAVE( iSource->StopInactivityTimerL() );

    TUint32 time( 5000 );
    iSource->StartInactivityTimerL( time );

    // OK
    EUNIT_ASSERT_NO_LEAVE( iSource->StopInactivityTimerL() );
    }

void UT_CMccRtpDataSource::UT_CMccRtpDataSource_SendMediaSignallingLL()
    {
    iSource->iEnableRtcp = ETrue;
    TMccEvent foo;
    UT_CMccRtpInterface::CreateRtcpEvent( foo );
    
    EUNIT_ASSERT_LEAVE( iSource->SendMediaSignallingL( foo ) );
    
    // No RtpApi
    foo.iEventCategory = KMccEventCategoryRtcp;
    foo.iEventType = KMccRtcpControl;
    TMccRtcpEventData eventData;
    eventData.iRtcpPacketType = KRtcpRrPacket;
    TMccRtcpEventDataPackage eventDataPkg( eventData );
    foo.iEventData.Copy( eventDataPkg );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iSource->SendMediaSignallingL( foo ), KErrNotReady );
    
    // Rr, stream does not exist
    InitializeL( EFalse, ETrue );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iSource->SendMediaSignallingL( foo ), KErrNotReady );
    
    // Stream exists
    CreateStreamL();
    EUNIT_ASSERT_NO_LEAVE( iSource->SendMediaSignallingL( foo ) );
    
    // Sr
    eventData.iRtcpPacketType = KRtcpSrPacket;
    eventDataPkg = eventData;
    foo.iEventData.Copy( eventDataPkg );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iSource->SendMediaSignallingL( foo ), KErrArgument );
    }

void UT_CMccRtpDataSource::UT_CMccRtpDataSource_SourceDataTypeCodeL()
    {
    TMediaId mediaId( KUidMediaTypeVideo );
    EUNIT_ASSERT_EQUALS( iSource->SourceDataTypeCode( mediaId ), TFourCC() );

    iSource->SourceThreadLogoff();
    TFourCC codec( ' ','A','M','R' );
    TMediaId media2( KUidMediaTypeAudio );
    iSource->SetSourceDataTypeCode( codec, media2 );

    // The codecs SHOULD be equal
    EUNIT_ASSERT_EQUALS( iSource->SourceDataTypeCode( media2 ), codec );
    }

void UT_CMccRtpDataSource::UT_CMccRtpDataSource_SetSourceDataTypeCodeL()
    {
    TFourCC codec;
    TMediaId media( KUidMediaTypeVideo ); // wrong media type

    EUNIT_ASSERT_EQUALS( iSource->SetSourceDataTypeCode( codec, media ), KErrNone );

    // Set the correct media type
    TMediaId media2( KUidMediaTypeAudio );
    EUNIT_ASSERT_EQUALS( iSource->SetSourceDataTypeCode( codec, media2 ), KErrNone );
    }

void UT_CMccRtpDataSource::UT_CMccRtpDataSource_BufferEmptiedLL()
    {
    EUNIT_ASSERT_LEAVE( iSource->BufferEmptiedL( NULL ) );
    }

void UT_CMccRtpDataSource::UT_CMccRtpDataSource_CanCreateSourceBufferL()
    {
    EUNIT_ASSERT_EQUALS( iSource->CanCreateSourceBuffer(), EFalse );
    }

void UT_CMccRtpDataSource::UT_CMccRtpDataSource_CreateSourceBufferLL()
    {
    TMediaId dummyId;
    TBool dummyBool;
    EUNIT_ASSERT_LEAVE( iSource->CreateSourceBufferL( dummyId, dummyBool ) );
    }

void UT_CMccRtpDataSource::UT_CMccRtpDataSource_SourceThreadLogonL()
    {
    iSource->SourceThreadLogon( *iEventHandler );
    iSource->SourceThreadLogon( *iEventHandler );
    iSource->SourceThreadLogoff();
    }

void UT_CMccRtpDataSource::UT_CMccRtpDataSource_SourceThreadLogoffL()
    {
    iSource->SourceThreadLogoff();

    // Reach more branches
    TMccRtpSessionParams params;
    params.iRtpAPI = iRtpApi;
    params.iSessionId = iSession;
    params.iEnableRTCP = EFalse;
    params.iRtpKeepalive = iRtpKeepaliveMechanism;
    EUNIT_ASSERT_NO_LEAVE( iSource->SetSessionParamsL( params ) );

    TMccCodecInfo cInfo;
    cInfo.iFourCC = TFourCC( ' ','A','M','R' );
    cInfo.iBitrate = 8000;
    cInfo.iPayloadType = KDefaultAmrNbPT;
    TMccCodecInfoBuffer cInfoBuf( cInfo );
    
    EUNIT_ASSERT_NO_LEAVE( iSource->ConfigureL( cInfoBuf, iRtpMediaClock ) );

    MCC_EUNIT_ASSERT_EQUALS( iSource->SourceThreadLogon( *iEventHandler ), KErrNone );
    EUNIT_ASSERT_NO_LEAVE( iSource->SourcePrimeL() );

    iSource->SourceThreadLogoff();

    }

void UT_CMccRtpDataSource::UT_CMccRtpDataSource_SourcePrimeLL()
    {
    // Ok
    TMccRtpSessionParams params;
    params.iRtpAPI = iRtpApi;
    params.iSessionId = iSession;
    params.iEnableRTCP = EFalse;
    params.iRtpKeepalive = iRtpKeepaliveMechanism;
    EUNIT_ASSERT_NO_LEAVE( iSource->SetSessionParamsL( params ) );

    TMccCodecInfo cInfo;
    cInfo.iFourCC = TFourCC( ' ','A','M','R' );
    cInfo.iBitrate = 8000;
    cInfo.iPayloadType = KDefaultAmrNbPT;
    TMccCodecInfoBuffer cInfoBuf( cInfo );
    
    EUNIT_ASSERT_NO_LEAVE( iSource->ConfigureL( cInfoBuf, iRtpMediaClock ) );

    MCC_EUNIT_ASSERT_EQUALS( iSource->SourceThreadLogon( *iEventHandler ), KErrNone );
    EUNIT_ASSERT_NO_LEAVE( iSource->SourcePrimeL() );
    
    iSource->SourceThreadLogoff();
    }

void UT_CMccRtpDataSource::UT_CMccRtpDataSource_SourcePlayLL()
    {
    // Not logged on
    EUNIT_ASSERT_SPECIFIC_LEAVE( iSource->SourcePlayL(), KErrNotFound );
    
    // Not allowed if not yet prepared
    InitializeL();
    EUNIT_ASSERT_SPECIFIC_LEAVE( iSource->SourcePlayL(), KErrNotReady );
    
    // Ok when already prepared
    iSource->iState = MMccRtpInterface::ERtpStatePrimed;
    
    EUNIT_ASSERT_NO_LEAVE( iSource->SourcePlayL() );
    EUNIT_ASSERT_EQUALS( iEventHandler->iLastEvent.iEventType, KMccStreamStarted );
    
    // Resuming
    iSource->iState = MMccRtpInterface::ERtpStatePaused; 
    EUNIT_ASSERT_NO_LEAVE( iSource->SourcePlayL() );
    EUNIT_ASSERT_EQUALS( iEventHandler->iLastEvent.iEventType, KMccStreamResumed );
    EUNIT_ASSERT( iRtpKeepaliveMechanism->iStopped == EFalse )
    
    // State downgrade is ignored
    EUNIT_ASSERT_NO_LEAVE( iSource->SourcePrimeL() );
    EUNIT_ASSERT( MMccRtpInterface::ERtpStatePlaying == iSource->State() );
    
    // "Secure session"
    iRtpKeepaliveMechanism->iStopped = ETrue;
    TInt fakeSecSession( 3 );
    iSource->iSecSession = reinterpret_cast<CSRTPSession*>( &fakeSecSession );
    EUNIT_ASSERT_NO_LEAVE( iSource->SourcePlayL() );
    EUNIT_ASSERT( iRtpKeepaliveMechanism->iStopped == ETrue )
    iSource->iSecSession = NULL;
    }

void UT_CMccRtpDataSource::UT_CMccRtpDataSource_SourcePauseLL()
    {
    EUNIT_ASSERT_LEAVE( iSource->SourcePauseL() );
    }

void UT_CMccRtpDataSource::UT_CMccRtpDataSource_SourceStopLL()
    {
    EUNIT_ASSERT_NO_LEAVE( iSource->SourceStopL() );
    }

void UT_CMccRtpDataSource::UT_CMccRtpDataSource_FillBufferLL()
    {
    EUNIT_ASSERT_NO_LEAVE( iSource->FillBufferL( NULL, NULL, TMediaId() ) );
    CMMFDataBuffer* buffer = CMMFDataBuffer::NewL( 100 );
    CleanupStack::PushL( buffer );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iSource->FillBufferL( buffer, NULL, TMediaId() ), KErrArgument );
    CleanupStack::PopAndDestroy( buffer );
    }

void UT_CMccRtpDataSource::UT_CMccRtpDataSource_SendRTCPReceiverReportL()
    {
    // No RTP API set
    EUNIT_ASSERT_EQUALS( iSource->SendRTCPReceiverReport(), KErrNotReady );

    TMccRtpSessionParams params;
    params.iRtpAPI = iRtpApi;
    params.iSessionId = iSession;
    params.iEnableRTCP = EFalse;
    params.iRtpKeepalive = iRtpKeepaliveMechanism;
    iSource->SetSessionParamsL( params );


    // Ok
    iSource->iRtpStreamId = 1; // the iStreamID must not be empty
    EUNIT_ASSERT_EQUALS( iSource->SendRTCPReceiverReport(), KErrNone );
    }

void UT_CMccRtpDataSource::UT_CMccRtpDataSource_SendRTCPSenderReportL()
    {
    // No RTP API set
    EUNIT_ASSERT_EQUALS( iSource->SendRTCPSenderReport(), KErrNotReady );

    TMccRtpSessionParams params;
    params.iRtpAPI = iRtpApi;
    params.iSessionId = iSession;
    params.iEnableRTCP = EFalse;
    params.iRtpKeepalive = iRtpKeepaliveMechanism;
    iSource->SetSessionParamsL( params );

    // Ok
    iSource->iRtpStreamId = 1; // the iStreamID must not be empty
    EUNIT_ASSERT_EQUALS( iSource->SendRTCPSenderReport(), KErrNone );
    }

void UT_CMccRtpDataSource::UT_CMccRtpDataSource_SendRTCPDataLL()
    {
    TBuf8<5> dummy;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iSource->SendRTCPDataL( dummy ), KErrNotSupported );
    }

void UT_CMccRtpDataSource::UT_CMccRtpDataSource_NegotiateSourceLL()
    {
    // wrong codec
    EUNIT_ASSERT_NO_LEAVE( iSource->NegotiateSourceL( *this ) );

    // Reach more branches
    TMccRtpSessionParams params;
    params.iRtpAPI = iRtpApi;
    params.iSessionId = iSession;
    params.iEnableRTCP = EFalse;
    params.iRtpKeepalive = iRtpKeepaliveMechanism;
    EUNIT_ASSERT_NO_LEAVE( iSource->SetSessionParamsL( params ) );

    TMccCodecInfo cInfo;
    cInfo.iFourCC = TFourCC( ' ','A','M','R' );
    cInfo.iBitrate = 8000;
    cInfo.iPayloadType = KDefaultAmrNbPT;
    TMccCodecInfoBuffer cInfoBuf( cInfo );
    
    EUNIT_ASSERT_NO_LEAVE( iSource->ConfigureL( cInfoBuf, iRtpMediaClock ) );
    MCC_EUNIT_ASSERT_EQUALS( iSource->SourceThreadLogon( *iEventHandler ), KErrNone );
    EUNIT_ASSERT_NO_LEAVE( iSource->SourcePrimeL() );

    EUNIT_ASSERT_NO_LEAVE( iSource->NegotiateSourceL( *this ) );
    iSource->SourceThreadLogoff();
    }

void UT_CMccRtpDataSource::UT_CMccRtpDataSource_ConstructSourceLL()
    {
    TUid dummyUid( TUid::Uid( 42 ) ); 
    TBuf8<5> dummyBuf( _L8( "foo" ) );
    CMccRtpDataSource* source = static_cast<CMccRtpDataSource*>(
        CMccRtpDataSource::NewSourceL( dummyUid, dummyBuf ) );
    CleanupStack::PushL( source );
    MCC_EUNIT_ASSERT_NO_LEAVE( source->ConstructSourceL( KNullDesC8 ) );
    CleanupStack::PopAndDestroy( source );
    }


void UT_CMccRtpDataSource::UT_CMccRtpDataSource_ValidatePacketL()
    {
    TRtpId streamId( KNullId );
    TRtpRecvHeader header;

    TBuf8<5> data5;
    TBuf8<15> data15;
    data5.Format( _L8( "foo42" ) );
    data15.Format( _L8( "foo42foo42foo42" ) );

    // Data too big, stream ok, wrong payload type
    EUNIT_ASSERT_LEAVE( iSource->ValidatePacketL( streamId, header, data15 ) );

    // Data ok, stream ok, wrong payload type
    EUNIT_ASSERT_LEAVE( iSource->ValidatePacketL( streamId, header, data5 ) );

    header.iPayloadType = KMccPayloadTypeMax;

    // header not OK
    CMMFDataBuffer* buffer = CMMFDataBuffer::NewL(10);
    CleanupStack::PushL( buffer );
    iSource->iBufferToFill = buffer;
    EUNIT_ASSERT_LEAVE( iSource->ValidatePacketL( streamId, header, data5 ) );
    
    // Get a new source
    //Teardown();
    if( iRtpKeepaliveMechanism )
    	{
    	delete iRtpKeepaliveMechanism;
    	iRtpKeepaliveMechanism = NULL;
    	}
    
    if ( iSource )
        {
        delete iSource;
        iSource = NULL;
        }
    //SetupL();
    TUid dummyUid( TUid::Uid( 42 ) ); 
    TBuf8<5> dummyBuf( _L8( "foo" ) );
    iSource = static_cast<CMccRtpDataSource*>(
        CMccRtpDataSource::NewSourceL( dummyUid, dummyBuf ) );
    
    TMccRtpSourceSetting setting;
    setting.iStandByTimerValue = 3000;   
    TMccRtpSourceSettingBuf settingBuf( setting );
    iSource->ConstructSourceL( settingBuf );
    
    MAsyncEventHandler* eventHandler = NULL;
    TRtpId rtpSessionId(0);
    iRtpKeepaliveMechanism = 
        CMccRtpKeepaliveContainer::NewL( *eventHandler, 
                                         *iRtpApi, 
                                         rtpSessionId ); 

    TMccRtpSessionParams params;
    params.iRtpAPI = iRtpApi;
    params.iSessionId = iSession;
    params.iEnableRTCP = EFalse;
    params.iRtpKeepalive = iRtpKeepaliveMechanism;
    EUNIT_ASSERT_NO_LEAVE( iSource->SetSessionParamsL( params ) );

    TMccCodecInfo cInfo;
    cInfo.iFourCC = TFourCC( KMccFourCCIdG711 );
    cInfo.iEnableDTX = ETrue;
    TMccCodecInfoBuffer cInfoBuf( cInfo );
    EUNIT_ASSERT_NO_LEAVE( iSource->ConfigureL( cInfoBuf, iRtpMediaClock ) );

    header.iPayloadType = KPcmuPayloadType;

    // Data ok, stream ok, payload type ok    
    CAmrPayloadFormatRead* amrPfRead = CAmrPayloadFormatRead::NewL( NULL );
    CleanupStack::PushL( amrPfRead );
    
    RArray<TUint> payloads;
    CleanupClosePushL( payloads );
    payloads.AppendL( KPcmuPayloadType );
    iSource->RegisterPayloadTypesL( payloads );
    iSource->FillBufferL( buffer, amrPfRead, TMediaId() );
    EUNIT_ASSERT_NO_LEAVE( iSource->ValidatePacketL( streamId, header, data5 ) );
    CleanupStack::PopAndDestroy( &payloads );
    CleanupStack::PopAndDestroy( amrPfRead );
    CleanupStack::PopAndDestroy( buffer );
    
    }

void UT_CMccRtpDataSource::UT_CMccRtpDataSource_CreateStreamLL()
    {
    TMccRtpSessionParams params;
    params.iRtpAPI = iRtpApi;
    params.iSessionId = iSession;
    params.iEnableRTCP = ETrue;
    params.iRtpKeepalive = iRtpKeepaliveMechanism;
    EUNIT_ASSERT_NO_LEAVE( iSource->SetSessionParamsL( params ) );

    TMccCodecInfo cInfo;
    TMccCodecInfoBuffer cInfoBuf( cInfo );
    
    EUNIT_ASSERT_NO_LEAVE( iSource->ConfigureL( cInfoBuf, iRtpMediaClock ) );

    // Get a new Source
    Teardown();
    SetupL();

    // This should leave because the string is too short
    TBuf8<5> dummy;
    dummy.Format( _L8( "foo42" ) );
    EUNIT_ASSERT_NO_LEAVE( iSource->SetSessionParamsL( params ) );
    EUNIT_ASSERT_LEAVE( iSource->ConfigureL( dummy, iRtpMediaClock ) );

    // Get a new Source
    Teardown();
    SetupL();

    TMccRtpSessionParams params2;
    params2.iRtpAPI = iRtpApi;
    params2.iSessionId = iSession;
    params2.iEnableRTCP = EFalse;
    params2.iRtpKeepalive = iRtpKeepaliveMechanism;
    iSource->SetSessionParamsL( params2 );
    
    TMccCodecInfo cInfo2;
    cInfo2.iFourCC = TFourCC( KMccFourCCIdG711 );
    cInfo2.iEnableDTX = ETrue;
    cInfo2.iRedundantPayload = 97;
    cInfo2.iRedundancyCount = 1;
    cInfo2.iAlgoUsed = EGenRedUsed;
    TMccCodecInfoBuffer cInfoBuf2( cInfo2 );

    EUNIT_ASSERT_NO_LEAVE( iSource->ConfigureL( cInfoBuf2, iRtpMediaClock ) );
    }

void UT_CMccRtpDataSource::UT_CMccRtpDataSource_DoCreateSrtpStreamL()
    {
    // Rtp stream not created
    iSource->iRtpStreamId = KNullId;
    EUNIT_ASSERT_NO_LEAVE( iSource->DoCreateSrtpStreamL() );
    }
         
void UT_CMccRtpDataSource::UT_CMccRtpDataSource_TestSendEventL()
    {
    iSource->SourceThreadLogon( *iEventHandler );
    
    // No target payload
    iSource->SendStreamEventToClient( KMccStreamPaused, KErrNone, KMccPTNotDefined );
    EUNIT_ASSERT_EQUALS( iEventHandler->iLastEvent.iEventType, KMccStreamPaused );
    EUNIT_ASSERT_EQUALS( iEventHandler->iLastEvent.iEventNumData, 0 );
    
    // Target payload
    iSource->SendStreamEventToClient( KMccStreamResumed, KErrNone, 100 );
    EUNIT_ASSERT_EQUALS( iEventHandler->iLastEvent.iEventType, KMccStreamResumed );
    EUNIT_ASSERT_EQUALS( iEventHandler->iLastEvent.iEventNumData, KMccPayloadSpecificEvent );
    EUNIT_ASSERT_EQUALS( iEventHandler->iLastEvent.iReserved, 100 );
    
    // No event handler
    iSource->iEventHandler = NULL;
    iSource->SendStreamEventToClient( KMccStreamStopped, KErrNone, KMccPTNotDefined );
    EUNIT_ASSERT_EQUALS( iEventHandler->iLastEvent.iEventType, KMccStreamResumed );
    
    // Jitter event when no event handler
    TMccRtpEventDataExtended eventData;
    iSource->SendJitterEvent( eventData, KErrNone );
    EUNIT_ASSERT_EQUALS( iEventHandler->iLastEvent.iEventType, KMccStreamResumed );

    // Event handler exists
    iSource->iEventHandler = iEventHandler;
    iSource->SendJitterEvent( eventData, KErrNone );
    EUNIT_ASSERT_EQUALS( iEventHandler->iLastEvent.iEventType, KMccMediaQualityStatus );
    }
         
void UT_CMccRtpDataSource::UT_CMccRtpDataSource_StandbyL()
    {
    EUNIT_ASSERT_EQUALS( iSource->StandBy( EForceStandby, 100 ), KErrNotFound );
    
    // Standby force when properly initialized
    InitializeL();
   
    EUNIT_ASSERT_EQUALS( iSource->iUsers.Count(), 2 );
    EUNIT_ASSERT_EQUALS( iSource->StandBy( EForceStandby, KPcmuPayloadType ), KErrNone );
    iSource->iState = MMccRtpInterface::ERtpStatePlaying;
    EUNIT_ASSERT_EQUALS( iSource->StandBy( EForceStandby, KPcmuPayloadType ), KErrNone );
    EUNIT_ASSERT( iSource->iUsers[ 0 ].iStandbyState == EInactive );
    
    // Test activation when already inactive
    EUNIT_ASSERT_EQUALS( iSource->StandBy( EActivateStandby, KPcmuPayloadType ), KErrNone );
    EUNIT_ASSERT( iSource->iUsers[ 0 ].iStandbyState == EInactive );
    
    // Test activation when activating
    iSource->iUsers[ 0 ].iStandbyState = EActivating;
    EUNIT_ASSERT_EQUALS( iSource->StandBy( EActivateStandby, KPcmuPayloadType ), KErrNone );
    EUNIT_ASSERT( iSource->iUsers[ 0 ].iStandbyState == EActivating );
    
    // Test activation when inactive
    iSource->iUsers[ 0 ].iStandbyState = EActive;
    EUNIT_ASSERT_EQUALS( iSource->StandBy( EActivateStandby, KPcmuPayloadType ), KErrNone );
    EUNIT_ASSERT( iSource->iUsers[ 0 ].iStandbyState == EInactive );
    
    // Test deactivation
    EUNIT_ASSERT_EQUALS( iSource->StandBy( EDeactivateStandby, KPcmuPayloadType ), KErrNone );
    EUNIT_ASSERT( iSource->iUsers[ 0 ].iStandbyState == ETurnedOff );
    
    // Test activation when standby is turned off (timer expires)
    iSource->iUsers[ 0 ].iTimerId = 2;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iSource->TimerExpiredL( 2, NULL ), KErrNotSupported );
    
    // Test timer expiration for inactivity
    iSource->iInactivityTimerId = 3;
    iSource->TimerExpiredL( 3, NULL );
    EUNIT_ASSERT_EQUALS( iEventHandler->iLastEvent.iEventType, KMccInactivityEvent );
    }
         
void UT_CMccRtpDataSource::UT_CMccRtpDataSource_SourceCustomCommandL()
    {
    // A bit hard to test
    EUNIT_ASSERT( ETrue );
    }
    
void UT_CMccRtpDataSource::UT_CMccRtpDataSource_TestBaseClassL()
    {
    // Test some base class functions which are hard to test by other means
    
    // Internal event to all clients (no users)
    iSource->SendInternalRtpEventToAllClients( 
        iSource->iUsers, KMccRtpSourceUid, EMccInternalEventNone, KMccStreamError, 0, KErrGeneral );
    
    // Users exist
    InitializeL();
    
    iSource->SendInternalRtpEventToAllClients( 
        iSource->iUsers, KMccRtpSourceUid, EMccInternalEventNone, KMccStreamError, 0, KErrGeneral );
    EUNIT_ASSERT_EQUALS( iEventHandler->iLastEvent.iEventType, KMccStreamError );
    
    // Secure events
    iSource->SendSecureRtpEventToClient( 
        NULL, KMccRtpSourceUid, EMccInternalEventNone, KMccStreamPaused, 0 );
    iSource->SendSecureRtpEventToClient( 
        iEventHandler, KMccRtpSourceUid, EMccInternalEventNone, KMccStreamPaused, 0 );
    EUNIT_ASSERT_EQUALS( iEventHandler->iLastEvent.iEventType, KMccStreamPaused );
    
    // SendJitterEventToClient()
    iSource->SendJitterEventToClient( iEventHandler, KMccRtpSourceUid, 
    	EMccInternalEventNone, KMccEventNone, 0, 0, 1,2,3,4,5,6 );
    EUNIT_ASSERT_EQUALS( iEventHandler->iLastEvent.iEventType, KMccEventNone );
    const TMccRtpEventDataExtended& rtpEvent = 
    	(*reinterpret_cast<const TMccRtpEventDataExtendedPackage*>( &iEventHandler->iLastEvent.iEventData ))();
    EUNIT_ASSERT_EQUALS( rtpEvent.iJitterEstimate, 1 );
    EUNIT_ASSERT_EQUALS( rtpEvent.iPacketsReceived, 2 );
    EUNIT_ASSERT_EQUALS( rtpEvent.iPrevTransTime, 3 );
    EUNIT_ASSERT_EQUALS( rtpEvent.iTriggeredJitterLevel, 4 );
    EUNIT_ASSERT_EQUALS( rtpEvent.iPacketLoss, 5 );
    EUNIT_ASSERT_EQUALS( rtpEvent.iTriggeredPacketLoss, 6 );
    }
    
void UT_CMccRtpDataSource::UT_CMccRtpDataSource_DoStandbyDecisionL()
    {
    // Standby not enabled
    iSource->DoStandByDecision( NULL );
    
    // Standby enabled
    InitializeL();
    iSource->DoStandByDecision( &iSource->iUsers[ 0 ] );
    EUNIT_ASSERT( iSource->iUsers[ 0 ].iStandbyState == EActive );
    }
    
void UT_CMccRtpDataSource::UT_CMccRtpDataSource_RegisterPayloadTypesLL()
    {
    RArray<TUint> payloads;
    CleanupClosePushL( payloads );
    payloads.AppendL( KPcmuPayloadType );
    payloads.AppendL( KDefaultAmrNbPT );
    iSource->RegisterPayloadTypesL( payloads );
    EUNIT_ASSERT_EQUALS( iSource->iUsers.Count(), 2 );
    
    // Do again, multiple entries are not created
    iSource->RegisterPayloadTypesL( payloads );
    EUNIT_ASSERT_EQUALS( iSource->iUsers.Count(), 2 );

    // Try to unregister not registered payload type
    RArray<TUint> payloads2;
    CleanupClosePushL( payloads2 );
    payloads2.AppendL( KPcmaPayloadType );
    iSource->UnRegisterPayloadTypes( payloads2 );
    EUNIT_ASSERT_EQUALS( iSource->iUsers.Count(), 2 );
    CleanupStack::PopAndDestroy( &payloads2 );
    
    // Unregister registered payload types
    iSource->UnRegisterPayloadTypes( payloads );
    EUNIT_ASSERT_EQUALS( iSource->iUsers.Count(), 0 );
    CleanupStack::PopAndDestroy( &payloads );
    }

void UT_CMccRtpDataSource::UT_CMccRtpDataSource_SRTPMasterKeyStaleEvent1L()
	{
	//SetUp SecureSession
	TInetAddr destination; 
	destination.Input(_L("127.0.0.1") );
	iSecSession = CSRTPSession::NewL( destination );
	
	//Set up MCCparams
	TMccRtpSessionParams params;
    params.iRtpAPI = iRtpApi;
    params.iSecSession = iSecSession;
    params.iSessionId = iSession;
    params.iEnableRTCP = EFalse;
    params.iRtpKeepalive = iRtpKeepaliveMechanism;
    EUNIT_ASSERT_NO_LEAVE( iSource->SetSessionParamsL( params ) );
    iSource->SourceThreadLogon( *iEventHandler );
    
	iSource->SRTPMasterKeyStaleEvent( *iSecSession );
	EUNIT_ASSERT( iSource->iSecureKeyExpired );
	EUNIT_ASSERT_EQUALS( iEventHandler->iLastEvent.iEventType, KMccMasterKeyStaled );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iSource->SourcePlayL(), KErrGeneral );
    }
    
void UT_CMccRtpDataSource::UT_CMccRtpDataSource_SRTPMasterKeyStaleEvent2L()
	{
	//SetUp SecureSession
	TInetAddr destination; 
	destination.Input(_L("127.0.0.1") );
	iSecSession = CSRTPSession::NewL( destination );
	
	//Set up Secure Stream
	//Secure Context
	
	HBufC8* masterKey = HBufC8::NewL(KRFC3711_TestMasterKey128bits().Length());
    CleanupStack::PushL( masterKey );
    *masterKey = KRFC3711_TestMasterKey128bits;
    
    HBufC8* masterSalt =HBufC8::NewL(KRFC3711_TestMasterSalt112bits().Length());
    CleanupStack::PushL( masterSalt );
	*masterSalt = KRFC3711_TestMasterSalt112bits;
	
	
	HBufC8* mki =HBufC8::NewL(KTestMKI128Bits().Length());
    CleanupStack::PushL( mki );
	*mki = KTestMKI128Bits;
	//Hex(*mki);
	
	CSRTPMasterKey* mKey = CSRTPMasterKey::NewL( *masterKey, *mki );
	CleanupStack::PushL( mKey );
    CSRTPMasterSalt* saltKey = CSRTPMasterSalt::NewL( *masterSalt );
    CleanupStack::PushL( saltKey );
    
    TSrtpCryptoParams srtpParams;
    CSRTPCryptoContext* context = CSRTPCryptoContext::NewL(mKey, saltKey, srtpParams );
    CleanupStack::Pop( saltKey );
    CleanupStack::Pop( mKey );

    CleanupStack::PopAndDestroy( mki );
    CleanupStack::PopAndDestroy( masterSalt );
    CleanupStack::PopAndDestroy( masterKey ); 
    
    CleanupStack::PushL( context );
	
	CSRTPStreamIn* srtpStream = CSRTPStreamIn::NewL( *iSecSession, context, *iSource );
	CleanupStack::Pop( context );
	CleanupStack::PushL( srtpStream );
	
	//Set up MCCparams
	TMccRtpSessionParams params;
    params.iRtpAPI = iRtpApi;
    params.iSecSession = iSecSession;
    params.iSessionId = iSession;
    params.iEnableRTCP = EFalse;
    
    params.iRtpKeepalive = iRtpKeepaliveMechanism;
    EUNIT_ASSERT_NO_LEAVE( iSource->SetSessionParamsL( params ) );
    iSource->SourceThreadLogon( *iEventHandler );
    //srtpStream is not equal
	iSource->SRTPMasterKeyStaleEvent( *srtpStream );
	EUNIT_ASSERT( !iSource->iSecureKeyExpired );
	EUNIT_ASSERT_NOT_EQUALS( iEventHandler->iLastEvent.iEventType, KMccMasterKeyStaled );
    //srtpStream is equal
    iSource->iSrtpStream = srtpStream;
    iSource->SRTPMasterKeyStaleEvent( *srtpStream );
	EUNIT_ASSERT( iSource->iSecureKeyExpired );
	EUNIT_ASSERT_EQUALS( iEventHandler->iLastEvent.iEventType, KMccMasterKeyStaled );
  	EUNIT_ASSERT_SPECIFIC_LEAVE( iSource->SourcePlayL(), KErrGeneral );
  	CleanupStack::Pop( srtpStream );
    }	
	
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMccRtpDataSource,
    "CMccRtpDataSource",
    "UNIT" )
    
EUNIT_TEST(
    "RtpPacketReceived - test ",
    "CMccRtpDataSource",
    "RtpPacketReceived",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSource_RtpPacketReceivedL, Teardown)

EUNIT_TEST(
    "StartInactivityTimerL - test ",
    "CMccRtpDataSource",
    "StartInactivityTimerL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSource_StartInactivityTimerLL, Teardown)

EUNIT_TEST(
    "StopInactivityTimerL - test ",
    "CMccRtpDataSource",
    "StopInactivityTimerL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSource_StopInactivityTimerLL, Teardown)

EUNIT_TEST(
    "SendMediaSignallingL - test ",
    "CMccRtpDataSource",
    "SendMediaSignallingL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSource_SendMediaSignallingLL, Teardown)

EUNIT_TEST(
    "SourceDataTypeCode - test ",
    "CMccRtpDataSource",
    "SourceDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSource_SourceDataTypeCodeL, Teardown)

EUNIT_TEST(
    "SetSourceDataTypeCode - test ",
    "CMccRtpDataSource",
    "SetSourceDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSource_SetSourceDataTypeCodeL, Teardown)

EUNIT_TEST(
    "BufferEmptiedL - test ",
    "CMccRtpDataSource",
    "BufferEmptiedL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSource_BufferEmptiedLL, Teardown)

EUNIT_TEST(
    "CanCreateSourceBuffer - test ",
    "CMccRtpDataSource",
    "CanCreateSourceBuffer",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSource_CanCreateSourceBufferL, Teardown)

EUNIT_TEST(
    "CreateSourceBufferL - test ",
    "CMccRtpDataSource",
    "CreateSourceBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSource_CreateSourceBufferLL, Teardown)

EUNIT_TEST(
    "SourceThreadLogon - test ",
    "CMccRtpDataSource",
    "SourceThreadLogon",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSource_SourceThreadLogonL, Teardown)

EUNIT_TEST(
    "SourceThreadLogoff - test ",
    "CMccRtpDataSource",
    "SourceThreadLogoff",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSource_SourceThreadLogoffL, Teardown)

EUNIT_TEST(
    "SourcePrimeL - test ",
    "CMccRtpDataSource",
    "SourcePrimeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSource_SourcePrimeLL, Teardown)

EUNIT_TEST(
    "SourcePlayL - test ",
    "CMccRtpDataSource",
    "SourcePlayL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSource_SourcePlayLL, Teardown)

EUNIT_TEST(
    "SourcePauseL - test ",
    "CMccRtpDataSource",
    "SourcePauseL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSource_SourcePauseLL, Teardown)

EUNIT_TEST(
    "SourceStopL - test ",
    "CMccRtpDataSource",
    "SourceStopL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSource_SourceStopLL, Teardown)

EUNIT_TEST(
    "FillBufferL - test ",
    "CMccRtpDataSource",
    "FillBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSource_FillBufferLL, Teardown)

EUNIT_TEST(
    "SendRTCPReceiverReport - test ",
    "CMccRtpDataSource",
    "SendRTCPReceiverReport",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSource_SendRTCPReceiverReportL, Teardown)

EUNIT_TEST(
    "SendRTCPSenderReport - test ",
    "CMccRtpDataSource",
    "SendRTCPSenderReport",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSource_SendRTCPSenderReportL, Teardown)

EUNIT_TEST(
    "SendRTCPDataL - test ",
    "CMccRtpDataSource",
    "SendRTCPDataL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSource_SendRTCPDataLL, Teardown)

EUNIT_TEST(
    "NegotiateSourceL - test ",
    "CMccRtpDataSource",
    "NegotiateSourceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSource_NegotiateSourceLL, Teardown)

EUNIT_TEST(
    "ConstructSourceL - test ",
    "CMccRtpDataSource",
    "ConstructSourceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSource_ConstructSourceLL, Teardown)

EUNIT_TEST(
    "ValidatePacket - test ",
    "CMccRtpDataSource",
    "ValidatePacket",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSource_ValidatePacketL, Teardown)

EUNIT_TEST(
    "DoCreateSrtpStreamL - test ",
    "CMccRtpDataSource",
    "DoCreateSrtpStreamL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSource_DoCreateSrtpStreamL, Teardown)

EUNIT_TEST(
    "TestSendEvent - test ",
    "CMccRtpDataSource",
    "TestSendEvent",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSource_TestSendEventL, Teardown)

EUNIT_TEST(
    "Standby - test ",
    "CMccRtpDataSource",
    "Standby",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSource_StandbyL, Teardown)

EUNIT_TEST(
    "SourceCustomCommand - test ",
    "CMccRtpDataSource",
    "SourceCustomCommand",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSource_SourceCustomCommandL, Teardown)

EUNIT_TEST(
    "DoStandbyDecision - test ",
    "CMccRtpDataSource",
    "DoStandbyDecision",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSource_DoStandbyDecisionL, Teardown)

EUNIT_TEST(
    "RegisterPayloadTypesL - test ",
    "CMccRtpDataSource",
    "RegisterPayloadTypesL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSource_RegisterPayloadTypesLL, Teardown)
        
EUNIT_TEST(
    "TestBaseClass - test ",
    "CMccRtpDataSource",
    "TestBaseClass",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSource_TestBaseClassL, Teardown)
    
EUNIT_TEST(
    "SRTPMasterKeyStaleEvent1L - test ",
    "CMccRtpDataSource",
    "SRTPMasterKeyStaleEvent1L",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSource_SRTPMasterKeyStaleEvent1L, Teardown)

EUNIT_TEST(
    "SRTPMasterKeyStaleEvent2L - test ",
    "CMccRtpDataSource",
    "SRTPMasterKeyStaleEvent2L",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSource_SRTPMasterKeyStaleEvent2L, Teardown)
                        
EUNIT_END_TEST_TABLE

//  END OF FILE
