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



#include <es_sock.h>
#include <in_sock.h>
#include <e32base.h>

// Local helper functions:
LOCAL_C TInt SimulateAllocFailure(TInt aError=KErrNone);

LOCAL_C void CompleteAsyncRequest(TRequestStatus& aStatus, TInt aError=KErrNone);

LOCAL_C void SimulateAllocFailureL()
    {
    HBufC* tmp = HBufC::NewL(10);
    delete tmp;
    }    
    
LOCAL_C TInt SimulateAllocFailure(TInt aError)
    {
    TInt err = aError;
    if (err == KErrNone)
        {
        TRAP(err, SimulateAllocFailureL());
        }
    return err;
    }

LOCAL_C void CompleteAsyncRequest(TRequestStatus& aStatus, TInt aError)
    {
    TInt err = SimulateAllocFailure(aError);
	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, err); 
    }


// TSockAddr

// -----------------------------------------------------------------------------
// TSockAddr::TSockAddr
// -----------------------------------------------------------------------------
//
TSockAddr::TSockAddr()
 : TBuf8<KMaxSockAddrSize>()
	{
	SetLength(KMaxSockAddrSize);
	SetFamily(KAFUnspec);   
    }

// -----------------------------------------------------------------------------
// TSockAddr::TSockAddr
// -----------------------------------------------------------------------------
//
TSockAddr::TSockAddr(TUint aFamily)
	: TBuf8<KMaxSockAddrSize>()
	{
	SetLength(KMaxSockAddrSize);
	SetFamily(aFamily);
    }

// -----------------------------------------------------------------------------
// TSockAddr::Family
// -----------------------------------------------------------------------------
//
TUint TSockAddr::Family() const
    {
    return BasePtr()->iFamily;
    }

// -----------------------------------------------------------------------------
// TSockAddr::SetFamily
// -----------------------------------------------------------------------------
//
void TSockAddr::SetFamily(TUint aFamily)
    {
	BasePtr()->iFamily = aFamily;
    }

// -----------------------------------------------------------------------------
// TSockAddr::Port
// -----------------------------------------------------------------------------
//
TUint TSockAddr::Port() const
    {
    return BasePtr()->iPort;
    }

// -----------------------------------------------------------------------------
// TSockAddr::SetPort
// -----------------------------------------------------------------------------
//
void TSockAddr::SetPort(TUint aPort)
    {
    BasePtr()->iPort = aPort;
    }

// -----------------------------------------------------------------------------
// TSockAddr::CmpPort
// -----------------------------------------------------------------------------
//
TBool TSockAddr::CmpPort(const TSockAddr& anAddr) const
    {
	return (BasePtr()->iFamily==anAddr.BasePtr()->iFamily) 
		&& (BasePtr()->iPort==anAddr.BasePtr()->iPort);
    }

// -----------------------------------------------------------------------------
// TSockAddr::SetUserLen
// -----------------------------------------------------------------------------
//
void TSockAddr::SetUserLen(TInt aLen)
	{
	SetLength(aLen+Align4(sizeof(SSockAddr)));
	}

// -----------------------------------------------------------------------------
// TSockAddr::GetUserLen
// -----------------------------------------------------------------------------
//
TInt TSockAddr::GetUserLen()
	{
	return Length()-Align4(sizeof(SSockAddr));
	}



// RSocketServ:

// -----------------------------------------------------------------------------
// RSocketServ::RSocketServ
// -----------------------------------------------------------------------------
//
RSocketServ::RSocketServ()
    {
    }
    
// -----------------------------------------------------------------------------
// RSocketServ::Connect
// -----------------------------------------------------------------------------
//    
TInt RSocketServ::Connect(TUint /*aMessageSlots*/)
    {
    return SimulateAllocFailure();
    }

// RConnection:

// -----------------------------------------------------------------------------
// RConnection::RConnection
// -----------------------------------------------------------------------------
//
RConnection::RConnection() 
 : iNewISPId(0)
	{
	}
	
// -----------------------------------------------------------------------------
// RConnection::~RConnection
// -----------------------------------------------------------------------------
//
RConnection::~RConnection()
	{
	}	
	
// -----------------------------------------------------------------------------
// RConnection::Open
// -----------------------------------------------------------------------------
//
TInt RConnection::Open(
    RSocketServ& /*aSocketServer*/, 
    TUint /*aConnectionType*/) 
	{
	return SimulateAllocFailure();
	}	

// -----------------------------------------------------------------------------
// RConnection::Close
// -----------------------------------------------------------------------------
//
void RConnection::Close()
    {
    }
    
// -----------------------------------------------------------------------------
// RConnection::Start
// -----------------------------------------------------------------------------
//
TInt RConnection::Start( TConnPref& /*aPref*/ )
    {
    return SimulateAllocFailure();    
    }

// -----------------------------------------------------------------------------
// RConnection::GetIntSetting
// -----------------------------------------------------------------------------
//
TInt RConnection::GetIntSetting( 
    const TDesC& /*aSettingName*/,
    TUint32& aValue )
	{
	TInt err = SimulateAllocFailure();
	if ( err == KErrNone )
	    {
	    aValue = 1;
	    }
	return err;
	}


// RSocket:

// -----------------------------------------------------------------------------
// RSocket::RSocket
// -----------------------------------------------------------------------------
//
RSocket::RSocket() 
 : iLocalAddrResolved(EFalse)
    {
    }

// -----------------------------------------------------------------------------
// RSocket::Open
// -----------------------------------------------------------------------------
//	
TInt RSocket::Open(
    RSocketServ& /*aServer*/,
    TUint /*addrFamily*/,
    TUint /*sockType*/,
    TUint /*protocol*/)
    {
    return SimulateAllocFailure();
    }

// -----------------------------------------------------------------------------
// RSocket::Close
// -----------------------------------------------------------------------------
//
void RSocket::Close()
    {
    }

// -----------------------------------------------------------------------------
// RSocket::SetOpt
// -----------------------------------------------------------------------------
//
TInt RSocket::SetOpt(
    TUint /*anOptionName*/,
    TUint /*anOptionLevel*/,
    const TDesC8& /*anOption*/)
    {
    return SimulateAllocFailure();
    }

// -----------------------------------------------------------------------------
// RSocket::GetOpt
// -----------------------------------------------------------------------------
//
TInt RSocket::GetOpt(
    TUint anOptionName,
    TUint anOptionLevel,
    TDes8& anOption)
    {
    TInt err = SimulateAllocFailure();

    if (err == KErrNone)
        {
        if (iLocalAddrResolved)
            {
            // See CMceLocalAddrResolver
            iLocalAddrResolved = EFalse;
            return KErrNotFound;
            }

	    if (anOptionName == KSoInetNextInterface && 
	        anOptionLevel == KSolInetIfCtrl)
	        {
	        TPckgBuf<TSoInetInterfaceInfo>& infoBuf =
	            reinterpret_cast<TPckgBuf<TSoInetInterfaceInfo>&>(anOption);    
	        infoBuf().iAddress.SetAddress(INET_ADDR(1,2,3,4));
	        }

        if (anOptionName == KSoInetIfQueryByName && 
            anOptionLevel == KSolInetIfQuery)
			{
			// Local address being resolved
			TPckgBuf<TSoInetIfQuery>& queryBuf = 
			    reinterpret_cast<TPckgBuf<TSoInetIfQuery>&>(anOption);
			queryBuf().iZone[1] = 1; // IAP id
			iLocalAddrResolved = ETrue;
			}
        }

    return err;
    }
