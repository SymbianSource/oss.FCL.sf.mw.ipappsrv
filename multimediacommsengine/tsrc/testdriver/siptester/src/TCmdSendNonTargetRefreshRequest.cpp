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
#include <sipdialogassocbase.h>
#include <sipmessageelements.h>
#include <sipstrings.h>
#include <stringpool.h>
#include <_sipcodecdefs.h>

#include "TCmdSendNonTargetRefreshRequest.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		To, Contact, From*, Content-Type*, Content-Encoding*,
 *					Expires*, Security-Client*
 *   Parameters:	Content*
 *   IDs:			InviteDialogId*, SubscribeDialogId*
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			TransactionId
 */
void TCmdSendNonTargetRefreshRequest::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get SIP objects from registry
	CSIPDialogAssocBase* dialogAssoc = GetAnyDialogAssocL();

	// Get request method
	TPtrC8 methodPtr = ExtractTextL( KParamMethod );
	RStringF methodStr = SIPStrings::Pool().OpenFStringL(methodPtr);
	CleanupClosePushL(methodStr);

	// Extract both headers (that are still left) and content.
	CSIPMessageElements* elements = ExtractHeadersAndContentLC();

	// -- Execution -----------------------------------------------------------

	// Start SIP Request transaction.
	CSIPClientTransaction* transaction =
			 dialogAssoc->SendNonTargetRefreshRequestL( methodStr, elements );
	CleanupStack::Pop( elements );
	CleanupStack::PopAndDestroy(); // methodStr

	// -- Response creation ---------------------------------------------------

	AddIdResponseL( KTransactionId, transaction );
	}

TBool TCmdSendNonTargetRefreshRequest::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("SendNonTargetRefreshRequest") );
	}

TTcCommandBase* TCmdSendNonTargetRefreshRequest::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendNonTargetRefreshRequest( aContext );
	}

