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



#include "mcenatstatecreateoffer.h"
#include "mcenatsipsession.h"
#include "mceactionset.h"
#include "mceclientserver.h"
#include "mcenatevents.h"
#include "mcesrvlogs.h"

// -----------------------------------------------------------------------------
// CMceNatStateCreateOffer::CMceNatStateCreateOffer
// -----------------------------------------------------------------------------
//
CMceNatStateCreateOffer::CMceNatStateCreateOffer()
	{
	}

// -----------------------------------------------------------------------------
// CMceNatStateCreateOffer::~CMceNatStateCreateOffer
// -----------------------------------------------------------------------------
//
CMceNatStateCreateOffer::~CMceNatStateCreateOffer()
	{
	}

// -----------------------------------------------------------------------------
// CMceNatStateCreateOffer::Accept
// -----------------------------------------------------------------------------
//
TBool CMceNatStateCreateOffer::Accept( TMceNatStateTransitionEvent& aEvent )
    {
    if ( EMceNatOffer == aEvent.NatCode() ||
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
// CMceNatStateCreateOffer::EntryL
// -----------------------------------------------------------------------------
//
void CMceNatStateCreateOffer::EntryL( TMceNatStateTransitionEvent& aEvent )
	{
	MCESRV_DEBUG( "*** CMceNatStateCreateOffer::EntryL, Entry" );
        		
	aEvent.Session().Actions().EncodeL();
	TMceReturnStatus status = aEvent.NatActions().CreateOfferL();
	
	   
	if ( KMceReady == status )
	    {
        aEvent.NatCode() = ( KMceReady == aEvent.ParamStatus() ?
                EMceNatSync : aEvent.NatCode() );
	    aEvent.NatActions().StateChangedL( aEvent, KMceNatStateWait );
	    }
    else
        {
        aEvent.ParamStatus() = status;
        }
	
	MCESRV_DEBUG( "*** CMceNatStateCreateOffer::EntryL, Exit" );
	}

// -----------------------------------------------------------------------------
// CMceNatStateCreateOffer::ExitL
// -----------------------------------------------------------------------------
//
void CMceNatStateCreateOffer::ExitL( TMceNatStateTransitionEvent& aEvent )
	{
	MCESRV_DEBUG( "*** CMceNatStateCreateOffer::ExitL, Entry" );
    
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
    
	MCESRV_DEBUG( "*** CMceNatStateCreateOffer::ExitL, Exit" );
	}

// -----------------------------------------------------------------------------
// CMceNatStateCreateOffer::ExitErrorL
// -----------------------------------------------------------------------------
//
void CMceNatStateCreateOffer::ExitErrorL( TMceNatStateTransitionEvent& aEvent )
    {
    if ( aEvent.ParamStatus() > KMceNatNonFatalErrorLimit )
        {
        aEvent.Session().Actions().ClientErrorOccured( aEvent.ParamStatus() );
        }
        
	TMceStateIndex sipState = aEvent.Session().CurrentState().Id();
    
    if (( aEvent.NatCode() == EMceNatICMPError && 
    		KMceStateEstablished == sipState ) || 
    		aEvent.NatCode() == EMceNatError )
    		
    	{
    	aEvent.NatActions().Continue();
    	User::Leave( aEvent.ParamStatus() );		
    	}
       
    }

// -----------------------------------------------------------------------------
// CMceNatStateCreateOffer::ExitDefaultL
// -----------------------------------------------------------------------------
//
void CMceNatStateCreateOffer::ExitDefaultL( TMceNatStateTransitionEvent& aEvent )
    {
    User::LeaveIfNull( aEvent.Document() );
    CMceSipSession& session = aEvent.Session();
    
    CSdpDocument* previous = session.Offer();
    aEvent.Session().SetOffer( aEvent.Document() );
    aEvent.NatActions().SetOffered( KMceNatOfferSdp );
    aEvent.NatActions().SdpCleanup( previous, session.Offer() );
    
    aEvent.NatActions().StateChangedL( aEvent, KMceNatStateWait );
    }
