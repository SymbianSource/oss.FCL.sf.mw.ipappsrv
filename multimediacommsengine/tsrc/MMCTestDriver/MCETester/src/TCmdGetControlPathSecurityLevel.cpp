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
#include "TCmdGetControlPathSecurityLevel.h"
#include "CTcMCEContext.h"
#include <MCESession.h>


void TCmdGetControlPathSecurityLevel::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------
	
	
	// Get session
	CMceSession* session = 
		reinterpret_cast<CMceSession*>(GetObjectForIdL(KSessionId, ETrue));	
	
	// ---------- Execution ----------------------------------------------------
	CMceSession::TControlPathSecurityLevel secLevel;
	//TInt secLevel;
	secLevel = session->ControlPathSecurityLevel();
	
	// ---------- Response creation --------------------------------------------
	CMceSession::TState state = session->State();	
	AddIdResponseL(KSessionId, *session);
	// Add Session state	
	AddIntegerResponseL( KResponseState, state );
	AddTextualSessionStateL( state );
	// Add path security
	switch (secLevel) 
		{
		case CMceSession::EControlPathSecurityUnknown: AddTextResponseL(KResponseControlPathSecurity, KValueControlPathSecurityUnknown); break;
		case CMceSession::EControlPathSecure: AddTextResponseL(KResponseControlPathSecurity, KValueControlPathSecure); break;
		case CMceSession::EControlPathUnsecure: AddTextResponseL(KResponseControlPathSecurity, KValueControlPathUnsecure); break;
		default: AddTextResponseL(KResponseControlPathSecurity, KValueControlPathSecurityUnknown); break;
		}
	}
	
TBool TCmdGetControlPathSecurityLevel::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("GetControlPathSecurityLevel") );
	}

TTcCommandBase* TCmdGetControlPathSecurityLevel::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetControlPathSecurityLevel( aContext );
	}
