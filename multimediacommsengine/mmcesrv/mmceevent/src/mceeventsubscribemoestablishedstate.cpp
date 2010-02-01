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
* Description:    MO event is established.
*
*/





// INCLUDE FILES
#include "mceeventsubscribemoestablishedstate.h"
#include "mcesipeventhelper.h"
#include "mcesip.h"
#include "mceeventslogs.h"
#include <sipcontenttypeheader.h>
#include <sipsubscribedialogassoc.h>
#include <sipdialog.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceEventSubscribeMoEstablishedState::CMceEventSubscribeMoEstablishedState
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMceEventSubscribeMoEstablishedState::CMceEventSubscribeMoEstablishedState()
    {
    }

// -----------------------------------------------------------------------------
// CMceEventSubscribeMoEstablishedState::~CMceEventSubscribeMoEstablishedState
// Destructor
// -----------------------------------------------------------------------------
//
CMceEventSubscribeMoEstablishedState::~CMceEventSubscribeMoEstablishedState()
    {
    }

// -----------------------------------------------------------------------------
// CMceEventSubscribeMoEstablishedState::ProcessSubscribeStateL
// Ápply concrete state.
// Be noticed that it will be also update, unsubscribe
// -----------------------------------------------------------------------------
//
void CMceEventSubscribeMoEstablishedState::HandleL(CMceComEvent& aEvent )
    {
    MCEEVENTS_DEBUG("CMceEventSubscribeMoEstablishedState::HandleL, Entry");
    User::LeaveIfError( aEvent.SIPEvent().Dialog() ?
        KErrNone : KMceEventStateError );
	
    User::LeaveIfError( aEvent.SIPEvent().Dialog()->Type() == 
	    SIPStrings::StringF( SipStrConsts::ESubscribe ) ?
	    KErrNone : KMceEventStateError );
	    
    //For MO only send subscribe update and unsubscribe to terminate
    if( aEvent.PreviousAction() == EMceItcSubscribeUpdate )
		{
		//Sending Update with new refresh interval
		//also checking the dialog state->check precondition
		//Updated->headers, contenttype, content
		CSIPMessageElements* message = 
		    MCESIPEventHelper::CreateMessageElementsLC( 
            		                            aEvent,
            		                            SipStrConsts::ESubscribe,
            		                            aEvent.RefreshInterval() );    
							    						    
		CSIPClientTransaction* transaction = 
		    static_cast< CSIPSubscribeDialogAssoc* >
		    ( aEvent.SIPEvent().Dialog() )->UpdateL( message );
        
        CleanupStack::Pop( message );
    	CleanupStack::PushL( transaction );
    	
    	aEvent.SIPEvent().SetPendingTransactionL( transaction );
    	CleanupStack::Pop( transaction );
    	
    	//To received 200 OK
    	aEvent.EventContext().SetCurrentStateL( 
    	                                aEvent, 
    	                                KMceEstablishingEventStateIndex );
		}

	else if( aEvent.PreviousAction() == EMceItcTerminateEvent )
		{
		//Send Unsubscribe goes to Terminating state
		//In MoEvent SubscribeDialog Assoc is there
		CSIPMessageElements* message = 
		    MCESIPEventHelper::CreateMessageElementsLC( 
		                                    aEvent,
		                                    SipStrConsts::ESubscribe );
		CSIPClientTransaction* transaction = 
		    static_cast< CSIPSubscribeDialogAssoc* >
		    ( aEvent.SIPEvent().Dialog() )-> SendUnsubscribeL( message );
	    
	    CleanupStack::Pop( message );
	    CleanupStack::PushL( transaction );
		
		aEvent.SIPEvent().SetPendingTransactionL( transaction );
		CleanupStack::Pop ( transaction );
		
		aEvent.EventContext().SetCurrentStateL( 
		                                aEvent, 
		                                KMceTerminatingEventStateIndex );
             
		}
    else
        {
        // Other actions not supported
        User::Leave( KMceEventStateError );
        }
	MCEEVENTS_DEBUG("CMceEventSubscribeMoEstablishedState::HandleL, Exit");        
	}

