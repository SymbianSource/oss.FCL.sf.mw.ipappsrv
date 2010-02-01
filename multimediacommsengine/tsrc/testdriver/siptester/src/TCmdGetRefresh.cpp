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

#include "CTcSIPContext.h"
#include "SIPConstants.h"
#include "TCmdGetRefresh.h"

#include <sipclienttransaction.h>
#include <siprefresh.h>

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	-
 *   IDs:			TransactionId
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			RefreshId
 */
void TCmdGetRefresh::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get SIP objects from registry
	const CSIPClientTransaction* transaction = GetClientTransactionL();

	// -- Execution -----------------------------------------------------------

	// Get refresh for the client transaction
	const CSIPRefresh* refresh = transaction->Refresh();
	if( !refresh )
		{
		// Report if it was not found
		User::Leave( KErrNotFound );
		}

	// Ownership is not transferred, specify by-reference
	iContext.Registry().AddObjectL( *refresh );

	// -- Response creation ---------------------------------------------------

	AddIdResponseL( KRefreshId, refresh );
	}

TBool TCmdGetRefresh::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("GetRefresh") );
	}

TTcCommandBase* TCmdGetRefresh::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetRefresh( aContext );
	}
