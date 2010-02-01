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
* Description:    fcsender.h
*
*/




#ifndef __CFCSENDER_H__
#define __CFCSENDER_H__

#include <e32base.h>
#include <in_sock.h>


// FORWARD DECLARATIONS
class MFCNotifier;

/**
 *  Class provides functions for Sending
 *
 *  CFCSender Class provides functions for Sending
 *
 *  @lib FCTBCP.lib
 *  @since S60 v3.0
 */
class CFCSender : public CActive
	{
    public:
		/**
		* Static constructor.
		* @param aSocket Reference to Socket
		* @param aNotifier, MFCNotifier
		* @return An initialized instance of this class.
		*/
		static CFCSender* NewL(RSocket& aSocket, MFCNotifier& aNotifier);
	
		/**
		* Destructor.
		*/
		~CFCSender ();

		/**
		* Sending the data to the addr
		* @param aData, Data to send
		* @param aAddr, the remote address
		*/
		void SendL (HBufC8* aData, TInetAddr& aAddr);
		
		void ConnectedL();

    protected: // From CActive
	    void RunL ();
        void DoCancel ();
	    TInt RunError (TInt aError);	
	    
    private: 
		/**
		* Constructor
		* @param aSocket Reference to Socket
		* @param aNotifier, MFCNotifier
		* @return An initialized instance of this class.
		*/
		CFCSender (RSocket& aSocket, MFCNotifier& aNotifier);
		
		void SendFromBufferL();

	private: //Data
		RSocket* iSocket;
		RSocketServ* iSocketServ;
		
		RPointerArray<HBufC8> iSendBuffer;
		HBufC8* iData;
		
		TPtrC8 iOutgoingData;
		TInetAddr iRemoteAddr;
		MFCNotifier& iFCNotifier;
		TInt iErr;
		
		TBool iConnected;
	
	private: // For testing purposes
		friend class UT_CFCSender;
	};

#endif // end of __CFCSENDER_H__

// End of File
