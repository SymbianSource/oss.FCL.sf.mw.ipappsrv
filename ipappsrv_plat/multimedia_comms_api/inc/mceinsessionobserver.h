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




#ifndef MMCEINSESSIONOBSERVER_H
#define MMCEINSESSIONOBSERVER_H

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CMceSession;
class CMceInSession;
class TMceTransactionDataContainer;

// CLASS DECLARATION

/**
*  An interface to be implemented by users of MCE if they wish to receive
*  invitations to session from network.
*
*  This observer is set using CMceManager::SetInSessionObserver function.
*
*  @lib mceclient.lib
*/
class MMceInSessionObserver
	{

	public: // Functions

	    /**
	    * Incoming session invitation. The CMceInSession is given to the
		* application as a callback.
	    * @param aSession, pointer to the incoming session. Ownership is
		*	     transferred.
	    * @param aContainer, if present, holds details of
	    *        transaction causing state change.
	    */
		virtual void IncomingSession(
	                CMceInSession* aSession,
    				TMceTransactionDataContainer* aContainer ) = 0;
    				
    				
	    /**
	    * Incoming session update. The new updated CMceInSession is given to the
		* application as a callback.
	    * @param aOrigSession, the original session to be updated. This instance
	    *        cannot be used anymore, all actions done using aUpdatedSession instance.
	    * @param aUpdatedSession, pointer to the new updated session. Ownership is
		*	     transferred.
	    * @param aContainer, if present, holds details of
	    *        update transaction.
	    */
		virtual void IncomingUpdate(
					CMceSession& aOrigSession, 
					CMceInSession* aUpdatedSession,
    				TMceTransactionDataContainer* aContainer ) = 0;

	};

#endif
