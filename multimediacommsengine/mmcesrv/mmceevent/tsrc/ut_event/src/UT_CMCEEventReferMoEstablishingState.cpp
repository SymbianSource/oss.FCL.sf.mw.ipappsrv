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
#include "ut_cmceeventrefermoestablishingstate.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <sipstrings.h>
#include <sipeventheader.h>
#include <sipextensionheader.h>
#include <sipcseqheader.h>

//  INTERNAL INCLUDES
#include "mceeventrefermoestablishingstate.h"
#include "MCEEventTestConsts.h"
#include "sipreferdialogassoc.h"
#include "sipsubscribedialogassoc.h"
#include "sipresponseelements.h"
#include "sipclienttransaction.h"
#include "mcesip.h"
#include "sipprofile.h"
#include "mcesipconnection.h"
#include "mceserial.h"


// CONSTRUCTION
UT_CMceEventReferMoEstablishingState* UT_CMceEventReferMoEstablishingState::NewL()
    {
    UT_CMceEventReferMoEstablishingState* self = UT_CMceEventReferMoEstablishingState::NewLC(); 
    CleanupStack::Pop(self);

    return self;
    }

UT_CMceEventReferMoEstablishingState* UT_CMceEventReferMoEstablishingState::NewLC()
    {
    UT_CMceEventReferMoEstablishingState* self = new( ELeave ) UT_CMceEventReferMoEstablishingState();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceEventReferMoEstablishingState::~UT_CMceEventReferMoEstablishingState()
    {
    }

// Default constructor
UT_CMceEventReferMoEstablishingState::UT_CMceEventReferMoEstablishingState()
    {
    }

// Second phase construct
void UT_CMceEventReferMoEstablishingState::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceEventReferMoEstablishingState::SetupL(  )
    {
    iEventState = 0;
    iState = new( ELeave ) CMceEventReferMoEstablishingState();
    SIPStrings::OpenL();
    iSipProfile = CSIPProfile::NewL();
	iConnection = CMceSipConnection::NewL( *iSipProfile );
	iSIPEvent = CMceSipEvent::NewL( *iConnection, *this );
	iSIPEvent->SetEventStateType( EMoRefer );
    MCE_DELETE( iSIPEvent->iEvent );
    iEvent = CMceComEvent::NewL( *iSIPEvent,
							     KDefaultExpires );
    iSIPEvent->iEvent = iEvent;
    iSIPEvent->SetCurrentState( KMceEstablishingEventStateIndex );
    } 

void UT_CMceEventReferMoEstablishingState::Teardown(  )
    {
    delete iState;
    
    delete iSIPEvent;
	iEvent = NULL;
    
    delete iConnection;
    
    delete iSipProfile;
    
	SIPStrings::Close();
    
    }

void UT_CMceEventReferMoEstablishingState::SetupEmptyL(  )
    {
    
    } 

void UT_CMceEventReferMoEstablishingState::UT_CMceEventReferMoEstablishingState_HandleLL()
    {
    EUNIT_ASSERT_LEAVE( iState->HandleL( *iEvent ) )
    }
        
void UT_CMceEventReferMoEstablishingState::UT_CMceEventReferMoEstablishingState_HandleReceiveRequestLL()
    {
    EUNIT_ASSERT_LEAVE( iState->HandleReceiveRequestL( *iEvent ) )
    }
           
void UT_CMceEventReferMoEstablishingState::UT_CMceEventReferMoEstablishingState_HandleResponseReceivedLL()
    {
    // failing cases
    // no client tran
    EUNIT_ASSERT_LEAVE( iState->HandleResponseReceivedL( *iEvent ) );
    
    // Set proper client tran
    CSIPClientTransaction* clientTran2 = CSIPClientTransaction::NewL();
    CleanupStack::PushL( clientTran2 );
    clientTran2->SetType( 2 );// refer
    iSIPEvent->SetPendingTransactionL( clientTran2 );
    CleanupStack::Pop( clientTran2 );
    iSIPEvent->iResponse = clientTran2;
    
    // Failing with no response elements-
    EUNIT_ASSERT_LEAVE( iState->HandleResponseReceivedL( *iEvent ) );
    
    // create and set response
    _LIT8 ( KAccepted, "Accepted" );
    RStringF accepted = SIPStrings::Pool().OpenFStringL( KAccepted );
    CleanupClosePushL( accepted );
    CSIPResponseElements* resp = CSIPResponseElements::NewL( 202,accepted );
    clientTran2->SetResponseElements( resp );
    
    // Failing with wrong action
    iEvent->SetPreviousAction( EMceItcSubscribe );
    EUNIT_ASSERT_LEAVE( iState->HandleResponseReceivedL( *iEvent ) );
    
    CleanupStack::PopAndDestroy();
    }
void UT_CMceEventReferMoEstablishingState::UT_CMceEventReferMoEstablishingState_HandleResponseReceivedL2L()
    {
    // accepted case
    CSIPClientTransaction* clientTran = CSIPClientTransaction::NewL();
    clientTran->SetType( 2 );// refer
    CleanupStack::PushL( clientTran );
    
    iSIPEvent->SetPendingTransactionL( clientTran );
    CleanupStack::Pop( clientTran );
    iSIPEvent->iResponse = clientTran;
    
    _LIT8 ( KAccepted, "Accepted" );
    RStringF accepted = SIPStrings::Pool().OpenFStringL( KAccepted );
    CleanupClosePushL( accepted );
    CSIPResponseElements* resp = CSIPResponseElements::NewL( 202,accepted );
    clientTran->SetResponseElements( resp );
    iEvent->SetPreviousAction( EMceItcReferEvent );
    iState->HandleResponseReceivedL( *iEvent );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceEstablishedEventStateIndex );
    EUNIT_ASSERT( iEventState == CMceRefer::EAccepted );
    CleanupStack::PopAndDestroy();
    }


