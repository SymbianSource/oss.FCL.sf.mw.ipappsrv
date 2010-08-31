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




#include "mcenatsipstatemachine.h"
#include "mcenatstateinitializing.h"
#include "mcenatstateidle.h"
#include "mcenatstatecreateoffer.h"
#include "mcenatstatecreateanswer.h"
#include "mcenatstatedecodeoffer.h"
#include "mcenatstatedecodeanswer.h"
#include "mcenatstatewait.h"
#include "mcenatstateconnected.h"
#include "mcenatstatecreateofferrequested.h"
#include "mcesrvlogs.h"


// -----------------------------------------------------------------------------
// CMceNatSipStateMachine::NewL
// -----------------------------------------------------------------------------
//
CMceNatSipStateMachine* CMceNatSipStateMachine::NewL()
	{
    CMceNatSipStateMachine* self = CMceNatSipStateMachine::NewLC();
    CleanupStack::Pop(self);
    return self;
	}

// -----------------------------------------------------------------------------
// CMceNatSipStateMachine::NewLC
// -----------------------------------------------------------------------------
//
CMceNatSipStateMachine* CMceNatSipStateMachine::NewLC()
	{
    CMceNatSipStateMachine* self = new (ELeave) CMceNatSipStateMachine();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
	}

// -----------------------------------------------------------------------------
// CMceNatSipStateMachine::CMceNatSipStateMachine
// -----------------------------------------------------------------------------
//
CMceNatSipStateMachine::CMceNatSipStateMachine()
	: iCurrentState( KMceNatStateInitializing )
	{
	}

// -----------------------------------------------------------------------------
// CMceNatSipStateMachine::NewState
// -----------------------------------------------------------------------------
//
void CMceNatSipStateMachine::NewStateL( TMceNatStateTransitionEvent& aEvent,
                                        TMceNatStateIndex aNewState )
	{
    MCESRV_DEBUG( "*** CMceNatSipStateMachine::NewStateL, Entry" );
    MCESRV_DEBUG_DVALUE( "*** new state", aNewState );
    
    iCurrentState = aNewState;
    
    TRAPD( error, iStates[iCurrentState]->TransitionHandler().EntryL( aEvent ) );
    
    if ( KErrNone != error )
        {
        User::Leave( error );
        }
    
    MCESRV_DEBUG( "*** CMceNatSipStateMachine::NewStateL, Exit" );
	}

// -----------------------------------------------------------------------------
// CMceNatSipStateMachine::ConstructL
// -----------------------------------------------------------------------------
//
void CMceNatSipStateMachine::ConstructL()
	{
    MCESRV_DEBUG( "*** CMceNatSipStateMachine::ConstructL, Entry" );
	
	CMceNatState* state = NULL;

	state = new (ELeave) CMceNatStateInitializing();
	CleanupStack::PushL( state );
	iStates.InsertL( state, KMceNatStateInitializing );
	CleanupStack::Pop( state );
    
	state = new (ELeave) CMceNatStateIdle();
	CleanupStack::PushL( state );
	iStates.InsertL( state, KMceNatStateIdle );
	CleanupStack::Pop( state );
	
	state = new (ELeave) CMceNatStateCreateOffer();
	CleanupStack::PushL( state );
	iStates.InsertL( state, KMceNatStateCreateOffer );
	CleanupStack::Pop( state );
	
	state = new (ELeave) CMceNatStateCreateAnswer();
	CleanupStack::PushL( state );
	iStates.InsertL( state, KMceNatStateCreateAnswer );
	CleanupStack::Pop( state );
	
	state = new (ELeave) CMceNatStateDecodeOffer();
	CleanupStack::PushL( state );
	iStates.InsertL( state, KMceNatStateDecodeOffer );
	CleanupStack::Pop( state );
	
	state = new (ELeave) CMceNatStateDecodeAnswer();
	CleanupStack::PushL( state );
	iStates.InsertL( state, KMceNatStateDecodeAnswer );
	CleanupStack::Pop( state );

	state = new (ELeave) CMceNatStateWait();
	CleanupStack::PushL( state );
	iStates.InsertL( state, KMceNatStateWait );
	CleanupStack::Pop( state );

	state = new (ELeave) CMceNatStateConnected();
	CleanupStack::PushL( state );
	iStates.InsertL( state, KMceNatStateConnected );
	CleanupStack::Pop( state );

	state = new (ELeave) CMceNatStateCreateOfferRequested();
	CleanupStack::PushL( state );
	iStates.InsertL( state, KMceNatStateCreateOfferRequested );
	CleanupStack::Pop( state );

    MCESRV_DEBUG( "*** CMceNatSipStateMachine::ConstructL, Exit" );    
	}

// -----------------------------------------------------------------------------
// CMceNatSipStateMachine::~CMceNatSipStateMachine
// -----------------------------------------------------------------------------
//
CMceNatSipStateMachine::~CMceNatSipStateMachine()
	{
    iStates.ResetAndDestroy();
    iStates.Close();
	}

// -----------------------------------------------------------------------------
// CMceNatSipStateMachine::ProcessL
// -----------------------------------------------------------------------------
//
void CMceNatSipStateMachine::ProcessL( TMceNatStateTransitionEvent& aEvent )
    {
    iStates[iCurrentState]->TransitionHandler().ExitL( aEvent );
    }

// -----------------------------------------------------------------------------
// CMceNatSipStateMachine::Accept
// -----------------------------------------------------------------------------
//
TBool CMceNatSipStateMachine::Accept( TMceNatStateTransitionEvent& aEvent )
    {
    return iStates[iCurrentState]->Accept( aEvent );
    }

// -----------------------------------------------------------------------------
// CMceNatSipStateMachine::State
// -----------------------------------------------------------------------------
//
TInt CMceNatSipStateMachine::State() const 
    {
    return iCurrentState;
    }

// -----------------------------------------------------------------------------
// CMceNatSipStateMachine::ErrorOccurred
// -----------------------------------------------------------------------------
//
void CMceNatSipStateMachine::ErrorOccurred()
    {
    iCurrentState = KMceNatStateIdle;
    }
