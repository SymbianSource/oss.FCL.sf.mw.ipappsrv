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

#include "TCmdGetTransactionType.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	-
 *   IDs:			ServerTransactionId, TransactionId
 *
 * OUTPUT:
 *   Parameters:	TransactionType
 *   IDs:			-
 */
void TCmdGetTransactionType::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get SIP objects from registry
	CSIPTransactionBase* transaction = GetClientTransactionL( EFalse );
	if( !transaction )
		{
		transaction = GetServerTransactionL();
		}

	// -- Execution -----------------------------------------------------------

	// -- Response creation ---------------------------------------------------
	
	AddTextResponseL( KParamTransactionType, transaction->Type().DesC() );
	}

TBool TCmdGetTransactionType::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("GetTransactionType") );
	}

TTcCommandBase* TCmdGetTransactionType::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetTransactionType( aContext );
	}

