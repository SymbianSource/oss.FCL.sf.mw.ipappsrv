/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  State machine implementation.
*
*/

#include "nspstatemachine.h"
#include "nspstatebase.h"
#include "nspsessiondata.h"
#include "nspevents.h"
#include "nspstateinitializing.h"
#include "nspstateidle.h"
#include "nspstatelocalcand.h"
#include "nspstatenegotiating.h"
#include "nspstateresolving.h"
#include "nspstateactivating.h"
#include "nspstateconnected.h"
#include "nspstatedeactivating.h"
#include "nspdefs.h"

#define STATE_ID_L( statearray, state ) \
        User::LeaveIfError( statearray.Find( state ) )

#define LOGSTATE( aState ) \
    switch( aState )\
        {\
        case KStateIndexInitializing:{ NSPLOG_STR( "state:Initializing" ); break; }\
        case KStateIndexIdle:{ NSPLOG_STR( "state:Idle" ); break; }\
        case KStateIndexLocalCand:{ NSPLOG_STR( "state:LocalCand" ); break; }\
        case KStateIndexNegotiating:{ NSPLOG_STR( "state:Negotiating" ); break; }\
        case KStateIndexResolving:{ NSPLOG_STR( "state:Resolving" ); break; }\
        case KStateIndexActivating:{ NSPLOG_STR( "state:Activating" ); break; }\
        case KStateIndexConnected:{ NSPLOG_STR( "state:Connected" ); break; }\
        case KStateIndexDeActivating:{ NSPLOG_STR( "state:DeActivating" ); break; }\
        default:{ NSPLOG_STR( "state:Unknown" ); break; }\
        }

#define LOGOLDSTATE( aState ) \
    switch( aState )\
        {\
        case KStateIndexInitializing:{ NSPLOG_STR( "    old state:Initializing" ); break; }\
        case KStateIndexIdle:{ NSPLOG_STR( "    old state:Idle" ); break; }\
        case KStateIndexLocalCand:{ NSPLOG_STR( "    old state:LocalCand" ); break; }\
        case KStateIndexNegotiating:{ NSPLOG_STR( "    old state:Negotiating" ); break; }\
        case KStateIndexResolving:{ NSPLOG_STR( "    old state:Resolving" ); break; }\
        case KStateIndexActivating:{ NSPLOG_STR( "    old state:Activating" ); break; }\
        case KStateIndexConnected:{ NSPLOG_STR( "    old state:Connected" ); break; }\
        case KStateIndexDeActivating:{ NSPLOG_STR( "    old state:DeActivating" ); break; }\
        default:{ NSPLOG_STR( "    old state:Unknown" ); break; }\
        }

#define LOGSNEWTATE( aState ) \
    switch( aState )\
        {\
        case KStateIndexInitializing:{ NSPLOG_STR( "    new state:Initializing" ); break; }\
        case KStateIndexIdle:{ NSPLOG_STR( "    new state:Idle" ); break; }\
        case KStateIndexLocalCand:{ NSPLOG_STR( "    new state:LocalCand" ); break; }\
        case KStateIndexNegotiating:{ NSPLOG_STR( "    new state:Negotiating" ); break; }\
        case KStateIndexResolving:{ NSPLOG_STR( "    new state:Resolving" ); break; }\
        case KStateIndexActivating:{ NSPLOG_STR( "    new state:Activating" ); break; }\
        case KStateIndexConnected:{ NSPLOG_STR( "    new state:Connected" ); break; }\
        case KStateIndexDeActivating:{ NSPLOG_STR( "    new state:DeActivating" ); break; }\
        default:{ NSPLOG_STR( "    new state:Unknown" ); break; }\
        }


#define LOGEVENT( aEvent ) \
    switch( (TNSPStateMachineEvent::TRequest) aEvent.Request() )\
        {\
        case TNSPStateMachineEvent::ENat:{ NSPLOG_STR( "event:ENat" ); break; }\
        case TNSPStateMachineEvent::ECreateOffer:{ NSPLOG_STR( "event:ECreateOffer" ); break; }\
        case TNSPStateMachineEvent::EResolve:{ NSPLOG_STR( "event:EResolve" ); break; }\
        case TNSPStateMachineEvent::EDecodeAnswer:{ NSPLOG_STR( "event:EDecodeAnswer" ); break; }\
        case TNSPStateMachineEvent::EUpdate:{ NSPLOG_STR( "event:EUpdate" ); break; }\
        case TNSPStateMachineEvent::EReservationStatus:{ NSPLOG_STR( "event:EReservationStatus" ); break; }\
        case TNSPStateMachineEvent::ECloseSession:{ NSPLOG_STR( "event:ECloseSession" ); break; }\
        default:{ NSPLOG_STR( "event:Unknown" ); break; }\
        }

// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// CNSPStateMachine::CNSPStateMachine
// ---------------------------------------------------------------------------
//
CNSPStateMachine::CNSPStateMachine()
    {
    }


// ---------------------------------------------------------------------------
// CNSPStateMachine::ConstructL
// ---------------------------------------------------------------------------
//
void CNSPStateMachine::ConstructL()
    {
    InitializeStateArrayL();
    iCurrentState = iStateArray[KStateIndexInitializing];
    }


// ---------------------------------------------------------------------------
// CNSPStateMachine::InitializeStateArrayL
// ---------------------------------------------------------------------------
//
void CNSPStateMachine::InitializeStateArrayL()
    {
    CNSPStateBase* state = NULL;
    
    state = CNSPStateInitializing::NewLC();
    User::LeaveIfError( iStateArray.Insert( state, KStateIndexInitializing ) );
    CleanupStack::Pop( state );
    
    state = CNSPStateIdle::NewLC();
    User::LeaveIfError( iStateArray.Insert( state, KStateIndexIdle ) );
    CleanupStack::Pop( state );
    
    state = CNSPStateLocalCand::NewLC();
    User::LeaveIfError( iStateArray.Insert( state, KStateIndexLocalCand ) );
    CleanupStack::Pop( state );
    
    state = CNSPStateNegotiating::NewLC();
    User::LeaveIfError( iStateArray.Insert( state, KStateIndexNegotiating ) );
    CleanupStack::Pop( state );
    
    state = CNSPStateResolving::NewLC();
    User::LeaveIfError( iStateArray.Insert( state, KStateIndexResolving ) );
    CleanupStack::Pop( state );
    
    state = CNSPStateActivating::NewLC();
    User::LeaveIfError( iStateArray.Insert( state, KStateIndexActivating ) );
    CleanupStack::Pop( state );
    
    state = CNSPStateConnected::NewLC();
    User::LeaveIfError( iStateArray.Insert( state, KStateIndexConnected ) );
    CleanupStack::Pop( state );

    state = CNSPStateDeActivating::NewLC();
    User::LeaveIfError( iStateArray.Insert( state, KStateIndexDeActivating ) );
    CleanupStack::Pop( state );
    }


