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
#include "UT_cmcemediamanager.h"
#include "MCETestHelper.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <digia/eunit/CEUnitAllocTestCaseDecorator.h>

//  INTERNAL INCLUDES
#include "mcemediamanager.h"
#include "mcesdpsession.h"
#include "mcecomsession.h"
#include "mcecomaudiostream.h"
#include "mcemediaobserver.h"
#include "mceamrcodec.h"
#include "mceg711codec.h"
#include "mcemediasdpcodec.h"

#include "mcecommicsource.h"
#include "mcecomspeakersink.h"
#include "mcecomrtpsource.h"
#include "mcertpsource.h"
#include "mcertpsink.h"
#include "mcecomrtpsink.h"
#include "mcemicsource.h"
#include "mcespeakersink.h"
#include "mceamrcodec.h"
#include "mceadoptedsrvstream.h"
#include "mcemediadefs.h"

#include "CMccController_stub.h"
#include "mcemediastate.h"
#include "cleanupresetanddestroy.h"


const TInt KVideo = 0;
const TInt KCamera = 1;
const TInt KRtpSource = 0;
const TInt KCameraSource = 0;
const TInt KRtpSink = 0;
const TInt KDisplaySink = 0;
const TInt KAudio = 0;
const TInt KSpeakerSink = 0;


#define SRVSTREAM( n ) iSession->MccStreams()[ n ]
#define SRVUPDATESTREAM( n ) iUpdate->MccStreams()[ n ]


#define PREPARESRVSTREAMS( n )\
    if ( n )\
        {\
        for( TInt i=0;i<n;i++)\
            {\
            iSession->MccStreams()[i]->Id() = i+10;\
            }\
        }


#define SETUP( CreateStreamL )\
    iSession = MceMediaManagerTestHelper::CreateStreamL();\
    iSession->SetObserver( *iServer )


#define FULL_SETUP( CreateStreamL )\
    if ( !iSession )\
        {\
        SETUP( CreateStreamL );\
        }\
    iManager->CreateSessionL( *iSession )


#define SETUP_AND_PREPARE( CreateStreamL )\
    SETUP( CreateStreamL );\
    iSession->PrepareL( *iManager )
    

#define FULL_SETUP_AND_PREPARE( CreateStreamL )\
    FULL_SETUP( CreateStreamL );\
    iSession->PrepareL( *iManager )


// CONSTRUCTION
UT_CMceMediaManager* UT_CMceMediaManager::NewL()
    {
    UT_CMceMediaManager* self = UT_CMceMediaManager::NewLC(); 
    CleanupStack::Pop( self );
    return self;
    }

UT_CMceMediaManager* UT_CMceMediaManager::NewLC()
    {
    UT_CMceMediaManager* self = new( ELeave ) UT_CMceMediaManager();
    CleanupStack::PushL( self );
	self->ConstructL(); 
    return self;
    }

// Destructor (virtual by CBase)
UT_CMceMediaManager::~UT_CMceMediaManager()
    {
    }

// Default constructor
UT_CMceMediaManager::UT_CMceMediaManager():
	iSession( NULL )
    {
    }

// Second phase construct
void UT_CMceMediaManager::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }


//  METHODS


void UT_CMceMediaManager::SetupL()
    {
	CMccControllerStub::OpenL();
    iMcc = CMccControllerStub::Stub();
    iServer = new (ELeave) CMceServerStub();
    iManager = CMceMediaManager::NewL( *iServer );
    
    RPointerArray<CMceComAudioCodec> audioCapabilities;
    RPointerArray<CMceComVideoCodec> videoCapabilities;
    MceCleanupResetAndDestroyPushL( audioCapabilities );
    MceCleanupResetAndDestroyPushL( videoCapabilities );
    
    iManager->GetCapabilitiesL( audioCapabilities );
    iManager->GetCapabilitiesL( videoCapabilities );
    
    CleanupStack::PopAndDestroy( 2 );//capas
    
    Reset();
    } 

void UT_CMceMediaManager::Setup_BasicL()
    {
	CMccControllerStub::OpenL();
    iMcc = CMccControllerStub::Stub();
    iServer = new (ELeave) CMceServerStub();
    iMcc->Reset();
    } 


void UT_CMceMediaManager::Teardown()
    {
    Reset();
    
	delete iSdpDocument; // received from the session
    iSdpDocument = NULL;
	CMceComSession::Delete( iUpdate, *iManager );
	CMceComSession::Delete( iSession, *iManager );

	delete iManager;
    iManager = NULL;
	delete iServer;
    iServer = NULL;
	CMccControllerStub::Close();
	iMcc = NULL;
    }

void UT_CMceMediaManager::UT_CMceMediaManager_BasicLL()
    {
    delete iManager;
    iManager = NULL;

	CMceMediaManager* manager = NULL;
	manager = CMceMediaManager::NewL( *iServer );
    CleanupStack::PushL( manager );

    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccGetSupportedCodecs );

    CleanupStack::PopAndDestroy( manager );
    }

void UT_CMceMediaManager::Reset()
    {
    if ( CMccControllerStub::Stub() )
    	{
    	iMcc->Reset();
    	}
    
    iServer->Reset();
    }
    

//
// Tests
//
// CSdpDocument* EncodeL( CMceComSession& aSession )
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_EncodeLL()
    {
    SETUP( CreateAudioOutSessionL );
    iSession->iClientCryptoSuites.Append( EAES_CM_128_HMAC_SHA1_32 );
    Reset();
    
    EUNIT_ASSERT( iSession->Streams().Count() == 1 );
    EUNIT_ASSERT( iSession->MccStreams().Count() == 0 );
    EUNIT_ASSERT( &iSession->NegotiationState() == NULL );
    EUNIT_ASSERT( &iSession->SdpSession() == NULL );
    EUNIT_ASSERT( iSession->iMccID == KMceNotAssigned );
    EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 0 );
    EUNIT_ASSERT( iManager->iSdpCodecs.Count() == 2 );
    
    //encode leaves
    MCE_EUNIT_ASSERT_LEAVE( iManager->EncodeL( *iSession ) );
    
    //update
    TMceReturnStatus status = iManager->UpdateL( *iSession );
    iSession->Modifier( KMceSecPreconditions ) = KMcePreconditionsE2ESupported;
    EUNIT_ASSERT( status == KMceAsync );
    
    //session asserts
    EUNIT_ASSERT( iSession->MccStreams().Count() == 2 );
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMcePreparingOffererStreams );
    EUNIT_ASSERT( &iSession->SdpSession() != NULL );
    EUNIT_ASSERT( iSession->SdpSession().MediaSession() == iSession );
    EUNIT_ASSERT( iSession->SdpSession().Backup() == NULL );
    EUNIT_ASSERT( iSession->iMccID == 0 );
    
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
	MCE_CHECK_MEMORY_LEAVE( iManager->iMceSdpSessions.Count() != 1 )
    //manager asserts
    EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 1 );
	
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccCreateSession );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccCreateLink );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::ENone );

    //srvstream  asserts
    EUNIT_ASSERT( SRVSTREAM(0)->StreamType() == CMceComMediaStream::EReceiveStream );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::ECreatingLink );
    EUNIT_ASSERT( SRVSTREAM(1)->StreamType() == CMceComMediaStream::ESendStream );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::ECreatingLink );
    
    //encode leaves
    EUNIT_ASSERT_LEAVE( iManager->EncodeL( *iSession ) );

    Reset();
    
    //1. link created
    MceMediaManagerTestHelper::LinkCreated( *iManager, SRVSTREAM(0)->LinkId() );
    
	MCE_CHECK_MEMORY_LEAVE( iMcc->iActions[ 0 ].iAction != CMccControllerStub::EMccGetSourceId )
    
	//actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccGetSourceId );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccGetSinkId );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccCreateStream );
    EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::EMccSetCodec );
    
    MCE_CHECK_MEMORY_LEAVE( iMcc->iActions[ 4 ].iAction != CMccControllerStub::EMccGetSourceId )
    EUNIT_ASSERT( iMcc->iActions[ 4 ].iAction == CMccControllerStub::EMccGetSourceId );
    EUNIT_ASSERT( iMcc->iActions[ 5 ].iAction == CMccControllerStub::EMccGetSinkId );
    EUNIT_ASSERT( iMcc->iActions[ 6 ].iAction == CMccControllerStub::EMccCreateStream );
    EUNIT_ASSERT( iMcc->iActions[ 7 ].iAction == CMccControllerStub::EMccSetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 8 ].iAction == CMccControllerStub::EMccPrepareStream );
    EUNIT_ASSERT( iMcc->iActions[ 9 ].iAction == CMccControllerStub::EMccPrepareStream );
    EUNIT_ASSERT( iMcc->iActions[ 10 ].iAction == CMccControllerStub::EMccGetSSRC );
    EUNIT_ASSERT( iMcc->iActions[ 11 ].iAction == CMccControllerStub::ENone );

    //srvstream  asserts
    EUNIT_ASSERT( SRVSTREAM(0)->StreamType() == CMceComMediaStream::EReceiveStream );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EPreparing );
    EUNIT_ASSERT( SRVSTREAM(1)->StreamType() == CMceComMediaStream::ESendStream );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EPreparing );

    Reset();
    
    //1. prepared
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    
    //session asserts
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMcePreparingOffererStreams );
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    //srvstream asserts
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EPrepared );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EPreparing );
    
    Reset();
    
    //2. prepared
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(1)->LinkId(),
                                SRVSTREAM(1)->Id(),
                                SRVSTREAM(1)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L                            
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(1)->LinkId(),
                                SRVSTREAM(1)->Id(),
                                SRVSTREAM(1)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 

	MCE_CHECK_MEMORY_LEAVE( iSession->NegotiationState().Id() != KMceStartingOffererReceiveStreams )
    //session asserts
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceStartingOffererReceiveStreams );
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStartStream );
    //always synchronizing
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccGetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccSetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::ENone );
    
    //srvstream asserts
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarting );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EPrepared );
                                
    Reset();
    
    //1. started
    iManager->MccMediaStarted( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                KNoEndpointId );
    MCE_MCC_EVENT_POSTCONDITION_L 

	MCE_CHECK_MEMORY_LEAVE( iSession->NegotiationState().Id() != KMceOfferingMedia )
    //session asserts
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceOfferingMedia );
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == EMceMediaUpdated );
    EUNIT_ASSERT( iServer->iSession == iSession );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    //srvstream asserts
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EPrepared );
    
    Reset();
    
    //encode 
    iSdpDocument = iManager->EncodeL( *iSession );
    
    HBufC8* sdpDocument = MceMediaManagerTestHelper::ToTextLC( *iSdpDocument );
    
    EUNIT_ASSERT( sdpDocument->Find( _L8("m=audio 5006 RTP/SAVP 96\r\n") ) > 0 );
    EUNIT_ASSERT( sdpDocument->Find( _L8("a=sendrecv\r\n") ) > 0 );
    EUNIT_ASSERT( sdpDocument->Find( _L8("a=fmtp:96 mode-set=0,1,2,3,4,5,6,7\r\n") ) > 0 );
    EUNIT_ASSERT( sdpDocument->Find( _L8("a=ptime:60\r\n") ) > 0 );
    EUNIT_ASSERT( sdpDocument->Find( _L8("a=maxptime:120\r\n") ) > 0 );
    EUNIT_ASSERT( sdpDocument->Find( _L8("a=client:session attribute 1\r\n") ) > 0 );
    EUNIT_ASSERT( sdpDocument->Find( _L8("a=client:session attribute 2\r\n") ) > 0 );
    EUNIT_ASSERT( sdpDocument->Find( _L8("a=client:media attribute 1\r\n") ) > 0 );
    EUNIT_ASSERT( sdpDocument->Find( _L8("a=client:media attribute 2\r\n") ) > 0 )
    EUNIT_ASSERT( sdpDocument->Find( _L8("a=curr:sec") ) > 0 );
    EUNIT_ASSERT( sdpDocument->Find( _L8("a=des:sec optional") ) > 0 );
    EUNIT_ASSERT( sdpDocument->Find( _L8("a=crypto:") ) > 0 );
    
    CleanupStack::PopAndDestroy( sdpDocument );
    
    //session asserts
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceOfferingMedia );
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    //srvstream asserts
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EPrepared );
    
    RPointerArray<CSdpMediaField>& mediafields = iSdpDocument->MediaFields();
    EUNIT_ASSERT( mediafields.Count() == 1 );
    }


//
// Tests
//
// TMceSipWarningCode DecodeL( CMceComSession& aSession,
//                             CSdpDocument& aSdpDocument )
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_DecodeLL()
    {

//
//Decode answer
//
    SETUP( CreateAudioOutSessionL );
    
    
    Reset();
    //update
    TMceReturnStatus status = iManager->UpdateL( *iSession );
    EUNIT_ASSERT( status == KMceAsync );
    
//decode leaves
    MCE_EUNIT_ASSERT_LEAVE( iManager->DecodeL( *iSession, *iSdpDocument ) );
    
//1. link created
    MceMediaManagerTestHelper::LinkCreated( *iManager, SRVSTREAM(0)->LinkId() );
//1. prepared
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
//2. prepared
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(1)->LinkId(),
                                SRVSTREAM(1)->Id(),
                                SRVSTREAM(1)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(1)->LinkId(),
                                SRVSTREAM(1)->Id(),
                                SRVSTREAM(1)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
//1. started
    iManager->MccMediaStarted( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    iManager->MccMediaStarted( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    Reset();
//encode 
    iSdpDocument = iManager->EncodeL( *iSession );

    //session asserts
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceOfferingMedia );
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    //srvstream asserts
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EPrepared );
    
    RPointerArray<CSdpMediaField>& mediafields = iSdpDocument->MediaFields();
    EUNIT_ASSERT( mediafields.Count() == 1 );

    delete iSdpDocument;
    iSdpDocument = NULL;
    
    iSdpDocument = CSdpDocument::DecodeL( KMceMMTestSdpAnswerAudioIMS5PrecsSendrecv );

    Reset();
//decode answer
    iSession->AnswerType() = KMceNegotiationAnswerTypeFinal;
    TMceSipWarningCode decodeStatus = iManager->DecodeL( *iSession, *iSdpDocument );
    EUNIT_ASSERT( decodeStatus == KErrNone );

    //session asserts
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceOfferingMedia );
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    //srvstream asserts
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EPrepared );

    CMceComSession::Delete( iSession, *iManager );
    Reset();

//
//Decode offer
//
    delete iSdpDocument;
    iSdpDocument = NULL;
    SETUP( CreateInSessionL );
    iSession->iUseRemotePreconditions = ETrue;
    EUNIT_ASSERT( iSession->MccStreams().Count() == 0 );
    //manager asserts
    EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 0 );
    
    iSdpDocument = CSdpDocument::DecodeL( KMceMMTestSdpOfferAudioIMS5PrecsSendrecv );
    decodeStatus = iManager->DecodeL( *iSession, *iSdpDocument );
    EUNIT_ASSERT( decodeStatus == KErrNone );
    
    EUNIT_ASSERT( iSession->Streams().Count() == 1 );
    EUNIT_ASSERT( iSession->Streams()[0]->iRemoteMediaPort == 5078 );
    EUNIT_ASSERT( iSession->Streams()[0]->BoundStream() );
    EUNIT_ASSERT( iSession->Streams()[0]->iStreamType == CMceComMediaStream::EReceiveStream );
    EUNIT_ASSERT( iSession->Streams()[0]->BoundStreamL().iStreamType == CMceComMediaStream::ESendStream );
    EUNIT_ASSERT( iSession->Streams()[0]->Preconditions().Count() != NULL );
    EUNIT_ASSERT( static_cast<CMceComAudioStream*>( iSession->Streams()[0] )->Codecs().Count() == 1 );
    EUNIT_ASSERT( static_cast<CMceComAudioStream*>( iSession->Streams()[0] )->Codecs()[0]->iSdpName.Compare( _L8("AMR") ) == 0 );
    EUNIT_ASSERT( static_cast<CMceComAudioStream*>( iSession->Streams()[0] )->Codecs()[0]->iPayloadType == 96 );
    EUNIT_ASSERT( static_cast<CMceComAudioStream*>( iSession->Streams()[0] )->Codecs()[0]->iFmtpAttr->Compare( _L8("octet-align=0; mode-set=0,1,2,3,4,5,6,7") ) == 0 );

    //manager asserts
    EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 1 );
    //session asserts
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceMediaIdle );
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    //srvstream asserts
    EUNIT_ASSERT( iSession->MccStreams().Count() == 0 );

    CMceComSession::Delete( iSession, *iManager );
    }

//
// Tests
//
// TMceReturnStatus UpdateL( CMceComSession& aSession )
// (MO audio)
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_UpdateLL()
    {
    SETUP( CreateAudioOutSessionL );
    
    EUNIT_ASSERT( iSession->Streams().Count() == 1 );
    EUNIT_ASSERT( iSession->MccStreams().Count() == 0 );
    EUNIT_ASSERT( &iSession->NegotiationState() == NULL );
    EUNIT_ASSERT( &iSession->SdpSession() == NULL );
    EUNIT_ASSERT( iSession->iMccID == KMceNotAssigned );
    EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 0 );
    EUNIT_ASSERT( iManager->iSdpCodecs.Count() == 2 );
    
    Reset();
    
    //update
    TMceReturnStatus status = iManager->UpdateL( *iSession );
    EUNIT_ASSERT( status == KMceAsync );
    
    //1. link created
    MceMediaManagerTestHelper::LinkCreated( *iManager, SRVSTREAM(0)->LinkId() );
    
    //1. prepared
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    
    //2. prepared
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(1)->LinkId(),
                                SRVSTREAM(1)->Id(),
                                SRVSTREAM(1)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(1)->LinkId(),
                                SRVSTREAM(1)->Id(),
                                SRVSTREAM(1)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    
    Reset();
    
    //1. started
    iManager->MccMediaStarted( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    
    iManager->MccMediaStarted( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
	
    MCE_CHECK_MEMORY_LEAVE( iServer->iEvent != EMceMediaUpdated )
    
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == EMceMediaUpdated );
    EUNIT_ASSERT( iServer->iSession == iSession );
    
    //update leaves
    MCE_EUNIT_ASSERT_LEAVE( iManager->UpdateL( *iSession ) );
    Reset();
    
    //encode 
    iSdpDocument = iManager->EncodeL( *iSession );

    delete iSdpDocument;
    iSdpDocument = NULL;
    iSdpDocument = CSdpDocument::DecodeL( KMceMMTestSdpAnswerAudioIMS5PrecsSendrecv );

    Reset();
//decode answer 
    iSession->AnswerType() = KMceNegotiationAnswerTypeFinal;
    TMceSipWarningCode decodeStatus = iManager->DecodeL( *iSession, *iSdpDocument );
    EUNIT_ASSERT( decodeStatus == KErrNone );

    //session asserts
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceOfferingMedia );
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    //srvstream asserts
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EPrepared );
    Reset();
    
    //update
    status = iManager->UpdateL( *iSession );
    EUNIT_ASSERT( status == KMceAsync );

    //session asserts
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceStartingOffererSendStreams );
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccGetCodec );//validate & adjust
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccSetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccSetRemoteAddress );
    EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::EMccStartStream );
    
    //always synchronizing
    EUNIT_ASSERT( iMcc->iActions[ 4 ].iAction == CMccControllerStub::EMccGetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 5 ].iAction == CMccControllerStub::EMccSetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 6 ].iAction == CMccControllerStub::ENone );
    
    //srvstream asserts
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarting );
    Reset();
    
    //2. started
    iManager->MccMediaStarted( iSession->iMccID,
                               SRVSTREAM(1)->LinkId(),
                               SRVSTREAM(1)->Id(),
                               KNoEndpointId );
    
    MCE_MCC_EVENT_POSTCONDITION_L 
    
    //session asserts
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceMediaNegotiated );
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == EMceMediaUpdated );
    EUNIT_ASSERT( iServer->iSession == iSession );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    //srvstream asserts
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    
    CMceComSession::Delete( iSession, *iManager );
    
    EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 0 );
    EUNIT_ASSERT( iManager->iSdpCodecs.Count() == 2 );
    }




//
// Tests
//
// TMceReturnStatus UpdateL( CMceComSession& aSession )
// (MO live video)
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_UpdateL_1L()
    {

    SETUP( CreateVideoOutSessionL );

    EUNIT_ASSERT( iSession->Streams().Count() == 2 );
    EUNIT_ASSERT( iSession->MccStreams().Count() == 0 );
    EUNIT_ASSERT( &iSession->NegotiationState() == NULL );
    EUNIT_ASSERT( &iSession->SdpSession() == NULL );
    EUNIT_ASSERT( iSession->iMccID == KMceNotAssigned );
    EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 0 );
    EUNIT_ASSERT( iManager->iSdpCodecs.Count() == 2 );
    
    Reset();
    
//update
    TMceReturnStatus status = iManager->UpdateL( *iSession );
    EUNIT_ASSERT( status == KMceAsync );

    //session asserts
    EUNIT_ASSERT( iSession->MccStreams().Count() == 3 );
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMcePreparingOffererStreams );
    EUNIT_ASSERT( &iSession->SdpSession() != NULL );
    EUNIT_ASSERT( iSession->SdpSession().MediaSession() == iSession );
    EUNIT_ASSERT( iSession->SdpSession().Backup() == NULL );
    EUNIT_ASSERT( iSession->iMccID == 0 );
    
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );

    //manager asserts
    EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 1 );

    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccCreateSession );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccCreateLink );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::ENone );

    //srvstream  asserts
    EUNIT_ASSERT( SRVSTREAM(0)->StreamType() == CMceComMediaStream::EReceiveStream );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::ECreatingLink );
    EUNIT_ASSERT( !SRVSTREAM(0)->IsMccPrepared() );
    EUNIT_ASSERT( SRVSTREAM(1)->StreamType() == CMceComMediaStream::ESendStream );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::ECreatingLink );
    EUNIT_ASSERT( !SRVSTREAM(1)->IsMccPrepared() );
    EUNIT_ASSERT( SRVSTREAM(2)->StreamType() == CMceComMediaStream::ELocalStream );
    EUNIT_ASSERT( SRVSTREAM(2)->State() == CMceSrvStream::ECreatingLink );
    EUNIT_ASSERT( !SRVSTREAM(2)->IsMccPrepared() );
    EUNIT_ASSERT( SRVSTREAM(2)->Id() == KMceNotAssigned );
    EUNIT_ASSERT( SRVSTREAM(2)->LinkId() != KMceNotAssigned );
    EUNIT_ASSERT( SRVSTREAM(2)->Source().Id() == KMceNotAssigned );
    EUNIT_ASSERT( SRVSTREAM(2)->Sink().Id() == KMceNotAssigned );

    Reset();    

//1. link created
    MceMediaManagerTestHelper::LinkCreated( *iManager, SRVSTREAM(0)->LinkId() );


	MCE_CHECK_MEMORY_LEAVE( iMcc->iActions[ 0 ].iAction != CMccControllerStub::EMccGetSourceId )
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccGetSourceId );
    
    MCE_CHECK_MEMORY_LEAVE( iMcc->iActions[ 1 ].iAction != CMccControllerStub::EMccGetSinkId )
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccGetSinkId );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccCreateStream );
    EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::EMccSetCodec );
    MCE_CHECK_MEMORY_LEAVE( iMcc->iActions[ 4 ].iAction != CMccControllerStub::EMccGetSourceId )
    EUNIT_ASSERT( iMcc->iActions[ 4 ].iAction == CMccControllerStub::EMccGetSourceId );
    EUNIT_ASSERT( iMcc->iActions[ 5 ].iAction == CMccControllerStub::EMccGetSinkId );
    EUNIT_ASSERT( iMcc->iActions[ 6 ].iAction == CMccControllerStub::EMccCreateStream );
    MCE_CHECK_MEMORY_LEAVE( iMcc->iActions[ 7 ].iAction != CMccControllerStub::EMccSetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 7 ].iAction == CMccControllerStub::EMccSetCodec );
    MCE_CHECK_MEMORY_LEAVE( iMcc->iActions[ 8 ].iAction != CMccControllerStub::EMccPrepareStream );
    EUNIT_ASSERT( iMcc->iActions[ 8 ].iAction == CMccControllerStub::EMccPrepareStream );
    EUNIT_ASSERT( iMcc->iActions[ 9 ].iAction == CMccControllerStub::EMccPrepareStream );
    EUNIT_ASSERT( iMcc->iActions[ 10 ].iAction == CMccControllerStub::EMccGetSSRC );
    EUNIT_ASSERT( iMcc->iActions[ 11 ].iAction == CMccControllerStub::ENone );

    //srvstream  asserts
    EUNIT_ASSERT( SRVSTREAM(0)->StreamType() == CMceComMediaStream::EReceiveStream );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EPreparing );
    EUNIT_ASSERT( SRVSTREAM(0)->IsMccPrepared() );
    EUNIT_ASSERT( SRVSTREAM(1)->StreamType() == CMceComMediaStream::ESendStream );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EPreparing );
    EUNIT_ASSERT( SRVSTREAM(1)->IsMccPrepared() );
    EUNIT_ASSERT( SRVSTREAM(2)->StreamType() == CMceComMediaStream::ELocalStream );
    EUNIT_ASSERT( SRVSTREAM(2)->State() == CMceSrvStream::EPrepared );
    EUNIT_ASSERT( !SRVSTREAM(2)->IsMccPrepared() );
    
    Reset();
//1. prepared
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 

    //session asserts
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMcePreparingOffererStreams );
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    //srvstream asserts
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EPrepared );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EPreparing );
    EUNIT_ASSERT( SRVSTREAM(2)->State() == CMceSrvStream::EPrepared );

    Reset();
//2. prepared
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(1)->LinkId(),
                                SRVSTREAM(1)->Id(),
                                SRVSTREAM(1)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(1)->LinkId(),
                                SRVSTREAM(1)->Id(),
                                SRVSTREAM(1)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    //session asserts
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceStartingOffererReceiveStreams );
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );//congigKey
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccGetCodec );//congigKey
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccSetCodec );//congigKey
    EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::EMccStreamSettings );//local
    EUNIT_ASSERT( iMcc->iActions[ 3 ].iKey == KMccCamLocalVideo );
    EUNIT_ASSERT( iMcc->iActions[ 4 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 5 ].iAction == CMccControllerStub::EMccStartStream );
    EUNIT_ASSERT( iMcc->iActions[ 6 ].iAction == CMccControllerStub::EMccGetCodec );//validate
    EUNIT_ASSERT( iMcc->iActions[ 7 ].iAction == CMccControllerStub::EMccSetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 8 ].iAction == CMccControllerStub::ENone );
    
    //srvstream asserts
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarting );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EPrepared );
    EUNIT_ASSERT( SRVSTREAM(2)->State() == CMceSrvStream::EStarted );
                                
    Reset();
//1. started
    iManager->MccMediaStarted( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    iManager->MccMediaStarted( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
                     
    MCE_CHECK_MEMORY_LEAVE( iSession->NegotiationState().Id() != KMceOfferingMedia )           
    //session asserts
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceOfferingMedia );
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == EMceMediaUpdated );
    EUNIT_ASSERT( iServer->iSession == iSession );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    //srvstream asserts
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EPrepared );
    EUNIT_ASSERT( SRVSTREAM(2)->State() == CMceSrvStream::EStarted );
    

    Reset();
//encode 

    iSdpDocument = iManager->EncodeL( *iSession );

    delete iSdpDocument;
    iSdpDocument = NULL;
    
    iSdpDocument = CSdpDocument::DecodeL( KMceMMTestSdpAnswerVideoIMS5PrecsSendrecv );
// not work
    Reset();
//decode answer    
    iSession->AnswerType() = KMceNegotiationAnswerTypeFinal;
    TMceSipWarningCode decodeStatus = iManager->DecodeL( *iSession, *iSdpDocument );
    EUNIT_ASSERT( decodeStatus == KErrNone );

    //session asserts
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceOfferingMedia );
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    //srvstream asserts
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EPrepared );
    EUNIT_ASSERT( SRVSTREAM(2)->State() == CMceSrvStream::EStarted );
    

    Reset();
//update
    status = iManager->UpdateL( *iSession );
    EUNIT_ASSERT( status == KMceAsync );

    //session asserts
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceStartingOffererSendStreams );
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccGetCodec );//always sync
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccSetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccStreamSettings );//local
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iKey == KMccCamLocalVideo );
    EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::EMccSetRemoteAddress );
    EUNIT_ASSERT( iMcc->iActions[ 4 ].iAction == CMccControllerStub::EMccStartStream );
    EUNIT_ASSERT( iMcc->iActions[ 5 ].iAction == CMccControllerStub::EMccGetCodec );//validate
    EUNIT_ASSERT( iMcc->iActions[ 6 ].iAction == CMccControllerStub::EMccSetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 7 ].iAction == CMccControllerStub::ENone );
    
    //srvstream asserts
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarting );
    EUNIT_ASSERT( SRVSTREAM(2)->State() == CMceSrvStream::EStarted );


    Reset();
//2. started
    iManager->MccMediaStarted( iSession->iMccID,
                               SRVSTREAM(1)->LinkId(),
                               SRVSTREAM(1)->Id(),
                               KNoEndpointId );
    MCE_MCC_EVENT_POSTCONDITION_L 
    
    //session asserts
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceMediaNegotiated );
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == EMceMediaUpdated );
    EUNIT_ASSERT( iServer->iSession == iSession );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    //srvstream asserts
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->State() == CMceSrvStream::EStarted );

    CMceComSession::Delete( iSession, *iManager );

    EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 0 );
    EUNIT_ASSERT( iManager->iSdpCodecs.Count() == 2 );


    }



//
// Tests
//
// TMceReturnStatus UpdateL( CMceComSession& aSession )
// (MT multimedia)
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_UpdateL_2L()
    {

    SETUP( CreateInSessionL );
    
    EUNIT_ASSERT( iSession->Streams().Count() == 0 );
    EUNIT_ASSERT( iSession->MccStreams().Count() == 0 );
    EUNIT_ASSERT( &iSession->NegotiationState() == NULL );
    EUNIT_ASSERT( &iSession->SdpSession() == NULL );
    EUNIT_ASSERT( iSession->iMccID == KMceNotAssigned );
    EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 0 );
    EUNIT_ASSERT( iManager->iSdpCodecs.Count() == 2 );
    
    Reset();
    iSdpDocument = CSdpDocument::DecodeL( KMceMMTestSdpOfferAudioVideoIMS5PrecsSendrecv );
    TMceSipWarningCode decodeStatus = iManager->DecodeL( *iSession, *iSdpDocument );
    EUNIT_ASSERT( decodeStatus == KErrNone );

    //manager asserts
    EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 1 );
    //session asserts
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceMediaIdle );
    EUNIT_ASSERT( iSession->Streams().Count() == 2 );
    EUNIT_ASSERT( iSession->iMccID == KMceNotAssigned );
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    //srvstream asserts
    EUNIT_ASSERT( iSession->MccStreams().Count() == 0 );

    Reset();
