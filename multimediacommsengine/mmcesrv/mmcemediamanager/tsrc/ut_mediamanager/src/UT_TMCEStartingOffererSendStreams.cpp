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
#include "UT_TMCEStartingOffererSendStreams.h"
#include "mcestartingofferersendstreams.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <sdpdocument.h>
#include "mcemediamanager.h"
#include "mcesdpsession.h"
#include "mcemediastate.h"
#include "CMccController_stub.h"
#define RESET()\
    iMcc->Reset();\
    iServer->Reset()
    
//#include "UT_TMCEStartingOffererSendStreams.h"

#define SETUP()\
    iSession->SetObserver( *iServer );\
    iManager->CreateSessionL( *iSession );\
    delete iSession->iNegotiationState;\
    iSession->iNegotiationState = NULL;\
    TMceStartingOffererSendStreams::SetAsCurrentStateL( *iSession );\
    iState = static_cast<TMcePreparingStreams*>( iSession->iNegotiationState )


//  INTERNAL INCLUDES

// CONSTRUCTION
UT_TMceStartingOffererSendStreams* UT_TMceStartingOffererSendStreams::NewL()
    {
    UT_TMceStartingOffererSendStreams* self = UT_TMceStartingOffererSendStreams::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_TMceStartingOffererSendStreams* UT_TMceStartingOffererSendStreams::NewLC()
    {
    UT_TMceStartingOffererSendStreams* self = new( ELeave ) UT_TMceStartingOffererSendStreams();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_TMceStartingOffererSendStreams::~UT_TMceStartingOffererSendStreams()
    {
    }

// Default constructor
UT_TMceStartingOffererSendStreams::UT_TMceStartingOffererSendStreams()
    {
    }

// Second phase construct
void UT_TMceStartingOffererSendStreams::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_TMceStartingOffererSendStreams::SetupL()
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

void UT_TMceStartingOffererSendStreams::Teardown()
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


void UT_TMceStartingOffererSendStreams::UT_TMceStartingOffererSendStreams_BasicL()
    { 
    EUNIT_ASSERT( iState->Id() == KMceStartingOffererSendStreams );
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
void UT_TMceStartingOffererSendStreams::UT_TMceStartingOffererSendStreams_EntryLL()
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
void UT_TMceStartingOffererSendStreams::UT_TMceStartingOffererSendStreams_StreamsPreparedLL()
    {

    EUNIT_ASSERT_LEAVE( iState->StreamsPreparedL() );

    }

//
// Tests
//
// void StreamsStartedL();
//
//
void UT_TMceStartingOffererSendStreams::UT_TMceStartingOffererSendStreams_StreamsStartedLL()
    {

    iState->StreamsStartedL();
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceMediaNegotiated );
    EUNIT_ASSERT( !iSession->NegotiationState().CallbackAllowed() );

    
    }

	
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_TMceStartingOffererSendStreams,
    "TMceStartingOffererSendStreams tests",
    "UNIT" )


EUNIT_TEST(
    "Basic test",
    "TMceStartingOffererSendStreams",
    "<basic>",
    "FUNCTIONALITY",
    SetupL, UT_TMceStartingOffererSendStreams_BasicL, Teardown)

EUNIT_TEST(
    "EntryL - test",
    "TMceStartingOffererSendStreams",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_TMceStartingOffererSendStreams_EntryLL, Teardown)

EUNIT_TEST(
    "StreamsPreparedL - test",
    "TMceStartingOffererSendStreams",
    "StreamsPreparedL",
    "FUNCTIONALITY",
    SetupL, UT_TMceStartingOffererSendStreams_StreamsPreparedLL, Teardown)

EUNIT_TEST(
    "StreamsStartedL - test",
    "TMceStartingOffererSendStreams",
    "StreamsStartedL",
    "FUNCTIONALITY",
    SetupL, UT_TMceStartingOffererSendStreams_StreamsStartedLL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
