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
#include "UT_tmcepreparinganswererstreams.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <sdpdocument.h>
#include "mcemediamanager.h"
#include "mcesdpsession.h"
#include "CMccController_stub.h"
#define RESET()\
    iMcc->Reset();\
    iServer->Reset()
    
#include "mcepreparinganswererstreams.h"
#define SETUP()\
    iSession->SetObserver( *iServer );\
    iManager->CreateSessionL( *iSession );\
    delete iSession->iNegotiationState;\
    iSession->iNegotiationState = NULL;\
    TMcePreparingAnswererStreams::SetAsCurrentStateL( *iSession );\
    iState = static_cast<TMcePreparingStreams*>( iSession->iNegotiationState )


//  INTERNAL INCLUDES

// CONSTRUCTION
UT_TMcePreparingAnswererStreams* UT_TMcePreparingAnswererStreams::NewL()
    {
    UT_TMcePreparingAnswererStreams* self = UT_TMcePreparingAnswererStreams::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_TMcePreparingAnswererStreams* UT_TMcePreparingAnswererStreams::NewLC()
    {
    UT_TMcePreparingAnswererStreams* self = new( ELeave ) UT_TMcePreparingAnswererStreams();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_TMcePreparingAnswererStreams::~UT_TMcePreparingAnswererStreams()
    {
    }

// Default constructor
UT_TMcePreparingAnswererStreams::UT_TMcePreparingAnswererStreams()
    {
    }

// Second phase construct
void UT_TMcePreparingAnswererStreams::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_TMcePreparingAnswererStreams::SetupL()
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

void UT_TMcePreparingAnswererStreams::Teardown()
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


void UT_TMcePreparingAnswererStreams::UT_TMcePreparingAnswererStreams_BasicL()
    { 
    EUNIT_ASSERT( iState->Id() == KMcePreparingAnswererStreams );
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
void UT_TMcePreparingAnswererStreams::UT_TMcePreparingAnswererStreams_EntryLL()
    {
    
    iState->EntryL();
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccCreateSession );
    
    
    }

//
// Tests
//
// void StreamsPreparedL();
//
//
void UT_TMcePreparingAnswererStreams::UT_TMcePreparingAnswererStreams_StreamsPreparedLL()
    {

    iState->StreamsPreparedL();
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    }

//
// Tests
//
// void StreamsStartedL();
//
//
void UT_TMcePreparingAnswererStreams::UT_TMcePreparingAnswererStreams_StreamsStartedLL()
    {

    EUNIT_ASSERT_LEAVE( iState->StreamsStartedL() );
    
    }

	
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_TMcePreparingAnswererStreams,
    "TMcePreparingAnswererStreams tests",
    "UNIT" )


EUNIT_TEST(
    "Basic test",
    "TMcePreparingAnswererStreams",
    "<basic>",
    "FUNCTIONALITY",
    SetupL, UT_TMcePreparingAnswererStreams_BasicL, Teardown)

EUNIT_TEST(
    "EntryL - test",
    "TMcePreparingAnswererStreams",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_TMcePreparingAnswererStreams_EntryLL, Teardown)

EUNIT_TEST(
    "StreamsPreparedL - test",
    "TMcePreparingAnswererStreams",
    "StreamsPreparedL",
    "FUNCTIONALITY",
    SetupL, UT_TMcePreparingAnswererStreams_StreamsPreparedLL, Teardown)

EUNIT_TEST(
    "StreamsStartedL - test",
    "TMcePreparingAnswererStreams",
    "StreamsStartedL",
    "FUNCTIONALITY",
    SetupL, UT_TMcePreparingAnswererStreams_StreamsStartedLL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
