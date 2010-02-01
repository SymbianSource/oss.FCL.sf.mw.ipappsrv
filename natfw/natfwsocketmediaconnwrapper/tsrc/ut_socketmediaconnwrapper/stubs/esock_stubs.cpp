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
#include <rsshared.h>
#include <ES_VER.H>

const TUint KMaxRtpPacketSize = 8192;

LOCAL_C void CompleteAsyncRequest(TRequestStatus& aStatus, TInt aError=KErrNone);

LOCAL_C void CompleteAsyncRequest(TRequestStatus& aStatus, TInt /*aError*/)
    {
    TInt err = KErrNone;
	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, err); 
    }

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
	return KErrNone;
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
	return KErrNone;
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
void RConnection::Start(TConnPref& /*aPref*/, TRequestStatus& aStatus)
    {
    CompleteAsyncRequest(aStatus);
    }
    
// -----------------------------------------------------------------------------
// RConnection::Start
// -----------------------------------------------------------------------------
//
TInt RConnection::Start(TConnPref& /*aPref*/)
    {
    return KErrNone;    
    }

// -----------------------------------------------------------------------------
// RConnection::ProgressNotification
// -----------------------------------------------------------------------------
//
void RConnection::ProgressNotification(
    TNifProgressBuf& aProgress,
    TRequestStatus& aStatus,
    TUint /*aSelectedProgress*/)
    {
    aProgress().iStage = KErrNone;
    CompleteAsyncRequest(aStatus);
    }

// -----------------------------------------------------------------------------
// RConnection::CancelProgressNotification
// -----------------------------------------------------------------------------
//
void RConnection::CancelProgressNotification()
    {
    }

