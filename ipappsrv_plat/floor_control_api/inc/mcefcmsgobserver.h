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





#ifndef MMCEFCMSGOBSERVER_H
#define MMCEFCMSGOBSERVER_H

// INCLUDES
#include <e32base.h>

// CLASS DECLARATION
class CMceSession;

/**
*  An interface to be implemented by users of MCE if they wish to use
*  floor control.
*
*  This observer is set using CMceFcMsgExchange::InitializeL function.
*
*  @lib mceclient.lib
*/
class MMceFcMsgObserver
	{
    public: // New functions
                 
        /**
        * Incoming floor control message.
        * @param aSession FC session that message was received.
        * @param aMessage Received floor control message. Owneship is
		*	     transferred.
        */
        virtual void FCMsgReceived(
					CMceSession& aSession,
					HBufC8* aMessage ) = 0;

        /**
        * An error has occurred concerning floor control.
        * @param aSession FC session that error was occurred.
        * @param aError Error code
        */
        virtual void FCMsgErrorOccurred(
					CMceSession& aSession,
					TInt aError ) = 0;
    };

#endif      
            
// End of File
