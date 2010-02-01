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




#ifndef MMCEEVENTOBSERVER_H
#define MMCEEVENTOBSERVER_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CMceEvent;
class TMceTransactionDataContainer;

// CLASS DECLARATION

/**
*  An interface to be implemented by users of MCE if they wish to use
*  events.
*
*  This observer is set using CMceManager::SetEventObserver function.
*
*  @lib mceclient.lib
*/
class MMceEventObserver
    {
    public: // New functions
        
        /**
        * The state of the event has changed.
        * @param aEvent, the event that has changed.
	    * @param aContainer, if present, holds details of
	    *        transaction causing state change.
        */
        virtual void EventStateChanged(
        			CMceEvent& aEvent,
    				TMceTransactionDataContainer* aContainer ) = 0;

		/**
		* The state of the event has changed.
		* @param aEvent, event received notification.
	    * @param aContainer, if present, holds details of
	    *        transaction causing state change.
		*/
		virtual void NotifyReceived(
					CMceEvent& aEvent,
    				TMceTransactionDataContainer* aContainer ) = 0;
					
					
	    /**
	    * The state of the event used by the refer has changed.
	    * @param aEvent, the event that has changed.
	    * @param aActive, ETrue if connection active, EFalse if connection inactive.
	    */
	    virtual void EventConnectionStateChanged(
	                CMceEvent& aEvent,
	                TBool aActive ) = 0;
        
		/**
		* An error has occurred concerning a specific SIP event.
		* Note, that each error causes the event state to be ETerminated.
		* @param aEvent, The event raising the error.
		* @param aError, Error code
		*/
		virtual void Failed(CMceEvent& aEvent, TInt aError ) = 0;

    };

#endif
