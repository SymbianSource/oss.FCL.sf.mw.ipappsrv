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
#include "UT_CMccRtpDataSink.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <mmf/server/mmfbuffer.h>




//  INTERNAL INCLUDES
#include "rtpheader.h"
#include "MccRtpDataSink.h"
#include <mmf/server/mmfdatasink.h>
#include <mmf/common/mmfcontrollerframework.h>
#include "MmccEvents.h"
#include "MccInternalCodecs.h"
#include <mmf/server/mmfvideoframebuffer.h>
#include "mccrtpkeepalivecontainer.h"
#include "amrpayloadformatwrite.h"
#include <srtpcryptocontext.h>
#include <srtpmastersalt.h>
#include <srtpstreamout.h>
#include "mccunittestmacros.h"
#include "mccrtpmediaclock.h"


#define MCCEVENTDATAPCKG_TO_RTCPEVENTDATAPCKG( event )\
    *reinterpret_cast<TMccRtcpEventDataPackage*>( &event.iEventData )

// CONSTANTS
const TUint8 KPayloadType( 96 );
_LIT8(KRFC3711_TestMasterKey128bits,    "E1F97A0D3E018BE0D64FA32C06DE4139");
_LIT8(KRFC3711_TestMasterSalt112bits,   "0EC675AD498AFEEBB6960B3AABE6");	
_LIT8(KTestMKI128Bits,      "ABCDEF1234567890ABCDEF1234567890");


