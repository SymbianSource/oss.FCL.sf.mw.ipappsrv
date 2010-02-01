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
* Description:  Implementation
*
*/

#include "TCmdCreateSession.h"
#include "CTcSIPContext.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers: -
 *   Parameters: ApplicationUid
 *   IDs: -
 *
 * OUTPUT:
 *   Parameters: -
 *   IDs: -
 */
void TCmdCreateSession::ExecuteL()
	{
	iContext.CreateClientDiscoveryL( ExtractUidL( KParamApplicationUid ) );
	}

TBool TCmdCreateSession::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("CreateSession") );
	}

TTcCommandBase* TCmdCreateSession::CreateL( MTcTestContext& aContext )
	{
	return new ( ELeave ) TCmdCreateSession( aContext );
	}

