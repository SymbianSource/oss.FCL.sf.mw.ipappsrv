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
#include "TCmdDeleteConnection.h"

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	-
 *   IDs:			ConnectionId
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			-
 */
void TCmdDeleteConnection::ExecuteL()
	{
	// Delete connectioncontainer (+connection) from registry
	// Wrapper can be used only for deleting connections created with
	// CreateNewConnection wrapper (default connection cannot be deleted)
	//
	TPtrC8 value = ExtractIdL( KConnectionId, ETrue );

	iContext.Registry().DestroyObject( value );
	}

TBool TCmdDeleteConnection::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("DeleteConnection") );
	}

TTcCommandBase* TCmdDeleteConnection::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdDeleteConnection( aContext );
	}