// CONSTRUCTION
UT_CMccRtpDataSink* UT_CMccRtpDataSink::NewL()
    {
    UT_CMccRtpDataSink* self = UT_CMccRtpDataSink::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMccRtpDataSink* UT_CMccRtpDataSink::NewLC()
    {
    UT_CMccRtpDataSink* self = new( ELeave ) UT_CMccRtpDataSink();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccRtpDataSink::~UT_CMccRtpDataSink()
    {
    }

// Default constructor
UT_CMccRtpDataSink::UT_CMccRtpDataSink()
    : iSession( 1 )
    {
    }

// Second phase construct
void UT_CMccRtpDataSink::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

// From MRtpErrNotify
void UT_CMccRtpDataSink::ErrorNotify( TInt /*aErrCode*/ )
    {
    }


void UT_CMccRtpDataSink::SetupL()
    {
    iRtpApi = CRtpAPI::NewL( *this );
    TUid dummyUid( TUid::Uid( 42 ) ); 
    TBuf8<5> dummyBuf( _L8( "foo" ) );
    iSink = static_cast<CMccRtpDataSink*>(
        CMccRtpDataSink::NewSinkL( dummyUid, dummyBuf ) );
    
    MAsyncEventHandler* eventHandler = NULL;
    TRtpId rtpSessionId(0);
    iRtpKeepaliveMechanism = 
        CMccRtpKeepaliveContainer::NewL( *eventHandler, 
                                         *iRtpApi, 
                                         rtpSessionId ); 
                                         
    iRtpMediaClock = CMccRtpMediaClock::NewL();                                     
    } 

void UT_CMccRtpDataSink::Teardown()
    {
    delete iRtpKeepaliveMechanism;
    
    if ( iSink )
        {
        delete iSink;
        iSink = NULL;
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
        
    if ( iRtpMediaClock )
        {
        delete iRtpMediaClock;
        iRtpMediaClock = NULL;
        }
    }

// HELPERS
//

void UT_CMccRtpDataSink::InitializeStreamL()
    {
    iSink->SinkThreadLogon( *this );
    iSink->SetCurrentUser( this );
    TMccRtpSessionParams params;
    params.iRtpAPI = iRtpApi;
    params.iSessionId = iSession;
    params.iEnableRTCP = ETrue;
    params.iRtpKeepalive = iRtpKeepaliveMechanism;

    iSink->SetSessionParamsL( params );

    TMccCodecInfo cInfo;
    TMccCodecInfoBuffer cInfoBuf( cInfo );
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iSink->ConfigureL( cInfoBuf, iRtpMediaClock ) );
    }


// TEST FUNCTIONS
//
void UT_CMccRtpDataSink::UT_CMccRtpDataSink_MuteL()
    {
    const TUint8 KDtmfPayloadType( 98 );
    TRtpSendHeader header;
    header.iPayloadType = KDtmfPayloadType;
    iSink->iState = MMccRtpInterface::ERtpStatePlaying;
    iSink->Mute( ETrue, KDtmfPayloadType );
    EUNIT_ASSERT( ETrue == iSink->SendingAllowed( header ) );
    
    header.iPayloadType = 0;
    EUNIT_ASSERT( EFalse == iSink->SendingAllowed( header ) );
    
    iSink->Mute( EFalse, KDtmfPayloadType );
    EUNIT_ASSERT( ETrue == iSink->SendingAllowed( header ) );
    }

void UT_CMccRtpDataSink::UT_CMccRtpDataSink_SendMediaSignallingLL()
    {
    TMccEvent wrongEvent;
    wrongEvent.iEventCategory = KMccEventCategoryDtmf;
    // Wrong type of signal
    EUNIT_ASSERT_LEAVE( iSink->SendMediaSignallingL( wrongEvent ) );

    TMccEvent event;
    event.iEventCategory = KMccEventCategoryRtcp;
    event.iEventType = KMccRtcpControl;
    
    TMccRtcpEventData data;
    TMccRtcpEventDataPackage dataPackage( data );
    event.iEventData.Copy( dataPackage );
    
    // No RTP API set
    EUNIT_ASSERT_LEAVE( iSink->SendMediaSignallingL( event ) );

    TMccRtpSessionParams params;
    params.iRtpAPI = iRtpApi;
    params.iSessionId = iSession;
    params.iEnableRTCP = EFalse;
    params.iRtpKeepalive= iRtpKeepaliveMechanism;
    EUNIT_ASSERT_NO_LEAVE( iSink->SetSessionParamsL( params ) );

    // RTCP not enabled
    EUNIT_ASSERT_LEAVE( iSink->SendMediaSignallingL( event ) );

    params.iEnableRTCP = ETrue;
    EUNIT_ASSERT_NO_LEAVE( iSink->SetSessionParamsL( params ) );

    // Wrong packet type
    EUNIT_ASSERT_LEAVE( iSink->SendMediaSignallingL( event ) );
    
    // Streams and RTP sender not constructed
    data.iRtcpPacketType = KRtcpAnyPacket;
    EUNIT_ASSERT_LEAVE( iSink->SendMediaSignallingL( event ) );
    
    TMccCodecInfo cInfo;
    TMccCodecInfoBuffer cInfoBuf( cInfo );
    MCC_EUNIT_ASSERT_NO_LEAVE( iSink->ConfigureL( cInfoBuf, iRtpMediaClock ) );
    MCC_EUNIT_ASSERT_EQUALS( iSink->SinkThreadLogon( *this ), KErrNone );
    iSink->SetCurrentUser( this );
    EUNIT_ASSERT_NO_LEAVE( iSink->SinkPrimeL() );

    TMccRtcpEventDataPackage& eventDataPackage = MCCEVENTDATAPCKG_TO_RTCPEVENTDATAPCKG( event );
    TMccRtcpEventData eventData;
    eventData.iRtcpPacketType = KRtcpSdesPacket;
    eventDataPackage = TMccRtcpEventDataPackage( eventData );

    // No RTCP data, but does not leave because empty TRtpSdesParams
    // will be constructed from zero length RTCP data
    EUNIT_ASSERT_NO_LEAVE( iSink->SendMediaSignallingL( event ) );
    
    TRtpSdesParams sdesParams;
    TPckgBuf<TRtpSdesParams> sdesPackage( sdesParams );
    
    eventData.iRtcpPacketData.Copy( sdesPackage );
    eventDataPackage = TMccRtcpEventDataPackage( eventData );
    
    EUNIT_ASSERT_NO_LEAVE( iSink->SendMediaSignallingL( event ) );

    TBuf<5> reason( _L( "foo" ) );
    eventData.iRtcpPacketType = KRtcpByePacket;
    eventData.iRtcpPacketData.Copy( reason );
    eventDataPackage = TMccRtcpEventDataPackage( eventData );
    EUNIT_ASSERT_NO_LEAVE( iSink->SendMediaSignallingL( event ) );

    TRtcpApp app;
    eventData.iRtcpPacketType = KRtcpAppPacket;
    TPckgBuf<TRtcpApp> appPackage( app );
    eventData.iRtcpPacketData.Copy( appPackage );
    eventDataPackage = TMccRtcpEventDataPackage( eventData );
    
    EUNIT_ASSERT_NO_LEAVE( iSink->SendMediaSignallingL( event ) );

    }

void UT_CMccRtpDataSink::UT_CMccRtpDataSink_EmptyBufferLL()
    {
    EUNIT_ASSERT_LEAVE( iSink->EmptyBufferL( NULL, NULL, TMediaId() ) );
    }

void UT_CMccRtpDataSink::UT_CMccRtpDataSink_SinkThreadLogonL()
    {
    MCC_EUNIT_ASSERT_EQUALS( iSink->SinkThreadLogon( *this ), KErrNone );
    iSink->SetCurrentUser( this );
    }

void UT_CMccRtpDataSink::UT_CMccRtpDataSink_SinkThreadLogoffL()
    {
    iSink->SinkThreadLogoff();

    TMccRtpSessionParams params;
    params.iRtpAPI = iRtpApi;
    params.iSessionId = iSession;
    params.iEnableRTCP = ETrue;
    params.iRtpKeepalive = iRtpKeepaliveMechanism;
    EUNIT_ASSERT_NO_LEAVE( iSink->SetSessionParamsL( params ) );
    iSink->SinkThreadLogoff();
    
    // Reach more branches
    TUint8 redPt( 97 );
    TMccCodecInfo cInfo;
    cInfo.iRedundancyCount = 1;
    cInfo.iRedundantPayload = redPt;
    TMccCodecInfoBuffer cInfoBuf( cInfo );
    
    MCC_EUNIT_ASSERT_EQUALS( iSink->SinkThreadLogon( *this ), KErrNone );
    iSink->SetCurrentUser( this );
    EUNIT_ASSERT_NO_LEAVE( iSink->SetSessionParamsL( params ) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iSink->ConfigureL( cInfoBuf, iRtpMediaClock ) );
    EUNIT_ASSERT_NO_LEAVE( iSink->SinkPrimeL() );
    iSink->SinkThreadLogoff();
    }

void UT_CMccRtpDataSink::UT_CMccRtpDataSink_SinkDataTypeCodeL()
    {
    TMediaId mediaId( KUidMediaTypeMidi );
    EUNIT_ASSERT_EQUALS( iSink->SinkDataTypeCode( mediaId ), TFourCC() );

    iSink->SinkThreadLogoff();
    TFourCC codec( ' ','A','M','R' );
    TMediaId media2( KUidMediaTypeAudio );
    iSink->SetSinkDataTypeCode( codec, media2 );

    // The codecs SHOULD be equal
    EUNIT_ASSERT_EQUALS( iSink->SinkDataTypeCode( media2 ), codec );
    
    TFourCC codec2( ' H','2','6','3' );
    TMediaId media3( KUidMediaTypeVideo );
    iSink->SetSinkDataTypeCode( codec2, media3 );

    // The codecs SHOULD be equal
    EUNIT_ASSERT_EQUALS( iSink->SinkDataTypeCode( media3 ), codec2 );
    }

void UT_CMccRtpDataSink::UT_CMccRtpDataSink_SetSinkDataTypeCodeL()
    {
    TFourCC codec;
    TMediaId media( KUidMediaTypeMidi ); // wrong media type
    EUNIT_ASSERT_EQUALS( iSink->SetSinkDataTypeCode( codec, media ), KErrNotSupported );

    // Set the correct media type
    TMediaId media2( KUidMediaTypeAudio );
    EUNIT_ASSERT_EQUALS( iSink->SetSinkDataTypeCode( codec, media2 ), KErrNone );
    TMediaId media3( KUidMediaTypeVideo );
    EUNIT_ASSERT_EQUALS( iSink->SetSinkDataTypeCode( codec, media3 ), KErrNone );
    }

void UT_CMccRtpDataSink::UT_CMccRtpDataSink_BufferFilledLL()
    {
    EUNIT_ASSERT_LEAVE( iSink->BufferFilledL( NULL ) );
    }

void UT_CMccRtpDataSink::UT_CMccRtpDataSink_CanCreateSinkBufferL()
    {
    EUNIT_ASSERT_EQUALS( iSink->CanCreateSinkBuffer(), EFalse );
    }

void UT_CMccRtpDataSink::UT_CMccRtpDataSink_CreateSinkBufferLL()
    {
    TBool dummy;
    EUNIT_ASSERT_LEAVE( iSink->CreateSinkBufferL( TMediaId(), dummy ) );
    }

void UT_CMccRtpDataSink::UT_CMccRtpDataSink_SinkPrimeLL()
    {
    // Set wrong state
    iSink->SinkThreadLogoff();
    EUNIT_ASSERT_LEAVE( iSink->SinkPrimeL() );

    // Set correct state
    iSink->SinkThreadLogon( *this );
    iSink->SetCurrentUser( this );
    EUNIT_ASSERT_LEAVE( iSink->SinkPrimeL() );

    TMccRtpSessionParams params;
    params.iRtpAPI = iRtpApi;
    params.iSessionId = iSession;
    params.iEnableRTCP = ETrue;
    params.iRtpKeepalive = iRtpKeepaliveMechanism;
    iSink->SetSessionParamsL( params );
    EUNIT_ASSERT_LEAVE( iSink->SinkPrimeL() );

    TMccCodecInfo cInfo;
    TMccCodecInfoBuffer cInfoBuf( cInfo );
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iSink->ConfigureL( cInfoBuf, iRtpMediaClock ) );
    EUNIT_ASSERT_NO_LEAVE( iSink->SinkPrimeL() );
    EUNIT_ASSERT( iRtpKeepaliveMechanism->iStopped == EFalse );
  
    // "Secure session"
    iRtpKeepaliveMechanism->iStopped = ETrue;
    TInt fakeSecSession( 3 );
    iSink->iSecSession = reinterpret_cast<CSRTPSession*>( &fakeSecSession );
    EUNIT_ASSERT_NO_LEAVE( iSink->SinkPrimeL() );
    EUNIT_ASSERT( iRtpKeepaliveMechanism->iStopped == ETrue )
    iSink->iSecSession = NULL;
    }

void UT_CMccRtpDataSink::UT_CMccRtpDataSink_SinkPlayLL()
    {
    // Wrong state
    EUNIT_ASSERT_LEAVE( iSink->SinkPlayL() );

    // Do it right
    InitializeStreamL();
    
    iSink->SinkPrimeL();
    EUNIT_ASSERT_NO_LEAVE( iSink->SinkPlayL() );
    
    // State downgrade is ignored
    EUNIT_ASSERT_NO_LEAVE( iSink->SinkPrimeL() );
    EUNIT_ASSERT( MMccRtpInterface::ERtpStatePlaying == iSink->State() );
    }

void UT_CMccRtpDataSink::UT_CMccRtpDataSink_SinkPauseLL()
    {
    // Wrong state
    EUNIT_ASSERT_LEAVE( iSink->SinkPauseL() );

    // Do it right
    InitializeStreamL();

    iSink->SinkPrimeL();
    iSink->SinkPlayL();
    EUNIT_ASSERT_NO_LEAVE( iSink->SinkPauseL() );
    }

void UT_CMccRtpDataSink::UT_CMccRtpDataSink_SinkStopLL()
    {
    // Do it right
    InitializeStreamL();

    iSink->SinkPrimeL();
    iSink->SinkPlayL();
    EUNIT_ASSERT_NO_LEAVE( iSink->SinkStopL() );
    }

void UT_CMccRtpDataSink::UT_CMccRtpDataSink_EmptyBufferL_2L()
    {
    CMMFBuffer* buf( NULL );
    TMediaId mediaId;
    TRtpSendHeader header;
    
    
    InitializeStreamL();
    
    //initial srtp realted
    iRtpKeepaliveMechanism->iStopped = ETrue;
    TInt fakeSecSession( 3 );
    iSink->iSecSession = reinterpret_cast<CSRTPSession*>( &fakeSecSession );
    iSink->iSecureKeyExpired = ETrue;
    iSink->iState = MMccRtpInterface::ERtpStatePlaying;
    EUNIT_ASSERT( iRtpKeepaliveMechanism->iStopped == ETrue )
    
    //null buffer
    EUNIT_ASSERT_LEAVE( iSink->EmptyBufferL( buf, NULL, mediaId, header ) );

    // Try different buffer types, null source
    CMMFDataBuffer* dataBuf = CMMFDataBuffer::NewL();
    EUNIT_ASSERT_LEAVE( iSink->EmptyBufferL( buf, NULL, mediaId, header ) );
    delete dataBuf;

    TBuf8<5> foo;
    foo.Format( _L8( "foo42" ) );
    TUint8* fooPtr = const_cast<TUint8*>( foo.Ptr() );
    TPtr8 ptr( fooPtr, foo.Length() ) ;
    CMMFPtrBuffer* ptrBuf = CMMFPtrBuffer::NewL( ptr );
    EUNIT_ASSERT_LEAVE( iSink->EmptyBufferL( buf, NULL, mediaId, header ) );
    delete ptrBuf;

    CMMFYUVBuffer* yuvBuf = CMMFYUVBuffer::NewL();
    EUNIT_ASSERT_SPECIFIC_LEAVE( iSink->EmptyBufferL( yuvBuf, NULL, mediaId, header ), KErrNotSupported );
    delete yuvBuf;

    // packet drop because of secure keyStream not playing
    CAmrPayloadFormatWrite* source = CAmrPayloadFormatWrite::NewL( iSink );
    CleanupStack::PushL( source );
    CMMFDataBuffer* dataBuf2 = CMMFDataBuffer::NewL();
    CleanupStack::PushL( dataBuf2 );
    EUNIT_ASSERT_NO_LEAVE( iSink->EmptyBufferL( dataBuf2, source, mediaId, header ) );
    
    // Stream playing
    iSink->SinkPrimeL();
    //becuase of secure key packet drop
    EUNIT_ASSERT_SPECIFIC_LEAVE(iSink->SinkPlayL(), KErrGeneral );
    iSink->iSecureKeyExpired = EFalse;
    iSink->SinkPlayL();
    //packet drop because of secure stream not there
    EUNIT_ASSERT_NO_LEAVE( iSink->EmptyBufferL( dataBuf2, source, mediaId, header ) );
    CleanupStack::PopAndDestroy( dataBuf2 );
    CleanupStack::PopAndDestroy( source );
    iSink->iSecSession = NULL;
    
    }


void UT_CMccRtpDataSink::UT_CMccRtpDataSink_EmptyBufferL_1L()
    {
    CMMFBuffer* buf( NULL );
    TMediaId mediaId;
    TRtpSendHeader header;
    
    // No stream
    EUNIT_ASSERT_LEAVE( iSink->EmptyBufferL( buf, NULL, mediaId, header ) );

    // NULL buffer
    InitializeStreamL();
    EUNIT_ASSERT_LEAVE( iSink->EmptyBufferL( buf, NULL, mediaId, header ) );

    // Try different buffer types, null source
    CMMFDataBuffer* dataBuf = CMMFDataBuffer::NewL();
    EUNIT_ASSERT_LEAVE( iSink->EmptyBufferL( buf, NULL, mediaId, header ) );
    delete dataBuf;

    TBuf8<5> foo;
    foo.Format( _L8( "foo42" ) );
    TUint8* fooPtr = const_cast<TUint8*>( foo.Ptr() );
    TPtr8 ptr( fooPtr, foo.Length() ) ;
    CMMFPtrBuffer* ptrBuf = CMMFPtrBuffer::NewL( ptr );
    EUNIT_ASSERT_LEAVE( iSink->EmptyBufferL( buf, NULL, mediaId, header ) );
    delete ptrBuf;

    CMMFYUVBuffer* yuvBuf = CMMFYUVBuffer::NewL();
    EUNIT_ASSERT_SPECIFIC_LEAVE( iSink->EmptyBufferL( yuvBuf, NULL, mediaId, header ), KErrNotSupported );
    delete yuvBuf;

    // Stream not playing
    CAmrPayloadFormatWrite* source = CAmrPayloadFormatWrite::NewL( iSink );
    CleanupStack::PushL( source );
    CMMFDataBuffer* dataBuf2 = CMMFDataBuffer::NewL();
    CleanupStack::PushL( dataBuf2 );
    EUNIT_ASSERT_NO_LEAVE( iSink->EmptyBufferL( dataBuf2, source, mediaId, header ) );
    
    // Stream playing
    iSink->SinkPrimeL();
    iSink->SinkPlayL();
    MCC_EUNIT_ASSERT_NO_LEAVE( iSink->EmptyBufferL( dataBuf2, source, mediaId, header ) );
    CleanupStack::PopAndDestroy( dataBuf2 );
    CleanupStack::PopAndDestroy( source );
    
    // TBD: test also scenario where failure occurs in rtp packet sending
    }

void UT_CMccRtpDataSink::UT_CMccRtpDataSink_SendRTCPReceiverReportL()
    {
    // No RTP API set
    EUNIT_ASSERT_EQUALS( iSink->SendRTCPReceiverReport(), KErrNotReady );

    TMccRtpSessionParams params;
    params.iRtpAPI = iRtpApi;
    params.iSessionId = iSession;
    params.iEnableRTCP = ETrue;      
    params.iRtpKeepalive = iRtpKeepaliveMechanism;

    iSink->SetSessionParamsL( params );

    EUNIT_ASSERT_NO_LEAVE( iSink->SendRTCPReceiverReport() );
    }

void UT_CMccRtpDataSink::UT_CMccRtpDataSink_SendRTCPSenderReportL()
    {
    // No RTP API set
    EUNIT_ASSERT_EQUALS( iSink->SendRTCPSenderReport(), KErrNotReady );

    TMccRtpSessionParams params;
    params.iRtpAPI = iRtpApi;
    params.iSessionId = iSession;
    params.iEnableRTCP = ETrue;
    params.iRtpKeepalive = iRtpKeepaliveMechanism;

    iSink->SetSessionParamsL( params );
    EUNIT_ASSERT_NO_LEAVE( iSink->SendRTCPSenderReport() );
    }

void UT_CMccRtpDataSink::UT_CMccRtpDataSink_SendRTCPDataLL()
    {
    TBuf8<5> data;
    data.Format( _L8( "foo42" ) );
    
    // No RTP API set
    EUNIT_ASSERT_SPECIFIC_LEAVE( iSink->SendRTCPDataL( data ), KErrNotReady );
    
    TMccRtpSessionParams params;
    params.iRtpAPI = iRtpApi;
    params.iSessionId = iSession;
    params.iEnableRTCP = ETrue;
    params.iRtpKeepalive = iRtpKeepaliveMechanism;
    EUNIT_ASSERT_NO_LEAVE( iSink->SetSessionParamsL( params ) );
    
    // No RTP sender constructed
    EUNIT_ASSERT_SPECIFIC_LEAVE( iSink->SendRTCPDataL( data ), KErrNotReady );
    
    // RTP sender constructed
    iSink->DoCreateStreamL();
    iSink->SendRTCPDataL( data );
    }

void UT_CMccRtpDataSink::UT_CMccRtpDataSink_ConstructSinkLL()
    {
    TBuf8<5> data;
    data.Format( _L8( "foo42" ) );

    // Can't have init data
    EUNIT_ASSERT_LEAVE( iSink->ConstructSinkL( data ) );

    EUNIT_ASSERT_NO_LEAVE( iSink->ConstructSinkL( KNullDesC8 ) );
    }


// FUNCTIONS INHERITED FROM CMccRtpInterface

void UT_CMccRtpDataSink::UT_CMccRtpDataSink_PrepareLL()
    {
    TFourCC codec( ' ','A','M','R' );

    // No session exists
    EUNIT_ASSERT_LEAVE( iSink->SinkPrimeL() );
    
    iSink->SinkThreadLogon( *this );
    iSink->SetCurrentUser( this );
    TMccRtpSessionParams params;
    params.iRtpAPI = iRtpApi;
    params.iSessionId = iSession;
    params.iEnableRTCP = ETrue;
    params.iRtpKeepalive = iRtpKeepaliveMechanism;

    iSink->SetSessionParamsL( params );

    // No stream exists
    EUNIT_ASSERT_LEAVE( iSink->SinkPrimeL() );

    TMccCodecInfo cInfo;
    TMccCodecInfoBuffer cInfoBuf( cInfo );
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iSink->ConfigureL( cInfoBuf, iRtpMediaClock ) );
    EUNIT_ASSERT_NO_LEAVE( iSink->SinkPrimeL() ); 

    // Create a comfort noise stream
    //codec = KMccFourCCIdG711;
    //EUNIT_ASSERT_NO_LEAVE( iSink->SinkPrimeL() );
    }

