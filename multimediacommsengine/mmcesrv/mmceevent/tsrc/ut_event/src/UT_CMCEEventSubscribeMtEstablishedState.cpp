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
#include "ut_cmceeventsubscribemtestablishedstate.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <sipstrings.h>
#include <sipexpiresheader.h>
#include <uri8.h>
#include <siprequestelements.h>
#include <sipmessageelements.h>
#include <sipresponseelements.h>

//  INTERNAL INCLUDES
#include "mceeventsubscribemtestablishedstate.h"
#include "MCEEventTestConsts.h"
#include "sipreferdialogassoc.h"
#include "sipsubscribedialogassoc.h"
#include "sipeventheader.h"
#include "sipnotifydialogassoc.h"
#include "sipprofile.h"
#include "mcesipconnection.h"
#include "mceserial.h"

// CONSTRUCTION
UT_CMceEventSubscribeMtEstablishedState* UT_CMceEventSubscribeMtEstablishedState::NewL()
    {
    UT_CMceEventSubscribeMtEstablishedState* self = UT_CMceEventSubscribeMtEstablishedState::NewLC(); 
    CleanupStack::Pop(self);

    return self;
    }

UT_CMceEventSubscribeMtEstablishedState* UT_CMceEventSubscribeMtEstablishedState::NewLC()
    {
    UT_CMceEventSubscribeMtEstablishedState* self = new( ELeave ) UT_CMceEventSubscribeMtEstablishedState();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceEventSubscribeMtEstablishedState::~UT_CMceEventSubscribeMtEstablishedState()
    {
    }

// Default constructor
UT_CMceEventSubscribeMtEstablishedState::UT_CMceEventSubscribeMtEstablishedState()
    {
    }

// Second phase construct
void UT_CMceEventSubscribeMtEstablishedState::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceEventSubscribeMtEstablishedState::SetupL(  )
    {
    iEventState = 0;
    iState = new( ELeave ) CMceEventSubscribeMtEstablishedState();
    SIPStrings::OpenL();
    iSipProfile = CSIPProfile::NewL();
	iConnection = CMceSipConnection::NewL( *iSipProfile );
	iSIPEvent = CMceSipEvent::NewL( *iConnection, *this );
	iSIPEvent->SetEventStateType( EMtSubscribe );
	MCE_DELETE( iSIPEvent->iEvent );
    iEvent = CMceComEvent::NewL( *iSIPEvent,
							     KDefaultExpires );
    iSIPEvent->iEvent = iEvent;
    iSIPEvent->SetCurrentState( KMceEstablishedEventStateIndex );
    
    } 

void UT_CMceEventSubscribeMtEstablishedState::SetupFullL(  )
    {
    iEventState = 0;
    iState = new( ELeave ) CMceEventSubscribeMtEstablishedState();
    SIPStrings::OpenL();
    iSipProfile = CSIPProfile::NewL();
	iConnection = CMceSipConnection::NewL( *iSipProfile );
	iSIPEvent = CMceSipEvent::NewL( *iConnection, *this );
	iSIPEvent->SetEventStateType( EMtSubscribe );
	MCE_DELETE( iSIPEvent->iEvent );
    iEvent = CMceComEvent::NewL( *iSIPEvent,
							     KDefaultExpires );
    iSIPEvent->iEvent = iEvent;
    iSIPEvent->SetCurrentState( KMceEstablishedEventStateIndex );
    CSIPNotifyDialogAssoc* dlg = CSIPNotifyDialogAssoc::NewL();
    iSIPEvent->SetDialog( dlg );
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


void UT_CMceEventSubscribeMtEstablishedState::Teardown(  )
    {
    delete iState;
    
    delete iSIPEvent;
    iEvent = NULL;
    
    delete iConnection;
    
    delete iSipProfile;
    	
	SIPStrings::Close();
    }

void UT_CMceEventSubscribeMtEstablishedState::SetupEmptyL(  )
    {
    
    } 

void UT_CMceEventSubscribeMtEstablishedState::UT_CMceEventSubscribeMtEstablishedState_HandleLL()
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
 
void UT_CMceEventSubscribeMtEstablishedState::UT_CMceEventSubscribeMtEstablishedState_HandleL2L()
    {
    // notify
    iEvent->SetPreviousAction( EMceItcNotify );
    //EUNIT_ASSERT_NO_LEAVE( iState->HandleL( *iEvent ) );
    iState->HandleL( *iEvent );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceEstablishingEventStateIndex);
    
    
    }

void UT_CMceEventSubscribeMtEstablishedState::UT_CMceEventSubscribeMtEstablishedState_HandleL3L()
    {
    // terminate
    iEvent->SetPreviousAction( EMceItcTerminateEvent );
    //EUNIT_ASSERT_NO_LEAVE( iState->HandleL( *iEvent ) );
    iState->HandleL( *iEvent );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceTerminatingEventStateIndex);
    
    }

void UT_CMceEventSubscribeMtEstablishedState::UT_CMceEventSubscribeMtEstablishedState_HandleL4L()
    {
    // notify with content
    HBufC8* ct = KContentType().AllocLC();
    HBufC8* c = KContent().AllocLC(); 
    
    iEvent->SetMsgContentL( ct, c ); 
    CleanupStack::Pop( c );
    CleanupStack::Pop( ct );
    iEvent->SetPreviousAction( EMceItcNotify );
    iState->HandleL( *iEvent );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceEstablishingEventStateIndex);
    
    }

void UT_CMceEventSubscribeMtEstablishedState::UT_CMceEventSubscribeMtEstablishedState_HandleReceiveRequestLL()
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

void UT_CMceEventSubscribeMtEstablishedState::UT_CMceEventSubscribeMtEstablishedState_HandleReceiveRequestL2L()
    {
    // !event -> send 400
    iState->HandleReceiveRequestL( *iEvent );
    
    }

