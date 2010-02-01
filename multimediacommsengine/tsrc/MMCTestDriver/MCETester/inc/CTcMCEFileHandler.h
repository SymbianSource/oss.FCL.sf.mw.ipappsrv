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
* Description:    See class definition below.
*
*/

#ifndef CTCMCEFILEHANDLER_H
#define CTCMCEFILEHANDLER_H

// INCLUDES

#include <f32file.h>
#include <bautils.h>
#include <e32std.h>
#include <e32base.h>

// CLASS DECLARATION

/**
 *  CCTcMCEFileHandler
 *  
 *  Class for small file operations
 * 
 */
class CTcMCEFileHandler : public CBase
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CTcMCEFileHandler();

	/**
	 * Two-phased constructor.
	 */
	static CTcMCEFileHandler* NewL();

	/**
	 * Two-phased constructor.
	 */
	static CTcMCEFileHandler* NewLC();

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CTcMCEFileHandler();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();	

public:	//user functions
	
    /**
    * Finds a files in the given path and file name or wildcard
    * Note that the trailing backslash is required to specify the directory 
    * I.e. path x:\dir1\dir2\
    * @param aPath The top level directory for the scan including the drive letter
    * @param aFileName name of the file or wildcard I.e. *.txt
    * @param aArray returned file list
    */
	void FindFile(const TDesC& aPath, const TDesC& aFileName, CDesC8Array* aArray);
	
	/**
	 * Function for reading all files from a specified folder.
	 * @param aFolder
	 * @param aArray returned file names. aArray must be initialized before function call
	 * @return error code. KErrNone if no error
	 */
	TInt GetFilesFromFolderL(TDesC& aFolder,CDesC8Array* aArray);

    /**
    * Deletes the file from the file system
    * @param aName fullname of the (including the path)
    * @param aSwitch specify: zero for no recursion; CFileMan::ERecurse for recursion
    * @return ETrue if there was no errors
    */
	TBool Delete(const TDesC& aName, TUint aSwitch=0); 
    
	/**
    * Checks if file exists in the file system
    * @param aFileName fullname of the (including the path)
    * @return ETrue if file was found or EFalse otherwise 
    */
	TBool CheckFileExists(const TDesC& aFileName);
	
	/**
	 * Gets the file information (size, date, attributes)
	 * @param aFileName fullname of the (including the path)
	 * @param aArray with file information returned by the pointer.
	 * response structure: aArray[n]=attribute_name: aArray[n+1]=value
	 */
	void GetFileInfoL(const TDesC& aFileName, CDesC8Array* aArray);

public:	//internal
	
	void FormatEntry(TDes8& aBuffer, const TEntry& aEntry);
	
	void ScanDirectoryL(
	   const TDesC& aDir, const TDesC& aWildCard, CDesC8Array* aArray);

	
private:
	RFs iFsSession;
	TFileName iFileName;
	};

#endif // CTCMCEFILEHANDLER_H
