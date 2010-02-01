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
#include "UT_cmcesrvsink.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include "mcemediamanager.h"
#include "CMccController_stub.h"

#define RESET()\
    iMcc->Reset();\
    iServer->Reset()
    
#include "mcesrvsink.h"
#include "mcecomrtpsink.h"


//  INTERNAL INCLUDES

// CONSTRUCTION
UT_CMceSrvSink* UT_CMceSrvSink::NewL()
    {
    UT_CMceSrvSink* self = UT_CMceSrvSink::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceSrvSink* UT_CMceSrvSink::NewLC()
    {
    UT_CMceSrvSink* self = new( ELeave ) UT_CMceSrvSink();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceSrvSink::~UT_CMceSrvSink()
    {
    }

// Default constructor
UT_CMceSrvSink::UT_CMceSrvSink()
    {
    }

// Second phase construct
void UT_CMceSrvSink::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_CMceSrvSink::SetupL()
    {
	CMccControllerStub::OpenL();
    iMcc = CMccControllerStub::Stub();
    
    iServer = new (ELeave) CMceServerStub();
    iManager = CMceMediaManager::NewL( *iServer );
    iSession = MceMediaManagerTestHelper::CreateMultimediaOutSessionL();
    iManager->CreateSessionL( *iSession );
    iSession->PrepareL( *iManager );
	iSink = &iSession->MccStreams()[0]->Sink();
    
    RESET();
    } 

void UT_CMceSrvSink::Teardown()
    {
	CMceComSession::Delete( iSession, *iManager );
	delete iManager;
	iManager = NULL;
	delete iServer;
	iServer = NULL;
	CMccControllerStub::Close();
	iMcc = NULL;
    iSink = NULL;
    }


void UT_CMceSrvSink::UT_CMceSrvSink_IdL()
    {
    
    EUNIT_ASSERT( iSink->Id() == KMceNotAssigned );
    iSink->iID = 100;
    EUNIT_ASSERT( iSink->Id() == 100 );
    
    }
    
void UT_CMceSrvSink::UT_CMceSrvSink_DataL()
    {
    
    EUNIT_ASSERT( &iSink->Data() != NULL );
    EUNIT_ASSERT( &iSink->Data() == iSession->Streams()[0]->Sinks()[0] );
    
    }
    
void UT_CMceSrvSink::UT_CMceSrvSink_EventReceivedL()
    {
    TMceMccComEvent event( KMceNotAssigned );

    event.iEvent = KMccStreamPrepared;
    iSink->EventReceived( event );
    EUNIT_ASSERT( iSink->State() == CMceSrvStream::EPrepared );

    event.iEvent = KMccStreamStarted;
    iSink->EventReceived( event );
    EUNIT_ASSERT( iSink->State() == CMceSrvStream::EStarted );

    event.iEvent = KMccStreamStopped;
    iSink->EventReceived( event );
    EUNIT_ASSERT( iSink->State() == CMceSrvStream::EStopped );
    
    event.iEvent = KMccStreamPaused;
    iSink->EventReceived( event );
    EUNIT_ASSERT( iSink->State() == CMceSrvStream::EPaused );

    event.iEvent = KMccStreamResumed;
    iSink->EventReceived( event );
    EUNIT_ASSERT( iSink->State() == CMceSrvStream::EStarted );
    
    event.iEvent = KMccStreamIdle;
    iSink->EventReceived( event );
    EUNIT_ASSERT( iSink->State() == CMceSrvStream::EStarted );
	
	event.iEvent = KMccMasterKeyStaled;
    iSink->EventReceived( event );
    EUNIT_ASSERT( iSink->State() == CMceSrvStream::EPaused );
    
    }
    
    
void UT_CMceSrvSink::UT_CMceSrvSink_TypeL()
    {
    
    EUNIT_ASSERT( iSink->Type() == KUidMmfAudioOutput );
    
    }
    
void UT_CMceSrvSink::UT_CMceSrvSink_MccPrepareCalledLL()
    {
    
    CMceSrvStream* stream = iSession->MccStreams()[0];
    iSink = &stream->Sink();
    
    iSink->MccPrepareCalledL( *stream );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );

    RESET();
    
    stream = iSession->MccStreams()[1];
    iSink = &stream->Sink();
    
    iSink->MccPrepareCalledL( *stream );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccGetSSRC );
    
    }
    
void UT_CMceSrvSink::UT_CMceSrvSink_SetStateL()
    {
    EUNIT_ASSERT( iSink->State() == CMceSrvStream::ECreated );
    iSink->SetState( CMceSrvStream::EStopped );
    EUNIT_ASSERT( iSink->State() == CMceSrvStream::EStopped );
    
    }
    
void UT_CMceSrvSink::UT_CMceSrvSink_StateL()
    {
    EUNIT_ASSERT( iSink->State() == CMceSrvStream::ECreated );
    iSink->SetState( CMceSrvStream::EStopped );
    EUNIT_ASSERT( iSink->State() == CMceSrvStream::EStopped );
    }
    
void UT_CMceSrvSink::UT_CMceSrvSink_MergeL()
    {

    EUNIT_ASSERT( iSink->iID == KMceNotAssigned );
    EUNIT_ASSERT( iSink->iState == CMceSrvStream::ECreated );

    CMceSrvSink* sink = &iSession->MccStreams()[1]->Sink();
    sink->iID = 100;
    sink->iState = CMceSrvStream::EStarted;
    
    iSink->Merge( *sink );

    EUNIT_ASSERT( iSink->iID == sink->iID );
    EUNIT_ASSERT( iSink->iState == sink->iState );
    
    }
    
