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

#include "CTcBaseConnection.h"
#include "debuglog.h"
#include "ErrorHandling.h"

/// Accept queue length for listening sockets.
const TInt KTcAcceptQueue( 2 );
/// Try binding twenty times, at intervals of 0.1 sec
const TInt KTcBindRetryCount( 20 );
const TInt KTcBindDelay( 100000 );

// Retry connect after delay if connecting fails
const TInt KRetryDelay( 5000000 );

CTcBaseConnection::~CTcBaseConnection()
	{
	LOG( _L("CTcBaseConnection::~CTcBaseConnection()") );
	Close();
	}

CTcBaseConnection::CTcBaseConnection()
	: CActive( CActive::EPriorityHigh )
	{
	CActiveScheduler::Add( this );
	iTimer.CreateLocal();
	}

void CTcBaseConnection::RunL()
	{
	LOG( _L("CTcBaseConnection::RunL() iState = %d, iStatus = %d"), iState, iStatus.Int() );

	// Take backup of the state for notifying AFTER state transition
	TInt status( iStatus.Int() );
	TState state( iState );

	// Do state transition
	switch( iState )
		{
		case EConnecting:
			{
			if( !status )
				{
				iState = EConnected;
				}
			else
				{
				iState = EIdle;
				
				// Retry if connecting failed
				if ( status == KErrCouldNotConnect)
					{
						iState = EWaitingRetry;
						iTimer.After(iStatus,KRetryDelay);
						SetActive();
						return;
					}
				}
			break;
			}
		case EWaitingRetry:
		{
			LOG( _L("CTcBaseConnection::RunL() retrying connect") );
			ConnectL( iRemoteAddr );
			return;
		}
		case ESending:
		case EReceiving:
			{
			iState = EConnected;
			break;
			}

		default:
			{
			// should not happen..
			break;
			}
		}

	Notify( state, status );
	LOG( _L("CTcBaseConnection::RunL() end, iState = %d"), iState );
	}

void CTcBaseConnection::DoCancel()
	{
	LOG( _L("CTcBaseConnection::DoCancel() start") );
	iSocket.CancelAll();
	
	switch( iState )
		{
		case EConnecting:
			{
			iState = EIdle;
			break;
			}
		case EWaitingRetry:
		{
			iTimer.Cancel();
			break;
			}
		case ESending:
		case EReceiving:
			{
			iState = EConnected;
			break;
			}

		default:
			{
			break;
			}
		}

	LOG( _L("CTcBaseConnection::DoCancel() end") );
	}

void CTcBaseConnection::ConnectL()
	{
	LOG( _L("CTcBaseConnection::ConnectL() start") );
	if( iState != EIdle )
		{
		User::Leave( KErrNotReady );
		}

	iState = EConnecting;
	
	__ASSERT_ALWAYS( iSocketServ, User::Leave( KErrNotReady ) );

	// Open data transfer socket
	User::LeaveIfError( iSocket.Open( *iSocketServ ) );

	// Bearer specific port setup and port selection
	SetupPortL();

	// Set local port
	iSockAddr.SetPort( iLocalPort );

	LOG( _L("CTcBaseConnection::ConnectL() binding port %d"), iLocalPort );
	// Bind it to listening socket
	TInt status( KErrInUse );
	for( TInt i = 0; ( i < KTcBindRetryCount ) && ( status != KErrNone ); i++ )
		{
		status = iListeningSocket.Bind( iSockAddr );
		if( status == KErrInUse )
			{
			LOG( _L("CTcBaseConnection::ConnectL() retrying after %dms"), KTcBindDelay / 1000 );
			User::After( KTcBindDelay );
			}
		}
	User::LeaveIfError( status );

	LOG( _L("CTcBaseConnection::ConnectL() listening") );

	// Set listening mode, we don't really need that much queue
	User::LeaveIfError( iListeningSocket.Listen( KTcAcceptQueue ) );

	LOG( _L("CTcBaseConnection::ConnectL() accepting") );
	// Set listening socket to accept incoming
	// connections to real communications socket
	iListeningSocket.Accept( iSocket, iStatus );

	// Now wait for a client to connect
	SetActive();

	if( iObserver )
		{
		iObserver->BearerCompletion( MTcBearerObserver::EListen, KErrNone );
		}

	LOG( _L("CTcBaseConnection::ConnectL() end") );
	}