//update
    TMceReturnStatus status = iManager->UpdateL( *iSession );
    EUNIT_ASSERT( status == KMceReady  );
    
    //session asserts
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceAnsweringMedia );
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    //srvstream asserts
    EUNIT_ASSERT( iSession->MccStreams().Count() == 4 );
    EUNIT_ASSERT( SRVSTREAM(0)->StreamType() == CMceComMediaStream::EReceiveStream );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::ECreated );
    EUNIT_ASSERT( SRVSTREAM(1)->StreamType() == CMceComMediaStream::ESendStream );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::ECreated );
    EUNIT_ASSERT( SRVSTREAM(2)->StreamType() == CMceComMediaStream::EReceiveStream );
    EUNIT_ASSERT( SRVSTREAM(2)->State() == CMceSrvStream::ECreated );
    EUNIT_ASSERT( SRVSTREAM(3)->StreamType() == CMceComMediaStream::ESendStream );
    EUNIT_ASSERT( SRVSTREAM(3)->State() == CMceSrvStream::ECreated );

    CMceComMediaStream* local = 
        MceMediaManagerTestHelper::CreateLocalVideoStreamL( 
            static_cast<CMceComMediaSource*>( &SRVSTREAM(3)->Source().Data() ) );
    CleanupStack::PushL( local );
    iSession->AddStreamL( local );
    CleanupStack::Pop( local );
    iSession->InitializeL();
    
    Reset();
//update
    status = iManager->UpdateL( *iSession );
    EUNIT_ASSERT( status == KMceAsync  );
    
    //session asserts
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMcePreparingAnswererStreams );
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccCreateSession );
    
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccCreateLink );//1
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccCreateLink );//2
    EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::ENone );//2
    
    EUNIT_ASSERT( iSession->MccStreams().Count() == 5 );
    EUNIT_ASSERT( SRVSTREAM(0)->StreamType() == CMceComMediaStream::EReceiveStream );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::ECreatingLink );
    EUNIT_ASSERT( SRVSTREAM(1)->StreamType() == CMceComMediaStream::ESendStream );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::ECreatingLink );
    EUNIT_ASSERT( SRVSTREAM(2)->StreamType() == CMceComMediaStream::EReceiveStream );
    EUNIT_ASSERT( SRVSTREAM(2)->State() == CMceSrvStream::ECreatingLink );
    EUNIT_ASSERT( SRVSTREAM(3)->StreamType() == CMceComMediaStream::ESendStream );
    EUNIT_ASSERT( SRVSTREAM(3)->State() == CMceSrvStream::ECreatingLink );
    EUNIT_ASSERT( SRVSTREAM(4)->StreamType() == CMceComMediaStream::ELocalStream );
    EUNIT_ASSERT( SRVSTREAM(4)->State() == CMceSrvStream::ECreatingLink );

    Reset();
    //1. link created
    MceMediaManagerTestHelper::LinkCreated( *iManager, SRVSTREAM(0)->LinkId() );
    
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::ELinkCreated );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::ELinkCreated );
    EUNIT_ASSERT( SRVSTREAM(2)->State() == CMceSrvStream::ECreatingLink );
    EUNIT_ASSERT( SRVSTREAM(3)->State() == CMceSrvStream::ECreatingLink );
    EUNIT_ASSERT( SRVSTREAM(4)->State() == CMceSrvStream::ECreatingLink );
    
    Reset();
    //2. link created
    MceMediaManagerTestHelper::LinkCreated( *iManager, SRVSTREAM(2)->LinkId() );
    
	MCE_CHECK_MEMORY_LEAVE( SRVSTREAM(0)->State() != CMceSrvStream::EPreparing )
	
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EPreparing );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EPreparing );
    EUNIT_ASSERT( SRVSTREAM(2)->State() == CMceSrvStream::EPreparing );
    EUNIT_ASSERT( SRVSTREAM(3)->State() == CMceSrvStream::EPreparing );
    EUNIT_ASSERT( SRVSTREAM(4)->State() == CMceSrvStream::EPrepared );
    
    for ( TInt i = 0; i < 4 ; i = i + 2 )
        {
        Reset();
        //1. prepared
        iManager->MccMediaPrepared( iSession->iMccID,
                                    SRVSTREAM(i)->LinkId(),
                                    SRVSTREAM(i)->Id(),
                                    SRVSTREAM(i)->Sink().Id() );
        MCE_MCC_EVENT_POSTCONDITION_L 
        
        iManager->MccMediaPrepared( iSession->iMccID,
                                    SRVSTREAM(i)->LinkId(),
                                    SRVSTREAM(i)->Id(),
                                    SRVSTREAM(i)->Source().Id() );
        MCE_MCC_EVENT_POSTCONDITION_L 
        
        //session asserts
        EUNIT_ASSERT( iSession->NegotiationState().Id() == KMcePreparingAnswererStreams );
        //server asserts
        EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
        EUNIT_ASSERT( iServer->iSession == NULL );
        
        //actions asserts
        EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
        //srvstream asserts
        EUNIT_ASSERT( SRVSTREAM(i)->State() == CMceSrvStream::EPrepared );
        
        Reset();
        
        //2. prepared
        iManager->MccMediaPrepared( iSession->iMccID,
                                    SRVSTREAM(i+1)->LinkId(),
                                    SRVSTREAM(i+1)->Id(),
                                    SRVSTREAM(i+1)->Sink().Id() );
        MCE_MCC_EVENT_POSTCONDITION_L 
        
        iManager->MccMediaPrepared( iSession->iMccID,
                                    SRVSTREAM(i+1)->LinkId(),
                                    SRVSTREAM(i+1)->Id(),
                                    SRVSTREAM(i+1)->Source().Id() );
        MCE_MCC_EVENT_POSTCONDITION_L 
        
        if ( i + 1 == 3 )
            {
            MCE_CHECK_MEMORY_LEAVE( iSession->NegotiationState().Id() != KMceStartingAnswererStreams )
            //session asserts
            EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceStartingAnswererStreams );
            //actions asserts
            EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );//congigKey
            EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccGetCodec );//congigKey
            EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccSetCodec );//congigKey
            EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::EMccStreamSettings );
            EUNIT_ASSERT( iMcc->iActions[ 3 ].iKey == KMccCamLocalVideo );
            EUNIT_ASSERT( iMcc->iActions[ 4 ].iAction == CMccControllerStub::EMccStreamSettings );
            EUNIT_ASSERT( iMcc->iActions[ 5 ].iAction == CMccControllerStub::EMccSetRemoteAddress );
            
            EUNIT_ASSERT( iMcc->iActions[ 6 ].iAction == CMccControllerStub::EMccStartStream );
            EUNIT_ASSERT( iMcc->iActions[ 7 ].iAction == CMccControllerStub::EMccGetCodec );
            EUNIT_ASSERT( iMcc->iActions[ 8 ].iAction == CMccControllerStub::EMccSetCodec );
            EUNIT_ASSERT( iMcc->iActions[ 9 ].iAction == CMccControllerStub::EMccSetRemoteAddress );
            EUNIT_ASSERT( iMcc->iActions[ 10 ].iAction == CMccControllerStub::EMccStartStream );
            EUNIT_ASSERT( iMcc->iActions[ 11 ].iAction == CMccControllerStub::EMccGetCodec );
            EUNIT_ASSERT( iMcc->iActions[ 12 ].iAction == CMccControllerStub::EMccSetCodec );
            EUNIT_ASSERT( iMcc->iActions[ 13 ].iAction == CMccControllerStub::EMccSetRemoteAddress );
            EUNIT_ASSERT( iMcc->iActions[ 14 ].iAction == CMccControllerStub::EMccStartStream );
            EUNIT_ASSERT( iMcc->iActions[ 15 ].iAction == CMccControllerStub::EMccGetCodec );
            EUNIT_ASSERT( iMcc->iActions[ 16 ].iAction == CMccControllerStub::EMccSetCodec );
            EUNIT_ASSERT( iMcc->iActions[ 17 ].iAction == CMccControllerStub::EMccSetRemoteAddress );
            EUNIT_ASSERT( iMcc->iActions[ 18 ].iAction == CMccControllerStub::EMccStartStream );
            EUNIT_ASSERT( iMcc->iActions[ 19 ].iAction == CMccControllerStub::EMccGetCodec );
            EUNIT_ASSERT( iMcc->iActions[ 20 ].iAction == CMccControllerStub::EMccSetCodec );
            EUNIT_ASSERT( iMcc->iActions[ 21 ].iAction == CMccControllerStub::ENone);
            //srvstream asserts
            EUNIT_ASSERT( SRVSTREAM(i+1)->State() == CMceSrvStream::EStarting );
            }
        else
            {
            EUNIT_ASSERT( iSession->NegotiationState().Id() == KMcePreparingAnswererStreams );
            EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
            //srvstream asserts
            EUNIT_ASSERT( SRVSTREAM(i+1)->State() == CMceSrvStream::EPrepared );
            }
        //server asserts
        EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
        EUNIT_ASSERT( iServer->iSession == NULL );
        }
         
    for( TInt j = 0; j < 4; j++ )
        {
        Reset();
    //1. started
        iManager->MccMediaStarted( iSession->iMccID,
                                    SRVSTREAM(j)->LinkId(),
                                    SRVSTREAM(j)->Id(),
                                    SRVSTREAM(j)->Sink().Id() );
        MCE_MCC_EVENT_POSTCONDITION_L 
        iManager->MccMediaStarted( iSession->iMccID,
                                    SRVSTREAM(j)->LinkId(),
                                    SRVSTREAM(j)->Id(),
                                    SRVSTREAM(j)->Source().Id() );
        MCE_MCC_EVENT_POSTCONDITION_L 

        if( j < 3 )
            {
            //session asserts
            EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceStartingAnswererStreams );
            //server asserts
            EUNIT_ASSERT( iServer->iEvent == EMceMediaEventReceived );
            EUNIT_ASSERT( iServer->iSession == NULL );
            }
        else
            {
            MCE_CHECK_MEMORY_LEAVE( iSession->NegotiationState().Id() != KMceMediaNegotiated )
            //session asserts
            EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceMediaNegotiated );
            //server asserts
            EUNIT_ASSERT( iServer->iEvent == EMceMediaUpdated );
            EUNIT_ASSERT( iServer->iSession == iSession );
            }
        
        //actions asserts
        EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
        
        //srvstream asserts
        EUNIT_ASSERT( SRVSTREAM(j)->State() == CMceSrvStream::EStarted );
        }
        
    
    //srvstream asserts
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(3)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(4)->State() == CMceSrvStream::EStarted );
    
    Reset();
//encode 
    iSdpDocument = iManager->EncodeL( *iSession );

    //session asserts
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceMediaNegotiated );
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    //srvstream asserts
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(3)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(4)->State() == CMceSrvStream::EStarted );
    
    RPointerArray<CSdpMediaField>& mediafields = iSdpDocument->MediaFields();
    EUNIT_ASSERT( mediafields.Count() >= 1 );
    }


//
// Tests
//
// TMceReturnStatus UpdateL( CMceComSession& aSession )
// (session update with signalling )
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_UpdateL_3L()
    {
    SETUP( CreateAudioOutSessionL );
    
//update

    TMceReturnStatus status = iManager->UpdateL( *iSession );
    
    EUNIT_ASSERT( status == KMceAsync );
    
//1. link created
    MceMediaManagerTestHelper::LinkCreated( *iManager, SRVSTREAM(0)->LinkId() );

//1. prepared
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
                                
//2. prepared
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(1)->LinkId(),
                                SRVSTREAM(1)->Id(),
                                SRVSTREAM(1)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(1)->LinkId(),
                                SRVSTREAM(1)->Id(),
                                SRVSTREAM(1)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    Reset();                                
//1. started
    iManager->MccMediaStarted( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    iManager->MccMediaStarted( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
	
	MCE_CHECK_MEMORY_LEAVE( iServer->iEvent != EMceMediaUpdated )
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == EMceMediaUpdated );
    EUNIT_ASSERT( iServer->iSession == iSession );


//encode 
    iSdpDocument = iManager->EncodeL( *iSession );

    delete iSdpDocument;
    iSdpDocument = NULL;
    iSdpDocument = CSdpDocument::DecodeL( KMceMMTestSdpAnswerAudioIMS5PrecsSendrecv );

//decode answer
    iSession->AnswerType() = KMceNegotiationAnswerTypeFinal;
    TMceSipWarningCode decodeStatus = iManager->DecodeL( *iSession, *iSdpDocument );
    
//update
    status = iManager->UpdateL( *iSession );
    
//2. started
    iManager->MccMediaStarted( iSession->iMccID,
                               SRVSTREAM(1)->LinkId(),
                               SRVSTREAM(1)->Id(),
                               KNoEndpointId );
    MCE_MCC_EVENT_POSTCONDITION_L 
    
    Reset();
//
//update
//    
    iUpdate = iSession->CloneL();
    iUpdate->SetObserver( *iServer );
    CMceComAudioStream* audio = static_cast<CMceComAudioStream*>( iUpdate->Streams()[0] );
    audio->Codecs()[0]->iPTime = audio->Codecs()[0]->iPTime + 40;

    EUNIT_ASSERT( iSession->Streams().Count() == 1 );
    EUNIT_ASSERT( iSession->MccStreams().Count() == 2 );
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceMediaNegotiated );
    EUNIT_ASSERT( &iSession->SdpSession() != NULL );
    EUNIT_ASSERT( iSession->iMccID != KMceNotAssigned );
    EUNIT_ASSERT( !iSession->IsMerged() );
    
    EUNIT_ASSERT( iUpdate->Streams().Count() == 1 );
    EUNIT_ASSERT( iUpdate->MccStreams().Count() == 0 );
    EUNIT_ASSERT( &iUpdate->NegotiationState() == NULL );
    EUNIT_ASSERT( iUpdate->iMccID == KMceNotAssigned );
    EUNIT_ASSERT( !iUpdate->IsMerged() );
    
    
    EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 1 );
    EUNIT_ASSERT( iSession->SdpSession().MediaSession() == iSession );
    EUNIT_ASSERT( iSession->SdpSession().Backup() == NULL );
    
//update
    // Update may fail also with KErrArgument in oom situation
    MCE_LEAVE_HANDLING( status = iManager->UpdateL( *iUpdate ), KErrArgument );
    EUNIT_ASSERT( status == KMceAsync );
    
    //session asserts
    EUNIT_ASSERT( iUpdate->MccStreams().Count() == 2 );
    EUNIT_ASSERT( iUpdate->NegotiationState().Id() == KMcePreparingOffererStreams );
    EUNIT_ASSERT( &iSession->SdpSession() != NULL );
    EUNIT_ASSERT( iSession->SdpSession().MediaSession() == iUpdate );
    EUNIT_ASSERT( iSession->SdpSession().Backup() == iSession );
    EUNIT_ASSERT( !iUpdate->IsMerged() );
    EUNIT_ASSERT( !iSession->IsMerged() );
    
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );

    //manager asserts
    EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 1 );

    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccGetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccCreateSession );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccCreateLink );
    EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::ENone );

    //srvstream  asserts
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->StreamType() == CMceComMediaStream::EReceiveStream );
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->State() == CMceSrvStream::ECreatingLink );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->StreamType() == CMceComMediaStream::ESendStream );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->State() == CMceSrvStream::ECreatingLink );
    
    //backup asserts
    EUNIT_ASSERT( iSession->MccStreams().Count() == 2 );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );

    Reset();
//1. link created
    MceMediaManagerTestHelper::LinkCreated( *iManager, SRVUPDATESTREAM(0)->LinkId(), iUpdate->iMccID );

	MCE_CHECK_MEMORY_LEAVE( iMcc->iActions[ 0 ].iAction != CMccControllerStub::EMccGetSourceId )
	
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccGetSourceId );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccGetSinkId );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccCreateStream );
    EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::EMccSetCodec );

	MCE_CHECK_MEMORY_LEAVE( iMcc->iActions[ 4 ].iAction != CMccControllerStub::EMccGetSourceId )
    EUNIT_ASSERT( iMcc->iActions[ 4 ].iAction == CMccControllerStub::EMccGetSourceId );
    EUNIT_ASSERT( iMcc->iActions[ 5 ].iAction == CMccControllerStub::EMccGetSinkId );
    EUNIT_ASSERT( iMcc->iActions[ 6 ].iAction == CMccControllerStub::EMccCreateStream );
    EUNIT_ASSERT( iMcc->iActions[ 7 ].iAction == CMccControllerStub::EMccSetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 8 ].iAction == CMccControllerStub::EMccPrepareStream );
    EUNIT_ASSERT( iMcc->iActions[ 9 ].iAction == CMccControllerStub::EMccPrepareStream );
    EUNIT_ASSERT( iMcc->iActions[ 10 ].iAction == CMccControllerStub::EMccGetSSRC );
    EUNIT_ASSERT( iMcc->iActions[ 11 ].iAction == CMccControllerStub::ENone );
    
    Reset();
//1. prepared
    iManager->MccMediaPrepared( iUpdate->iMccID,
                                SRVUPDATESTREAM(0)->LinkId(),
                                SRVUPDATESTREAM(0)->Id(),
                                SRVUPDATESTREAM(0)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    iManager->MccMediaPrepared( iUpdate->iMccID,
                                SRVUPDATESTREAM(0)->LinkId(),
                                SRVUPDATESTREAM(0)->Id(),
                                SRVUPDATESTREAM(0)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L                            

    //session asserts
    EUNIT_ASSERT( iUpdate->NegotiationState().Id() == KMcePreparingOffererStreams );
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    //srvstream asserts
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->State() == CMceSrvStream::EPrepared );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->State() == CMceSrvStream::EPreparing );
    //backup asserts
    EUNIT_ASSERT( iSession->MccStreams().Count() == 2 );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
                                
    Reset();
//2. prepared
    iManager->MccMediaPrepared( iUpdate->iMccID,
                                SRVUPDATESTREAM(1)->LinkId(),
                                SRVUPDATESTREAM(1)->Id(),
                                SRVUPDATESTREAM(1)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    iManager->MccMediaPrepared( iUpdate->iMccID,
                                SRVUPDATESTREAM(1)->LinkId(),
                                SRVUPDATESTREAM(1)->Id(),
                                SRVUPDATESTREAM(1)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 

    //session asserts
    EUNIT_ASSERT( iUpdate->NegotiationState().Id() == KMceStartingOffererReceiveStreams );
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccSetRemoteAddress );//known from backup, OK?
    
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccStartStream );
    //always synchronizing
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccGetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::EMccSetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 4 ].iAction == CMccControllerStub::ENone );
    
    //srvstream asserts
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->State() == CMceSrvStream::EStarting );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->State() == CMceSrvStream::EPrepared );
    //backup asserts
    EUNIT_ASSERT( iSession->MccStreams().Count() == 2 );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );

                                
    Reset();
//1. started
    iManager->MccMediaStarted( iUpdate->iMccID,
                                SRVUPDATESTREAM(0)->LinkId(),
                                SRVUPDATESTREAM(0)->Id(),
                                SRVUPDATESTREAM(0)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    iManager->MccMediaStarted( iUpdate->iMccID,
                                SRVUPDATESTREAM(0)->LinkId(),
                                SRVUPDATESTREAM(0)->Id(),
                                SRVUPDATESTREAM(0)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 

    //session asserts
    EUNIT_ASSERT( iUpdate->NegotiationState().Id() == KMceOfferingMedia );
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == EMceMediaUpdated );
    EUNIT_ASSERT( iServer->iSession == iUpdate );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    //srvstream asserts
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->State() == CMceSrvStream::EPrepared );
    //backup asserts
    EUNIT_ASSERT( iSession->MccStreams().Count() == 2 );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );

    Reset();
//encode 
    iSdpDocument = iManager->EncodeL( *iUpdate );

    //session asserts
    EUNIT_ASSERT( iUpdate->NegotiationState().Id() == KMceOfferingMedia );
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    //srvstream asserts
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->State() == CMceSrvStream::EPrepared );
    //backup asserts
    EUNIT_ASSERT( iSession->MccStreams().Count() == 2 );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );

    delete iSdpDocument;
    iSdpDocument = NULL;
    iSdpDocument = CSdpDocument::DecodeL( KMceMMTestSdpAnswerAudioIMS5PrecsSendrecv );

    Reset();
//decode answer
    iUpdate->AnswerType() = KMceNegotiationAnswerTypeFinal;
    decodeStatus = iManager->DecodeL( *iUpdate, *iSdpDocument );
    EUNIT_ASSERT( decodeStatus == KErrNone );

    //session asserts
    EUNIT_ASSERT( iUpdate->NegotiationState().Id() == KMceOfferingMedia );
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    //srvstream asserts
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->State() == CMceSrvStream::EPrepared );
    //backup asserts
    EUNIT_ASSERT( iSession->MccStreams().Count() == 2 );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );

    Reset();
//update
    // Update may fail also with KErrArgument in oom situation
    MCE_LEAVE_HANDLING( status = iManager->UpdateL( *iUpdate ), KErrArgument );
    EUNIT_ASSERT( status == KMceAsync );

    //session asserts
    EUNIT_ASSERT( iUpdate->NegotiationState().Id() == KMceStartingOffererSendStreams );
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccGetCodec );//validate
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccSetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccSetRemoteAddress );
    EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::EMccStartStream );
    //always synchronizing
    EUNIT_ASSERT( iMcc->iActions[ 4 ].iAction == CMccControllerStub::EMccGetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 5 ].iAction == CMccControllerStub::EMccSetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 6 ].iAction == CMccControllerStub::ENone );
    
    //srvstream asserts
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->State() == CMceSrvStream::EStarting );
    //backup asserts
    EUNIT_ASSERT( iSession->MccStreams().Count() == 2 );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );

    
    Reset();
//2. started
    iManager->MccMediaStarted( iUpdate->iMccID,
                               SRVUPDATESTREAM(1)->LinkId(),
                               SRVUPDATESTREAM(1)->Id(),
                               KNoEndpointId );
    MCE_MCC_EVENT_POSTCONDITION_L 

    //session asserts
    EUNIT_ASSERT( iUpdate->NegotiationState().Id() == KMceMediaNegotiated );
    EUNIT_ASSERT( iUpdate->AdoptedMccStreams().Count() == 0 );
    
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == EMceMediaUpdated );
    EUNIT_ASSERT( iServer->iSession == iUpdate );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    //srvstream asserts
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->State() == CMceSrvStream::EStarted );
    //backup asserts
    EUNIT_ASSERT( iSession->MccStreams().Count() == 2 );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
                               
    Reset();
//server accepts session
    User::LeaveIfError( iManager->CloseSession( *iSession ) );
    CMceComSession::Delete( iSession, *iManager );

    //manager asserts
    EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 1 );
    EUNIT_ASSERT( iUpdate->SdpSession().MediaSession() == iUpdate );
    EUNIT_ASSERT( iUpdate->SdpSession().Backup() == NULL );
    
    //session asserts
    EUNIT_ASSERT( iSession == NULL );
    EUNIT_ASSERT( iUpdate->MccStreams().Count() == 2 );
    EUNIT_ASSERT( iUpdate->AdoptedMccStreams().Count() == 1 );
    EUNIT_ASSERT( iUpdate->AdoptedMccStreams()[0]->State() == CMceSrvStream::EAdopted );

    EUNIT_ASSERT( iUpdate->NegotiationState().Id() == KMceMediaNegotiated );
    EUNIT_ASSERT( &iUpdate->SdpSession() != NULL );
    EUNIT_ASSERT( iUpdate->iMccID != KMceNotAssigned );
    EUNIT_ASSERT( !iUpdate->IsMerged() );

    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStopStream );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccDeleteStream );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::ENone );
    
    //srvstream asserts
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->State() == CMceSrvStream::EStarted );
    
    Reset();
//1. receives data
    iManager->MccMediaActive( iUpdate->iMccID,
                              SRVUPDATESTREAM(0)->LinkId(),
                              SRVUPDATESTREAM(0)->Id(),
                              KNoEndpointId );
    MCE_MCC_EVENT_POSTCONDITION_L 
                              
    //manager asserts
    EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 1 );
    EUNIT_ASSERT( iUpdate->SdpSession().MediaSession() == iUpdate );
    EUNIT_ASSERT( iUpdate->SdpSession().Backup() == NULL );
    
    //session asserts
    EUNIT_ASSERT( iSession == NULL );
    EUNIT_ASSERT( iUpdate->MccStreams().Count() == 2 );
    EUNIT_ASSERT( iUpdate->AdoptedMccStreams().Count() == 0 );
    
    EUNIT_ASSERT( iUpdate->NegotiationState().Id() == KMceMediaNegotiated );
    EUNIT_ASSERT( &iUpdate->SdpSession() != NULL );
    EUNIT_ASSERT( iUpdate->iMccID != KMceNotAssigned );
    EUNIT_ASSERT( !iUpdate->IsMerged() );

    //server asserts
    EUNIT_ASSERT( iServer->iEvent == EMceMediaEventReceived );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    //srvstream asserts
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->State() == CMceSrvStream::EStarted );
    
    }


//
// Tests
//
// TMceReturnStatus UpdateL( CMceComSession& aSession )
// (hold)
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_UpdateL_4L()
    {

    SETUP( CreateAudioOutSessionL );
    
//update
    TMceReturnStatus status = iManager->UpdateL( *iSession );
    EUNIT_ASSERT( status == KMceAsync );
    
//1. link created
    MceMediaManagerTestHelper::LinkCreated( *iManager, SRVSTREAM(0)->LinkId() );
//1. prepared
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
                                
//2. prepared
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(1)->LinkId(),
                                SRVSTREAM(1)->Id(),
                                SRVSTREAM(1)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(1)->LinkId(),
                                SRVSTREAM(1)->Id(),
                                SRVSTREAM(1)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    Reset();                                
//1. started
    iManager->MccMediaStarted( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    iManager->MccMediaStarted( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 

	MCE_CHECK_MEMORY_LEAVE( iServer->iEvent != EMceMediaUpdated )
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == EMceMediaUpdated );
    EUNIT_ASSERT( iServer->iSession == iSession );

//encode 
    iSdpDocument = iManager->EncodeL( *iSession );

    delete iSdpDocument;
    iSdpDocument = NULL;
    iSdpDocument = CSdpDocument::DecodeL( KMceMMTestSdpAnswerAudioIMS5PrecsSendrecv );

//decode answer
    iSession->AnswerType() = KMceNegotiationAnswerTypeFinal;
    TMceSipWarningCode decodeStatus = iManager->DecodeL( *iSession, *iSdpDocument );
    
//update
    status = iManager->UpdateL( *iSession );
    
//2. started
    iManager->MccMediaStarted( iSession->iMccID,
                               SRVSTREAM(1)->LinkId(),
                               SRVSTREAM(1)->Id(),
                               KNoEndpointId );
    MCE_MCC_EVENT_POSTCONDITION_L 
    
    Reset();
//
//update
//    
    CMceComAudioStream* audio = static_cast<CMceComAudioStream*>( iSession->Streams()[0] );
    audio->iIsEnabled = EFalse;
    iUpdate = iSession->CloneL();
    iUpdate->SetObserver( *iServer );

    audio = static_cast<CMceComAudioStream*>( iUpdate->Streams()[0] );
    EUNIT_ASSERT( !audio->IsEnabled() );
    
    EUNIT_ASSERT( iSession->Streams().Count() == 1 );
    EUNIT_ASSERT( iSession->MccStreams().Count() == 2 );
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceMediaNegotiated );
    EUNIT_ASSERT( &iSession->SdpSession() != NULL );
    EUNIT_ASSERT( iSession->iMccID != KMceNotAssigned );
    EUNIT_ASSERT( !iSession->IsMerged() );
    
    EUNIT_ASSERT( iUpdate->Streams().Count() == 1 );
    EUNIT_ASSERT( iUpdate->MccStreams().Count() == 0 );
    EUNIT_ASSERT( &iUpdate->NegotiationState() == NULL );
    EUNIT_ASSERT( iUpdate->iMccID == KMceNotAssigned );
    EUNIT_ASSERT( !iUpdate->IsMerged() );
    
    
    EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 1 );
    EUNIT_ASSERT( iSession->SdpSession().MediaSession() == iSession );
    EUNIT_ASSERT( iSession->SdpSession().Backup() == NULL );
    
//update
    // Update may fail also with KErrArgument in oom situation
    MCE_LEAVE_HANDLING( status = iManager->UpdateL( *iUpdate ), KErrArgument );
    EUNIT_ASSERT( status == KMceAsync );

    //session asserts
    EUNIT_ASSERT( iUpdate->MccStreams().Count() == 2 );
    EUNIT_ASSERT( iUpdate->NegotiationState().Id() == KMcePreparingOffererStreams );
    EUNIT_ASSERT( &iSession->SdpSession() != NULL );
    EUNIT_ASSERT( iSession->SdpSession().MediaSession() == iUpdate );
    EUNIT_ASSERT( iSession->SdpSession().Backup() == iSession );
    EUNIT_ASSERT( !iUpdate->IsMerged() );
    EUNIT_ASSERT( !iSession->IsMerged() );
    
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );

    //manager asserts
    EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 1 );

    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccGetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccCreateSession );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccCreateLink );
    EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::ENone );

    //srvstream  asserts
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->StreamType() == CMceComMediaStream::EReceiveStream );
    EUNIT_ASSERT( !SRVUPDATESTREAM(0)->Data().IsEnabled() );
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->State() == CMceSrvStream::EInactive );
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->Id() == KMceNotAssigned );
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->LinkId() == KMce_MM_LinkIdBase + 1 );
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->Source().Id() == KMceNotAssigned );
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->Sink().Id() == KMceNotAssigned );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->StreamType() == CMceComMediaStream::ESendStream );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->State() == CMceSrvStream::ECreatingLink );
    
    //backup asserts
    EUNIT_ASSERT( iSession->MccStreams().Count() == 2 );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );

    Reset();
    
//1. link created
    MceMediaManagerTestHelper::LinkCreated( *iManager, SRVUPDATESTREAM(1)->LinkId(), iUpdate->iMccID );
    
    Reset();
//2. prepared 
    iManager->MccMediaPrepared( iUpdate->iMccID,
                                SRVUPDATESTREAM(1)->LinkId(),
                                SRVUPDATESTREAM(1)->Id(),
                                SRVUPDATESTREAM(1)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    iManager->MccMediaPrepared( iUpdate->iMccID,
                                SRVUPDATESTREAM(1)->LinkId(),
                                SRVUPDATESTREAM(1)->Id(),
                                SRVUPDATESTREAM(1)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 

    //session asserts
    EUNIT_ASSERT( iUpdate->NegotiationState().Id() == KMceOfferingMedia );
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == EMceMediaUpdated );
    EUNIT_ASSERT( iServer->iSession == iUpdate );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    //srvstream asserts
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->State() == CMceSrvStream::EInactive );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->State() == CMceSrvStream::EPrepared );
    //backup asserts
    EUNIT_ASSERT( iSession->MccStreams().Count() == 2 );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );

    EUNIT_ASSERT( !SRVUPDATESTREAM(0)->Data().IsEnabled() );
                                
    Reset();
//encode 
    iSdpDocument = iManager->EncodeL( *iUpdate );

    //session asserts
    EUNIT_ASSERT( iUpdate->NegotiationState().Id() == KMceOfferingMedia );
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    //srvstream asserts
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->State() == CMceSrvStream::EInactive );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->State() == CMceSrvStream::EPrepared );
    //backup asserts
    EUNIT_ASSERT( iSession->MccStreams().Count() == 2 );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );

    EUNIT_ASSERT( !SRVUPDATESTREAM(0)->Data().IsEnabled() );

    delete iSdpDocument;
    iSdpDocument = NULL;
    iSdpDocument = CSdpDocument::DecodeL( KMceMMTestSdpAnswerAudioIMS5PrecsRecvonly );

    Reset();
