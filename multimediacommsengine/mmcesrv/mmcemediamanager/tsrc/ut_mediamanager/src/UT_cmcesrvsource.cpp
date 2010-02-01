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
#include "UT_cmcesrvsource.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include "mcemediamanager.h"
#include "CMccController_stub.h"
#include "mcecomavsink.h"

#define RESET()\
    iMcc->Reset();\
    iServer->Reset()
    
#include "mcesrvsource.h"
#include "mcecomrtpsource.h"


//  INTERNAL INCLUDES

// CONSTRUCTION
UT_CMceSrvSource* UT_CMceSrvSource::NewL()
    {
    UT_CMceSrvSource* self = UT_CMceSrvSource::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceSrvSource* UT_CMceSrvSource::NewLC()
    {
    UT_CMceSrvSource* self = new( ELeave ) UT_CMceSrvSource();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceSrvSource::~UT_CMceSrvSource()
    {
    }

// Default constructor
UT_CMceSrvSource::UT_CMceSrvSource()
    {
    }

// Second phase construct
void UT_CMceSrvSource::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_CMceSrvSource::SetupL()
    {
	CMccControllerStub::OpenL();
    iMcc = CMccControllerStub::Stub();
    
    iServer = new (ELeave) CMceServerStub();
    iManager = CMceMediaManager::NewL( *iServer );
    iSession = MceMediaManagerTestHelper::CreateMultimediaOutSessionL();
    iManager->CreateSessionL( *iSession );
    iSession->PrepareL( *iManager );
	iSource = &iSession->MccStreams()[0]->Source();
    
    RESET();
    } 

void UT_CMceSrvSource::Teardown()
    {
	CMceComSession::Delete( iSession, *iManager );
	delete iManager;
	iManager = NULL;
	delete iServer;
	iServer = NULL;
	CMccControllerStub::Close();
	iMcc = NULL;
    iSource = NULL;
    }


void UT_CMceSrvSource::UT_CMceSrvSource_IdL()
    {
    
    EUNIT_ASSERT( iSource->Id() == KMceNotAssigned );
    iSource->iID = 100;
    EUNIT_ASSERT( iSource->Id() == 100 );
    
    }
    
void UT_CMceSrvSource::UT_CMceSrvSource_DataL()
    {
    
    EUNIT_ASSERT( &iSource->Data() != NULL );
    EUNIT_ASSERT( &iSource->Data() == iSession->Streams()[0]->Source() );
    
    }
    
void UT_CMceSrvSource::UT_CMceSrvSource_EventReceivedL()
    {
    TMceMccComEvent event( KMceNotAssigned );

    event.iEvent = KMccStreamPrepared;
    iSource->EventReceived( event );
    EUNIT_ASSERT( iSource->State() == CMceSrvStream::EPrepared );

    event.iEvent = KMccStreamStarted;
    iSource->EventReceived( event );
    EUNIT_ASSERT( iSource->State() == CMceSrvStream::EStarted );

    event.iEvent = KMccStreamStopped;
    iSource->EventReceived( event );
    EUNIT_ASSERT( iSource->State() == CMceSrvStream::EStopped );
    
    event.iEvent = KMccStreamPaused;
    iSource->EventReceived( event );
    EUNIT_ASSERT( iSource->State() == CMceSrvStream::EPaused );

    event.iEvent = KMccStreamResumed;
    iSource->EventReceived( event );
    EUNIT_ASSERT( iSource->State() == CMceSrvStream::EStarted );
    
    event.iEvent = KMccStreamIdle;
    iSource->EventReceived( event );
    EUNIT_ASSERT( iSource->State() == CMceSrvStream::EStarted );

    event.iEvent = KMccMasterKeyStaled;
    iSource->EventReceived( event );
    EUNIT_ASSERT( iSource->State() == CMceSrvStream::EPaused );
    
    }
    
    
void UT_CMceSrvSource::UT_CMceSrvSource_TypeL()
    {
    
    EUNIT_ASSERT( iSource->Type() == KUidMccRtpSource );
    
    }
    
void UT_CMceSrvSource::UT_CMceSrvSource_MccPrepareCalledLL()
    {
    
    CMceSrvStream* stream = iSession->MccStreams()[0];
    iSource = &stream->Source();
    
    iSource->MccPrepareCalledL( *stream );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );

    RESET();
    
    stream = iSession->MccStreams()[1];
    iSource = &stream->Source();
    
    iSource->MccPrepareCalledL( *stream );
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    }
    
void UT_CMceSrvSource::UT_CMceSrvSource_SetStateL()
    {
    EUNIT_ASSERT( iSource->State() == CMceSrvStream::ECreated );
    iSource->SetState( CMceSrvStream::EStopped );
    EUNIT_ASSERT( iSource->State() == CMceSrvStream::EStopped );
    
    }
    
void UT_CMceSrvSource::UT_CMceSrvSource_StateL()
    {
    EUNIT_ASSERT( iSource->State() == CMceSrvStream::ECreated );
    iSource->SetState( CMceSrvStream::EStopped );
    EUNIT_ASSERT( iSource->State() == CMceSrvStream::EStopped );
    }
    
void UT_CMceSrvSource::UT_CMceSrvSource_MergeL()
    {

    EUNIT_ASSERT( iSource->iID == KMceNotAssigned );
    EUNIT_ASSERT( iSource->iState == CMceSrvStream::ECreated );

    CMceSrvSource* source = &iSession->MccStreams()[1]->Source();
    source->iID = 100;
    source->iState = CMceSrvStream::EStarted;
    
    iSource->Merge( *source );

    EUNIT_ASSERT( iSource->iID == source->iID );
    EUNIT_ASSERT( iSource->iState == source->iState );
    
    }
    
void UT_CMceSrvSource::UT_CMceSrvSource_EnableLL()
    {
    CMceSrvStream& stream = *iSession->MccStreams()[4];
	iSource = &stream.Source(); //camera
    
    EUNIT_ASSERT( iSource->State() == CMceSrvStream::ECreated );
    EUNIT_ASSERT( iSource->EnableL( stream, ETrue ) );
    EUNIT_ASSERT( iSource->State() == CMceSrvStream::ECreated );


    iSource->SetState( CMceSrvStream::EStarted );
    EUNIT_ASSERT( iSource->EnableL( stream, ETrue ) );
    EUNIT_ASSERT( iSource->State() == CMceSrvStream::EStarted );
    
    iSource->SetState( CMceSrvStream::EPaused );
    EUNIT_ASSERT( iSource->EnableL( stream, ETrue ) );
    EUNIT_ASSERT( iSource->State() == CMceSrvStream::EStarted );
    
    iSource->SetState( CMceSrvStream::EPaused );
    iSource->Data().Enabled( EFalse );
    EUNIT_ASSERT( iSource->EnableL( stream, EFalse ) );
    EUNIT_ASSERT( iSource->State() == CMceSrvStream::EStarted );

    iSource->SetState( CMceSrvStream::EPaused );
    iSource->Data().Enabled( ETrue );
    EUNIT_ASSERT( iSource->EnableL( stream, EFalse ) );
    EUNIT_ASSERT( iSource->State() == CMceSrvStream::EStarted );

    iSource->SetState( CMceSrvStream::EStarted );
    
//mcc prepared    
    stream.Id() = 100;
    
    EUNIT_ASSERT( !iSource->EnableL( stream, ETrue ) );
    
    //faked situation
    CMceComAvSink* fake = CMceComAvSink::NewLC();
    iSource->Data().iEndpointProxy = fake;
    
    EUNIT_ASSERT( !iSource->EnableL( stream, ETrue ) );
    
    CleanupStack::PopAndDestroy( fake );
    iSource->Data().iEndpointProxy = NULL;

    }
    
void UT_CMceSrvSource::UT_CMceSrvSource_DisableLL()
    {
    
    CMceSrvStream& stream = *iSession->MccStreams()[4];
	iSource = &stream.Source(); //camera
    
    EUNIT_ASSERT( iSource->State() == CMceSrvStream::ECreated );
    EUNIT_ASSERT( iSource->DisableL( stream, ETrue ) );
    EUNIT_ASSERT( iSource->State() == CMceSrvStream::ECreated );


    iSource->SetState( CMceSrvStream::EStarted );
    EUNIT_ASSERT( iSource->DisableL( stream, ETrue ) );
    EUNIT_ASSERT( iSource->State() == CMceSrvStream::EPaused );
    
    iSource->SetState( CMceSrvStream::EPaused );
    EUNIT_ASSERT( iSource->DisableL( stream, ETrue ) );
    EUNIT_ASSERT( iSource->State() == CMceSrvStream::EPaused );
    
    iSource->SetState( CMceSrvStream::EPaused );
    iSource->Data().Enabled( EFalse );
    EUNIT_ASSERT( iSource->DisableL( stream, EFalse ) );
    EUNIT_ASSERT( iSource->State() == CMceSrvStream::EPaused );

    iSource->SetState( CMceSrvStream::EPaused );
    iSource->Data().Enabled( ETrue );
    EUNIT_ASSERT( iSource->DisableL( stream, EFalse ) );
    EUNIT_ASSERT( iSource->State() == CMceSrvStream::EPaused );

    iSource->SetState( CMceSrvStream::EStarted );
    
//mcc prepared    
    stream.Id() = 100;
    
    EUNIT_ASSERT( !iSource->DisableL( stream, ETrue ) );
    
    //faked situation
    CMceComAvSink* fake = CMceComAvSink::NewLC();
    iSource->Data().iEndpointProxy = fake;
    
    EUNIT_ASSERT( !iSource->DisableL( stream, ETrue ) );
    
    CleanupStack::PopAndDestroy( fake );
    iSource->Data().iEndpointProxy = NULL;
    
    iSource->DisableL( stream, ETrue );
    
    }
	
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceSrvSource,
    "CMceSrvSource tests",
    "UNIT" )


