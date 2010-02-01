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



#include "mcenatstatedecodeoffer.h"
#include "mcenatsipsession.h"
#include "mceactionset.h"
#include "mceclientserver.h"
#include "mcenatevents.h"
#include "mcesrvlogs.h"
#include "nspplugin.h"

// -----------------------------------------------------------------------------
// CMceNatStateDecodeOffer::CMceNatStateDecodeOffer
// -----------------------------------------------------------------------------
//
CMceNatStateDecodeOffer::CMceNatStateDecodeOffer()
	{
	}


// -----------------------------------------------------------------------------
// CMceNatStateDecodeOffer::~CMceNatStateDecodeOffer
// -----------------------------------------------------------------------------
//
CMceNatStateDecodeOffer::~CMceNatStateDecodeOffer()
	{
	}

// -----------------------------------------------------------------------------
// CMceNatStateDecodeOffer::Accept
// -----------------------------------------------------------------------------
//
TBool CMceNatStateDecodeOffer::Accept( TMceNatStateTransitionEvent& aEvent )
    {
    if ( EMceNatOffer == aEvent.NatCode() ||
         ( EMceNatHandleSIP == aEvent.NatCode() &&
           EMceInvite == aEvent.Code() )||
         EMceNatError == aEvent.NatCode() ||
         EMceNatICMPError == aEvent.NatCode() )
        {
        return ETrue;
        }
    else
        {
        return EFalse;
        }

    }

// -----------------------------------------------------------------------------
// CMceNatStateDecodeOffer::EntryL
// -----------------------------------------------------------------------------
//
void CMceNatStateDecodeOffer::EntryL( TMceNatStateTransitionEvent& aEvent )
	{
	MCESRV_DEBUG( "*** CMceNatStateDecodeOffer::EntryL, Entry" );
	
    aEvent.NatActions().Continue();
        
	MCESRV_DEBUG( "*** CMceNatStateDecodeOffer::EntryL, Exit" );
	}

// -----------------------------------------------------------------------------
// CMceNatStateDecodeOffer::ExitL
// -----------------------------------------------------------------------------
//
void CMceNatStateDecodeOffer::ExitL( TMceNatStateTransitionEvent& aEvent )
	{
	MCESRV_DEBUG( "*** CMceNatStateDecodeOffer::ExitL, Entry" );

	switch ( aEvent.NatCode() )
	    {
	    case EMceNatError:
	    case EMceNatICMPError:
	        {
	        ExitErrorL( aEvent );
	        break;
	        }        
	    case EMceNatHandleSIP:
	        {
	        ExitHandleInviteL( aEvent );
	        break;
	        }
	    default:
	        {
	        ExitDefaultL( aEvent );
	        break;
	        }
	    }

    MCESRV_DEBUG( "*** CMceNatStateDecodeOffer::ExitL, Exit" );
	}

// -----------------------------------------------------------------------------
// CMceNatStateDecodeOffer::ExitErrorL
// -----------------------------------------------------------------------------
//
void CMceNatStateDecodeOffer::ExitErrorL( TMceNatStateTransitionEvent& aEvent )
    {
    
    TMceStateIndex sipState = aEvent.Session().CurrentState().Id();
    
    if (( aEvent.NatCode() == EMceNatICMPError && 
    		KMceStateEstablished == sipState ) || 
    		aEvent.NatCode() == EMceNatError )
    
    	{
    	aEvent.Session().Actions().ClientErrorOccured( aEvent.ParamStatus() );
    	if ( aEvent.NatCode() == EMceNatError )
    		{
    		User::Leave( aEvent.ParamStatus() );
    		}
    	}
    }


// -----------------------------------------------------------------------------
// CMceNatStateDecodeOffer::ExitHandleInviteL
// -----------------------------------------------------------------------------
//
void CMceNatStateDecodeOffer::ExitHandleInviteL( TMceNatStateTransitionEvent& aEvent )
    {
    CMceSipSession& session = aEvent.Session();
    
    if ( MceSip::HasContent( session.Request() ) )
        {
        // Incoming INVITE contained new SDP, which can contain new candidates,
        // hence it is given to NAT - without state update.
        User::LeaveIfError( session.Actions().CreateSDP( session.Request() ) );
        aEvent.NatActions().UpdateL();
        }
    
    // Not consumed, done by SIP state machine
    aEvent.NatActions().Continue();
    }

// -----------------------------------------------------------------------------
// CMceNatStateDecodeOffer::ExitDefaultL 
// -----------------------------------------------------------------------------
//
void CMceNatStateDecodeOffer::ExitDefaultL( TMceNatStateTransitionEvent& aEvent )
    {
    User::LeaveIfNull( aEvent.Document() );
    CMceSipSession& session = aEvent.Session();
    
    aEvent.Session().SetPrevOffer( aEvent.Document() );
    aEvent.NatActions().SetOffered( KMceNatPrevOfferSdp );
    aEvent.Session().Actions().DecodeL();
    aEvent.NatActions().UpdateRemoteAddressL();
       
    CSdpDocument* previous = session.Offer();
    aEvent.NatActions().SetOffered( KMceNatOfferSdp );
    aEvent.NatActions().SdpCleanup( previous, session.Offer() );
       
    aEvent.NatActions().StateChangedL( aEvent, KMceNatStateConnected );
    
    
    // MCE should use reserved only when the plugin returns  
    // ENSPResourcesReserved enumeration and only in session initiation.

	if ( aEvent.NatActions().GetReservationStatus() == KMceAsync &&
		aEvent.NatActions().GetResourceReservationStatus() == 
    	  					 CNSPPlugin::ENSPResourcesReserved  ) 
		{
		//iReserveStatus = KMceReady;
		aEvent.NatActions().SetReservationStatus( KMceReady );
		session.Reserved( session.ActiveBody());		
		}
    }
