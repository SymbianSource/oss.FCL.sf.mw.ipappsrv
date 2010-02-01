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
* Description:  Negotiating state implementation.
*
*/

#include <sdpdocument.h>
#include "nspstatenegotiating.h"
#include "nspevents.h"
#include "nspsession.h"
#include "nspactionset.h"
#include "nspsessiondata.h"
#include "nspdefs.h"

#define MISMATCH( aData ) \
    ( aData.SubState() == CNSPSessionData::ESubMismatch ||\
      aData.SubState() == CNSPSessionData::ESubAll )
#define REMOTELITE( aData ) \
    ( aData.SubState() == CNSPSessionData::ESubRemoteLite ||\
      aData.SubState() == CNSPSessionData::ESubAll )

// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// CNSPStateNegotiating::CNSPStateNegotiating
// ---------------------------------------------------------------------------
//
CNSPStateNegotiating::CNSPStateNegotiating()
    {
    }


// ---------------------------------------------------------------------------
// CNSPStateNegotiating::NewL
// ---------------------------------------------------------------------------
//
CNSPStateNegotiating* CNSPStateNegotiating::NewL()
    {
    CNSPStateNegotiating* self = CNSPStateNegotiating::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPStateNegotiating::NewLC
// ---------------------------------------------------------------------------
//
CNSPStateNegotiating* CNSPStateNegotiating::NewLC()
    {
    CNSPStateNegotiating* self = new ( ELeave ) CNSPStateNegotiating;
    CleanupStack::PushL( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPStateNegotiating::~CNSPStateNegotiating
// ---------------------------------------------------------------------------
//
CNSPStateNegotiating::~CNSPStateNegotiating()
    {
    }


// ---------------------------------------------------------------------------
// CNSPStateNegotiating::DoAcceptL
// ---------------------------------------------------------------------------
//
TBool CNSPStateNegotiating::DoAcceptL( TNSPStateMachineEvent& aEvent )
    {
    User::LeaveIfError(
            TNSPStateMachineEvent::ENat == aEvent.Request() ||
            TNSPStateMachineEvent::EUpdate == aEvent.Request() ||
            TNSPStateMachineEvent::EDecodeAnswer == aEvent.Request() ||
            TNSPStateMachineEvent::EReservationStatus == aEvent.Request() ||
            TNSPStateMachineEvent::ECloseSession == aEvent.Request() ?
                    KErrNone : KErrArgument );
    
    return ETrue;
    }


// ---------------------------------------------------------------------------
// CNSPStateNegotiating::DoStateEntryL
// ---------------------------------------------------------------------------
//
void CNSPStateNegotiating::DoStateEntryL( TNSPStateMachineEvent& aEvent )
    {
    CNSPSession& session = aEvent.Session();
    
    if ( OFFERER( session.Role() ) )
        {
        session.Actions().SetInboundCredentialsL();
        aEvent.Offer() = session.Data().ReleaseOffer();
        aEvent.CallbackType() = TEventReturnStatus::EOfferReady;
        }
    else
        {
        User::Leave( KErrArgument );
        }
    }


// ---------------------------------------------------------------------------
// CNSPStateNegotiating::DoReverse
// ---------------------------------------------------------------------------
//
void CNSPStateNegotiating::DoReverse( TNSPStateMachineEvent& aEvent )
    {
    aEvent.Session().Data().SetAnswer( NULL );
    aEvent.Session().Role() = CNSPSession::EUndefined;
    }


// ---------------------------------------------------------------------------
// CNSPStateNegotiating::DoApplyL
// ---------------------------------------------------------------------------
//
void CNSPStateNegotiating::DoApplyL( TNSPStateMachineEvent& aEvent )
    {
    switch( aEvent.Request() )
        {
        case TNSPStateMachineEvent::ENat:
            {
            ControlMediaL( aEvent );
            break;
            }
        
        case TNSPStateMachineEvent::EDecodeAnswer:
            {
            ApplyDecodeAnswerL( aEvent );
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
        case TNSPStateMachineEvent::EUpdate:
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
// CNSPStateNegotiating::DoExitL
// ---------------------------------------------------------------------------
//
void CNSPStateNegotiating::DoExitL( TNSPStateMachineEvent& aEvent )
    {
    CNSPSession& session = aEvent.Session();
    
    switch( aEvent.Request() )
        {
        case TNSPStateMachineEvent::ENat:
            {
            break;
            }
        
        case TNSPStateMachineEvent::EDecodeAnswer:
            {
            session.Data().SetAnswer( aEvent.Answer()->CloneL() );
            session.Role() = CNSPSession::EOfferer;
            break;
            }
        
        default:
            {
            User::Leave( KErrArgument );
            }
        }
    }

// ---------------------------------------------------------------------------
// CNSPStateNegotiating::ControlMediaL
// ---------------------------------------------------------------------------
//
void CNSPStateNegotiating::ControlMediaL( TNSPStateMachineEvent& aEvent ) const
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
// CNSPStateNegotiating::ApplyNatL
// ---------------------------------------------------------------------------
//
void CNSPStateNegotiating::ApplyNatL( TNSPStateMachineEvent& aEvent ) const
    {
    CNSPSession& session = aEvent.Session();
    
    if ( OFFERER( session.Role() ) )
        {
        CSdpDocument& answer = session.Data().Answer();
        session.Actions().GetCredentialsL( answer );
        session.Actions().GetCandidatesL( answer );
        aEvent.NextState() = ( session.Data().UseIce() ?
                KStateIndexResolving : KStateIndexActivating );
        aEvent.Status() = KNatAsync;
        }
    else if ( ANSWERER( session.Role() ) )
        {
        session.Actions().FirstRoundL( session.Data().UpdatedSdp(), EFalse );
        aEvent.Offer() =  session.Data().ReleaseUpdatedSdp();
        aEvent.CallbackType() = TEventReturnStatus::EUpdateSdp;
        }
    else
        {
        User::Leave( KErrArgument );
        }
    }


// ---------------------------------------------------------------------------
// CNSPStateNegotiating::ApplyDecodeAnswerL
// ---------------------------------------------------------------------------
//
void CNSPStateNegotiating::ApplyDecodeAnswerL( TNSPStateMachineEvent& aEvent )
        const
    {
    TNatReturnStatus status = KNatReady;
    CNSPSession& session = aEvent.Session();
    CSdpDocument* answer = aEvent.Answer();
    
    session.Actions().RemoveStreamsL( *answer );
    status = session.Actions().GetSessionModifiersL( *answer,
        CNSPSession::EOfferer );
    
    CNSPSessionData& sessionData = session.Data();
    if ( KNatAsync == status && !REMOTELITE( sessionData ) )
        {
        session.Data().SetAnswer( answer->CloneL() );
        session.Role() = CNSPSession::EOfferer;
        session.Actions().RestartWithoutIce();
        session.Actions().LoadPluginL();
        session.Actions().FetchL();
        }
    else
        {
        session.Actions().GetCredentialsL( *answer );
        session.Actions().GetCandidatesL( *answer );
        aEvent.NextState() = ( session.Data().UseIce() ?
                KStateIndexResolving : KStateIndexActivating );
        }
    
    aEvent.Status() = KNatAsync;
    }


// end of file
