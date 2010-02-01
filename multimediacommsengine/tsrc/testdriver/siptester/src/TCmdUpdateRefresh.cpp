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
#include <siprefresh.h>
#include <sipregistrationbinding.h>
#include <sipsubscribedialogassoc.h>

#include "TCmdUpdateRefresh.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		Content-Type*, Content-Encoding*, Expires*
 *   Parameters:	Content*
 *   IDs:			RefreshId*, RegistrationId*, SubscribeDialogId*
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			TransactionId
 */
void TCmdUpdateRefresh::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get SIP objects from registry
	CSIPRefresh* refresh = GetRefreshL( EFalse );
	CSIPRegistrationBinding* registration = GetRegistrationL( EFalse );
	CSIPSubscribeDialogAssoc* dialogAssoc = GetSubscribeDialogAssocL( EFalse );

	// Get the refresh object from a registration or from a dialog association
	// if it's  not defined as such.
	if( !refresh )
		{
		if( registration )
			{
			refresh = const_cast< CSIPRefresh* >( registration->SIPRefresh() );
			}
		else if( dialogAssoc )
			{
			refresh = const_cast< CSIPRefresh* >( dialogAssoc->SIPRefresh() );
			}
		else
			{
			User::Leave( KTcErrMandatoryIdNotFound );
			}
		}

	// Extract both headers (that are still left) and content.
	CSIPMessageElements* elements = ExtractHeadersAndContentLC();

	// -- Execution -----------------------------------------------------------

	// Start refresh update
	CSIPClientTransaction* transaction = refresh->UpdateL( elements );
	CleanupStack::Pop( elements );

	// -- Response creation ---------------------------------------------------

	AddIdResponseL( KTransactionId, transaction );
	}

TBool TCmdUpdateRefresh::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("UpdateRefresh") );
	}

TTcCommandBase* TCmdUpdateRefresh::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdUpdateRefresh( aContext );
	}

