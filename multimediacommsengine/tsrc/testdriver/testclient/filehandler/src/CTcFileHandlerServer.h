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

#ifndef __CTCFILEHANDLERSERVER_H__
#define __CTCFILEHANDLERSERVER_H__

//  INCLUDES
#include <e32base.h>

//  CLASS DEFINITION
/**
 * CTcFileHandler is the main server class responsible for starting
 * and running the server process/thread.
 */
class CTcFileHandlerServer : public CServer2
    {
    public: // Constructors and destructor

		/**
		 * Static constructor, leaves pointer to cleanup-stack
		 *
		 * @return An initialised instance of this class.
		 */
		static CTcFileHandlerServer* NewLC();

		/// Destructor
		~CTcFileHandlerServer();

    private: // Constructors

		/// Default constructor
        CTcFileHandlerServer();

		/// 2nd phase constructor
        void ConstructL();

	private: // From CActive

		TInt RunError( TInt aError );

	private: // From CServer

	    CSession2* NewSessionL( const TVersion& aVersion,
	    						const RMessage2& aMessage ) const;

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

		/// Called by ThreadMain, starts and initialises the server.
		/// This function will block for the lifetime of the server.
		static void InitServerL();

	private: // Data

		/// Number of active sessions.
		TInt iSessionCount;

    };

#endif // __CTCFILEHANDLERSERVER_H__
