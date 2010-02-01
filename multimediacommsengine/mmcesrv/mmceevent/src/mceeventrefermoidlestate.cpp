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
* Description:    Initial state for MO refer.
*
*/





// INCLUDE FILES
#include <sipreferdialogassoc.h>
#include "mceeventrefermoidlestate.h"
#include "mcesipeventhelper.h"
#include <sipmessageelements.h>
#include "mceeventslogs.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceEventReferMoIdleState::CMceEventReferMoIdleState
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMceEventReferMoIdleState::CMceEventReferMoIdleState()
    {
    }

// -----------------------------------------------------------------------------
// CMceEventReferMoIdleState::~CMceEventReferMoIdleState
// Destructor
// -----------------------------------------------------------------------------
//
CMceEventReferMoIdleState::~CMceEventReferMoIdleState()
    {
    }

// -----------------------------------------------------------------------------
// CMceEventReferMoIdleState::ProcessReferStateL
// Ápply concrete state.
// -----------------------------------------------------------------------------
//
void CMceEventReferMoIdleState::HandleL( CMceComEvent& aEvent )
    {
    MCEEVENTS_DEBUG("CMceEventReferMoIdleState::HandleL, Entry");
    User::LeaveIfError( aEvent.SIPEvent().Dialog() ?
        KErrNone : KMceEventStateError );
        
    User::LeaveIfError( aEvent.SIPEvent().Dialog()->Type() ==
        SIPStrings::StringF( SipStrConsts::ERefer ) ?
        KErrNone : KMceEventStateError );
        
    CSIPMessageElements* msgElem = 
    	MCESIPEventHelper::CreateMessageElementsLC(
    	                                    aEvent,
    	                                    SipStrConsts::ERefer,
    	                                    aEvent.ReferType() );
    // send Refer
    CSIPClientTransaction* transaction = 
        static_cast< CSIPReferDialogAssoc* >( aEvent.SIPEvent().Dialog() )->
        SendReferL( msgElem );
    CleanupStack::Pop( msgElem );
    CleanupStack::PushL( transaction );
    
    // save the transaction    
    aEvent.SIPEvent().SetPendingTransactionL( transaction );
	CleanupStack::Pop( transaction ); 
    
    // Set the next state
    aEvent.EventContext().SetCurrentStateL( 
                                    aEvent, 
                                    KMceEstablishingEventStateIndex );
	MCEEVENTS_DEBUG("CMceEventReferMoIdleState::HandleL, Exit");                                    
    }

// -----------------------------------------------------------------------------
// CMceEventReferMoIdleState::HandleReceiveRequestL
// Ápply concrete state.
// Be noticed that it will be also update, unRefer
// -----------------------------------------------------------------------------
//
void CMceEventReferMoIdleState::HandleReceiveRequestL(
												 CMceComEvent& /*aEvent*/ )
	{
	User::Leave( KMceEventStateError );
	}
	
// -----------------------------------------------------------------------------
// CMceEventReferMoIdleState::HandleResponseReceivedL
// Ápply concrete state.
// Be noticed that it will be also update, unRefer
// -----------------------------------------------------------------------------
//	
void CMceEventReferMoIdleState::HandleResponseReceivedL( 
												CMceComEvent& /*aEvent*/ )
	{
	User::Leave( KMceEventStateError );
	}

//  End of File  
