/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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




#include "mcenatstateinitializing.h"
#include "mcenatevents.h"
#include "mcenatactions.h"
#include "mcesrvlogs.h"

#include "mcenatsipsession.h"
#include "mceactionset.h"
#include "mceclientserver.h"

// -----------------------------------------------------------------------------
// CMceNatStateInitializing::CMceNatStateInitializing
// -----------------------------------------------------------------------------
//
CMceNatStateInitializing::CMceNatStateInitializing()
	{
	}


// -----------------------------------------------------------------------------
// CMceNatStateInitializing::~CMceNatStateInitializing
// -----------------------------------------------------------------------------
//
CMceNatStateInitializing::~CMceNatStateInitializing()
	{
	}

// -----------------------------------------------------------------------------
// CMceNatStateInitializing::Accept
// -----------------------------------------------------------------------------
//
TBool CMceNatStateInitializing::Accept( TMceNatStateTransitionEvent& aEvent )
    {
	switch ( aEvent.NatCode() )
		{
		case EMceNatInitialized:
		case EMceNatUpdated:
        	return ETrue;

		default:
        	return EFalse;
        }
    }

// -----------------------------------------------------------------------------
// CMceNatStateInitializing::EntryL
// -----------------------------------------------------------------------------
//
void CMceNatStateInitializing::EntryL( TMceNatStateTransitionEvent& /*aEvent*/ )
	{
	}	

// -----------------------------------------------------------------------------
// CMceNatStateInitializing::ExitL
// When either EMceNatInitialized or EMceNatUpdated is received, enter state
// KMceNatStateCreateOfferRequested, and wait there for the other event, before
// entering KMceNatStateIdle.
// -----------------------------------------------------------------------------
//
void CMceNatStateInitializing::ExitL( TMceNatStateTransitionEvent& aEvent )
	{
	MCESRV_DEBUG( "*** CMceNatStateInitializing::ExitL, Entry" );

#if 0
	TMceNatStateIndex nextState( KMceNatStateIdle );
	
	if ( aEvent.NatCode() == EMceNatUpdated )
		{
		// Media is now ready, but must wait for NAT plugin to get initialized.
		nextState = KMceNatStateCreateOfferRequested;
		}
	aEvent.NatActions().StateChangedL( aEvent, nextState );

#else

	if ( ( CMceComSession::TType(aEvent.Session().Type()) == CMceComSession::EInSession ) 
	       && ( &aEvent.Session().Request()) 
	       && ( !MceSip::HasContent( aEvent.Session().Request() )))
		{
		aEvent.NatActions().StateChangedL( aEvent,
									   KMceNatStateIdle );					
		}
		
	else 
		{
		
		aEvent.NatActions().StateChangedL( aEvent,
							   KMceNatStateCreateOfferRequested );														   
		}
		
#endif

	MCESRV_DEBUG( "*** CMceNatStateInitializing::ExitL, Exit" );
	}
