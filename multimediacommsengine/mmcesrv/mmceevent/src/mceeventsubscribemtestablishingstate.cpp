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
* Description:    MT subscribe is received and waiting for action from client.
*
*/





// INCLUDE FILES
#include "mceeventsubscribemtestablishingstate.h"
#include <sipsubscriptionstateheader.h>
#include <sipresponseelements.h>
#include <sipnotifydialogassoc.h>
#include "mcesip.h"
#include "mcesipeventhelper.h"
#include "mceeventslogs.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceEventSubscribeMtEstablishingState::CMceEventSubscribeMtEstablishingState
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMceEventSubscribeMtEstablishingState::CMceEventSubscribeMtEstablishingState()
    {
    }
    
// -----------------------------------------------------------------------------
// CMceEventSubscribeMtEstablishingState::~CMceEventSubscribeMtEstablishingState
// Destructor
// -----------------------------------------------------------------------------
//
CMceEventSubscribeMtEstablishingState::~CMceEventSubscribeMtEstablishingState()
    {
    }

// -----------------------------------------------------------------------------
// CMceEventSubscribeMtEstablishingState::ProcessSubscribeStateL
// Ápply concrete state.
// Be noticed that it will be also update, unsubscribe
// -----------------------------------------------------------------------------
//
void CMceEventSubscribeMtEstablishingState::HandleL( CMceComEvent& aEvent)
    {
    MCEEVENTS_DEBUG("CMceEventSubscribeMtEstablishingState::HandleL, Entry");        
    //Check after the receiving Subscribe, client want to accept/Reject the event
    User::LeaveIfError( aEvent.SIPEvent().Dialog() ?
        KErrNone : KMceEventStateError );
        
    User::LeaveIfError( aEvent.SIPEvent().Dialog()->Type() 
        == SIPStrings::StringF( SipStrConsts::ENotify ) ?
        KErrNone : KMceEventStateError );
        
    User::LeaveIfError( &aEvent.SIPEvent().Request() ?
        KErrNone : KMceEventStateError );
        
    const CSIPRequestElements* request = 
        aEvent.SIPEvent().Request().RequestElements();
    User::LeaveIfError( request ? KErrNone : KErrCorrupt );
     
    CSIPResponseElements* response = NULL;	

    switch( aEvent.PreviousAction() )
        {
        case EMceItcAcceptEvent:
            {
            //Accept event	
            response = MCESIPEventHelper::CreateResponseL( 
                                                    aEvent,
                                                    KMceSipOK );
    	    break;
            }
        case EMceItcRejectEvent:
            {
            //Reject event
            response = MCESIPEventHelper::CreateResponseL( 
                                                    aEvent,
                                                    KMceSipDecline );
    	    
    	    aEvent.EventContext().SetCurrentStateL( 
                                            aEvent, 
                                            KMceTerminatedEventStateIndex );
    	    break;
            }
        case EMceItcRespondEvent:
            {
            //Respond event 
    	    RStringF reason = 
    	        SIPStrings::Pool().OpenFStringL( aEvent.ReasonPhrase() );
    	    CleanupClosePushL( reason );
    	    response = MCESIPEventHelper::CreateResponseL( 
                                                    aEvent,
                                                    aEvent.StatusCode(),
                                                    reason,
                                                    ETrue /*aAddClientData*/ );
    	    
    	    CleanupStack::PopAndDestroy(); // reason
            break;
            }
        default:
            {
            User::Leave( KMceEventStateError );
            break;
            }
        }
        
    CleanupStack::PushL( response );
   	aEvent.SIPEvent().Request().SendResponseL( response );
   	CleanupStack::Pop( response );
	//then sendNotify
	if ( response->StatusCode() == KMceSipOK|| 
		response->StatusCode() == aEvent.StatusCode() )
		{
		if( response->StatusCode() >= KMceSipOK )
			{
	        if ( response->StatusCode() < KMceSipMultipleChoices )
		
				{
				CSIPSubscriptionStateHeader& subStateHeader = 
				( static_cast< CSIPNotifyDialogAssoc* >
				( aEvent.SIPEvent().Dialog() ) )->SubscriptionState();
	        	
	        	subStateHeader.SetSubStateValueL( KStateActive );
	        	subStateHeader.SetExpiresParameterL( aEvent.RefreshInterval() );
	        	
	        	// message elements
	        	CSIPMessageElements* notifyMsg = 
                    MCESIPEventHelper::CreateMessageElementsLC( 
                                                aEvent,
                                                SipStrConsts::ENotify );
				CSIPClientTransaction* transaction = 
					( static_cast< CSIPNotifyDialogAssoc* >
					( aEvent.SIPEvent().Dialog() ) )->SendNotifyL( notifyMsg );
			    CleanupStack::Pop( notifyMsg );
				CleanupStack::PushL( transaction );
				aEvent.SIPEvent().SetPendingTransactionL( transaction );
				CleanupStack::Pop( transaction );
				aEvent.EventContext().SetCurrentStateL( 
		                                aEvent, 
		                                KMceEstablishedEventStateIndex );
				}
			else
				{
				aEvent.EventContext().SetCurrentStateL( 
				                          aEvent, 
				                          KMceTerminatedEventStateIndex );
				}
			}
		}
	MCEEVENTS_DEBUG("CMceEventSubscribeMtEstablishingState::HandleL, Exit");   
    }

// -----------------------------------------------------------------------------
// CMceEventSubscribeMtEstablishingState::HandleReceiveRequestL
// Ápply concrete state.
// Be noticed that it will be also update, unsubscribe
// -----------------------------------------------------------------------------
//
void CMceEventSubscribeMtEstablishingState::HandleReceiveRequestL( 
													CMceComEvent& /*aEvent*/ )
	{
	}
	
// -----------------------------------------------------------------------------
// CMceEventSubscribeMtEstablishingState::HandleResponseReceivedL
// Ápply concrete state.
// Be noticed that it will be also update, unsubscribe
// -----------------------------------------------------------------------------
//	
void CMceEventSubscribeMtEstablishingState::HandleResponseReceivedL(
													CMceComEvent& aEvent )
	{
	MCEEVENTS_DEBUG("CMceEventSubscribeMtEstablishingState::HandleResponseReceivedL, Entry");   
	User::LeaveIfError( &aEvent.SIPEvent().Response() ?
	    KErrNone : KMceEventStateError );
        
    User::LeaveIfError( aEvent.SIPEvent().Response().Type() 
        == SIPStrings::StringF( SipStrConsts::ENotify ) ?
        KErrNone : KMceEventStateError );
    
    User::LeaveIfError( aEvent.PreviousAction()== EMceItcNotify ?
        KErrNone : KMceEventStateError );
            
    const CSIPResponseElements* response = 
        aEvent.SIPEvent().Response().ResponseElements();
    User::LeaveIfError( response ? KErrNone : KErrCorrupt );
    
 	aEvent.SetPreviousAction( KErrNotFound ); 
    
    TUint statusCode = response->StatusCode();
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
		MCEEVENTS_DEBUG_NEXTCLIENTSTATE("NextClientState :", nextClientState )				            
		MCEEVENTS_DEBUG_NEXTSTATE("NextState :", nextState )		            
        aEvent.SIPEvent().ClientStateChangedL( nextClientState, ETrue );
        aEvent.EventContext().SetCurrentStateL( aEvent, nextState );
        }
	MCEEVENTS_DEBUG("CMceEventSubscribeMtEstablishingState::HandleResponseReceivedL, Exit");           
    }

//  End of File  
