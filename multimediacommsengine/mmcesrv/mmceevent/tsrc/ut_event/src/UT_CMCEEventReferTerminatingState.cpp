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
#include "ut_cmceeventreferterminatingstate.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <sipstrings.h>


//  INTERNAL INCLUDES
#include "mceeventreferterminatingstate.h"
#include "MCEEventTestConsts.h"
#include "sipreferdialogassoc.h"
#include "sipsubscribedialogassoc.h"
#include "sipeventheader.h"
#include "sipnotifydialogassoc.h"
#include "sipresponseelements.h"
#include "sipprofile.h"
#include "mcesipconnection.h"
#include "mceserial.h"


// CONSTRUCTION
UT_CMceEventReferTerminatingState* UT_CMceEventReferTerminatingState::NewL()
    {
    UT_CMceEventReferTerminatingState* self = UT_CMceEventReferTerminatingState::NewLC(); 
    CleanupStack::Pop(self);

    return self;
    }

UT_CMceEventReferTerminatingState* UT_CMceEventReferTerminatingState::NewLC()
    {
    UT_CMceEventReferTerminatingState* self = new( ELeave ) UT_CMceEventReferTerminatingState();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceEventReferTerminatingState::~UT_CMceEventReferTerminatingState()
    {
    }

// Default constructor
UT_CMceEventReferTerminatingState::UT_CMceEventReferTerminatingState()
    {
    }

// Second phase construct
void UT_CMceEventReferTerminatingState::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceEventReferTerminatingState::SetupL(  )
    {
    iEventState = 0;
    iState = new( ELeave ) CMceEventReferTerminatingState();
    SIPStrings::OpenL();
    iSipProfile = CSIPProfile::NewL();
	iConnection = CMceSipConnection::NewL( *iSipProfile );
	iSIPEvent = CMceSipEvent::NewL( *iConnection, *this );
	iSIPEvent->SetEventStateType( EMtRefer );
    MCE_DELETE( iSIPEvent->iEvent );
    iEvent = CMceComEvent::NewL( *iSIPEvent,
							     KDefaultExpires );
    iSIPEvent->iEvent = iEvent;
    iSIPEvent->SetCurrentState( KMceTerminatingEventStateIndex );
    } 

void UT_CMceEventReferTerminatingState::Teardown(  )
    {
    delete iState;
    
    delete iSIPEvent;
	iEvent = NULL;
    
    delete iConnection;
    
    delete iSipProfile;
    
	SIPStrings::Close();
    }

void UT_CMceEventReferTerminatingState::SetupEmptyL(  )
    {
    
    } 

void UT_CMceEventReferTerminatingState::UT_CMceEventReferTerminatingState_HandleLL()
    {
    EUNIT_ASSERT_LEAVE( iState->HandleL( *iEvent ) );
    }
        
void UT_CMceEventReferTerminatingState::UT_CMceEventReferTerminatingState_HandleReceiveRequestLL()
    {
    EUNIT_ASSERT_LEAVE( iState->HandleReceiveRequestL( *iEvent ) );
    }
           
void UT_CMceEventReferTerminatingState::UT_CMceEventReferTerminatingState_HandleResponseReceivedLL()
    {
    // Leaving with no dialog;
    EUNIT_ASSERT_LEAVE( iState->HandleResponseReceivedL( *iEvent ) ); 
    
    // Create and set notify dialog
    CSIPNotifyDialogAssoc* notify = CSIPNotifyDialogAssoc::NewL();
    iSIPEvent->SetDialog( notify );
    
    // Leaving with wrong action
    iEvent->SetPreviousAction( EMceItcSubscribe );
    EUNIT_ASSERT_LEAVE( iState->HandleResponseReceivedL( *iEvent ) ); 
    
    iEvent->SetPreviousAction( EMceItcTerminateEvent );
    
    // Leaving with no client transaction
    EUNIT_ASSERT_LEAVE( iState->HandleResponseReceivedL( *iEvent ) ); 
    
    // Set client transaction
    CSIPClientTransaction* clientTran = CSIPClientTransaction::NewL();
    clientTran->SetType( 3 );// notify
    CleanupStack::PushL( clientTran );
    iSIPEvent->SetPendingTransactionL( clientTran );
    CleanupStack::Pop( clientTran );
    iSIPEvent->iResponse = clientTran;
    
    // Leaving with no response elements
    iEvent->SetPreviousAction( EMceItcTerminateEvent );
    EUNIT_ASSERT_LEAVE( iState->HandleResponseReceivedL( *iEvent ) ); 
    }

void UT_CMceEventReferTerminatingState::UT_CMceEventReferTerminatingState_HandleResponseReceivedL2L()
    {
    // accepted case
    CSIPNotifyDialogAssoc* notify = CSIPNotifyDialogAssoc::NewL();
    iSIPEvent->SetDialog( notify );
    
    CSIPClientTransaction* clientTran = CSIPClientTransaction::NewL();
    clientTran->SetType( 3 );// Notify
    CleanupStack::PushL( clientTran );
    iSIPEvent->SetPendingTransactionL( clientTran );
    CleanupStack::Pop( clientTran );
    iSIPEvent->iResponse = clientTran;
    
    _LIT8 ( KOK, "OK" );
    RStringF ok = SIPStrings::Pool().OpenFStringL( KOK );
    CleanupClosePushL( ok );
    CSIPResponseElements* resp = CSIPResponseElements::NewL( 200,ok );
    clientTran->SetResponseElements( resp );
    
    iEvent->SetPreviousAction( EMceItcTerminateEvent );
    iState->HandleResponseReceivedL( *iEvent );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceTerminatedEventStateIndex );
    EUNIT_ASSERT( iEventState == CMceRefer::ETerminated );
    CleanupStack::PopAndDestroy();
    }

