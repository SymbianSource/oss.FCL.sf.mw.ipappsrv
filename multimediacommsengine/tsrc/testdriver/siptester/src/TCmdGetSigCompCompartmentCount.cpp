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

#include <sipinternalstates.h>

#include "TCmdGetSigCompCompartmentCount.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	-
 *   IDs:			-
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			CompartmentCount
 */
void TCmdGetSigCompCompartmentCount::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// -- Execution -----------------------------------------------------------

	// Get SigComp compartment count
	CSIPInternalStates* internalStates = CSIPInternalStates::NewLC();
	TInt count = internalStates->SigCompCompartmentCount();
	CleanupStack::PopAndDestroy(); // internalStates

	// -- Response creation ---------------------------------------------------

	AddIntegerResponseL( KParamCompartmentCount, count );
	}

TBool TCmdGetSigCompCompartmentCount::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("GetSigCompCompartmentCount") );
	}

TTcCommandBase* TCmdGetSigCompCompartmentCount::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetSigCompCompartmentCount( aContext );
	}
