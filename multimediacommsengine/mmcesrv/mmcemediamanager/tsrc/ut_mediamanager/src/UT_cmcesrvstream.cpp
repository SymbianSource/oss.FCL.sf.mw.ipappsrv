/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
#include "UT_cmcesrvstream.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <sdpcodecstringconstants.h>
#include "mcemediamanager.h"
#include "CMccController_stub.h"

#define RESET()\
    iMcc->Reset();\
    iServer->Reset()
    
#include "mcesrvstream.h"
#include "mcesrvsink.h"
#include "mcesrvsource.h"


//  INTERNAL INCLUDES

// CONSTRUCTION
UT_CMceSrvStream* UT_CMceSrvStream::NewL()
    {
    UT_CMceSrvStream* self = UT_CMceSrvStream::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceSrvStream* UT_CMceSrvStream::NewLC()
    {
    UT_CMceSrvStream* self = new( ELeave ) UT_CMceSrvStream();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceSrvStream::~UT_CMceSrvStream()
    {
    }

// Default constructor
UT_CMceSrvStream::UT_CMceSrvStream()
    {
    }

// Second phase construct
void UT_CMceSrvStream::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_CMceSrvStream::SetupL()
    {
	CMccControllerStub::OpenL();
    iMcc = CMccControllerStub::Stub();
    
    iServer = new (ELeave) CMceServerStub();
    iManager = CMceMediaManager::NewL( *iServer );
    iSession = MceMediaManagerTestHelper::CreateMultimediaOutSessionL();
    iSession->SetObserver( *iServer );
    iSession->PrepareL( *iManager );
    iStream = new (ELeave) CMceSrvStream();
    delete iStream;
    iStream = NULL;
	iStream = iSession->MccStreams()[0];
    } 

void UT_CMceSrvStream::Setup2L()
    {
    CMccControllerStub::OpenL();
    iMcc = CMccControllerStub::Stub();
    
    iServer = new (ELeave) CMceServerStub();
    iManager = CMceMediaManager::NewL( *iServer );
    iSession = MceMediaManagerTestHelper::CreateMultimediaBundledOutSessionL();
    iSession->SetObserver( *iServer );
    iSession->PrepareL( *iManager );
    iStream = new (ELeave) CMceSrvStream();
    delete iStream;
    iStream = NULL;
	iStream = iSession->MccStreams()[0];
    }
    
void UT_CMceSrvStream::Teardown()
    {
	CMceComSession::Delete( iSession, *iManager );
	delete iManager;
	iManager = NULL;
	delete iServer;
	iServer = NULL;
	CMccControllerStub::Close();
	iMcc = NULL;
    iStream = NULL;
	
    }


void UT_CMceSrvStream::UT_CMceSrvStream_SessionIdL()
    {
    
    EUNIT_ASSERT( iStream->SessionId() == KMceNotAssigned );
    
    }



void UT_CMceSrvStream::UT_CMceSrvStream_LinkIdL()
    {
    
    EUNIT_ASSERT( iStream->LinkId() == KMceNotAssigned );

    }



void UT_CMceSrvStream::UT_CMceSrvStream_SetLinkIdL()
    {
    EUNIT_ASSERT( iStream->LinkId() == KMceNotAssigned );
    
    iStream->SetLinkId( 200 );
    
    EUNIT_ASSERT( iStream->LinkId() == 200 );
    
    }



void UT_CMceSrvStream::UT_CMceSrvStream_IapIdL()
    {
    
    EUNIT_ASSERT( iStream->IapId() == 0 );
    
    }



void UT_CMceSrvStream::UT_CMceSrvStream_LocalMediaPortL()
    {
    
    EUNIT_ASSERT( iStream->LocalMediaPort() != 0 );
    
    }


void UT_CMceSrvStream::UT_CMceSrvStream_IsEqualL()
    {
    CMceSrvStream* stream = NULL;

    EUNIT_ASSERT( !iStream->IsEqual( *stream ) );

    stream = iSession->MccStreams()[1];
    stream->Codec().iID = 100;
    stream->Source().Data().iID = 100;
    stream->Sink().Data().iID = 100;

    EUNIT_ASSERT( !iStream->IsEqual( *stream ) );

    stream->Codec().iID = iStream->Codec().iID;
    EUNIT_ASSERT( !iStream->IsEqual( *stream ) );

    stream->Source().Data().iID = iStream->Source().Data().iID;
    EUNIT_ASSERT( !iStream->IsEqual( *stream ) );

    stream->Sink().Data().iID = iStream->Sink().Data().iID;
    EUNIT_ASSERT( iStream->IsEqual( *stream ) );
    
    }

void UT_CMceSrvStream::UT_CMceSrvStream_IsMccPreparedL()
    {
    
    EUNIT_ASSERT( !iStream->IsMccPrepared() );

    iStream->Id() = 200;
    EUNIT_ASSERT( iStream->IsMccPrepared() );
    
    }

void UT_CMceSrvStream::UT_CMceSrvStream_IsAdoptedL()
    {
    
    EUNIT_ASSERT( !iStream->IsAdopted() );
    
    }

void UT_CMceSrvStream::UT_CMceSrvStream_StateL()
    {
    
    EUNIT_ASSERT( iStream->State() == CMceSrvStream::ECreated );
    iStream->Data().Enable( EFalse, EFalse );

    EUNIT_ASSERT( iStream->State() == CMceSrvStream::EInactive );

    iStream->Source().SetState( CMceSrvStream::EStopped );
    EUNIT_ASSERT( iStream->State() == CMceSrvStream::EInactive );

    iStream->Sink().SetState( CMceSrvStream::EStopped );
    EUNIT_ASSERT( iStream->State() == CMceSrvStream::EStopped );

    iStream->Source().SetState( CMceSrvStream::EPending );
    EUNIT_ASSERT( iStream->State() == CMceSrvStream::EInactive );

    iStream->Sink().SetState( CMceSrvStream::EPending );
    EUNIT_ASSERT( iStream->State() == CMceSrvStream::EPending );

    iStream->Sink().SetState( CMceSrvStream::EPreparing );
    EUNIT_ASSERT( iStream->State() == CMceSrvStream::EPreparing );

    iStream->Source().SetState( CMceSrvStream::EPreparing );
    EUNIT_ASSERT( iStream->State() == CMceSrvStream::EPreparing );

    iStream->Source().SetState( CMceSrvStream::ECreated );
    iStream->Sink().SetState( CMceSrvStream::EStarting );
    EUNIT_ASSERT( iStream->State() == CMceSrvStream::EStarting );

    iStream->Source().SetState( CMceSrvStream::EStarting );
    EUNIT_ASSERT( iStream->State() == CMceSrvStream::EStarting );

    iStream->Source().SetState( CMceSrvStream::ECreated );
    iStream->Sink().SetState( CMceSrvStream::EPrepared );
    EUNIT_ASSERT( iStream->State() == CMceSrvStream::EPrepared );

    iStream->Source().SetState( CMceSrvStream::EPrepared );
    EUNIT_ASSERT( iStream->State() == CMceSrvStream::EPrepared );

    iStream->Source().SetState( CMceSrvStream::ECreated );
    iStream->Sink().SetState( CMceSrvStream::EStarted );
    EUNIT_ASSERT( iStream->State() == CMceSrvStream::EStarted );

    iStream->Source().SetState( CMceSrvStream::EStarted );
    EUNIT_ASSERT( iStream->State() == CMceSrvStream::EStarted );

    iStream->Source().SetState( CMceSrvStream::ECreated );
    iStream->Sink().SetState( CMceSrvStream::EPaused );
    EUNIT_ASSERT( iStream->State() == CMceSrvStream::EStarted );

    iStream->Source().SetState( CMceSrvStream::EPaused );
    EUNIT_ASSERT( iStream->State() == CMceSrvStream::EStarted );
    
    
    }

void UT_CMceSrvStream::UT_CMceSrvStream_SetLocalMediaPortL()
    {
    iStream->SetLocalMediaPort( 100 );
    EUNIT_ASSERT( iStream->LocalMediaPort() == 100 );
    
    }

void UT_CMceSrvStream::UT_CMceSrvStream_IdL()
    {
    EUNIT_ASSERT( iStream->Id() == KMceNotAssigned );
    
    }

void UT_CMceSrvStream::UT_CMceSrvStream_StreamTypeL()
    {
    EUNIT_ASSERT( iStream->StreamType() == CMceComMediaStream::EReceiveStream );
    
    }

void UT_CMceSrvStream::UT_CMceSrvStream_DirectionL()
    {
    EUNIT_ASSERT( iStream->Direction() == CMceComMediaStream::EReceiveStream );
    
    iStream->Data().Enable( EFalse, EFalse );

    EUNIT_ASSERT( iStream->Direction() == CMceComMediaStream::EReceiveStream );

    iStream->Data().Enable( ETrue, EFalse );
    iStream->Data().iLinkedStream->Enable( EFalse, EFalse );

    EUNIT_ASSERT( iStream->Direction() == CMceComMediaStream::EReceiveOnlyStream );
    iStream->Data().iLinkedStream->Enable( ETrue, EFalse );

    CMceSrvStream* stream = iSession->MccStreams()[1];

    EUNIT_ASSERT( stream->Direction() == CMceComMediaStream::ESendStream );
    
    stream->Data().Enable( EFalse, EFalse );

    EUNIT_ASSERT( stream->Direction() == CMceComMediaStream::ESendStream );

    stream->Data().Enable( ETrue, EFalse );
    stream->Data().iLinkedStream->Enable( EFalse, EFalse );

    EUNIT_ASSERT( stream->Direction() == CMceComMediaStream::ESendOnlyStream );
    
    }

void UT_CMceSrvStream::UT_CMceSrvStream_IsMergedL()
    {
    
    EUNIT_ASSERT( !iStream->IsMerged() );
    
    }

void UT_CMceSrvStream::UT_CMceSrvStream_DataL()
    {

    EUNIT_ASSERT( &iStream->Data() != NULL );
    
    }


void UT_CMceSrvStream::UT_CMceSrvStream_CodecL()
    {
    EUNIT_ASSERT( &iStream->Codec() != NULL );
    }

void UT_CMceSrvStream::UT_CMceSrvStream_SourceL()
    {
    EUNIT_ASSERT( &iStream->Source() != NULL );
    }

void UT_CMceSrvStream::UT_CMceSrvStream_SinkL()
    {
    EUNIT_ASSERT( &iStream->Sink() != NULL );
    }

void UT_CMceSrvStream::UT_CMceSrvStream_RemoteIpAddressL()
    {
    
    EUNIT_ASSERT( iStream->RemoteIpAddress().Length() > 0 );
    
    }

void UT_CMceSrvStream::UT_CMceSrvStream_MccStreamTypeL()
    {
    
    EUNIT_ASSERT( iStream->MccStreamType() == KMccAudioDownlinkStream );
    
    }

void UT_CMceSrvStream::UT_CMceSrvStream_LinkTypeL()
    {
    EUNIT_ASSERT( iStream->LinkType() == KMccLinkGeneral );
    
    }

void UT_CMceSrvStream::UT_CMceSrvStream_ConsumesL()
    {

    TMceMccComEvent event( KMccStreamStarted );
    event.iStreamId = 10;
    event.iEndpointId = 10;
    event.iLinkId = 10;
        
    EUNIT_ASSERT( !iStream->Consumes( event ) );

    iStream->Id() = event.iStreamId;
    EUNIT_ASSERT( iStream->Consumes( event ) );

    event.iStreamId = 0;
    EUNIT_ASSERT( !iStream->Consumes( event ) );

    iStream->SetLinkId( event.iLinkId );
    EUNIT_ASSERT( !iStream->Consumes( event ) );
    
    iStream->Source().Id() = event.iEndpointId;
    EUNIT_ASSERT( iStream->Consumes( event ) );

    iStream->Sink().Id() = event.iEndpointId;
    EUNIT_ASSERT( iStream->Consumes( event ) );

    event.iEndpointId = 0;
    EUNIT_ASSERT( iStream->Consumes( event ) );

    iStream->SetLinkId( 500 );
    EUNIT_ASSERT( !iStream->Consumes( event ) );
    
    }

void UT_CMceSrvStream::UT_CMceSrvStream_MccPrepareCalledLL()
    {
    //null test
    iStream->MccPrepareCalledL();
    
    }



void UT_CMceSrvStream::UT_CMceSrvStream_DecodeLL()
    {

    EUNIT_ASSERT( iSession->MccStreams().Count() == 5 );
	for ( TInt i = 0; i < iSession->Streams().Count(); i++)
		{
		CMceSrvStream::DecodeL( iSession->MccStreams(), *iSession->Streams()[i], *iManager );
		}

    EUNIT_ASSERT( iSession->MccStreams().Count() == 10 );


    iSession->Streams()[0]->BindL( NULL );
    CMceSrvStream::DecodeL( iSession->MccStreams(), *iSession->Streams()[0], *iManager );

    iSession->Streams()[0]->iType = KMceNotAssigned;
	EUNIT_ASSERT_LEAVE( 
	    CMceSrvStream::DecodeL( iSession->MccStreams(), *iSession->Streams()[0], *iManager ) );
    		
    EUNIT_ASSERT( iSession->MccStreams().Count() == 11 );
		
    
    }


void UT_CMceSrvStream::UT_CMceSrvStream_EventReceived_Static_L()
    {
    TMceMccComEvent event( KMccStreamStarted );
    event.iStreamId = 10;
    event.iEndpointId = 10;
    event.iLinkId = 10;

	EUNIT_ASSERT( CMceSrvStream::EventReceived( iSession->MccStreams(), event ) == NULL );
	
    iStream->Id() = event.iStreamId;
	EUNIT_ASSERT( CMceSrvStream::EventReceived( iSession->MccStreams(), event ) != NULL );
	
    
    }

void UT_CMceSrvStream::UT_CMceSrvStream_EventReceivedL()
    {
    TMceMccComEvent event( KMccStreamStarted );
    event.iStreamId = 10;
    event.iEndpointId = 0;
    event.iLinkId = 10;
    EUNIT_ASSERT( iStream->EventReceived( event, ETrue ) == KMceEventConsumed );
    
    iStream->Id() = event.iStreamId;
    EUNIT_ASSERT( iStream->EventReceived( event, ETrue ) == KMceEventConsumed );
    
    iStream->SetLinkId( event.iLinkId );
    EUNIT_ASSERT( iStream->EventReceived( event, ETrue ) == KMceEventConsumed );
    EUNIT_ASSERT( iStream->EventReceived( event, EFalse ) == KMceEventConsumed );
    
    iStream->Sink().Id() = 100;
    iStream->Source().Id() = 200;
    event.iEndpointId = 10;
    EUNIT_ASSERT( iStream->EventReceived( event, ETrue ) == KMceEventConsumed );
    
    event.iEndpointId = 100;
    EUNIT_ASSERT( iStream->EventReceived( event, ETrue ) == KMceEventConsumed );
    
    event.iEndpointId = 200;
    EUNIT_ASSERT( iStream->EventReceived( event, ETrue ) == KMceEventConsumed );
    
    // Test that media error (ICMP) is ignored if stream is already disabled.
    event.iEvent = KMccStreamError;
    event.iError = KErrHostUnreach;
    iStream->Data().iState = CMceMediaStream::EStreaming;
    iServer->iItcData = CMceMediaStream::EStreaming;
    EUNIT_ASSERT( iStream->EventReceived( event, ETrue ) == KMceEventConsumed );
    EUNIT_ASSERT( iStream->Data().iState == CMceMediaStream::EDisabled );
    EUNIT_ASSERT( iServer->iItcData >> 8 == CMceMediaStream::EDisabled || 
                  iServer->iItcData == CMceMediaStream::EDisabled );
    
    iServer->iItcData = CMceMediaStream::EStreaming;
    EUNIT_ASSERT( iStream->EventReceived( event, ETrue ) == KMceEventConsumed );
    EUNIT_ASSERT( iServer->iItcData == CMceMediaStream::EStreaming );
    }

void UT_CMceSrvStream::UT_CMceSrvStream_CleanupL()
    {
    
    //Null testing
    iStream->Cleanup();
    
    iMcc->iRefCount = 0;
    iStream->Cleanup();
    
    }

void UT_CMceSrvStream::UT_CMceSrvStream_MergeL()
    {
    CMceSrvStream* stream = iSession->MccStreams()[1];
    EUNIT_ASSERT( !stream->IsMerged() );
    EUNIT_ASSERT( !iStream->IsMerged() );
    iStream->Merge( *stream );
    
    EUNIT_ASSERT( stream->IsMerged() );
    EUNIT_ASSERT( iStream->IsMerged() );
    
    }

void UT_CMceSrvStream::UT_CMceSrvStream_InvalidateL()
    {
    
    iStream->Invalidate();
    EUNIT_ASSERT( iStream->State() == CMceSrvStream::EStopped );
    
    }

void UT_CMceSrvStream::UT_CMceSrvStream_PrepareLL()
    {
    
    EUNIT_ASSERT_LEAVE( iStream->PrepareL() );

    iStream->Source().SetState( CMceSrvStream::ELinkCreated );
    iStream->Sink().SetState( CMceSrvStream::ELinkCreated );

    EUNIT_ASSERT( !iStream->PrepareL() );
    
    CMceSrvStream* stream = iSession->MccStreams()[4];
    stream->Source().SetState( CMceSrvStream::ELinkCreated );
    stream->Sink().SetState( CMceSrvStream::ELinkCreated );
    
    EUNIT_ASSERT( stream->PrepareL() );
    
    stream->Invalidate();
    EUNIT_ASSERT_LEAVE( stream->PrepareL() );

    
    }

void UT_CMceSrvStream::UT_CMceSrvStream_StartLL()
    {

    EUNIT_ASSERT( iStream->StartL() );
    
    iStream->Data().Enable( EFalse, ETrue );
    EUNIT_ASSERT( iStream->StartL() );
    
    iStream->Id() = 200;
    EUNIT_ASSERT( !iStream->StartL() );
    
    
    }

void UT_CMceSrvStream::UT_CMceSrvStream_SynchronizeLL()
    {

    EUNIT_ASSERT( iStream->SynchronizeL() );
    
    iStream->Data().Enable( EFalse, ETrue );
    EUNIT_ASSERT( iStream->SynchronizeL() );
    
    iStream->Id() = 200;
    EUNIT_ASSERT( !iStream->SynchronizeL() );

    }

void UT_CMceSrvStream::UT_CMceSrvStream_StopL()
    {
    EUNIT_ASSERT( iStream->Source().Data().IsEnabled() );
    EUNIT_ASSERT( iStream->Sink().Data().IsEnabled() );
    
    EUNIT_ASSERT( iStream->Stop() );
    
    EUNIT_ASSERT( !iStream->Source().Data().IsEnabled() );
    EUNIT_ASSERT( !iStream->Sink().Data().IsEnabled() );
    
    // Stop local video stream, source cannot be stopped as it is used by
    // other streams
    CMceSrvStream* localVideoStream = iSession->MccStreams()[3];
    EUNIT_ASSERT( localVideoStream->Source().Data().IsEnabled() );
    EUNIT_ASSERT( localVideoStream->Sink().Data().IsEnabled() );
    
    EUNIT_ASSERT( localVideoStream->Stop() );
    
    EUNIT_ASSERT( localVideoStream->Source().Data().IsEnabled() );
    EUNIT_ASSERT( !localVideoStream->Sink().Data().IsEnabled() );
    
    // Mcc prepared stream
    iStream->Id() = 200;
    EUNIT_ASSERT( !iStream->Stop() );

    }

void UT_CMceSrvStream::UT_CMceSrvStream_ValidateL()
    {

    EUNIT_ASSERT( !iStream->Validate() );
    
    CMceSrvStream* stream = iSession->MccStreams()[4];

    EUNIT_ASSERT( stream->Validate() );
    
    }

void UT_CMceSrvStream::UT_CMceSrvStream_UnMergeL()
    {
    EUNIT_ASSERT( !iStream->IsMerged() );
    iStream->iMerged = ETrue;
    EUNIT_ASSERT( iStream->IsMerged() );
    
    iStream->UnMerge();
    
    EUNIT_ASSERT( !iStream->IsMerged() );
    
    }

void UT_CMceSrvStream::UT_CMceSrvStream_RequireSignallingL()
    {
    CMceSrvStream* stream = iSession->MccStreams()[4];
    CMccCodecInformation* current = iManager->CreateMccCodecLC( *stream, EMceRoleOfferer );
    CMccCodecInformation* update = iManager->CreateMccCodecLC( *stream, EMceRoleOfferer );
    iStream->Data().SetLocalMediaPort( stream->LocalMediaPort() );
    iStream->Data().SetRemoteMediaPort( stream->Data().RemoteMediaPort() );

    iMcc->iRequireSignalling = EFalse;
    EUNIT_ASSERT( !iStream->RequireSignalling( *stream, *current, *update ) );
    
    iMcc->iRequireSignalling = ETrue;
    EUNIT_ASSERT( iStream->RequireSignalling( *stream, *current, *update ) );
    
    iMcc->iRequireSignalling = EFalse;
    iStream->Data().SetLocalMediaPort( 10 );
    EUNIT_ASSERT( iStream->RequireSignalling( *stream, *current, *update ) );

    iMcc->iRequireSignalling = EFalse;
    iStream->Data().SetLocalMediaPort( stream->LocalMediaPort() );
    iStream->Data().SetRemoteMediaPort( 10 );
    EUNIT_ASSERT( iStream->RequireSignalling( *stream, *current, *update ) == KMceRequiresSipSignallingOnly );

    iMcc->iRequireSignalling = EFalse;
    iStream->Data().SetLocalMediaPort( stream->LocalMediaPort() );
    iStream->Data().SetRemoteMediaPort( stream->Data().RemoteMediaPort() );
    iStream->Data().Enable( EFalse, EFalse );
    EUNIT_ASSERT( iStream->RequireSignalling( *stream, *current, *update ) );
    
    
    CleanupStack::PopAndDestroy( update );
    CleanupStack::PopAndDestroy( current );
    
    }

void UT_CMceSrvStream::UT_CMceSrvStream_RequireSignalling_1L()
    {

    EUNIT_ASSERT( iStream->RequireSignalling( iSession->MccStreams() ) == KMceRequiresSipSignallingOnly );
    
    iStream->Id() = 10;
    EUNIT_ASSERT( iStream->RequireSignalling( iSession->MccStreams() ) == KMceRequiresSignalling );
    
    iStream = iSession->MccStreams()[4];//local
    EUNIT_ASSERT( iStream->RequireSignalling( iSession->MccStreams() ) == KMceNoSignalling );
    
    
    }

void UT_CMceSrvStream::UT_CMceSrvStream_UseDefaultStartupSequenceL()
    {
    
    EUNIT_ASSERT( iStream->UseDefaultStartupSequence() );

    iStream = iSession->MccStreams()[4];//local
    EUNIT_ASSERT( !iStream->UseDefaultStartupSequence() );
    
    iStream->Sink().Data().iType = 56;

    EUNIT_ASSERT( iStream->UseDefaultStartupSequence() );
    
    iStream->Source().Data().iType = 57;

    EUNIT_ASSERT( iStream->UseDefaultStartupSequence() );
    
    iStream->Sink().Data().iType = KMceDisplaySink;
    iStream->Source().Data().iType = KMceCameraSource;
    iSession->MccStreams()[3]->Data().iStreamType = CMceComMediaStream::EReceiveStream;
    
    EUNIT_ASSERT( iStream->UseDefaultStartupSequence() );
    
    
    }






	
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceSrvStream,
    "CMceSrvStream tests",
    "UNIT" )

EUNIT_TEST (
    "DecodeL test",
    "CMceSrvStream",
    "DecodeL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_DecodeLL, Teardown)

EUNIT_TEST (
    "EventReceived static test",
    "CMceSrvStream",
    "EventReceived",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_EventReceived_Static_L, Teardown)

EUNIT_TEST (
    "SessionId test",
    "CMceSrvStream",
    "SessionId",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_SessionIdL, Teardown)

EUNIT_TEST (
    "LinkId test",
    "CMceSrvStream",
    "LinkId",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_LinkIdL, Teardown)

EUNIT_TEST (
    "SetLinkId test",
    "CMceSrvStream",
    "SetLinkId",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_SetLinkIdL, Teardown)

EUNIT_TEST (
    "IapId test",
    "CMceSrvStream",
    "IapId",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_IapIdL, Teardown)

EUNIT_TEST (
    "LocalMediaPort test",
    "CMceSrvStream",
    "LocalMediaPort",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_LocalMediaPortL, Teardown)

EUNIT_TEST (
    "EventReceived test",
    "CMceSrvStream",
    "EventReceived",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_EventReceivedL, Teardown)

EUNIT_TEST (
    "Cleanup test",
    "CMceSrvStream",
    "Cleanup",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_CleanupL, Teardown)

EUNIT_TEST (
    "IsEqual test",
    "CMceSrvStream",
    "IsEqual",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_IsEqualL, Teardown)

EUNIT_TEST (
    "Merge test",
    "CMceSrvStream",
    "Merge",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_MergeL, Teardown)

EUNIT_TEST (
    "IsMccPrepared test",
    "CMceSrvStream",
    "IsMccPrepared",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_IsMccPreparedL, Teardown)

EUNIT_TEST (
    "State test",
    "CMceSrvStream",
    "State",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_StateL, Teardown)

EUNIT_TEST (
    "Invalidate test",
    "CMceSrvStream",
    "Invalidate",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_InvalidateL, Teardown)

EUNIT_TEST (
    "IsAdopted test",
    "CMceSrvStream",
    "IsAdopted",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_IsAdoptedL, Teardown)

EUNIT_TEST (
    "SetLocalMediaPort test",
    "CMceSrvStream",
    "SetLocalMediaPort",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_SetLocalMediaPortL, Teardown)

EUNIT_TEST (
    "Id test",
    "CMceSrvStream",
    "Id",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_IdL, Teardown)

EUNIT_TEST (
    "StreamType test",
    "CMceSrvStream",
    "StreamType",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_StreamTypeL, Teardown)

EUNIT_TEST (
    "Direction test",
    "CMceSrvStream",
    "Direction",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_DirectionL, Teardown)

EUNIT_TEST (
    "IsMerged test",
    "CMceSrvStream",
    "IsMerged",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_IsMergedL, Teardown)

EUNIT_TEST (
    "Data test",
    "CMceSrvStream",
    "Data",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_DataL, Teardown)

EUNIT_TEST (
    "RemoteIpAddress test",
    "CMceSrvStream",
    "RemoteIpAddress",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_RemoteIpAddressL, Teardown)

EUNIT_TEST (
    "Codec test",
    "CMceSrvStream",
    "Codec",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_CodecL, Teardown)

EUNIT_TEST (
    "Source test",
    "CMceSrvStream",
    "Source",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_SourceL, Teardown)

EUNIT_TEST (
    "Sink test",
    "CMceSrvStream",
    "Sink",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_SinkL, Teardown)

EUNIT_TEST (
    "MccStreamType test",
    "CMceSrvStream",
    "MccStreamType",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_MccStreamTypeL, Teardown)

EUNIT_TEST (
    "LinkType test",
    "CMceSrvStream",
    "LinkType",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_LinkTypeL, Teardown)

EUNIT_TEST (
    "Consumes test",
    "CMceSrvStream",
    "Consumes",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_ConsumesL, Teardown)

EUNIT_TEST (
    "PrepareL test",
    "CMceSrvStream",
    "PrepareL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_PrepareLL, Teardown)

EUNIT_TEST (
    "StartL test",
    "CMceSrvStream",
    "StartL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_StartLL, Teardown)

EUNIT_TEST (
    "SynchronizeL test",
    "CMceSrvStream",
    "SynchronizeL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_SynchronizeLL, Teardown)

EUNIT_TEST (
    "Stop test",
    "CMceSrvStream",
    "Stop",
    "FUNCTIONALITY",
    Setup2L, UT_CMceSrvStream_StopL, Teardown)

EUNIT_TEST (
    "Validate test",
    "CMceSrvStream",
    "Validate",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_ValidateL, Teardown)

EUNIT_TEST (
    "UnMerge test",
    "CMceSrvStream",
    "UnMerge",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_UnMergeL, Teardown)

EUNIT_TEST (
    "RequireSignalling test",
    "CMceSrvStream",
    "RequireSignalling",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_RequireSignallingL, Teardown)

EUNIT_TEST (
    "RequireSignalling 1 test",
    "CMceSrvStream",
    "RequireSignalling_1",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_RequireSignalling_1L, Teardown)

EUNIT_TEST (
    "MccPrepareCalledL test",
    "CMceSrvStream",
    "MccPrepareCalledL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_MccPrepareCalledLL, Teardown)

EUNIT_TEST (
    "UseDefaultStartupSequence test",
    "CMceSrvStream",
    "UseDefaultStartupSequence",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvStream_UseDefaultStartupSequenceL, Teardown)



EUNIT_END_TEST_TABLE


//  END OF FILE
