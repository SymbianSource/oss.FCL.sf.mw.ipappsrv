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
#include <sipreferdialogassoc.h>
#include <sipregistrationbinding.h>
#include <sipdialog.h>

#include "CTcSIPConnectionContainer.h"
#include "TCmdSendRefer.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		From, To*, Contact*, Content-Type*
 *   Parameters:	Content*, RemoteURI*
 *   IDs:			RegistrationId*, ConnectionId*
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			TransactionId, DialogId, ReferDialogId
 */
void TCmdSendRefer::ExecuteL()
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
    CSIPReferToHeader* referToHeader = ExtractReferToHeaderLC();

	// Extract optional headers
	CSIPToHeader* toHeader = ExtractToHeaderLC( EFalse );
	CSIPContactHeader* contactHeader = ExtractContactHeaderLC( EFalse );

	// Conditionally create remote URI
	CUri8* remoteUri = ExtractRemoteURILC();
	
	// Construct the dialog object
	CSIPReferDialogAssoc* dialogAssoc;
	
	if( registration )
		{
		dialogAssoc = CSIPReferDialogAssoc::NewL(
								connection, remoteUri, *registration,
								referToHeader, fromHeader, toHeader, contactHeader );
		}
	else
		{
		dialogAssoc = CSIPReferDialogAssoc::NewL(
								connection, fromHeader, remoteUri,
								referToHeader, toHeader, contactHeader );
		}
	
	// Purge items from cleanup stack, now they are owned by CSIPInviteDialogAssoc
	// Some items are optional (i.e. NULL), so we've been tracking the number
	// of items in pushed to CleanupStack
	CleanupStack::Pop( iPushed );
	iPushed = 0;

	CleanupStack::PushL( dialogAssoc );

	// Conditionally create refresh object
	CSIPRefresh* refresh = ExtractRefreshLC();

	// Extract both headers (that are still left) and content.
	CSIPMessageElements* elements = ExtractHeadersAndContentLC();

	// -- Execution -----------------------------------------------------------

	// Start SIP Invite transaction.
	CSIPClientTransaction* transaction = dialogAssoc->SendReferL( elements );
	CleanupStack::Pop( elements );
	if( refresh )
		{
		CleanupStack::Pop( refresh );
		}

	// -- Response creation ---------------------------------------------------

	AddIdResponseL( KTransactionId, transaction );
	AddIdResponseL( KDialogId, dialogAssoc->Dialog() );
	CleanupStack::Pop( dialogAssoc );
	AddIdResponseL( KReferDialogId, dialogAssoc );
	}

TBool TCmdSendRefer::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("SendRefer") );
	}

TTcCommandBase* TCmdSendRefer::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendRefer( aContext );
	}

