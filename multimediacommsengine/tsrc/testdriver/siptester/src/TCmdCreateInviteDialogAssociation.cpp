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
#include <sipsubscriptionstateheader.h>
#include <sipeventheader.h>

#include "TCmdCreateInviteDialogAssociation.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	-
 *   IDs:			ServerTransactionId
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			InviteDialogId
 */
void TCmdCreateInviteDialogAssociation::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get SIP objects from registry
	CSIPServerTransaction* serverTransaction = GetServerTransactionL();

	// -- Execution -----------------------------------------------------------

	// Create an Invite dialog association
	CSIPInviteDialogAssoc* dialogAssoc =
					CSIPInviteDialogAssoc::NewLC( *serverTransaction );
					
	// -- Response creation ---------------------------------------------------

	CleanupStack::Pop( 1 ); //dialogAssoc
	AddIdResponseL( KInviteDialogId, dialogAssoc );
	}

TBool TCmdCreateInviteDialogAssociation::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("CreateInviteDialogAssociation") );
	}

TTcCommandBase* TCmdCreateInviteDialogAssociation::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave )TCmdCreateInviteDialogAssociation( aContext );
	}

