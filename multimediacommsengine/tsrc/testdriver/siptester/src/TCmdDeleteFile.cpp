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

#include "CTcFileHandler.h"

#include "TCmdDeleteFile.h"
#include "SIPConstants.h"
#include "CTcSIPContext.h"

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	File
 *   IDs:			-
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			-
 */
void TCmdDeleteFile::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------
    
    // Use FileHandler server for deleting since SIPTester cannot have access
    // to /sys and /resource directories
    TPtrC8 fileName = ExtractTextL( KParamFile );
    iContext.FileHandler().DeleteFileL( fileName ); 
    
    
    /*
	// Get file name to be deleted, convert to unicode
	TFileName fileName16;
	fileName16.Copy( ExtractTextL( KParamFile ) );

	// -- Execution -----------------------------------------------------------

	// Connect to file server
	RFs fs;
	User::LeaveIfError( fs.Connect() );
	CleanupClosePushL( fs );

	// Clear read-only and system flags in order to be able to delete the file
	// Ignore errors.
	fs.SetAtt( fileName16, 0, KEntryAttReadOnly | KEntryAttSystem );

	// Try deleting the file, report errors (typically KErrNotFound)
	User::LeaveIfError( fs.Delete( fileName16 ) );

	// Close file server session
	CleanupStack::PopAndDestroy();	// fs
	*/

	// -- Response creation ---------------------------------------------------
	}

TBool TCmdDeleteFile::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("DeleteFile") );
	}

TTcCommandBase* TCmdDeleteFile::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdDeleteFile( aContext );
	}

