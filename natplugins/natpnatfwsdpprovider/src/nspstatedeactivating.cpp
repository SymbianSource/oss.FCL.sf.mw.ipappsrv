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
* Description:  Activating state implementation.
*
*/

#include "nspstatedeactivating.h"
#include "nspevents.h"
#include "nspsession.h"
#include "nspactionset.h"
#include "nspdefs.h"

// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// CNSPStateDeActivating::CNSPStateDeActivating
// ---------------------------------------------------------------------------
//
CNSPStateDeActivating::CNSPStateDeActivating()
    {
    }


// ---------------------------------------------------------------------------
// CNSPStateDeActivating::NewL
// ---------------------------------------------------------------------------
//
CNSPStateDeActivating* CNSPStateDeActivating::NewL()
    {
    CNSPStateDeActivating* self = CNSPStateDeActivating::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPStateDeActivating::NewLC
// ---------------------------------------------------------------------------
//
CNSPStateDeActivating* CNSPStateDeActivating::NewLC()
    {
    CNSPStateDeActivating* self = new ( ELeave ) CNSPStateDeActivating;
    CleanupStack::PushL( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPStateDeActivating::~CNSPStateDeActivating
// ---------------------------------------------------------------------------
//
CNSPStateDeActivating::~CNSPStateDeActivating()
    {
    }


// ---------------------------------------------------------------------------
// CNSPStateDeActivating::DoAcceptL
// ---------------------------------------------------------------------------
//
TBool CNSPStateDeActivating::DoAcceptL( TNSPStateMachineEvent& aEvent )
    {
    User::LeaveIfError(
            TNSPStateMachineEvent::ENat == aEvent.Request() ||
            TNSPStateMachineEvent::EReservationStatus == aEvent.Request() ||
            TNSPStateMachineEvent::ECloseSession == aEvent.Request() ?
            KErrNone : KErrArgument );
    
    return ETrue;
    }


// ---------------------------------------------------------------------------
// CNSPStateDeActivating::DoStateEntryL
// ---------------------------------------------------------------------------
//
void CNSPStateDeActivating::DoStateEntryL( TNSPStateMachineEvent& aEvent )
    {
    aEvent.Session().Actions().DeActivateL();
    }


// ---------------------------------------------------------------------------
// CNSPStateDeActivating::DoReverse
// ---------------------------------------------------------------------------
//
void CNSPStateDeActivating::DoReverse( TNSPStateMachineEvent& /*aEvent*/ )
    {
    }


// ---------------------------------------------------------------------------
// CNSPStateDeActivating::DoApplyL
// ---------------------------------------------------------------------------
//
void CNSPStateDeActivating::DoApplyL( TNSPStateMachineEvent& aEvent )
    {
    switch( aEvent.Request() )
        {
        case TNSPStateMachineEvent::ENat:
        	{
        	ControlMediaL( aEvent );
        	break;
        	}
        
        case TNSPStateMachineEvent::EReservationStatus:
        case TNSPStateMachineEvent::ECloseSession:
            {
            aEvent.Status() = KNatReady;
            break;
            }
        
        default:
            {
            User::Leave( KErrArgument );
            }
        }
    }


// ---------------------------------------------------------------------------
// CNSPStateDeActivating::DoExitL
// ---------------------------------------------------------------------------
//
void CNSPStateDeActivating::DoExitL( TNSPStateMachineEvent& /*aEvent*/ )
    {
    User::Leave( KErrNotSupported );
    }

// ---------------------------------------------------------------------------
// CNSPStateDeActivating::ControlMediaL
// ---------------------------------------------------------------------------
//
void CNSPStateDeActivating::ControlMediaL( TNSPStateMachineEvent& aEvent ) const
    {
	TNSPStateMachineMediaEvent& event = MEDIAEVENT( aEvent );
    TNatReturnStatus status = event.Session().Actions().ControlMediaL(
    		event.StreamId(), event.Event(), event.Status(), event.Data() );
    
    if ( NSP_TRIGGER( status ) )
    	{
    	aEvent.Status() = KNatReady;
    	}
    else
    	{
    	event.Status() = status;
    	}
    }

// end of file
