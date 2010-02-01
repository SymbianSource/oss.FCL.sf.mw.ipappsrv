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

#ifndef __CTCBTCONNECTION_H__
#define __CTCBTCONNECTION_H__

// INCLUDES
#include "CTcBaseConnection.h"

// CLASS DEFINITION
/**
 * CTcBtConnection implements Bluetooth RFCOMM specific connection
 * initialization on top of CTcBaseConnection.
 */
class CTcBtConnection
	: public CTcBaseConnection
	{
	public:	// Constructors and destructor

		/// Default constructor
		CTcBtConnection();

		/// Destructor
		~CTcBtConnection();

	protected: // From CTcBaseConnection

		void SetupPortL();

	};

#endif // __CTCBTCONNECTION_H__