void UT_CMceEventSubscribeMtEstablishedState::UT_CMceEventSubscribeMtEstablishedState_HandleReceiveRequestL3L()
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

void UT_CMceEventSubscribeMtEstablishedState::UT_CMceEventSubscribeMtEstablishedState_HandleReceiveRequestL4L()
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

void UT_CMceEventSubscribeMtEstablishedState::UT_CMceEventSubscribeMtEstablishedState_HandleReceiveRequestL5L()
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
void UT_CMceEventSubscribeMtEstablishedState::UT_CMceEventSubscribeMtEstablishedState_HandleResponseReceivedLL()
    {
    // Leaving cases
    // Failing with no client transaction.
    EUNIT_ASSERT_LEAVE( iState->HandleResponseReceivedL( *iEvent ) );
    
    // Failing with wrong client transaction
    CSIPClientTransaction* clientTran = CSIPClientTransaction::NewL();
    CleanupStack::PushL( clientTran );
    clientTran->SetType( 1 );// subscribe
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
    
    // Failing with no response elements-
    EUNIT_ASSERT_LEAVE( iState->HandleResponseReceivedL( *iEvent ) );
    }

void UT_CMceEventSubscribeMtEstablishedState::UT_CMceEventSubscribeMtEstablishedState_HandleResponseReceivedL2L()
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
    EUNIT_ASSERT( iEventState == CMceEvent::EActive );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceEstablishedEventStateIndex );
    CleanupStack::PopAndDestroy();
    }
    
void UT_CMceEventSubscribeMtEstablishedState::UT_CMceEventSubscribeMtEstablishedState_HandleResponseReceivedL3L()
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
    EUNIT_ASSERT( iEventState == CMceEvent::ETerminated );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceTerminatedEventStateIndex );
    CleanupStack::PopAndDestroy();
    }

void UT_CMceEventSubscribeMtEstablishedState::UT_CMceEventSubscribeMtEstablishedState_HandleResponseReceivedL4L()
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

    
void UT_CMceEventSubscribeMtEstablishedState::IncomingSubscribeL(
					CDesC8Array* aHeaders,
                    HBufC8* aContentType,
					HBufC8* aBody)
	{
	delete aHeaders;
	delete aContentType;
	delete aBody;	
	}
void UT_CMceEventSubscribeMtEstablishedState::IncomingReferL(
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
void UT_CMceEventSubscribeMtEstablishedState::EventStateChangedL(TUint32 aState)
	{
	iEventState = aState;
	}
		
void UT_CMceEventSubscribeMtEstablishedState::NotifyReceivedL(CDesC8Array* aHeaders,
                    		HBufC8* aContentType,
							HBufC8* aBody)
	{
	delete aHeaders;
	delete aContentType;
	delete aBody;	
	}

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceEventSubscribeMtEstablishedState,
    "UT_CMceEventSubscribeMtEstablishedState",
    "UNIT" )

EUNIT_TEST(
    "HandleL - test ",
    "CMceEventSubscribeMtEstablishedState",
    "HandleL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeMtEstablishedState_HandleLL, Teardown)

EUNIT_TEST(
    "HandleL - test - notify",
    "CMceEventSubscribeMtEstablishedState",
    "HandleL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventSubscribeMtEstablishedState_HandleL2L, Teardown)

EUNIT_TEST(
    "HandleL - test - terminate",
    "CMceEventSubscribeMtEstablishedState",
    "HandleL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventSubscribeMtEstablishedState_HandleL3L, Teardown)

EUNIT_TEST(
    "HandleL - test - notify with content",
    "CMceEventSubscribeMtEstablishedState",
    "HandleL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventSubscribeMtEstablishedState_HandleL4L, Teardown)


EUNIT_TEST(
    "HandleReceiveRequestL - test ",
    "CMceEventSubscribeMtEstablishedState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeMtEstablishedState_HandleReceiveRequestLL, Teardown)

EUNIT_TEST(
    "HandleReceiveRequestL - test 2",
    "CMceEventSubscribeMtEstablishedState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventSubscribeMtEstablishedState_HandleReceiveRequestL2L, Teardown)

EUNIT_TEST(
    "HandleReceiveRequestL - test 3",
    "CMceEventSubscribeMtEstablishedState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventSubscribeMtEstablishedState_HandleReceiveRequestL3L, Teardown)

EUNIT_TEST(
    "HandleReceiveRequestL - test 4",
    "CMceEventSubscribeMtEstablishedState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventSubscribeMtEstablishedState_HandleReceiveRequestL4L, Teardown)
    
EUNIT_TEST(
    "HandleReceiveRequestL - test 5",
    "CMceEventSubscribeMtEstablishedState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventSubscribeMtEstablishedState_HandleReceiveRequestL5L, Teardown)

EUNIT_TEST(
    "HandleResponseReceivedL - test ",
    "CMceEventSubscribeMtEstablishedState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeMtEstablishedState_HandleResponseReceivedLL, Teardown)

EUNIT_TEST(
    "HandleResponseReceivedL - test - accepted ",
    "CMceEventSubscribeMtEstablishedState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeMtEstablishedState_HandleResponseReceivedL2L, Teardown)

EUNIT_TEST(
    "HandleResponseReceivedL - test - rejected ",
    "CMceEventSubscribeMtEstablishedState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeMtEstablishedState_HandleResponseReceivedL3L, Teardown)

EUNIT_TEST(
    "HandleResponseReceivedL - test - provisional ",
    "CMceEventSubscribeMtEstablishedState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeMtEstablishedState_HandleResponseReceivedL4L, Teardown)



EUNIT_END_TEST_TABLE

//  END OF FILE
