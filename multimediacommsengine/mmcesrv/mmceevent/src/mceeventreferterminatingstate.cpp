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
* Description:    Refer terminating is in progress.
*
*/





// INCLUDE FILES
#include <sipresponseelements.h>
#include <sipdialogassocbase.h>
#include <sipdialog.h>
#include "mceeventreferterminatingstate.h"
#include "mcesip.h"
#include "mceeventslogs.h"
#include "mcesipeventhelper.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceEventReferTerminatingState::CMceEventReferTerminatingState
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMceEventReferTerminatingState::CMceEventReferTerminatingState()
    {
    }

// -----------------------------------------------------------------------------
// CMceEventReferTerminatingState::~CMceEventReferTerminatingState
// Destructor
// -----------------------------------------------------------------------------
//
CMceEventReferTerminatingState::~CMceEventReferTerminatingState()
    {
    }

// -----------------------------------------------------------------------------
// CMceEventReferTerminatingState::ProcessReferStateL
// Ápply concrete state.
// Be noticed that it will be also update, unRefer
// -----------------------------------------------------------------------------
//
void CMceEventReferTerminatingState::HandleL( CMceComEvent& /*aEvent*/ )
    {
    User::Leave( KMceEventStateError );
    }

// -----------------------------------------------------------------------------
// CMceEventReferTerminatingState::HandleReceiveRequestL
// Ápply concrete state.
// Be noticed that it will be also update, unRefer
// -----------------------------------------------------------------------------
//
void CMceEventReferTerminatingState::HandleReceiveRequestL( 
												CMceComEvent& aEvent )
	{
	MCEEVENTS_DEBUG("CMceEventReferTerminatingState::HandleReceiveRequestL, Entry");
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
        MCESIPEventHelper::ReferNotifyRequestReceivedLC( aEvent, msgElem );
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

   	MCEEVENTS_DEBUG("CMceEventReferTerminatingState::HandleReceiveRequestL, Exit");
	}
	
// -----------------------------------------------------------------------------
// CMceEventReferTerminatingState::HandleResponseReceivedL
// Ápply concrete state.
// Be noticed that it will be also update, unRefer
// -----------------------------------------------------------------------------
//	
void CMceEventReferTerminatingState::HandleResponseReceivedL( 
													CMceComEvent& aEvent )
	{
	MCEEVENTS_DEBUG("CMceEventReferTerminatingState::HandleResponseReceivedL, Entry");
	User::LeaveIfError( aEvent.SIPEvent().Dialog() ?
	    KErrNone : KMceEventStateError );
	
     // Wait for 200 ok response
     //check if this is 200 OK
     //and then wait for Notify goes to terminated state   
    if ( CSIPDialog::EConfirmed == aEvent.SIPEvent().Dialog()->Dialog().State()
        && aEvent.PreviousAction() == EMceItcTerminateEvent )
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
			    
			    if (aEvent.SIPEvent().EventStateType()==EMtRefer)
			    	{
			    	aEvent.SIPEvent().ClientStateChangedL(
                				                    	CMceRefer::ETerminated,
                					                    ETrue );   
			    	}
	            }
	        else 
	        	{
	            aEvent.SIPEvent().ClientStateChangedL(
	            								        CMceRefer::ETerminated,
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
        						                CMceRefer::ETerminated,
        					                	ETrue );          
        }
    else
        {
        User::Leave( KMceEventStateError );
        }
        
	MCEEVENTS_DEBUG("CMceEventReferTerminatingState::HandleResponseReceivedL, Exit");        
    }
    
//  End of File  