void UT_CMccRtpDataSink::UT_CMccRtpDataSink_CreateStreamLL()
    {
    iSink->SinkThreadLogon( *this );
    iSink->SetCurrentUser( this );
    TMccRtpSessionParams params;
    params.iRtpAPI = iRtpApi;
    params.iSessionId = iSession;
    params.iEnableRTCP = ETrue;
    params.iRtpKeepalive = iRtpKeepaliveMechanism;

    iSink->SetSessionParamsL( params );

    TMccCodecInfo cInfo;
    TMccCodecInfoBuffer cInfoBuf( cInfo );
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iSink->ConfigureL( cInfoBuf, iRtpMediaClock ) );

    // This should leave because the string is too short
    TBuf8<5> dummy;
    dummy.Format( _L8( "foo42" ) );
    EUNIT_ASSERT_LEAVE( iSink->ConfigureL( dummy, iRtpMediaClock ) );

    // Get a new sink
    //Teardown();
    if( iRtpKeepaliveMechanism )
    	{
    	delete iRtpKeepaliveMechanism;
    	iRtpKeepaliveMechanism = NULL;
    	}
    
    if ( iSink )
        {
        delete iSink;
        iSink = NULL;
        }
        
    //SetupL();
    TUid dummyUid( TUid::Uid( 42 ) ); 
    TBuf8<5> dummyBuf( _L8( "foo" ) );
    iSink = static_cast<CMccRtpDataSink*>(
        CMccRtpDataSink::NewSinkL( dummyUid, dummyBuf ) );

	MAsyncEventHandler* eventHandler = NULL;
    TRtpId rtpSessionId(0);
    iRtpKeepaliveMechanism = 
        CMccRtpKeepaliveContainer::NewL( *eventHandler, 
                                         *iRtpApi, 
                                         rtpSessionId );

    iSink->SinkThreadLogon( *this );
    iSink->SetCurrentUser( this );
    TMccRtpSessionParams params2;
    params2.iRtpAPI = iRtpApi;
    params2.iSessionId = iSession;
    params2.iEnableRTCP = ETrue;
    params2.iRtpKeepalive = iRtpKeepaliveMechanism;

    iSink->SetSessionParamsL( params2 );
    
    TMccCodecInfo cInfo2;
    cInfo2.iFourCC = TFourCC( KMccFourCCIdG711 );
    cInfo2.iEnableDTX = ETrue;
    cInfo2.iRedundantPayload = 97;
    cInfo2.iRedundancyCount = 1;
    cInfo2.iAlgoUsed = EGenRedUsed;
    TMccCodecInfoBuffer cInfoBuf2( cInfo2 );

    EUNIT_ASSERT_NO_LEAVE( iSink->ConfigureL( cInfoBuf2, iRtpMediaClock ) );
    }