void UT_CMceSrvSink::UT_CMceSrvSink_EnableLL()
    {
    CMceSrvStream& stream = *iSession->MccStreams()[4];
	iSink = &stream.Sink(); //display
    static_cast<CMceComDisplaySink&>( iSink->Data() ).SetViewFinder( ETrue );
    
    EUNIT_ASSERT( iSink->State() == CMceSrvStream::ECreated );
    EUNIT_ASSERT( iSink->EnableL( stream, ETrue ) );
    EUNIT_ASSERT( iSink->State() == CMceSrvStream::ECreated );


    iSink->SetState( CMceSrvStream::EStarted );
    EUNIT_ASSERT( iSink->EnableL( stream, ETrue ) );
    EUNIT_ASSERT( iSink->State() == CMceSrvStream::EStarted );
    
    iSink->SetState( CMceSrvStream::EPaused );
    EUNIT_ASSERT( iSink->EnableL( stream, ETrue ) );
    EUNIT_ASSERT( iSink->State() == CMceSrvStream::EStarted );
    
    iSink->SetState( CMceSrvStream::EPaused );
    iSink->Data().Enabled( EFalse );
    EUNIT_ASSERT( iSink->EnableL( stream, EFalse ) );
    EUNIT_ASSERT( iSink->State() == CMceSrvStream::EStarted );

    iSink->SetState( CMceSrvStream::EPaused );
    iSink->Data().Enabled( ETrue );
    EUNIT_ASSERT( iSink->EnableL( stream, EFalse ) );
    EUNIT_ASSERT( iSink->State() == CMceSrvStream::EStarted );

    iSink->SetState( CMceSrvStream::EStarted );
    
//mcc prepared    
    stream.Id() = 100;
    
    EUNIT_ASSERT( !iSink->EnableL( stream, ETrue ) );
    
    iSink->Data().SetupEndpointProxyL( NULL );
    EUNIT_ASSERT( !iSink->EnableL( stream, ETrue ) );
    
    }
    
void UT_CMceSrvSink::UT_CMceSrvSink_DisableLL()
    {
    
    CMceSrvStream& stream = *iSession->MccStreams()[4];
	iSink = &stream.Sink(); //display
    static_cast<CMceComDisplaySink&>( iSink->Data() ).SetViewFinder( ETrue );
    
    EUNIT_ASSERT( iSink->State() == CMceSrvStream::ECreated );
    EUNIT_ASSERT( iSink->DisableL( stream, ETrue ) );
    EUNIT_ASSERT( iSink->State() == CMceSrvStream::ECreated );


    iSink->SetState( CMceSrvStream::EStarted );
    EUNIT_ASSERT( iSink->DisableL( stream, ETrue ) );
    EUNIT_ASSERT( iSink->State() == CMceSrvStream::EPaused );
    
    iSink->SetState( CMceSrvStream::EPaused );
    EUNIT_ASSERT( iSink->DisableL( stream, ETrue ) );
    EUNIT_ASSERT( iSink->State() == CMceSrvStream::EPaused );
    
    iSink->SetState( CMceSrvStream::EPaused );
    iSink->Data().Enabled( EFalse );
    EUNIT_ASSERT( iSink->DisableL( stream, EFalse ) );
    EUNIT_ASSERT( iSink->State() == CMceSrvStream::EPaused );

    iSink->SetState( CMceSrvStream::EPaused );
    iSink->Data().Enabled( ETrue );
    EUNIT_ASSERT( iSink->DisableL( stream, EFalse ) );
    EUNIT_ASSERT( iSink->State() == CMceSrvStream::EPaused );

    // Forced disable
    RESET();
    iSink->SetState( CMceSrvStream::EPrepared );
    TMceMccComEvent event( KMceNotAssigned );
    event.iEvent = KMccStreamStarted;
    iSink->Data().SetState( event );
    EUNIT_ASSERT( iSink->DisableL( stream, EFalse, ETrue ) );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iKey == KMccCamLocalVideo );
    
    iSink->SetState( CMceSrvStream::EStarted );   
    
//mcc prepared    
    stream.Id() = 100;
    
    EUNIT_ASSERT( !iSink->DisableL( stream, ETrue ) );
    
    iSink->Data().SetupEndpointProxyL( NULL );
    EUNIT_ASSERT( !iSink->DisableL( stream, ETrue ) );
    
    }
	
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceSrvSink,
    "CMceSrvSink tests",
    "UNIT" )


EUNIT_TEST(
    "Id test",
    "CMceSrvSink",
    "Id",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvSink_IdL, Teardown)

EUNIT_TEST(
    "Data test",
    "CMceSrvSink",
    "Data",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvSink_DataL, Teardown)

EUNIT_TEST(
    "EventReceived test",
    "CMceSrvSink",
    "EventReceived",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvSink_EventReceivedL, Teardown)

EUNIT_TEST(
    "Type test",
    "CMceSrvSink",
    "Type",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvSink_TypeL, Teardown)

EUNIT_TEST(
    "MccPrepareCalledL test",
    "CMceSrvSink",
    "MccPrepareCalledL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvSink_MccPrepareCalledLL, Teardown)

EUNIT_TEST(
    "SetState test",
    "CMceSrvSink",
    "SetState",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvSink_SetStateL, Teardown)
    
EUNIT_TEST(
    "State test",
    "CMceSrvSink",
    "State",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvSink_StateL, Teardown)

EUNIT_TEST(
    "Merge test",
    "CMceSrvSink",
    "Merge",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvSink_MergeL, Teardown)

EUNIT_TEST(
    "EnableL test",
    "CMceSrvSink",
    "EnableL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvSink_EnableLL, Teardown)
    
EUNIT_TEST(
    "DisableL test",
    "CMceSrvSink",
    "DisableL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvSink_DisableLL, Teardown)
    

EUNIT_END_TEST_TABLE

//  END OF FILE
