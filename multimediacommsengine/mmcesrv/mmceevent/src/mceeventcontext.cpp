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
* Description:    Class for event context. Contains methods for
*                creating the state array and controlling current state.
*
*/




#include "mcecomevent.h"
#include "mceeventcontext.h"
#include "mceeventstatebase.h"
#include "mceeventsubscribemoestablishedstate.h"
#include "mceeventsubscribemtestablishedstate.h"
#include "mceeventsubscribeterminatingstate.h"
#include "mceeventsubscribeterminatedstate.h"
#include "mceeventrefermoestablishedstate.h"
#include "mceeventrefermtestablishedstate.h"
#include "mceeventreferterminatingstate.h"
#include "mceeventreferterminatedstate.h"
#include "mceeventsubscribemoidlestate.h"
#include "mceeventsubscribemoestablishingstate.h"
#include "mceeventrefermoidlestate.h"
#include "mceeventrefermoestablishingstate.h"
#include "mceeventsubscribemtidlestate.h"
#include "mceeventsubscribemtestablishingstate.h"
#include "mceeventrefermtidlestate.h"
#include "mceeventrefermtestablishingstate.h"

// -----------------------------------------------------------------------------
// CMCEMoEventSubscribeContext::NewL
// -----------------------------------------------------------------------------
//
CMceEventContext* CMceEventContext::NewL( CMceComEvent& aEvent )
	{
	CMceEventContext* self = new( ELeave ) CMceEventContext;
    CleanupStack::PushL( self );
    self->ConstructL( aEvent );
    CleanupStack::Pop(self);
    return self;
	}

// -----------------------------------------------------------------------------
// CMceEventContext::CMceEventContext
// -----------------------------------------------------------------------------
//
CMceEventContext::CMceEventContext()
    {
    }

// -----------------------------------------------------------------------------
// CMceEventContext::ConstructL
// -----------------------------------------------------------------------------
//
void CMceEventContext::ConstructL( CMceComEvent& aEvent )
	{
	InitializeStateArrayL(aEvent);
    SetCurrentStateL( aEvent, KMceIdleEventStateIndex );
	}
	
// -----------------------------------------------------------------------------
// CMceEventContext::~CMceEventContext
// -----------------------------------------------------------------------------
//
CMceEventContext::~CMceEventContext()
	{
	if ( iStates )
	    {
	    iStates->ResetAndDestroy();
        iStates->Close();
        delete iStates;
        iStates = NULL;    
	    }
    }
	
// -----------------------------------------------------------------------------
// CMceEventContext::SetCurrentState
// -----------------------------------------------------------------------------
//
void CMceEventContext::SetCurrentStateL( CMceComEvent& aEvent, 
            				          	 TMceEventStateIndex aNewEventState )
	{
	TInt leave = KErrNone;
	if (aNewEventState > KMceSrvEventStateArraySize-1)
		{
		leave = KMceEventStateError;	
		}
	   // Check, if the transition is valid
	if(aEvent.SIPEvent().EventStateType()==EMoSubscribe ||
	aEvent.SIPEvent().EventStateType()==EMoRefer)
		{
		if ( !IsMoStateTransitionAccepted( aNewEventState ) )
        	{
        	leave = KMceEventStateError;
        	}
    	else
        	{
        	iCurrentState = ( *iStates )[ aNewEventState ];
        	}
		}
    else //MT
    	{
    	if ( !IsMtStateTransitionAccepted( aNewEventState ) )
        	{
        	leave = KMceEventStateError;
        	}
    	else
        	{
        	iCurrentState = ( *iStates )[ aNewEventState ];
        	}	
    	}
    User::LeaveIfError( leave );
    }
	
// -----------------------------------------------------------------------------
// CMceEventContext::CurrentState()
// -----------------------------------------------------------------------------
//
TMceEventStateIndex CMceEventContext::CurrentState()
	{
	return iStates->Find( iCurrentState );
	}

// -----------------------------------------------------------------------------
// CMceEventContext::ProcessSubscribeEventL
// -----------------------------------------------------------------------------
//
void CMceEventContext::ProcessEventL( CMceComEvent& aEvent )
	{
	iCurrentState->ProcessStateL( aEvent );
	}

// -----------------------------------------------------------------------------
// CMceEventContext::ProcessRequestReceivedEventL
// -----------------------------------------------------------------------------
//
void CMceEventContext::ProcessRequestReceivedEventL( CMceComEvent& aEvent )
	{
	iCurrentState->ProcessReqRecvStateL( aEvent );
	}

// -----------------------------------------------------------------------------
// CMceEventContext::ProcessIncomingResponseEventL
// -----------------------------------------------------------------------------
//
void CMceEventContext::ProcessIncomingResponseEventL( CMceComEvent& aEvent )
	{
	iCurrentState->ProcessRespRecvStateL( aEvent );
	}

