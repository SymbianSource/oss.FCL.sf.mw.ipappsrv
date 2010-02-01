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




#ifndef __UT_TMceACTIONSET_H__
#define __UT_TMceACTIONSET_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "mceclientserver.h"

//  FORWARD DECLARATIONS
class TMceActionSet;
class CMceSipSession;
class CMceServerCore;
class CMceCsSession;
class CMceSipConnection;
class CMceSipManager;
class CSIPProfile;
class CMCETls;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_TMceActionSet
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_TMceActionSet* NewL();
        static UT_TMceActionSet* NewLC();
        /**
         * Destructor
         */
        ~UT_TMceActionSet();

    private:    // Constructors and destructors

        UT_TMceActionSet();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods

         void SetupL();
        
         void Teardown();
         
         void UT_TMceActionSet_UpdateBodyLL();
         
         void UT_TMceActionSet_CreateBodyCandidateL();
         
         void UT_TMceActionSet_ProcessReliableResponseL();
         
         void UT_TMceActionSet_ProcessSessionTimerServerL_1L();
         
         void UT_TMceActionSet_ProcessSessionTimerServerL_2L();
         
         void UT_TMceActionSet_ProcessSessionTimerServerL_3L();
         
         void UT_TMceActionSet_ProcessSessionTimerClientL_1L();
         
         void UT_TMceActionSet_ProcessSessionTimerClientL_2L();
         
         void UT_TMceActionSet_ProcessSessionTimerClientL_3L();
         
         void UT_TMceActionSet_AddSessionTimerFieldsClientL_1L();
        
         void UT_TMceActionSet_ClientErrorOccuredL();
         
         void UT_TMceActionSet_SendRejectOfferWithWarningLL();
         
         void UT_TMceActionSet_CheckContactIsSecureLL();
         
         void UT_TMceActionSet_CheckContactIsSecureL2L();
         
         void UT_TMceActionSet_ProcessSessionTimerServerLL();
         
         void UT_TMceActionSet_ProcessSessionTimerServerL2L();
         
         void UT_TMceActionSet_AddSessionTimerFieldsClientLL();
         
         void UT_TMceActionSet_ClientMediaStateChangedL();
         
         void UT_TMceActionSet_ReceiveExtensionResponseL();
         
		 void UT_TMceActionSet_ReceiveExtensionRequestL();
		 
		 void UT_TMceActionSet_SendExtensionRequestL();
		 
		 void UT_TMceActionSet_SendExtensionResponseL();
		 
		 void UT_TMceActionSet_ReceiveExtensionErrorL();
		 
		 void UT_TMceActionSet_IsExtensionRequest();

    private:    // Data

		EUNIT_DECLARE_TEST_TABLE; 
		TMceIds iIds;
	    CMceServerCore* iServerCore;
	    CMceCsSession* iSession;
	    CMceSipSession* iSipSession;
	    
    private: //not Owned
    	    
    CMCETls* iStorage;
	    
		
    };

#endif      //  __UT_TMceACTIONSET_H__

// End of file
