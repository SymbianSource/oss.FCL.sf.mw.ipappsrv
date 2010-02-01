/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
#include "TCmdSendRejectSession.h"
#include "CTcMCEContext.h"
#include <MceInSession.h>


void TCmdSendRejectSession::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------
	
	// Get session
	CMceInSession* session = reinterpret_cast<CMceInSession*>
		(GetObjectForIdL(KSessionId, ETrue));	
	
	// ---------- Execution --------------------------------

	// Get reasonphrase
	TPtrC8 reason = ExtractTextL(KParamReasonPhrase, EFalse);
		
	// Get statuscode
	TInt status = ExtractIntegerL(KParamStatusCode, 0, EFalse);
	
	// Get Headers
	CDesC8Array* headers = ExtractHeadersL( EFalse );
	CleanupStack::PushL(headers);

	// Get ContentType
	HBufC8* contentType = HBufCParameterL(ExtractTextL(KParamContentType, 
													   EFalse));
	CleanupStack::PushL(contentType);
	// Get Content
	HBufC8* body = HBufCParameterL(ExtractTextL(KParamBody, EFalse));
	CleanupStack::PushL( body );
	
	// Reject
	if( reason == KNullDesC8 && status == 0 && !headers && 
		(body && body->Length()==0) && (contentType && contentType->Length()==0))  
		{
		CleanupStack::PopAndDestroy( body ); body = NULL;
		CleanupStack::PopAndDestroy( contentType ); contentType = NULL;
		CleanupStack::PopAndDestroy( headers ); headers = NULL;
		session->RejectL();
		}
	else
		{
		session->RejectL( reason, status, headers, contentType, body );
		CleanupStack::Pop( body );
		CleanupStack::Pop( contentType );	
		CleanupStack::Pop(headers);
	
		}
  
	CMceSession::TState state = session->State();			

	// ---------- Response creation -----------------------
 
	AddIdResponseL( KSessionId, *session );

	// Add Session state	
	AddIntegerResponseL( KResponseState, state );
	AddTextualSessionStateL( state );
	
	}
	
TBool TCmdSendRejectSession::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("SendRejectSession") );
	}

TTcCommandBase* TCmdSendRejectSession::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendRejectSession( aContext );
	}