void UT_CMccRtpDataSink::UT_CMccRtpDataSink_RemoveStreamLL()
    {
/*
    // No RTP API set
    EUNIT_ASSERT_LEAVE( iSink->RemoveStreamL( 42 ) );

    // Do it right
    iSink->SinkThreadLogon( *this );
    iSink->SetCurrentUser( this );
    TMccRtpSessionParams params;
    params.iRtpAPI = iRtpApi;
    params.iSessionId = iSession;
    params.iEnableRTCP = ETrue;
    params.iEventHandler = this;
    iSink->SetSessionParamsL( params );

    TMccRtpStreamParams strParams;
    strParams.iPayloadType = 96;
    strParams.iCodec = TFourCC( ' ','A','M','R' );
    strParams.iSampleRate = 8000;
    strParams.iEnableVAD = EFalse;
    TRtpSSRC ssrc( 5 );
    strParams.iSSRC = &ssrc;

    TUint32 stream( iSink->CreateStreamL( strParams ) );

    EUNIT_ASSERT_NO_LEAVE( iSink->RemoveStreamL( stream ) );
*/
    }

void UT_CMccRtpDataSink::UT_CMccRtpDataSink_PlayStreamLL()
    {
    // No stream
    EUNIT_ASSERT_LEAVE( iSink->SinkPlayL() );

    iSink->SinkThreadLogon( *this );
    iSink->SetCurrentUser( this );
    TMccRtpSessionParams params;
    params.iRtpAPI = iRtpApi;
    params.iSessionId = iSession;
    params.iEnableRTCP = ETrue;
    params.iRtpKeepalive = iRtpKeepaliveMechanism;

    iSink->SetSessionParamsL( params );
    
    TMccCodecInfo cInfo;
    TMccCodecInfoBuffer cInfoBuf( cInfo );
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iSink->ConfigureL( cInfoBuf, iRtpMediaClock ) );

    // Wrong state
    EUNIT_ASSERT_LEAVE( iSink->SinkPlayL() );

    iSink->SinkPrimeL();

    // Ok
    EUNIT_ASSERT_NO_LEAVE( iSink->SinkPlayL() );
    }

