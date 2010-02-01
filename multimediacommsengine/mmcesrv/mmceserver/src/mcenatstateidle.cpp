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



#include "mcenatstateidle.h"
#include "mcenatsipsession.h"
#include "mceactionset.h"
#include "mceclientserver.h"
#include "mcenatevents.h"
#include "mcenatactions.h"
#include "mcesrvlogs.h"
#include "mcestate.h"

// -----------------------------------------------------------------------------
// CMceNatStateIdle::CMceNatStateIdle
// -----------------------------------------------------------------------------
//
CMceNatStateIdle::CMceNatStateIdle()
	{
	}


// -----------------------------------------------------------------------------
// CMceNatStateIdle::~CMceNatStateIdle
// -----------------------------------------------------------------------------
//
CMceNatStateIdle::~CMceNatStateIdle()
	{
	}

// -----------------------------------------------------------------------------
// CMceNatStateIdle::Accept
// -----------------------------------------------------------------------------
//
TBool CMceNatStateIdle::Accept( TMceNatStateTransitionEvent& aEvent )
    {
    if ( EMceNatUpdated == aEvent.NatCode() ||
         EMceNatError == aEvent.NatCode() ||
         EMceNatICMPError == aEvent.NatCode())
        {
        return ETrue;
        }
    else
        {
        return EFalse;
        }
    }

// -----------------------------------------------------------------------------
// CMceNatStateIdle::EntryL
// -----------------------------------------------------------------------------
//
void CMceNatStateIdle::EntryL( TMceNatStateTransitionEvent& aEvent )
	{
	if ( CMceCsSubSession::EInSession == aEvent.Session().Type() &&
			( &aEvent.Session().Request()) && 
			MceSip::HasContent( aEvent.Session().Request()  ))
		{
		aEvent.Session().ServerInitializedL();
		}
	}

// -----------------------------------------------------------------------------
// CMceNatStateIdle::ExitL
// -----------------------------------------------------------------------------
//
void CMceNatStateIdle::ExitL( TMceNatStateTransitionEvent& aEvent )
	{
	MCESRV_DEBUG( "*** CMceNatStateIdle::ExitL, Entry" );
	
	switch ( aEvent.NatCode() )
	    {
	    case EMceNatError:
	    case EMceNatICMPError:
	        {
	        ExitErrorL( aEvent );
	        break;
	        }
	    default:
	        {
	        ExitDefaultL( aEvent );
	        break;
	        }
	    }
    
	MCESRV_DEBUG( "*** CMceNatStateIdle::ExitL, Exit" );
	}

// -----------------------------------------------------------------------------
// CMceNatStateIdle::ExitErrorL
// -----------------------------------------------------------------------------
//
void CMceNatStateIdle::ExitErrorL( TMceNatStateTransitionEvent& aEvent )
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
// CMceNatStateIdle::ExitDefaultL
// -----------------------------------------------------------------------------
//
void CMceNatStateIdle::ExitDefaultL( TMceNatStateTransitionEvent& aEvent )
    {       
    if ( CMceCsSubSession::EInSession == aEvent.Session().Type() )
        {
        if ( &aEvent.Session().Request() && 
        		!MceSip::HasContent( aEvent.Session().Request() ))
        	{
    		aEvent.NatActions().SetOffered( KMceNatOfferSdp );
    		aEvent.NatActions().StateChangedL( aEvent, KMceNatStateCreateOffer );				
        	}
        else
        	{
        	aEvent.NatActions().SetOffered( KMceNatOfferSdp );
        	aEvent.NatActions().StateChangedL( aEvent, KMceNatStateCreateAnswer );		
        	}
        
        }
    else if ( CMceCsSubSession::EOutSession == aEvent.Session().Type() )
        {
        aEvent.NatActions().SetOffered( KMceNatOfferSdp );
        aEvent.NatActions().StateChangedL( aEvent, KMceNatStateCreateOffer );
        }
    else
        {
        User::Leave( KErrArgument );
        }
    }

