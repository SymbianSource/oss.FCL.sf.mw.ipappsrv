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
#include "ut_cmceeventsubscribemoestablishingstate.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <sipstrings.h>


//  INTERNAL INCLUDES
#include "mceeventsubscribemoestablishingstate.h"
#include "MCEEventTestConsts.h"
#include "sipreferdialogassoc.h"
#include "sipsubscribedialogassoc.h"
#include "sipeventheader.h"
#include "sipresponseelements.h"
#include "sipprofile.h"
#include "mcesipconnection.h"
#include "mceserial.h"


// CONSTRUCTION
UT_CMceEventSubscribeMoEstablishingState* UT_CMceEventSubscribeMoEstablishingState::NewL()
    {
    UT_CMceEventSubscribeMoEstablishingState* self = UT_CMceEventSubscribeMoEstablishingState::NewLC(); 
    CleanupStack::Pop(self);

    return self;
    }

UT_CMceEventSubscribeMoEstablishingState* UT_CMceEventSubscribeMoEstablishingState::NewLC()
    {
    UT_CMceEventSubscribeMoEstablishingState* self = new( ELeave ) UT_CMceEventSubscribeMoEstablishingState();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceEventSubscribeMoEstablishingState::~UT_CMceEventSubscribeMoEstablishingState()
    {
    }

// Default constructor
UT_CMceEventSubscribeMoEstablishingState::UT_CMceEventSubscribeMoEstablishingState()
    {
    }

// Second phase construct
void UT_CMceEventSubscribeMoEstablishingState::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceEventSubscribeMoEstablishingState::SetupL(  )
    {
    iEventState = 0;
    iState = new( ELeave ) CMceEventSubscribeMoEstablishingState();
    SIPStrings::OpenL();
    iSipProfile = CSIPProfile::NewL();
	iConnection = CMceSipConnection::NewL( *iSipProfile );
	iSIPEvent = CMceSipEvent::NewL( *iConnection, *this );
	iSIPEvent->SetEventStateType( EMoSubscribe );
    MCE_DELETE( iSIPEvent->iEvent );
    iEvent = CMceComEvent::NewL( *iSIPEvent,
							     KDefaultExpires );
    iSIPEvent->iEvent = iEvent;
    iSIPEvent->SetCurrentState( KMceEstablishingEventStateIndex );
    
    } 

void UT_CMceEventSubscribeMoEstablishingState::Teardown(  )
    {
    delete iState;
	
	delete iSIPEvent;
	iEvent = NULL;
    
    delete iConnection;
    
    delete iSipProfile;
    
	SIPStrings::Close();
    }

void UT_CMceEventSubscribeMoEstablishingState::SetupEmptyL(  )
    {
    
    } 

void UT_CMceEventSubscribeMoEstablishingState::UT_CMceEventSubscribeMoEstablishingState_HandleLL()
    {
    EUNIT_ASSERT_LEAVE( iState->HandleL( *iEvent ) );
    }
        
void UT_CMceEventSubscribeMoEstablishingState::UT_CMceEventSubscribeMoEstablishingState_HandleReceiveRequestLL()
    {
    EUNIT_ASSERT_LEAVE( iState->HandleReceiveRequestL( *iEvent ) ); 
    
    }
           
void UT_CMceEventSubscribeMoEstablishingState::UT_CMceEventSubscribeMoEstablishingState_HandleResponseReceivedLL()
    {
    // failing cases
    // leaving with no client transaction
    EUNIT_ASSERT_LEAVE( iState->HandleResponseReceivedL( *iEvent ) );
    
    // Set proper client tran
    CSIPClientTransaction* clientTran = CSIPClientTransaction::NewL();
    CleanupStack::PushL( clientTran );
    clientTran->SetType( 1 );// subscribe
    iSIPEvent->SetPendingTransactionL( clientTran );
    CleanupStack::Pop( clientTran );
    iSIPEvent->iResponse = clientTran;
    
    // Failing with no response elements-
    EUNIT_ASSERT_LEAVE( iState->HandleResponseReceivedL( *iEvent ) );
    
    // create and set response
    _LIT8 ( KAccepted, "Accepted" );
    RStringF accepted = SIPStrings::Pool().OpenFStringL( KAccepted );
    CleanupClosePushL( accepted );
    CSIPResponseElements* resp = CSIPResponseElements::NewL( 202,accepted );
    clientTran->SetResponseElements( resp );
    
    // Failing with wrong action
    iEvent->SetPreviousAction(  EMceItcReferEvent );
    EUNIT_ASSERT_LEAVE( iState->HandleResponseReceivedL( *iEvent ) );
    
    
    
    CleanupStack::PopAndDestroy();
    
    }
void UT_CMceEventSubscribeMoEstablishingState::UT_CMceEventSubscribeMoEstablishingState_HandleResponseReceivedL2L()
    {
    // accepted case
    CSIPClientTransaction* clientTran = CSIPClientTransaction::NewL();
    clientTran->SetType( 1 );// subscribe
    CleanupStack::PushL( clientTran );
    iSIPEvent->SetPendingTransactionL( clientTran );
    CleanupStack::Pop( clientTran );
    iSIPEvent->iResponse = clientTran;
    
    _LIT8 ( KOK, "OK" );
    RStringF ok = SIPStrings::Pool().OpenFStringL( KOK );
    CleanupClosePushL( ok );
    CSIPResponseElements* resp = CSIPResponseElements::NewL( 200,ok );
    clientTran->SetResponseElements( resp );
    
    iEvent->SetPreviousAction( EMceItcSubscribe );
    EUNIT_ASSERT_NO_LEAVE( iState->HandleResponseReceivedL( *iEvent ) );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceEstablishedEventStateIndex );
    EUNIT_ASSERT( iEventState == CMceEvent::EActive );
    CleanupStack::PopAndDestroy();
    }