//decode answer
    iUpdate->AnswerType() = KMceNegotiationAnswerTypeFinal;
    decodeStatus = iManager->DecodeL( *iUpdate, *iSdpDocument );
    EUNIT_ASSERT( decodeStatus == KErrNone );

    //session asserts
    EUNIT_ASSERT( iUpdate->NegotiationState().Id() == KMceOfferingMedia );
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    //srvstream asserts
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->State() == CMceSrvStream::EInactive );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->State() == CMceSrvStream::EPrepared );
    //backup asserts
    EUNIT_ASSERT( iSession->MccStreams().Count() == 2 );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );

    EUNIT_ASSERT( !SRVUPDATESTREAM(0)->Data().IsEnabled() );
    
    Reset();
//update
    // Update may fail also with KErrArgument in oom situation
    MCE_LEAVE_HANDLING( status = iManager->UpdateL( *iUpdate ), KErrArgument );
    EUNIT_ASSERT( status == KMceAsync );

    //session asserts
    EUNIT_ASSERT( iUpdate->NegotiationState().Id() == KMceStartingOffererSendStreams );
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccGetCodec );//validate
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccSetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccSetRemoteAddress );
    EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::EMccStartStream );
     
    //always synchronizing
    EUNIT_ASSERT( iMcc->iActions[ 4 ].iAction == CMccControllerStub::EMccGetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 5 ].iAction == CMccControllerStub::EMccSetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 6 ].iAction == CMccControllerStub::ENone );
    
    //srvstream asserts
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->State() == CMceSrvStream::EInactive );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->State() == CMceSrvStream::EStarting );
    //backup asserts
    EUNIT_ASSERT( iSession->MccStreams().Count() == 2 );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );

    EUNIT_ASSERT( !SRVUPDATESTREAM(0)->Data().IsEnabled() );
    
    Reset();
//2. started
    iManager->MccMediaStarted( iUpdate->iMccID,
                               SRVUPDATESTREAM(1)->LinkId(),
                               SRVUPDATESTREAM(1)->Id(),
                               KNoEndpointId );
    MCE_MCC_EVENT_POSTCONDITION_L 

    //session asserts
    EUNIT_ASSERT( iUpdate->NegotiationState().Id() == KMceMediaNegotiated );
    EUNIT_ASSERT( iUpdate->AdoptedMccStreams().Count() == 0 );
    
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == EMceMediaUpdated );
    EUNIT_ASSERT( iServer->iSession == iUpdate );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    //srvstream asserts
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->State() == CMceSrvStream::EInactive );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->State() == CMceSrvStream::EStarted );
    //backup asserts
    EUNIT_ASSERT( iSession->MccStreams().Count() == 2 );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
                       
    EUNIT_ASSERT( !SRVUPDATESTREAM(0)->Data().IsEnabled() );
                               
    Reset();
//server accepts session
    User::LeaveIfError( iManager->CloseSession( *iSession ) );
    CMceComSession::Delete( iSession, *iManager );

    //manager asserts
    EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 1 );
    EUNIT_ASSERT( iUpdate->SdpSession().MediaSession() == iUpdate );
    EUNIT_ASSERT( iUpdate->SdpSession().Backup() == NULL );
    
    //session asserts
    EUNIT_ASSERT( iSession == NULL );
    EUNIT_ASSERT( iUpdate->MccStreams().Count() == 2 );
    EUNIT_ASSERT( iUpdate->AdoptedMccStreams().Count() == 0 );

    EUNIT_ASSERT( iUpdate->NegotiationState().Id() == KMceMediaNegotiated );
    EUNIT_ASSERT( &iUpdate->SdpSession() != NULL );
    EUNIT_ASSERT( iUpdate->iMccID != KMceNotAssigned );
    EUNIT_ASSERT( !iUpdate->IsMerged() );

    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    
    //srvstream asserts
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->State() == CMceSrvStream::EInactive );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->State() == CMceSrvStream::EStarted );
       
    }



//
// Tests
//
// TMceReturnStatus UpdateL( CMceComSession& aSession )
// (hold, which fails )
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_UpdateL_4_1L()
    {

    SETUP( CreateAudioOutSessionL );
    
//update
    TMceReturnStatus status = iManager->UpdateL( *iSession );
    EUNIT_ASSERT( status == KMceAsync );
    
//1. link created
    MceMediaManagerTestHelper::LinkCreated( *iManager, SRVSTREAM(0)->LinkId() );
//1. prepared
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
                                
//2. prepared
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(1)->LinkId(),
                                SRVSTREAM(1)->Id(),
                                SRVSTREAM(1)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(1)->LinkId(),
                                SRVSTREAM(1)->Id(),
                                SRVSTREAM(1)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    Reset();                                
//1. started
    iManager->MccMediaStarted( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    iManager->MccMediaStarted( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L                             

	MCE_CHECK_MEMORY_LEAVE( iServer->iEvent != EMceMediaUpdated )
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == EMceMediaUpdated );
    EUNIT_ASSERT( iServer->iSession == iSession );

    Reset();
//encode 
    iSdpDocument = iManager->EncodeL( *iSession );

    delete iSdpDocument;
    iSdpDocument = NULL;
    iSdpDocument = CSdpDocument::DecodeL( KMceMMTestSdpAnswerAudioIMS5PrecsSendrecv );

    Reset();
//decode answer    
    iSession->AnswerType() = KMceNegotiationAnswerTypeFinal;
    TMceSipWarningCode decodeStatus = iManager->DecodeL( *iSession, *iSdpDocument );
    
    Reset();
//update
    status = iManager->UpdateL( *iSession );
    
    Reset();
//2. started
    iManager->MccMediaStarted( iSession->iMccID,
                               SRVSTREAM(1)->LinkId(),
                               SRVSTREAM(1)->Id(),
                               KNoEndpointId );
    MCE_MCC_EVENT_POSTCONDITION_L 
    
    Reset();
//
//update
//    
    CMceComAudioStream* oaudio = static_cast<CMceComAudioStream*>( iSession->Streams()[0] );

    iUpdate = iSession->CloneL();
    iUpdate->SetObserver( *iServer );
    
    CMceComAudioStream* audio = static_cast<CMceComAudioStream*>( iUpdate->Streams()[0] );
    //disable
    audio->Enable( EFalse, ETrue );
    
    EUNIT_ASSERT( oaudio->IsEnabled() );
    EUNIT_ASSERT( oaudio->Source()->IsEnabled() );
    EUNIT_ASSERT( oaudio->Sinks()[0]->IsEnabled() );
    EUNIT_ASSERT( !audio->IsEnabled() );
    EUNIT_ASSERT( !audio->Source()->IsEnabled() );
    EUNIT_ASSERT( !audio->Sinks()[0]->IsEnabled() );
    
    
    EUNIT_ASSERT( iSession->Streams().Count() == 1 );
    EUNIT_ASSERT( iSession->MccStreams().Count() == 2 );
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceMediaNegotiated );
    EUNIT_ASSERT( &iSession->SdpSession() != NULL );
    EUNIT_ASSERT( iSession->iMccID != KMceNotAssigned );
    EUNIT_ASSERT( !iSession->IsMerged() );
    
    EUNIT_ASSERT( iUpdate->Streams().Count() == 1 );
    EUNIT_ASSERT( iUpdate->MccStreams().Count() == 0 );
    EUNIT_ASSERT( &iUpdate->NegotiationState() == NULL );
    EUNIT_ASSERT( iUpdate->iMccID == KMceNotAssigned );
    EUNIT_ASSERT( !iUpdate->IsMerged() );
    
    
    EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 1 );
    EUNIT_ASSERT( iSession->SdpSession().MediaSession() == iSession );
    EUNIT_ASSERT( iSession->SdpSession().Backup() == NULL );
    
//update
    iMcc->iRequireSignalling = EFalse;
    // Update may fail also with KErrArgument in oom situation
    MCE_LEAVE_HANDLING( status = iManager->UpdateL( *iUpdate ), KErrArgument );
    EUNIT_ASSERT( status == KMceReady );

    //session asserts
    EUNIT_ASSERT( iUpdate->MccStreams().Count() == 2 );
    EUNIT_ASSERT( iUpdate->NegotiationState().Id() == KMceOfferingMedia );
    EUNIT_ASSERT( &iSession->SdpSession() != NULL );
    EUNIT_ASSERT( iSession->SdpSession().MediaSession() == iUpdate );
    EUNIT_ASSERT( iSession->SdpSession().Backup() == iSession );
    EUNIT_ASSERT( iUpdate->IsMerged() );
    EUNIT_ASSERT( iSession->IsMerged() );
    
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );

    //manager asserts
    EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 1 );

    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccGetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccGetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccSetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::EMccPauseStream );
    EUNIT_ASSERT( iMcc->iActions[ 4 ].iAction == CMccControllerStub::ENone );

    //srvstream  asserts
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->StreamType() == CMceComMediaStream::EReceiveStream );
    EUNIT_ASSERT( !SRVUPDATESTREAM(0)->Data().IsEnabled() );
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->Source().State() == CMceSrvStream::EPausing );
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->Sink().State() == CMceSrvStream::EPausing );
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->Id() == SRVSTREAM(0)->Id() );
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->LinkId() == SRVSTREAM(0)->LinkId() );
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->Source().Id() == SRVSTREAM(0)->Source().Id() );
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->Sink().Id() == SRVSTREAM(0)->Sink().Id() );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->StreamType() == CMceComMediaStream::ESendStream );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->Data().IsEnabled() );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->Id() == SRVSTREAM(1)->Id() );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->LinkId() == SRVSTREAM(1)->LinkId() );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->Source().Id() == SRVSTREAM(1)->Source().Id() );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->Sink().Id() == SRVSTREAM(1)->Sink().Id() );
    
    //backup asserts
    EUNIT_ASSERT( iSession->MccStreams().Count() == 2 );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStarted );

    Reset();
    iManager->MccMediaPaused( iUpdate->iMccID,
                              SRVUPDATESTREAM(0)->LinkId(),
                              SRVUPDATESTREAM(0)->Id(),
                              KNoEndpointId );
    MCE_MCC_EVENT_POSTCONDITION_L 
    
    //srvstream  asserts
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->StreamType() == CMceComMediaStream::EReceiveStream );
    EUNIT_ASSERT( !SRVUPDATESTREAM(0)->Data().IsEnabled() );
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->Source().State() == CMceSrvStream::EPaused );
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->Sink().State() == CMceSrvStream::EPaused );
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->Id() == SRVSTREAM(0)->Id() );
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->LinkId() == SRVSTREAM(0)->LinkId() );
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->Source().Id() == SRVSTREAM(0)->Source().Id() );
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->Sink().Id() == SRVSTREAM(0)->Sink().Id() );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->StreamType() == CMceComMediaStream::ESendStream );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->Data().IsEnabled() );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->Id() == SRVSTREAM(1)->Id() );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->LinkId() == SRVSTREAM(1)->LinkId() );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->Source().Id() == SRVSTREAM(1)->Source().Id() );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->Sink().Id() == SRVSTREAM(1)->Sink().Id() );

    //backup asserts
    EUNIT_ASSERT( iSession->MccStreams().Count() == 2 );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStarted );
    
    Reset();
//encode 
    iSdpDocument = iManager->EncodeL( *iUpdate );

    //session asserts
    EUNIT_ASSERT( iUpdate->NegotiationState().Id() == KMceOfferingMedia );
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    //srvstream asserts
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->State() == CMceSrvStream::EStarted );
    //backup asserts
    EUNIT_ASSERT( iSession->MccStreams().Count() == 2 );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );

    EUNIT_ASSERT( !SRVUPDATESTREAM(0)->Data().IsEnabled() );

    delete iSdpDocument;
    iSdpDocument = NULL;
    iSdpDocument = CSdpDocument::DecodeL( KMceMMTestSdpAnswerAudioIMS5PrecsRecvonly );

    Reset();
//decode answer    
    iUpdate->AnswerType() = KMceNegotiationAnswerTypeFinal;
    decodeStatus = iManager->DecodeL( *iUpdate, *iSdpDocument );
    EUNIT_ASSERT( decodeStatus == KErrNone );

    //session asserts
    EUNIT_ASSERT( iUpdate->NegotiationState().Id() == KMceOfferingMedia );
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    //srvstream asserts
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->State() == CMceSrvStream::EStarted );
    //backup asserts
    EUNIT_ASSERT( iSession->MccStreams().Count() == 2 );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );

    EUNIT_ASSERT( !SRVUPDATESTREAM(0)->Data().IsEnabled() );
    
    Reset();
//update
    status = iManager->UpdateL( *iUpdate );
    EUNIT_ASSERT( status == KMceReady );

    //session asserts
    EUNIT_ASSERT( iUpdate->NegotiationState().Id() == KMceMediaNegotiated );
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccGetCodec );//validate
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccSetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccGetCodec );//validate
    EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::EMccSetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 4 ].iAction == CMccControllerStub::EMccSetRemoteAddress );//validate
    
    EUNIT_ASSERT( iMcc->iActions[ 5 ].iAction == CMccControllerStub::EMccGetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 6 ].iAction == CMccControllerStub::EMccSetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 7 ].iAction == CMccControllerStub::ENone );
    
    //srvstream asserts
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->Source().State() == CMceSrvStream::EPaused );
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->Sink().State() == CMceSrvStream::EPaused );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->Sink().State() == CMceSrvStream::EStarted );
    
    //backup asserts
    EUNIT_ASSERT( iSession->MccStreams().Count() == 2 );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStarted );

    EUNIT_ASSERT( !SRVUPDATESTREAM(0)->Data().IsEnabled() );
    
    Reset();
//server rejects update
    CMceComSession::Delete( iUpdate, *iManager );

    EUNIT_ASSERT( iSession != NULL );
    EUNIT_ASSERT( iUpdate == NULL );
    
    //manager asserts
    EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 1 );
    EUNIT_ASSERT( iSession->SdpSession().MediaSession() == iSession );
    EUNIT_ASSERT( iSession->SdpSession().Backup() == NULL );
    
    //session asserts
    EUNIT_ASSERT( iSession->MccStreams().Count() == 2 );
    EUNIT_ASSERT( iSession->AdoptedMccStreams().Count() == 0 );

    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceMediaNegotiated );
    EUNIT_ASSERT( &iSession->SdpSession() != NULL );
    EUNIT_ASSERT( iSession->iMccID != KMceNotAssigned );
    EUNIT_ASSERT( !iSession->IsMerged() );

    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccResumeStream );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );

    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    
    //srvstream asserts
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarting );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarting );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarting );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStarted );
    

    audio = static_cast<CMceComAudioStream*>( iSession->Streams()[0] );
    EUNIT_ASSERT( audio->IsEnabled() );
    EUNIT_ASSERT( audio->Source()->IsEnabled() );
    EUNIT_ASSERT( audio->Sinks()[0]->IsEnabled() );

    iManager->MccMediaResumed( iSession->iMccID,
                              SRVSTREAM(0)->LinkId(),
                              SRVSTREAM(0)->Id(),
                              KNoEndpointId );
    MCE_MCC_EVENT_POSTCONDITION_L 
    
    
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStarted );
    
    }


//
// Tests
//
// TMceReturnStatus UpdateL( CMceComSession& aSession )
// (session update with no signalling )
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_UpdateL_5L()
    {

    SETUP( CreateAudioOutSessionL );
    
//update
    TMceReturnStatus status = iManager->UpdateL( *iSession );
    
//1. link created
    MceMediaManagerTestHelper::LinkCreated( *iManager, SRVSTREAM(0)->LinkId() );
//1. prepared
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Sink().Id() );
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Source().Id() );
                                
//2. prepared
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(1)->LinkId(),
                                SRVSTREAM(1)->Id(),
                                SRVSTREAM(1)->Sink().Id() );
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(1)->LinkId(),
                                SRVSTREAM(1)->Id(),
                                SRVSTREAM(1)->Source().Id() );
    Reset();
//1. started
    iManager->MccMediaStarted( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Sink().Id() );
    iManager->MccMediaStarted( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Source().Id() );
                                

	MCE_CHECK_MEMORY_LEAVE( iServer->iEvent != EMceMediaUpdated )
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == EMceMediaUpdated );
    EUNIT_ASSERT( iServer->iSession == iSession );

    Reset();
//encode 
    iSdpDocument = iManager->EncodeL( *iSession );

    delete iSdpDocument;
    iSdpDocument = NULL;
    iSdpDocument = CSdpDocument::DecodeL( KMceMMTestSdpAnswerAudioIMS5PrecsSendrecv );

    Reset();
//decode answer    
    iSession->AnswerType() = KMceNegotiationAnswerTypeFinal;
    TMceSipWarningCode decodeStatus = iManager->DecodeL( *iSession, *iSdpDocument );
    
    Reset();
//update
    status = iManager->UpdateL( *iSession );
    
    Reset();
//2. started
    iManager->MccMediaStarted( iSession->iMccID,
                               SRVSTREAM(1)->LinkId(),
                               SRVSTREAM(1)->Id(),
                               KNoEndpointId );
    
    Reset();
//
//update
//    
    iUpdate = iSession->CloneL();
    iUpdate->SetObserver( *iServer );
    CMceComAudioStream* audio = static_cast<CMceComAudioStream*>( iUpdate->Streams()[0] );
    audio->Codecs()[0]->iBitrate = KMceAmrNbBitrate670;

    EUNIT_ASSERT( iSession->Streams().Count() == 1 );
    EUNIT_ASSERT( iSession->MccStreams().Count() == 2 );
    
    MCE_CHECK_MEMORY_LEAVE( iSession->NegotiationState().Id() != KMceMediaNegotiated )
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceMediaNegotiated );
    EUNIT_ASSERT( &iSession->SdpSession() != NULL );
    EUNIT_ASSERT( iSession->iMccID != KMceNotAssigned );
    EUNIT_ASSERT( !iSession->IsMerged() );
    
    EUNIT_ASSERT( iUpdate->Streams().Count() == 1 );
    EUNIT_ASSERT( iUpdate->MccStreams().Count() == 0 );
    EUNIT_ASSERT( &iUpdate->NegotiationState() == NULL );
    EUNIT_ASSERT( iUpdate->iMccID == KMceNotAssigned );
    EUNIT_ASSERT( !iUpdate->IsMerged() );
    
    
    EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 1 );
    EUNIT_ASSERT( iSession->SdpSession().MediaSession() == iSession );
    EUNIT_ASSERT( iSession->SdpSession().Backup() == NULL );
    
//update
    iMcc->iRequireSignalling = EFalse;
    MCE_LEAVE_HANDLING( status = iManager->UpdateL( *iUpdate ), KErrArgument );
    EUNIT_ASSERT( status == KMceReady );
    
    //session asserts
    EUNIT_ASSERT( iUpdate->MccStreams().Count() == 2 );
    EUNIT_ASSERT( iUpdate->NegotiationState().Id() == KMceMediaNegotiated );
    EUNIT_ASSERT( &iSession->SdpSession() != NULL );
    EUNIT_ASSERT( iSession->SdpSession().MediaSession() == iUpdate );
    EUNIT_ASSERT( iSession->SdpSession().Backup() == iSession );
    EUNIT_ASSERT( iUpdate->IsMerged() );
    EUNIT_ASSERT( iSession->IsMerged() );
    
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );

    //manager asserts
    EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 1 );

    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccGetCodec );//check signalling
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccGetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccGetCodec );//update codec and stream
    EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::EMccSetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 4 ].iAction == CMccControllerStub::EMccSetRemoteAddress );//update codec and stream
    
    EUNIT_ASSERT( iMcc->iActions[ 5 ].iAction == CMccControllerStub::EMccGetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 6 ].iAction == CMccControllerStub::EMccSetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 7 ].iAction == CMccControllerStub::ENone );

    //srvstream  asserts
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->StreamType() == SRVSTREAM(0)->StreamType() );
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->State() == SRVSTREAM(0)->State() );
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->Id() == SRVSTREAM(0)->Id() );
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->LinkId() == SRVSTREAM(0)->LinkId() );
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->Source().Id() == SRVSTREAM(0)->Source().Id() );
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->Sink().Id() == SRVSTREAM(0)->Sink().Id() );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->StreamType() == SRVSTREAM(1)->StreamType() );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->State() == SRVSTREAM(1)->State() );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->Id() == SRVSTREAM(1)->Id() );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->LinkId() == SRVSTREAM(1)->LinkId() );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->Source().Id() == SRVSTREAM(1)->Source().Id() );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->Sink().Id() == SRVSTREAM(1)->Sink().Id() );
    
                               
    Reset();
//server accepts session
    CMceComSession::Delete( iSession, *iManager );

    //manager asserts
    EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 1 );
    EUNIT_ASSERT( iUpdate->SdpSession().MediaSession() == iUpdate );
    EUNIT_ASSERT( iUpdate->SdpSession().Backup() == NULL );
    
    //session asserts
    EUNIT_ASSERT( iSession == NULL );
    EUNIT_ASSERT( iUpdate->MccStreams().Count() == 2 );
    EUNIT_ASSERT( iUpdate->AdoptedMccStreams().Count() == 0 );

    EUNIT_ASSERT( iUpdate->NegotiationState().Id() == KMceMediaNegotiated );
    EUNIT_ASSERT( &iUpdate->SdpSession() != NULL );
    EUNIT_ASSERT( iUpdate->iMccID != KMceNotAssigned );
    EUNIT_ASSERT( !iUpdate->IsMerged() );

    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    //srvstream asserts
    EUNIT_ASSERT( SRVUPDATESTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVUPDATESTREAM(1)->State() == CMceSrvStream::EStarted );
    
    
    }

//
// Tests
//
// TMceReturnStatus UpdateL( CMceComSession& aSession )
//(received session update )
//
//
void UT_CMceMediaManager::UT_CMceMediaManager_UpdateL_6L()
    {
    for( TInt i = 0; i < 3; i++ )
        {
        if ( i == 0 )//mm
            {
            SETUP( CreateMultimediaOutSessionL );
            
            iSession->iUseLocalPreconditions = ETrue;
            iSession->iUseRemotePreconditions = ETrue;
            
            //establish    
            iSdpDocument = MceMediaManagerTestHelper::EstablishOutMultimediaSessionL( 
                                                    *iSession,
                                                    *iManager,
                                                    KMceMMTestSdpAnswerAudioVideoIMS5PrecsSendrecv,
                                                    iServer );
            
            delete iSdpDocument;
            iSdpDocument = NULL;
            iSdpDocument = CSdpDocument::DecodeL(
                KMceMMTestSdpAnswerAudioVideoIMS5PrecsSendrecv );
            }
        else if ( i == 1 )//a
            {
            SETUP( CreateAudioOutSessionL );
            
            //establish    
            iSdpDocument = MceMediaManagerTestHelper::EstablishOutSessionL( 
                                                    *iSession,
                                                    *iManager,
                                                    KMceMMTestSdpAnswerAudioIMS5PrecsSendrecv,
                                                    iServer );
            
            delete iSdpDocument;
            iSdpDocument = NULL;
            iSdpDocument = CSdpDocument::DecodeL( 
                KMceMMTestSdpAnswerAudioIMS5PrecsSendrecv );
            }
        else if ( i == 2 )//v
            {
            SETUP( CreateVideoOutSessionL );
            
            //establish    
            iSdpDocument = MceMediaManagerTestHelper::EstablishOutSessionL( 
                                                    *iSession,
                                                    *iManager,
                                                    KMceMMTestSdpAnswerVideoIMS5PrecsSendrecv,
                                                    iServer );
            
            delete iSdpDocument;
            iSdpDocument = NULL;
            iSdpDocument = CSdpDocument::DecodeL( 
                KMceMMTestSdpAnswerVideoIMS5PrecsSendrecv );
            }
        
        iUpdate = iSession->CloneL();
        iUpdate->SetObserver( *iServer );
        iUpdate->iUseLocalPreconditions = ETrue;
        iUpdate->iUseRemotePreconditions = ETrue;
        Reset();
        
		TMceSipWarningCode decodeStatus;
		MCE_LEAVE_HANDLING( decodeStatus = iManager->DecodeL( *iUpdate, *iSdpDocument ), KErrArgument );
        EUNIT_ASSERT( decodeStatus == KErrNone );
        
        Reset();
        
        //update
        TMceReturnStatus status;
        MCE_LEAVE_HANDLING( status = iManager->UpdateL( *iUpdate ), KErrArgument );
        EUNIT_ASSERT( status == KMceReady  );
        
        //session asserts
        EUNIT_ASSERT( iUpdate->NegotiationState().Id() == KMceAnsweringMediaUpdate );
        //server asserts
        EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
        EUNIT_ASSERT( iServer->iSession == NULL );
        
        //actions asserts
        EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
        
        //srvstream asserts
        EUNIT_ASSERT( SRVUPDATESTREAM(0)->StreamType() == CMceComMediaStream::EReceiveStream );
        EUNIT_ASSERT( SRVUPDATESTREAM(0)->State() == CMceSrvStream::ECreated );
        EUNIT_ASSERT( SRVUPDATESTREAM(1)->StreamType() == CMceComMediaStream::ESendStream );
        EUNIT_ASSERT( SRVUPDATESTREAM(1)->State() == CMceSrvStream::ECreated );
        
        if ( i == 0 )
            {
            //srvstream asserts
            EUNIT_ASSERT( iUpdate->MccStreams().Count() == 5 );
            EUNIT_ASSERT( SRVUPDATESTREAM(2)->StreamType() == CMceComMediaStream::EReceiveStream );
            EUNIT_ASSERT( SRVUPDATESTREAM(2)->State() == CMceSrvStream::ECreated );
            EUNIT_ASSERT( SRVUPDATESTREAM(3)->StreamType() == CMceComMediaStream::ESendStream );
            EUNIT_ASSERT( SRVUPDATESTREAM(3)->State() == CMceSrvStream::ECreated );
            EUNIT_ASSERT( SRVUPDATESTREAM(4)->StreamType() == CMceComMediaStream::ELocalStream );
            EUNIT_ASSERT( SRVUPDATESTREAM(4)->State() == CMceSrvStream::ECreated );
            }
        else if ( i == 1 )//a
            {
            //srvstream asserts
            EUNIT_ASSERT( iUpdate->MccStreams().Count() == 2 );
            }
        else if ( i == 2 )//v
            {
            EUNIT_ASSERT( iUpdate->MccStreams().Count() == 3 );
            EUNIT_ASSERT( SRVUPDATESTREAM(2)->StreamType() == CMceComMediaStream::ELocalStream );
            EUNIT_ASSERT( SRVUPDATESTREAM(2)->State() == CMceSrvStream::ECreated );
            }
        
        Reset();
        
        //update
    	MCE_LEAVE_HANDLING( status = iManager->UpdateL( *iUpdate ), KErrArgument );
        EUNIT_ASSERT( status == KMceAsync  );
        
        //session asserts
        EUNIT_ASSERT( iUpdate->NegotiationState().Id() == KMcePreparingAnswererStreams );
        //server asserts
        EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
        EUNIT_ASSERT( iServer->iSession == NULL );
        
        EUNIT_ASSERT( SRVUPDATESTREAM(0)->StreamType() == CMceComMediaStream::EReceiveStream );
        EUNIT_ASSERT( SRVUPDATESTREAM(0)->State() == CMceSrvStream::ECreatingLink );
        EUNIT_ASSERT( SRVUPDATESTREAM(1)->StreamType() == CMceComMediaStream::ESendStream );
        EUNIT_ASSERT( SRVUPDATESTREAM(1)->State() == CMceSrvStream::ECreatingLink );
        
        if ( i == 0 )
            {
            //srvstream asserts
            EUNIT_ASSERT( iUpdate->MccStreams().Count() == 5 );
            EUNIT_ASSERT( SRVUPDATESTREAM(2)->StreamType() == CMceComMediaStream::EReceiveStream );
            EUNIT_ASSERT( SRVUPDATESTREAM(2)->State() == CMceSrvStream::ECreatingLink );
            EUNIT_ASSERT( SRVUPDATESTREAM(3)->StreamType() == CMceComMediaStream::ESendStream );
            EUNIT_ASSERT( SRVUPDATESTREAM(3)->State() == CMceSrvStream::ECreatingLink );
            EUNIT_ASSERT( SRVUPDATESTREAM(4)->StreamType() == CMceComMediaStream::ELocalStream );
            EUNIT_ASSERT( SRVUPDATESTREAM(4)->State() == CMceSrvStream::ECreatingLink );
            }
        else if ( i == 1 )//a
            {
            //srvstream asserts
            EUNIT_ASSERT( iUpdate->MccStreams().Count() == 2 );
            }
        else if ( i == 2 )//v
            {
            EUNIT_ASSERT( iUpdate->MccStreams().Count() == 3 );
            EUNIT_ASSERT( SRVUPDATESTREAM(2)->StreamType() == CMceComMediaStream::ELocalStream );
            EUNIT_ASSERT( SRVUPDATESTREAM(2)->State() == CMceSrvStream::ECreatingLink );
            }

        Reset();
        
        //1. link created
        MceMediaManagerTestHelper::LinkCreated( *iManager, SRVUPDATESTREAM(0)->LinkId(), iUpdate->iMccID );
        if ( i == 0 )
            {
            //2. link created
            MceMediaManagerTestHelper::LinkCreated( *iManager, SRVUPDATESTREAM(2)->LinkId(), iUpdate->iMccID );
            }
        
        Reset();
        //1. prepared
        iManager->MccMediaPrepared( iUpdate->iMccID,
                                    SRVUPDATESTREAM(0)->LinkId(),
                                    SRVUPDATESTREAM(0)->Id(),
                                    SRVUPDATESTREAM(0)->Sink().Id() );
        iManager->MccMediaPrepared( iUpdate->iMccID,
                                    SRVUPDATESTREAM(0)->LinkId(),
                                    SRVUPDATESTREAM(0)->Id(),
                                    SRVUPDATESTREAM(0)->Source().Id() );
                                    
        Reset();
        
        //2. prepared
        iManager->MccMediaPrepared( iUpdate->iMccID,
                                    SRVUPDATESTREAM(1)->LinkId(),
                                    SRVUPDATESTREAM(1)->Id(),
                                    SRVUPDATESTREAM(1)->Sink().Id() );
        iManager->MccMediaPrepared( iUpdate->iMccID,
                                    SRVUPDATESTREAM(1)->LinkId(),
                                    SRVUPDATESTREAM(1)->Id(),
                                    SRVUPDATESTREAM(1)->Source().Id() );

        if ( i == 0 )
            {
            //3. prepared
            iManager->MccMediaPrepared( iUpdate->iMccID,
                                        SRVUPDATESTREAM(2)->LinkId(),
                                        SRVUPDATESTREAM(2)->Id(),
                                        SRVUPDATESTREAM(2)->Sink().Id() );
            iManager->MccMediaPrepared( iUpdate->iMccID,
                                        SRVUPDATESTREAM(2)->LinkId(),
                                        SRVUPDATESTREAM(2)->Id(),
                                        SRVUPDATESTREAM(2)->Source().Id() );
            Reset();
            
            //4. prepared
            iManager->MccMediaPrepared( iUpdate->iMccID,
                                        SRVUPDATESTREAM(3)->LinkId(),
                                        SRVUPDATESTREAM(3)->Id(),
                                        SRVUPDATESTREAM(3)->Sink().Id() );
            iManager->MccMediaPrepared( iUpdate->iMccID,
                                        SRVUPDATESTREAM(3)->LinkId(),
                                        SRVUPDATESTREAM(3)->Id(),
                                        SRVUPDATESTREAM(3)->Source().Id() );
            }

        Reset();
        
        //1. started
        iManager->MccMediaStarted( iUpdate->iMccID,
                                    SRVUPDATESTREAM(0)->LinkId(),
                                    SRVUPDATESTREAM(0)->Id(),
                                    SRVUPDATESTREAM(0)->Sink().Id() );
        iManager->MccMediaStarted( iUpdate->iMccID,
                                    SRVUPDATESTREAM(0)->LinkId(),
                                    SRVUPDATESTREAM(0)->Id(),
                                    SRVUPDATESTREAM(0)->Source().Id() );

        Reset();
        
        //2. started
        iManager->MccMediaStarted( iUpdate->iMccID,
                                    SRVUPDATESTREAM(1)->LinkId(),
                                    SRVUPDATESTREAM(1)->Id(),
                                    SRVUPDATESTREAM(1)->Sink().Id() );
        iManager->MccMediaStarted( iUpdate->iMccID,
                                    SRVUPDATESTREAM(1)->LinkId(),
                                    SRVUPDATESTREAM(1)->Id(),
                                    SRVUPDATESTREAM(1)->Source().Id() );

        if ( i == 0 )
            {
            Reset();
            //3. started
            iManager->MccMediaStarted( iUpdate->iMccID,
                                        SRVUPDATESTREAM(2)->LinkId(),
                                        SRVUPDATESTREAM(2)->Id(),
                                        SRVUPDATESTREAM(2)->Sink().Id() );
            iManager->MccMediaStarted( iUpdate->iMccID,
                                        SRVUPDATESTREAM(2)->LinkId(),
                                        SRVUPDATESTREAM(2)->Id(),
                                        SRVUPDATESTREAM(2)->Source().Id() );
            
            Reset();
            
            //4. started
            iManager->MccMediaStarted( iUpdate->iMccID,
                                        SRVUPDATESTREAM(3)->LinkId(),
                                        SRVUPDATESTREAM(3)->Id(),
                                        SRVUPDATESTREAM(3)->Sink().Id() );
            iManager->MccMediaStarted( iUpdate->iMccID,
                                        SRVUPDATESTREAM(3)->LinkId(),
                                        SRVUPDATESTREAM(3)->Id(),
                                        SRVUPDATESTREAM(3)->Source().Id() );
            }
            
        MCE_CHECK_MEMORY_LEAVE( iUpdate->NegotiationState().Id() != KMceMediaNegotiated )
        //session asserts
        EUNIT_ASSERT( iUpdate->NegotiationState().Id() == KMceMediaNegotiated );
        //server asserts
        MCE_CHECK_MEMORY_LEAVE( iServer->iEvent != EMceMediaUpdated )
        EUNIT_ASSERT( iServer->iEvent == EMceMediaUpdated );
        EUNIT_ASSERT( iServer->iSession == iUpdate );
        
        for ( TInt j = 0; j < iUpdate->MccStreams().Count(); j++)
            {
            EUNIT_ASSERT( SRVUPDATESTREAM(j)->State() == CMceSrvStream::EStarted );
            }
        
        Reset();
        
        //encode 
        iSdpDocument = iManager->EncodeL( *iUpdate );
        
        //session asserts
        EUNIT_ASSERT( iUpdate->NegotiationState().Id() == KMceMediaNegotiated );
        //server asserts
        EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
        EUNIT_ASSERT( iServer->iSession == NULL );
        
        CMceComSession::Delete( iSession, *iManager );
        CMceComSession::Delete( iUpdate, *iManager );
        delete iSdpDocument;
        iSdpDocument = NULL;  
        }
    }
   

