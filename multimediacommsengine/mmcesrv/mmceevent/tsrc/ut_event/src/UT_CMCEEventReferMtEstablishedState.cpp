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
#include "ut_cmceeventrefermtestablishedstate.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <sipstrings.h>
#include <sipexpiresheader.h>
#include <uri8.h>
#include <siprequestelements.h>
#include <sipmessageelements.h>
#include <sipresponseelements.h>


//  INTERNAL INCLUDES
#include "mceeventrefermtestablishedstate.h"
#include "MCEEventTestConsts.h"
#include "sipreferdialogassoc.h"
#include "sipsubscribedialogassoc.h"
#include "sipnotifydialogassoc.h"
#include "sipeventheader.h"
#include "sipprofile.h"
#include "mcesipconnection.h"
#include "mceserial.h"
_LIT8( KMceSubscriptionStateHeader, "Subscription-State: terminated; reason=probation; retry-after=3600");


// CONSTRUCTION
UT_CMceEventReferMtEstablishedState* UT_CMceEventReferMtEstablishedState::NewL()
    {
    UT_CMceEventReferMtEstablishedState* self = UT_CMceEventReferMtEstablishedState::NewLC(); 
    CleanupStack::Pop(self);

    return self;
    }

UT_CMceEventReferMtEstablishedState* UT_CMceEventReferMtEstablishedState::NewLC()
    {
    UT_CMceEventReferMtEstablishedState* self = new( ELeave ) UT_CMceEventReferMtEstablishedState();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceEventReferMtEstablishedState::~UT_CMceEventReferMtEstablishedState()
    {
    }

// Default constructor
UT_CMceEventReferMtEstablishedState::UT_CMceEventReferMtEstablishedState()
    {
    }

// Second phase construct
void UT_CMceEventReferMtEstablishedState::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceEventReferMtEstablishedState::SetupL(  )
    {
    iEventState = 0;
    iState = new( ELeave ) CMceEventReferMtEstablishedState();
    SIPStrings::OpenL();
    iSipProfile = CSIPProfile::NewL();
	iConnection = CMceSipConnection::NewL( *iSipProfile );
	iSIPEvent = CMceSipEvent::NewL( *iConnection, *this );
	iSIPEvent->SetEventStateType( EMtRefer );
    MCE_DELETE( iSIPEvent->iEvent );
    iEvent = CMceComEvent::NewL( *iSIPEvent,
							     KDefaultExpires );
    iSIPEvent->iEvent = iEvent;
    iSIPEvent->SetCurrentState( KMceEstablishedEventStateIndex );
    
    } 

void UT_CMceEventReferMtEstablishedState::SetupFullL(  )
    {
    iEventState = 0;
    iState = new( ELeave ) CMceEventReferMtEstablishedState();
    SIPStrings::OpenL();
    iSipProfile = CSIPProfile::NewL();
	iConnection = CMceSipConnection::NewL( *iSipProfile );
	iSIPEvent = CMceSipEvent::NewL( *iConnection, *this );
	iSIPEvent->SetEventStateType( EMtRefer );
    MCE_DELETE( iSIPEvent->iEvent );
    iEvent = CMceComEvent::NewL( *iSIPEvent,
							     KDefaultExpires );
    iSIPEvent->iEvent = iEvent;
    CSIPNotifyDialogAssoc* dlg = CSIPNotifyDialogAssoc::NewL();
    iSIPEvent->SetDialog( dlg );
    iSIPEvent->SetCurrentState( KMceEstablishedEventStateIndex );
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
    } 


void UT_CMceEventReferMtEstablishedState::Teardown(  )
    {
    delete iState;
    
    delete iSIPEvent;
	iEvent = NULL;
    
    delete iConnection;
    
    delete iSipProfile;
    
	SIPStrings::Close();
    }

void UT_CMceEventReferMtEstablishedState::SetupEmptyL(  )
    {
    
    } 

void UT_CMceEventReferMtEstablishedState::UT_CMceEventReferMtEstablishedState_HandleLL()
    {
    // Leaving cases
    // Leaving with no dialog
    EUNIT_ASSERT_LEAVE( iState->HandleL( *iEvent ) );
    
    // Leaving with subscribe dialog 
    CSIPEventHeader* eventHdr = CSIPEventHeader::DecodeL( KEventPackage );
    CleanupStack::PushL( eventHdr );
    CSIPSubscribeDialogAssoc* subscrDlg = CSIPSubscribeDialogAssoc::NewL( eventHdr );
    CleanupStack::Pop( eventHdr );
    iSIPEvent->SetDialog( subscrDlg );
    EUNIT_ASSERT_LEAVE( iState->HandleL( *iEvent ) );
    
    // Create and set notify dialog
    delete iSIPEvent->iDialog;
    iSIPEvent->iDialog = NULL;
    
    CSIPNotifyDialogAssoc* notifyDlg = CSIPNotifyDialogAssoc::NewL();
    iSIPEvent->SetDialog( notifyDlg );
   
    // Failing with wrong action.
    iEvent->SetPreviousAction( EMceItcSubscribe );
    EUNIT_ASSERT_LEAVE( iState->HandleL( *iEvent ) );
    }
    
void UT_CMceEventReferMtEstablishedState::UT_CMceEventReferMtEstablishedState_HandleL2L()
    {
    // Notify
    iEvent->SetPreviousAction( EMceItcNotify );
    iState->HandleL( *iEvent );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceEstablishingEventStateIndex);
    
    }

