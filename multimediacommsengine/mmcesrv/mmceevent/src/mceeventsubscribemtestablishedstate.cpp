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
* Description:    MT event is established.
*
*/





// INCLUDE FILES
#include <sipsubscriptionstateheader.h>
#include <sipnotifydialogassoc.h>
#include <sipdialog.h>
#include "mcesipeventhelper.h"
#include "mceeventsubscribemtestablishedstate.h"
#include "mcesip.h"
#include "mceeventslogs.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceEventSubscribeMtEstablishedState::CMceEventSubscribeMtEstablishedState
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMceEventSubscribeMtEstablishedState::CMceEventSubscribeMtEstablishedState()
    {
    }

// -----------------------------------------------------------------------------
// CMceEventSubscribeMtEstablishedState::~CMceEventSubscribeMtEstablishedState
// Destructor
// -----------------------------------------------------------------------------
//
CMceEventSubscribeMtEstablishedState::~CMceEventSubscribeMtEstablishedState()
    {
    }

// -----------------------------------------------------------------------------
// CMceEventSubscribeMtEstablishedState::ProcessSubscribeStateL
// Ápply concrete state.
// Be noticed that it will be also update, unsubscribe
// -----------------------------------------------------------------------------
//
void CMceEventSubscribeMtEstablishedState::HandleL(CMceComEvent& aEvent )
    {
    MCEEVENTS_DEBUG("CMceEventSubscribeMtEstablishedState::HandleL, Entry");
    User::LeaveIfError( aEvent.SIPEvent().Dialog() ?
        KErrNone : KMceEventStateError );
        
    User::LeaveIfError( aEvent.SIPEvent().Dialog()->Type() == 
        SIPStrings::StringF( SipStrConsts::ENotify ) ?
        KErrNone : KMceEventStateError );
    
    // Set subscription state and parameters
    TInt nextState = KErrNotFound;		    
    CSIPSubscriptionStateHeader& subStateHeader = 
		( static_cast< CSIPNotifyDialogAssoc* >
		( aEvent.SIPEvent().Dialog() ) )->SubscriptionState();

    switch ( aEvent.PreviousAction() )
        {
        case EMceItcNotify:
            {
            subStateHeader.SetSubStateValueL( KStateActive );
            subStateHeader.SetExpiresParameterL( aEvent.RefreshInterval() );
            nextState = KMceEstablishingEventStateIndex;
		    break;
            }
        case EMceItcTerminateEvent:
            {
            subStateHeader.SetSubStateValueL( KStateTerminated );
            RStringF expires = SIPStrings::StringF( SipStrConsts::EExpires );
            subStateHeader.DeleteParam( expires );
            nextState = KMceTerminatingEventStateIndex;
            break;
            }
        default:
            {
            // Other actions not supported
            User::Leave( KMceEventStateError );
            break;
            }
        }
    
    CSIPMessageElements* message = 
	    MCESIPEventHelper::CreateMessageElementsLC( aEvent,
	                                                SipStrConsts::ENotify );
    CSIPClientTransaction* transaction = 
	    ( static_cast< CSIPNotifyDialogAssoc* >
	    ( aEvent.SIPEvent().Dialog() ) )->SendNotifyL( message );
	
	CleanupStack::Pop( message );
	CleanupStack::PushL( transaction );
    aEvent.SIPEvent().SetPendingTransactionL( transaction );
	CleanupStack::Pop( transaction );
	
	aEvent.EventContext().SetCurrentStateL( aEvent, nextState );
	MCEEVENTS_DEBUG("CMceEventSubscribeMtEstablishedState::HandleL, Exit");
	}

