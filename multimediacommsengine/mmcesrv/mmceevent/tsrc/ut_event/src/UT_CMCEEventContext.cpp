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
#include "ut_cmceeventcontext.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include "MCEEventTestConsts.h"
#include "mcesipevent.h"
#include "mcecomevent.h"
#include "mceeventstatebase.h"
#include "sipprofile.h"
#include "mcesipconnection.h"
#include "mceserial.h"


//  INTERNAL INCLUDES
#include "mceeventcontext.h"

// CONSTRUCTION
UT_CMceEventContext* UT_CMceEventContext::NewL()
    {
    UT_CMceEventContext* self = UT_CMceEventContext::NewLC(); 
    CleanupStack::Pop(self);

    return self;
    }

UT_CMceEventContext* UT_CMceEventContext::NewLC()
    {
    UT_CMceEventContext* self = new( ELeave ) UT_CMceEventContext();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceEventContext::~UT_CMceEventContext()
    {
    }

// Default constructor
UT_CMceEventContext::UT_CMceEventContext()
    {
    }

// Second phase construct
void UT_CMceEventContext::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceEventContext::SetupL(  )
    {
    iSipProfile = CSIPProfile::NewL();
	iConnection = CMceSipConnection::NewL( *iSipProfile );
	iSIPEvent = CMceSipEvent::NewL( *iConnection, *this );
	iSIPEvent->SetEventStateType( EMoRefer );
	MCE_DELETE( iSIPEvent->iEvent );
    iEvent = CMceComEvent::NewL( *iSIPEvent,
							     KDefaultExpires );
    iSIPEvent->iEvent = iEvent;
    iContext = &iEvent->EventContext();
    } 

void UT_CMceEventContext::SetupEmptyL(  )
    {
    iSipProfile = CSIPProfile::NewL();
	iConnection = CMceSipConnection::NewL( *iSipProfile );
	iSIPEvent = CMceSipEvent::NewL( *iConnection, *this );
	iSIPEvent->SetEventStateType( EMoRefer );
	MCE_DELETE( iSIPEvent->iEvent );
    iEvent = CMceComEvent::NewL( *iSIPEvent,
							     KDefaultExpires );
    iSIPEvent->iEvent = iEvent;
    iSIPEvent->SetCurrentState( KMceEstablishedEventStateIndex );
    } 


void UT_CMceEventContext::Teardown(  )
    {
    delete iSIPEvent;
    iEvent = NULL;
    delete iConnection;
    delete iSipProfile;
    }

void UT_CMceEventContext::UT_CMceEventContext_NewLL(  )
    {
    CMceEventContext* context = NULL;
    //EUNIT_ASSERT_NO_LEAVE( context = CMceEventContext::NewL( *iEvent )  );
    context = CMceEventContext::NewL( *iEvent );
    delete context;
    }

void UT_CMceEventContext::UT_CMceEventContext_DestructorL(  )
    {
    // for coverage, manually delete headers.
    CMceEventContext* context = CMceEventContext::NewL( *iEvent );
    context->iStates->ResetAndDestroy();
    delete context->iStates;
    context->iStates = NULL;
    delete context;
    }


void UT_CMceEventContext::UT_CMceEventContext_SetCurrentStateL(  )
    {
    // MO Refer
    // Fail with too big state number
    EUNIT_ASSERT_LEAVE( iContext->SetCurrentStateL( *iEvent, 100 ) );
    
    // Fail with wrong state
    EUNIT_ASSERT_LEAVE( iContext->SetCurrentStateL( *iEvent, KMceIdleEventStateIndex ) );
    
    // Successfull
    EUNIT_ASSERT_NO_LEAVE( iContext->SetCurrentStateL( *iEvent, KMceEstablishingEventStateIndex ) );
    
    // MO Subscribe
    MCE_DELETE( iSIPEvent->iEvent );
    iSIPEvent->SetEventStateType( EMoSubscribe );
    iEvent = CMceComEvent::NewL( *iSIPEvent,
							     KDefaultExpires );
    iSIPEvent->iEvent = iEvent;
    iContext = &iEvent->EventContext();
    // Fail with wrong state
    EUNIT_ASSERT_LEAVE( iContext->SetCurrentStateL( *iEvent, KMceIdleEventStateIndex ) );
    
    // Successfull
    EUNIT_ASSERT_NO_LEAVE( iContext->SetCurrentStateL( *iEvent, KMceEstablishingEventStateIndex ) );
    
    // MT
    // Fail with wrong state
    MCE_DELETE( iSIPEvent->iEvent );
    iSIPEvent->SetEventStateType( EMtRefer );
    iEvent = CMceComEvent::NewL( *iSIPEvent,
							     KDefaultExpires );
    iSIPEvent->iEvent = iEvent;
    iContext = &iEvent->EventContext();
    EUNIT_ASSERT_LEAVE( iContext->SetCurrentStateL( *iEvent, KMceIdleEventStateIndex ) );
    
    // Successfull
    EUNIT_ASSERT_NO_LEAVE( iContext->SetCurrentStateL( *iEvent, KMceEstablishingEventStateIndex ) );
    }

void UT_CMceEventContext::UT_CMceEventContext_CurrentStateL(  )
    {
    EUNIT_ASSERT( iContext->CurrentState() == KMceIdleEventStateIndex );
    }

void UT_CMceEventContext::UT_CMceEventContext_ProcessEventLL(  )
    {
    iSIPEvent->SetCurrentState( KMceEstablishingEventStateIndex );
    EUNIT_ASSERT_LEAVE( iContext->ProcessEventL( *iEvent ) );
    }

void UT_CMceEventContext::UT_CMceEventContext_ProcessRequestReceivedEventLL(  )
    {
    EUNIT_ASSERT_LEAVE( iContext->ProcessRequestReceivedEventL( *iEvent ) );
    }

void UT_CMceEventContext::UT_CMceEventContext_ProcessIncomingResponseEventLL(  )
    {
    EUNIT_ASSERT_LEAVE( iContext->ProcessIncomingResponseEventL( *iEvent ) );
    }

void UT_CMceEventContext::UT_CMceEventContext_IsStateTransitionAcceptedL()
    {
    // KErrNotFound
    iContext->iCurrentState = NULL;
    EUNIT_ASSERT( iContext->IsStateTransitionAccepted( KMceIdleEventStateIndex ) );
    EUNIT_ASSERT( !iContext->IsStateTransitionAccepted( KMceEstablishingEventStateIndex ) );
    
    // KMceIdleEventStateIndex
    iSIPEvent->SetCurrentState( KMceIdleEventStateIndex );
    EUNIT_ASSERT( iContext->IsStateTransitionAccepted( KMceEstablishingEventStateIndex ) );
    EUNIT_ASSERT( !iContext->IsStateTransitionAccepted( KMceIdleEventStateIndex ) );
    
    // KMceEstablishingEventStateIndex
    iSIPEvent->SetCurrentState( KMceEstablishingEventStateIndex );
    EUNIT_ASSERT( iContext->IsStateTransitionAccepted( KMceEstablishedEventStateIndex ) );
    EUNIT_ASSERT( iContext->IsStateTransitionAccepted( KMceTerminatedEventStateIndex ) );
    EUNIT_ASSERT( !iContext->IsStateTransitionAccepted( KMceIdleEventStateIndex ) );
    
    // KMceEstablishedEventStateIndex
    iSIPEvent->SetCurrentState( KMceEstablishedEventStateIndex );
    EUNIT_ASSERT( iContext->IsStateTransitionAccepted( KMceTerminatingEventStateIndex ) );
    EUNIT_ASSERT( iContext->IsStateTransitionAccepted( KMceEstablishingEventStateIndex ) );
    EUNIT_ASSERT( iContext->IsStateTransitionAccepted( KMceEstablishedEventStateIndex ) );
    EUNIT_ASSERT( iContext->IsStateTransitionAccepted( KMceTerminatedEventStateIndex ) );
    EUNIT_ASSERT( !iContext->IsStateTransitionAccepted( KMceIdleEventStateIndex ) );
    
    // KMceETerminatingEventStateIndex
    iSIPEvent->SetCurrentState( KMceTerminatingEventStateIndex );
    EUNIT_ASSERT( iContext->IsStateTransitionAccepted( KMceTerminatedEventStateIndex ) );
    EUNIT_ASSERT( !iContext->IsStateTransitionAccepted( KMceIdleEventStateIndex ) );
    
    // KMceETerminatedEventStateIndex, default
    iSIPEvent->SetCurrentState( KMceTerminatedEventStateIndex );
    EUNIT_ASSERT( !iContext->IsStateTransitionAccepted( KMceIdleEventStateIndex ) );
    }

void UT_CMceEventContext::UT_CMceEventContext_IsMoStateTransitionAcceptedL()
    {
    // The method does nothing new, only calls IsStateTransitionAccepted.
    // KErrNotFound
    iContext->iCurrentState = NULL;
    EUNIT_ASSERT( iContext->IsMoStateTransitionAccepted( KMceIdleEventStateIndex ) );
    EUNIT_ASSERT( !iContext->IsMoStateTransitionAccepted( KMceEstablishingEventStateIndex ) );
    
    // KMceIdleEventStateIndex
    iSIPEvent->SetCurrentState( KMceIdleEventStateIndex );
    EUNIT_ASSERT( iContext->IsMoStateTransitionAccepted( KMceEstablishingEventStateIndex ) );
    EUNIT_ASSERT( !iContext->IsMoStateTransitionAccepted( KMceIdleEventStateIndex ) );
    
    // KMceEstablishingEventStateIndex
    iSIPEvent->SetCurrentState( KMceEstablishingEventStateIndex );
    EUNIT_ASSERT( iContext->IsMoStateTransitionAccepted( KMceEstablishedEventStateIndex ) );
    EUNIT_ASSERT( iContext->IsMoStateTransitionAccepted( KMceTerminatedEventStateIndex ) );
    EUNIT_ASSERT( !iContext->IsMoStateTransitionAccepted( KMceIdleEventStateIndex ) );
    
    // KMceEstablishedEventStateIndex
    iSIPEvent->SetCurrentState( KMceEstablishedEventStateIndex );
    EUNIT_ASSERT( iContext->IsMoStateTransitionAccepted( KMceTerminatingEventStateIndex ) );
    EUNIT_ASSERT( iContext->IsMoStateTransitionAccepted( KMceEstablishingEventStateIndex ) );
    EUNIT_ASSERT( iContext->IsMoStateTransitionAccepted( KMceEstablishedEventStateIndex ) );
    EUNIT_ASSERT( iContext->IsMoStateTransitionAccepted( KMceTerminatedEventStateIndex ) );
    EUNIT_ASSERT( !iContext->IsMoStateTransitionAccepted( KMceIdleEventStateIndex ) );
    
    // KMceETerminatingEventStateIndex
    iSIPEvent->SetCurrentState( KMceTerminatingEventStateIndex );
    EUNIT_ASSERT( iContext->IsMoStateTransitionAccepted( KMceTerminatedEventStateIndex ) );
    EUNIT_ASSERT( !iContext->IsMoStateTransitionAccepted( KMceIdleEventStateIndex ) );
    
    // KMceETerminatedEventStateIndex, default
    iSIPEvent->SetCurrentState( KMceTerminatedEventStateIndex );
    EUNIT_ASSERT( !iContext->IsMoStateTransitionAccepted( KMceIdleEventStateIndex ) );
    }
    
void UT_CMceEventContext::UT_CMceEventContext_IsMtStateTransitionAcceptedL()
    {
    // KErrNotFound
    iContext->iCurrentState = NULL;
    EUNIT_ASSERT( iContext->IsMtStateTransitionAccepted( KMceIdleEventStateIndex ) );
    EUNIT_ASSERT( !iContext->IsMtStateTransitionAccepted( KMceEstablishingEventStateIndex ) );
    
    // KMceIdleEventStateIndex
    iSIPEvent->SetCurrentState( KMceIdleEventStateIndex );
    EUNIT_ASSERT( iContext->IsMtStateTransitionAccepted( KMceEstablishingEventStateIndex ) );
    EUNIT_ASSERT( iContext->IsMtStateTransitionAccepted( KMceEstablishedEventStateIndex ) );
    EUNIT_ASSERT( iContext->IsMtStateTransitionAccepted( KMceTerminatedEventStateIndex ) );
    EUNIT_ASSERT( !iContext->IsMtStateTransitionAccepted( KMceIdleEventStateIndex ) );
    
    // KMceEstablishingEventStateIndex
    iSIPEvent->SetCurrentState( KMceEstablishingEventStateIndex );
    EUNIT_ASSERT( iContext->IsMtStateTransitionAccepted( KMceEstablishedEventStateIndex ) );
    EUNIT_ASSERT( iContext->IsMtStateTransitionAccepted( KMceTerminatedEventStateIndex ) );
    EUNIT_ASSERT( !iContext->IsMtStateTransitionAccepted( KMceIdleEventStateIndex ) );
    
    // KMceEstablishedEventStateIndex
    iSIPEvent->SetCurrentState( KMceEstablishedEventStateIndex );
    EUNIT_ASSERT( iContext->IsMtStateTransitionAccepted( KMceTerminatingEventStateIndex ) );
    EUNIT_ASSERT( iContext->IsMtStateTransitionAccepted( KMceEstablishingEventStateIndex ) );
    EUNIT_ASSERT( iContext->IsMtStateTransitionAccepted( KMceEstablishedEventStateIndex ) );
    EUNIT_ASSERT( iContext->IsMtStateTransitionAccepted( KMceTerminatedEventStateIndex ) );
    EUNIT_ASSERT( !iContext->IsMtStateTransitionAccepted( KMceIdleEventStateIndex ) );
    
    // KMceETerminatingEventStateIndex
    iSIPEvent->SetCurrentState( KMceTerminatingEventStateIndex );
    EUNIT_ASSERT( iContext->IsMtStateTransitionAccepted( KMceTerminatedEventStateIndex ) );
    EUNIT_ASSERT( !iContext->IsMtStateTransitionAccepted( KMceIdleEventStateIndex ) );
    
    // KMceETerminatedEventStateIndex, default
    iSIPEvent->SetCurrentState( KMceTerminatedEventStateIndex );
    EUNIT_ASSERT( !iContext->IsMtStateTransitionAccepted( KMceIdleEventStateIndex ) );
    }
    
void UT_CMceEventContext::IncomingSubscribeL(
					CDesC8Array* aHeaders,
                    HBufC8* aContentType,
					HBufC8* aBody)
	{
	delete aHeaders;
	delete aContentType;
	delete aBody;	
	}
void UT_CMceEventContext::IncomingReferL(
					HBufC8* aReferTo,
					CDesC8Array* aHeaders,
                    HBufC8* aContentType,
					HBufC8* aBody)
	{
	if ( aReferTo )
	    delete aReferTo;
	
	if ( aHeaders )
	    delete aHeaders;
	
	if ( aContentType )
	    delete aContentType;
	
	if ( aBody )
	    delete aBody;	
	}
void UT_CMceEventContext::EventStateChangedL(TUint32 /*aState*/)
	{
	
	}
		
void UT_CMceEventContext::NotifyReceivedL(CDesC8Array* aHeaders,
                    		HBufC8* aContentType,
							HBufC8* aBody)
	{
	delete aHeaders;
	delete aContentType;
	delete aBody;	
	}
									

		
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceEventContext,
    "UT_CMceEventContext",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMceEventContext",
    "NewL",
    "FUNCTIONALITY",
    SetupEmptyL, UT_CMceEventContext_NewLL, Teardown)

