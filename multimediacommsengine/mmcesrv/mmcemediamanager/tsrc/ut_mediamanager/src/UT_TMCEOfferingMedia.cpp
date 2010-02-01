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
#include "UT_tmceofferingmedia.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <sdpdocument.h>
#include "mcemediamanager.h"
#include "CMccController_stub.h"
#include "mcesdpsession.h"
#include "cleanupresetanddestroy.h"

#define RESET()\
    iMcc->Reset();\
    iServer->Reset()
    
#include "mceofferingmedia.h"
#define SETUP_1()\
    iSession->SetObserver( *iServer );\
    iManager->CreateSessionL( *iSession );\
    delete iSession->iNegotiationState;\
    iSession->iNegotiationState = NULL;\
    iSession->SdpSession().StockMediaSessionL();\
    iSession->SdpSession().iInitialSession->Streams()[0]->SdpIndex() = 0

#define SETUP_2( sub )\
    TMceOfferingMedia::SetAsCurrentStateL( *iSession, sub );\
    iState = iSession->iNegotiationState

#define SETUP( sub )\
    SETUP_1();\
    SETUP_2( sub )
    


//  INTERNAL INCLUDES

// CONSTRUCTION
UT_TMceOfferingMedia* UT_TMceOfferingMedia::NewL()
    {
    UT_TMceOfferingMedia* self = UT_TMceOfferingMedia::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_TMceOfferingMedia* UT_TMceOfferingMedia::NewLC()
    {
    UT_TMceOfferingMedia* self = new( ELeave ) UT_TMceOfferingMedia();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_TMceOfferingMedia::~UT_TMceOfferingMedia()
    {
    }

// Default constructor
UT_TMceOfferingMedia::UT_TMceOfferingMedia()
    {
    }

// Second phase construct
void UT_TMceOfferingMedia::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_TMceOfferingMedia::SetupL()
    {
	CMccControllerStub::OpenL();
    iMcc = CMccControllerStub::Stub();
    iServer = new (ELeave) CMceServerStub();
    iManager = CMceMediaManager::NewL( *iServer );
    iSdpDocument = CSdpDocument::DecodeL( KMceMMTestSdpAnswerAudioIMS5PrecsSendrecv );
    
    iSession = MceMediaManagerTestHelper::CreateAudioOutSessionL();
    SETUP( ETrue );
    RESET();
    
    RPointerArray<CMceComAudioCodec> audioCapabilities;
    RPointerArray<CMceComVideoCodec> videoCapabilities;
    
    MceCleanupResetAndDestroyPushL( audioCapabilities );
    MceCleanupResetAndDestroyPushL( videoCapabilities );
    
    iManager->GetCapabilitiesL( audioCapabilities );
    iManager->GetCapabilitiesL( videoCapabilities );
    
    CleanupStack::PopAndDestroy( 2 );
    
    } 

void UT_TMceOfferingMedia::Teardown()
    {
	delete iSdpDocument; 
    iSdpDocument = NULL;
	if( iSession )
        {
      	iManager->CloseSession( *iSession );
        }
	delete iServer;
	iServer = NULL;
	delete iManager;
	iManager = NULL;
	if ( iSession )
		{
		iSession->iMccStreams.ResetAndDestroy();
        iSession->iMccAdoptedStreams.ResetAndDestroy();
        delete iSession->iNegotiationState;
        iSession->iNegotiationState = NULL;
        
        delete iSession;
        iSession = NULL;
		}
	CMccControllerStub::Close();
	iMcc = NULL;
    }


void UT_TMceOfferingMedia::UT_TMceOfferingMedia_BasicL()
    {
    
    EUNIT_ASSERT( iState->Id() == KMceOfferingMedia );
    EUNIT_ASSERT( !iState->CallbackAllowed() );
    
    EUNIT_ASSERT( iState->StackIndex() == 0 );
    iState->Pushed();
    EUNIT_ASSERT( iState->StackIndex() == -1 );

//substate TMceOfferingMedia::TMceMediaReady
	CMceComSession::Delete( iSession, *iManager );
    iSession = MceMediaManagerTestHelper::CreateAudioOutSessionL();
    SETUP_1();
    iSession->SdpSession().iBackup = NULL;
    iSession->SdpSession().SignallingRequired() = KMceNoSignalling;
    SETUP_2( EFalse );
    RESET();
    iSession->SdpSession().iBackup = NULL;

    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceOfferingMedia );
    EUNIT_ASSERT( !iSession->NegotiationState().CallbackAllowed() );
    

//substate TMceOfferingMedia::TMceMediaUpdateReady
	CMceComSession::Delete( iSession, *iManager );
    iSession = MceMediaManagerTestHelper::CreateAudioOutSessionL();
    SETUP_1();
    iSession->SdpSession().iBackup = NULL;
    iSession->SdpSession().SignallingRequired() = KMceRequiresSignalling;
    SETUP_2( EFalse );
    RESET();
    iSession->SdpSession().iBackup = NULL;

    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceOfferingMedia );
    EUNIT_ASSERT( !iSession->NegotiationState().CallbackAllowed() );

//substate TMceStartingOffererSendStreams
	CMceComSession::Delete( iSession, *iManager );
    iSession = MceMediaManagerTestHelper::CreateAudioOutSessionL();
    SETUP_1();
    iSession->SdpSession().iBackup = iSession;
    iSession->SdpSession().SignallingRequired() = KMceNoSignalling;
    SETUP_2( EFalse );
    RESET();
    iSession->SdpSession().iBackup = NULL;

    EUNIT_ASSERT( iSession->NegotiationState().Id() != KMceOfferingMedia );
    EUNIT_ASSERT( iSession->NegotiationState().CallbackAllowed() )

//substate TMceOfferingMedia::TMceMediaUpdateReady
	CMceComSession::Delete( iSession, *iManager );
    iSession = MceMediaManagerTestHelper::CreateAudioOutSessionL();
    SETUP_1();
    iSession->SdpSession().iBackup = iSession;
    iSession->SdpSession().SignallingRequired() = KMceRequiresSignalling;
    SETUP_2( EFalse );
    RESET();
    iSession->SdpSession().iBackup = NULL;

    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceOfferingMedia );
    EUNIT_ASSERT( !iSession->NegotiationState().CallbackAllowed() );
    
    }

	
