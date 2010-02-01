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
* Description:    MO refer is sent and waiting for response.
*
*/





// INCLUDE FILES
#include <sipresponseelements.h>
#include <sipcseqheader.h>
#include <sipsubscribedialogassoc.h>
#include "mceeventrefermoestablishingstate.h"
#include "mcesipeventhelper.h"
#include "mcesip.h"
#include "mceeventslogs.h"

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceEventReferMoEstablishingState::CMceEventReferMoEstablishingState
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMceEventReferMoEstablishingState::CMceEventReferMoEstablishingState()
    {
    }

// -----------------------------------------------------------------------------
// CMceEventReferMoEstablishingState::~CMceEventReferMoEstablishingState
// Destructor
// -----------------------------------------------------------------------------
//
CMceEventReferMoEstablishingState::~CMceEventReferMoEstablishingState()
    {
    }

// -----------------------------------------------------------------------------
// CMceEventReferMoEstablishingState::ProcessReferStateL
// Ápply concrete state.
// Be noticed that it will be also update, unRefer
// -----------------------------------------------------------------------------
//
void CMceEventReferMoEstablishingState::HandleL( CMceComEvent& /*aEvent*/ )
    {
    User::Leave( KMceEventStateError );
    }

// -----------------------------------------------------------------------------
// CMceEventReferMoEstablishingState::HandleReceiveRequestL
// Ápply concrete state.
// Be noticed that it will be also update, unRefer
// -----------------------------------------------------------------------------
//
void CMceEventReferMoEstablishingState::HandleReceiveRequestL(
												CMceComEvent& aEvent )
	{
	MCEEVENTS_DEBUG("CMceEventReferMoEstablishingState::HandleReceiveRequestL, Entry");
	
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
       
        TBool terminated = EFalse;        
        MCESIPEventHelper::HandleSubscriptionStateHeaderL( msgElem, terminated );

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

	MCEEVENTS_DEBUG("CMceEventReferMoEstablishingState::HandleResponseReceivedL, Exit");        
	}
	
// -----------------------------------------------------------------------------
// CMceEventReferMoEstablishingState::HandleResponseReceivedL
// Ápply concrete state.
// Be noticed that it will be also update, unRefer
// -----------------------------------------------------------------------------
//	
void CMceEventReferMoEstablishingState::HandleResponseReceivedL(
														CMceComEvent& aEvent)
	{
	MCEEVENTS_DEBUG("CMceEventReferMoEstablishingState::HandleResponseReceivedL, Entry");
	    
	CSIPClientTransaction& clientTran = aEvent.SIPEvent().Response();
	CSIPClientTransaction* clientTranCheck = &clientTran;
	User::LeaveIfError( clientTranCheck ? KErrNone : KMceEventStateError );
    
    const CSIPResponseElements* response = clientTran.ResponseElements();
    User::LeaveIfError( response ? KErrNone : KErrCorrupt );
    
    //Checking if getting the error after sending the Refer
	User::LeaveIfError( aEvent.PreviousAction() == EMceItcReferEvent ?
	    KErrNone : KMceEventStateError );
	    
    aEvent.SetPreviousAction( KErrNotFound ); 
    
    TUint statusCode = response->StatusCode();
    if ( statusCode >= KMceSipOK )
		{
		// Store the CSeq header value
		const CSIPCSeqHeader* cSeq = response->CSeqHeader();
		User::LeaveIfError( cSeq ? KErrNone : KErrCorrupt );
		aEvent.SetIdValue( cSeq->Seq() );
		
		TMceEventStateIndex nextState = KMceEstablishedEventStateIndex;
        CMceRefer::TState nextObsState = CMceRefer::EAccepted;
        TBool isResponse = ETrue;
        if ( statusCode < KMceSipMultipleChoices )
            {
            if ( aEvent.ReferType() != CMceRefer::ENoSuppression )
                {
                // For suppressed refer, first notify established state
                aEvent.SIPEvent().ClientStateChangedL( nextObsState, isResponse );
                isResponse = EFalse;
                }
                
            if ( aEvent.ReferType() == CMceRefer::ENoSuppression )
                {
                // next state is established
                nextObsState = CMceRefer::EAccepted;
                nextState = KMceEstablishedEventStateIndex;
                }
            else if ( aEvent.ReferType() == CMceRefer::ESuppressed 
                && ( MCESIPEventHelper::
                ResolveReferTypeL( response->MessageElements() )
                == CMceRefer::ENoSuppression ) )
                {
                // Remote end did not understand suppression.
                // Notify terminated state and suppress subscription
                // silently.
                aEvent.SetSilentSuppressionOn();
                nextObsState = CMceRefer::ETerminated;
                nextState = KMceEstablishedEventStateIndex;
        	    }
            else
                {
                // suppressed, next state is terminated 
                nextObsState = CMceRefer::ETerminated; 
                nextState = KMceTerminatedEventStateIndex;
                }
            }
        else 
            {
            nextObsState = CMceRefer::ETerminated; 
            nextState = KMceTerminatedEventStateIndex;
            }
        aEvent.SIPEvent().ClientStateChangedL( nextObsState, isResponse );
        aEvent.EventContext().SetCurrentStateL( aEvent, nextState );
        }
        
	MCEEVENTS_DEBUG("CMceEventReferMoEstablishingState::HandleResponseReceivedL, Exit");        
	}

//  End of File  
