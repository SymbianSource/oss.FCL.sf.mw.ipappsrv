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

#include <sdpdocument.h>
#include "nspstateactivating.h"
#include "nspevents.h"
#include "nspsession.h"
#include "nspactionset.h"
#include "nspsessiondata.h"
#include "nspdefs.h"

// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// CNSPStateActivating::CNSPStateActivating
// ---------------------------------------------------------------------------
//
CNSPStateActivating::CNSPStateActivating()
    {
    }


// ---------------------------------------------------------------------------
// CNSPStateActivating::NewL
// ---------------------------------------------------------------------------
//
CNSPStateActivating* CNSPStateActivating::NewL()
    {
    CNSPStateActivating* self = CNSPStateActivating::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPStateActivating::NewLC
// ---------------------------------------------------------------------------
//
CNSPStateActivating* CNSPStateActivating::NewLC()
    {
    CNSPStateActivating* self = new ( ELeave ) CNSPStateActivating;
    CleanupStack::PushL( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPStateActivating::~CNSPStateActivating
// ---------------------------------------------------------------------------
//
CNSPStateActivating::~CNSPStateActivating()
    {
    }


// ---------------------------------------------------------------------------
// CNSPStateActivating::DoAcceptL
// ---------------------------------------------------------------------------
//
TBool CNSPStateActivating::DoAcceptL( TNSPStateMachineEvent& aEvent )
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
// CNSPStateActivating::DoStateEntryL
// ---------------------------------------------------------------------------
//
void CNSPStateActivating::DoStateEntryL( TNSPStateMachineEvent& aEvent )
    {
    CNSPSession& session = aEvent.Session();
    CSdpDocument* doc = ( OFFERER( session.Role() ) ? &session.Data().Answer() :
            ( ANSWERER( session.Role() ) ? &session.Data().Offer() : NULL ) );
    
    __ASSERT_ALWAYS( doc, User::Leave( KErrArgument ) );
    
    session.Actions().ActivateL( *doc );
    }


// ---------------------------------------------------------------------------
// CNSPStateActivating::DoReverse
// ---------------------------------------------------------------------------
//
void CNSPStateActivating::DoReverse( TNSPStateMachineEvent& /*aEvent*/ )
    {
    }


// ---------------------------------------------------------------------------
// CNSPStateActivating::DoApplyL
// ---------------------------------------------------------------------------
//
void CNSPStateActivating::DoApplyL( TNSPStateMachineEvent& aEvent )
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
// CNSPStateActivating::DoExitL
// ---------------------------------------------------------------------------
//
void CNSPStateActivating::DoExitL( TNSPStateMachineEvent& aEvent )
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
// CNSPStateActivating::ControlMediaL
// ---------------------------------------------------------------------------
//
void CNSPStateActivating::ControlMediaL( TNSPStateMachineEvent& aEvent ) const
	{
	TNSPStateMachineMediaEvent& event = MEDIAEVENT( aEvent );
    TNatReturnStatus status = event.Session().Actions().ControlMediaL(
    		event.StreamId(), event.Event(), event.Status(), event.Data() );
    
    if ( NSP_TRIGGER( status ) )
    	{
    	ApplyNatL( event );
    	}
    else
    	{
    	event.Status() = status;
    	}
	}


// ---------------------------------------------------------------------------
// CNSPStateActivating::ApplyNatL
// ---------------------------------------------------------------------------
//
void CNSPStateActivating::ApplyNatL( TNSPStateMachineEvent& aEvent ) const
    {
    CNSPSession& session = aEvent.Session();
    CSdpDocument* doc = ( OFFERER( session.Role() ) ? &session.Data().Answer() :
            ( ANSWERER( session.Role() ) ? &session.Data().Offer() : NULL ) );

    __ASSERT_ALWAYS( doc, User::Leave( KErrArgument ) );
    session.Actions().LocalRoundL( *doc );
    
    aEvent.NextState() = KStateIndexConnected;
    }


// ---------------------------------------------------------------------------
// CNSPStateActivating::ApplyUpdateL
// ---------------------------------------------------------------------------
//
void CNSPStateActivating::ApplyUpdateL( TNSPStateMachineEvent& aEvent ) const
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