// ---------------------------------------------------------------------------
// CNSPStateMachine::NewL
// ---------------------------------------------------------------------------
//
CNSPStateMachine* CNSPStateMachine::NewL()
    {
    CNSPStateMachine* self = CNSPStateMachine::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPStateMachine::NewLC
// ---------------------------------------------------------------------------
//
CNSPStateMachine* CNSPStateMachine::NewLC()
    {
    CNSPStateMachine* self = new ( ELeave ) CNSPStateMachine();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPStateMachine::~CNSPStateMachine
// ---------------------------------------------------------------------------
//
CNSPStateMachine::~CNSPStateMachine()
    {
    iStateArray.ResetAndDestroy();
    iStateArray.Close();
    }


// ---------------------------------------------------------------------------
// CNSPStateMachine::ProcessL
// ---------------------------------------------------------------------------
//
void CNSPStateMachine::ProcessL( TNSPStateMachineEvent& aEvent )
    {
    __ASSERT_ALWAYS( iCurrentState, User::Leave( KErrTotalLossOfPrecision ) );
    
    NSPLOG_STR( "CNSPStateMachine::ProcessL, Entry" )
    
    TBool accept = iCurrentState->AcceptL( aEvent );
    
    if ( accept )
        {
        TNSPStateIndex currState = STATE_ID_L( iStateArray, iCurrentState );
        aEvent.NextState() = currState;
        
        LOGSTATE( currState );
        LOGEVENT( aEvent );
        
        iCurrentState->ApplyL( aEvent );
        
        if ( currState != aEvent.NextState() )
            {
            NSPLOG_STR( "CNSPStateMachine::ProcessL, Changing session state." )
            LOGOLDSTATE( currState )
            
            IsAcceptableStateTransitionL( aEvent.NextState() );
            
            LOGSNEWTATE( aEvent.NextState() )
            
            iCurrentState->ExitL( aEvent );
            CNSPStateBase* newState = iStateArray[aEvent.NextState()];
            TRAPD( error, newState->StateEntryL( aEvent ) );
            
            if ( KErrNone != error )
                {
                NSPLOG_STR( "CNSPStateMachine::ProcessL, StateEntryL leave!" )
                NSPLOG_INT( "CNSPStateMachine::ProcessL, With leave code", error )
                NSPLOG_STR( "CNSPStateMachine::ProcessL, Reversing.." )
                iCurrentState->Reverse( aEvent );
                User::Leave( error );
                }
            else
                {
                iCurrentState = newState;
                }
            }
        }
    else
    	{
    	NSPLOG_STR( "CNSPStateMachine::ProcessL, Ignoring event." )
    	}
    
    NSPLOG_STR( "CNSPStateMachine::ProcessL, Exit" )
    }


// ---------------------------------------------------------------------------
// CNSPStateMachine::IsAcceptableStateTransitionL
// ---------------------------------------------------------------------------
//
void CNSPStateMachine::IsAcceptableStateTransitionL( 
        TNSPStateIndex aStateIndex ) const
    {
    User::LeaveIfError( IsAcceptableStateTransition( aStateIndex ) ?
            KErrNone : KErrArgument );
    }


// ---------------------------------------------------------------------------
// CNSPStateMachine::IsAcceptableStateTransition
// ---------------------------------------------------------------------------
//
TBool CNSPStateMachine::IsAcceptableStateTransition(
        TNSPStateIndex aStateIndex ) const
    {
    const TNSPStateIndex currstate = iStateArray.Find( iCurrentState );
    
    switch( currstate )
        {
        case KErrNotFound:
            {
            NSPLOG_STR(
            "CNSPStateMachine::IsAcceptableStateTransition(), Not found" )
            return EFalse;
            }
        
        case KStateIndexInitializing:
        	{
        	if ( KStateIndexIdle == aStateIndex )
        		{
        		return ETrue;
        		}
        	else
        		{
        		return EFalse;
        		}
        	}
        
        case KStateIndexIdle:
            {
            if ( KStateIndexLocalCand == aStateIndex )
                {
                return ETrue;
                }
            else
                {
                return EFalse;
                }
            }
        
        case KStateIndexLocalCand:
            {
            if ( KStateIndexNegotiating == aStateIndex ||
                 KStateIndexResolving == aStateIndex ||
                 KStateIndexActivating == aStateIndex )
                {
                return ETrue;
                }
            else
                {
                return EFalse;
                }
            }
        
        case KStateIndexNegotiating:
            {
            if ( KStateIndexResolving == aStateIndex ||
                 KStateIndexActivating == aStateIndex )
                {
                return ETrue;
                }
            else
                {
                return EFalse;
                }
            }
        
        case KStateIndexResolving:
            {
            if ( KStateIndexActivating == aStateIndex )
                {
                return ETrue;
                }
            else
                {
                return EFalse;
                }
            }
        
        case KStateIndexActivating:
            {
            if ( KStateIndexConnected == aStateIndex ||
                 KStateIndexDeActivating == aStateIndex )
                {
                return ETrue;
                }
            else
                {
                return EFalse;
                }
            }
        
        case KStateIndexConnected:
            {
            if ( KStateIndexLocalCand == aStateIndex ||
                 KStateIndexActivating == aStateIndex ||
                 KStateIndexResolving == aStateIndex ||
                 KStateIndexDeActivating == aStateIndex )
                {
                return ETrue;
                }
            else
                {
                return EFalse;
                }
            }
        
        case KStateIndexDeActivating:
            {
            return EFalse; // end state, no transitions allowed
            }
        
        default:
            {
            // never should come here,
            // since all state transitions are handled above
            NSPLOG_STR(
            "CNSPStateMachine::IsAcceptableStateTransition(), DEFAULT" )
            return EFalse;
            }
        }
    }


// end of file
