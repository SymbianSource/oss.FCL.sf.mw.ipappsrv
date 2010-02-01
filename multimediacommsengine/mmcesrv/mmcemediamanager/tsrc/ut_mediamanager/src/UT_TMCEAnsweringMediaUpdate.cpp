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
#include "UT_tmceansweringmediaupdate.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <sdpdocument.h>
#include "mcemediamanager.h"
#include "mcesdpsession.h"
#include "CMccController_stub.h"
#define RESET()\
    iMcc->Reset();\
    iServer->Reset()
    
#include "mceansweringmediaupdate.h"
#define SETUP()\
    iSession->SetObserver( *iServer );\
    iManager->CreateSessionL( *iSession );\
    delete iSession->iNegotiationState;\
    iSession->iNegotiationState = NULL;\
    TMceAnsweringMediaUpdate::SetAsCurrentStateL( *iSession );\
    iState = iSession->iNegotiationState

_LIT8( KFmtpAttrib, "octet-aling=0; mode-set=0,2");


//  INTERNAL INCLUDES

// CONSTRUCTION
UT_TMceAnsweringMediaUpdate* UT_TMceAnsweringMediaUpdate::NewL()
    {
    UT_TMceAnsweringMediaUpdate* self = UT_TMceAnsweringMediaUpdate::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_TMceAnsweringMediaUpdate* UT_TMceAnsweringMediaUpdate::NewLC()
    {
    UT_TMceAnsweringMediaUpdate* self = new( ELeave ) UT_TMceAnsweringMediaUpdate();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_TMceAnsweringMediaUpdate::~UT_TMceAnsweringMediaUpdate()
    {
    }

// Default constructor
UT_TMceAnsweringMediaUpdate::UT_TMceAnsweringMediaUpdate()
    {
    }

// Second phase construct
void UT_TMceAnsweringMediaUpdate::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_TMceAnsweringMediaUpdate::SetupL()
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

void UT_TMceAnsweringMediaUpdate::Teardown()
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


void UT_TMceAnsweringMediaUpdate::UT_TMceAnsweringMediaUpdate_BasicL()
    { 
    EUNIT_ASSERT( iState->Id() == KMceAnsweringMediaUpdate );
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
void UT_TMceAnsweringMediaUpdate::UT_TMceAnsweringMediaUpdate_UpdateLL()
    {
    
    CMceComSession* backup = iSession->CloneL();
    CleanupStack::PushL( backup );
    
    backup->PrepareL( *iManager );
    iSession->SdpSession().iBackup = backup;
    
    //force to be simple validation
    iSession->Streams()[ 0 ]->iStreamType = CMceComMediaStream::ELocalStream;
    iSession->Streams()[ 0 ]->iLinkedStream->iStreamType = CMceComMediaStream::ELocalStream;
    backup->Streams()[ 0 ]->iStreamType = CMceComMediaStream::ELocalStream;
    backup->Streams()[ 0 ]->iLinkedStream->iStreamType = CMceComMediaStream::ELocalStream;

    iState->UpdateL();
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMcePreparingAnswererStreams );
    EUNIT_ASSERT( iSession->NegotiationState().CallbackAllowed() );
    
    CleanupStack::Pop( backup );
    iSession->SdpSession().iBackup = NULL;
    CMceComSession::Delete( backup, *iManager );
    
    }

	
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_TMceAnsweringMediaUpdate,
    "TMceAnsweringMediaUpdate tests",
    "UNIT" )


EUNIT_TEST(
    "Basic test",
    "TMceAnsweringMediaUpdate",
    "<basic>",
    "FUNCTIONALITY",
    SetupL, UT_TMceAnsweringMediaUpdate_BasicL, Teardown)

EUNIT_TEST(
    "UpdateL - test",
    "TMceAnsweringMediaUpdate",
    "UpdateL",
    "FUNCTIONALITY",
    SetupL, UT_TMceAnsweringMediaUpdate_UpdateLL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
