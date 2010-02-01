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
* Description:  Stream component async state class implementation.
*
*/

#include "nspnatfwasyncstate.h"
#include "nspdefs.h"

// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// CNSPNATFWAsyncState::CNSPNATFWAsyncState
// ---------------------------------------------------------------------------
//
CNSPNATFWAsyncState::CNSPNATFWAsyncState()
     : iCurrentState( ENATFWIdle )
    {
    }


// ---------------------------------------------------------------------------
// CNSPNATFWAsyncState::Construct(copy)
// ---------------------------------------------------------------------------
//
void CNSPNATFWAsyncState::Construct( const CNSPNATFWAsyncState& aState )
    {
    iCurrentState = aState.iCurrentState;
    }


// ---------------------------------------------------------------------------
// CNSPNATFWAsyncState::NewL
// ---------------------------------------------------------------------------
//
CNSPNATFWAsyncState* CNSPNATFWAsyncState::NewL()
    {
    CNSPNATFWAsyncState* self = CNSPNATFWAsyncState::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPNATFWAsyncState::NewLC
// ---------------------------------------------------------------------------
//
CNSPNATFWAsyncState* CNSPNATFWAsyncState::NewLC()
    {
    CNSPNATFWAsyncState* self = new ( ELeave ) CNSPNATFWAsyncState();
    CleanupStack::PushL( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPNATFWAsyncState::NewL(copy)
// ---------------------------------------------------------------------------
//
CNSPNATFWAsyncState* CNSPNATFWAsyncState::NewL(
                                           const CNSPNATFWAsyncState& aState )
    {
    CNSPNATFWAsyncState* self = CNSPNATFWAsyncState::NewLC( aState );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPNATFWAsyncState::NewLC(copy)
// ---------------------------------------------------------------------------
//
CNSPNATFWAsyncState* CNSPNATFWAsyncState::NewLC(
                                           const CNSPNATFWAsyncState& aState )
    {
    CNSPNATFWAsyncState* self = new ( ELeave ) CNSPNATFWAsyncState();
    CleanupStack::PushL( self );
    self->Construct( aState );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPNATFWAsyncState::~CNSPNATFWAsyncState
// ---------------------------------------------------------------------------
//
CNSPNATFWAsyncState::~CNSPNATFWAsyncState()
    {
    }


// ---------------------------------------------------------------------------
// CNSPNATFWAsyncState::CheckState
// ---------------------------------------------------------------------------
//
TInt CNSPNATFWAsyncState::CheckState( TNATFWEvent aEvent ) const
    {
    TInt error( KErrNone );
    
    switch( aEvent )
        {
        case ENATFWEventFetchCand:
        case ENATFWEventPerformChecks:
            {
            if ( ENATFWIdle == iCurrentState )
                {
                error = KErrNone;
                }
            else
                {
                error = KErrArgument;
                }
            
            break;
            }
        
        case ENATFWEventRecvAct:
            {
            if ( ENATFWIdle == iCurrentState ||
                 ENATFWSend == iCurrentState ||
                 ENATFWSendRecv == iCurrentState )
                {
                error = KErrNone;
                }
            else
                {
                error = KErrArgument;
                }
            
            break;
            }
        
        case ENATFWEventSendAct:
            {
            if ( ENATFWIdle == iCurrentState ||
                 ENATFWRecv == iCurrentState ||
                 ENATFWSendRecv == iCurrentState )
                {
                error = KErrNone;
                }
            else
                {
                error = KErrArgument;
                }
            
            break;
            }
        
        case ENATFWEventRecvDeAct:
            {
            if ( ENATFWIdle == iCurrentState ||
                 ENATFWRecv == iCurrentState ||
                 ENATFWSendRecv == iCurrentState )
                {
                error = KErrNone;
                }
            else
                {
                error = KErrArgument;
                }
            
            break;
            }

        case ENATFWEventSendDeAct:
            {            
            if ( ENATFWIdle == iCurrentState ||
                 ENATFWSend == iCurrentState ||
                 ENATFWSendRecv == iCurrentState )
                {
                error = KErrNone;
                }
            else
                {
                error = KErrArgument;
                }
            
            break;
            }
        
        default:
            {
            error = KErrNotSupported;
            break;
            }
        }
    
    return error;
    }


// ---------------------------------------------------------------------------
// CNSPNATFWAsyncState::CheckStateL
// ---------------------------------------------------------------------------
//
void CNSPNATFWAsyncState::CheckStateL( TNATFWEvent aEvent ) const
    {
    User::LeaveIfError( CheckState( aEvent ) );
    }


// ---------------------------------------------------------------------------
// CNSPNATFWAsyncState::ChangeState
// ---------------------------------------------------------------------------
//
TInt CNSPNATFWAsyncState::ChangeState( TNATFWEvent aEvent )
    {
    NSPLOG_INT( "CNSPNATFWAsyncState::ChangeState(),curr state:", iCurrentState )
    
    TInt error( KErrNone );
    
    switch( aEvent )
        {
        case ENATFWEventFetchCand:
            {
            if ( ENATFWIdle == iCurrentState )
                {
                iCurrentState = ENATFWFetchCand;
                error = KErrNone;
                }
            else
                {
                error = KErrArgument;
                }
            
            break;
            }
        
        case ENATFWEventPerformChecks:
            {
            if ( ENATFWIdle == iCurrentState )
                {
                iCurrentState = ENATFWCandChecks;
                error = KErrNone;
                }
            else
                {
                error = KErrArgument;
                }
            
            break;
            }
        
        case ENATFWEventRecvAct:
            {
            if ( ENATFWIdle == iCurrentState )
                {
                iCurrentState = ENATFWRecv;
                error = KErrNone;
                }
            else if ( ENATFWSend == iCurrentState )
                {
                iCurrentState = ENATFWSendRecv;
                error = KErrNone;
                }
            else
                {
                error = KErrArgument;
                }
            
            break;
            }
        
        case ENATFWEventSendAct:
            {
            if ( ENATFWIdle == iCurrentState )
                {
                iCurrentState = ENATFWSend;
                error = KErrNone;
                }
            else if ( ENATFWRecv == iCurrentState )
                {
                iCurrentState = ENATFWSendRecv;
                error = KErrNone;
                }
            else
                {
                error = KErrArgument;
                }
            
            break;
            }
        
        case ENATFWEventRecvDeAct:
            {
            if ( ENATFWIdle == iCurrentState )
                {
                error = KErrNone;
                }
            else if ( ENATFWRecv == iCurrentState )
                {
                iCurrentState = ENATFWIdle;
                error = KErrNone;
                }
            else if ( ENATFWSendRecv == iCurrentState )
                {
                iCurrentState = ENATFWSend;
                error = KErrNone;
                }
            else
                {
                error = KErrArgument;
                }
            
            break;
            }
        
        case ENATFWEventSendDeAct:
            {
            if ( ENATFWIdle == iCurrentState )
                {
                error = KErrNone;
                }
            else if ( ENATFWSend == iCurrentState )
                {
                iCurrentState = ENATFWIdle;
                error = KErrNone;
                }
            else if ( ENATFWSendRecv == iCurrentState )
                {
                iCurrentState = ENATFWRecv;
                error = KErrNone;
                }
            else
                {
                error = KErrArgument;
                }
            
            break;
            }
        
        default:
            {
            error = KErrNotSupported;
            break;
            }
        }
    
    NSPLOG_INT(
    "CNSPNATFWAsyncState::ChangeState(),new state :", iCurrentState )
    
    return error;
    }


// ---------------------------------------------------------------------------
// CNSPNATFWAsyncState::ChangeStateL
// ---------------------------------------------------------------------------
//
void CNSPNATFWAsyncState::ChangeStateL( TNATFWEvent aEvent )
    {
    User::LeaveIfError( ChangeState( aEvent ) );
    }


// ---------------------------------------------------------------------------
// CNSPNATFWAsyncState::Continue
// ---------------------------------------------------------------------------
//
TBool CNSPNATFWAsyncState::Continue()
    {
    NSPLOG_INT( "CNSPNATFWAsyncState::Continue(),curr state:", iCurrentState )
    
    TBool readyToContinue = EFalse;
    
    switch( iCurrentState )
        {
        case ENATFWIdle:
            {
            readyToContinue = ETrue;
            break;
            }
        case ENATFWFetchCand:
        case ENATFWCandChecks:
            {
            readyToContinue = ETrue;
            iCurrentState = ENATFWIdle;
            break;
            }
        case ENATFWSendRecv:
            {
            readyToContinue = ETrue;
            break;
            }
        default:
            {
            // NOP
            }
        }
    
    NSPLOG_INT( "CNSPNATFWAsyncState::Continue(),new state :", iCurrentState )
    return readyToContinue;
    }


// end of file
