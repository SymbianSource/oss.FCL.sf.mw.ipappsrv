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

#include "TCmdSetOutboundProxy.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	Proxy*
 *   IDs:			RegistrationId
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			-
 */
void TCmdSetOutboundProxy::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get SIP objects from registry
	CSIPRegistrationBinding* registration = GetRegistrationL();

	// Conditionally create proxy 
	CSIPRouteHeader* proxy = ExtractProxyLC();

	// -- Execution -----------------------------------------------------------

	registration->SetOutboundProxyL( proxy );
	if( proxy )
		{
		CleanupStack::Pop();
		}

	// -- Response creation ---------------------------------------------------
	}

TBool TCmdSetOutboundProxy::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("SetOutboundProxy") );
	}

TTcCommandBase* TCmdSetOutboundProxy::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSetOutboundProxy( aContext );
	}

