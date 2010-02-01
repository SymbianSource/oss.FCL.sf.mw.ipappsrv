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
* Description:  Resolving state implementation.
*
*/

#include <sdpdocument.h>
#include "nspstateresolving.h"
#include "nspevents.h"
#include "nspsession.h"
#include "nspactionset.h"
#include "nspsessiondata.h"
#include "nspdefs.h"

// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// CNSPStateResolving::CNSPStateResolving
// ---------------------------------------------------------------------------
//
CNSPStateResolving::CNSPStateResolving()
    {
    }


// ---------------------------------------------------------------------------
// CNSPStateResolving::NewL
// ---------------------------------------------------------------------------
//
CNSPStateResolving* CNSPStateResolving::NewL()
    {
    CNSPStateResolving* self = CNSPStateResolving::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPStateResolving::NewLC
// ---------------------------------------------------------------------------
//
CNSPStateResolving* CNSPStateResolving::NewLC()
    {
    CNSPStateResolving* self = new ( ELeave ) CNSPStateResolving;
    CleanupStack::PushL( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPStateResolving::~CNSPStateResolving
// ---------------------------------------------------------------------------
//
CNSPStateResolving::~CNSPStateResolving()
    {
    }


// ---------------------------------------------------------------------------
// CNSPStateResolving::DoAcceptL
// ---------------------------------------------------------------------------
//
TBool CNSPStateResolving::DoAcceptL( TNSPStateMachineEvent& aEvent )
    {
    User::LeaveIfError(
            TNSPStateMachineEvent::ENat == aEvent.Request() ||
            TNSPStateMachineEvent::EUpdate == aEvent.Request() ||
            TNSPStateMachineEvent::EReservationStatus == aEvent.Request() ||
            TNSPStateMachineEvent::ECloseSession == aEvent.Request() ?
                    KErrNone : KErrArgument );
    
    return ETrue;
    }


// ---------------------------------------------------------------------------
// CNSPStateResolving::DoStateEntryL
// ---------------------------------------------------------------------------
//
void CNSPStateResolving::DoStateEntryL( TNSPStateMachineEvent& aEvent )
    {
    aEvent.Session().Actions().SetRoleL();
    aEvent.Session().Actions().SetOutboundCredentialsL();
    aEvent.Session().Actions().ConnChecksL();
    }


// ---------------------------------------------------------------------------
// CNSPStateResolving::DoReverse
// ---------------------------------------------------------------------------
//
void CNSPStateResolving::DoReverse( TNSPStateMachineEvent& /*aEvent*/ )
    {
    }


// ---------------------------------------------------------------------------
// CNSPStateResolving::DoApplyL
// ---------------------------------------------------------------------------
//
void CNSPStateResolving::DoApplyL( TNSPStateMachineEvent& aEvent )
    {
    switch( aEvent.Request() )
        {
        case TNSPStateMachineEvent::ENat:
            {
            ControlMediaL( aEvent );
            break;
            }
        
        case TNSPStateMachineEvent::EUpdate:
            {
            ApplyUpdateL( aEvent );
            break;
            }
        
        case TNSPStateMachineEvent::EReservationStatus:
            {
            aEvent.Status() = KNatAsync;
            break;
            }
        
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
// CNSPStateResolving::DoExitL
// ---------------------------------------------------------------------------
//
void CNSPStateResolving::DoExitL( TNSPStateMachineEvent& aEvent )
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
// CNSPStateResolving::ControlMediaL
// ---------------------------------------------------------------------------
//
void CNSPStateResolving::ControlMediaL( TNSPStateMachineEvent& aEvent ) const
	{
	TNSPStateMachineMediaEvent& event = MEDIAEVENT( aEvent );
    TNatReturnStatus status = event.Session().Actions().ControlMediaL(
    		event.StreamId(), event.Event(), event.Status(), event.Data() );
    
    if ( NSP_TRIGGER( status ) )
    	{
    	event.NextState() = KStateIndexActivating;
        event.Status() = KNatAsync;
    	}
    else
    	{
    	event.Status() = status;
    	}
	}


// ---------------------------------------------------------------------------
// CNSPStateResolving::ApplyUpdateL
// ---------------------------------------------------------------------------
//
void CNSPStateResolving::ApplyUpdateL( TNSPStateMachineEvent& aEvent ) const
    {
    CNSPSession& session = aEvent.Session();
    CSdpDocument* offer = aEvent.Offer();
    
    if ( KNatAsync == session.Actions().GetCandidatesL( *offer ) )
        {
        session.Actions().UpdateL();
        }
    
    aEvent.Status() = KNatAsync;
    }


// end of file
