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
* Description:  
*
*/




#ifndef __UT_CFCINTERFACE_H__
#define __UT_CFCINTERFACE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES

#include "fcmessageobserver.h"
//  FORWARD DECLARATIONS
class CFCInterface;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CFCInterface: public CEUnitTestSuiteClass,
     public MFCMessageObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CFCInterface* NewL();
        static UT_CFCInterface* NewLC();
        /**
         * Destructor
         */
        ~UT_CFCInterface();

    private:    // Constructors and destructors

        UT_CFCInterface();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CFCInterface_NewLL();
        
		 void ReceivedData(HBufC8* aData, MFCPlugIn* aPlugIn);
      	 void ErrorNotify(TInt aErrCode, MFCPlugIn* aPlugIn);
    private:    // Data

		EUNIT_DECLARE_TEST_TABLE;
    };

#endif      //  __UT_CFCINTERFACE_H__

// End of file
