/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Implementation
*
*/



#include "MCEConstants.h"
#include "TCmdSendResponse.h"
#include "CTcMCEContext.h"
#include "mcesession.h"

// -----------------------------------------------------------------------------
// TCmdSendResponse::ExecuteL
// -----------------------------------------------------------------------------
//
void TCmdSendResponse::ExecuteL()
	{

	// ---------- Setup --------------------------------------------------------
	const TBool KOptional = EFalse;

	CMceSession* session =
		reinterpret_cast< CMceSession* >( GetObjectForIdL( KSessionId ) );
	// Check that session is in a correct state for sending a response
	switch ( session->State() )
		{
		case CMceSession::EOffering:
		case CMceSession::EReserving:
		case CMceSession::EAnswering:
		case CMceSession::EProceeding:
		case CMceSession::EEstablished:
			break;
		
		default:
			User::Leave( KErrArgument );
		}

	TMceTransactionId transactionId = ExtractIntegerL( KParamTransactionId );

	// Get reason phrase
	TPtrC8 reasonPhrase = ExtractTextL( KParamReasonPhrase );

	// Get status code
	TInt statusCode = ExtractIntegerL( KParamStatusCode );

	// Get Headers
	CDesC8Array* headers = ExtractHeadersL( KOptional );
	CleanupStack::PushL( headers );
	
	// Get ContentType
	HBufC8* contentType = HBufCParameterL( ExtractTextL( KParamContentType, 
													     KOptional ) );
	CleanupStack::PushL( contentType );
	
	// Get Body
	HBufC8* body = HBufCParameterL( ExtractTextL( KParamBody, KOptional ) );
	CleanupStack::PushL( body );

	// ---------- Execution ----------------------------------------------------

	session->SendResponseL( transactionId,
				      		reasonPhrase,
			          		statusCode,
				      		headers,
				      		contentType,
				      		body );
	CleanupStack::Pop( body );
    CleanupStack::Pop( contentType );
    CleanupStack::Pop( headers );
    
	// ---------- Response creation --------------------------------------------
 
	// No output ids or output params
    }

// -----------------------------------------------------------------------------
// TCmdSendResponse::Match
// -----------------------------------------------------------------------------
//
TBool TCmdSendResponse::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8( "SendResponse" ) );
	}

// -----------------------------------------------------------------------------
// TCmdSendResponse::CreateL
// -----------------------------------------------------------------------------
//
TTcCommandBase* TCmdSendResponse::CreateL( MTcTestContext& aContext )
	{
	return new ( ELeave ) TCmdSendResponse( aContext );
	}
