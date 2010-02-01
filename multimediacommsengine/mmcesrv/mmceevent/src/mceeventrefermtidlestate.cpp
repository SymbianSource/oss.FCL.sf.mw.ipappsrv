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
* Description:    Initial state for MT refer.
*
*/




// INCLUDE FILES
#include <sipcontenttypeheader.h>
#include <siprefertoheader.h>
#include <sipdialogassocbase.h>
#include "mcesipeventhelper.h"
#include "mceeventrefermtidlestate.h"
#include "mcesip.h"
#include "mceeventslogs.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceEventReferMtIdleState::CMceEventReferMtIdleState
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMceEventReferMtIdleState::CMceEventReferMtIdleState()
    {
    }

// -----------------------------------------------------------------------------
// CMceEventReferMtIdleState::~CMceEventReferMtIdleState
// Destructor
// -----------------------------------------------------------------------------
//
CMceEventReferMtIdleState::~CMceEventReferMtIdleState()
    {
    }

// -----------------------------------------------------------------------------
// CMceEventReferMtIdleState::ProcessReferStateL
// Ápply concrete state.
// Be noticed that it will be also update, unRefer
// -----------------------------------------------------------------------------
//
void CMceEventReferMtIdleState::HandleL( CMceComEvent& /*aEvent*/ )
    {
    User::Leave( KMceEventStateError );
    }

// -----------------------------------------------------------------------------
// CMceEventReferMtIdleState::HandleReceiveRequestL
// Ápply concrete state.
// Be noticed that it is receiving notify in the terminated state
// -----------------------------------------------------------------------------
//
void CMceEventReferMtIdleState::HandleReceiveRequestL( CMceComEvent& aEvent )
	{
	MCEEVENTS_DEBUG("CMceEventReferMtIdleState::HandleReceiveRequestL, Entry");
	User::LeaveIfError( aEvent.SIPEvent().Dialog() ? 
	    KErrNone : KMceEventStateError );
	CSIPServerTransaction& serverTran = aEvent.SIPEvent().Request();
	CSIPServerTransaction* serverTranCheck = &serverTran;
    
	User::LeaveIfError( serverTranCheck ? KErrNone : KMceEventStateError );
	
    User::LeaveIfError( serverTran.RequestElements() ? KErrNone : KErrCorrupt );
    
    const CSIPMessageElements& msgElem = 
        serverTran.RequestElements()->MessageElements();
    
    // Set the REFER type
    aEvent.SetReferType( MCESIPEventHelper::ResolveReferTypeL( msgElem ) );
    
    // Gather parameters and notify observer.
	CDesC8Array* headers = MceSip::UserHeadersToTextArrayL( 
	                                serverTran, 
	                                aEvent.SIPEvent().Dialog()->Dialog() );
	CleanupStack::PushL( headers );
	
	HBufC8* contenttype= NULL;
	if( msgElem.ContentType() )
		{
		contenttype=msgElem.ContentType()->ToTextValueL();	
		}
	CleanupStack::PushL( contenttype );
	
	HBufC8* content = msgElem.Content().AllocLC();
	
	CSIPHeaderBase* referToHeader= MceSip::FindHeader(
						msgElem,
						SIPStrings::StringF(SipStrConsts::EReferToHeader));
	User::LeaveIfError( referToHeader ? KErrNone : KErrNotFound );
	
	HBufC8* refer = referToHeader->ToTextValueLC();
    
	aEvent.SIPEvent().IncomingReferL( refer,
								      headers,
                				      contenttype,
								      content );

	
    CleanupStack::Pop( refer );
	CleanupStack::Pop( content ); 
	CleanupStack::Pop( contenttype ); 
	CleanupStack::Pop( headers );
	//move to EstablishingState 
	aEvent.EventContext().SetCurrentStateL( aEvent, 
	                                        KMceEstablishingEventStateIndex );	
	MCEEVENTS_DEBUG("CMceEventReferMtIdleState::HandleReceiveRequestL, Exit");	                                        	
	}
 
// -----------------------------------------------------------------------------
// CMceEventReferMtIdleState::HandleResponseReceivedL
// Ápply concrete state.
// Be noticed that it will be also update, unRefer
// -----------------------------------------------------------------------------
//	
void CMceEventReferMtIdleState::HandleResponseReceivedL( 
                                                    CMceComEvent& /*aEvent*/ )
	{
	User::Leave( KMceEventStateError );
	}

//  End of File  
