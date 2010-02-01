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
#include "UT_tmcemedianegotiated.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <sdpdocument.h>
#include <sdpmediafield.h>
#include "mcemediamanager.h"
#include "CMccController_stub.h"
#include "mcesdpsession.h"
#include "cleanupresetanddestroy.h"

#define RESET()\
    iMcc->Reset();\
    iServer->Reset()
    
#include "mcemedianegotiated.h"
#define SETUP( role )\
    iSession->SetObserver( *iServer );\
    iManager->CreateSessionL( *iSession );\
    delete iSession->iNegotiationState;\
    iSession->iNegotiationState = NULL;\
    if ( role != KRoleNone )\
        {\
        TMceMediaNegotiated::SetAsCurrentStateL( *iSession, role );\
        }\
    else\
        {\
        iSession->iNegotiationState = new (ELeave) TMceMediaNegotiated( *iSession );\
        }\
    iSession->SdpSession().iSdpDocument = iSdpDocument;\
    iSession->SdpSession().StoreRemoteOriginL();\
    iSession->SdpSession().StoreRemoteMediaFieldsL();\
    iState = iSession->iNegotiationState;\
    iSession->SdpSession().StockMediaSessionL();\
    iSession->SdpSession().iInitialSession->Streams()[0]->SdpIndex() = 0
    

const TMceNegotiationRole KRoleNone = (TMceNegotiationRole)-1;
_LIT8( KFmtpAttrib, "octet-aling=0; mode-set=0,2");

//  INTERNAL INCLUDES

// CONSTRUCTION
UT_TMceMediaNegotiated* UT_TMceMediaNegotiated::NewL()
    {
    UT_TMceMediaNegotiated* self = UT_TMceMediaNegotiated::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_TMceMediaNegotiated* UT_TMceMediaNegotiated::NewLC()
    {
    UT_TMceMediaNegotiated* self = new( ELeave ) UT_TMceMediaNegotiated();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_TMceMediaNegotiated::~UT_TMceMediaNegotiated()
    {
    }

// Default constructor
UT_TMceMediaNegotiated::UT_TMceMediaNegotiated()
    {
    }

// Second phase construct
void UT_TMceMediaNegotiated::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_TMceMediaNegotiated::SetupL()
    {
	CMccControllerStub::OpenL();
    iMcc = CMccControllerStub::Stub();
    iServer = new (ELeave) CMceServerStub();
    iManager = CMceMediaManager::NewL( *iServer );

    RPointerArray<CMceComAudioCodec> audioCapabilities;
    RPointerArray<CMceComVideoCodec> videoCapabilities;
    
    MceCleanupResetAndDestroyPushL( audioCapabilities );
    MceCleanupResetAndDestroyPushL( videoCapabilities );
    
    iManager->GetCapabilitiesL( audioCapabilities );
    iManager->GetCapabilitiesL( videoCapabilities );
    
    CleanupStack::PopAndDestroy( 2 );
    
    iSdpDocument = CSdpDocument::DecodeL( KMceMMTestSdpOfferAudioIMS5PrecsSendrecv );
    
    iSession = MceMediaManagerTestHelper::CreateAudioOutSessionL();
    SETUP( KRoleNone );
    RESET();
    } 

void UT_TMceMediaNegotiated::Teardown()
    {
	delete iSdpDocument; 
    iSdpDocument = NULL;
	CMceComSession::Delete( iBackup, *iManager );
	CMceComSession::Delete( iSession, *iManager );
	delete iServer;
	iServer = NULL;
	delete iManager;
	iManager = NULL;
	CMccControllerStub::Close();
	iMcc = NULL;
    }


void UT_TMceMediaNegotiated::UT_TMceMediaNegotiated_BasicL()
    { 
    EUNIT_ASSERT( iState->Id() == KMceMediaNegotiated );
    EUNIT_ASSERT( !iState->CallbackAllowed() );
    
    EUNIT_ASSERT( iState->StackIndex() == 0 );
    iState->Pushed();
    EUNIT_ASSERT( iState->StackIndex() == -1 );

//substate TMceEndState
	CMceComSession::Delete( iSession, *iManager );
    iSession = MceMediaManagerTestHelper::CreateAudioOutSessionL();
    SETUP( EMceRoleOfferer );
    RESET();

    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceMediaNegotiated );
    
//substate TMceAnswererEndState
	CMceComSession::Delete( iSession, *iManager );
    iSession = MceMediaManagerTestHelper::CreateAudioOutSessionL();
    SETUP( EMceRoleAnswerer );
    RESET();

    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceMediaNegotiated );
    
    }