//
// Tests
//
// void UpdateL();
//
//
void UT_TMceOfferingMedia::UT_TMceOfferingMedia_UpdateLL()
    {

    //just case where media is rejected
    iSession->PrepareL();
    iSession->PrepareL( *iManager );
    iSession->AnswerType() = KMceNegotiationAnswerTypeFinal;

//TMceOfferingMedia
    iState->UpdateL();
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceMediaNegotiated );
    EUNIT_ASSERT( !iSession->NegotiationState().CallbackAllowed() );
    
//substate TMceOfferingMedia::TMceMediaReady
	CMceComSession::Delete( iSession, *iManager );
    iSession = MceMediaManagerTestHelper::CreateAudioOutSessionL();
    iSession->AnswerType() = KMceNegotiationAnswerTypeFinal;
    SETUP_1();
    iSession->SdpSession().iBackup = NULL;
    iSession->SdpSession().SignallingRequired() = KMceNoSignalling;
    SETUP_2( EFalse );
    RESET();
    iSession->SdpSession().iBackup = NULL;

    EUNIT_ASSERT_LEAVE( iState->UpdateL() );
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceOfferingMedia );
    EUNIT_ASSERT( !iSession->NegotiationState().CallbackAllowed() );
    

//substate TMceOfferingMedia::TMceMediaUpdateReady
	CMceComSession::Delete( iSession, *iManager );
    iSession = MceMediaManagerTestHelper::CreateAudioOutSessionL();
    iSession->AnswerType() = KMceNegotiationAnswerTypeFinal;
    SETUP_1();
    iSession->SdpSession().iBackup = NULL;
    iSession->SdpSession().SignallingRequired() = KMceRequiresSignalling;
    SETUP_2( EFalse );
    RESET();
    iSession->SdpSession().iBackup = NULL;

    EUNIT_ASSERT_LEAVE( iState->UpdateL() );
    
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceOfferingMedia );
    EUNIT_ASSERT( !iSession->NegotiationState().CallbackAllowed() );

