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
#include <sipinvitedialogassoc.h>
#include <sipmessageelements.h>

#include "TCmdSendBye.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		Contact*, Content-Type*, Content-Encoding*
 *   Parameters:	Content*
 *   IDs:			InviteDialogId
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			TransactionId
 */
void TCmdSendBye::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get SIP objects from registry
	CSIPInviteDialogAssoc* dialog = GetInviteDialogAssocL();

	// Extract both headers (that are still left) and content.
	CSIPMessageElements* elements = ExtractHeadersAndContentLC();

	// -- Execution -----------------------------------------------------------

	// Start SIP Bye transaction
	CSIPClientTransaction* transaction = dialog->SendByeL( elements );
	CleanupStack::Pop( elements );

	// -- Response creation ---------------------------------------------------

	AddIdResponseL( KTransactionId, transaction );
	}

TBool TCmdSendBye::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("SendBye") );
	}

TTcCommandBase* TCmdSendBye::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendBye( aContext );
	}

