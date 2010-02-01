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

#include "CTcSIPContext.h"
#include "SIPConstants.h"
#include "TCmdIsSigCompSupported.h"

#include <sip.h>

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	-
 *   IDs:			-
 *
 * OUTPUT:
 *   Parameters:	Boolean
 *   IDs:			-
 */
void TCmdIsSigCompSupported::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// -- Execution -----------------------------------------------------------

	TBool supported = iContext.SIP().IsSigCompSupportedL();

	// -- Response creation ---------------------------------------------------

	AddBooleanResponseL( KParamBoolean, supported );
	}

TBool TCmdIsSigCompSupported::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("IsSigCompSupported") );
	}

TTcCommandBase* TCmdIsSigCompSupported::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdIsSigCompSupported( aContext );
	}

