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

#include "RTcWatcher.h"
#include "WatcherConstants.h"

EXPORT_C RTcWatcher::RTcWatcher()
	: iIsConnected( EFalse )
	{
	}

EXPORT_C void RTcWatcher::Close()
	{
	// Close the session if we are connected
	if( iIsConnected )
		{
		SendReceive( ECloseSession,
#ifdef __IPC_V2_PRESENT__
					 TIpcArgs()
#else
					 NULL
#endif
					 );
				
		RHandleBase::Close();
		iIsConnected = EFalse;
		}
	}

EXPORT_C TInt RTcWatcher::Connect( const TDesC& aAppName, TInt aTimeout )
	{
	// Are we already connected?
	if( iIsConnected )
		{
		return KErrAlreadyExists;
		}

	// Try starting the server, it is safe
	// to call StartServer() even if a server is already running.
	TRAPD( status, StartServerL() );

	// If server was started (or was already running),
	// establish a session to it.
	if( status == KErrNone )
		{
		status = CreateSession( KTcWatcherName,
								Version(),
								KTcWatcherMessageSlots );

		// if connection was established succesfully, go configure the session.
		if( status == KErrNone )
			{
			iIsConnected = ETrue;

#ifdef __IPC_V2_PRESENT__
			return SendReceive( EConfigAndStart,
								TIpcArgs( &aAppName, aTimeout ) );
#else
			TInt p[ KMaxMessageArguments ] =
					{
					(TInt)const_cast< TDesC*>( &aAppName ),
					aTimeout
					};
			status = SendReceive( EConfigAndStart, p );
#endif

			if( status != KErrNone )
				{
				Close();
				}
			}
		}

	return status;
	}

EXPORT_C TInt RTcWatcher::Ping()
	{
	return SendReceive( EPing,
#ifdef __IPC_V2_PRESENT__
						TIpcArgs()
#else
						NULL
#endif
						);
	}

EXPORT_C TVersion RTcWatcher::Version() const
	{
	TVersion version( KTcWatcherMajorVersion,
					  KTcWatcherMinorVersion,
					  KTcWatcherBuildVersion );
	return version;
	}

void RTcWatcher::StartServerL()
	{
	// Let the server have a change at dying if it was just doing so :-)
	if( IsServerStarted() )
		{
		User::After( KTcServerDyingTime );
		}

	if( !IsServerStarted() )
		{
		// Create a global semaphore for waiting the server to start
		RSemaphore semaphore;
		TInt status = semaphore.OpenGlobal( KTcWatcherName );
		if( status == KErrNotFound )
			{
			User::LeaveIfError( semaphore.CreateGlobal( KTcWatcherName, 0 ) );
			}
		CleanupClosePushL( semaphore );

		// We need to create a thread on WINS and a new process on ARMI/THUMB.
#if ( defined (__WINS__) && !defined (EKA2) )

		// Load the server dll
		RLibrary library;
		User::LeaveIfError( library.Load( KTcWatcherName,
							   TUidType( KNullUid, KNullUid, KNullUid ) ) );
		CleanupClosePushL( library );

		// Look up the entry point function (always at ordinal #1)
		TLibraryFunction first = library.Lookup( 1 );
		TThreadFunction entry = reinterpret_cast< TThreadFunction >( first() );
		if( !entry )
			{
			User::Leave( KErrBadLibraryEntryPoint );
			}

		// We have to simulate the process with a thread on WINS
		RThread server;
		User::LeaveIfError( server.Create( KTcWatcherName,
										   entry,
										   KDefaultStackSize,
										   NULL,
										   &library,
										   NULL,
										   KTcDefaultHeapSize,
										   KTcMaxHeapSize,
										   EOwnerProcess
										   ) );
        
										   
		// If successful, server thread has handle to library now
		CleanupStack::PopAndDestroy();	// library

#else
		// Load the server executable.
		// Path (\System\Programs or \sys\bin) and extension (.exe) are added automatically
		RProcess server;
		User::LeaveIfError( server.Create( KTcWatcherName, 
		                                   KNullDesC, 
		                                   TUidType( KNullUid, KNullUid, KNullUid ) ) );
#endif

		CleanupClosePushL( server );

		// Start executing the server.
		server.Resume();
		// Wait until it has initialized.
		semaphore.Wait();

		// Close handles
		CleanupStack::PopAndDestroy( 2 );	// server, semaphore
		}

	}

TBool RTcWatcher::IsServerStarted() const
	{
	TFindServer findServer( KTcWatcherName );
	TFullName name;
	return ( findServer.Next( name ) == KErrNone );
	}
