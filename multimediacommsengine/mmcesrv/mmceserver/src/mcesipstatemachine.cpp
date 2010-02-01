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
* Description:  
*
*/





#include "mcesipstatemachine.h"
#include "mcesipsession.h"
#include "mcestateidle.h"
#include "mcestateclientestablishing.h"
#include "mcestateoffering.h"
#include "mcestateestablished.h"
#include "mcestateterminating.h"
#include "mcestateterminated.h"
#include "mcestateserverestablishing.h"
#include "mcestateupdating.h"
#include "mcestateupdated.h"
#include "mcestateanswering.h"
#include "mcestateerror.h"
#include "mcestatecanceled.h"
#include "mcestateconfirming.h"
#include "mcestateclientreserving.h"
#include "mcestateacknowledgementrequired.h"
#include "mcestateserverreserving.h"
#include "mcestateconfirmationrequired.h"
#include "mcestateserveroffering.h"
#include "mcestateserverinitializing.h"

#include "mceclientserver.h"
#include "mceevents.h"
#include "mcesrvlogs.h"


// -----------------------------------------------------------------------------
// CMceSipStateMachine::NewL
// -----------------------------------------------------------------------------
//
CMceSipStateMachine* CMceSipStateMachine::NewL()
	{
    CMceSipStateMachine* self = CMceSipStateMachine::NewLC();
    CleanupStack::Pop(self);
    return self;
	}

// -----------------------------------------------------------------------------
// CMceSipStateMachine::NewLC
// -----------------------------------------------------------------------------
//
CMceSipStateMachine* CMceSipStateMachine::NewLC ()
	{
    CMceSipStateMachine* self = new (ELeave) CMceSipStateMachine();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
	}

// -----------------------------------------------------------------------------
// CMceSipStateMachine::CMceSipStateMachine
// -----------------------------------------------------------------------------
//
CMceSipStateMachine::CMceSipStateMachine ()
	{
	}

// -----------------------------------------------------------------------------
// CMceSipStateMachine::State
// -----------------------------------------------------------------------------
//
CMceState& CMceSipStateMachine::State ( TMceStateIndex aStateIndex )
	{
	return *(iStates[ aStateIndex ]);
    
	}

	
	
// -----------------------------------------------------------------------------
// CMceSipStateMachine::ConstructL
// -----------------------------------------------------------------------------
//
void CMceSipStateMachine::ConstructL ()
	{
    MCESRV_DEBUG("CMceSipStateMachine::ConstructL, Entry");
	
	CMceState* state = NULL;
	//
	state = new (ELeave) CMceStateIdle();
	CleanupStack::PushL( state );
	iStates.InsertL( state, state->Id() );
	CleanupStack::Pop( state );
	state = new (ELeave) CMceStateClientEstablishing();
	CleanupStack::PushL( state );
	iStates.InsertL( state, state->Id() );
	CleanupStack::Pop( state );
	state = new (ELeave) CMceStateOffering();
	CleanupStack::PushL( state );
	iStates.InsertL( state, state->Id() );
	CleanupStack::Pop( state );
	state = new (ELeave) CMceStateEstablished();
	CleanupStack::PushL( state );
	iStates.InsertL( state, state->Id() );
	CleanupStack::Pop( state );
	state = new (ELeave) CMceStateTerminating();
	CleanupStack::PushL( state );
	iStates.InsertL( state, state->Id() );
	CleanupStack::Pop( state );
	state = new (ELeave) CMceStateTerminated();
	CleanupStack::PushL( state );
	iStates.InsertL( state, state->Id() );
	CleanupStack::Pop( state );
	state = new (ELeave) CMceStateServerEstablishing();
	CleanupStack::PushL( state );
	iStates.InsertL( state, state->Id() );
	CleanupStack::Pop( state );
	state = new (ELeave) CMceStateUpdating();
	CleanupStack::PushL( state );
	iStates.InsertL( state, state->Id() );
	CleanupStack::Pop( state );
	state = new (ELeave) CMceStateUpdated();
	CleanupStack::PushL( state );
	iStates.InsertL( state, state->Id() );
	CleanupStack::Pop( state );
	state = new (ELeave) CMceStateAnswering();
	CleanupStack::PushL( state );
	iStates.InsertL( state, state->Id() );
	CleanupStack::Pop( state );
	state = new (ELeave) CMceStateError();
	CleanupStack::PushL( state );
	iStates.InsertL( state, state->Id() );
	CleanupStack::Pop( state );
	state = new (ELeave) CMceStateCanceled();
	CleanupStack::PushL( state );
	iStates.InsertL( state, state->Id() );
	CleanupStack::Pop( state );
	state = new (ELeave) CMceStateConfirming();
	CleanupStack::PushL( state );
	iStates.InsertL( state, state->Id() );
	CleanupStack::Pop( state );
	state = new (ELeave) CMceStateClientReserving();
	CleanupStack::PushL( state );
	iStates.InsertL( state, state->Id() );
	CleanupStack::Pop( state );
	state = new (ELeave) CMceStateAcknowledgementRequired();
	CleanupStack::PushL( state );
	iStates.InsertL( state, state->Id() );
	CleanupStack::Pop( state );
	state = new (ELeave) CMceStateServerReserving();
	CleanupStack::PushL( state );
	iStates.InsertL( state, state->Id() );
	CleanupStack::Pop( state );
	state = new (ELeave) CMceStateConfirmationRequired();
	CleanupStack::PushL( state );
	iStates.InsertL( state, state->Id() );
	CleanupStack::Pop( state );
    state = new (ELeave) CMceStateServerOffering();
	CleanupStack::PushL( state );
	iStates.InsertL( state, state->Id() );
	CleanupStack::Pop( state );
	state = new (ELeave) CMceStateServerInitializing();
	CleanupStack::PushL( state );
	iStates.InsertL( state, state->Id() );
	CleanupStack::Pop( state );

    MCESRV_DEBUG("CMceSipStateMachine::ConstructL, Exit");    
	}


