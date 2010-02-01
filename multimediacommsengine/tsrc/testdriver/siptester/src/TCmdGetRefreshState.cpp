/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation
*
*/

#include <sipregistrationbinding.h>
#include <siprefresh.h>
#include <sipsubscribedialogassoc.h>

#include "TCmdGetRefreshState.h"
#include "SIPConstants.h"

// State enumeration names
_LIT8( KInactive, "inactive" );
_LIT8( KActive, "active" );
_LIT8( KTerminated, "terminated" );

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	-
 *   IDs:			RefreshId*, RegistrationId*, SubscribeDialogId*
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			RefreshState
 */
void TCmdGetRefreshState::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get SIP objects from registry
	CSIPRefresh* refresh = GetRefreshL( EFalse );
	CSIPRegistrationBinding* registration = GetRegistrationL( EFalse );
	CSIPSubscribeDialogAssoc* dialogAssoc = GetSubscribeDialogAssocL( EFalse );

	// Get the refresh object from a registration or from a dialog association
	// if it's  not defined as such.
	if( !refresh )
		{
		if( registration )
			{
			refresh = const_cast< CSIPRefresh* >( registration->SIPRefresh() );
			}
		else if( dialogAssoc )
			{
			refresh = const_cast< CSIPRefresh* >( dialogAssoc->SIPRefresh() );
			}
		else
			{
			User::Leave( KTcErrMandatoryIdNotFound );
			}
		}

	// -- Execution -----------------------------------------------------------

	// Report and terminate if no refresh was found
	if( !refresh )
		{
		User::Leave( KErrNotFound );
		}

	// Fetch refresh state
	CSIPRefresh::TState state = refresh->State();

	// -- Response creation ---------------------------------------------------

	// Map enumeration to text
	TPtrC8 stateName;
	switch( state )
		{
		case CSIPRefresh::EActive:
			{
			stateName.Set( KActive );
			break;
			}
		case CSIPRefresh::ETerminated:
			{
			stateName.Set( KTerminated );
			break;
			}
		case CSIPRefresh::EInactive:
		default:
			{
			stateName.Set( KInactive );
			break;
			}
		}

	AddTextResponseL( KParamRefreshState, stateName );
	}

TBool TCmdGetRefreshState::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("GetRefreshState") );
	}

TTcCommandBase* TCmdGetRefreshState::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetRefreshState( aContext );
	}

