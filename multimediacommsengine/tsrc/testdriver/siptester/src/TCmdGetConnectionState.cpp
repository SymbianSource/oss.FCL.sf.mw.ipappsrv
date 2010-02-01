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

#include "CTcSIPConnectionContainer.h"

#include "TCmdGetConnectionState.h"
#include "SIPConstants.h"

// State enumeration names
_LIT8( KInit, "init" );
_LIT8( KActive, "active" );
_LIT8( KSuspended, "suspended" );
_LIT8( KInactive, "inactive" );
_LIT8( KUnavailable, "unavailable" );

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	-
 *   IDs:			ConnectionId*
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			ConnectionState
 */
void TCmdGetConnectionState::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Select connection; either default or user specified (and existing)
	CSIPConnection& connection = SelectConnectionL().Connection();

	// -- Execution -----------------------------------------------------------

	// Get connection state
	CSIPConnection::TState state = connection.State();

	// -- Response creation ---------------------------------------------------

	// Map enumeration to text
	TPtrC8 stateName;
	switch( state )
		{
		case CSIPConnection::EActive:
			{
			stateName.Set( KActive );
			break;
			}
		case CSIPConnection::ESuspended:
			{
			stateName.Set( KSuspended );
			break;
			}
		case CSIPConnection::EInactive:
			{
			stateName.Set( KInactive );
			break;
			}
		case CSIPConnection::EUnavailable:
			{
			stateName.Set( KUnavailable );
			break;
			}
		case CSIPConnection::EInit:
		default:
			{
			stateName.Set( KInit );
			break;
			}
		}

	AddTextResponseL( KParamConnectionState, stateName );
	}

TBool TCmdGetConnectionState::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("GetConnectionState") );
	}

TTcCommandBase* TCmdGetConnectionState::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetConnectionState( aContext );
	}
