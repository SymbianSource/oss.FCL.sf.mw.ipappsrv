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

#include "TCmdSendAck.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		Contact*, Content-Type*, Content-Encoding*
 *   Parameters:	Content*
 *   IDs:			InviteDialogId, TransactionId
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			-
 */
void TCmdSendAck::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get SIP objects from registry
	CSIPInviteDialogAssoc* dialog =	GetInviteDialogAssocL();
	CSIPClientTransaction* transaction = GetClientTransactionL();

	// Extract both headers (that are still left) and content.
	CSIPMessageElements* elements = ExtractHeadersAndContentLC();

	// -- Execution -----------------------------------------------------------

	// Start SIP Ack
	dialog->SendAckL( *transaction, elements );
 	CleanupStack::Pop( elements );

	// -- Response creation ---------------------------------------------------
	}

TBool TCmdSendAck::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("SendAck") );
	}

TTcCommandBase* TCmdSendAck::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendAck( aContext );
	}