void UT_CMceEventReferMtEstablishedState::UT_CMceEventReferMtEstablishedState_HandleL3L()
    {
    // Terminate
    iEvent->SetPreviousAction( EMceItcTerminateEvent );
    iState->HandleL( *iEvent );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceTerminatingEventStateIndex );
   	//Set state
   	 iEvent->EventContext().iCurrentState = ( *iEvent->EventContext().iStates )[ KMceEstablishedEventStateIndex ];
    // Terminate
   
   // Notify with content
   
    HBufC8* contentType = KContentTypeRefer().AllocLC();
    HBufC8* content = KNotifyContent().AllocLC(); 
    
    iEvent->SetMsgContentL( contentType, content ); 
    CleanupStack::Pop( content );
    CleanupStack::Pop( contentType );
 	//notify with headers
 	CDesC8Array* headers = new (ELeave) CDesC8ArrayFlat(2);
	CleanupStack::PushL( headers );
	headers->AppendL( KMceSubscriptionStateHeader );
	iEvent->SetMsgHeaders( headers );
	CleanupStack::Pop( headers );
    iEvent->SetPreviousAction( EMceItcTerminateEvent );
    iState->HandleL( *iEvent );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceTerminatingEventStateIndex);
    				
    
    }
    
   
    
void UT_CMceEventReferMtEstablishedState::UT_CMceEventReferMtEstablishedState_HandleL4L()
    {
    // Notify with content
    HBufC8* contentType = KContentTypeRefer().AllocLC();
    HBufC8* content = KNotifyContent().AllocLC(); 
    
    iEvent->SetMsgContentL( contentType, content ); 
    CleanupStack::Pop( content );
    CleanupStack::Pop( contentType );
    
    iEvent->SetPreviousAction( EMceItcNotify );
    iState->HandleL( *iEvent );
    EUNIT_ASSERT( iEvent->EventContext().CurrentState() ==
    				KMceEstablishingEventStateIndex );
    
    }

    
void UT_CMceEventReferMtEstablishedState::UT_CMceEventReferMtEstablishedState_HandleReceiveRequestLL()
    {
    // Failing with no dialog
    EUNIT_ASSERT_LEAVE( iState->HandleReceiveRequestL( *iEvent ) );
    
    // The leaving cases.
    // Failing with subscribe dialog
    CSIPEventHeader* eventHdr = CSIPEventHeader::DecodeL( KEventPackage );
    CleanupStack::PushL( eventHdr );
    CSIPSubscribeDialogAssoc* subscrDlg = CSIPSubscribeDialogAssoc::NewL( eventHdr );
    CleanupStack::Pop( eventHdr );
    iSIPEvent->SetDialog( subscrDlg );
    EUNIT_ASSERT_LEAVE( iState->HandleReceiveRequestL( *iEvent ) );
    
    // Create and set notify dialog
    delete iSIPEvent->iDialog;
    iSIPEvent->iDialog = NULL;
    
    CSIPNotifyDialogAssoc* notifyDlg = CSIPNotifyDialogAssoc::NewL();
    iSIPEvent->SetDialog( notifyDlg );
    
    // Failing with no request
    EUNIT_ASSERT_LEAVE( iState->HandleReceiveRequestL( *iEvent ) );
    
    // Create and set subscribe transaction
    
    CSIPServerTransaction* serverTran = CSIPServerTransaction::NewL();
    serverTran->SetType( 1 ); // Subscribe
    iSIPEvent->iRequest = serverTran;
  
    // Failing with wrong action
    iEvent->SetPreviousAction( EMceItcSubscribe );
    EUNIT_ASSERT_LEAVE( iState->HandleReceiveRequestL( *iEvent ) );
    
    // Set proper action
    iEvent->SetPreviousAction( KErrNotFound );
    
    // Leaving with wrong dialog state
    notifyDlg->Dialog().SetState( CSIPDialog::ETerminated );
    EUNIT_ASSERT_LEAVE( iState->HandleReceiveRequestL( *iEvent ) );
    notifyDlg->Dialog().SetState( CSIPDialog::EConfirmed );
    
    // Failing with no request elements
    EUNIT_ASSERT_LEAVE( iState->HandleReceiveRequestL( *iEvent ) );
    }
         
