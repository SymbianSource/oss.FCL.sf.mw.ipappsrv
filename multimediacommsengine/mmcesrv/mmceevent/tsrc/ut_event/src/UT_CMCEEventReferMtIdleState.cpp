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
#include "ut_cmceeventrefermtidlestate.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <sipstrings.h>
#include <sipcontenttypeheader.h>
#include <uri8.h>
#include <siprequestelements.h>
#include <sipmessageelements.h>
#include <sipresponseelements.h>


//  INTERNAL INCLUDES
#include "mceeventrefermtidlestate.h"
#include "MCEEventTestConsts.h"
#include "sipreferdialogassoc.h"
#include "sipsubscribedialogassoc.h"
#include "sipeventheader.h"
#include "siprefertoheader.h"
#include "sipnotifydialogassoc.h"
#include "sipprofile.h"
#include "mcesipconnection.h"
#include "mceserial.h"


// CONSTRUCTION
UT_CMceEventReferMtIdleState* UT_CMceEventReferMtIdleState::NewL()
    {
    UT_CMceEventReferMtIdleState* self = UT_CMceEventReferMtIdleState::NewLC(); 
    CleanupStack::Pop(self);

    return self;
    }

UT_CMceEventReferMtIdleState* UT_CMceEventReferMtIdleState::NewLC()
    {
    UT_CMceEventReferMtIdleState* self = new( ELeave ) UT_CMceEventReferMtIdleState();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceEventReferMtIdleState::~UT_CMceEventReferMtIdleState()
    {
    }

// Default constructor
UT_CMceEventReferMtIdleState::UT_CMceEventReferMtIdleState()
    {
    }

// Second phase construct
void UT_CMceEventReferMtIdleState::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceEventReferMtIdleState::SetupL(  )
    {
    iReceived = 0;
    iState = new( ELeave ) CMceEventReferMtIdleState();
    SIPStrings::OpenL();
    iSipProfile = CSIPProfile::NewL();
	iConnection = CMceSipConnection::NewL( *iSipProfile );
	iSIPEvent = CMceSipEvent::NewL( *iConnection, *this );
	iSIPEvent->SetEventStateType( EMtRefer );
    MCE_DELETE( iSIPEvent->iEvent );
    iEvent = CMceComEvent::NewL( *iSIPEvent,
							     KDefaultExpires );
    iSIPEvent->iEvent = iEvent;
    iSIPEvent->SetCurrentState( KMceIdleEventStateIndex );
    } 

void UT_CMceEventReferMtIdleState::Teardown(  )
    {
    delete iState;
    
    delete iSIPEvent;
	iEvent = NULL;
    
    delete iConnection;
    
    delete iSipProfile;
    
	SIPStrings::Close();
    }

void UT_CMceEventReferMtIdleState::SetupEmptyL(  )
    {
    
    } 

void UT_CMceEventReferMtIdleState::UT_CMceEventReferMtIdleState_HandleLL()
    {
    EUNIT_ASSERT_LEAVE( iState->HandleL( *iEvent ) ); 
    }
        
void UT_CMceEventReferMtIdleState::UT_CMceEventReferMtIdleState_HandleReceiveRequestLL()
    {
    // Leave with no dialog
    EUNIT_ASSERT_LEAVE( iState->HandleReceiveRequestL( *iEvent ) ); 
    
    CSIPNotifyDialogAssoc* notifyDlg = CSIPNotifyDialogAssoc::NewL();
    iSIPEvent->SetDialog( notifyDlg );
    
    // Leave with no server transaction
    EUNIT_ASSERT_LEAVE( iState->HandleReceiveRequestL( *iEvent ) ); 
    
    // Create and set Refer transaction
    CSIPServerTransaction* serverTran = CSIPServerTransaction::NewL();
    serverTran->SetType( 2 ); // Refer
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
    
    // Leave with no refer to header
    EUNIT_ASSERT_LEAVE( iState->HandleReceiveRequestL( *iEvent ) );
    
    // create refer to header
    RPointerArray<CSIPHeaderBase> hdrs;
    CSIPHeaderBase::PushLC( &hdrs );

    CSIPReferToHeader* rt = CSIPReferToHeader::DecodeL( KReferTo );
    CleanupStack::PushL( rt );
    hdrs.AppendL( rt );
    CleanupStack::Pop( rt );
    elem->MessageElements().SetUserHeadersL( hdrs );
    CleanupStack::Pop( &hdrs );
    
    // successfull
    iState->HandleReceiveRequestL( *iEvent );
    EUNIT_ASSERT( iReceived == EReferRequest );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceEstablishingEventStateIndex );
    
    }

