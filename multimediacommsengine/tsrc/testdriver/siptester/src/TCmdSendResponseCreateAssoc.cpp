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

#include <sipinvitedialogassoc.h>
#include <sipservertransaction.h>

#include "TCmdSendResponseCreateAssoc.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		Contact*, Content-Type*, Content-Encoding*
 *   Parameters:	StatusCode, Reason, Content*
 *   IDs:			ServerTransactionId
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			InviteDialogId
 */
void TCmdSendResponseCreateAssoc::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get SIP objects from registry
	CSIPServerTransaction* serverTransaction = GetServerTransactionL();

	// -- Execution -----------------------------------------------------------

	// Create an invite dialog association
	CSIPInviteDialogAssoc* dialogAssoc =
							CSIPInviteDialogAssoc::NewLC( *serverTransaction );

	// Use base class to do the hard work
	TCmdSendResponse::ExecuteL();

	// -- Response creation ---------------------------------------------------

	CleanupStack::Pop( dialogAssoc );
	AddIdResponseL( KInviteDialogId, dialogAssoc );
	}

TBool TCmdSendResponseCreateAssoc::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("SendResponseCreateAssoc") );
	}

TTcCommandBase* TCmdSendResponseCreateAssoc::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendResponseCreateAssoc( aContext );
	}

