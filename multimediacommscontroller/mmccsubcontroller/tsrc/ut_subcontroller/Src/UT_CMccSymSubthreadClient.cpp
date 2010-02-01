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
#include "UT_CMccSymSubthreadClient.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <mmf/common/mmfbase.h>
#include <mmf/common/mmfcontrollerframeworkbase.h>
#include <ecom/ecom.h>

//  INTERNAL INCLUDES
#include "MccSymSubthreadClient.h"
#include "MmccEvents.h"
#include "mcccodecconfigurator.h"
#include "Mcctesteventhandler.h"
#include "MccRtpDataSink.h"
#include "MccRtpDataSource.h"
#include "mccresourcepool_stub.h"
#include "mccrtpmediaclock.h"
#include "Mccrtpmanager.h"
#include "mmcccryptocontext.h"
#include "mmccsecureinterface.h"
#include "mccunittestmacros.h"

// MACROS
#define MCC_TEST_GET_RTP_API_SOCKET_SIZE( a ) \
iClient->iRtpmanager->iRtpApi->CustomCommandSync( 4, KNullDesC8, KNullDesC8, a )

#define MCC_TEST_GET_RTP_API_SECURE( a ) \
iClient->iRtpmanager->iRtpApi->CustomCommandSync( 3, KNullDesC8, KNullDesC8, a )

// CONSTANTS
const TUint KUT_CMccSymSubthreadClientDataSourceType( 42 );
const TUint KUT_CMccSymSubthreadClientDataSinkType( 43 );
const TInt KAmrNbBitrate122 = 12200;
const TInt KAmrNbBitrate475 = 4750;

const TUid KRtpDataSource = { KImplUidRtpDataSource }; 
const TUid KRtpDataSink = { KImplUidRtpDataSink };

// CONSTRUCTION
UT_CMccSymSubthreadClient* UT_CMccSymSubthreadClient::NewL()
    {
    UT_CMccSymSubthreadClient* self = UT_CMccSymSubthreadClient::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccSymSubthreadClient* UT_CMccSymSubthreadClient::NewLC()
    {
    UT_CMccSymSubthreadClient* self = new( ELeave ) UT_CMccSymSubthreadClient();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccSymSubthreadClient::~UT_CMccSymSubthreadClient()
    {
    }

// Default constructor
UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient() :
    iClient( NULL ),
  //  iSourceStub( NULL ),
  //  iSink( NULL ),
    iStreamId( 42 )
    {
    }

// Second phase construct
void UT_CMccSymSubthreadClient::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_CMccSymSubthreadClient::SetupL()
    {
    iResources = CMccResourcePoolStub::NewL();
    
    TUid dummyUid( TUid::Uid( 42 ) ); 
    TBuf8<5> dummyBuf( _L8( "foo" ) );

    iHandler = CMccTestEventHandler::NewL();
  
  
    iSourceStub = static_cast< CMccRtpDataSource* >( MDataSource::NewSourceL( KRtpDataSource, KNullDesC8 ) );
    iSinkStub = static_cast< CMccRtpDataSink* >(MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 ));

    iClient = CMccSymSubthreadClient::NewL( iHandler, iResources, KMccLinkGeneral, iMccSessionId );
    }

void UT_CMccSymSubthreadClient::Teardown()
    {
    if ( iClient )
        {
        delete iClient;
        }
    if ( iSinkStub )
        {
        delete iSinkStub;
        }   
    if ( iSourceStub )
        {
        delete iSourceStub;
        }
  
    if (iHandler)
    	{
    	delete iHandler;
    	}
    delete iResources;
    REComSession::FinalClose();
    }

void UT_CMccSymSubthreadClient::Setup2L()
    {
    iResources = CMccResourcePoolStub::NewL();
    
    TUid dummyUid( TUid::Uid( 42 ) ); 
    TBuf8<5> dummyBuf( _L8( "foo" ) );
    
    iHandler = CMccTestEventHandler::NewL();
   
    TPckgBuf<TInt> params( 30000 );
    iSourceStub = MDataSource::NewSourceL( KRtpDataSource, params );
    iSinkStub = MDataSink::NewSinkL( KUidMmfAudioOutput, KNullDesC8 );

    iClient = CMccSymSubthreadClient::NewL( iHandler, iResources, KMccLinkGeneral, iMccSessionId );
    
    iRtpMediaClock = CMccRtpMediaClock::NewL();
    
    // Create session
    TUint port( 5000 );
    TUint iap( -1 );
    TBool enableRTCP( ETrue );
    TInt ipTos( 0 );
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->CreateRtpSessionL( port, enableRTCP, ipTos, *iRtpMediaClock ) );

    // Create stream, add sink & source
    TUint8 payloadType( 96 );
    TFourCC fourCC( KMccFourCCIdAMRNB );
    TMMFPrioritySettings prioritySettings;
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->OpenL( payloadType,
                                    fourCC,
                                    iSourceStub, 
                                    iSinkStub,
                                    iStreamId,
                                    prioritySettings ) );
    
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
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->SetCodecInformationL( iStreamId, cInfo, buf ) );
    }