void UT_CMceEventReferTerminatingState::UT_CMceEventReferTerminatingState_HandleResponseReceivedL3L()
    {
    // Rejected case 
    CSIPNotifyDialogAssoc* notify = CSIPNotifyDialogAssoc::NewL();
    iSIPEvent->SetDialog( notify );
    
    CSIPClientTransaction* clientTran = CSIPClientTransaction::NewL();
    clientTran->SetType( 3 );// notify
    CleanupStack::PushL( clientTran );
    iSIPEvent->SetPendingTransactionL( clientTran );
    CleanupStack::Pop( clientTran );
    iSIPEvent->iResponse = clientTran;
    _LIT8( KDecline, "Decline" );
    RStringF rejected = SIPStrings::Pool().OpenFStringL( KDecline );
    CleanupClosePushL( rejected );
    CSIPResponseElements* resp = CSIPResponseElements::NewL( 603,rejected );
    clientTran->SetResponseElements( resp );
    iEvent->SetPreviousAction( EMceItcTerminateEvent );
    iState->HandleResponseReceivedL( *iEvent );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceTerminatedEventStateIndex );
    EUNIT_ASSERT( iEventState == CMceRefer::ETerminated );
    CleanupStack::PopAndDestroy();  
    }
    

void UT_CMceEventReferTerminatingState::UT_CMceEventReferTerminatingState_HandleResponseReceivedL4L()
    {
    // dialog terminated
    CSIPNotifyDialogAssoc* notify = CSIPNotifyDialogAssoc::NewL();
    iSIPEvent->SetDialog( notify );
    notify->Dialog().SetState( CSIPDialog::ETerminated );
    
    iState->HandleResponseReceivedL( *iEvent );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceTerminatedEventStateIndex );
    EUNIT_ASSERT( iEventState == CMceRefer::ETerminated );
    }
void UT_CMceEventReferTerminatingState::UT_CMceEventReferTerminatingState_HandleResponseReceivedL5L()
    {
    // mo refer
    CSIPReferDialogAssoc* refer = CSIPReferDialogAssoc::NewL();
    iSIPEvent->SetDialog( refer );
    delete iEvent;
    iEvent = NULL;
    iSIPEvent ->iEvent = NULL;
    iSIPEvent->SetEventStateType( EMoRefer );
    iEvent = CMceComEvent::NewL( *iSIPEvent,
							     KDefaultExpires );
	iSIPEvent ->iEvent = iEvent;
    iSIPEvent->SetCurrentState( KMceTerminatingEventStateIndex );
    
    CSIPClientTransaction* clientTran = CSIPClientTransaction::NewL();
    clientTran->SetType( 2 );// Subscribe
    CleanupStack::PushL( clientTran );
    iSIPEvent->SetPendingTransactionL( clientTran );
    CleanupStack::Pop( clientTran );
    iSIPEvent->iResponse = clientTran;
    
    _LIT8 ( KOK, "OK" );
    RStringF ok = SIPStrings::Pool().OpenFStringL( KOK );
    CleanupClosePushL( ok );
    CSIPResponseElements* resp = CSIPResponseElements::NewL( 200,ok );
    clientTran->SetResponseElements( resp );
    
    iEvent->SetPreviousAction( EMceItcTerminateEvent );
    iState->HandleResponseReceivedL( *iEvent );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceTerminatedEventStateIndex );
    CleanupStack::PopAndDestroy();
    }

