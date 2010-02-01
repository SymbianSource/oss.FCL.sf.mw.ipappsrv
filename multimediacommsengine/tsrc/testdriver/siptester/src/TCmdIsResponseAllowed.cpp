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

#include <sipservertransaction.h>

#include "TCmdIsResponseAllowed.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	-
 *   IDs:			ServerTransactionId
 *
 * OUTPUT:
 *   Parameters:	Boolean
 *   IDs:			-
 */
void TCmdIsResponseAllowed::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get SIP objects from registry
	CSIPServerTransaction* transaction = GetServerTransactionL();

	// -- Execution -----------------------------------------------------------

	TBool allowed = transaction->ResponseAllowed();

	// -- Response creation ---------------------------------------------------

	AddBooleanResponseL( KParamBoolean, allowed );
	}

TBool TCmdIsResponseAllowed::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("IsResponseAllowed") );
	}

TTcCommandBase* TCmdIsResponseAllowed::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdIsResponseAllowed( aContext );
	}

