/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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





#ifndef __CTBCPFLOORCONTROLIMPL_H__
#define __CTBCPFLOORCONTROLIMPL_H__


#include <E32base.h>
#include <E32std.h>
#include <E32def.h>
#include <in_sock.h>

#include "fcmessageobserver.h"


#include "TestInterface.h"

const TInt KMaxAddressLength = 256;
_LIT8(KTest, "TestPlugIn");

// FORWARD DECLARATIONS
class MFCMessageObserver;

// CLASS DECLARATION
/**
* Class provides functions for connecting, sending and receiving
*
*  @lib FCTBCP.dll/ECom plugIn
*/

class CTBCPFloorControlImpl : public CTestInterface
							  
	{
public:
	/**
	* Constructs an object; leaves on failure.
	* @param aInitParams an interface for requesting floor control events
	* @return new instance
	*/
	static CTBCPFloorControlImpl* NewL(TAny* aInitParams);

	/**
	* Destructor
	*/
	~CTBCPFloorControlImpl();

	//From MFCPlugIn 
	/**
	* Gets supported Floor contrl PlugIn Name (e.g. TBCP);
	* @return supported Floor control plugins name 
    *         
	*/
	const TDesC8& Name () const;
	
	/**
	* Send the Data to the predefined addresses
	* @param aData, sending via this plugIn
    *         
	*/
	void SendToNetL(HBufC8* aData);

	/**
	* Open the connection with the predefined IAP id and socket server
	*/
	void ConnectL(TBool aReceiver);

	/**
	* Set IAPID
	* @param aIapId
    *         
	*/	
	void SetIapId(TUint32 aIapId);
	
	/**
	* Set Specified Address and Port which will connect to
	* @param aAddr, aRemote Addr
    * @param aPort, aRemote Port        
	*/		
	void SetAddressL(const TDesC& aAddr, TUint aPort);
	
	/**
	* Set Specified Local Port
    * @param aPort, aLocal Port        
	*/		
	void SetLocalPort(TUint aPort);
	
	/**
	* Stop the connection, Sending and Receving
    * Socket is not closed yet    
	*/		
	void Stop();


/*
  @enum TPlugInState

  @discussion Tracks the state of this object through the connection process
  @value ENotConnected The initial (idle) state
  @value EConnecting A connect request is pending with the socket server
  @value EConnected A connection has been established
  @value ELookingUp A DNS lookup request is pending with the socket server
  */
    enum TPlugInState 
        {
        EIAPIdSet=1,
        EPortSet,
        ESocketStop,
        EConnected,
        EErrorNotify,
        EDataReceived
        };

/*!
  @function ChangeStatus
   
  @discussion handle a change in this object's status
  @param aNewStatus new status
  */
    void ChangeStatus(TPlugInState aNewStatus);

	void TestErrorNotify(TInt aError);
	void TestDataReceived(HBufC8* data);
	TDesC& RemoteAddr();
	TInt Port();


private:
	// Constructor
	CTBCPFloorControlImpl(MFCMessageObserver &aObserver);
	void ConstructL();

	//data
	

	
	MFCMessageObserver& iMsgObserver;

	TestInterfaceInitParams* initParams;
	TUint32 iIapId;
	TInt iErr;
	TBuf<KMaxAddressLength>iRemoteAddr;
	TInt iRemotePort;
	
	TUint iLocalPort;
	HBufC8* iData;
	TPlugInState         iPlugInStatus;

	friend class UT_CFCSession;
	};

#endif // end of __CTBCPFloorControlImpl_H__

// End of File
