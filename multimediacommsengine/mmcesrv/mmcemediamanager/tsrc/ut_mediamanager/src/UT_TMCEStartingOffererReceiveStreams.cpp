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
#include "UT_tmcestartingoffererreceivestreams.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <sdpdocument.h>
#include "mcemediamanager.h"
#include "mcesdpsession.h"
#include "CMccController_stub.h"
#define RESET()\
    iMcc->Reset();\
    iServer->Reset()
    
#include "mcestartingoffererreceivestreams.h"
#define SETUP()\
    iSession->SetObserver( *iServer );\
    iManager->CreateSessionL( *iSession );\
    delete iSession->iNegotiationState;\
    iSession->iNegotiationState = NULL;\
    TMceStartingOffererReceiveStreams::SetAsCurrentStateL( *iSession );\
    iState = static_cast<TMcePreparingStreams*>( iSession->iNegotiationState )


//  INTERNAL INCLUDES

// CONSTRUCTION
UT_TMceStartingOffererReceiveStreams* UT_TMceStartingOffererReceiveStreams::NewL()
    {
    UT_TMceStartingOffererReceiveStreams* self = UT_TMceStartingOffererReceiveStreams::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_TMceStartingOffererReceiveStreams* UT_TMceStartingOffererReceiveStreams::NewLC()
    {
    UT_TMceStartingOffererReceiveStreams* self = new( ELeave ) UT_TMceStartingOffererReceiveStreams();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_TMceStartingOffererReceiveStreams::~UT_TMceStartingOffererReceiveStreams()
    {
    }

// Default constructor
UT_TMceStartingOffererReceiveStreams::UT_TMceStartingOffererReceiveStreams()
    {
    }

// Second phase construct
void UT_TMceStartingOffererReceiveStreams::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_TMceStartingOffererReceiveStreams::SetupL()
    {
	CMccControllerStub::OpenL();
    iMcc = CMccControllerStub::Stub();
    iServer = new (ELeave) CMceServerStub();
    iManager = CMceMediaManager::NewL( *iServer );
    iSdpDocument = CSdpDocument::DecodeL( KMceMMTestSdpAnswerAudioIMS5PrecsSendrecv );
    
    iSession = MceMediaManagerTestHelper::CreateAudioOutSessionL();
    SETUP();
    RESET();
    } 

void UT_TMceStartingOffererReceiveStreams::Teardown()
    {
	delete iSdpDocument;
    iSdpDocument = NULL;
	CMceComSession::Delete( iSession, *iManager );
	delete iManager;
	iManager = NULL;
	delete iServer;
	iServer = NULL;
	CMccControllerStub::Close();
	iMcc = NULL;
    }


void UT_TMceStartingOffererReceiveStreams::UT_TMceStartingOffererReceiveStreams_BasicL()
    { 
    EUNIT_ASSERT( iState->Id() == KMceStartingOffererReceiveStreams );
    EUNIT_ASSERT( iState->CallbackAllowed() );
    
    EUNIT_ASSERT( iState->StackIndex() == 0 );
    iState->Pushed();
    EUNIT_ASSERT( iState->StackIndex() == -1 );
    }

//
// Tests
//
// void EntryL();
//
//
void UT_TMceStartingOffererReceiveStreams::UT_TMceStartingOffererReceiveStreams_EntryLL()
    {
    
    iState->EntryL();
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    }

//
// Tests
//
// void StreamsPreparedL();
//
//
void UT_TMceStartingOffererReceiveStreams::UT_TMceStartingOffererReceiveStreams_StreamsPreparedLL()
    {

    EUNIT_ASSERT_LEAVE( iState->StreamsPreparedL() );

    }

//
// Tests
//
// void StreamsStartedL();
//
//
void UT_TMceStartingOffererReceiveStreams::UT_TMceStartingOffererReceiveStreams_StreamsStartedLL()
    {

    iState->StreamsStartedL();
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceOfferingMedia );
    EUNIT_ASSERT( !iSession->NegotiationState().CallbackAllowed() );

    
    }

	
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_TMceStartingOffererReceiveStreams,
    "TMceStartingOffererReceiveStreams tests",
    "UNIT" )


EUNIT_TEST(
    "Basic test",
    "TMceStartingOffererReceiveStreams",
    "<basic>",
    "FUNCTIONALITY",
    SetupL, UT_TMceStartingOffererReceiveStreams_BasicL, Teardown)

EUNIT_TEST(
    "EntryL - test",
    "TMceStartingOffererReceiveStreams",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_TMceStartingOffererReceiveStreams_EntryLL, Teardown)

EUNIT_TEST(
    "StreamsPreparedL - test",
    "TMceStartingOffererReceiveStreams",
    "StreamsPreparedL",
    "FUNCTIONALITY",
    SetupL, UT_TMceStartingOffererReceiveStreams_StreamsPreparedLL, Teardown)

EUNIT_TEST(
    "StreamsStartedL - test",
    "TMceStartingOffererReceiveStreams",
    "StreamsStartedL",
    "FUNCTIONALITY",
    SetupL, UT_TMceStartingOffererReceiveStreams_StreamsStartedLL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
