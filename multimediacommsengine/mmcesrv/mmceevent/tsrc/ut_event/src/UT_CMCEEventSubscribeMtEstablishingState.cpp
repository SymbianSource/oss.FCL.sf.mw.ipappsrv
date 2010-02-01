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
#include "ut_cmceeventsubscribemtestablishingstate.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <sipstrings.h>
#include <uri8.h>


//  INTERNAL INCLUDES
#include "mceeventsubscribemtestablishingstate.h"
#include "MCEEventTestConsts.h"
#include "sipreferdialogassoc.h"
#include "sipsubscribedialogassoc.h"
#include "sipeventheader.h"
#include "sipnotifydialogassoc.h"
#include "siprequestelements.h"
#include "sipresponseelements.h"
#include "sipprofile.h"
#include "mcesipconnection.h"
#include "mceserial.h"


// CONSTRUCTION
UT_CMceEventSubscribeMtEstablishingState* UT_CMceEventSubscribeMtEstablishingState::NewL()
    {
    UT_CMceEventSubscribeMtEstablishingState* self = UT_CMceEventSubscribeMtEstablishingState::NewLC(); 
    CleanupStack::Pop(self);

    return self;
    }

UT_CMceEventSubscribeMtEstablishingState* UT_CMceEventSubscribeMtEstablishingState::NewLC()
    {
    UT_CMceEventSubscribeMtEstablishingState* self = new( ELeave ) UT_CMceEventSubscribeMtEstablishingState();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceEventSubscribeMtEstablishingState::~UT_CMceEventSubscribeMtEstablishingState()
    {
    }

// Default constructor
UT_CMceEventSubscribeMtEstablishingState::UT_CMceEventSubscribeMtEstablishingState()
    {
    }

// Second phase construct
void UT_CMceEventSubscribeMtEstablishingState::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceEventSubscribeMtEstablishingState::SetupL(  )
    {
    iEventState = 0;
    iState = new( ELeave ) CMceEventSubscribeMtEstablishingState();
    SIPStrings::OpenL();
    iSipProfile = CSIPProfile::NewL();
	iConnection = CMceSipConnection::NewL( *iSipProfile );
	iSIPEvent = CMceSipEvent::NewL( *iConnection, *this );
	iSIPEvent->SetEventStateType( EMtSubscribe );
	MCE_DELETE( iSIPEvent->iEvent );
    iEvent = CMceComEvent::NewL( *iSIPEvent,
							     KDefaultExpires );
    iSIPEvent->iEvent = iEvent;
    iSIPEvent->SetCurrentState( KMceEstablishingEventStateIndex );
    } 
    
void UT_CMceEventSubscribeMtEstablishingState::SetupFullL(  )
    {
    iEventState = 0;
    iState = new( ELeave ) CMceEventSubscribeMtEstablishingState();
    SIPStrings::OpenL();
    iSipProfile = CSIPProfile::NewL();
	iConnection = CMceSipConnection::NewL( *iSipProfile );
	iSIPEvent = CMceSipEvent::NewL( *iConnection, *this );
	iSIPEvent->SetEventStateType( EMtSubscribe );
    MCE_DELETE( iSIPEvent->iEvent );
    iEvent = CMceComEvent::NewL( *iSIPEvent,
							     KDefaultExpires );
    iSIPEvent->iEvent = iEvent;
    iSIPEvent->SetCurrentState( KMceEstablishingEventStateIndex );
 
    CSIPNotifyDialogAssoc* notify = CSIPNotifyDialogAssoc::NewL();
    iSIPEvent->SetDialog( notify );
    
    // Create server transaction
    CSIPServerTransaction* serverTran = CSIPServerTransaction::NewL();
    serverTran->SetType( 1 ); // Subscribe
    iSIPEvent->iRequest = serverTran;
  
    // Create request elements
    TUriParser8 uriParser;
    User::LeaveIfError( uriParser.Parse( KRemoteUri() ) );
    CUri8* remoteUri = CUri8::NewL(uriParser);
    CleanupStack::PushL(remoteUri);
    
    CSIPRequestElements* elem =CSIPRequestElements::NewL( remoteUri );
    CleanupStack::Pop( remoteUri );
    serverTran->SetRequestElements( elem );
    iMsgElem = &elem->MessageElements();
    
    // Create client transaction
    CSIPClientTransaction* clientTran = CSIPClientTransaction::NewL();
    clientTran->SetType( 3 );// notify
    CleanupStack::PushL( clientTran );
    iSIPEvent->SetPendingTransactionL( clientTran );
    CleanupStack::Pop( clientTran );
    iSIPEvent->iResponse = clientTran;
    }

void UT_CMceEventSubscribeMtEstablishingState::Teardown(  )
    {
    delete iState;
    
    delete iSIPEvent;
	iEvent = NULL;
    
    delete iConnection;
    
    delete iSipProfile;
    
	SIPStrings::Close();
    }
    
void UT_CMceEventSubscribeMtEstablishingState::SetupEmptyL(  )
    {
    } 

void UT_CMceEventSubscribeMtEstablishingState::UT_CMceEventSubscribeMtEstablishingState_HandleLL()
    {
    // leaving cases
    // Failing with no dialog
    EUNIT_ASSERT_LEAVE( iState->HandleL( *iEvent ) );
    
    // Failing with subscribe dialog
    CSIPEventHeader* eventHdr = CSIPEventHeader::DecodeL( KEventPackage );
    CleanupStack::PushL( eventHdr );
    CSIPSubscribeDialogAssoc* subscrDlg = CSIPSubscribeDialogAssoc::NewL( eventHdr );
    CleanupStack::Pop( eventHdr );
    iSIPEvent->SetDialog( subscrDlg );
    EUNIT_ASSERT_LEAVE( iState->HandleL( *iEvent ) );
    
    // Create and set notify dialog
    delete iSIPEvent->iDialog;
    iSIPEvent->iDialog = NULL;
    
    CSIPNotifyDialogAssoc* referDlg = CSIPNotifyDialogAssoc::NewL();
    iSIPEvent->SetDialog( referDlg );
    
    // Failing with no server transaction
    EUNIT_ASSERT_LEAVE( iState->HandleL( *iEvent ) );
    
    // set server transaction
    CSIPServerTransaction* serverTran = CSIPServerTransaction::NewL();
    serverTran->SetType( 1 ); // subscribe
    iSIPEvent->iRequest = serverTran;
  
    // Failing with no request elements
    EUNIT_ASSERT_LEAVE( iState->HandleL( *iEvent ) );
    
    // Create request elements
    TUriParser8 uriParser;
    User::LeaveIfError( uriParser.Parse( KRemoteUri() ) );
    CUri8* remoteUri = CUri8::NewL(uriParser);
    CleanupStack::PushL(remoteUri);
    
    CSIPRequestElements* elem =CSIPRequestElements::NewL( remoteUri );
    CleanupStack::Pop( remoteUri );
    serverTran->SetRequestElements( elem );
    
    // Failing with wrong action
    iEvent->SetPreviousAction( EMceItcReferEvent );
    EUNIT_ASSERT_LEAVE( iState->HandleL( *iEvent ) );
    }
        
void UT_CMceEventSubscribeMtEstablishingState::UT_CMceEventSubscribeMtEstablishingState_HandleL2L()
    {
    // accept
    iEvent->SetPreviousAction( EMceItcAcceptEvent );
    iState->HandleL( *iEvent );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceEstablishedEventStateIndex);
    }

