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
#include "ut_cmceeventsubscribemoidlestate.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <sipstrings.h>

// CONSTANTS

//  INTERNAL INCLUDES
#include "mceeventsubscribemoidlestate.h"
#include "MCEEventTestConsts.h"
#include "sipreferdialogassoc.h"
#include "sipsubscribedialogassoc.h"
#include "sipeventheader.h"
#include "mcesipevent.h"
#include "sipprofile.h"
#include "mcesipconnection.h"
#include "mceserial.h"

// CONSTRUCTION
UT_CMceEventSubscribeMoIdleState* UT_CMceEventSubscribeMoIdleState::NewL()
    {
    UT_CMceEventSubscribeMoIdleState* self = UT_CMceEventSubscribeMoIdleState::NewLC(); 
    CleanupStack::Pop(self);

    return self;
    }

UT_CMceEventSubscribeMoIdleState* UT_CMceEventSubscribeMoIdleState::NewLC()
    {
    UT_CMceEventSubscribeMoIdleState* self = new( ELeave ) UT_CMceEventSubscribeMoIdleState();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceEventSubscribeMoIdleState::~UT_CMceEventSubscribeMoIdleState()
    {
    }

// Default constructor
UT_CMceEventSubscribeMoIdleState::UT_CMceEventSubscribeMoIdleState()
    {
    }

// Second phase construct
void UT_CMceEventSubscribeMoIdleState::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceEventSubscribeMoIdleState::SetupL(  )
    {
    iState = new( ELeave ) CMceEventSubscribeMoIdleState();
    SIPStrings::OpenL();
    iSipProfile = CSIPProfile::NewL();
	iConnection = CMceSipConnection::NewL( *iSipProfile );
	iSIPEvent = CMceSipEvent::NewL( *iConnection, *this );
	iSIPEvent->SetEventStateType( EMoSubscribe );
	MCE_DELETE( iSIPEvent->iEvent );
    iEvent = CMceComEvent::NewL( *iSIPEvent,
							     KDefaultExpires );
    iSIPEvent->iEvent = iEvent;
    iSIPEvent->SetCurrentState( KMceIdleEventStateIndex );
    } 

void UT_CMceEventSubscribeMoIdleState::Teardown(  )
    {
    delete iState;
    
    delete iSIPEvent;
    iEvent = NULL;
    
    delete iConnection;
    
    delete iSipProfile;
    
	SIPStrings::Close();
    }

void UT_CMceEventSubscribeMoIdleState::SetupEmptyL(  )
    {
    
    } 

void UT_CMceEventSubscribeMoIdleState::UT_CMceEventSubscribeMoIdleState_HandleLL()
    {
    // error, no dialog.
    EUNIT_ASSERT_LEAVE( iState->HandleL( *iEvent ) );
    
    // error wrong kind of dialog.
    CSIPReferDialogAssoc* referDlg = CSIPReferDialogAssoc::NewL();
    iSIPEvent->SetDialog( referDlg );
    EUNIT_ASSERT_LEAVE( iState->HandleL( *iEvent ) );
    
    delete iSIPEvent->iDialog;
    iSIPEvent->iDialog = NULL;
    
    HBufC8* ct = KContentType().AllocLC();
    HBufC8* c = KContent().AllocLC();
        
    iEvent->SetMsgContentL( ct, c );
    CleanupStack::Pop( c );
    CleanupStack::Pop( ct );
    
    CSIPEventHeader* eventHdr = CSIPEventHeader::DecodeL( KEventPackage );
    CleanupStack::PushL( eventHdr );
    CSIPSubscribeDialogAssoc* subscrDlg = CSIPSubscribeDialogAssoc::NewL( eventHdr );
    CleanupStack::Pop( eventHdr );
    iSIPEvent->SetDialog( subscrDlg );
    iState->HandleL( *iEvent );
    }
    
void UT_CMceEventSubscribeMoIdleState::UT_CMceEventSubscribeMoIdleState_HandleL2L()
    {
    CSIPEventHeader* eventHdr = CSIPEventHeader::DecodeL( KEventPackage );
    CleanupStack::PushL( eventHdr );
    CSIPSubscribeDialogAssoc* subscrDlg = CSIPSubscribeDialogAssoc::NewL( eventHdr );
    CleanupStack::Pop( eventHdr );
    iSIPEvent->SetDialog( subscrDlg );
    
    HBufC8* ct = KContentType().AllocLC();
    HBufC8* c = KContent().AllocLC();
        
    iEvent->SetMsgContentL( ct, c );
    CleanupStack::Pop( c );
    CleanupStack::Pop( ct );
    iState->HandleL( *iEvent );
    
    }
       
void UT_CMceEventSubscribeMoIdleState::UT_CMceEventSubscribeMoIdleState_HandleReceiveRequestLL()
    {
    EUNIT_ASSERT_LEAVE( iState->HandleReceiveRequestL( *iEvent ) ); 
    }
           
void UT_CMceEventSubscribeMoIdleState::UT_CMceEventSubscribeMoIdleState_HandleResponseReceivedLL()
    {
    EUNIT_ASSERT_LEAVE( iState->HandleResponseReceivedL( *iEvent ) ); 
    }

void UT_CMceEventSubscribeMoIdleState::IncomingSubscribeL(
					CDesC8Array* aHeaders,
                    HBufC8* aContentType,
					HBufC8* aBody)
	{
	delete aHeaders;
	delete aContentType;
	delete aBody;	
	}
void UT_CMceEventSubscribeMoIdleState::IncomingReferL(
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
void UT_CMceEventSubscribeMoIdleState::EventStateChangedL(TUint32 /*aState*/)
	{
	
	}
		
void UT_CMceEventSubscribeMoIdleState::NotifyReceivedL(CDesC8Array* aHeaders,
                    		HBufC8* aContentType,
							HBufC8* aBody)
	{
	delete aHeaders;
	delete aContentType;
	delete aBody;	
	}
									
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceEventSubscribeMoIdleState,
    "UT_CMceEventSubscribeMoIdleState",
    "UNIT" )

EUNIT_TEST(
    "HandleL - test ",
    "CMceEventSubscribeMoIdleState",
    "HandleL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeMoIdleState_HandleLL, Teardown)

EUNIT_TEST(
    "HandleL - test 2 ",
    "CMceEventSubscribeMoIdleState",
    "HandleL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeMoIdleState_HandleL2L, Teardown)

EUNIT_TEST(
    "HandleReceiveRequestL - test ",
    "CMceEventSubscribeMoIdleState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeMoIdleState_HandleReceiveRequestLL, Teardown)
    
EUNIT_TEST(
    "HandleResponseReceivedL - test ",
    "CMceEventSubscribeMoIdleState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeMoIdleState_HandleResponseReceivedLL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
