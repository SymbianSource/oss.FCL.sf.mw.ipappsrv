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
* Description:  Connected state implementation.
*
*/

#include <sdpdocument.h>
#include "nspstateconnected.h"
#include "nspevents.h"
#include "nspsession.h"
#include "nspactionset.h"
#include "nspsessiondata.h"
#include "nspdefs.h"

// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// CNSPStateConnected::CNSPStateConnected
// ---------------------------------------------------------------------------
//
CNSPStateConnected::CNSPStateConnected()
    {
    }


// ---------------------------------------------------------------------------
// CNSPStateConnected::NewL
// ---------------------------------------------------------------------------
//
CNSPStateConnected* CNSPStateConnected::NewL()
    {
    CNSPStateConnected* self = CNSPStateConnected::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPStateConnected::NewLC
// ---------------------------------------------------------------------------
//
CNSPStateConnected* CNSPStateConnected::NewLC()
    {
    CNSPStateConnected* self = new ( ELeave ) CNSPStateConnected;
    CleanupStack::PushL( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPStateConnected::~CNSPStateConnected
// ---------------------------------------------------------------------------
//
CNSPStateConnected::~CNSPStateConnected()
    {
    }


// ---------------------------------------------------------------------------
// CNSPStateConnected::DoAcceptL
// ---------------------------------------------------------------------------
//
TBool CNSPStateConnected::DoAcceptL( TNSPStateMachineEvent& aEvent )
    {
    TBool accept = ETrue;
    
    User::LeaveIfError(
    		TNSPStateMachineEvent::ENat == aEvent.Request() ||
            TNSPStateMachineEvent::ECreateOffer == aEvent.Request() ||
            TNSPStateMachineEvent::EResolve == aEvent.Request() ||
            TNSPStateMachineEvent::EDecodeAnswer == aEvent.Request() ||
            TNSPStateMachineEvent::EUpdate == aEvent.Request() ||
            TNSPStateMachineEvent::EReservationStatus == aEvent.Request() ||
            TNSPStateMachineEvent::ECloseSession == aEvent.Request() ?
                    KErrNone : KErrTotalLossOfPrecision );
    
    if ( TNSPStateMachineEvent::ENat == aEvent.Request() )
    	{
    	// Events are ignored if return value from this method is 'EFalse'.
    	TNSPStateMachineMediaEvent& event = MEDIAEVENT( aEvent );
    	
    	accept = (
    		MNATFWConnectivityObserver::EReceivingActivated == event.Event() ||
        	MNATFWConnectivityObserver::ESendingActivated == event.Event() ||
        	( MNATFWConnectivityObserver::EGeneralError == event.Event() &&
        			NSP_ERROR( event.Status() ) ) );
    	}
    
    return accept;
    }


// ---------------------------------------------------------------------------
// CNSPStateConnected::DoStateEntryL
// ---------------------------------------------------------------------------
//
void CNSPStateConnected::DoStateEntryL( TNSPStateMachineEvent& aEvent )
    {
    CNSPSession& session = aEvent.Session();
    
    session.Actions().UpdateRoundL( session.Data().UpdatedSdp() );
    
    if ( ANSWERER( session.Role() ) )
        {
    	aEvent.Offer() = session.Data().ReleaseOffer();
    	aEvent.CallbackType() = TEventReturnStatus::EOfferReady;
        }
    else if ( OFFERER( session.Role() ) )
        {
    	aEvent.Answer() = session.Data().ReleaseAnswer();
    	aEvent.CallbackType() = TEventReturnStatus::EAnswerReady;
        }
    else
        {
        User::Leave( KErrArgument );
        }
    }


// ---------------------------------------------------------------------------
// CNSPStateConnected::DoReverse
// ---------------------------------------------------------------------------
//
void CNSPStateConnected::DoReverse( TNSPStateMachineEvent& aEvent )
    {
    CNSPSession& session = aEvent.Session();
    session.Data().SetOffer( NULL );
    session.Data().SetAnswer( NULL );
    session.Role() = CNSPSession::EUndefined;
    }


// ---------------------------------------------------------------------------
// CNSPStateConnected::DoApplyL
// ---------------------------------------------------------------------------
//
void CNSPStateConnected::DoApplyL( TNSPStateMachineEvent& aEvent )
    {
    switch ( aEvent.Request() )
        {
        case TNSPStateMachineEvent::ENat:
        	{
        	ControlMediaL( aEvent );
        	break;
        	}
        
        case TNSPStateMachineEvent::ECreateOffer:
            {
            ApplyCreateOfferL( aEvent );
            break;
            }
        
        case TNSPStateMachineEvent::EResolve:
            {
            ApplyResolveL( aEvent );
            break;
            }
        
        case TNSPStateMachineEvent::EDecodeAnswer:
            {
            ApplyDecodeAnswerL( aEvent );
            break;
            }
        
        case TNSPStateMachineEvent::EUpdate:
            {
            ApplyUpdateL( aEvent );
            break;
            }
        
        case TNSPStateMachineEvent::EReservationStatus:
            {
            aEvent.Status() = KNatReady;
            break;
            }
        
        case TNSPStateMachineEvent::ECloseSession:
            {
            aEvent.NextState() = KStateIndexDeActivating;
            aEvent.Status() = KNatAsync;
            break;
            }
        
        default:
            {
            User::Leave( KErrArgument );
            }
        }
    }


// ---------------------------------------------------------------------------
// CNSPStateConnected::DoExitL
// ---------------------------------------------------------------------------
//
void CNSPStateConnected::DoExitL( TNSPStateMachineEvent& aEvent )
    {
    CNSPSession& session = aEvent.Session();
    
    switch ( aEvent.Request() )
        {
        case TNSPStateMachineEvent::EResolve:
            {
            session.Data().SetOffer( aEvent.Offer()->CloneL() );
            session.Data().SetAnswer( aEvent.Answer()->CloneL() );
            session.Role() = CNSPSession::EAnswerer;
            break;
            }
        
        case TNSPStateMachineEvent::EDecodeAnswer:
            {
            session.Data().SetAnswer( aEvent.Answer()->CloneL() );
            session.Role() = CNSPSession::EOfferer;
            break;
            }
        case TNSPStateMachineEvent::ECreateOffer:
        	{
        	session.Data().SetOffer( aEvent.Offer()->CloneL() );
        	session.Role() = CNSPSession::EOfferer;
        	break;
        	}
        case TNSPStateMachineEvent::ECloseSession:
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
// CNSPStateConnected::ControlMediaL
// ---------------------------------------------------------------------------
//
void CNSPStateConnected::ControlMediaL( TNSPStateMachineEvent& aEvent ) const
	{
	TNSPStateMachineMediaEvent& event = MEDIAEVENT( aEvent );
    TNatReturnStatus status = event.Session().Actions().ControlMediaL(
    		event.StreamId(), event.Event(), event.Status(), event.Data() );
    
    if ( NSP_TRIGGER( status ) )
    	{
    	// NOP
    	}
    else
    	{
    	event.Status() = status;
    	}
	}


// ---------------------------------------------------------------------------
// CNSPStateConnected::ApplyCreateOfferL
// ---------------------------------------------------------------------------
//
void CNSPStateConnected::ApplyCreateOfferL( TNSPStateMachineEvent& aEvent ) const
    {
    CNSPSession& session = aEvent.Session();
    TNatReturnStatus status = KNatReady;
    
    status = session.Actions().CreateStreamsL( *aEvent.Offer() );
    
    if ( KNatAsync == status )
        {	
        aEvent.NextState() = KStateIndexLocalCand;  	
        }
    else
        {
        session.Actions().LatterRoundL( *aEvent.Offer() );
        }
    aEvent.Status() = status;
    }


// ---------------------------------------------------------------------------
// CNSPStateConnected::ApplyResolveL
// ---------------------------------------------------------------------------
//
void CNSPStateConnected::ApplyResolveL( TNSPStateMachineEvent& aEvent ) const
    {
    CNSPSession& session = aEvent.Session();
    TNatReturnStatus status = KNatReady;
    
    status = session.Actions().CreateStreamsL( *aEvent.Answer() );
    
    if ( KNatAsync == status )
        {
        aEvent.NextState() = KStateIndexLocalCand;  	
        // TBD: Adding media streams, goto Localcand state.
        }
    else
        {
        session.Actions().LatterRoundL( *aEvent.Answer() );
        session.Actions().GetSessionConnInfoL( *aEvent.Offer() ); // ret ignored
        session.Actions().LocalRoundL( *aEvent.Offer() );
        }
    
    aEvent.Status() = status;
    }


// ---------------------------------------------------------------------------
// CNSPStateConnected::ApplyDecodeAnswerL
// ---------------------------------------------------------------------------
//
void CNSPStateConnected::ApplyDecodeAnswerL( TNSPStateMachineEvent& aEvent ) const
    {
    CNSPSession& session = aEvent.Session();
    
    session.Actions().GetSessionConnInfoL( *aEvent.Answer() );
    session.Actions().LocalRoundL( *aEvent.Answer() );
    }


// ---------------------------------------------------------------------------
// CNSPStateConnected::ApplyUpdateL
// ---------------------------------------------------------------------------
//
void CNSPStateConnected::ApplyUpdateL( TNSPStateMachineEvent& aEvent ) const
    {
    CNSPSession& session = aEvent.Session();
    
    if ( OFFERER( session.Role() ) )
        {
        session.Actions().LocalRoundL( *aEvent.Offer() );
        }
    else
        {
        User::Leave( KErrArgument );
        }
    }

// end of file
