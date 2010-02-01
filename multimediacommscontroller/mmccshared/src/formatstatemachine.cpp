/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    StateMachine for payloadformat plugins.
*
*/





// INCLUDE FILES
#include "formatstatemachine.h"


// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CFormatEncodeStateMachine::CFormatEncodeStateMachine
// C++ default constructor can NOT contain any code, that
// might leave
// -----------------------------------------------------------------------------
//
CFormatEncodeStateMachine::CFormatEncodeStateMachine( 
    MPayloadFormatWrite* aClient ) : CActive( EPriorityStandard ),
    iClient( aClient ), iState( EEncodeIdle )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CFormatEncodeStateMachine::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CFormatEncodeStateMachine::ConstructL()
    {
    User::LeaveIfNull( iClient );
    }

// -----------------------------------------------------------------------------
// CFormatEncodeStateMachine::NewL
// Static constructor.
// -----------------------------------------------------------------------------
//
CFormatEncodeStateMachine* CFormatEncodeStateMachine::NewL( 
    MPayloadFormatWrite* aClient )
    {
    CFormatEncodeStateMachine* self = 
            new ( ELeave ) CFormatEncodeStateMachine( aClient );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// ------------------------------------------------------------------------------
// CFormatEncodeStateMachine::~CFormatEncodeStateMachine
// Destructor
// -----------------------------------------------------------------------------
//
CFormatEncodeStateMachine::~CFormatEncodeStateMachine()
    {
    Cancel();
    }

// -----------------------------------------------------------------------------
// CFormatEncodeStateMachine::ChangeState
// Changes state
// -----------------------------------------------------------------------------
//
void CFormatEncodeStateMachine::ChangeState( TFormatEncodeState aNewState )
    {
    // Observe the transition rules
    TBool changeState = EFalse;
    switch ( aNewState )
        {
        case EEncodeIdle:
            // Can change to EEncodeIdle state from any other state.
            changeState = ETrue;
            break;
        
        case EEmptySourceBuffer:
            if ( ESourceBufferEmptied == iState || EEncodeIdle == iState )
                {
                changeState = ETrue;
                }
            break;
        
        case ESourceBufferEmptied:
            if ( EEmptySourceBuffer == iState )
                {
                changeState = ETrue;
                }
            break;
        
        default:
            break;
        }
    
    // Change the state if everything is OK.
    if ( changeState )
        {
        iState = aNewState;
        
        // Idle states, no callbacks to client, thus no need for activation.
        if ( EEncodeIdle != iState )
            {
            TRequestStatus* stat = &iStatus;
            Cancel();
            User::RequestComplete( stat, KErrNone );
            SetActive();
            }
        }
    }

// -----------------------------------------------------------------------------
// CFormatEncodeStateMachine::CurrentState
// Returns the current state
// -----------------------------------------------------------------------------
//
TFormatEncodeState CFormatEncodeStateMachine::CurrentState() const
    {
    return iState;
    }

// -----------------------------------------------------------------------------
// CFormatEncodeStateMachine::RunL
// From CActive Handles an active object’s request completion event.
// -----------------------------------------------------------------------------
//
void CFormatEncodeStateMachine::RunL()
    {
    if ( KErrNone == iStatus.Int() )
        {
        switch ( iState )
            {
            case EEncodeIdle:
                // Do nothing but wait.
                break;
            
            case EEmptySourceBuffer:
                iClient->EmptySourceBufferL();
                break;
            
            case ESourceBufferEmptied:
                iClient->SourceBufferEmptiedL();
                break;
            
            default:
                User::Leave( KErrArgument );
                break;
            }
        }
    else
        {
        User::Leave( iStatus.Int() );
        }
    }

// -----------------------------------------------------------------------------
// CFormatEncodeStateMachine::DoCancel
// From CActive Implements cancellation of an outstanding request.
// -----------------------------------------------------------------------------
//
void CFormatEncodeStateMachine::DoCancel()
    {
    
    }

// -----------------------------------------------------------------------------
// CFormatEncodeStateMachine::RunError
// Handle the leave occured in RunL() function
// -----------------------------------------------------------------------------
//
TInt CFormatEncodeStateMachine::RunError( TInt /*aError*/ )
    {
    return KErrNone;
    };
    
// -----------------------------------------------------------------------------
// CFormatDecodeStateMachine::CFormatDecodeStateMachine
// C++ default constructor can NOT contain any code, that
// might leave
// -----------------------------------------------------------------------------
//
CFormatDecodeStateMachine::CFormatDecodeStateMachine( 
    MPayloadFormatRead* aClient ) : CActive( EPriorityStandard ),
    iClient( aClient ), iState( EDecodeIdle )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CFormatDecodeStateMachine::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CFormatDecodeStateMachine::ConstructL()
    {
    User::LeaveIfNull( iClient );
    }

// -----------------------------------------------------------------------------
// CFormatDecodeStateMachine::NewL
// Static constructor.
// -----------------------------------------------------------------------------
//
CFormatDecodeStateMachine* CFormatDecodeStateMachine::NewL( 
    MPayloadFormatRead* aClient )
    {
    CFormatDecodeStateMachine* self = 
            new ( ELeave ) CFormatDecodeStateMachine( aClient );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CFormatDecodeStateMachine::~CFormatDecodeStateMachine
// Destructor
// -----------------------------------------------------------------------------
//
CFormatDecodeStateMachine::~CFormatDecodeStateMachine()
    {
    Cancel();
    }

// -----------------------------------------------------------------------------
// CFormatDecodeStateMachine::ChangeState
// Change State
// -----------------------------------------------------------------------------
//
void CFormatDecodeStateMachine::ChangeState( TFormatDecodeState aNewState )
    {
    // Observe the state transition rules.
    TBool changeState = EFalse;
    switch ( aNewState )
        {
        // These state transitions allowed from all states.
        case EDecodeIdle:
        case ESourceDataReady:
            changeState = ETrue;
            break;
        
        case EEmptyDataToSink:
            if ( ESourceDataReady == iState || EEmptyDataToSink == iState )
                {
                changeState = ETrue;
                }
            break;
        
        default:
            break;
        }
    
    if ( changeState )
        {
        iState = aNewState;
        
        // Idle state, no callbacks to client, thus no need for activation.
        if ( EDecodeIdle != iState )
            {
            TRequestStatus* stat = &iStatus;
            Cancel();
            User::RequestComplete( stat, KErrNone );
            SetActive();
            }
        }
    }

// -----------------------------------------------------------------------------
// CFormatDecodeStateMachine::CurrentState
// Get current state
// -----------------------------------------------------------------------------
//
TFormatDecodeState CFormatDecodeStateMachine::CurrentState() const
    {
    return iState;
    }

// -----------------------------------------------------------------------------
// CFormatDecodeStateMachine::RunL
// From CActive Handles an active object’s request completion event.
// -----------------------------------------------------------------------------
//
void CFormatDecodeStateMachine::RunL()
    {
    if ( KErrNone == iStatus.Int() )
        {
        switch ( iState )
            {
            case EDecodeIdle: 
                // TBD: Do idle processing.
                break;
            
            case ESourceDataReady:
                iClient->FillSinkBufferL();
                break;
            
            case EEmptyDataToSink:
                iClient->SendDataToSinkL();
                break;
            
            default:
                User::Leave( KErrArgument );
                break;
            }
        }
    else
        {
        User::Leave( iStatus.Int() );
        }
    };

// -----------------------------------------------------------------------------
// CFormatDecodeStateMachine::DoCancel
// From CActive Implements cancellation of an outstanding request.
// -----------------------------------------------------------------------------
//
void CFormatDecodeStateMachine::DoCancel()
    {
    
    };

// -----------------------------------------------------------------------------
// CFormatDecodeStateMachine::RunError
// Handle the leave occured in RunL() function
// -----------------------------------------------------------------------------
//
TInt CFormatDecodeStateMachine::RunError( TInt /*aError*/ )
    {
    return KErrNone;
    };

//  End of File  
