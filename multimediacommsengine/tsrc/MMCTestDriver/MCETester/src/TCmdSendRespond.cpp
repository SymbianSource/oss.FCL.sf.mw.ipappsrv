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
#include "TCmdSendRespond.h"
#include "CTcMCEContext.h"
#include <MceInSession.h>


void TCmdSendRespond::ExecuteL()
	{	
	// ---------- Setup ------------------------------------
	
	// Get session
	CMceInSession* session = 
	    reinterpret_cast<CMceInSession*>(GetObjectForIdL(KSessionId, ETrue));
		
	// ---------- Execution --------------------------------

	// Get reasonphrase
	TPtrC8 reason = ExtractTextL(KParamReasonPhrase, ETrue);
		
	// Get statuscode
	TInt status = ExtractIntegerL(KParamStatusCode, 180, ETrue);

	session->RespondL(reason, status);

	CMceSession::TState state = session->State();	

	// ---------- Response creation -----------------------
 
	AddIdResponseL(KSessionId, *session);

	// Add Session state	
	AddIntegerResponseL( KResponseState, state );
	AddTextualSessionStateL( state );
	
	}
	
TBool TCmdSendRespond::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("SendRespond") );
	}

TTcCommandBase* TCmdSendRespond::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendRespond( aContext );
	}
