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
#include "ut_cmcesipsession.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <sipstrings.h>
#include <sipcontactheader.h>

//  INTERNAL INCLUDES
#include "mcetesthelper.h"
#include "mcesipsession.h"
#include "mcesipstatemachine.h"
#include "mceservercore.h"
#include "mcecssession.h"
#include "mcecssessionimplementation.h"
#include "mcesipconnection.h"
#include "cmcetls.h"
#include "mceactionset.h"
#include "mcefcactionset.h"
#include "mcecssubsession.h"
#include "mceclientserver.h"

#include "mcecomcameraSource.h"
#include "mcecomh263codec.h"


_LIT8(KFmtAttribute,"a=fmtp:TBCP format parameters\r\n");
_LIT8(KFmtAttribute2,"a=fmtp:TestPlugIn format parameters\r\n");
_LIT8(KSessionAttribute,"a=application");
_LIT8(KSessionAttribute2,"a=application2");

// CONSTRUCTION

// ---------------------------------------------------------------------------
// UT_CMceSipSession::NewL
// ---------------------------------------------------------------------------
//
UT_CMceSipSession* UT_CMceSipSession::NewL()
    {
    UT_CMceSipSession* self = UT_CMceSipSession::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::NewLC
// ---------------------------------------------------------------------------
//
UT_CMceSipSession* UT_CMceSipSession::NewLC()
    {
    UT_CMceSipSession* self = new( ELeave ) UT_CMceSipSession();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::~UT_CMceSipSession
// ---------------------------------------------------------------------------
//
UT_CMceSipSession::~UT_CMceSipSession()
    {
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession
// ---------------------------------------------------------------------------
//
UT_CMceSipSession::UT_CMceSipSession()
    {
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::ConstructL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

// ---------------------------------------------------------------------------
// UT_CMceSipSession::SetupL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::SetupL()
    {
    CMCETls::OpenL();
    SdpCodecStringPool::OpenL();
    
    //TMceIds ids;
    iIds.iAppUID = 10;
    iIds.iProfileID = 1;
    iIds.iManagerType = KMceCSSIPSession;
    iIds.iDialogType = KMceDlgTypeInvite;

    iServerCore = CMceServerCore::NewL();
    
    iStorage = CMCETls::Storage();
    
	CDesC8ArrayFlat* params = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
	CleanupStack::PushL( params );
	params->AppendL( _L8("") );
	params->AppendL( _L8("User <user@host>") );
    
    CMceMsgTextArray* msgParams = new (ELeave) CMceMsgTextArray( *params );
    CleanupStack::PushL( msgParams );
    iStorage->StoreL( iIds, *msgParams );
    CleanupStack::PopAndDestroy( msgParams );
    CleanupStack::PopAndDestroy( params );
    
    iSession = CMceCsSession::NewL( *iServerCore );
    iSession->SetClientUidL( iIds, RMessage2() );
    
    iSession->CreateSubSessionL(iIds, RMessage2());
    iSipSession = static_cast<CMceSipSession*>((iSession->Implementation().SubSessions())[0]);
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::Teardown
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::Teardown()
    {
    CMCETls::Close();

	delete iSession;
	iSession = NULL;
    delete iServerCore;
    iServerCore = NULL;
    SdpCodecStringPool::Close();
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_NeedToSendL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_NeedToSendL()
    {
    TMceMediaState* state = NULL;
    // No need to send:
    // Media manager returns no need to negotiate, all the offers answered
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    iSipSession->IncrementSentSdpCount();
    EUNIT_ASSERT_EQUALS( 1, iSipSession->iSentSdpCount )
    iSipSession->IncrementReceivedSdpCount();
    EUNIT_ASSERT_EQUALS( 1, iSipSession->iReceivedSdpCount )
    EUNIT_ASSERT( !iSipSession->NeedToSend( NULL ) )
    
    // Need to send:
    // Media manager returns need to negotiate, all the offers answered
    iStorage->iMediaManagerNeedToNegotiate = ETrue;
    iSipSession->ResetSdpCounts();
    EUNIT_ASSERT_EQUALS( 0, iSipSession->iSentSdpCount )
    EUNIT_ASSERT_EQUALS( 0, iSipSession->iReceivedSdpCount )
    EUNIT_ASSERT( iSipSession->NeedToSend( NULL ) )    
    
    // Need to send:
    // Media manager returns no need to negotiate, all the offers not answered
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    iSipSession->IncrementReceivedSdpCount();
    EUNIT_ASSERT_EQUALS( 1, iSipSession->iReceivedSdpCount )
    EUNIT_ASSERT( iSipSession->NeedToSend( NULL ) )  
    
    
    //Need to Send 
    //If role is an offerer, and media manager indicates no need to send
    //we will not send new offer. 
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    iSipSession->ResetSdpCounts();
    iSipSession->IncrementReceivedSdpCount();  
    EUNIT_ASSERT_EQUALS( 1, iSipSession->iReceivedSdpCount )
    iSipSession->IncrementSentSdpCount();
    EUNIT_ASSERT_EQUALS( 1, iSipSession->iSentSdpCount )
    iSipSession->IncrementReceivedSdpCount();  
    EUNIT_ASSERT_EQUALS( 2, iSipSession->iReceivedSdpCount )
    
    iStorage->iRole = EMceRoleOfferer;
    EUNIT_ASSERT( !iSipSession->NeedToSend( NULL ) )
    
    // Need to send:
    // Media manager returns need to negotiate, all the offers not answered
    iStorage->iMediaManagerNeedToNegotiate = ETrue;
    EUNIT_ASSERT( iSipSession->NeedToSend( NULL ) )  
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_DoesMediaManagerNeedToNegotiateL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_DoesMediaManagerNeedToNegotiateL()
    {
    // Media manager returns need to negotiate
    iStorage->iMediaManagerNeedToNegotiate = ETrue;
    EUNIT_ASSERT( iSipSession->DoesMediaManagerNeedToNegotiate( NULL ) )

    // Media manager returns no need to negotiate
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    EUNIT_ASSERT( !iSipSession->DoesMediaManagerNeedToNegotiate( NULL ) )
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_ServiceLL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_ServiceLL()
    {
    iIds.iProfileID = 1;
    iIds.iManagerType = KMceCSSIPSession;
    iIds.iDialogType = KMceDlgTypeInvite;
    iIds.iMsgType = EMceItcMsgTypeSession;

    RMessage2 msg;
    CMceCsSubSession* ses = iSipSession;
    
    _LIT8( originator, "sip:originator@foo.bar" );
    _LIT8( recipient, "sip:recipient@foo.bar" );
    _LIT8( contentType, "contenttype" );
    _LIT8( header, "header" );
    _LIT8( amr, "AMR" );
    _LIT8( h263, "H263" ); 
    
    TBuf8<KMceMaxSdpNameLength> amrName;
    amrName.Append( amr );
    
    
    TBuf8<KMceMaxSdpNameLength> vidocodecName;
    vidocodecName.Append( h263 );
        

	CDesC8ArrayFlat* medialines = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
	CleanupStack::PushL( medialines );
	medialines->AppendL( KFmtAttribute );
    CMceMsgTextArray* msgLines = new (ELeave) CMceMsgTextArray( *medialines );
    CleanupStack::PushL( msgLines );
    CMceComSession* clientSession = CMceComSession::NewL( CMceComSession::EOutSession );
    CleanupStack::PushL( clientSession );
        
    CMceComAudioStream* inStream = CMceComAudioStream::NewLC();
    CMceComAudioStream* outStream = CMceComAudioStream::NewLC();
    inStream->BindL( outStream );
    CleanupStack::Pop( outStream );
        
    
    CMceComVideoStream* inStreamVideo = CMceComVideoStream::NewLC();
    CMceComVideoStream* outStreamVideo = CMceComVideoStream::NewLC();
    inStreamVideo->BindL( outStreamVideo );
    CleanupStack::Pop( outStreamVideo );
    
    CMceComAMRCodec* codec1 = CMceComAMRCodec::NewLC( amrName );
    inStream->AddCodecL( codec1 );
    CleanupStack::Pop( codec1 );

    CMceComAMRCodec* codec2 = CMceComAMRCodec::NewLC( amrName );
    outStream->AddCodecL( codec2 );
    CleanupStack::Pop( codec2 );
       
        
    CMceComH263Codec* codec3 = CMceComH263Codec::NewLC( vidocodecName );
    inStreamVideo->AddCodecL( codec3 );
    CleanupStack::Pop( codec3 );
    
    CMceComH263Codec* codec4 = CMceComH263Codec::NewLC( vidocodecName );
    outStreamVideo->AddCodecL( codec4 );
    CleanupStack::Pop( codec4 );
       
           
    CMceComRtpSource* rtpSource = CMceComRtpSource::NewLC();
    inStream->SetSourceL( rtpSource );
    CleanupStack::Pop( rtpSource );
    
    CMceComSpeakerSink* speaker = CMceComSpeakerSink::NewLC();
    inStream->AddSinkL( speaker );
    CleanupStack::Pop( speaker );
    

    CMceComRtpSink* rtpSink = CMceComRtpSink::NewLC();
    outStream->AddSinkL( rtpSink );
    CleanupStack::Pop( rtpSink );
    
    
    CMceComMicSource* mic = CMceComMicSource::NewLC();
    outStream->SetSourceL( mic );
    CleanupStack::Pop( mic );
        
    CMceComCameraSource* camera = CMceComCameraSource::NewLC();
    outStreamVideo->SetSourceL( camera );
    CleanupStack::Pop( camera );

    clientSession->AddStreamL( inStream ); 
    CleanupStack::Pop( inStreamVideo );
    CleanupStack::Pop( inStream );
    CleanupStack::PushL( inStreamVideo );
    clientSession->AddStreamL( inStreamVideo );
    CleanupStack::Pop( inStreamVideo );
          
    clientSession->InitializeL();

    //set ids
    TUid appUid = TUid::Uid( 10 );
    clientSession->iID = reinterpret_cast<TUint32>(&iSipSession);
    inStream->iID   = TMceMediaId(appUid,1);
    outStream->iID  = TMceMediaId(appUid,2);
    rtpSource->iID  = TMceMediaId(appUid,3);
    speaker->iID    = TMceMediaId(appUid,4);
    rtpSink->iID    = TMceMediaId(appUid,5);
    mic->iID        = TMceMediaId(appUid,6);
    
    inStreamVideo->iID   = TMceMediaId(appUid,7);
    outStreamVideo->iID  = TMceMediaId(appUid,8);
    camera->iID          = TMceMediaId(appUid,9);
    

    //set session
    clientSession->iIsConnectionActive = ETrue;
    clientSession->iDialogId = 434;
    clientSession->iIapId = 4;
    clientSession->iMccID = 101;
    clientSession->iMinSE = 200;
    clientSession->iTimeout = 30;
    clientSession->iLocalIpAddress = TInetAddr( 5005 );
    clientSession->iRemoteIpAddress = TInetAddr( 5010 );
    clientSession->SetOriginatorL( originator );
    clientSession->SetRecipientL( recipient );
    MCE_TH_SET( clientSession->iSIPContentType, contentType().AllocL() );
    MCE_TH_SET( clientSession->iSIPHeaders, new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity ) );
    clientSession->iSIPHeaders->AppendL( header );
    MCE_TH_SET( clientSession->iLocalSessionSDPLines, new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity ) );
    clientSession->iLocalSessionSDPLines->AppendL( KSessionAttribute );
    MCE_TH_SET( clientSession->iRemoteSessionSDPLines, new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity ) );
	clientSession->iRemoteSessionSDPLines->AppendL( KSessionAttribute2 );
	
	inStream->iLocalMediaPort = 321;
	inStream->iIsEnabled = ETrue;
	inStream->iRemoteMediaPort = 354;
	inStream->iLinkId = 31;
	inStream->iRemoteRtcpAddress = TInetAddr( 3004 );
    MCE_TH_SET( inStream->iLocalMediaSDPLines, new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity ) );
    inStream->iLocalMediaSDPLines->AppendL( KFmtAttribute );
    MCE_TH_SET( inStream->iRemoteMediaSDPLines, new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity ) );
	inStream->iRemoteMediaSDPLines->AppendL( KFmtAttribute2 );

	outStream->iLocalMediaPort = 521;
	outStream->iRemoteMediaPort = 554;
	outStream->iLinkId = 51;
	outStream->iIsEnabled = EFalse;
	outStream->iRemoteRtcpAddress = TInetAddr( 5004 );
    MCE_TH_SET( outStream->iLocalMediaSDPLines, new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity ) );
    outStream->iLocalMediaSDPLines->AppendL( KFmtAttribute2 );
    MCE_TH_SET( outStream->iRemoteMediaSDPLines, new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity ) );
	outStream->iRemoteMediaSDPLines->AppendL( KFmtAttribute );
    
    inStreamVideo->iLocalMediaPort = 321;
	inStreamVideo->iIsEnabled = ETrue;
	inStreamVideo->iRemoteMediaPort = 354;
	inStreamVideo->iLinkId = 31;
	inStreamVideo->iRemoteRtcpAddress = TInetAddr( 3004 );
    MCE_TH_SET( inStreamVideo->iLocalMediaSDPLines, new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity ) );
    inStreamVideo->iLocalMediaSDPLines->AppendL( KFmtAttribute );
    MCE_TH_SET( inStreamVideo->iRemoteMediaSDPLines, new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity ) );
	inStreamVideo->iRemoteMediaSDPLines->AppendL( KFmtAttribute2 );

	outStreamVideo->iLocalMediaPort = 521;
	outStreamVideo->iRemoteMediaPort = 554;
	outStreamVideo->iLinkId = 51;
	outStreamVideo->iIsEnabled = EFalse;
	outStreamVideo->iRemoteRtcpAddress = TInetAddr( 5004 );
    MCE_TH_SET( outStreamVideo->iLocalMediaSDPLines, new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity ) );
    outStreamVideo->iLocalMediaSDPLines->AppendL( KFmtAttribute2 );
    MCE_TH_SET( outStreamVideo->iRemoteMediaSDPLines, new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity ) );
	outStreamVideo->iRemoteMediaSDPLines->AppendL( KFmtAttribute );
    
	
    codec1->iEnableVAD = ETrue;
    codec1->iSamplingFreq = 111;
    codec1->iPTime = 150;
    codec1->iMaxPTime = 190;
    codec1->iBitrate = 1122;
    codec1->iAllowedBitrates = 11111;
    codec1->iPayloadType = 197;
    codec1->iCodecMode = 12;
    codec1->iFourCC = 1567;
    codec1->iFrameSize = 165;
    MCE_TH_SET( codec1->iFmtpAttr, KFmtAttribute().AllocL() );
    
    codec2->iEnableVAD = ETrue;
    codec2->iSamplingFreq = 211;
    codec2->iPTime = 250;
    codec2->iMaxPTime = 290;
    codec2->iBitrate = 2122;
    codec2->iAllowedBitrates = 21111;
    codec2->iPayloadType = 297;
    codec2->iCodecMode = 22;
    codec2->iFourCC = 2567;
    codec2->iFrameSize = 265;
    MCE_TH_SET( codec2->iFmtpAttr, KFmtAttribute().AllocL() );
    
       
    codec3->iFrameRate = 9;
    codec3->iMaxBitRate = 40000;
    codec3->iAllowedResolutions = 24;
    codec3->iResolutionWidth = 200;
    codec3->iResolutionHeight = 200;
    codec3->iAllowedFrameRates = 100;
    codec3->iClockRate = 5000;
      
    codec3->iBitrate = 1122;
    codec3->iAllowedBitrates = 11111;
    codec3->iPayloadType = 197;
    codec3->iCodecMode = 12;
    codec3->iFourCC = 1567;
    codec3->iFrameSize = 165;
    MCE_TH_SET( codec3->iFmtpAttr, KFmtAttribute().AllocL() );
    
    codec4->iFrameRate = 9;
    codec4->iMaxBitRate = 40000;
    codec4->iAllowedResolutions = 24;
    codec4->iResolutionWidth = 200;
    codec4->iResolutionHeight = 200;
    codec4->iAllowedFrameRates = 100;
    codec4->iClockRate = 5000;
    
    codec4->iBitrate = 2122;
    codec4->iAllowedBitrates = 21111;
    codec4->iPayloadType = 297;
    codec4->iCodecMode = 22;
    codec4->iFourCC = 2567;
    codec4->iFrameSize = 265;
    MCE_TH_SET( codec4->iFmtpAttr, KFmtAttribute().AllocL() );
      
    camera->Enabled( ETrue );
    mic->Enabled( ETrue );
    rtpSource->Enabled( EFalse );
    rtpSink->Enabled( ETrue );
    speaker->Enabled( EFalse );
    
    rtpSource->iInactivityTimer = 999;
    rtpSource->iBufferLength = 999;
    rtpSource->iBufferTreshold = 999;
    MCE_TH_SET( rtpSource->iTalkBurstIndicator, KFmtAttribute().AllocL() );


    rtpSink->iSuppressRTCP = ETrue;
    rtpSink->iSSRC = 888;
    rtpSink->iLastPacket = 8899;


