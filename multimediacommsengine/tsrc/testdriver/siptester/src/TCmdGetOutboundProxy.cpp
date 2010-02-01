/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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

#include <sipregistrationbinding.h>
#include <siprouteheader.h>

#include "TCmdGetOutboundProxy.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	-
 *   IDs:			RegistrationId
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			Address*
 */
void TCmdGetOutboundProxy::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get SIP objects from registry
	CSIPRegistrationBinding* registration = GetRegistrationL();

	// -- Execution -----------------------------------------------------------

	// Get proxy header, if present
	const CSIPRouteHeader* proxy = registration->OutboundProxy();

	// Get proxy address, if present
    HBufC8* proxyAddress = NULL;
	if( proxy )
		{
		proxyAddress = proxy->ToTextValueLC();
		}

	// -- Response creation ---------------------------------------------------

	if( proxyAddress )
		{
		AddTextResponseL( KParamAddress, proxyAddress->Des() );
		CleanupStack::PopAndDestroy( proxyAddress );
		}
	}

TBool TCmdGetOutboundProxy::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("GetOutboundProxy") );
	}

TTcCommandBase* TCmdGetOutboundProxy::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetOutboundProxy( aContext );
	}
