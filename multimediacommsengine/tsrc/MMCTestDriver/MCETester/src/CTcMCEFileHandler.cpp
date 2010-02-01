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
* Description:    Implementation.
*
*/

#include "CTcMCEFileHandler.h"

_LIT(KDateString,"%D%M%Y%/0%1%/1%2%/2%3%/3 %-B%:0%J%:1%T%:2%S%:3%+B");

CTcMCEFileHandler::CTcMCEFileHandler()
	{
	// No implementation required
	}

CTcMCEFileHandler::~CTcMCEFileHandler()
	{
	iFsSession.Close();
	}

CTcMCEFileHandler* CTcMCEFileHandler::NewLC()
	{
	CTcMCEFileHandler* self = new (ELeave) CTcMCEFileHandler();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CTcMCEFileHandler* CTcMCEFileHandler::NewL()
	{
	CTcMCEFileHandler* self = CTcMCEFileHandler::NewLC();
	CleanupStack::Pop(); // self;
	return self;
	}

void CTcMCEFileHandler::ConstructL()
	{
	User::LeaveIfError(iFsSession.Connect());
	}

void CTcMCEFileHandler::FindFile(const TDesC& aPath, 
			const TDesC& aFileName, CDesC8Array* aArray)
	{
	CDirScan* DirScan = CDirScan::NewLC(iFsSession);
	DirScan->SetScanDataL(aPath, 
		KEntryAttDir|KEntryAttMatchExclusive, 
		ESortNone, CDirScan::EScanDownTree);
	 
	while(true)
		{
		CDir* dir = NULL;
		TRAPD(error, DirScan->NextL(dir));
		if (error || !dir)
			break;
		
		delete dir;
 
		ScanDirectoryL(DirScan->FullPath(), aFileName, aArray);
		};
	
	CleanupStack::PopAndDestroy(DirScan);

	}

TInt CTcMCEFileHandler::GetFilesFromFolderL(TDesC& aFolder,CDesC8Array* aArray)
	{
	if(aArray == NULL)
		return KErrArgument;
	
	CDir* FileList(NULL);
	TInt res = iFsSession.GetDir(aFolder,
		KEntryAttMaskSupported, EDirsFirst, FileList);
	if( res == KErrNone )	
		{
			if(FileList && aArray)
			{
				FileList->Sort(ESortByName);
				for(TInt i=0; i < FileList->Count(); i++)
				{
					if((*FileList)[i].IsSystem() 
					|| (*FileList)[i].IsHidden()
					|| (*FileList)[i].IsDir())
					{
						// ignore System & Hidden Files & folders
					}
					else
					{
						TBuf8<255> buf8;
						buf8.Copy((*FileList)[i].iName);
						//add the file name to the array
						aArray->AppendL(buf8);
					}
				}
			}
		
			delete FileList;
			FileList = NULL;
		}
	return res;
	}

TBool CTcMCEFileHandler::Delete(const TDesC& aName, TUint aSwitch)
	{
	CFileMan* fileMan=CFileMan::NewL(iFsSession);
	CleanupStack::PushL(fileMan);
 
	TInt err=fileMan->Delete(aName,aSwitch);
	CleanupStack::PopAndDestroy(fileMan);
 
	if(err==KErrNone)
		return ETrue;
	else
		return EFalse;

	}


TBool CTcMCEFileHandler::CheckFileExists(const TDesC& aFileName)
	{
	TParse p;
	// do isolated parse
	User::LeaveIfError(p.Set(aFileName, NULL, NULL));
	    	
	User::LeaveIfError(iFsSession.Parse(aFileName, p));
	
	TBool result = BaflUtils::FileExists(iFsSession, aFileName);
	
	if(result)
		return ETrue;
	else
		return EFalse;
	}

//name: value
void CTcMCEFileHandler::GetFileInfoL(const TDesC& aFileName, CDesC8Array* aArray)
	{
	if(aArray == NULL)
		User::LeaveIfError(KErrArgument);

	if(CheckFileExists(aFileName) == EFalse)
		User::LeaveIfError(KErrNotFound);
	
    _LIT8(KFileName,"File Name:");
    _LIT8(KDateModified,"Date Modified:");
    _LIT8(KFileSize,"File Size:");
    _LIT8(KFileAttrib,"Entry details:");
	
	TParse parse;
	User::LeaveIfError(parse.Set(aFileName, NULL, NULL));
	
    //Consists of a drive and path
    iFsSession.SetSessionPath( parse.DriveAndPath() );   
    
    TFileName name( parse.NameAndExt() );
		
	// Get entry details for file and print them    
	TEntry entry;  
	User::LeaveIfError(iFsSession.Entry(name, entry)); 
	
	TBuf<30> dateString;
	entry.iModified.FormatL(dateString, KDateString);
	
	TBuf8<255> buf8;
	//file name
	aArray->AppendL( KFileName );
	buf8.Copy( parse.NameAndExt() );
	aArray->AppendL( buf8 );
	buf8.Zero();
	//date modified
	aArray->AppendL( KDateModified );
	buf8.Copy(dateString);
	aArray->AppendL( buf8 );
	buf8.Zero();
	
	//file size
	TBuf8< 12 > convert;
	convert.AppendNum( entry.iSize );
	aArray->AppendL( KFileSize );
	aArray->AppendL( convert );
	
	TBuf8<15> buffer;
	FormatEntry(buffer,entry); // Archive attribute should be set
	//attribs
	aArray->AppendL(KFileAttrib);
	aArray->AppendL(buffer);
	}

void CTcMCEFileHandler::FormatEntry(TDes8& aBuffer, const TEntry& aEntry)
    {
    _LIT(KReadOnly,"Read-only");
    _LIT(KHidden,"Hidden");
    _LIT(KSystem,"System");
    _LIT(KDirectory,"Directory");
    _LIT(KArchive,"Archive");
    
    if(aEntry.IsReadOnly())
        aBuffer.Append(KReadOnly);
    if(aEntry.IsHidden())
        aBuffer.Append(KHidden);
    if(aEntry.IsSystem())
        aBuffer.Append(KSystem);
    if(aEntry.IsDir())
        aBuffer.Append(KDirectory);
    if(aEntry.IsArchive())
        aBuffer.Append(KArchive);
    }

void CTcMCEFileHandler::ScanDirectoryL(const TDesC& aDir,
		const TDesC& aWildCard, CDesC8Array* aArray)
    {
    TParse parse;
    parse.Set(aWildCard, &aDir, NULL);
    TPtrC spec(parse.FullName());
 
    TFindFile FindFile(iFsSession);
    CDir* dir;
 
    if (!FindFile.FindWildByPath(parse.FullName(), NULL, dir))
        {
        CleanupStack::PushL(dir);
 
        for(TInt i = 0; i < dir->Count(); i++)
            {
            parse.Set((*dir)[i].iName, &spec, NULL);
            TBuf8<255> buf8;
            buf8.Copy(parse.FullName());
            aArray->AppendL(buf8);
            }
        CleanupStack::PopAndDestroy(dir);
        }
    }
