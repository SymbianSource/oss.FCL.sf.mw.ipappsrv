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

#include <sipclienttransaction.h>
#include <sipservertransaction.h>
#include <sipinternalstates.h>

#include "TCmdGetTransactionState.h"
#include "SIPConstants.h"

// State enumeration names
_LIT8( KTrying, "trying" );
_LIT8( KCalling, "calling" );
_LIT8( KProceeding, "proceeding" );
_LIT8( KCompleted, "completed" );
_LIT8( KConfirmed, "confirmed" );
_LIT8( KTerminated, "terminated" );

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	-
 *   IDs:			ServerTransactionId, TransactionId
 *
 * OUTPUT:
 *   Parameters:	TransactionState
 *   IDs:			-
 */
void TCmdGetTransactionState::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get SIP objects from registry
	CSIPTransactionBase* transaction = GetClientTransactionL( EFalse );
	if( !transaction )
		{
		transaction = GetServerTransactionL();
		}

	// -- Execution -----------------------------------------------------------

	CSIPInternalStates* internalStates = CSIPInternalStates::NewLC();
	CSIPInternalStates::TState state;
	User::LeaveIfError( internalStates->GetTransactionState(
													*transaction, state ) );
	CleanupStack::PopAndDestroy( internalStates );

	// -- Response creation ---------------------------------------------------

	// Map enumeration to text
	TPtrC8 stateName;
	switch( state )
		{
		case CSIPInternalStates::ETransactionTrying:
			{
			stateName.Set( KTrying );
			break;
			}
		case CSIPInternalStates::ETransactionCalling:
			{
			stateName.Set( KCalling );
			break;
			}
		case CSIPInternalStates::ETransactionProceeding:
			{
			stateName.Set( KProceeding );
			break;
			}
		case CSIPInternalStates::ETransactionCompleted:
			{
			stateName.Set( KCompleted );
			break;
			}
		case CSIPInternalStates::ETransactionConfirmed:
			{
			stateName.Set( KConfirmed );
			break;
			}
		case CSIPInternalStates::ETransactionTerminated:
		default:
			{
			stateName.Set( KTerminated );
			break;
			}
		}

	AddTextResponseL( KParamTransactionState, stateName );
	}

TBool TCmdGetTransactionState::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("GetTransactionState") );
	}

TTcCommandBase* TCmdGetTransactionState::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetTransactionState( aContext );
	}