void UT_CMccRtpDataSink::UT_CMccRtpDataSink_PauseStreamLL()
    {
    // No stream
    EUNIT_ASSERT_LEAVE( iSink->SinkPauseL() );

    iSink->SinkThreadLogon( *this );
    iSink->SetCurrentUser( this );
    TMccRtpSessionParams params;
    params.iRtpAPI = iRtpApi;
    params.iSessionId = iSession;
    params.iEnableRTCP = ETrue;
    params.iRtpKeepalive = iRtpKeepaliveMechanism;

    iSink->SetSessionParamsL( params );
    
    TMccCodecInfo cInfo;
    TMccCodecInfoBuffer cInfoBuf( cInfo );
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iSink->ConfigureL( cInfoBuf, iRtpMediaClock ) );

    // Wrong state
    EUNIT_ASSERT_LEAVE( iSink->SinkPauseL() );

    iSink->SinkPrimeL();
    iSink->SinkPlayL();

    // Ok
    EUNIT_ASSERT_NO_LEAVE( iSink->SinkPauseL() );
    }

void UT_CMccRtpDataSink::UT_CMccRtpDataSink_StopStreamLL()
    {
    iSink->SinkThreadLogon( *this );
    iSink->SetCurrentUser( this );
    TMccRtpSessionParams params;
    params.iRtpAPI = iRtpApi;
    params.iSessionId = iSession;
    params.iEnableRTCP = ETrue;
    params.iRtpKeepalive = iRtpKeepaliveMechanism;

    iSink->SetSessionParamsL( params );
    
    TMccCodecInfo cInfo;
    TMccCodecInfoBuffer cInfoBuf( cInfo );
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iSink->ConfigureL( cInfoBuf, iRtpMediaClock ) );
    iSink->SinkPrimeL();
    iSink->SinkPlayL();

    // Ok
    EUNIT_ASSERT_NO_LEAVE( iSink->SinkStopL() );
    }

