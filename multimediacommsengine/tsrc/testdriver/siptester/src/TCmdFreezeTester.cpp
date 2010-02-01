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

#include "TCmdFreezeTester.h"
#include "CTcSIPContext.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers: -
 *   Parameters: Timeout
 *   IDs: -
 *
 * OUTPUT:
 *   Parameters: -
 *   IDs: -
 */
void TCmdFreezeTester::ExecuteL()
	{
	TInt timeout = ExtractIntegerL( KParamTimeout );
	
	// Freeze SIPTester thread but first return from the wrapper
	iContext.IssueFreezeL( timeout );
	}

TBool TCmdFreezeTester::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("FreezeTester") );
	}

TTcCommandBase* TCmdFreezeTester::CreateL( MTcTestContext& aContext )
	{
	return new ( ELeave ) TCmdFreezeTester( aContext );
	}

