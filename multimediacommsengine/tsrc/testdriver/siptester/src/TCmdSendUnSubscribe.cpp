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
#include <sipmessageelements.h>
#include <sipsubscribedialogassoc.h>

#include "TCmdSendUnsubscribe.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		Event*, Content-Type*, Content-Encoding*
 *   Parameters:	Content*
 *   IDs:			SubscribeDialogId
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			TransactionIdd
 */
void TCmdSendUnsubscribe::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get SIP objects from registry
	CSIPSubscribeDialogAssoc* dialogAssoc = GetSubscribeDialogAssocL();

	// Extract both headers (that are still left) and content.
	CSIPMessageElements* elements = ExtractHeadersAndContentLC();

	// -- Execution -----------------------------------------------------------

	// Start SIP Register transaction.
	CSIPClientTransaction* transaction =
									dialogAssoc->SendUnsubscribeL( elements );
	CleanupStack::Pop( elements );

	// -- Response creation ---------------------------------------------------

	AddIdResponseL( KTransactionId, transaction );
	}

TBool TCmdSendUnsubscribe::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("SendUnsubscribe") );
	}

TTcCommandBase* TCmdSendUnsubscribe::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendUnsubscribe( aContext );
	}