//test cloning
    CMceComSession* clone = NULL;
    const TInt KTestCloning = 0;

    for( TInt i = 0; i < 2; i++ )
        {        
        if ( i == KTestCloning )
            {
            clone = clientSession->CloneL();
            CleanupStack::PushL( clone );
            }

        EUNIT_ASSERT( clone->iID == clientSession->iID );
        EUNIT_ASSERT( clone->iType == clientSession->iType );
        EUNIT_ASSERT( clone->iState == clientSession->iState );
        EUNIT_ASSERT( clone->iIsConnectionActive == clientSession->iIsConnectionActive );
        EUNIT_ASSERT( clone->iDialogId == clientSession->iDialogId );
        EUNIT_ASSERT( clone->iMinSE == clientSession->iMinSE );
        EUNIT_ASSERT( clone->iTimeout == clientSession->iTimeout );
        EUNIT_ASSERT( clone->iOriginator->Compare( *clientSession->iOriginator ) == 0 );
        EUNIT_ASSERT( clone->iRecipient->Compare( *clientSession->iRecipient ) == 0 );
        EUNIT_ASSERT( clone->iSIPContentType->Compare( *clientSession->iSIPContentType ) == 0 );
        EUNIT_ASSERT( clone->iSIPHeaders->MdcaCount() ==  1 );
        EUNIT_ASSERT( clone->iSIPHeaders->MdcaCount() == clientSession->iSIPHeaders->MdcaCount() );
        EUNIT_ASSERT( clone->iSIPHeaders->MdcaPoint( 0 ).Compare( clientSession->iSIPHeaders->MdcaPoint( 0 ) ) == 0 );
        EUNIT_ASSERT( clone->iLocalSessionSDPLines->MdcaCount() ==  1 );
        EUNIT_ASSERT( clone->iLocalSessionSDPLines->MdcaCount() == clientSession->iLocalSessionSDPLines->MdcaCount() );
        EUNIT_ASSERT( clone->iLocalSessionSDPLines->MdcaPoint( 0 ).Compare( clientSession->iLocalSessionSDPLines->MdcaPoint( 0 ) ) == 0 );
        EUNIT_ASSERT( clone->iRemoteSessionSDPLines->MdcaCount() ==  1 );
        EUNIT_ASSERT( clone->iRemoteSessionSDPLines->MdcaCount() == clientSession->iRemoteSessionSDPLines->MdcaCount() );
        EUNIT_ASSERT( clone->iRemoteSessionSDPLines->MdcaPoint( 0 ).Compare( clientSession->iRemoteSessionSDPLines->MdcaPoint( 0 ) ) == 0 );
        
        if ( i == KTestCloning )//test clone
            {
            EUNIT_ASSERT( clone->iIapId == clientSession->iIapId );
            EUNIT_ASSERT( clone->iLocalIpAddress == clientSession->iLocalIpAddress );
            EUNIT_ASSERT( clone->iRemoteIpAddress == clientSession->iRemoteIpAddress );
            }
        EUNIT_ASSERT( clone->Streams().Count() == clientSession->Streams().Count() );
        
        CMceComAudioStream* cinStream = static_cast<CMceComAudioStream*>( clone->Streams()[0] );
        CMceComAudioStream* coutStream = static_cast<CMceComAudioStream*>(&cinStream->BoundStreamL());
        CMceComAMRCodec* ccodec1 = static_cast<CMceComAMRCodec*>(cinStream->Codecs()[0]);
        CMceComAMRCodec* ccodec2 = static_cast<CMceComAMRCodec*>(coutStream->Codecs()[0]);
        
        CMceComRtpSource* crtpSource = static_cast<CMceComRtpSource*>(cinStream->Source());
        CMceComSpeakerSink* cspeaker = static_cast<CMceComSpeakerSink*>(cinStream->Sinks()[0]);
        CMceComRtpSink* crtpSink = static_cast<CMceComRtpSink*>(coutStream->Sinks()[0]);
        CMceComMicSource* cmic = static_cast<CMceComMicSource*>(coutStream->Source());
        
        CMceComVideoStream* cinStreamVideo = static_cast<CMceComVideoStream*>( clone->Streams()[1] );
        CMceComVideoStream* coutStreamVideo = static_cast<CMceComVideoStream*>(&cinStreamVideo->BoundStreamL());
        
        CMceComH263Codec* ccodec3 = static_cast<CMceComH263Codec*>(cinStreamVideo->Codecs()[0]);
        CMceComH263Codec* ccodec4 = static_cast<CMceComH263Codec*>(coutStreamVideo->Codecs()[0]);
        
        
        CMceComCameraSource* ccamera = static_cast<CMceComCameraSource*>(coutStream->Source());
        
        
        EUNIT_ASSERT( cinStream->Session() == clone );
        EUNIT_ASSERT( cinStream->iID == inStream->iID );
        EUNIT_ASSERT( cinStream->iType == inStream->iType );
        EUNIT_ASSERT( cinStream->iIsEnabled );
        EUNIT_ASSERT( cinStream->iLocalMediaPort == inStream->iLocalMediaPort );
        EUNIT_ASSERT( cinStream->iRemoteMediaPort == inStream->iRemoteMediaPort );
        EUNIT_ASSERT( cinStream->iLocalMediaSDPLines->MdcaPoint( 0 ).Compare( inStream->iLocalMediaSDPLines->MdcaPoint( 0 ) ) == 0 );
        EUNIT_ASSERT( cinStream->iRemoteMediaSDPLines->MdcaPoint( 0 ).Compare( inStream->iRemoteMediaSDPLines->MdcaPoint( 0 ) ) == 0 );
        
        if ( i == KTestCloning )//test clone
            {
            EUNIT_ASSERT( cinStream->iLinkId != inStream->iLinkId ); // link ID not cloned
            EUNIT_ASSERT( cinStream->iRemoteRtcpAddress == inStream->iRemoteRtcpAddress );
            }

        EUNIT_ASSERT( coutStream->Session() == clone );
        EUNIT_ASSERT( coutStream->iID == outStream->iID );
        EUNIT_ASSERT( coutStream->iType == outStream->iType );
        EUNIT_ASSERT( !coutStream->iIsEnabled );
        EUNIT_ASSERT( coutStream->iLocalMediaPort == outStream->iLocalMediaPort );
        EUNIT_ASSERT( coutStream->iRemoteMediaPort == outStream->iRemoteMediaPort );
        EUNIT_ASSERT( coutStream->iRemoteMediaSDPLines->MdcaPoint( 0 ).Compare( outStream->iRemoteMediaSDPLines->MdcaPoint( 0 ) ) == 0 );
        EUNIT_ASSERT( coutStream->iLocalMediaSDPLines->MdcaPoint( 0 ).Compare( outStream->iLocalMediaSDPLines->MdcaPoint( 0 ) ) == 0 );

        if ( i == KTestCloning )//test clone
            {
            EUNIT_ASSERT( coutStream->iLinkId != outStream->iLinkId );// link ID not cloned
            EUNIT_ASSERT( coutStream->iRemoteRtcpAddress == outStream->iRemoteRtcpAddress );
            }

        EUNIT_ASSERT( ccodec1->Stream() == cinStream );
        EUNIT_ASSERT( ccodec1->iEnableVAD == codec1->iEnableVAD );
        EUNIT_ASSERT( ccodec1->iSamplingFreq == codec1->iSamplingFreq );
        EUNIT_ASSERT( ccodec1->iPTime == codec1->iPTime );
        EUNIT_ASSERT( ccodec1->iMaxPTime == codec1->iMaxPTime );
        EUNIT_ASSERT( ccodec1->iAllowedBitrates == codec1->iAllowedBitrates );
        EUNIT_ASSERT( ccodec1->iCodecMode == codec1->iCodecMode );
        EUNIT_ASSERT( ccodec1->iFrameSize == codec1->iFrameSize );
        if ( i == KTestCloning )//test clone
            {
            EUNIT_ASSERT( ccodec1->iFmtpAttr->Compare( *codec1->iFmtpAttr ) == 0 );
            }

        EUNIT_ASSERT( ccodec2->Stream() == coutStream );
        EUNIT_ASSERT( ccodec2->iEnableVAD == codec2->iEnableVAD );
        EUNIT_ASSERT( ccodec2->iSamplingFreq == codec2->iSamplingFreq );
        EUNIT_ASSERT( ccodec2->iPTime == codec2->iPTime );
        EUNIT_ASSERT( ccodec2->iMaxPTime == codec2->iMaxPTime );
        EUNIT_ASSERT( ccodec2->iAllowedBitrates == codec2->iAllowedBitrates );
        EUNIT_ASSERT( ccodec2->iCodecMode == codec2->iCodecMode );
        EUNIT_ASSERT( ccodec2->iFrameSize == codec2->iFrameSize );
        if ( i == KTestCloning )//test clone
            {
            EUNIT_ASSERT( ccodec2->iFmtpAttr->Compare( *codec2->iFmtpAttr ) == 0 );
            }


        EUNIT_ASSERT( cmic->MediaStream() == coutStream );
        EUNIT_ASSERT( cmic->IsEnabled() == mic->IsEnabled() );
        
        EUNIT_ASSERT( crtpSource->MediaStream() == cinStream );
        EUNIT_ASSERT( crtpSource->IsEnabled() == rtpSource->IsEnabled() );
        
        EUNIT_ASSERT( crtpSink->MediaStream() == coutStream );
        EUNIT_ASSERT( crtpSink->CMceComEndpoint::IsEnabled() == rtpSink->CMceComEndpoint::IsEnabled() );
       
        EUNIT_ASSERT( cspeaker->MediaStream() == cinStream );
        EUNIT_ASSERT( cspeaker->IsEnabled() == speaker->IsEnabled() );
        
        EUNIT_ASSERT( crtpSource->iInactivityTimer == rtpSource->iInactivityTimer );
        EUNIT_ASSERT( crtpSource->iBufferLength == rtpSource->iBufferLength );
        EUNIT_ASSERT( crtpSource->iBufferTreshold == rtpSource->iBufferTreshold );
        EUNIT_ASSERT( crtpSource->iTalkBurstIndicator->Compare( *rtpSource->iTalkBurstIndicator ) == 0 );

        EUNIT_ASSERT( crtpSink->iSuppressRTCP == rtpSink->iSuppressRTCP );
        EUNIT_ASSERT( crtpSink->iSSRC == rtpSink->iSSRC );
        EUNIT_ASSERT( crtpSink->iLastPacket == rtpSink->iLastPacket );

        if ( i == KTestCloning )
            {
            //encode clone
            CMceMsgObject<CMceComSession>* clientSessionMsg = 
                new (ELeave) CMceMsgObject<CMceComSession>( *clone, EMceItcMsgTypeSession );
            CleanupStack::PushL( clientSessionMsg );
            HBufC8* content = _L8("data").AllocLC();
            iStorage->StoreL( iIds, *clientSessionMsg, content );

            CleanupStack::Pop(content);

            CleanupStack::PopAndDestroy( clientSessionMsg );

            CleanupStack::PopAndDestroy(clone);
            
            //decode clone
            clientSessionMsg = new (ELeave) CMceMsgObject<CMceComSession>();
            CleanupStack::PushL( clientSessionMsg );
            clientSessionMsg->DecodeL( *iStorage->iContextClient );
            
            clone = clientSessionMsg->Object(); //gets ownership
            CleanupStack::PopAndDestroy( clientSessionMsg );
            CleanupStack::PushL( clone );
            }
       }
	CleanupStack::PopAndDestroy( clone );

    CMceMsgObject<CMceComSession>* clientSessionMsg = 
        new (ELeave) CMceMsgObject<CMceComSession>( *clientSession, EMceItcMsgTypeSession );
    CleanupStack::PushL( clientSessionMsg );
    HBufC8* content = _L8("data").AllocLC();
    iStorage->StoreL( iIds, *clientSessionMsg, content );
    CleanupStack::Pop( content );
    
    
//EMceItcEstablishSession
    
    ses->ServiceL( iIds, EMceItcEstablishSession, msg );
    
    TMceIds ids;
    ids.iAppUID = appUid.iUid;
    ids.iSessionID = clientSession->iID;
    ids.iMediaID = inStream->iID;
    ids.iSinkID = speaker->iID;
    ids.iMsgType = EMceItcMsgTypeReadString;

//EMceItcVolume

    iStorage->iMediaManagerAction = CMCETls::ENone;
    TMceItcArgTInt volume( 12 );
    iStorage->StoreL( ids, volume );
    
    ses->ServiceL( ids, EMceItcVolume, msg );
    EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::EGetVolume );

//EMceItcSetVolume
    ids.iMsgType = EMceItcMsgTypeWriteString;
    iStorage->iMediaManagerAction = CMCETls::ENone;
    ses->ServiceL( ids, EMceItcSetVolume, msg );
    EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::ESetVolume );
    
  
//EMceItcSetAudioRouting  
    iStorage->iMediaManagerAction = CMCETls::ENone;
    ses->ServiceL( ids, EMceItcSetAudioRouting, msg );
    EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::ESetAudioRouting );


//EMceItcAudioRouting  
    ids.iMsgType = EMceItcMsgTypeReadString;
    TMceItcArgTInt audioRouting( 4 );
    iStorage->StoreL( ids, audioRouting );
    iStorage->iMediaManagerAction = CMCETls::ENone;
    ses->ServiceL( ids, EMceItcAudioRouting, msg );
    EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::EGetAudioRouting );
    
    ids.iMediaID = outStream->iID;
    ids.iSinkID = KMceMediaIdNotAssigned;
    ids.iSourceID = mic->iID;
    
//EMceItcSetGain
    ids.iMsgType = EMceItcMsgTypeWriteString;
    iStorage->iMediaManagerAction = CMCETls::ENone;
    ses->ServiceL( ids, EMceItcSetGain, msg );
    EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::ESetGain );


