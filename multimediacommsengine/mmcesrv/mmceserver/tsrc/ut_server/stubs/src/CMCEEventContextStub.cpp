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
* Description:    Base class for event contexts. Contains methods for
*                creating the state array and controlling current state.
*
*/




#include "cmcecomevent.h"
#include "cmceeventcontext.h"

// -----------------------------------------------------------------------------
// CMCEMoEventSubscribeContext::NewL
// -----------------------------------------------------------------------------
//
CMceEventContext* CMceEventContext::NewL( CMceComEvent& /*aEvent*/ )
	{
	CMceEventContext* self = new( ELeave ) CMceEventContext;
    return self;
	}

// -----------------------------------------------------------------------------
// CMceEventContext::CMceEventContext
// -----------------------------------------------------------------------------
//
CMceEventContext::CMceEventContext():
    iCurrentState( KMceIdleEventStateIndex )
    {
    }
// -----------------------------------------------------------------------------
// CMceEventContext::~CMceEventContext
// -----------------------------------------------------------------------------
//
CMceEventContext::~CMceEventContext()
	{
	}
	
// -----------------------------------------------------------------------------
// CMceEventContext::CurrentState()
// -----------------------------------------------------------------------------
//
TMceEventStateIndex CMceEventContext::CurrentState()
	{
	return iCurrentState;
	}

// End of file

