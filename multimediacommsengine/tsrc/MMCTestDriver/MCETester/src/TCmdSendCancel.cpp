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
#include "TCmdSendCancel.h"
#include "CTcMCEContext.h"
#include <MceOutSession.h>


void TCmdSendCancel::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------
	
	// Get session
	CMceOutSession* session = 
		reinterpret_cast<CMceOutSession*>(GetObjectForIdL(KSessionId, ETrue));	
	
	// ---------- Execution ----------------------------------------------------

	session->CancelL();

	CMceSession::TState state = session->State();
	
	// ---------- Response creation --------------------------------------------
 
	AddIdResponseL(KSessionId, *session);
	
	// Add Session state	
	AddIntegerResponseL( KResponseState, state );
	AddTextualSessionStateL( state );
	
	}
	
TBool TCmdSendCancel::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("SendCancel") );
	}

TTcCommandBase* TCmdSendCancel::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendCancel( aContext );
	}
