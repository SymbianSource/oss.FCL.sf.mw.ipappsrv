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
#include <siprefresh.h>
#include <sipmessageelements.h>
#include <sipnotifydialogassoc.h>

#include "TCmdUpdateNotify.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		Contact*, Content-Type*, Content-Encoding*,
 *   Parameters:	Content*
*   IDs:			NotifyDialogId
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			TransactionId
 */
void TCmdUpdateNotify::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get SIP objects from registry
	CSIPNotifyDialogAssoc* dialogAssoc = GetNotifyDialogAssocL();

	// Extract both headers (that are still left) and content.
	CSIPMessageElements* elements = ExtractHeadersAndContentLC();

	// -- Execution -----------------------------------------------------------

	// Start SIP Subscribe transaction.
	CSIPClientTransaction* transaction =
							dialogAssoc->SendNotifyL( elements );
	CleanupStack::Pop( elements );

	// -- Response creation ---------------------------------------------------

	AddIdResponseL( KTransactionId, transaction );
	}

TBool TCmdUpdateNotify::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("UpdateNotify") );
	}

TTcCommandBase* TCmdUpdateNotify::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdUpdateNotify( aContext );
	}

