/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  See class definition below.
*
*/

#ifndef __CTCWATCHERSERVER_H__
#define __CTCWATCHERSERVER_H__

//  INCLUDES
#include <e32base.h>
#include "MTcDeathObserver.h"

// FORWARD DECLARATIONS
class CTcAppLauncher;

//  CLASS DEFINITION
/**
 * CTcWatcher is the main server class responsible for starting
 * and running the server process/thread.
 */
class CTcWatcherServer
#ifdef __IPC_V2_PRESENT__
   : public CServer2,
#else
   : public CServer,
#endif
 	  public MTcDeathObserver
    {
    public: // Constructors and destructor

		/**
		 * Static constructor, leaves pointer to cleanup-stack
		 *
		 * @return An initialised instance of this class.
		 */
		static CTcWatcherServer* NewLC();

		/// Destructor
		~CTcWatcherServer();

    private: // Constructors

		/// Default constructor
        CTcWatcherServer();

		/// 2nd phase constructor
        void ConstructL();

	private: // From CActive

		TInt RunError( TInt aError );

	private: // From CServer

#ifdef __IPC_V2_PRESENT__
	    CSession2* NewSessionL( const TVersion& aVersion,
	    						const RMessage2& aMessage ) const;
#else
	    CSharableSession* NewSessionL( const TVersion& aVersion ) const;
#endif

   public: // from MTcDeathObserver

		void Died( const TDesC& aAppName, TBool aDismissDialog );

	public: // New methods

		/**
		 * Server entry point method (Thread entry point!)
		 * Starts the server up. Creates a cleanupstack and activescheduler
		 *
		 * @param aParam Thread arguments. Not used.
		 */
		static TInt ThreadMain( TAny* aParam );

		/// Called by a session when it is created. Increments iSessionCount
		void SessionAdded();

		/// Called by a session when it is destroyed. Decrements iSessionCount.
		/// Stops the ActiveScheduler if there are no more sessions.
		void SessionRemoved();

		/// Shutdown timer callback function
		static TInt Shutdown( TAny* aPtr );

		/// Called by ThreadMain, starts and initialises the server.
		/// This function will block for the lifetime of the server.
		static void InitServerL();

	private: // Data

		/// Number of active sessions.
		TInt iSessionCount;

		/// Server shutdown timer. Owned.
		CDeltaTimer* iShutdownTimer;

		/// Server shutdown timer entry item. Owned.
		TDeltaTimerEntry iShutdownTimerEntry;

		/// Array of application launchers. Owned.
		CArrayPtrFlat< CTcAppLauncher > iLaunchers;

    };

#endif // __CTCWATCHERSERVER_H__