EUNIT_TEST(
    "Id test",
    "CMceSrvSource",
    "Id",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvSource_IdL, Teardown)

EUNIT_TEST(
    "Data test",
    "CMceSrvSource",
    "Data",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvSource_DataL, Teardown)

EUNIT_TEST(
    "EventReceived test",
    "CMceSrvSource",
    "EventReceived",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvSource_EventReceivedL, Teardown)

EUNIT_TEST(
    "Type test",
    "CMceSrvSource",
    "Type",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvSource_TypeL, Teardown)

EUNIT_TEST(
    "MccPrepareCalledL test",
    "CMceSrvSource",
    "MccPrepareCalledL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvSource_MccPrepareCalledLL, Teardown)

EUNIT_TEST(
    "SetState test",
    "CMceSrvSource",
    "SetState",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvSource_SetStateL, Teardown)
    
EUNIT_TEST(
    "State test",
    "CMceSrvSource",
    "State",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvSource_StateL, Teardown)

EUNIT_TEST(
    "Merge test",
    "CMceSrvSource",
    "Merge",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvSource_MergeL, Teardown)

EUNIT_TEST(
    "EnableL test",
    "CMceSrvSource",
    "EnableL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvSource_EnableLL, Teardown)
    
EUNIT_TEST(
    "DisableL test",
    "CMceSrvSource",
    "DisableL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSrvSource_DisableLL, Teardown)
    

EUNIT_END_TEST_TABLE

//  END OF FILE
