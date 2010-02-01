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



#include "mcenatstatecreateofferrequested.h"
#include "mcenatevents.h"
#include "mcenatactions.h"
#include "mcesipsession.h"
#include "mcesrvlogs.h"

// -----------------------------------------------------------------------------
// CMceNatStateCreateOfferRequested::CMceNatStateCreateOfferRequested
// -----------------------------------------------------------------------------
//
CMceNatStateCreateOfferRequested::CMceNatStateCreateOfferRequested()
	{
	}

// -----------------------------------------------------------------------------
// CMceNatStateCreateOfferRequested::~CMceNatStateCreateOfferRequested
// -----------------------------------------------------------------------------
//
CMceNatStateCreateOfferRequested::~CMceNatStateCreateOfferRequested()
	{
	}

// -----------------------------------------------------------------------------
// CMceNatStateCreateOfferRequested::Accept
// EMceNatUpdated and EMceNatInitialized can be received in any order.
// -----------------------------------------------------------------------------
//
TBool
CMceNatStateCreateOfferRequested::Accept( TMceNatStateTransitionEvent& aEvent )
    {    
    return aEvent.NatCode() == EMceNatInitialized ||
    	   aEvent.NatCode() == EMceNatUpdated;
    }

// -----------------------------------------------------------------------------
// CMceNatStateCreateOfferRequested::EntryL
// -----------------------------------------------------------------------------
//
void CMceNatStateCreateOfferRequested::EntryL(
	TMceNatStateTransitionEvent& aEvent )
	{
	aEvent.ParamStatus() = KMceAsync;
	}

// -----------------------------------------------------------------------------
// CMceNatStateCreateOfferRequested::ExitL
// EMceNatUpdated and EMceNatInitialized can be received in any order
// -----------------------------------------------------------------------------
//
void
CMceNatStateCreateOfferRequested::ExitL( TMceNatStateTransitionEvent& aEvent )
	{
	MCESRV_DEBUG( "*** CMceNatStateCreateOfferRequested::ExitL, Entry" );

	aEvent.NatActions().StateChangedL( aEvent, KMceNatStateIdle );

	if ( CMceCsSubSession::EInSession != aEvent.Session().Type() )
		{
		aEvent.NatActions().ResumeNatUpdated();
		}

	MCESRV_DEBUG( "*** CMceNatStateCreateOfferRequested::ExitL, Exit" );
	}
