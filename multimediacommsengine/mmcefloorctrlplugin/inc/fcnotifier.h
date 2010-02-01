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
* Description:    fcnotifier.h
*
*/




#ifndef __MFCNOTIFIER_H__
#define __MFCNOTIFIER_H__

// INCLUDES
#include <e32std.h>


/**
 *  an interface for receiving the notifier 
 *
 *  MFCNotifier defines an interface for observing network state
 *  changes. Derive from this interface if you want to receive events
 *  @lib FCTBCP.lib
 *  @since S60 v3.0
 */
class MFCNotifier
	{
	protected: // Constructors and destructors
		/**
        * Virtual destructor. Prohibit deletion through this interface.       
        */
		virtual ~MFCNotifier() {};

	public: // Abstract methods
		/**
	 	* Called by a CFCReceiver & CFCSender
		* @param aData, Receving Data
	 	*/
		virtual void ReceivedData(HBufC8* aData) = 0;
		
		/**
		* Called by a CFCReceiver & CFCSender
		* @param aErrCode KErrNone if connection succesfully, otherwise
		*				 one of the system-wide error codes.
		* @param aErrMessage, error message
		*/
		virtual void ErrorNotify(TInt aErrCode) = 0;
	};

#endif // __MFCNOTIFIER_H__