// -----------------------------------------------------------------------------
// CMceEventSubscribeMoEstablishedState::HandleReceiveRequestL
// Ápply concrete state.
// Be noticed that it will be also update, unsubscribe
// -----------------------------------------------------------------------------
//
void CMceEventSubscribeMoEstablishedState::HandleReceiveRequestL(
													CMceComEvent& aEvent )
	{
	MCEEVENTS_DEBUG("CMceEventSubscribeMoEstablishedState::HandleReceiveRequestL, Entry");
	User::LeaveIfError( aEvent.SIPEvent().Dialog() ?
        KErrNone : KMceEventStateError );
	
	CSIPServerTransaction& serverTran = aEvent.SIPEvent().Request();
	CSIPServerTransaction* serverTranCheck = &serverTran;
    User::LeaveIfError( serverTranCheck ? KErrNone : KMceEventStateError );
	     
    User::LeaveIfError( aEvent.PreviousAction() == KErrNotFound ?
        KErrNone : KMceEventStateError );
    
    User::LeaveIfError( CSIPDialog::EConfirmed ==
        aEvent.SIPEvent().Dialog()->Dialog().State() ?
        KErrNone : KMceEventStateError );
     
    User::LeaveIfError( serverTran.RequestElements() ? KErrNone : KErrCorrupt );	    		 
	//Received Notify  form Mt
	//Receiving Notify and Send 200
	//Check the Notify Subscription state so knows which state should go
	//Observer inform the notify received, and Established
	const CSIPMessageElements& msgElem = 
	    serverTran.RequestElements()->MessageElements();
    
    CSIPResponseElements* response = 
        MCESIPEventHelper::NotifyRequestReceivedLC( aEvent, msgElem );
    if ( response->StatusCode() == KMceSipOK )
    	{
        CDesC8Array* headers = MceSip::UserHeadersToTextArrayL( 
                                    serverTran, 
                                    aEvent.SIPEvent().Dialog()->Dialog() );
        CleanupStack::PushL( headers );
        HBufC8* contenttype = NULL;
        if( msgElem.ContentType() )
    	    {
    	    contenttype= msgElem.ContentType()->ToTextValueL();
    	    }
        CleanupStack::PushL( contenttype );
        HBufC8* content = msgElem.Content().AllocLC();
    	aEvent.SIPEvent().NotifyReceivedL( headers,
    			    					   contenttype,
    									   content );
        
        CleanupStack::Pop( content );
        CleanupStack::Pop( contenttype );
        CleanupStack::Pop( headers );		
        TBool terminated = EFalse;
        MCESIPEventHelper::HandleSubscriptionStateHeaderL( msgElem, terminated );

        serverTran.SendResponseL( response );
        CleanupStack::Pop( response );
        if ( terminated )
            {
            // when the subscription state is terminated
            //receive notify send 200 OK
            //if we sent notify in terminating,if not should go terminated state
            aEvent.EventContext().SetCurrentStateL( 
                                            aEvent, 
                                            KMceTerminatedEventStateIndex );
            aEvent.SIPEvent().ClientStateChangedL(
                                                CMceEvent::ETerminated,
                                                EFalse );        
                    
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
        //when the notify request is bad, state doesnt change
        serverTran.SendResponseL( response );
        CleanupStack::Pop( response );
        }
	MCEEVENTS_DEBUG("CMceEventSubscribeMoEstablishedState::HandleReceiveRequestL, Exit");    
	}
	
// -----------------------------------------------------------------------------
// CMceEventSubscribeMoEstablishedState::HandleResponseReceivedL
// Ápply concrete state.
// Be noticed that it will be also update, unsubscribe
// -----------------------------------------------------------------------------
//	
void CMceEventSubscribeMoEstablishedState::HandleResponseReceivedL( 
												CMceComEvent& aEvent )
	{
	MCEEVENTS_DEBUG(
		"CMceEventSubscribeMoEstablishedState::HandleResponseReceivedL, Entry");

	User::LeaveIfError( &aEvent.SIPEvent().Response() ?
	    KErrNone : KMceEventStateError );
    
    const CSIPResponseElements& response = 
        *aEvent.SIPEvent().Response().ResponseElements();
    const CSIPResponseElements* responseCheck = &response;
    
    User::LeaveIfError( responseCheck ? KErrNone : KErrCorrupt );

    TUint statusCode = response.StatusCode();

	// Only case when we should come here is when SUBSCRIBE's 200 OK response
	// and the first NOTIFY message have swapped
    if ( statusCode >= KMceSipOK && statusCode < KMceSipMultipleChoices )
	    {
        aEvent.SetPreviousAction( KErrNotFound ); 	    	
	    }
	else 
		{ // Other cases are not allowed
		MCEEVENTS_DEBUG(
			"CMceEventSubscribeMoEstablishedState::HandleResponseReceivedL, LEAVE!");
		User::Leave( KMceEventStateError );	
		}

	MCEEVENTS_DEBUG(
		"CMceEventSubscribeMoEstablishedState::HandleResponseReceivedL, Exit");
	}

//  End of File  