void UT_CMccSymSubthreadClient::Teardown2()
    {
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->CloseL() );
    

    if ( iClient )
        {
        delete iClient;
        }
 
    if ( iSinkStub )
        {
        delete iSinkStub;
        iSinkStub = NULL;
        }
  
    if ( iSourceStub )
        {
        delete iSourceStub;
        iSourceStub = NULL;
        }

    if (iHandler)
    	{
    	delete iHandler;
    	}
    delete iRtpMediaClock;	
    delete iResources;
  
    REComSession::FinalClose();
    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_HandleEventL()
    {
    TMccInternalEvent internalEvent;
    
    iClient->SendEventToClient( internalEvent );
    
    TMccEvent event;
    iClient->DoMccEvent( event, internalEvent );
    EUNIT_ASSERT( event.iEventCategory == KMccEventCategoryStream );
    EUNIT_ASSERT( event.iEventType == KMccStreamMMFEvent );

    // TMccEvent missing from TMccInternalEvent
    internalEvent.iEventType = KMccRtpSinkUid;
    internalEvent.iInternalEventType = EMccInternalRtpSinkError;
    
    iClient->SendEventToClient( internalEvent );
    
    // TMccEvent included in TMccInternalEvent
    TMccEvent event2;
    event2.iEventCategory = KMccEventCategoryStream;
    event2.iEventType = KMccRtcpReceived;
    event2.iEventData.Copy( _L8("foofoo") );
    internalEvent.iMccEvent = &event2;
    
    iClient->SendEventToClient( internalEvent );
   
    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_DoMccEvent()
    {
    TMccEvent mccevent;
    TMccInternalEvent internalEvent;
    internalEvent.iErrorCode = KErrGeneral;
    
    iClient->DoMccEvent( mccevent, internalEvent );
    
    EUNIT_ASSERT( mccevent.iEventCategory == KMccEventCategoryStream );
    EUNIT_ASSERT( mccevent.iEventType == KMccStreamMMFEvent );
    EUNIT_ASSERT( mccevent.iErrorCode == KErrGeneral );
    EUNIT_ASSERT( mccevent.iLinkId == iClient->iLinkId );

    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_SendEventToClientL()
    {
    TMccInternalEvent internalEvent;
    
    internalEvent.iEventType = KMccAmrFormatterUid;
    internalEvent.iInternalEventType = EMccInternalAmrEventCmr;
   
    EUNIT_ASSERT( iClient->SendEventToClient( internalEvent ) == KErrNone );

    TMccEvent event;
    event.iEventType = KMccRtcpReceived;
    internalEvent.iMccEvent = &event;
    TMccAmrEventData eventdata;
    eventdata.iModeRequestBitrate = KAmrNbBitrate475;
    
    TMccAmrEventDataPackage eventdataPkg(eventdata);
    event.iEventData = eventdataPkg;
   
    EUNIT_ASSERT( iClient->SendEventToClient( internalEvent ) == KErrNone );
    
    TMccRtcpEventData eventData;
    eventData.iRtcpPacketType = KRtcpByePacket;
    
    TUint port( 5000 );
    TUint iap( -1 );
    TBool enableRTCP( ETrue );
    TInt ipTos( 0 );
    TUint8 payloadType( 96 );
    TFourCC fourCC( KMccFourCCIdAMRNB );
    TMMFPrioritySettings prioritySettings;

    // Create a stream so that there is something to close
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->CreateRtpSessionL( port, enableRTCP, ipTos, *iRtpMediaClock ) );

    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->OpenL( payloadType,
                                    fourCC,
                                    iSourceStub, 
                                    iSinkStub,
                                    iStreamId,
                                    prioritySettings ) );
                                 
    EUNIT_ASSERT( iClient->SendEventToClient( internalEvent ) == KErrNone );
    
    internalEvent.iEventType = KMccAmrFormatterUid;
    
    internalEvent.iInternalEventType = EMccInternalAmrEventCmr;
    
    EUNIT_ASSERT( iClient->SendEventToClient( internalEvent ) == KErrNone );
 
    eventdata.iModeRequestBitrate = KAmrNbBitrate475;
    
    eventdataPkg = eventdata;
    event.iEventData = eventdataPkg;
        
    EUNIT_ASSERT( iClient->SendEventToClient( internalEvent ) == KErrNone );

    eventdata.iModeRequestBitrate = 0;
    eventdataPkg = eventdata;
    event.iEventData = eventdataPkg;
    
    EUNIT_ASSERT( iClient->SendEventToClient( internalEvent ) == KErrNone );
   
    eventData.iRtcpPacketType = KRtcpByePacket;
    eventdataPkg = eventdata;
    event.iEventData = eventdataPkg;
    
    EUNIT_ASSERT( iClient->SendEventToClient( internalEvent ) == KErrNone );


    internalEvent.iEventType = KUidMediaTypeAudio; 
    EUNIT_ASSERT( iClient->SendEventToClient( internalEvent ) == KErrNone );

    internalEvent.iEventType = KUidMediaTypeVideo;
    EUNIT_ASSERT( iClient->SendEventToClient( internalEvent ) == KErrNone );

    internalEvent.iEventType = KMccFileSinkUid;
    EUNIT_ASSERT( iClient->SendEventToClient( internalEvent ) == KErrNone );

    internalEvent.iEventType = KMccFileSourceUid;
    EUNIT_ASSERT( iClient->SendEventToClient( internalEvent ) == KErrNone );

    internalEvent.iEventType = KMccRtpSourceUid;
    EUNIT_ASSERT( iClient->SendEventToClient( internalEvent ) == KErrNone );

    internalEvent.iEventType = KMccRtpSinkUid;
    EUNIT_ASSERT( iClient->SendEventToClient( internalEvent ) == KErrNone );

    internalEvent.iEventType = KMccVideoSourceUid;
    EUNIT_ASSERT( iClient->SendEventToClient( internalEvent ) == KErrNone );

    internalEvent.iEventType = KMccVideoSinkUid;
    EUNIT_ASSERT( iClient->SendEventToClient( internalEvent ) == KErrNone );

    internalEvent.iEventType = KMccJitterBufferUid;
    EUNIT_ASSERT( iClient->SendEventToClient( internalEvent ) == KErrNone );

    internalEvent.iEventType = KMccMultiplexerUid;
    EUNIT_ASSERT( iClient->SendEventToClient( internalEvent ) == KErrNone );

    internalEvent.iEventType = KMccDtmfFormatterUid;
    EUNIT_ASSERT( iClient->SendEventToClient( internalEvent ) == KErrNone );

    internalEvent.iEventType = KMccAnySourceUid;
    EUNIT_ASSERT( iClient->SendEventToClient( internalEvent ) == KErrNone );
    
    internalEvent.iEventType = KMccAnySinkUid;
    EUNIT_ASSERT( iClient->SendEventToClient( internalEvent ) == KErrNone );
    }


