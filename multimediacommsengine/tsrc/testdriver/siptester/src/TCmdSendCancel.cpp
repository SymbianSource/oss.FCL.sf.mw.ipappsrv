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

#include "TCmdSendCancel.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	-
 *   IDs:			TransactionId
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			TransactionId
 */
void TCmdSendCancel::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get SIP objects from registry
	CSIPClientTransaction* transaction = GetClientTransactionL();

	// -- Execution -----------------------------------------------------------

	// Start SIP Bye transaction
	CSIPClientTransaction* newTransaction = transaction->CancelL();

	// -- Response creation ---------------------------------------------------

	AddIdResponseL( KTransactionId, newTransaction );
	}

TBool TCmdSendCancel::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("SendCancel") );
	}

TTcCommandBase* TCmdSendCancel::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendCancel( aContext );
	}

