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




#ifndef __UT_CMCESTATEESTABLISHED_H__
#define __UT_CMCESTATEESTABLISHED_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "mceclientserver.h"

//  FORWARD DECLARATIONS
class CMceSipSession;
class CMceCsSession;
class CMceServerCore;
class CMCETls;
class CMceStateEstablished;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMceStateEstablished
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMceStateEstablished* NewL();
        static UT_CMceStateEstablished* NewLC();
        /**
         * Destructor
         */
        ~UT_CMceStateEstablished();

    private:    // Constructors and destructors

        UT_CMceStateEstablished();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods

         void SetupL();
         void Teardown();        

		 void UT_CMceStateEstablished_StateEntryLL();
         void UT_CMceStateEstablished_AcceptLL();
         void UT_CMceStateEstablished_EntryLL();
         void UT_CMceStateEstablished_ExitLL();
         void UT_CMceStateEstablished_MediaErrorOccured();
         
		 void UT_CMceStateEstablished_EntrySendRequestL();
		 void UT_CMceStateEstablished_EntrySendResponseL();
 	 	 void UT_CMceStateEstablished_EntryResponseL();
		 void UT_CMceStateEstablished_EntryRequestL();
 		 void UT_CMceStateEstablished_EMceProvisionalResponse();
		 void UT_CMceStateEstablished_EntryErrorResponseL();
        

    private:    // Data

    	EUNIT_DECLARE_TEST_TABLE; 
    		
    	TMceIds iIds;
        CMceServerCore* iServerCore;
        CMceCsSession* iSession;
        CMceSipSession* iSipSession;
    
    private: //not Owned


        CMCETls* iStorage;
		CMceStateEstablished* iState;
		
    };

#endif      //  __UT_CMCESTATEESTABLISHED_H__

// End of file