void UT_CMccRtpDataSink::UT_CMccRtpDataSink_StartInactivityTimerLL()
    {
    TUint32 dummyTime( 0 );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iSink->StartInactivityTimerL( dummyTime ),
                                 KErrNotSupported );
    }

void UT_CMccRtpDataSink::UT_CMccRtpDataSink_StopInactivityTimerLL()
    {
    EUNIT_ASSERT_SPECIFIC_LEAVE( iSink->StopInactivityTimerL(),
                                 KErrNotSupported );
    }

void UT_CMccRtpDataSink::UT_CMccRtpDataSink_SetSessionParamsLL()
    {
    TMccRtpSessionParams params;
    params.iRtpAPI = iRtpApi;
    params.iSessionId = iSession;
    params.iEnableRTCP = ETrue;
    params.iRtpKeepalive = iRtpKeepaliveMechanism;

    EUNIT_ASSERT_NO_LEAVE( iSink->SetSessionParamsL( params ) );
    }

void UT_CMccRtpDataSink::UT_CMccRtpDataSink_SRTPMasterKeyStaleEvent1L()
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
    EUNIT_ASSERT_NO_LEAVE( iSink->SetSessionParamsL( params ) );
    iSink->SinkThreadLogon( *this );
    iSink->SetCurrentUser( this);
    
	iSink->SRTPMasterKeyStaleEvent( *iSecSession );
	EUNIT_ASSERT( iSink->iSecureKeyExpired );
	EUNIT_ASSERT_EQUALS( iEventType, KMccMasterKeyStaled );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iSink->SinkPlayL(), KErrGeneral );
    }
    
