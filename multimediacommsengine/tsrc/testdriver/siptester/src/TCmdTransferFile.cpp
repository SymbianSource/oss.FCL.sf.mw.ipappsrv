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
#include <bautils.h>

#include "CTcFileHandler.h"

#include "TCmdTransferFile.h"
#include "SIPConstants.h"
#include "CTcSIPContext.h"

// Literals used for action name
_LIT8( KWrite, "write" );
_LIT8( KDelete, "delete" );
_LIT8( KExists, "exists" );


/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	ActionName, *SrcPath, DestinationPath, *Data
 *   IDs:			-
 *
 * OUTPUT:
 *   Parameters:	StatusCode
 *   IDs:			-
 */
void TCmdTransferFile::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

    // Get action name for operation (e.g. write&read)
    TPtrC8 actionName = ExtractTextL( KParamActionName );

    // Get parameter destination path
    TPtrC8 sourcePath = ExtractTextL( KParamSourcePath, EFalse );
    
	// Get parameter destination path
    TPtrC8 destinationPath = ExtractTextL( KParamDestinationPath );
    
	// Get actual data. Data parameter is having data in binary format.
	TPtrC8 data = ExtractTextL( KParamData, EFalse );
	
	// Extract content. Content parameter is having textual data.
	HBufC8* content = ExtractHBufLC( KParamContent );
	if ( !content )
	    {
	    // Create dummy zero length content
	    content = HBufC8::NewLC( 0 );
	    }
	    

	// -- Execution -----------------------------------------------------------
 
    // Compare the operation and do it
    if( !actionName.CompareC( KWrite ) )
		{
		if ( sourcePath.Compare( KNullDesC8 ) != 0 )
		    {
		    // If source path was defined copy file from sourcepath
		    // to destinationpath
		    DoCopyL( sourcePath, destinationPath );
		    }
		else if ( content->Length() != 0 )
		    {
		     // Otherwise write received text data to destinationpath
		    TPtrC8 ptr(content->Right(content->Length()));
		    DoWriteL( destinationPath, ptr );   
		    }		    
		else if ( data.Compare( KNullDesC8 ) != 0 )
		    {
		    // Otherwise write received data to destinationpath
		    DoWriteL( destinationPath, data );   
		    }
		else
		    {
		    User::Leave( KErrNotFound );
		    }
		}
    else if( !actionName.CompareC( KExists ) )
    	{
    	TFileName filename;
    	filename.Copy(destinationPath);
    	if( CheckFileExists(filename) )
			{
			AddBooleanResponseL(_L8("FileExists"), ETrue);
			}
		else
			{
			AddBooleanResponseL(_L8("FileExists"), EFalse);
			}
    	}
    else if( !actionName.CompareC( KDelete ) )
    	{   
    	TParse parse;
    	TFileName name;
    	name.Copy(destinationPath);
    	parse.Set(name, NULL, NULL);
    	if( parse.DrivePresent() && parse.PathPresent() && parse.NameOrExtPresent() )
    		{
    		iContext.FileHandler().DeleteFileL(destinationPath);
    		}
    	else
    		{
    		User::Leave( KErrArgument);
    		}
    	}
    else { User::Leave( KErrNotSupported ); }
    
    // -- Response creation ---------------------------------------------------
	
    AddIntegerResponseL( KResponseActionReturnCode, KSIPReasonOk );
    CleanupStack::Pop(1);
    }


TBool TCmdTransferFile::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("fileXfer") );
	}

TTcCommandBase* TCmdTransferFile::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdTransferFile( aContext );
	}

void TCmdTransferFile::DoWriteL( TPtrC8& aDestinationPath, TPtrC8& aData )
    {
    // Use FileHandler server for writing since SIPTester cannot have access
    // to /sys and /resource directories
    iContext.FileHandler().CreateFileL( aDestinationPath, aData );
    }
    
void TCmdTransferFile::DoCopyL( TPtrC8& aSourcePath, TPtrC8& aDestinationPath )
    {
    iContext.FileHandler().CopyFileL( aSourcePath, aDestinationPath );
    }

TDesC8 TCmdTransferFile::DoReadL( TFileName& aDestinationPath )
    {
    // Read the document into a buffer.
	RFs fs;
	User::LeaveIfError( fs.Connect() );
	CleanupClosePushL( fs );

	RFile in;
	User::LeaveIfError( in.Open( fs, aDestinationPath, 
						EFileRead ) );
	CleanupClosePushL( in );

    //TBuf8< KTcRequestMaxLength > data; // 5120
    //TBuf8< KTcResponseMaxLength > data; // 40960
    TBuf8< 20 > data;
	User::LeaveIfError( in.Read( data ) );

    in.Close();
    fs.Close();
	CleanupStack::PopAndDestroy( 2 );	// in, fs

    return data;
    }

TBool TCmdTransferFile::CheckFileExists(const TDesC& aFileName)
	{
	RFs fs;
	User::LeaveIfError( fs.Connect() );
	CleanupClosePushL( fs );
	TParse p;
	// do isolated parse
	User::LeaveIfError(p.Set(aFileName, NULL, NULL));
	    	
	User::LeaveIfError(fs.Parse(aFileName, p));
	
	TBool result = BaflUtils::FileExists(fs, aFileName);
	
	fs.Close();
	CleanupStack::PopAndDestroy( 1 );
	
	if(result)
		return ETrue;
	else
		return EFalse;
	}
