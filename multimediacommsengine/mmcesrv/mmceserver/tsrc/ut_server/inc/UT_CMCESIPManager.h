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




#ifndef __UT_CMceSipMANAGER_H__
#define __UT_CMceSipMANAGER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "mceclientserver.h"

//  FORWARD DECLARATIONS
class CMceSipManager;
class CMceServerCore;
class CMceCsSession;
class CMCETls;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMceSipManager
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMceSipManager* NewL();
        static UT_CMceSipManager* NewLC();
        /**
         * Destructor
         */
        ~UT_CMceSipManager();

    private:    // Constructors and destructors

        UT_CMceSipManager();
        void ConstructL();

    private:    // New methods

        void SetupL();

        void Teardown();
         
        void UT_CMceSipManager_ProfileL_1_L();

        void UT_CMceSipManager_ProfileL_2_L();

        void UT_CMceSipManager_ConnectionLL();
        
        void UT_CMceSipManager_IncomingRequestL();

        void UT_CMceSipManager_TimedOutL();

        void UT_CMceSipManager_ProfileRegistryEventOccurredL();

        void UT_CMceSipManager_ProfileRegistryErrorOccurredL();

        void UT_CMceSipManager_MediaErrorL();

        void UT_CMceSipManager_NextDialogIdL();

        void UT_CMceSipManager_UnRegisterClientL();

        void UT_CMceSipManager_CreateSubSessionL_1L();

        void UT_CMceSipManager_CreateSubSessionL_2L();
        
        void UT_CMceSipManager_CreateSubSessionL_3L();

        void UT_CMceSipManager_CreateSubSessionNoProfileL();
        
        void UT_CMceSipManager_UnRegisterSubSessionL();

        void UT_CMceSipManager_UnRegisterSubSessionNoProfileL();

        void UT_CMceSipManager_MediaManagerL();

        void UT_CMceSipManager_SIPClientL();

        void UT_CMceSipManager_OrphanSubSessionCountL();
        
        void UT_CMceSipManager_UpdateProfileToSubSessionsL();
         

    private:    // Data

		EUNIT_DECLARE_TEST_TABLE; 
		TMceIds iIds;
	    CMceServerCore* iServerCore;
        CMceSipManager* iManager;
        CMceCsSession* iSession;
	    
    private: //not Owned
    	    
        CMCETls* iStorage;
	    
		
    };

#endif      //  __UT_CMceSipMANAGER_H__

// End of file
