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




#ifndef MMCESESSIONOBSERVER_H
#define MMCESESSIONOBSERVER_H

// INCLUDES
#include <e32std.h>  

// FORWARD DECLARATIONS
class CMceSession;
class TMceTransactionDataContainer;

// CLASS DECLARATION

/**
*  An interface to be implemented by users of MCE if they wish to use
*  sessions.
*
*  This observer is set using CMceManager::SetSessionObserver function.
*
*  @lib mceclient.lib
*/
class MMceSessionObserver
	{
	public: // Session control callbacks

	    /**
	    * The state of the session has changed.
	    * @param aSession, the session that has changed.
	    * @param aContainer, if present, holds details of
	    *        transaction causing state change.
	    */
	    virtual void SessionStateChanged(
	    			CMceSession& aSession,
	    			TMceTransactionDataContainer* aContainer ) = 0;

	    /**
	    * The state of the connection used by the session has changed.
	    * @param aSession, the session that has changed.
	    * @param aActive, ETrue if connection active, EFalse if connection inactive.
	    */
	    virtual void SessionConnectionStateChanged(
	                CMceSession& aSession,
	                TBool aActive ) = 0;
	    
	    /**
	    * An error has occurred concerning a specific session.
	    * Note, that each error causes the session state to be ETerminated.
	    * @param aSession, The session raising the error.
	    * @param aError, Error code
	    */
		virtual void Failed( CMceSession& aSession, TInt aError ) = 0;

		/**
	    * An session update has been failed concerning a specific session.
	    * Note, that each error causes the session state to be ETerminated.
	    * @param aSession, The session raising the error.
	    * @param aContainer, if present, holds details of
	    *        transaction causing update.
	    */
		virtual void UpdateFailed(
	    			CMceSession& aSession,
	    			TMceTransactionDataContainer* aContainer ) = 0;
	};

#endif
