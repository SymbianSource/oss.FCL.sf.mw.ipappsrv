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




#ifndef __MFCPLUGIN_H__
#define __MFCPLUGIN_H__

// INCLUDES
#include <e32base.h>
#include <in_sock.h>

/**
* @brief Interface class for getting FC PlugIn info and request FC plugIn service 
* @lib FCPlugInEngine.lib
*/
class MFCPlugIn 
	{
    public:
		/**
        * Virtual destructor. Prohibit deletion through this interface.       
        */
        virtual ~MFCPlugIn() {}

	  	/**
	  	* Gets the name of the FC PlugIn name ex: TBCP
		* @return name of the FC PlugIn name 
		*/
		virtual const TDesC8& Name() const = 0;
		
		/**
	  	* Sends out the data via specific FC plugIn 
		* @param aData data requested to be sent
		*/
		virtual void SendToNetL(HBufC8* aData)=0;

	  	/**
	  	* Connect Socket via specific FC plugIn 
	  	*/
	    virtual void ConnectL(TBool aReceiver )=0;
	  	
	  	/**
	  	* Sets IapId to the specific FC plugIn
		* @param aIapId an IAPId
		*/
	    virtual void SetIapId(TUint32 aIapId)=0;

	  	/**
	  	* Sets the address and port for the sending destination
		* @param aAddr a destination addr
		* @param aPort a port to send
		*/
	    virtual void SetAddressL(const TDesC& aAddr, TUint aPort)=0;

	  	/**
	  	* Sets the LocalPort via the specific FC plugin
		* @param aPort a local port
		*/
	    virtual void SetLocalPort(TUint aPort)=0;

	  	/**
	  	* Stops the connection, sending and receiving
		*/
	    virtual void Stop()=0;
	};
	
#endif // __FC_PLUGIN_H__

// End of File