void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_OpenLL()
    {
    TUint port( 5000 );
    TUint iap( -1 );
    TBool enableRTCP( ETrue );
    TInt ipTos( 0 );
    TUint8 payloadType( 96 );
    TFourCC fourCC( KMccFourCCIdAMRNB );
    TMMFPrioritySettings prioritySettings;

    // No session    
    EUNIT_ASSERT_LEAVE( iClient->OpenL( payloadType,
                                    fourCC,
                                    iSourceStub, 
                                    iSinkStub,
                                    iStreamId,
                                    prioritySettings ) );

    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->CreateRtpSessionL( port, enableRTCP, ipTos, *iRtpMediaClock ) );

    // KErrAlreadyExists
    EUNIT_ASSERT_LEAVE( iClient->CreateRtpSessionL( port, enableRTCP, ipTos, *iRtpMediaClock ) );

    // NULL parameter    
    EUNIT_ASSERT_LEAVE( iClient->OpenL( payloadType,
                                    fourCC,
                                    iSourceStub, 
                                    NULL,
                                    iStreamId,
                                    prioritySettings ) );

    // NULL parameter    
    EUNIT_ASSERT_LEAVE( iClient->OpenL( payloadType,
                                    fourCC,
                                    NULL, 
                                    iSinkStub,
                                    iStreamId,
                                    prioritySettings ) );
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->OpenL( payloadType,
                                    fourCC,
                                    iSourceStub, 
                                    iSinkStub,
                                    iStreamId,
                                    prioritySettings ) );
    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_SetBalanceLL()
    {
    TInt left( 50 );
    TInt right( 50 );
    
    // No stream
    EUNIT_ASSERT_LEAVE( iClient->SetBalanceL( iStreamId, left, right ) );

    //Teardown();
    //MCC_EUNIT_ASSERT_NO_LEAVE( Setup2L() );

    //MCC_EUNIT_ASSERT_NO_LEAVE( iClient->SetBalanceL( iStreamId, left, right ) );
    
    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_GetBalanceLL()
    {
    TInt left( 0 );
    TInt right( 0 );
    
    // No stream
    EUNIT_ASSERT_LEAVE( iClient->GetBalanceL( iStreamId, left, right ) );

    //Teardown();
    //MCC_EUNIT_ASSERT_NO_LEAVE( Setup2L() );

    //MCC_EUNIT_ASSERT_NO_LEAVE( iClient->GetBalanceL( iStreamId, left, right ) );
    
    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_PrepareLL()
    {
    // KErrNotReady
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iClient->PrepareL( iStreamId, 0 ), KErrNotFound );
    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_PrepareL2L()
    {                                       
    MCC_EUNIT_ASSERT_NO_LEAVE(iClient->PrepareL( iStreamId, 0 ) );
    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_PlayLL()
    {
    // KErrNotReady
    EUNIT_ASSERT_LEAVE( iClient->PlayL( iStreamId, 0, EFalse, ETrue ) );
    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_PlayL2L()
    {
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->PrepareL( iStreamId, 0) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->PlayL( iStreamId, 0, EFalse, ETrue ) );
    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_StopLL()
    {
    // KErrNotReady
    EUNIT_ASSERT_LEAVE( iClient->StopL( iStreamId, 0 ) );
 
    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_StopL2L()
    {
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->PrepareL( iStreamId, 0 ) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->PlayL( iStreamId, 0, EFalse, EFalse ) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->StopL( iStreamId, 0 ) );
    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_PauseLL()
    {
    // KErrNotReady
    EUNIT_ASSERT_LEAVE( iClient->PauseL( iStreamId, 0, ETrue ) );
    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_ResumeLL()
    {
    // KErrNotReady
    EUNIT_ASSERT_LEAVE( iClient->ResumeL( iStreamId, 0, ETrue ) );
    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_ResumeL2L()
    {/*
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->PrepareL( iStreamId, 0 ) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->PlayL( iStreamId, 0, EFalse, EFalse ) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->PauseL( iStreamId, 0, EFalse ) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->ResumeL( iStreamId, 0, ETrue ) );
    */
    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_CloseLL()
    {
    // KErrNotReady
    EUNIT_ASSERT_LEAVE( iClient->CloseL( iStreamId ) );
    
    TUint port( 5000 );
    TUint iap( -1 );
    TBool enableRTCP( ETrue );
    TInt ipTos( 0 );
    TUint8 payloadType( 96 );
    TFourCC fourCC( KMccFourCCIdAMRNB );
    TMMFPrioritySettings prioritySettings;

    // Create a stream so that there is something to close
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->CreateRtpSessionL( port, enableRTCP, ipTos, *iRtpMediaClock ) );

    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->OpenL( payloadType,
                                    fourCC,
                                    iSourceStub, 
                                    iSinkStub,
                                    iStreamId,
                                    prioritySettings ) );
                                    
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->CloseL(iStreamId) );

    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->OpenL( payloadType,
                                    fourCC,
                                    iSourceStub, 
                                    iSinkStub,
                                    iStreamId,
                                    prioritySettings ) );
                                    
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->UnuseL( iStreamId ) );
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->CloseL(iStreamId) );
   
    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_CloseL_1L()
    {
    // KErrNotReady
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->CloseL() );
    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_SetVolumeLL()
    {
    TInt volume( 5 );
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->SetVolumeL( volume ) );
    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_SetGainLL()
    {
    TInt gain( 5 );
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->SetGainL( gain ) );
    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_MaxVolumeLL()
    {
    TInt volume( 0 );
    EUNIT_ASSERT_LEAVE( iClient->MaxVolumeL( iStreamId ) );
    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_MaxGainLL()
    {
    TInt gain( 0 );
    EUNIT_ASSERT_LEAVE( iClient->MaxGainL( iStreamId ) );
    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_GetVolumeLL()
    {
    TInt volume( 0 );
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( volume = iClient->GetVolumeL( iStreamId ), KErrNotFound  );
    
    TUint port( 5000 );
    TUint iap( -1 );
    TBool enableRTCP( ETrue );
    TInt ipTos( 0 );
    TUint8 payloadType( 96 );
    TFourCC fourCC( KMccFourCCIdAMRNB );
    TMMFPrioritySettings prioritySettings;

    // Create a stream so that there is something to close
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->CreateRtpSessionL( port, enableRTCP, ipTos, *iRtpMediaClock ) );

    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->OpenL( payloadType,
                                    fourCC,
                                    iSourceStub, 
                                    iSinkStub,
                                    iStreamId,
                                    prioritySettings ) );
    
    MCC_EUNIT_ASSERT_NO_LEAVE( volume = iClient->GetVolumeL( iStreamId ) );
    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_GetGainLL()
    {
    TInt gain( 0 );
    EUNIT_ASSERT_LEAVE( gain = iClient->GetGainL( iStreamId ) );
    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_SendMediaSignalLL()
    {
    TMccEvent event;

    // KErrNotReady
    EUNIT_ASSERT_LEAVE( iClient->SendMediaSignalL( event ) );
    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_SendMediaSignalL2L()
    {
    TMccEvent event;
    CreateRtcpEvent( event, KMccRtcpControl );

    // KErrNotSupported
    EUNIT_ASSERT_LEAVE( iClient->SendMediaSignalL( event ) );
    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_SendRTCPReceiverReportLL()
    {
    // KErrNotReady
    EUNIT_ASSERT_LEAVE( iClient->SendRTCPReceiverReportL( iStreamId ) );
    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_SendRTCPSenderReportLL()
    {
    // KErrNotReady
    EUNIT_ASSERT_LEAVE( iClient->SendRTCPReceiverReportL( iStreamId ) );
    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_SendRTCPDataLL()
    {
    TBuf8<5> buf;
    _LIT8( KData, "dummy" );
    buf.Format( KData );
    
    EUNIT_ASSERT_LEAVE( iClient->SendRTCPDataL( iStreamId, buf ) );
    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_StartInactivityTimerLL()
    {
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iClient->StartInactivityTimerL( 1, 5000 ), KErrNotFound );

    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->StartInactivityTimerL( iStreamId, 5000 ) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->StopInactivityTimerL( iStreamId ) );
    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_StopInactivityTimerLL()
    {
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iClient->StopInactivityTimerL( 1  ), KErrNotFound );

    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->StartInactivityTimerL( iStreamId, 5000 ) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->StopInactivityTimerL( iStreamId ) );
    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_GetSupportedBitratesLL()
    {
    RArray<TUint> bitrates;
    
    // KErrNotSupported
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->GetSupportedBitratesL( iStreamId, bitrates ) );
    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_GetSSRCLL()
    {
    TRtpSSRC ssrc( 0 );
    
    // KErrNotReady
    EUNIT_ASSERT_LEAVE( iClient->GetSSRCL( iStreamId, ssrc ) );
    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_SetUplinkIdL()
    {

    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_SetDownlinkIdL()
    {

    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_GetUplinkIdL()
    {

    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_GetDownlinkIdL()
    {

    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_DownlinkCreatedL()
    {

    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_UplinkCreatedL()
    {
 
    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_SetDownlinkCreatedL()
    {
    TUint port( 5000 );
    TUint iap( -1 );
    TBool enableRTCP( ETrue );
    TInt ipTos( 0 );
    TUint8 payloadType( 96 );
    TFourCC fourCC( KMccFourCCIdAMRNB );
    TMMFPrioritySettings prioritySettings;

    // Create a stream so that there is something to close
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->CreateRtpSessionL( port, enableRTCP, ipTos, *iRtpMediaClock ) );

    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->OpenL( payloadType,
                                    fourCC,
                                    iSourceStub, 
                                    iSinkStub,
                                    iStreamId,
                                    prioritySettings ) );

    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_SetUplinkCreatedL()
    {
    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_GetFmtpAttrLL()
    {
    TBuf8<10> buf;
    
    // KErrNotReady
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->GetFmtpAttrL( iStreamId, buf ) );
    }

void UT_CMccSymSubthreadClient::UT_CMccSymSubthreadClient_GetCodecLL()
    {
    TMccCodecInfo cInfo;
    
    // KErrNotReady
    EUNIT_ASSERT_LEAVE( iClient->CodecInformationL( iStreamId, cInfo ) );
    }

void UT_CMccSymSubthreadClient::UT_CMccSubThreadClient_UnuseL()
    {
   
    TUint32 streamId(3);
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iClient->UnuseL( streamId ), KErrNotFound );
    
    TUint port( 5000 );
    TUint iap( -1 );
    TBool enableRTCP( ETrue );
    TInt ipTos( 0 );
    TUint8 payloadType( 96 );
    TFourCC fourCC( KMccFourCCIdAMRNB );
    TMMFPrioritySettings prioritySettings;

    // Create a stream so that there is something to close
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->CreateRtpSessionL( port, enableRTCP, ipTos, *iRtpMediaClock ) );

    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->OpenL( payloadType,
                                    fourCC,
                                    iSourceStub, 
                                    iSinkStub,
                                    iStreamId,
                                    prioritySettings ) );
                                    
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->UnuseL( iStreamId ) );
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->CloseL() );
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->UnuseL( iStreamId ) );

    }

void UT_CMccSymSubthreadClient::UT_CMccSubThreadClient_GetLocalIpAddressesL()
    {
    TMccCreateLink clientData;
    iClient->iRtpmanager->iRtpSessionId = 5;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iClient->GetLocalIpAddressesL( clientData ), KErrGeneral );
    }

void UT_CMccSymSubthreadClient::UT_CMccSubThreadClient_BindContextIntoStreamL()
    {
    TUint port( 5000 );
    TUint iap( -1 );
    TBool enableRTCP( ETrue );
    TInt ipTos( 0 );
    TUint8 payloadType( 96 );
    TFourCC fourCC( KMccFourCCIdAMRNB );
    TMMFPrioritySettings prioritySettings;

    // Create a stream so that there is something to close
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->CreateRtpSessionL( port, enableRTCP, ipTos, *iRtpMediaClock ) );

    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->OpenL( payloadType,
                                    fourCC,
                                    iSourceStub, 
                                    iSinkStub,
                                    iStreamId,
                                    prioritySettings ) );
         
    
    TMccCryptoContext cryptoContext;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iClient->BindContextIntoStreamL( iStreamId, 0, cryptoContext ), KErrNotFound );
   
    }

void UT_CMccSymSubthreadClient::UT_CMccSubThreadClient_RemoveContextL()
    {
    TUint port( 5000 );
    TUint iap( -1 );
    TBool enableRTCP( ETrue );
    TInt ipTos( 0 );
    TUint8 payloadType( 96 );
    TFourCC fourCC( KMccFourCCIdAMRNB );
    TMMFPrioritySettings prioritySettings;

    // Create a stream so that there is something to close
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->CreateRtpSessionL( port, enableRTCP, ipTos, *iRtpMediaClock ) );

    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->OpenL( payloadType,
                                    fourCC,
                                    iSourceStub, 
                                    iSinkStub,
                                    iStreamId,
                                    prioritySettings ) );
       
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->RemoveContextL( iStreamId, 0) );   
    }

void UT_CMccSymSubthreadClient::UT_CMccSubThreadClient_SetGetParameterL()
    {
    TUint32 param = KMccRtpCName;
    TUint32 streamId = 0;
    TUint32 endpointId =0;
    TBuf8<20> buf;
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->SetParameterL( param, 0, 0, _L8("ser")) );
    param = 0;
    
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iClient->SetParameterL( param, 0, 0, _L8("ser") ), KErrNotSupported );
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->GetParameterL( param, streamId, endpointId, buf) );
    }

