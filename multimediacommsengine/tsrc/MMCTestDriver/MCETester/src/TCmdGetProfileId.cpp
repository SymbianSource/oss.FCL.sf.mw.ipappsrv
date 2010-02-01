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




#include <MceSession.h>

#include "MCEConstants.h"
#include "TCmdGetProfileId.h"
#include "CTcMCEContext.h"
#include "CTcMCEReceived.h"

void TCmdGetProfileId::ExecuteL()
	{
			// -- Execution ------
	
		// Get session
	CMceSession* session = 
		reinterpret_cast<CMceSession*>(GetObjectForIdL(KSessionId, ETrue));	
		
	CMceSession::TState state = session->State();
	
	TUint32 profileId( 0 );
	
	profileId = session->Profile();
	
	AddIdResponseL( KSessionId, *session );	
	
	AddIntegerResponseL( KParamProfileId, profileId );
	
	AddTextualSessionStateL(state);

	}

TBool TCmdGetProfileId::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("GetProfileId") );
	}

TTcCommandBase* TCmdGetProfileId::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetProfileId( aContext );
	}
	
