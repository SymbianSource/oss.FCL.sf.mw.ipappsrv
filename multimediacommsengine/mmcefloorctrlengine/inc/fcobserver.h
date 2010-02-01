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




#ifndef __MFCOBSERVER_H__
#define __MFCOBSERVER_H__

// INCLUDES
#include <e32base.h>

// FORWARD CLASS DECLARATION
class CFCSession;

// CLASS DECLARATION

/**
*  An interface to be implemented by user of CFCPlugInEngine.
*  Interface allows to be able to receive FloorControl message from the FloorControl Connection
*  that has not been initialized by the user.
*
*  @lib n/a
*/
class MFCObserver
	{
	protected: // Constructors and destructors
		/**
        * Virtual destructor. Prohibit deletion through this interface.       
        */
	    virtual ~MFCObserver() {};
	
	public:
	    /**
		* A message from specific FC session plugin has been received from the network.        
        * @param aData The Data received from FCsession specific plugIn 
        *        The ownership is transferred.
		* @param aFCSession The Session which data is received to.
        */
    	virtual void FCReceivedData( HBufC8* aData, const CFCSession* aFCSession ) = 0;

    	/**
		* An asynchronous error has occurred in the FC related to the
		* given session indicated.
		* @param aError KErrNone succesfully, otherwise system wide or Socket error code
		* @param aFCSession The Session which error is received to.
		*/
		virtual void FCErrorNotify( TInt aErrCode, const CFCSession* aFCSession ) = 0;
	};

#endif // end of __MFCObserver_H__

// End of File
