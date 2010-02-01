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
#include "TCmdSendResumeSession.h"
#include "CTcMCEContext.h"
#include <MCESession.h>


void TCmdSendResumeSession::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------
	
	// Get session
	CMceSession* session = 
		reinterpret_cast<CMceSession*>(GetObjectForIdL(KSessionId, ETrue));	
	
	// ---------- Execution ----------------------------------------------------

	session->ResumeL();
	
	// ---------- Response creation --------------------------------------------
 
	AddIdResponseL(KSessionId, *session);
	
	}
	
TBool TCmdSendResumeSession::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("SendResumeSession") );
	}

TTcCommandBase* TCmdSendResumeSession::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendResumeSession( aContext );
	}
