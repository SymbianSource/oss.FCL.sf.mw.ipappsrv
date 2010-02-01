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




#ifndef __UT_TMCEOFFERINGMEDIA_H__
#define __UT_TMCEOFFERINGMEDIA_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include "MceMediaManagerTestHelper.h"
class TMceMediaState;
class CMceMediaManager;
class CMccControllerStub;

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_TMceOfferingMedia : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_TMceOfferingMedia* NewL();
        static UT_TMceOfferingMedia* NewLC();
        /**
         * Destructor
         */
        ~UT_TMceOfferingMedia();

    private:    // Constructors and destructors

        UT_TMceOfferingMedia();
        void ConstructL();



    private:    // New methods

         void SetupL();
        
         void Teardown();
        
        /**
        * Basic
        */
        void UT_TMceOfferingMedia_BasicL();

        /**
        * Tests
        *
        * void UpdateL();
        *
        */
        void UT_TMceOfferingMedia_UpdateLL();

        /**
        * Tests
        *
        * TMceSipWarningCode DecodeL( CSdpDocument& aSdpDocument );
        *
        */
        void UT_TMceOfferingMedia_DecodeLL();

        /**
        * Tests
        *
        * CSdpDocument* EncodeL();
        *
        */
        void UT_TMceOfferingMedia_EncodeLL();

        /**
        * Tests
        *
        * Callback();
        *
        */
        void UT_TMceOfferingMedia_CallbackL();


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

#endif      //  __UT_TMCEOFFERINGMEDIA_H__

// End of file
