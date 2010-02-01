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
#include "ut_cmceeventrefermtestablishingstate.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <sipstrings.h>
#include <uri8.h>

//  INTERNAL INCLUDES
#include "mceeventrefermtestablishingstate.h"
#include "MCEEventTestConsts.h"
#include "sipsubscribedialogassoc.h"
#include "sipeventheader.h"
#include "siprequestelements.h"
#include "sipmessageelements.h"
#include "sipnotifydialogassoc.h"
#include "sipresponseelements.h"
#include "sipprofile.h"
#include "mcesipconnection.h"
#include "mceserial.h"

// CONSTRUCTION
UT_CMceEventReferMtEstablishingState* UT_CMceEventReferMtEstablishingState::NewL()
    {
    UT_CMceEventReferMtEstablishingState* self = UT_CMceEventReferMtEstablishingState::NewLC(); 
    CleanupStack::Pop(self);

    return self;
    }

UT_CMceEventReferMtEstablishingState* UT_CMceEventReferMtEstablishingState::NewLC()
    {
    UT_CMceEventReferMtEstablishingState* self = new( ELeave ) UT_CMceEventReferMtEstablishingState();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceEventReferMtEstablishingState::~UT_CMceEventReferMtEstablishingState()
    {
    }

// Default constructor
UT_CMceEventReferMtEstablishingState::UT_CMceEventReferMtEstablishingState()
    {
    }

// Second phase construct
void UT_CMceEventReferMtEstablishingState::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceEventReferMtEstablishingState::SetupL(  )
    {
    iEventState = 0;
    iState = new( ELeave ) CMceEventReferMtEstablishingState();
    SIPStrings::OpenL();
    iSipProfile = CSIPProfile::NewL();
	iConnection = CMceSipConnection::NewL( *iSipProfile );
	iSIPEvent = CMceSipEvent::NewL( *iConnection, *this );
	iSIPEvent->SetEventStateType( EMtRefer );
    MCE_DELETE( iSIPEvent->iEvent );
    iEvent = CMceComEvent::NewL( *iSIPEvent,
							     KDefaultExpires );
    iSIPEvent->iEvent = iEvent;
    iSIPEvent->SetCurrentState( KMceEstablishingEventStateIndex );
    } 

void UT_CMceEventReferMtEstablishingState::SetupFullL(  )
    {
    iEventState = 0;
    iState = new( ELeave ) CMceEventReferMtEstablishingState();
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
    
    CSIPNotifyDialogAssoc* notify = CSIPNotifyDialogAssoc::NewL();
    iSIPEvent->SetDialog( notify );
    
    // Create server transaction
    CSIPServerTransaction* serverTran = CSIPServerTransaction::NewL();
    serverTran->SetType( 2 ); // Refer
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

    
void UT_CMceEventReferMtEstablishingState::Teardown(  )
    {
    delete iState;
    
    delete iSIPEvent;
	iEvent = NULL;
    
    delete iConnection;
    
    delete iSipProfile;
    	
	SIPStrings::Close();
    }

void UT_CMceEventReferMtEstablishingState::SetupEmptyL(  )
    {
    
    } 

void UT_CMceEventReferMtEstablishingState::UT_CMceEventReferMtEstablishingState_HandleLL()
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
    
    // Create and set Refer transaction
    CSIPServerTransaction* serverTran2 = CSIPServerTransaction::NewL();
    serverTran2->SetType( 2 ); // Refer
    iSIPEvent->iRequest = serverTran2;
  
    // Failing with no request elements
    EUNIT_ASSERT_LEAVE( iState->HandleL( *iEvent ) );
    
    // Create request elements
    TUriParser8 uriParser;
    User::LeaveIfError( uriParser.Parse( KRemoteUri() ) );
    CUri8* remoteUri = CUri8::NewL(uriParser);
    CleanupStack::PushL(remoteUri);
    
    CSIPRequestElements* elem =CSIPRequestElements::NewL( remoteUri );
    CleanupStack::Pop( remoteUri );
    serverTran2->SetRequestElements( elem );
    
    // Failing with wrong action
    iEvent->SetPreviousAction( EMceItcSubscribe );
    EUNIT_ASSERT_LEAVE( iState->HandleL( *iEvent ) );
    }

void UT_CMceEventReferMtEstablishingState::UT_CMceEventReferMtEstablishingState_HandleL2L()
    {
    // accept
    iEvent->SetPreviousAction( EMceItcAcceptEvent );
    iState->HandleL( *iEvent );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceEstablishedEventStateIndex);
    
    }
    
void UT_CMceEventReferMtEstablishingState::UT_CMceEventReferMtEstablishingState_HandleL3L()
    {
    // reject
    iEvent->SetPreviousAction( EMceItcRejectEvent );
    iState->HandleL( *iEvent );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceTerminatedEventStateIndex);
    
    }

void UT_CMceEventReferMtEstablishingState::UT_CMceEventReferMtEstablishingState_HandleL4L()
    {
    // respond
    iEvent->SetPreviousAction( EMceItcRespondEvent );
    _LIT8 ( KFuu, "fuu" );
    iEvent->SetReasonPhraseL(405, KFuu().AllocL() );
    iState->HandleL( *iEvent );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceTerminatedEventStateIndex);
    }

void UT_CMceEventReferMtEstablishingState::UT_CMceEventReferMtEstablishingState_HandleL5L()
    {
    // respond with provisional
    iEvent->SetPreviousAction( EMceItcRespondEvent );
    _LIT8 ( KRinging, "Ringing" );
    iEvent->SetReasonPhraseL(180, KRinging().AllocL() );
    iState->HandleL( *iEvent );
    }