//
// Tests
//
// void UpdateL();
//
//
void UT_TMceMediaNegotiated::UT_TMceMediaNegotiated_UpdateLL()
    {

//no substate
    
    iBackup = iSession->CloneL();
    iBackup->SetObserver( *iServer );
    iBackup->AttachSDPSessionL( iSession->SdpSession() );
    
    iSession->SdpSession().iBackup = iBackup;
    
    iState->UpdateL();
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMcePreparingOffererStreams );
    EUNIT_ASSERT( iSession->NegotiationState().CallbackAllowed() );

	CMceComSession::Delete( iBackup, *iManager );
	CMceComSession::Delete( iSession, *iManager );
    iSession = MceMediaManagerTestHelper::CreateAudioOutSessionL( NULL, KFmtpAttrib );
    SETUP( KRoleNone );
    iState->SetRole( EMceRoleAnswerer );
    RESET();

    iState->UpdateL();
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceAnsweringMediaUpdate );
    EUNIT_ASSERT( !iSession->NegotiationState().CallbackAllowed() );
    
        
//substate TMceEndState
	CMceComSession::Delete( iSession, *iManager );
    iSession = MceMediaManagerTestHelper::CreateAudioOutSessionL();
    SETUP( EMceRoleOfferer );
    RESET();
    
    iState->UpdateL();
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceOfferingMedia );
    EUNIT_ASSERT( !iSession->NegotiationState().CallbackAllowed() );
    
//substate TMceAnswererEndState
	CMceComSession::Delete( iSession, *iManager );
    iSession = MceMediaManagerTestHelper::CreateAudioOutSessionL();
    SETUP( EMceRoleAnswerer );
    RESET();

    iState->UpdateL();
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceOfferingMedia );
    EUNIT_ASSERT( !iSession->NegotiationState().CallbackAllowed() );
    
    
    
    }


//
// Tests
//
// TMceSipWarningCode DecodeL( CSdpDocument& aSdpDocument );
//
//
void UT_TMceMediaNegotiated::UT_TMceMediaNegotiated_DecodeLL()
    {
    TMceSipWarningCode status;
    CSIPDialogAssocBase* a = (CSIPDialogAssocBase*)1;
    CSIPDialogAssocBase* b = (CSIPDialogAssocBase*)2;

//no substate

	CMceComSession::Delete( iSession, *iManager );
	
    iSession = MceMediaManagerTestHelper::CreateAudioOutSessionL( NULL, KFmtpAttrib );
    SETUP( KRoleNone );
    RESET();
    
    status = iState->DecodeL( *iSdpDocument );
    EUNIT_ASSERT( status == KErrNone );
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceMediaNegotiated );
    EUNIT_ASSERT( !iSession->NegotiationState().CallbackAllowed() );
        
//substate TMceEndState
	CMceComSession::Delete( iSession, *iManager );
    iSession = MceMediaManagerTestHelper::CreateAudioOutSessionL();
    SETUP( EMceRoleOfferer );
    RESET();
    
    iSession->Streams()[0]->iSdpIndex = 0;
    status = iState->DecodeL( *iSdpDocument );
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceOfferingMedia );
    EUNIT_ASSERT( !iSession->NegotiationState().CallbackAllowed() );

//substate TMceEndState
	CMceComSession::Delete( iSession, *iManager );
    iSession = MceMediaManagerTestHelper::CreateAudioOutSessionL();
    SETUP( EMceRoleOfferer );
    RESET();
    
    iSession->Streams()[0]->iSdpIndex = 0;
    
    iSession->SdpSession().AttachContext( a );
    iSession->SdpSession().AttachContext( b );
    status = iState->DecodeL( *iSdpDocument );
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceMediaNegotiated );
    EUNIT_ASSERT( iServer->iStockedSession  != NULL );
    EUNIT_ASSERT( iServer->iStockedSession->NegotiationState().Id() == KMceOfferingMedia );
    EUNIT_ASSERT( !iSession->NegotiationState().CallbackAllowed() );

