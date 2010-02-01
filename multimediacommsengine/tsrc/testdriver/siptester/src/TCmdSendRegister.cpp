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
#include <sipmessageelements.h>
#include <siprefresh.h>
#include <sipregistrationbinding.h>
#include <siprouteheader.h>
#include <siptoheader.h>

#include "CTcSIPConnectionContainer.h"
#include "TCmdSendRegister.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		To, Contact, Route*, From*, Content-Type*, Content-Encoding*
 *   Parameters:	Refresh*, RemoteURI*, Content*, Proxy*
 *   IDs:			ConnectionId*
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			TransactionId, RegistrationId
 */
void TCmdSendRegister::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Select connection; either default or user specified (and existing)
	CSIPConnection& connection = SelectConnectionL().Connection();

	// Extract required headers
	CSIPToHeader* toHeader = ExtractToHeaderLC();
	CSIPContactHeader* contactHeader = ExtractContactHeaderLC();

	// Extract optional headers	
	CSIPFromHeader* fromHeader = ExtractFromHeaderLC( EFalse );
	
	// Conditionally create proxy 
	CSIPRouteHeader* proxy = ExtractProxyLC();

	// Conditionally create refresh object
	CSIPRefresh* refresh = ExtractRefreshLC();

	// Conditionally create remote URI
	CUri8* remoteUri = ExtractRemoteURILC();

	// Construct the registration object
	CSIPRegistrationBinding* registration =
				CSIPRegistrationBinding::NewL( connection,
										toHeader, contactHeader,
										refresh, proxy, remoteUri, fromHeader );

	// Purge items from cleanup stack, now they are owned by CSIPRegistration
	// Some items are optional (i.e. NULL), so we've been tracking the number
	// of items in pushed to CleanupStack
	CleanupStack::Pop( iPushed );
	iPushed = 0;

	CleanupStack::PushL( registration );

	// Extract rest of headers (that are still left) and content.
	CSIPMessageElements* elements = ExtractHeadersAndContentLC();

	// -- Execution -----------------------------------------------------------

	// Start SIP Register transaction.
	CSIPClientTransaction* transaction = registration->RegisterL( elements );
	CleanupStack::Pop( elements );

	// -- Response creation ---------------------------------------------------

	AddIdResponseL( KTransactionId, transaction );
	CleanupStack::Pop( registration );
	AddIdResponseL( KRegistrationId, registration );
	}

TBool TCmdSendRegister::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("SendRegister") );
	}

TTcCommandBase* TCmdSendRegister::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendRegister( aContext );
	}

