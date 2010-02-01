/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef __UT_TMCEANSWERINGMEDIA_H__
#define __UT_TMCEANSWERINGMEDIA_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include "MceMediaManagerTestHelper.h"

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class TMceMediaState;
class CMceMediaManager;
class CMccControllerStub;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_TMceAnsweringMedia : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_TMceAnsweringMedia* NewL();
        static UT_TMceAnsweringMedia* NewLC();
        /**
         * Destructor
         */
        ~UT_TMceAnsweringMedia();

    private:    // Constructors and destructors

        UT_TMceAnsweringMedia();
        void ConstructL();



    private:    // New methods

         void SetupL();
        
         void Teardown();
                
        /**
        * Basic
        */
        void UT_TMceAnsweringMedia_BasicL();

        /**
        * Tests
        *
        * void UpdateL();
        *
        */
        void UT_TMceAnsweringMedia_UpdateLL();

    private:    // Data


        TMceMediaState* iState;
		CMceMediaManager* iManager;
		CMceComSession* iSession;
		CMceComSession* iInSession;
		CSdpDocument* iSdpDocument; 
		CMceServerStub* iServer;
		CMccControllerStub* iMcc;

		EUNIT_DECLARE_TEST_TABLE; 

		
    };

#endif      //  __UT_TMCEANSWERINGMEDIA_H__

// End of file