//substate TMceAnswererEndState
	CMceComSession::Delete( iSession, *iManager );
    iSession = MceMediaManagerTestHelper::CreateAudioOutSessionL();
    SETUP( EMceRoleAnswerer );
    RESET();

	// Before calling DecodeL, check that media line has non-zero port
	RPointerArray<CSdpMediaField>& mediaLines = iSdpDocument->MediaFields();
	EUNIT_ASSERT( mediaLines.Count() > 0 );
	CSdpMediaField* mediaLine = mediaLines[ 0 ];
	EUNIT_ASSERT( mediaLine->Port() > 0 );

	// DecodeL no longer leaves if the answer contains media line for which
	// a stream does not exist.
    EUNIT_ASSERT( iState->DecodeL( *iSdpDocument ) == KErrNone );
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceOfferingMedia );
    EUNIT_ASSERT( !iSession->NegotiationState().CallbackAllowed() );

	// Check that port in media line has been changed to zero    
	mediaLines = iSdpDocument->MediaFields();
	EUNIT_ASSERT( mediaLines.Count() > 0 );
	mediaLine = mediaLines[ 0 ];
	EUNIT_ASSERT( mediaLine->Port() == 0 );

    }

//
// Tests
//
// CSdpDocument* EncodeL();
//
//
void UT_TMceMediaNegotiated::UT_TMceMediaNegotiated_EncodeLL()
    {
    
//no substate

    EUNIT_ASSERT_LEAVE( iState->EncodeL() );
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceMediaNegotiated );
    EUNIT_ASSERT( !iSession->NegotiationState().CallbackAllowed() );

//substate TMceEndState
	CMceComSession::Delete( iSession, *iManager );
    iSession = MceMediaManagerTestHelper::CreateAudioOutSessionL();
    SETUP( EMceRoleOfferer );
    RESET();
    
    iState->EncodeL();
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceMediaNegotiated );
    EUNIT_ASSERT( !iSession->NegotiationState().CallbackAllowed() );

    
//substate TMceAnswererEndState
	CMceComSession::Delete( iSession, *iManager );
    iSession = MceMediaManagerTestHelper::CreateAudioOutSessionL();
    SETUP( EMceRoleAnswerer );
    RESET();

    iSession->SdpSession().iSdpDocument = iSdpDocument;
    CSdpDocument* sdp = iState->EncodeL();
    EUNIT_ASSERT( sdp == iSdpDocument );
    EUNIT_ASSERT( iSession->NegotiationState().Id() == KMceMediaNegotiated );
    EUNIT_ASSERT( !iSession->NegotiationState().CallbackAllowed() );
    
    }


//
// Tests
//
// Callback();
//
//
void UT_TMceMediaNegotiated::UT_TMceMediaNegotiated_CallbackL()
    {
//no substate
    
    iState->Callback();
    EUNIT_ASSERT( iServer->iEvent == KMMTestNoValue );
    EUNIT_ASSERT( iServer->iSession == NULL );
    
//substate TMceEndState
	CMceComSession::Delete( iSession, *iManager );
    iSession = MceMediaManagerTestHelper::CreateAudioOutSessionL();
    SETUP( EMceRoleOfferer );
    RESET();

    iState->Callback();
    EUNIT_ASSERT( iServer->iEvent == EMceMediaUpdated );
    EUNIT_ASSERT( iServer->iSession == iSession );

//substate TMceAnswererEndState
	CMceComSession::Delete( iSession, *iManager );
    iSession = MceMediaManagerTestHelper::CreateAudioOutSessionL();
    SETUP( EMceRoleAnswerer );
    RESET();
    iState->Callback();
    
    EUNIT_ASSERT( iServer->iEvent == EMceMediaUpdated );
    EUNIT_ASSERT( iServer->iSession == iSession );

    
    }
    
