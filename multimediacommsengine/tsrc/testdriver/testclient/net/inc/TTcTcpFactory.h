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

#ifndef __TTCTCPFACTORY_H__
#define __TTCTCPFACTORY_H__

// INCLUDES
#include "MTcBearerFactory.h"

// CLASS DEFINITION
/**
 * TTcTcpFactory implements a factory class for creating
 * CTcTcpManager instances.
 */
class TTcTcpFactory
	: public MTcBearerFactory
	{
	public:	// Constructors and destructor

		/**
		 * Constructor
		 *
		 * @param aPort Local TCP port number for the connection
		 * @param aIAPId IAP Id for the connection
		 *				 Default value is 0 = ask user
		 */
		IMPORT_C TTcTcpFactory( TInt aPort, TInt aIAPId = 0 );

		/// Destructor
		IMPORT_C ~TTcTcpFactory();

	private: // Constructors and destructor

		/// Default constructor. Not implemented.
		TTcTcpFactory();

	public: // FromMTcBearerFactory

		MTcBearerManager* CreateManagerL() const;

	public: // Data

		/// Local TCP port number for the connection
		TInt iPort;

		/// IAP Id for the connection
		TInt iIAPId;

	};

#endif // __TTCTCPFACTORY_H__
