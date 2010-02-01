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
#include "SIPConstants.h"
#include "TCmdGetIAPNames.h"

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	-
 *   IDs:			-
 *
 * OUTPUT:
 *   Parameters:	IAPNames
 *   IDs:			-
 */
void TCmdGetIAPNames::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get IAP name list
	const MDesC8Array& names = iContext.IAPNames();

	// -- Execution -----------------------------------------------------------

	// -- Response creation ---------------------------------------------------

	AddArrayResponseL( KParamIAPNames, names );
	}

TBool TCmdGetIAPNames::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("GetIAPNames") );
	}

TTcCommandBase* TCmdGetIAPNames::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetIAPNames( aContext );
	}

