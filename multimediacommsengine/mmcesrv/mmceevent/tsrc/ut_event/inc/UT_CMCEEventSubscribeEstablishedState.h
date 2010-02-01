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




#ifndef __UT_CMCEEVENTSUBSCRIBEESTABLISHEDSTATE_H__
#define __UT_CMCEEVENTSUBSCRIBEESTABLISHEDSTATE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMceEventSubscribeEstablishedState;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMceEventSubscribeEstablishedState
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMceEventSubscribeEstablishedState* NewL();
        static UT_CMceEventSubscribeEstablishedState* NewLC();
        /**
         * Destructor
         */
        ~UT_CMceEventSubscribeEstablishedState();

    private:    // Constructors and destructors

        UT_CMceEventSubscribeEstablishedState();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMceEventSubscribeEstablishedState_NewLL();
        

    private:    // Data

		EUNIT_DECLARE_TEST_TABLE; 
		
    };

#endif      //  __UT_CMCEEVENTSUBSCRIBEESTABLISHEDSTATE_H__

// End of file
