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
* Description:  Idle state implementation
*
*/

#include <sdpdocument.h>
#include "nspstateidle.h"
#include "nspevents.h"
#include "nspsession.h"
#include "nspactionset.h"
#include "nspsessiondata.h"
#include "nspdefs.h"

#define REMOTELITE( aData ) \
    ( aData.SubState() == CNSPSessionData::ESubRemoteLite ||\
      aData.SubState() == CNSPSessionData::ESubAll )

// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// CNSPStateIdle::CNSPStateIdle
// ---------------------------------------------------------------------------
//
CNSPStateIdle::CNSPStateIdle()
    {
    }


// ---------------------------------------------------------------------------
// CNSPStateIdle::NewL
// ---------------------------------------------------------------------------
//
CNSPStateIdle* CNSPStateIdle::NewL()
    {
    CNSPStateIdle* self = CNSPStateIdle::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPStateIdle::NewLC
// ---------------------------------------------------------------------------
//
CNSPStateIdle* CNSPStateIdle::NewLC()
    {
    CNSPStateIdle* self = new ( ELeave ) CNSPStateIdle;
    CleanupStack::PushL( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPStateIdle::~CNSPStateIdle
// ---------------------------------------------------------------------------
//
CNSPStateIdle::~CNSPStateIdle()
    {
    }


// ---------------------------------------------------------------------------
// CNSPStateIdle::DoAcceptL
// ---------------------------------------------------------------------------
//
TBool CNSPStateIdle::DoAcceptL( TNSPStateMachineEvent& aEvent )
    {
    User::LeaveIfError(
            TNSPStateMachineEvent::ECreateOffer == aEvent.Request() ||
            TNSPStateMachineEvent::EResolve == aEvent.Request() ||
            TNSPStateMachineEvent::EReservationStatus == aEvent.Request() ||
            TNSPStateMachineEvent::ECloseSession == aEvent.Request() ?
                    KErrNone : KErrArgument );
    
    return ETrue;
    }


// ---------------------------------------------------------------------------
// CNSPStateIdle::DoStateEntryL
// ---------------------------------------------------------------------------
//
void CNSPStateIdle::DoStateEntryL( TNSPStateMachineEvent& aEvent )
    {
    aEvent.CallbackType() = TEventReturnStatus::EInitialized;
    }


// ---------------------------------------------------------------------------
// CNSPStateIdle::DoReverse
// ---------------------------------------------------------------------------
//
void CNSPStateIdle::DoReverse( TNSPStateMachineEvent& aEvent )
    {
    CNSPSession& session = aEvent.Session();
    session.Data().SetOffer( NULL );
    session.Data().SetAnswer( NULL );
    session.Role() = CNSPSession::EUndefined;
    }


// ---------------------------------------------------------------------------
// CNSPStateIdle::DoApplyL
// ---------------------------------------------------------------------------
//
void CNSPStateIdle::DoApplyL( TNSPStateMachineEvent& aEvent )
    {
    switch( aEvent.Request() )
        {
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
// CNSPStateIdle::DoExitL
// ---------------------------------------------------------------------------
//
void CNSPStateIdle::DoExitL( TNSPStateMachineEvent& aEvent )
    {
    CNSPSession& session = aEvent.Session();
    
    switch( aEvent.Request() )
        {
        case TNSPStateMachineEvent::ECreateOffer:
            {
            session.Data().SetOffer( aEvent.Offer()->CloneL() );
            session.Role() = CNSPSession::EOfferer;
            break;
            }
        
        case TNSPStateMachineEvent::EResolve:
            {
            session.Data().SetOffer( aEvent.Offer()->CloneL() );
            session.Data().SetAnswer( aEvent.Answer()->CloneL() );
            session.Role() = CNSPSession::EAnswerer;
            break;
            }
        
        default:
            {
            User::Leave( KErrArgument );
            }
        }
    }


// ---------------------------------------------------------------------------
// CNSPStateIdle::ApplyCreateOfferL
// ---------------------------------------------------------------------------
//
void CNSPStateIdle::ApplyCreateOfferL( TNSPStateMachineEvent& aEvent ) const
    {
    aEvent.Session().Actions().CreateStreamsL( *aEvent.Offer() );
    aEvent.NextState() = KStateIndexLocalCand;
    aEvent.Status() = KNatAsync;
    }


// ---------------------------------------------------------------------------
// CNSPStateIdle::ApplyResolveL
// ---------------------------------------------------------------------------
//
void CNSPStateIdle::ApplyResolveL( TNSPStateMachineEvent& aEvent ) const
    {
    TNatReturnStatus status = KNatReady;
    CNSPSession& session = aEvent.Session();
    
    session.Actions().CreateStreamsL( *aEvent.Answer() );
    status = session.Actions().GetSessionModifiersL(
    			*aEvent.Offer(), CNSPSession::EAnswerer );
    
    CNSPSessionData& sessionData = session.Data();
    if ( status == KNatAsync && !REMOTELITE( sessionData ) )
        {
        session.Actions().RestartWithoutIce();
        }
    
    aEvent.NextState() = KStateIndexLocalCand;
    aEvent.Status() = KNatAsync;
    }


// end of file
