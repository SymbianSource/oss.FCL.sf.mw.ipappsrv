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
#include "ut_cmceeventsubscribemtidlestate.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <sipstrings.h>
#include <sipcontenttypeheader.h>
#include <uri8.h>
#include <siprequestelements.h>
#include <sipmessageelements.h>
#include <sipresponseelements.h>


//  INTERNAL INCLUDES
#include "mceeventsubscribemtidlestate.h"
#include "MCEEventTestConsts.h"
#include "sipreferdialogassoc.h"
#include "sipsubscribedialogassoc.h"
#include "sipeventheader.h"
#include "sipexpiresheader.h"
#include "sipnotifydialogassoc.h"
#include "sipprofile.h"
#include "mcesipconnection.h"
#include "mceserial.h"


// CONSTRUCTION
UT_CMceEventSubscribeMtIdleState* UT_CMceEventSubscribeMtIdleState::NewL()
    {
    UT_CMceEventSubscribeMtIdleState* self = UT_CMceEventSubscribeMtIdleState::NewLC(); 
    CleanupStack::Pop(self);

    return self;
    }

UT_CMceEventSubscribeMtIdleState* UT_CMceEventSubscribeMtIdleState::NewLC()
    {
    UT_CMceEventSubscribeMtIdleState* self = new( ELeave ) UT_CMceEventSubscribeMtIdleState();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceEventSubscribeMtIdleState::~UT_CMceEventSubscribeMtIdleState()
    {
    }

// Default constructor
UT_CMceEventSubscribeMtIdleState::UT_CMceEventSubscribeMtIdleState()
    {
    }

// Second phase construct
void UT_CMceEventSubscribeMtIdleState::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceEventSubscribeMtIdleState::SetupL(  )
    {
    iEventState = 0;
    iReceived = 0;
    iState = new( ELeave ) CMceEventSubscribeMtIdleState();
    SIPStrings::OpenL();
    iSipProfile = CSIPProfile::NewL();
	iConnection = CMceSipConnection::NewL( *iSipProfile );
	iSIPEvent = CMceSipEvent::NewL( *iConnection, *this );
	iSIPEvent->SetEventStateType( EMtSubscribe );
    MCE_DELETE( iSIPEvent->iEvent );
    iEvent = CMceComEvent::NewL( *iSIPEvent,
								 KDefaultExpires );
	iSIPEvent->iEvent = iEvent;
    iSIPEvent->SetCurrentState( KMceIdleEventStateIndex );
    } 

void UT_CMceEventSubscribeMtIdleState::Teardown(  )
    {
    delete iState;
    
    delete iSIPEvent;
	iEvent = NULL;
    
    delete iConnection;
    
    delete iSipProfile;
    
	SIPStrings::Close();
    }
    
void UT_CMceEventSubscribeMtIdleState::SetupEmptyL(  )
    {
    } 

void UT_CMceEventSubscribeMtIdleState::UT_CMceEventSubscribeMtIdleState_HandleLL()
    {
    EUNIT_ASSERT_LEAVE( iState->HandleL( *iEvent ) ); 
    }
        
void UT_CMceEventSubscribeMtIdleState::UT_CMceEventSubscribeMtIdleState_HandleReceiveRequestLL()
    {
    // Leave with no dialog
    EUNIT_ASSERT_LEAVE( iState->HandleReceiveRequestL( *iEvent ) ); 
    
    CSIPNotifyDialogAssoc* notifyDlg = CSIPNotifyDialogAssoc::NewL();
    iSIPEvent->SetDialog( notifyDlg );
    
    // Leave with no server transaction
    EUNIT_ASSERT_LEAVE( iState->HandleReceiveRequestL( *iEvent ) ); 
    
    // Create and set Subscribe transaction
    CSIPServerTransaction* serverTran = CSIPServerTransaction::NewL();
    serverTran->SetType( 1 ); // subscribe
    iSIPEvent->iRequest = serverTran;
  
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
    
    // Leave with no event header
    EUNIT_ASSERT_LEAVE( iState->HandleReceiveRequestL( *iEvent ) );
    
    // create event header
    RPointerArray<CSIPHeaderBase> hdrs;
    CSIPHeaderBase::PushLC( &hdrs );
    
    CSIPEventHeader* e = CSIPEventHeader::DecodeL( KEventPackage );
    CleanupStack::PushL( e );
    hdrs.AppendL( e );
    CleanupStack::Pop( e );
    
    // expires header
    CSIPExpiresHeader* ex = new ( ELeave ) CSIPExpiresHeader( 60 );
    CleanupStack::PushL( ex );
    hdrs.AppendL( ex );
    CleanupStack::Pop( ex );
    elem->MessageElements().SetUserHeadersL( hdrs );
    
     CleanupStack::Pop( &hdrs );
    // Fail with no expires header.
    // successfull
    iState->HandleReceiveRequestL( *iEvent );
    EUNIT_ASSERT( iReceived == ESubscribeRequest );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceEstablishingEventStateIndex );
    }

