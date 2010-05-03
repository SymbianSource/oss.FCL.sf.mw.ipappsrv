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



#include "mcenatstatewait.h"
#include "mcenatevents.h"
#include "mcenatsipsession.h"
#include "mceactionset.h"
#include "mceclientserver.h"
#include "mcesrvlogs.h"

// -----------------------------------------------------------------------------
// CMceNatStateWait::CMceNatStateWait
// -----------------------------------------------------------------------------
//
CMceNatStateWait::CMceNatStateWait()
	{
	}


// -----------------------------------------------------------------------------
// CMceNatStateWait::~CMceNatStateWait
// -----------------------------------------------------------------------------
//
CMceNatStateWait::~CMceNatStateWait()
	{
	}

// -----------------------------------------------------------------------------
// CMceNatStateWait::Accept
// -----------------------------------------------------------------------------
//
TBool CMceNatStateWait::Accept( TMceNatStateTransitionEvent& aEvent )
    {
	if ( ( EMceNatHandleSIP == aEvent.NatCode() && 
	       ( EMceResponse == aEvent.Code() ||
	         EMceProvisionalResponse == aEvent.Code() ) &&
	     	 MceSip::HasContent( aEvent.Session().Response() ) ) ||
	     	 EMceNatError == aEvent.NatCode() ||
	     	 EMceNatUpdated == aEvent.NatCode() ||
	         ( EMceAck == aEvent.Code() &&  
            	    	MceSip::HasContent( aEvent.Session().Request())) ||
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
// CMceNatStateWait::EntryL
// -----------------------------------------------------------------------------
//
void CMceNatStateWait::EntryL( TMceNatStateTransitionEvent& aEvent )
	{
	MCESRV_DEBUG( "*** CMceNatStateWait::EntryL, Entry" );
	
	if ( EMceNatSync == aEvent.NatCode() )
	    {
	    // NOP
	    }
	else
	    {
	    aEvent.NatActions().Continue();
	    }
    
	MCESRV_DEBUG( "*** CMceNatStateWait::EntryL, Exit" );
	}

// -----------------------------------------------------------------------------
// CMceNatStateWait::ExitL
// -----------------------------------------------------------------------------
//
void CMceNatStateWait::ExitL( TMceNatStateTransitionEvent& aEvent )
	{
	MCESRV_DEBUG( "*** CMceNatStateWait::ExitL, Entry" );
	
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

	MCESRV_DEBUG( "*** CMceNatStateWait::ExitL, Exit" );
	}

// -----------------------------------------------------------------------------
// CMceNatStateWait::ExitErrorL
// -----------------------------------------------------------------------------
//
void CMceNatStateWait::ExitErrorL( TMceNatStateTransitionEvent& aEvent )
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
// CMceNatStateWait::ExitDefaultL
// -----------------------------------------------------------------------------
//
void CMceNatStateWait::ExitDefaultL( TMceNatStateTransitionEvent& aEvent )
    {
    CMceSipSession& session = aEvent.Session();
    TMceStateIndex sipState = aEvent.Session().CurrentState().Id();
    
    if ( EMceAck == aEvent.Code() &&  
          MceSip::HasContent( aEvent.Session().Request()))
    	{
    	User::LeaveIfError( session.Actions().CreateSDP( session.Request() ) );
        aEvent.NatActions().UpdateL();
    	aEvent.NatActions().StateChangedL( aEvent, KMceNatStateDecodeAnswer );		
    	}
    else if( EMceNatUpdated == aEvent.NatCode() && 
    		 KMceStateOffering == sipState )                //for prack
    	{
		aEvent.NatActions().StateChangedL( aEvent, KMceNatStateCreateOffer );	
    	}
    else
    	{
    	User::LeaveIfError( session.Actions().CreateSDP( session.Response() ) );
    	aEvent.NatActions().StateChangedL( aEvent, KMceNatStateDecodeAnswer );		
    	}
             
    }



// End of File

