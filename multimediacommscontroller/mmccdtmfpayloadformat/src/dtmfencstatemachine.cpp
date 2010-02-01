/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    State machine for DTMF payload format encoding and sending.
*
*/




// INCLUDES
#include "dtmfencstatemachine.h"
#include "dtmfpayloadformatdefs.h"

// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// CDtmfEncStateMachine::CMccSymDtmfStream
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CDtmfEncStateMachine::CDtmfEncStateMachine( MDtmfEncClient& aClient )
    : CActive( EPriorityStandard ), iClient( aClient ),
    iState( EStateSendingIdle )
    {
    CActiveScheduler::Add( this );
    }

// ---------------------------------------------------------------------------
// CDtmfEncStateMachine::NewL
// Two-phased constructor
// ---------------------------------------------------------------------------
//
CDtmfEncStateMachine* CDtmfEncStateMachine::NewL( MDtmfEncClient& aClient )
    {
    CDtmfEncStateMachine* self = 
        new( ELeave ) CDtmfEncStateMachine( aClient );
    return self;
    }

// ---------------------------------------------------------------------------
// CDtmfEncStateMachine::~CDtmfEncStateMachine
// Destructor.
// ---------------------------------------------------------------------------
//
CDtmfEncStateMachine::~CDtmfEncStateMachine()
    {
    Cancel();
    }

// ---------------------------------------------------------------------------
// CDtmfEncStateMachine::State
// Returns current state.
// ---------------------------------------------------------------------------
//
TDTMFSendingState CDtmfEncStateMachine::State() const
    {
    return iState;
    }

// ---------------------------------------------------------------------------
// CDtmfEncStateMachine::ChangeStateTo
// Changes state to the new one if transition is valid.
// ---------------------------------------------------------------------------
//
void CDtmfEncStateMachine::ChangeStateTo( TDTMFSendingState aNewState )
    {
    DP_DTMF_WRITE3( _L("CDtmfEncStateMachine::ChangeStateTo aNewState: %d iState: %d"), 
        aNewState, iState );
    
    TBool validTransition( EFalse );
    switch ( aNewState )
        {
        case EStateSendingIdle:
            DP_DTMF_WRITE( _L("ChangeStateTo: EStateSendingIdle") );
            
            if ( EStateEncodeNextDigit == iState || 
                 EStateSendStopped == iState )
                {
                validTransition = ETrue;
                }
            break;
            
        case EStateEncodeNextDigit:
            DP_DTMF_WRITE( _L("ChangeStateTo: EStateEncodeNextDigit") );
            
            if ( EStateToneOn != iState )
                {
                validTransition = ETrue;
                }
            break;
            
        case EStateToneOn:
            DP_DTMF_WRITE( _L("ChangeStateTo: EStateToneOn") );
            
            if ( EStateEncodeNextDigit == iState )
                {
                validTransition = ETrue;
                }
            break;

        case EStateToneOff:
            DP_DTMF_WRITE( _L("ChangeStateTo: EStateToneOff") );
            
            if ( EStateToneOn == iState )
                {
                validTransition = ETrue;
                }
            break;

        case EStateSendPaused:
            DP_DTMF_WRITE( _L("ChangeStateTo: EStateSendPaused") );
            
            if ( EStateEncodeNextDigit == iState )
                {
                validTransition = ETrue;
                }
            break;
            
        case EStateSendStopped:
            DP_DTMF_WRITE( _L("ChangeStateTo: EStateSendStopped") );
            
            if ( EStateEncodeNextDigit == iState )
                {
                validTransition = ETrue;
                }
            break;
        
        default:
            DP_DTMF_WRITE( _L("ChangeStateTo: default") );
            break;            
        }
    
    // Change the state if everything is OK.
    if ( validTransition && !IsActive() )
        {
        iState = aNewState;
        
        // Do the active state transfer only if needed. This saves again a
        // few cycles by not doing idle rounds trough activescheduler.
        if ( EStateEncodeNextDigit == iState ||
             EStateToneOn == iState || 
             EStateToneOff == iState )
            {
            TRequestStatus* stat = &iStatus;
            User::RequestComplete( stat, KErrNone );
            SetActive();
            }
        }
    
    DP_DTMF_WRITE3( _L("ChangeStateTo - VALIDITY: %d, ACTIVE: %d"),
        validTransition, IsActive() );
    }

// ---------------------------------------------------------------------------
// CDtmfEncStateMachine::RunL
// Handles an active object’s request completion event.
// ---------------------------------------------------------------------------
//
void CDtmfEncStateMachine::RunL()
    {
    DP_DTMF_WRITE2( _L("CDtmfEncStateMachine::RunL - status: %d"), 
        iStatus.Int() );

    if ( KErrNone == iStatus.Int() )
        {
        switch ( iState )
            {
            case EStateEncodeNextDigit:
                iClient.EncodeNextDigitL();
                break;
            case EStateToneOn:
                iClient.DoToneOnActionsL( ETrue );
                break;
            case EStateToneOff:
                iClient.DoToneOffActionsL( ETrue );
                break;
            case EStateSendingIdle:
            case EStateSendPaused:
            case EStateSendStopped:
                break;
            default:
                break;
            }
        }
    else
        {
        User::Leave( iStatus.Int() );
        }
    }

// ---------------------------------------------------------------------------
// CDtmfEncStateMachine::DoCancel
// Implements cancellation of an outstanding request.
// ---------------------------------------------------------------------------
//
void CDtmfEncStateMachine::DoCancel()
    {
    DP_DTMF_WRITE( _L("CDtmfEncStateMachine::DoCancel") );
    }

// ---------------------------------------------------------------------------
// CDtmfEncStateMachine::RunError
// Error handling of the active object.
// ---------------------------------------------------------------------------
//
TInt CDtmfEncStateMachine::RunError( TInt aError )
    {
    DP_DTMF_WRITE2( _L("CDtmfEncStateMachine::RunError aError: %d"), aError );
    
    if ( KErrNoMemory == aError )
        {
        return KErrNoMemory;
        }
    else
        {
        return KErrNone;
        }
    }

// ---------------------------------------------------------------------------
// CDtmfEncStateMachine::ResetStateMachine
// ---------------------------------------------------------------------------
//
void CDtmfEncStateMachine::ResetStateMachine()
    {
    Cancel();
    iState = EStateSendingIdle;
    }

//  End of File
