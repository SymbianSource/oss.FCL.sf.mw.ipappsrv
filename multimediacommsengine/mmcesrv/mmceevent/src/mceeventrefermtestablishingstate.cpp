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
* Description:    MT refer is received and waiting for action from client.
*
*/




// INCLUDE FILES
#include <sipsubscriptionstateheader.h>
#include <sipnotifydialogassoc.h>
#include <sipresponseelements.h>
#include "mceeventrefermtestablishingstate.h"
#include "mcesipeventhelper.h"
#include "mcesip.h"
#include "mceeventslogs.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceEventReferMtEstablishingState::CMceEventReferMtEstablishingState
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMceEventReferMtEstablishingState::CMceEventReferMtEstablishingState()
    {
    }

// -----------------------------------------------------------------------------
// CMceEventReferMtEstablishingState::~CMceEventReferMtEstablishingState
// Destructor
// -----------------------------------------------------------------------------
//
CMceEventReferMtEstablishingState::~CMceEventReferMtEstablishingState()
    {
    }

// -----------------------------------------------------------------------------
// CMceEventReferMtEstablishingState::ProcessReferStateL
// Ápply concrete state.
// Be noticed that it will be also update, unRefer
// -----------------------------------------------------------------------------
//
void CMceEventReferMtEstablishingState::HandleL( 
												CMceComEvent& aEvent )
    {
    MCEEVENTS_DEBUG("CMceEventReferMtEstablishingState::HandleL, Entry");
    //Check after the receiving Refer, client want to accept/Reject the event
    User::LeaveIfError( aEvent.SIPEvent().Dialog() ?
        KErrNone : KMceEventStateError );
        
    User::LeaveIfError( aEvent.SIPEvent().Dialog()->Type() 
        == SIPStrings::StringF( SipStrConsts::ENotify ) ?
       KErrNone : KMceEventStateError );
        
    CSIPServerTransaction& serverTran = aEvent.SIPEvent().Request();    
    CSIPServerTransaction* serverTranCheck = &serverTran;    
    User::LeaveIfError( serverTranCheck ? KErrNone : KMceEventStateError );
    
    const CSIPRequestElements* request = serverTran.RequestElements();
    User::LeaveIfError( request ? KErrNone : KErrCorrupt );
     
    CSIPResponseElements* response =NULL;	

    switch ( aEvent.PreviousAction() )
        {
        case EMceItcAcceptEvent:
            {
            //return 202
            RStringF phrase = 
        	    SIPStrings::Pool().OpenFStringL( KMceSipPhraseAccepted );
        	CleanupClosePushL( phrase );
            response = MCESIPEventHelper::CreateResponseL( 
                                                    aEvent,
                                                    KMceSipAccepted,
                                                    phrase );
    	    CleanupStack::PopAndDestroy(); // phrase	
    	    break;
            }
        case EMceItcRejectEvent:
            {
            //RejectEvent
            response = MCESIPEventHelper::CreateResponseL( aEvent,
                                                           KMceSipDecline );
            aEvent.EventContext().SetCurrentStateL( 
                                            aEvent, 
                                            KMceTerminatedEventStateIndex );
        	break;
            }
        case EMceItcRespondEvent:
            {
            //Respond event 
        	RStringF phrase = 
        	    SIPStrings::Pool().OpenFStringL( aEvent.ReasonPhrase() );
        	CleanupClosePushL( phrase );
        	response = MCESIPEventHelper::CreateResponseL( 
                                                    aEvent,
                                                    aEvent.StatusCode(),
                                                    phrase,
                                                    ETrue /* aAddClientData */);
            CleanupStack::PopAndDestroy(); // phrase                                                    
        	break;
            }
        default:
            {
            User::Leave( KMceEventStateError );
            break;
            }
        }
    
    CleanupStack::PushL( response );
    
    serverTran.SendResponseL( response );
   	CleanupStack::Pop( response );
	
	//then send Notify
	TUint statusCode = response->StatusCode();
   	
	if ( statusCode == KMceSipAccepted || statusCode == aEvent.StatusCode() )
		{
		if(response->StatusCode() >= KMceSipOK)
			{
	        if ( statusCode < KMceSipMultipleChoices 
	          && aEvent.ReferType() == CMceRefer::ENoSuppression )
				{
				CSIPSubscriptionStateHeader& subStateHeader = 
				( static_cast< CSIPNotifyDialogAssoc* >
				( aEvent.SIPEvent().Dialog() ) )->SubscriptionState();
				
	        	subStateHeader.SetSubStateValueL( KStateActive );
	        	subStateHeader.SetExpiresParameterL( aEvent.RefreshInterval() );
	        	CSIPMessageElements* msgElem = 
	        	    MCESIPEventHelper::CreateReferNotifyMessageElementsLC( aEvent );
	        	                    
				CSIPClientTransaction* transaction = 
					( static_cast< CSIPNotifyDialogAssoc* >
					( aEvent.SIPEvent().Dialog() ) )->SendNotifyL( msgElem );
					
				CleanupStack::Pop( msgElem );
				CleanupStack::PushL( transaction );
				aEvent.SIPEvent().SetPendingTransactionL( transaction );
				CleanupStack::Pop( transaction );
				aEvent.EventContext().SetCurrentStateL( 
		                                    aEvent, 
		                                    KMceEstablishedEventStateIndex );
				}
			else
				{
				// Terminated, if received error response or the 
				// REFER is supppressed.
				aEvent.EventContext().SetCurrentStateL( 
		                                    aEvent, 
		                                    KMceTerminatedEventStateIndex );
				}
			}
		}
	MCEEVENTS_DEBUG("CMceEventReferMtEstablishingState::HandleL, Exit");		
    }

// -----------------------------------------------------------------------------
// CMceEventReferMtEstablishingState::HandleReceiveRequestL
// Ápply concrete state.
// Be noticed that it will be also update, unRefer
// -----------------------------------------------------------------------------
//
void CMceEventReferMtEstablishingState::HandleReceiveRequestL( 
												CMceComEvent& /*aEvent*/ )
	{
	}
	
// -----------------------------------------------------------------------------
// CMceEventReferMtEstablishingState::HandleResponseReceivedL
// Ápply concrete state.
// Be noticed that it will be also update, unRefer
// -----------------------------------------------------------------------------
//	
void CMceEventReferMtEstablishingState::HandleResponseReceivedL(
													CMceComEvent& aEvent )
	{
	MCEEVENTS_DEBUG("CMceEventReferMtEstablishingState::HandleResponseReceivedL, Entry");
    User::LeaveIfError( aEvent.PreviousAction() == EMceItcNotify ?
        KErrNone : KMceEventStateError );
    
    User::LeaveIfError( &aEvent.SIPEvent().Response() ?
        KErrNone : KMceEventStateError );
        
    User::LeaveIfError( aEvent.SIPEvent().Response().Type()==
	    SIPStrings::StringF(SipStrConsts::ENotify) ?
	    KErrNone : KMceEventStateError );
	
    aEvent.SetPreviousAction( KErrNotFound ); 
    
    const CSIPResponseElements* response = 
        aEvent.SIPEvent().Response().ResponseElements();
    User::LeaveIfError( response ? KErrNone : KErrCorrupt );

    TUint statusCode = response->StatusCode();
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
	MCEEVENTS_DEBUG("CMceEventReferMtEstablishingState::HandleResponseReceivedL, Exit");        
    }

//  End of File  