// BASE CLASS TEST METHODS

void UT_CMccSymSubthreadClient::UT_CMccSubThreadClientBase_GetCodecLL()
    {
    TMccCodecInfo cInfo;

    // KErrNotReady
    EUNIT_ASSERT_LEAVE(
        iClient->CMccSubThreadClientBase::CodecInformationL( iStreamId, cInfo ) );
    }

void UT_CMccSymSubthreadClient::UT_CMccSubThreadClientBase_GetCodecL2L()
    {
    TMccCodecInfo cInfo;
    
    MCC_EUNIT_ASSERT_NO_LEAVE(
        iClient->CMccSubThreadClientBase::CodecInformationL( iStreamId, cInfo ) );
    }

void UT_CMccSymSubthreadClient::UT_CMccSubThreadClientBase_SetPrioritySettingsLL()
    {
    TMMFPrioritySettings prioritySet;

    // KErrNotReady
    EUNIT_ASSERT_LEAVE( iClient->SetPrioritySettingsL( iStreamId, prioritySet ) );
    }

void UT_CMccSymSubthreadClient::UT_CMccSubThreadClientBase_SetLinkIdL()
    {
    iClient->SetLinkId( 42 );
    EUNIT_ASSERT_EQUALS( iClient->GetLinkId(), 42 );
    }

void UT_CMccSymSubthreadClient::UT_CMccSubThreadClientBase_GetLinkIdL()
    {
    iClient->SetLinkId( 42 );
    EUNIT_ASSERT_EQUALS( iClient->GetLinkId(), 42 );
    }

void UT_CMccSymSubthreadClient::UT_CMccSubThreadClientBase_SetRemoteAddressLL()
    {
    TInetAddr addr( INET_ADDR( 127,0,0,1 ) );
    
    // KErrNotReady
    EUNIT_ASSERT_LEAVE( iClient->SetRemoteAddressL( addr ) );
    
    delete iClient->iRtpmanager;
    iClient->iRtpmanager = NULL;
    
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iClient->SetRemoteAddressL( addr ),KErrArgument );
    }

void UT_CMccSymSubthreadClient::UT_CMccSubThreadClientBase_SetRemoteAddressL2L()
    {
    TInetAddr addr( INET_ADDR( 127,0,0,1 ) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->SetRemoteAddressL( addr ) );
    }

void UT_CMccSymSubthreadClient::UT_CMccSubThreadClientBase_CloseLL()
    {
    // Base class function
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iClient->CMccSubThreadClientBase::CloseL(), 
                                 KErrNotSupported );
    }

void UT_CMccSymSubthreadClient::UT_CMccSubThreadClientBase_SendMediaSignalLL()
    {
    TMccEvent event;
    CreateRtcpEvent( event );
    
    // Base class function
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(
        iClient->CMccSubThreadClientBase::SendMediaSignalL( event ), 
            KErrNotSupported );
    }

void UT_CMccSymSubthreadClient::UT_CMccSubThreadClientBase_SendRTCPReceiverReportLL()
    {
    // Base class function
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(
        iClient->CMccSubThreadClientBase::SendRTCPReceiverReportL( iStreamId ), 
            KErrNotSupported );
    }

void UT_CMccSymSubthreadClient::UT_CMccSubThreadClientBase_SendRTCPSenderReportLL()
    {
    // Base class function
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(
        iClient->CMccSubThreadClientBase::SendRTCPSenderReportL( iStreamId ), 
            KErrNotSupported );
    }

