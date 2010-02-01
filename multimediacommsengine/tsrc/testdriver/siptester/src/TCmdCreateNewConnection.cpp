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
#include "TCmdCreateNewConnection.h"

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	IAPName
 *   IDs:			-
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			ConnectionId
 */
void TCmdCreateNewConnection::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get mandatory IAPName
	TPtrC8 name = ExtractTextL( KParamIAPName );

	// -- Execution -----------------------------------------------------------

	// Create a new connection container (+connection) and store to registry
	CTcSIPConnectionContainer* conn =
						CTcSIPConnectionContainer::NewL( iContext, name );

	// -- Response creation ---------------------------------------------------

	AddIdResponseL( KConnectionId, conn );
	}

TBool TCmdCreateNewConnection::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("CreateNewConnection") );
	}

TTcCommandBase* TCmdCreateNewConnection::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdCreateNewConnection( aContext );
	}
