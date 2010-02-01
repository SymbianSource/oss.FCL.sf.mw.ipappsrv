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
* Description:  Localcand state implementation.
*
*/

#include <sdpdocument.h>
#include "nspstatelocalcand.h"
#include "nspevents.h"
#include "nspsession.h"
#include "nspactionset.h"
#include "nspsessiondata.h"
#include "nspcontentparser.h"
#include "nspdefs.h"

static void CondAddMismatchL( CNSPSession& aSession,
		CSdpDocument& aOffer, CSdpDocument& aAnswer )
	{
	if ( aSession.Data().SubState() == CNSPSessionData::ESubMismatch ||
	     aSession.Data().SubState() == CNSPSessionData::ESubAll )
		{
		aSession.Parser().AddMismatchL( aOffer, aAnswer );
		}
	}

// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// CNSPStateLocalCand::CNSPStateLocalCand
// ---------------------------------------------------------------------------
//
CNSPStateLocalCand::CNSPStateLocalCand()
    {
    }


// ---------------------------------------------------------------------------
// CNSPStateLocalCand::NewL
// ---------------------------------------------------------------------------
//
CNSPStateLocalCand* CNSPStateLocalCand::NewL()
    {
    CNSPStateLocalCand* self = CNSPStateLocalCand::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPStateLocalCand::NewLC
// ---------------------------------------------------------------------------
//
CNSPStateLocalCand* CNSPStateLocalCand::NewLC()
    {
    CNSPStateLocalCand* self = new ( ELeave ) CNSPStateLocalCand;
    CleanupStack::PushL( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPStateLocalCand::~CNSPStateLocalCand
// ---------------------------------------------------------------------------
//
CNSPStateLocalCand::~CNSPStateLocalCand()
    {
    }


// ---------------------------------------------------------------------------
// CNSPStateLocalCand::DoAcceptL
// ---------------------------------------------------------------------------
//
TBool CNSPStateLocalCand::DoAcceptL( TNSPStateMachineEvent& aEvent )
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
// CNSPStateLocalCand::DoStateEntryL
// ---------------------------------------------------------------------------
//
void CNSPStateLocalCand::DoStateEntryL( TNSPStateMachineEvent& aEvent )
    {
    aEvent.Session().Actions().LoadPluginL();
    aEvent.Session().Actions().FetchL();
    }


// ---------------------------------------------------------------------------
// CNSPStateLocalCand::DoReverse
// ---------------------------------------------------------------------------
//
void CNSPStateLocalCand::DoReverse( TNSPStateMachineEvent& /*aEvent*/ )
    {
    }


// ---------------------------------------------------------------------------
// CNSPStateLocalCand::DoApplyL
// ---------------------------------------------------------------------------
//
void CNSPStateLocalCand::DoApplyL( TNSPStateMachineEvent& aEvent )
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
// CNSPStateLocalCand::DoExitL
// ---------------------------------------------------------------------------
//
void CNSPStateLocalCand::DoExitL( TNSPStateMachineEvent& aEvent )
    {
    CNSPSession& session = aEvent.Session();
    
    switch( aEvent.Request() )
        {
        case TNSPStateMachineEvent::ENat:
            {
            if ( ANSWERER( session.Role() ) )
            	{
            	aEvent.Answer() = session.Data().ReleaseAnswer();
            	aEvent.CallbackType() = TEventReturnStatus::EAnswerReady;
            	}
            
            break;
            }
        
        default:
            {
            User::Leave( KErrArgument );
            }
        }
    }

// ---------------------------------------------------------------------------
// CNSPStateLocalCand::ControlMediaL
// ---------------------------------------------------------------------------
//
void CNSPStateLocalCand::ControlMediaL( TNSPStateMachineEvent& aEvent ) const
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
// CNSPStateLocalCand::ApplyNatL
// ---------------------------------------------------------------------------
//
void CNSPStateLocalCand::ApplyNatL( TNSPStateMachineEvent& aEvent ) const
    {
    CNSPSession& session = aEvent.Session();
    CSdpDocument& offer = session.Data().Offer();
    TNSPStateIndex nextstate = KStateIndexNegotiating;
    TNatReturnStatus status = KNatReady;
    
    if ( OFFERER( session.Role() ) )
        {
        session.Actions().FirstRoundL( offer );
        }
    else if ( ANSWERER( session.Role() ) )
        {
        CSdpDocument& answer = session.Data().Answer();
        
        session.Actions().FirstRoundL( answer );
        CondAddMismatchL( session, offer, answer );
        session.Actions().SetInboundCredentialsL();
        
        session.Actions().GetCredentialsL( offer );
        session.Actions().GetCandidatesL( offer );
        
        nextstate = ( session.Data().UseIce() ?
                KStateIndexResolving : KStateIndexActivating );
        status = KNatAsync;
        }
    else
        {
        User::Leave( KErrArgument );
        }
    
    aEvent.NextState() = nextstate;
    aEvent.Status() = status;
    }


// ---------------------------------------------------------------------------
// CNSPStateLocalCand::ApplyUpdateL
// ---------------------------------------------------------------------------
//
void CNSPStateLocalCand::ApplyUpdateL( TNSPStateMachineEvent& aEvent ) const
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
