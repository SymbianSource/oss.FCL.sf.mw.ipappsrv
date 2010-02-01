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
#include "TCmdFCUpdate.h"
#include "CTcMCEContext.h"

#include <MCESession.h>
#include "TcLog.h" //For testing purposes

void TCmdFCUpdate::ExecuteL()
	{	
	// ---------- Setup ------------------------------------
	
	// Get session
	CMceSession* session = 
		reinterpret_cast<CMceSession*>(GetObjectForIdL(KSessionId, ETrue));	
		
	// ---------- Execution --------------------------------
	
	CDesC8Array* lines = ExtractArrayL( KParamFCMediaAttrLines, ETrue );
	CleanupStack::PushL(lines);
	
	iContext.FC().UpdateL(*session, lines);
	
	CleanupStack::Pop(lines);

	// ---------- Response creation -----------------------
 
 	AddIdResponseL(KSessionId, *session);
 	
	}
	
TBool TCmdFCUpdate::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("FCUpdate") );
	}

TTcCommandBase* TCmdFCUpdate::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdFCUpdate( aContext );
	}