void UT_CMceEventReferMtIdleState::UT_CMceEventReferMtIdleState_HandleReceiveRequestL2L()
    {
    // Successfull with content type
    CSIPNotifyDialogAssoc* notifyDlg = CSIPNotifyDialogAssoc::NewL();
    iSIPEvent->SetDialog( notifyDlg );
    
    // Create and set Refer transaction
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
    
    // create refer to header
    RPointerArray<CSIPHeaderBase> hdrs;
    CSIPHeaderBase::PushLC( &hdrs );
    CSIPReferToHeader* rt = CSIPReferToHeader::DecodeL( KReferTo );
    CleanupStack::PushL( rt );
    hdrs.AppendL( rt );
    CleanupStack::Pop( rt );
    
    // Create and set content type and content.
    HBufC8* content = KContent().AllocLC();
    CSIPContentTypeHeader* ct = CSIPContentTypeHeader::DecodeL( KContentType );
    CleanupStack::PushL( ct );
    // put content for coverage
    elem->MessageElements().SetContentL( content, ct );
    CleanupStack::Pop( ct );
    CleanupStack::Pop( content );
    
    elem->MessageElements().SetUserHeadersL( hdrs );
    CleanupStack::Pop( &hdrs );
    
    // successfull
    iState->HandleReceiveRequestL( *iEvent );
    EUNIT_ASSERT( iReceived == EReferRequest );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceEstablishingEventStateIndex );
    
    }

           
void UT_CMceEventReferMtIdleState::UT_CMceEventReferMtIdleState_HandleResponseReceivedLL()
    {
    EUNIT_ASSERT_LEAVE( iState->HandleResponseReceivedL( *iEvent ) ); 
    }

void UT_CMceEventReferMtIdleState::IncomingSubscribeL(
					CDesC8Array* aHeaders,
                    HBufC8* aContentType,
					HBufC8* aBody)
	{
	iReceived = ESubscribeRequest;
	delete aHeaders;
	delete aContentType;
	delete aBody;	
	}
void UT_CMceEventReferMtIdleState::IncomingReferL(
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
void UT_CMceEventReferMtIdleState::EventStateChangedL(TUint32 /*aState*/)
	{
	
	}
		
void UT_CMceEventReferMtIdleState::NotifyReceivedL(CDesC8Array* aHeaders,
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
    UT_CMceEventReferMtIdleState,
    "UT_CMceEventReferMtIdleState.",
    "UNIT" )

EUNIT_TEST(
    "HandleL - test ",
    "CMceEventReferMtIdleState",
    "HandleL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferMtIdleState_HandleLL, Teardown)

EUNIT_TEST(
    "HandleReceiveRequestL - test ",
    "CMceEventReferMtIdleState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferMtIdleState_HandleReceiveRequestLL, Teardown)

EUNIT_TEST(
    "HandleReceiveRequestL - test with content type ",
    "CMceEventReferMtIdleState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferMtIdleState_HandleReceiveRequestL2L, Teardown)

    
EUNIT_TEST(
    "HandleResponseReceivedL - test ",
    "CMceEventReferMtIdleState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferMtIdleState_HandleResponseReceivedLL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
