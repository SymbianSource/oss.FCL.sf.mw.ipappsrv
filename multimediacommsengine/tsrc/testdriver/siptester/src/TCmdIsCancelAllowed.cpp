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

#include "TCmdIsCancelAllowed.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	-
 *   IDs:			TransactionId
 *
 * OUTPUT:
 *   Parameters:	Boolean
 *   IDs:			-
 */
void TCmdIsCancelAllowed::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get SIP objects from registry
	CSIPClientTransaction* transaction = GetClientTransactionL();

	// -- Execution -----------------------------------------------------------

	TBool allowed = transaction->CancelAllowed();

	// -- Response creation ---------------------------------------------------

	AddBooleanResponseL( KParamBoolean, allowed );
	}

TBool TCmdIsCancelAllowed::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("IsCancelAllowed") );
	}

TTcCommandBase* TCmdIsCancelAllowed::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdIsCancelAllowed( aContext );
	}

