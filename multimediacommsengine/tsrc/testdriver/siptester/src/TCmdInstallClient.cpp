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

#include <f32file.h>
#include <sipclientinstaller.h>

#include "TCmdInstallClient.h"
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
void TCmdInstallClient::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get ClientInfo XML document and write it to a file
	PrepareFileL();

	// -- Execution -----------------------------------------------------------

	// Create SIP client installer and install
	CSIPClientInstaller* clientInstaller =
									CSIPClientInstaller::NewLC( KInstallerPath );
	clientInstaller->InstallL();
	CleanupStack::PopAndDestroy(); // clientInstaller

	// -- Response creation ---------------------------------------------------
	}

void TCmdInstallClient::PrepareFileL()
	{
	// Get ClientInfo XML document
	TPtrC8 info = ExtractTextL( KParamClientInfo );

	// Write the document into a file.
	RFs fs;
	User::LeaveIfError( fs.Connect() );
	CleanupClosePushL( fs );

	RFile out;
	User::LeaveIfError( out.Replace( fs, CSIPClientInstaller::XMLFileName(),
						EFileWrite ) );
	CleanupClosePushL( out );
	User::LeaveIfError( out.Write( info ) );

    out.Close();
    fs.Close();
	CleanupStack::PopAndDestroy( 2 );	// out, fs
	}

TBool TCmdInstallClient::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("InstallClient") );
	}

TTcCommandBase* TCmdInstallClient::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdInstallClient( aContext );
	}

