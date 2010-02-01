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




#ifndef MMCEREFEROBSERVER_H
#define MMCEREFEROBSERVER_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CMceRefer;
class TMceTransactionDataContainer;

// CLASS DECLARATION

/**
*  An interface to be implemented by users of MCE if they wish to use
*  refers.
*
*  This observer is set using CMceManager::SetReferObserver function.
*
*  @lib mceclient.lib
*/
class MMceReferObserver
    {
    public: // New functions
        
		/**
        * The state of the refer has changed.
        * @param aRefer, the refer that has changed.
	    * @param aContainer, if present, holds details of
	    *        transaction causing state change.
        */
        virtual void ReferStateChanged(
        			CMceRefer& aRefer,
    				TMceTransactionDataContainer* aContainer ) = 0;
			
	    /**
	    * The state of the connection used by the refer has changed.
	    * @param aRefer, the refer that has changed.
	    * @param aActive, ETrue if connection active, EFalse if connection inactive.
	    */
	    virtual void ReferConnectionStateChanged(
	                CMceRefer& aRefer,
	                TBool aActive ) = 0;
                
		/**
		* An error has occurred concerning a specific SIP refer.
		* Note, that each error causes the event state to be ETerminated.
		* @param aRefer, The refer raising the error.
		* @param aError, Error code
		*/
		virtual void Failed(CMceRefer& aRefer, TInt aError ) = 0;
    };

#endif
