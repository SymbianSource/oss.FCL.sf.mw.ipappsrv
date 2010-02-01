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
* Description:    See class definition below.
*
*/



#ifndef __CTTCMCEFACTORY_H__
#define __CTTCMCEFACTORY_H__

// INCLUDES
#include "CTcContextFactory.h"

// CLASS DEFINITION
/**
 * CTcMCEFactory implements the test context factory (CTcContextFactory)
 * for MCE API testing. It is resonposible for creating the MCE test
 * context.
 */
class CTcMCEFactory
	: public CTcContextFactory
	{
	public:	// Constructors and destructor

        static CTcMCEFactory* NewL();
        
        /// Destructor
		~CTcMCEFactory();
		
		CTcMCEFactory();

	public: // from CTcContextFactory

		MTcTestContext* CreateLC( const TDesC8& aTestCaseName,
								  CTcCTRLCodec& aCodec,
								  TInt aIAPId ) const;
								  
    private:
    
		/// Default constructor.
		
	};

#endif // __CTTCMCEFACTORY_H__
