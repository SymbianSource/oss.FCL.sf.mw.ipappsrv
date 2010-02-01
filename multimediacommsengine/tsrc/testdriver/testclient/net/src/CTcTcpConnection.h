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

#ifndef __CTCTCPCONNECTION_H__
#define __CTCTCPCONNECTION_H__

// INCLUDES
#include "CTcBaseConnection.h"

// CLASS DEFINITION
/**
 * CTcTcpConnection implements TCP/IP specific connection
 * initialization on top of CTcBaseConnection.
 */
class CTcTcpConnection
	: public CTcBaseConnection
	{
	public:	// Constructors and destructor

		/**
		 * Constructor
		 *
		 * @param aPort Local TCP port number for the connection
		 */
		CTcTcpConnection( TInt aPort );

		/// Destructor
		~CTcTcpConnection();

	private: // Constructors and destructor

		/// Default constructor. Not implemented.
		CTcTcpConnection();

	protected: // From CTcBaseConnection

		void SetupPortL();
		
		void SetupPort2L();

	};

#endif // __CTCTCPCONNECTION_H__
