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
#include <sipsubscribedialogassoc.h>

#include "TCmdSendSubscribeWithinDialog.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		Contact*, Content-Type*, Content-Encoding*,
 *					Event*, Expires*
 *   Parameters:	Content*, Refresh*
 *   IDs:			InviteDialogId*, SubscribeDialogId*
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			TransactionId, SubscribeDialogId
 */
void TCmdSendSubscribeWithinDialog::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get SIP objects from registry
	CSIPDialogAssocBase* dialogAssoc = GetAnyDialogAssocL();

	// Extract required headers
	CSIPEventHeader* eventHeader = ExtractEventHeaderLC();

	// Create a new dialog association
	CSIPSubscribeDialogAssoc* newdialogAssoc =
						CSIPSubscribeDialogAssoc::NewL( dialogAssoc->Dialog(),
														 eventHeader );
	CleanupStack::Pop( eventHeader );
	CleanupStack::PushL( newdialogAssoc );

	// Conditionally create refresh object
	CSIPRefresh* refresh = ExtractRefreshLC();

	// Extract both headers (that are still left) and content.
	CSIPMessageElements* elements = ExtractHeadersAndContentLC();

	// -- Execution -----------------------------------------------------------

	// Start SIP Subscribe transaction.
	CSIPClientTransaction* transaction =
							newdialogAssoc->SendSubscribeL( elements, refresh );
	CleanupStack::Pop( elements );
	if( refresh )
		{
		CleanupStack::Pop( refresh );
		}

	// -- Response creation ---------------------------------------------------

	AddIdResponseL( KTransactionId, transaction );
	CleanupStack::Pop( newdialogAssoc );
	AddIdResponseL( KSubscribeDialogId, newdialogAssoc );
	}

TBool TCmdSendSubscribeWithinDialog::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("SendSubscribeWithinDialog") );
	}

TTcCommandBase* TCmdSendSubscribeWithinDialog::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendSubscribeWithinDialog( aContext );
	}

