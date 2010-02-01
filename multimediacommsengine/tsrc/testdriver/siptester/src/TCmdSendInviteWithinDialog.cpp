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
#include <sipsubscribedialogassoc.h>

#include "TCmdSendInviteWithinDialog.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		Contact*, Content-Type*, Content-Encoding*, Route*
 *   Parameters:	Content*
 *   IDs:			InviteDialogId*, SubscribeDialogId*
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			TransactionId, InviteDialogId
 */
void TCmdSendInviteWithinDialog::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get SIP objects from registry
	CSIPSubscribeDialogAssoc* subscribeDialogAssoc =
										GetSubscribeDialogAssocL( EFalse );

	// If there was a SubscribeDialogId, use it to create a new InviteDialogAssoc
	// If not, try to use a InviteDialogId for an existing InviteDialogAssoc
	CSIPInviteDialogAssoc* inviteDialogAssoc;
	if( subscribeDialogAssoc )
		{
		// Create a new dialog association
		inviteDialogAssoc =
				CSIPInviteDialogAssoc::NewLC( subscribeDialogAssoc->Dialog() );
		}
	else
		{
		inviteDialogAssoc = GetInviteDialogAssocL();
		}

	// Extract both headers (that are still left) and content.
	CSIPMessageElements* elements = ExtractHeadersAndContentLC();

	// -- Execution -----------------------------------------------------------

	// Start SIP Invite transaction.
	CSIPClientTransaction* transaction = inviteDialogAssoc->SendInviteL( elements );
	CleanupStack::Pop( elements );

	// -- Response creation ---------------------------------------------------

	AddIdResponseL( KTransactionId, transaction );
	// If subscribeDialogAssoc is valid, inviteDialogAssoc ptr is in CleanupStack
	if( subscribeDialogAssoc )
		{
		CleanupStack::Pop( inviteDialogAssoc );
		}
	AddIdResponseL( KInviteDialogId, inviteDialogAssoc );
	}

TBool TCmdSendInviteWithinDialog::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("SendInviteWithinDialog") );
	}

TTcCommandBase* TCmdSendInviteWithinDialog::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendInviteWithinDialog( aContext );
	}

