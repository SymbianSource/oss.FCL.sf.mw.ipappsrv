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
#include <sipexpiresheader.h>
#include <sipmessageelements.h>
#include <sipregistrationbinding.h>

#include "TCmdUpdateRegistration.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		Content-Type*, Content-Encoding*, Expires*, Security-Client*
 *   Parameters:	Content*
 *   IDs:			RegistrationId
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			TransactionId
 */
void TCmdUpdateRegistration::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get SIP objects from registry
	CSIPRegistrationBinding* registration = GetRegistrationL();

	// Get optional Expires header. It's important to extract the header
	// as CSIPRegistration::UpdateL() doesn't allow it to be passed as part
	// of the user headers.
	CSIPExpiresHeader* expiresHeader = ExtractExpiresHeaderLC( EFalse );
	TInt expires( KErrNotFound );
	if( expiresHeader )
		{
		expires = expiresHeader->Value();
		}

	// Extract both headers (that are still left) and content.
	CSIPMessageElements* elements = ExtractHeadersAndContentLC();

	// -- Execution -----------------------------------------------------------

	// Start registration update, either with or without an Expires value
	CSIPClientTransaction* transaction;
	if( expires != KErrNotFound )
		{
		transaction = registration->UpdateL( expires, elements );
		}
	else
		{
		transaction = registration->UpdateL( elements );
		}

 	CleanupStack::Pop( elements );
 	if( expiresHeader )
 		{
 		CleanupStack::Pop( expiresHeader );
 		}

	// -- Response creation ---------------------------------------------------

	AddIdResponseL( KTransactionId, transaction );
	}

TBool TCmdUpdateRegistration::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("UpdateRegistration") );
	}

TTcCommandBase* TCmdUpdateRegistration::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdUpdateRegistration( aContext );
	}

