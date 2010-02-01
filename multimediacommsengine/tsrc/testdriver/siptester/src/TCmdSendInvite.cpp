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
#include <sipregistrationbinding.h>

#include "CTcSIPConnectionContainer.h"
#include "TCmdSendInvite.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		From, To*, Contact*, Content-Type*
 *   Parameters:	Content*, RemoteURI*
 *   IDs:			RegistrationId*, InviteDialogId, ConnectionId*
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			TransactionId, InviteDialogId
 */
void TCmdSendInvite::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Select connection; either default or user specified (and existing)
	CSIPConnection& connection = SelectConnectionL().Connection();

	// Get SIP objects from registry
	CSIPRegistrationBinding* registration = GetRegistrationL( EFalse );
	TBool fromHeaderMandatory( ETrue );
	if( registration )
		{
		fromHeaderMandatory = EFalse;
		}

	// Extract required headers (From may be either mandatory or optional)
	CSIPFromHeader* fromHeader = ExtractFromHeaderLC( fromHeaderMandatory );

	// Extract optional headers
	CSIPToHeader* toHeader = ExtractToHeaderLC( EFalse );
	CSIPContactHeader* contactHeader = ExtractContactHeaderLC( EFalse );

	// Conditionally create remote URI
	CUri8* remoteUri = ExtractRemoteURILC();

	// Construct the dialog association object
	CSIPInviteDialogAssoc* dialogAssoc;

	if( registration )
		{
		dialogAssoc = CSIPInviteDialogAssoc::NewL(
								connection, remoteUri, *registration,
								fromHeader, toHeader, contactHeader );
		}
	else
		{
		dialogAssoc = CSIPInviteDialogAssoc::NewL(
								connection, fromHeader,
								remoteUri, toHeader, contactHeader );
		}
	
	// Purge items from cleanup stack, now they are owned by CSIPInviteDialogAssoc
	// Some items are optional (i.e. NULL), so we've been tracking the number
	// of items in pushed to CleanupStack
	CleanupStack::Pop( iPushed );
	iPushed = 0;

	CleanupStack::PushL( dialogAssoc );

	// Extract both headers (that are still left) and content.
	CSIPMessageElements* elements = ExtractHeadersAndContentLC();

	// -- Execution -----------------------------------------------------------

	// Start SIP Invite transaction.
	CSIPClientTransaction* transaction = dialogAssoc->SendInviteL( elements );
	CleanupStack::Pop( elements );

	// -- Response creation ---------------------------------------------------

	AddIdResponseL( KTransactionId, transaction );
	CleanupStack::Pop( dialogAssoc );
	AddIdResponseL( KInviteDialogId, dialogAssoc );
	}

TBool TCmdSendInvite::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("SendInvite") );
	}

TTcCommandBase* TCmdSendInvite::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendInvite( aContext );
	}

