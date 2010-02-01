/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   
*
*/




#include "mcelocaladdrresolver.h"
#include <es_sock.h>
#include <commdbconnpref.h>


// -----------------------------------------------------------------------------
// CMceLocalAddrResolver::NewL
// -----------------------------------------------------------------------------
//
CMceLocalAddrResolver* CMceLocalAddrResolver::NewL(
    RSocketServ& aSocketServ,
    TUint32 aIapId )
	{
	CMceLocalAddrResolver* self = NewLC( aSocketServ, aIapId );
	CleanupStack::Pop();
	return self;	
	}

// -----------------------------------------------------------------------------
// CMceLocalAddrResolver::NewLC
// -----------------------------------------------------------------------------
//
CMceLocalAddrResolver* CMceLocalAddrResolver::NewLC(
    RSocketServ& aSocketServ,
    TUint32 aIapId )
	{
	CMceLocalAddrResolver* self = 
	    new( ELeave )CMceLocalAddrResolver( aSocketServ );
	CleanupStack::PushL(self);
	self->ConstructL( aIapId );
	return self;
	}

// -----------------------------------------------------------------------------
// CMceLocalAddrResolver::CMceLocalAddrResolver
// -----------------------------------------------------------------------------
//
CMceLocalAddrResolver::CMceLocalAddrResolver( RSocketServ& aSocketServ )
    : iSocketServ( aSocketServ )
	{
	}

// -----------------------------------------------------------------------------
// CMceLocalAddrResolver::ConstructL
// -----------------------------------------------------------------------------
//
void CMceLocalAddrResolver::ConstructL( TUint32 aIapId )
    {
	// The actual IAP ID is fetched dynamically 
	// to make local IP address lookup work in a case
	// where there are several IAPs used which have different IDs 
	// but all point to the same physical AP.
    
    RConnection connection;
    User::LeaveIfError( connection.Open( iSocketServ ) );
    CleanupClosePushL( connection );
    
    // The connection has to be started to get the actual IAP ID from it
    // At this point the physical connection should be already up
    // as the related SIP profile has been registered using it and
    // the synchronous RConnection::Start should return quickly and
    // should not hang the whole MCE server process for too long
    TCommDbConnPref prefs;
    prefs.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );
    prefs.SetDirection( ECommDbConnectionDirectionOutgoing );
    prefs.SetIapId( aIapId );
    connection.Start( prefs );    
    
    // Fetch the actual IAP ID from the started connection
	_LIT( KIapIdSetting, "IAP\\Id" );
	User::LeaveIfError( connection.GetIntSetting( KIapIdSetting, iIapId ) );
	
	CleanupStack::PopAndDestroy(); // connection
    }

// -----------------------------------------------------------------------------
// CMceLocalAddrResolver::~CMceLocalAddrResolver
// -----------------------------------------------------------------------------
//
CMceLocalAddrResolver::~CMceLocalAddrResolver()
	{
	}

// -----------------------------------------------------------------------------
// CMceLocalAddrResolver::Addr
// -----------------------------------------------------------------------------
//
const TInetAddr& CMceLocalAddrResolver::Addr() const
    {
    return iAddr;
    }

// -----------------------------------------------------------------------------
// CMceLocalAddrResolver::HasIPv4Addr
// -----------------------------------------------------------------------------
//
TBool CMceLocalAddrResolver::HasIPv4Addr() const
    {
    return (!iIPv4Addr.IsUnspecified());
    }

// -----------------------------------------------------------------------------
// CMceLocalAddrResolver::IPv4Addr
// -----------------------------------------------------------------------------
//	
const TInetAddr& CMceLocalAddrResolver::IPv4Addr() const
    {
    return iIPv4Addr;
    }

// -----------------------------------------------------------------------------
// CMceLocalAddrResolver::RefreshLocalAddressesL
// -----------------------------------------------------------------------------
//
void CMceLocalAddrResolver::RefreshLocalAddressesL()
    {
    RSocket socket;
	User::LeaveIfError( socket.Open( iSocketServ, KAfInet, KSockDatagram, 
		                             KProtocolInetUdp ) );

	iAddr.Init( KAfInet );
	iIPv4Addr.Init( KAfInet );

	if ( socket.SetOpt( KSoInetEnumInterfaces, KSolInetIfCtrl ) == KErrNone )
		{
		TPckgBuf<TSoInetInterfaceInfo> info;
		while ( socket.GetOpt( KSoInetNextInterface,
		                       KSolInetIfCtrl, info ) == KErrNone )
			{
			TPckgBuf<TSoInetIfQuery> query;
			query().iName = info().iName;
			if ( socket.GetOpt( KSoInetIfQueryByName,
			                    KSolInetIfQuery, query ) == KErrNone )
				{
				if ( Match( info(), query() ) )
				    {
                    SetAddressesIfNotSet( info().iAddress );
				    }
				}
			}
		}
		
	socket.Close();
	}	

// -----------------------------------------------------------------------------
// CMceLocalAddrResolver::Match
// -----------------------------------------------------------------------------
//
TBool CMceLocalAddrResolver::Match(
    const TSoInetInterfaceInfo& aInfo,
    const TSoInetIfQuery& aQuery) const
    {
    TBool match = EFalse;
    if ( !aInfo.iAddress.IsUnspecified() && 
         !aInfo.iAddress.IsLoopback() &&
         !aInfo.iAddress.IsLinkLocal() &&
         aQuery.iZone[1] == iIapId )
        {
        match = ETrue;
        }
    return match;
    }

// -----------------------------------------------------------------------------
// CMceLocalAddrResolver::SetAddressesIfNotSet
// -----------------------------------------------------------------------------
//
void CMceLocalAddrResolver::SetAddressesIfNotSet( const TInetAddr& aAddr )
    {
    if ( aAddr.Family() == KAfInet )
        {
        if ( iAddr.IsUnspecified() )
            {
            iAddr = aAddr;
            }
        iIPv4Addr = aAddr;
        }
    else
        {
        iAddr = aAddr;
        }
    if ( iIPv4Addr.IsUnspecified() && iAddr.IsV4Mapped() )
        {
        iIPv4Addr = iAddr;
        iIPv4Addr.ConvertToV4();
        }    
    }

