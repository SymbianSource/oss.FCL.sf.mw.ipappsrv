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





#include "mcestateterminated.h"
#include "mcesipsession.h"
#include "mceactionset.h"
#include "mceclientserver.h"


// -----------------------------------------------------------------------------
// CMceStateTerminated::CMceStateTerminated
// -----------------------------------------------------------------------------
//
CMceStateTerminated::CMceStateTerminated ()
    : CMceState( KMceStateTerminated )
	{
	}


// -----------------------------------------------------------------------------
// CMceStateTerminated::~CMceStateTerminated
// -----------------------------------------------------------------------------
//
CMceStateTerminated::~CMceStateTerminated()
	{
	}


// -----------------------------------------------------------------------------
// CMceStateTerminated::DoAcceptL
// -----------------------------------------------------------------------------
//
TBool CMceStateTerminated::DoAcceptL( TMceStateTransitionEvent& aEvent )
	{
	User::LeaveIfError( aEvent.EventSource() == EMceClient ? 
                        KErrTotalLossOfPrecision :
                        KErrNone );
	
	return ETrue;
	}

// -----------------------------------------------------------------------------
// CMceStateTerminated::EntryL
// -----------------------------------------------------------------------------
//
void CMceStateTerminated::EntryL( TMceStateTransitionEvent& /*aEvent*/ )
	{
         
	}

// -----------------------------------------------------------------------------
// CMceStateTerminated::ExitL
// -----------------------------------------------------------------------------
//
void CMceStateTerminated::ExitL( TMceStateTransitionEvent& /*aEvent*/ )
	{
	
	}

