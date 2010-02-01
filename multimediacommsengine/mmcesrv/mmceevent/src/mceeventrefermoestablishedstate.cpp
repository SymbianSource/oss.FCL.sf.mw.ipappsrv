/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    State for established MO refer.
*
*/




// INCLUDE FILES
#include "mceeventrefermoestablishedstate.h"
#include "mcesipeventhelper.h"
#include "mcesip.h"
#include "mceeventslogs.h"
#include <sipcontenttypeheader.h>
#include <sipdialogassocbase.h>
#include <sipdialog.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceEventReferMoEstablishedState::CMceEventReferMoEstablishedState
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMceEventReferMoEstablishedState::CMceEventReferMoEstablishedState()
    {
    }

// -----------------------------------------------------------------------------
// CMceEventReferMoEstablishedState::~CMceEventReferMoEstablishedState
// Destructor
// -----------------------------------------------------------------------------
//
CMceEventReferMoEstablishedState::~CMceEventReferMoEstablishedState()
    {
    }

// -----------------------------------------------------------------------------
// CMceEventReferMoEstablishedState::ProcessReferStateL
// Ápply concrete state.
// Be noticed that it will be also update, unRefer
// -----------------------------------------------------------------------------
//
void CMceEventReferMoEstablishedState::HandleL( CMceComEvent& aEvent )
    {
    if(aEvent.PreviousAction() == EMceItcTerminateEvent)
		{
		// TBD: terminate REFER with SUBSCRIBE
		}
	else
	    {
	    // Other actions not supported.
	    User::Leave( KMceEventStateError );
	    }
    }

// -----------------------------------------------------------------------------
// CMceEventReferMoEstablishedState::HandleReceiveRequestL
// Ápply concrete state.
// Be noticed that it will be also Refer/Bye or Refer/Invite(update)
// -----------------------------------------------------------------------------
//
void CMceEventReferMoEstablishedState::HandleReceiveRequestL( 
														CMceComEvent& aEvent )
	{
	MCEEVENTS_DEBUG("CMceEventReferMoEstablishedState::HandleReceiveRequestL, Entry");
	User::LeaveIfError( aEvent.SIPEvent().Dialog() ?
	    KErrNone : KMceEventStateError );
	    
	CSIPServerTransaction& serverTran = aEvent.SIPEvent().Request();
	CSIPServerTransaction* serverTranCheck = &serverTran;
	User::LeaveIfError( serverTranCheck ? KErrNone : KMceEventStateError );
		 
	//Received Notify for MO, there is no action in the previous action
    User::LeaveIfError( aEvent.PreviousAction() == KErrNotFound ?
        KErrNone : KMceEventStateError );
    
     User::LeaveIfError( CSIPDialog::EConfirmed ==
	        aEvent.SIPEvent().Dialog()->Dialog().State() ?
	        KErrNone : KMceEventStateError );
	
	User::LeaveIfError( serverTran.RequestElements() ? KErrNone : KErrCorrupt );
	
	//Receiving Notify and Send 200
	//Check the Notify Subscription state so knows which state should go
	//Observer inform the notify received, and Established
	const CSIPMessageElements& msgElem = 
	    serverTran.RequestElements()->MessageElements();
    
    CSIPResponseElements* response = 
        MCESIPEventHelper::ReferNotifyRequestReceivedLC( aEvent, msgElem );
    if ( response->StatusCode() == KMceSipOK )
		{
		if ( !aEvent.SilentSuppression() )
		    {
		    // Do not notify the observer, is silent suppression is on
		    CDesC8Array* headers = 
		        MceSip::UserHeadersToTextArrayL( 
		                            serverTran, 
		                            aEvent.SIPEvent().Dialog()->Dialog() );
    		CleanupStack::PushL( headers );
            HBufC8* contenttype= NULL;
            if( msgElem.ContentType() )
    	        {
    	        contenttype = msgElem.ContentType()->ToTextValueL();	
    	        }
    		
            CleanupStack::PushL( contenttype );
            HBufC8* content = msgElem.Content().AllocLC();

            aEvent.SIPEvent().NotifyReceivedL( headers,
    					                       contenttype,
						                       content );
            CleanupStack::Pop( content );
	        CleanupStack::Pop( contenttype );
	        CleanupStack::Pop( headers );		
            }
		TBool terminated = EFalse;
        MCESIPEventHelper::HandleSubscriptionStateHeaderL( msgElem,
                                                           terminated );
        serverTran.SendResponseL( response );
        CleanupStack::Pop( response );
        if ( terminated )
            {
            // when the subscription state is terminated
            //receive notify send 200 OK
            aEvent.EventContext().SetCurrentStateL( 
                                        aEvent, 
                                        KMceTerminatedEventStateIndex );
            //Go Terminated directly        
            if ( !aEvent.SilentSuppression() )
		        {
		        // Do not notify the observer, is silent suppression is on
                aEvent.SIPEvent().ClientStateChangedL(
                                                    CMceRefer::ETerminated,
                                                    EFalse );
		        }
            }
        else
            {
            //when subscription state is active
            aEvent.EventContext().SetCurrentStateL( 
                    	                aEvent, 
                    	                KMceEstablishedEventStateIndex );
            }
        }
	 else
        {
            //when the notify request is bad, state doesn't change
        serverTran.SendResponseL( response );
        CleanupStack::Pop( response );
        }	
	MCEEVENTS_DEBUG("CMceEventReferMoEstablishedState::HandleReceiveRequestL, Exit");        
	}
	
// -----------------------------------------------------------------------------
// CMceEventReferMoEstablishedState::HandleResponseReceivedL
// Ápply concrete state.
// Be noticed that it will be also update, unRefer
// -----------------------------------------------------------------------------
//	
void CMceEventReferMoEstablishedState::HandleResponseReceivedL( 
												CMceComEvent& aEvent )
	{
	MCEEVENTS_DEBUG(
		"CMceEventReferMoEstablishedState::HandleResponseReceivedL, Entry");

	User::LeaveIfError( &aEvent.SIPEvent().Response() ?
	    KErrNone : KMceEventStateError );
    
    const CSIPResponseElements& response = 
        *aEvent.SIPEvent().Response().ResponseElements();
    const CSIPResponseElements* responseCheck = &response;
    
    User::LeaveIfError( responseCheck ? KErrNone : KErrCorrupt );

    TUint statusCode = response.StatusCode();

	// Only case when we should come here is when REFER's 200 OK response
	// and the first NOTIFY message have swapped
    if ( statusCode >= KMceSipOK && statusCode < KMceSipMultipleChoices )
	    {
        aEvent.SetPreviousAction( KErrNotFound ); 	    	
	    }
	else 
		{ // Other cases are not allowed
		MCEEVENTS_DEBUG(
			"CMceEventReferMoEstablishedState::HandleResponseReceivedL, LEAVE!");        
		User::Leave( KMceEventStateError );	
		}
	MCEEVENTS_DEBUG(
		"CMceEventReferMoEstablishedState::HandleResponseReceivedL, Exit");   
    }
    
//  End of File  
