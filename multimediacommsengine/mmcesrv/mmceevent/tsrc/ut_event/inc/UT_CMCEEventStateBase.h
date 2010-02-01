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




#ifndef __UT_CMCEEVENTSTATEBASE_H__
#define __UT_CMCEEVENTSTATEBASE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMceEventStateBase;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMceEventStateBase
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMceEventStateBase* NewL();
        static UT_CMceEventStateBase* NewLC();
        /**
         * Destructor
         */
        ~UT_CMceEventStateBase();

    private:    // Constructors and destructors

        UT_CMceEventStateBase();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMceEventStateBase_ProcessStateLL();
        
        
         void UT_CMceEventStateBase_ProcessReqRecvStateLL();
        
        
         void UT_CMceEventStateBase_ProcessRespRecvStateLL();
        
        
         void UT_CMceEventStateBase_EnterStateL();
        

    private:    // Data

		EUNIT_DECLARE_TEST_TABLE; 
		
    };

#endif      //  __UT_CMCEEVENTSTATEBASE_H__

// End of file
