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




#ifndef MMCEINEVENTOBSERVER_H
#define MMCEINEVENTOBSERVER_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CMceInEvent;
class TMceTransactionDataContainer;

// CLASS DECLARATION

/**
*  An interface to be implemented by users of MCE if they wish to receive
*  event subscriptions from network.
*
*  This observer is set using CMceManager::SetInEventObserver function.
*
*  @lib mceclient.lib
*/
class MMceInEventObserver
    {
    public: // New functions
        
        /**
        * New incoming event received.
		* @param aEvent, the new event. Ownership is
		*		 transferred.
	    * @param aContainer, if present, holds details of
	    *        transaction causing state change.
        */
        virtual void IncomingEvent(
					CMceInEvent* aEvent,
    				TMceTransactionDataContainer* aContainer ) = 0;
					
    };

#endif
