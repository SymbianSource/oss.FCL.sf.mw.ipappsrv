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
#include "UT_tmcepreparingoffererstreams.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <sdpdocument.h>
#include "mcemediamanager.h"
#include "mcesdpsession.h"
#include "CMccController_stub.h"
#define RESET()\
    iMcc->Reset();\
    iServer->Reset()
    
#include "mcepreparingoffererstreams.h"
#define SETUP()\
    iSession->SetObserver( *iServer );\
    iManager->CreateSessionL( *iSession );\
    delete iSession->iNegotiationState;\
    iSession->iNegotiationState = NULL;\
    TMcePreparingOffererStreams::SetAsCurrentStateL( *iSession );\
    iState = static_cast<TMcePreparingStreams*>( iSession->iNegotiationState )


//  INTERNAL INCLUDES

// CONSTRUCTION
UT_TMcePreparingOffererStreams* UT_TMcePreparingOffererStreams::NewL()
    {
    UT_TMcePreparingOffererStreams* self = UT_TMcePreparingOffererStreams::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_TMcePreparingOffererStreams* UT_TMcePreparingOffererStreams::NewLC()
    {
    UT_TMcePreparingOffererStreams* self = new( ELeave ) UT_TMcePreparingOffererStreams();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_TMcePreparingOffererStreams::~UT_TMcePreparingOffererStreams()
    {
    }

// Default constructor
UT_TMcePreparingOffererStreams::UT_TMcePreparingOffererStreams()
    {
    }

// Second phase construct
void UT_TMcePreparingOffererStreams::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_TMcePreparingOffererStreams::SetupL()
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

void UT_TMcePreparingOffererStreams::Teardown()
    {
	delete iSdpDocument;
    iSdpDocument = NULL;
	if ( iSession )
		{
		iManager->CloseSession( *iSession );
		iSession->iMccStreams.ResetAndDestroy();
        iSession->iMccAdoptedStreams.ResetAndDestroy();
        delete iSession->iNegotiationState;
        iSession->iNegotiationState = NULL;
		}
	delete iManager;
	iManager = NULL;
	if ( iSession )
		{
		delete iSession;
		iSession = NULL;
		}
	delete iServer;
	iServer = NULL;
	CMccControllerStub::Close();
	iMcc = NULL;
    }


void UT_TMcePreparingOffererStreams::UT_TMcePreparingOffererStreams_BasicL()
    { 
    EUNIT_ASSERT( iState->Id() == KMcePreparingOffererStreams );
    EUNIT_ASSERT( iState->CallbackAllowed() );
    
    EUNIT_ASSERT( iState->StackIndex() == 0 );
    iState->Pushed();
    EUNIT_ASSERT( iState->StackIndex() == -1 );
    }


//
// Mcc
//
// void EntryL();
//
//
void UT_TMcePreparingOffererStreams::UT_TMcePreparingOffererStreams_EntryLL()
    {
    
    iState->EntryL();
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::EMccCreateSession );
    
	CMceComSession::Delete( iSession, *iManager );
	iSession = MceMediaManagerTestHelper::CreateAudioOutSessionL();
    SETUP();
    iSession->SdpSession().iBackup = iSession;
	
    RESET();

    iState->EntryL();

    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    iSession->SdpSession().iBackup = NULL;
    }

//
// Mcc
//
// void StreamsPreparedL();
//
//
void UT_TMcePreparingOffererStreams::UT_TMcePreparingOffererStreams_StreamsPreparedLL()
    {

    iState->StreamsPreparedL();
    EUNIT_ASSERT( iMcc->iActions[ 0 ].iAction == CMccControllerStub::ENone );
    
    }

//
// Mcc
//
// void StreamsStartedL();
//
//
void UT_TMcePreparingOffererStreams::UT_TMcePreparingOffererStreams_StreamsStartedLL()
    {

    EUNIT_ASSERT_LEAVE( iState->StreamsStartedL() );
    
    }

	
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_TMcePreparingOffererStreams,
    "TMcePreparingOffererStreams tests",
    "UNIT" )


EUNIT_TEST(
    "Basic test",
    "TMcePreparingOffererStreams",
    "<basic>",
    "FUNCTIONALITY",
    SetupL, UT_TMcePreparingOffererStreams_BasicL, Teardown)

EUNIT_TEST(
    "EntryL - test",
    "TMcePreparingOffererStreams",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_TMcePreparingOffererStreams_EntryLL, Teardown)

EUNIT_TEST(
    "StreamsPreparedL - test",
    "TMcePreparingOffererStreams",
    "StreamsPreparedL",
    "FUNCTIONALITY",
    SetupL, UT_TMcePreparingOffererStreams_StreamsPreparedLL, Teardown)

EUNIT_TEST(
    "StreamsStartedL - test",
    "TMcePreparingOffererStreams",
    "StreamsStartedL",
    "FUNCTIONALITY",
    SetupL, UT_TMcePreparingOffererStreams_StreamsStartedLL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
