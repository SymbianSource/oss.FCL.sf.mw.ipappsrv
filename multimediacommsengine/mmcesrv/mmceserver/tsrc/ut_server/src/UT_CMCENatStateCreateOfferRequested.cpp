/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
#include "ut_cmcenatstatecreateofferrequested.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "MCETestHelper.h"

//#include "mcesipsession.h"
#include "mcenatsipsession.h"
#include "mceservercore.h"
#include "mcecssession.h"
#include "mcecssessionimplementation.h"
#include "mcenatstatecreateofferrequested.h"
#include "mcenatsipstatemachine.h"


// CONSTRUCTION
UT_CMceNatStateCreateOfferRequested* UT_CMceNatStateCreateOfferRequested::NewL()
    {
    UT_CMceNatStateCreateOfferRequested* self =
    	UT_CMceNatStateCreateOfferRequested::NewLC();
    CleanupStack::Pop();
    return self;
    }

UT_CMceNatStateCreateOfferRequested* UT_CMceNatStateCreateOfferRequested::NewLC()
    {
    UT_CMceNatStateCreateOfferRequested* self =
    	new ( ELeave ) UT_CMceNatStateCreateOfferRequested();
    CleanupStack::PushL( self );
	self->ConstructL();
    return self;
    }

// Destructor (virtual by CBase)
UT_CMceNatStateCreateOfferRequested::~UT_CMceNatStateCreateOfferRequested()
    {
    }

// Default constructor
UT_CMceNatStateCreateOfferRequested::UT_CMceNatStateCreateOfferRequested()
    {
    }

// Second phase construct
void UT_CMceNatStateCreateOfferRequested::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CMceNatStateCreateOfferRequested::SetupL()
    {
    CMCETls::OpenL();
    SdpCodecStringPool::OpenL();

    iIds.iProfileID = 1;
    iIds.iManagerType = KMceCSSIPSession;
    iIds.iDialogType = KMceDlgTypeInvite;
    iServerCore = CMceServerCore::NewL();
    
    iStorage = CMCETls::Storage();
    
	CDesC8ArrayFlat* params = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
	CleanupStack::PushL( params );
	params->AppendL( _L8("User <user2@host>") );
	params->AppendL( _L8("User <user@host>") );
    
    CMceMsgTextArray* msgParams = new (ELeave) CMceMsgTextArray( *params );
    CleanupStack::PushL( msgParams );
    iStorage->StoreL( iIds, *msgParams );
    CleanupStack::PopAndDestroy( msgParams );
    CleanupStack::PopAndDestroy( params );

    iSession = CMceCsSession::NewL(*iServerCore);
    iSession->CreateSubSessionL(iIds, RMessage2());
    iSipSession = static_cast<CMceSipSession*>
    	((iSession->Implementation().SubSessions())[0]);    

	CMceSipManager& manager = iServerCore->Manager();
    CSIPProfile& profile = manager.ProfileL( iIds.iProfileID );

    iNatSession = CMceNatSipSession::NewL( *iSession,
		iSipSession->SIPConnection(),
		*iSipSession->iStateMachine,
		profile,
		NULL );
	iNatSession->iNatStateMachine->iCurrentState = KMceNatStateCreateOfferRequested;
    iState = iNatSession->iNatStateMachine->iStates[
    	iNatSession->iNatStateMachine->iCurrentState ];
    }

void UT_CMceNatStateCreateOfferRequested::Teardown()
    {
    CMCETls::Close();
	delete iSession;
	iSession = NULL;
	delete iNatSession;
	iNatSession = NULL;
    delete iServerCore;
    iServerCore = NULL;

    SdpCodecStringPool::Close();
    }

void UT_CMceNatStateCreateOfferRequested::
	UT_CMceNatStateCreateOfferRequested_AcceptLL()
    {
    EUNIT_ASSERT( DoesAccept( EMceNatInitialized ) );
    EUNIT_ASSERT( DoesAccept( EMceNatUpdated ) );
    EUNIT_ASSERT( !DoesAccept( EMceNatOffer ) );
    EUNIT_ASSERT( !DoesAccept( EMceNatUpdateSdp ) );
    EUNIT_ASSERT( !DoesAccept( EMceNatError ) );
    EUNIT_ASSERT( !DoesAccept( EMceNatHandleSIP ) );
    EUNIT_ASSERT( !DoesAccept( EMceNatSync ) );
    }

void UT_CMceNatStateCreateOfferRequested::
	UT_CMceNatStateCreateOfferRequested_EntryLL()
    {
    TMceNatStateTransitionEvent event( NULL,
    								   *iNatSession,
    								   EMceNatInitialized,
    								   *iNatSession,
    								   EMceInvite );
    event.ParamStatus() = KMceReady;

	iState->TransitionHandler().EntryL( event );
	
    EUNIT_ASSERT( event.ParamStatus() == KMceAsync );
    }
    
void UT_CMceNatStateCreateOfferRequested::
	UT_CMceNatStateCreateOfferRequested_ExitLL()
	{
	TMceNatStateTransitionEvent event( NULL,
    								   *iNatSession,
    								   EMceNatInitialized,
    								   *iNatSession,
    								   EMceInvite );
	iState->TransitionHandler().ExitL( event );

	EUNIT_ASSERT( iNatSession->iNatStateMachine->iCurrentState ==
				  KMceNatStateIdle );
	}

TBool
UT_CMceNatStateCreateOfferRequested::DoesAccept( TMceNatEventCode aEventCode )
    {
    TMceNatStateTransitionEvent event( NULL,
    								   *iNatSession,
    								   aEventCode,
    								   *iNatSession,
    								   EMceInvite );
    return iState->Accept( event );
    }
    
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceNatStateCreateOfferRequested,
    "UT_CMceNatStateCreateOfferRequested",
    "UNIT" )

EUNIT_TEST(
    "DoAcceptL - test ",
    "CMceNatStateCreateOfferRequested",
    "AcceptL",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatStateCreateOfferRequested_AcceptLL, Teardown)

EUNIT_TEST(
    "EntryL - test ",
    "CMceNatStateCreateOfferRequested",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatStateCreateOfferRequested_EntryLL, Teardown)

EUNIT_TEST(
    "ExitL - test ",
    "CMceNatStateCreateOfferRequested",
    "ExitL",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatStateCreateOfferRequested_ExitLL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