void UT_CMccSymSubthreadClient::UT_CMccSubThreadClientBase_SendRTCPDataLL()
    {
    _LIT8( KText, "dummy" );
    TBuf8<5> buf( KText() );
    // Base class function
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(
        iClient->CMccSubThreadClientBase::SendRTCPDataL( iStreamId, buf ), 
            KErrNotSupported );
    }

void UT_CMccSymSubthreadClient::UT_CMccSubThreadClientBase_StartInactivityTimerLL()
    {
    // Base class function
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(
        iClient->CMccSubThreadClientBase::StartInactivityTimerL( iStreamId, 5000 ), 
            KErrNotSupported );
    }

void UT_CMccSymSubthreadClient::UT_CMccSubThreadClientBase_StopInactivityTimerLL()
    {
    // Base class function
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(
        iClient->CMccSubThreadClientBase::StopInactivityTimerL( iStreamId ), 
            KErrNotSupported );
    }

void UT_CMccSymSubthreadClient::UT_CMccSubThreadClientBase_GetSupportedBitratesLL()
    {
    RArray<TUint> bitrates;
    
    // KErrNotSupported
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(
        iClient->CMccSubThreadClientBase::GetSupportedBitratesL( iStreamId, bitrates ), 
            KErrNotSupported );
    }

void UT_CMccSymSubthreadClient::UT_CMccSubThreadClientBase_GetSSRCLL()
    {
    TRtpSSRC ssrc;
    
    // Base class function
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(
        iClient->CMccSubThreadClientBase::GetSSRCL( iStreamId, ssrc ), 
            KErrNotSupported );
    }

void UT_CMccSymSubthreadClient::UT_CMccSubThreadClientBase_GetFmtpAttrLL()
    {
    HBufC8* buf = HBufC8::NewLC( 5 );
    TPtr8 ptr = buf->Des();
    
    // Base class function
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(
        iClient->CMccSubThreadClientBase::GetFmtpAttrL( iStreamId, ptr ), 
            KErrNotSupported );

    CleanupStack::PopAndDestroy( buf );
    }

void UT_CMccSymSubthreadClient::UT_CMccSubThreadClientBase_SetUplinkIdL()
    {
    }

void UT_CMccSymSubthreadClient::UT_CMccSubThreadClientBase_SetDownlinkIdL()
    {
    }

void UT_CMccSymSubthreadClient::UT_CMccSubThreadClientBase_GetUplinkIdL()
    {
    }

void UT_CMccSymSubthreadClient::UT_CMccSubThreadClientBase_GetDownlinkIdL()
    {
    }

void UT_CMccSymSubthreadClient::UT_CMccSubThreadClientBase_DownlinkCreatedL()
    {
    }

void UT_CMccSymSubthreadClient::UT_CMccSubThreadClientBase_UplinkCreatedL()
    {
    }

void UT_CMccSymSubthreadClient::UT_CMccSubThreadClientBase_SetDownlinkCreatedL()
    {
    }

void UT_CMccSymSubthreadClient::UT_CMccSubThreadClientBase_SetUplinkCreatedL()
    {
    }

void UT_CMccSymSubthreadClient::UT_CMccSubThreadClientBase_RestartEventMonitorL()
    {
    }

void UT_CMccSymSubthreadClient::UT_CMccSubThreadClientBase_IsDtmfL()
    {
    EUNIT_ASSERT( !( iClient->CMccSubThreadClientBase::IsDtmf() ) );
    }

void UT_CMccSymSubthreadClient::UT_CMccSubThreadClientBase_DataSink()
    {  
    TUint32 streamId(1);
    
    MDataSink* sink = iClient->DataSink( streamId );
    EUNIT_ASSERT( sink == NULL );
    
    TUint port( 5000 );
    TUint iap( -1 );
    TBool enableRTCP( ETrue );
    TInt ipTos( 0 );
    TUint8 payloadType( 96 );
    TFourCC fourCC( KMccFourCCIdAMRNB );
    TMMFPrioritySettings prioritySettings;

    // Create a stream so that there is something to close
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->CreateRtpSessionL( port, enableRTCP, ipTos, *iRtpMediaClock ) );

    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->OpenL( payloadType,
                                    fourCC,
                                    iSourceStub, 
                                    iSinkStub,
                                    iStreamId,
                                    prioritySettings ) );
                                    
    sink = iClient->DataSink( iStreamId );
    EUNIT_ASSERT( sink != NULL );     
    }

void UT_CMccSymSubthreadClient::UT_CMccSubThreadClientBase_DataSource()
    {
    TUint32 streamId(1);
    
    MDataSource* source = iClient->DataSource( streamId );
    EUNIT_ASSERT( source == NULL );
    
    TUint port( 5000 );
    TUint iap( -1 );
    TBool enableRTCP( ETrue );
    TInt ipTos( 0 );
    TUint8 payloadType( 96 );
    TFourCC fourCC( KMccFourCCIdAMRNB );
    TMMFPrioritySettings prioritySettings;

    // Create a stream so that there is something to close
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->CreateRtpSessionL( port, enableRTCP, ipTos, *iRtpMediaClock ) );

    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->OpenL( payloadType,
                                    fourCC,
                                    iSourceStub, 
                                    iSinkStub,
                                    iStreamId,
                                    prioritySettings ) );
                                    
    source = iClient->DataSource( iStreamId );
    EUNIT_ASSERT( source != NULL ); 
    }
    
void UT_CMccSymSubthreadClient::UT_CMccSubThreadClientBase_CreateEventReceiverLL()
    {
    }

void UT_CMccSymSubthreadClient::UT_CMccSubThreadClientBase_CancelEventReceiverL()
    {
    }
    
void UT_CMccSymSubthreadClient::CreateRtcpEvent( TMccEvent& aEvent, TMccEventType aRtcpEventType )
    {
    TMccRtcpEventData rtcpData;
    rtcpData.iRtcpPacketData.Copy( _L8("foofoo") );
    rtcpData.iRtcpPacketType = KRtcpAnyPacket;
    rtcpData.iSsrc = 2;
    
    TMccRtcpEventDataPackage rtcpDataPackage( rtcpData );
    
    //Create a media signal and send it 
    TMccEvent event( 1, 1, 1, 1, KMccEventCategoryRtcp, aRtcpEventType,
                         KErrNone, rtcpDataPackage );
    aEvent = event;
    }
    
