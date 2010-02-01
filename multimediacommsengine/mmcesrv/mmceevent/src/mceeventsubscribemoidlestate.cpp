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
* Description:    Initial state for MO event.
*
*/





// INCLUDE FILES
#include <siprefresh.h>
#include <sipsubscribedialogassoc.h>
#include <sipmessageelements.h>
#include "mceeventsubscribemoidlestate.h"
#include "mcesipevent.h"
#include "mcesipeventhelper.h"
#include "mceeventslogs.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceEventSubscribeMoIdleState::CMceEventSubscribeMoIdleState
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMceEventSubscribeMoIdleState::CMceEventSubscribeMoIdleState()
    {
    }

// -----------------------------------------------------------------------------
// CMceEventSubscribeMoIdleState::~CMceEventSubscribeMoIdleState
// Destructor
// -----------------------------------------------------------------------------
//
CMceEventSubscribeMoIdleState::~CMceEventSubscribeMoIdleState()
    {
    }

// -----------------------------------------------------------------------------
// CMceEventSubscribeMoIdleState::ProcessSubscribeStateL
// Ápply concrete state.
// -----------------------------------------------------------------------------
//
void CMceEventSubscribeMoIdleState::HandleL( CMceComEvent& aEvent )
    {  
    MCEEVENTS_DEBUG("CMceEventSubscribeMoIdleState::ProcessSubscribeStateL, Entry");
    User::LeaveIfError( aEvent.SIPEvent().Dialog() ?
        KErrNone : KMceEventStateError );
        
    User::LeaveIfError( aEvent.SIPEvent().Dialog()->Type() == 
        SIPStrings::StringF( SipStrConsts::ESubscribe ) ?
        KErrNone : KMceEventStateError );
        
    // create message elements.
    CSIPMessageElements* msgElem = MCESIPEventHelper::CreateMessageElementsLC(
                                                    aEvent,
                                                    SipStrConsts::ESubscribe,
                                                    aEvent.RefreshInterval() );
    CleanupStack::Pop( msgElem );		
	CleanupStack::PushL( msgElem );					    
    // create refresh    
    CSIPRefresh* refresh = CSIPRefresh::NewL();
    CleanupStack::PushL( refresh );
    
    // send subscribe
    CSIPClientTransaction* transaction = 
        static_cast<CSIPSubscribeDialogAssoc*> 
        ( aEvent.SIPEvent().Dialog())->SendSubscribeL( msgElem, refresh );
    CleanupStack::Pop( refresh );
    CleanupStack::Pop( msgElem );
   	CleanupStack::PushL( transaction );
    // save the transaction    
    aEvent.SIPEvent().SetPendingTransactionL( transaction );
	CleanupStack::Pop( transaction );
    // Set the next state
    aEvent.EventContext().SetCurrentStateL( aEvent, 
                                            KMceEstablishingEventStateIndex );
	MCEEVENTS_DEBUG("CMceEventSubscribeMoIdleState::ProcessSubscribeStateL, Exit");                                            
    }

// -----------------------------------------------------------------------------
// CMceEventSubscribeMoIdleState::HandleReceiveRequestL
// Ápply concrete state.
// Be noticed that it will be also update, unsubscribe
// -----------------------------------------------------------------------------
//
void CMceEventSubscribeMoIdleState::HandleReceiveRequestL(
												CMceComEvent& /*aEvent*/ )
	{
	User::Leave( KMceEventStateError );
	}
	
// -----------------------------------------------------------------------------
// CMceEventSubscribeMoIdleState::HandleResponseReceivedL
// Ápply concrete state.
// Be noticed that it will be also update, unsubscribe
// -----------------------------------------------------------------------------
//	
void CMceEventSubscribeMoIdleState::HandleResponseReceivedL(
												CMceComEvent& /*aEvent*/ )
	{
	User::Leave( KMceEventStateError );
	}

//  End of File  