//EMceItcGain,
    ids.iMsgType = EMceItcMsgTypeReadString;
    TMceItcArgTInt gain( 20 );
    iStorage->StoreL( ids, gain );
    iStorage->iMediaManagerAction = CMCETls::ENone;
    ses->ServiceL( ids, EMceItcGain, msg );
    EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::EGetGain );
 

    ids.iMediaID = outStreamVideo->iID;
    ids.iSinkID = KMceMediaIdNotAssigned;
    ids.iSourceID = camera->iID;


//EMceItcZoomFactor           
    ids.iMsgType = EMceItcMsgTypeReadString;
    TMceItcArgTInt zoomFactor( 20 );
    iStorage->StoreL( ids, zoomFactor );
    iStorage->iMediaManagerAction = CMCETls::ENone;
    ses->ServiceL( ids, EMceItcZoomFactor, msg );
    EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::EGetZoomFactor );
    
    
//EMceItcDigitalZoomFactor
    ids.iMsgType = EMceItcMsgTypeReadString;      
    iStorage->iMediaManagerAction = CMCETls::ENone;
    TMceItcArgTInt digitalZoomFactor( 20 );
    iStorage->StoreL( ids, digitalZoomFactor );
    ses->ServiceL( ids, EMceItcDigitalZoomFactor, msg );
    EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::EGetDigitalZoomFactor );

//EMceItcContrast               
    ids.iMsgType = EMceItcMsgTypeReadString;
    iStorage->iMediaManagerAction = CMCETls::ENone;
    TMceItcArgTInt contrast( 20 );
    iStorage->StoreL( ids, contrast );
    ses->ServiceL( ids, EMceItcContrast, msg );
    EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::EGetContrast );

//EMceItcBrightness            
    ids.iMsgType = EMceItcMsgTypeReadString;
    iStorage->iMediaManagerAction = CMCETls::ENone;
    TMceItcArgTInt brightness( 20 );
    iStorage->StoreL( ids, brightness );
    ses->ServiceL( ids, EMceItcBrightness, msg );
    EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::EGetBrightness );

//EMceItcExposure               
    ids.iMsgType = EMceItcMsgTypeReadString;
    TMceItcArgTInt exposure( 20 );
    iStorage->StoreL( ids, exposure );
    iStorage->iMediaManagerAction = CMCETls::ENone;
    ses->ServiceL( ids, EMceItcExposure, msg );
    EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::EGetExposure );

//EMceItcWhiteBalance
    ids.iMsgType = EMceItcMsgTypeReadString;
    iStorage->iMediaManagerAction = CMCETls::ENone;
    TMceItcArgTInt whiteBalance( 20 );
    iStorage->StoreL( ids, whiteBalance );
    ses->ServiceL( ids, EMceItcWhiteBalance, msg );
    EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::EGetWhiteBalance );

//EMceItcSetCameraIndex
    ids.iMsgType = EMceItcMsgTypeReadString;
    iStorage->iMediaManagerAction = CMCETls::ENone;
    TMceCameraInfo cameraInfo;
    cameraInfo.iIndex = 1;
    TMceItcArgTCameraInfo cameraInfoBuf( cameraInfo );
    iStorage->StoreL( ids, cameraInfoBuf );
    ses->ServiceL( ids, EMceItcSetCameraIndex, msg );
    EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::ESetCameraIndex );
    TMceCameraInfo cameraInfoResult;
    
    TMceItcArgTCameraInfo cameraInfoResultBuf;
    cameraInfoResultBuf.Copy( *iStorage->iContextServer );
    EUNIT_ASSERT( cameraInfoResultBuf().iIndex == 1 );
    EUNIT_ASSERT( cameraInfoResultBuf().iInfo.iMinZoom == 0 );

//EMceItcSetZoomFactor          
    ids.iMsgType = EMceItcMsgTypeWriteString;
    iStorage->iMediaManagerAction = CMCETls::ENone;
    TMceItcArgTInt setZoomFactor( 0 );
    iStorage->StoreL( ids, setZoomFactor );
    ses->ServiceL( ids, EMceItcSetZoomFactor, msg );
    EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::ESetZoomFactor );

//EMceItcSetDigitalZoomFactor   
    ids.iMsgType = EMceItcMsgTypeWriteString;
    iStorage->iMediaManagerAction = CMCETls::ENone;
    TMceItcArgTInt setDigitalZoomFactor( 0 );
    iStorage->StoreL( ids, setDigitalZoomFactor );
    ses->ServiceL( ids, EMceItcSetDigitalZoomFactor, msg );
    EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::ESetDigitalZoomFactor );

//EMceItcSetContrast            
    ids.iMsgType = EMceItcMsgTypeWriteString;
    iStorage->iMediaManagerAction = CMCETls::ENone;
    TMceItcArgTInt setContrast( 0 );
    iStorage->StoreL( ids, setContrast );
    ses->ServiceL( ids, EMceItcSetContrast, msg );
    EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::ESetContrast );

//EMceItcSetBrightness          
    ids.iMsgType = EMceItcMsgTypeWriteString;
    iStorage->iMediaManagerAction = CMCETls::ENone;
    TMceItcArgTInt setBrightness( 0 );
    iStorage->StoreL( ids, setBrightness );
    ses->ServiceL( ids, EMceItcSetBrightness, msg );
    EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::ESetBrightness );
    
//EMceItcSetExposure            
    ids.iMsgType = EMceItcMsgTypeWriteString;
    iStorage->iMediaManagerAction = CMCETls::ENone;
    TMceItcArgTInt setExposure( 0 );
    iStorage->StoreL( ids, setExposure );
    ses->ServiceL( ids, EMceItcSetExposure, msg );
    EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::ESetExposure );
    
//EMceItcSetWhiteBalance        
    ids.iMsgType = EMceItcMsgTypeWriteString;
    iStorage->iMediaManagerAction = CMCETls::ENone;
    TMceItcArgTInt setWhiteBalance( 0 );
    iStorage->StoreL( ids, setWhiteBalance );
    ses->ServiceL( ids, EMceItcSetWhiteBalance, msg );
    EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::ESetWhiteBalance );

    
//EMceItcIsDtmfActive        
    ids.iMsgType = KMceNotAssigned;
    iStorage->iMediaManagerAction = CMCETls::ENone;
    TMceItcArgTInt dtmfActive( 0 );
    iStorage->StoreL( ids, dtmfActive );
    ses->ServiceL( ids, EMceItcIsDtmfActive, msg );
    EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::EDtmf );
    
//EMceItcStartDtmfTone        
    ids.iMsgType = KMceNotAssigned;
    iStorage->iMediaManagerAction = CMCETls::ENone;
    iStorage->StoreL( ids, dtmfActive );
    ses->ServiceL( ids, EMceItcStartDtmfTone, msg );
    EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::EDtmf );
    
//EMceItcStopDtmfTone        
    ids.iMsgType = KMceNotAssigned;
    iStorage->iMediaManagerAction = CMCETls::ENone;
    iStorage->StoreL( ids, dtmfActive );
    ses->ServiceL( ids, EMceItcStopDtmfTone, msg );
    EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::EDtmf );
    
//EMceItcSendDtmfTone        
    ids.iMsgType = KMceNotAssigned;
    iStorage->iMediaManagerAction = CMCETls::ENone;
    iStorage->StoreL( ids, dtmfActive );
    ses->ServiceL( ids, EMceItcSendDtmfTone, msg );
    EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::EDtmf );
    
//EMceItcSendDtmfToneSequence        
    ids.iMsgType = EMceItcMsgTypeWriteString;
    iStorage->iMediaManagerAction = CMCETls::ENone;
    TBuf8<5> teststr;
    teststr.FillZ();
    iStorage->StoreL( ids, teststr );
    ses->ServiceL( ids, EMceItcSendDtmfToneSequence, msg );
    EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::EDtmf );
    
//EMceItcCancelSendDtmfToneSequence        
    ids.iMsgType = KMceNotAssigned;
    iStorage->iMediaManagerAction = CMCETls::ENone;
    ses->ServiceL( ids, EMceItcCancelSendDtmfToneSequence, msg );
    EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::EDtmf );
    
    ids.iMsgType = EMceItcMsgTypeWriteString;
    ids.iMediaID = inStream->iID;
    ids.iSinkID = KMceMediaIdNotAssigned;
    ids.iSourceID = rtpSource->iID;
    
//EMceItcEnableInactivityTimer,
    iStorage->iMediaManagerAction = CMCETls::ENone;
    ses->ServiceL( ids, EMceItcEnableInactivityTimer, msg );
    EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::EStartInactivityTimer );

//EMceItcDisableInactivityTimer
    ids.iMsgType = KMceNotAssigned;
    iStorage->iMediaManagerAction = CMCETls::ENone;
    ses->ServiceL( ids, EMceItcDisableInactivityTimer, msg );
    EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::EStopInactivityTimer );
    
   
// EMceItcSendRR
    iStorage->iMediaManagerAction = CMCETls::ENone;
    ses->ServiceL( ids, EMceItcSendRR, msg );
    EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::ESendRR );
    
//fails
    iStorage->StoreL( iIds, *msgLines, NULL );
    iIds.iMsgType = msgLines->Type();
    EUNIT_ASSERT_LEAVE( ses->ServiceL( iIds, EMceItcFCSetMediaLines, msg ) );
    
	CFCSession* fcSession = NULL;
    
//EMceItcInitFC
    delete iStorage->iContextClient;
    iStorage->iContextClient = NULL;
    iStorage->iContextClient = _L8("TBCP").AllocL();
    iIds.iMsgType = EMceItcMsgTypeReadString;
    iIds.Set( KMceIdsIndexFCPort, 0 );
    
    EUNIT_ASSERT( ses->iFCSession == NULL );
    ses->iFCSession = CFCSession::NewL( NULL, 0, 0 );
    EUNIT_ASSERT_LEAVE( ses->ServiceL( iIds, EMceItcInitFC, msg ) );
    EUNIT_ASSERT_LEAVE( iSipSession->FCActions().InitializeFCL() );
    
    delete ses->iFCSession;
    ses->iFCSession = NULL;
    delete iStorage->iContextClient;
    iStorage->iContextClient = NULL;
    iStorage->iContextClient = _L8("TBCP").AllocL();
    iIds.iMsgType = EMceItcMsgTypeWriteString;
    
    ses->ServiceL( iIds, EMceItcInitFC, msg );
    
//EMceItcFCSetMediaLines
    MCE_DELETE( iStorage->iContextServer );
    iStorage->StoreL( iIds, *msgLines, NULL );
    iIds.iMsgType = msgLines->Type();
    ses->ServiceL( iIds, EMceItcFCSetMediaLines, msg );

        
//EMceItcFCGetMediaLines

    EUNIT_ASSERT( iStorage->iContextServer == NULL );
    iIds.iMsgType = KMceNotAssigned;
    ses->ServiceL( iIds, EMceItcFCGetMediaLines, msg );
    EUNIT_ASSERT( iStorage->iContextServer != NULL );

//EMceItcSendFCMsg

    delete iStorage->iContextClient;
    iStorage->iContextClient = NULL;
    iStorage->iContextClient = _L8("data").AllocL();
    iIds.iMsgType = EMceItcMsgTypeReadString;

    fcSession = ses->iFCSession;ses->iFCSession = NULL;
    EUNIT_ASSERT_LEAVE( ses->ServiceL( iIds, EMceItcSendFCMsg, msg ) );
    
    ses->iFCSession = fcSession;
    iSipSession->iNewBodyCandidate = iSipSession->iBody;
    iSipSession->iBody = NULL;
    EUNIT_ASSERT_LEAVE( ses->ServiceL( iIds, EMceItcSendFCMsg, msg ) );
    
    iSipSession->iBody = iSipSession->iNewBodyCandidate;
    iSipSession->iNewBodyCandidate = NULL;
    iIds.iMsgType = EMceItcMsgTypeWriteString;
    
    ses->ServiceL( iIds, EMceItcSendFCMsg, msg );

//EMceItcUpdateFC

    iIds.iMsgType = EMceItcMsgTypeWriteString;
    iIds.Set( KMceIdsIndexFCPort, 5010 );
    ses->ServiceL( iIds, EMceItcUpdateFC, msg );
    delete iStorage->iContextClient;
    iStorage->iContextClient = NULL;
    iStorage->iContextClient = _L8("stuff").AllocL();
    iIds.Set( KMceIdsIndexFCPort, 5010 );

    fcSession = ses->iFCSession;ses->iFCSession = NULL;
    EUNIT_ASSERT_LEAVE( ses->ServiceL( iIds, EMceItcUpdateFC, msg ) );
    
    ses->iFCSession = fcSession;
    ses->ServiceL( iIds, EMceItcUpdateFC, msg );
    
    CleanupStack::PopAndDestroy( clientSessionMsg );
    CleanupStack::PopAndDestroy( clientSession );    
    CleanupStack::PopAndDestroy( msgLines );
    CleanupStack::PopAndDestroy( medialines );
    
    