//
// Tests
//
// TMceReturnStatus UpdateL( CMceComSession& aSession )
// (session received update with added and removed streams)
//
//
void UT_CMceMediaManager::UT_CMceMediaManager_UpdateL_8L()
    {
    //TBD
    }


//
// Tests
//
// TMceReturnStatus UpdateL( CMceComSession& aSession )
// (MO POC audio)
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_UpdateL_9L()
    {
    SETUP( CreateAudioOutSessionL );
    
    CMceComAudioStream& audio = static_cast<CMceComAudioStream&>( iSession->Streams()[0]->BoundStreamL() );
    audio.Sinks()[0]->Enabled( EFalse );
    iSession->iUseLocalPreconditions = ETrue;
    iSession->iUseRemotePreconditions = ETrue;
    
    EUNIT_ASSERT( iSession->Streams().Count() == 1 );
    EUNIT_ASSERT( iSession->MccStreams().Count() == 0 );
    EUNIT_ASSERT( &iSession->NegotiationState() == NULL );
    EUNIT_ASSERT( &iSession->SdpSession() == NULL );
    EUNIT_ASSERT( iSession->iMccID == KMceNotAssigned );
    EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 0 );
    EUNIT_ASSERT( iManager->iSdpCodecs.Count() == 2 );
    
    Reset();
    
//update
    TMceReturnStatus status = iManager->UpdateL( *iSession );
    EUNIT_ASSERT( status == KMceAsync );

    EUNIT_ASSERT( SRVSTREAM(0)->Sink().Data().IsEnabled() );
    EUNIT_ASSERT( !SRVSTREAM(1)->Sink().Data().IsEnabled() );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().Data().IsEnabled() );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().Data().IsEnabled() );


    Reset();
//1. link created
    MceMediaManagerTestHelper::LinkCreated( *iManager, SRVSTREAM(0)->LinkId() );
    
    Reset();
//1. prepared
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
	MCE_CHECK_MEMORY_LEAVE( !SRVSTREAM(0)->Sink().Data().IsEnabled() )
    //srvstream asserts
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().Data().IsEnabled() );
    EUNIT_ASSERT( !SRVSTREAM(1)->Sink().Data().IsEnabled() );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().Data().IsEnabled() );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().Data().IsEnabled() );
    
    Reset();
//2. prepared
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(1)->LinkId(),
                                SRVSTREAM(1)->Id(),
                                SRVSTREAM(1)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(1)->LinkId(),
                                SRVSTREAM(1)->Id(),
                                SRVSTREAM(1)->Source().Id() ); 
    MCE_MCC_EVENT_POSTCONDITION_L                             
    MCE_CHECK_MEMORY_LEAVE( !SRVSTREAM(0)->Sink().Data().IsEnabled() )                         
    //srvstream asserts
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().Data().IsEnabled() );
    EUNIT_ASSERT( !SRVSTREAM(1)->Sink().Data().IsEnabled() );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().Data().IsEnabled() );
    
    MCE_CHECK_MEMORY_LEAVE( !SRVSTREAM(1)->Source().Data().IsEnabled() )
    EUNIT_ASSERT( SRVSTREAM(1)->Source().Data().IsEnabled() );
                                
    Reset();
//1. started
    iManager->MccMediaStarted( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    iManager->MccMediaStarted( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 

    //srvstream asserts
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().Data().IsEnabled() );
    EUNIT_ASSERT( !SRVSTREAM(1)->Sink().Data().IsEnabled() );
    MCE_CHECK_MEMORY_LEAVE( !SRVSTREAM(0)->Source().Data().IsEnabled() )
    EUNIT_ASSERT( SRVSTREAM(0)->Source().Data().IsEnabled() );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().Data().IsEnabled() );

    //server asserts
    EUNIT_ASSERT( iServer->iEvent == EMceMediaUpdated );
    EUNIT_ASSERT( iServer->iSession == iSession );

    
    Reset();
//encode 
    iSdpDocument = iManager->EncodeL( *iSession );

    delete iSdpDocument;
    iSdpDocument = NULL;
    
    iSdpDocument = CSdpDocument::DecodeL( KMceMMTestSdpAnswerAudioIMS5PrecsSendrecv );

    Reset();
//decode answer    
    iSession->AnswerType() = KMceNegotiationAnswerTypeFinal;
    TMceSipWarningCode decodeStatus = iManager->DecodeL( *iSession, *iSdpDocument );
    EUNIT_ASSERT( decodeStatus == KErrNone );

    //srvstream asserts
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().Data().IsEnabled() );
    EUNIT_ASSERT( !SRVSTREAM(1)->Sink().Data().IsEnabled() );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().Data().IsEnabled() );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().Data().IsEnabled() );

    //session asserts
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceOfferingMedia );
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    //srvstream asserts
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EPrepared );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().Data().IsEnabled() );
    EUNIT_ASSERT( !SRVSTREAM(1)->Sink().Data().IsEnabled() );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().Data().IsEnabled() );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().Data().IsEnabled() );

    Reset();
//update
    status = iManager->UpdateL( *iSession );
    EUNIT_ASSERT( status == KMceAsync );

    //session asserts
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceStartingOffererSendStreams );
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccGetCodec );//validate
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccSetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccSetRemoteAddress );
    EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::EMccStartStream );
    //always synchronizing
    EUNIT_ASSERT( iMcc->iActions[ 4 ].iAction == CMccControllerStub::EMccGetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 5 ].iAction == CMccControllerStub::EMccSetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 6 ].iAction == CMccControllerStub::ENone );
    
    //srvstream asserts
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarting );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().Data().IsEnabled() );
    EUNIT_ASSERT( !SRVSTREAM(1)->Sink().Data().IsEnabled() );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().Data().IsEnabled() );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().Data().IsEnabled() );

    Reset();
//2. started source
    iManager->MccMediaStarted( iSession->iMccID,
                               SRVSTREAM(1)->LinkId(),
                               SRVSTREAM(1)->Id(),
                               SRVSTREAM(1)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    
    //session asserts
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceMediaNegotiated );
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == EMceMediaUpdated );
    EUNIT_ASSERT( iServer->iSession == iSession );
    
    //actions asserts
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    //srvstream asserts
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EPrepared ); //sink prepared & source started == prepared
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().Data().IsEnabled() );
    EUNIT_ASSERT( !SRVSTREAM(1)->Sink().Data().IsEnabled() );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().Data().IsEnabled() );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().Data().IsEnabled() );

    CMceComSession::Delete( iSession, *iManager );

    EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 0 );
    EUNIT_ASSERT( iManager->iSdpCodecs.Count() == 2 );

    }

//
// Tests
//
// TMceReturnStatus ReserveL( CMceComSession& aSession, const TDesC8& aToken )
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_ReserveLL()
    {
    SETUP( CreateMultimediaOutSessionL );

//reserve no session
    MCE_EUNIT_ASSERT_LEAVE( iManager->ReserveL( *iSession, KNullDesC8 ) );

    FULL_SETUP( CreateMultimediaOutSessionL );

//reserve
    TMceReturnStatus status = iManager->ReserveL( *iSession, KNullDesC8 );
    EUNIT_ASSERT( status == KMceReady );
    }

//
// Tests
//
// TBool ReservationNeeded( CMceComSession& aSession )
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_ReservationNeededL()
    {
    FULL_SETUP( CreateMultimediaOutSessionL );

//update
    TMceReturnStatus status = iManager->UpdateL( *iSession );
    EUNIT_ASSERT( status == KMceAsync );

    EUNIT_ASSERT( !iManager->ReservationNeeded( *iSession ) );
    }

//
// Tests
//
// TBool NeedToNegotiate( CMceComSession& aSession )
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_NeedToNegotiateL()
    {
    FULL_SETUP( CreateMultimediaOutSessionL );

//update
    TMceReturnStatus status = iManager->UpdateL( *iSession );
    EUNIT_ASSERT( status == KMceAsync );

    EUNIT_ASSERT( !iManager->NeedToNegotiate( *iSession ) );
    }

void UT_CMceMediaManager::UT_CMceMediaManager_TranscodeFileLL()
    {
    EUNIT_ASSERT_LEAVE( iManager->TranscodeFileL( NULL ) );
    
    CMceComFileSource* endpoint = CMceComFileSource::NewLC();
    endpoint->iFileInfo.iFileName.Copy( _L8("C:\\test.3gp" ) );
    endpoint->iTranscodeInfo.iFileName.Copy( _L8("C:\\test.mpeg" ) );
    
    iMcc->iLeaveError = KErrNotFound;
    EUNIT_ASSERT_LEAVE( iManager->TranscodeFileL( endpoint ) );
    EUNIT_ASSERT( iManager->iTranscodeSessions.Count() == 0 );
    
    Reset();
    iMcc->iLeaveError = KErrNone;
    EUNIT_ASSERT( iManager->TranscodeFileL( endpoint ) > 0 );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccTranscodeFile );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iManager->iTranscodeSessions.Count() == 1 );
    
    CleanupStack::Pop( endpoint );
    }

void UT_CMceMediaManager::UT_CMceMediaManager_TranscodeFileLWithCodecL()
    {
	CMceComFileSource* endpoint = NULL;
    FULL_SETUP_AND_PREPARE( CreateVideoOutSessionFileSourceL );
    RPointerArray<CMceComMediaStream>& streams = iSession->Streams();
    for ( int i = 0; i < streams.Count();i++ )
    	{
    	if ( streams[i]->Source()->iType == 4 )
    		{
    		endpoint = static_cast<CMceComFileSource*>( 
                  								streams[i]->Source());
            endpoint->iFileInfo.iFileName.Copy( _L8("C:\\test.3gp" ) );
    		endpoint->iTranscodeInfo.iFileName.Copy( _L8("C:\\test.mpeg" ) );      								
    		}
    	}

    Reset();
    iMcc->iLeaveError = KErrNone;
    TUint32 id = 0;
    id = iManager->TranscodeFileL( endpoint );
    EUNIT_ASSERT( id > 0 );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccTranscodeFile );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iManager->iTranscodeSessions.Count() == 1 );
    iManager->CancelTranscodeFileL( id );
    
    }


void UT_CMceMediaManager::UT_CMceMediaManager_TranscodingSessionL()
    {
    CMceComFileSource* endpoint = CMceComFileSource::NewLC();
    endpoint->iFileInfo.iFileName.Copy( _L8("C:\\test.3gp" ) );
    endpoint->iTranscodeInfo.iFileName.Copy( _L8("C:\\test.mpeg" ) );
    
    TUint32 sessionid = KMceNotAssigned;

    EUNIT_ASSERT( iManager->TranscodingSession( sessionid ) == NULL );
    
    sessionid = iManager->TranscodeFileL( endpoint );
    CleanupStack::Pop( endpoint );

    EUNIT_ASSERT( iManager->TranscodingSession( sessionid ) == endpoint );
    
    EUNIT_ASSERT( iManager->TranscodingSession( KMceNotAssigned ) == NULL );
    }

void UT_CMceMediaManager::UT_CMceMediaManager_TranscodingSession_2L()
    {
    CMceComFileSource* endpoint = CMceComFileSource::NewLC();
    endpoint->iFileInfo.iFileName.Copy( _L8("C:\\test.3gp" ) );
    endpoint->iTranscodeInfo.iFileName.Copy( _L8("C:\\test.mpeg" ) );
    
    EUNIT_ASSERT( iManager->TranscodingSession( *endpoint ) == NULL );
    
    iManager->TranscodeFileL( endpoint );
    CleanupStack::Pop( endpoint );

    EUNIT_ASSERT( iManager->TranscodingSession( *endpoint ) == endpoint );
    
    endpoint->iID = TMceMediaId(1);
    EUNIT_ASSERT( iManager->TranscodingSession( KMceNotAssigned ) == NULL );
    }

void UT_CMceMediaManager::UT_CMceMediaManager_CancelTranscodeFileLL()
    {
    iManager->CancelTranscodeFileL( 10 );
    
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iManager->iTranscodeSessions.Count() == 0 );
    
    CMceComFileSource* endpoint = CMceComFileSource::NewLC();
    endpoint->iFileInfo.iFileName.Copy( _L8("C:\\test.3gp" ) );
    endpoint->iTranscodeInfo.iFileName.Copy( _L8("C:\\test.mpeg" ) );
    
    Reset();
    TUint32 sessionId = iManager->TranscodeFileL( endpoint );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccTranscodeFile );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iManager->iTranscodeSessions.Count() == 1 );
    
    CleanupStack::Pop( endpoint );
    
    Reset();
    iManager->CancelTranscodeFileL( sessionId );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccCancelTranscodeFile );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iManager->iTranscodeSessions.Count() == 0 );
    }

void UT_CMceMediaManager::UT_CMceMediaManager_ConfirmationNeededL()
    {
    SETUP( CreateAudioOutSessionL );

    EUNIT_ASSERT( !iManager->ConfirmationNeeded( *iSession ) )
    
    FULL_SETUP( CreateAudioOutSessionL );
    EUNIT_ASSERT( !iManager->ConfirmationNeeded( *iSession ) )
    }


void UT_CMceMediaManager::UT_CMceMediaManager_SessionL()
    {
    const TUint32 KMyMccId = 100;
    
    EUNIT_ASSERT( iManager->Session( KMceNotAssigned ) == NULL );
    
    FULL_SETUP( CreateAudioOutSessionL );
    
    EUNIT_ASSERT( iManager->Session( iSession->iMccID ) == NULL );

    iSession->iMccID = KMyMccId;

    EUNIT_ASSERT( iManager->Session( iSession->iMccID ) == iSession );
    EUNIT_ASSERT( iManager->Session( iSession->iMccID+1 ) == NULL );
    
    iSession->SdpSession().SetMediaSession( NULL );
    EUNIT_ASSERT( iManager->Session( iSession->iMccID ) == NULL );

    iSession->SdpSession().SetBackup( iSession );
    iSession->iMccID = KMceNotAssigned;
    EUNIT_ASSERT( iManager->Session( iSession->iMccID ) == NULL );
    
    iSession->iMccID = KMyMccId;
    EUNIT_ASSERT( iManager->Session( iSession->iMccID ) == iSession );
    
    // Forking cases
    CMceComSession* clone = iSession->CloneL();
    CleanupStack::PushL( clone );
    clone->iMccID = KMyMccId;
    CMceSdpSession* sdpSession = 
        CMceSdpSession::NewLC( *clone, *iManager );
    sdpSession->iIsMaster = EFalse;
    iManager->iMceSdpSessions.InsertL( sdpSession, 0 );
    CleanupStack::Pop( sdpSession );
    
    // Original is the master session, but second in the array.
    // Clone is first in the array, but not the master.
    // -> Original matches.
    EUNIT_ASSERT( iManager->Session( KMyMccId ) == iSession );
    
    // Clone is the master and the first in the array.
    // -> Clone matches.
    iSession->SdpSession().iIsMaster = EFalse;
    sdpSession->iIsMaster = ETrue; // clone  
    EUNIT_ASSERT( iManager->Session( KMyMccId ) == clone );
    
    CleanupStack::PopAndDestroy( clone );
    
    iSession->SdpSession().SetMediaSession( iSession );
    iSession->SdpSession().SetBackup( NULL );
    }
    
    
void UT_CMceMediaManager::UT_CMceMediaManager_InitializeMccStreamLL()
    {
    FULL_SETUP_AND_PREPARE( CreateMultimediaOutSessionL );
    
    iManager->InitializeMccStreamL( *iSession->MccStreams()[0] );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    Reset();
    iSession->MccStreams()[0]->Id() = 1;
    CMceAdoptedSrvStream* adopt = CMceAdoptedSrvStream::NewL( *iSession->MccStreams()[0] );
    CleanupStack::PushL( adopt );
    
    EUNIT_ASSERT_LEAVE( iManager->InitializeMccStreamL( *adopt ) );
    
    CleanupStack::PopAndDestroy( adopt );
    }
    
void UT_CMceMediaManager::UT_CMceMediaManager_CreateMccLinkLL()
    {
    FULL_SETUP_AND_PREPARE( CreateMultimediaBundledOutSessionL );
    
    iManager->CreateMccLinkL( *iSession->MccStreams()[0] );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccCreateLink );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    
    Reset();
    EUNIT_ASSERT_LEAVE( iManager->CreateMccLinkL( *iSession->MccStreams()[0] ) );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );

    Reset();
    iSession->MccStreams()[0]->Source().SetState( CMceSrvStream::ECreated );
    iSession->MccStreams()[0]->Sink().SetState( CMceSrvStream::ECreated );
    iSession->MccStreams()[0]->Data().iLinkId = KMceNotAssigned;
    iSession->MccStreams()[1]->Data().iLinkId = 10;
    
    
    iManager->CreateMccLinkL( *iSession->MccStreams()[0] );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccCreateLink );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );

    Reset();
    iSession->MccStreams()[0]->Source().SetState( CMceSrvStream::ECreated );
    iSession->MccStreams()[0]->Sink().SetState( CMceSrvStream::ECreated );
    iManager->CreateMccLinkL( *iSession->MccStreams()[5] );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccCreateLink );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );

    Reset();
    iSession->MccStreams()[0]->Source().SetState( CMceSrvStream::ECreated );
    iSession->MccStreams()[0]->Sink().SetState( CMceSrvStream::ECreated );
    iManager->CreateMccLinkL( *iSession->MccStreams()[6] );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    
    iSession->MccStreams()[0]->Id() = 1;
    CMceAdoptedSrvStream* adopt = CMceAdoptedSrvStream::NewL( *iSession->MccStreams()[0] );
    CleanupStack::PushL( adopt );

    EUNIT_ASSERT_LEAVE( iManager->CreateMccLinkL( *adopt ) );
    CleanupStack::PopAndDestroy( adopt );
    
    // Secure Session Create MCC Links
    CMccInterface* Interface = CMccInterface::NewL( *iManager );

    CleanupStack::PushL( Interface );
    CMccSecureInterface* SecureInterface = CMccSecureInterface::NewL( *Interface );
	CleanupStack::PushL( SecureInterface );
	CMceSecureMediaSession* SecSession =
		CMceSecureMediaSession::NewL( *iSession,
                              		  *iManager,
                                      *SecureInterface );
                                     
    CleanupStack::PushL( SecSession );
    iSession->iSecureMediaSession = SecSession;
     
    CleanupStack::Pop( SecSession );
    CleanupStack::Pop( SecureInterface );
    
    iSession->iIsSecureSession = ETrue;
	                                     
	iManager->CreateMccLinkL( *iSession->MccStreams()[3] );    
	
	EUNIT_ASSERT( iSession->SecureSession() );
	EUNIT_ASSERT( iSession->SecureSession()->iCryptoContextUpdate == ETrue );

 	iSession->SdpSession().iIsMaster = EFalse;
 	
 	delete iSession->iSecureMediaSession;
 	iSession->iSecureMediaSession = NULL;
 
 	CleanupStack::PopAndDestroy( Interface );
    delete SecureInterface;
    }
    
void UT_CMceMediaManager::UT_CMceMediaManager_CreateMccStreamLL()
    {
    FULL_SETUP_AND_PREPARE( CreateMultimediaBundledOutSessionL );
    
    EUNIT_ASSERT_LEAVE( iManager->CreateMccStreamL( *iSession->MccStreams()[4], EMceRoleOfferer ) );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );

    Reset();
    iSession->MccStreams()[4]->Source().SetState( CMceSrvStream::ELinkCreated );
    iSession->MccStreams()[4]->Sink().SetState( CMceSrvStream::ELinkCreated );
    iManager->CreateMccStreamL( *iSession->MccStreams()[4], EMceRoleOfferer );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );

    iSession->MccStreams()[0]->Source().SetState( CMceSrvStream::ELinkCreated );
    iSession->MccStreams()[0]->Sink().SetState( CMceSrvStream::ELinkCreated );
    iManager->CreateMccStreamL( *iSession->MccStreams()[0], EMceRoleOfferer );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccGetSourceId );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccGetSinkId );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccCreateStream );
    EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::EMccSetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 4 ].iAction == CMccControllerStub::ENone );

    iSession->MccStreams()[2]->Source().SetState( CMceSrvStream::ELinkCreated );
    iSession->MccStreams()[2]->Sink().SetState( CMceSrvStream::ELinkCreated );
    iManager->CreateMccStreamL( *iSession->MccStreams()[2], EMceRoleOfferer );
    iSession->MccStreams()[3]->Source().SetState( CMceSrvStream::ELinkCreated );
    iSession->MccStreams()[3]->Sink().SetState( CMceSrvStream::ELinkCreated );
    iManager->CreateMccStreamL( *iSession->MccStreams()[3], EMceRoleOfferer );
    
    Reset();
    iSession->MccStreams()[5]->Source().SetState( CMceSrvStream::ELinkCreated );
    iSession->MccStreams()[5]->Sink().SetState( CMceSrvStream::ELinkCreated );
    iManager->CreateMccStreamL( *iSession->MccStreams()[5], EMceRoleOfferer );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccGetSourceId );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccGetSinkId );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccCreateStream );
    EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::EMccSetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 4 ].iAction == CMccControllerStub::ENone );
    
    Reset();
    iSession->MccStreams()[6]->Source().SetState( CMceSrvStream::ELinkCreated );
    iSession->MccStreams()[6]->Sink().SetState( CMceSrvStream::ELinkCreated );
    iManager->CreateMccStreamL( *iSession->MccStreams()[6], EMceRoleOfferer );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccReuse );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccReuse );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccCreateStream );
    EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::EMccSetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 4 ].iAction == CMccControllerStub::ENone );

    
    iSession->MccStreams()[0]->Id() = 1;
    CMceAdoptedSrvStream* adopt = CMceAdoptedSrvStream::NewL( *iSession->MccStreams()[0] );
    CleanupStack::PushL( adopt );
    
    EUNIT_ASSERT_LEAVE( iManager->CreateMccStreamL( *adopt, EMceRoleOfferer ) );
    
    CleanupStack::PopAndDestroy( adopt );
    }
    
void UT_CMceMediaManager::UT_CMceMediaManager_PrepareMccStreamLL()
    {
    FULL_SETUP_AND_PREPARE( CreateMultimediaBundledOutSessionL );
    
    iSession->MccStreams()[4]->Source().SetState( CMceSrvStream::ELinkCreated );
    iSession->MccStreams()[4]->Sink().SetState( CMceSrvStream::ELinkCreated );
    iManager->PrepareMccStreamL( *iSession->MccStreams()[4], EMceRoleOfferer );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );

    Reset();
    iSession->MccStreams()[0]->Source().SetState( CMceSrvStream::ELinkCreated );
    iSession->MccStreams()[0]->Sink().SetState( CMceSrvStream::ELinkCreated );
    iManager->PrepareMccStreamL( *iSession->MccStreams()[0], EMceRoleOfferer );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccPrepareStream );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    
    
    iSession->MccStreams()[0]->Id() = 1;
    CMceAdoptedSrvStream* adopt = CMceAdoptedSrvStream::NewL( *iSession->MccStreams()[0] );
    CleanupStack::PushL( adopt );
    
    EUNIT_ASSERT_LEAVE( iManager->PrepareMccStreamL( *adopt, EMceRoleOfferer ) );
    
    CleanupStack::PopAndDestroy( adopt );
    }
    
void UT_CMceMediaManager::UT_CMceMediaManager_StartMccStreamLL()
    {
    FULL_SETUP_AND_PREPARE( CreateMultimediaBundledOutSessionL );

    EUNIT_ASSERT_LEAVE( iManager->StartMccStreamL( *iSession->MccStreams()[4] ) );
    
    iSession->MccStreams()[4]->Source().SetState( CMceSrvStream::EPrepared );
    iSession->MccStreams()[4]->Sink().SetState( CMceSrvStream::EPrepared );
    static_cast<CMceComDisplaySink&>( iSession->MccStreams()[4]->Sink().Data() ).SetViewFinder( ETrue );
    
    iManager->StartMccStreamL( *iSession->MccStreams()[4] );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );


    Reset();
    iSession->MccStreams()[0]->Source().SetState( CMceSrvStream::EPrepared );
    iSession->MccStreams()[0]->Sink().SetState( CMceSrvStream::EPrepared );
    iSession->MccStreams()[0]->Id() = 1;
    iManager->StartMccStreamL( *iSession->MccStreams()[0] );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStartStream );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );


    Reset();
    iSession->MccStreams()[0]->Source().Data().Enabled( EFalse );
    iSession->MccStreams()[0]->Sink().Data().Enabled( EFalse );
    
    iSession->MccStreams()[0]->Source().SetState( CMceSrvStream::EPrepared );
    iSession->MccStreams()[0]->Sink().SetState( CMceSrvStream::EPrepared );
    
    iManager->StartMccStreamL( *iSession->MccStreams()[0] );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );

    iSession->MccStreams()[0]->Source().SetState( CMceSrvStream::EPrepared );
    iSession->MccStreams()[0]->Sink().SetState( CMceSrvStream::EPending );
    iManager->StartMccStreamL( *iSession->MccStreams()[0] );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );

    iSession->MccStreams()[0]->Source().SetState( CMceSrvStream::EPending );
    iSession->MccStreams()[0]->Sink().SetState( CMceSrvStream::EPrepared );
    iManager->StartMccStreamL( *iSession->MccStreams()[0] );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    
    iSession->MccStreams()[0]->Id() = 1;
    CMceAdoptedSrvStream* adopt = CMceAdoptedSrvStream::NewL( *iSession->MccStreams()[0] );
    CleanupStack::PushL( adopt );
    
    EUNIT_ASSERT_LEAVE( iManager->StartMccStreamL( *adopt ) );
    
    CleanupStack::PopAndDestroy( adopt );
    }
    
void UT_CMceMediaManager::UT_CMceMediaManager_AdjustMccStreamLL()
    {
    FULL_SETUP_AND_PREPARE( CreateMultimediaBundledOutSessionL );

    EUNIT_ASSERT_LEAVE( iManager->AdjustMccStreamL( *iSession->MccStreams()[4] ) );
    
    iSession->MccStreams()[0]->Source().SetState( CMceSrvStream::EPrepared );
    iSession->MccStreams()[0]->Sink().SetState( CMceSrvStream::EPrepared );
    
    iManager->AdjustMccStreamL( *iSession->MccStreams()[0] );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccGetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccSetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::ENone );

    
    iSession->MccStreams()[0]->Id() = 1;
    CMceAdoptedSrvStream* adopt = CMceAdoptedSrvStream::NewL( *iSession->MccStreams()[0] );
    CleanupStack::PushL( adopt );
    
    EUNIT_ASSERT_LEAVE( iManager->AdjustMccStreamL( *adopt ) );
    
    CleanupStack::PopAndDestroy( adopt );
    
    Reset();
    
    // Test adjusting bidirectional codec
    iSession->iMccStreams.ResetAndDestroy();
    iSession->iMccAdoptedStreams.ResetAndDestroy();
    delete iSession->iNegotiationState;
    iSession->iNegotiationState = NULL;
    delete iSession;
    iSession = NULL;
    iSession = MceMediaManagerTestHelper::CreateAudioOutSessionL( NULL, KNullDesC8, KMceSDPNameiLBC() );
    iSession->SetObserver( *iServer );
    iManager->CreateSessionL( *iSession );
    iSession->PrepareL( *iManager );

    iSession->MccStreams()[0]->Source().SetState( CMceSrvStream::EPrepared );
    iSession->MccStreams()[0]->Sink().SetState( CMceSrvStream::EPrepared );
    iSession->MccStreams()[1]->Source().SetState( CMceSrvStream::EPrepared );
    iSession->MccStreams()[1]->Sink().SetState( CMceSrvStream::EPrepared );
    
    iSession->MccStreams()[0]->Codec().iBitrate = 15200;
    
    iManager->AdjustMccStreamL( *iSession->MccStreams()[0] );
    
    Reset();
    }
    
