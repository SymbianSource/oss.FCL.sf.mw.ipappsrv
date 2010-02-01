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





#include "mcestateerror.h"
#include "mcesipsession.h"
#include "mceactionset.h"
#include "mceclientserver.h"
#include "mcesrvlogs.h"


// -----------------------------------------------------------------------------
// CMceStateError::CMceStateError
// -----------------------------------------------------------------------------
//
CMceStateError::CMceStateError ()
    : CMceState( KMceStateError )
	{
	}


// -----------------------------------------------------------------------------
// CMceStateError::~CMceStateError
// -----------------------------------------------------------------------------
//
CMceStateError::~CMceStateError()
	{
	}


// -----------------------------------------------------------------------------
// CMceStateError::DoAcceptL
// -----------------------------------------------------------------------------
//
TBool CMceStateError::DoAcceptL( TMceStateTransitionEvent& aEvent )
	{
    MCESRV_DEBUG("CMceStateError::DoAcceptL, Entry");
    TUint& eventCode = aEvent.Code();
    MCESRV_DEBUG_EVENT("EVENT", eventCode );
	
	User::LeaveIfError( aEvent.EventSource() == EMceClient ? 
                        KErrTotalLossOfPrecision :
                        KErrNone );

    MCESRV_DEBUG("CMceStateError::DoAcceptL, Exit");
	return ETrue;
	}

// -----------------------------------------------------------------------------
// CMceStateError::EntryL
// -----------------------------------------------------------------------------
//
void CMceStateError::EntryL( TMceStateTransitionEvent& /*aEvent*/ )
	{
    MCESRV_DEBUG("CMceStateError::EntryL, Entry");
    MCESRV_DEBUG("CMceStateError::EntryL, Exit");
    }

// -----------------------------------------------------------------------------
// CMceStateError::ExitL
// -----------------------------------------------------------------------------
//
void CMceStateError::ExitL( TMceStateTransitionEvent& /*aEvent*/ )
	{
    
	
	}

