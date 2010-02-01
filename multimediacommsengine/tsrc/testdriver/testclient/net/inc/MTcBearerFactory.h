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

#ifndef __MTCBEARERFACTORY_H__
#define __MTCBEARERFACTORY_H__

// INCLUDES
#include <e32std.h>

// FORWARD DECLARATIONS
class MTcBearerManager;

// CLASS DEFINITION
/**
 * MTcBearerFactory defines an interface for bearer factories
 * (abstract factory design pattern).
 */
class MTcBearerFactory
	{
	public:	// Constructors and destructor

		/// Virtual destructor. Allow deletion through this interface.
		virtual ~MTcBearerFactory() {}

	public: // Abstract methods

		/// @return pointer to a created MTcBearerManager implementation.
		/// Ownership is transferred.
		virtual MTcBearerManager* CreateManagerL() const = 0;

	};

#endif // __MTCBEARERFACTORY_H__
