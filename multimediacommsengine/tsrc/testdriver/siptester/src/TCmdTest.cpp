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

#include "SIPConstants.h"
#include "TCmdTest.h"

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	TestPanic*, TestHang*
 *   IDs:			-
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			-
 */
void TCmdTest::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get optional TestPanic
	TPtrC8 wantPanic = ExtractTextL( KParamTestPanic, EFalse );

	// Get optional TestHang
	TPtrC8 wantHang = ExtractTextL( KParamTestHang, EFalse );

	// -- Execution -----------------------------------------------------------

	if( wantPanic != KNullDesC8 )
		{
		User::Panic( _L("TCmdTest"), 2004 );
		}

	if( wantHang != KNullDesC8 )
		{
		User::After( 100000000 );
		}

	// -- Response creation ---------------------------------------------------
	}

TBool TCmdTest::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("Test") );
	}

TTcCommandBase* TCmdTest::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdTest( aContext );
	}
