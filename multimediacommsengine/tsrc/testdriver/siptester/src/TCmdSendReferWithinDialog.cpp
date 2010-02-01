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

#include <sipclienttransaction.h>
#include <sipreferdialogassoc.h>
#include <sipnotifydialogassoc.h>

#include "TCmdSendReferWithinDialog.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		Contact*, Content-Type*, Content-Encoding*, Route*
 *   Parameters:	Content*
 *   IDs:			NotifyDialogId*, ReferDialogId*
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			TransactionId, ReferDialogId
 */
void TCmdSendReferWithinDialog::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get SIP objects from registry
	CSIPDialogAssocBase* dialogAssoc = GetAnyDialogAssocL();

	// Extract required headers
    CSIPReferToHeader* referToHeader = ExtractReferToHeaderLC();
    
	// Create a new dialog association
	CSIPReferDialogAssoc* newdialogAssoc =
						CSIPReferDialogAssoc::NewL( dialogAssoc->Dialog(),
													referToHeader );
	CleanupStack::Pop( referToHeader );
	CleanupStack::PushL( newdialogAssoc );

	// Extract both headers (that are still left) and content.
	CSIPMessageElements* elements = ExtractHeadersAndContentLC();

	// -- Execution -----------------------------------------------------------

	// Start SIP refer transaction.
	CSIPClientTransaction* transaction =
							newdialogAssoc->SendReferL( elements );
	CleanupStack::Pop( elements );

	// -- Response creation ---------------------------------------------------

	AddIdResponseL( KTransactionId, transaction );
	CleanupStack::Pop( newdialogAssoc );
	AddIdResponseL( KReferDialogId, newdialogAssoc );
	}

TBool TCmdSendReferWithinDialog::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("SendReferWithinDialog") );
	}

TTcCommandBase* TCmdSendReferWithinDialog::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendReferWithinDialog( aContext );
	}

