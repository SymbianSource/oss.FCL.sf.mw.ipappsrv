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




#ifndef __UT_TMCESECUREPRECONDITIONS_H__
#define __UT_TMCESECUREPRECONDITIONS_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include "MceMediaManagerTestHelper.h"

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class TMceSecurePreconditions;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_TMceSecurePreconditions : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_TMceSecurePreconditions* NewL();
        static UT_TMceSecurePreconditions* NewLC();
        /**
         * Destructor
         */
        ~UT_TMceSecurePreconditions();

    private:    // Constructors and destructors

        UT_TMceSecurePreconditions();
        void ConstructL();



    private:    // New methods

        void SetupL();
        void Teardown();

        void UT_TMceSecurePreconditions_DoConstructLL();
        void UT_TMceSecurePreconditions_EncodeLL();
        void UT_TMceSecurePreconditions_DecodeLL();
        void UT_TMceSecurePreconditions_ReservedL();
        void UT_TMceSecurePreconditions_CloneLL();
        void UT_TMceSecurePreconditions_EncodeCurrentStatusLL();
        void UT_TMceSecurePreconditions_EncodeDesiredStatusLL();
        void UT_TMceSecurePreconditions_EncodeConfirmationLL();
        void UT_TMceSecurePreconditions_CurrentStatusTextLCL();
        void UT_TMceSecurePreconditions_DesiredStatusTextLCL();
        void UT_TMceSecurePreconditions_DecodeCurrentStatusLL();
        void UT_TMceSecurePreconditions_DecodeDesiredStatusLL();
        void UT_TMceSecurePreconditions_DecodeConfStatusLL();
        void UT_TMceSecurePreconditions_ParseCurrentStatusL();
        void UT_TMceSecurePreconditions_ParseDesiredStatusL();
        void UT_TMceSecurePreconditions_DecodeMediaDirectionLL();
        void UT_TMceSecurePreconditions_DecodeStrengthLL();
        void UT_TMceSecurePreconditions_SetStateL();
        void UT_TMceSecurePreconditions_SetRemoteStatusL();
        void UT_TMceSecurePreconditions_SetDesiredStatusL();
        void UT_TMceSecurePreconditions_StrengthDowngradedL();
        void UT_TMceSecurePreconditions_DecodeUnknownSecLL();
        

    private:    // Data


		CMceMediaManager* iManager;
		CMceComSession* iSession;
		CMceServerStub* iServer;
		CSdpDocument* iSdpDocument; 
		
        TMceSecurePreconditions* iPreconditions;

		EUNIT_DECLARE_TEST_TABLE; 

		
    };

#endif      //  __UT_TMCESECUREPRECONDITIONS_H__

// End of file
