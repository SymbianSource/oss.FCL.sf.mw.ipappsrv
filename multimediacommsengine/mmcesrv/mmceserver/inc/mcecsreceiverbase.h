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




#ifndef CMCECSRECEIVER_BASE_H
#define CMCECSRECEIVER_BASE_H

//  INCLUDES
#include <e32base.h>
#include "mcesrv.h"
#include "mceclientserver.h"

//  FORWARD DECLARATION
class CMceCsReceiveQueue;
class CMceCsServerITC;
class TMceCSReceiveItem;
class CMceCsServerITC;

// CLASS DECLARATION

/**
*  Abstract base class for MCE receivers.
*
*  @lib 
*  @since
*/
class CMceCsReceiverBase: public CBase
	{
	public: // Constructors and destructor
  
		/**
		* Destructor
		*/
		~CMceCsReceiverBase();

	public: // new

		/**
		* Sends data to client
		* @param aId ids to send
		*/
		void SendToClientL( TMceIds& aId, TInt aError = KErrNone );

		/**
		* Sends data to client
		* @param aId ids to send
		* @param aContext context to send
		* @param aContent content to send
		*/
		void SendToClientL( TMceIds& aId, HBufC8* aContext );
		
		/**
		* Sends data to client
		* @param aId ids to send
		* @param aContext context to send
		* @param aContent content to send
		*/
		void SendToClientL( TMceIds& aId, HBufC8* aContext, HBufC8* aContent );

		/**
		* Stores message from client for asyncronous event. 
		* @param aMessage message to be stored
		*/
		void ClientReadyToReceiveL (const RMessage2& aMessage);
    
		/**
		* Removes stored message and completes that with KErrCancel.
		*/
		void CancelClientReceiveL ();
		
		/**
		* Gets ITC ids of current message.
		*/
		TMceIds Ids() const;
		
		/**
		* Retrieves stored event. 
		* @param aMessage message to receive data
		*/
		void ReceiveL (const RMessage2& aMessage);
		
    	/**
    	* Gets reference to server ITC helper
    	* @return reference to server ITC helper
    	*/
    	CMceCsServerITC& ITC();
		
		
	private: 
		/**
		* Adds item to the end of the queue
		* @param aItem item to be added
		* @return KErrNone if succesfull
		*/
		void WriteMessageToClientL (const RMessage2& aMessage);


	protected:

		CMceCsReceiverBase (CMceCsServerITC& aItc);

		void ConstructL ();

		void HandleNewReceiveItemL (TMceCSReceiveItem& aItem);  
		
	private: // Data
		
		CMceCsServerITC&	iITC; 
		
		CMceCsReceiveQueue* iReceiveQueue;
		
		RMessage2           iReceiveRMessage;   
		
		TBool               iClientReadyToReceive;
		
		TMceIds				iIds;

    	//definitions for unit testing
    	MCESRV_UT_DEFINITIONS


	};

#endif

// End of File
