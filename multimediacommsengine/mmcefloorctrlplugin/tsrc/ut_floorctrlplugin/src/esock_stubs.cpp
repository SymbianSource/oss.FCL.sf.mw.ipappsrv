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

LOCAL_C void CompleteAsyncRequest(TRequestStatus& aStatus, TInt /*aError*/)
    {
	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, KErrNone); 
    }


// RSocket

// -----------------------------------------------------------------------------
// RSocket::RSocket
// -----------------------------------------------------------------------------
//
RSocket::RSocket()
/*
 : iNextError(KErrNone),
   iLocalAddrResolved(EFalse),
   iLocalPort(0),
   iCancelType(EFalse),
   iOption(0)
   */
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
// RSocket::Open
// -----------------------------------------------------------------------------
//	
TInt RSocket::Open(
    RSocketServ& /*aServer*/,
    TUint /*addrFamily*/,
    TUint /*sockType*/,
    TUint /*protocol*/,
    RConnection& /*aConnection*/)
    {
    return /*KErrNone*/ SimulateAllocFailure(); 
    }

// -----------------------------------------------------------------------------
// RSocket::Open
// -----------------------------------------------------------------------------
//
TInt RSocket::Open(RSocketServ& /*aServer*/)
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

void RSocket::SendTo(const TDesC8& /*aDesc*/,TSockAddr& /*anAddr*/,TUint /*flags*/,TRequestStatus& aStatus)
	{
	CompleteAsyncRequest(aStatus);	
	}
	

// -----------------------------------------------------------------------------
// RSocket::CancelSend
// -----------------------------------------------------------------------------
//

void RSocket::CancelSend( )
	{
		
	}	
	
	


// -----------------------------------------------------------------------------
// RSocket::CancelRecv
// -----------------------------------------------------------------------------
//

void RSocket::CancelRecv( )
	{
		
	}	
	

// -----------------------------------------------------------------------------
// RSocket::RecvFrom
// -----------------------------------------------------------------------------
//

void RSocket::RecvFrom( TDes8& /*aDesc*/, TSockAddr& /*anAddr*/, TUint /*flags*/, TRequestStatus& aStatus )
	{
	CompleteAsyncRequest(aStatus);	
	}	


// -----------------------------------------------------------------------------
// RSocket::SetOpt
// -----------------------------------------------------------------------------
//

TInt RSocket::SetOpt( TUint /*anOptionName*/,TUint /*anOptionLevel*/, TInt /*anOption*/)
	{
	return KErrNone;	
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
TInt RConnection::Start(TConnPref& /*aPref*/)
    {
    return SimulateAllocFailure();    
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
// RConnection::Name
// -----------------------------------------------------------------------------
//

TInt RConnection::Name( TName& /*aName*/ )

    {
    return KErrNone;
    }    
    


// RHostResolver:

// -----------------------------------------------------------------------------
// RHostResolver::Open
// -----------------------------------------------------------------------------
//
TInt RHostResolver::Open(
    RSocketServ& /*aSocketServer*/,
    TUint /*anAddrFamily*/,
    TUint /*aProtocol*/, 
    RConnection& /*aConnection*/)
    {
    return SimulateAllocFailure();
    }

// -----------------------------------------------------------------------------
// RHostResolver::GetByName
// -----------------------------------------------------------------------------
//
void RHostResolver::GetByName(
    const TDesC& /*aName*/,
    TNameEntry& /*aResult*/,
    TRequestStatus& aStatus)
    {
    CompleteAsyncRequest(aStatus);
    }

// -----------------------------------------------------------------------------
// RHostResolver::Next
// -----------------------------------------------------------------------------
//
TInt RHostResolver::Next(TNameEntry& /*aResult*/)
    {
    return SimulateAllocFailure();
    }

// -----------------------------------------------------------------------------
// RHostResolver::Close
// -----------------------------------------------------------------------------
//
void RHostResolver::Close()
    {
    }
    
// -----------------------------------------------------------------------------
// RHostResolver::Cancel
// -----------------------------------------------------------------------------
//
void RHostResolver::Cancel()
    {
    
    }

// -----------------------------------------------------------------------------
// RHostResolver::Query
// -----------------------------------------------------------------------------
//
void RHostResolver::Query(
    const TDesC8& /*aQuery*/, 
    TDes8& /*aResult*/, 
    TRequestStatus& aStatus)
    {
    CompleteAsyncRequest(aStatus);
    }

// -----------------------------------------------------------------------------
// RHostResolver::QueryGetNext
// -----------------------------------------------------------------------------
//
TInt RHostResolver::QueryGetNext(TDes8& /*aResult*/)
    {
    return SimulateAllocFailure();
    }
