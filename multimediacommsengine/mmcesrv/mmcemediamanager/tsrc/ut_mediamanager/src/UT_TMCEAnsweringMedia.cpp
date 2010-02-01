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
#include "UT_tmceansweringmedia.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <sdpdocument.h>
#include "mcemediamanager.h"
#include "CMccController_stub.h"
#define RESET()\
    iMcc->Reset();\
    iServer->Reset()
    
#include "mceansweringmedia.h"
#define SETUP()\
    iSession->SetObserver( *iServer );\
    iManager->CreateSessionL( *iSession );\
    delete iSession->iNegotiationState;\
    iSession->iNegotiationState = NULL;\
    TMceAnsweringMedia::SetAsCurrentStateL( *iSession );\
    iState = iSession->iNegotiationState

_LIT8( KFmtpAttrib, "octet-aling=0; mode-set=0,2");

//  INTERNAL INCLUDES

// CONSTRUCTION
UT_TMceAnsweringMedia* UT_TMceAnsweringMedia::NewL()
    {
    UT_TMceAnsweringMedia* self = UT_TMceAnsweringMedia::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_TMceAnsweringMedia* UT_TMceAnsweringMedia::NewLC()
    {
    UT_TMceAnsweringMedia* self = new( ELeave ) UT_TMceAnsweringMedia();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_TMceAnsweringMedia::~UT_TMceAnsweringMedia()
    {
    }

// Default constructor
UT_TMceAnsweringMedia::UT_TMceAnsweringMedia()
    {
    }

// Second phase construct
void UT_TMceAnsweringMedia::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_TMceAnsweringMedia::SetupL()
    {
	CMccControllerStub::OpenL();
    iMcc = CMccControllerStub::Stub();
    iServer = new (ELeave) CMceServerStub();
    iManager = CMceMediaManager::NewL( *iServer );
    iSdpDocument = CSdpDocument::DecodeL( KMceMMTestSdpAnswerAudioIMS5PrecsSendrecv );
    
    iSession = MceMediaManagerTestHelper::CreateAudioOutSessionL( NULL, KFmtpAttrib );
    SETUP();
    RESET();
    
    } 

void UT_TMceAnsweringMedia::Teardown()
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


void UT_TMceAnsweringMedia::UT_TMceAnsweringMedia_BasicL()
    { 
    EUNIT_ASSERT( iState->Id() == KMceAnsweringMedia );
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
void UT_TMceAnsweringMedia::UT_TMceAnsweringMedia_UpdateLL()
    {

    //force to be simple validation
    iSession->Streams()[ 0 ]->iStreamType = CMceComMediaStream::ELocalStream;
    iSession->Streams()[ 0 ]->iLinkedStream->iStreamType = CMceComMediaStream::ELocalStream;
    
    
    //to just test update
    iState->UpdateL();
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMcePreparingAnswererStreams );
    EUNIT_ASSERT( iSession->NegotiationState().CallbackAllowed() );
    
    
    }


	
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_TMceAnsweringMedia,
    "TMceAnsweringMedia tests",
    "UNIT" )


EUNIT_TEST(
    "Basic test",
    "TMceAnsweringMedia",
    "<basic>",
    "FUNCTIONALITY",
    SetupL, UT_TMceAnsweringMedia_BasicL, Teardown)

EUNIT_TEST(
    "UpdateL - test",
    "TMceAnsweringMedia",
    "UpdateL",
    "FUNCTIONALITY",
    SetupL, UT_TMceAnsweringMedia_UpdateLL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
