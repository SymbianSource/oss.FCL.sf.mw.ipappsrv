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
#include "ut_cmceeventsubscribemoestablishedstate.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <sipstrings.h>
#include <sipsubscriptionstateheader.h>
#include <sipcontenttypeheader.h>
#include <uri8.h>
#include <siprequestelements.h>
#include <sipresponseelements.h>
#include <sipmessageelements.h>

//  INTERNAL INCLUDES
#include "mceeventsubscribemoestablishedstate.h"
#include "MCEEventTestConsts.h"
#include "sipreferdialogassoc.h"
#include "sipsubscribedialogassoc.h"
#include "sipeventheader.h"
#include "sipnotifydialogassoc.h"
#include "sipprofile.h"
#include "mcesipconnection.h"
#include "mceserial.h"


// CONSTRUCTION
UT_CMceEventSubscribeMoEstablishedState* UT_CMceEventSubscribeMoEstablishedState::NewL()
    {
    UT_CMceEventSubscribeMoEstablishedState* self = UT_CMceEventSubscribeMoEstablishedState::NewLC(); 
    CleanupStack::Pop(self);

    return self;
    }

UT_CMceEventSubscribeMoEstablishedState* UT_CMceEventSubscribeMoEstablishedState::NewLC()
    {
    UT_CMceEventSubscribeMoEstablishedState* self = new( ELeave ) UT_CMceEventSubscribeMoEstablishedState();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceEventSubscribeMoEstablishedState::~UT_CMceEventSubscribeMoEstablishedState()
    {
    }

// Default constructor
UT_CMceEventSubscribeMoEstablishedState::UT_CMceEventSubscribeMoEstablishedState()
    {
    }

// Second phase construct
void UT_CMceEventSubscribeMoEstablishedState::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceEventSubscribeMoEstablishedState::SetupL(  )
    {
    iEventState = 0;
    iReceived = 0;
    iState = new( ELeave ) CMceEventSubscribeMoEstablishedState();
    SIPStrings::OpenL();
    iSipProfile = CSIPProfile::NewL();
	iConnection = CMceSipConnection::NewL( *iSipProfile );
	iSIPEvent = CMceSipEvent::NewL( *iConnection, *this );
	iSIPEvent->SetEventStateType( EMoSubscribe );
	MCE_DELETE( iSIPEvent->iEvent );
    iEvent = CMceComEvent::NewL( *iSIPEvent,
							     KDefaultExpires );
    iSIPEvent->iEvent = iEvent;
    iSIPEvent->SetCurrentState( KMceEstablishedEventStateIndex );
    } 

void UT_CMceEventSubscribeMoEstablishedState::SetupFullL(  )
    {
    iEventState = 0;
    iReceived = 0;
    iState = new( ELeave ) CMceEventSubscribeMoEstablishedState();
    SIPStrings::OpenL();
    iSipProfile = CSIPProfile::NewL();
	iConnection = CMceSipConnection::NewL( *iSipProfile );
	iSIPEvent = CMceSipEvent::NewL( *iConnection, *this );
	iSIPEvent->SetEventStateType( EMoSubscribe );
	MCE_DELETE( iSIPEvent->iEvent );
    iEvent = CMceComEvent::NewL( *iSIPEvent,
							     KDefaultExpires );
    iSIPEvent->iEvent = iEvent;
    iSIPEvent->SetCurrentState( KMceEstablishedEventStateIndex );
    
    // create and set dialog
    CSIPEventHeader* eventHdr = CSIPEventHeader::DecodeL( KEventPackage );
    CleanupStack::PushL( eventHdr );
    CSIPSubscribeDialogAssoc* subscrDlg = CSIPSubscribeDialogAssoc::NewL( eventHdr );
    CleanupStack::Pop( eventHdr );
    iSIPEvent->SetDialog( subscrDlg );
    
    // Create server transaction
    CSIPServerTransaction* serverTran = CSIPServerTransaction::NewL();
    serverTran->SetType( 3 ); // NOTIFY
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


void UT_CMceEventSubscribeMoEstablishedState::Teardown(  )
    {
    delete iState;
  
    delete iSIPEvent;
    iEvent = NULL;
    
    delete iConnection;
    
    delete iSipProfile;
    
	SIPStrings::Close();
    }
    
void UT_CMceEventSubscribeMoEstablishedState::SetupEmptyL(  )
    {
    
    } 

void UT_CMceEventSubscribeMoEstablishedState::UT_CMceEventSubscribeMoEstablishedState_HandleLL()
    {
    // Failing with no dialog
    EUNIT_ASSERT_LEAVE( iState->HandleL( *iEvent ) );
    
    // Create and set notify dialog
    CSIPNotifyDialogAssoc* notifyDlg = CSIPNotifyDialogAssoc::NewL();
    iSIPEvent->SetDialog( notifyDlg );
    
    // Failing with wrong dialog
    EUNIT_ASSERT_LEAVE( iState->HandleL( *iEvent ) );
    
    // Create and set subscribe dialog
    delete iSIPEvent->iDialog;
    iSIPEvent->iDialog = NULL;
    
    CSIPEventHeader* eventHdr = CSIPEventHeader::DecodeL( KEventPackage );
    CleanupStack::PushL( eventHdr );
    CSIPSubscribeDialogAssoc* subscrDlg = CSIPSubscribeDialogAssoc::NewL( eventHdr );
    CleanupStack::Pop( eventHdr );
    iSIPEvent->SetDialog( subscrDlg );
    
    // Failing with wrong action
    iEvent->SetPreviousAction( EMceItcReferEvent );
    EUNIT_ASSERT_LEAVE( iState->HandleL( *iEvent ) );
    }
        
void UT_CMceEventSubscribeMoEstablishedState::UT_CMceEventSubscribeMoEstablishedState_HandleL2L()
    {
    // update
    iEvent->SetPreviousAction( EMceItcSubscribeUpdate );
    iState->HandleL( *iEvent );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceEstablishingEventStateIndex );
    
    }

void UT_CMceEventSubscribeMoEstablishedState::UT_CMceEventSubscribeMoEstablishedState_HandleL3L()
    {
    // terminate
    iEvent->SetPreviousAction( EMceItcTerminateEvent );
    iState->HandleL( *iEvent );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceTerminatingEventStateIndex );
    
    }

