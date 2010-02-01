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

#include "CTcSIPConnectionContainer.h"
#include "CTcSIPContext.h"
#include "TCmdGetSecurityMechanisms.h"
#include "SIPConstants.h"

#include <sip.h>

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	-
 *   IDs:			-
 *
 * OUTPUT:
 *   Parameters:	SecurityMechanisms
 *   IDs:			-
 */
void TCmdGetSecurityMechanisms::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get security mechanism name list
	CDesC8Array* mechanisms = iContext.SIP().SupportedSecurityMechanismsL();
	CleanupStack::PushL( mechanisms );

	// -- Execution -----------------------------------------------------------

	// -- Response creation ---------------------------------------------------

	AddArrayResponseL( KParamSecurityMechanisms, *mechanisms );
	CleanupStack::PopAndDestroy( mechanisms );
	}

TBool TCmdGetSecurityMechanisms::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("GetSecurityMechanisms") );
	}

TTcCommandBase* TCmdGetSecurityMechanisms::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetSecurityMechanisms( aContext );
	}

