/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Implementation
*
*/




#include <flogger.h>
#include "TCmdFileHandler.h"
#include "MCEConstants.h"


/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	-
 *   IDs:			-
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			-
 */


void TCmdFileHandler::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------
	TPtrC8 fileName_GetInfo = ExtractTextL( KParamGetFileInfo, EFalse );
	
	TPtrC8 fileName_Delete = ExtractTextL( KParamDeleteFile, EFalse );
	
	TPtrC8 fileName_Exists = ExtractTextL( KParamCheckFileExists, EFalse );
	
	TPtrC8 dirName = ExtractTextL( KParamGetFilesFromFolder, EFalse );

	// -- Execution -----------------------------------------------------------
	iHandler = CTcMCEFileHandler::NewL();
	
	//delete the file	
	if( fileName_Delete.Compare( KNullDesC8 ) != 0 ) 
		{
		TFileName filename;
		filename.Copy(fileName_Delete);
		if(iHandler->Delete(filename) == EFalse) User::LeaveIfError(KErrNotFound);
		}
	
	//check if file exists
	if( fileName_Exists.Compare( KNullDesC8 ) != 0 ) 
		{
		TFileName filename;
		filename.Copy(fileName_Exists);
		
		if(iHandler->CheckFileExists(filename))
			{
			AddBooleanResponseL(_L8("FileExists"), ETrue);
			}
		else
			{
			AddBooleanResponseL(_L8("FileExists"), EFalse);
			}
		}
	
	//get file info
	if( fileName_GetInfo.Compare( KNullDesC8 ) != 0 ) 
		{
		TFileName filename;
		filename.Copy(fileName_GetInfo);
		
		CDesC8Array* fileInfo = new(ELeave)CDesC8ArraySeg(1);
		CleanupStack::PushL(fileInfo);
		iHandler->GetFileInfoL(filename, fileInfo);
		
		AddArrayResponseL( _L8("FileInfo"), *fileInfo );
		
		CleanupStack::PopAndDestroy(fileInfo);
		}
	
	//get files from folder
	if( dirName.Compare( KNullDesC8 ) != 0 ) 
		{
		TFileName folderName;
		folderName.Copy(dirName);
		
		CDesC8Array* files = new(ELeave)CDesC8ArraySeg(1);
		CleanupStack::PushL(files);
		iHandler->GetFilesFromFolderL(folderName, files);
		
		AddArrayResponseL( _L8("FilesFromFolder"), *files );
		
		CleanupStack::PopAndDestroy(files);
		}
	
	//find file
	CTcStructure* structure;
	//get the folder name and file name from the structure
	do
		{
		structure = FindStructureL( KParamFindFile, EFalse );
		if( structure )
			{
			CTcNameValue* item;
			//structure contains a list of name-value pairs
			for (int i=0; i < structure->Count(); i++)
				{
				item = structure->ItemL( i );
				if( item )break;
				}			
			if( item )
				{
				//get the folder name
				TFileName folderName;
				folderName.Copy(item->Name());
				//name of the file or wildcard
				TFileName fileName;
				fileName.Copy(item->Value());
				
				CDesC8Array* fileList = new(ELeave)CDesC8ArraySeg(1);
				CleanupStack::PushL(fileList);
				iHandler->FindFile(folderName, fileName, fileList);
					
				AddArrayResponseL( _L8("FileList"), *fileList );
					
				CleanupStack::PopAndDestroy(fileList);
				
				iContext.List().RemoveParameter( KParamFindFile );
				break;
				}
			}
	} while( structure );
	
	// -- Response creation ---------------------------------------------------
	delete iHandler;
	}

TBool TCmdFileHandler::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("FileHandler") );
	}

TTcCommandBase* TCmdFileHandler::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdFileHandler( aContext );
	}
