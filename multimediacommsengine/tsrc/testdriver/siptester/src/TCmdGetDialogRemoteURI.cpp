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

#include <sipdialogassocbase.h>
#include <sipdialog.h>
#include <uri8.h>

#include "TCmdGetDialogRemoteURI.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	-
 *   IDs:			InviteDialogId*, SubscribeDialogId*
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			RemoteURI*
 */

void TCmdGetDialogRemoteURI::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get SIP objects from registry
	CSIPDialogAssocBase* dialogAssoc = GetAnyDialogAssocL();

	// -- Execution -----------------------------------------------------------

	// Get dialog remote URI, if present
	const CUri8* remoteUri = &dialogAssoc->Dialog().RemoteURI();


	// -- Response creation ---------------------------------------------------

	AddTextResponseL( KParamRemoteURI, remoteUri->Uri().UriDes() );

	}

TBool TCmdGetDialogRemoteURI::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("GetDialogRemoteURI") );
	}

TTcCommandBase* TCmdGetDialogRemoteURI::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetDialogRemoteURI( aContext );
	}
