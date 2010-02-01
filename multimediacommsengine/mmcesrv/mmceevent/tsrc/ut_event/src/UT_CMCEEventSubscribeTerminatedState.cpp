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
#include "ut_cmceeventsubscribeterminatedstate.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <sipstrings.h>
#include <uri8.h>
#include <sipcontenttypeheader.h>
#include <sipsubscriptionstateheader.h>
#include <sipmessageelements.h>


//  INTERNAL INCLUDES
#include "mceeventsubscribeterminatedstate.h"
#include "MCEEventTestConsts.h"
#include "sipreferdialogassoc.h"
#include "sipsubscribedialogassoc.h"
#include "sipeventheader.h"
#include "siprequestelements.h"
#include "sipprofile.h"
#include "mcesipconnection.h"
#include "mceserial.h"


// CONSTRUCTION
UT_CMceEventSubscribeTerminatedState* UT_CMceEventSubscribeTerminatedState::NewL()
    {
    UT_CMceEventSubscribeTerminatedState* self = UT_CMceEventSubscribeTerminatedState::NewLC(); 
    CleanupStack::Pop(self);

    return self;
    }

UT_CMceEventSubscribeTerminatedState* UT_CMceEventSubscribeTerminatedState::NewLC()
    {
    UT_CMceEventSubscribeTerminatedState* self = new( ELeave ) UT_CMceEventSubscribeTerminatedState();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceEventSubscribeTerminatedState::~UT_CMceEventSubscribeTerminatedState()
    {
    }

// Default constructor
UT_CMceEventSubscribeTerminatedState::UT_CMceEventSubscribeTerminatedState()
    {
    }

// Second phase construct
void UT_CMceEventSubscribeTerminatedState::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceEventSubscribeTerminatedState::SetupL(  )
    {
    iReceived = 0;
    iEventState = 0;
    
    iState = new( ELeave ) CMceEventSubscribeTerminatedState();
    SIPStrings::OpenL();
    iSipProfile = CSIPProfile::NewL();
	iConnection = CMceSipConnection::NewL( *iSipProfile );
	iSIPEvent = CMceSipEvent::NewL( *iConnection, *this );
	iSIPEvent->SetEventStateType( EMoSubscribe );
	MCE_DELETE( iSIPEvent->iEvent );
    iEvent = CMceComEvent::NewL( *iSIPEvent, KDefaultExpires );
	iSIPEvent->iEvent = iEvent;
    iSIPEvent->SetCurrentState( KMceTerminatedEventStateIndex );
    
    
    } 

void UT_CMceEventSubscribeTerminatedState::SetupFullL(  )
    {
    iReceived = 0;
    iEventState = 0;
    
    iState = new( ELeave ) CMceEventSubscribeTerminatedState();
    SIPStrings::OpenL();
    iSipProfile = CSIPProfile::NewL();
	iConnection = CMceSipConnection::NewL( *iSipProfile );
	iSIPEvent = CMceSipEvent::NewL( *iConnection, *this );
	iSIPEvent->SetEventStateType( EMoSubscribe );
    MCE_DELETE( iSIPEvent->iEvent );
    iEvent = CMceComEvent::NewL( *iSIPEvent,
								 KDefaultExpires );
	iSIPEvent->iEvent = iEvent;
    iSIPEvent->SetCurrentState( KMceTerminatedEventStateIndex );
    
    CSIPEventHeader* eventHdr = CSIPEventHeader::DecodeL( KEventPackage );
    CleanupStack::PushL( eventHdr );
    CSIPSubscribeDialogAssoc* subscrDlg = CSIPSubscribeDialogAssoc::NewL( eventHdr );
    CleanupStack::Pop( eventHdr );
    iSIPEvent->SetDialog( subscrDlg );
    
    CSIPServerTransaction* serverTran = CSIPServerTransaction::NewL();
    serverTran->SetType( 3 ); // Notify
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


void UT_CMceEventSubscribeTerminatedState::Teardown(  )
    {
    delete iState;
    
    delete iSIPEvent;
	iEvent = NULL;
    
    delete iConnection;
    
    delete iSipProfile;
    	
	SIPStrings::Close();
    }

void UT_CMceEventSubscribeTerminatedState::SetupEmptyL(  )
    {
    
    } 

void UT_CMceEventSubscribeTerminatedState::UT_CMceEventSubscribeTerminatedState_HandleLL()
    {
    EUNIT_ASSERT_LEAVE( iState->HandleL( *iEvent ) ); 
    }
        
void UT_CMceEventSubscribeTerminatedState::UT_CMceEventSubscribeTerminatedState_HandleReceiveRequestLL()
    {
    // The leaving cases.
    // Failing with no dialog
    EUNIT_ASSERT_LEAVE( iState->HandleReceiveRequestL( *iEvent ) );
    
    // Create and set Subscribe dialog
    CSIPEventHeader* eventHdr = CSIPEventHeader::DecodeL( KEventPackage );
    CleanupStack::PushL( eventHdr );
    CSIPSubscribeDialogAssoc* subscrDlg = CSIPSubscribeDialogAssoc::NewL( eventHdr );
    CleanupStack::Pop( eventHdr );
    iSIPEvent->SetDialog( subscrDlg );
    
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
    
    // Failing with no request elements
    EUNIT_ASSERT_LEAVE( iState->HandleReceiveRequestL( *iEvent ) );
    
    // Create request elements
    TUriParser8 uriParser;
    User::LeaveIfError( uriParser.Parse( KRemoteUri() ) );
    CUri8* remoteUri = CUri8::NewL(uriParser);
    CleanupStack::PushL(remoteUri);
    
    CSIPRequestElements* elem =CSIPRequestElements::NewL( remoteUri );
    CleanupStack::Pop( remoteUri );
    serverTran->SetRequestElements( elem );
    
    // failing, bad NOTIFY
    //EUNIT_ASSERT_LEAVE( iState->HandleReceiveRequestL( *iEvent ) );
    TRAPD( error, iState->HandleReceiveRequestL( *iEvent ) );
    EUNIT_ASSERT( error );
    if ( error != KErrNoMemory )
        {
        EUNIT_ASSERT( iReceived == ENotifyRequest );      
        }
    }

void UT_CMceEventSubscribeTerminatedState::UT_CMceEventSubscribeTerminatedState_HandleReceiveRequestL2L()
    {
    // With active subscription state
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
    
    //EUNIT_ASSERT_LEAVE( iState->HandleReceiveRequestL( *iEvent ) );
    
    // EUNIT_ASSERT( iReceived == ENotifyRequest );

    TRAPD( error, iState->HandleReceiveRequestL( *iEvent ) );
    EUNIT_ASSERT( error );
    if ( error != KErrNoMemory )
        {
        EUNIT_ASSERT( iReceived == ENotifyRequest );      
        }
    }

void UT_CMceEventSubscribeTerminatedState::UT_CMceEventSubscribeTerminatedState_HandleReceiveRequestL3L()
    {
    // With terminated subscriptions state
    CSIPContentTypeHeader* ct = CSIPContentTypeHeader::DecodeL( KContentType );
    CleanupStack::PushL( ct );
    HBufC8* c = KContent().AllocLC();
    iMsgElem->SetContentL( c, ct );
    CleanupStack::Pop( c );
    CleanupStack::Pop( ct );
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
    CleanupStack::Pop( &hdrs );
    iState->HandleReceiveRequestL( *iEvent );
    EUNIT_ASSERT( iReceived == ENotifyRequest );
    EUNIT_ASSERT( iEventState == CMceEvent::ETerminated );
    }
           
void UT_CMceEventSubscribeTerminatedState::UT_CMceEventSubscribeTerminatedState_HandleResponseReceivedLL()
    {
    EUNIT_ASSERT_NO_LEAVE( iState->HandleResponseReceivedL( *iEvent ) ); 
    }

void UT_CMceEventSubscribeTerminatedState::IncomingSubscribeL(
					CDesC8Array* aHeaders,
                    HBufC8* aContentType,
					HBufC8* aBody)
	{
	iReceived = ESubscribeRequest;
	delete aHeaders;
	delete aContentType;
	delete aBody;	
	}
void UT_CMceEventSubscribeTerminatedState::IncomingReferL(
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
void UT_CMceEventSubscribeTerminatedState::EventStateChangedL(TUint32 aState)
	{
	iEventState = aState;
	}
		
void UT_CMceEventSubscribeTerminatedState::NotifyReceivedL(CDesC8Array* aHeaders,
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
    UT_CMceEventSubscribeTerminatedState,
    "UT_CMceEventSubscribeTerminatedState",
    "UNIT" )

EUNIT_TEST(
    "HandleL - test ",
    "CMceEventSubscribeTerminatedState",
    "HandleL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeTerminatedState_HandleLL, Teardown)

EUNIT_TEST(
    "HandleReceiveRequestL - test ",
    "CMceEventSubscribeTerminatedState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeTerminatedState_HandleReceiveRequestLL, Teardown)

EUNIT_TEST(
    "HandleReceiveRequestL - test 2",
    "CMceEventSubscribeTerminatedState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventSubscribeTerminatedState_HandleReceiveRequestL2L, Teardown)

EUNIT_TEST(
    "HandleReceiveRequestL - test 3",
    "CMceEventSubscribeTerminatedState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventSubscribeTerminatedState_HandleReceiveRequestL3L, Teardown)

    
EUNIT_TEST(
    "HandleResponseReceivedL - test ",
    "CMceEventSubscribeTerminatedState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeTerminatedState_HandleResponseReceivedLL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
