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
#include <sipregistrationbinding.h>
#include <siprequestelements.h>
#include <siptoheader.h>
#include <sipstrings.h>
#include <stringpool.h>
#include <_sipcodecdefs.h>

#include "CTcSIPConnectionContainer.h"
#include "TCmdSendMessage.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		To, From*, Contact*, Content-Type*, Content-Encoding*, P-Preferred-ID*
 *   Parameters:	Content*, RemoteURI*
 *   IDs:			RegistrationId*, ConnectionId*
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			TransactionId
 */
void TCmdSendMessage::ExecuteL()
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

	// Extract remote URI
	CUri8* uri = ExtractRemoteURILC();

	// Create SIP request elements, giving uri away
	CSIPRequestElements* reqElements = CSIPRequestElements::NewL( uri );
	CleanupStack::Pop( uri );
	CleanupStack::PushL( reqElements );


	// Modify to RStringF
	RStringF methodStr = SIPStrings::Pool().OpenFStringL(KMethodMessage);
	CleanupClosePushL(methodStr);
	
	// Set request method to "MESSAGE"
    reqElements->SetMethodL( methodStr );

	// Extract From header (may be either mandatory or optional)
	CSIPFromHeader* fromHeader = ExtractFromHeaderLC( fromHeaderMandatory );
	if( fromHeader )
		{
		reqElements->SetFromHeaderL( fromHeader );
		CleanupStack::Pop( fromHeader );
		}
		
    // Extract To header
	CSIPToHeader* toHeader = ExtractToHeaderLC( EFalse );
	if( toHeader )
	    {
	    reqElements->SetToHeaderL( toHeader );
	    CleanupStack::Pop( toHeader );
	    }

	// Extract both headers (that are still left) and content.
	ExtractHeadersAndContentL( reqElements->MessageElements() );
    
	// -- Execution -----------------------------------------------------------

	// Start SIP Request transaction.
	CSIPClientTransaction* transaction;
	if( registration )
		{
		transaction = connection.SendRequestL( reqElements, *registration );
		}
	else
		{
		transaction = connection.SendRequestL( reqElements );
		}

    CleanupStack::PopAndDestroy(); // methodStr
	CleanupStack::Pop( reqElements );

	// -- Response creation ---------------------------------------------------

	AddIdResponseL( KTransactionId, transaction );
	}

TBool TCmdSendMessage::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("SendMessage") );
	}

TTcCommandBase* TCmdSendMessage::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendMessage( aContext );
	}

