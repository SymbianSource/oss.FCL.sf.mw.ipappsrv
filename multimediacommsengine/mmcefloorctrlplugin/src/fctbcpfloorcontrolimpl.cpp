/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    tbcpfloorcontorlimpl.cpp
*
*/




#include <e32base.h>
#include <e32std.h>
#include <e32def.h>
#include "fctbcpfloorcontrolimpl.h"
#include "fcconnectioncontainer.h"
#include "fcsender.h"
#include "fcreceiver.h"
#include "utf.h"
#include "fcinterfaceinitparams.h"
#include "fcnotifier.h"

// -----------------------------------------------------------------------------
// CTBCPFloorControlImpl::NewL
// -----------------------------------------------------------------------------
//
CTBCPFloorControlImpl* CTBCPFloorControlImpl::NewL(TAny* aInitParams)
	{
	TFCInterfaceInitParams* initParams= 
					static_cast<TFCInterfaceInitParams*>( aInitParams);

	CTBCPFloorControlImpl* self = 
			new (ELeave) CTBCPFloorControlImpl(initParams->iMsgObserver);
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}


// -----------------------------------------------------------------------------
// CTBCPFloorControlImpl::CTBCPFloorControlImpl
// -----------------------------------------------------------------------------
//
 CTBCPFloorControlImpl::CTBCPFloorControlImpl(MFCMessageObserver &aObserver)
: iMsgObserver (aObserver)
	{		
	}

// -----------------------------------------------------------------------------
// CTBCPFloorControlImpl::ConstructL
// -----------------------------------------------------------------------------
//
void CTBCPFloorControlImpl::ConstructL()
	{
	iSender = CFCSender::NewL(iSocket, *this);
	iReceiver = CFCReceiver::NewL(iSocket, *this);
	iConnection = CFCConnectionContainer::NewL(*this);
	//Open the socketserver First
	User::LeaveIfError( iSocketServ.Connect( KESockDefaultMessageSlots) );
	}

// -----------------------------------------------------------------------------
// CTBCPFloorControlImpl::~CTBCPFloorControlImpl
// -----------------------------------------------------------------------------
//
CTBCPFloorControlImpl::~CTBCPFloorControlImpl()
	{
	if (iReceiver)
		{		
		iReceiver->Cancel();
		}
	if (iSender)
		{
		iSender->Cancel();
		}
	if(iConnection)
		{
		iConnection->CloseConnection();

		}
	iSocket.Close();
	delete iReceiver;
	delete iSender;
	delete iConnection;
	iSocketServ.Close();
	}
	
// -----------------------------------------------------------------------------
// CTBCPFloorControlImpl::ConnectL
// -----------------------------------------------------------------------------
//
void CTBCPFloorControlImpl::ConnectL(TBool aReceiver)
	{
	//Open RConnection by calling CFCConnectionContainer
	iStartListen = aReceiver;
	User::LeaveIfError(iConnection->OpenConnection(iIapId, iSocketServ));
	}

// -----------------------------------------------------------------------------
// CTBCPFloorControlImpl::ResetSocketL
// -----------------------------------------------------------------------------
//
void CTBCPFloorControlImpl::ResetSocketL()
	{
	iSocket.Close();
	User::LeaveIfError(iSocket.Open(iSocketServ, KAfInet, KSockDatagram,
					   KProtocolInetUdp, iConnection->Connection()));
	iSocket.SetOpt(KSoReuseAddr, KSolInetIp, 1);
	TInetAddr addr;
	addr.SetAddress(KInetAddrAny);
	addr.SetPort(iLocalPort);
	iSocket.Bind(addr);
	}

// -----------------------------------------------------------------------------
// CTBCPFloorControlImpl::Stop
// -----------------------------------------------------------------------------
//
void CTBCPFloorControlImpl::Stop()
	{
	if (iReceiver)
		{		
		iReceiver->Cancel();
		}
	if (iSender)
		{
		iSender->Cancel();
		}
	if(iConnection)
		{
		iConnection->CloseConnection();
		}
	}