//failed
    iIds.iMsgType = KMceNotAssigned;
    EUNIT_ASSERT_LEAVE( ses->ServiceL( iIds, EMceItcSendFCMsg, msg ) );
    
    HBufC8* returnMessage = NULL;
    
    CMceMsgBase* msgbase = NULL;
    EUNIT_ASSERT_LEAVE( ses->DoServiceL( iIds, EMceItcSetAppUid, *returnMessage ) );
    EUNIT_ASSERT_LEAVE( ses->DoServiceL( iIds, EMceItcSetAppUid, KNullDesC8 ) );
    EUNIT_ASSERT_LEAVE( ses->DoServiceL( iIds, EMceItcSetAppUid, *msgbase ) );      
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_FCTestsL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_FCTestsL()
    {
    iSipSession->iOffer = CSdpDocument::NewL();
    iSipSession->iBody = MCETestHelper::ComSessionL( 1 );
    
//UpdateFCOfferL + UpdateFCAnswerL + StartFCL

    iSipSession->FCActions().UpdateFCOfferL( *iSipSession->iOffer );
    iSipSession->FCActions().UpdateFCAnswerL( *iSipSession->iOffer );
    iSipSession->FCActions().StartFCL();
    
    
    iStorage->iContextClient = _L8("TBCP").AllocL();
    iIds.iMsgType = EMceItcMsgTypeReadString;
    iIds.Set( KMceIdsIndexFCPort, 0 );
    iSipSession->FCActions().InitializeFCL();
    
    iSipSession->iUseFC = EFalse;
    iSipSession->FCActions().UpdateFCOfferL( *iSipSession->iOffer );
    iSipSession->FCActions().UpdateFCAnswerL( *iSipSession->iOffer );
    iSipSession->FCActions().StartFCL();
    
    
    iSipSession->iUseFC = ETrue;
    iSipSession->iNewBodyCandidate = iSipSession->iBody;    
    iSipSession->iBody = NULL;
    EUNIT_ASSERT_LEAVE( iSipSession->FCActions().UpdateFCOfferL( *iSipSession->iOffer ) );
    EUNIT_ASSERT_LEAVE( iSipSession->FCActions().UpdateFCAnswerL( *iSipSession->iOffer ) );
    iSipSession->iBody = iSipSession->iNewBodyCandidate;
    iSipSession->iNewBodyCandidate = NULL;
    
    CMceComMediaStream* media = iSipSession->iBody->Streams()[ 0 ];
    CleanupStack::PushL( media );
    iSipSession->iBody->Streams().Remove( 0 );
    EUNIT_ASSERT_LEAVE( iSipSession->FCActions().UpdateFCOfferL( *iSipSession->iOffer ) );
    EUNIT_ASSERT_LEAVE( iSipSession->FCActions().UpdateFCAnswerL( *iSipSession->iOffer ) );
    
    iSipSession->iBody->Streams().AppendL( media );
    CleanupStack::Pop( media );
    
    iSipSession->FCActions().UpdateFCOfferL( *iSipSession->iOffer );
    iSipSession->FCActions().UpdateFCAnswerL( *iSipSession->iOffer );
    iSipSession->FCActions().StartFCL();
    iSipSession->FCActions().SendFCMsgL( _L8("data") );
    
    
//RTCPPort   
    iSipSession->iNewBodyCandidate = iSipSession->iBody;
    EUNIT_ASSERT( iSipSession->FCActions().RTCPPort() > 0 );
    iSipSession->iNewBodyCandidate = NULL;
    EUNIT_ASSERT( iSipSession->FCActions().RTCPPort() > 0 );    
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_CurrentStateL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_CurrentStateL()
    {
    CMceSipManager& manager = iServerCore->Manager();
    TUint32 profileId = 1;
    CSIPProfile& profile = manager.ProfileL( profileId );
    
    CMceSipSession* session = CMceSipSession::NewLC( *iSession,
                                                    iSipSession->SIPConnection(),
                                                    *iSipSession->iStateMachine,
                                                    profile );
                                                    
    session->iBody = MCETestHelper::ComSessionL( 1 );
    
    EUNIT_ASSERT( session->CurrentState().Id() == KMceStateIdle );
    //CloseSessionL
    iStorage->SetLeave( KErrNotFound );
    CleanupStack::PopAndDestroy( session );
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_BodyL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_BodyL()
    {
    EUNIT_ASSERT( iSipSession->Body() == NULL );
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_ClearRemoteUriL
// ---------------------------------------------------------------------------
// 
void UT_CMceSipSession::UT_CMceSipSession_ClearRemoteUriL()
    {
    TMceStateIndex nextState = KMceStateEstablished;
    iSipSession->ClearRemoteUri( nextState );
    
    nextState = KMceStateTerminated;
    iSipSession->ClearRemoteUri( nextState );

    nextState = KMceStateUpdated;
    iSipSession->ClearRemoteUri( nextState );
    
    nextState = KMceStateOffering;
    iSipSession->ClearRemoteUri( nextState );
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_FindRemoteUriL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_FindRemoteUriL()
    {
    iSipSession->ClearRemoteUri( KMceStateTerminated );
    TUriParser8 uri8Parser;
    User::LeaveIfError( uri8Parser.Parse( _L8("sip:n@172.21.11.69") ) );
    CUri8* remoteUriOne = CUri8::NewLC( uri8Parser );
    MCE_EUNIT_ASSERT_NO_LEAVE( iSipSession->AddRemoteUriL( *remoteUriOne ) );
    CleanupStack::PopAndDestroy( remoteUriOne );  
    EUNIT_ASSERT( iSipSession->iRemoteUri.Count() == 1 );
 
    User::LeaveIfError( uri8Parser.Parse( _L8("sip:w@172.21.11.69") ) );
    CUri8* remoteUriTwo = CUri8::NewLC( uri8Parser );
    EUNIT_ASSERT( iSipSession->FindRemoteUri( *remoteUriTwo ) == KErrNotFound );
    CleanupStack::PopAndDestroy( remoteUriTwo );  
    
    User::LeaveIfError( uri8Parser.Parse( _L8("sip:n@172.21.11.69") ) );
    CUri8* remoteUriThree = CUri8::NewLC( uri8Parser );
    EUNIT_ASSERT( iSipSession->FindRemoteUri( *remoteUriThree ) == 0 );
    CleanupStack::PopAndDestroy( remoteUriThree );  
    
    EUNIT_ASSERT( iSipSession->iRemoteUri.Count() == 1 );
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_AddRemoteUriL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_AddRemoteUriL()
    {
    iSipSession->ClearRemoteUri( KMceStateTerminated );

    TUriParser8 uri8Parser;
    User::LeaveIfError( uri8Parser.Parse( _L8("sip:n@172.21.11.69") ) );
    CUri8* remoteUriOne = CUri8::NewLC( uri8Parser );
    MCE_EUNIT_ASSERT_NO_LEAVE( iSipSession->AddRemoteUriL( *remoteUriOne ) );
    CleanupStack::PopAndDestroy( remoteUriOne );   
    EUNIT_ASSERT( iSipSession->iRemoteUri.Count() == 1 );
    
    User::LeaveIfError( uri8Parser.Parse( _L8("sip:n@172.21.11.69") ) );
    CUri8* remoteUriTwo = CUri8::NewLC( uri8Parser );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iSipSession->AddRemoteUriL( *remoteUriTwo ), KErrAlreadyExists );
    CleanupStack::PopAndDestroy( remoteUriTwo ); 
    EUNIT_ASSERT( iSipSession->iRemoteUri.Count() == 1 );

    User::LeaveIfError( uri8Parser.Parse( _L8("sip:w@172.21.11.69") ) );
    CUri8* remoteUriThree = CUri8::NewLC( uri8Parser );
    MCE_EUNIT_ASSERT_NO_LEAVE( iSipSession->AddRemoteUriL( *remoteUriThree ) );
    CleanupStack::PopAndDestroy( remoteUriThree );    
    EUNIT_ASSERT( iSipSession->iRemoteUri.Count() == 2 );
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_AddRemoteUriFromResponseL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_AddRemoteUriFromResponseL()
    {
    iSipSession->ClearRemoteUri( KMceStateTerminated );

    RStringF phrase = 
	    SIPStrings::Pool().OpenFStringL( _L8("SIP/2.0 302 Removed Temporarily") );
	CleanupClosePushL( phrase );
    
    CSIPResponseElements* response = 
        CSIPResponseElements::NewL( 302, phrase );
    CleanupStack::PopAndDestroy(); // phrase
    CleanupStack::PushL( response );
    
    CSIPMessageElements& responseMessageElements =
            response->MessageElements();

    CSIPContactHeader* contact = MceSip::ToContactHeaderL(_L8("Contact:n@10.162.26.51:5060"));
    CleanupStack::PushL( contact );

    responseMessageElements.AddHeaderL( contact );
    CleanupStack::Pop( contact );

    MCE_EUNIT_ASSERT_NO_LEAVE( iSipSession->AddRemoteUriFromResponseL( *response ) );
 
    CleanupStack::PopAndDestroy( response );
    EUNIT_ASSERT( iSipSession->iRemoteUri.Count() == 1 );
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_CreateDialogFrom3XXResponseL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_CreateDialogFrom3XXResponseL()
    {
    UT_CMceSipSession_AddRemoteUriFromResponseL();
    
    CSIPClientTransaction* clientTransaction =  
		MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite );
                                    
    MCETestHelper::SetResponseL( *clientTransaction, KMceSipExtensionRequired,
    							 SipStrConsts::EPhraseExtensionRequired );
        
    iSipSession->SetPendingTransactionL( clientTransaction );
    iSipSession->SetResponse( *clientTransaction );
    CleanupStack::Pop( clientTransaction );

    MCE_EUNIT_ASSERT_NO_LEAVE( iSipSession->CreateDialogFrom3XXResponseL() );
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_OfferL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_OfferL()
    {
    EUNIT_ASSERT( iSipSession->Offer() == NULL );
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_NextStateL
// ---------------------------------------------------------------------------
//  
void UT_CMceSipSession::UT_CMceSipSession_NextStateL()
    {
    EUNIT_ASSERT( iSipSession->CurrentState().Id() == KMceStateIdle );
    iSipSession->NextState( KMceStateClientEstablishing );
    EUNIT_ASSERT( iSipSession->CurrentState().Id() == KMceStateClientEstablishing );
    iSipSession->NextState( KMceStateTerminated );
    EUNIT_ASSERT( iSipSession->CurrentState().Id() == KMceStateTerminated );
    
    // Automatic event without msg
    TMceIds ids;
    TMceStateTransitionEvent acceptEvent( *iSipSession, EMceItcAcceptSession, ids );
    iSipSession->StoreAutoEventL( &acceptEvent, NULL );
    iSipSession->SetAutoEvent( ETrue );
    iSipSession->NextState( KMceStateTerminated );
    EUNIT_ASSERT( iSipSession->CurrentState().Id() == KMceStateTerminated );
    EUNIT_ASSERT( !iSipSession->AutoEvent() );
    
    // Automatic event with msg
    TMceIds ids2;
    CMceMsgSIPReply* msg = new ( ELeave ) CMceMsgSIPReply();
    CleanupStack::PushL( msg );
    TMceStateTransitionEvent acceptEvent2( *iSipSession, EMceMediaUpdated );
    acceptEvent2.SetParamClientMessage( msg );
    iSipSession->StoreAutoEventL( &acceptEvent2, NULL );
    iSipSession->SetAutoEvent( ETrue );
    iSipSession->NextState( KMceStateTerminated );
    EUNIT_ASSERT( iSipSession->CurrentState().Id() == KMceStateTerminated );
    EUNIT_ASSERT( !iSipSession->AutoEvent() );
    CleanupStack::PopAndDestroy( msg );
    
    // Automatic event with associated request
    iSipSession->NextState( KMceStateIdle );
    CSIPServerTransaction* srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, ETrue, ETrue );
    CleanupStack::PushL( srvtransaction );
    iSipSession->iPendingReceivedRequests.AppendL( srvtransaction );
    CleanupStack::Pop( srvtransaction );
    CSIPServerTransaction* srvtransaction2 = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EPrack, ETrue, ETrue );
    CleanupStack::PushL( srvtransaction2 );
    iSipSession->iPendingReceivedRequests.AppendL( srvtransaction2 );
    CleanupStack::Pop( srvtransaction2 );
    CSIPServerTransaction* initialRequest = &iSipSession->InitialRequest();
    iSipSession->StoreAutoEventL( &acceptEvent, initialRequest );
    iSipSession->SetAutoEvent( ETrue );
    EUNIT_ASSERT( &iSipSession->Request() == initialRequest );
    iSipSession->NextState( KMceStateTerminated );
    EUNIT_ASSERT( !iSipSession->AutoEvent() );
    
    // Automatic event but it does not exist in session
    iSipSession->NextState( KMceStateIdle );
    CSIPServerTransaction* srvtransaction3 = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, ETrue, ETrue );
    CleanupStack::PushL( srvtransaction3 );
    iSipSession->StoreAutoEventL( &acceptEvent, srvtransaction3 );
    iSipSession->SetAutoEvent( ETrue );
    EUNIT_ASSERT( &iSipSession->Request() == srvtransaction2 );
    iSipSession->NextState( KMceStateTerminated );
    EUNIT_ASSERT( !iSipSession->AutoEvent() );
    CleanupStack::PopAndDestroy( srvtransaction3 );
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_InviteTransactionL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_InviteTransactionL()
    {
    EUNIT_ASSERT( iSipSession->InviteTransaction() == NULL );
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_AcceptMethodL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_AcceptMethodL()
    {
    EUNIT_ASSERT( iSipSession->AcceptMethod( SIPStrings::StringF( SipStrConsts::EInvite ) ) );
    EUNIT_ASSERT( iSipSession->AcceptMethod( SIPStrings::StringF( SipStrConsts::ECancel ) ) );
    EUNIT_ASSERT( iSipSession->AcceptMethod( SIPStrings::StringF( SipStrConsts::EAck ) ) );
    EUNIT_ASSERT( iSipSession->AcceptMethod( SIPStrings::StringF( SipStrConsts::EBye ) ) );
    EUNIT_ASSERT( iSipSession->AcceptMethod( SIPStrings::StringF( SipStrConsts::EPrack ) ) );
    EUNIT_ASSERT( iSipSession->AcceptMethod( SIPStrings::StringF( SipStrConsts::EUpdate ) ) );
    EUNIT_ASSERT( iSipSession->AcceptMethod( SIPStrings::StringF( SipStrConsts::EInfo ) ) );
    
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_RequestReceivedL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_RequestReceivedL()
    {
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    
    iSipSession->NextState( KMceStateTerminated );
    
    CSIPServerTransaction* srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EBye );
    CleanupStack::PushL( srvtransaction );
    srvtransaction->iState = CSIPTransactionBase::EProceeding;

	TBool consumed = iSipSession->DialogRequestReceived( srvtransaction, iSipSession->Dialog()->Dialog() );
	MCE_CHECK_MEMORY_LEAVE( !consumed )
	CleanupStack::Pop( srvtransaction );
    EUNIT_ASSERT( &iSipSession->Request() == srvtransaction );   
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_RequestReceived2L
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_RequestReceived2L()
    {
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    
    iSipSession->NextState( KMceStateTerminated );
    
    CSIPServerTransaction* srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInvite );

	CleanupStack::PushL( srvtransaction );
    srvtransaction->iState = CSIPTransactionBase::EProceeding;
	TBool consumed = iSipSession->DialogRequestReceived( srvtransaction, iSipSession->Dialog()->Dialog() );
	MCE_CHECK_MEMORY_LEAVE( !consumed )
    CleanupStack::Pop( srvtransaction );
    EUNIT_ASSERT( !&iSipSession->Request() );


    // Re-invite received before ack is received (allowed, ack is faked)
    CSIPServerTransaction* srvtransaction2 = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInvite );
    
    CleanupStack::PushL( srvtransaction2 );
    srvtransaction->iState = CSIPTransactionBase::EProceeding;
    iSipSession->NextState( KMceStateAnswering );
    
	consumed = iSipSession->DialogRequestReceived( srvtransaction2, iSipSession->Dialog()->Dialog() );
	MCE_CHECK_MEMORY_LEAVE( !consumed )
    CleanupStack::Pop( srvtransaction2 );
    EUNIT_ASSERT( &iSipSession->Request() == srvtransaction2 );
    }


// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_ForkedResponseReceivedL
// ---------------------------------------------------------------------------
//

void UT_CMceSipSession::UT_CMceSipSession_ForkedResponseReceivedL()
    {
         
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    CSIPClientTransaction* clitransaction = NULL;
    
    iSipSession->NextState( KMceStateEstablished );
    
    clitransaction = MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite );
        
    MCETestHelper::SetResponseL( *clitransaction, KMceSipOK, SipStrConsts::EPhraseOk );
        
	iSipSession->SetPendingTransactionL( clitransaction );
    CleanupStack::Pop( clitransaction );
    EUNIT_ASSERT( iSipSession->PendingTransactions().Count() == 1 );
    
        // create dialog
    CUri8* uri = CUri8::NewLC(); 
    MCE_SET_URIL( uri );
  	
    CSIPFromHeader* fromheader = CSIPFromHeader::DecodeL(_L8("User <user2@host>")); 
    CleanupStack::PushL( fromheader );

    CSIPInviteDialogAssoc* dialog = 
        CSIPInviteDialogAssoc::NewL( iSipSession->SIPConnection().Connection(),
                                     fromheader, uri );
    CleanupStack::Pop( fromheader );
    CleanupStack::Pop( uri );
    CleanupStack::PushL( dialog );

    iSipSession->InviteForkedL( *clitransaction, dialog );
    CleanupStack::Pop( dialog );
    
    iSipSession->InviteCompleted( *clitransaction );
    
    EUNIT_ASSERT( iSipSession->iForkedDialogs.Count() == 0 );	 
    }


// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_ResponseReceivedL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_ResponseReceivedL()
    {
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    CSIPClientTransaction* clitransaction1 = NULL;
    CSIPClientTransaction* clitransaction2 = NULL;
    
    iSipSession->NextState( KMceStateTerminated );
    
    //sends BYE
    clitransaction1 = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EBye );
        
    MCETestHelper::SetResponseL( *clitransaction1, KMceSipOK, SipStrConsts::EPhraseOk );
        
	iSipSession->SetPendingTransactionL( clitransaction1 );
    CleanupStack::Pop( clitransaction1 );
    EUNIT_ASSERT( iSipSession->PendingTransactions().Count() == 1 );

    
    clitransaction2 = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EBye );
	iSipSession->SetPendingTransactionL( clitransaction2 );
    CleanupStack::Pop( clitransaction2 );
    EUNIT_ASSERT( iSipSession->PendingTransactions().Count() == 2 );
    
    
    //200OK is received
    MCETestHelper::SetResponseL( *clitransaction2, KMceSipOK, SipStrConsts::EPhraseOk );
    

	iSipSession->DialogResponseReceived( *clitransaction1 );
    EUNIT_ASSERT( &iSipSession->Response() == clitransaction1 );
    EUNIT_ASSERT( iSipSession->PendingTransactions().Count() == 2 );
	
    clitransaction1->iState = CSIPTransactionBase::ECompleted;
	iSipSession->DialogResponseReceived( *clitransaction1 );
    EUNIT_ASSERT( iSipSession->PendingTransactions().Count() == 1 );
    EUNIT_ASSERT( &iSipSession->Response() == clitransaction1 );

	iSipSession->DialogResponseReceived( *clitransaction2 );
    EUNIT_ASSERT( iSipSession->PendingTransactions().Count() == 1 );
    EUNIT_ASSERT( &iSipSession->Response() == clitransaction2 );

    clitransaction2->iState = CSIPTransactionBase::ECompleted;
	iSipSession->DialogResponseReceived( *clitransaction2 );
    EUNIT_ASSERT( iSipSession->PendingTransactions().Count() == 0 );
    EUNIT_ASSERT( &iSipSession->Response() == clitransaction2 );  
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_ProvisionalResponseReceivedL
// ---------------------------------------------------------------------------
//  
void UT_CMceSipSession::UT_CMceSipSession_ProvisionalResponseReceivedL()
    {

    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    iSipSession->NextState( KMceStateTerminated );
    
    CSIPClientTransaction* clitransaction = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite );
	iSipSession->SetPendingTransactionL( clitransaction );
    CleanupStack::Pop( clitransaction );

    MCETestHelper::SetResponseL( *clitransaction, KMceSipRinging, SipStrConsts::EPhraseRinging );
        
	iSipSession->DialogResponseReceived( *clitransaction );
    EUNIT_ASSERT( &iSipSession->Response() == clitransaction );
    
    //fails
    iSipSession->NextState( KMceStateIdle );
	iSipSession->DialogResponseReceived( *clitransaction );
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_RedirectionResponseReceivedL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_RedirectionResponseReceivedL()
    {
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    iSipSession->NextState( KMceStateTerminated );
    
    CSIPClientTransaction* clitransaction = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, KMceSipUseProxy, 
                                            SipStrConsts::EPhraseRinging );
                                            
	iSipSession->SetPendingTransactionL( clitransaction );
    CleanupStack::Pop( clitransaction );
    
	iSipSession->DialogResponseReceived( *clitransaction );
    EUNIT_ASSERT( &iSipSession->Response() == clitransaction );

    //fails
    iSipSession->NextState( KMceStateIdle );
	iSipSession->DialogResponseReceived( *clitransaction );
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_ErrorResponseReceivedL
// ---------------------------------------------------------------------------
//  
void UT_CMceSipSession::UT_CMceSipSession_ErrorResponseReceivedL()
    {
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    iSipSession->NextState( KMceStateTerminated );
    
    CSIPClientTransaction* clitransaction = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EBye );
    
	iSipSession->SetPendingTransactionL( clitransaction );
    CleanupStack::Pop( clitransaction );
    
    
    //400
    MCETestHelper::SetResponseL( *clitransaction, KMceSipRequestTimeout,
    							 SipStrConsts::EPhraseRequestTimeout );
    
	iSipSession->DialogResponseReceived( *clitransaction );
    EUNIT_ASSERT( &iSipSession->Response() == clitransaction );

    //500
    MCETestHelper::SetResponseL( *clitransaction, KMceSipServerInternalError,
    							 SipStrConsts::EPhraseServerInternalError );
    
	iSipSession->DialogResponseReceived( *clitransaction );
    EUNIT_ASSERT( &iSipSession->Response() == clitransaction );

    //600
    MCETestHelper::SetResponseL( *clitransaction, KMceSipDecline,
    							 SipStrConsts::EPhraseDecline );

	iSipSession->DialogResponseReceived( *clitransaction );
    EUNIT_ASSERT( &iSipSession->Response() == clitransaction );
    
    //fails
    iSipSession->NextState( KMceStateIdle );
	iSipSession->DialogResponseReceived( *clitransaction );
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_StandAloneRequestReceivedL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_StandAloneRequestReceivedL()
    {
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    CSIPServerTransaction* srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInvite );
	CleanupStack::PushL( srvtransaction );
	TBool consumed = iSipSession->MessageRequestReceived( srvtransaction );
	MCE_CHECK_MEMORY_LEAVE( !consumed )
	CleanupStack::Pop( srvtransaction );
    EUNIT_ASSERT( &iSipSession->Request() == srvtransaction );
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_StandAloneResponseReceivedL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_StandAloneResponseReceivedL()
    {
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    CSIPClientTransaction* clitransaction = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::ECancel );
        
	iSipSession->SetPendingTransactionL( clitransaction );
    CleanupStack::Pop( clitransaction );
    
    MCETestHelper::SetResponseL( *clitransaction, KMceSipOK,
    							 SipStrConsts::EPhraseOk );
    
	iSipSession->MessageResponseReceived( *clitransaction );
    EUNIT_ASSERT( &iSipSession->Response() == clitransaction );
    
    clitransaction->iState = CSIPTransactionBase::ECompleted;
    
    CSIPClientTransaction* clitransaction1 = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EInfo );
        
	iSipSession->SetPendingTransactionL( clitransaction1 );
    CleanupStack::Pop( clitransaction1 );


    MCETestHelper::SetResponseL( *clitransaction1, KMceSipOK,
    							 SipStrConsts::EPhraseOk );
    
	iSipSession->MessageResponseReceived( *clitransaction1 );
    EUNIT_ASSERT( &iSipSession->Response() == clitransaction1 );
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_DoInitializeIncomingDialogLL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_DoInitializeIncomingDialogLL()
    {
    RPointerArray<CSIPHeaderBase> sipHeaders;
    CSIPHeaderBase::PushLC( &sipHeaders );

    // Supported: timer 
    RStringF timerKey = SIPStrings::Pool().OpenFStringL( KMceSipTimer() );
	CleanupClosePushL( timerKey );
    
    CSIPSupportedHeader* supportedHeader = CSIPSupportedHeader::NewLC( timerKey );
    
    User::LeaveIfError( sipHeaders.Append( supportedHeader ) );
    CleanupStack::Pop( supportedHeader );
    CleanupStack::PopAndDestroy();//timerKey
    
    // Session-Expires: 10
    TUint timeoutInSec = 10000 / KMillisecondToSecond;
    
    HBufC8* timeout = HBufC8::NewLC( KMaxLengthOfSessionExpires );
    timeout->Des().Num( timeoutInSec, EDecimal );
    				
    CSIPExtensionHeader* sessionExpiresHeader = 
    						CSIPExtensionHeader::NewLC( KSessionExpires(), *timeout );
    User::LeaveIfError( sipHeaders.Append( sessionExpiresHeader ) );
    CleanupStack::Pop( sessionExpiresHeader );
    CleanupStack::PopAndDestroy( timeout );

    CSIPServerTransaction* srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, sipHeaders, ETrue );
    CleanupStack::PopAndDestroy( &sipHeaders );
    CleanupStack::PushL( srvtransaction );
    
    iSipSession->DoInitializeIncomingDialogL( *srvtransaction );
    CleanupStack::PopAndDestroy( srvtransaction );
    
    CMceState& state =  iSipSession->CurrentState();
    CMceComSession* session = iSipSession->Body();
    
    EUNIT_ASSERT( state.Id() == KMceStateIdle );
    
    EUNIT_ASSERT(session->iType == CMceComSession::EInSession );
    EUNIT_ASSERT(session->iIapId == 1 );
    EUNIT_ASSERT(iSipSession->iOffer == NULL );
    EUNIT_ASSERT(session->iState == CMceSession::EIncoming );
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_ActionsL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_ActionsL()
    {
    CMceSipManager& manager = iServerCore->Manager();
    TUint32 profileId = 1;
    CSIPProfile& profile = manager.ProfileL( profileId );
    
    CMceSipSession* session = CMceSipSession::NewL( *iSession, 
    												iSipSession->iSIPConnection, 
    												*iSipSession->iStateMachine,
    												profile );
    CleanupStack::PushL( session );
    session->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    
    TMceActionSet actions = session->Actions();
    EUNIT_ASSERT( &actions.iSession == session );
    
    CleanupStack::PopAndDestroy( session );    
    }    

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_OnExpiredL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_OnExpiredL()
    {
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    
    iStorage->iSipSentMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    
    TAny* params = NULL;
    TMceTimerId timerId = 1;
    TMceTimerId pendingTimerId = 2;
    iSipSession->iPendingTimer = pendingTimerId;
    
    //fails
    iSipSession->iBody->iRefresh = ETrue;
    iSipSession->iTimer = timerId;
    iSipSession->TimerExpiredL( timerId, params );
    
    EUNIT_ASSERT ( iStorage->iSipSentMethod == SIPStrings::StringF( SipStrConsts::EEmpty ) );
    EUNIT_ASSERT ( iSipSession->iTimer == MMceTimerManager::KNoSuchTimer );
    EUNIT_ASSERT ( iSipSession->iPendingTimer == pendingTimerId );
    
    //fails
    iSipSession->iBody->iRefresh = EFalse;
    iSipSession->iTimer = timerId;
    iSipSession->TimerExpiredL( timerId, params );
    EUNIT_ASSERT ( iSipSession->iTimer == MMceTimerManager::KNoSuchTimer );
    EUNIT_ASSERT ( iSipSession->iPendingTimer == pendingTimerId );
    

    EUNIT_ASSERT ( iStorage->iSipSentMethod == SIPStrings::StringF( SipStrConsts::EEmpty ) );
    
    // pending timer expires
    iSipSession->NextState( KMceStateEstablished );
    
    iSipSession->iBody->iRefresh = ETrue;
    iSipSession->iTimer = timerId;
    iSipSession->iNewBodyCandidate = CMceComSession::NewL( CMceComSession::EOutSession );
    iSipSession->TimerExpiredL( pendingTimerId, params );
    MCE_RECEIVE_QUEUE_POSTCONDITION( iSipSession )

    EUNIT_ASSERT ( iStorage->iSipSentMethod == SIPStrings::StringF( SipStrConsts::EInvite ) );
    EUNIT_ASSERT ( iSipSession->iTimer == timerId );
    EUNIT_ASSERT ( iSipSession->iPendingTimer == pendingTimerId );
    
    iStorage->iSipSentMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
    
    // session timer expires
    iSipSession->NextState( KMceStateEstablished );
    
    iSipSession->iBody->iRefresh = ETrue;
    iSipSession->iTimer = timerId;
    iSipSession->TimerExpiredL( timerId, params );
    MCE_RECEIVE_QUEUE_POSTCONDITION( iSipSession )

    EUNIT_ASSERT ( iStorage->iSipSentMethod == SIPStrings::StringF( SipStrConsts::EInvite ) );
    EUNIT_ASSERT ( iSipSession->iTimer == MMceTimerManager::KNoSuchTimer );
    EUNIT_ASSERT ( iSipSession->iPendingTimer == pendingTimerId );
    
    
    iSipSession->NextState( KMceStateEstablished );
    iSipSession->iBody->iRefresh = EFalse;
    iSipSession->iTimer = timerId;
    iSipSession->TimerExpiredL( timerId, params );
    MCE_RECEIVE_QUEUE_POSTCONDITION( iSipSession )

    EUNIT_ASSERT ( iSipSession->iTimer == MMceTimerManager::KNoSuchTimer );
    EUNIT_ASSERT ( iSipSession->iPendingTimer == pendingTimerId );
    
    EUNIT_ASSERT ( iStorage->iSipSentMethod == SIPStrings::StringF( SipStrConsts::EBye ) );
    EUNIT_ASSERT ( iStorage->iMediaManagerAction == CMCETls::ECloseSession );
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_CanDisposeL
// ---------------------------------------------------------------------------
//    
void UT_CMceSipSession::UT_CMceSipSession_CanDisposeL()
    {
    EUNIT_ASSERT( !iSipSession->CanDispose() );
    iSipSession->NextState( KMceStateTerminated );
    EUNIT_ASSERT( iSipSession->CanDispose() );
    }
    
// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_ConnectionStateChangedL
// ---------------------------------------------------------------------------
//  
void UT_CMceSipSession::UT_CMceSipSession_ConnectionStateChangedL()
    {
    iSipSession->ConnectionStateChanged( CSIPConnection::ESuspended );
    }
    
// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_CreateOutgoingDialogL
// ---------------------------------------------------------------------------
//  
void UT_CMceSipSession::UT_CMceSipSession_CreateOutgoingDialogL()
    {
    CDesC8Array* array = NULL;
    CSIPDialog* dialog = NULL;
    
    EUNIT_ASSERT_LEAVE( iSipSession->CreateOutgoingDialogL(
    	KMceDlgTypeRefer, *array ) );
    EUNIT_ASSERT_LEAVE( iSipSession->CreateOutgoingDialogL(
    	KMceDlgTypeRefer, *array, *dialog ) );

    dialog = CSIPDialog::NewL();
    CleanupStack::PushL( dialog );
    CSIPDialogAssocBase* dlg = iSipSession->CreateOutgoingDialogL(
    	KMceDlgTypeInvite, *array, *dialog );
    CleanupStack::PopAndDestroy( dialog );
    delete dlg;
    }    

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_ReCreateOutgoingDialogL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_ReCreateOutgoingDialogL()
    {
    CSIPClientTransaction* clientTransaction =  
		MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite );
                                    
    MCETestHelper::SetResponseL( *clientTransaction, KMceSipExtensionRequired,
    							 SipStrConsts::EPhraseExtensionRequired );

    iSipSession->SetPendingTransactionL( clientTransaction );
    iSipSession->SetResponse( *clientTransaction );
    CleanupStack::Pop( clientTransaction );
        
    iSipSession->ReCreateOutgoingDialogL(); 
             
    //Create Client Transaction and Msg Elements.
    iSipSession->ReCreateOutgoingDialogL();        
    }    

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_SessionClosedL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_SessionClosedL()
    {
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    iSipSession->NextState( KMceStateTerminated );
    iSipSession->SessionClosed( *iSipSession->iBody );
    
    //fails
    iSipSession->NextState( KMceStateIdle );
    iSipSession->SessionClosed( *iSipSession->iBody );
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_EventReceivedL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_EventReceivedL()
    {
    CMceComMediaSink* sink = NULL;
    CMceComMediaSource* source = NULL;
    iSipSession->iBody = MCETestHelper::ComSessionL( 0 );
    iSipSession->NextState( KMceStateTerminated );

    TMceMccComEvent sinkEvent( KMccStreamPaused );
    MCETestHelper::SinkEvent( sinkEvent, iSipSession );
    
    TMceMccComEvent sourceEvent( KMccStreamPaused );
    MCETestHelper::SourceEvent( sourceEvent, iSipSession );
    
    //fails
    iSipSession->NextState( KMceStateIdle );
    iSipSession->EventReceived( sinkEvent );

    iSipSession->NextState( KMceStateEstablished );
    iSipSession->EventReceived( sinkEvent );
    //fails
    iSipSession->NextState( KMceStateIdle );
    iSipSession->EventReceived( sourceEvent );

    iSipSession->NextState( KMceStateEstablished );
    iSipSession->EventReceived( sourceEvent );

    sinkEvent.iEvent = KMccStreamResumed;
    MCETestHelper::SinkEvent( sinkEvent, iSipSession );
    sourceEvent.iEvent = KMccStreamResumed;
    MCETestHelper::SourceEvent( sourceEvent, iSipSession );

    //fails
    iSipSession->NextState( KMceStateIdle );
    iSipSession->EventReceived( sinkEvent );

    iSipSession->NextState( KMceStateEstablished );
    iSipSession->EventReceived( sinkEvent );

    //fails
    iSipSession->NextState( KMceStateIdle );
    iSipSession->EventReceived( sourceEvent );

    iSipSession->NextState( KMceStateEstablished );
    iSipSession->EventReceived( sourceEvent );


    sinkEvent.iEvent = KMccInactivityEvent;
    MCETestHelper::SinkEvent( sinkEvent, iSipSession );
    sourceEvent.iEvent = KMccInactivityEvent;
    MCETestHelper::SourceEvent( sourceEvent, iSipSession );

    //fails
    iSipSession->NextState( KMceStateIdle );
    iSipSession->EventReceived( sinkEvent );

    iSipSession->NextState( KMceStateEstablished );
    iSipSession->EventReceived( sinkEvent );

    //fails
    iSipSession->NextState( KMceStateIdle );
    iSipSession->EventReceived( sourceEvent );

    iSipSession->NextState( KMceStateEstablished );
    iSipSession->EventReceived( sourceEvent );

    sinkEvent.iEvent = KMccRtcpReceived;
    sinkEvent.iEventSubType = KRtcpSrPacket;
    MCETestHelper::SinkEvent( sinkEvent, iSipSession );
    
    sourceEvent.iEvent = KMccRtcpReceived;
    sourceEvent.iEventSubType = KRtcpSrPacket;
    MCETestHelper::SourceEvent( sourceEvent, iSipSession );

    //fails
    iSipSession->NextState( KMceStateIdle );
    iSipSession->EventReceived( sinkEvent );

    iSipSession->NextState( KMceStateEstablished );
    iSipSession->EventReceived( sinkEvent );
    
    //fails
    iSipSession->NextState( KMceStateIdle );
    iSipSession->EventReceived( sourceEvent );

    iSipSession->NextState( KMceStateEstablished );
    iSipSession->EventReceived( sourceEvent );

    sinkEvent.iEvent = KMccRtcpReceived;
    sinkEvent.iEventSubType = KRtcpRrPacket;
    MCETestHelper::SinkEvent( sinkEvent, iSipSession );
    
    sourceEvent.iEvent = KMccRtcpReceived;
    sourceEvent.iEventSubType = KRtcpRrPacket;
    MCETestHelper::SourceEvent( sourceEvent, iSipSession );
    
    //fails
    iSipSession->NextState( KMceStateIdle );
    iSipSession->EventReceived( sinkEvent );

    iSipSession->NextState( KMceStateEstablished );
    iSipSession->EventReceived( sinkEvent );
    
    //fails
    iSipSession->NextState( KMceStateIdle );
    iSipSession->EventReceived( sourceEvent );

    iSipSession->NextState( KMceStateEstablished );
    iSipSession->EventReceived( sourceEvent );


    const TDesC8& data = _L8("FC DATA");
    sinkEvent.iEvent = KMccRtcpReceived;
    sinkEvent.iEventSubType = KRtcpPacketUndefined;
    sinkEvent.iItcDataDesC = &data;
    MCETestHelper::SinkEvent( sinkEvent, iSipSession );
    
    sourceEvent.iEvent = KMccRtcpReceived;
    sourceEvent.iEventSubType = KRtcpPacketUndefined;
    sourceEvent.iItcDataDesC = &data;
    MCETestHelper::SourceEvent( sourceEvent, iSipSession );
    

    //fails
    iSipSession->NextState( KMceStateIdle );
    iSipSession->EventReceived( sinkEvent );

    iSipSession->NextState( KMceStateEstablished );
    iSipSession->EventReceived( sinkEvent );
    
    //fails
    iSipSession->NextState( KMceStateIdle );
    iSipSession->EventReceived( sourceEvent );

    iSipSession->NextState( KMceStateEstablished );
    iSipSession->EventReceived( sourceEvent );


    iSipSession->iFCSession = CFCSession::NewL( NULL, 0, 0 );
    

    //fails
    iSipSession->NextState( KMceStateIdle );
    iSipSession->EventReceived( sourceEvent );

    iSipSession->NextState( KMceStateEstablished );
    iSipSession->EventReceived( sourceEvent );
    
    delete iSipSession->iFCSession;
    iSipSession->iFCSession = NULL;
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_MediaErrorL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_MediaErrorL()
    {
    // Media Error Handled
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    iSipSession->NextState( KMceStateEstablished );

	// MCE Server State is not changed, only Client is informed
	EUNIT_ASSERT( iSipSession->CurrentState().Id() == KMceStateEstablished )
	
    TMceMccComEvent event( KMccStreamError );
    event.iError = KErrGeneral;
    iSipSession->MediaError( event );
           
    
    //MediaError IGnored
    iSipSession->NextState( KMceStateIdle );
    iSipSession->MediaError( event );
    
    // MCE Server State is not changed, Media Error is IGnored
	EUNIT_ASSERT( iSipSession->CurrentState().Id() == KMceStateIdle )
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_TerminateL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_TerminateL()
    {
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );

    CSIPClientTransaction* clitransaction =
        MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite );
	iSipSession->SetPendingTransactionL( clitransaction );
    CleanupStack::Pop( clitransaction );
    
    MCE_RESET_STUBS();
    
    // KMceStateIdle
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EIdle /*clientState*/, 
                    KMceStateIdle /*serverState*/ );
    
    EUNIT_ASSERT( !iSipSession->Terminate() );
    
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EIdle /*clientState*/, 
                       KMceStateTerminated /*serverState*/ );
    MCE_RESET_STUBS();
    
    // KMceStateClientEstablishing
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EIdle /*clientState*/, 
                    KMceStateClientEstablishing /*serverState*/ );

    TBool canDispose = iSipSession->Terminate();
    MCE_CHECK_MEMORY_LEAVE( canDispose )
    EUNIT_ASSERT( !canDispose );

    EUNIT_ASSERT( iStorage->iSipSentMethod ==
    			  SIPStrings::StringF( SipStrConsts::EEmpty ) );
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EIdle /*clientState*/, 
                       KMceStateClientEstablishing /*serverState*/ );
    MCE_RESET_STUBS();

    // KMceStateOffering
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EOffering /*clientState*/, 
                    KMceStateOffering /*serverState*/ );
    iSipSession->SetSubState( CMceSipSession::EOffering );
    clitransaction->iState = CSIPTransactionBase::EProceeding;

    canDispose = iSipSession->Terminate();
    MCE_CHECK_MEMORY_LEAVE( canDispose )
    EUNIT_ASSERT( !canDispose );

    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::ECancel /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);

	MCE_CHECK_MEMORY_LEAVE( iSipSession->CurrentState().Id() != KMceStateCanceled )
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EOffering /*clientState*/, 
                       KMceStateCanceled /*serverState*/ );
    MCE_RESET_STUBS();
    
    //INVITE,180,PRACK,200 ok to PRACK,delete session,CANCEL
    CSIPClientTransaction* clientransaction2 =
        MCETestHelper::ClientTransactionLC( SipStrConsts::EPrack,
        								    200,
        									SipStrConsts::EPhraseOk,
        								 	EFalse);
	iSipSession->SetPendingTransactionL( clientransaction2 );
    CleanupStack::Pop( clientransaction2 );
    
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EOffering /*clientState*/, 
                    KMceStateOffering /*serverState*/ );
    iSipSession->SetSubState( CMceSipSession::EOffering );
    clitransaction->iState = CSIPTransactionBase::EProceeding;
 
    iSipSession->SetResponse( *clientransaction2 );



    iSipSession->DoTerminateL();
