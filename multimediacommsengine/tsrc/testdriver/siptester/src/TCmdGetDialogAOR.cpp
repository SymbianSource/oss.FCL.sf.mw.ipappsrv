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

#include <sipaddress.h>
#include <sipdialog.h>
#include <sipdialogassocbase.h>
#include <siptoheader.h>

#include "TCmdGetDialogAOR.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	-
 *   IDs:			InviteDialogId*, SubscribeDialogId*
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			Address
 */
void TCmdGetDialogAOR::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get SIP objects from registry
	CSIPDialogAssocBase* dialogAssoc = GetAnyDialogAssocL();

	// -- Execution -----------------------------------------------------------

	// Get dialog To address
	HBufC8* aor = dialogAssoc->Dialog().ToHeader().SIPAddress().ToTextLC();

	// -- Response creation ---------------------------------------------------

	AddTextResponseL( KParamAddress, aor->Des() );
	CleanupStack::PopAndDestroy( aor );
	}

TBool TCmdGetDialogAOR::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("GetDialogAOR") );
	}

TTcCommandBase* TCmdGetDialogAOR::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetDialogAOR( aContext );
	}
