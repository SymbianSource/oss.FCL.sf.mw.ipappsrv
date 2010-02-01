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
#include "ut_cmceeventrefermoestablishedstate.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <sipstrings.h>
#include <sipcontenttypeheader.h>
#include <sipheaderbase.h>
#include <sipeventheader.h>
#include <sipsubscriptionstateheader.h>
#include <e32base.h>
#include <sipresponseelements.h>

//  INTERNAL INCLUDES
#include "mceeventrefermoestablishedstate.h"
#include "MCEEventTestConsts.h"
#include "mcesipevent.h"
#include "sipreferdialogassoc.h"
#include "sipsubscribedialogassoc.h"
#include "sipservertransaction.h"
#include "sipmessageelements.h"
#include "siprequestelements.h"
#include "sipprofile.h"
#include "mcesipconnection.h"
#include "mceserial.h"


// CONSTRUCTION
UT_CMceEventReferMoEstablishedState* UT_CMceEventReferMoEstablishedState::NewL()
    {
    UT_CMceEventReferMoEstablishedState* self = UT_CMceEventReferMoEstablishedState::NewLC(); 
    CleanupStack::Pop(self);

    return self;
    }

UT_CMceEventReferMoEstablishedState* UT_CMceEventReferMoEstablishedState::NewLC()
    {
    UT_CMceEventReferMoEstablishedState* self = new( ELeave ) UT_CMceEventReferMoEstablishedState();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceEventReferMoEstablishedState::~UT_CMceEventReferMoEstablishedState()
    {
    }

// Default constructor
UT_CMceEventReferMoEstablishedState::UT_CMceEventReferMoEstablishedState()
    {
    }

// Second phase construct
void UT_CMceEventReferMoEstablishedState::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS
void UT_CMceEventReferMoEstablishedState::IncomingSubscribeL(
					CDesC8Array* aHeaders,
                    HBufC8* aContentType,
					HBufC8* aBody)
	{
	iReceived = ESubscribeRequest;
	delete aHeaders;
	delete aContentType;
	delete aBody;	
	}
void UT_CMceEventReferMoEstablishedState::IncomingReferL(
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
void UT_CMceEventReferMoEstablishedState::EventStateChangedL(TUint32 aState)
	{
	iEventState = aState;
	}
		
void UT_CMceEventReferMoEstablishedState::NotifyReceivedL(CDesC8Array* aHeaders,
                    		HBufC8* aContentType,
							HBufC8* aBody)
	{
	iReceived = ENotifyRequest;
	delete aHeaders;
	delete aContentType;
	delete aBody;	
	}
									
void UT_CMceEventReferMoEstablishedState::SetupL(  )
    {
    iEventState = 0;
    iReceived = 0;
    iState = new( ELeave ) CMceEventReferMoEstablishedState();
    SIPStrings::OpenL();
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
    
void UT_CMceEventReferMoEstablishedState::SetupFullL(  )
    {
    iEventState = 0;
    iReceived = 0;
    iState = new( ELeave ) CMceEventReferMoEstablishedState();
    SIPStrings::OpenL();
    iSipProfile = CSIPProfile::NewL();
	iConnection = CMceSipConnection::NewL( *iSipProfile );
	iSIPEvent = CMceSipEvent::NewL( *iConnection, *this );
	iSIPEvent->SetEventStateType( EMoRefer );
    MCE_DELETE( iSIPEvent->iEvent );
    iEvent = CMceComEvent::NewL( *iSIPEvent,
							     KDefaultExpires );
    iSIPEvent->iEvent = iEvent;
    iSIPEvent->SetCurrentState( KMceEstablishedEventStateIndex );
	// create dialog
    CSIPReferDialogAssoc* referDlg = CSIPReferDialogAssoc::NewL();
    iSIPEvent->SetDialog( referDlg );
    
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


void UT_CMceEventReferMoEstablishedState::Teardown(  )
    {
    delete iState;
    
    delete iSIPEvent;
	iEvent = NULL;
    
    delete iConnection;
    
    delete iSipProfile;
    
	SIPStrings::Close();
    }

void UT_CMceEventReferMoEstablishedState::SetupEmptyL(  )
    {
    
    } 

void UT_CMceEventReferMoEstablishedState::UT_CMceEventReferMoEstablishedState_HandleLL()
    {
    // Failing with wrong action
    iEvent->SetPreviousAction( EMceItcSubscribe );
    EUNIT_ASSERT_LEAVE( iState->HandleL( *iEvent ) );
    
    // Successfull
    iEvent->SetPreviousAction( EMceItcTerminateEvent );
    EUNIT_ASSERT_NO_LEAVE( iState->HandleL( *iEvent ) );
    }
        
void UT_CMceEventReferMoEstablishedState::UT_CMceEventReferMoEstablishedState_HandleReceiveRequestLL()
    {
    // Failing with no dialog
    EUNIT_ASSERT_LEAVE( iState->HandleReceiveRequestL( *iEvent ) );
    
    // The leaving cases.
    // Create and set refer dialog
    CSIPReferDialogAssoc* referDlg = CSIPReferDialogAssoc::NewL();
    iSIPEvent->SetDialog( referDlg );
    
    // Failing with no request
    EUNIT_ASSERT_LEAVE( iState->HandleReceiveRequestL( *iEvent ) );
    
    // Create and set Notify transaction
    CSIPServerTransaction* serverTran2 = CSIPServerTransaction::NewL();
    serverTran2->SetType( 3 ); // NOTIFY
    iSIPEvent->iRequest = serverTran2;
  
    // Failing with wrong action
    iEvent->SetPreviousAction( EMceItcSubscribe );
    EUNIT_ASSERT_LEAVE( iState->HandleReceiveRequestL( *iEvent ) );
    
    // Set proper action
    iEvent->SetPreviousAction( KErrNotFound );
    
    // Leaving with wrong dialog state
    referDlg->Dialog().SetState( CSIPDialog::ETerminated );
    EUNIT_ASSERT_LEAVE( iState->HandleReceiveRequestL( *iEvent ) );
    referDlg->Dialog().SetState( CSIPDialog::EConfirmed );
    
    // Failing with no request elements
    EUNIT_ASSERT_LEAVE( iState->HandleReceiveRequestL( *iEvent ) );
    
    }
      
void UT_CMceEventReferMoEstablishedState::UT_CMceEventReferMoEstablishedState_HandleReceiveRequestL2L()
    {
    // With only contenttype
    RPointerArray<CSIPHeaderBase> hdrs;
    CSIPHeaderBase::PushLC( &hdrs );
    CSIPContentTypeHeader* ct = CSIPContentTypeHeader::DecodeL( KContentTypeRefer );
    CleanupStack::PushL( ct );
    hdrs.AppendL( ct );
    CleanupStack::Pop( ct );
    iMsgElem->SetUserHeadersL( hdrs );
    CleanupStack::Pop( &hdrs );
    iState->HandleReceiveRequestL( *iEvent );  
    }

void UT_CMceEventReferMoEstablishedState::UT_CMceEventReferMoEstablishedState_HandleReceiveRequestL3L()
    {
    // With contenttype && subscription state
    RPointerArray<CSIPHeaderBase> hdrs;
    CSIPHeaderBase::PushLC( &hdrs );
      
    CSIPContentTypeHeader* ct = CSIPContentTypeHeader::DecodeL( KContentType );
    CleanupStack::PushL( ct );
    hdrs.AppendL( ct );
    CleanupStack::Pop( ct );
    
    CSIPSubscriptionStateHeader* ss = CSIPSubscriptionStateHeader::DecodeL( KSubscrStateActive );
    CleanupStack::PushL( ss );
    hdrs.AppendL( ss );
    CleanupStack::Pop( ss );
    iMsgElem->SetUserHeadersL( hdrs );
    CleanupStack::Pop( &hdrs );
    iState->HandleReceiveRequestL( *iEvent );
    }

void UT_CMceEventReferMoEstablishedState::UT_CMceEventReferMoEstablishedState_HandleReceiveRequestL4L()
    {
    // With contenttype, subscription state active && event
    RPointerArray<CSIPHeaderBase> hdrs;
    CSIPHeaderBase::PushLC( &hdrs );
    HBufC8* content = KContent().AllocLC();
    CSIPContentTypeHeader* ct = CSIPContentTypeHeader::DecodeL( KContentTypeRefer );
    CleanupStack::PushL( ct );
    // put content for coverage
    iMsgElem->SetContentL( content, ct );
    CleanupStack::Pop( ct );
    CleanupStack::Pop( content );
    
    CSIPSubscriptionStateHeader* ss = CSIPSubscriptionStateHeader::DecodeL( KSubscrStateActive );
    CleanupStack::PushL( ss );
    hdrs.AppendL( ss );
    CleanupStack::Pop( ss );
    
    CSIPEventHeader* e = CSIPEventHeader::DecodeL( KReferEventPackage );
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

void UT_CMceEventReferMoEstablishedState::UT_CMceEventReferMoEstablishedState_HandleReceiveRequestL5L()
    {
    // With contenttype, subscription state terminated && event
    RPointerArray<CSIPHeaderBase> hdrs;
    CSIPHeaderBase::PushLC( &hdrs );
    CSIPContentTypeHeader* ct = CSIPContentTypeHeader::DecodeL( KContentType );
    CleanupStack::PushL( ct );
    hdrs.AppendL( ct );
    CleanupStack::Pop( ct );
    
    CSIPSubscriptionStateHeader* ss = CSIPSubscriptionStateHeader::DecodeL( KSubscrStateTerminated );
    CleanupStack::PushL( ss );
    hdrs.AppendL( ss );
    CleanupStack::Pop( ss );
    
    CSIPEventHeader* e = CSIPEventHeader::DecodeL( KReferEventPackage );
    CleanupStack::PushL( e );
    hdrs.AppendL( e );
    CleanupStack::Pop( e );
    iMsgElem->SetUserHeadersL( hdrs );
    CleanupStack::Pop( &hdrs );
    iState->HandleReceiveRequestL( *iEvent );
    EUNIT_ASSERT( iReceived == ENotifyRequest );
    EUNIT_ASSERT( iEventState == CMceRefer::ETerminated );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceTerminatedEventStateIndex );
    }
     
void UT_CMceEventReferMoEstablishedState::UT_CMceEventReferMoEstablishedState_HandleReceiveRequestL6L()
    {
    // With contenttype, subscription state active && event && silent suppression
    iEvent->SetSilentSuppressionOn();
    RPointerArray<CSIPHeaderBase> hdrs;
    CSIPHeaderBase::PushLC( &hdrs );
    HBufC8* content = KContent().AllocLC();
    CSIPContentTypeHeader* ct = CSIPContentTypeHeader::DecodeL( KContentTypeRefer );
    CleanupStack::PushL( ct );
    // put content for coverage
    iMsgElem->SetContentL( content, ct );
    CleanupStack::Pop( ct );
    CleanupStack::Pop( content );
    
    CSIPSubscriptionStateHeader* ss = CSIPSubscriptionStateHeader::DecodeL( KSubscrStateActive );
    CleanupStack::PushL( ss );
    hdrs.AppendL( ss );
    CleanupStack::Pop( ss );
    
    CSIPEventHeader* e = CSIPEventHeader::DecodeL( KReferEventPackage );
    CleanupStack::PushL( e );
    hdrs.AppendL( e );
    CleanupStack::Pop( e );
    
    iMsgElem->SetUserHeadersL( hdrs );
    CleanupStack::Pop( &hdrs );
    iState->HandleReceiveRequestL( *iEvent );
    EUNIT_ASSERT( iReceived == ENoRequest );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceEstablishedEventStateIndex);
    
    }

void UT_CMceEventReferMoEstablishedState::UT_CMceEventReferMoEstablishedState_HandleReceiveRequestL7L()
    {
    // With contenttype, subscription state terminated && event && silent suppression
    iEvent->SetSilentSuppressionOn();
    RPointerArray<CSIPHeaderBase> hdrs;
    CSIPHeaderBase::PushLC( &hdrs );
    CSIPContentTypeHeader* ct = CSIPContentTypeHeader::DecodeL( KContentType );
    CleanupStack::PushL( ct );
    hdrs.AppendL( ct );
    CleanupStack::Pop( ct );
    
    CSIPSubscriptionStateHeader* ss = CSIPSubscriptionStateHeader::DecodeL( KSubscrStateTerminated );
    CleanupStack::PushL( ss );
    hdrs.AppendL( ss );
    CleanupStack::Pop( ss );
    
    CSIPEventHeader* e = CSIPEventHeader::DecodeL( KReferEventPackage );
    CleanupStack::PushL( e );
    hdrs.AppendL( e );
    CleanupStack::Pop( e );
    iMsgElem->SetUserHeadersL( hdrs );
    CleanupStack::Pop( &hdrs );
    iState->HandleReceiveRequestL( *iEvent );
    EUNIT_ASSERT( iReceived == ENoRequest );
    EUNIT_ASSERT( iEventState != CMceRefer::ETerminated );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceTerminatedEventStateIndex );
    }

void UT_CMceEventReferMoEstablishedState::UT_CMceEventReferMoEstablishedState_HandleResponseReceivedLL()
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

	resp->SetStatusCodeL( 301 );
	resp->SetReasonPhraseL( moved );

    // Should fail with 301 Moved Permanently response
    EUNIT_ASSERT_LEAVE( iState->HandleResponseReceivedL( *iEvent ) ); 

    CleanupStack::PopAndDestroy(); // moved
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceEventReferMoEstablishedState,
    "UT_CMceEventReferMoEstablishedState.",
    "UNIT" )

EUNIT_TEST(
    "HandleL - test ",
    "CMceEventReferMoEstablishedState",
    "HandleL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferMoEstablishedState_HandleLL, Teardown)

EUNIT_TEST(
    "HandleReceiveRequestL - test ",
    "CMceEventReferMoEstablishedState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferMoEstablishedState_HandleReceiveRequestLL, Teardown)

EUNIT_TEST(
    "HandleReceiveRequestL - test 2",
    "CMceEventReferMoEstablishedState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventReferMoEstablishedState_HandleReceiveRequestL2L, Teardown)

EUNIT_TEST(
    "HandleReceiveRequestL - test 3",
    "CMceEventReferMoEstablishedState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventReferMoEstablishedState_HandleReceiveRequestL3L, Teardown)

EUNIT_TEST(
    "HandleReceiveRequestL - test 4",
    "CMceEventReferMoEstablishedState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventReferMoEstablishedState_HandleReceiveRequestL4L, Teardown)

EUNIT_TEST(
    "HandleReceiveRequestL - test 5",
    "CMceEventReferMoEstablishedState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventReferMoEstablishedState_HandleReceiveRequestL5L, Teardown)
    
EUNIT_TEST(
    "HandleReceiveRequestL - test 6",
    "CMceEventReferMoEstablishedState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventReferMoEstablishedState_HandleReceiveRequestL6L, Teardown)

EUNIT_TEST(
    "HandleReceiveRequestL - test 7",
    "CMceEventReferMoEstablishedState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventReferMoEstablishedState_HandleReceiveRequestL7L, Teardown)
    
EUNIT_TEST(
    "HandleResponseReceivedL - test ",
    "CMceEventReferMoEstablishedState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferMoEstablishedState_HandleResponseReceivedLL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
