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




#ifndef MMCEINREFEROBSERVER_H
#define MMCEINREFEROBSERVER_H

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CMceInRefer;
class TMceTransactionDataContainer;

// CLASS DECLARATION

/**
*  An interface to be implemented by users of MCE if they wish to receive
*  refers from network.
*
*  This observer is set using CMceManager::SetInReferObserver function.
*
*  @lib mceclient.lib
*/
class MMceInReferObserver
    {
    public: // New functions
        
        /**
        * New incoming refer received.
		* @param aRefer, the new inbound refer. Ownership is
		*		 transferred.
        * @param aReferTo SIP specific Refer-to header 
	    * @param aContainer, if present, holds details of
	    *        transaction causing state change.
        */
        virtual void IncomingRefer(
					CMceInRefer* aRefer,
					const TDesC8& aReferTo,
    				TMceTransactionDataContainer* aContainer ) = 0;
    };

#endif
