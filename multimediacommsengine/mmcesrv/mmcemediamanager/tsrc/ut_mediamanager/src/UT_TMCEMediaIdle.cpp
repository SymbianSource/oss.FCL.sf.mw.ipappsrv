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
#include "UT_tmcemediaidle.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <sdpdocument.h>
#include "mcemediamanager.h"
#include "CMccController_stub.h"
#define RESET()\
    iMcc->Reset();\
    iServer->Reset()
    
#include "mcemediaidle.h"
#define SETUP()\
    iSession->SetObserver( *iServer );\
    iManager->CreateSessionL( *iSession );\
    delete iSession->iNegotiationState;\
    iSession->iNegotiationState = NULL;\
    TMceMediaIdle::SetAsCurrentStateL( *iSession );\
    iState = iSession->iNegotiationState
    

//  INTERNAL INCLUDES

// CONSTRUCTION
UT_TMceMediaIdle* UT_TMceMediaIdle::NewL()
    {
    UT_TMceMediaIdle* self = UT_TMceMediaIdle::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_TMceMediaIdle* UT_TMceMediaIdle::NewLC()
    {
    UT_TMceMediaIdle* self = new( ELeave ) UT_TMceMediaIdle();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_TMceMediaIdle::~UT_TMceMediaIdle()
    {
    }

// Default constructor
UT_TMceMediaIdle::UT_TMceMediaIdle()
    {
    }

// Second phase construct
void UT_TMceMediaIdle::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_TMceMediaIdle::SetupL()
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

void UT_TMceMediaIdle::Teardown()
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


void UT_TMceMediaIdle::UT_TMceMediaIdle_BasicL()
    {
    
    EUNIT_ASSERT( iState->Id() == KMceMediaIdle );
    EUNIT_ASSERT( !iState->CallbackAllowed() );
    
    EUNIT_ASSERT( iState->StackIndex() == 0 );
    iState->Pushed();
    EUNIT_ASSERT( iState->StackIndex() == -1 );

    
    }

	
//
// Tests
//
// void UpdateL();
//
//
void UT_TMceMediaIdle::UT_TMceMediaIdle_UpdateLL()
    {

//offerer
    iState->UpdateL();
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMcePreparingOffererStreams );
    EUNIT_ASSERT( iSession->NegotiationState().CallbackAllowed() );
    
//answerer
	CMceComSession::Delete( iSession, *iManager );
    iSession = MceMediaManagerTestHelper::CreateInSessionL();
    SETUP();
    iState->SetRole( EMceRoleAnswerer );
    
    RESET();
    iState->UpdateL();
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceAnsweringMedia );
    EUNIT_ASSERT( !iSession->NegotiationState().CallbackAllowed() );
    
    
    }

//
// Tests
//
// TMceSipWarningCode DecodeL( CSdpDocument& aSdpDocument );
//
//
void UT_TMceMediaIdle::UT_TMceMediaIdle_DecodeLL()
    {
    TMceSipWarningCode status;
    
    status = iState->DecodeL( *iSdpDocument );
    EUNIT_ASSERT( status == KErrNone );
    EUNIT_ASSERT( &iSession->NegotiationState() == iState );
    EUNIT_ASSERT( !iState->CallbackAllowed() );
    
    }
	
	
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_TMceMediaIdle,
    "TMceMediaIdle tests",
    "UNIT" )


EUNIT_TEST(
    "Basic test",
    "TMceMediaIdle",
    "<basic>",
    "FUNCTIONALITY",
    SetupL, UT_TMceMediaIdle_BasicL, Teardown)

EUNIT_TEST(
    "UpdateL - test",
    "TMceMediaIdle",
    "UpdateL",
    "FUNCTIONALITY",
    SetupL, UT_TMceMediaIdle_UpdateLL, Teardown)

EUNIT_TEST(
    "DecodeL - test",
    "TMceMediaIdle",
    "DecodeL",
    "FUNCTIONALITY",
    SetupL, UT_TMceMediaIdle_DecodeLL, Teardown)
    


EUNIT_END_TEST_TABLE

//  END OF FILE
