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

#include <sipdialog.h>
#include <sipdialogassocbase.h>
#include <sipinternalstates.h>

#include "TCmdGetDialogState.h"
#include "SIPConstants.h"

// State enumeration names
_LIT8( KInit, "init" );
_LIT8( KEarly, "early" );
_LIT8( KConfirmed, "confirmed" );
_LIT8( KTerminated, "terminated" );

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	-
 *   IDs:			InviteDialogId*, SubscribeDialogId*
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			DialogState
 */
void TCmdGetDialogState::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get SIP objects from registry
	CSIPDialogAssocBase* dialogAssoc = GetAnyDialogAssocL();

	// -- Execution -----------------------------------------------------------

	// Get dialog state
	CSIPInternalStates* internalStates = CSIPInternalStates::NewLC();
	CSIPInternalStates::TState state;
	User::LeaveIfError( internalStates->GetDialogState(
											dialogAssoc->Dialog(), state ) );
	CleanupStack::PopAndDestroy( internalStates );

	// -- Response creation ---------------------------------------------------

	// Map enumeration to text
	TPtrC8 stateName;
	switch( state )
		{
		case CSIPInternalStates::EDialogEarly:
			{
			stateName.Set( KEarly );
			break;
			}
		case CSIPInternalStates::EDialogConfirmed:
			{
			stateName.Set( KConfirmed );
			break;
			}
		case CSIPInternalStates::EDialogTerminated:
			{
			stateName.Set( KTerminated );
			break;
			}
		case CSIPInternalStates::EDialogEstablishing:
		default:
			{
			stateName.Set( KInit );
			break;
			}
		}

	AddTextResponseL( KParamDialogState, stateName );
	}

TBool TCmdGetDialogState::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("GetDialogState") );
	}

TTcCommandBase* TCmdGetDialogState::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetDialogState( aContext );
	}
