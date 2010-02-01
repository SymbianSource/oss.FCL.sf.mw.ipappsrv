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




#ifndef MMCETRANSACTIONOBSERVER_H
#define MMCETRANSACTIONOBSERVER_H

// INCLUDES
#include <mcedefs.h>
#include <e32std.h>  

// FORWARD DECLARATIONS
class TMceTransactionDataContainer;

// CLASS DECLARATION

/**
*  An interface to be implemented by users of MCE if they wish to 
*  send extension SIP requests such as INFO within an existing session.
*
*  This observer is set using CMceManager::SetTransactionObserver function.
*
*  @lib mceclient.lib
*/
class MMceTransactionObserver
	{
	public: // 

	    /**
	    * A response related to a transaction was received.
	    * @param aSession, the session that was used to create the transaction
	    * @param aTransactionId, the identifier for the transaction
	    * @param aContainer, if present, 
	    *        holds details of the incoming SIP response.
	    */
	    virtual void TransactionResponse(
	    			CMceSession& aSession,
	    			TMceTransactionId aTransactionId,
	    			TMceTransactionDataContainer* aContainer ) = 0;
	    
	    /**
	    * An error related to a transaction was received.
	    * @param aSession, the session that was used to create the transaction
	    * @param aTransactionId, the identifier for the transaction
	    * @param aError, Error code
	    */
		virtual void TransactionFailed( 
                    CMceSession& aSession,
	    			TMceTransactionId aTransactionId, 
	    			TInt aError ) = 0;
	};

#endif // MMCETRANSACTIONOBSERVER_H


// End of File

