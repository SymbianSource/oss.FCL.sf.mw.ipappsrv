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

#include "TCmdIsContextActive.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	-
 *   IDs:			RegistrationId
 *
 * OUTPUT:
 *   Parameters:	Boolean
 *   IDs:			-
 */
void TCmdIsContextActive::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get SIP objects from registry
	CSIPRegistrationBinding* registration = GetRegistrationL( EFalse );

	// -- Execution -----------------------------------------------------------

	TBool active = registration->IsContextActive();

	// -- Response creation ---------------------------------------------------

	AddBooleanResponseL( KParamBoolean, active );
	}

TBool TCmdIsContextActive::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("IsContextActive") );
	}

TTcCommandBase* TCmdIsContextActive::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdIsContextActive( aContext );
	}

