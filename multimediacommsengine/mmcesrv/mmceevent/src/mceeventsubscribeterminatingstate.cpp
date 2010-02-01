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
* Description:    Terminating event is in progress.
*
*/





// INCLUDE FILES
#include <sipresponseelements.h>
#include <sipsubscribedialogassoc.h>
#include <sipdialog.h>
#include "mceeventsubscribeterminatingstate.h"
#include "mcesip.h"
#include "mceeventslogs.h"
#include "mcesipeventhelper.h"



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceEventSubscribeTerminatingState::CMceEventSubscribeTerminatingState
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMceEventSubscribeTerminatingState::CMceEventSubscribeTerminatingState()
    {
    }

// -----------------------------------------------------------------------------
// CMceEventSubscribeTerminatingState::~CMceEventSubscribeTerminatingState
// Destructor
// -----------------------------------------------------------------------------
//
CMceEventSubscribeTerminatingState::~CMceEventSubscribeTerminatingState()
    {
    }

// -----------------------------------------------------------------------------
// CMceEventSubscribeTerminatingState::ProcessSubscribeStateL
// Ápply concrete state.
// Be noticed that it will be also update, unsubscribe
// -----------------------------------------------------------------------------
//
void CMceEventSubscribeTerminatingState::HandleL( 
												CMceComEvent& /*aEvent*/ )
    {
    User::Leave( KMceEventStateError );
    }

// -----------------------------------------------------------------------------
// CMceEventSubscribeTerminatingState::HandleReceiveRequestL
// Ápply concrete state.
// Be noticed that it will be also update, unsubscribe
// -----------------------------------------------------------------------------
//
void CMceEventSubscribeTerminatingState::HandleReceiveRequestL( 
												CMceComEvent& aEvent )
	{
	MCEEVENTS_DEBUG("CMceEventSubscribeTerminatingState::HandleReceiveRequestL, Entry");
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
        
        serverTran.SendResponseL( response );
        CleanupStack::Pop( response );

        aEvent.EventContext().SetCurrentStateL( 
                                            aEvent, 
                                            KMceTerminatedEventStateIndex );
        aEvent.SIPEvent().ClientStateChangedL(
                                            CMceEvent::ETerminated,
                                            EFalse );                                        
      	}
    else
      	{
        //when the notify request is bad, state doesnt change
        serverTran.SendResponseL( response );
        CleanupStack::Pop( response );
        }

   	MCEEVENTS_DEBUG("CMceEventSubscribeTerminatingState::HandleReceiveRequestL, Exit");
	}
	
// -----------------------------------------------------------------------------
// CMceEventSubscribeTerminatingState::HandleResponseReceivedL
// Ápply concrete state.
// Be noticed that it will be also update, unsubscribe
// -----------------------------------------------------------------------------
//	
void CMceEventSubscribeTerminatingState::HandleResponseReceivedL(
													CMceComEvent& aEvent )
	{
	MCEEVENTS_DEBUG("CMceEventSubscribeTerminatingState::HandleResponseReceivedL, Entry");
	User::LeaveIfError( aEvent.SIPEvent().Dialog() ?
	    KErrNone : KMceEventStateError );
        
    if ( CSIPDialog::EConfirmed == aEvent.SIPEvent().Dialog()->Dialog().State() &&
         ( aEvent.PreviousAction() == EMceItcTerminateEvent || 
           aEvent.PreviousAction() == KErrNotFound ) )
        {
	    aEvent.SetPreviousAction( KErrNotFound );
	    
	    User::LeaveIfError( &aEvent.SIPEvent().Response() ?
	        KErrNone : KMceEventStateError );
	        
	    const CSIPResponseElements* response = 
	        aEvent.SIPEvent().Response().ResponseElements();
	    
	    User::LeaveIfError( response ? KErrNone : KErrCorrupt );

	    TUint statusCode = response->StatusCode();
		if (statusCode >= KMceSipOK)
		    {
		    if (statusCode < KMceSipMultipleChoices )
		        {
		        aEvent.EventContext().SetCurrentStateL( 
				                            aEvent, 
				                            KMceTerminatedEventStateIndex );
				if (aEvent.SIPEvent().EventStateType()==EMtSubscribe)
				    {
				    aEvent.SIPEvent().ClientStateChangedL(
	                                                    CMceEvent::ETerminated,
	                                                    ETrue );   	
				    }
		        }
		    else 
		        {
		        aEvent.SIPEvent().ClientStateChangedL(
		                                                CMceEvent::ETerminated,
		                                                ETrue );
		        aEvent.EventContext().SetCurrentStateL( 
	                                        aEvent, 
	                                        KMceTerminatedEventStateIndex );
		        }
		    }    
        }
    else if ( CSIPDialog::ETerminated ==
    						aEvent.SIPEvent().Dialog()->Dialog().State() )
        {
        // Set next state: terminated.
        aEvent.EventContext().SetCurrentStateL( 
                                            aEvent, 
                                            KMceTerminatedEventStateIndex );
        aEvent.SIPEvent().ClientStateChangedL(
                                            CMceEvent::ETerminated,
                                            ETrue );         
        }
    else
        {
        User::Leave( KMceEventStateError );
        }
	MCEEVENTS_DEBUG("CMceEventSubscribeTerminatingState::HandleResponseReceivedL, Exit");        
    }
    
//  End of File  