void UT_CMceEventSubscribeMtIdleState::UT_CMceEventSubscribeMtIdleState_HandleReceiveRequestL2L()
    {
    // with content
    CSIPNotifyDialogAssoc* notifyDlg = CSIPNotifyDialogAssoc::NewL();
    iSIPEvent->SetDialog( notifyDlg );
    
    // Create and set Subscribe transaction
    CSIPServerTransaction* serverTran = CSIPServerTransaction::NewL();
        
    serverTran->SetType( 1 ); // subscribe
    iSIPEvent->iRequest = serverTran;
     
    // Create request elements
    TUriParser8 uriParser;
    User::LeaveIfError( uriParser.Parse( KRemoteUri() ) );
    CUri8* remoteUri = CUri8::NewL(uriParser);
    CleanupStack::PushL(remoteUri);
    
    CSIPRequestElements* elem =CSIPRequestElements::NewL( remoteUri );
    CleanupStack::Pop( remoteUri );
    serverTran->SetRequestElements( elem );
    
    // Create and set content type and content.
    HBufC8* content = KContent().AllocLC();
    CSIPContentTypeHeader* ct = CSIPContentTypeHeader::DecodeL( KContentType );
    CleanupStack::PushL( ct );
    // put content for coverage
    elem->MessageElements().SetContentL( content, ct );
    CleanupStack::Pop( ct );
    CleanupStack::Pop( content );
    
    // create event header
    RPointerArray<CSIPHeaderBase> hdrs;
    CSIPHeaderBase::PushLC( &hdrs );
    CSIPEventHeader* e = CSIPEventHeader::DecodeL( KEventPackage );
    CleanupStack::PushL( e );
    hdrs.AppendL( e );
    CleanupStack::Pop( e );
    
    // expires header
    CSIPExpiresHeader* ex = new ( ELeave ) CSIPExpiresHeader( 60 );
    CleanupStack::PushL( ex );
    hdrs.AppendL( ex );
    CleanupStack::Pop( ex );
    elem->MessageElements().SetUserHeadersL( hdrs );
    
    CleanupStack::Pop( &hdrs );
    // Fail with no expires header.
    
    // successfull
    iState->HandleReceiveRequestL( *iEvent );
    EUNIT_ASSERT( iReceived == ESubscribeRequest );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceEstablishingEventStateIndex );
    }
    
