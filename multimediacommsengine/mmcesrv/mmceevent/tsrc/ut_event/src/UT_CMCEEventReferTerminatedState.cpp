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
#include "ut_cmceeventreferterminatedstate.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <sipstrings.h>
#include <uri8.h>
#include <sipcontenttypeheader.h>
#include <sipsubscriptionstateheader.h>
#include <siprequestelements.h>
#include <sipmessageelements.h>
#include <sipresponseelements.h>


//  INTERNAL INCLUDES
#include "mceeventreferterminatedstate.h"
#include "MCEEventTestConsts.h"
#include "sipreferdialogassoc.h"
#include "sipnotifydialogassoc.h"
#include "sipeventheader.h"
#include "siprequestelements.h"
#include "sipprofile.h"
#include "mcesipconnection.h"
#include "mceserial.h"

// CONSTRUCTION
UT_CMceEventReferTerminatedState* UT_CMceEventReferTerminatedState::NewL()
    {
    UT_CMceEventReferTerminatedState* self = UT_CMceEventReferTerminatedState::NewLC(); 
    CleanupStack::Pop(self);

    return self;
    }

UT_CMceEventReferTerminatedState* UT_CMceEventReferTerminatedState::NewLC()
    {
    UT_CMceEventReferTerminatedState* self = new( ELeave ) UT_CMceEventReferTerminatedState();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceEventReferTerminatedState::~UT_CMceEventReferTerminatedState()
    {
    }

// Default constructor
UT_CMceEventReferTerminatedState::UT_CMceEventReferTerminatedState()
    {
    }

// Second phase construct
void UT_CMceEventReferTerminatedState::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceEventReferTerminatedState::SetupL(  )
    {
    iReceived = 0;
    iEventState = 0;
        
    iState = new( ELeave ) CMceEventReferTerminatedState();
    SIPStrings::OpenL();
    iSipProfile = CSIPProfile::NewL();
	iConnection = CMceSipConnection::NewL( *iSipProfile );
	iSIPEvent = CMceSipEvent::NewL( *iConnection, *this );
	iSIPEvent->SetEventStateType( EMoRefer );
    MCE_DELETE( iSIPEvent->iEvent );
    iEvent = CMceComEvent::NewL( *iSIPEvent,
							     KDefaultExpires );
    iSIPEvent->iEvent = iEvent;
    iSIPEvent->SetCurrentState( KMceTerminatedEventStateIndex );
    
    } 

void UT_CMceEventReferTerminatedState::SetupFullL(  )
    {
    iReceived = 0;
    iEventState = 0;
        
    iState = new( ELeave ) CMceEventReferTerminatedState();
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

    CSIPReferDialogAssoc* refer = CSIPReferDialogAssoc::NewL();
    iSIPEvent->SetDialog( refer );
    
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
    iEvent->SetPreviousAction( KErrNotFound );
    }

void UT_CMceEventReferTerminatedState::Teardown(  )
    {
    delete iState;
    
    delete iSIPEvent;
	iEvent = NULL;
    
    delete iConnection;
    
    delete iSipProfile;
    	
	SIPStrings::Close();
    }

void UT_CMceEventReferTerminatedState::SetupEmptyL(  )
    {
    
    } 

void UT_CMceEventReferTerminatedState::UT_CMceEventReferTerminatedState_HandleLL()
    {
    EUNIT_ASSERT_LEAVE( iState->HandleL( *iEvent ) ); 
    }
        
void UT_CMceEventReferTerminatedState::UT_CMceEventReferTerminatedState_HandleReceiveRequestLL()
    {
    // failing cases
    // with no dialog
    EUNIT_ASSERT_LEAVE( iState->HandleReceiveRequestL( *iEvent ) ); 
    
    CSIPReferDialogAssoc* referDlg = CSIPReferDialogAssoc::NewL();
    iSIPEvent->SetDialog( referDlg );
    
    // Failing with no transaction
    EUNIT_ASSERT_LEAVE( iState->HandleReceiveRequestL( *iEvent ) ); 
   
    // Create and set Refer transaction
    CSIPServerTransaction* serverTran = CSIPServerTransaction::NewL();
    serverTran->SetType( 3 ); // Notify
    iSIPEvent->iRequest = serverTran;
 
    // leaving with wrong action
    iEvent->SetPreviousAction( EMceItcSubscribe );
    EUNIT_ASSERT_LEAVE( iState->HandleReceiveRequestL( *iEvent ) ); 
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
    //EUNIT_ASSERT( iReceived == ENotifyRequest );
    
    TRAPD( error, iState->HandleReceiveRequestL( *iEvent ) );
    EUNIT_ASSERT( error );
    if ( error != KErrNoMemory )
        {
        EUNIT_ASSERT( iReceived == ENotifyRequest );      
        }
   }

void UT_CMceEventReferTerminatedState::UT_CMceEventReferTerminatedState_HandleReceiveRequestL2L()
    {
    // With active subscription state
    // With contenttype, subscription state active && event
    RPointerArray<CSIPHeaderBase> hdrs;
    CSIPHeaderBase::PushLC( &hdrs );
    
    CSIPContentTypeHeader* ct = CSIPContentTypeHeader::DecodeL( KContentTypeRefer );
    CleanupStack::PushL( ct );
    HBufC8* c = KContent().AllocLC();
    iMsgElem->SetContentL( c, ct );
    CleanupStack::Pop( c );
    CleanupStack::Pop( ct );
    
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
    
    //EUNIT_ASSERT_LEAVE( iState->HandleReceiveRequestL( *iEvent ) );
    //EUNIT_ASSERT( iReceived == ENotifyRequest );

    TRAPD( error, iState->HandleReceiveRequestL( *iEvent ) );
    EUNIT_ASSERT( error );
    if ( error != KErrNoMemory )
        {
        EUNIT_ASSERT( iReceived == ENotifyRequest );      
        }
    }

void UT_CMceEventReferTerminatedState::UT_CMceEventReferTerminatedState_HandleReceiveRequestL3L()
    {
    // With terminated subscriptions state
    RPointerArray<CSIPHeaderBase> hdrs;
    CSIPHeaderBase::PushLC( &hdrs );
    
    CSIPContentTypeHeader* ct = CSIPContentTypeHeader::DecodeL( KContentTypeRefer );
    CleanupStack::PushL( ct );
    HBufC8* c = KContent().AllocLC();
    iMsgElem->SetContentL( c, ct );
    CleanupStack::Pop( c );
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
    }

void UT_CMceEventReferTerminatedState::UT_CMceEventReferTerminatedState_HandleReceiveRequestL4L()
    {
    // With terminated subscriptions state && silent suppression
    iEvent->SetSilentSuppressionOn();
    RPointerArray<CSIPHeaderBase> hdrs;
    CSIPHeaderBase::PushLC( &hdrs );
  
    CSIPContentTypeHeader* ct = CSIPContentTypeHeader::DecodeL( KContentTypeRefer );
    CleanupStack::PushL( ct );
    HBufC8* c = KContent().AllocLC();
    iMsgElem->SetContentL( c, ct );
    CleanupStack::Pop( c );
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
    }

void UT_CMceEventReferTerminatedState::UT_CMceEventReferTerminatedState_HandleResponseReceivedLL()
    {
    EUNIT_ASSERT_NO_LEAVE( iState->HandleResponseReceivedL( *iEvent ) ); 
    }

void UT_CMceEventReferTerminatedState::IncomingSubscribeL(
					CDesC8Array* aHeaders,
                    HBufC8* aContentType,
					HBufC8* aBody)
	{
	iReceived = ESubscribeRequest;
	delete aHeaders;
	delete aContentType;
	delete aBody;	
	}
void UT_CMceEventReferTerminatedState::IncomingReferL(
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
void UT_CMceEventReferTerminatedState::EventStateChangedL(TUint32 aState)
	{
	iEventState = aState;
	}
		
void UT_CMceEventReferTerminatedState::NotifyReceivedL(CDesC8Array* aHeaders,
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
    UT_CMceEventReferTerminatedState,
    "UT_CMceEventReferTerminatedState",
    "UNIT" )

EUNIT_TEST(
    "HandleL - test ",
    "CMceEventReferTerminatedState",
    "HandleL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferTerminatedState_HandleLL, Teardown)

EUNIT_TEST(
    "HandleReceiveRequestL - test ",
    "CMceEventReferTerminatedState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferTerminatedState_HandleReceiveRequestLL, Teardown)

EUNIT_TEST(
    "HandleReceiveRequestL - test 2",
    "CMceEventReferTerminatedState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventReferTerminatedState_HandleReceiveRequestL2L, Teardown)

EUNIT_TEST(
    "HandleReceiveRequestL - test 3",
    "CMceEventReferTerminatedState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventReferTerminatedState_HandleReceiveRequestL3L, Teardown)

EUNIT_TEST(
    "HandleReceiveRequestL - test 4",
    "CMceEventReferTerminatedState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupFullL, UT_CMceEventReferTerminatedState_HandleReceiveRequestL4L, Teardown)
    
EUNIT_TEST(
    "HandleResponseReceivedL - test ",
    "CMceEventReferTerminatedState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferTerminatedState_HandleResponseReceivedLL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
