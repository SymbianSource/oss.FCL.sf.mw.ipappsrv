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
#include "TCmdSendTerminate.h"
#include "CTcMCEContext.h"
#include <MCESession.h>


void TCmdSendTerminate::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------
	
	
	// Get session
	CMceSession* session = 
		reinterpret_cast<CMceSession*>(GetObjectForIdL(KSessionId, ETrue));	
	
	// ---------- Execution ----------------------------------------------------
	TBool deleteSession = ExtractBooleanL( KParamDeleteSession, EFalse );
	if (deleteSession) 
		{
		iContext.Registry().DestroyObject ( session );
		}
	else 
		{
		session->TerminateL();
		// ---------- Response creation --------------------------------------------
		CMceSession::TState state = session->State();	
		AddIdResponseL(KSessionId, *session);
		// Add Session state	
		AddIntegerResponseL( KResponseState, state );
		AddTextualSessionStateL( state );
		}
 
	}
	
TBool TCmdSendTerminate::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("SendEndSession") );
	}

TTcCommandBase* TCmdSendTerminate::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendTerminate( aContext );
	}
