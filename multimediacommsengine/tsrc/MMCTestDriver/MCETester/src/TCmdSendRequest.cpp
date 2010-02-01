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
#include "TCmdSendRequest.h"
#include "CTcMCEContext.h"
#include "mcesession.h"

// -----------------------------------------------------------------------------
// TCmdSendRequest::ExecuteL
// -----------------------------------------------------------------------------
//
void TCmdSendRequest::ExecuteL()
	{

	// ---------- Setup --------------------------------------------------------
	const TBool KOptional = EFalse;

	CMceSession* session =
		reinterpret_cast< CMceSession* >( GetObjectForIdL( KSessionId ) );
	// Check that session is in a correct state for sending a request
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
	
		
	// Get request method
	TPtrC8 method = ExtractTextL( KParamMethod );

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

	TMceTransactionId transactionId =
		session->SendRequestL( method, headers, contentType, body );

	CleanupStack::Pop( body );
    CleanupStack::Pop( contentType );
    CleanupStack::Pop( headers );

	// ---------- Response creation --------------------------------------------

	AddIntegerResponseL( KResponseTransactionId, transactionId );
    }

// -----------------------------------------------------------------------------
// TCmdSendRequest::Match
// -----------------------------------------------------------------------------
//
TBool TCmdSendRequest::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8( "SendRequest" ) );
	}

// -----------------------------------------------------------------------------
// TCmdSendRequest::CreateL
// -----------------------------------------------------------------------------
//
TTcCommandBase* TCmdSendRequest::CreateL( MTcTestContext& aContext )
	{
	return new ( ELeave ) TCmdSendRequest( aContext );
	}
