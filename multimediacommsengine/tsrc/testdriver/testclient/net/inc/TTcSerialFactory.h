/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __TTCSERIALFACTORY_H__
#define __TTCSERIALFACTORY_H__

// INCLUDES
#include "MTcBearerFactory.h"


const TInt KMaxCSYNameLen = 20;
const TInt KMaxPortNameLen = 20;

// CLASS DEFINITION
/**
 * TTcSerialFactory implements a factory class for creating
 * CTcSerialManager instances.
 */
class TTcSerialFactory
	: public MTcBearerFactory
	{
	public:	// Constructors and destructor

		/// Default constructor
		IMPORT_C TTcSerialFactory( const TDesC& aCSYName, 
                                   const TDesC& aPortName, 
                                   TUint aPortSpeed );

		/// Destructor
		IMPORT_C ~TTcSerialFactory();

	public: // FromMTcBearerFactory

		MTcBearerManager* CreateManagerL() const;
		
    public: // Data
    
        TBuf<KMaxCSYNameLen> iCSYName;
	    TBuf<KMaxPortNameLen> iPortName;
	    TUint iPortSpeed;

	};

#endif // __TTCSERIALFACTORY_H__
