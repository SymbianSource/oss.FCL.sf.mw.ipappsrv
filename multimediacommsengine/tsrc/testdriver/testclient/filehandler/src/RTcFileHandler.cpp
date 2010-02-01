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

#include "RTcFileHandler.h"
#include "FileHandlerConstants.h"

EXPORT_C RTcFileHandler::RTcFileHandler()
	: iIsConnected( EFalse )
	{
	}

EXPORT_C void RTcFileHandler::Close()
	{
	// Close the session if we are connected
	if( iIsConnected )
		{
		SendReceive( ECloseSession, TIpcArgs() );
				
		RHandleBase::Close();
		iIsConnected = EFalse;
		}
	}

EXPORT_C TInt RTcFileHandler::Connect()
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
		status = CreateSession( KTcFileHandlerName,
								Version(),
								KTcFileHandlerMessageSlots );

		// if connection was established succesfully, go configure the session.
		if( status == KErrNone )
			{
			iIsConnected = ETrue;

			return SendReceive( EConfigAndStart,
								TIpcArgs() );
			}
		}

	return status;
	}

EXPORT_C TInt RTcFileHandler::CreateFile( 
    const TDesC8& aDestinationPath, 
    const TDesC8& aData )
	{
	return SendReceive( ECreateFile, TIpcArgs( &aDestinationPath, &aData ) );
	}
	
EXPORT_C TInt RTcFileHandler::CopyFile( 
    const TDesC8& aSourcePath,
    const TDesC8& aDestinationPath )
    {
    return SendReceive( ECopyFile, TIpcArgs( &aSourcePath, &aDestinationPath ) );
    }
	
EXPORT_C TInt RTcFileHandler::DeleteFile( const TDesC8& aDestinationPath )
	{
	return SendReceive( EDeleteFile, TIpcArgs( &aDestinationPath ) );
	}

EXPORT_C TVersion RTcFileHandler::Version() const
	{
	TVersion version( KTcFileHandlerMajorVersion,
					  KTcFileHandlerMinorVersion,
					  KTcFileHandlerBuildVersion );
	return version;
	}

void RTcFileHandler::StartServerL()
	{
	if( !IsServerStarted() )
		{
		// Create a global semaphore for waiting the server to start
		RSemaphore semaphore;
		TInt status = semaphore.OpenGlobal( KTcFileHandlerName );
		if( status == KErrNotFound )
			{
			User::LeaveIfError( semaphore.CreateGlobal( KTcFileHandlerName, 0 ) );
			}
		CleanupClosePushL( semaphore );

		// We need to create a thread on WINS and a new process on ARMI/THUMB.
#if ( defined (__WINS__) && !defined (EKA2) )

		// Load the server dll
		RLibrary library;
		User::LeaveIfError( library.Load( KTcFileHandlerName,
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
		User::LeaveIfError( server.Create( KTcFileHandlerName,
										   entry,
										   KDefaultStackSize,
										   NULL,
										   &library,
										   NULL,
										   0x4000,
										   0x100000,
										   EOwnerProcess
										   ) );
        
										   
		// If successful, server thread has handle to library now
		CleanupStack::PopAndDestroy();	// library

#else
		// Load the server executable.
		// Path (\System\Programs or \sys\bin) and extension (.exe) are added automatically
		RProcess server;
		User::LeaveIfError( server.Create( KTcFileHandlerName, 
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

TBool RTcFileHandler::IsServerStarted() const
	{
	TFindServer findServer( KTcFileHandlerName );
	TFullName name;
	return ( findServer.Next( name ) == KErrNone );
	}
