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




#ifndef __UT_TMCEMEDIANEGOTIATED_H__
#define __UT_TMCEMEDIANEGOTIATED_H__

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
class UT_TMceMediaNegotiated : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_TMceMediaNegotiated* NewL();
        static UT_TMceMediaNegotiated* NewLC();
        /**
         * Destructor
         */
        ~UT_TMceMediaNegotiated();

    private:    // Constructors and destructors

        UT_TMceMediaNegotiated();
        void ConstructL();



    private:    // New methods

         void SetupL();
        
         void Teardown();
        
        /**
        * Basic
        */
        void UT_TMceMediaNegotiated_BasicL();

        /**
        * Tests
        *
        * void UpdateL();
        *
        */
        void UT_TMceMediaNegotiated_UpdateLL();

        /**
        * Tests
        *
        * TMceSipWarningCode DecodeL( CSdpDocument& aSdpDocument );
        *
        */
        void UT_TMceMediaNegotiated_DecodeLL();

        /**
        * Tests
        *
        * CSdpDocument* EncodeL();
        *
        */
        void UT_TMceMediaNegotiated_EncodeLL();

        /**
        * Tests
        *
        * Callback();
        *
        */
        void UT_TMceMediaNegotiated_CallbackL();

        /**
        * Tests
        *
        * NeedToNegotiate( CSdpDocument* aSdpDocument );
        *
        */
        void UT_TMceMediaNegotiated_NeedToNegotiateL();
        
        /**
        * Tests
        *
        * AnswerL();
        *
        */
        void UT_TMceMediaNegotiated_AnswerL();

        
    private:    // Data

        TMceMediaState* iState;
		CMceMediaManager* iManager;
		CMceComSession* iSession;
		CMceComSession* iBackup;
		CSdpDocument* iSdpDocument; 
		CMceServerStub* iServer;
		CMccControllerStub* iMcc;


		EUNIT_DECLARE_TEST_TABLE; 

		
    };

#endif      //  __UT_TMCEMEDIANEGOTIATED_H__

// End of file