void UT_CMceEventSubscribeMtIdleState::UT_CMceEventSubscribeMtIdleState_HandleReceiveRequestL3L()
    {
    // terminated
    CSIPNotifyDialogAssoc* notifyDlg = CSIPNotifyDialogAssoc::NewL();
    iSIPEvent->SetDialog( notifyDlg );
    
    CSIPServerTransaction* serverTran = CSIPServerTransaction::NewL();
    serverTran->SetType( 1 ); // subscribe
    iSIPEvent->iRequest = serverTran;
  
    // Create request elements
    TUriParser8 uriParser;
    User::LeaveIfError( uriParser.Parse( KRemoteUri() ) );
    CUri8* remoteUri = CUri8::NewL(uriParser);
    CleanupStack::PushL(remoteUri);
    
    CSIPRequestElements* elem =CSIPRequestElements::NewL( remoteUri );
    CleanupStack::Pop( remoteUri );
    serverTran->SetRequestElements( elem );
    
    // Create and set content type and content.
    HBufC8* content = KContent().AllocLC();
    CSIPContentTypeHeader* ct = CSIPContentTypeHeader::DecodeL( KContentType );
    CleanupStack::PushL( ct );
    // put content for coverage
    elem->MessageElements().SetContentL( content, ct );
    CleanupStack::Pop( ct );
    CleanupStack::Pop( content );
    
    // create event header
    RPointerArray<CSIPHeaderBase> hdrs;
    CSIPHeaderBase::PushLC( &hdrs );
    CSIPEventHeader* e = CSIPEventHeader::DecodeL( KEventPackage );
    CleanupStack::PushL( e );
    hdrs.AppendL( e );
    CleanupStack::Pop( e );
    
    // expires header
    CSIPExpiresHeader* ex = new ( ELeave ) CSIPExpiresHeader( 0 );
    CleanupStack::PushL( ex );
    hdrs.AppendL( ex );
    CleanupStack::Pop( ex );
    elem->MessageElements().SetUserHeadersL( hdrs );
    
    CleanupStack::Pop( &hdrs );
    // Fail with no expires header.
    
    // successfull
    iState->HandleReceiveRequestL( *iEvent );
    EUNIT_ASSERT( iReceived == ESubscribeRequest );
    EUNIT_ASSERT( iEventState == CMceEvent::ETerminated );
    
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceTerminatedEventStateIndex );
    }


           
void UT_CMceEventSubscribeMtIdleState::UT_CMceEventSubscribeMtIdleState_HandleResponseReceivedLL()
    {
    EUNIT_ASSERT_LEAVE( iState->HandleResponseReceivedL( *iEvent ) ); 
    }

void UT_CMceEventSubscribeMtIdleState::IncomingSubscribeL(
					CDesC8Array* aHeaders,
                    HBufC8* aContentType,
					HBufC8* aBody)
	{
	iReceived = ESubscribeRequest;
	delete aHeaders;
	delete aContentType;
	delete aBody;	
	}
void UT_CMceEventSubscribeMtIdleState::IncomingReferL(
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
void UT_CMceEventSubscribeMtIdleState::EventStateChangedL(TUint32 aState )
	{
	iEventState = aState;
	}
		
void UT_CMceEventSubscribeMtIdleState::NotifyReceivedL(CDesC8Array* aHeaders,
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
    UT_CMceEventSubscribeMtIdleState,
    "UT_CMceEventSubscribeMtIdleState",
    "UNIT" )

EUNIT_TEST(
    "HandleL - test ",
    "CMceEventSubscribeMtIdleState",
    "HandleL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeMtIdleState_HandleLL, Teardown)

EUNIT_TEST(
    "HandleReceiveRequestL - test ",
    "CMceEventSubscribeMtIdleState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeMtIdleState_HandleReceiveRequestLL, Teardown)

EUNIT_TEST(
    "HandleReceiveRequestL - test 2",
    "CMceEventSubscribeMtIdleState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeMtIdleState_HandleReceiveRequestL2L, Teardown)


EUNIT_TEST(
    "HandleReceiveRequestL - test 3 ",
    "CMceEventSubscribeMtIdleState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeMtIdleState_HandleReceiveRequestL3L, Teardown)

    
EUNIT_TEST(
    "HandleResponseReceivedL - test ",
    "CMceEventSubscribeMtIdleState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeMtIdleState_HandleResponseReceivedLL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
