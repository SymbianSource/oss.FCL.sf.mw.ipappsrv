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




#ifndef __UT_CMCESTATEOFFERING_H__
#define __UT_CMCESTATEOFFERING_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "mceclientserver.h"
#include "MCEMediaManagerTestHelper.h"

//  FORWARD DECLARATIONS
class CMceSipSession;
class CMceCsSession;
class CMceServerCore;
class CMCETls;
class CMceStateOffering;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMceStateOffering
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMceStateOffering* NewL();
        static UT_CMceStateOffering* NewLC();
        /**
         * Destructor
         */
        ~UT_CMceStateOffering();

    private:    // Constructors and destructors

        UT_CMceStateOffering();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods

         void SetupL();
         void Setup2L();
         void Teardown();        
        
         void UT_CMceStateOffering_AcceptLL();
         void UT_CMceStateOffering_EntryL_WithUpdateL();
         void UT_CMceStateOffering_EntryL_WithProvisionalResponsesL();
         void UT_CMceStateOffering_EntryL_With2XXResponsesL();
         void UT_CMceStateOffering_EntryL_WithErrorResponsesL();
         void UT_CMceStateOffering_EntryL_WithOtherEventsL();
         void UT_CMceStateOffering_ExitLL();
        

    private:    // Data

    	EUNIT_DECLARE_TEST_TABLE; 
    		
    	TMceIds iIds;
        CMceServerCore* iServerCore;
        CMceCsSession* iSession;
        CMceSipSession* iSipSession;
        CMceServerStub *imServer;
    	CMceMediaManager* iManager;
		CMceSdpSession* iSdpSession;

    private: //not Owned


        CMCETls* iStorage;
		CMceStateOffering* iState;
		
    };

#endif      //  __UT_CMCESTATEOFFERING_H__

// End of file
