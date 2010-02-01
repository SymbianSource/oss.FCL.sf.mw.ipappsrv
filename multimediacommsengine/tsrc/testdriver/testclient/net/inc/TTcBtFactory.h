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

#ifndef __TTCBTFACTORY_H__
#define __TTCBTFACTORY_H__

// INCLUDES
#include "MTcBearerFactory.h"

// CLASS DEFINITION
/**
 * TTcBtFactory implements a factory class for creating
 * CTcBtManager instances.
 */
class TTcBtFactory
	: public MTcBearerFactory
	{
	public:	// Constructors and destructor

		/// Default constructor
		IMPORT_C TTcBtFactory();

		/// Destructor
		IMPORT_C ~TTcBtFactory();

	public: // FromMTcBearerFactory

		MTcBearerManager* CreateManagerL() const;

	};

#endif // __TTCBTFACTORY_H__
