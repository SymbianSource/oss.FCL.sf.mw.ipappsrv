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

#ifndef __RTCWATCHER_H__
#define __RTCWATCHER_H__

//	INCLUDES
#include <e32base.h>

//	CLASS DEFINITION
/**
 * RTcWatcher implements the client-side endpoint of a Watcher
 * client-server session. A connected client must call Ping()
 * at intervals shorter than aTimeout specified in Connect().
 * Failing to do that will result in the application specified
 * in Connect() being terminated and restarted.
 */
class RTcWatcher
	: public RSessionBase
	{
	public: // Constructors and destructor

		/// Default constructor
		IMPORT_C RTcWatcher();

	public: // from RHandleBase

		/// Closes the connection to the server.
		/// Stops watching for this client.
		IMPORT_C void Close();

	public:	// New methods

		/**
		 * Establishes a connection to the Watcher server.
		 * The server is automatically started if it was not already
		 * up and running.
		 *
		 * Start watching for this client. The client is killed if it does
		 * not report in every aTimeout seconds. The client is restarted
		 * if it dies by itself or gets killed by the timeout.
		 *
		 * @param aAppName Application name and full path, for restarting.
		 * @param aTimeout Kill timeout.
		 * @return KErrNone if operation was successful, otherwise
		 *		   one of the standard system-wide error codes.
		 */
		IMPORT_C TInt Connect( const TDesC& aAppName, TInt aTimeout );

		/// To be called a couple of times within the aTimeout
		/// Failing to do that will result in client death.
		IMPORT_C TInt Ping();

		/// @return Server version number.
		IMPORT_C TVersion Version() const;

	private: // New methods

		/// Starts the server process.
		void StartServerL();

		/// @return ETrue if the server is already/still running
		TBool IsServerStarted() const;

	private: // Data

		/// ETrue if there is an open connection, EFalse otherwise.
		TBool iIsConnected;

    };

#endif  // __RTCWATCHER_H__
