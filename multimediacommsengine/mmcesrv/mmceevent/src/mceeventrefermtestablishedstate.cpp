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
* Description:    State for established MT refer.
*
*/




// INCLUDE FILES
#include <sipnotifydialogassoc.h>
#include <sipsubscriptionstateheader.h>
#include <sipdialog.h>
#include "mceeventrefermtestablishedstate.h"
#include "mcesipeventhelper.h"
#include "mcesip.h"
#include "mceeventslogs.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceEventReferMtEstablishedState::CMceEventReferMtEstablishedState
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMceEventReferMtEstablishedState::CMceEventReferMtEstablishedState()
    {
    }

// -----------------------------------------------------------------------------
// CMceEventReferMtEstablishedState::~CMceEventReferMtEstablishedState
// Destructor
// -----------------------------------------------------------------------------
//
CMceEventReferMtEstablishedState::~CMceEventReferMtEstablishedState()
    {
    }

// -----------------------------------------------------------------------------
// CMceEventReferMtEstablishedState::ProcessReferStateL
// Ápply concrete state.
// Be noticed that it will be also update, unRefer
// -----------------------------------------------------------------------------
//
void CMceEventReferMtEstablishedState::HandleL( CMceComEvent& aEvent )
    {
    MCEEVENTS_DEBUG("CMceEventReferMtEstablishedState::HandleL, Entry");
    //For Mt
    User::LeaveIfError( aEvent.SIPEvent().Dialog() ?
        KErrNone : KMceEventStateError );
        
    User::LeaveIfError( aEvent.SIPEvent().Dialog()->Type() 
        == SIPStrings::StringF( SipStrConsts::ENotify ) ?
        KErrNone : KMceEventStateError );
    
    // set subscription state and parameters
	TMceEventStateIndex nextState = KErrNotFound;
	CSIPSubscriptionStateHeader& subStateHeader = 
				( static_cast< CSIPNotifyDialogAssoc* >
				( aEvent.SIPEvent().Dialog() ) )->SubscriptionState();
		
    switch ( aEvent.PreviousAction() )
        {
        case EMceItcNotify:
            {
            MCESIPEventHelper::SetSubsStateHeaderValueAndParamsL( 
            									aEvent, 
            								   	SipStrConsts::ENotify,
            								   	subStateHeader,	
            									KStateActive, 
            									aEvent.RefreshInterval(),
            									EFalse );
           
             
            nextState = KMceEstablishingEventStateIndex;
            break;
            }
        case EMceItcTerminateEvent:
            {
             
            MCESIPEventHelper::SetSubsStateHeaderValueAndParamsL( 
            									aEvent, 
            								   SipStrConsts::ENotify,
            								   subStateHeader,	
            								   KStateTerminated,
            								   NULL );
         
            
           
            nextState = KMceTerminatingEventStateIndex;
            break;
            }
        default:
            {
            User::Leave( KMceEventStateError );
            break;
            }
        }
    
    // message elements
    CSIPMessageElements* message = 
	    MCESIPEventHelper::CreateMessageElementsLC( aEvent,
	                                                SipStrConsts::ENotify );
	
	// send Notify
	
    CSIPClientTransaction* transaction = 
		( static_cast< CSIPNotifyDialogAssoc* >
		( aEvent.SIPEvent().Dialog() ) )->SendNotifyL( message );		
		
    CleanupStack::Pop( message );
	CleanupStack::PushL( transaction );
	
	aEvent.SIPEvent().SetPendingTransactionL( transaction );
	CleanupStack::Pop( transaction );
	
	aEvent.EventContext().SetCurrentStateL( aEvent, nextState );
	MCEEVENTS_DEBUG("CMceEventReferMtEstablishedState::HandleL, Exit");
    }

