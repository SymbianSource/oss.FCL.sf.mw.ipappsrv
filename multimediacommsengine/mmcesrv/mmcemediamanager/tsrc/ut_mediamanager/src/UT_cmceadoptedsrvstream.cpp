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
#include "UT_cmceadoptedsrvstream.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include "mcemediamanager.h"
#include "CMccController_stub.h"

#define RESET()\
    iMcc->Reset();\
    iServer->Reset()
    
#include "mceadoptedsrvstream.h"
#include "mcesrvsink.h"
#include "mcesrvsource.h"


//  INTERNAL INCLUDES

// CONSTRUCTION
UT_CMceAdoptedSrvStream* UT_CMceAdoptedSrvStream::NewL()
    {
    UT_CMceAdoptedSrvStream* self = UT_CMceAdoptedSrvStream::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceAdoptedSrvStream* UT_CMceAdoptedSrvStream::NewLC()
    {
    UT_CMceAdoptedSrvStream* self = new( ELeave ) UT_CMceAdoptedSrvStream();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceAdoptedSrvStream::~UT_CMceAdoptedSrvStream()
    {
    }

// Default constructor
UT_CMceAdoptedSrvStream::UT_CMceAdoptedSrvStream()
    {
    }

// Second phase construct
void UT_CMceAdoptedSrvStream::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_CMceAdoptedSrvStream::SetupL()
    {
	CMccControllerStub::OpenL();
    iMcc = CMccControllerStub::Stub();
    
    iServer = new (ELeave) CMceServerStub();
    iManager = CMceMediaManager::NewL( *iServer );
    iSession = MceMediaManagerTestHelper::CreateMultimediaOutSessionL();
    iSession->SetObserver( *iServer );
    iSession->PrepareL( *iManager );
    iSession->MccStreams()[0]->Id() = 10;
	iStream = CMceAdoptedSrvStream::NewL( *iSession->MccStreams()[0] );

    } 

void UT_CMceAdoptedSrvStream::Teardown()
    {
	CMceComSession::Delete( iSession, *iManager );
	delete iManager;
	iManager = NULL;
	delete iServer;
	iServer = NULL;
	CMccControllerStub::Close();
	iMcc = NULL;
    delete iStream;
    }


void UT_CMceAdoptedSrvStream::UT_CMceAdoptedSrvStream_IsAdoptedL()
    {
    EUNIT_ASSERT( iStream->IsAdopted() );
    }

void UT_CMceAdoptedSrvStream::UT_CMceAdoptedSrvStream_StateL()
    {
    EUNIT_ASSERT( iStream->State() == CMceSrvStream::EAdopted );
    
    iStream->iMerged = ETrue;
    
    EUNIT_ASSERT( iStream->State() == CMceSrvStream::EStopped );
    }

void UT_CMceAdoptedSrvStream::UT_CMceAdoptedSrvStream_IsEqualL()
    {
    CMceAdoptedSrvStream* stream = NULL;

    EUNIT_ASSERT( !iStream->IsEqual( *stream ) );

    EUNIT_ASSERT_LEAVE( stream = CMceAdoptedSrvStream::NewL( *iStream ) );
    EUNIT_ASSERT_LEAVE( stream = CMceAdoptedSrvStream::NewL( *iSession->MccStreams()[1] ) );
    
    iSession->MccStreams()[1]->Id() = 10;
    stream = CMceAdoptedSrvStream::NewL( *iSession->MccStreams()[1] );
    CleanupStack::PushL( stream );


    EUNIT_ASSERT( !iStream->IsEqual( *stream ) );
    iStream->iCodecId = stream->iCodecId;
    iStream->iSourceId = stream->iSourceId;
    iStream->iSinkId = stream->iSinkId;

    EUNIT_ASSERT( iStream->IsEqual( *stream ) );

    iStream->iSinkId = 10;
    EUNIT_ASSERT( !iStream->IsEqual( *stream ) );
    
    iStream->iSourceId = 10;
    EUNIT_ASSERT( !iStream->IsEqual( *stream ) );

    iStream->iCodecId = 10;
    EUNIT_ASSERT( !iStream->IsEqual( *stream ) );
    
    CleanupStack::PopAndDestroy( stream );
    
    CMceSrvStream* stream1 = iSession->MccStreams()[1];
    
    iStream->iCodecId = stream1->Codec().Id();
    iStream->iSourceId = stream1->Source().Data().Id();
    iStream->iSinkId = stream1->Sink().Data().Id();
    
    EUNIT_ASSERT( iStream->IsEqual( *stream1 ) );
    iStream->iSinkId = 10;
    EUNIT_ASSERT( !iStream->IsEqual( *stream1 ) );
    
    iStream->iSourceId = 10;
    EUNIT_ASSERT( !iStream->IsEqual( *stream1 ) );

    iStream->iCodecId = 10;
    EUNIT_ASSERT( !iStream->IsEqual( *stream1 ) );
    
    
    }

void UT_CMceAdoptedSrvStream::UT_CMceAdoptedSrvStream_IsMccPreparedL()
    {
    EUNIT_ASSERT( iStream->IsMccPrepared() );
    
    }

void UT_CMceAdoptedSrvStream::UT_CMceAdoptedSrvStream_InvalidateL()
    {
    iStream->Invalidate();
    EUNIT_ASSERT( iStream->IsMerged() );
    
    }

void UT_CMceAdoptedSrvStream::UT_CMceAdoptedSrvStream_SessionIdL()
    {
    
    EUNIT_ASSERT( iStream->SessionId() == iSession->MccStreams()[0]->SessionId() );
    
    }

void UT_CMceAdoptedSrvStream::UT_CMceAdoptedSrvStream_LinkIdL()
    {
    EUNIT_ASSERT( iStream->LinkId() == iSession->MccStreams()[0]->LinkId() );
    }

void UT_CMceAdoptedSrvStream::UT_CMceAdoptedSrvStream_SetLinkIdL()
    {
    iStream->SetLinkId( 10 );
    
    EUNIT_ASSERT( iStream->LinkId() == 10 );
    }

void UT_CMceAdoptedSrvStream::UT_CMceAdoptedSrvStream_IapIdL()
    {
    EUNIT_ASSERT( iStream->IapId() == iSession->MccStreams()[0]->IapId() );
    }

void UT_CMceAdoptedSrvStream::UT_CMceAdoptedSrvStream_LocalMediaPortL()
    {
    EUNIT_ASSERT( iStream->LocalMediaPort() == iSession->MccStreams()[0]->LocalMediaPort() );
    }

void UT_CMceAdoptedSrvStream::UT_CMceAdoptedSrvStream_CleanupL()
    {
    //Null testing
    iStream->Cleanup();
    
    iMcc->iRefCount = 0;
    iStream->Cleanup();
    
    }

	
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceAdoptedSrvStream,
    "CMceAdoptedSrvStream tests",
    "UNIT" )

EUNIT_TEST (
    "IsAdopted test",
    "CMceAdoptedSrvStream",
    "IsAdopted",
    "FUNCTIONALITY",
    SetupL, UT_CMceAdoptedSrvStream_IsAdoptedL, Teardown)

EUNIT_TEST (
    "State test",
    "CMceAdoptedSrvStream",
    "State",
    "FUNCTIONALITY",
    SetupL, UT_CMceAdoptedSrvStream_StateL, Teardown)

EUNIT_TEST (
    "IsEqual test",
    "CMceAdoptedSrvStream",
    "IsEqual",
    "FUNCTIONALITY",
    SetupL, UT_CMceAdoptedSrvStream_IsEqualL, Teardown)

EUNIT_TEST (
    "IsMccPrepared test",
    "CMceAdoptedSrvStream",
    "IsMccPrepared",
    "FUNCTIONALITY",
    SetupL, UT_CMceAdoptedSrvStream_IsMccPreparedL, Teardown)

EUNIT_TEST (
    "Invalidate test",
    "CMceAdoptedSrvStream",
    "Invalidate",
    "FUNCTIONALITY",
    SetupL, UT_CMceAdoptedSrvStream_InvalidateL, Teardown)

EUNIT_TEST (
    "SessionId test",
    "CMceAdoptedSrvStream",
    "SessionId",
    "FUNCTIONALITY",
    SetupL, UT_CMceAdoptedSrvStream_SessionIdL, Teardown)

EUNIT_TEST (
    "LinkId test",
    "CMceAdoptedSrvStream",
    "LinkId",
    "FUNCTIONALITY",
    SetupL, UT_CMceAdoptedSrvStream_LinkIdL, Teardown)

EUNIT_TEST (
    "SetLinkId test",
    "CMceAdoptedSrvStream",
    "SetLinkId",
    "FUNCTIONALITY",
    SetupL, UT_CMceAdoptedSrvStream_SetLinkIdL, Teardown)

EUNIT_TEST (
    "IapId test",
    "CMceAdoptedSrvStream",
    "IapId",
    "FUNCTIONALITY",
    SetupL, UT_CMceAdoptedSrvStream_IapIdL, Teardown)

EUNIT_TEST (
    "LocalMediaPort test",
    "CMceAdoptedSrvStream",
    "LocalMediaPort",
    "FUNCTIONALITY",
    SetupL, UT_CMceAdoptedSrvStream_LocalMediaPortL, Teardown)

EUNIT_TEST (
    "Cleanup test",
    "CMceAdoptedSrvStream",
    "Cleanup",
    "FUNCTIONALITY",
    SetupL, UT_CMceAdoptedSrvStream_CleanupL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