void UT_CMceEventReferMtEstablishingState::UT_CMceEventReferMtEstablishingState_HandleL6L()
    {
    // suppressed
    iEvent->SetReferType( CMceRefer::ESuppressed );
    iEvent->SetPreviousAction( EMceItcAcceptEvent );
    iState->HandleL( *iEvent );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceTerminatedEventStateIndex );
    }

    
void UT_CMceEventReferMtEstablishingState::UT_CMceEventReferMtEstablishingState_HandleReceiveRequestLL()
    {
    EUNIT_ASSERT_NO_LEAVE( iState->HandleReceiveRequestL( *iEvent ) );
    }
           
void UT_CMceEventReferMtEstablishingState::UT_CMceEventReferMtEstablishingState_HandleResponseReceivedLL()
    {
    // failing cases
    // Failing with wrong action
    iEvent->SetPreviousAction( EMceItcSubscribe );
    EUNIT_ASSERT_LEAVE( iState->HandleResponseReceivedL( *iEvent ) );
    iEvent->SetPreviousAction( EMceItcNotify );
    
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
    
    // Failing with no response elements
    EUNIT_ASSERT_LEAVE( iState->HandleResponseReceivedL( *iEvent ) );
    
    // create and set response
    _LIT8 ( KAccepted, "Accepted" );
    RStringF accepted = SIPStrings::Pool().OpenFStringL( KAccepted );
    CleanupClosePushL( accepted );
    CSIPResponseElements* resp = CSIPResponseElements::NewL( 202,accepted );
    clientTran2->SetResponseElements( resp );
    
    CleanupStack::PopAndDestroy();
    }

void UT_CMceEventReferMtEstablishingState::UT_CMceEventReferMtEstablishingState_HandleResponseReceivedL2L()
    {
    // accepted case
    _LIT8 ( KOK, "OK" );
    RStringF ok = SIPStrings::Pool().OpenFStringL( KOK );
    CleanupClosePushL( ok );
    CSIPResponseElements* resp = CSIPResponseElements::NewL( 200,ok );
    iSIPEvent->iResponse->SetResponseElements( resp );
    
    iEvent->SetPreviousAction( EMceItcNotify );
    EUNIT_ASSERT_NO_LEAVE( iState->HandleResponseReceivedL( *iEvent ) );
    EUNIT_ASSERT( iEventState == CMceRefer::EAccepted );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceEstablishedEventStateIndex );
    CleanupStack::PopAndDestroy();
    }

void UT_CMceEventReferMtEstablishingState::UT_CMceEventReferMtEstablishingState_HandleResponseReceivedL3L()
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
    EUNIT_ASSERT( iEventState == CMceRefer::ETerminated );
    CleanupStack::PopAndDestroy();  
    }

void UT_CMceEventReferMtEstablishingState::UT_CMceEventReferMtEstablishingState_HandleResponseReceivedL4L()
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

void UT_CMceEventReferMtEstablishingState::IncomingSubscribeL(
					CDesC8Array* aHeaders,
                    HBufC8* aContentType,
					HBufC8* aBody)
	{
	delete aHeaders;
	delete aContentType;
	delete aBody;	
	}
void UT_CMceEventReferMtEstablishingState::IncomingReferL(
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
void UT_CMceEventReferMtEstablishingState::EventStateChangedL(TUint32 aState)
	{
	iEventState = aState;
	}
		
void UT_CMceEventReferMtEstablishingState::NotifyReceivedL(CDesC8Array* aHeaders,
                    		HBufC8* aContentType,
							HBufC8* aBody)
	{
	delete aHeaders;
	delete aContentType;
	delete aBody;	
	}
									
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceEventReferMtEstablishingState,
    "UT_CMceEventReferMtEstablishingState",
    "UNIT" )

EUNIT_TEST(
    "HandleL - test ",
    "CMceEventReferMtEstablishingState",
    "HandleL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferMtEstablishingState_HandleLL, Teardown)

EUNIT_TEST(
    "HandleL - test - accept",
    "CMceEventReferMtEstablishingState",
    "HandleL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventReferMtEstablishingState_HandleL2L, Teardown)

EUNIT_TEST(
    "HandleL - test - reject",
    "CMceEventReferMtEstablishingState",
    "HandleL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventReferMtEstablishingState_HandleL3L, Teardown)

EUNIT_TEST(
    "HandleL - test - respond",
    "CMceEventReferMtEstablishingState",
    "HandleL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventReferMtEstablishingState_HandleL4L, Teardown)

EUNIT_TEST(
    "HandleL - test - provisional",
    "CMceEventReferMtEstablishingState",
    "HandleL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventReferMtEstablishingState_HandleL5L, Teardown)

EUNIT_TEST(
    "HandleL - test - suppressed",
    "CMceEventReferMtEstablishingState",
    "HandleL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventReferMtEstablishingState_HandleL6L, Teardown)


EUNIT_TEST(
    "HandleReceiveRequestL - test ",
    "CMceEventReferMtEstablishingState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferMtEstablishingState_HandleReceiveRequestLL, Teardown)
    
EUNIT_TEST(
    "HandleResponseReceivedL - test ",
    "CMceEventReferMtEstablishingState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferMtEstablishingState_HandleResponseReceivedLL, Teardown)

EUNIT_TEST(
    "HandleResponseReceivedL - test - accepted",
    "CMceEventReferMtEstablishingState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventReferMtEstablishingState_HandleResponseReceivedL2L, Teardown)

EUNIT_TEST(
    "HandleResponseReceivedL - test - rejected",
    "CMceEventReferMtEstablishingState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventReferMtEstablishingState_HandleResponseReceivedL3L, Teardown)

EUNIT_TEST(
    "HandleResponseReceivedL - test - provisional",
    "CMceEventReferMtEstablishingState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventReferMtEstablishingState_HandleResponseReceivedL4L, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
