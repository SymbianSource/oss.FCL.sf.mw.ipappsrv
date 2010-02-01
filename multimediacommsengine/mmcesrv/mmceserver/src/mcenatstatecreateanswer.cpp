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



#include "mcenatstatecreateanswer.h"
#include "mcenatsipsession.h"
#include "mcenatactions.h"
#include "mcenatevents.h"
#include "mceactionset.h"
#include "mceclientserver.h"
#include "mcesrvlogs.h"
#include "nspplugin.h"

// -----------------------------------------------------------------------------
// CMceNatStateCreateAnswer::CMceNatStateCreateAnswer
// -----------------------------------------------------------------------------
//
CMceNatStateCreateAnswer::CMceNatStateCreateAnswer()
	{
	}

// -----------------------------------------------------------------------------
// CMceNatStateCreateAnswer::~CMceNatStateCreateAnswer
// -----------------------------------------------------------------------------
//
CMceNatStateCreateAnswer::~CMceNatStateCreateAnswer()
	{
	}

// -----------------------------------------------------------------------------
// CMceNatStateCreateAnswer::Accept
// -----------------------------------------------------------------------------
//
TBool CMceNatStateCreateAnswer::Accept( TMceNatStateTransitionEvent& aEvent )
    {
    if ( EMceNatAnswer == aEvent.NatCode() ||
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
// CMceNatStateCreateAnswer::EntryL
// -----------------------------------------------------------------------------
//
void CMceNatStateCreateAnswer::EntryL( TMceNatStateTransitionEvent& aEvent )
	{
	MCESRV_DEBUG( "*** CMceNatStateCreateAnswer::EntryL, Entry" );
    
    aEvent.Session().Actions().EncodeL();
    TMceReturnStatus status = aEvent.NatActions().ResolveL();
        
    if ( KMceReady == status )
	    {
	    CMceSipSession& session = aEvent.Session();
	    
        aEvent.NatCode() = ( KMceReady == aEvent.ParamStatus() ?
                EMceNatSync : aEvent.NatCode() );
	    
        aEvent.NatActions().SetOffered( KMceNatPrevOfferSdp );
        session.Actions().DecodeL(); 
        aEvent.NatActions().UpdateRemoteAddressL();
        
        CSdpDocument* previous = session.Offer();
        aEvent.NatActions().SetOffered( KMceNatOfferSdp );
        aEvent.NatActions().SdpCleanup( previous, session.Offer() );
       
	    aEvent.NatActions().StateChangedL( aEvent, KMceNatStateConnected );
	    	    
	    }
    else
        {
        aEvent.ParamStatus() = status;
        }
    
	MCESRV_DEBUG( "*** CMceNatStateCreateAnswer::EntryL, Exit" );
	}

// -----------------------------------------------------------------------------
// CMceNatStateCreateAnswer::ExitL
// -----------------------------------------------------------------------------
//
void CMceNatStateCreateAnswer::ExitL( TMceNatStateTransitionEvent& aEvent )
	{
	MCESRV_DEBUG( "*** CMceNatStateCreateAnswer::ExitL, Entry" );
    
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
    
	MCESRV_DEBUG( "*** CMceNatStateCreateAnswer::ExitL, Exit" );	
	}

// -----------------------------------------------------------------------------
// CMceNatStateCreateAnswer::ExitErrorL
// -----------------------------------------------------------------------------
//
void CMceNatStateCreateAnswer::ExitErrorL( TMceNatStateTransitionEvent& aEvent )
    {
    
    TMceStateIndex sipState = aEvent.Session().CurrentState().Id();
          
    if ( aEvent.ParamStatus() > KMceNatNonFatalErrorLimit )
        {
        aEvent.Session().Actions().ClientErrorOccured( aEvent.ParamStatus() );
        }
    
    
    if (( aEvent.NatCode() == EMceNatICMPError && 
    		KMceStateEstablished == sipState ) || 
    		aEvent.NatCode() == EMceNatError )  
    	{
    	aEvent.NatActions().Continue();
    	User::Leave( aEvent.ParamStatus() );		
    	}
       
    }

// -----------------------------------------------------------------------------
// CMceNatStateCreateAnswer::ExitHandleInviteL
// -----------------------------------------------------------------------------
//
void CMceNatStateCreateAnswer::ExitHandleInviteL( TMceNatStateTransitionEvent& aEvent )
    {
    CMceSipSession& session = aEvent.Session();
    
    if ( MceSip::HasContent( session.Request() ) )
        {
        // Incoming INVITE contained new SDP, which can contain new candidates,
        // hence it is given to NAT FW - without state update.
        User::LeaveIfError( session.Actions().CreateSDP( session.Request() ) );
        aEvent.NatActions().UpdateL();
        }
    
    // Not consumed, done by SIP state machine
    aEvent.NatActions().Continue();
    }

// -----------------------------------------------------------------------------
// CMceNatStateCreateAnswer::ExitDefaultL
// -----------------------------------------------------------------------------
//
void CMceNatStateCreateAnswer::ExitDefaultL( TMceNatStateTransitionEvent& aEvent )
    {
	
	// Check if plugin has flag setup if yes signall that at the session level.
	TInt status = aEvent.NatActions().GetResourceReservationStatus();
	
	if ( status == CNSPPlugin::ENSPResourcesNotReserved )
		{
		aEvent.NatActions().SetNeedtoReceive();	
		}
	
    User::LeaveIfNull( aEvent.Document() );
    CMceSipSession& session = aEvent.Session();
    
    CSdpDocument* previous = session.Offer();
    aEvent.Session().SetOffer( aEvent.Document() );
    aEvent.NatActions().SetOffered( KMceNatOfferSdp );
    aEvent.NatActions().SdpCleanup( previous, session.Offer() );
    
    aEvent.NatActions().StateChangedL( aEvent, KMceNatStateDecodeOffer );
    }