void UT_CMceEventReferMtEstablishedState::UT_CMceEventReferMtEstablishedState_HandleReceiveRequestL2L()
    {
    // !event -> send 400
    iState->HandleReceiveRequestL( *iEvent );
    }
    
void UT_CMceEventReferMtEstablishedState::UT_CMceEventReferMtEstablishedState_HandleReceiveRequestL3L()
    {
    // event && !expires -> leave
    RPointerArray<CSIPHeaderBase> hdrs;
    CSIPHeaderBase::PushLC( &hdrs );
    
    CSIPEventHeader* e = CSIPEventHeader::DecodeL( KEventPackage );
    CleanupStack::PushL( e );
    hdrs.AppendL( e );
    CleanupStack::Pop( e );
    
    iMsgElem->SetUserHeadersL( hdrs );
    CleanupStack::Pop( &hdrs );
    
    EUNIT_ASSERT_LEAVE( iState->HandleReceiveRequestL( *iEvent ) );
    }

void UT_CMceEventReferMtEstablishedState::UT_CMceEventReferMtEstablishedState_HandleReceiveRequestL4L()
    {
    // event && ( expires == 0 ) ->Terminating 
    RPointerArray<CSIPHeaderBase> hdrs;
    CSIPHeaderBase::PushLC( &hdrs );
    CSIPEventHeader* e = CSIPEventHeader::DecodeL( KEventPackage );
    CleanupStack::PushL( e );
    hdrs.AppendL( e );
    CleanupStack::Pop( e );
   
    CSIPExpiresHeader* ex = new ( ELeave ) CSIPExpiresHeader( 0 );
    CleanupStack::PushL( ex );
    hdrs.AppendL( ex );
    CleanupStack::Pop( ex );
    
    iMsgElem->SetUserHeadersL( hdrs );
    CleanupStack::Pop( &hdrs );
    
    iState->HandleReceiveRequestL( *iEvent );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceTerminatingEventStateIndex );
    }

void UT_CMceEventReferMtEstablishedState::UT_CMceEventReferMtEstablishedState_HandleReceiveRequestL5L()
    {
    // event && ( expires > 0 ) ->Established 
    RPointerArray<CSIPHeaderBase> hdrs;
    CSIPHeaderBase::PushLC( &hdrs );
    CSIPEventHeader* e = CSIPEventHeader::DecodeL( KEventPackage );
    CleanupStack::PushL( e );
    hdrs.AppendL( e );
    CleanupStack::Pop( e );
    
    CSIPExpiresHeader* ex = new ( ELeave ) CSIPExpiresHeader( 180 );
    CleanupStack::PushL( ex );
    hdrs.AppendL( ex );
    CleanupStack::Pop( ex );
    
    iMsgElem->SetUserHeadersL( hdrs );
    CleanupStack::Pop( &hdrs );
    iState->HandleReceiveRequestL( *iEvent );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceEstablishedEventStateIndex );
    
    }

void UT_CMceEventReferMtEstablishedState::UT_CMceEventReferMtEstablishedState_HandleResponseReceivedLL()
    {
    // Leaving cases
    // Failing with no client transaction.
    EUNIT_ASSERT_LEAVE( iState->HandleResponseReceivedL( *iEvent ) );
    
    // Set proper client tran
    CSIPClientTransaction* clientTran2 = CSIPClientTransaction::NewL();
    CleanupStack::PushL( clientTran2 );
    clientTran2->SetType( 3 );// Notify
    iSIPEvent->SetPendingTransactionL( clientTran2 );
    CleanupStack::Pop( clientTran2 );
    iSIPEvent->iResponse = clientTran2;
    
    // Failing with no response elements-
    EUNIT_ASSERT_LEAVE( iState->HandleResponseReceivedL( *iEvent ) );
    }
void UT_CMceEventReferMtEstablishedState::UT_CMceEventReferMtEstablishedState_HandleResponseReceivedL2L()
    {
    // accepted case
    CSIPClientTransaction* clientTran = CSIPClientTransaction::NewL();
    clientTran->SetType( 3 );// notify
    CleanupStack::PushL( clientTran );
    iSIPEvent->SetPendingTransactionL( clientTran );
    CleanupStack::Pop( clientTran );
    iSIPEvent->iResponse = clientTran;
    
    _LIT8 ( KOK, "OK" );
    RStringF accepted = SIPStrings::Pool().OpenFStringL( KOK );
    CleanupClosePushL( accepted );
    CSIPResponseElements* resp = CSIPResponseElements::NewL( 200,accepted );
    clientTran->SetResponseElements( resp );
    
    EUNIT_ASSERT_NO_LEAVE( iState->HandleResponseReceivedL( *iEvent ) );
    EUNIT_ASSERT( iEventState == CMceRefer::EAccepted );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceEstablishedEventStateIndex );
    CleanupStack::PopAndDestroy();
    }
    