void UT_CMccSymSubthreadClient::UT_CMccSubThreadClientBase_CreateRtpSessionL()
    {
    // Create normal session
    TUint port( 5000 );
    TUint iap( -1 );
    TBool enableRTCP( ETrue );
    TInt ipTos( 0 );
    TUint8 payloadType( 96 );
    TFourCC fourCC( KMccFourCCIdAMRNB );
    TMMFPrioritySettings prioritySettings;
    TBuf8<3> dummy;

    // Create a stream so that there is something to close
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->CreateRtpSessionL( port, enableRTCP, ipTos, *iRtpMediaClock ) );
    TInt socketSize = MCC_TEST_GET_RTP_API_SOCKET_SIZE( dummy );
    TBool secure = MCC_TEST_GET_RTP_API_SECURE( dummy );
    EUNIT_ASSERT( !secure );
    
    // Create secure video session (increased socket size)
    delete iClient;
    iClient = NULL;
    iClient = CMccSymSubthreadClient::NewL( iHandler, iResources, KMccLinkSecureVideo, iMccSessionId );
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->CreateRtpSessionL( port, enableRTCP, ipTos, *iRtpMediaClock ) );
    TInt socketSizeVideo = MCC_TEST_GET_RTP_API_SOCKET_SIZE( dummy );
    EUNIT_ASSERT( socketSizeVideo > socketSize );
    TBool secure2 = MCC_TEST_GET_RTP_API_SECURE( dummy );
    EUNIT_ASSERT( secure2 );
    }
    

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccSymSubthreadClient,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "HandleEvent - test ",
    "CMccSymSubthreadClient",
    "HandleEvent",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSubthreadClient_HandleEventL, Teardown)

EUNIT_TEST(
    "DoMccEvent - test ",
    "CMccSymSubthreadClient",
    "DoMccEvent",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSubthreadClient_DoMccEvent, Teardown)

