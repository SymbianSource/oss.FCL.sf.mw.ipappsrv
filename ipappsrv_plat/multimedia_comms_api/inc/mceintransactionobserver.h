/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef MMCEINTRANSACTIONOBSERVER_H
#define MMCEINTRANSACTIONOBSERVER_H

// INCLUDES
#include <mcedefs.h>
#include <e32std.h>  

// FORWARD DECLARATIONS
class TMceTransactionDataContainer;
class CMceSession;

// CLASS DECLARATION

/**
*  An interface to be implemented by users of MCE if they wish to receive
*  extension SIP requests such as INFO within an existing session.
*
*  This observer is set using CMceManager::SetInTransactionObserver function.
*
*  @lib mceclient.lib
*/
class MMceInTransactionObserver
	{
	public: // Session control callbacks

		/**
        * New incoming Request received.
		* @param aSession
        * @param aTransactionId transaction id of the transaction
	    * @param aContainer, holds details of the incoming SIP request.
        */
        virtual void IncomingRequest(
        			const TDesC8& aMethod,
					CMceSession& aSession,
	    			TMceTransactionId aTransactionId,
	    			TMceTransactionDataContainer* aContainer ) = 0;

	};

#endif // MMCEINTRANSACTIONOBSERVER_H


//End of File

