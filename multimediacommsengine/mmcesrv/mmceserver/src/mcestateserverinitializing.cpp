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




#include "mcestateserverinitializing.h"
#include "mcedefs.h"
#include "mcesipsession.h"
#include "mceactionset.h"
#include "mcefcactionset.h"


// -----------------------------------------------------------------------------
// CMceStateServerInitializing::CMceStateServerInitializing
// -----------------------------------------------------------------------------
//
CMceStateServerInitializing::CMceStateServerInitializing()
    : CMceState( KMceStateServerInitializing )
	{
	}

// -----------------------------------------------------------------------------
// CMceStateServerInitializing::~CMceStateServerInitializing
// -----------------------------------------------------------------------------
//
CMceStateServerInitializing::~CMceStateServerInitializing()
	{
	}

// -----------------------------------------------------------------------------
// CMceStateServerInitializing::DoAcceptL
// -----------------------------------------------------------------------------
//
TBool
CMceStateServerInitializing::DoAcceptL( TMceStateTransitionEvent& aEvent )
	{
	return aEvent.Code() == EMceServerInitialized;
	}

// -----------------------------------------------------------------------------
// CMceStateServerInitializing::EntryL
// -----------------------------------------------------------------------------
//
void CMceStateServerInitializing::EntryL( TMceStateTransitionEvent& aEvent )
	{
	aEvent.Session().ContinueHandlingIncomingSessionL();
	}

// -----------------------------------------------------------------------------
// CMceStateServerInitializing::ExitL
// -----------------------------------------------------------------------------
//
void CMceStateServerInitializing::ExitL( TMceStateTransitionEvent& aEvent )
	{
	TMceStateIndex nextState( KMceStateTerminated );

	if ( aEvent.ParamStatus() == KErrNone )
        {
        nextState = KMceStateServerEstablishing;
        }

    aEvent.Session().Actions().StateChanged( nextState );
	}
