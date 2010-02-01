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

#include "CTcFileHandlerServer.h"
#include "CTcFileHandlerSession.h"
#include "FileHandlerConstants.h"

#ifdef __WINS__
#include <e32svr.h>		// UserSvr
#endif

void PanicServer( TInt aPanic )
	{
	User::Panic( KTcFileHandlerName, aPanic );
	}

CTcFileHandlerServer* CTcFileHandlerServer::NewLC()
	{
	CTcFileHandlerServer* self = new( ELeave ) CTcFileHandlerServer;
	CleanupStack::PushL( self );

	self->ConstructL();
	return self;
	}

CTcFileHandlerServer::~CTcFileHandlerServer()
	{
	}

CTcFileHandlerServer::CTcFileHandlerServer()
	: CServer2( EPriorityStandard )
	{
	}

void CTcFileHandlerServer::ConstructL()
	{
	}

TInt CTcFileHandlerServer::ThreadMain( TAny* /*aParam*/ )
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

CSession2* CTcFileHandlerServer::NewSessionL( 
    const TVersion& aVersion,
	const RMessage2& /*aMessage*/ ) const
	{
	// Check that the client version is OK
	TVersion version( KTcFileHandlerMajorVersion,
					  KTcFileHandlerMinorVersion,
					  KTcFileHandlerBuildVersion );
	if( !User::QueryVersionSupported( version, aVersion ) )
		{
		User::Leave( KErrNotSupported );
		}

	// Create a new session
	return CTcFileHandlerSession::NewL( const_cast< CTcFileHandlerServer* >( this ) );
	}

TInt CTcFileHandlerServer::RunError( TInt aError )
	{
	// Panic client if we had an error that is caused by the client itself
	if( ( aError == KErrBadDescriptor ) ||
		( aError == KErrBadHandle ) )
		{
		Message().Panic( KTcFileHandlerName, aError );
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

void CTcFileHandlerServer::SessionAdded()
	{
	iSessionCount++;
	}

void CTcFileHandlerServer::SessionRemoved()
	{
	iSessionCount--;

	if( iSessionCount == 0 )
		{
		// No more server users, shutdown server
		CActiveScheduler::Stop();
		}
	}

void CTcFileHandlerServer::InitServerL()
	{
	// Open the semaphore that was created by the first client
	RSemaphore semaphore;
	User::LeaveIfError( semaphore.OpenGlobal( KTcFileHandlerName ) );

	CActiveScheduler* scheduler = NULL;
	CTcFileHandlerServer* srv = NULL;

	// We don't want the client waiting on the semaphore indefinitely
	// even if server start fails.
	TRAPD( err,
		{
		// Start scheduler and server
		scheduler = new( ELeave ) CActiveScheduler;
		CleanupStack::PushL( scheduler );
		CActiveScheduler::Install( scheduler );

		// Create server instance
		srv = CTcFileHandlerServer::NewLC( );
		// Start the server using CServer::StartL()
		srv->StartL( KTcFileHandlerName );
		// we have to pop this before crossing TRAP boundary
		CleanupStack::Pop( 2 );	// srv, scheduler
		} )

	// Signal the client that we are ready and willing
	semaphore.Signal();
	semaphore.Close();
	User::LeaveIfError( err );

	// Start fielding requests from clients (execution stops here)
	CActiveScheduler::Start();

	// Remove the active scheduler
	CActiveScheduler::Install( NULL );

	delete srv;
	delete scheduler;
	}