//substate TMceOfferingMedia::TMceWaitingAnswer
    TMceOfferingMedia::TMceWaitingAnswer::SetAsCurrentStateL( *iSession );
    iState = iSession->iNegotiationState;

    EUNIT_ASSERT_LEAVE( iState->UpdateL() );
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceOfferingMedia );
    EUNIT_ASSERT( !iSession->NegotiationState().CallbackAllowed() );
    
    }

//
// Tests
//
// TMceSipWarningCode DecodeL( CSdpDocument& aSdpDocument );
//
//
void UT_TMceOfferingMedia::UT_TMceOfferingMedia_DecodeLL()
    {
    TMceSipWarningCode status;
    CSIPDialogAssocBase* a = (CSIPDialogAssocBase*)1;
    CSIPDialogAssocBase* b = (CSIPDialogAssocBase*)2;
    iSession->AnswerType() = KMceNegotiationAnswerTypeFinal;
    
    
//no substate

    EUNIT_ASSERT_LEAVE( iState->DecodeL( *iSdpDocument ) );
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceOfferingMedia );
    EUNIT_ASSERT( !iSession->NegotiationState().CallbackAllowed() );

//substate TMceOfferingMedia::TMceMediaReady
	CMceComSession::Delete( iSession, *iManager );
    iSession = MceMediaManagerTestHelper::CreateAudioOutSessionL();
    iSession->AnswerType() = KMceNegotiationAnswerTypeFinal;
    SETUP_1();
    iSession->SdpSession().iBackup = NULL;
    iSession->SdpSession().SignallingRequired() = KMceNoSignalling;
    SETUP_2( EFalse );
    RESET();
    iSession->SdpSession().iBackup = NULL;

    EUNIT_ASSERT_LEAVE( iState->DecodeL( *iSdpDocument ) );
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceOfferingMedia );
    EUNIT_ASSERT( !iSession->NegotiationState().CallbackAllowed() );
    

//substate TMceOfferingMedia::TMceMediaUpdateReady
	CMceComSession::Delete( iSession, *iManager );
    iSession = MceMediaManagerTestHelper::CreateAudioOutSessionL();
    iSession->AnswerType() = KMceNegotiationAnswerTypeFinal;
    SETUP_1();
    iSession->SdpSession().iBackup = NULL;
    iSession->SdpSession().SignallingRequired() = KMceRequiresSignalling;
    SETUP_2( EFalse );
    RESET();
    iSession->SdpSession().iBackup = NULL;

    EUNIT_ASSERT_LEAVE( iState->DecodeL( *iSdpDocument ) );
    
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceOfferingMedia );
    EUNIT_ASSERT( !iSession->NegotiationState().CallbackAllowed() );


//substate TMceOfferingMedia::TMceWaitingAnswer
    TMceOfferingMedia::TMceWaitingAnswer::SetAsCurrentStateL( *iSession );
    iState = iSession->iNegotiationState;
    
    delete iSdpDocument;
    iSdpDocument = NULL;
    iSdpDocument = CSdpDocument::DecodeL( KMceMMTestSdpAnswerAudioIMS5PrecsSendrecv );
    iSession->Streams()[0]->iSdpIndex = 0;
    status = iState->DecodeL( *iSdpDocument );
    
    EUNIT_ASSERT( status == KErrNone );        
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceOfferingMedia );
    EUNIT_ASSERT( !iSession->NegotiationState().CallbackAllowed() );

//substate TMceOfferingMedia::TMceWaitingAnswer
    TMceOfferingMedia::TMceWaitingAnswer::SetAsCurrentStateL( *iSession );
    iState = iSession->iNegotiationState;
    iSession->SdpSession().AttachContext( a );
    iSession->SdpSession().AttachContext( b );
    
    delete iSdpDocument;
    iSdpDocument = NULL;
    iSdpDocument = CSdpDocument::DecodeL( KMceMMTestSdpAnswerAudioIMS5PrecsSendrecv );
    iSession->Streams()[0]->iSdpIndex = 0;
    status = iState->DecodeL( *iSdpDocument );
    EUNIT_ASSERT( status == KErrNone );        
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceOfferingMedia );
    EUNIT_ASSERT( !iSession->NegotiationState().CallbackAllowed() );

    
    }

