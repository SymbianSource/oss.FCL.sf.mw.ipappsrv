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
#include <sipconnection.h>
#include <sipcontactheader.h>
#include <sipfromheader.h>
#include <sipregistrationbinding.h>
#include <sipsubscribedialogassoc.h>
#include <siptoheader.h>
#include <sipdialog.h>

#include "CTcSIPConnectionContainer.h"
#include "TCmdSendSubscribe.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		From, To*, Contact*, Content-Type*, Content-Encoding*,
 *					Accept*, Event*, Route*, Expires*
 *   Parameters:	Content*, RemoteURI*, Refresh*
 *   IDs:			RegistrationId*
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			TransactionId, SubscribeDialogId
 */
void TCmdSendSubscribe::ExecuteL()
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
	CSIPToHeader* toHeader = ExtractToHeaderLC( EFalse );
	CSIPFromHeader* fromHeader = ExtractFromHeaderLC( fromHeaderMandatory );
	CSIPEventHeader* eventHeader = ExtractEventHeaderLC();
	
	// Extract optional headers
	CSIPContactHeader* contactHeader = ExtractContactHeaderLC( EFalse );

	// Conditionally create remote URI
	CUri8* remoteUri = ExtractRemoteURILC();

	// Construct the dialog object
	CSIPSubscribeDialogAssoc* dialogAssoc;
	
    if( registration )
		{
		dialogAssoc = CSIPSubscribeDialogAssoc::NewL(
								connection, remoteUri, *registration,
								eventHeader, fromHeader, toHeader, contactHeader );
		}
	else
		{
		dialogAssoc = CSIPSubscribeDialogAssoc::NewL(
											connection, fromHeader, remoteUri,
											eventHeader, toHeader, contactHeader );
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

	// Start SIP Subscribe transaction.
	CSIPClientTransaction* transaction =
							dialogAssoc->SendSubscribeL( elements, refresh );
	CleanupStack::Pop( elements );
	if( refresh )
		{
		CleanupStack::Pop( refresh );
		}

	// -- Response creation ---------------------------------------------------

	AddIdResponseL( KTransactionId, transaction );	
	AddIdResponseL( KDialogId, dialogAssoc->Dialog() );
	CleanupStack::Pop( dialogAssoc );
	AddIdResponseL( KSubscribeDialogId, dialogAssoc );	
	}

TBool TCmdSendSubscribe::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("SendSubscribe") );
	}

TTcCommandBase* TCmdSendSubscribe::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendSubscribe( aContext );
	}

