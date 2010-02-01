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



#include "mcenatstateconnected.h"
#include "mcenatsipsession.h"
#include "mceactionset.h"
#include "mceclientserver.h"
#include "mcenatevents.h"
#include "mcenatactions.h"
#include "mcesrvlogs.h"

// -----------------------------------------------------------------------------
// CMceNatStateConnected::CMceNatStateConnected
// -----------------------------------------------------------------------------
//
CMceNatStateConnected::CMceNatStateConnected()
	{
	}

// -----------------------------------------------------------------------------
// CMceNatStateConnected::~CMceNatStateConnected
// -----------------------------------------------------------------------------
//
CMceNatStateConnected::~CMceNatStateConnected()
	{
	}

// -----------------------------------------------------------------------------
// CMceNatStateConnected::Accept
// -----------------------------------------------------------------------------
//

TBool CMceNatStateConnected::Accept( TMceNatStateTransitionEvent& aEvent )
    {
    switch ( aEvent.NatCode() )
        {
        case EMceNatUpdated:
            {
            return aEvent.Session().DoesMediaManagerNeedToNegotiate( NULL );
            }
        case EMceNatHandleSIP:
            {
            return ( EMceInvite == aEvent.Code() || 
                     EMcePrack == aEvent.Code()  ||
            	  	 ( EMceAck == aEvent.Code() &&
            	  	   MceSip::HasContent( aEvent.Session().Request() ) ) );
           	}
           	
        case EMceNatUpdateSdp:
        case EMceNatError:
        case EMceNatICMPError: 
            {
            return ETrue;
            }
        default:
            {
            return EFalse;
            }
        }
    }    

// -----------------------------------------------------------------------------
// CMceNatStateConnected::EntryL
// -----------------------------------------------------------------------------
//
void CMceNatStateConnected::EntryL( TMceNatStateTransitionEvent& aEvent )
	{
	MCESRV_DEBUG( "*** CMceNatStateConnected::EntryL, Entry" );
	
	if ( EMceNatSync == aEvent.NatCode() ||
	     EMceNatOffer == aEvent.NatCode() )
	    {
	    // NOP
	    }
    else
        {
	    aEvent.NatActions().Continue();
	    }
    
    MCESRV_DEBUG( "*** CMceNatStateConnected::EntryL, Exit" );
	}

// -----------------------------------------------------------------------------
// CMceNatStateConnected::ExitL
// -----------------------------------------------------------------------------
//
void CMceNatStateConnected::ExitL( TMceNatStateTransitionEvent& aEvent )
	{
	MCESRV_DEBUG( "*** CMceNatStateConnected::ExitL, Entry" );
	
	switch ( aEvent.NatCode() )
	    {
	    case EMceNatError:
	    case EMceNatICMPError:
	        {
	        ExitErrorL( aEvent );
	        break;
	        }
	    case EMceNatUpdateSdp:
	        {
	        ExitUpdateSdpL( aEvent );
	        break;
	        }

	  	case EMceNatHandleSIP:
	        {
	        if ( EMceInvite == aEvent.Code() )
	        	{
		        ExitHandleInviteL( aEvent );
	        	}
			else
				{
	        	ExitDefaultL( aEvent );
				}	        	
	        break;
	        }
	    	
	    default:
	        {
	        ExitDefaultL( aEvent );
	        break;
	        }
	    }
    
	MCESRV_DEBUG( "*** CMceNatStateConnected::ExitL, Exit" );
	}

// -----------------------------------------------------------------------------
// CMceNatStateConnected::ExitErrorL
// -----------------------------------------------------------------------------
//
void CMceNatStateConnected::ExitErrorL( TMceNatStateTransitionEvent& aEvent )
    {
    TMceStateIndex sipState = aEvent.Session().CurrentState().Id();
    
    if (( aEvent.NatCode() == EMceNatICMPError && 
    		KMceStateEstablished == sipState ) || 
    		aEvent.NatCode() == EMceNatError )
    
    	{
    	aEvent.Session().Actions().ClientErrorOccured( aEvent.ParamStatus() );
    	User::Leave( aEvent.ParamStatus() );		
    	}
      
    }

// -----------------------------------------------------------------------------
// CMceNatStateConnected::ExitUpdateSdpL
// -----------------------------------------------------------------------------
//
void CMceNatStateConnected::ExitUpdateSdpL( 
    TMceNatStateTransitionEvent& /*aEvent*/ )
    {
    // NAT has resulted in different transport address than initial O/A 
    // exchange suggested, hence re-INVITE must be sent.
    
    //CMceSipSession& session = aEvent.Session();
    //CSdpDocument* previous = session.Offer();
    //aEvent.Session().SetOffer( aEvent.Document() );
    //aEvent.NatActions().SetOffered( KMceNatOfferSdp );
    //aEvent.NatActions().SdpCleanup( previous, session.Offer() );
    //aEvent.Session().Actions().SendInviteL();
    }

// -----------------------------------------------------------------------------
// CMceNatStateConnected::ExitHandleInviteL
// -----------------------------------------------------------------------------
//
void CMceNatStateConnected::ExitHandleInviteL( 
    TMceNatStateTransitionEvent& aEvent )
    {
    if ( !MceSip::HasContent( aEvent.Session().Request() ) )
        {
        // TBD: Restart of ICE
        }
    
    // Not consumed, done by SIP state machine
    aEvent.NatActions().Continue();        
    }

// -----------------------------------------------------------------------------
// CMceNatStateConnected::ExitDefaultL
// -----------------------------------------------------------------------------
//
void CMceNatStateConnected::ExitDefaultL( TMceNatStateTransitionEvent& aEvent )
    {
  	
  	TMceStateIndex sipState = aEvent.Session().CurrentState().Id();
  
    if ( KMceStateServerEstablishing == sipState || 
        KMceStateUpdating == sipState || 
       	( aEvent.NatCode() == EMceNatUpdated && 
       	KMceStateAcknowledgementRequired == sipState )) //i.e. DecodeL
    	{
    	// MT state loop
    	//Check if the invite package received includes corresponding SDP;
    	if ( !MceSip::HasContent( aEvent.Session().Request() ) )
    		{
    		//no SDP, so certify to Pull mode Re-INVITE 
    		aEvent.NatActions().StateChangedL( aEvent, KMceNatStateCreateOffer );
    		}
    	else
    		{
    		//including SDP, so certify to normal INVITE request
    		aEvent.NatActions().StateChangedL( aEvent, KMceNatStateCreateAnswer  );
    		}
    	}
    
    else if ( aEvent.NatCode() == EMceNatUpdated && 
		 	  ( KMceStateEstablished == sipState ||
     	  	    KMceStateConfirming == sipState ||
    	  	    KMceStateClientEstablishing == sipState ) ||
    	  	 ( aEvent.ParamStatus() == KMceReady ) &&
    	  	 ( aEvent.Code() == EMceProvisionalResponse ) )
          
		//i.e. UpdateL	
		{
        // MO state loop
        aEvent.NatActions().StateChangedL( aEvent, KMceNatStateCreateOffer );
		}

	else
		{
		aEvent.NatActions().Continue();        	
		}		
    }

// End of File
