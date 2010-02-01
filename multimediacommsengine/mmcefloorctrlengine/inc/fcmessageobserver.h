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




#ifndef __MFCMESSAGEOBSERVER_H__
#define __MFCMESSAGEOBSERVER_H__

//  INCLUDES
#include <e32base.h>

// FORWARD CLASS DECLARATIONS
class MFCPlugIn;

// CLASS DECLARATION
/**
* MFCConnectionNotifier defines an interface for observing receiving data 
* and erro notify from the predefined plugin
* @lib FCPlugInEngine.lib
*/
class MFCMessageObserver
	{
	protected: // Constructors and destructors
		/**
        * Virtual destructor. Prohibit deletion through this interface.        
        */
		virtual ~MFCMessageObserver() {};
	
	public:
	    /**
	    * Called by specific plugIn
	    * @param aData, Receving Data
	    * @param aPlugIn, The FC plug-in giving this notification.
        */
	    virtual void ReceivedData( HBufC8* aData, MFCPlugIn* aPlugin ) = 0;

	    /**
	    * Called by FC PlugIn
	    * @param aErrCode KErrNone if connection succesfully, otherwise
	    *		 one of the system-wide error codes.
	    * @param aPlugIn, The FC plug-in giving this notification.
	    */
	    virtual void ErrorNotify( TInt aErrCode, MFCPlugIn* aPlugIn ) = 0;
	};

#endif // end of __MFCMessageObserver_H__

// End of File
