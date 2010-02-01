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

#ifndef __RTCFILEHANDLER_H__
#define __RTCFILEHANDLER_H__

//	INCLUDES
#include <e32base.h>

//	CLASS DEFINITION
/**
 * RTcFileHandler implements the client-side endpoint of a FileHandler
 * client-server session.
 */
class RTcFileHandler
	: public RSessionBase
	{
	public: // Constructors and destructor

		/// Default constructor
		IMPORT_C RTcFileHandler();

	public: // from RHandleBase

		/// Closes the connection to the server.
		/// Stops watching for this client.
		IMPORT_C void Close();

	public:	// New methods

		/**
		 * Establishes a connection to the FileHandler server.
		 * The server is automatically started if it was not already
		 * up and running.
		 *
		 */
		IMPORT_C TInt Connect();


		IMPORT_C TInt CreateFile( const TDesC8& aDestinationPath, 
                                  const TDesC8& aData );
                                
        IMPORT_C TInt CopyFile( const TDesC8& aSourcePath,
                                const TDesC8& aDestinationPath );

        IMPORT_C TInt DeleteFile( const TDesC8& aDestinationPath );
        
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

#endif  // __RTCFILEHANDLER_H__