// -----------------------------------------------------------------------------
// CMceEventReferMtEstablishedState::HandleReceiveRequestL
// Ápply concrete state.
// Be noticed that it will be also Refer/Bye or Refer/Invite(update)
// -----------------------------------------------------------------------------
//
void CMceEventReferMtEstablishedState::HandleReceiveRequestL( 
														CMceComEvent& aEvent )
	{
	MCEEVENTS_DEBUG("CMceEventReferMtEstablishedState::HandleReceiveRequestL, Entry");
	User::LeaveIfError( aEvent.SIPEvent().Dialog() ?
	    KErrNone : KMceEventStateError );
        
	User::LeaveIfError( aEvent.SIPEvent().Dialog()->Type() 
        == SIPStrings::StringF( SipStrConsts::ENotify ) ?
        KErrNone : KMceEventStateError );
    
    CSIPServerTransaction& serverTran = aEvent.SIPEvent().Request();
    CSIPServerTransaction* serverTranCheck = &serverTran;
	User::LeaveIfError( serverTranCheck ? KErrNone : KMceEventStateError );
	
	//Received Subscribe from Mo there is no action in the previous action
	User::LeaveIfError( aEvent.PreviousAction() == KErrNotFound ?
	    KErrNone : KMceEventStateError );
	
    User::LeaveIfError( CSIPDialog::EConfirmed ==
        aEvent.SIPEvent().Dialog()->Dialog().State() ?
        KErrNone : KMceEventStateError );
    
    User::LeaveIfError( serverTran.RequestElements() ? KErrNone : KErrCorrupt) ;
					
	const CSIPMessageElements& subscribeMsg = 
	    serverTran.RequestElements()->MessageElements();
			
	TBool terminated = EFalse;
    CSIPResponseElements* response = 
        MCESIPEventHelper::SubscribeRequestReceivedLC( aEvent, subscribeMsg );
    
    if ( response->StatusCode() == KMceSipOK )
		{

		MCESIPEventHelper::HandleExpireHeaderL(
        								subscribeMsg,
        								terminated ); 
   
        aEvent.SIPEvent().Request().SendResponseL( response );
        
        CleanupStack::Pop( response );
        //then send Notify

		CSIPNotifyDialogAssoc* dialogAssoc = 
    			static_cast <CSIPNotifyDialogAssoc* >
    			(aEvent.SIPEvent().Dialog()); 

		CSIPMessageElements* notifyMsg = 
	        MCESIPEventHelper::CreateReferNotifyMessageElementsLC( aEvent );
		
		CSIPSubscriptionStateHeader& subscrState = 
		    dialogAssoc->SubscriptionState();
		
		TInt nextState = KErrNotFound;		
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
			subscrState.SetExpiresParameterL(aEvent.RefreshInterval());
            nextState = KMceEstablishedEventStateIndex;
			}
        
        CSIPClientTransaction* tx = dialogAssoc->SendNotifyL( notifyMsg );
		CleanupStack::Pop( notifyMsg );
	    CleanupStack::PushL( tx );
		aEvent.SIPEvent().SetPendingTransactionL( tx );
		CleanupStack::Pop( tx );	
		
        aEvent.EventContext().SetCurrentStateL( aEvent, nextState );
        }
	else
	  	{
	  	// bad request
	    aEvent.SIPEvent().Request().SendResponseL( response );
	    CleanupStack::Pop( response );
	    }

	MCEEVENTS_DEBUG("CMceEventReferMtEstablishedState::HandleReceiveRequestL, Exit");
	}
	
// -----------------------------------------------------------------------------
// CMceEventReferMtEstablishedState::HandleResponseReceivedL
// Ápply concrete state.
// Be noticed that it will be also update, unRefer
// -----------------------------------------------------------------------------
//	
void CMceEventReferMtEstablishedState::HandleResponseReceivedL( 
												CMceComEvent& aEvent )
    {
    MCEEVENTS_DEBUG("CMceEventReferMtEstablishedState::HandleResponseReceivedL, Entry");
    CSIPClientTransaction& clientTran = aEvent.SIPEvent().Response();
    CSIPClientTransaction* clientTranCheck = &clientTran;;
	User::LeaveIfError( clientTranCheck ? KErrNone : KMceEventStateError );
		 
    User::LeaveIfError( 
        clientTran.ResponseElements() ? KErrNone : KErrCorrupt );
	
	// Response for first Notify comes here.
    aEvent.SetPreviousAction( KErrNotFound ); 
    TUint statusCode = clientTran.ResponseElements()->StatusCode();
    if ( statusCode >= KMceSipOK )
		{
		CMceRefer::TState nextClientState = CMceRefer::EPending;
		TMceEventStateIndex nextState = KErrNotFound;
        if ( statusCode < KMceSipMultipleChoices )
            {
            nextClientState = CMceRefer::EAccepted;
            nextState = KMceEstablishedEventStateIndex;
            }
        else 
            {
            nextClientState = CMceRefer::ETerminated;
            nextState = KMceTerminatedEventStateIndex;
            }
        aEvent.SIPEvent().ClientStateChangedL( nextClientState, ETrue );
        aEvent.EventContext().SetCurrentStateL( aEvent, nextState );
        }
	MCEEVENTS_DEBUG("CMceEventReferMtEstablishedState::HandleResponseReceivedL, Exit");        
    }
    
//  End of File  