void UT_CMceMediaManager::UT_CMceMediaManager_ValidateMccStreamLL()
    {
    FULL_SETUP_AND_PREPARE( CreateMultimediaBundledOutSessionL );

    iManager->ValidateMccStreamL( *iSession->MccStreams()[4] );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    Reset();
    iSession->MccStreams()[0]->Source().SetState( CMceSrvStream::EPrepared );
    iSession->MccStreams()[0]->Sink().SetState( CMceSrvStream::EPrepared );
    iManager->ValidateMccStreamL( *iSession->MccStreams()[0] );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );

    iSession->MccStreams()[0]->Source().SetState( CMceSrvStream::ELinkCreated );
    iSession->MccStreams()[0]->Sink().SetState( CMceSrvStream::ELinkCreated );
    iManager->CreateMccStreamL( *iSession->MccStreams()[0], EMceRoleOfferer );
    
    Reset();
    iManager->ValidateMccStreamL( *iSession->MccStreams()[0] );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccGetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );

    
    CMceAdoptedSrvStream* adopt = CMceAdoptedSrvStream::NewL( *iSession->MccStreams()[0] );
    CleanupStack::PushL( adopt );
    
    EUNIT_ASSERT_LEAVE( iManager->ValidateMccStreamL( *adopt ) );
    
    CleanupStack::PopAndDestroy( adopt );
    }
    
void UT_CMceMediaManager::UT_CMceMediaManager_StopMccStreamL()
    {
    FULL_SETUP_AND_PREPARE( CreateMultimediaBundledOutSessionL );
    
    iManager->StopMccStream( *iSession->MccStreams()[0] );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    iSession->MccStreams()[0]->Id() = 1;
    Reset();
    iManager->StopMccStream( *iSession->MccStreams()[0] );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStopStream );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccDeleteStream );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccCloseLink );
    EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::ENone );
    
    Reset();
    iMcc->iStreamExists = 1;
    iManager->StopMccStream( *iSession->MccStreams()[0] );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStopStream );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccDeleteStream );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::ENone );
    
    CMceAdoptedSrvStream* adopt = CMceAdoptedSrvStream::NewL( *iSession->MccStreams()[0] );
    CleanupStack::PushL( adopt );
    
    iManager->StopMccStream( *adopt );
    
    CleanupStack::PopAndDestroy( adopt );
    }
    
    
void UT_CMceMediaManager::UT_CMceMediaManager_SynchronizeMccStreamLL()
    {
    FULL_SETUP_AND_PREPARE( CreateMultimediaBundledOutSessionL );

    iManager->SynchronizeMccStreamL( *iSession->MccStreams()[0], EMceRoleOfferer );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    iSession->MccStreams()[0]->Source().SetState( CMceSrvStream::ELinkCreated );
    iSession->MccStreams()[0]->Sink().SetState( CMceSrvStream::ELinkCreated );
    iManager->CreateMccStreamL( *iSession->MccStreams()[0], EMceRoleOfferer );
    
    iSession->MccStreams()[0]->Source().SetState( CMceSrvStream::EPrepared );
    iSession->MccStreams()[0]->Sink().SetState( CMceSrvStream::EPrepared );
    Reset();
    iManager->SynchronizeMccStreamL( *iSession->MccStreams()[0], EMceRoleOfferer );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccGetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccSetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccStartStream );
    EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::ENone );

    iSession->MccStreams()[0]->Source().SetState( CMceSrvStream::EPending );
    iSession->MccStreams()[0]->Sink().SetState( CMceSrvStream::EPending );
    Reset();
    iManager->SynchronizeMccStreamL( *iSession->MccStreams()[0], EMceRoleOfferer );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccGetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccSetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccStartStream );
    EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::ENone );

    TUint32 sourceId = iSession->MccStreams()[0]->Source().Id();
    TUint32 sinkId = iSession->MccStreams()[0]->Sink().Id();
    
    iSession->MccStreams()[0]->Source().SetState( CMceSrvStream::EPrepared );
    iSession->MccStreams()[0]->Sink().SetState( CMceSrvStream::EPrepared );
    iSession->MccStreams()[0]->Source().Data().Enabled( EFalse );
    Reset();
    
    iManager->SynchronizeMccStreamL( *iSession->MccStreams()[0], sourceId, 0 );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStartStream );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );

    iSession->MccStreams()[0]->Source().SetState( CMceSrvStream::EPrepared );
    iSession->MccStreams()[0]->Sink().SetState( CMceSrvStream::EPrepared );
    iSession->MccStreams()[0]->Sink().Data().Enabled( EFalse );
    Reset();
    
    iManager->SynchronizeMccStreamL( *iSession->MccStreams()[0], sourceId, 0 );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStartStream );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );

    iSession->MccStreams()[0]->Source().SetState( CMceSrvStream::EPrepared );
    iSession->MccStreams()[0]->Sink().SetState( CMceSrvStream::EPrepared );
    Reset();
    
    iManager->SynchronizeMccStreamL( *iSession->MccStreams()[0], sinkId, 0 );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStartStream );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );

    
    iSession->MccStreams()[0]->Source().Data().Enabled( ETrue );
    iSession->MccStreams()[0]->Sink().Data().Enabled( ETrue );
    iSession->MccStreams()[0]->Source().SetState( CMceSrvStream::EPaused );
    iSession->MccStreams()[0]->Sink().SetState( CMceSrvStream::EStarted );
    Reset();
    iManager->SynchronizeMccStreamL( *iSession->MccStreams()[0], EMceRoleOfferer );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccGetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccSetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccResumeStream );
    EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::ENone );

    iSession->MccStreams()[0]->Source().SetState( CMceSrvStream::EStarted );
    iSession->MccStreams()[0]->Sink().SetState( CMceSrvStream::EPaused );
    Reset();
    iManager->SynchronizeMccStreamL( *iSession->MccStreams()[0], EMceRoleOfferer );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccGetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccSetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccResumeStream );
    EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::ENone );

    iSession->MccStreams()[0]->Source().SetState( CMceSrvStream::EPaused );
    iSession->MccStreams()[0]->Sink().SetState( CMceSrvStream::EPaused );
    Reset();
    iManager->SynchronizeMccStreamL( *iSession->MccStreams()[0], EMceRoleOfferer );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccGetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccSetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccResumeStream );
    EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::ENone );

    iSession->MccStreams()[0]->Source().SetState( CMceSrvStream::EStarted );
    iSession->MccStreams()[0]->Sink().SetState( CMceSrvStream::EStarted );
    Reset();
    iManager->SynchronizeMccStreamL( *iSession->MccStreams()[0], EMceRoleOfferer );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccGetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccSetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::ENone );

    iSession->MccStreams()[0]->Source().SetState( CMceSrvStream::EStarted );
    iSession->MccStreams()[0]->Sink().SetState( CMceSrvStream::EStarted );
    iSession->MccStreams()[0]->Source().Data().Enabled( EFalse );
    Reset();
    iManager->SynchronizeMccStreamL( *iSession->MccStreams()[0], EMceRoleOfferer );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccGetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccSetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccPauseStream );
    EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::ENone );

    iSession->MccStreams()[0]->Source().SetState( CMceSrvStream::EStarted );
    iSession->MccStreams()[0]->Sink().SetState( CMceSrvStream::EStarted );
    iSession->MccStreams()[0]->Source().Data().Enabled( ETrue );
    iSession->MccStreams()[0]->Sink().Data().Enabled( EFalse );
    Reset();
    iManager->SynchronizeMccStreamL( *iSession->MccStreams()[0], EMceRoleOfferer );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccGetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccSetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccPauseStream );
    EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::ENone );

    iSession->MccStreams()[0]->Source().SetState( CMceSrvStream::EStarted );
    iSession->MccStreams()[0]->Sink().SetState( CMceSrvStream::EStarted );
    iSession->MccStreams()[0]->Source().Data().Enabled( EFalse );
    iSession->MccStreams()[0]->Sink().Data().Enabled( EFalse );
    Reset();
    
    iManager->SynchronizeMccStreamL( *iSession->MccStreams()[0], EMceRoleOfferer );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccGetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccSetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccPauseStream );
    EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::ENone );
    }
    
void UT_CMceMediaManager::UT_CMceMediaManager_CleanupLL()
    {
    FULL_SETUP( CreateMultimediaBundledOutSessionL );

    EUNIT_ASSERT( iSession->MccStreams().Count() == 0 );
    iManager->CleanupL( *iSession );
    EUNIT_ASSERT( iSession->MccStreams().Count() == 0 );
    
    iSession->PrepareL( *iManager );
    EUNIT_ASSERT( iSession->MccStreams().Count() == 7 );

    iSession->MccStreams()[0]->Data().SetRemoteMediaPort( 400 );
    iSession->MccStreams()[0]->Source().SetState( CMceSrvStream::EStarted );
    iSession->MccStreams()[0]->Sink().SetState( CMceSrvStream::EStarted );
    iSession->MccStreams()[0]->Data().Enable( EFalse, EFalse );
    iSession->MccStreams()[1]->Codec().iIsNegotiated = EFalse;
    iSession->MccStreams()[2]->Sink().SetState( CMceSrvStream::EStarted );
    iSession->MccStreams()[2]->Source().SetState( CMceSrvStream::EStarted );

    Reset();
    iSession->AnswerType() = KMceNegotiationAnswerTypeFinal;
    iManager->CleanupL( *iSession );
    
    EUNIT_ASSERT( iSession->MccStreams().Count() == 3 ); //video sendrecv + audio send removed
    
    Reset();
    iManager->CleanupL( *iSession );
    EUNIT_ASSERT( iSession->MccStreams().Count() == 3 );
    }
    
void UT_CMceMediaManager::UT_CMceMediaManager_DisposeMediaStreamLL()
    {
    EUNIT_ASSERT_SPECIFIC_LEAVE( iManager->DisposeMediaStreamL( NULL ), KErrArgument );
    
    FULL_SETUP_AND_PREPARE( CreateMultimediaBundledOutSessionL );

    EUNIT_ASSERT( iSession->Streams().Count() == 5 );
    EUNIT_ASSERT( iSession->MccStreams().Count() == 7 );
    
    iSession->MccStreams()[0]->Data().SetRemoteMediaPort( 400 );
    iSession->MccStreams()[0]->Source().SetState( CMceSrvStream::EStarted );
    iSession->MccStreams()[0]->Sink().SetState( CMceSrvStream::EStarted );
    iSession->MccStreams()[1]->Source().SetState( CMceSrvStream::EPrepared );
    iSession->MccStreams()[1]->Sink().SetState( CMceSrvStream::EPrepared );
    
    CMceComMediaStream* stream = &iSession->MccStreams()[0]->Data();
    
    iManager->DisposeMediaStreamL( stream );
    EUNIT_ASSERT( iSession->Streams().Count() == 5 );
    EUNIT_ASSERT( iSession->MccStreams().Count() == 6 );
    EUNIT_ASSERT( !stream->IsEnabled() );
    
    stream = CMceComAudioStream::NewLC();
    stream->InitializeL( *iSession );
    EUNIT_ASSERT( stream->IsEnabled() );

    iManager->DisposeMediaStreamL( stream );
    EUNIT_ASSERT( !stream->IsEnabled() );
    
    iSession->AddStreamL( stream );
    CleanupStack::Pop( stream );
    EUNIT_ASSERT( iSession->Streams().Count() == 6 );
    EUNIT_ASSERT( iSession->MccStreams().Count() == 6 );

    iManager->DisposeMediaStreamL( stream );
    EUNIT_ASSERT( iSession->Streams().Count() == 5 );
    EUNIT_ASSERT( iSession->MccStreams().Count() == 6 );
    }
    
        
void UT_CMceMediaManager::UT_CMceMediaManager_DisposeCodecLL()
    {
    FULL_SETUP_AND_PREPARE( CreateMultimediaBundledOutSessionL );
    EUNIT_ASSERT( iSession->MccStreams().Count() == 7 );

    CMceComAudioStream* stream = &static_cast<CMceComAudioStream&>( iSession->MccStreams()[0]->Data() );
    EUNIT_ASSERT( stream->Codecs().Count() == 1 );
    CMceComAudioCodec* codec = stream->Codecs()[0];
    codec->iIsEnabled = EFalse;
    
    iManager->DisposeCodecL( codec );
    EUNIT_ASSERT( stream->Codecs().Count() == 0 );
    EUNIT_ASSERT( iSession->MccStreams().Count() == 5 );
    
    CMceComVideoStream* vstream1 = &static_cast<CMceComVideoStream&>( iSession->MccStreams()[0]->Data() );
    CMceComVideoStream* vstream2 = &static_cast<CMceComVideoStream&>( iSession->MccStreams()[1]->Data() );
    CMceComVideoStream* vstream3 = &static_cast<CMceComVideoStream&>( iSession->MccStreams()[2]->Data() );
    CMceComVideoStream* vstream4 = &static_cast<CMceComVideoStream&>( iSession->MccStreams()[3]->Data() );
    CMceComVideoStream* vstream5 = &static_cast<CMceComVideoStream&>( iSession->MccStreams()[4]->Data() );
    EUNIT_ASSERT( vstream1->Codecs().Count() == 1 );
    EUNIT_ASSERT( vstream2->Codecs().Count() == 1 );
    EUNIT_ASSERT( vstream3->Codecs().Count() == 1 );
    EUNIT_ASSERT( vstream4->Codecs().Count() == 1 );
    EUNIT_ASSERT( vstream5->Codecs().Count() == 1 );
    CMceComVideoCodec* vcodec = vstream1->Codecs()[0];
    vcodec->iIsEnabled = EFalse;
    
    iManager->DisposeCodecL( vcodec );
    EUNIT_ASSERT( vstream1->Codecs().Count() == 0 );
    EUNIT_ASSERT( vstream2->Codecs().Count() == 0 );
    EUNIT_ASSERT( vstream3->Codecs().Count() == 1 );
    EUNIT_ASSERT( vstream4->Codecs().Count() == 1 );
    EUNIT_ASSERT( vstream5->Codecs().Count() == 1 );
    EUNIT_ASSERT( iSession->MccStreams().Count() == 2 );

    
    }

void UT_CMceMediaManager::UT_CMceMediaManager_RequireSignallingLL()
    {
    FULL_SETUP_AND_PREPARE( CreateMultimediaBundledOutSessionL );

    iUpdate = iSession->CloneL();
    
    EUNIT_ASSERT( iManager->RequireSignallingL( *iSession, *iUpdate ) == KMceRequiresSipSignallingOnly );

    EUNIT_ASSERT( iManager->RequireSignallingL( *iUpdate, *iSession ) == KMceRequiresSipSignallingOnly );

    iUpdate->PrepareL( *iManager );
    EUNIT_ASSERT( iManager->RequireSignallingL( *iSession, *iUpdate ) == KMceRequiresSipSignallingOnly );

    iSession->MccStreams()[0]->Source().SetState( CMceSrvStream::ELinkCreated );
    iSession->MccStreams()[0]->Sink().SetState( CMceSrvStream::ELinkCreated );
    iManager->CreateMccStreamL( *iSession->MccStreams()[0], EMceRoleOfferer );
    EUNIT_ASSERT( iManager->RequireSignallingL( *iUpdate, *iSession ) == KMceRequiresSignalling );
    }
    
void UT_CMceMediaManager::UT_CMceMediaManager_CleanupSessionLL()
    {

    //TBD
        
    }
     
        

//
// Tests
//
// void UT_CMceMediaManager::CreateRejectedAnswer( CMceComSession& aSession )
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_CreateRejectedAnswerL()
    {
    SETUP( CreateInSessionL );
    Reset();
    
    //no SDP session
    iManager->CreateRejectedAnswer( *iSession );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    
    //ok
    FULL_SETUP(CreateInSessionL );
    Reset();
    iSdpDocument = CSdpDocument::DecodeL( KMceMMTestSdpOfferAudioIMS5PrecsSendrecv );
    iManager->DecodeL( *iSession, *iSdpDocument );
    EUNIT_DISABLE_ALLOC_DECORATOR;
    iManager->CreateRejectedAnswer( *iSession );
    EUNIT_ENABLE_ALLOC_DECORATOR;
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    
    //fails
    Reset();
    iSession->iLocalIpAddress = TInetAddr();
    iManager->CreateRejectedAnswer( *iSession );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iServer->iEvent == EMceMediaError );
    }

//
// Tests
//
// void UT_CMceMediaManager::PauseL( CMceComMediaSink& aSink )
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_PauseL_1L()
    {
    SETUP( CreateVideoOutSessionL );
    
    CMceComMediaSink& rtpsink = *iSession->Streams()[KVideo]->BoundStreamL().Sinks()[KRtpSink];
    CMceComMediaSink& displaysink = *iSession->Streams()[KCamera]->Sinks()[KDisplaySink];
    
    Reset();
//pause no sessiom    
    iManager->PauseL( rtpsink );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
//establish    
    iSdpDocument = MceMediaManagerTestHelper::EstablishOutSessionL( 
                                                *iSession,
                                                *iManager,
                                                KMceMMTestSdpAnswerVideoIMS5PrecsSendrecv,
                                                iServer );

    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->Sink().State() == CMceSrvStream::EStarted );
    
    Reset();
//pause audio rtp
    iManager->PauseL( rtpsink );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccPauseStream );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EPausing );
    EUNIT_ASSERT( SRVSTREAM(2)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->Sink().State() == CMceSrvStream::EStarted );


    Reset();
//pause camera display
    iManager->PauseL( displaysink );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamLocalVideo );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EPausing );
    EUNIT_ASSERT( SRVSTREAM(2)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->Sink().State() == CMceSrvStream::EPaused );
    }

//
// Tests
//
// void UT_CMceMediaManager::PauseL( CMceComMediaSource& aSource )
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_PauseL_2L()
    {
    SETUP( CreateVideoOutSessionL );
    
    CMceComMediaSource& rtpsource = *iSession->Streams()[KVideo]->Source();
    CMceComMediaSource& camera = *iSession->Streams()[KCamera]->Source();
    
    Reset();
//pause no sessiom    
    iManager->PauseL( rtpsource );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
//establish    
    iSdpDocument = MceMediaManagerTestHelper::EstablishOutSessionL( 
                                                *iSession,
                                                *iManager,
                                                KMceMMTestSdpAnswerVideoIMS5PrecsSendrecv,
                                                iServer );

    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->Sink().State() == CMceSrvStream::EStarted );
    
    Reset();
//pause video rtp
    iManager->PauseL( rtpsource );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccPauseStream );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EPausing );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->Sink().State() == CMceSrvStream::EStarted );


    Reset();
//pause camera 
    iManager->PauseL( camera );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccPauseStream );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EPausing );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EPausing );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->Source().State() == CMceSrvStream::EPaused );
    EUNIT_ASSERT( SRVSTREAM(2)->Sink().State() == CMceSrvStream::EStarted );
    }

//
// Tests
//
// void UT_CMceMediaManager::ResumeL( CMceComMediaSink& aSink )
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_ResumeL_1L()
    {
    SETUP( CreateVideoOutSessionL );
    
    CMceComMediaSink& rtpsink = *iSession->Streams()[KVideo]->BoundStreamL().Sinks()[KRtpSink];
    CMceComMediaSink& displaysink = *iSession->Streams()[KCamera]->Sinks()[KDisplaySink];
    
    Reset();
//resume no sessiom    
    iManager->ResumeL( rtpsink );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
//establish    
    iSdpDocument = MceMediaManagerTestHelper::EstablishOutSessionL( 
                                                *iSession,
                                                *iManager,
                                                KMceMMTestSdpAnswerVideoIMS5PrecsSendrecv,
                                                iServer );

    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->Sink().State() == CMceSrvStream::EStarted );
    
    Reset();
//resume video rtp
    iManager->ResumeL( rtpsink );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->Sink().State() == CMceSrvStream::EStarted );


    Reset();
    TMceMccComEvent paused( KMccStreamPaused );
    SRVSTREAM(1)->Sink().EventReceived( paused );
    SRVSTREAM(1)->Sink().Data().Enabled( EFalse );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EPaused );
    Reset();
    
//resume video rtp
    iManager->ResumeL( rtpsink );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccResumeStream );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarting );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStarting );
    EUNIT_ASSERT( SRVSTREAM(2)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->Sink().State() == CMceSrvStream::EStarted );


    Reset();
//resume camera display
    iManager->ResumeL( displaysink );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarting );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStarting );
    EUNIT_ASSERT( SRVSTREAM(2)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->Sink().State() == CMceSrvStream::EStarted );

//pause camera display
    Reset();
    SRVSTREAM(2)->Sink().EventReceived( paused );
    SRVSTREAM(2)->Sink().Data().Enabled( EFalse );
    EUNIT_ASSERT( SRVSTREAM(2)->Sink().State() == CMceSrvStream::EPaused );
    Reset();
    
//resume camera display
    iManager->ResumeL( displaysink );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamLocalVideo );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarting );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStarting );
    EUNIT_ASSERT( SRVSTREAM(2)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->Sink().State() == CMceSrvStream::EStarted );
    }

//
// Tests
//
// void UT_CMceMediaManager::ResumeL( CMceComMediaSource& aSource )
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_ResumeL_2L()
    {
    SETUP( CreateVideoOutSessionL );
    
    CMceComMediaSource& rtpsource = *iSession->Streams()[KVideo]->Source();
    CMceComMediaSource& camera = *iSession->Streams()[KCamera]->Source();
    
    Reset();
//resume no sessiom    
    iManager->ResumeL( rtpsource );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
//establish    
    iSdpDocument = MceMediaManagerTestHelper::EstablishOutSessionL( 
                                                *iSession,
                                                *iManager,
                                                KMceMMTestSdpAnswerVideoIMS5PrecsSendrecv,
                                                iServer );

    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->Sink().State() == CMceSrvStream::EStarted );
    
    Reset();
    TMceMccComEvent paused( KMccStreamPaused );
    SRVSTREAM(0)->Source().EventReceived( paused );
    SRVSTREAM(0)->Source().Data().Enabled( EFalse );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EPaused );
//resume video rtp
    iManager->ResumeL( rtpsource );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccResumeStream );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarting );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarting );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->Sink().State() == CMceSrvStream::EStarted );


    Reset();
//resume camera 
    iManager->ResumeL( camera );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarting );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarting );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->Sink().State() == CMceSrvStream::EStarted );

    SRVSTREAM(1)->Source().EventReceived( paused );
    SRVSTREAM(2)->Source().EventReceived( paused );
    SRVSTREAM(1)->Source().Data().Enabled( EFalse );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EPaused );
    EUNIT_ASSERT( SRVSTREAM(2)->Source().State() == CMceSrvStream::EPaused );

//resume camera 
    iManager->ResumeL( camera );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccResumeStream );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarting );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarting );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarting );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStarting );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(2)->Sink().State() == CMceSrvStream::EStarted );
    }

