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
#include "ut_cmceeventsubscribeterminatingstate.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <sipstrings.h>


//  INTERNAL INCLUDES
#include "mceeventsubscribeterminatingstate.h"
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
UT_CMceEventSubscribeTerminatingState* UT_CMceEventSubscribeTerminatingState::NewL()
    {
    UT_CMceEventSubscribeTerminatingState* self = UT_CMceEventSubscribeTerminatingState::NewLC(); 
    CleanupStack::Pop(self);

    return self;
    }

UT_CMceEventSubscribeTerminatingState* UT_CMceEventSubscribeTerminatingState::NewLC()
    {
    UT_CMceEventSubscribeTerminatingState* self = new( ELeave ) UT_CMceEventSubscribeTerminatingState();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceEventSubscribeTerminatingState::~UT_CMceEventSubscribeTerminatingState()
    {
    }

// Default constructor
UT_CMceEventSubscribeTerminatingState::UT_CMceEventSubscribeTerminatingState()
    {
    }

// Second phase construct
void UT_CMceEventSubscribeTerminatingState::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceEventSubscribeTerminatingState::SetupL(  )
    {
    iEventState = 0;
    iState = new( ELeave ) CMceEventSubscribeTerminatingState();
    SIPStrings::OpenL();
    iSipProfile = CSIPProfile::NewL();
	iConnection = CMceSipConnection::NewL( *iSipProfile );
	iSIPEvent = CMceSipEvent::NewL( *iConnection, *this );
	iSIPEvent->SetEventStateType( EMtSubscribe );
    MCE_DELETE( iSIPEvent->iEvent );
    iEvent = CMceComEvent::NewL( *iSIPEvent,
								 KDefaultExpires );
    iSIPEvent->iEvent = iEvent;
    iSIPEvent->SetCurrentState( KMceTerminatingEventStateIndex );
    } 

void UT_CMceEventSubscribeTerminatingState::Teardown(  )
    {
    delete iState;
    
    delete iSIPEvent;
	iEvent = NULL;
    
    delete iConnection;
    
    delete iSipProfile;
    
	SIPStrings::Close();
    }

void UT_CMceEventSubscribeTerminatingState::SetupEmptyL(  )
    {
    
    } 

void UT_CMceEventSubscribeTerminatingState::UT_CMceEventSubscribeTerminatingState_HandleLL()
    {
    EUNIT_ASSERT_LEAVE( iState->HandleL( *iEvent ) ); 
    }
        
void UT_CMceEventSubscribeTerminatingState::UT_CMceEventSubscribeTerminatingState_HandleReceiveRequestLL()
    {
    // leave with no dialog
    EUNIT_ASSERT_LEAVE( iState->HandleReceiveRequestL( *iEvent ) ); 
    }
           
void UT_CMceEventSubscribeTerminatingState::UT_CMceEventSubscribeTerminatingState_HandleResponseReceivedLL()
    {
    // Leaving with no dialog;
    EUNIT_ASSERT_LEAVE( iState->HandleResponseReceivedL( *iEvent ) ); 
    
    // Create and set notify dialog
    CSIPNotifyDialogAssoc* notify = CSIPNotifyDialogAssoc::NewL();
    iSIPEvent->SetDialog( notify );
    
    // Leaving with wrong action
    iEvent->SetPreviousAction( EMceItcSubscribe );
    EUNIT_ASSERT_LEAVE( iState->HandleResponseReceivedL( *iEvent ) ); 
    
    iEvent->SetPreviousAction( KErrNotFound );
    
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
    EUNIT_ASSERT_LEAVE( iState->HandleResponseReceivedL( *iEvent ) ); 
    }
    
void UT_CMceEventSubscribeTerminatingState::UT_CMceEventSubscribeTerminatingState_HandleResponseReceivedL2L()
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
    EUNIT_ASSERT( iEventState == CMceEvent::ETerminated );
    CleanupStack::PopAndDestroy();
    }

void UT_CMceEventSubscribeTerminatingState::UT_CMceEventSubscribeTerminatingState_HandleResponseReceivedL3L()
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
    EUNIT_ASSERT( iEventState == CMceEvent::ETerminated );
    CleanupStack::PopAndDestroy();  
    }

void UT_CMceEventSubscribeTerminatingState::UT_CMceEventSubscribeTerminatingState_HandleResponseReceivedL4L()
    {
    // dialog terminated
    CSIPNotifyDialogAssoc* notify = CSIPNotifyDialogAssoc::NewL();
    iSIPEvent->SetDialog( notify );
    notify->Dialog().SetState( CSIPDialog::ETerminated );
    iState->HandleResponseReceivedL( *iEvent );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceTerminatedEventStateIndex );
    EUNIT_ASSERT( iEventState == CMceEvent::ETerminated );
    }
    