void UT_CMceEventSubscribeMtEstablishingState::UT_CMceEventSubscribeMtEstablishingState_HandleL3L()
    {
    // reject
    iEvent->SetPreviousAction( EMceItcRejectEvent );
    iState->HandleL( *iEvent );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceTerminatedEventStateIndex);
     
    }

void UT_CMceEventSubscribeMtEstablishingState::UT_CMceEventSubscribeMtEstablishingState_HandleL4L()
    {
    // respond
    iEvent->SetPreviousAction( EMceItcRespondEvent );
    _LIT8 ( KFuu, "fuu" );
    iEvent->SetReasonPhraseL(405, KFuu().AllocL() );
    iState->HandleL( *iEvent );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceTerminatedEventStateIndex);
    
    }
    
void UT_CMceEventSubscribeMtEstablishingState::UT_CMceEventSubscribeMtEstablishingState_HandleL5L()
    {
    // provisional
    iEvent->SetPreviousAction( EMceItcRespondEvent );
    _LIT8 ( KRinging, "Ringing" );
    iEvent->SetReasonPhraseL(180, KRinging().AllocL() );
    iState->HandleL( *iEvent );
    }


void UT_CMceEventSubscribeMtEstablishingState::UT_CMceEventSubscribeMtEstablishingState_HandleReceiveRequestLL()
    {
    EUNIT_ASSERT_NO_LEAVE( iState->HandleReceiveRequestL( *iEvent ) ); 
    }
           