//
// Tests
//
// TInt CloseSession( CMceComSession& aSession )
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_CloseSessionL()
    {
    SETUP( CreateVideoOutSessionL );

    EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 0 );
    EUNIT_ASSERT( &iSession->SdpSession() == NULL );
    Reset();
    
    //close unregistered    
    TInt error = iManager->CloseSession( *iSession );
    if ( error == KErrNoMemory )
        {
        User::Leave( KErrNoMemory );
        }
    
    EUNIT_ASSERT( error != KErrNone );
    
    EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 0 );
    EUNIT_ASSERT( &iSession->SdpSession() == NULL );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    //close main
    iSdpDocument = MceMediaManagerTestHelper::EstablishOutSessionL( 
                                            *iSession,
                                            *iManager,
                                            KMceMMTestSdpAnswerVideoIMS5PrecsSendrecv,
                                            iServer );
    
    EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 1 );
    EUNIT_ASSERT( &iSession->SdpSession() == iManager->iMceSdpSessions[ 0 ] );
    EUNIT_ASSERT( iManager->iMceSdpSessions[ 0 ]->MediaSession() == iSession );
    EUNIT_ASSERT( iManager->iMceSdpSessions[ 0 ]->Backup() == NULL );
    
    Reset();
    MCE_EUNIT_ASSERT_EQUALS( iManager->CloseSession( *iSession ), KErrNone );

    EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 0 );
    EUNIT_ASSERT( &iSession->SdpSession() == NULL );

    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStopStream );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccDeleteStream );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccStopStream );
    EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::EMccRemoveDataSource );
    EUNIT_ASSERT( iMcc->iActions[ 4 ].iAction == CMccControllerStub::EMccRemoveDataSink );
    EUNIT_ASSERT( iMcc->iActions[ 5 ].iAction == CMccControllerStub::EMccDeleteStream );
    EUNIT_ASSERT( iMcc->iActions[ 6 ].iAction == CMccControllerStub::EMccCloseLink );
    EUNIT_ASSERT( iMcc->iActions[ 7 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 7 ].iKey == KMccCamLocalVideo );
    EUNIT_ASSERT( iMcc->iActions[ 8 ].iAction == CMccControllerStub::EMccCloseSession );
    EUNIT_ASSERT( iMcc->iActions[ 9 ].iAction == CMccControllerStub::ENone );
    
    CMceComSession::Delete( iSession, *iManager );
    
    for( TInt i = 0; i < 3; i++ )
        {
        delete iSdpDocument;
        iSdpDocument = NULL;
        
        iSession = MceMediaManagerTestHelper::CreateVideoOutSessionL();
        iSession->SetObserver( *iServer );
        
        //establish    
        iSdpDocument = MceMediaManagerTestHelper::EstablishOutSessionL( 
                                                *iSession,
                                                *iManager,
                                                KMceMMTestSdpAnswerVideoIMS5PrecsSendrecv,
                                                iServer );

        EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 1 );
        EUNIT_ASSERT( &iSession->SdpSession() == iManager->iMceSdpSessions[ 0 ] );
        EUNIT_ASSERT( iManager->iMceSdpSessions[ 0 ]->MediaSession() == iSession );
        EUNIT_ASSERT( iManager->iMceSdpSessions[ 0 ]->Backup() == NULL );

        EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
        EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarted );
        EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
        EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
        EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStarted );
        EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStarted );
        EUNIT_ASSERT( SRVSTREAM(2)->State() == CMceSrvStream::EStarted );
        EUNIT_ASSERT( SRVSTREAM(2)->Source().State() == CMceSrvStream::EStarted );
        EUNIT_ASSERT( SRVSTREAM(2)->Sink().State() == CMceSrvStream::EStarted );
                                                    
        //create update
        iUpdate = iSession->CloneL();
        iUpdate->SetObserver( *iServer );
        
        //update
        CSdpDocument* tempSdpDocument = iSdpDocument;
        iSdpDocument = NULL;
        iSdpDocument = MceMediaManagerTestHelper::EstablishOutSessionL( 
                                                *iUpdate,
                                                *iManager,
                                                KMceMMTestSdpAnswerVideoIMS5PrecsSendrecv,
                                                iServer,
                                                tempSdpDocument );
        
        EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 1 );
        EUNIT_ASSERT( &iSession->SdpSession() == iManager->iMceSdpSessions[ 0 ] );
        EUNIT_ASSERT( &iUpdate->SdpSession() == iManager->iMceSdpSessions[ 0 ] );
        EUNIT_ASSERT( iManager->iMceSdpSessions[ 0 ]->MediaSession() == iUpdate );
        EUNIT_ASSERT( iManager->iMceSdpSessions[ 0 ]->Backup() == iSession );

        EUNIT_ASSERT( SRVUPDATESTREAM(0)->State() == CMceSrvStream::EStarted );
        EUNIT_ASSERT( SRVUPDATESTREAM(0)->Source().State() == CMceSrvStream::EStarted );
        EUNIT_ASSERT( SRVUPDATESTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
        EUNIT_ASSERT( SRVUPDATESTREAM(1)->State() == CMceSrvStream::EStarted );
        EUNIT_ASSERT( SRVUPDATESTREAM(1)->Source().State() == CMceSrvStream::EStarted );
        EUNIT_ASSERT( SRVUPDATESTREAM(1)->Sink().State() == CMceSrvStream::EStarted );
        EUNIT_ASSERT( SRVUPDATESTREAM(2)->State() == CMceSrvStream::EStarted );
        EUNIT_ASSERT( SRVUPDATESTREAM(2)->Source().State() == CMceSrvStream::EStarted );
        EUNIT_ASSERT( SRVUPDATESTREAM(2)->Sink().State() == CMceSrvStream::EStarted );
        
        Reset();
        
        //reject update        
        if ( i == 0 )
            {
            //reject update
            MCE_EUNIT_ASSERT_EQUALS( iManager->CloseSession( *iUpdate ), KErrNone );
            EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 1 );
            EUNIT_ASSERT( &iSession->SdpSession() == iManager->iMceSdpSessions[ 0 ] );
            EUNIT_ASSERT( &iUpdate->SdpSession() == NULL );
            EUNIT_ASSERT( iManager->iMceSdpSessions[ 0 ]->MediaSession() == iSession );
            EUNIT_ASSERT( iManager->iMceSdpSessions[ 0 ]->Backup() == NULL );
            EUNIT_ASSERT( iSession->AdoptedMccStreams().Count() == 0 );
            
            EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStopStream );
            EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccDeleteStream );
            EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccStopStream );
            EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::EMccRemoveDataSource );
            EUNIT_ASSERT( iMcc->iActions[ 4 ].iAction == CMccControllerStub::EMccRemoveDataSink );
            EUNIT_ASSERT( iMcc->iActions[ 5 ].iAction == CMccControllerStub::EMccDeleteStream );
            EUNIT_ASSERT( iMcc->iActions[ 6 ].iAction == CMccControllerStub::EMccCloseLink );
            EUNIT_ASSERT( iMcc->iActions[ 7 ].iAction == CMccControllerStub::EMccStreamSettings );
            EUNIT_ASSERT( iMcc->iActions[ 7 ].iKey == KMccCamLocalVideo );
            EUNIT_ASSERT( iMcc->iActions[ 8 ].iAction == CMccControllerStub::EMccCloseSession );
            EUNIT_ASSERT( iMcc->iActions[ 9 ].iAction == CMccControllerStub::ENone );

            EUNIT_ASSERT( SRVUPDATESTREAM(0)->State() == CMceSrvStream::EStopped );
            EUNIT_ASSERT( SRVUPDATESTREAM(0)->Source().State() == CMceSrvStream::EStopped );
            EUNIT_ASSERT( SRVUPDATESTREAM(0)->Sink().State() == CMceSrvStream::EStopped );
            EUNIT_ASSERT( SRVUPDATESTREAM(1)->State() == CMceSrvStream::EStopped );
            EUNIT_ASSERT( SRVUPDATESTREAM(1)->Source().State() == CMceSrvStream::EStopped );
            EUNIT_ASSERT( SRVUPDATESTREAM(1)->Sink().State() == CMceSrvStream::EStopped );
            EUNIT_ASSERT( SRVUPDATESTREAM(2)->State() == CMceSrvStream::EStopped );
            EUNIT_ASSERT( SRVUPDATESTREAM(2)->Source().State() == CMceSrvStream::EStopped );
            EUNIT_ASSERT( SRVUPDATESTREAM(2)->Sink().State() == CMceSrvStream::EStopped );
            
            Reset();
            //close
            MCE_EUNIT_ASSERT_EQUALS( iManager->CloseSession( *iSession ), KErrNone );
            EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 0 );
            EUNIT_ASSERT( &iSession->SdpSession() == NULL );
            EUNIT_ASSERT( &iUpdate->SdpSession() == NULL );
            EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStopStream );
            EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccDeleteStream );
            EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccStopStream );
            EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::EMccRemoveDataSource );
            EUNIT_ASSERT( iMcc->iActions[ 4 ].iAction == CMccControllerStub::EMccRemoveDataSink );
            EUNIT_ASSERT( iMcc->iActions[ 5 ].iAction == CMccControllerStub::EMccDeleteStream );
            EUNIT_ASSERT( iMcc->iActions[ 6 ].iAction == CMccControllerStub::EMccCloseLink );
            EUNIT_ASSERT( iMcc->iActions[ 7 ].iAction == CMccControllerStub::EMccStreamSettings );
            EUNIT_ASSERT( iMcc->iActions[ 7 ].iKey == KMccCamLocalVideo );
            EUNIT_ASSERT( iMcc->iActions[ 8 ].iAction == CMccControllerStub::EMccCloseSession );
            EUNIT_ASSERT( iMcc->iActions[ 9 ].iAction == CMccControllerStub::ENone );
            }
        //accept update and close it before receiving any data from new
        else if ( i == 1 )
            {
            //accept update
            MCE_EUNIT_ASSERT_EQUALS( iManager->CloseSession( *iSession ), KErrNone );
            EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 1 );
            EUNIT_ASSERT( &iSession->SdpSession() == NULL );
            EUNIT_ASSERT( &iUpdate->SdpSession() == iManager->iMceSdpSessions[ 0 ] );
            EUNIT_ASSERT( iManager->iMceSdpSessions[ 0 ]->MediaSession() == iUpdate );
            EUNIT_ASSERT( iManager->iMceSdpSessions[ 0 ]->Backup() == NULL );
            EUNIT_ASSERT( iUpdate->AdoptedMccStreams().Count() == 1 );
            
            EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStopStream );//send
            EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccDeleteStream );
            EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::ENone );

            EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
            EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarted );
            EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
            EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStopped );
            EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStopped );
            EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStopped );
            EUNIT_ASSERT( SRVSTREAM(2)->State() == CMceSrvStream::EStarted );
            EUNIT_ASSERT( SRVSTREAM(2)->Source().State() == CMceSrvStream::EStarted );
            EUNIT_ASSERT( SRVSTREAM(2)->Sink().State() == CMceSrvStream::EStarted );
            
            Reset();
            //close
            MCE_EUNIT_ASSERT_EQUALS( iManager->CloseSession( *iUpdate ), KErrNone );
            EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 0 );
            EUNIT_ASSERT( &iSession->SdpSession() == NULL );
            EUNIT_ASSERT( &iUpdate->SdpSession() == NULL );
            EUNIT_ASSERT( iUpdate->AdoptedMccStreams().Count() == 1 );
            EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStopStream );//rec
            EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccDeleteStream );
            EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccStopStream );//send
            EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::EMccRemoveDataSource );
            EUNIT_ASSERT( iMcc->iActions[ 4 ].iAction == CMccControllerStub::EMccRemoveDataSink );
            
            EUNIT_ASSERT( iMcc->iActions[ 5 ].iAction == CMccControllerStub::EMccDeleteStream );
            EUNIT_ASSERT( iMcc->iActions[ 6 ].iAction == CMccControllerStub::EMccCloseLink );
            EUNIT_ASSERT( iMcc->iActions[ 7 ].iAction == CMccControllerStub::EMccStreamSettings );
            EUNIT_ASSERT( iMcc->iActions[ 7 ].iKey == KMccCamLocalVideo );
            
            EUNIT_ASSERT( iMcc->iActions[ 8 ].iAction == CMccControllerStub::EMccStopStream );//adopted
            EUNIT_ASSERT( iMcc->iActions[ 9 ].iAction == CMccControllerStub::EMccRemoveDataSource );
            EUNIT_ASSERT( iMcc->iActions[ 10 ].iAction == CMccControllerStub::EMccRemoveDataSink );
            EUNIT_ASSERT( iMcc->iActions[ 11 ].iAction == CMccControllerStub::EMccDeleteStream );
            EUNIT_ASSERT( iMcc->iActions[ 12 ].iAction == CMccControllerStub::EMccCloseLink );
            EUNIT_ASSERT( iMcc->iActions[ 13 ].iAction == CMccControllerStub::EMccCloseSession );//old ses
            EUNIT_ASSERT( iMcc->iActions[ 14 ].iAction == CMccControllerStub::EMccCloseSession );//new ses
            EUNIT_ASSERT( iMcc->iActions[ 15 ].iAction == CMccControllerStub::ENone );
            }
        //accept update and close it after receiving any data from new
        else if ( i == 2 )
            {
            //accept update
            MCE_EUNIT_ASSERT_EQUALS( iManager->CloseSession( *iSession ), KErrNone );
            EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 1 );
            EUNIT_ASSERT( &iSession->SdpSession() == NULL );
            EUNIT_ASSERT( &iUpdate->SdpSession() == iManager->iMceSdpSessions[ 0 ] );
            EUNIT_ASSERT( iManager->iMceSdpSessions[ 0 ]->MediaSession() == iUpdate );
            EUNIT_ASSERT( iManager->iMceSdpSessions[ 0 ]->Backup() == NULL );
            EUNIT_ASSERT( iUpdate->AdoptedMccStreams().Count() == 1 );
            
            EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStopStream );//send
            EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccDeleteStream );
            EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::ENone );

            EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
            EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarted );
            EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
            EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStopped );
            EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStopped );
            EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStopped );
            EUNIT_ASSERT( SRVSTREAM(2)->State() == CMceSrvStream::EStarted );
            EUNIT_ASSERT( SRVSTREAM(2)->Source().State() == CMceSrvStream::EStarted );
            EUNIT_ASSERT( SRVSTREAM(2)->Sink().State() == CMceSrvStream::EStarted );

            Reset();
            //1. receives data
            iManager->MccMediaActive( iUpdate->iMccID,
                                      SRVUPDATESTREAM(0)->LinkId(),
                                      SRVUPDATESTREAM(0)->Id(),
                                      KNoEndpointId );
            MCE_MCC_EVENT_POSTCONDITION_L 

            EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStopStream );//adopted
            EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccRemoveDataSource );
            EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccRemoveDataSink );
            EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::EMccDeleteStream );
            EUNIT_ASSERT( iMcc->iActions[ 4 ].iAction == CMccControllerStub::EMccCloseLink );
            EUNIT_ASSERT( iMcc->iActions[ 5 ].iAction == CMccControllerStub::EMccCloseSession );//old ses
            EUNIT_ASSERT( iMcc->iActions[ 6 ].iAction == CMccControllerStub::ENone );

            EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
            EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarted );
            EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
            EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStopped );
            EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStopped );
            EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStopped );
            EUNIT_ASSERT( SRVSTREAM(2)->State() == CMceSrvStream::EStarted );
            EUNIT_ASSERT( SRVSTREAM(2)->Source().State() == CMceSrvStream::EStarted );
            EUNIT_ASSERT( SRVSTREAM(2)->Sink().State() == CMceSrvStream::EStarted );
            
            Reset();
            //close
            MCE_EUNIT_ASSERT_EQUALS( iManager->CloseSession( *iUpdate ), KErrNone );
            EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 0 );
            EUNIT_ASSERT( &iSession->SdpSession() == NULL );
            EUNIT_ASSERT( &iUpdate->SdpSession() == NULL );
            EUNIT_ASSERT( iUpdate->AdoptedMccStreams().Count() == 0 );
            EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStopStream );//rec
            EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccDeleteStream );
            EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccStopStream );//send
            EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::EMccRemoveDataSource );
            EUNIT_ASSERT( iMcc->iActions[ 4 ].iAction == CMccControllerStub::EMccRemoveDataSink );
            EUNIT_ASSERT( iMcc->iActions[ 5 ].iAction == CMccControllerStub::EMccDeleteStream );
            EUNIT_ASSERT( iMcc->iActions[ 6 ].iAction == CMccControllerStub::EMccCloseLink );
            EUNIT_ASSERT( iMcc->iActions[ 7 ].iAction == CMccControllerStub::EMccStreamSettings );
            EUNIT_ASSERT( iMcc->iActions[ 7 ].iKey == KMccCamLocalVideo );
            EUNIT_ASSERT( iMcc->iActions[ 8 ].iAction == CMccControllerStub::EMccCloseSession );//new ses
            EUNIT_ASSERT( iMcc->iActions[ 9 ].iAction == CMccControllerStub::ENone );
            }
        
        EUNIT_ASSERT( SRVUPDATESTREAM(0)->State() == CMceSrvStream::EStopped );
        EUNIT_ASSERT( SRVUPDATESTREAM(0)->Source().State() == CMceSrvStream::EStopped );
        EUNIT_ASSERT( SRVUPDATESTREAM(0)->Sink().State() == CMceSrvStream::EStopped );
        EUNIT_ASSERT( SRVUPDATESTREAM(1)->State() == CMceSrvStream::EStopped );
        EUNIT_ASSERT( SRVUPDATESTREAM(1)->Source().State() == CMceSrvStream::EStopped );
        EUNIT_ASSERT( SRVUPDATESTREAM(1)->Sink().State() == CMceSrvStream::EStopped );
        EUNIT_ASSERT( SRVUPDATESTREAM(2)->State() == CMceSrvStream::EStopped );
        EUNIT_ASSERT( SRVUPDATESTREAM(2)->Source().State() == CMceSrvStream::EStopped );
        EUNIT_ASSERT( SRVUPDATESTREAM(2)->Sink().State() == CMceSrvStream::EStopped );
        
        Reset();
        CMceComSession::Delete( iUpdate, *iManager );
        EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
        
        Reset();
        CMceComSession::Delete( iSession, *iManager );
        EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
        }
    }


//
// Tests
//
// TInt CloseSession( CMceComSession& aSession )
// (merged)
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_CloseSession_1L()
    {
    TInt error = KErrNone;
    
    for( TInt i = 0; i < 2; i++ )
        {
        delete iSdpDocument;
        iSdpDocument = NULL;
        
        SETUP( CreateVideoOutSessionL );
        
        //establish    
        iSdpDocument = MceMediaManagerTestHelper::EstablishOutSessionL( 
                                                    *iSession,
                                                    *iManager,
                                                    KMceMMTestSdpAnswerVideoIMS5PrecsSendrecv,
                                                    iServer );

        EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 1 );
        EUNIT_ASSERT( &iSession->SdpSession() == iManager->iMceSdpSessions[ 0 ] );
        EUNIT_ASSERT( iManager->iMceSdpSessions[ 0 ]->MediaSession() == iSession );
        EUNIT_ASSERT( iManager->iMceSdpSessions[ 0 ]->Backup() == NULL );
                                                    
        //create update
        iUpdate = iSession->CloneL();
        iUpdate->SetObserver( *iServer );
        iMcc->iRequireSignalling = EFalse;
        TMceReturnStatus status;
        MCE_LEAVE_HANDLING( status = iManager->UpdateL( *iUpdate ), KErrArgument );
        EUNIT_ASSERT( status == KMceReady );
        EUNIT_ASSERT( iSession->IsMerged() );
        EUNIT_ASSERT( iUpdate->IsMerged() );
        
        Reset();
        
        //reject update        
        if ( i == 0 )
            {
            //reject update
            MCE_EUNIT_ASSERT_EQUALS( iManager->CloseSession( *iUpdate ), KErrNone );
            EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 1 );
            EUNIT_ASSERT( &iSession->SdpSession() == iManager->iMceSdpSessions[ 0 ] );
            EUNIT_ASSERT( &iUpdate->SdpSession() == NULL );
            EUNIT_ASSERT( iManager->iMceSdpSessions[ 0 ]->MediaSession() == iSession );
            EUNIT_ASSERT( iManager->iMceSdpSessions[ 0 ]->Backup() == NULL );
            EUNIT_ASSERT( iSession->AdoptedMccStreams().Count() == 0 );
            EUNIT_ASSERT( !iSession->IsMerged() );
            EUNIT_ASSERT( !iUpdate->IsMerged() );
            EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );//merged session => no actions
            
            Reset();
            //close
            MCE_EUNIT_ASSERT_EQUALS( iManager->CloseSession( *iSession ), KErrNone );
            EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 0 );
            EUNIT_ASSERT( &iSession->SdpSession() == NULL );
            EUNIT_ASSERT( &iUpdate->SdpSession() == NULL );
            }
        //accept update 
        else if ( i == 1 )
            {
            //accept update
            MCE_EUNIT_ASSERT_EQUALS( iManager->CloseSession( *iSession ), KErrNone );
            EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 1 );
            EUNIT_ASSERT( &iSession->SdpSession() == NULL );
            EUNIT_ASSERT( &iUpdate->SdpSession() == iManager->iMceSdpSessions[ 0 ] );
            EUNIT_ASSERT( iManager->iMceSdpSessions[ 0 ]->MediaSession() == iUpdate );
            EUNIT_ASSERT( iManager->iMceSdpSessions[ 0 ]->Backup() == NULL );
            EUNIT_ASSERT( iUpdate->AdoptedMccStreams().Count() == 0 );
            EUNIT_ASSERT( !iSession->IsMerged() );
            EUNIT_ASSERT( !iUpdate->IsMerged() );
            
            EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );//merged session => no actions
            
            Reset();
            //close
            MCE_EUNIT_ASSERT_EQUALS( iManager->CloseSession( *iUpdate ), KErrNone );
            EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 0 );
            EUNIT_ASSERT( &iSession->SdpSession() == NULL );
            EUNIT_ASSERT( &iUpdate->SdpSession() == NULL );
            EUNIT_ASSERT( iUpdate->AdoptedMccStreams().Count() == 0 );
            EUNIT_ASSERT( !iSession->IsMerged() );
            EUNIT_ASSERT( !iUpdate->IsMerged() );
            }
        
        EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStopStream );//rec
        EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccDeleteStream );
        EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccStopStream );//send
        EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::EMccRemoveDataSource );
        EUNIT_ASSERT( iMcc->iActions[ 4 ].iAction == CMccControllerStub::EMccRemoveDataSink );
        EUNIT_ASSERT( iMcc->iActions[ 5 ].iAction == CMccControllerStub::EMccDeleteStream );
        EUNIT_ASSERT( iMcc->iActions[ 6 ].iAction == CMccControllerStub::EMccCloseLink );
        EUNIT_ASSERT( iMcc->iActions[ 7 ].iAction == CMccControllerStub::EMccStreamSettings );
        EUNIT_ASSERT( iMcc->iActions[ 7 ].iKey == KMccCamLocalVideo );
        EUNIT_ASSERT( iMcc->iActions[ 8 ].iAction == CMccControllerStub::EMccCloseSession );//new ses
        EUNIT_ASSERT( iMcc->iActions[ 9 ].iAction == CMccControllerStub::ENone );
        
        Reset();
        CMceComSession::Delete( iUpdate, *iManager );
        EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
        Reset();
        CMceComSession::Delete( iSession, *iManager );
        EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
        }
    }


//
// Tests
//
// void StartInactivityTimerL( CMceComRtpSource& aRtpSource )
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_StartInactivityTimerLL()
    {
    SETUP( CreateVideoOutSessionL );
    
    CMceComRtpSource& rtpsource = static_cast<CMceComRtpSource&>( *iSession->Streams()[KVideo]->Source() );
    
    Reset();
    
    TUint32 value = 10;
    iManager->StartInactivityTimerL( rtpsource, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
//establish    
    iSdpDocument = MceMediaManagerTestHelper::EstablishOutSessionL( 
                                                *iSession,
                                                *iManager,
                                                KMceMMTestSdpAnswerVideoIMS5PrecsSendrecv,
                                                iServer );
    
    Reset();
    iManager->StartInactivityTimerL( rtpsource, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccInactivityTimerStart );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    }

//
// Tests
//
// void StopInactivityTimerL( CMceComRtpSource& aRtpSource )
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_StopInactivityTimerLL()
    {
    SETUP( CreateVideoOutSessionL );
    
    CMceComRtpSource& rtpsource = static_cast<CMceComRtpSource&>( *iSession->Streams()[KVideo]->Source() );
    
    Reset();
    iManager->StopInactivityTimerL( rtpsource );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
//establish    
    iSdpDocument = MceMediaManagerTestHelper::EstablishOutSessionL( 
                                                *iSession,
                                                *iManager,
                                                KMceMMTestSdpAnswerVideoIMS5PrecsSendrecv,
                                                iServer );
    
    Reset();
    iManager->StopInactivityTimerL( rtpsource );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccInactivityTimerStop );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    }

//
// Tests
//
// void SendRTCPAnyDataL( CMceComMediaStream& aMediaStream, const TDesC8& aData )
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_SendRTCPAnyDataLL()
    {
    _LIT8( anyData, "Any Data");
    
    SETUP( CreateVideoOutSessionL );
    
    CMceComMediaStream& videoOut = iSession->Streams()[KVideo]->BoundStreamL();
    
    Reset();
    iManager->SendRTCPAnyDataL( videoOut, anyData );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
//establish    
    iSdpDocument = MceMediaManagerTestHelper::EstablishOutSessionL( 
                                                *iSession,
                                                *iManager,
                                                KMceMMTestSdpAnswerVideoIMS5PrecsSendrecv,
                                                iServer );
    
    Reset();
    iManager->SendRTCPAnyDataL( videoOut, anyData );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccSendMediaSignal );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iEventType == KMccRtcpControl );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    }

//
// Tests
//
// void SendRTCPReceiverReportL( CMceComMediaStream& aMediaStream )
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_SendRTCPReceiverReportLL()
    {
    SETUP( CreateVideoOutSessionL );
    
    CMceComMediaStream& videoOut = iSession->Streams()[KVideo]->BoundStreamL();
    CMceComRtpSource& rtpsource = static_cast<CMceComRtpSource&>( *iSession->Streams()[KVideo]->Source() );
    
    Reset();
    iManager->SendRTCPReceiverReportL( rtpsource );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
//establish    
    iSdpDocument = MceMediaManagerTestHelper::EstablishOutSessionL( 
                                                *iSession,
                                                *iManager,
                                                KMceMMTestSdpAnswerVideoIMS5PrecsSendrecv,
                                                iServer );
    
    Reset();
    iManager->SendRTCPReceiverReportL( rtpsource );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccSendMediaSignal );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iEventType == KMccRtcpControl );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    }

//
// Tests
//
// void SendRTCPSenderReportL( CMceComMediaStream& aMediaStream )
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_SendRTCPSenderReportLL()
    {
    SETUP( CreateVideoOutSessionL );
    
    CMceComMediaStream& videoOut = iSession->Streams()[KVideo]->BoundStreamL();
    CMceComRtpSink& rtpsink = static_cast<CMceComRtpSink&>( *iSession->Streams()[KVideo]->BoundStreamL().Sinks()[KRtpSink] );
    
    Reset();
    iManager->SendRTCPSenderReportL( rtpsink );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
//establish    
    iSdpDocument = MceMediaManagerTestHelper::EstablishOutSessionL( 
                                                *iSession,
                                                *iManager,
                                                KMceMMTestSdpAnswerVideoIMS5PrecsSendrecv,
                                                iServer );
    
    Reset();
    iManager->SendRTCPSenderReportL( rtpsink );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccSendMediaSignal );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iEventType == KMccRtcpControl );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    }


//
// Tests media attributes setting & getting
//
//

//
// Tests
//
// void GetVolumeL( TInt& aVolume ) const
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_GetVolumeLL()
    {
    TInt volume = 10;
    
    FULL_SETUP_AND_PREPARE( CreateAudioOutSessionL );
    
    CMceComMediaStream& audioIn = *iSession->Streams()[KAudio];
    CMceComSpeakerSink& speaker = static_cast<CMceComSpeakerSink&>( *audioIn.Sinks()[KSpeakerSink] );
    
    Reset();
    iManager->GetVolumeL( speaker, volume );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccSpeakerVolume );
    
    PREPARESRVSTREAMS(2);
    Reset();
    iManager->GetVolumeL( speaker, volume );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccSpeakerVolume );   
    }

//
// Tests
//
// void GetMaxVolumeL( CMceComMediaStream& aMediaStream, TInt& aMaxVolume )
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_GetMaxVolumeLL()
    {
    TInt maxVolume = 10;
    
    FULL_SETUP_AND_PREPARE( CreateAudioOutSessionL );
    
    CMceComMediaStream& audioIn = *iSession->Streams()[KAudio];
    CMceComSpeakerSink& speaker = static_cast<CMceComSpeakerSink&>( *audioIn.Sinks()[KSpeakerSink] );
    
    Reset();
    iManager->GetMaxVolumeL( maxVolume );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccSpeakerMaxVolume );

    PREPARESRVSTREAMS(2);
    Reset();
    iManager->GetMaxVolumeL( maxVolume );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccSpeakerMaxVolume );
    
    }

//
// Tests
//
// void SetVolumeL( TInt aVolume )
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_SetVolumeLL()
    {
    TInt volume = 10;
    
    FULL_SETUP_AND_PREPARE( CreateAudioOutSessionL );
    
    CMceComMediaStream& audioIn = *iSession->Streams()[KAudio];
    CMceComSpeakerSink& speaker = static_cast<CMceComSpeakerSink&>( *audioIn.Sinks()[KSpeakerSink] );
    
    Reset();
    iManager->SetVolumeL( speaker, volume );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccSpeakerVolume );
    
    PREPARESRVSTREAMS(2);
    Reset();
    iManager->SetVolumeL( speaker, volume );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccSpeakerVolume );
    }

//
// Tests
//
// void SetBalanceL( CMceComMediaStream& aMediaStream, 
//                  TInt aBalance )
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_SetBalanceLL()
    {
    TInt balance = 10;
    
    FULL_SETUP_AND_PREPARE( CreateAudioOutSessionL );
    
    CMceComMediaStream& audioIn = *iSession->Streams()[KAudio];
    CMceComSpeakerSink& speaker = static_cast<CMceComSpeakerSink&>( *audioIn.Sinks()[KSpeakerSink] );
    CMceComMicSource& mic = static_cast<CMceComMicSource&>( *iSession->Streams()[KAudio]->BoundStreamL().Source() );
    
    Reset();
    iManager->SetBalanceL( speaker, balance );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccSpeakerBalance );
    
    Reset();
    iManager->SetBalanceL( mic, balance );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccMicBalance );

    PREPARESRVSTREAMS(2);
    Reset();
    iManager->SetBalanceL( mic, balance );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccMicBalance );
    
    }

//
// Tests
//
// void GetGainL( TInt& aGain ) const
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_GetGainLL()
    {
    TInt gain = 10;
    
    FULL_SETUP_AND_PREPARE( CreateAudioOutSessionL );
    
    CMceComMediaStream& audioIn = *iSession->Streams()[KAudio];
    CMceComMicSource& mic = static_cast<CMceComMicSource&>( *iSession->Streams()[KAudio]->BoundStreamL().Source() );
    
    Reset();
    iManager->GetGainL( mic, gain );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccMicGain );
    
    PREPARESRVSTREAMS(2);
    Reset();
    iManager->GetGainL( mic, gain );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccMicGain );
    }

//
// Tests
//
// void GetMaxGainL( CMceComMediaStream& aMediaStream, TInt& aMaxGain )
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_GetMaxGainLL()
    {
    TInt maxGain = 10;
    
    FULL_SETUP_AND_PREPARE( CreateAudioOutSessionL );
    
    CMceComMediaStream& audioIn = *iSession->Streams()[KAudio];
    CMceComMicSource& mic = static_cast<CMceComMicSource&>( *iSession->Streams()[KAudio]->BoundStreamL().Source() );
    
    Reset();
    iManager->GetMaxGainL( maxGain );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccMicMaxGain );
    
    PREPARESRVSTREAMS(2);
    Reset();
    iManager->GetMaxGainL( maxGain );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccMicMaxGain );
    }

//
// Tests
//
// void SetGainL( TInt aGain )
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_SetGainLL()
    {
    TInt gain = 10;
    
    FULL_SETUP_AND_PREPARE( CreateAudioOutSessionL );
    
    CMceComMediaStream& audioIn = *iSession->Streams()[KAudio];
    CMceComMicSource& mic = static_cast<CMceComMicSource&>( *iSession->Streams()[KAudio]->BoundStreamL().Source() );
    
    Reset();
    iManager->SetGainL( mic, gain );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccMicGain );
    
    PREPARESRVSTREAMS(2);
    Reset();
    iManager->SetGainL( mic, gain );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccMicGain );
    }
 
void UT_CMceMediaManager::UT_CMceMediaManager_GetAudioRoutingLL()
    {
    TInt value = 10;
    const TInt KMedia = 0;
    
    FULL_SETUP_AND_PREPARE( CreateMultimediaOutSessionL );
    
    CMceComMediaStream& stream = *iSession->Streams()[KMedia];
    CMceComSpeakerSink& speaker = static_cast<CMceComSpeakerSink&>( *stream.Sinks()[0] );
    
    Reset();
    iManager->GetAudioRoutingL( speaker, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccSpeakerRouting );
    
    PREPARESRVSTREAMS(2);
    Reset();
    iManager->GetAudioRoutingL( speaker, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccSpeakerRouting );
    }

void UT_CMceMediaManager::UT_CMceMediaManager_SetAudioRoutingLL()
    {
    TInt value = 10;
    const TInt KMedia = 0;
    
    FULL_SETUP_AND_PREPARE( CreateMultimediaOutSessionL );
    
    CMceComMediaStream& stream = *iSession->Streams()[KMedia];
    CMceComSpeakerSink& speaker = static_cast<CMceComSpeakerSink&>( *stream.Sinks()[0] );
    
    Reset();
    iManager->SetAudioRoutingL( speaker, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccSpeakerRouting );

    PREPARESRVSTREAMS(2);
    Reset();
    iManager->SetAudioRoutingL( speaker, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccSpeakerRouting );
    }

 

void UT_CMceMediaManager::UT_CMceMediaManager_GetDisplayRotationLL()
    {
    TInt value = 10;
    const TInt KMedia = 1;
    
    FULL_SETUP_AND_PREPARE( CreateMultimediaOutSessionL );
    
    
    CMceComMediaStream& stream = *iSession->Streams()[KMedia];
    CMceComDisplaySink& endpoint = 
        static_cast<CMceComDisplaySink&>( *iSession->Streams()[KMedia]->Sinks()[0] );
    
    Reset();
    iManager->GetDisplayRotationL( endpoint, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccDisplayLocalVideo );

    PREPARESRVSTREAMS(4);
    Reset();
    iManager->GetDisplayRotationL( endpoint, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccDisplayLocalVideo );
    
    Reset();
    endpoint.SetViewFinder( ETrue );
    EUNIT_ASSERT_LEAVE( iManager->GetDisplayRotationL( endpoint, value ) );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMceNotAssigned );
    }


void UT_CMceMediaManager::UT_CMceMediaManager_SetDisplayRotationLL()
    {
    TInt value = 10;
    const TInt KMedia = 1;
    
    FULL_SETUP_AND_PREPARE( CreateMultimediaOutSessionL );
    
    CMceComMediaStream& stream = *iSession->Streams()[KMedia];
    CMceComDisplaySink& endpoint = 
        static_cast<CMceComDisplaySink&>( *iSession->Streams()[KMedia]->Sinks()[0] );
    
    Reset();
    iManager->SetDisplayRotationL( endpoint, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccDisplayLocalVideo );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iKey == KMccDisplayLocalVideo );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::ENone );

    PREPARESRVSTREAMS(4);
    Reset();
    iManager->SetDisplayRotationL( endpoint, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccDisplayLocalVideo );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iKey == KMccDisplayLocalVideo );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::ENone );
    
    Reset();
    endpoint.SetViewFinder( ETrue );
    EUNIT_ASSERT_LEAVE( iManager->SetDisplayRotationL( endpoint, value ) );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMceNotAssigned );
    
    }

void UT_CMceMediaManager::UT_CMceMediaManager_GetDisplayPriorityLL()
    {
    TInt value = 10;
    const TInt KMedia = 1;
    
    FULL_SETUP_AND_PREPARE( CreateMultimediaOutSessionL );
    
    CMceComMediaStream& stream = *iSession->Streams()[KMedia];
    CMceComDisplaySink& endpoint = 
        static_cast<CMceComDisplaySink&>( *iSession->Streams()[KMedia]->Sinks()[0] );
    
    Reset();
    iManager->GetDisplayPriorityL( endpoint, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccDisplayLocalVideo );

    PREPARESRVSTREAMS(4);
    Reset();
    iManager->GetDisplayPriorityL( endpoint, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccDisplayLocalVideo );
    
    Reset();
    endpoint.SetViewFinder( ETrue );
    iManager->GetDisplayPriorityL( endpoint, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamLocalVideo );
    }

void UT_CMceMediaManager::UT_CMceMediaManager_GetDisplayRectLL()
    {
    TRect value;
    const TInt KMedia = 1;
    
    FULL_SETUP_AND_PREPARE( CreateMultimediaOutSessionL );
    
    CMceComMediaStream& stream = *iSession->Streams()[KMedia];
    CMceComDisplaySink& endpoint = 
        static_cast<CMceComDisplaySink&>( *iSession->Streams()[KMedia]->Sinks()[0] );
    
    Reset();
    iManager->GetDisplayRectL( endpoint, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccDisplayLocalVideo );

    PREPARESRVSTREAMS(4);
    Reset();
    iManager->GetDisplayRectL( endpoint, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccDisplayLocalVideo );
    
    Reset();
    endpoint.SetViewFinder( ETrue );
    iManager->GetDisplayRectL( endpoint, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamLocalVideo );
    }

void UT_CMceMediaManager::UT_CMceMediaManager_SetDisplayIndexLL()
    {
    TInt value = 10;
    const TInt KMedia = 1;
    
    FULL_SETUP_AND_PREPARE( CreateMultimediaOutSessionL );
    
    CMceComMediaStream& stream = *iSession->Streams()[KMedia];
    CMceComDisplaySink& endpoint = 
        static_cast<CMceComDisplaySink&>( *iSession->Streams()[KMedia]->Sinks()[0] );
    
    Reset();
    iManager->SetDisplayIndexL( endpoint, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccDisplayLocalVideo );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iKey == KMccDisplayLocalVideo );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::ENone );

    PREPARESRVSTREAMS(4);
    Reset();
    iManager->SetDisplayIndexL( endpoint, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccDisplayLocalVideo );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iKey == KMccDisplayLocalVideo );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::ENone );
    
    Reset();
    endpoint.SetViewFinder( ETrue );
    iManager->SetDisplayIndexL( endpoint, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamLocalVideo );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iKey == KMccCamLocalVideo );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::ENone );
    }