//
// Tests
//
// NeedToNegotiate( CSdpDocument* aSdpDocument );
//
//
void UT_TMceMediaNegotiated::UT_TMceMediaNegotiated_NeedToNegotiateL()
    {

//no substate
    
    EUNIT_ASSERT( !iState->NeedToNegotiate( NULL ) );
    EUNIT_ASSERT( !iState->NeedToNegotiate( iSdpDocument ) );
    
    
//substate TMceEndState
	CMceComSession::Delete( iSession, *iManager );
    iSession = MceMediaManagerTestHelper::CreateAudioOutSessionL();
    SETUP( EMceRoleOfferer );
    RESET();

    EUNIT_ASSERT( !iState->NeedToNegotiate( NULL ) );
    EUNIT_ASSERT( !iState->NeedToNegotiate( iSdpDocument ) );
    iSession->SdpSession().iBackup = iSession;
    EUNIT_ASSERT( !iState->NeedToNegotiate( NULL ) );
    iSession->SdpSession().iBackup = NULL;

//substate TMceAnswererEndState
	CMceComSession::Delete( iSession, *iManager );
    iSession = MceMediaManagerTestHelper::CreateAudioOutSessionL();
    SETUP( EMceRoleAnswerer );
    RESET();

    EUNIT_ASSERT( iState->NeedToNegotiate( NULL ) );
    EUNIT_ASSERT( iState->NeedToNegotiate( iSdpDocument ) );
    
    }

//
// Tests
//
// AnswerL();
//
//
void UT_TMceMediaNegotiated::UT_TMceMediaNegotiated_AnswerL()
    {
    CMceComSession::Delete( iSession, *iManager );
    iSession = MceMediaManagerTestHelper::CreateAudioOutSessionL( NULL, KFmtpAttrib );
    SETUP( KRoleNone );
    iState->SetRole( EMceRoleAnswerer );
    RESET();

    iState->UpdateL();
    EUNIT_ASSERT( iSession->MccStreams().Count() == 2 );
   
   CMceComSession::Delete( iSession, *iManager );
    iSession = MceMediaManagerTestHelper::CreateAudioOutSessionL( NULL, KFmtpAttrib );
    SETUP( KRoleNone );
    iState->SetRole( EMceRoleAnswerer );
    RESET();
	iSession->iCodecSelection = KMceCodecSelectionMerge;
    iState->UpdateL();
    EUNIT_ASSERT( iSession->MccStreams().Count() == 0 );
    }
	
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_TMceMediaNegotiated,
    "TMceMediaNegotiated tests",
    "UNIT" )


EUNIT_TEST(
    "Basic test",
    "TMceMediaNegotiated",
    "<basic>",
    "FUNCTIONALITY",
    SetupL, UT_TMceMediaNegotiated_BasicL, Teardown)

EUNIT_TEST(
    "UpdateL - test",
    "TMceMediaNegotiated",
    "UpdateL",
    "FUNCTIONALITY",
    SetupL, UT_TMceMediaNegotiated_UpdateLL, Teardown)

EUNIT_TEST(
    "DecodeL - test",
    "TMceMediaNegotiated",
    "DecodeL",
    "FUNCTIONALITY",
    SetupL, UT_TMceMediaNegotiated_DecodeLL, Teardown)

EUNIT_TEST(
    "EncodeL - test",
    "TMceMediaNegotiated",
    "EncodeL",
    "FUNCTIONALITY",
    SetupL, UT_TMceMediaNegotiated_EncodeLL, Teardown)

EUNIT_TEST(
    "Callback - test",
    "TMceMediaNegotiated",
    "Callback",
    "FUNCTIONALITY",
    SetupL, UT_TMceMediaNegotiated_CallbackL, Teardown)


EUNIT_TEST(
    "NeedToNegotiate - test",
    "TMceMediaNegotiated",
    "NeedToNegotiate",
    "FUNCTIONALITY",
    SetupL, UT_TMceMediaNegotiated_NeedToNegotiateL, Teardown)


EUNIT_TEST(
    "Answer - test",
    "TMceMediaNegotiated",
    "Answer",
    "FUNCTIONALITY",
    SetupL, UT_TMceMediaNegotiated_AnswerL, Teardown)




EUNIT_END_TEST_TABLE

//  END OF FILE