// -----------------------------------------------------------------------------
// CMceEventContext::InitializeStateArrayL
// -----------------------------------------------------------------------------
//
void CMceEventContext::InitializeStateArrayL( CMceComEvent& aEvent )
	{
	    // Create state array
    iStates = new ( ELeave ) RPointerArray< CMceEventStateBase >(
                                       KMceSrvEventStateArraySize );

    // States common for MO and MT are created here.
    // Established event state
    if (aEvent.SIPEvent().EventStateType()==EMoSubscribe)
    	{
    	InitialSubscribeCommonStatesL();
    	//moSubscirbe State
    	    // MO Idle state
    	CMceEventSubscribeMoIdleState* idleEvent = 
    	    new( ELeave ) CMceEventSubscribeMoIdleState();
		CleanupStack::PushL( idleEvent );
		User::LeaveIfError( iStates->Insert(
									idleEvent, KMceIdleEventStateIndex ) );   
		CleanupStack::Pop( idleEvent );

		// MO Establishing state
		  
		CMceEventSubscribeMoEstablishingState* establishingEvent = 
		    new( ELeave ) CMceEventSubscribeMoEstablishingState();
		CleanupStack::PushL( establishingEvent );
		User::LeaveIfError( iStates->Insert(
	        establishingEvent, KMceEstablishingEventStateIndex ) );   
		CleanupStack::Pop( establishingEvent );
		
		CMceEventSubscribeMoEstablishedState* established = 
		    new( ELeave ) CMceEventSubscribeMoEstablishedState();
    	CleanupStack::PushL( established );
		User::LeaveIfError( iStates->Insert( 
	    	established, KMceEstablishedEventStateIndex ) );
		CleanupStack::Pop( established );
		}
	else if(aEvent.SIPEvent().EventStateType()==EMtSubscribe)
		{
		InitialSubscribeCommonStatesL();
		//mtSubscribe State
		    // MT Idle state
	   	CMceEventSubscribeMtIdleState* idleEvent = 
	   	    new( ELeave ) CMceEventSubscribeMtIdleState();
	    CleanupStack::PushL( idleEvent );
	    User::LeaveIfError( iStates->Insert(
	        idleEvent, KMceIdleEventStateIndex ) );   
	    CleanupStack::Pop( idleEvent );
	    
	    // Mt establishing state
	    CMceEventSubscribeMtEstablishingState* establishing = 
	        new ( ELeave ) CMceEventSubscribeMtEstablishingState();
	    CleanupStack::PushL( establishing );
	    User::LeaveIfError( iStates->Insert( 
	        establishing, KMceEstablishingEventStateIndex ) );
	    CleanupStack::Pop( establishing );
	    
	    //Mt established
	    CMceEventSubscribeMtEstablishedState* established = 
	        new( ELeave ) CMceEventSubscribeMtEstablishedState();
		CleanupStack::PushL( established );
		User::LeaveIfError( iStates->Insert( 
		    established, KMceEstablishedEventStateIndex ) );
		CleanupStack::Pop( established );
		}
	else if(aEvent.SIPEvent().EventStateType()==EMoRefer)
		{
		InitialReferCommonStatesL();
		//moReferState	
		// MO Idle state
	    CMceEventReferMoIdleState* idleEvent = 
	        new( ELeave )CMceEventReferMoIdleState();
	    CleanupStack::PushL( idleEvent );
	    User::LeaveIfError( iStates->Insert(
	    								idleEvent, KMceIdleEventStateIndex ) );
	    CleanupStack::Pop( idleEvent );
	    
	    // MO Establishing state
	    CMceEventReferMoEstablishingState* establishingEvent = 
	        new( ELeave )CMceEventReferMoEstablishingState;
	    CleanupStack::PushL( establishingEvent );
	    User::LeaveIfError( iStates->Insert(
	        establishingEvent, KMceEstablishingEventStateIndex ) );   
	    CleanupStack::Pop( establishingEvent );
	    //Mo Established
	    CMceEventReferMoEstablishedState* established = 
    	    new( ELeave ) CMceEventReferMoEstablishedState();
		CleanupStack::PushL( established );
		User::LeaveIfError( iStates->Insert( 
	    	established, KMceEstablishedEventStateIndex ) );
		CleanupStack::Pop( established );
        }
	else
		{
		InitialReferCommonStatesL();
		//mtReferState	
		// MT Idle state
	   	CMceEventReferMtIdleState* idleEvent = 
	   	    new( ELeave ) CMceEventReferMtIdleState();
	    CleanupStack::PushL( idleEvent );
	    User::LeaveIfError( iStates->Insert(
	        idleEvent, KMceIdleEventStateIndex ) );   
	    CleanupStack::Pop( idleEvent );
	    
	    // Server establishing state
	    CMceEventReferMtEstablishingState* establishing = 
	        new( ELeave ) CMceEventReferMtEstablishingState();
	    CleanupStack::PushL( establishing );
	    User::LeaveIfError( iStates->Insert( 
	        establishing, KMceEstablishingEventStateIndex ) );
	    CleanupStack::Pop( establishing );
	   	//Mt established
	   	CMceEventReferMtEstablishedState* established = 
    	    new( ELeave )CMceEventReferMtEstablishedState();
		CleanupStack::PushL( established );
		User::LeaveIfError( iStates->Insert( 
	    	established, KMceEstablishedEventStateIndex ) );
		CleanupStack::Pop( established );	
		}
 
  	}