// -----------------------------------------------------------------------------
// CMceEventSubscribeMtEstablishedState::HandleReceiveRequestL
// Ápply concrete state.
// Be noticed that it will be also update, unsubscribe
// -----------------------------------------------------------------------------
//
void CMceEventSubscribeMtEstablishedState::HandleReceiveRequestL(
													CMceComEvent& aEvent )
	{
	MCEEVENTS_DEBUG("CMceEventSubscribeMtEstablishedState::HandleReceiveRequestL, Entry");
	User::LeaveIfError( aEvent.SIPEvent().Dialog() ?
	    KErrNone : KMceEventStateError );
        
	User::LeaveIfError( aEvent.SIPEvent().Dialog()->Type() 
        == SIPStrings::StringF( SipStrConsts::ENotify ) ?
        KErrNone : KMceEventStateError );
    
    CSIPServerTransaction& serverTA = aEvent.SIPEvent().Request();
    CSIPServerTransaction* serverTACheck = &serverTA;
	User::LeaveIfError( serverTACheck ? KErrNone : KMceEventStateError );
	
	//Received Subscribe from Mo there is noaction in the previous action
	User::LeaveIfError( aEvent.PreviousAction() == KErrNotFound ?
	    KErrNone : KMceEventStateError );
	
    User::LeaveIfError( CSIPDialog::EConfirmed == 
        aEvent.SIPEvent().Dialog()->Dialog().State() ?
        KErrNone : KMceEventStateError );
    
    User::LeaveIfError( serverTA.RequestElements() ? KErrNone : KErrCorrupt );
	
	const CSIPMessageElements& subscribeMsg = 
	    serverTA.RequestElements()->MessageElements();
	//receiveing MO Subscribe Update or Mo Subscribe Terminate
	//Check the expire header so knows which state should go
	//Observer inform the incomingsubscribe and send response and notify
		    
    TBool terminated = EFalse;
    CSIPResponseElements* response = 
        MCESIPEventHelper::SubscribeRequestReceivedLC( aEvent, subscribeMsg );
    if ( response->StatusCode() == KMceSipOK )
		{

		MCESIPEventHelper::HandleExpireHeaderL( subscribeMsg, terminated );
   
        aEvent.SIPEvent().Request().SendResponseL( response );
        
        CleanupStack::Pop( response );
        // Send notify
        TInt nextState = KErrNotFound;
        
		CSIPNotifyDialogAssoc* dialogAssoc = 
    		static_cast <CSIPNotifyDialogAssoc* >
    		( aEvent.SIPEvent().Dialog() ); 
    
        CSIPSubscriptionStateHeader& subscrState = 
            dialogAssoc->SubscriptionState();
		
		if ( terminated )
            {
            subscrState.SetSubStateValueL( KStateTerminated );
			RStringF expires = SIPStrings::StringF( SipStrConsts::EExpires );
            subscrState.DeleteParam( expires );
            nextState = KMceTerminatingEventStateIndex;
            }
		else
            {
            //when subscription state is active
            subscrState.SetSubStateValueL( KStateActive );
			subscrState.SetExpiresParameterL( aEvent.RefreshInterval() );
			nextState = KMceEstablishedEventStateIndex;
            }
        
        CSIPMessageElements* notifyMsg = 
            MCESIPEventHelper::CreateMessageElementsLC( 
                                                aEvent,
                                                SipStrConsts::ENotify );
		CSIPClientTransaction* tx = dialogAssoc->SendNotifyL( notifyMsg );
		CleanupStack::Pop( notifyMsg );
		CleanupStack::PushL( tx );
		
	    aEvent.SIPEvent().SetPendingTransactionL( tx );
		CleanupStack::Pop( tx );	
		
	    aEvent.EventContext().SetCurrentStateL( aEvent, nextState );
        }
	else
	  	{
	    aEvent.SIPEvent().Request().SendResponseL( response );
	    CleanupStack::Pop( response );
	    }

	MCEEVENTS_DEBUG("CMceEventSubscribeMtEstablishedState::HandleReceiveRequestL, Exit");
	}
	
// -----------------------------------------------------------------------------
// CMceEventSubscribeMtEstablishedState::HandleResponseReceivedL
// Ápply concrete state.
// Be noticed that it will be also update, unsubscribe
// -----------------------------------------------------------------------------
//	
void CMceEventSubscribeMtEstablishedState::HandleResponseReceivedL( 
												CMceComEvent& aEvent )
	{
	MCEEVENTS_DEBUG("CMceEventSubscribeMtEstablishedState::HandleResponseReceivedL, Entry");
	//MT
	//Getting subscribe->sendResponse->Send Notify and now getting response
	//Checking if getting the error after sending the Subscribe
	User::LeaveIfError( &aEvent.SIPEvent().Response() ?
	    KErrNone : KMceEventStateError );
	    
	User::LeaveIfError( aEvent.SIPEvent().Response().Type()==
	    SIPStrings::StringF( SipStrConsts::ENotify ) ?
	    KErrNone : KMceEventStateError );
	
    //Get response from sending subscribe in Established state
    aEvent.SetPreviousAction( KErrNotFound ); 
    const CSIPResponseElements& response = 
    			*aEvent.SIPEvent().Response().ResponseElements();
    const CSIPResponseElements* responseCheck = &response;
    User::LeaveIfError( responseCheck ? KErrNone : KErrCorrupt );

    //Notify the subsession the response received
    //From the MtIdle 
    //Getting subscribe->sendResponse->Send Notify and now getting response
    TUint statusCode = response.StatusCode();
    if (statusCode >= KMceSipOK)
		{
		CMceEvent::TState nextClientState = CMceEvent::EPending;
		TMceEventStateIndex nextState = KErrNotFound;
        
        if (statusCode < KMceSipMultipleChoices )
            {
            nextClientState = CMceEvent::EActive;
            nextState = KMceEstablishedEventStateIndex;
            }
        else 
            {
            nextClientState = CMceEvent::ETerminated;
            nextState = KMceTerminatedEventStateIndex;
            }
        aEvent.SIPEvent().ClientStateChangedL( nextClientState, ETrue );
        aEvent.EventContext().SetCurrentStateL( aEvent, nextState );
        }
    
	MCEEVENTS_DEBUG("CMceEventSubscribeMtEstablishedState::HandleResponseReceivedL, Exit");
	}

//  End of File  
