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

#include <sipclientinstaller.h>

#include "TCmdUninstallClient.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	ClientInfo
 *   IDs:			-
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			-
 */
void TCmdUninstallClient::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get ClientInfo XML document and write it to a file
	PrepareFileL();

	// -- Execution -----------------------------------------------------------

	// Create SIP client installer and uninstall
	CSIPClientInstaller* clientInstaller =
									CSIPClientInstaller::NewLC( KInstallerPath );
	clientInstaller->UnInstallL();
	CleanupStack::PopAndDestroy(); // clientInstaller

	// -- Response creation ---------------------------------------------------
	}

TBool TCmdUninstallClient::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("UninstallClient") );
	}

TTcCommandBase* TCmdUninstallClient::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdUninstallClient( aContext );
	}

