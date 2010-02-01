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

#include <es_enum.h>
#include <in_sock.h>

#include "CTcTcpManager.h"
#include "TTcTcpFactory.h"
#include "debuglog.h"

CTcTcpManager* CTcTcpManager::NewL( const TTcTcpFactory& aFactory )
	{
	CTcTcpManager* self = new( ELeave ) CTcTcpManager( aFactory );

	CleanupStack::PushL( self );
	self->ConstructL( );
	CleanupStack::Pop( self );

	return self;
	}

CTcTcpManager::~CTcTcpManager()
	{
	LOG( _L("CTcTcpManager::~CTcTcpManager()") );
	// Close connection
	Close();
	iConnServ.Close();
	iSockServ.Close();
	delete iRemoteAddr;
	}

CTcTcpManager::CTcTcpManager( const TTcTcpFactory& aFactory )
	: CActive( CActive::EPriorityStandard ),
	  iConn( aFactory.iPort ), iIAPId( aFactory.iIAPId )
	{
	CActiveScheduler::Add( this );
	}

void CTcTcpManager::ConstructL()
	{
	LOG( _L("CTcTcpManager::ConstructL()") );
	// Open a session to socket server and connection server
	User::LeaveIfError( iSockServ.Connect() );
	User::LeaveIfError( iConnServ.Open( iSockServ ) );
	iConn.SetObserver( this );
	iConn.SetConnection( &iSockServ, &iConnServ );
	
	iPrefs.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );
    iPrefs.SetDirection( ECommDbConnectionDirectionOutgoing );
    iPrefs.SetIapId( iIAPId );
	}

void CTcTcpManager::RunL()
	{
	LOG( _L("CTcTcpManager::RunL() start") );

	// Report any intermediate connection errors
	TInt status( iStatus.Int() );
	if( status != KErrNone )
		{
		BearerCompletion( MTcBearerObserver::EConnect, status );
		}
	else
		{
		TInt err( KErrNone );
		// Start waiting for connections
		if ( iRemoteAddr )
		    {
		    TRAP( err, iConn.ConnectL( *iRemoteAddr ) );
		    }
		else
		    {
		    TRAP( err, iConn.ConnectL() );
		    }
		// Catch and report errors
		if( err )
			{
			BearerCompletion( MTcBearerObserver::EConnect, err );
			}
		}

	LOG( _L("CTcTcpManager::RunL() end") );
	}

void CTcTcpManager::DoCancel()
	{
	LOG( _L("CTcTcpManager::DoCancel()") );
	// Cannot use Stop() as it forces the whole IAP connection down
	// even if somebody else is still using it.
	iConnServ.Close();
	}

void CTcTcpManager::ConnectL( TInetAddr* aRemoteAddr )
	{
	LOG( _L("CTcTcpManager::ConnectL() start, IAP = %d"), iIAPId );
	
	delete iRemoteAddr;
	iRemoteAddr = NULL;
	    
	if ( aRemoteAddr )
	    {
	    iRemoteAddr = new ( ELeave ) TInetAddr( *aRemoteAddr );
	    }

	// Fire up a net connection, use the defined IAP if any
	if( iIAPId )
		{
		iConnServ.Start( iPrefs, iStatus );
		}
    else
		{
		// This will query the user for an IAP
		iConnServ.Start( iStatus );
		}
		
	SetActive();

	LOG( _L("CTcTcpManager::ConnectL() end") );
	}

void CTcTcpManager::Close()
	{
	LOG( _L("CTcTcpManager::Close()") );

	Cancel();
	// Shut down the service port
	iConn.Close();
	}

void CTcTcpManager::Send( const TDesC8& aDes )
	{
	iConn.Send( aDes );
	}

void CTcTcpManager::Receive( TDes8& aDes )
	{
	iConn.Receive( aDes );
	}

void CTcTcpManager::ReceiveOneOrMore( TDes8& aDes )
	{
	iConn.ReceiveOneOrMore( aDes );
	}

void CTcTcpManager::SetObserver( MTcBearerObserver* aObserver )
	{
	iObserver = aObserver;
	}

void CTcTcpManager::GetLocalAddressL( TDes& aDes )
	{ 
	// Array for addresses 
	RArray< TBuf<40> > addresses;
		
	// Open socket for querying socket options
	RSocket socket;
	User::LeaveIfError(socket.Open(
			iSockServ, KAfInet, KSockDatagram, KProtocolInetUdp, iConnServ));
	
	// Get IAP id from CommDb for the active connection
	TUint32 iapId;
	User::LeaveIfError(iConnServ.GetIntSetting(_L("IAP\\Id"), iapId));
	
	// Get address from socket options
	TInt err = socket.SetOpt(KSoInetEnumInterfaces, KSolInetIfCtrl);
	if (err == KErrNone)
		{
		TPckgBuf<TSoInetInterfaceInfo> info;
		err = socket.GetOpt(KSoInetNextInterface,KSolInetIfCtrl,info);
		while (err == KErrNone)
			{
			TPckgBuf<TSoInetIfQuery> query;
			query().iName = info().iName;
			err = socket.GetOpt(KSoInetIfQueryByName,KSolInetIfQuery,query);
			if (err == KErrNone)
				{
					if (!info().iAddress.IsUnspecified() && 
							!info().iAddress.IsLoopback() &&
							!info().iAddress.IsLinkLocal())
					{
						
						TInetAddr addr;
						
						// Initialize address for both ipv4 and ipv6
						addr.Init(KAfInet6);
						
						addr = info().iAddress; 
						
						TBuf<40> addrDesc;
						addr.Output( addrDesc );
						
						TInt oldIndex = addresses.Find(addrDesc);
												
						// Address of current IAP is shown first
						if (query().iZone[1] == iapId)
						{
							// remove the old one
							if(KErrNotFound != oldIndex){
								addresses.Remove(oldIndex);
							}
							addresses.Insert(addrDesc,0);
						}
						else
						{
							if(KErrNotFound == oldIndex){
								addresses.Append(addrDesc);
							}
						}       
					}
				err = socket.GetOpt(KSoInetNextInterface,KSolInetIfCtrl,info);
			}
		}
	}
		
	socket.Close();
	
	if (err == KErrNoMemory)
			{
			User::LeaveNoMemory();
			}
	_LIT(KLineFeed,"\n");
	
	// append address from the array to descriptor
	for(TInt i=0;i<addresses.Count();i++)
	{
		aDes.Append(addresses[i]);
		aDes.Append(KLineFeed);
	}
	addresses.Close();

	}

void CTcTcpManager::BearerCompletion( MTcBearerObserver::TOperation aOp,
									  TInt aStatus )
	{
	LOG( _L("CTcTcpManager::BearerCompletion( %d, %d ) start"), aOp, aStatus );
	if( iObserver )
		{
		iObserver->BearerCompletion( aOp, aStatus );
		}
	LOG( _L("CTcTcpManager::BearerCompletion() end") );
	}