void UT_CMceEventReferMoEstablishingState::UT_CMceEventReferMoEstablishingState_HandleResponseReceivedL3L()
    {
    // Rejected case 
    CSIPClientTransaction* clientTran = CSIPClientTransaction::NewL();
    clientTran->SetType( 2 );// refer
    CleanupStack::PushL( clientTran );
    iSIPEvent->SetPendingTransactionL( clientTran );
    CleanupStack::Pop( clientTran );
    iSIPEvent->iResponse = clientTran;
    _LIT8( KDecline, "Decline" );
    RStringF rejected = SIPStrings::Pool().OpenFStringL( KDecline );
    CleanupClosePushL( rejected );
    CSIPResponseElements* resp = CSIPResponseElements::NewL( 603,rejected );
    clientTran->SetResponseElements( resp );
    iEvent->SetPreviousAction( EMceItcReferEvent );
    EUNIT_ASSERT_NO_LEAVE( iState->HandleResponseReceivedL( *iEvent ) );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceTerminatedEventStateIndex );
    EUNIT_ASSERT( iEventState == CMceRefer::ETerminated );
    CleanupStack::PopAndDestroy();
    }

void UT_CMceEventReferMoEstablishingState::UT_CMceEventReferMoEstablishingState_HandleResponseReceivedL4L()
    {
    // Provisional case 
    CSIPClientTransaction* clientTran = CSIPClientTransaction::NewL();
    clientTran->SetType( 2 );// refer
    CleanupStack::PushL( clientTran );
    iSIPEvent->SetPendingTransactionL( clientTran );
    CleanupStack::Pop( clientTran );
    iSIPEvent->iResponse = clientTran;
    _LIT8( KRinging, "Ringing" );
    RStringF ringing = SIPStrings::Pool().OpenFStringL( KRinging );
    CleanupClosePushL( ringing );
    CSIPResponseElements* resp = CSIPResponseElements::NewL( 180,ringing );
    clientTran->SetResponseElements( resp );
    iEvent->SetPreviousAction( EMceItcReferEvent );
    EUNIT_ASSERT_NO_LEAVE( iState->HandleResponseReceivedL( *iEvent ) );
    EUNIT_ASSERT( !iEvent->SilentSuppression() );
    
    CleanupStack::PopAndDestroy();
    }

void UT_CMceEventReferMoEstablishingState::UT_CMceEventReferMoEstablishingState_HandleResponseReceivedL5L()
    {
    // silent suppression
    iEvent->SetReferType( CMceRefer::ESuppressed );
    CSIPClientTransaction* clientTran = CSIPClientTransaction::NewL();
    clientTran->SetType( 2 );// refer
    CleanupStack::PushL( clientTran );
    iSIPEvent->SetPendingTransactionL( clientTran );
    CleanupStack::Pop( clientTran );
    iSIPEvent->iResponse = clientTran;
    
    _LIT8 ( KAccepted, "Accepted" );
    RStringF accepted = SIPStrings::Pool().OpenFStringL( KAccepted );
    CleanupClosePushL( accepted );
    CSIPResponseElements* resp = CSIPResponseElements::NewL( 202,accepted );
    
    clientTran->SetResponseElements( resp );
    
    iEvent->SetPreviousAction( EMceItcReferEvent );
    EUNIT_ASSERT_NO_LEAVE( iState->HandleResponseReceivedL( *iEvent ) );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceEstablishedEventStateIndex );
    EUNIT_ASSERT( iEventState == CMceRefer::ETerminated );
    EUNIT_ASSERT( iEvent->SilentSuppression() );
    
    CleanupStack::PopAndDestroy();
    }

