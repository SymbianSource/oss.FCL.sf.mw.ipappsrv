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




#include <MCESession.h>

#include "MCEConstants.h"
#include "TCmdGetCurrentState.h"
#include "CTcMCEContext.h"


void TCmdGetCurrentState::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------
	
	// Get session
	CMceSession* session = 
		reinterpret_cast<CMceSession*>(GetObjectForIdL(KSessionId, ETrue));	

	// ---------- Execution ----------------------------------------------------

	CMceSession::TState state = session->State();

	// ---------- Response creation --------------------------------------------

	AddIdResponseL( KSessionId, *session );
	
	AddIntegerResponseL(KResponseState, state);
	AddTextualSessionStateL(state);

	}
	
TBool TCmdGetCurrentState::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("GetCurrentState") );
	}

TTcCommandBase* TCmdGetCurrentState::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetCurrentState( aContext );
	}