void UT_CMceEventSubscribeMoEstablishedState::UT_CMceEventSubscribeMoEstablishedState_HandleL4L()
    {
    // update with content
    HBufC8* contentType = KContentType().AllocLC();
    HBufC8* content = KContent().AllocLC(); 
    
    iEvent->SetMsgContentL( contentType, content ); 
    CleanupStack::Pop( content );
    CleanupStack::Pop( contentType );
    iEvent->SetPreviousAction( EMceItcSubscribeUpdate );
    iState->HandleL( *iEvent );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceEstablishingEventStateIndex );
    
    }
    
void UT_CMceEventSubscribeMoEstablishedState::UT_CMceEventSubscribeMoEstablishedState_HandleReceiveRequestLL()
    {
    // Failing with no dialog
    EUNIT_ASSERT_LEAVE( iState->HandleReceiveRequestL( *iEvent ) );
    
    // The leaving cases.
    // Create and set notify dialog
    CSIPReferDialogAssoc* notifyDlg = CSIPReferDialogAssoc::NewL();
    iSIPEvent->SetDialog( notifyDlg );
    
    // Failing with no request
    EUNIT_ASSERT_LEAVE( iState->HandleReceiveRequestL( *iEvent ) );
    
    // Create and set Notify transaction
    CSIPServerTransaction* serverTran = CSIPServerTransaction::NewL();
    serverTran->SetType( 3 ); // NOTIFY
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
    
void UT_CMceEventSubscribeMoEstablishedState::UT_CMceEventSubscribeMoEstablishedState_HandleReceiveRequestL2L()
    {
    // No message elements
    iState->HandleReceiveRequestL( *iEvent );
    }
    
void UT_CMceEventSubscribeMoEstablishedState::UT_CMceEventSubscribeMoEstablishedState_HandleReceiveRequestL3L()
    {
    // With contenttype, subscription state active && event
    RPointerArray<CSIPHeaderBase> hdrs;
    CSIPHeaderBase::PushLC( &hdrs );
    
    CSIPContentTypeHeader* ct = CSIPContentTypeHeader::DecodeL( KContentType );
    CleanupStack::PushL( ct );
    HBufC8* c = KContent().AllocLC();
    iMsgElem->SetContentL( c, ct );
    CleanupStack::Pop( c );
    CleanupStack::Pop( ct );
    
    
    CSIPSubscriptionStateHeader* ss = CSIPSubscriptionStateHeader::DecodeL( KSubscrStateActive );
    CleanupStack::PushL( ss );
    hdrs.AppendL( ss );
    CleanupStack::Pop( ss );
    
    CSIPEventHeader* e = CSIPEventHeader::DecodeL( KEventPackage );
    CleanupStack::PushL( e );
    hdrs.AppendL( e );
    CleanupStack::Pop( e );
    
    iMsgElem->SetUserHeadersL( hdrs );
    CleanupStack::Pop( &hdrs );
    iState->HandleReceiveRequestL( *iEvent );
    EUNIT_ASSERT( iReceived == ENotifyRequest );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceEstablishedEventStateIndex);
    
    }

void UT_CMceEventSubscribeMoEstablishedState::UT_CMceEventSubscribeMoEstablishedState_HandleReceiveRequestL4L()
    {
    // subscriptionstate terminated && event header
    RPointerArray<CSIPHeaderBase> hdrs;
    CSIPHeaderBase::PushLC( &hdrs );
    
    CSIPSubscriptionStateHeader* ss = CSIPSubscriptionStateHeader::DecodeL( KSubscrStateTerminated );
    CleanupStack::PushL( ss );
    hdrs.AppendL( ss );
    CleanupStack::Pop( ss );
    
    CSIPEventHeader* e = CSIPEventHeader::DecodeL( KEventPackage );
    CleanupStack::PushL( e );
    hdrs.AppendL( e );
    CleanupStack::Pop( e );
    
    iMsgElem->SetUserHeadersL( hdrs );
    CleanupStack::Pop(&hdrs);
    
    iState->HandleReceiveRequestL( *iEvent );
 
    EUNIT_ASSERT( iReceived == ENotifyRequest );
    EUNIT_ASSERT( iEventState == CMceEvent::ETerminated );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceTerminatedEventStateIndex );
    }
    