EUNIT_TEST(
    "SendEventToClientL - test ",
    "CMccSymSubthreadClient",
    "SendEventToClientL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSubthreadClient_SendEventToClientL, Teardown)

EUNIT_TEST(
    "OpenL - test ",
    "CMccSymSubthreadClient",
    "OpenL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSubthreadClient_OpenLL, Teardown)

EUNIT_TEST(
    "SetBalanceL - test ",
    "CMccSymSubthreadClient",
    "SetBalanceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSubthreadClient_SetBalanceLL, Teardown)

EUNIT_TEST(
    "GetBalanceL - test ",
    "CMccSymSubthreadClient",
    "GetBalanceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSubthreadClient_GetBalanceLL, Teardown)

EUNIT_TEST(
    "PrepareL - test ",
    "CMccSymSubthreadClient",
    "PrepareL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSubthreadClient_PrepareLL, Teardown)

EUNIT_TEST(
    "PrepareL2 - test ",
    "CMccSymSubthreadClient",
    "PrepareL2",
    "FUNCTIONALITY",
    Setup2L, UT_CMccSymSubthreadClient_PrepareL2L, Teardown2)
    
EUNIT_TEST(
    "PlayL - test ",
    "CMccSymSubthreadClient",
    "PlayL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSubthreadClient_PlayLL, Teardown)


EUNIT_TEST(
    "PlayL - test 2",
    "CMccSymSubthreadClient",
    "PlayL 2",
    "FUNCTIONALITY",
    Setup2L, UT_CMccSymSubthreadClient_PlayL2L, Teardown2)

EUNIT_TEST(
    "StopL - test ",
    "CMccSymSubthreadClient",
    "StopL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSubthreadClient_StopLL, Teardown)


EUNIT_TEST(
    "StopL - test 2",
    "CMccSymSubthreadClient",
    "StopL 2",
    "FUNCTIONALITY",
    Setup2L, UT_CMccSymSubthreadClient_StopL2L, Teardown2)


EUNIT_TEST(
    "PauseL - test ",
    "CMccSymSubthreadClient",
    "PauseL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSubthreadClient_PauseLL, Teardown)

EUNIT_TEST(
    "ResumeL - test ",
    "CMccSymSubthreadClient",
    "ResumeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSubthreadClient_ResumeLL, Teardown)

EUNIT_TEST(
    "ResumeL2 - test 2",
    "CMccSymSubthreadClient",
    "ResumeL2",
    "FUNCTIONALITY",
    Setup2L, UT_CMccSymSubthreadClient_ResumeL2L, Teardown2)


EUNIT_TEST(
    "CloseL - test ",
    "CMccSymSubthreadClient",
    "CloseL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSubthreadClient_CloseLL, Teardown)

EUNIT_TEST(
    "CloseL_1 - test ",
    "CMccSymSubthreadClient",
    "CloseL_1",
    "FUNCTIONALITY",
    Setup2L, UT_CMccSymSubthreadClient_CloseL_1L, Teardown2)

EUNIT_TEST(
    "SetVolumeL - test ",
    "CMccSymSubthreadClient",
    "SetVolumeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSubthreadClient_SetVolumeLL, Teardown)

EUNIT_TEST(
    "SetGainL - test ",
    "CMccSymSubthreadClient",
    "SetGainL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSubthreadClient_SetGainLL, Teardown)

EUNIT_TEST(
    "MaxVolumeL - test ",
    "CMccSymSubthreadClient",
    "MaxVolumeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSubthreadClient_MaxVolumeLL, Teardown)

EUNIT_TEST(
    "MaxGainL - test ",
    "CMccSymSubthreadClient",
    "MaxGainL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSubthreadClient_MaxGainLL, Teardown)

EUNIT_TEST(
    "GetVolumeL - test ",
    "CMccSymSubthreadClient",
    "GetVolumeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSubthreadClient_GetVolumeLL, Teardown)

EUNIT_TEST(
    "GetGainL - test ",
    "CMccSymSubthreadClient",
    "GetGainL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSubthreadClient_GetGainLL, Teardown)

EUNIT_TEST(
    "SendMediaSignalL - test ",
    "CMccSymSubthreadClient",
    "SendMediaSignalL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSubthreadClient_SendMediaSignalLL, Teardown)

EUNIT_TEST(
    "SendMediaSignalL - test 2",
    "CMccSymSubthreadClient",
    "SendMediaSignalL 2",
    "FUNCTIONALITY",
    Setup2L, UT_CMccSymSubthreadClient_SendMediaSignalL2L, Teardown2)

EUNIT_TEST(
    "SendRTCPReceiverReportL - test ",
    "CMccSymSubthreadClient",
    "SendRTCPReceiverReportL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSubthreadClient_SendRTCPReceiverReportLL, Teardown)

EUNIT_TEST(
    "SendRTCPSenderReportL - test ",
    "CMccSymSubthreadClient",
    "SendRTCPSenderReportL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSubthreadClient_SendRTCPSenderReportLL, Teardown)

EUNIT_TEST(
    "SendRTCPDataL - test ",
    "CMccSymSubthreadClient",
    "SendRTCPDataL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSubthreadClient_SendRTCPDataLL, Teardown)

EUNIT_TEST(
    "StartInactivityTimerL - test ",
    "CMccSymSubthreadClient",
    "StartInactivityTimerL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccSymSubthreadClient_StartInactivityTimerLL, Teardown2)

EUNIT_TEST(
    "StopInactivityTimerL - test ",
    "CMccSymSubthreadClient",
    "StopInactivityTimerL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccSymSubthreadClient_StopInactivityTimerLL, Teardown2)
EUNIT_TEST(
    "GetSupportedBitratesL - test ",
    "CMccSymSubthreadClient",
    "GetSupportedBitratesL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSubthreadClient_GetSupportedBitratesLL, Teardown)

EUNIT_TEST(
    "GetSSRCL - test ",
    "CMccSymSubthreadClient",
    "GetSSRCL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSubthreadClient_GetSSRCLL, Teardown)


EUNIT_TEST(
    "SetUplinkId - test ",
    "CMccSymSubthreadClient",
    "SetUplinkId",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSubthreadClient_SetUplinkIdL, Teardown)

EUNIT_TEST(
    "SetDownlinkId - test ",
    "CMccSymSubthreadClient",
    "SetDownlinkId",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSubthreadClient_SetDownlinkIdL, Teardown)

EUNIT_TEST(
    "GetUplinkId - test ",
    "CMccSymSubthreadClient",
    "GetUplinkId",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSubthreadClient_GetUplinkIdL, Teardown)

EUNIT_TEST(
    "GetDownlinkId - test ",
    "CMccSymSubthreadClient",
    "GetDownlinkId",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSubthreadClient_GetDownlinkIdL, Teardown)


EUNIT_TEST(
    "DownlinkCreated - test ",
    "CMccSymSubthreadClient",
    "DownlinkCreated",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSubthreadClient_DownlinkCreatedL, Teardown)

EUNIT_TEST(
    "UplinkCreated - test ",
    "CMccSymSubthreadClient",
    "UplinkCreated",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSubthreadClient_UplinkCreatedL, Teardown)

EUNIT_TEST(
    "SetDownlinkCreated - test ",
    "CMccSymSubthreadClient",
    "SetDownlinkCreated",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSubthreadClient_SetDownlinkCreatedL, Teardown)

EUNIT_TEST(
    "SetUplinkCreated - test ",
    "CMccSymSubthreadClient",
    "SetUplinkCreated",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSubthreadClient_SetUplinkCreatedL, Teardown)

EUNIT_TEST(
    "GetFmtpAttrL - test ",
    "CMccSymSubthreadClient",
    "GetFmtpAttrL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSubthreadClient_GetFmtpAttrLL, Teardown)

EUNIT_TEST(
    "GetCodecL - test ",
    "CMccSymSubthreadClient",
    "GetCodecL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymSubthreadClient_GetCodecLL, Teardown)

EUNIT_TEST(
    "UnuseL - test ",
    "CMccSymSubthreadClient",
    "UnuseL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadClient_UnuseL, Teardown)

EUNIT_TEST(
    "GetLocalIpAddressesL - test ",
    "CMccSymSubthreadClient",
    "GetLocalIpAddressesL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadClient_GetLocalIpAddressesL, Teardown)

EUNIT_TEST(
    "BindContextIntoStreamL - test ",
    "CMccSymSubthreadClient",
    "BindContextIntoStreamL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadClient_BindContextIntoStreamL, Teardown)
    
EUNIT_TEST(
    "RemoveContextL - test ",
    "CMccSymSubthreadClient",
    "RemoveContextL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadClient_RemoveContextL, Teardown)

EUNIT_TEST(
    "SetGetParameterL - test ",
    "CMccSymSubthreadClient",
    "SetGetParameterL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadClient_SetGetParameterL, Teardown)



// BASE CLASS TEST DECLARATIONS

EUNIT_TEST(
    "GetCodecL - test ",
    "CMccSubThreadClientBase",
    "GetCodecL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadClientBase_GetCodecLL, Teardown)

EUNIT_TEST(
    "GetCodecL - test 2",
    "CMccSubThreadClientBase",
    "GetCodecL 2",
    "FUNCTIONALITY",
    Setup2L, UT_CMccSubThreadClientBase_GetCodecL2L, Teardown2)

EUNIT_TEST(
    "SetPrioritySettingsL - test ",
    "CMccSubThreadClientBase",
    "SetPrioritySettingsL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadClientBase_SetPrioritySettingsLL, Teardown)

EUNIT_TEST(
    "SetLinkId - test ",
    "CMccSubThreadClientBase",
    "SetLinkId",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadClientBase_SetLinkIdL, Teardown)

EUNIT_TEST(
    "GetLinkId - test ",
    "CMccSubThreadClientBase",
    "GetLinkId",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadClientBase_GetLinkIdL, Teardown)

EUNIT_TEST(
    "SetRemoteAddressL - test ",
    "CMccSubThreadClientBase",
    "SetRemoteAddressL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadClientBase_SetRemoteAddressLL, Teardown)

EUNIT_TEST(
    "SetRemoteAddressL - test 2",
    "CMccSubThreadClientBase",
    "SetRemoteAddressL 2",
    "FUNCTIONALITY",
    Setup2L, UT_CMccSubThreadClientBase_SetRemoteAddressL2L, Teardown2)

EUNIT_TEST(
    "CloseL - test ",
    "CMccSubThreadClientBase",
    "CloseL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadClientBase_CloseLL, Teardown)

EUNIT_TEST(
    "SendMediaSignalL - test ",
    "CMccSubThreadClientBase",
    "SendMediaSignalL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadClientBase_SendMediaSignalLL, Teardown)

EUNIT_TEST(
    "SendRTCPReceiverReportL - test ",
    "CMccSubThreadClientBase",
    "SendRTCPReceiverReportL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadClientBase_SendRTCPReceiverReportLL, Teardown)

EUNIT_TEST(
    "SendRTCPSenderReportL - test ",
    "CMccSubThreadClientBase",
    "SendRTCPSenderReportL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadClientBase_SendRTCPSenderReportLL, Teardown)

EUNIT_TEST(
    "SendRTCPDataL - test ",
    "CMccSubThreadClientBase",
    "SendRTCPDataL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadClientBase_SendRTCPDataLL, Teardown)

EUNIT_TEST(
    "StartInactivityTimerL - test ",
    "CMccSubThreadClientBase",
    "StartInactivityTimerL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadClientBase_StartInactivityTimerLL, Teardown)

EUNIT_TEST(
    "StopInactivityTimerL - test ",
    "CMccSubThreadClientBase",
    "StopInactivityTimerL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadClientBase_StopInactivityTimerLL, Teardown)

EUNIT_TEST(
    "GetSupportedBitratesL - test ",
    "CMccSubThreadClientBase",
    "GetSupportedBitratesL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadClientBase_GetSupportedBitratesLL, Teardown)

EUNIT_TEST(
    "GetSSRCL - test ",
    "CMccSubThreadClientBase",
    "GetSSRCL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadClientBase_GetSSRCLL, Teardown)

EUNIT_TEST(
    "GetFmtpAttrL - test ",
    "CMccSubThreadClientBase",
    "GetFmtpAttrL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadClientBase_GetFmtpAttrLL, Teardown)

EUNIT_TEST(
    "SetUplinkId - test ",
    "CMccSubThreadClientBase",
    "SetUplinkId",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadClientBase_SetUplinkIdL, Teardown)

EUNIT_TEST(
    "SetDownlinkId - test ",
    "CMccSubThreadClientBase",
    "SetDownlinkId",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadClientBase_SetDownlinkIdL, Teardown)

EUNIT_TEST(
    "GetUplinkId - test ",
    "CMccSubThreadClientBase",
    "GetUplinkId",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadClientBase_GetUplinkIdL, Teardown)

EUNIT_TEST(
    "GetDownlinkId - test ",
    "CMccSubThreadClientBase",
    "GetDownlinkId",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadClientBase_GetDownlinkIdL, Teardown)

EUNIT_TEST(
    "DownlinkCreated - test ",
    "CMccSubThreadClientBase",
    "DownlinkCreated",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadClientBase_DownlinkCreatedL, Teardown)

EUNIT_TEST(
    "UplinkCreated - test ",
    "CMccSubThreadClientBase",
    "UplinkCreated",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadClientBase_UplinkCreatedL, Teardown)

EUNIT_TEST(
    "SetDownlinkCreated - test ",
    "CMccSubThreadClientBase",
    "SetDownlinkCreated",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadClientBase_SetDownlinkCreatedL, Teardown)

EUNIT_TEST(
    "SetUplinkCreated - test ",
    "CMccSubThreadClientBase",
    "SetUplinkCreated",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadClientBase_SetUplinkCreatedL, Teardown)

EUNIT_TEST(
    "RestartEventMonitor - test ",
    "CMccSubThreadClientBase",
    "RestartEventMonitor",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadClientBase_RestartEventMonitorL, Teardown)

EUNIT_TEST(
    "IsDtmf - test ",
    "CMccSubThreadClientBase",
    "IsDtmf",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadClientBase_IsDtmfL, Teardown)

EUNIT_TEST(
    "CreateEventReceiverL - test ",
    "CMccSubThreadClientBase",
    "CreateEventReceiverL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadClientBase_CreateEventReceiverLL, Teardown)

EUNIT_TEST(
    "CancelEventReceiver - test ",
    "CMccSubThreadClientBase",
    "CancelEventReceiver",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadClientBase_CancelEventReceiverL, Teardown)

EUNIT_TEST(
    "IsDtmf - test ",
    "CMccSubThreadClientBase",
    "IsDtmf",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadClientBase_IsDtmfL, Teardown)

EUNIT_TEST(
    "DataSink - test ",
    "CMccSubThreadClientBase",
    "DataSink",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadClientBase_DataSink, Teardown)

EUNIT_TEST(
    "DataSource - test ",
    "CMccSubThreadClientBase",
    "DataSource",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadClientBase_DataSource, Teardown)

EUNIT_TEST(
    "CreateRtpSessionL - test ",
    "CMccSubThreadClientBase",
    "CreateRtpSessionL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadClientBase_CreateRtpSessionL, Teardown)


EUNIT_END_TEST_TABLE

//////////////////////////////////////////////////////////////////////////////
// UT_CMccSymSubthreadClientDataSource methods
//////////////////////////////////////////////////////////////////////////////

UT_CMccSymSubthreadClientDataSource* UT_CMccSymSubthreadClientDataSource::NewL()
    {
    UT_CMccSymSubthreadClientDataSource* self = new( ELeave ) UT_CMccSymSubthreadClientDataSource();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

UT_CMccSymSubthreadClientDataSource::UT_CMccSymSubthreadClientDataSource() :
    MDataSource( TUid::Uid( KUT_CMccSymSubthreadClientDataSourceType ) )
    {
    }

UT_CMccSymSubthreadClientDataSource::~UT_CMccSymSubthreadClientDataSource()
    {
    
    }

void UT_CMccSymSubthreadClientDataSource::ConstructL()
    {
    }

TFourCC UT_CMccSymSubthreadClientDataSource::SourceDataTypeCode( TMediaId /*aMediaId*/ )
    {
    
    return TFourCC();
    }
    
void UT_CMccSymSubthreadClientDataSource::FillBufferL( CMMFBuffer* /*aBuffer*/,
                                                       MDataSink* /*aConsumer*/,
                                                       TMediaId /*aMediaId*/ )
    {
    }
    
void UT_CMccSymSubthreadClientDataSource::BufferEmptiedL( CMMFBuffer* /*aBuffer*/ )
    {
    }
    
TBool UT_CMccSymSubthreadClientDataSource::CanCreateSourceBuffer()
    {
    return EFalse;
    }

CMMFBuffer* UT_CMccSymSubthreadClientDataSource::CreateSourceBufferL(
    TMediaId /*aMediaId*/, TBool& /*aReference*/ )
    {
    return NULL;
    }

void UT_CMccSymSubthreadClientDataSource::ConstructSourceL( const TDesC8& /*aInitData*/ )
    {
    }

TUid UT_CMccSymSubthreadClientDataSource::DataSourceType() const
    {
    return KUidMmfAudioInput;
    }

//////////////////////////////////////////////////////////////////////////////
// UT_CMccSymSubthreadClientDataSink methods
//////////////////////////////////////////////////////////////////////////////

UT_CMccSymSubthreadClientDataSink* UT_CMccSymSubthreadClientDataSink::NewL()
    {
    UT_CMccSymSubthreadClientDataSink* self = new( ELeave ) UT_CMccSymSubthreadClientDataSink();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

UT_CMccSymSubthreadClientDataSink::UT_CMccSymSubthreadClientDataSink()
    {
    }

UT_CMccSymSubthreadClientDataSink::~UT_CMccSymSubthreadClientDataSink()
    {
    if ( iDevSound )
        {
        delete iDevSound;
        }
    _LIT (KName1, "UT_CMccSymSubthreadClientDataSink::~UT_CMccSymSubthreadClientDataSink cells-1: %d");
    RDebug::Print(KName1, User::CountAllocCells());
    }

void UT_CMccSymSubthreadClientDataSink::ConstructL()
    {
    iDevSound = CMMFDevSound::NewL();  
    _LIT (KName1, "UT_CMccSymSubthreadClientDataSink::ConstructL cells-1: %d");
    RDebug::Print(KName1, User::CountAllocCells());
    }

TFourCC UT_CMccSymSubthreadClientDataSink::SinkDataTypeCode( TMediaId /*aMediaId*/ )
    {
    return TFourCC();
    }

void UT_CMccSymSubthreadClientDataSink::EmptyBufferL( CMMFBuffer* /*aBuffer*/,
                                                      MDataSource* /*aSupplier*/,
                                                      TMediaId /*aMediaId*/ )
    {
    
    }

void UT_CMccSymSubthreadClientDataSink::BufferFilledL( CMMFBuffer* /*aBuffer*/ )
    {
    
    }

TBool UT_CMccSymSubthreadClientDataSink::CanCreateSinkBuffer()
    {
    return EFalse;
    }

CMMFBuffer* UT_CMccSymSubthreadClientDataSink::CreateSinkBufferL(
    TMediaId /*aMediaId*/, TBool& /*aReference*/ )
    {
    return NULL;
    }

void UT_CMccSymSubthreadClientDataSink::ConstructSinkL( const TDesC8& /*aInitData*/ )
    {
    
    }

TUid UT_CMccSymSubthreadClientDataSink::DataSinkType() const
    {
  //  return TUid::Uid( 270562316 );
    return KUidMmfAudioOutput;
    }

CMMFDevSound& UT_CMccSymSubthreadClientDataSink::SoundDevice()
    {
    return *iDevSound;
    }
    
TInt UT_CMccSymSubthreadClientDataSink::BytesPlayed()
    {
    return 0;
    }

void UT_CMccSymSubthreadClientDataSink::HWEmptyBufferL( CMMFBuffer* /*aBuffer*/,
                                                        MDataSource* /*aSupplier*/ )
    {
    }

void UT_CMccSymSubthreadClientDataSink::SetSinkPrioritySettings(
    const TMMFPrioritySettings& /*aPrioritySettings*/ )
    {
    }
    
//  END OF FILE
