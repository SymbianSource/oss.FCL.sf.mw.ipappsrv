/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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



#include <E32base.h>
#include <E32std.h>
#include <E32def.h>


#include "ctbcpfloorcontrolimpl.h"

#include "utf.h"
#include "TestInterfaceInitParams.h"


// -----------------------------------------------------------------------------
// CTBCPFloorControlImpl::NewL
// -----------------------------------------------------------------------------
//
CTBCPFloorControlImpl* CTBCPFloorControlImpl::NewL(TAny* aInitParams)
	{
	TestInterfaceInitParams* initParams= static_cast<TestInterfaceInitParams*>( aInitParams);

	CTBCPFloorControlImpl* self = new (ELeave) CTBCPFloorControlImpl(initParams->iMsgObserver);
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
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
	iPlugInStatus=EIAPIdSet;


	}

// -----------------------------------------------------------------------------
// CTBCPFloorControlImpl::~CTBCPFloorControlImpl
// -----------------------------------------------------------------------------
//
CTBCPFloorControlImpl::~CTBCPFloorControlImpl()
	{
	}
	
// -----------------------------------------------------------------------------
// CTBCPFloorControlImpl::ConnectL
// -----------------------------------------------------------------------------
//
void CTBCPFloorControlImpl::ConnectL(TBool /*aReceiver*/)
	{
	ChangeStatus(EConnected);
	}

// -----------------------------------------------------------------------------
// CTBCPFloorControlImpl::Stop
// -----------------------------------------------------------------------------
//
void CTBCPFloorControlImpl::Stop()
	{
	ChangeStatus(ESocketStop);
	}

// -----------------------------------------------------------------------------
// CTBCPFloorControlImpl::SendToNetL
// -----------------------------------------------------------------------------
//
void CTBCPFloorControlImpl::SendToNetL(HBufC8* aData)
	{
	if(iData)
		iData=0;
	iData=aData;
	ChangeStatus(EDataReceived);
 	}




// -----------------------------------------------------------------------------
// CTBCPFloorControlImpl::SetIapId
// -----------------------------------------------------------------------------
//
void CTBCPFloorControlImpl::SetIapId(TUint32 aIapId )
	{
	iIapId=aIapId;
	ChangeStatus(EIAPIdSet);
	}

// -----------------------------------------------------------------------------
// CTBCPFloorControlImpl::SetAddress
// -----------------------------------------------------------------------------
//
void CTBCPFloorControlImpl::SetAddressL(const TDesC& aAddr, TUint aPort)
	{
	iRemotePort=aPort;
	iRemoteAddr=aAddr;
	}

TDesC& CTBCPFloorControlImpl::RemoteAddr()
	{
	return iRemoteAddr;
	}

TInt CTBCPFloorControlImpl::Port()
{
	return iRemotePort;
}
// -----------------------------------------------------------------------------
// CTBCPFloorControlImpl::SetLocalPort
// -----------------------------------------------------------------------------
//
void CTBCPFloorControlImpl::SetLocalPort(TUint aPort)
	{

	iLocalPort=aPort;
	ChangeStatus(EPortSet);
	}




// -----------------------------------------------------------------------------
// CTBCPFloorControlImpl::Name
// -----------------------------------------------------------------------------
//
const TDesC8& CTBCPFloorControlImpl::Name() const
	{
	return KTest;
	}


void CTBCPFloorControlImpl::TestErrorNotify(TInt aError)
{
	iErr=aError;
	ChangeStatus(EErrorNotify);
}
void CTBCPFloorControlImpl::TestDataReceived(HBufC8* data)
	{
	if(iData)
		iData=0;
	iData=data;
	ChangeStatus(EDataReceived);	
	}
void CTBCPFloorControlImpl::ChangeStatus(TPlugInState  aNewStatus)
    {
    // Update the status (and the status display)
    switch (aNewStatus)
        {
        
        case EIAPIdSet:
            iMsgObserver.ErrorNotify(iIapId, this);
            break;
        case EPortSet:
            iMsgObserver.ErrorNotify(iLocalPort, this);
            break;
   
        case ESocketStop:
            iMsgObserver.ErrorNotify(3, this);
            break;
        case EConnected:
            iMsgObserver.ErrorNotify(4, this);
            break;
        case EErrorNotify:
            iMsgObserver.ErrorNotify(iErr, this);
            break;
        case EDataReceived:
        	iMsgObserver.ReceivedData(iData, this);
        	break;
        default:
            iMsgObserver.ErrorNotify(0, this);
            break;
        }

    iPlugInStatus = aNewStatus;
    }