void UT_CMceEventReferMoEstablishingState::UT_CMceEventReferMoEstablishingState_HandleResponseReceivedL6L()
    {
    // suppressed
    iEvent->SetReferType( CMceRefer::ESuppressed );
    CSIPClientTransaction* clientTran = CSIPClientTransaction::NewL();
    CleanupStack::PushL( clientTran );
    clientTran->SetType( 2 );// refer
    iSIPEvent->SetPendingTransactionL( clientTran );
    
    CleanupStack::Pop( clientTran );
    iSIPEvent->iResponse = clientTran;
    
    _LIT8 ( KAccepted, "Accepted" );
    RStringF accepted = SIPStrings::Pool().OpenFStringL( KAccepted );
    CleanupClosePushL( accepted );
    
    CSIPResponseElements* resp = CSIPResponseElements::NewL( 202,accepted );
    CleanupStack::PushL( resp );
    
    RPointerArray< CSIPHeaderBase > hdrs;
    CSIPHeaderBase::PushLC( &hdrs );
    
    CSIPExtensionHeader* ex = CSIPExtensionHeader::NewLC( KMceSipHeaderReferSub, KMceSipValueFalse );
    hdrs.AppendL( ex );
    CleanupStack::Pop( ex );
    resp->MessageElements().SetUserHeadersL( hdrs );
    
    CleanupStack::Pop( &hdrs );
    
    clientTran->SetResponseElements( resp );
    CleanupStack::Pop( resp );
    
    iEvent->SetPreviousAction( EMceItcReferEvent );
    iState->HandleResponseReceivedL( *iEvent );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceTerminatedEventStateIndex );
    EUNIT_ASSERT( iEventState == CMceRefer::ETerminated );
    EUNIT_ASSERT( !iEvent->SilentSuppression() );
    CleanupStack::PopAndDestroy();
    }

void UT_CMceEventReferMoEstablishingState::UT_CMceEventReferMoEstablishingState_HandleResponseReceivedL7L()
    {
    // Multiple refer
    iEvent->SetReferType( CMceRefer::EMultipleRefer );
    CSIPClientTransaction* clientTran = CSIPClientTransaction::NewL();
    clientTran->SetType( 2 );// refer
    CleanupStack::PushL( clientTran );
    iSIPEvent->SetPendingTransactionL( clientTran );
    CleanupStack::Pop( clientTran );
    iSIPEvent->iResponse = clientTran;
    
    _LIT8 ( KAccepted, "Accepted" );
    RStringF accepted = SIPStrings::Pool().OpenFStringL( KAccepted );
    CleanupClosePushL( accepted );
    CSIPResponseElements* resp = CSIPResponseElements::NewL( 202,accepted );
    
    clientTran->SetResponseElements( resp );
    
    iEvent->SetPreviousAction( EMceItcReferEvent );
    EUNIT_ASSERT_NO_LEAVE( iState->HandleResponseReceivedL( *iEvent ) );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceTerminatedEventStateIndex );
    EUNIT_ASSERT( iEventState == CMceRefer::ETerminated );
    EUNIT_ASSERT( !iEvent->SilentSuppression() );
    CleanupStack::PopAndDestroy();
    
    }

void UT_CMceEventReferMoEstablishingState::IncomingSubscribeL(
					CDesC8Array* aHeaders,
                    HBufC8* aContentType,
					HBufC8* aBody)
	{
	delete aHeaders;
	delete aContentType;
	delete aBody;	
	}
void UT_CMceEventReferMoEstablishingState::IncomingReferL(
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
void UT_CMceEventReferMoEstablishingState::EventStateChangedL(TUint32 aState)
	{
	iEventState = aState;
	}
		
void UT_CMceEventReferMoEstablishingState::NotifyReceivedL(CDesC8Array* aHeaders,
                    		HBufC8* aContentType,
							HBufC8* aBody)
	{
	delete aHeaders;
	delete aContentType;
	delete aBody;	
	}
									

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceEventReferMoEstablishingState,
    "UT_CMceEventReferMoEstablishingState",
    "UNIT" )

EUNIT_TEST(
    "HandleL - test ",
    "CMceEventReferMoEstablishingState",
    "HandleL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferMoEstablishingState_HandleLL, Teardown)

EUNIT_TEST(
    "HandleReceiveRequestL - test ",
    "CMceEventReferMoEstablishingState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferMoEstablishingState_HandleReceiveRequestLL, Teardown)
    
EUNIT_TEST(
    "HandleResponseReceivedL - test ",
    "CMceEventReferMoEstablishingState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferMoEstablishingState_HandleResponseReceivedLL, Teardown)
    
EUNIT_TEST(
    "HandleResponseReceivedL - test 2",
    "CMceEventReferMoEstablishingState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferMoEstablishingState_HandleResponseReceivedL2L, Teardown)

EUNIT_TEST(
    "HandleResponseReceivedL - test 3",
    "CMceEventReferMoEstablishingState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferMoEstablishingState_HandleResponseReceivedL3L, Teardown)

EUNIT_TEST(
    "HandleResponseReceivedL - test 4",
    "CMceEventReferMoEstablishingState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferMoEstablishingState_HandleResponseReceivedL4L, Teardown)

EUNIT_TEST(
    "HandleResponseReceivedL - test silent suppression",
    "CMceEventReferMoEstablishingState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferMoEstablishingState_HandleResponseReceivedL5L, Teardown)

EUNIT_TEST(
    "HandleResponseReceivedL - test suppressed",
    "CMceEventReferMoEstablishingState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferMoEstablishingState_HandleResponseReceivedL6L, Teardown)

EUNIT_TEST(
    "HandleResponseReceivedL - test multiple refer",
    "CMceEventReferMoEstablishingState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferMoEstablishingState_HandleResponseReceivedL7L, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