/* TODO: should above be changed to:
	canDispose = iSipSession->Terminate();
    MCE_CHECK_MEMORY_LEAVE( canDispose )
    EUNIT_ASSERT( !canDispose ); */
   
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::ECancel /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EOffering /*clientState*/, 
                       KMceStateCanceled /*serverState*/ );
                       
    MCE_RESET_STUBS();


    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EOffering /*clientState*/, 
                    KMceStateOffering /*serverState*/ );
    iSipSession->SetSubState( CMceSipSession::EOffering );
    clitransaction->iState = CSIPTransactionBase::ECalling;
    
    iSipSession->DoTerminateL();
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EOffering /*clientState*/, 
                       KMceStateCanceled /*serverState*/ );
    MCE_RESET_STUBS();

    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EOffering /*clientState*/, 
                    KMceStateOffering /*serverState*/ );
    iSipSession->SetSubState( CMceSipSession::EUpdating );

    canDispose = iSipSession->Terminate();
    MCE_CHECK_MEMORY_LEAVE( canDispose )
    EUNIT_ASSERT( !canDispose );

    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EOffering /*clientState*/, 
                       KMceStateOffering /*serverState*/ );
    MCE_RESET_STUBS();
    
    // KMceStateEstablished
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EEstablished /*clientState*/, 
                    KMceStateEstablished /*serverState*/ );

    canDispose = iSipSession->Terminate();
    MCE_CHECK_MEMORY_LEAVE( canDispose )
    EUNIT_ASSERT( !canDispose );

	MCE_CHECK_MEMORY_LEAVE( !( SIPStrings::StringF( SipStrConsts::EBye ).DesC()
							   == iStorage->iSipSentMethod.DesC() ) )
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EBye /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EEstablished /*clientState*/, 
                       KMceStateTerminating /*serverState*/ );
    MCE_RESET_STUBS();
    
    // KMceStateTerminating
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::ETerminating /*clientState*/, 
                    KMceStateTerminating /*serverState*/ );
    
    iSipSession->DoTerminateL();
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::ETerminating /*clientState*/, 
                       KMceStateTerminated /*serverState*/ );
    MCE_RESET_STUBS();
    
    // KMceStateTerminated
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::ETerminating /*clientState*/, 
                    KMceStateTerminated /*serverState*/ );
    
    iSipSession->DoTerminateL();
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::ETerminating /*clientState*/, 
                       KMceStateTerminated /*serverState*/ );
    MCE_RESET_STUBS();
    
    // KMceStateServerEstablishing, no request
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EIncoming /*clientState*/, 
                    KMceStateServerEstablishing /*serverState*/ );
    
    iSipSession->DoTerminateL();
    EUNIT_ASSERT( iStorage->iSipSentMethod ==
    			  SIPStrings::StringF( SipStrConsts::EEmpty ) );
    MCE_RESET_STUBS();
    
    // KMceStateServerEstablishingrequest, dialog terminated
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EIncoming /*clientState*/, 
                    KMceStateServerEstablishing /*serverState*/ );
    CSIPServerTransaction* request =
    	MCETestHelper::ServerTransactionL( SipStrConsts::EInvite );
    CleanupStack::PushL( request );
    iSipSession->iPendingReceivedRequests.AppendL( request );
    CleanupStack::Pop( request );

	iSipSession->DoTerminateL();
	MCE_CHECK_MEMORY_LEAVE( !iStorage->iSipSentResponse || 
		iStorage->iSipSentResponse->StatusCode() != KMceSipDecline )

    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipDecline /*sentResponse*/);
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EIncoming /*clientState*/, 
                       KMceStateTerminated /*serverState*/ );
    
    MCE_RESET_STUBS();
    
    // KMceStateServerEstablishingrequest, dialog confirmed
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EIncoming /*clientState*/, 
                    KMceStateServerEstablishing /*serverState*/ );
    iSipSession->Dialog()->Dialog().iState = CSIPDialog::EConfirmed;

	canDispose = iSipSession->Terminate();
    MCE_CHECK_MEMORY_LEAVE( canDispose )
    EUNIT_ASSERT( !canDispose );
    
    MCE_CHECK_MEMORY_LEAVE( !iStorage->iSipSentResponse || 
		iStorage->iSipSentResponse->StatusCode() != KMceSipDecline )

	MCE_CHECK_MEMORY_LEAVE( !( SIPStrings::StringF( SipStrConsts::EBye ).DesC()
							   == iStorage->iSipSentMethod.DesC() ) )
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EBye /*sentMethod*/, 
                      KMceSipDecline /*sentResponse*/);
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EIncoming /*clientState*/, 
                       KMceStateTerminating /*serverState*/ );
    
    MCE_RESET_STUBS();
    
    // KMceStateUpdating    
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EReserving /*clientState*/, 
                    KMceStateUpdating /*serverState*/ );

	canDispose = iSipSession->Terminate();
    MCE_CHECK_MEMORY_LEAVE( canDispose )
    EUNIT_ASSERT( !canDispose );

    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EReserving /*clientState*/, 
                       KMceStateUpdating /*serverState*/ );
    MCE_RESET_STUBS();
    
    // KMceStateUpdated
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EReserving /*clientState*/, 
                    KMceStateUpdated /*serverState*/ );
    
    iSipSession->DoTerminateL();
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EReserving /*clientState*/, 
                       KMceStateTerminated /*serverState*/ );
    MCE_RESET_STUBS();
    
    // KMceStateUpdated, no request
    iSipSession->iPendingReceivedRequests.ResetAndDestroy();
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EReserving /*clientState*/, 
                    KMceStateUpdated /*serverState*/ );
    
    iSipSession->DoTerminateL();
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EReserving /*clientState*/, 
                       KMceStateTerminated /*serverState*/ );
    MCE_RESET_STUBS();
    
    // KMceStateAnswering
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EProceeding /*clientState*/, 
                    KMceStateAnswering /*serverState*/ );
    
    //EUNIT_ASSERT( !iSipSession->Terminate() );
    
	canDispose = iSipSession->Terminate();
    MCE_CHECK_MEMORY_LEAVE( canDispose )
    EUNIT_ASSERT( !canDispose );

	MCE_CHECK_MEMORY_LEAVE( !( SIPStrings::StringF( SipStrConsts::EBye ).DesC()
							   == iStorage->iSipSentMethod.DesC() ) )
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EBye /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EProceeding /*clientState*/, 
                       KMceStateAnswering /*serverState*/ );
    MCE_RESET_STUBS();
    
    // KMceStateError -> exit from KMceStateAnswering
    iSipSession->CurrentState( ETrue );
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EProceeding /*clientState*/, 
                    KMceStateError /*serverState*/ );

	canDispose = iSipSession->Terminate();
    MCE_CHECK_MEMORY_LEAVE( canDispose )
    EUNIT_ASSERT( !canDispose );

	MCE_CHECK_MEMORY_LEAVE( !( SIPStrings::StringF( SipStrConsts::EBye ).DesC()
							   == iStorage->iSipSentMethod.DesC() ) )
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EBye /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EProceeding /*clientState*/, 
                       KMceStateTerminating /*serverState*/ );
    MCE_RESET_STUBS();
    
    // KMceStateCanceled
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::ECancelling /*clientState*/, 
                    KMceStateCanceled /*serverState*/ );
    
	canDispose = iSipSession->Terminate();
    MCE_CHECK_MEMORY_LEAVE( canDispose )
    EUNIT_ASSERT( !canDispose );

    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::ECancelling /*clientState*/, 
                       KMceStateCanceled /*serverState*/ );
    MCE_RESET_STUBS();
    
    // KMceStateError -> exit from KMceStateCanceled
    delete iSipSession->iBody;iSipSession->iBody = NULL;
    iSipSession->CurrentState( ETrue );
    iSipSession->NextState( KMceStateError );
    iSipSession->DoTerminateL();
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    EUNIT_ASSERT( iSipSession->CurrentState().Id() == KMceStateTerminated );                      
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_UseFCL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_UseFCL()
    {
    EUNIT_ASSERT( !iSipSession->UseFC() );
    iSipSession->iUseFC = ETrue;
    EUNIT_ASSERT( iSipSession->UseFC() );
    }  

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_FCReceivedDataL
// ---------------------------------------------------------------------------
//      
void UT_CMceSipSession::UT_CMceSipSession_FCReceivedDataL()
    {
    iSipSession->FCReceivedData( NULL, NULL );
    
    HBufC8* data = _L8( "FC DATA" ).AllocL();
    iSipSession->FCReceivedData( data, NULL );
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_FCErrorNotifyL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_FCErrorNotifyL()
    {
    iSipSession->FCErrorNotify( KErrArgument, NULL );
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_InviteForkedLL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_InviteForkedLL()
    {
    CSIPClientTransaction* clitransaction = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EBye );
    iSipSession->SetPendingTransactionL( clitransaction );
    CleanupStack::Pop( clitransaction );
    
    // leaves with NULL
    EUNIT_ASSERT_LEAVE( iSipSession->InviteForkedL( *clitransaction, NULL ) );
    
    EUNIT_ASSERT( iSipSession->iForkedDialogs.Count() == 0 );
    
    // create dialog
    CUri8* uri = CUri8::NewLC(); 
    MCE_SET_URIL( uri );
  	
    CSIPFromHeader* fromheader = CSIPFromHeader::DecodeL(_L8("User <user2@host>")); 
    CleanupStack::PushL( fromheader );

    CSIPInviteDialogAssoc* dialog = 
        CSIPInviteDialogAssoc::NewL( iSipSession->SIPConnection().Connection(),
                                     fromheader, uri );
    CleanupStack::Pop( fromheader );
    CleanupStack::Pop( uri );
    CleanupStack::PushL( dialog );
    
    //180
    MCETestHelper::SetResponseL( *clitransaction, KMceSipRinging,
    							 SipStrConsts::EPhraseRinging );
    
    iSipSession->NextState( KMceStateTerminated );
    iSipSession->InviteForkedL( *clitransaction, dialog );
    CleanupStack::Pop( dialog );
    
    EUNIT_ASSERT( iSipSession->iForkedDialogs.Count() == 1 );
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_HandleForkedDialogsL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_HandleForkedDialogsL()
    {
    iSipSession->HandleForkedDialogs();
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_WaitingMediaCallbackL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_WaitingMediaCallbackL()
    {
    for ( TInt l = 0; l< KMceTestLoops; l++  )
        {
        EUNIT_ASSERT( !iSipSession->WaitingMediaCallback() );
        iSipSession->iWaitingMediaCallback = ETrue;
        EUNIT_ASSERT( iSipSession->WaitingMediaCallback() );
        iSipSession->iWaitingMediaCallback = EFalse;
        }   
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_SetWaitingMediaCallbackL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_SetWaitingMediaCallbackL()
    {
    for ( TInt l = 0; l< KMceTestLoops; l++  )
        {
        EUNIT_ASSERT( !iSipSession->iWaitingMediaCallback );
        iSipSession->SetWaitingMediaCallback( ETrue );
        EUNIT_ASSERT( iSipSession->iWaitingMediaCallback );
        iSipSession->SetWaitingMediaCallback( EFalse );
        }       
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_ReliableFailedL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_ReliableFailedL()
    {
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    
    TInt aErrorCode;
    aErrorCode = KErrNoMemory;
    
    // Test1: Create a Server Transaction to be matched with current sip session 
    CSIPServerTransaction& serverTrans1 =
    	MCETestHelper::RequestL( iSipSession, SipStrConsts::EInvite );
    iSipSession->ReliableFailed( serverTrans1, aErrorCode );
    MCE_RECEIVE_QUEUE_POSTCONDITION( iSipSession )
    EUNIT_ASSERT( iSipSession->Request() == serverTrans1 );
  
                  
    // Test2: Dummy server transaction that does not match with the server transaction 
    // of the session
    CSIPServerTransaction* serverTrans =
    	MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, ETrue );
    CleanupStack::PushL( serverTrans );
    iSipSession->ReliableFailed( *serverTrans, aErrorCode );
    CleanupStack::PopAndDestroy( serverTrans );
    EUNIT_ASSERT( !( iSipSession->Request() == *serverTrans ) );             
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_NoPrackReceivedL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_NoPrackReceivedL()
    {
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    
    // Test 1
    CSIPServerTransaction& serverTrans1 =
    	MCETestHelper::RequestL( iSipSession, SipStrConsts::EInvite );
    iSipSession->NextState( KMceStateAcknowledgementRequired );
    iSipSession->NoPrackReceived( serverTrans1 );
    EUNIT_ASSERT( iSipSession->Request() == serverTrans1 )
    
    iSipSession->NextState( KMceStateTerminated );
    iSipSession->NoPrackReceived( serverTrans1 );
            
            
    // Test 2
    CSIPServerTransaction* serverTrans =
    	MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, ETrue );
    CleanupStack::PushL( serverTrans );
    iSipSession->NoPrackReceived( *serverTrans );
    CleanupStack::PopAndDestroy( serverTrans );
    EUNIT_ASSERT( !( iSipSession->Request() == *serverTrans ) );            
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_UpdateMediaLL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_UpdateMediaLL()
    {
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    EUNIT_ASSERT( iSipSession->UpdateMediaL() == KErrNone );
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_ActiveBodyL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_ActiveBodyL()
    {

    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    EUNIT_ASSERT( &iSipSession->ActiveBody() == iSipSession->iBody );

    iSipSession->SetSubState( CMceSipSession::EUpdating );
    EUNIT_ASSERT( &iSipSession->ActiveBody() == iSipSession->iBody );
    
    iSipSession->iNewBodyCandidate = CMceComSession::NewL( CMceComSession::EOutSession );
    
    EUNIT_ASSERT( &iSipSession->ActiveBody() == iSipSession->iNewBodyCandidate );
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_CanceledL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_CanceledL()
    {
    // Test canceling invite situation
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EInSession );
    iSipSession->Dialog()->Dialog().iState = CSIPDialog::EInit;
    iSipSession->Actions().StateChanged( KMceStateUpdating );
    iSipSession->Canceled();
    
    EUNIT_ASSERT( iSipSession->CurrentState().Id() == KMceStateTerminated );
    
    // Test canceling reinvite situation
    iSipSession->iNewBodyCandidate = 
        CMceComSession::NewL( CMceComSession::EInSession );
    iSipSession->Dialog()->Dialog().iState = CSIPDialog::EConfirmed;
    iSipSession->Actions().StateChanged( KMceStateUpdating );

    iSipSession->Canceled();
    
    EUNIT_ASSERT( iSipSession->CurrentState().Id() == KMceStateEstablished );
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_ForceSDPSending
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_ForceSDPSending()
	{
	EUNIT_ASSERT( iSipSession->ForceSDPSendingStatus() == EFalse );
	}

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_IsProfileContactSecureLL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_IsProfileContactSecureLL()
    {
    // test the functionality of Checking Contact header if it is secure
    EUNIT_ASSERT( !iSipSession->IsProfileContactSecureL( ) );
    //change profile contact header
    HBufC8* profileRegisContactTls = KContactValueTls().AllocLC();
    iSipSession->Profile().SetParam( KSIPRegisteredContact,
    								 profileRegisContactTls );
    CleanupStack::Pop(profileRegisContactTls );
  	
  	EUNIT_ASSERT( iSipSession->IsProfileContactSecureL( ) );
  	
  	//KContactValueSips
  	HBufC8* profileRegisContactSips = KContactValueSips().AllocLC();
    iSipSession->Profile().SetParam( KSIPRegisteredContact,
    								 profileRegisContactSips );
    CleanupStack::Pop(profileRegisContactSips );
  	
  	EUNIT_ASSERT( iSipSession->IsProfileContactSecureL( ) );
  	
  	//KContactValueSipsAndTls
    
    HBufC8* profileRegisContactTlsAndSips = KContactValueSipsAndTls().AllocLC();
    iSipSession->Profile().SetParam( KSIPRegisteredContact,
    								 profileRegisContactTlsAndSips );
    CleanupStack::Pop(profileRegisContactTlsAndSips );
  	
  	EUNIT_ASSERT( iSipSession->IsProfileContactSecureL( ) );
    }

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_ErrorOccuredL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_ErrorOccuredL()
    {
	TMceActionSet actions = iSipSession->Actions();
	
	// Test getting an error in terminating state (an error to BYE)
	TUint newState( KMceStateTerminating );
	actions.StateChanged( newState );
	iSipSession->ErrorOccured( KErrGeneral );
	EUNIT_ASSERT_EQUALS( iSipSession->CurrentState().Id(), KMceStateTerminated );	
	
	// Test a normal situation
	newState = KMceStateOffering;
	actions.StateChanged( newState );
	iSipSession->ErrorOccured( KErrNoMemory );
	EUNIT_ASSERT_EQUALS( iSipSession->CurrentState().Id(), KMceStateTerminated );

	//test re-invite situation( it should send BYE when KErrSIPNoAckReceived error occurs)
	newState = KMceStateAnswering;
	actions.StateChanged( newState );
	iSipSession->SetSubState( CMceSipSession::EUpdating );
	iSipSession->Dialog()->Dialog().iState = CSIPDialog::EConfirmed;
	iSipSession->ErrorOccured( KErrSIPNoAckReceived );
	EUNIT_ASSERT_EQUALS( iSipSession->CurrentState().Id(), KMceStateTerminating );
	}


// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_ErrorOccuredL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_DoErrorOccured()
	{
	TInt errorCode = KErrTimedOut;
	TUint newState;
	TMceActionSet actions = iSipSession->Actions();
	
	// Test1: Error for an Extension Request
	newState = KMceStateOffering;
	actions.StateChanged(newState);
	  
	CSIPServerTransaction* srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInfo );
    
    iSipSession->DoErrorOccured( errorCode, *srvtransaction  );
    
    delete srvtransaction;

    //Ensure no change to the state occurs, when error for extension request is received
    EUNIT_ASSERT_EQUALS( iSipSession->CurrentState().Id(), KMceStateOffering );	
    
        
    // Test2: Error for an Known SIP Request    
	CSIPServerTransaction* srvtransaction2 = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInvite );
    
    iSipSession->DoErrorOccured( errorCode, *srvtransaction2 );
    
    //Ensure state goes to terminated if error is not for extension request 
    EUNIT_ASSERT_EQUALS( iSipSession->CurrentState().Id(), KMceStateTerminated );	
    
    delete srvtransaction2;
	}

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_ConsumesL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_ConsumesL()
	{

    // create dialog
    CUri8* uri = CUri8::NewLC(); 
    MCE_SET_URIL( uri );
    
    CSIPFromHeader* fromheader = CSIPFromHeader::DecodeL(_L8("User <user2@host>")); 
    CleanupStack::PushL( fromheader );

    CSIPInviteDialogAssoc* dialog = 
        CSIPInviteDialogAssoc::NewL( iSipSession->SIPConnection().Connection(),
                                     fromheader, uri );
    CleanupStack::Pop( fromheader );
    CleanupStack::Pop( uri );
    CleanupStack::PushL( dialog );

	// Test 1: Non Forking Case
	
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    
    iSipSession->NextState( KMceStateOffering );

 //INFO
    CSIPServerTransaction* srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInfo );
    CleanupStack::PushL( srvtransaction );

    CSIPDialogAssocBase* originalDialog = iSipSession->Dialog();
    
//INFO - sip session request within session's dialog
	TBool assocDialog( EFalse );
	TBool consumed = iSipSession->Consumes( iSipSession->Dialog()->Dialog(), *srvtransaction,  assocDialog );
	
	EUNIT_ASSERT( consumed );
    EUNIT_ASSERT( assocDialog );
    EUNIT_ASSERT_EQUALS( iSipSession->CurrentState().Id(), KMceStateOffering );
    
//INFO - sip session request within other dialog
    assocDialog = EFalse;
    consumed = iSipSession->Consumes( dialog->Dialog(), *srvtransaction,  assocDialog );
    EUNIT_ASSERT( !consumed );
    EUNIT_ASSERT( !assocDialog );
    
	CleanupStack::PopAndDestroy( srvtransaction );
	
	
//REFER
    assocDialog = EFalse;

    CSIPServerTransaction* refer = 
        MCETestHelper::ServerTransactionL( SipStrConsts::ERefer );
    CleanupStack::PushL( refer );
    
//REFER - event request within session's dialog 
    TBool consumed1 = iSipSession->Consumes( iSipSession->Dialog()->Dialog(), *refer,  assocDialog );
    EUNIT_ASSERT( !consumed1 );
    EUNIT_ASSERT( assocDialog );

//REFER - event request within other dialog 
    assocDialog = EFalse;
    consumed = iSipSession->Consumes( dialog->Dialog(), *refer,  assocDialog );
    EUNIT_ASSERT( !consumed );
    EUNIT_ASSERT( !assocDialog );
    
    CleanupStack::PopAndDestroy( refer );


    
	// Test 2: When Invite Dialog is Forked:	

    // create dialog
    CUri8* uri2 = CUri8::NewLC(); 
    MCE_SET_URIL( uri2 );
    
    CSIPFromHeader* fromheader2 = CSIPFromHeader::DecodeL(_L8("User <user2@host>")); 
    CleanupStack::PushL( fromheader2 );

    CSIPInviteDialogAssoc* dialog2 = 
        CSIPInviteDialogAssoc::NewL( iSipSession->SIPConnection().Connection(),
                                     fromheader2, uri2 );
    CleanupStack::Pop( fromheader2 );
    CleanupStack::Pop( uri2 );
    CleanupStack::PushL( dialog2 );
    
    CSIPClientTransaction* clitransaction = 
    MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite );
    iSipSession->SetPendingTransactionL( clitransaction );
    CleanupStack::Pop( clitransaction );
    
    // leaves with NULL
    EUNIT_ASSERT_LEAVE( iSipSession->InviteForkedL( *clitransaction, NULL ) );
    
    EUNIT_ASSERT( iSipSession->iForkedDialogs.Count() == 0 );
    
    
    //180
    MCETestHelper::SetResponseL( *clitransaction, KMceSipRinging,
    							 SipStrConsts::EPhraseRinging );
    
    iSipSession->NextState( KMceStateOffering );
    iSipSession->InviteForkedL( *clitransaction, dialog2 );
    CleanupStack::Pop( dialog2 );
    
    EUNIT_ASSERT( iSipSession->iForkedDialogs.Count() == 1 );

//INFO
    
    CSIPServerTransaction* info = 
    MCETestHelper::ServerTransactionL( SipStrConsts::EInfo );
    CleanupStack::PushL( info );

//INFO - sip session request within session's dialog
    assocDialog = EFalse;
	consumed1 = iSipSession->Consumes( iSipSession->Dialog()->Dialog(), *info,  assocDialog );
	
	EUNIT_ASSERT( consumed1 )
	EUNIT_ASSERT( assocDialog )
	EUNIT_ASSERT_EQUALS( iSipSession->CurrentState().Id(), KMceStateOffering );

//INFO - sip session request within forked dialog
	iSipSession->iCurrentDialog = originalDialog; 
    assocDialog = EFalse;
    consumed1 = iSipSession->Consumes( iSipSession->iForkedDialogs[0]->Dialog(), *info,  assocDialog );
    // we should not have consumed the event here?
    EUNIT_ASSERT( consumed1 )
    EUNIT_ASSERT( assocDialog )
	
    CleanupStack::PopAndDestroy( info );

//REFER
    assocDialog = EFalse;
    iSipSession->iCurrentDialog = iSipSession->iForkedDialogs[0]; 

    refer = 
        MCETestHelper::ServerTransactionL( SipStrConsts::ERefer );
    CleanupStack::PushL( refer );
    
//REFER - event request within session's dialog 
    consumed1 = iSipSession->Consumes( iSipSession->Dialog()->Dialog(), *refer,  assocDialog );
    EUNIT_ASSERT( !consumed1 );
    EUNIT_ASSERT( assocDialog );

//REFER - event request within forked dialog 
    iSipSession->iCurrentDialog = originalDialog; 
    assocDialog = EFalse;
    consumed = iSipSession->Consumes( iSipSession->iForkedDialogs[0]->Dialog(), *refer,  assocDialog );
    EUNIT_ASSERT( !consumed );
    EUNIT_ASSERT( assocDialog );
    
    CleanupStack::PopAndDestroy( refer );
    CleanupStack::PopAndDestroy( dialog );
    
	
	}



// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_ConsumesL //HKK
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_MethodCodeL()
	{
	TMceSipEventCode aCode = (TMceSipEventCode) KErrNotFound;

	
	//Test1: Extension Request Test
    _LIT8( KInfo, "INFO" );
    RStringF info = SIPStrings::Pool().OpenFStringL( KInfo );
    aCode = iSipSession->MethodCode( info );
    EUNIT_ASSERT_EQUALS( aCode, EMceRequest );	
    
	//Test2: SIP Events method should results in KErrNotFound
	//REFER 
	_LIT8( KRefer, "REFER" );
    RStringF refer = SIPStrings::Pool().OpenFStringL( KRefer );
    aCode = iSipSession->MethodCode( refer );
    EUNIT_ASSERT_EQUALS( aCode, KErrNotFound );	
	
	//Test3: Valid SIP Event, Appropriate code will be returned
	_LIT8( KInvite, "INVITE" );
    RStringF invite = SIPStrings::Pool().OpenFStringL( KInvite );
    aCode = iSipSession->MethodCode( invite );
    EUNIT_ASSERT_EQUALS( aCode, EMceInvite );	
    
    _LIT8( KCancel, "CANCEL" );
    RStringF cancel = SIPStrings::Pool().OpenFStringL( KCancel );
    aCode = iSipSession->MethodCode( cancel );
    EUNIT_ASSERT_EQUALS( aCode, EMceCancel );	
    
    _LIT8( KAck, "ACK" );
    RStringF ack = SIPStrings::Pool().OpenFStringL( KAck );
    aCode = iSipSession->MethodCode( ack );
    EUNIT_ASSERT_EQUALS( aCode, EMceAck );	

	_LIT8( KBye, "BYE" );
    RStringF bye = SIPStrings::Pool().OpenFStringL( KBye );
    aCode = iSipSession->MethodCode( bye );
    EUNIT_ASSERT_EQUALS( aCode, EMceBye );	
    
    _LIT8( KPrack, "PRACK" );
    RStringF prack = SIPStrings::Pool().OpenFStringL( KPrack );
    aCode = iSipSession->MethodCode( prack );
    EUNIT_ASSERT_EQUALS( aCode, EMcePrack );	
    
    _LIT8( KUpdate, "UPDATE" );
    RStringF update = SIPStrings::Pool().OpenFStringL( KUpdate );
    aCode = iSipSession->MethodCode( update );
    EUNIT_ASSERT_EQUALS( aCode, EMceUpdate );	
	
	}

// ---------------------------------------------------------------------------
// UT_CMceSipSession::UT_CMceSipSession_IsSipDialogConfirmedL
// ---------------------------------------------------------------------------
//
void UT_CMceSipSession::UT_CMceSipSession_IsSipDialogConfirmedL()
    {
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EInSession );
    
    // Confirmation status should be EFalse, 
    // if CSIPDialog is something else than EConfirmed.
    iSipSession->Dialog()->Dialog().iState = CSIPDialog::EEarly;
    EUNIT_ASSERT_EQUALS( iSipSession->IsSipDialogConfirmed(), EFalse );
    
    // Confirmation status should be ETrue, if CSIPDialog is EConfirmed.
    iSipSession->Dialog()->Dialog().iState = CSIPDialog::EConfirmed;
    EUNIT_ASSERT_EQUALS( iSipSession->IsSipDialogConfirmed(), ETrue );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceSipSession,
    "UT_CMceSipSession",
    "UNIT" )

EUNIT_TEST(
    "NeedToSend - test ",
    "CMceSipSession",
    "NeedToSend",
    "FUNCTIONALITY", SetupL, UT_CMceSipSession_NeedToSendL, Teardown)

EUNIT_TEST(
    "DoesMediaManagerNeedToNegotiate - test ",
    "CMceSipSession",
    "DoesMediaManagerNeedToNegotiate",
    "FUNCTIONALITY", SetupL, UT_CMceSipSession_DoesMediaManagerNeedToNegotiateL, Teardown)

EUNIT_TEST(
    "ServiceL - test ",
    "CMceSipSession",
    "ServiceL",
    "FUNCTIONALITY", SetupL, UT_CMceSipSession_ServiceLL, Teardown)

EUNIT_TEST(
    "fc - test ",
    "CMceSipSession",
    "-",
    "FUNCTIONALITY", SetupL, UT_CMceSipSession_FCTestsL, Teardown)

EUNIT_TEST(
    "CurrentState - test ",
    "CMceSipSession",
    "CurrentState",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_CurrentStateL, Teardown)

EUNIT_TEST(
    "Body - test ",
    "CMceSipSession",
    "Body",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_BodyL, Teardown)
    
EUNIT_TEST(
    "ClearRemoteUriy - test ",
    "CMceSipSession",
    "ClearRemoteUri",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_ClearRemoteUriL, Teardown)

EUNIT_TEST(
    "FindRemoteUri - test ",
    "CMceSipSession",
    "FindRemoteUri",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_FindRemoteUriL, Teardown)

EUNIT_TEST(
    "AddRemoteUri - test ",
    "CMceSipSession",
    "AddRemoteUri",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_AddRemoteUriL, Teardown)
    
EUNIT_TEST(
    "AddRemoteUriFromResponse - test ",
    "CMceSipSession",
    "AddRemoteUriFromResponse",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_AddRemoteUriFromResponseL, Teardown)

EUNIT_TEST(
    "CreateDialogFrom3XXResponseL - test ",
    "CMceSipSession",
    "CreateDialogFrom3XXResponseL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_CreateDialogFrom3XXResponseL, Teardown)

EUNIT_TEST(
    "Offer - test ",
    "CMceSipSession",
    "Offer",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_OfferL, Teardown)

EUNIT_TEST(
    "NextState - test ",
    "CMceSipSession",
    "NextState",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_NextStateL, Teardown)

EUNIT_TEST(
    "InviteTransaction - test ",
    "CMceSipSession",
    "InviteTransaction",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_InviteTransactionL, Teardown)

EUNIT_TEST(
    "AcceptMethod - test ",
    "CMceSipSession",
    "AcceptMethod",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_AcceptMethodL, Teardown)
    
EUNIT_TEST(
    "RequestReceived - test ",
    "CMceSipSession",
    "RequestReceived",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_RequestReceivedL, Teardown)
    
EUNIT_TEST(
    "RequestReceived - 2 test ",
    "CMceSipSession",
    "RequestReceived",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_RequestReceived2L, Teardown)

EUNIT_TEST(
    "ForkedResponseReceived - test ",
    "CMceSipSession",
    "ResponseReceived",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_ForkedResponseReceivedL, Teardown)
    
EUNIT_TEST(
    "ResponseReceived - test ",
    "CMceSipSession",
    "ResponseReceived",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_ResponseReceivedL, Teardown)

EUNIT_TEST(
    "ProvisionalResponseReceived - test ",
    "CMceSipSession",
    "ProvisionalResponseReceived",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_ProvisionalResponseReceivedL, Teardown)
    
EUNIT_TEST(
    "RedirectionResponseReceived - test ",
    "CMceSipSession",
    "RedirectionResponseReceived",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_RedirectionResponseReceivedL, Teardown)

EUNIT_TEST(
    "ErrorResponseReceived - test ",
    "CMceSipSession",
    "ErrorResponseReceived",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_ErrorResponseReceivedL, Teardown)
    
EUNIT_TEST(
    "StandAloneRequestReceived - test ",
    "CMceSipSession",
    "StandAloneRequestReceived",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_StandAloneRequestReceivedL, Teardown)

EUNIT_TEST(
    "StandAloneResponseReceived - test ",
    "CMceSipSession",
    "StandAloneResponseReceived",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_StandAloneResponseReceivedL, Teardown)

EUNIT_TEST(
    "DoInitializeIncomingDialogL - test ",
    "CMceSipSession",
    "DoInitializeIncomingDialogL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_DoInitializeIncomingDialogLL, Teardown)

EUNIT_TEST(
    "Actions - test ",
    "CMceSipSession",
    "Actions",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_ActionsL, Teardown)

EUNIT_TEST(
    "OnExpired - test ",
    "CMceSipSession",
    "OnExpired",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_OnExpiredL, Teardown)

EUNIT_TEST(
    "CanDispose - test ",
    "CMceSipSession",
    "CanDispose",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_CanDisposeL, Teardown)

EUNIT_TEST(
    "ConnectionStateChanged - test ",
    "CMceSipSession",
    "ConnectionStateChanged",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_ConnectionStateChangedL, Teardown)

EUNIT_TEST(
    "CreateOutgoingDialog - test ",
    "CMceSipSession",
    "CreateOutgoingDialog",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_CreateOutgoingDialogL, Teardown)
    
EUNIT_TEST(
    "ReCreateOutgoingDialog - test ",
    "CMceSipSession",
    "ReCreateOutgoingDialog",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_ReCreateOutgoingDialogL, Teardown)

EUNIT_TEST(
    "SessionClosed - test ",
    "CMceSipSession",
    "SessionClosed",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_SessionClosedL, Teardown)

EUNIT_TEST(
    "EventReceived - test ",
    "CMceSipSession",
    "EventReceived",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_EventReceivedL, Teardown)

EUNIT_TEST(
    "MediaError - test ",
    "CMceSipSession",
    "MediaError",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_MediaErrorL, Teardown)

EUNIT_TEST(
    "Terminate - test ",
    "CMceSipSession",
    "Terminate",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_TerminateL, Teardown)

EUNIT_TEST(
    "UseFC - test ",
    "CMceSipSession",
    "UseFC",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_UseFCL, Teardown)

EUNIT_TEST(
    "FCReceivedData - test ",
    "CMceSipSession",
    "FCReceivedData",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_FCReceivedDataL, Teardown)

EUNIT_TEST(
    "FCErrorNotify - test ",
    "CMceSipSession",
    "FCErrorNotify",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_FCErrorNotifyL, Teardown)

EUNIT_TEST(
    "InviteForkedL - test ",
    "CMceSipSession",
    "InviteForkedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_InviteForkedLL, Teardown)
    
EUNIT_TEST(
    "HandleForkedDialogs - test ",
    "CMceSipSession",
    "HandleForkedDialogs",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_HandleForkedDialogsL, Teardown)

EUNIT_TEST(
    "WaitingMediaCallback - test ",
    "CMceSipSession",
    "WaitingMediaCallback",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_WaitingMediaCallbackL, Teardown)

EUNIT_TEST(
    "SetWaitingMediaCallback - test ",
    "CMceSipSession",
    "WaitingMediaCallback",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_SetWaitingMediaCallbackL, Teardown)
    
EUNIT_TEST(
    "ReliableFailed - test", 
    "CMceSipSession",
    "ReliableFailed",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_ReliableFailedL, Teardown)
    
        
EUNIT_TEST(
    "NoPrackReceived - test", 
    "CMceSipSession",
    "NoPrackReceived",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_NoPrackReceivedL, Teardown)

EUNIT_TEST(
    "UpdateMediaL - test", 
    "CMceSipSession",
    "UpdateMediaL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_UpdateMediaLL, Teardown)

EUNIT_TEST(
    "ActiveBody - test", 
    "CMceSipSession",
    "ActiveBody",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_ActiveBodyL, Teardown)

EUNIT_TEST(
    "Canceled - test", 
    "CMceSipSession",
    "Canceled",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_CanceledL, Teardown)

EUNIT_TEST(
    "Canceled - test", 
    "CMceSipSession",
    "Canceled",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_ForceSDPSending, Teardown)

EUNIT_TEST(
    "IsProfileContactSecureL - test ",
    "CMceSipSession",
    "IsProfileContactSecureL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipSession_IsProfileContactSecureLL, Teardown)  
    
EUNIT_TEST(
	"ErrorOccured - test",
	"CMceSipSession",
	"ErrorOccured",
	"FUNCTIONALITY",
	SetupL, UT_CMceSipSession_ErrorOccuredL, Teardown)      

EUNIT_TEST(
	"DoErrorOccured - test",
	"CMceSipSession",
	"ErrorOccured",
	"FUNCTIONALITY",
	SetupL, UT_CMceSipSession_DoErrorOccured, Teardown)      

EUNIT_TEST(
	"Consume - test",
	"CMceSipSession",
	"Consumes",
	"FUNCTIONALITY",
	SetupL, UT_CMceSipSession_ConsumesL, Teardown) 

EUNIT_TEST(
	"MethodCode - test",
	"CMceSipSession",
	"MethodCode",
	"FUNCTIONALITY",
	SetupL, UT_CMceSipSession_MethodCodeL, Teardown) 
	
      
EUNIT_TEST(
	"IsSipDialogConfirmed - test",
	"CMceSipSession",
	"IsSipDialogConfirmed",
	"FUNCTIONALITY",
	SetupL, UT_CMceSipSession_IsSipDialogConfirmedL, Teardown)
	
EUNIT_END_TEST_TABLE

//  END OF FILE
