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
* Description:  Common constant definitions for FileHandler
*
*/

#ifndef __FILEHANDLERCONSTANTS_H__
#define __FILEHANDLERCONSTANTS_H__

// INCLUDES
#include <e32std.h>

/**
 * Unique name for the server used as:
 * - server name
 * - server executable file name (+.exe)
 * - server thread name on WINS
 * - server semaphore name
 */
_LIT( KTcFileHandlerName, "TCFileHandlerServ" );

/**
 * The server version. A version must be specifyed when creating
 * a session with the server. It is used to verify that the
 * client dll is compatible with the server. Doesn't matter
 * much in our case.
 */
const TUint KTcFileHandlerMajorVersion = 1;
const TUint KTcFileHandlerMinorVersion = 0;
const TUint KTcFileHandlerBuildVersion = 0;

/// Opcodes used in message passing between client and server.
/// Used to identify functions in the server sessions.
enum TTcFileHandlerRequest
	{
	/// Client-server framework opcodes
	ECloseSession,
	EConfigAndStart,
	ECreateFile,
	ECopyFile,
	EDeleteFile
	};

/// Number of message slots (1 read, 1 write, 1 cancel, 1 spare)
/// As recommended by Symbian.
const TUint KTcFileHandlerMessageSlots( 4 );


/**
 * Global function for PANICing the server
 *
 * @param aPanic Panic code
 */
void PanicServer( TInt aPanic );

#endif // __FILEHANDLERCONSTANTS_H__
