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
#include "TCmdSendRinging.h"
#include "CTcMCEContext.h"
#include <MceInSession.h>


void TCmdSendRinging::ExecuteL()
	{	
	// ---------- Setup ------------------------------------
	
	// Get session
	CMceInSession* session = 
	    reinterpret_cast<CMceInSession*>(GetObjectForIdL(KSessionId, ETrue));
		
	// ---------- Execution --------------------------------

	session->RingL();

	CMceSession::TState state = session->State();	

	// ---------- Response creation -----------------------
 
	AddIdResponseL(KSessionId, *session);

	// Add Session state	
	AddIntegerResponseL( KResponseState, state );
	AddTextualSessionStateL( state );
	
	}
	
TBool TCmdSendRinging::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("SendRinging") );
	}

TTcCommandBase* TCmdSendRinging::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendRinging( aContext );
	}
