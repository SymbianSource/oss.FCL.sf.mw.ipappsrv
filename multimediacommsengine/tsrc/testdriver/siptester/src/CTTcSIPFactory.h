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

#ifndef __CTTCSIPFACTORY_H__
#define __CTTCSIPFACTORY_H__

// INCLUDES
#include "CTcContextFactory.h"

// CLASS DEFINITION
/**
 * CTTcSIPFactory implements the test context factory (CTcContextFactory)
 * for SIP API testing. It is resonposible for creating the SIP test
 * context.
 */
class CTTcSIPFactory
	: public CTcContextFactory
	{
	public:	// Constructors and destructor

        static CTTcSIPFactory* NewL();
        
        /// Destructor
		~CTTcSIPFactory();

	public: // from CTcContextFactory

		MTcTestContext* CreateLC( const TDesC8& aTestCaseName,
								  CTcCTRLCodec& aCodec,
								  TInt aIAPId ) const;
								  
    private:
    
		/// Default constructor.
		CTTcSIPFactory();
	};

#endif // __CTTCSIPFACTORY_H__
