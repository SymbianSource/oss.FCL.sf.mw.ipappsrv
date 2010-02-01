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

#include <sipaddress.h>
#include <sipregistrationbinding.h>
#include <siptoheader.h>

#include "TCmdGetRegistrationAOR.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	-
 *   IDs:			RegistrationId
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			Address
 */
void TCmdGetRegistrationAOR::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get SIP objects from registry
	CSIPRegistrationBinding* registration = GetRegistrationL();

	// -- Execution -----------------------------------------------------------

	// Get registration To address
	HBufC8* aor = registration->Aor().SIPAddress().ToTextLC();

	// -- Response creation ---------------------------------------------------

	AddTextResponseL( KParamAddress, aor->Des() );
	CleanupStack::PopAndDestroy( aor );
	}

TBool TCmdGetRegistrationAOR::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("GetRegistrationAOR") );
	}

TTcCommandBase* TCmdGetRegistrationAOR::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetRegistrationAOR( aContext );
	}
