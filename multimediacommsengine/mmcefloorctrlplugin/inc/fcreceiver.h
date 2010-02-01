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
* Description:    fcreceiver.h
*
*/




#ifndef __CFCRECEIVER_H__
#define __CFCRECEIVER_H__

//  INCLUDES
#include "fcnotifier.h"
#include <e32base.h>
#include <in_sock.h>

const TInt KMaxMessageLength = 64000;

// FORWARD DECLARATIONS
class MFCNotifier;

/**
 *  Class provides functions for Receiving
 *
 *  CFCReceiver Class provides functions for Receiving
 *
 *  @lib FCTBCP.lib
 *  @since S60 v3.0
 */
class CFCReceiver : public CActive
	{
    public:
	    /**
		* Static constructor.
		* @param aSocket Reference to Socket
		* @param aMFCNotifier, MFCNotifier
		* @return An initialized instance of this class.
		*/
		static CFCReceiver* NewL(RSocket& aSocket, MFCNotifier& aMFCNotifier);

		/**
		* Constructs an object and adds the pointer to the cleanup stack;
		* leaves on failure.
		* @param aSocket Reference to Socket
		* @param aMFCNotifier an interface for messaging receiving event
		* @return new instance
		*/
		static CFCReceiver* NewLC(RSocket& aSocket, MFCNotifier& aMFCNotifier);

		/**
		* Destructor
		*/
		~CFCReceiver();
	
		/**
		* Start to Listen
		*/	
		void Listen();
		
		
	public:	//From MFCNotifier
		/**
		* Pass the receiving Data to the client
		* @param aData, Receving Data
	    * @param aRemoteAddr, the data receving from this addr      
		*/	
		void ReceivedData(HBufC8* aData);

	protected: //From CActive
		void RunL();
		void DoCancel();
		TInt RunError(TInt aError);

	private:
		/**
		* Constructor
		* @param aSocket Reference to Socket
		* @param aNotifier, MFCNotifier
		* @return An initialized instance of this class.
		*/
	    CFCReceiver(RSocket& aSocket, MFCNotifier& aMFCNotifier);
		void ConstructL();

	private: //Data
		TInetAddr iAddr;
		MFCNotifier& iFCNotifier;
		HBufC8* iReceivedMsg;
		TPtr8   iReceivedMsgPtr;
		RSocket& iSocket;

	private: // For testing purposes
		friend class UT_CFCReceiver;
	};
	
#endif // end of __CFCRECEIVER_H__

// End of File