// -----------------------------------------------------------------------------
// CMceSipStateMachine::Consumes
// -----------------------------------------------------------------------------
//
TBool CMceSipStateMachine::Consumes( TMceStateTransitionEvent& aEvent )
    {
    TBool consumes = EFalse;
    
    if ( aEvent.EventSource() == EMceSip )
        {
        consumes = ETrue;
        }
    else if ( aEvent.EventSource() == EMceMedia )
        {
        consumes = ETrue;
        }
    else 
        {
        consumes = 
            aEvent.Code() == EMceItcEstablishSession ||
            aEvent.Code() == EMceItcAcceptSession ||
            aEvent.Code() == EMceItcRejectSession ||
            aEvent.Code() == EMceItcTerminateSession ||
            aEvent.Code() == EMceItcCancel ||
            aEvent.Code() == EMceItcRing ||
            aEvent.Code() == EMceItcCallInQueue	||		
            aEvent.Code() == EMceItcUpdate ||
            aEvent.Code() == EMceItcEnable ||
            aEvent.Code() == EMceItcRequestSend ||  
            aEvent.Code() == EMceItcReplySend || 
            aEvent.Code() == EMceItcDisable;
        }
        
    return consumes;
    }
	
    
// -----------------------------------------------------------------------------
// CMceSipStateMachine::~CMceSipStateMachine
// -----------------------------------------------------------------------------
//
CMceSipStateMachine::~CMceSipStateMachine()
	{
    MCESRV_DEBUG("CMceSipStateMachine::~CMceSipStateMachine, Entry");
    
    iStates.ResetAndDestroy();
    iStates.Close();
    
    MCESRV_DEBUG("CMceSipStateMachine::~CMceSipStateMachine, Exit");
	}


// -----------------------------------------------------------------------------
// CMceSipStateMachine::ProcessL
// -----------------------------------------------------------------------------
//
void CMceSipStateMachine::ProcessL( TMceStateTransitionEvent& aEvent )
    {
    MCESRV_DEBUG("CMceSipStateMachine::ProcessL, Entry");
    CMceState& currentState = aEvent.Session().CurrentState();
    TMceStateIndex currentStateId = currentState.Id();
    TUint eventCode = aEvent.Code();
    MCESRV_DEBUG_STATE( "current state", currentStateId );
    MCESRV_DEBUG_EVENT( "trigger event", eventCode );
    
    if ( aEvent.EventSource() == EMceMedia )
        {
        // clear waiting flag
        aEvent.Session().SetWaitingMediaCallback( EFalse );
        }
        
    MMCEStateTransitionHandler& transition = currentState.TransitionHandler();
    TBool accept = currentState.AcceptL( aEvent );

    currentState.StateExitL( aEvent );
    
    if ( accept )
        {
        transition.EntryL( aEvent );
	    if ( aEvent.Session().CurrentState().Id() == currentState.Id() )
	        {
            transition.ExitL( aEvent );
	        }
        }

    aEvent.Session().CurrentState().StateEntry( aEvent );
        
    currentStateId = aEvent.Session().CurrentState().Id();
    MCESRV_DEBUG_STATE( "ProcessL: new state", currentStateId );
    
    MCESRV_DEBUG("CMceSipStateMachine::ProcessL, Exit");        
    }


// End of File

