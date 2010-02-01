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
#include "SIPConstants.h"
#include "TCmdGetIAPName.h"

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	ConnectionId*
 *   IDs:			-
 *
 * OUTPUT:
 *   Parameters:	IAPName
 *   IDs:			-
 */
void TCmdGetIAPName::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Select connection; either default or user specified (and existing)
	CTcSIPConnectionContainer& conn = SelectConnectionL();

	// -- Execution -----------------------------------------------------------

	// -- Response creation ---------------------------------------------------

	AddTextResponseL( KParamIAPName, conn.IAPName() );
	}

TBool TCmdGetIAPName::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("GetIAPName") );
	}

TTcCommandBase* TCmdGetIAPName::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetIAPName( aContext );
	}