void UT_CMceEventSubscribeMoEstablishedState::UT_CMceEventSubscribeMoEstablishedState_HandleResponseReceivedLL()
    {
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
    
    // create and set response 202 Accepted response
    _LIT8 ( KAccepted, "Accepted" );
    RStringF accepted = SIPStrings::Pool().OpenFStringL( KAccepted );
    CleanupClosePushL( accepted );

    CSIPResponseElements* resp = CSIPResponseElements::NewL( 202,accepted );
    clientTran->SetResponseElements( resp );
    // Should not fail with 202 Accepted response
    EUNIT_ASSERT_NO_LEAVE( iState->HandleResponseReceivedL( *iEvent ) );
    CleanupStack::PopAndDestroy(); // accepted
    
    // create and set response 301  Moved Permanently response
    _LIT8 ( KMoved, "Moved Permanently" );
    RStringF moved = SIPStrings::Pool().OpenFStringL( KMoved );
    CleanupClosePushL( moved );
    CSIPResponseElements* resp2 = CSIPResponseElements::NewL( 301,moved );
    clientTran->SetResponseElements( resp2 );
    
    // Should fail with 301 Moved Permanently response
    EUNIT_ASSERT_LEAVE( iState->HandleResponseReceivedL( *iEvent ) ); 
    CleanupStack::PopAndDestroy(); // moved
    }
    

void UT_CMceEventSubscribeMoEstablishedState::IncomingSubscribeL(
					CDesC8Array* aHeaders,
                    HBufC8* aContentType,
					HBufC8* aBody)
	{
	iReceived = ESubscribeRequest;
	delete aHeaders;
	delete aContentType;
	delete aBody;	
	}
void UT_CMceEventSubscribeMoEstablishedState::IncomingReferL(
					HBufC8* aReferTo,
					CDesC8Array* aHeaders,
                    HBufC8* aContentType,
					HBufC8* aBody)
	{
	iReceived = EReferRequest;
	if ( aReferTo )
	    delete aReferTo;
	
	if ( aHeaders )
	    delete aHeaders;
	
	if ( aContentType )
	    delete aContentType;
	
	if ( aBody )
	    delete aBody;	
	}
void UT_CMceEventSubscribeMoEstablishedState::EventStateChangedL(TUint32 aState)
	{
	iEventState = aState;
	}
		
void UT_CMceEventSubscribeMoEstablishedState::NotifyReceivedL(CDesC8Array* aHeaders,
                    		HBufC8* aContentType,
							HBufC8* aBody)
	{
	iReceived = ENotifyRequest;
	delete aHeaders;
	delete aContentType;
	delete aBody;	
	}
									
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceEventSubscribeMoEstablishedState,
    "UT_CMceEventSubscribeMoEstablishedState",
    "UNIT" )

EUNIT_TEST(
    "HandleL - test ",
    "CMceEventSubscribeMoEstablishedState",
    "HandleL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeMoEstablishedState_HandleLL, Teardown)

EUNIT_TEST(
    "HandleL - test - update ",
    "CMceEventSubscribeMoEstablishedState",
    "HandleL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventSubscribeMoEstablishedState_HandleL2L, Teardown)

EUNIT_TEST(
    "HandleL - test - terminate",
    "CMceEventSubscribeMoEstablishedState",
    "HandleL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventSubscribeMoEstablishedState_HandleL3L, Teardown)

EUNIT_TEST(
    "HandleL - test - update with content",
    "CMceEventSubscribeMoEstablishedState",
    "HandleL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventSubscribeMoEstablishedState_HandleL4L, Teardown)


EUNIT_TEST(
    "HandleReceiveRequestL - test ",
    "CMceEventSubscribeMoEstablishedState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeMoEstablishedState_HandleReceiveRequestLL, Teardown)

EUNIT_TEST(
    "HandleReceiveRequestL - test 2",
    "CMceEventSubscribeMoEstablishedState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventSubscribeMoEstablishedState_HandleReceiveRequestL2L, Teardown)

EUNIT_TEST(
    "HandleReceiveRequestL - test 3",
    "CMceEventSubscribeMoEstablishedState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventSubscribeMoEstablishedState_HandleReceiveRequestL3L, Teardown)

EUNIT_TEST(
    "HandleReceiveRequestL - test 4",
    "CMceEventSubscribeMoEstablishedState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventSubscribeMoEstablishedState_HandleReceiveRequestL4L, Teardown)
    
EUNIT_TEST(
    "HandleResponseReceivedL - test ",
    "CMceEventSubscribeMoEstablishedState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeMoEstablishedState_HandleResponseReceivedLL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