void UT_CMceMediaManager::UT_CMceMediaManager_SetDisplayPriorityLL()
    {
    TInt value = 10;
    const TInt KMedia = 1;
    
    FULL_SETUP_AND_PREPARE( CreateMultimediaOutSessionL );
    
    CMceComMediaStream& stream = *iSession->Streams()[KMedia];
    CMceComDisplaySink& endpoint = 
        static_cast<CMceComDisplaySink&>( *iSession->Streams()[KMedia]->Sinks()[0] );
    
    Reset();
    iManager->SetDisplayPriorityL( endpoint, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccDisplayLocalVideo );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iKey == KMccDisplayLocalVideo );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::ENone );
    
    PREPARESRVSTREAMS(4);
    Reset();
    iManager->SetDisplayPriorityL( endpoint, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccDisplayLocalVideo );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iKey == KMccDisplayLocalVideo );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::ENone );
    
    Reset();
    endpoint.SetViewFinder( ETrue );
    iManager->SetDisplayPriorityL( endpoint, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamLocalVideo );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iKey == KMccCamLocalVideo );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::ENone );
    }

void UT_CMceMediaManager::UT_CMceMediaManager_SetDisplayRectLL()
    {
    TRect value;
    const TInt KMedia = 1;
    
    FULL_SETUP_AND_PREPARE( CreateMultimediaOutSessionL );
    
    CMceComMediaStream& stream = *iSession->Streams()[KMedia];
    CMceComDisplaySink& endpoint = 
        static_cast<CMceComDisplaySink&>( *iSession->Streams()[KMedia]->Sinks()[0] );
    
    Reset();
    iManager->SetDisplayRectL( endpoint, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccDisplayLocalVideo );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iKey == KMccDisplayLocalVideo );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::ENone );
    
    PREPARESRVSTREAMS(4);
    Reset();
    iManager->SetDisplayRectL( endpoint, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccDisplayLocalVideo );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iKey == KMccDisplayLocalVideo );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::ENone );
    
    Reset();
    endpoint.SetViewFinder( ETrue );
    iManager->SetDisplayRectL( endpoint, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamLocalVideo );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iKey == KMccCamLocalVideo );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::ENone );
    }

void UT_CMceMediaManager::UT_CMceMediaManager_GetDisplaysAvailableLL()
    {
    TInt value;
    Reset();
    iManager->GetDisplaysAvailableL( value );
    
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    }



void UT_CMceMediaManager::UT_CMceMediaManager_GetBrightnessLL()
    {
    TInt value = 10;
    const TInt KMedia = 2;
    
    FULL_SETUP_AND_PREPARE( CreateMultimediaOutSessionL );
    
    CMceComMediaStream& stream = *iSession->Streams()[KMedia];
    CMceComCameraSource& camera = static_cast<CMceComCameraSource&>( *stream.Source() );
    
    Reset();
    iManager->GetBrightnessL( camera, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamBrightness );
    
    PREPARESRVSTREAMS(4);
    Reset();
    iManager->GetBrightnessL( camera, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamBrightness );
    }
    

void UT_CMceMediaManager::UT_CMceMediaManager_GetCameraInfoLL()
    {
    TMceCameraInfo value;
    value.iIndex = 0;
    
    
    FULL_SETUP_AND_PREPARE( CreateMultimediaOutSessionL );
        
    Reset();
    iManager->GetCameraInfoL( value.iIndex, value.iInfo );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamInfo );
    
    PREPARESRVSTREAMS(4);
    Reset();
    iManager->GetCameraInfoL( value.iIndex, value.iInfo );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamInfo );
    }

void UT_CMceMediaManager::UT_CMceMediaManager_GetCamerasAvailableLL()
    {
    TInt value;
    
    FULL_SETUP_AND_PREPARE( CreateMultimediaOutSessionL );
        
    Reset();
    iManager->GetCamerasAvailableL( value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamCount );

    PREPARESRVSTREAMS(4);
    Reset();
    iManager->GetCamerasAvailableL( value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamCount );
    }

void UT_CMceMediaManager::UT_CMceMediaManager_GetContrastLL()
    {
    TInt value = 10;
    const TInt KMedia = 2;
    
    FULL_SETUP_AND_PREPARE( CreateMultimediaOutSessionL );
    
    CMceComMediaStream& stream = *iSession->Streams()[KMedia];
    CMceComCameraSource& camera = static_cast<CMceComCameraSource&>( *stream.Source() );
    
    Reset();
    iManager->GetContrastL( camera, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamContrast );

    PREPARESRVSTREAMS(4);
    Reset();
    iManager->GetContrastL( camera, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamContrast );
    }

void UT_CMceMediaManager::UT_CMceMediaManager_GetDigitalZoomFactorLL()
    {
    TInt value = 10;
    const TInt KMedia = 2;
    
    FULL_SETUP_AND_PREPARE( CreateMultimediaOutSessionL );
    
    CMceComMediaStream& stream = *iSession->Streams()[KMedia];
    CMceComCameraSource& camera = static_cast<CMceComCameraSource&>( *stream.Source() );
    
    Reset();
    iManager->GetDigitalZoomFactorL( camera, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamDigitZoomFactor );
    
    PREPARESRVSTREAMS(4);
    Reset();
    iManager->GetDigitalZoomFactorL( camera, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamDigitZoomFactor );
    }


void UT_CMceMediaManager::UT_CMceMediaManager_GetExposureLL()
    {
    TInt value = 10;
    const TInt KMedia = 2;
    
    FULL_SETUP_AND_PREPARE( CreateMultimediaOutSessionL );
    
    CMceComMediaStream& stream = *iSession->Streams()[KMedia];
    CMceComCameraSource& camera = static_cast<CMceComCameraSource&>( *stream.Source() );
    
    Reset();
    iManager->GetExposureL( camera, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamExposure );
    
    PREPARESRVSTREAMS(4);
    Reset();
    iManager->GetExposureL( camera, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamExposure );
    }


void UT_CMceMediaManager::UT_CMceMediaManager_GetWhiteBalanceLL()
    {
    TInt value = 10;
    const TInt KMedia = 2;
    
    FULL_SETUP_AND_PREPARE( CreateMultimediaOutSessionL );
    
    CMceComMediaStream& stream = *iSession->Streams()[KMedia];
    CMceComCameraSource& camera = static_cast<CMceComCameraSource&>( *stream.Source() );
    
    Reset();
    iManager->GetWhiteBalanceL( camera, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamWhiteBalance );
    
    PREPARESRVSTREAMS(4);
    Reset();
    iManager->GetWhiteBalanceL( camera, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamWhiteBalance );
    }

void UT_CMceMediaManager::UT_CMceMediaManager_GetZoomFactorLL()
    {
    TInt value = 10;
    const TInt KMedia = 2;
    
    FULL_SETUP_AND_PREPARE( CreateMultimediaOutSessionL );
    
    CMceComMediaStream& stream = *iSession->Streams()[KMedia];
    CMceComCameraSource& camera = static_cast<CMceComCameraSource&>( *stream.Source() );
    
    Reset();
    iManager->GetZoomFactorL( camera, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamZoomFactor );
    
    PREPARESRVSTREAMS(4);
    Reset();
    iManager->GetZoomFactorL( camera, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamZoomFactor );
    }

    
void UT_CMceMediaManager::UT_CMceMediaManager_SetWhiteBalanceLL()
    {
    TInt value = 10;
    const TInt KMedia = 2;
    
    FULL_SETUP_AND_PREPARE( CreateMultimediaOutSessionL );
    
    CMceComMediaStream& stream = *iSession->Streams()[KMedia];
    CMceComCameraSource& camera = static_cast<CMceComCameraSource&>( *stream.Source() );
    
    Reset();
    iManager->SetWhiteBalanceL( camera, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamWhiteBalance );
    
    PREPARESRVSTREAMS(4);
    Reset();
    iManager->SetWhiteBalanceL( camera, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamWhiteBalance );
    }

void UT_CMceMediaManager::UT_CMceMediaManager_SetZoomFactorLL()
    {
    TInt value = 10;
    const TInt KMedia = 2;
    
    FULL_SETUP_AND_PREPARE( CreateMultimediaOutSessionL );
    
    CMceComMediaStream& stream = *iSession->Streams()[KMedia];
    CMceComCameraSource& camera = static_cast<CMceComCameraSource&>( *stream.Source() );
    
    Reset();
    iManager->SetZoomFactorL( camera, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamZoomFactor );
    
    PREPARESRVSTREAMS(4);
    Reset();
    iManager->SetZoomFactorL( camera, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamZoomFactor );
    }

void UT_CMceMediaManager::UT_CMceMediaManager_SetBrightnessLL()
    {
    TInt value = 10;
    const TInt KMedia = 2;
    
    FULL_SETUP_AND_PREPARE( CreateMultimediaOutSessionL );
    
    CMceComMediaStream& stream = *iSession->Streams()[KMedia];
    CMceComCameraSource& camera = static_cast<CMceComCameraSource&>( *stream.Source() );
    
    Reset();
    iManager->SetBrightnessL( camera, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamBrightness );

    PREPARESRVSTREAMS(4);
    Reset();
    iManager->SetBrightnessL( camera, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamBrightness );
    }

void UT_CMceMediaManager::UT_CMceMediaManager_SetCameraIndexLL()
    {
    TMceCameraInfo value;
    value.iIndex = 1;
    
    const TInt KMedia = 2;
    FULL_SETUP_AND_PREPARE( CreateMultimediaOutSessionL );
    
    CMceComMediaStream& stream = *iSession->Streams()[KMedia];
    CMceComCameraSource& camera = static_cast<CMceComCameraSource&>( *stream.Source() );
    
    Reset();
    iManager->SetCameraIndexL( camera, value.iIndex, value.iInfo );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamInfo );

    PREPARESRVSTREAMS(4);
    Reset();
    iManager->SetCameraIndexL( camera, value.iIndex, value.iInfo );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccUpdateDataSource );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iKey == KMccCamInfo );
    
    }

void UT_CMceMediaManager::UT_CMceMediaManager_SetContrastLL()
    {
    TInt value = 10;
    const TInt KMedia = 2;
    
    FULL_SETUP_AND_PREPARE( CreateMultimediaOutSessionL );
    
    CMceComMediaStream& stream = *iSession->Streams()[KMedia];
    CMceComCameraSource& camera = static_cast<CMceComCameraSource&>( *stream.Source() );
    
    Reset();
    iManager->SetContrastL( camera, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamContrast );
    
    PREPARESRVSTREAMS(4);
    Reset();
    iManager->SetContrastL( camera, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamContrast );
    
    }

void UT_CMceMediaManager::UT_CMceMediaManager_SetDigitalZoomFactorLL()
    {
    TInt value = 10;
    const TInt KMedia = 2;
    
    FULL_SETUP_AND_PREPARE( CreateMultimediaOutSessionL );
    
    CMceComMediaStream& stream = *iSession->Streams()[KMedia];
    CMceComCameraSource& camera = static_cast<CMceComCameraSource&>( *stream.Source() );
    
    Reset();
    iManager->SetDigitalZoomFactorL( camera, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamDigitZoomFactor );
    
    PREPARESRVSTREAMS(4);
    Reset();
    iManager->SetDigitalZoomFactorL( camera, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamDigitZoomFactor );
    
    }


void UT_CMceMediaManager::UT_CMceMediaManager_SetExposureLL()
    {
    TInt value = 10;
    const TInt KMedia = 2;
    
    FULL_SETUP_AND_PREPARE( CreateMultimediaOutSessionL );
    
    CMceComMediaStream& stream = *iSession->Streams()[KMedia];
    CMceComCameraSource& camera = static_cast<CMceComCameraSource&>( *stream.Source() );
    
    Reset();
    iManager->SetExposureL( camera, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamExposure );
    
    PREPARESRVSTREAMS(4);
    Reset();
    iManager->SetExposureL( camera, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamExposure );
    }


void UT_CMceMediaManager::UT_CMceMediaManager_SetRemoteAddressLL()
    {
    FULL_SETUP_AND_PREPARE( CreateMultimediaOutSessionL );
    
    TInt count = iSession->iMccStreams.Count();
    for ( TInt k = 0; k < count; k++ )
        {
        iManager->SetRemoteAddressL( *SRVSTREAM( k ) );
        }
    
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    Reset();
    
    // Prepare all the streams.
    count = iSession->iMccStreams.Count();
    for( TInt i = 0; i < count; i++ )
        {
        iSession->MccStreams()[i]->Id() = i + 10;
        }
    
    count = iSession->iMccStreams.Count();
    for ( TInt k = 0; k < count; k++ )
        {
        iManager->SetRemoteAddressL( *SRVSTREAM( k ) );
        }
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    Reset();
    
    iSession->iRemoteIpAddress.SetPort( 0 );
    iSession->iRemoteIpAddress.SetAddress( KInetAddrLoop );
    
    count = iSession->iMccStreams.Count();
    for ( TInt k = 0; k < count; k++ )
        {
        iManager->SetRemoteAddressL( *SRVSTREAM( k ) );
        }
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    Reset();
    
    iSession->iRemoteIpAddress.SetPort( 5000 );
    count = iSession->iMccStreams.Count();
    for ( TInt k = 0; k < count; k++ )
        {
        SRVSTREAM( k )->Data().SetRemoteMediaPort( 5000 );
        iManager->SetRemoteAddressL( *SRVSTREAM( k ) );
        }
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccSetRemoteAddress );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccSetRemoteAddress );
    
    for ( TInt k = 0; k < count; k++ )
        {
        TBuf<54> address;
        iSession->iRemoteIpAddress.Output( address );
        
        SRVSTREAM( k )->Data().SetRemoteRtcpMediaPort( 5001 );
        iManager->SetRemoteAddressL( *SRVSTREAM( k ) );
        SRVSTREAM( k )->Data().SetRemoteRtcpMediaAddrL( address );
        iManager->SetRemoteAddressL( *SRVSTREAM( k ) );
        }
    
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccSetRemoteAddress );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccSetRemoteAddress );
    Reset();
    
    iSession->iRemoteIpAddress.SetPort( 5000 );
    iSession->iRemoteIpAddress.SetAddress( KInet6AddrNone );
    
    count = iSession->iMccStreams.Count();
    for ( TInt k = 0; k < count; k++ )
        {
        iManager->SetRemoteAddressL( *SRVSTREAM( k ) );
        }
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    Reset();
    
    const TIp6Addr KIp6TestAddr = {{{1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8}}};
    
    iSession->iRemoteIpAddress.SetPort( 5000 );
    iSession->iRemoteIpAddress.SetAddress( KIp6TestAddr );
    
    count = iSession->iMccStreams.Count();
    for ( TInt k = 0; k < count; k++ )
        {
        iManager->SetRemoteAddressL( *SRVSTREAM( k ) );
        }
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccSetRemoteAddress );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccSetRemoteAddress );
    Reset();
    }


void UT_CMceMediaManager::UT_CMceMediaManager_GetFileInfoLL()
    {
    TMceFileInfo value;
    const TInt KMedia = 4;
    
    FULL_SETUP_AND_PREPARE( CreateMultimediaBundledOutSessionL );
    
    CMceComMediaStream& stream = *iSession->Streams()[KMedia];
    CMceComFileSource& endpoint = 
        static_cast<CMceComFileSource&>( *iSession->Streams()[KMedia]->Source() );
    
    
    Reset();
    iManager->GetFileInfoL( endpoint, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccFileInfo );
    
    PREPARESRVSTREAMS(7);
    Reset();
    iManager->GetFileInfoL( endpoint, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccFileInfo );
    
    // Test fill file info when file contains unsupported codec
    TMccFileSourceSetting mccFileInfo;
    mccFileInfo.iAudioFourCC = TFourCC( 3 );
    mccFileInfo.iVideoFourCC = TFourCC( 2 );
    
    iManager->FillMceFileInfoL( mccFileInfo, value );
    EUNIT_ASSERT( value.iAudioElementCount == 1 );
    EUNIT_ASSERT( value.iVideoElementCount == 1 );
    }

void UT_CMceMediaManager::UT_CMceMediaManager_GetFileInfoL_2L()
    {
    TMceFileInfo value;
    
    Reset();
    iManager->GetFileInfoL( value );
    
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccFileInfo );
    }

void UT_CMceMediaManager::UT_CMceMediaManager_FastForwardLL()
    {
    TBool value = ETrue;
    const TInt KMedia = 4;
    
    FULL_SETUP_AND_PREPARE( CreateMultimediaBundledOutSessionL );
    
    CMceComMediaStream& stream = *iSession->Streams()[KMedia];
    CMceComFileSource& endpoint = 
        static_cast<CMceComFileSource&>( *iSession->Streams()[KMedia]->Source() );
    
    Reset();
    iManager->FastForwardL( endpoint, value );

    PREPARESRVSTREAMS(7);
    Reset();
    iManager->FastForwardL( endpoint, value );
    }

void UT_CMceMediaManager::UT_CMceMediaManager_FastRewindLL()
    {
    TBool value = ETrue;
    const TInt KMedia = 4;
    
    FULL_SETUP_AND_PREPARE( CreateMultimediaBundledOutSessionL );
    
    CMceComMediaStream& stream = *iSession->Streams()[KMedia];
    CMceComFileSource& endpoint = 
        static_cast<CMceComFileSource&>( *iSession->Streams()[KMedia]->Source() );
    
    Reset();
    iManager->FastRewindL( endpoint, value );
    
    PREPARESRVSTREAMS(7);
    Reset();
    iManager->FastRewindL( endpoint, value );
    }

void UT_CMceMediaManager::UT_CMceMediaManager_GetFilePositionLL()
    {
    TTimeIntervalMicroSeconds value;
    const TInt KMedia = 4;
    
    FULL_SETUP_AND_PREPARE( CreateMultimediaBundledOutSessionL );
    
    CMceComMediaStream& stream = *iSession->Streams()[KMedia];
    CMceComFileSource& endpoint = 
        static_cast<CMceComFileSource&>( *iSession->Streams()[KMedia]->Source() );
    
    
    Reset();
    iManager->GetFilePositionL( endpoint, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccFilePosition );
    
    PREPARESRVSTREAMS(7);
    Reset();
    iManager->GetFilePositionL( endpoint, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccFilePosition );
    }

void UT_CMceMediaManager::UT_CMceMediaManager_SetFileInfoLL()
    {
    TMceFileInfo value;
    const TInt KMedia = 4;
    
    FULL_SETUP_AND_PREPARE( CreateMultimediaBundledOutSessionL );
    
    CMceComMediaStream& stream = *iSession->Streams()[KMedia];
    CMceComFileSource& endpoint = 
        static_cast<CMceComFileSource&>( *iSession->Streams()[KMedia]->Source() );
    
    
    Reset();
    iManager->SetFileInfoL( endpoint, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccFileName );
    
    PREPARESRVSTREAMS(7);
    Reset();
    iManager->SetFileInfoL( endpoint, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccFileName );
    }

void UT_CMceMediaManager::UT_CMceMediaManager_SetFilePositionLL()
    {
    TTimeIntervalMicroSeconds value;
    const TInt KMedia = 4;
    
    FULL_SETUP_AND_PREPARE( CreateMultimediaBundledOutSessionL );
    
    CMceComMediaStream& stream = *iSession->Streams()[KMedia];
    CMceComFileSource& endpoint = 
        static_cast<CMceComFileSource&>( *iSession->Streams()[KMedia]->Source() );
    
    
    Reset();
    iManager->SetFilePositionL( endpoint, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccFilePosition );

    PREPARESRVSTREAMS(7);
    Reset();
    iManager->SetFilePositionL( endpoint, value );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccFilePosition );
    }

    
void UT_CMceMediaManager::UT_CMceMediaManager_FillMceFileInfoLL()
    {
    //TBD
    }
 
void UT_CMceMediaManager::UT_CMceMediaManager_SetCNameLL()
    {
    iSession = MceMediaManagerTestHelper::CreateAudioOutSessionL();
    
    CMceComCameraSource* camera = CMceComCameraSource::NewL();
    CMceComMediaStream* local = MceMediaManagerTestHelper::CreateLocalVideoStreamL( camera );
    CleanupStack::PushL( local );
    local->InitializeL( *iSession );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iManager->SetCNameL( *camera, _L8("foofighter") ), KErrNotSupported );
    CleanupStack::PopAndDestroy( local );
    
    EUNIT_ASSERT_NO_LEAVE( iManager->SetCNameL( *iSession->Streams()[ 0 ]->Source(), _L8("foofighter") ) );
    EUNIT_ASSERT_NO_LEAVE( 
        iManager->SetCNameL( *iSession->Streams()[ 0 ]->BoundStreamL().Sinks()[ 0 ], _L8("foofighter") ) );
    }
     
//
// Tests
//
// TInt GetSupportedCodecs( RArray<TFourCC>& aCodecs )
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_GetSupportedCodecsL()
    {
    RArray<TFourCC> codecs;

    iManager->GetSupportedCodecs( codecs );
    EUNIT_ASSERT( codecs.Count() == 0 ); 
    
    codecs.Close();
    }
                        
//
// Tests
//
// void GetCapabilitiesL( RPointerArray<CMceComAudioCodec>& aCapabilities )
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_GetCapabilitiesL_1L()
    {
    RPointerArray<CMceComAudioCodec> audioCodecs;
    MceCleanupResetAndDestroyPushL( audioCodecs );
    
    iManager->GetCapabilitiesL( audioCodecs );
    EUNIT_ASSERT( audioCodecs.Count() > 0 ); 
    CleanupStack::PopAndDestroy();
    }

//
// Tests
//
// void GetCapabilitiesL( RPointerArray<CMceComVideoCodec>& aCapabilities )
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_GetCapabilitiesL_2L()
    {
    RPointerArray<CMceComVideoCodec> videoCodecs;
    MceCleanupResetAndDestroyPushL( videoCodecs );
    
    iManager->GetCapabilitiesL( videoCodecs );
    EUNIT_ASSERT( videoCodecs.Count() > 0 ); 

    CleanupStack::PopAndDestroy();
    }


//
// Tests
//
// void void MccEventReceived( const TMccEvent& aEvent );
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_MccEventReceivedL()
    {
    TMccEvent event;
    TMccRtcpEventData rtcp;
    rtcp.iRtcpPacketData.Copy( _L8("PACKET") );
    
    SETUP( CreateAudioOutSessionL );

    Reset();
    iManager->MccEventReceived( event );
    EUNIT_ASSERT( iServer->iEvent == EMceMediaError );

    Reset();

//establish    
    iSdpDocument = MceMediaManagerTestHelper::EstablishOutSessionL( 
                                                *iSession,
                                                *iManager,
                                                KMceMMTestSdpAnswerAudioIMS5PrecsSendrecv,
                                                iServer );
  
    Reset();
    event.iSessionId = iSession->iMccID;
    event.iLinkId = SRVSTREAM(0)->LinkId();
    event.iStreamId = KMceNotAssigned;
    
    iManager->MccEventReceived( event );
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );

    Reset();
    event.iStreamId = SRVSTREAM(0)->Id();
    event.iEventCategory = KMccEventCategoryDtmf;
    iManager->MccEventReceived( event );
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    
    Reset();
    event.iEventCategory = KMccEventCategoryRtcp;
    event.iEventType = KMccRtcpReceived;

    Reset();
    rtcp.iRtcpPacketType = KRtcpSrPacket;
    TMccRtcpEventDataPackage rtcpEvent1( rtcp );
    event.iEventData.Copy( rtcpEvent1 );
    iManager->MccEventReceived( event );
    EUNIT_ASSERT( iServer->iEvent == EMceMediaEventReceived );//stub thing
    EUNIT_ASSERT( iServer->iItcEvent == EMceItcSRReceived );
    
    Reset();
    rtcp.iRtcpPacketType = KRtcpRrPacket;
    TMccRtcpEventDataPackage rtcpEvent2( rtcp );
    event.iEventData.Copy( rtcpEvent2 );
    iManager->MccEventReceived( event );
    EUNIT_ASSERT( iServer->iEvent == EMceMediaEventReceived );//stub thing
    EUNIT_ASSERT( iServer->iItcEvent == EMceItcRRReceived );

    Reset();
    rtcp.iRtcpPacketType = KRtcpPacketUndefined;
    TMccRtcpEventDataPackage rtcpEvent3( rtcp );
    event.iEventData.Copy( rtcpEvent3 );
    iManager->MccEventReceived( event );
    EUNIT_ASSERT( iServer->iEvent == EMceMediaEventReceived );//stub thing
    EUNIT_ASSERT( iServer->iItcEvent == EMceItcFCMsgReceived );
    
    Reset();
    rtcp.iRtcpPacketType = KRtcpSdesPacket;
    TMccRtcpEventDataPackage rtcpEvent4( rtcp );
    event.iEventData.Copy( rtcpEvent4 );
    iManager->MccEventReceived( event );
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );//stub thing
    
    Reset();
    event.iEventType = KMccRtcpControl;
    iManager->MccEventReceived( event );
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );//stub thing
    
    Reset(); 
    event.iEventCategory = KMccEventCategoryDtmf;
    event.iEventType = KMccDtmfReceived;
    TMccDtmfEventData dtmf;
    dtmf.iDtmfEventType = KMccDtmfSequenceStop;
    TMccDtmfEventDataPackage dtmfEvent( dtmf );
    event.iEventData.Copy( dtmfEvent );
    iManager->MccEventReceived( event );
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );//stub thing

    Reset(); 
    event.iEventCategory = KMccEventCategoryTranscode;
    event.iEventType = KMccTranscodeCompleted;
    iManager->MccEventReceived( event );
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );//stub thing
    
    Reset();
    event.iEventCategory = KMccEventCategoryRtp;
    event.iEventType = KMccMasterKeyStaled;
    event.iStreamId =  1;
    event.iEndpointId =  SRVSTREAM(0)->LinkId();
    event.iEventNumData = 0;
    iManager->MccEventReceived( event );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EPausing );
    
    event.iEventNumData = 1;
    iManager->MccEventReceived( event );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EPausing );
    
    
    event.iStreamId =  1;
    iManager->MccEventReceived( event );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EPausing );
    
    
    iSession->iMccStreams.ResetAndDestroy();
    iSession->iMccAdoptedStreams.ResetAndDestroy();
    delete iSession->iNegotiationState;
    iSession->iNegotiationState = NULL;
    delete iSession;
    iSession = NULL;
    }
    

void UT_CMceMediaManager::UT_CMceMediaManager_MccTranscodeEventReceivedL()
    {
    TMccEvent event;
    event.iEventCategory = KMccEventCategoryTranscode;
    event.iEventType = KMccTranscodeCompleted;
    
    CMceComFileSource* endpoint = CMceComFileSource::NewLC();
    endpoint->iFileInfo.iFileName.Copy( _L8("C:\\test.3gp" ) );
    endpoint->iTranscodeInfo.iFileName.Copy( _L8("C:\\test.mpeg" ) );
    
    iManager->MccTranscodeEventReceived( event );

    TUint32 sessid = iManager->TranscodeFileL( endpoint );
    CleanupStack::Pop( endpoint );
    EUNIT_ASSERT( iManager->iTranscodeSessions.Count() == 1 );
    
    event.iSessionId = sessid;
    event.iEventType = 99;
    iManager->MccTranscodeEventReceived( event );
    EUNIT_ASSERT( iManager->iTranscodeSessions.Count() == 1 );
    
    event.iEventType = KMccTranscodeCompleted;
    iManager->MccTranscodeEventReceived( event );
    
    EUNIT_ASSERT( iManager->iTranscodeSessions.Count() == 0 );
    }

//
// Tests
//
// void MccMediaPrepared( TUint32 aSessionId,
//                        TUint32 aLinkId,
//                        TUint32 aStreamId,
//                        TUint32 aSinkSourceId )
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_MccMediaPreparedL()
    {
    SETUP( CreateAudioOutSessionL );
    
    Reset();
    iManager->MccMediaPrepared( iSession->iMccID,0,0,0);
    
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iServer->iEvent == EMceMediaError );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    Reset();
//establish executes happy case
    iSdpDocument = MceMediaManagerTestHelper::EstablishOutSessionL( 
                                                *iSession,
                                                *iManager,
                                                KMceMMTestSdpAnswerAudioIMS5PrecsSendrecv,
                                                iServer );
    
    Reset();
//in wrong state                                                    
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                KNoEndpointId );
                                
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iServer->iEvent == EMceMediaError );
    EUNIT_ASSERT( iServer->iSession == NULL );
    }
                               
//
// Tests
//
// void MccMediaStarted( TUint32 aSessionId,
//                        TUint32 aLinkId,     
//                        TUint32 aStreamId,
//                        TUint32 aSinkSourceId )
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_MccMediaStartedL()
    {
    SETUP( CreateAudioOutSessionL );
    
    Reset();
    iManager->MccMediaStarted( iSession->iMccID,0,0,0);
    
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iServer->iEvent == EMceMediaError );
    EUNIT_ASSERT( iServer->iSession == NULL );
    Reset();
    
//establish executes happy case
    iSdpDocument = MceMediaManagerTestHelper::EstablishOutSessionL( 
                                                *iSession,
                                                *iManager,
                                                KMceMMTestSdpAnswerAudioIMS5PrecsSendrecv,
                                                iServer );
    
    Reset();
//in wrong state                                                    
    iManager->MccMediaStarted( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                KNoEndpointId );
                                
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    }

//
// Tests
//
// void MccMediaStopped( TUint32 aSessionId,
//                       TUint32 aLinkId,
//                       TUint32 aStreamId,
//                       TUint32 aSinkSourceId )
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_MccMediaStoppedL()
    {
    SETUP( CreateAudioOutSessionL );
    
    Reset();
    iManager->MccMediaStopped( iSession->iMccID,0,0,0);
    
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iServer->iEvent == EMceMediaError );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    Reset();
//establish
    iSdpDocument = MceMediaManagerTestHelper::EstablishOutSessionL( 
                                                *iSession,
                                                *iManager,
                                                KMceMMTestSdpAnswerAudioIMS5PrecsSendrecv,
                                                iServer );
    

    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStarted );
    
    Reset();
    iManager->MccMediaStopped( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                KNoEndpointId );
    MCE_MCC_EVENT_POSTCONDITION_L                             
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iServer->iEvent == EMceMediaEventReceived );

    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStopped );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStopped );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStopped );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStarted );

    Reset();
    iManager->MccMediaStopped( iSession->iMccID,
                                SRVSTREAM(1)->LinkId(),
                                SRVSTREAM(1)->Id(),
                                KNoEndpointId );
    MCE_MCC_EVENT_POSTCONDITION_L                             
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iServer->iEvent == EMceMediaEventReceived );

    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStopped );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStopped );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStopped );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStopped );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStopped );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStopped );
    }

//
// Tests
//
// void MccMediaPaused( TUint32 aSessionId,
//                      TUint32 aLinkId,
//                      TUint32 aStreamId,
//                      TUint32 aSinkSourceId )
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_MccMediaPausedL()
    {
    SETUP( CreateAudioOutSessionL );

    CMceComMediaSink& rtpsink = *iSession->Streams()[KAudio]->BoundStreamL().Sinks()[KRtpSink];
    
    
    Reset();
    iManager->MccMediaPaused( iSession->iMccID,0,0,0);
    
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iServer->iEvent == EMceMediaError );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    Reset();
//establish
    iSdpDocument = MceMediaManagerTestHelper::EstablishOutSessionL( 
                                                *iSession,
                                                *iManager,
                                                KMceMMTestSdpAnswerAudioIMS5PrecsSendrecv,
                                                iServer );
    

    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStarted );
    
    Reset();
