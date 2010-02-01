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
* Description:    Refer is terminated.
*
*/





// INCLUDE FILES
#include <siprequestelements.h>
#include <sipcontenttypeheader.h>
#include <sipresponseelements.h>
#include <sipdialogassocbase.h>
#include "mcesipeventhelper.h"
#include "mceeventreferterminatedstate.h"
#include "mcesip.h"
#include "mceeventslogs.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceEventReferTerminatedState::CMceEventReferTerminatedState
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMceEventReferTerminatedState::CMceEventReferTerminatedState()
    {
    }

// -----------------------------------------------------------------------------
// CMceEventReferTerminatedState::~CMceEventReferTerminatedState
// Destructor
// -----------------------------------------------------------------------------
//
CMceEventReferTerminatedState::~CMceEventReferTerminatedState()
    {
    }

// -----------------------------------------------------------------------------
// CMceEventReferTerminatedState::ProcessReferStateL
// Ápply concrete state.
// Be noticed that it will be also update, unRefer
// -----------------------------------------------------------------------------
//
void CMceEventReferTerminatedState::HandleL( CMceComEvent& /*aEvent*/ )
    {
    User::Leave( KMceEventStateError );
    }

// -----------------------------------------------------------------------------
// CMceEventReferTerminatedState::HandleReceiveRequestL
// Ápply concrete state.
// Be noticed that it will be also update, unRefer
// -----------------------------------------------------------------------------
//
void CMceEventReferTerminatedState::HandleReceiveRequestL( 
												CMceComEvent& aEvent )
	{
	MCEEVENTS_DEBUG("CMceEventReferTerminatedState::HandleReceiveRequestL, Entry");
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
    
    if ( !aEvent.SilentSuppression() )
        {
        // Notify after received notify
        CDesC8Array* reqheaders = MceSip::UserHeadersToTextArrayL( 
                                    serverTran, 
                                    aEvent.SIPEvent().Dialog()->Dialog() );
  
        CleanupStack::PushL( reqheaders );
        HBufC8* reqcontenttype= NULL;
        if( msgElem.ContentType() )
            {
            reqcontenttype = msgElem.ContentType()->ToTextValueL();	
            }

        CleanupStack::PushL( reqcontenttype );
        HBufC8* reqcontent =
        		msgElem.Content().AllocLC();
        aEvent.SIPEvent().NotifyReceivedL( reqheaders,
        							  	   reqcontenttype,
        							  	   reqcontent );
        CleanupStack::Pop( reqcontent );
        CleanupStack::Pop( reqcontenttype );
    	CleanupStack::Pop( reqheaders );
        }
    
	CSIPResponseElements* newresponse = 
		MCESIPEventHelper::ReferNotifyRequestReceivedLC( aEvent, msgElem );
	if ( newresponse->StatusCode() == KMceSipOK )
		{
    	TBool terminated = EFalse;
    	MCESIPEventHelper::HandleSubscriptionStateHeaderL( msgElem, 
    	                                                   terminated );
    	serverTran.SendResponseL( newresponse );
    	CleanupStack::Pop( newresponse );
    	if ( terminated )
        	{
        	// when the subscription state is terminated
        	//receive notify send 200 OK
            if ( !aEvent.SilentSuppression() )
                {
                aEvent.SIPEvent().ClientStateChangedL(
            					                    CMceRefer::ETerminated,
            					                    EFalse );          
                }
            }
        else
            {
            //the message Notify request is not corrected
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
	MCEEVENTS_DEBUG("CMceEventReferTerminatedState::HandleReceiveRequestL, Exit");        
    }
	
// -----------------------------------------------------------------------------
// CMceEventReferTerminatedState::HandleResponseReceivedL
// Ápply concrete state.
// Be noticed that it will be also update, unRefer
// -----------------------------------------------------------------------------
//	
void CMceEventReferTerminatedState::HandleResponseReceivedL( 
												CMceComEvent& /*aEvent*/ )
	{
	}

//  End of File  
