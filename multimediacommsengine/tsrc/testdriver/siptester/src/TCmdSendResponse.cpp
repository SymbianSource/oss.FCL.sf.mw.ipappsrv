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

#include <sipresponseelements.h>
#include <sipservertransaction.h>
#include <sipstrings.h>
#include <stringpool.h>
#include <_sipcodecdefs.h>

#include "TCmdSendResponse.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		Contact*, Content-Type*, Content-Encoding*
 *   Parameters:	StatusCode, Reason, Content*
 *   IDs:			ServerTransactionId
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			-
 */
void TCmdSendResponse::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get SIP objects from registry
	CSIPServerTransaction* serverTransaction = GetServerTransactionL();

	// Get StatusCode and Reason
	TInt statusCode = ExtractIntegerL( KParamStatusCode );
	TPtrC8 reasonPtr = ExtractTextL( KParamReason );
	RStringF reasonStr = SIPStrings::Pool().OpenFStringL(reasonPtr);
	CleanupClosePushL(reasonStr);

	// Create SIP response elements
	CSIPResponseElements* resElements =
							CSIPResponseElements::NewLC( statusCode, reasonStr );
	
	// Extract both headers (that are still left) and content.
	ExtractHeadersAndContentL( resElements->MessageElements() );

	// -- Execution -----------------------------------------------------------

	// Send SIP Response
	serverTransaction->SendResponseL( resElements );
	CleanupStack::Pop( resElements );
	CleanupStack::PopAndDestroy(); // reasonStr
	
	// -- Response creation ---------------------------------------------------
	}

TBool TCmdSendResponse::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("SendResponse") );
	}

TTcCommandBase* TCmdSendResponse::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendResponse( aContext );
	}