void UT_CMccRtpDataSink::UT_CMccRtpDataSink_SRTPMasterKeyStaleEvent2L()
	{
	this->iEventType = KErrNone;
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
    CleanupStack::PushL( masterSalt);
	*masterSalt = KRFC3711_TestMasterSalt112bits;
	
	
	HBufC8* mki =HBufC8::NewL(KTestMKI128Bits().Length());
    CleanupStack::PushL( mki);
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
	
	CSRTPStreamOut* srtpStream = CSRTPStreamOut::NewL( *iSecSession, iSink->GetSSRC(), context, *iSink );
	CleanupStack::Pop( context );
	CleanupStack::PushL( srtpStream );
	
	//Set up MCCparams
	TMccRtpSessionParams params;
    params.iRtpAPI = iRtpApi;
    params.iSecSession = iSecSession;
    params.iSessionId = iSession;
    params.iEnableRTCP = EFalse;
    
    params.iRtpKeepalive = iRtpKeepaliveMechanism;
    EUNIT_ASSERT_NO_LEAVE( iSink->SetSessionParamsL( params ) );
    iSink->SinkThreadLogon( *this );
    iSink->SetCurrentUser( this);
    
    //srtpStream is not equal
	iSink->SRTPMasterKeyStaleEvent( *srtpStream );
	EUNIT_ASSERT( !iSink->iSecureKeyExpired );
	EUNIT_ASSERT_NOT_EQUALS( this->iEventType, KMccMasterKeyStaled );
    //srtpStream is equal
    iSink->iSrtpStream = srtpStream;
    iSink->SRTPMasterKeyStaleEvent( *srtpStream );
	EUNIT_ASSERT( iSink->iSecureKeyExpired );
	EUNIT_ASSERT_EQUALS( iEventType, KMccMasterKeyStaled );
  	EUNIT_ASSERT_SPECIFIC_LEAVE( iSink->SinkPlayL(), KErrGeneral );
  	CleanupStack::Pop( srtpStream );
    }	
	
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMccRtpDataSink,
    "MccRtpDataSink",
    "UNIT" )

