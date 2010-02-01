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

#ifndef __TTCSETTINGS_H__
#define __TTCSETTINGS_H__

// INCLUDES
#include <e32std.h>
#include <in_sock.h>

// CLASS DEFINITION
/**
 * TTcSettings implements a simple container for application settings.
 * It is able to load and store the settings items to/from a binary file
 * (c:\system\data\TestClient.ini).
 */
class TTcSettings
	{
	public:  // Constructors and destructor
            
		/// Default constructor
		TTcSettings();

		// Destructor
		~TTcSettings();

	public: // New methods

		/// Load the settings from the dedicated settings file.
		void Load();

		/// Save the settings to the settings file.
		void Store();

	private: // New methods

		/**
		 * Performs the actual loading from a open data stream
		 *
		 * @param aStream Reference to an initialized read stream
		 */
		void doLoadL( RReadStream& aStream );

		/**
		 * Performs the actual storing to a open data stream
		 *
		 * @param aStream Reference to an initialized write stream
		 */
		void doStoreL( RWriteStream& aStream );

	public: // Data

		/// Bearer selection (corresponding enumerations can be found from testclientconstants.h)
		TInt iBearerType;

		/// Autoconnect flag: TestClient starts listening for incoming
		/// connections automatically at startup if this is ETrue
		/// Default value is EFalse.
		TBool iAutoConnect;

		/// TCP Port number. Default value is 5070.
		TInt iTCPPort;

		/// Internet Access Point ID. Default value is 1.
		TInt iIAPId;

		/// Indicates if TestClient has been run before. Default value is ETrue.
		TBool iIsFirstRun;
		
		// CSY name (corresponding enumerations can be found from testclientconstants.h)
		TInt iCSYName;
		
		// Comms port name (corresponding enumerations can be found from testclientconstants.h)
		TInt iCommPort;
		
		// Baudrate of comms
		TInt iBaudrate;
		
		// TCP remote address. Default value is 0.0.0.0 which means that remote address is not used.
		TInetAddr iTCPRemoteAddr;

	};

#endif // __TTCSETTINGS_H__
