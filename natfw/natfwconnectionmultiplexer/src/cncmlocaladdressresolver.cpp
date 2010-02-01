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
* Description:    Connection multiplexer local IP resolver
*
*/




#include "cncmlocaladdressresolver.h"
#include "ncmconnectionmultiplexerlogs.h"

// ---------------------------------------------------------------------------
// CNcmLocalAddressResolver::CNcmLocalAddressResolver
// ---------------------------------------------------------------------------
//
CNcmLocalAddressResolver::CNcmLocalAddressResolver(
    RSocketServ& aSocketServ ) : iSocketServ( aSocketServ )
    {
    }


// ---------------------------------------------------------------------------
// CNcmLocalAddressResolver::NewL
// ---------------------------------------------------------------------------
//
CNcmLocalAddressResolver* CNcmLocalAddressResolver::NewL(
    RSocketServ& aSocketServ )
    {   
    CNcmLocalAddressResolver* self = NewLC( aSocketServ );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNcmLocalAddressResolver::NewLC
// ---------------------------------------------------------------------------
//
CNcmLocalAddressResolver* CNcmLocalAddressResolver::NewLC(
    RSocketServ& aSocketServ )
    {
    CNcmLocalAddressResolver* self =
        new ( ELeave ) CNcmLocalAddressResolver( aSocketServ );
    CleanupStack::PushL(self);
    return self;
    }  

    
// ---------------------------------------------------------------------------
// CNcmLocalAddressResolver::~CNcmLocalAddressResolver
// ---------------------------------------------------------------------------
//
CNcmLocalAddressResolver::~CNcmLocalAddressResolver()
    {
    }


// -----------------------------------------------------------------------------
// CNcmLocalAddressResolver::GetLocalAddrL
// -----------------------------------------------------------------------------
//
void CNcmLocalAddressResolver::GetLocalAddrL( TInetAddr& aIPv4Addr,
    TInetAddr& aIPv6Addr, TUint32 aIap )
    {
    iIPv6Addr.Init( KAFUnspec );
    iIPv4Addr.Init( KAFUnspec );
          
    RSocket socket;
    CleanupClosePushL( socket );
    
    User::LeaveIfError( socket.Open(
        iSocketServ, KAfInet, KSockDatagram, KProtocolInetUdp ) );

    if ( KErrNone == socket.SetOpt( KSoInetEnumInterfaces, KSolInetIfCtrl ) )
        {
        TPckgBuf<TSoInetInterfaceInfo> info;

        while ( KErrNone == socket.GetOpt(
            KSoInetNextInterface, KSolInetIfCtrl, info ) )
            {
            TPckgBuf<TSoInetIfQuery> query;
            query().iName = info().iName;

            if( KErrNone == socket.GetOpt( KSoInetIfQueryByName,
                KSolInetIfQuery, query ) )
                {
                CheckAndSetAddr( info(), query(), aIap );
                }
            }
        }

    aIPv4Addr = iIPv4Addr;
    aIPv6Addr = iIPv6Addr;

#ifdef _DEBUG
    TBuf<40> buffer;
    aIPv6Addr.Output( buffer );
    RDebug::Print( _L("CNcmLocalAddressResolver::GetLocalAddrL - IPv6 ADDRESS: %S "), &buffer );
#endif

    CleanupStack::PopAndDestroy( &socket );
    }


// -----------------------------------------------------------------------------
// CNcmLocalAddressResolver::CheckAndSetAddr
// -----------------------------------------------------------------------------
//
void CNcmLocalAddressResolver::CheckAndSetAddr( 
    const TSoInetInterfaceInfo& aInfo, const TSoInetIfQuery& aQuery,
    TUint32 aSpecifiedIap )
    { 
    __CONNECTIONMULTIPLEXER_ADDRLOG(
        "CNcmLocalAddressResolver::CheckAndSetAddr - IPv4 ADDRESS: ", aInfo.iAddress )
           
    if ( Match( aInfo, aQuery, aSpecifiedIap ) )
        {
        TInetAddr address( aInfo.iAddress );

        if ( address.Family() == KAfInet )
            {
            iIPv4Addr = address;
            }
        else
            {
            iIPv6Addr = address;
            }

        if ( iIPv4Addr.IsUnspecified() && iIPv6Addr.IsV4Mapped() )
            {
            iIPv4Addr = iIPv6Addr;
            iIPv4Addr.ConvertToV4();            
            iIPv6Addr = KAFUnspec;
            }
        }
    }


// -----------------------------------------------------------------------------
// CNcmLocalAddressResolver::Match
// -----------------------------------------------------------------------------
//
TBool CNcmLocalAddressResolver::Match(
    const TSoInetInterfaceInfo& aInfo,
    const TSoInetIfQuery& aQuery,
    TUint32 aSpecifiedIap ) const
    {
    TBool match = EFalse;
    if ( !aInfo.iAddress.IsUnspecified() && 
         !aInfo.iAddress.IsLoopback() &&
         !aInfo.iAddress.IsLinkLocal() &&
         aQuery.iZone[1] == aSpecifiedIap )
        {
        match = ETrue;
        }
    return match;
    }
