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
* Description:    fctbcpfloorcontrolimpl.h
*
*/




#ifndef __CTBCPFLOORCONTROLIMPL_H__
#define __CTBCPFLOORCONTROLIMPL_H__

// INCLUDES
#include <e32base.h>
#include <e32std.h>
#include <e32def.h>
#include <in_sock.h>
#include "fcconnectioncontainer.h"
#include "fcsender.h"
#include "fcreceiver.h"
#include "fcmessageobserver.h"
#include "fcconnectionnotifier.h"
#include "fcnotifier.h"
#include "fcinterface.h"

_LIT8(KTBCP, "TBCP");

// FORWARD DECLARATIONS
class MFCMessageObserver;
class CFCConnectionContainer;
class CFCSender;
class CFCReceiver;

/**
 *  Class provides functions for connecting, sending and receiving
 *
 *  CTBCPFloorControlImpl Class provides functions for Sending
 *
 *  @lib FCTBCP.lib
 *  @since S60 v3.0
 */
class CTBCPFloorControlImpl : public CFCInterface,
							  public MFCConnectionNotifier, public MFCNotifier
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

	public:	//From MFCPlugIn 
		/**
		* Gets supported Floor contrl PlugIn Name (e.g. TBCP);
		* @return supported Floor control plugins name 
	    *         
		*/
		const TDesC8& Name () const;
		
		/**
		* Sends the Data to the predefined addresses
		* @param aData, sending via this plugIn
	    *         
		*/
		void SendToNetL(HBufC8* aData);
		
		/**
		* Opens the connection with the predefined IAP id and socket server
		*/
		void ConnectL(TBool aReceiver);

		/**
		* Sets IAPID
		* @param aIapId
	    *         
		*/	
		void SetIapId(TUint32 aIapId);
		
		/**
		* Sets Specified Address and Port which will connect to
		* @param aAddr, aRemote Addr
	    * @param aPort, aRemote Port        
		*/		
		void SetAddressL(const TDesC& aAddr, TUint aPort);
		
		/**
		* Sets Specified Local Port
	    * @param aPort, aLocal Port        
		*/		
		void SetLocalPort(TUint aPort);
		
		/**
		* Stops the connection, Sending and Receving
	    * Socket is not closed yet    
		*/		
		void Stop();

	public:	//FRom MFCConnectionNotifier
		/**
		* CallBack function for ConnectionConainer
	    * @param aError, aError for the connection status       
		*/			
		void ConnectionStarted( TInt aError );
		
		/**
		* function for ConnectionStarted
	    * @param aError, aError for the connection status       
		*/			
		void ConnectionStartedL(TInt aError);
		
	public:	// From MFCNotifier
		/**
		* CallBack function for Sending and Receiving
	    * @param aError, aError for the sending status       
		*/			
		void ErrorNotify( TInt aErrCode);

		/**
		* Receives the Data from predefined addr and port
		* @param aData, Receving Data
		*/	
		void ReceivedData(HBufC8* aData);
		
	private: // New functions

	    /**
		* Starts to Listen to the predefined Addr and Port
	 	*/			
		void StartListen();
		
		/**
		* Socket is reset and open again
		*/		
		void ResetSocketL();

		/**
		* CancelSending
		*/		
		void CancelSend();
		
	private:
		/**
		* Constructor
		* @param aObserver, MFCMessageObserver
		* @return An initialized instance of this class.
		*/
		CTBCPFloorControlImpl(MFCMessageObserver &aObserver);
		
		void ConstructL();
	
	private://data
		RSocketServ iSocketServ;
		RSocket iSocket;
		CFCSender* iSender;
		CFCReceiver* iReceiver;
		CFCConnectionContainer* iConnection;
		MFCMessageObserver& iMsgObserver;
		TUint32 iIapId;
		TInetAddr iRemoteAddr;
		TUint iLocalPort;
		TBool iStartListen;
		
	private: // For testing purposes
		friend class UT_CTBCPFloorControlImpl;
		};

#endif // end of __CTBCPFLOORCONTROLIMPL_H__

// End of File
