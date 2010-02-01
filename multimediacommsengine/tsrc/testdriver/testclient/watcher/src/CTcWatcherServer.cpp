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

#include "CTcWatcherServer.h"
#include "CTcWatcherSession.h"
#include "CTcAppLauncher.h"
#include "WatcherConstants.h"

#ifdef __WINS__
#include <e32svr.h>		// UserSvr
#endif

void PanicServer( TInt aPanic )
	{
	User::Panic( KTcWatcherName, aPanic );
	}

CTcWatcherServer* CTcWatcherServer::NewLC()
	{
	CTcWatcherServer* self = new( ELeave ) CTcWatcherServer;
	CleanupStack::PushL( self );

	self->ConstructL();
	return self;
	}

CTcWatcherServer::~CTcWatcherServer()
	{
	delete iShutdownTimer;
	iLaunchers.ResetAndDestroy();
	}

CTcWatcherServer::CTcWatcherServer()
	: CServer2( EPriorityStandard ), iLaunchers( 4 )
	{
	TCallBack cb( Shutdown );
	iShutdownTimerEntry.Set( cb );
	}

void CTcWatcherServer::ConstructL()
	{
	iShutdownTimer = CDeltaTimer::NewL( CActive::EPriorityLow );
	}

TInt CTcWatcherServer::ThreadMain( TAny* /*aParam*/ )
	{
	__UHEAP_MARK;

	// Create the cleanup stack
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if( cleanup == NULL )
		{
		PanicServer( KErrNoMemory );
		}

	// Initialize and start the server
	TRAPD( error, InitServerL() );
	if( error != KErrNone )
		{
		PanicServer( error );
		}

	// InitServerL() will hold execution for the lifetime
	// of the server. The server has died now, delete cleanup stack.
	delete cleanup;

	__UHEAP_MARKEND;

	return KErrNone;
	}

#ifdef __IPC_V2_PRESENT__
CSession2* CTcWatcherServer::NewSessionL( const TVersion& aVersion,
	    								  const RMessage2& /*aMessage*/ ) const
#else
CSharableSession* CTcWatcherServer::NewSessionL( const TVersion& aVersion ) const
#endif
	{
	// Check that the client version is OK
	TVersion version( KTcWatcherMajorVersion,
					  KTcWatcherMinorVersion,
					  KTcWatcherBuildVersion );
	if( !User::QueryVersionSupported( version, aVersion ) )
		{
		User::Leave( KErrNotSupported );
		}

	// Create a new session
#ifdef __IPC_V2_PRESENT__
	return CTcWatcherSession::NewL( const_cast< CTcWatcherServer* >( this ) );
#else
	RThread client = Message().Client();
	return CTcWatcherSession::NewL( client,
									const_cast< CTcWatcherServer* >( this ) );
#endif
	}

void CTcWatcherServer::Died( const TDesC& aAppName, TBool aDismissDialog )
	{
	TRAP_IGNORE(
		{
		CTcAppLauncher* launcher = CTcAppLauncher::NewLC( aAppName, aDismissDialog, iLaunchers );
		iLaunchers.AppendL( launcher );
		CleanupStack::Pop( launcher );
		} )
	}

TInt CTcWatcherServer::RunError( TInt aError )
	{
	// Panic client if we had an error that is caused by the client itself
	if( ( aError == KErrBadDescriptor ) ||
		( aError == KErrBadHandle ) )
		{
		Message().Panic( KTcWatcherName, aError );
		}
	// Otherwise just complete the request with an error code
	else
		{
		Message().Complete( aError );
		}

	// Restart server as it was interrupted when
	// the leave at CServer::RunL() happened
	ReStart();

	// We're fully recovered now
	return KErrNone;
	}

void CTcWatcherServer::SessionAdded()
	{
	iSessionCount++;
	if( iShutdownTimer->IsActive() )
		{
		iShutdownTimer->Remove( iShutdownTimerEntry );
		}
	}

void CTcWatcherServer::SessionRemoved()
	{
	iSessionCount--;

	// way to go..
	if( iSessionCount == 0 )
		{
		// Queue a shutdown after 30 seconds
		iShutdownTimer->Queue( KTcShutdownTime, iShutdownTimerEntry );
		}
	}

TInt CTcWatcherServer::Shutdown( TAny* /*aPtr*/)
	{
	CActiveScheduler::Stop();
	return KErrNone;
	}

void CTcWatcherServer::InitServerL()
	{
	// Open the semaphore that was created by the first client
	RSemaphore semaphore;
	User::LeaveIfError( semaphore.OpenGlobal( KTcWatcherName ) );

	CActiveScheduler* scheduler = NULL;
	CTcWatcherServer* srv = NULL;

	// We don't want the client waiting on the semaphore indefinitely
	// even if server start fails.
	TRAPD( err,
		{
		// Start scheduler and server
		scheduler = new( ELeave ) CActiveScheduler;
		CleanupStack::PushL( scheduler );
		CActiveScheduler::Install( scheduler );

		// Create server instance
		srv = CTcWatcherServer::NewLC( );
		// Start the server using CServer::StartL()
		srv->StartL( KTcWatcherName );
		// we have to pop this before crossing TRAP boundary
		CleanupStack::Pop( 2 );	// srv, scheduler
		} )

	// Signal the client that we are ready and willing
	semaphore.Signal();
	semaphore.Close();
	User::LeaveIfError( err );
/*
#ifdef __WINS__
	// Notify the kernel that a server has been started.
	UserSvr::ServerStarted();
#endif
*/
	// Start fielding requests from clients (execution stops here)
	CActiveScheduler::Start();

	// Remove the active scheduler
	CActiveScheduler::Install( NULL );

	delete srv;
	delete scheduler;
	}