EUNIT_TEST(
    "MuteL - test ",
    "CMccRtpDataSink",
    "MuteL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSink_MuteL, Teardown)

EUNIT_TEST(
    "SendMediaSignallingL - test ",
    "CMccRtpDataSink",
    "SendMediaSignallingL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSink_SendMediaSignallingLL, Teardown)

EUNIT_TEST(
    "EmptyBufferL - test ",
    "CMccRtpDataSink",
    "EmptyBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSink_EmptyBufferLL, Teardown)

EUNIT_TEST(
    "SinkThreadLogon - test ",
    "CMccRtpDataSink",
    "SinkThreadLogon",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSink_SinkThreadLogonL, Teardown)

EUNIT_TEST(
    "SinkThreadLogoff - test ",
    "CMccRtpDataSink",
    "SinkThreadLogoff",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSink_SinkThreadLogoffL, Teardown)

EUNIT_TEST(
    "SinkDataTypeCode - test ",
    "CMccRtpDataSink",
    "SinkDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSink_SinkDataTypeCodeL, Teardown)

EUNIT_TEST(
    "SetSinkDataTypeCode - test ",
    "CMccRtpDataSink",
    "SetSinkDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSink_SetSinkDataTypeCodeL, Teardown)

EUNIT_TEST(
    "BufferFilledL - test ",
    "CMccRtpDataSink",
    "BufferFilledL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSink_BufferFilledLL, Teardown)

EUNIT_TEST(
    "CanCreateSinkBuffer - test ",
    "CMccRtpDataSink",
    "CanCreateSinkBuffer",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSink_CanCreateSinkBufferL, Teardown)

EUNIT_TEST(
    "CreateSinkBufferL - test ",
    "CMccRtpDataSink",
    "CreateSinkBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSink_CreateSinkBufferLL, Teardown)

EUNIT_TEST(
    "SinkPrimeL - test ",
    "CMccRtpDataSink",
    "SinkPrimeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSink_SinkPrimeLL, Teardown)

EUNIT_TEST(
    "SinkPlayL - test ",
    "CMccRtpDataSink",
    "SinkPlayL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSink_SinkPlayLL, Teardown)

EUNIT_TEST(
    "SinkPauseL - test ",
    "CMccRtpDataSink",
    "SinkPauseL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSink_SinkPauseLL, Teardown)

EUNIT_TEST(
    "SinkStopL - test ",
    "CMccRtpDataSink",
    "SinkStopL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSink_SinkStopLL, Teardown)

EUNIT_TEST(
    "EmptyBufferL - test ",
    "CMccRtpDataSink",
    "EmptyBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSink_EmptyBufferL_1L, Teardown)

EUNIT_TEST(
    "EmptyBuffer2L - test ",
    "CMccRtpDataSink",
    "EmptyBuffer2L",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSink_EmptyBufferL_2L, Teardown)

EUNIT_TEST(
    "SendRTCPReceiverReport - test ",
    "CMccRtpDataSink",
    "SendRTCPReceiverReport",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSink_SendRTCPReceiverReportL, Teardown)

EUNIT_TEST(
    "SendRTCPSenderReport - test ",
    "CMccRtpDataSink",
    "SendRTCPSenderReport",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSink_SendRTCPSenderReportL, Teardown)

EUNIT_TEST(
    "SendRTCPDataL - test ",
    "CMccRtpDataSink",
    "SendRTCPDataL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSink_SendRTCPDataLL, Teardown)

EUNIT_TEST(
    "ConstructSinkL - test ",
    "CMccRtpDataSink",
    "ConstructSinkL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSink_ConstructSinkLL, Teardown)


// FUNCTIONS INHERITED FROM CMccRtpInterface

EUNIT_TEST(
    "PrepareL - test ",
    "CMccRtpInterface",
    "PrepareL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSink_PrepareLL, Teardown)

EUNIT_TEST(
    "CreateStreamL - test ",
    "CMccRtpInterface",
    "CreateStreamL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSink_CreateStreamLL, Teardown)

EUNIT_TEST(
    "RemoveStreamL - test ",
    "CMccRtpInterface",
    "RemoveStreamL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSink_RemoveStreamLL, Teardown)

EUNIT_TEST(
    "PlayStreamL - test ",
    "CMccRtpInterface",
    "PlayStreamL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSink_PlayStreamLL, Teardown)

EUNIT_TEST(
    "PauseStreamL - test ",
    "CMccRtpInterface",
    "PauseStreamL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSink_PauseStreamLL, Teardown)

EUNIT_TEST(
    "StopStreamL - test ",
    "CMccRtpInterface",
    "StopStreamL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSink_StopStreamLL, Teardown)

EUNIT_TEST(
    "StartInactivityTimerL - test ",
    "CMccRtpInterface",
    "StartInactivityTimerL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSink_StartInactivityTimerLL, Teardown)

EUNIT_TEST(
    "StopInactivityTimerL - test ",
    "CMccRtpInterface",
    "StopInactivityTimerL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSink_StopInactivityTimerLL, Teardown)

EUNIT_TEST(
    "SetSessionParamsL - test ",
    "CMccRtpInterface",
    "SetSessionParamsL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSink_SetSessionParamsLL, Teardown)

EUNIT_TEST(
    "SRTPMasterKeyStaleEvent1L - test ",
    "CMccRtpDataSink",
    "SRTPMasterKeyStaleEvent1L",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSink_SRTPMasterKeyStaleEvent1L, Teardown)

EUNIT_TEST(
    "SRTPMasterKeyStaleEvent2L - test ",
    "CMccRtpDataSink",
    "SRTPMasterKeyStaleEvent2L",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpDataSink_SRTPMasterKeyStaleEvent2L, Teardown)
    
EUNIT_END_TEST_TABLE

//  END OF FILE
