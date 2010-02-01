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

#include "CTcSIPConnectionContainer.h"
#include "SIPConstants.h"
#include "TCmdFetchRegistrations.h"

#include <sipclienttransaction.h>
#include <sipfromheader.h>
#include <siprequestelements.h>
#include <siptoheader.h>
#include <uri8.h>

/**
 * INPUT:
 *   Headers:		To
 *   Parameters:	-
 *   IDs:			ConnectionId*
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			TransactionId
 */
void TCmdFetchRegistrations::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Select connection; either default or user specified (and existing)
	CSIPConnection& connection = SelectConnectionL().Connection();

    // Extract remote URI
	CUri8* uri = ExtractRemoteURILC();

    // Create SIP request elements, giving remote URI away
	CSIPRequestElements* reqElements = CSIPRequestElements::NewL( uri );
	CleanupStack::Pop( iPushed ); //possibly uri
	iPushed = 0;
		
	CleanupStack::PushL( reqElements );
	
	// Extract required headers
	CSIPToHeader* toHeader = ExtractToHeaderLC();
	
	// Create a From header from the value of To header
	CSIPFromHeader* fromHeader = CSIPFromHeader::NewLC( *toHeader );
	reqElements->SetFromHeaderL( fromHeader );
	CleanupStack::Pop( fromHeader );
	
    CleanupStack::PopAndDestroy( iPushed ); //possibly toHeader
    iPushed = 0;

	// -- Execution -----------------------------------------------------------

	// Start a SIP Register transaction
	CSIPClientTransaction* transaction =
								connection.FetchRegistrationsL( reqElements );
	CleanupStack::Pop( reqElements );

	// -- Response creation ---------------------------------------------------

	AddIdResponseL( KTransactionId, transaction );
	}

TBool TCmdFetchRegistrations::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("FetchRegistrations") );
	}

TTcCommandBase* TCmdFetchRegistrations::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdFetchRegistrations( aContext );
	}
