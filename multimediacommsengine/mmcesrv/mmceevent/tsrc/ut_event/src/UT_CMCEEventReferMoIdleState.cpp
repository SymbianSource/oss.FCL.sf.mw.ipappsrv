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
#include "ut_cmceeventrefermoidlestate.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <sipstrings.h>


//  INTERNAL INCLUDES
#include "mceeventrefermoidlestate.h"
#include "MCEEventTestConsts.h"
#include "sipreferdialogassoc.h"
#include "sipsubscribedialogassoc.h"
#include <sipmessageelements.h>
#include "sipeventheader.h"
#include "sipprofile.h"
#include "mcesipconnection.h"
#include "mceserial.h"


// CONSTRUCTION
UT_CMceEventReferMoIdleState* UT_CMceEventReferMoIdleState::NewL()
    {
    UT_CMceEventReferMoIdleState* self = UT_CMceEventReferMoIdleState::NewLC(); 
    CleanupStack::Pop(self);

    return self;
    }

UT_CMceEventReferMoIdleState* UT_CMceEventReferMoIdleState::NewLC()
    {
    UT_CMceEventReferMoIdleState* self = new( ELeave ) UT_CMceEventReferMoIdleState();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceEventReferMoIdleState::~UT_CMceEventReferMoIdleState()
    {
    }

// Default constructor
UT_CMceEventReferMoIdleState::UT_CMceEventReferMoIdleState()
    {
    }

// Second phase construct
void UT_CMceEventReferMoIdleState::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceEventReferMoIdleState::SetupL(  )
    {
    iState = new( ELeave ) CMceEventReferMoIdleState();
    SIPStrings::OpenL();
    iSipProfile = CSIPProfile::NewL();
	iConnection = CMceSipConnection::NewL( *iSipProfile );
	iSIPEvent = CMceSipEvent::NewL( *iConnection, *this );
	iSIPEvent->SetEventStateType( EMoRefer );
    MCE_DELETE( iSIPEvent->iEvent );
    iEvent = CMceComEvent::NewL( *iSIPEvent,
							     KDefaultExpires );
    iSIPEvent->iEvent = iEvent;								 
    iSIPEvent->SetCurrentState( KMceIdleEventStateIndex );
    } 

void UT_CMceEventReferMoIdleState::Teardown(  )
    {
    delete iState;
    
    delete iSIPEvent;
	iEvent = NULL;
    
    delete iConnection;
    
    delete iSipProfile;
    
	SIPStrings::Close();
    }

void UT_CMceEventReferMoIdleState::SetupEmptyL(  )
    {
    
    } 

void UT_CMceEventReferMoIdleState::UT_CMceEventReferMoIdleState_HandleLL()
    {
    // Leaving with no dialog
    EUNIT_ASSERT_LEAVE( iState->HandleL( *iEvent ) );
    
    // Leaving with subscribe dialog 
    CSIPEventHeader* eventHdr = CSIPEventHeader::DecodeL( KEventPackage );
    CleanupStack::PushL( eventHdr );
    CSIPSubscribeDialogAssoc* subscrDlg = CSIPSubscribeDialogAssoc::NewL( eventHdr );
    CleanupStack::Pop( eventHdr );
    iSIPEvent->SetDialog( subscrDlg );
    EUNIT_ASSERT_LEAVE( iState->HandleL( *iEvent ) );
    
    // Create and set refer dialog
    delete iSIPEvent->iDialog;
    iSIPEvent->iDialog = NULL;
    
    CSIPReferDialogAssoc* referDlg = CSIPReferDialogAssoc::NewL();
    iSIPEvent->SetDialog( referDlg );
   
    // Successfull
    iState->HandleL( *iEvent );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceEstablishingEventStateIndex );
    }

void UT_CMceEventReferMoIdleState::UT_CMceEventReferMoIdleState_HandleL2L()
    {
    // successfull with content
    CSIPReferDialogAssoc* referDlg = CSIPReferDialogAssoc::NewL();
    iSIPEvent->SetDialog( referDlg );
    
    HBufC8* contentType = KContentType().AllocLC();
    HBufC8* content = KContent().AllocLC(); 
    
    iEvent->SetMsgContentL( contentType, content ); 
    CleanupStack::Pop( content );
    CleanupStack::Pop( contentType );
    // Successfull
    iState->HandleL( *iEvent );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceEstablishingEventStateIndex );
    }
        
void UT_CMceEventReferMoIdleState::UT_CMceEventReferMoIdleState_HandleReceiveRequestLL()
    {
    EUNIT_ASSERT_LEAVE( iState->HandleReceiveRequestL( *iEvent ) )
    }
           
void UT_CMceEventReferMoIdleState::UT_CMceEventReferMoIdleState_HandleResponseReceivedLL()
    {
    EUNIT_ASSERT_LEAVE( iState->HandleResponseReceivedL( *iEvent ) )
    }
    
void UT_CMceEventReferMoIdleState::IncomingSubscribeL(
					CDesC8Array* aHeaders,
                    HBufC8* aContentType,
					HBufC8* aBody)
	{
	delete aHeaders;
	delete aContentType;
	delete aBody;	
	}
void UT_CMceEventReferMoIdleState::IncomingReferL(
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
void UT_CMceEventReferMoIdleState::EventStateChangedL(TUint32 /*aState*/)
	{
	
	}
		
void UT_CMceEventReferMoIdleState::NotifyReceivedL(CDesC8Array* aHeaders,
                    		HBufC8* aContentType,
							HBufC8* aBody)
	{
	delete aHeaders;
	delete aContentType;
	delete aBody;	
	}
									

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceEventReferMoIdleState,
    "UT_CMceEventReferMoIdleState",
    "UNIT" )

EUNIT_TEST(
    "HandleL - test ",
    "CMceEventReferMoIdleState",
    "HandleL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferMoIdleState_HandleLL, Teardown)

EUNIT_TEST(
    "HandleL - test - content",
    "CMceEventReferMoIdleState",
    "HandleL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferMoIdleState_HandleL2L, Teardown)

EUNIT_TEST(
    "HandleReceiveRequestL - test ",
    "CMceEventReferMoIdleState",
    "HandleReceiveRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferMoIdleState_HandleReceiveRequestLL, Teardown)
    
EUNIT_TEST(
    "HandleResponseReceivedL - test ",
    "CMceEventReferMoIdleState",
    "HandleResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventReferMoIdleState_HandleResponseReceivedLL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