// -----------------------------------------------------------------------------
// RConnection::Stop
// -----------------------------------------------------------------------------
//
TInt RConnection::Stop()
	{
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// RConnection::EnumerateConnections
// -----------------------------------------------------------------------------
//
TInt RConnection::EnumerateConnections(TUint& /*aCount*/ )
	{
	return KErrNone;
	}

// RSocket: This stub does not model properly the port reservation
// Thus it is possible to set to multiple RSockets the same local port.
// If it is wanted that localportset will fail, NextSetLocalPortShouldFail
// can be called and the next localportset will fail.

// -----------------------------------------------------------------------------
// RSocket::RSocket
// -----------------------------------------------------------------------------
//
RSocket::RSocket()
	{
	TBool shouldItFail = EFalse;
	NextSetLocalPortShouldFail(ETrue, shouldItFail); // will not fail
	}

// -----------------------------------------------------------------------------
// RSocket::Open
// -----------------------------------------------------------------------------
//	
TInt RSocket::Open(
    RSocketServ& /*aServer*/,
    TUint /*anAddrFamily*/,
    TUint /*aSockType*/,
    TUint /*aProtocol*/)
    {
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// RSocket::Open
// -----------------------------------------------------------------------------
//	
TInt RSocket::Open(
    RSocketServ& /*aServer*/,
    TUint /*anAddrFamily*/,
    TUint /*aSockType*/,
    TUint /*aProtocol*/,
    RConnection& /*aConnection*/)
    {
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// RSocket::Open
// -----------------------------------------------------------------------------
//
TInt RSocket::Open(RSocketServ& /*aServer*/)
    {
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// RSocket::Close
// -----------------------------------------------------------------------------
//
void RSocket::Close()
    {
    }

// -----------------------------------------------------------------------------
// RSocket::Send
// -----------------------------------------------------------------------------
//
void RSocket::Send(
    const TDesC8& /*aDesc*/,
    TUint /*someFlags*/,
    TRequestStatus& aStatus)
    {
    CompleteAsyncRequest(aStatus);   
    }

// -----------------------------------------------------------------------------
// RSocket::CancelSend
// -----------------------------------------------------------------------------
//
void RSocket::CancelSend()
    {
    }

// -----------------------------------------------------------------------------
// RSocket::CancelRead
// -----------------------------------------------------------------------------
//
void RSocket::CancelRead()
	{
	}

// -----------------------------------------------------------------------------
// RSocket::Connect
// -----------------------------------------------------------------------------
//
void RSocket::Connect(
    TSockAddr& /*anAddr*/,
    TRequestStatus& aStatus)
    {
    CompleteAsyncRequest(aStatus);
    }

// -----------------------------------------------------------------------------
// RSocket::CancelConnect
// -----------------------------------------------------------------------------
//
void RSocket::CancelConnect()
    {
    }

// -----------------------------------------------------------------------------
// RSocket::Bind
// -----------------------------------------------------------------------------
//
TInt RSocket::Bind(TSockAddr& /*anAddr*/)
    {
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// RSocket::SendTo
// -----------------------------------------------------------------------------
//
void RSocket::SendTo(
    const TDesC8& aDesc,
    TSockAddr& /*anAddr*/,
    TUint /*flags*/,
    TRequestStatus& aStatus)
    {
    if ( aDesc.Length() && aDesc.Length() >= KMaxRtpPacketSize + 1 )
    	{
    	TRequestStatus* status = &aStatus;
		User::RequestComplete( status, KErrTooBig ); 
    	}
    else
    	{
    	CompleteAsyncRequest(aStatus);
    	}
    }
    
    
// -----------------------------------------------------------------------------
// RSocket::Accept
// -----------------------------------------------------------------------------
//
void RSocket::Accept(
    RSocket& /*aBlankSocket*/,
    TRequestStatus& aStatus)
    {
    CompleteAsyncRequest(aStatus);
    }
    
// -----------------------------------------------------------------------------
// RSocket::NextSetLocalPortShouldFail
// -----------------------------------------------------------------------------
//
void RSocket::NextSetLocalPortShouldFail(TBool aWriteMode, TBool& aValue)
	{
	static TBool shouldItFail = EFalse;
	
	if (aWriteMode)
		{
		shouldItFail = aValue;
		}
	else
		{
		aValue = shouldItFail;	
		shouldItFail = EFalse; // will never fail twice in a row
		}	
	}
   
// -----------------------------------------------------------------------------
// RSocket::SetLocalPort
// -----------------------------------------------------------------------------
//
TInt RSocket::SetLocalPort(TInt /*aPort*/)
    {    
    
    TBool shouldItFail;
	NextSetLocalPortShouldFail(EFalse, shouldItFail); // reading the value
 
    if (shouldItFail)
    	{
    	return KErrInUse; // simulates error setting the port
    	}
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// RSocket::CancelAll
// -----------------------------------------------------------------------------
// 
void RSocket::CancelAll()
    {
    iLocalPort = 0;
    }
    
// -----------------------------------------------------------------------------
// RSocket::Listen
// -----------------------------------------------------------------------------
//
TInt RSocket::Listen(TUint /*qSize*/)
    {
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// RSocket::RecvFrom
// -----------------------------------------------------------------------------
//
void RSocket::RecvFrom(TDes8& /*aDesc*/,
	TSockAddr& /*anAddr*/,
	TUint /*flags*/,TRequestStatus& aStatus)
    {
    CompleteAsyncRequest(aStatus);
    }
// -----------------------------------------------------------------------------
// RSocket::RecvOneOrMore
// -----------------------------------------------------------------------------
//
void RSocket::RecvOneOrMore(TDes8& /*aDesc*/,
	TUint /*flags*/,
	TRequestStatus& aStatus,
	TSockXfrLength& /*aLen*/)
	{
	CompleteAsyncRequest(aStatus);
	}
 
// -----------------------------------------------------------------------------
// RSocket::Info
// -----------------------------------------------------------------------------
// 
TInt RSocket::Info(TProtocolDesc& aProtocol)
	{	
	aProtocol.iAddrFamily = KAFUnspec;
    aProtocol.iByteOrder = EBigEndian;
    aProtocol.iMessageSize = KSocketMessageSizeIsStream;
   	aProtocol.iNamingServices = KNSNameResolution;
    aProtocol.iSecurity = KSocketNoSecurity;
    aProtocol.iServiceInfo = KSockDatagram;
    aProtocol.iSockType = KSockDatagram; 
    return KErrNone;     
	}
	
// RHostResolver

// -----------------------------------------------------------------------------
// RHostResolver::Open
// -----------------------------------------------------------------------------
// 	
TInt RHostResolver::Open(RSocketServ& /*aSocketServer*/,
	TUint /*anAddrFamily*/,
	TUint /*aProtocol*/)
	{
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// RHostResolver::GetByName
// -----------------------------------------------------------------------------
// 		
TInt RHostResolver::GetByName(const TDesC& /*aName*/, TNameEntry& /*aResult*/)
	{
	return KErrNone;
	}
