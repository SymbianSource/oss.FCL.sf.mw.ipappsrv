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
* Description:  Common constant definitions for Watcher
*
*/

#ifndef __WATCHERCONSTANTS_H__
#define __WATCHERCONSTANTS_H__

// INCLUDES
#include <e32std.h>

/// Watcher log name
_LIT( KTcLogName, "TestClientWatcher.log" );
#define LOG_NAME_DEFINED

/**
 * Unique name for the server used as:
 * - server name
 * - server executable file name (+.exe)
 * - server thread name on WINS
 * - server semaphore name
 */
_LIT( KTcWatcherName, "WatcherServ" );

/**
 * The server version. A version must be specifyed when creating
 * a session with the server. It is used to verify that the
 * client dll is compatible with the server. Doesn't matter
 * much in our case.
 */
const TUint KTcWatcherMajorVersion = 1;
const TUint KTcWatcherMinorVersion = 0;
const TUint KTcWatcherBuildVersion = 0;

/// Opcodes used in message passing between client and server.
/// Used to identify functions in the server sessions.
enum TTcWatcherRequest
	{
	/// Client-server framework opcodes
	ECloseSession,
	EConfigAndStart,
	EPing
	};

/// Number of message slots (1 read, 1 write, 1 cancel, 1 spare)
/// As recommended by Symbian.
const TUint KTcWatcherMessageSlots( 4 );

/// Time to wait before trying to restart
/// a dying server in RTcWatcher::StartServer()
const TInt KTcServerDyingTime( 100000 );

// Shut down server after 10 seconds have elapsed since last client disconnect
const TInt KTcShutdownTime( 10000000 );

/// One second as microseconds
const TInt KSecondAsMicroseconds( 1000000 );

#ifdef __WINS__
/// Default heap size for the server thread on WINS = 16Kb
const TUint KTcDefaultHeapSize( 0x4000 );
/// Maximum heap size for the server thread on WINS = 1Mb
const TUint KTcMaxHeapSize( 0x100000 );
#endif

/**
 * Global function for PANICing the server
 *
 * @param aPanic Panic code
 */
void PanicServer( TInt aPanic );

#endif // __WATCHERCONSTANTS_H__