void UT_CMceEventSubscribeMtEstablishingState::UT_CMceEventSubscribeMtEstablishingState_HandleResponseReceivedLL()
    {
    // failing cases
    // Failing with no client transaction
    EUNIT_ASSERT_LEAVE( iState->HandleResponseReceivedL( *iEvent ) );
    // Failing with wrong client transaction
    CSIPClientTransaction* clientTran = CSIPClientTransaction::NewL();
    CleanupStack::PushL( clientTran );
    clientTran->SetType( 2 );// refer
    iSIPEvent->SetPendingTransactionL( clientTran );
    CleanupStack::Pop( clientTran );
    iSIPEvent->iResponse = clientTran;
    EUNIT_ASSERT_LEAVE( iState->HandleResponseReceivedL( *iEvent ) );
    
    // Set proper client tran
    CSIPClientTransaction* clientTran2 = CSIPClientTransaction::NewL();
    CleanupStack::PushL( clientTran2 );
    clientTran2->SetType( 3 );// Notify
    iSIPEvent->SetPendingTransactionL( clientTran2 );
    CleanupStack::Pop( clientTran2 );
    iSIPEvent->iResponse = clientTran2;
    
    // Failing with wrong action
    iEvent->SetPreviousAction( EMceItcSubscribe );
    EUNIT_ASSERT_LEAVE( iState->HandleResponseReceivedL( *iEvent ) );
    iEvent->SetPreviousAction( EMceItcNotify );
    // Failing with no response elements-
    EUNIT_ASSERT_LEAVE( iState->HandleResponseReceivedL( *iEvent ) );
    
    // create and set response
    _LIT8 ( KAccepted, "Accepted" );
    RStringF accepted = SIPStrings::Pool().OpenFStringL( KAccepted );
    CleanupClosePushL( accepted );
    CSIPResponseElements* resp = CSIPResponseElements::NewL( 202,accepted );
    clientTran2->SetResponseElements( resp );
    
    // Failing with wrong action
    iEvent->SetPreviousAction( EMceItcReferEvent );
    EUNIT_ASSERT_LEAVE( iState->HandleResponseReceivedL( *iEvent ) );
    
    CleanupStack::PopAndDestroy();
    }

void UT_CMceEventSubscribeMtEstablishingState::UT_CMceEventSubscribeMtEstablishingState_HandleResponseReceivedL2L()
    {
    // accepted case
    _LIT8 ( KOK, "OK" );
    RStringF ok = SIPStrings::Pool().OpenFStringL( KOK );
    CleanupClosePushL( ok );
    CSIPResponseElements* resp = CSIPResponseElements::NewL( 200,ok );
    iSIPEvent->iResponse->SetResponseElements( resp );
    
    iEvent->SetPreviousAction( EMceItcNotify );
    EUNIT_ASSERT_NO_LEAVE( iState->HandleResponseReceivedL( *iEvent ) );
    EUNIT_ASSERT( iEventState == CMceEvent::EActive );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceEstablishedEventStateIndex );
    CleanupStack::PopAndDestroy();
    
    }

void UT_CMceEventSubscribeMtEstablishingState::UT_CMceEventSubscribeMtEstablishingState_HandleResponseReceivedL3L()
    {
    // Rejected case 
    _LIT8( KDecline, "Decline" );
    RStringF rejected = SIPStrings::Pool().OpenFStringL( KDecline );
    CleanupClosePushL( rejected );
    CSIPResponseElements* resp = CSIPResponseElements::NewL( 603,rejected );
    iSIPEvent->iResponse->SetResponseElements( resp );
    iEvent->SetPreviousAction( EMceItcNotify );
    EUNIT_ASSERT_NO_LEAVE( iState->HandleResponseReceivedL( *iEvent ) );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceTerminatedEventStateIndex );
    EUNIT_ASSERT( iEventState == CMceEvent::ETerminated );
    CleanupStack::PopAndDestroy();  
    }