// -----------------------------------------------------------------------------
// CTBCPFloorControlImpl::SendToNetL
// -----------------------------------------------------------------------------
//
void CTBCPFloorControlImpl::SendToNetL(HBufC8* aData)
	{
	iSender->SendL(aData, iRemoteAddr);
 	}

// -----------------------------------------------------------------------------
// CTBCPFloorControlImpl::StartListenL
// -----------------------------------------------------------------------------
//
void CTBCPFloorControlImpl::StartListen()
	{
	iReceiver->Listen();
 	}

// -----------------------------------------------------------------------------
// CTBCPFloorControlImpl::CancelSend
// -----------------------------------------------------------------------------
//
void CTBCPFloorControlImpl::CancelSend()
	{
	 iSender->Cancel();
	 //DoCancel
	}
	
// -----------------------------------------------------------------------------
// CTBCPFloorControlImpl::SetIapId
// -----------------------------------------------------------------------------
//
void CTBCPFloorControlImpl::SetIapId(TUint32 aIapId )
	{
	//require from user 
	iIapId=aIapId;
	}

// -----------------------------------------------------------------------------
// CTBCPFloorControlImpl::SetAddress
// -----------------------------------------------------------------------------
//
void CTBCPFloorControlImpl::SetAddressL(const TDesC& aAddr, TUint aPort)
	{
	TInetAddr address;
	User::LeaveIfError(address.Input(aAddr));
	
    if (address.Family() == KAfInet6 )
		{
		iRemoteAddr.SetAddress( address.Ip6Address() ); 
		}
	else
		{
		iRemoteAddr.SetAddress( address.Address() ); 		
		}

	iRemoteAddr.SetPort(aPort);
	}

// -----------------------------------------------------------------------------
// CTBCPFloorControlImpl::SetLocalPort
// -----------------------------------------------------------------------------
//
void CTBCPFloorControlImpl::SetLocalPort(TUint aPort)
	{
	iLocalPort=aPort;
	}

// -----------------------------------------------------------------------------
// CTBCPFloorControlImpl::ReceivedData
// -----------------------------------------------------------------------------
//
void CTBCPFloorControlImpl::ReceivedData(HBufC8* aData)
	{
	iMsgObserver.ReceivedData( aData, this );
	}

// -----------------------------------------------------------------------------
// CTBCPFloorControlImpl::ConnectionStarted
// -----------------------------------------------------------------------------
//
void CTBCPFloorControlImpl::ConnectionStarted( TInt aError )
	{
	TRAPD(leaveErr ,ConnectionStartedL(aError) );
	if(leaveErr!=KErrNone)
		{
		ErrorNotify( leaveErr );	
		}
	}	

// -----------------------------------------------------------------------------
// CTBCPFloorControlImpl::ConnectionStartedL
// -----------------------------------------------------------------------------
//
void CTBCPFloorControlImpl::ConnectionStartedL( TInt aError )
	{
	if(aError!=KErrNone)
		{
		//return to Application-->ErrorNotify
		iMsgObserver.ErrorNotify( aError, this );
		}
	else
		{
		User::LeaveIfError(iSocket.Open(iSocketServ, KAfInet, KSockDatagram,
								KProtocolInetUdp, iConnection->Connection()));
		
		
		iSender->ConnectedL();
		
		TInetAddr addr;
		addr.SetAddress(KInetAddrAny);
		addr.SetPort(iLocalPort);
		iSocket.Bind(addr);

		if(iStartListen)
			{
			StartListen();
			}	
		}
	}

// -----------------------------------------------------------------------------
// CTBCPFloorControlImpl::Name
// -----------------------------------------------------------------------------
//
const TDesC8& CTBCPFloorControlImpl::Name() const
	{
	return KTBCP;
	}

// -----------------------------------------------------------------------------
// CTBCPFloorControlImpl::ErrorNotify
// -----------------------------------------------------------------------------
//
void CTBCPFloorControlImpl::ErrorNotify( TInt aErrCode)
	{
	if(aErrCode!=KErrNone)
		{
		TRAP_IGNORE(ResetSocketL());
		iMsgObserver.ErrorNotify( aErrCode, this );
		}
	}