void UT_CMceEventReferMtEstablishedState::UT_CMceEventReferMtEstablishedState_HandleResponseReceivedL3L()
    {
    // Rejected case 
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
    iEvent->SetPreviousAction( EMceItcReferEvent );
    EUNIT_ASSERT_NO_LEAVE( iState->HandleResponseReceivedL( *iEvent ) );
    EUNIT_ASSERT( iEventState == CMceRefer::ETerminated );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceTerminatedEventStateIndex );
    CleanupStack::PopAndDestroy();
    }

void UT_CMceEventReferMtEstablishedState::UT_CMceEventReferMtEstablishedState_HandleResponseReceivedL4L()
    {
    // Provisional case 
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
    iEvent->SetPreviousAction( EMceItcReferEvent );
    EUNIT_ASSERT_NO_LEAVE( iState->HandleResponseReceivedL( *iEvent ) );
    CleanupStack::PopAndDestroy();
    }

void UT_CMceEventReferMtEstablishedState::IncomingSubscribeL(
					CDesC8Array* aHeaders,
                    HBufC8* aContentType,
					HBufC8* aBody)
	{
	delete aHeaders;
	delete aContentType;
	delete aBody;	
	}
void UT_CMceEventReferMtEstablishedState::IncomingReferL(
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
void UT_CMceEventReferMtEstablishedState::EventStateChangedL(TUint32 aState)
	{
	iEventState = aState;
	}
		
void UT_CMceEventReferMtEstablishedState::NotifyReceivedL(CDesC8Array* aHeaders,
                    		HBufC8* aContentType,
							HBufC8* aBody)
	{
	delete aHeaders;
	delete aContentType;
	delete aBody;	
	}
									
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceEventReferMtEstablishedState,
    "UT_CMceEventReferMtEstablishedState",
    "UNIT" )

EUNIT_TEST(
    "HandleL - test ",
    "CMceEventReferMtEstablishedState",
    "HandleL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferMtEstablishedState_HandleLL, Teardown)

EUNIT_TEST(
    "HandleL - test Notify",
    "CMceEventReferMtEstablishedState",
    "HandleL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventReferMtEstablishedState_HandleL2L, Teardown)

EUNIT_TEST(
    "HandleL - test Terminate",
    "CMceEventReferMtEstablishedState",
    "HandleL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventReferMtEstablishedState_HandleL3L, Teardown)

EUNIT_TEST(
    "HandleL - test Notify with Content",
    "CMceEventReferMtEstablishedState",
    "HandleL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventReferMtEstablishedState_HandleL4L, Teardown)

EUNIT_TEST(
    "HandleReceiveRequestL - test ",
    "CMceEventReferMtEstablishedState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferMtEstablishedState_HandleReceiveRequestLL, Teardown)

EUNIT_TEST(
    "HandleReceiveRequestL - test 2",
    "CMceEventReferMtEstablishedState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventReferMtEstablishedState_HandleReceiveRequestL2L, Teardown)

EUNIT_TEST(
    "HandleReceiveRequestL - test 3",
    "CMceEventReferMtEstablishedState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventReferMtEstablishedState_HandleReceiveRequestL3L, Teardown)

EUNIT_TEST(
    "HandleReceiveRequestL - test 4",
    "CMceEventReferMtEstablishedState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventReferMtEstablishedState_HandleReceiveRequestL4L, Teardown)

EUNIT_TEST(
    "HandleReceiveRequestL - test 5",
    "CMceEventReferMtEstablishedState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventReferMtEstablishedState_HandleReceiveRequestL5L, Teardown)
    
EUNIT_TEST(
    "HandleResponseReceivedL - test ",
    "CMceEventReferMtEstablishedState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferMtEstablishedState_HandleResponseReceivedLL, Teardown)

EUNIT_TEST(
    "HandleResponseReceivedL - test - Accepted",
    "CMceEventReferMtEstablishedState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferMtEstablishedState_HandleResponseReceivedL2L, Teardown)

EUNIT_TEST(
    "HandleResponseReceivedL - test - Rejected ",
    "CMceEventReferMtEstablishedState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferMtEstablishedState_HandleResponseReceivedL3L, Teardown)

EUNIT_TEST(
    "HandleResponseReceivedL - test - Provisional ",
    "CMceEventReferMtEstablishedState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferMtEstablishedState_HandleResponseReceivedL4L, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