void UT_CMceEventSubscribeMtEstablishingState::UT_CMceEventSubscribeMtEstablishingState_HandleResponseReceivedL4L()
    {
    // Provisional case 
    _LIT8( KRinging, "Ringing" );
    RStringF ringing = SIPStrings::Pool().OpenFStringL( KRinging );
    CleanupClosePushL( ringing );
    CSIPResponseElements* resp = CSIPResponseElements::NewL( 180,ringing );
    iSIPEvent->iResponse->SetResponseElements( resp );
    iEvent->SetPreviousAction( EMceItcNotify );
    EUNIT_ASSERT_NO_LEAVE( iState->HandleResponseReceivedL( *iEvent ) );
    CleanupStack::PopAndDestroy();  
    }


void UT_CMceEventSubscribeMtEstablishingState::IncomingSubscribeL(
					CDesC8Array* aHeaders,
                    HBufC8* aContentType,
					HBufC8* aBody)
	{
	delete aHeaders;
	delete aContentType;
	delete aBody;	
	}
void UT_CMceEventSubscribeMtEstablishingState::IncomingReferL(
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
void UT_CMceEventSubscribeMtEstablishingState::EventStateChangedL(TUint32 aState)
	{
	iEventState = aState;
	}
		
void UT_CMceEventSubscribeMtEstablishingState::NotifyReceivedL(CDesC8Array* aHeaders,
                    		HBufC8* aContentType,
							HBufC8* aBody)
	{
	delete aHeaders;
	delete aContentType;
	delete aBody;	
	}
									
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceEventSubscribeMtEstablishingState,
    "UT_CMceEventSubscribeMtEstablishingState",
    "UNIT" )

EUNIT_TEST(
    "HandleL - test ",
    "CMceEventSubscribeMtEstablishingState",
    "HandleL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeMtEstablishingState_HandleLL, Teardown)

EUNIT_TEST(
    "HandleL - test 2",
    "CMceEventSubscribeMtEstablishingState",
    "HandleL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventSubscribeMtEstablishingState_HandleL2L, Teardown)

EUNIT_TEST(
    "HandleL - test 3",
    "CMceEventSubscribeMtEstablishingState",
    "HandleL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventSubscribeMtEstablishingState_HandleL3L, Teardown)

EUNIT_TEST(
    "HandleL - test 4",
    "CMceEventSubscribeMtEstablishingState",
    "HandleL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventSubscribeMtEstablishingState_HandleL4L, Teardown)

EUNIT_TEST(
    "HandleL - test 5",
    "CMceEventSubscribeMtEstablishingState",
    "HandleL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventSubscribeMtEstablishingState_HandleL5L, Teardown)


EUNIT_TEST(
    "HandleReceiveRequestL - test ",
    "CMceEventSubscribeMtEstablishingState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeMtEstablishingState_HandleReceiveRequestLL, Teardown)
    
EUNIT_TEST(
    "HandleResponseReceivedL - test -leaves",
    "CMceEventSubscribeMtEstablishingState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeMtEstablishingState_HandleResponseReceivedLL, Teardown)

EUNIT_TEST(
    "HandleResponseReceivedL - test - accepted",
    "CMceEventSubscribeMtEstablishingState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventSubscribeMtEstablishingState_HandleResponseReceivedL2L, Teardown)

EUNIT_TEST(
    "HandleResponseReceivedL - test -rejected",
    "CMceEventSubscribeMtEstablishingState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventSubscribeMtEstablishingState_HandleResponseReceivedL3L, Teardown)

EUNIT_TEST(
    "HandleResponseReceivedL - test -provisional",
    "CMceEventSubscribeMtEstablishingState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventSubscribeMtEstablishingState_HandleResponseReceivedL4L, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