//
// Tests
//
// CSdpDocument* EncodeL();
//
//
void UT_TMceOfferingMedia::UT_TMceOfferingMedia_EncodeLL()
    {

    delete iSdpDocument;
    iSdpDocument = NULL;
    
//no substate

    EUNIT_ASSERT_LEAVE( iState->EncodeL() );
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceOfferingMedia );
    EUNIT_ASSERT( !iSession->NegotiationState().CallbackAllowed() );

//substate TMceOfferingMedia::TMceMediaReady
	CMceComSession::Delete( iSession, *iManager );
    iSession = MceMediaManagerTestHelper::CreateAudioOutSessionL();
    SETUP_1();
    iSession->SdpSession().iBackup = NULL;
    iSession->SdpSession().SignallingRequired() = KMceNoSignalling;
    SETUP_2( EFalse );
    RESET();
    iSession->SdpSession().iBackup = NULL;

    iSdpDocument = iState->EncodeL();
    
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceOfferingMedia );
    EUNIT_ASSERT( !iSession->NegotiationState().CallbackAllowed() );

//substate TMceOfferingMedia::TMceMediaUpdateReady
	CMceComSession::Delete( iSession, *iManager );
    iSession = MceMediaManagerTestHelper::CreateAudioOutSessionL();
    SETUP_1();
    iSession->SdpSession().iBackup = NULL;
    iSession->SdpSession().SignallingRequired() = KMceRequiresSignalling;
    SETUP_2( EFalse );
    RESET();
    iSession->SdpSession().iBackup = NULL;
    iSession->SdpSession().iSdpDocument = iSdpDocument;
    CSdpDocument* sdp = iState->EncodeL();
    EUNIT_ASSERT( sdp == iSdpDocument );

    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceOfferingMedia );
    EUNIT_ASSERT( !iSession->NegotiationState().CallbackAllowed() );

//substate TMceOfferingMedia::TMceWaitingAnswer
    TMceOfferingMedia::TMceWaitingAnswer::SetAsCurrentStateL( *iSession );
    iState = iSession->iNegotiationState;
    
    EUNIT_ASSERT_LEAVE( iState->EncodeL() );
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceOfferingMedia );
    EUNIT_ASSERT( !iSession->NegotiationState().CallbackAllowed() );

    iSession->SdpSession().iBackup = NULL;
    }


//
// Tests
//
// Callback();
//
//
void UT_TMceOfferingMedia::UT_TMceOfferingMedia_CallbackL()
    {
//no substate
    
    iState->Callback();
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
//substate TMceMediaReady
	CMceComSession::Delete( iSession, *iManager );
    iSession = MceMediaManagerTestHelper::CreateAudioOutSessionL();
    SETUP( EFalse );
    RESET();

    iState->Callback();
    EUNIT_ASSERT( iServer->iEvent == EMceMediaUpdated );
    EUNIT_ASSERT( iServer->iSession == iSession );
    
    }

	
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_TMceOfferingMedia,
    "TMceOfferingMedia tests",
    "UNIT" )


EUNIT_TEST(
    "Basic test",
    "TMceOfferingMedia",
    "<basic>",
    "FUNCTIONALITY",
    SetupL, UT_TMceOfferingMedia_BasicL, Teardown)

EUNIT_TEST(
    "UpdateL - test",
    "TMceOfferingMedia",
    "UpdateL",
    "FUNCTIONALITY",
    SetupL, UT_TMceOfferingMedia_UpdateLL, Teardown)

EUNIT_TEST(
    "DecodeL - test",
    "TMceOfferingMedia",
    "DecodeL",
    "FUNCTIONALITY",
    SetupL, UT_TMceOfferingMedia_DecodeLL, Teardown)

EUNIT_TEST(
    "EncodeL - test",
    "TMceOfferingMedia",
    "EncodeL",
    "FUNCTIONALITY",
    SetupL, UT_TMceOfferingMedia_EncodeLL, Teardown)

EUNIT_TEST(
    "Callback - test",
    "TMceOfferingMedia",
    "Callback",
    "FUNCTIONALITY",
    SetupL, UT_TMceOfferingMedia_CallbackL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