void UT_CMceEventReferTerminatingState::UT_CMceEventReferTerminatingState_HandleResponseReceivedL6L()
    {
    // Provisional case 
    CSIPNotifyDialogAssoc* notify = CSIPNotifyDialogAssoc::NewL();
    iSIPEvent->SetDialog( notify );
    
    CSIPClientTransaction* clientTran = CSIPClientTransaction::NewL();
    clientTran->SetType( 3 );// notify
    CleanupStack::PushL( clientTran );
    iSIPEvent->SetPendingTransactionL( clientTran );
    CleanupStack::Pop( clientTran );
    iSIPEvent->iResponse = clientTran;
    _LIT8( KRinging, "Decline" );
    RStringF ringing = SIPStrings::Pool().OpenFStringL( KRinging );
    CleanupClosePushL( ringing );
    CSIPResponseElements* resp = CSIPResponseElements::NewL( 180,ringing );
    clientTran->SetResponseElements( resp );
    iEvent->SetPreviousAction( EMceItcTerminateEvent );
    iState->HandleResponseReceivedL( *iEvent );
    CleanupStack::PopAndDestroy();  
    }

    
void UT_CMceEventReferTerminatingState::IncomingSubscribeL(
					CDesC8Array* aHeaders,
                    HBufC8* aContentType,
					HBufC8* aBody)
	{
	delete aHeaders;
	delete aContentType;
	delete aBody;	
	}
void UT_CMceEventReferTerminatingState::IncomingReferL(
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
void UT_CMceEventReferTerminatingState::EventStateChangedL(TUint32 aState)
	{
	iEventState = aState;
	}
		
void UT_CMceEventReferTerminatingState::NotifyReceivedL(CDesC8Array* aHeaders,
                    		HBufC8* aContentType,
							HBufC8* aBody)
	{
	delete aHeaders;
	delete aContentType;
	delete aBody;	
	}
									
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceEventReferTerminatingState,
    "UT_CMceEventReferTerminatingState",
    "UNIT" )

EUNIT_TEST(
    "HandleL - test ",
    "CMceEventReferTerminatingState",
    "HandleL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferTerminatingState_HandleLL, Teardown)

EUNIT_TEST(
    "HandleReceiveRequestL - test ",
    "CMceEventReferTerminatingState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferTerminatingState_HandleReceiveRequestLL, Teardown)
    
EUNIT_TEST(
    "HandleResponseReceivedL - test ",
    "CMceEventReferTerminatingState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferTerminatingState_HandleResponseReceivedLL, Teardown)

EUNIT_TEST(
    "HandleResponseReceivedL - test - accepted ",
    "CMceEventReferTerminatingState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferTerminatingState_HandleResponseReceivedL2L, Teardown)

EUNIT_TEST(
    "HandleResponseReceivedL - test - rejected",
    "CMceEventReferTerminatingState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferTerminatingState_HandleResponseReceivedL3L, Teardown)

EUNIT_TEST(
    "HandleResponseReceivedL - test - terminated",
    "CMceEventReferTerminatingState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferTerminatingState_HandleResponseReceivedL4L, Teardown)

EUNIT_TEST(
    "HandleResponseReceivedL - test - mt refer",
    "CMceEventReferTerminatingState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferTerminatingState_HandleResponseReceivedL5L, Teardown)

EUNIT_TEST(
    "HandleResponseReceivedL - test - provisional",
    "CMceEventReferTerminatingState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferTerminatingState_HandleResponseReceivedL6L, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
