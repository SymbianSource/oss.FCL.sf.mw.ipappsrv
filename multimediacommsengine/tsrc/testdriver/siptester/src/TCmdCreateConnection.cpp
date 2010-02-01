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
#include "TCmdCreateConnection.h"
#include "CTcSIPContext.h"

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
void TCmdCreateConnection::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get mandatory IAPName
	TPtrC8 name = ExtractTextL( KParamIAPName );

	// -- Execution -----------------------------------------------------------
    CTcSIPConnectionContainer* conn = NULL;
    if ( iContext.GetMandatoryValue() )
        {
	    conn = iContext.CreateSipStack( name );
        }

	// -- Response creation ---------------------------------------------------

	AddIdResponseL( KConnectionId, *conn ); //ownership is not transferred
	}

TBool TCmdCreateConnection::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("CreateConnection") );
	}

TTcCommandBase* TCmdCreateConnection::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdCreateConnection( aContext );
	}
