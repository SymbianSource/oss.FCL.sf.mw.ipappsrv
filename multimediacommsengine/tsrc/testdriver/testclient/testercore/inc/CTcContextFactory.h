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

#ifndef __CTCCONTEXTFACTORY_H__
#define __CTCCONTEXTFACTORY_H__

// INCLUDES
#include <e32base.h>
#include <ecom/ecom.h>

#include "MTcTestContext.h"

// FORWARD DECLARATIONS
class CTcCTRLCodec;

	
// CLASS DEFINITION
/**
 * CTcContextFactory defines a factory interface for creating ECom plugin
 * test contexts.
 */
class CTcContextFactory : public CBase
	{
	public: // Constructors and destructors

		/// Virtual destructor. Allows deletion through this interface.
		virtual inline ~CTcContextFactory() 
		    {
			REComSession::DestroyedImplementation(iInstanceKey);
		    };
										  
	public:	
	
	    /**
		 * Creates and initializes a new test context. The concrete context
		 * type is chosen based on the specified testcase name.
		 * Leaves pointer to clenaup stack.
		 *
		 * @param aTestCaseName Used for determining context type
		 *						Should in form "CONTEXT:testcasename",
		 *						e.g. "SIP:REG_36"
		 * @param aCodec CTRL protocol codec reference, to be passed down to
		 *				 the instantiated codec.
		 * @param aIAPId IAPId as selected by user from TC settings.
		 * @return An initialized test context implementation object.
		 *		   Must return NULL if the factory implementation did not
		 *		   support the specified context type (in test case name).
		 */
		virtual MTcTestContext* CreateLC( const TDesC8& aTestCaseName,
										  CTcCTRLCodec& aCodec,
										  TInt aIAPId ) const = 0;
		
   protected:
    
        inline CTcContextFactory(){};
        
   public:

	    /// Unique key for implementations of this interface.
		TUid iInstanceKey;
		
	};

#endif // __CTCCONTEXTFACTORY_H__