void CTcBaseConnection::ConnectL( TInetAddr& aRemoteAddr )
	{
	LOG( _L("CTcBaseConnection::ConnectL() 2 start") );
	if( iState != EIdle && iState != EWaitingRetry)
		{
		User::Leave( KErrNotReady );
		}

	iState = EConnecting;
	
	__ASSERT_ALWAYS( iSocketServ, User::Leave( KErrNotReady ) );


    SetupPort2L();
											   
	// Set local port
	iSockAddr.SetPort( iLocalPort );
	
	LOG( _L("CTcBaseConnection::ConnectL() binding port %d"), iLocalPort );
	// Bind it to listening socket
	TInt status( KErrInUse );
	for( TInt i = 0; ( i < KTcBindRetryCount ) && ( status != KErrNone ); i++ )
		{
		status = iSocket.Bind( iSockAddr );
		if( status == KErrInUse )
			{
			LOG( _L("CTcBaseConnection::ConnectL() retrying after %dms"), KTcBindDelay / 1000 );
			User::After( KTcBindDelay );
			}
		}

	LOG( _L("CTcBaseConnection::ConnectL() connecting") );
	iSocket.Connect( aRemoteAddr, iStatus );
	
	// Save address so that we can retry in RunL if connection times out
	iRemoteAddr = aRemoteAddr;

	// Now wait for connection establishemt
	SetActive();

	if( iObserver )
		{
		iObserver->BearerCompletion( MTcBearerObserver::EConnecting, KErrNone );
		}

	LOG( _L("CTcBaseConnection::ConnectL() 2 end") );
	}
	
void CTcBaseConnection::Close()
	{
	LOG( _L("CTcBaseConnection::Close() start") );
	
	if( iState == EIdle )
		{
		// no need to do anything
		LOG( _L("CTcBaseConnection::Close() end, already closed") );
		return;
		}

	// Close listening socket if its open
	if( iListeningSocket.SubSessionHandle() )
		{
		iListeningSocket.CancelAll();
		iListeningSocket.Close();
		}
		
	// Cancel read/write requests and timeouts
	Cancel();

	// Close down data socket
	iSocket.Close();
	
	// Close timer
	iTimer.Close();

	iState = EIdle;
	LOG( _L("CTcBaseConnection::Close() end") );
	}

void CTcBaseConnection::Send( const TDesC8& aDes )
	{
	LOG( _L("CTcBaseConnection::Send() start") );
	// Make sure we're in correct state
	__ASSERT_ALWAYS( iState == EConnected, Panic( KErrNotReady ) );
	__ASSERT_ALWAYS( !IsActive(), Panic( KErrInUse ) );

	// Start writing
	iState = ESending;
	iSocket.Write( aDes, iStatus );
	SetActive();

	LOG( _L("CTcBaseConnection::Send() end") );
	}

void CTcBaseConnection::Receive( TDes8& aDes )
	{
	LOG( _L("CTcBaseConnection::Receive() start") );
	// Make sure we're in correct state
	__ASSERT_ALWAYS( iState == EConnected, Panic( KErrNotReady ) );
	__ASSERT_ALWAYS( !IsActive(), Panic( KErrInUse ) );

	// Start reading
	iState = EReceiving;
	iSocket.Read( aDes, iStatus );
	SetActive();

	LOG( _L("CTcBaseConnection::Receive() end") );
	}

void CTcBaseConnection::ReceiveOneOrMore( TDes8& aDes )
	{
	LOG( _L("CTcBaseConnection::ReceiveOneOrMore() start") );
	// Make sure we're in correct state
	__ASSERT_ALWAYS( iState == EConnected, Panic( KErrNotReady ) );
	__ASSERT_ALWAYS( !IsActive(), Panic( KErrInUse ) );

	// Start reading
	iState = EReceiving;
	iSocket.RecvOneOrMore( aDes, 0, iStatus, iIgnoredLength );
	SetActive();

	LOG( _L("CTcBaseConnection::ReceiveOneOrMore() end") );
	}

void CTcBaseConnection::SetObserver( MTcBearerObserver* aObserver )
	{
	LOG( _L("CTcBaseConnection::SetObserver( %d )"), (TInt)aObserver );
	iObserver = aObserver;
	}

void CTcBaseConnection::SetConnection( RSocketServ* aSocketServ, RConnection* aConnection )
    {
    iSocketServ = aSocketServ;
    iConnection = aConnection;
    }
    
TInt CTcBaseConnection::LocalPort() const
	{
	return iLocalPort;
	}

void CTcBaseConnection::Notify( TState aState, TInt aStatus )
	{
	LOG( _L("CTcBaseConnection::Notify( %d, %d ) start"), aState, aStatus );
	MTcBearerObserver::TOperation op;

	switch( aState )
		{
		case EConnecting:
			{
			op = MTcBearerObserver::EConnect;
			break;
			}
		case ESending:
			{
			op = MTcBearerObserver::ESend;
			break;
			}
		case EReceiving:
			{
			op = MTcBearerObserver::EReceive;
			break;
			}

		case EIdle:
		case EConnected:
		default:
			{
			op = MTcBearerObserver::EUnknown;
			break;
			}
		}

	if( iObserver )
		{
		iObserver->BearerCompletion( op, aStatus );
		}
	LOG( _L("CTcBaseConnection::Notify() end") );
	}

void CTcBaseConnection::SetupPort2L()
    {
    User::Leave( KErrNotSupported );
    }
    
