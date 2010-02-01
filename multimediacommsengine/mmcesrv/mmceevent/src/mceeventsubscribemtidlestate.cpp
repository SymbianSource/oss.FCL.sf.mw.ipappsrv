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
* Description:    Initial state for MT event.
*
*/




// INCLUDE FILES
#include <sipcontenttypeheader.h>
#include <sipdialogassocbase.h>
#include "mceeventsubscribemtidlestate.h"
#include "mcesipeventhelper.h"
#include "mcesip.h"
#include "mceeventslogs.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceEventSubscribeMtIdleState::CMceEventSubscribeMtIdleState
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMceEventSubscribeMtIdleState::CMceEventSubscribeMtIdleState()
    {
    }

// -----------------------------------------------------------------------------
// CMceEventSubscribeMtIdleState::~CMceEventSubscribeMtIdleState
// Destructor
// -----------------------------------------------------------------------------
//
CMceEventSubscribeMtIdleState::~CMceEventSubscribeMtIdleState()
    {
    }

// -----------------------------------------------------------------------------
// CMceEventSubscribeMtIdleState::ProcessSubscribeStateL
// For sending out subscribe, update
// Be noticed that it will be also update, unsubscribe
// -----------------------------------------------------------------------------
//
void CMceEventSubscribeMtIdleState::HandleL( CMceComEvent& /*aEvent*/ )
    {
    User::Leave( KMceEventStateError );
    }

// -----------------------------------------------------------------------------
// CMceEventSubscribeMtIdleState::HandleReceiveRequestL
// Ápply concrete state.
// Be noticed that it is receiving notify and subscribe in the terminated state
// -----------------------------------------------------------------------------
//
void CMceEventSubscribeMtIdleState::HandleReceiveRequestL( 
													CMceComEvent& aEvent )
	{       
	MCEEVENTS_DEBUG("CMceEventSubscribeMtIdleState::HandleReceiveRequestL, Entry");
	User::LeaveIfError( aEvent.SIPEvent().Dialog() ? 
	    KErrNone : KMceEventStateError );
	 
	CSIPServerTransaction& serverTran = aEvent.SIPEvent().Request();
	CSIPServerTransaction* serverTranCheck = &serverTran;
    User::LeaveIfError( serverTranCheck ? KErrNone : KMceEventStateError );   
    User::LeaveIfError( serverTran.RequestElements() ?
         KErrNone : KMceEventStateError );
    
    const CSIPMessageElements& msgElem = 
        serverTran.RequestElements()->MessageElements();
    
	CDesC8Array* headers = MceSip::UserHeadersToTextArrayL( 
	                                serverTran, 
	                                aEvent.SIPEvent().Dialog()->Dialog() );
	
	CleanupStack::PushL( headers );
	
	HBufC8* contenttype= NULL;
	if( msgElem.ContentType() )
		{
		contenttype = msgElem.ContentType()->ToTextValueL();	
		}
			
	CleanupStack::PushL( contenttype );
	HBufC8* content = msgElem.Content().AllocLC();
	aEvent.SIPEvent().IncomingSubscribeL( headers,
                					      contenttype,
									      content );
	CleanupStack::Pop( content ); 
	CleanupStack::Pop( contenttype ); 
	CleanupStack::Pop( headers );
	
	CSIPResponseElements* response = 
	    MCESIPEventHelper::SubscribeRequestReceivedLC( aEvent, msgElem );	
	if ( response->StatusCode() == KMceSipOK )
		{
		TBool terminated = EFalse;
        
        MCESIPEventHelper::HandleExpireHeaderL( msgElem,terminated );
        if ( terminated )
            {
            //send 200 OK, and turn to Terminated
            //SendResponse
            serverTran.SendResponseL( response );
            CleanupStack::Pop( response ); 
            aEvent.EventContext().SetCurrentStateL( 
                                            aEvent, 
                                            KMceTerminatedEventStateIndex );
            //Tell client state has terminated
            aEvent.SIPEvent().ClientStateChangedL(
            				CMceEvent::ETerminated,
            				EFalse );
            }
        else
            {
            //to Estalishing state
			//delete the response
        	CleanupStack::PopAndDestroy( response ); 
			aEvent.EventContext().SetCurrentStateL( 
                                            aEvent, 
                                            KMceEstablishingEventStateIndex );
            }
		}
	else
		{
		//when the subscribe request is bad, sendresponse
        //state terminated
        serverTran.SendResponseL( response );
        CleanupStack::Pop( response ); 
        User::Leave( KErrArgument );
        //Should inform the subsession to delete the session 
		}
	MCEEVENTS_DEBUG("CMceEventSubscribeMtIdleState::HandleReceiveRequestL, Exit");
    }

// -----------------------------------------------------------------------------
// CMceEventSubscribeMtIdleState::HandleResponseReceivedL
// Ápply concrete state.
// Be noticed that it will be also update, unsubscribe
// -----------------------------------------------------------------------------
//	
void CMceEventSubscribeMtIdleState::HandleResponseReceivedL( 
                                                    CMceComEvent& /*aEvent*/ )
	{
	User::Leave( KMceEventStateError );
	}

//  End of File  
