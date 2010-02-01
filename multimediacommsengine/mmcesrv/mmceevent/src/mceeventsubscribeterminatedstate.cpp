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
* Description:    Event is terminated.
*
*/





// INCLUDE FILES
#include <siprequestelements.h>
#include <sipcontenttypeheader.h>
#include <sipresponseelements.h>
#include <sipdialogassocbase.h>
#include "mcesipeventhelper.h"
#include "mceeventsubscribeterminatedstate.h"
#include "mcesip.h"
#include "mceeventslogs.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceEventSubscribeTerminatedState::CMceEventSubscribeTerminatedState
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMceEventSubscribeTerminatedState::CMceEventSubscribeTerminatedState()
    {
    }

// -----------------------------------------------------------------------------
// CMceEventSubscribeTerminatedState::~CMceEventSubscribeTerminatedState
// Destructor
// -----------------------------------------------------------------------------
//
CMceEventSubscribeTerminatedState::~CMceEventSubscribeTerminatedState()
    {
    }

// -----------------------------------------------------------------------------
// CMceEventSubscribeTerminatedState::ProcessSubscribeStateL
// Ápply concrete state.
// Be noticed that it will be also update, unsubscribe
// -----------------------------------------------------------------------------
//
void CMceEventSubscribeTerminatedState::HandleL( 
												CMceComEvent& /*aEvent*/ )
    {
    User::Leave( KMceEventStateError );
    }

// -----------------------------------------------------------------------------
// CMceEventSubscribeTerminatedState::HandleReceiveRequestL
// Ápply concrete state.
// Be noticed that it will be also update, unsubscribe
// -----------------------------------------------------------------------------
//
void CMceEventSubscribeTerminatedState::HandleReceiveRequestL( 
												CMceComEvent& aEvent )
	{
	MCEEVENTS_DEBUG("CMceEventSubscribeTerminatedState::HandleReceiveRequestL, Entry");        
	User::LeaveIfError( aEvent.SIPEvent().Dialog() ?
	    KErrNone : KMceEventStateError );
        
    CSIPServerTransaction& serverTran = aEvent.SIPEvent().Request();
    CSIPServerTransaction* serverTranCheck = &serverTran;
	User::LeaveIfError( serverTranCheck ? KErrNone : KMceEventStateError );
	    
	//Received Subscribe from Mo there is noaction in the previous action
	User::LeaveIfError( aEvent.PreviousAction() == KErrNotFound ?
	    KErrNone : KMceEventStateError );
	
    User::LeaveIfError( serverTran.RequestElements() ? KErrNone : KErrCorrupt );
        
	const CSIPMessageElements& msgElem = 
	    serverTran.RequestElements()->MessageElements();
    // send 200 ok after received notify
    CDesC8Array* reqheaders = MceSip::UserHeadersToTextArrayL( 
                                        serverTran,
                                        aEvent.SIPEvent().Dialog()->Dialog() );
	CleanupStack::PushL( reqheaders );
	
	HBufC8* reqcontenttype=NULL;
	if( msgElem.ContentType() ) 
		{
		reqcontenttype = msgElem.ContentType()->ToTextValueL();
		}
		
	CleanupStack::PushL( reqcontenttype );
	HBufC8* reqcontent = 
        msgElem.Content().AllocL();
	CleanupStack::PushL( reqcontent );
	aEvent.SIPEvent().NotifyReceivedL( reqheaders,
									   reqcontenttype,
									   reqcontent );
    CleanupStack::Pop( reqcontent );
    CleanupStack::Pop( reqcontenttype );
    CleanupStack::Pop( reqheaders );

	CSIPResponseElements* newresponse = 
	    MCESIPEventHelper::NotifyRequestReceivedLC( aEvent, msgElem );
	if ( newresponse->StatusCode() == KMceSipOK )
		{
    
    	TBool terminated = EFalse;
    	MCESIPEventHelper::
    	    HandleSubscriptionStateHeaderL( msgElem, terminated );
    	serverTran.SendResponseL( newresponse );
    	CleanupStack::Pop( newresponse );
    	if ( terminated )
        	{
        	// when the subscription state is terminated
        	//receive notify send 200 OK

            aEvent.SIPEvent().ClientStateChangedL(
            					                CMceEvent::ETerminated,
            					                EFalse );                      
            }
        else
            {
            //the message Notify request-subscriptionstate is not corrected
            User::Leave( KErrArgument );
            }     
  		}
  	else
  		{
        //when the notify request is bad, state doesnt change
        serverTran.SendResponseL( newresponse );
        CleanupStack::Pop( newresponse );
   		User::Leave( KErrArgument );
    	}
	MCEEVENTS_DEBUG("CMceEventSubscribeTerminatedState::HandleReceiveRequestL, Exit");    	
	}
	
// -----------------------------------------------------------------------------
// CMceEventSubscribeTerminatedState::HandleResponseReceivedL
// Ápply concrete state.
// Be noticed that it will be also update, unsubscribe
// -----------------------------------------------------------------------------
//	
void CMceEventSubscribeTerminatedState::HandleResponseReceivedL( 
												CMceComEvent& /*aEvent*/ )
	{
	}
	
//  End of File  
