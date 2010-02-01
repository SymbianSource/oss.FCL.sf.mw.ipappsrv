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

#include "TCmdRegisterClient.h"
#include "CTcSIPContext.h"
#include "SIPConstants.h"
#include "CTcSIPClientDiscoveryContainer.h"

#include <sipclientdiscovery.h>

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
void TCmdRegisterClient::ExecuteL()
	{
	iContext.ClientDiscoveryL().ClientDiscovery().RegisterL( ExtractUidL( KParamApplicationUid ) );
	}

TBool TCmdRegisterClient::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("RegisterClient") );
	}

TTcCommandBase* TCmdRegisterClient::CreateL( MTcTestContext& aContext )
	{
	return new ( ELeave ) TCmdRegisterClient( aContext );
	}