// -----------------------------------------------------------------------------
// CMceEventContext::InitialSubscribeCommonStates
// -----------------------------------------------------------------------------
//
void CMceEventContext::InitialSubscribeCommonStatesL()
	{
	// Terminating event state
	CMceEventSubscribeTerminatingState* terminating = 
	    new( ELeave )CMceEventSubscribeTerminatingState();
	CleanupStack::PushL( terminating );
    User::LeaveIfError( iStates->Insert( 
	    terminating, KMceTerminatingEventStateIndex ) );
	CleanupStack::Pop( terminating ); 

	// Terminated event state
	CMceEventSubscribeTerminatedState* terminated = 
	    new( ELeave ) CMceEventSubscribeTerminatedState();
    CleanupStack::PushL( terminated );
    User::LeaveIfError( iStates->Insert( 
	    terminated, KMceTerminatedEventStateIndex ) );
	CleanupStack::Pop( terminated );
	}

// -----------------------------------------------------------------------------
// CMceEventContext::InitialReferCommonStates
// -----------------------------------------------------------------------------
//
void CMceEventContext::InitialReferCommonStatesL()
	{
	// Terminating event state
	CMceEventReferTerminatingState* terminating = 
	    new( ELeave ) CMceEventReferTerminatingState();
	CleanupStack::PushL( terminating );
	User::LeaveIfError( iStates->Insert( 
	    terminating, KMceTerminatingEventStateIndex ) );
	CleanupStack::Pop( terminating ); 

	// Terminated event state
	CMceEventReferTerminatedState* terminated = 
	    new( ELeave ) CMceEventReferTerminatedState();
	CleanupStack::PushL( terminated );
	User::LeaveIfError( iStates->Insert( 
	    terminated, KMceTerminatedEventStateIndex ) );
	CleanupStack::Pop( terminated );		
	}
	
// -----------------------------------------------------------------------------
// CMCEMoEventSubscribeContext::IsMoStateTransitionAccepted
// -----------------------------------------------------------------------------
//     
TBool CMceEventContext::IsMoStateTransitionAccepted( 
                                            TMceEventStateIndex aNewState )
	{
    // Call the parent function.
    return IsStateTransitionAccepted( aNewState );
	}
	
// -----------------------------------------------------------------------------
// CMceSrvMoEventContext::IsMtStateTransitionAccepted
// -----------------------------------------------------------------------------
//     
TBool CMceEventContext::IsMtStateTransitionAccepted( 
                                            TMceEventStateIndex aNewState )
	{
	TMceEventStateIndex currentState = CurrentState();
	TBool ret = IsStateTransitionAccepted( aNewState );
	if ( !ret )
	    {
	    switch ( currentState )
            {
            case KMceIdleEventStateIndex:
                {
                // From MT event idle state, transition to client establishing 
                // state is allowed.
                if (KMceEstablishedEventStateIndex == aNewState ||
            	    KMceTerminatedEventStateIndex == aNewState ) 
                    {
                    ret = ETrue;
                    }
                break;
                }
            
            default:
                {
                ret = EFalse;
                break;
                }
    	    }
	    }
	return ret;
	}


// -----------------------------------------------------------------------------
// CMceEventContext::IsStateTransitionAccepted
// -----------------------------------------------------------------------------
//
TMceEventStateIndex CMceEventContext::IsStateTransitionAccepted(
										    TMceEventStateIndex aNewState )
	{
	TMceEventStateIndex current = CurrentState();
    switch( current )
        {
        case KErrNotFound:
            {
            // Only idle state can be the first one.
            if ( KMceIdleEventStateIndex == aNewState )
                {
                return ETrue;
                }
            else
                {
                return EFalse;
                }
            }
            
        case KMceIdleEventStateIndex:
            {
            if ( KMceEstablishingEventStateIndex == aNewState )
                {
                return ETrue;
                }
            else
                {
                return EFalse;
                }
            }
        
        case KMceEstablishingEventStateIndex:
            {
            if ( KMceEstablishedEventStateIndex == aNewState ||
                 KMceTerminatedEventStateIndex == aNewState )
                {
                return ETrue;
                }
            else
                {
                return EFalse;
                }
            }
        
        case KMceEstablishedEventStateIndex:
            { 
            if ( KMceEstablishingEventStateIndex == aNewState ||
            	 KMceEstablishedEventStateIndex == aNewState || 
            	 KMceTerminatingEventStateIndex == aNewState ||
            	 KMceTerminatedEventStateIndex == aNewState )
                {
                return ETrue;
                }
            else
                {
                return EFalse;
                }
            }
               
        case KMceTerminatingEventStateIndex:
            {
            // From terminating state, transition to terminated is accepted.
            if ( KMceTerminatedEventStateIndex == aNewState )
                {
                return ETrue;
                }
            else
                {
                return EFalse;
                }
            } 
        
        default:
            {
            // From terminated state no transition is allowed.
            // All the states are handled either here or in separate methods
            // for MO or MT events. 
            return EFalse;
            }
   
        }

	}