EUNIT_TEST(
    "Destructor - test ",
    "CMceEventContext",
    "~CMceEventContext",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventContext_DestructorL, Teardown)

EUNIT_TEST(
    "SetCurrentState - test ",
    "CMceEventContext",
    "SetCurrentState",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventContext_SetCurrentStateL, Teardown)

EUNIT_TEST(
    "CurrentState - test ",
    "CMceEventContext",
    "CurrentState",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventContext_CurrentStateL, Teardown)

EUNIT_TEST(
    "ProcessEventL - test ",
    "CMceEventContext",
    "ProcessEventL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventContext_ProcessEventLL, Teardown)

EUNIT_TEST(
    "ProcessRequestReceivedEventL - test ",
    "CMceEventContext",
    "ProcessRequestReceivedEventL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventContext_ProcessRequestReceivedEventLL, Teardown)

EUNIT_TEST(
    "ProcessIncomingResponseEventL - test ",
    "CMceEventContext",
    "ProcessIncomingResponseEventL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventContext_ProcessIncomingResponseEventLL, Teardown)


EUNIT_TEST(
    "IsStateTransitionAccepted - test ",
    "CMceEventContext",
    "IsStateTransitionAccepted",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventContext_IsStateTransitionAcceptedL, Teardown)


EUNIT_TEST(
    "IsMoStateTransitionAccepted - test ",
    "CMceEventContext",
    "IsMoStateTransitionAccepted",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventContext_IsMoStateTransitionAcceptedL, Teardown)

EUNIT_TEST(
    "IsMtStateTransitionAccepted - test ",
    "CMceEventContext",
    "IsMtStateTransitionAccepted",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventContext_IsMtStateTransitionAcceptedL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
