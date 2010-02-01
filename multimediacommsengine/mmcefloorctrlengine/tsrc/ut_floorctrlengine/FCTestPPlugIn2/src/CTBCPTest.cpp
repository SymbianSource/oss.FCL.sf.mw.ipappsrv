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


#include "CTBCPTest.h"

#include "utf.h"
#include "fcinterfaceinitparams.h"

_LIT8(KTest, "TBCPTest");


// -----------------------------------------------------------------------------
// CTBCPTest::NewL
// -----------------------------------------------------------------------------
//
CTBCPTest* CTBCPTest::NewL(TAny* aInitParams)
	{
	TFCInterfaceInitParams* initParams= static_cast<TFCInterfaceInitParams*>( aInitParams);

	CTBCPTest* self = new (ELeave) CTBCPTest(initParams->iMsgObserver);
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
	}


// -----------------------------------------------------------------------------
// CTBCPTest::CTBCPTest
// -----------------------------------------------------------------------------
//

 CTBCPTest::CTBCPTest(MFCMessageObserver &aObserver)
: iMsgObserver (aObserver)
	{		

	}

// -----------------------------------------------------------------------------
// CTBCPTest::ConstructL
// -----------------------------------------------------------------------------
//
void CTBCPTest::ConstructL()
	{
	iPlugInStatus=EIAPIdSet;


	}

// -----------------------------------------------------------------------------
// CTBCPTest::~CTBCPTest
// -----------------------------------------------------------------------------
//
CTBCPTest::~CTBCPTest()
	{

		

	}
	
// -----------------------------------------------------------------------------
// CTBCPTest::ConnectL
// -----------------------------------------------------------------------------
//
void CTBCPTest::ConnectL(TBool /*aReceiver*/)
	{
	ChangeStatus(EConnected);

	}


// -----------------------------------------------------------------------------
// CTBCPTest::Stop
// -----------------------------------------------------------------------------
//
void CTBCPTest::Stop()
	{
	ChangeStatus(ESocketStop);
	}


// -----------------------------------------------------------------------------
// CTBCPTest::SendToNetL
// -----------------------------------------------------------------------------
//
void CTBCPTest::SendToNetL(HBufC8* aData)
	{
		if(iData)
		iData=0;
	iData=aData;
	ChangeStatus(EDataReceived);
 	}




// -----------------------------------------------------------------------------
// CTBCPTest::SetIapId
// -----------------------------------------------------------------------------
//
void CTBCPTest::SetIapId(TUint32 aIapId )
	{
	
	iIapId=aIapId;
	ChangeStatus(EIAPIdSet);
	}

// -----------------------------------------------------------------------------
// CTBCPTest::SetAddress
// -----------------------------------------------------------------------------
//
void CTBCPTest::SetAddressL(const TDesC& aAddr, TUint aPort)
	{
	iRemotePort=aPort;
	iRemoteAddr=aAddr;
	
	ChangeStatus(ERemoteAddrSet);
	}

// -----------------------------------------------------------------------------
// CTBCPTest::SetLocalPort
// -----------------------------------------------------------------------------
//
void CTBCPTest::SetLocalPort(TUint aPort)
	{

	//iRemoteAddr.SetPort(aPort);
	iLocalPort=aPort;
	ChangeStatus(EPortSet);
	}




// -----------------------------------------------------------------------------
// CTBCPTest::Name
// -----------------------------------------------------------------------------
//
const TDesC8& CTBCPTest::Name() const
	{
	return KTest;
	}


void CTBCPTest::TestErrorNotify(TInt aError)
{
	iErr=aError;
	ChangeStatus(EErrorNotify);
}
void CTBCPTest::TestDataReceived(HBufC8* data)
	{
	if(iData)
		iData=0;
	iData=data;
	ChangeStatus(EDataReceived);	
	}
void CTBCPTest::ChangeStatus(TPlugInState  aNewStatus)
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
   		case ERemoteAddrSet:
   			
            iMsgObserver.ErrorNotify(iRemotePort, this);
            
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
        	iMsgObserver.ReceivedData(iData,this);
        	break;
        default:
            iMsgObserver.ErrorNotify(0, this);
            break;
        }

    iPlugInStatus = aNewStatus;
    }


// End of file

