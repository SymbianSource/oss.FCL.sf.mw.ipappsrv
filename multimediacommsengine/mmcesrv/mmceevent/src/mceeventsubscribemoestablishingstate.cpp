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
* Description:    MO subscribe is sent and waiting for a response.
*
*/





// INCLUDE FILES
#include <sipresponseelements.h>
#include <sipsubscribedialogassoc.h>
#include <sipdialog.h>

#include "mceeventsubscribemoestablishingstate.h"
#include "mcesipeventhelper.h"
#include "mcesip.h"
#include "mceeventslogs.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceEventSubscribeMoEstablishingState::CMceEventSubscribeMoEstablishingState
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMceEventSubscribeMoEstablishingState::CMceEventSubscribeMoEstablishingState()
    {
    }

// -----------------------------------------------------------------------------
// CMceEventSubscribeMoEstablishingState::~CMceEventSubscribeMoEstablishingState
// Destructor
// -----------------------------------------------------------------------------
//
CMceEventSubscribeMoEstablishingState::~CMceEventSubscribeMoEstablishingState()
    {
    }

// -----------------------------------------------------------------------------
// CMceEventSubscribeMoEstablishingState::ProcessSubscribeStateL
// Ápply concrete state.
// Be noticed that it will be also update, unsubscribe
// -----------------------------------------------------------------------------
//
void CMceEventSubscribeMoEstablishingState::HandleL( CMceComEvent& /*aEvent*/ )
    {
    User::Leave( KMceEventStateError );
    }


// -----------------------------------------------------------------------------
// CMceEventSubscribeMoEstablishingState::HandleResponseReceivedL
// Ápply concrete state.
// Be noticed that it will be also update, unsubscribe
// -----------------------------------------------------------------------------
//	
/*lint -e408 */

void CMceEventSubscribeMoEstablishingState::HandleResponseReceivedL(
														CMceComEvent& aEvent)
	{
	MCEEVENTS_DEBUG("CMceEventSubscribeMoEstablishingState::HandleResponseReceivedL, Entry");

	User::LeaveIfError( &aEvent.SIPEvent().Response() ?
	    KErrNone : KMceEventStateError );
    
    const CSIPResponseElements& response = 
        *aEvent.SIPEvent().Response().ResponseElements();
    const CSIPResponseElements* responseCheck = &response;
    
    User::LeaveIfError( responseCheck ? KErrNone : KErrCorrupt );
    
	//MO
	//Checking if getting the error after sending the Subscribe
	if ( EMceItcSubscribe == aEvent.PreviousAction() ||
		 aEvent.PreviousAction() == EMceItcSubscribeUpdate )
        {
        aEvent.SetPreviousAction( KErrNotFound ); 
        TUint statusCode = response.StatusCode();
	    if ( statusCode >= KMceSipOK )
			{
			CMceEvent::TState nextClientState = CMceEvent::EPending;
			TMceEventStateIndex nextState = KErrNotFound;
	        if ( statusCode < KMceSipMultipleChoices )
	            {
	            nextClientState = CMceEvent::EActive;
	            nextState = KMceEstablishedEventStateIndex;
	            }
	        else 
	            {
	            nextClientState = CMceEvent::ETerminated;
	            nextState = KMceTerminatedEventStateIndex;
	            
	            }
			MCEEVENTS_DEBUG_NEXTCLIENTSTATE("NextClientState :", nextClientState )				            
			MCEEVENTS_DEBUG_NEXTSTATE("NextState :", nextState )		
					            
			aEvent.SIPEvent().ClientStateChangedL( nextClientState, ETrue );
	        aEvent.EventContext().SetCurrentStateL( aEvent, nextState );
	        }
      	}
     else // other actions not supported
     	{
     	User::Leave( KMceEventStateError );
     	}
	MCEEVENTS_DEBUG("CMceEventSubscribeMoEstablishingState::HandleResponseReceivedL, Exit");     	
    }
    
// -----------------------------------------------------------------------------
// CMceEventSubscribeMoEstablishedState::HandleReceiveRequestL
// Ápply concrete state.
// Be noticed that it will be also update, unsubscribe
// -----------------------------------------------------------------------------
//
void CMceEventSubscribeMoEstablishingState::HandleReceiveRequestL(
													CMceComEvent& aEvent )
	{
	MCEEVENTS_DEBUG("CMceEventSubscribeMoEstablishingState::HandleReceiveRequestL, Entry");
	User::LeaveIfError( aEvent.SIPEvent().Dialog() ?
        KErrNone : KMceEventStateError );
	
	CSIPServerTransaction& serverTran = aEvent.SIPEvent().Request();
	CSIPServerTransaction* serverTranCheck = &serverTran;
    User::LeaveIfError( serverTranCheck ? KErrNone : KMceEventStateError );
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

   	MCEEVENTS_DEBUG("CMceEventSubscribeMoEstablishingState::HandleReceiveRequestL, Exit");
	}    

//  End of File  