void UT_CMceEventSubscribeTerminatingState::UT_CMceEventSubscribeTerminatingState_HandleResponseReceivedL5L()
    {
    // mo subscribe
    CSIPEventHeader* eventHdr = CSIPEventHeader::DecodeL( KEventPackage );
    CleanupStack::PushL( eventHdr );
    
    CSIPSubscribeDialogAssoc* subscrDlg = CSIPSubscribeDialogAssoc::NewL( eventHdr );
    CleanupStack::Pop( eventHdr );
    iSIPEvent->SetDialog( subscrDlg );
    delete iEvent;
    iEvent = NULL;
    iSIPEvent ->iEvent = NULL;
    iSIPEvent->SetEventStateType( EMoSubscribe );
    iEvent = CMceComEvent::NewL( *iSIPEvent,
								 KDefaultExpires );
	iSIPEvent ->iEvent = iEvent;
    iSIPEvent->SetCurrentState( KMceTerminatingEventStateIndex );
    CSIPClientTransaction* clientTran = CSIPClientTransaction::NewL();
    CleanupStack::PushL( clientTran );
    clientTran->SetType( 1 );// subscribe
      
    iSIPEvent->SetPendingTransactionL( clientTran );
    iSIPEvent->iResponse = clientTran;
 
    CleanupStack::Pop( clientTran );
    
    _LIT8 ( KOK, "OK" );
    RStringF ok = SIPStrings::Pool().OpenFStringL( KOK );
    CleanupClosePushL( ok );
    
    CSIPResponseElements* resp = CSIPResponseElements::NewL( 200,ok );
    CleanupStack::PushL(resp);
    clientTran->SetResponseElements( resp );
    CleanupStack::Pop(resp);
    
    CleanupStack::PopAndDestroy();
    iEvent->SetPreviousAction( KErrNotFound );
    iState->HandleResponseReceivedL( *iEvent );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceTerminatedEventStateIndex );
          
    }

void UT_CMceEventSubscribeTerminatingState::UT_CMceEventSubscribeTerminatingState_HandleResponseReceivedL6L()
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
    _LIT8( KRinging, "Ringing" );
    RStringF ringing = SIPStrings::Pool().OpenFStringL( KRinging );
    CleanupClosePushL( ringing );
    CSIPResponseElements* resp = CSIPResponseElements::NewL( 180,ringing );
    clientTran->SetResponseElements( resp );
    iEvent->SetPreviousAction( EMceItcTerminateEvent );
    iState->HandleResponseReceivedL( *iEvent );
    CleanupStack::PopAndDestroy();  
    }

void UT_CMceEventSubscribeTerminatingState::IncomingSubscribeL(
					CDesC8Array* aHeaders,
                    HBufC8* aContentType,
					HBufC8* aBody)
	{
	delete aHeaders;
	delete aContentType;
	delete aBody;	
	}
void UT_CMceEventSubscribeTerminatingState::IncomingReferL(
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
void UT_CMceEventSubscribeTerminatingState::EventStateChangedL(TUint32 aState)
	{
	iEventState = aState;
	}
		
void UT_CMceEventSubscribeTerminatingState::NotifyReceivedL(CDesC8Array* aHeaders,
                    		HBufC8* aContentType,
							HBufC8* aBody)
	{
	delete aHeaders;
	delete aContentType;
	delete aBody;	
	}

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceEventSubscribeTerminatingState,
    "UT_CMceEventSubscribeTerminatingState",
    "UNIT" )

EUNIT_TEST(
    "HandleL - test ",
    "CMceEventSubscribeTerminatingState",
    "HandleL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeTerminatingState_HandleLL, Teardown)

EUNIT_TEST(
    "HandleReceiveRequestL - test ",
    "CMceEventSubscribeTerminatingState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeTerminatingState_HandleReceiveRequestLL, Teardown)
    
EUNIT_TEST(
    "HandleResponseReceivedL - test ",
    "CMceEventSubscribeTerminatingState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeTerminatingState_HandleResponseReceivedLL, Teardown)

EUNIT_TEST(
    "HandleResponseReceivedL - test - accepted ",
    "CMceEventSubscribeTerminatingState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeTerminatingState_HandleResponseReceivedL2L, Teardown)

EUNIT_TEST(
    "HandleResponseReceivedL - test - rejected",
    "CMceEventSubscribeTerminatingState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeTerminatingState_HandleResponseReceivedL3L, Teardown)

EUNIT_TEST(
    "HandleResponseReceivedL - test - terminated",
    "CMceEventSubscribeTerminatingState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeTerminatingState_HandleResponseReceivedL4L, Teardown)

EUNIT_TEST(
    "HandleResponseReceivedL - test - mo subscribe",
    "CMceEventSubscribeTerminatingState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeTerminatingState_HandleResponseReceivedL5L, Teardown)

EUNIT_TEST(
    "HandleResponseReceivedL - test - provisional",
    "CMceEventSubscribeTerminatingState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeTerminatingState_HandleResponseReceivedL6L, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
