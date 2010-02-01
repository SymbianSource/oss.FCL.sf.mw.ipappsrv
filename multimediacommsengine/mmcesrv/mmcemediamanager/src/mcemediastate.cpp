/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  
*
*/




#include <mmccevents.h>

#include "mcemediastate.h"
#include "mcemediaobserver.h"
#include "mcecomsession.h"
#include "mcesdpsession.h"
#include "mceevents.h"
#include "mcesrvstream.h"
#include "mcemmlogs.h"


#define DEFAULT_IMPLEMENTATION() User::Leave( KErrTotalLossOfPrecision )
#define DEFAULT_RETURN_IMPLEMENTATION( returnVal ) DEFAULT_IMPLEMENTATION();return returnVal



// -----------------------------------------------------------------------------
// TMceMediaState::TMceMediaState
// -----------------------------------------------------------------------------
//
TMceMediaState::TMceMediaState ( CMceComSession& aSession )
    : iSession( aSession ),
      iRole( EMceRoleOfferer ),
      iStackIndex( 0 )
    {
    }
    
// -----------------------------------------------------------------------------
// TMceMediaState::~TMceMediaState
// -----------------------------------------------------------------------------
//
TMceMediaState::~TMceMediaState ()
	{
	}


// -----------------------------------------------------------------------------
// TMceMediaState::CloneL
// -----------------------------------------------------------------------------
//
TMceMediaState* TMceMediaState::CloneL( CMceComSession& /*aSession*/ ) const
    {
    DEFAULT_RETURN_IMPLEMENTATION( NULL );
    }


// -----------------------------------------------------------------------------
// TMceMediaState::EntryL
// -----------------------------------------------------------------------------
//
void TMceMediaState::EntryL()
    {
    //NOP
    }


// -----------------------------------------------------------------------------
// TMceMediaState::Pushed
// -----------------------------------------------------------------------------
//
void TMceMediaState::Pushed()
    {
    iStackIndex--;
    }
    
// -----------------------------------------------------------------------------
// TMceMediaState::StackIndex
// -----------------------------------------------------------------------------
//
TInt TMceMediaState::StackIndex()
    {
    return iStackIndex;
    }

// -----------------------------------------------------------------------------
// TMceMediaState::CallbackAllowed
// -----------------------------------------------------------------------------
//
TBool TMceMediaState::CallbackAllowed()
    {
     return iSession.NegotiationState().Id() != KMceMediaIdle &&
            iSession.NegotiationState().Id() != KMceAnsweringMedia &&
            iSession.NegotiationState().Id() != KMceAnsweringMediaUpdate &&
            iSession.NegotiationState().Id() != KMceOfferingMedia &&
            iSession.NegotiationState().Id() != KMceMediaNegotiated;
    
    }

// -----------------------------------------------------------------------------
// TMceMediaState::Callback
// -----------------------------------------------------------------------------
//
void TMceMediaState::Callback()
    {
    //NOP
    }

// -----------------------------------------------------------------------------
// TMceMediaState::SetRole
// -----------------------------------------------------------------------------
//
void TMceMediaState::SetRole( TMceNegotiationRole aRole )
    {
    iRole = aRole;
    }
    
// -----------------------------------------------------------------------------
// TMceMediaState::SetRole
// -----------------------------------------------------------------------------
//
TMceNegotiationRole TMceMediaState::Role() const
    {
    return iRole;
    }
    


// -----------------------------------------------------------------------------
// TMceMediaState::MccError
// -----------------------------------------------------------------------------
//
void TMceMediaState::MccError( TInt aError, TMceMccComEvent& aEvent )
    {
    //force to be error, if not yet
    aEvent.iEvent = KMccStreamError;
    aEvent.iError = aError;
    

    CMceSrvStream::EventReceived( iSession.MccStreams(),
                                  aEvent );

    }



// -----------------------------------------------------------------------------
// TMceMediaState::MccLinkCreatedL
// -----------------------------------------------------------------------------
//
void TMceMediaState::MccLinkCreatedL( TMceMccComEvent& /*aEvent*/ )
    {
    DEFAULT_IMPLEMENTATION();
    }

// -----------------------------------------------------------------------------
// TMceMediaState::MccMediaPreparedL
// -----------------------------------------------------------------------------
//
void TMceMediaState::MccMediaPreparedL( TMceMccComEvent& /*aEvent*/ )
    {
    DEFAULT_IMPLEMENTATION();
    }


// -----------------------------------------------------------------------------
// TMceMediaState::MccMediaStartedL
// -----------------------------------------------------------------------------
//
void TMceMediaState::MccMediaStartedL( TMceMccComEvent& aEvent )
    {
    CMceSrvStream::EventReceived( iSession.MccStreams(), aEvent );
    }

// -----------------------------------------------------------------------------
// TMceMediaState::MccMediaPausedL
// -----------------------------------------------------------------------------
//
void TMceMediaState::MccMediaPausedL( TMceMccComEvent& aEvent )
    {
    CMceSrvStream::EventReceived( iSession.MccStreams(), aEvent );
    }
    
// -----------------------------------------------------------------------------
// TMceMediaState::MccMediaResumedL
// -----------------------------------------------------------------------------
//
void TMceMediaState::MccMediaResumedL( TMceMccComEvent& aEvent )
    {
    CMceSrvStream::EventReceived( iSession.MccStreams(), aEvent );
    }
    
// -----------------------------------------------------------------------------
// TMceMediaState::UpdateL
// -----------------------------------------------------------------------------
//
void TMceMediaState::UpdateL()
    {
    DEFAULT_IMPLEMENTATION();
    }


// -----------------------------------------------------------------------------
// TMceMediaState::DecodeL
// -----------------------------------------------------------------------------
//
TMceSipWarningCode TMceMediaState::DecodeL( CSdpDocument& /*aSdpDocument*/ )
    {
    DEFAULT_RETURN_IMPLEMENTATION( KMceSipWarnMediaTypeNotAvailable );
    }

// -----------------------------------------------------------------------------
// TMceMediaState::EncodeL
// -----------------------------------------------------------------------------
//
CSdpDocument* TMceMediaState::EncodeL()
    {
    DEFAULT_RETURN_IMPLEMENTATION( NULL );
    }

// -----------------------------------------------------------------------------
// TMceMediaState::NeedToNegotiate
// -----------------------------------------------------------------------------
//
TBool TMceMediaState::NeedToNegotiate( CSdpDocument* /*aSdpDocument*/ )
    {
    MCEMM_DEBUG("TMceMediaState::NeedToNegotiate() ");
    
    return EFalse;
    }

// -----------------------------------------------------------------------------
// TMceMediaState::ConfirmationNeeded
// -----------------------------------------------------------------------------
//
TBool TMceMediaState::ConfirmationNeeded()
    {
    return iSession.SdpSession().NeedToNegotiate( iSession );
    }

// -----------------------------------------------------------------------------
// TMceMediaState::ReservationNeeded
// -----------------------------------------------------------------------------
//
TBool TMceMediaState::ReservationNeeded()
    {
    return iSession.SdpSession().ReservationNeeded( iSession );
    }

// -----------------------------------------------------------------------------
// TMceMediaState::ReserveL
// -----------------------------------------------------------------------------
//
TMceReturnStatus TMceMediaState::ReserveL()
    {
    return iSession.SdpSession().ReserveL( iSession );
    }



// End of File


