/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Initializing state implementation
*
*/

#include "nspstateinitializing.h"
#include "nspevents.h"
#include "nspactionset.h"


// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// CNSPStateInitializing::CNSPStateInitializing
// ---------------------------------------------------------------------------
//
CNSPStateInitializing::CNSPStateInitializing()
    {
    }


// ---------------------------------------------------------------------------
// CNSPStateInitializing::NewL
// ---------------------------------------------------------------------------
//
CNSPStateInitializing* CNSPStateInitializing::NewL()
    {
    CNSPStateInitializing* self = CNSPStateInitializing::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPStateInitializing::NewLC
// ---------------------------------------------------------------------------
//
CNSPStateInitializing* CNSPStateInitializing::NewLC()
    {
    CNSPStateInitializing* self = new ( ELeave ) CNSPStateInitializing;
    CleanupStack::PushL( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPStateInitializing::~CNSPStateInitializing
// ---------------------------------------------------------------------------
//
CNSPStateInitializing::~CNSPStateInitializing()
    {
    }


// ---------------------------------------------------------------------------
// CNSPStateInitializing::DoAcceptL
// ---------------------------------------------------------------------------
//
TBool CNSPStateInitializing::DoAcceptL( TNSPStateMachineEvent& aEvent )
    {
    User::LeaveIfError( TNSPStateMachineEvent::ENat == aEvent.Request() ?
                    KErrNone : KErrArgument );
    
    return ETrue;
    }


// ---------------------------------------------------------------------------
// CNSPStateInitializing::DoStateEntryL
// ---------------------------------------------------------------------------
//
void CNSPStateInitializing::DoStateEntryL( TNSPStateMachineEvent& /*aEvent*/ )
    {
    }


// ---------------------------------------------------------------------------
// CNSPStateInitializing::DoReverse
// ---------------------------------------------------------------------------
//
void CNSPStateInitializing::DoReverse( TNSPStateMachineEvent& /*aEvent*/ )
    {
    }


// ---------------------------------------------------------------------------
// CNSPStateInitializing::DoApplyL
// ---------------------------------------------------------------------------
//
void CNSPStateInitializing::DoApplyL( TNSPStateMachineEvent& aEvent )
    {
    switch( aEvent.Request() )
        {
        case TNSPStateMachineEvent::ENat:
            {
            ControlMediaL( aEvent );
            break;
            }
        
        default:
            {
            User::Leave( KErrArgument );
            }
        }
    }


// ---------------------------------------------------------------------------
// CNSPStateInitializing::DoExitL
// ---------------------------------------------------------------------------
//
void CNSPStateInitializing::DoExitL( TNSPStateMachineEvent& aEvent )
    {
    switch( aEvent.Request() )
        {
        case TNSPStateMachineEvent::ENat:
            {
            break;
            }
        
        default:
            {
            User::Leave( KErrArgument );
            }
        }
    }


// ---------------------------------------------------------------------------
// CNSPStateInitializing::ControlMediaL
// ---------------------------------------------------------------------------
//
void CNSPStateInitializing::ControlMediaL( TNSPStateMachineEvent& aEvent ) const
	{
	TNSPStateMachineMediaEvent& event = MEDIAEVENT( aEvent );
	User::LeaveIfError( event.Status() );
	
	if ( MNATFWConnectivityObserver::ESessionCreated == event.Event() )
		{
		event.NextState() = KStateIndexIdle;
		}
	else
		{
		User::Leave( KErrArgument );
		}
	}


// end of file