void UT_CMceEventSubscribeMoEstablishingState::UT_CMceEventSubscribeMoEstablishingState_HandleResponseReceivedL3L()
    {
    // Rejected case 
    CSIPClientTransaction* clientTran = CSIPClientTransaction::NewL();
    clientTran->SetType( 1 );// subscribe
    CleanupStack::PushL( clientTran );
    iSIPEvent->SetPendingTransactionL( clientTran );
    CleanupStack::Pop( clientTran );
    iSIPEvent->iResponse = clientTran;
    _LIT8( KDecline, "Decline" );
    RStringF rejected = SIPStrings::Pool().OpenFStringL( KDecline );
    CleanupClosePushL( rejected );
    CSIPResponseElements* resp = CSIPResponseElements::NewL( 603,rejected );
    clientTran->SetResponseElements( resp );
    iEvent->SetPreviousAction( EMceItcSubscribeUpdate );
    EUNIT_ASSERT_NO_LEAVE( iState->HandleResponseReceivedL( *iEvent ) );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceTerminatedEventStateIndex );
    EUNIT_ASSERT( iEventState == CMceEvent::ETerminated );
    CleanupStack::PopAndDestroy();  
    }

void UT_CMceEventSubscribeMoEstablishingState::UT_CMceEventSubscribeMoEstablishingState_HandleResponseReceivedL4L()
    {
    // Provisional case 
    CSIPClientTransaction* clientTran = CSIPClientTransaction::NewL();
    clientTran->SetType( 1 );// subscribe
    CleanupStack::PushL( clientTran );
    iSIPEvent->SetPendingTransactionL( clientTran );
    CleanupStack::Pop( clientTran );
    iSIPEvent->iResponse = clientTran;
    _LIT8( KRinging, "Ringing" );
    RStringF ringing = SIPStrings::Pool().OpenFStringL( KRinging );
    CleanupClosePushL( ringing );
    CSIPResponseElements* resp = CSIPResponseElements::NewL( 180,ringing );
    clientTran->SetResponseElements( resp );
    iEvent->SetPreviousAction( EMceItcSubscribe );
    EUNIT_ASSERT_NO_LEAVE( iState->HandleResponseReceivedL( *iEvent ) );
    CleanupStack::PopAndDestroy();  
    }


void UT_CMceEventSubscribeMoEstablishingState::IncomingSubscribeL(
					CDesC8Array* aHeaders,
                    HBufC8* aContentType,
					HBufC8* aBody)
	{
	delete aHeaders;
	delete aContentType;
	delete aBody;	
	}
void UT_CMceEventSubscribeMoEstablishingState::IncomingReferL(
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
void UT_CMceEventSubscribeMoEstablishingState::EventStateChangedL(TUint32 aState)
	{
	iEventState = aState;
	}
		
void UT_CMceEventSubscribeMoEstablishingState::NotifyReceivedL(CDesC8Array* aHeaders,
                    		HBufC8* aContentType,
							HBufC8* aBody)
	{
	delete aHeaders;
	delete aContentType;
	delete aBody;	
	}
									
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceEventSubscribeMoEstablishingState,
    "UT_CMceEventSubscribeMoEstablishingState.",
    "UNIT" )

EUNIT_TEST(
    "HandleL - test ",
    "CMceEventSubscribeMoEstablishingState",
    "HandleL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeMoEstablishingState_HandleLL, Teardown)

EUNIT_TEST(
    "HandleReceiveRequestL - test ",
    "CMceEventSubscribeMoEstablishingState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeMoEstablishingState_HandleReceiveRequestLL, Teardown)
    
EUNIT_TEST(
    "HandleResponseReceivedL - test ",
    "CMceEventSubscribeMoEstablishingState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeMoEstablishingState_HandleResponseReceivedLL, Teardown)

EUNIT_TEST(
    "HandleResponseReceivedL - test - accepted ",
    "CMceEventSubscribeMoEstablishingState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeMoEstablishingState_HandleResponseReceivedL2L, Teardown)

EUNIT_TEST(
    "HandleResponseReceivedL - test - rejected",
    "CMceEventSubscribeMoEstablishingState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeMoEstablishingState_HandleResponseReceivedL3L, Teardown)

EUNIT_TEST(
    "HandleResponseReceivedL - test - provisional",
    "CMceEventSubscribeMoEstablishingState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeMoEstablishingState_HandleResponseReceivedL4L, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
