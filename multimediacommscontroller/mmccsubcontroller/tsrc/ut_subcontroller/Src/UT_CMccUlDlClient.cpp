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
#include "UT_CMccUlDlClient.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "MccUlDlClient.h"
#include "MccRtpDataSink.h"
#include "MccRtpDataSource.h"
#include "mccresourcepool_stub.h"
#include "mccuids.hrh"
#include "mccunittestmacros.h"
#include "mmcccryptocontext.h"
#include "mmccsecureinterface.h"
#include "UT_CMccSymSubthreadClient.h"

const TUid KRtpDataSource  =  { KImplUidRtpDataSource   }; 
const TUid KRtpDataSink    =  { KImplUidRtpDataSink     };



// CONSTRUCTION
UT_CMccUlDlClient* UT_CMccUlDlClient::NewL()
    {
    UT_CMccUlDlClient* self = UT_CMccUlDlClient::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccUlDlClient* UT_CMccUlDlClient::NewLC()
    {
    UT_CMccUlDlClient* self = new( ELeave ) UT_CMccUlDlClient();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccUlDlClient::~UT_CMccUlDlClient()
    {
    }

// Default constructor
UT_CMccUlDlClient::UT_CMccUlDlClient()
    {
    }

// Second phase construct
void UT_CMccUlDlClient::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccUlDlClient::SetupL(  )
    {
    iResources = CMccResourcePoolStub::NewL();
    
	iHandler = CMccTestEventHandler::NewL();
	TUint32 sessionId = 1;
	TPckgBuf<TInt> params( 30000 );
	iSourceStub = static_cast< CMccRtpDataSource* >( MDataSource::NewSourceL( KRtpDataSource, params ) );
    iSinkStub = static_cast< CMccRtpDataSink* >(MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 ));
    
	iClient = CMccUlDlClient::NewL( iHandler, iResources, sessionId);
	TInt linkType = KMccLinkGeneral;

	TUint   port = 5000;
	TUint   IapId = 2;
	TInt    ipTOS = 23;
	TInt    mediaSig = 10;
	
	iClient->CreateLinkL( iLinkId, linkType );
	
	TRequestStatus status;
    iClient->InitializeLinkL( status, iLinkId, IapId );
    User::WaitForAnyRequest();
    iClient->CreateRtpSessionL( iLinkId, port, ipTOS, mediaSig );
 
    TUint8 payloadType = 100;
    TFourCC fourCC;
    TMMFPrioritySettings prioritySettings;
    iStreamId = iClient->GenerateStreamId();
    iClient->OpenL( iLinkId,
                    iStreamId,
                    payloadType,
                    fourCC,
                    iSourceStub,
                    iSinkStub,
                    prioritySettings );                  
    }


void UT_CMccUlDlClient::Teardown(  )
    {
    if ( iClient && iClient->StreamsExistsL( iLinkId ) )
        {
        iClient->CloseL(iLinkId, iStreamId);
        }
    delete iClient;
    delete iSinkStub;
    delete iSourceStub;
	delete iHandler;
	delete iResources;
    }
    

void UT_CMccUlDlClient::UT_CMccUlDlClient_NewLL(  )
    {
    TUint32 sessionId = 2;
	CMccUlDlClient* client = CMccUlDlClient::NewL( iHandler, iResources, sessionId );
	CleanupStack::PushL( client );
	CleanupStack::PopAndDestroy( client );
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_GetCodecDefaultsLL()
    {
    TMccCodecInfo codecInfo;
    TMccCodecInfoBuffer buffer( codecInfo );
    TUid nullUid = KNullUid;
    TBuf8<1> nullSettings;
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->GetCodecDefaultsL( nullUid, nullSettings, codecInfo ) );
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_GetGainLL(  )
    {
   	TInt gain = 10;
	iClient->SetGainL( gain );
	EUNIT_ASSERT_EQUALS(
		iClient->GetGainL(iLinkId,iStreamId,gain), 
		KErrNone);
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_SetGainLL(  )
    {
   	TInt gain = 10;
	iClient->SetGainL( gain );

    iClient->GetGainL(iLinkId,iStreamId,gain);
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_MaxGainLL(  )
    {
    TInt gain = 10;
	iClient->SetGainL( gain );
	iClient->MaxGainL(iLinkId, iStreamId, gain);
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_SetVolumeLL(  )
    {
    TInt volumn = 10;
	iClient->SetVolumeL( volumn );
	iClient->MaxVolumeL(iLinkId, iStreamId, volumn);
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_MaxVolumeLL(  )
    {
    TInt volumn = 10;
	iClient->SetVolumeL( volumn );
	iClient->MaxVolumeL(iLinkId, iStreamId, volumn);
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_GetCodecLL(  )
    {
    TUint32 streamId = 0;

    TMccCodecInfo codec;
	iClient->GetCodecL( iLinkId, iStreamId, codec );
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_GetFmtpAttrLL(  )
    {
    HBufC8* buf = HBufC8::NewLC( 10 );
	TPtr8 bufPtr = buf->Des();
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->GetFmtpAttrL(iLinkId, iStreamId, bufPtr )); 
	CleanupStack::PopAndDestroy( buf );
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_SetPriorityLL(  )
    {
    TMMFPrioritySettings prioSettings;
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->SetPriorityL(iLinkId,iStreamId, prioSettings) );
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_OpenLL(  )
    {
    TInt linkType( KMccLinkGeneral );
    TUint32 sessionId = 2;
    TUint32 linkId = 0;
    TUint port = 5050;
    TInt testIAPId = 1;
    TInt testOpt = 0;
    TInt testSig = 0;
    
	CMccUlDlClient* client = CMccUlDlClient::NewL( iHandler, iResources, sessionId );
	CleanupStack::PushL( client );

    client->CreateLinkL( linkId, linkType );
    
    TRequestStatus status;
    client->InitializeLinkL( status, linkId, testIAPId );
    User::WaitForAnyRequest();
    client->CreateRtpSessionL( linkId, port, testOpt, testSig );

    TUint8 payloadType = 100;
    TFourCC fourCC;
    TMMFPrioritySettings prioritySettings;
    TUint32 streamId = iClient->GenerateStreamId();
    client->OpenL( linkId,
                   streamId,
                   payloadType,
                   fourCC,
                   iSourceStub,
                   iSinkStub,
                   prioritySettings );
    client->CloseL(linkId, streamId);
    client->CloseLinkL(linkId);
	CleanupStack::PopAndDestroy( client );
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_PrepareLL(  )
    {
    TUint32 endpointId = 0;
    EUNIT_ASSERT_LEAVE(iClient->PrepareL(iLinkId, iStreamId, endpointId));
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_PlayLL(  )
    {
    TUint32 endpointId = 0;
    EUNIT_ASSERT_LEAVE(iClient->PrepareL(iLinkId, iStreamId, endpointId));
    EUNIT_ASSERT_LEAVE(iClient->PlayL(iLinkId, iStreamId, endpointId, EFalse, EFalse));
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_PauseLL(  )
    {
    TUint32 endpointId = 0;
    EUNIT_ASSERT_LEAVE(iClient->PrepareL(iLinkId, iStreamId, endpointId));   
    EUNIT_ASSERT_LEAVE(iClient->PlayL(iLinkId, iStreamId, endpointId, EFalse, EFalse));
    EUNIT_ASSERT_LEAVE(iClient->PauseL(iLinkId, iStreamId, endpointId, EFalse));
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_StopLL(  )
    {
    TUint32 endpointId = 0;
    EUNIT_ASSERT_LEAVE(iClient->PrepareL(iLinkId, iStreamId, endpointId));   
    EUNIT_ASSERT_LEAVE(iClient->PlayL(iLinkId, iStreamId, endpointId, EFalse, EFalse));
    EUNIT_ASSERT_LEAVE(iClient->StopL(iLinkId, iStreamId, endpointId));    
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_ResumeLL(  )
    {
    TUint32 endpointId = 0;
    EUNIT_ASSERT_LEAVE(iClient->PrepareL(iLinkId, iStreamId, endpointId));   
    EUNIT_ASSERT_LEAVE(iClient->PlayL(iLinkId, iStreamId, endpointId, EFalse, EFalse));
    EUNIT_ASSERT_LEAVE(iClient->StopL(iLinkId, iStreamId, endpointId)); 
    EUNIT_ASSERT_LEAVE(iClient->ResumeL(iLinkId, iStreamId, endpointId, EFalse));           
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_StreamsExistsLL()
    {
    TBool streamsExist;
    MCC_EUNIT_ASSERT_NO_LEAVE( streamsExist = iClient->StreamsExistsL( iLinkId ) );
    EUNIT_ASSERT( streamsExist );
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_GetPlayBalanceLL(  )
    {
    TInt left;
    TInt right;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(iClient->GetPlayBalanceL(iLinkId, iStreamId, left, right),KErrNotReady);
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_GetRecordBalanceLL(  )
    {
    TInt left;
    TInt right;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(iClient->GetRecordBalanceL(iLinkId, iStreamId, left, right),KErrNotReady);
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_SetRecordBalanceLL(  )
    {
    TInt left = 10;
    TInt right = 20;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(iClient->SetRecordBalanceL(iLinkId, iStreamId, left, right),KErrNotReady);
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_SetPlayBalanceLL(  )
    {
    TInt left = 10;
    TInt right = 20;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(iClient->SetPlayBalanceL(iLinkId, iStreamId, left, right),KErrNotReady);
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_CloseLL(  )
    {    
    iClient->CloseL(iLinkId, iStreamId);
    TUint8 payloadType = 100;
    TFourCC fourCC;
    TMMFPrioritySettings prioritySettings;
    iStreamId = iClient->GenerateStreamId();
    iClient->OpenL( iLinkId,
                    iStreamId,
                    payloadType,
                    fourCC,
                    iSourceStub,
                    iSinkStub,
                    prioritySettings );
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_GetSessionIdL(  )
    {
    TRtpId sessionId = iClient->GetSessionId();   
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_SetRemoteAddressLL(  )
    {
    TInetAddr inetAddr(5000); // port
    iClient->SetRemoteAddressL(inetAddr, iLinkId);
    }
    
void UT_CMccUlDlClient::UT_CMccUlDlClient_SetRemoteRtcpAddrLL(  )
    {
    TInetAddr inetAddr(5000); // port
    iClient->SetRemoteRtcpAddrL(inetAddr, iLinkId);
    }    

void UT_CMccUlDlClient::UT_CMccUlDlClient_CreateLinkLL(  )
    {
    TInt linkType( KMccLinkGeneral );
    TUint32 sessionId = 1;
    TUint32 linkId = 0;
    TUint port = 5050;
    TInt testIAPId = 1;
    TInt testOpt = 0;
    TInt testSig = 0;
    
	CMccUlDlClient* client = CMccUlDlClient::NewL( iHandler, iResources, sessionId );
	CleanupStack::PushL( client );

    client->CreateLinkL( linkId, linkType );
    EUNIT_ASSERT( linkId != 0 );
    
    TUint32 linkId2 = 0;
    linkType = KMccLinkLocal;
    client->CreateLinkL( linkId2, linkType );
    EUNIT_ASSERT( linkId2 != 0 );

    TUint32 linkId3 = 0;
    linkType = KMccLinkGeneralVideo;
    client->CreateLinkL( linkId3, linkType );
    EUNIT_ASSERT( linkId3 != 0 );
    
    TUint32 linkId4 = 0;
    linkType = KMccLinkSecure;
    client->CreateLinkL( linkId4, linkType );
    EUNIT_ASSERT( linkId4 != 0 );
    
    TUint32 linkId5 = 0;
    linkType = KMccLinkSecureVideo;
    client->CreateLinkL( linkId5, linkType );
    EUNIT_ASSERT( linkId5 != 0 );
    
	CleanupStack::PopAndDestroy( client );
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_InitializeLinkLL()
    {
    
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_CreateRtpSessionLL()
    {
    
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_CloseLinkLL(  )
    {
    TInt linkType( KMccLinkGeneral );
    TUint32 sessionId = 1;
    TUint32 linkId = 0;
    TUint port = 5050;
    TInt testIAPId = 1;
    TInt testOpt = 0;
    TInt testSig = 0;
    
	CMccUlDlClient* client = CMccUlDlClient::NewL( iHandler, iResources, sessionId );
	CleanupStack::PushL( client );

    client->CreateLinkL( linkId, linkType );

    client->CloseLinkL( linkId );
    
	CleanupStack::PopAndDestroy( client );
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_SendMediaSignalLL(  )
    {
    TMccEvent event;
    UT_CMccSymSubthreadClient::CreateRtcpEvent( event, KMccRtcpControl );
    
    event.iLinkId = iLinkId;
    event.iStreamId = iStreamId;
    
    iClient->SendMediaSignalL( event );
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_SetCodecInformationLL(  )
    {
    TFourCC fourAMR(KMccFourCCIdAMRNB);
    TFourCC fourG711(KMccFourCCIdG711);
    TFourCC fourILBC(KMccFourCCIdILBC);
    
    TMccCodecInfo codec;
    codec.iType = KUidMediaTypeAudio;
    codec.iFourCC = fourAMR;
    codec.iBitrate = 12200;
    codec.iPtime = 20;
    codec.iMaxPtime = 40;
    codec.iCodecMode = KAmrCodecModeOctetAlign;
    codec.iEnableDTX = EFalse;
    codec.iPayloadType = 96;
    codec.iPayloadFormatEncoder = KImplUidAmrPayloadFormatEncode;

    TBuf8<4> buf( _L8( "foo" ) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iClient->SetCodecInformationL(iLinkId, iStreamId, codec, buf) );
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_GetSupportedBitratesLL(  )
    {
    RArray<TUint> array;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iClient->GetSupportedBitratesL(iLinkId, iStreamId, array), KErrNotSupported);
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_GetSSRCLL(  )
    {
    TUint32 ssrc;
    iClient->GetSSRCL( iLinkId, iStreamId, ssrc );
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_SendRTCPReceiverReportLL(  )
    {
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(iClient->SendRTCPReceiverReportL( iLinkId, iStreamId ),KErrNotSupported);
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_SendRTCPSenderReportLL(  )
    {
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(iClient->SendRTCPSenderReportL( iLinkId, iStreamId ), KErrNotReady );
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_SendRTCPDataLL(  )
    {
    TBuf8<4> buf( _L8( "foo" ) );
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(iClient->SendRTCPSenderReportL( iLinkId, iStreamId), KErrNotReady );
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_StartInactivityTimerLL( )
    {
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(iClient->StartInactivityTimerL( iLinkId, iStreamId, 80000 ), KErrNotSupported );
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_StopInactivityTimerLL(  )
    {
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(iClient->StopInactivityTimerL( iLinkId, iStreamId ), KErrNotSupported );
    }
    
void UT_CMccUlDlClient::UT_CMccUlDlClient_AudioRoutingLL()
    {
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(iClient->SetAudioRouteL( iLinkId, iStreamId, KMccAudioToPublic ), KErrNotSupported );
    TUint32 route;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(iClient->GetAudioRouteL( iLinkId, iStreamId, route ), KErrNotSupported );
    }


void UT_CMccUlDlClient::UT_CMccUlDlClient_GenerateStreamIdL(  )
    {
    TUint32 streamId = iClient->GenerateStreamId();
    EUNIT_ASSERT( streamId != 0 );
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_UnuseLL(  )
    {
    
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_ResolveIpAddressesLL(  )
    {
    
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_StunBindingKeepAliveLL(  )
    {
    
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_SetResolveStateL(  )
    {
    
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_BindContextIntoStreamLL(  )
    {
    TMccCryptoContext defContext;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(
        iClient->BindContextIntoStreamL( iLinkId, iStreamId, 
            MCC_ENDPOINT_ID( iSourceStub ), defContext ),
        KErrNotFound );
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_RemoveContextLL(  )
    {
    TMccCryptoContext defContext;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(
        iClient->BindContextIntoStreamL( iLinkId, iStreamId, 
            MCC_ENDPOINT_ID( iSourceStub ), defContext ),
        KErrNotFound );
    
    MCC_EUNIT_ASSERT_NO_LEAVE(
        iClient->RemoveContextL( iLinkId, iStreamId, 
        MCC_ENDPOINT_ID( iSourceStub ) ) );
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_SendMccEventToClientL(  )
    {
    
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_StateChangeL(  )
    {
    
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_ErrorOccuredL(  )
    {
    
    }

void UT_CMccUlDlClient::UT_CMccUlDlClient_SetParameterLL()
    {
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(iClient->SetParameterL( KMccRtpCName, 400, iStreamId, 0, _L8("foofighter") ), 
                                KErrNotFound );
    
    // Not supported param
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(iClient->SetParameterL( 999, iLinkId, iStreamId, 0, _L8("foofighter") ), 
                                KErrNotSupported );
                                
    // Ok param
    MCC_EUNIT_ASSERT_NO_LEAVE(iClient->SetParameterL( KMccRtpCName, iLinkId, iStreamId, 0, _L8("foofighter") ) );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccUlDlClient,
    "MccUlDlClient test",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMccUlDlClient",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_NewLL, Teardown)

EUNIT_TEST(
    "GetCodecDefaultsL - test ",
    "CMccUlDlClient",
    "GetCodecDefaultsL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_GetCodecDefaultsLL, Teardown)

EUNIT_TEST(
    "GetGainL - test ",
    "CMccUlDlClient",
    "GetGainL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_GetGainLL, Teardown)

EUNIT_TEST(
    "SetGainL - test ",
    "CMccUlDlClient",
    "SetGainL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_SetGainLL, Teardown)

EUNIT_TEST(
    "MaxGainL - test ",
    "CMccUlDlClient",
    "MaxGainL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_MaxGainLL, Teardown)

EUNIT_TEST(
    "SetVolumeL - test ",
    "CMccUlDlClient",
    "SetVolumeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_SetVolumeLL, Teardown)

EUNIT_TEST(
    "MaxVolumeL - test ",
    "CMccUlDlClient",
    "MaxVolumeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_MaxVolumeLL, Teardown)

EUNIT_TEST(
    "GetCodecL - test ",
    "CMccUlDlClient",
    "GetCodecL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_GetCodecLL, Teardown)

EUNIT_TEST(
    "GetFmtpAttrL - test ",
    "CMccUlDlClient",
    "GetFmtpAttrL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_GetFmtpAttrLL, Teardown)

EUNIT_TEST(
    "SetPriorityL - test ",
    "CMccUlDlClient",
    "SetPriorityL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_SetPriorityLL, Teardown)

EUNIT_TEST(
    "OpenL - test ",
    "CMccUlDlClient",
    "OpenL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_OpenLL, Teardown)

EUNIT_TEST(
    "PrepareL - test ",
    "CMccUlDlClient",
    "PrepareL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_PrepareLL, Teardown)

EUNIT_TEST(
    "PlayL - test ",
    "CMccUlDlClient",
    "PlayL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_PlayLL, Teardown)

EUNIT_TEST(
    "PauseL - test ",
    "CMccUlDlClient",
    "PauseL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_PauseLL, Teardown)

EUNIT_TEST(
    "StopL - test ",
    "CMccUlDlClient",
    "StopL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_StopLL, Teardown)

EUNIT_TEST(
    "ResumeL - test ",
    "CMccUlDlClient",
    "ResumeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_ResumeLL, Teardown)

EUNIT_TEST(
    "StreamsExistsL - test ",
    "CMccUlDlClient",
    "StreamsExistsL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_StreamsExistsLL, Teardown)

EUNIT_TEST(
    "GetPlayBalanceL - test ",
    "CMccUlDlClient",
    "GetPlayBalanceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_GetPlayBalanceLL, Teardown)

EUNIT_TEST(
    "GetRecordBalanceL - test ",
    "CMccUlDlClient",
    "GetRecordBalanceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_GetRecordBalanceLL, Teardown)

EUNIT_TEST(
    "SetRecordBalanceL - test ",
    "CMccUlDlClient",
    "SetRecordBalanceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_SetRecordBalanceLL, Teardown)

EUNIT_TEST(
    "SetPlayBalanceL - test ",
    "CMccUlDlClient",
    "SetPlayBalanceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_SetPlayBalanceLL, Teardown)

EUNIT_TEST(
    "CloseL - test ",
    "CMccUlDlClient",
    "CloseL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_CloseLL, Teardown)

EUNIT_TEST(
    "GetSessionId - test ",
    "CMccUlDlClient",
    "GetSessionId",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_GetSessionIdL, Teardown)

EUNIT_TEST(
    "SetRemoteAddressL - test ",
    "CMccUlDlClient",
    "SetRemoteAddressL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_SetRemoteAddressLL, Teardown)

EUNIT_TEST(
    "SetRemoteAddressL - test ",
    "CMccUlDlClient",
    "SetRemoteAddressL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_SetRemoteRtcpAddrLL, Teardown)


EUNIT_TEST(
    "CreateLinkL - test ",
    "CMccUlDlClient",
    "CreateLinkL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_CreateLinkLL, Teardown)

EUNIT_TEST(
    "InitializeLinkL - test ",
    "CMccUlDlClient",
    "InitializeLinkL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_InitializeLinkLL, Teardown)

EUNIT_TEST(
    "CreateRtpSessionL - test ",
    "CMccUlDlClient",
    "CreateRtpSessionL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_CreateRtpSessionLL, Teardown)

EUNIT_TEST(
    "CloseLinkL - test ",
    "CMccUlDlClient",
    "CloseLinkL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_CloseLinkLL, Teardown)

EUNIT_TEST(
    "SendMediaSignalL - test ",
    "CMccUlDlClient",
    "SendMediaSignalL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_SendMediaSignalLL, Teardown)

EUNIT_TEST(
    "SetCodecInformationL - test ",
    "CMccUlDlClient",
    "SetCodecInformationL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_SetCodecInformationLL, Teardown)

EUNIT_TEST(
    "GetSupportedBitratesL - test ",
    "CMccUlDlClient",
    "GetSupportedBitratesL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_GetSupportedBitratesLL, Teardown)

EUNIT_TEST(
    "GetSSRCL - test ",
    "CMccUlDlClient",
    "GetSSRCL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_GetSSRCLL, Teardown)

EUNIT_TEST(
    "SendRTCPReceiverReportL - test ",
    "CMccUlDlClient",
    "SendRTCPReceiverReportL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_SendRTCPReceiverReportLL, Teardown)

EUNIT_TEST(
    "SendRTCPSenderReportL - test ",
    "CMccUlDlClient",
    "SendRTCPSenderReportL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_SendRTCPSenderReportLL, Teardown)

EUNIT_TEST(
    "SendRTCPDataL - test ",
    "CMccUlDlClient",
    "SendRTCPDataL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_SendRTCPDataLL, Teardown)

EUNIT_TEST(
    "StartInactivityTimerL - test ",
    "CMccUlDlClient",
    "StartInactivityTimerL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_StartInactivityTimerLL, Teardown)

EUNIT_TEST(
    "StopInactivityTimerL - test ",
    "CMccUlDlClient",
    "StopInactivityTimerL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_StopInactivityTimerLL, Teardown)

EUNIT_TEST(
    "AudioRouting - test ",
    "CMccUlDlClient",
    "AudioRouting",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_AudioRoutingLL, Teardown)

EUNIT_TEST(
    "GenerateStreamId - test ",
    "CMccUlDlClient",
    "GenerateStreamId",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_GenerateStreamIdL, Teardown)

EUNIT_TEST(
    "UnuseL - test ",
    "CMccUlDlClient",
    "UnuseL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_UnuseLL, Teardown)

EUNIT_TEST(
    "ResolveIpAddressesL - test ",
    "CMccUlDlClient",
    "ResolveIpAddressesL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_ResolveIpAddressesLL, Teardown)

EUNIT_TEST(
    "StunBindingKeepAliveL - test ",
    "CMccUlDlClient",
    "StunBindingKeepAliveL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_StunBindingKeepAliveLL, Teardown)

EUNIT_TEST(
    "SetResolveState - test ",
    "CMccUlDlClient",
    "SetResolveState",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_SetResolveStateL, Teardown)

EUNIT_TEST(
    "BindContextIntoStreamL - test ",
    "CMccUlDlClient",
    "BindContextIntoStreamL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_BindContextIntoStreamLL, Teardown)

EUNIT_TEST(
    "RemoveContextL - test ",
    "CMccUlDlClient",
    "RemoveContextL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_RemoveContextLL, Teardown)

EUNIT_TEST(
    "SendMccEventToClient - test ",
    "CMccUlDlClient",
    "SendMccEventToClient",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_SendMccEventToClientL, Teardown)

EUNIT_TEST(
    "StateChange - test ",
    "CMccUlDlClient",
    "StateChange",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_StateChangeL, Teardown)

EUNIT_TEST(
    "ErrorOccured - test ",
    "CMccUlDlClient",
    "ErrorOccured",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_ErrorOccuredL, Teardown)

EUNIT_TEST(
    "SetParameterL - test ",
    "CMccUlDlClient",
    "SetParameterL",
    "FUNCTIONALITY",
    SetupL, UT_CMccUlDlClient_SetParameterLL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
