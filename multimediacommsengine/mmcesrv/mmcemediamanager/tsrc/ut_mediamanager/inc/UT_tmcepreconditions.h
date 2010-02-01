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




#ifndef __UT_TMCEPRECONDITIONS_H__
#define __UT_TMCEPRECONDITIONS_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include "MceMediaManagerTestHelper.h"

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class TMceSegmentedPreconditions;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_TMceSegmentedPreconditions : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_TMceSegmentedPreconditions* NewL();
        static UT_TMceSegmentedPreconditions* NewLC();
        /**
         * Destructor
         */
        ~UT_TMceSegmentedPreconditions();

    private:    // Constructors and destructors

        UT_TMceSegmentedPreconditions();
        void ConstructL();



    private:    // New methods

        void SetupL();
        void Teardown();

        void UT_TMceSegmentedPreconditions_DoConstructLL();
        void UT_TMceSegmentedPreconditions_EncodeLL();
        void UT_TMceSegmentedPreconditions_DecodeLL();
        void UT_TMceSegmentedPreconditions_ReservedL();
        void UT_TMceSegmentedPreconditions_CloneLL();
        void UT_TMceSegmentedPreconditions_EncodeCurrentStatusLL();
        void UT_TMceSegmentedPreconditions_EncodeDesiredStatusLL();
        void UT_TMceSegmentedPreconditions_EncodeConfirmationLL();
        void UT_TMceSegmentedPreconditions_CurrentStausTextLCL();
        void UT_TMceSegmentedPreconditions_DesiredStausTextLCL();
        void UT_TMceSegmentedPreconditions_DecodeCurrentStatusLL();
        void UT_TMceSegmentedPreconditions_DecodeDesiredStatusLL();
        void UT_TMceSegmentedPreconditions_DecodeConfStatusLL();
        void UT_TMceSegmentedPreconditions_ParseCurrentStatusL();
        void UT_TMceSegmentedPreconditions_ParseDesiredStatusL();
        void UT_TMceSegmentedPreconditions_DecodeMediaDirectionLL();
        void UT_TMceSegmentedPreconditions_DecodeStrengthLL();
        void UT_TMceSegmentedPreconditions_SetStateL();
        void UT_TMceSegmentedPreconditions_SetRemoteStatusL();
        void UT_TMceSegmentedPreconditions_DesiredDirectionL();
        void UT_TMceSegmentedPreconditions_SetDesiredStatusL();
        
        
		/**
        * Tests: Decodes UnknownQoS parameters
        */
        void UT_TMceSegmentedPreconditions_DecodeUnknownQoSLL();

        

    private:    // Data


		CMceMediaManager* iManager;
		CMceComSession* iSession;
		CMceServerStub* iServer;
		CSdpDocument* iSdpDocument; 
		
        TMceSegmentedPreconditions* iPreconditions;

		EUNIT_DECLARE_TEST_TABLE; 

		
    };

#endif      //  __UT_TMCEPRECONDITIONS_H__

// End of file