//pause 
    iManager->PauseL( rtpsink );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccPauseStream );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EPausing );
    
    
    Reset();
    iManager->MccMediaPaused( iSession->iMccID,
                                SRVSTREAM(1)->LinkId(),
                                SRVSTREAM(1)->Id(),
                                KNoEndpointId );
    MCE_MCC_EVENT_POSTCONDITION_L 
                                
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iServer->iEvent == EMceMediaEventReceived );
    EUNIT_ASSERT( iServer->iItcEvent == EMceItcStateChanged );
    EUNIT_ASSERT( iServer->iItcData == EFalse );

    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EPaused );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EPaused );
    }

//
// Tests
//
// void MccMediaResumed( TUint32 aSessionId,
//                       TUint32 aLinkId,
//                       TUint32 aStreamId,
//                       TUint32 aSinkSourceId )
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_MccMediaResumedL()
    {
    SETUP( CreateAudioOutSessionL );

    CMceComMediaSink& rtpsink = *iSession->Streams()[KAudio]->BoundStreamL().Sinks()[KRtpSink];
    
    
    Reset();
    iManager->MccMediaResumed( iSession->iMccID,0,0,0);
    
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iServer->iEvent == EMceMediaError );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    Reset();
//establish
    iSdpDocument = MceMediaManagerTestHelper::EstablishOutSessionL( 
                                                *iSession,
                                                *iManager,
                                                KMceMMTestSdpAnswerAudioIMS5PrecsSendrecv,
                                                iServer );
    

    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStarted );
    
    Reset();
//pause 
    iManager->PauseL( rtpsink );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccPauseStream );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EPausing );
    
    
    Reset();
    iManager->MccMediaPaused( iSession->iMccID,
                                SRVSTREAM(1)->LinkId(),
                                SRVSTREAM(1)->Id(),
                                KNoEndpointId );
    MCE_MCC_EVENT_POSTCONDITION_L 
                                
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iServer->iEvent == EMceMediaEventReceived );
    EUNIT_ASSERT( iServer->iItcEvent == EMceItcStateChanged );
    EUNIT_ASSERT( iServer->iItcData == EFalse );

    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EPaused );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EPaused );

    Reset();    
    iManager->ResumeL( rtpsink );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccResumeStream );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarting );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EPaused );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStarting );

    Reset();
    iManager->MccMediaResumed( iSession->iMccID,
                                SRVSTREAM(1)->LinkId(),
                                SRVSTREAM(1)->Id(),
                                KNoEndpointId );
    MCE_MCC_EVENT_POSTCONDITION_L 
               
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iServer->iEvent == EMceMediaEventReceived );
    EUNIT_ASSERT( iServer->iItcEvent == EMceItcStateChanged );
    EUNIT_ASSERT( iServer->iItcData == 1 );

    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStarted );
    }

//
// Tests
//
// void MccMediaInactive( TUint32 aSessionId,
//                        TUint32 aLinkId,
//                        TUint32 aStreamId,
//                        TUint32 aSinkSourceId )
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_MccMediaInactiveL()
    {
    SETUP( CreateAudioOutSessionL );

    Reset();
    iManager->MccMediaInactive( iSession->iMccID,0,0,0);
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iServer->iEvent == EMceMediaError );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    Reset();
//establish
    iSdpDocument = MceMediaManagerTestHelper::EstablishOutSessionL( 
                                                *iSession,
                                                *iManager,
                                                KMceMMTestSdpAnswerAudioIMS5PrecsSendrecv,
                                                iServer );
    

    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStarted );
    
    
    Reset();
    iManager->MccMediaInactive( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                KNoEndpointId );
    MCE_MCC_EVENT_POSTCONDITION_L 
                                
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iServer->iEvent == EMceMediaEventReceived );
    EUNIT_ASSERT( iServer->iItcEvent == EMceItcInactivityTimeout );
    

    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStarted );

    Reset();
    iManager->MccMediaInactive( iSession->iMccID,
                                SRVSTREAM(1)->LinkId(),
                                SRVSTREAM(1)->Id(),
                                KNoEndpointId );
    MCE_MCC_EVENT_POSTCONDITION_L 
                                
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iServer->iEvent == EMceMediaEventReceived );

    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStarted );
    }

//
// Tests
//
// void MccMediaActive( TUint32 aSessionId,
//                      TUint32 aLinkId,
//                      TUint32 aStreamId,
//                      TUint32 aSinkSourceId )
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_MccMediaActiveL()
    {
    //NOTE: main functionality tested in session update cases
    
    SETUP( CreateAudioOutSessionL );

    Reset();
    iManager->MccMediaActive( iSession->iMccID,0,0,0);
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iServer->iEvent == EMceMediaError );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    Reset();
//establish
    iSdpDocument = MceMediaManagerTestHelper::EstablishOutSessionL( 
                                                *iSession,
                                                *iManager,
                                                KMceMMTestSdpAnswerAudioIMS5PrecsSendrecv,
                                                iServer );
    

    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStarted );
    

    Reset();
    iManager->MccMediaActive( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                KNoEndpointId );
    MCE_MCC_EVENT_POSTCONDITION_L 
                                
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iServer->iEvent == EMceMediaEventReceived );

    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStarted );
    
    Reset();
    iManager->MccMediaInactive( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                KNoEndpointId );
    MCE_MCC_EVENT_POSTCONDITION_L 
                                
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iServer->iEvent == EMceMediaEventReceived );
    EUNIT_ASSERT( iServer->iItcEvent == EMceItcInactivityTimeout );

    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStarted );

    Reset();
    iManager->MccMediaActive( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                KNoEndpointId );
    MCE_MCC_EVENT_POSTCONDITION_L 
                                
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iServer->iEvent == EMceMediaEventReceived );

    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStarted );
    }

//
// Tests
//
// void MccCtrlError( TInt aError )
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_MccCtrlError_1L()
    {
    Reset();
    iManager->MccCtrlError( KErrGeneral, 0, 0, 0, 0 );

    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iServer->iEvent == EMceMediaError );
    EUNIT_ASSERT( iServer->iSession == NULL );
    }

//
// Tests
//
// void UT_CMceMediaManager::MccCtrlError( TInt aError,
//                    TUint32 aSessionId,
//                    TUint32 aLinkId,
//                    TUint32 aStreamId,
//                    TUint32 aSinkSourceId )
//
///
void UT_CMceMediaManager::UT_CMceMediaManager_MccCtrlError_2L()
    {
    SETUP( CreateAudioOutSessionL );

    Reset();
    iManager->MccCtrlError( KErrGeneral, iSession->iMccID,0,0,0);
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iServer->iEvent == EMceMediaError );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
    Reset();
//establish
    iSdpDocument = MceMediaManagerTestHelper::EstablishOutSessionL( 
                                                *iSession,
                                                *iManager,
                                                KMceMMTestSdpAnswerAudioIMS5PrecsSendrecv,
                                                iServer );
    

    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStarted );
    

    Reset();
    iManager->MccCtrlError( KErrGeneral,
                            iSession->iMccID,
                            SRVSTREAM(0)->LinkId(),
                            SRVSTREAM(0)->Id(),
                            KNoEndpointId );
                                
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    EUNIT_ASSERT( iServer->iEvent == EMceMediaError );
    EUNIT_ASSERT( iServer->iSession == NULL );

    EUNIT_ASSERT( SRVSTREAM(0)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(0)->Sink().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Source().State() == CMceSrvStream::EStarted );
    EUNIT_ASSERT( SRVSTREAM(1)->Sink().State() == CMceSrvStream::EStarted );
    }


void UT_CMceMediaManager::UT_CMceMediaManager_UpdateConfigKeyLL()
    {
    CMceComFileSource* endpoint = CMceComFileSource::NewLC();
    
    iManager->UpdateConfigKeyL( *endpoint );
    
    CleanupStack::PopAndDestroy( endpoint );

    FULL_SETUP_AND_PREPARE( CreateVideoOutSessionL );
    
    CMceComVideoStream& videoOut = static_cast<CMceComVideoStream&>( iSession->Streams()[KVideo]->BoundStreamL() );
    CMceComCameraSource* camera = static_cast<CMceComCameraSource*>( videoOut.Source() );
    
    // Nothing updated as stream not mcc prepared
    Reset();
    iManager->UpdateConfigKeyL( *camera );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    // Updated as stream is are mcc prepared
    iSdpDocument = MceMediaManagerTestHelper::EstablishOutSessionL( 
                                                *iSession,
                                                *iManager,
                                                KMceMMTestSdpAnswerVideoIMS5PrecsSendrecv,
                                                iServer );

    
    // Modify key to enable update
    HBufC8* modKey = _L8("confkeymodified").AllocLC();
    for ( TInt i = 0; i < videoOut.Codecs().Count(); i++ )
        {
        videoOut.Codecs()[ i ]->SetConfigKeyL( modKey );
        }
    CleanupStack::PopAndDestroy( modKey );
    Reset();
    iManager->UpdateConfigKeyL( *camera );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccStreamSettings );
    EUNIT_ASSERT( iMcc->iActions[ 1 ].iAction == CMccControllerStub::EMccGetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 2 ].iAction == CMccControllerStub::EMccSetCodec );
    EUNIT_ASSERT( iMcc->iActions[ 3 ].iAction == CMccControllerStub::ENone );
    }

void UT_CMceMediaManager::UT_CMceMediaManager_PrackUpdateLL()
    {
    SETUP( CreateAudioOutSessionL );
   
//update

    TMceReturnStatus status = iManager->UpdateL( *iSession );
    
    EUNIT_ASSERT( status == KMceAsync );
     
//1. link created
    MceMediaManagerTestHelper::LinkCreated( *iManager, SRVSTREAM(0)->LinkId() );

//1. prepared
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
                                
//2. prepared
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(1)->LinkId(),
                                SRVSTREAM(1)->Id(),
                                SRVSTREAM(1)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    iManager->MccMediaPrepared( iSession->iMccID,
                                SRVSTREAM(1)->LinkId(),
                                SRVSTREAM(1)->Id(),
                                SRVSTREAM(1)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    Reset();                                
//1. started
    iManager->MccMediaStarted( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Sink().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
    iManager->MccMediaStarted( iSession->iMccID,
                                SRVSTREAM(0)->LinkId(),
                                SRVSTREAM(0)->Id(),
                                SRVSTREAM(0)->Source().Id() );
    MCE_MCC_EVENT_POSTCONDITION_L 
	
	MCE_CHECK_MEMORY_LEAVE( iServer->iEvent != EMceMediaUpdated )
    //server asserts
    EUNIT_ASSERT( iServer->iEvent == EMceMediaUpdated );
    EUNIT_ASSERT( iServer->iSession == iSession );


//encode 
    iSdpDocument = iManager->EncodeL( *iSession );

    delete iSdpDocument;
    iSdpDocument = NULL;
    iSdpDocument = CSdpDocument::DecodeL( KMceMMTestSdpAnswerAudioIMS5PrecsSendrecv );

//decode answer
    iSession->AnswerType() = KMceNegotiationAnswerTypeFinal;
    TMceSipWarningCode decodeStatus = iManager->DecodeL( *iSession, *iSdpDocument );
    
//update
    status = iManager->UpdateL( *iSession );
    
//2. started
    iManager->MccMediaStarted( iSession->iMccID,
                               SRVSTREAM(1)->LinkId(),
                               SRVSTREAM(1)->Id(),
                               KNoEndpointId );
    MCE_MCC_EVENT_POSTCONDITION_L 
    
    Reset();

//
//update
//    
    iUpdate = iSession->CloneL();
    iUpdate->SetObserver( *iServer );
    CMceComAudioStream* audio = static_cast<CMceComAudioStream*>( iUpdate->Streams()[0] );
   
//update
    // Update may fail also with KErrArgument in oom situation
    
    iUpdate->iCodecSelection = KMceCodecSelectionMerge;
    
    MCE_LEAVE_HANDLING( status = iManager->UpdateL( *iUpdate ), KErrArgument );
    EUNIT_ASSERT( status == KMceReady );
     
    }


void UT_CMceMediaManager::UT_CMceMediaManager_SdpSessionL()
    {
    SETUP( CreateVideoOutSessionL );
    
	EUNIT_ASSERT( iManager->iMceSdpSessions.Count() == 0 );
	EUNIT_ASSERT( &iSession->SdpSession() == NULL);
	EUNIT_ASSERT( iSession->iCloneOrigin == NULL);
	
	Reset();
	EUNIT_ASSERT( iManager->SdpSession( *iSession) == NULL);
	
//	//close main
	iSdpDocument = MceMediaManagerTestHelper::EstablishOutSessionL( *iSession,
			*iManager, KMceMMTestSdpAnswerVideoIMS5PrecsSendrecv, iServer);

	EUNIT_ASSERT(iManager->iMceSdpSessions.Count() == 1);
	EUNIT_ASSERT( &iSession->SdpSession() == iManager->iMceSdpSessions[ 0 ]);
	EUNIT_ASSERT(iManager->iMceSdpSessions[ 0 ]->MediaSession() == iSession);
	EUNIT_ASSERT(iManager->iMceSdpSessions[ 0 ]->Backup() == NULL);
	
	Reset();
	EUNIT_ASSERT( iManager->SdpSession( *iSession) != NULL);

	iUpdate = iSession->CloneL();
	iUpdate->SetObserver( *iServer );
	iUpdate->DetachSDPSession();

	EUNIT_ASSERT( &iUpdate->SdpSession() == NULL);
	EUNIT_ASSERT( iUpdate->iCloneOrigin != NULL);
	EUNIT_ASSERT( &iUpdate->iCloneOrigin->SdpSession() != NULL);

//	Reset();
//	EUNIT_ASSERT( iManager->SdpSession( *iUpdate) != NULL);
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceMediaManager,
    "CMceMediaManager tests",
    "UNIT" )

EUNIT_TEST(
    "Basic - test ",
    "CMceMediaManager",
    "<basic>",
    "FUNCTIONALITY",
    Setup_BasicL, UT_CMceMediaManager_BasicLL, Teardown)

EUNIT_TEST(
    "EncodeL - test ",
    "CMceMediaManager",
    "EncodeL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_EncodeLL, Teardown)

EUNIT_TEST(
    "DecodeL - test ",
    "CMceMediaManager",
    "DecodeL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_DecodeLL, Teardown)

EUNIT_TEST(
    "UpdateL - MO audio test ",
    "CMceMediaManager",
    "UpdateL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_UpdateLL, Teardown)

EUNIT_TEST(
    "UpdateL - MO live video test ",
    "CMceMediaManager",
    "UpdateL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_UpdateL_1L, Teardown)

EUNIT_TEST(
    "UpdateL - MT multimedia test ",
    "CMceMediaManager",
    "UpdateL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_UpdateL_2L, Teardown)

EUNIT_TEST(
    "UpdateL - signalling test",
    "CMceMediaManager",
    "UpdateL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_UpdateL_3L, Teardown)

EUNIT_TEST(
    "UpdateL - hold test",
    "CMceMediaManager",
    "UpdateL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_UpdateL_4L, Teardown)

EUNIT_TEST(
    "UpdateL - failed hold test",
    "CMceMediaManager",
    "UpdateL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_UpdateL_4_1L, Teardown)

EUNIT_TEST(
    "UpdateL - no signalling test",
    "CMceMediaManager",
    "UpdateL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_UpdateL_5L, Teardown)

EUNIT_TEST(
    "UpdateL - MTU received update test",
    "CMceMediaManager",
    "UpdateL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_UpdateL_6L, Teardown)
    
EUNIT_TEST(
    "UpdateL - MTU added&removed streams test",
    "CMceMediaManager",
    "UpdateL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_UpdateL_8L, Teardown)

EUNIT_TEST(
    "UpdateL - MO POC audio",
    "CMceMediaManager",
    "UpdateL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_UpdateL_9L, Teardown)

EUNIT_TEST(
    "ReserveL - test ",
    "CMceMediaManager",
    "ReserveL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_ReserveLL, Teardown)

EUNIT_TEST(
    "ReservationNeeded - test ",
    "CMceMediaManager",
    "ReservationNeeded",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_ReservationNeededL, Teardown)

EUNIT_TEST(
    "NeedToNegotiate - test ",
    "CMceMediaManager",
    "NeedToNegotiate",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_NeedToNegotiateL, Teardown)

EUNIT_TEST(
    "CreateRejectedAnswer - test ",
    "CMceMediaManager",
    "CreateRejectedAnswer",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_CreateRejectedAnswerL, Teardown)

EUNIT_TEST(
    "PauseL(sink) - test ",
    "CMceMediaManager",
    "PauseL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_PauseL_1L, Teardown)

EUNIT_TEST(
    "PauseL(source) - test ",
    "CMceMediaManager",
    "PauseL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_PauseL_2L, Teardown)

EUNIT_TEST(
    "ResumeL(sink) - test ",
    "CMceMediaManager",
    "ResumeL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_ResumeL_1L, Teardown)

EUNIT_TEST(
    "ResumeL(source) - test ",
    "CMceMediaManager",
    "ResumeL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_ResumeL_2L, Teardown)

EUNIT_TEST(
    "CloseSession - test ",
    "CMceMediaManager",
    "CloseSession",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_CloseSessionL, Teardown)
 
EUNIT_TEST(
    "CloseSession(merged)- test ",
    "CMceMediaManager",
    "CloseSession",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_CloseSession_1L, Teardown)

EUNIT_TEST(
    "StartInactivityTimerL - test ",
    "CMceMediaManager",
    "StartInactivityTimerL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_StartInactivityTimerLL, Teardown)

EUNIT_TEST(
    "StopInactivityTimerL - test ",
    "CMceMediaManager",
    "StopInactivityTimerL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_StopInactivityTimerLL, Teardown)

EUNIT_TEST(
    "SendRTCPAnyDataL - test ",
    "CMceMediaManager",
    "SendRTCPAnyDataL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_SendRTCPAnyDataLL, Teardown)

EUNIT_TEST(
    "SendRTCPReceiverReportL - test ",
    "CMceMediaManager",
    "SendRTCPReceiverReportL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_SendRTCPReceiverReportLL, Teardown)

EUNIT_TEST(
    "SendRTCPSenderReportL - test ",
    "CMceMediaManager",
    "SendRTCPSenderReportL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_SendRTCPSenderReportLL, Teardown)

EUNIT_TEST(
    "GetVolumeL - test ",
    "CMceMediaManager",
    "GetVolumeL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_GetVolumeLL, Teardown)

EUNIT_TEST(
    "GetMaxVolumeL - test ",
    "CMceMediaManager",
    "GetMaxVolumeL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_GetMaxVolumeLL, Teardown)

EUNIT_TEST(
    "SetVolumeL - test ",
    "CMceMediaManager",
    "SetVolumeL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_SetVolumeLL, Teardown)

EUNIT_TEST(
    "SetBalanceL - test ",
    "CMceMediaManager",
    "SetBalanceL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_SetBalanceLL, Teardown)

EUNIT_TEST(
    "GetGainL - test ",
    "CMceMediaManager",
    "GetGainL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_GetGainLL, Teardown)

EUNIT_TEST(
    "GetMaxGainL - test ",
    "CMceMediaManager",
    "GetMaxGainL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_GetMaxGainLL, Teardown)

EUNIT_TEST(
    "SetGainL - test ",
    "CMceMediaManager",
    "SetGainL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_SetGainLL, Teardown)

EUNIT_TEST (
    "GetAudioRoutingL test",
    "CMceMediaManager",
    "GetAudioRoutingL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_GetAudioRoutingLL, Teardown)

EUNIT_TEST (
    "SetAudioRoutingL test",
    "CMceMediaManager",
    "SetAudioRoutingL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_SetAudioRoutingLL, Teardown)


EUNIT_TEST (
    "GetDisplayPriorityL test",
    "CMceMediaManager",
    "GetDisplayPriorityL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_GetDisplayPriorityLL, Teardown)

EUNIT_TEST (
    "GetDisplayRectL test",
    "CMceMediaManager",
    "GetDisplayRectL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_GetDisplayRectLL, Teardown)

EUNIT_TEST (
    "GetDisplaysAvailableL test",
    "CMceMediaManager",
    "GetDisplaysAvailableL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_GetDisplaysAvailableLL, Teardown)

EUNIT_TEST (
    "GetDisplayRotationL test",
    "CMceMediaManager",
    "GetDisplayRotationL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_GetDisplayRotationLL, Teardown)

EUNIT_TEST (
    "SetDisplayRotationL test",
    "CMceMediaManager",
    "SetDisplayRotationL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_SetDisplayRotationLL, Teardown)

EUNIT_TEST (
    "SetContrastL test",
    "CMceMediaManager",
    "SetContrastL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_SetContrastLL, Teardown)

EUNIT_TEST (
    "SetDisplayIndexL test",
    "CMceMediaManager",
    "SetDisplayIndexL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_SetDisplayIndexLL, Teardown)

EUNIT_TEST (
    "SetDisplayPriorityL test",
    "CMceMediaManager",
    "SetDisplayPriorityL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_SetDisplayPriorityLL, Teardown)

EUNIT_TEST (
    "SetDisplayRectL test",
    "CMceMediaManager",
    "SetDisplayRectL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_SetDisplayRectLL, Teardown)

EUNIT_TEST (
    "GetBrightnessL test",
    "CMceMediaManager",
    "GetBrightnessL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_GetBrightnessLL, Teardown)

EUNIT_TEST (
    "GetCameraInfoL test",
    "CMceMediaManager",
    "GetCameraInfoL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_GetCameraInfoLL, Teardown)

EUNIT_TEST (
    "GetCamerasAvailableL test",
    "CMceMediaManager",
    "GetCamerasAvailableL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_GetCamerasAvailableLL, Teardown)

EUNIT_TEST (
    "GetContrastL test",
    "CMceMediaManager",
    "GetContrastL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_GetContrastLL, Teardown)

EUNIT_TEST (
    "GetDigitalZoomFactorL test",
    "CMceMediaManager",
    "GetDigitalZoomFactorL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_GetDigitalZoomFactorLL, Teardown)

EUNIT_TEST (
    "GetExposureL test",
    "CMceMediaManager",
    "GetExposureL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_GetExposureLL, Teardown)

EUNIT_TEST (
    "GetWhiteBalanceL test",
    "CMceMediaManager",
    "GetWhiteBalanceL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_GetWhiteBalanceLL, Teardown)

EUNIT_TEST (
    "GetZoomFactorL test",
    "CMceMediaManager",
    "GetZoomFactorL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_GetZoomFactorLL, Teardown)

EUNIT_TEST (
    "SetWhiteBalanceL test",
    "CMceMediaManager",
    "SetWhiteBalanceL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_SetWhiteBalanceLL, Teardown)

EUNIT_TEST (
    "SetZoomFactorL test",
    "CMceMediaManager",
    "SetZoomFactorL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_SetZoomFactorLL, Teardown)

EUNIT_TEST (
    "SetBrightnessL test",
    "CMceMediaManager",
    "SetBrightnessL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_SetBrightnessLL, Teardown)

EUNIT_TEST (
    "SetCameraIndexL test",
    "CMceMediaManager",
    "SetCameraIndexL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_SetCameraIndexLL, Teardown)

EUNIT_TEST (
    "SetDigitalZoomFactorL test",
    "CMceMediaManager",
    "SetDigitalZoomFactorL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_SetDigitalZoomFactorLL, Teardown)

EUNIT_TEST (
    "SetExposureL test",
    "CMceMediaManager",
    "SetExposureL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_SetExposureLL, Teardown)

EUNIT_TEST (
    "FastForwardL test",
    "CMceMediaManager",
    "FastForwardL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_FastForwardLL, Teardown)

EUNIT_TEST (
    "FastRewindL test",
    "CMceMediaManager",
    "FastRewindL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_FastRewindLL, Teardown)

EUNIT_TEST (
    "GetFileInfoL test",
    "CMceMediaManager",
    "GetFileInfoL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_GetFileInfoLL, Teardown)

EUNIT_TEST (
    "GetFileInfoL 2 test",
    "CMceMediaManager",
    "GetFileInfoL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_GetFileInfoL_2L, Teardown)

EUNIT_TEST (
    "GetFilePositionL test",
    "CMceMediaManager",
    "GetFilePositionL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_GetFilePositionLL, Teardown)

EUNIT_TEST (
    "SetFileInfoL test",
    "CMceMediaManager",
    "SetFileInfoL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_SetFileInfoLL, Teardown)

EUNIT_TEST (
    "SetFilePositionL test",
    "CMceMediaManager",
    "SetFilePositionL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_SetFilePositionLL, Teardown)

EUNIT_TEST (
    "SetCNameL test",
    "CMceMediaManager",
    "SetCNameL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_SetCNameLL, Teardown)

EUNIT_TEST(
    "GetSupportedCodecs - test ",
    "CMceMediaManager",
    "GetSupportedCodecs",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_GetSupportedCodecsL, Teardown)

EUNIT_TEST(
    "GetCapabilitiesL(audio) - test ",
    "CMceMediaManager",
    "GetCapabilitiesL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_GetCapabilitiesL_1L, Teardown)

EUNIT_TEST(
    "GetCapabilitiesL(video) - test ",
    "CMceMediaManager",
    "GetCapabilitiesL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_GetCapabilitiesL_2L, Teardown)


EUNIT_TEST(
    "MccEventReceived - test ",
    "CMceMediaManager",
    "MccEventReceived",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_MccEventReceivedL, Teardown)

EUNIT_TEST(
    "MccMediaPrepared - test ",
    "CMceMediaManager",
    "MccMediaPrepared",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_MccMediaPreparedL, Teardown)

EUNIT_TEST(
    "MccMediaStarted - test ",
    "CMceMediaManager",
    "MccMediaStarted",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_MccMediaStartedL, Teardown)

EUNIT_TEST(
    "MccMediaStopped - test ",
    "CMceMediaManager",
    "MccMediaStopped",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_MccMediaStoppedL, Teardown)

EUNIT_TEST(
    "MccMediaPaused - test ",
    "CMceMediaManager",
    "MccMediaPaused",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_MccMediaPausedL, Teardown)

EUNIT_TEST(
    "MccMediaResumed - test ",
    "CMceMediaManager",
    "MccMediaResumed",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_MccMediaResumedL, Teardown)

EUNIT_TEST(
    "MccMediaInactive - test ",
    "CMceMediaManager",
    "MccMediaInactive",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_MccMediaInactiveL, Teardown)

EUNIT_TEST(
    "MccMediaActive - test ",
    "CMceMediaManager",
    "MccMediaActive",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_MccMediaActiveL, Teardown)


EUNIT_TEST(
    "MccCtrlError(general) - test ",
    "CMceMediaManager",
    "MccCtrlError",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_MccCtrlError_1L, Teardown)

EUNIT_TEST(
    "MccCtrlError(media) - test ",
    "CMceMediaManager",
    "MccCtrlError",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_MccCtrlError_2L, Teardown)

EUNIT_TEST (
    "TranscodeFileL test",
    "CMceMediaManager",
    "TranscodeFileL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_TranscodeFileLL, Teardown)

EUNIT_TEST (
    "TranscodeFileLWithCodecL test",
    "CMceMediaManager",
    "TranscodeFileL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_TranscodeFileLWithCodecL, Teardown)



EUNIT_TEST (
    "MccTranscodeEventReceived test",
    "CMceMediaManager",
    "MccTranscodeEventReceived",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_MccTranscodeEventReceivedL, Teardown)

EUNIT_TEST (
    "TranscodingSession test",
    "CMceMediaManager",
    "TranscodingSession",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_TranscodingSessionL, Teardown)

EUNIT_TEST (
    "TranscodingSession 2 test",
    "CMceMediaManager",
    "TranscodingSession",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_TranscodingSession_2L, Teardown)

EUNIT_TEST (
    "CancelTranscodeFileL test",
    "CMceMediaManager",
    "CancelTranscodeFileL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_CancelTranscodeFileLL, Teardown)

EUNIT_TEST (
    "ConfirmationNeeded test",
    "CMceMediaManager",
    "ConfirmationNeeded",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_ConfirmationNeededL, Teardown)

EUNIT_TEST (
    "Session test",
    "CMceMediaManager",
    "Session",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_SessionL, Teardown)

EUNIT_TEST (
    "InitializeMccStreamL test",
    "CMceMediaManager",
    "InitializeMccStreamL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_InitializeMccStreamLL, Teardown)
        
EUNIT_TEST (
    "CreateMccLinkL test",
    "CMceMediaManager",
    "CreateMccLinkL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_CreateMccLinkLL, Teardown)
        
EUNIT_TEST (
    "CreateMccStreamL test",
    "CMceMediaManager",
    "CreateMccStreamL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_CreateMccStreamLL, Teardown)
        
EUNIT_TEST (
    "PrepareMccStreamL test",
    "CMceMediaManager",
    "PrepareMccStreamL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_PrepareMccStreamLL, Teardown)
        
EUNIT_TEST (
    "StartMccStreamL test",
    "CMceMediaManager",
    "StartMccStreamL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_StartMccStreamLL, Teardown)
        
EUNIT_TEST (
    "AdjustMccStreamL test",
    "CMceMediaManager",
    "AdjustMccStreamL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_AdjustMccStreamLL, Teardown)
        
EUNIT_TEST (
    "ValidateMccStreamL test",
    "CMceMediaManager",
    "ValidateMccStreamL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_ValidateMccStreamLL, Teardown)
        
EUNIT_TEST (
    "StopMccStream test",
    "CMceMediaManager",
    "StopMccStream",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_StopMccStreamL, Teardown)
        
        
EUNIT_TEST (
    "SynchronizeMccStreamL test",
    "CMceMediaManager",
    "SynchronizeMccStreamL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_SynchronizeMccStreamLL, Teardown)
        
EUNIT_TEST (
    "CleanupL test",
    "CMceMediaManager",
    "CleanupL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_CleanupLL, Teardown)
        
        
EUNIT_TEST (
    "DisposeMediaStreamL test",
    "CMceMediaManager",
    "DisposeMediaStreamL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_DisposeMediaStreamLL, Teardown)
        

EUNIT_TEST (
    "DisposeCodecL test",
    "CMceMediaManager",
    "DisposeCodecL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_DisposeCodecLL, Teardown)

EUNIT_TEST (
    "RequireSignallingL test",
    "CMceMediaManager",
    "RequireSignallingL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_RequireSignallingLL, Teardown)

EUNIT_TEST (
    "CleanupSessionL test",
    "CMceMediaManager",
    "CleanupSessionL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_CleanupSessionLL, Teardown)

EUNIT_TEST (
    "FillMceFileInfoL test",
    "CMceMediaManager",
    "FillMceFileInfoL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_FillMceFileInfoLL, Teardown)

EUNIT_TEST (
    "SetRemoteAddressL test",
    "CMceMediaManager",
    "SetRemoteAddressL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_SetRemoteAddressLL, Teardown)

EUNIT_TEST (
    "UpdateConfigKeyL test",
    "CMceMediaManager",
    "UpdateConfigKeyL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_UpdateConfigKeyLL, Teardown)

EUNIT_TEST (
    "PrackUpdateL test",
    "CMceMediaManager",
    "PrackUpdateL",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_PrackUpdateLL, Teardown)

EUNIT_TEST (
    "SdpSession test",
    "CMceMediaManager",
    "SdpSession",
    "FUNCTIONALITY",
    SetupL, UT_CMceMediaManager_SdpSessionL, Teardown)
    
EUNIT_END_TEST_TABLE

//  END OF FILE
