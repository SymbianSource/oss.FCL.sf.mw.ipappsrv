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




#ifndef __UT_CMceState_H__
#define __UT_CMceState_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "mceclientserver.h"



//  FORWARD DECLARATIONS
class CMceSipSession;
class CMceCsSession;
class CMceServerCore;
class CMCETls;
class CMceState;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMceState
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMceState* NewL();
        static UT_CMceState* NewLC();
        /**
         * Destructor
         */
        ~UT_CMceState();

    private:    // Constructors and destructors

        UT_CMceState();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods

         void SetupL();
         void Teardown();        

		 void UT_CMceState_EntryReceiveExtensionResponseL();
		 void UT_CMceState_EntryReceiveExtensionRequestL();
		 void UT_CMceState_EntrySendExtensionResponseL();
		 void UT_CMceState_EntrySendExtensionRequestL();
		 void UT_CMceState_IsExtensionRequestEventL();

    private:    // Data

    	EUNIT_DECLARE_TEST_TABLE; 
    		
    	TMceIds iIds;
        CMceServerCore* iServerCore;
        CMceCsSession* iSession;
        CMceSipSession* iSipSession;
    
    private: //not Owned

        CMCETls* iStorage;
		CMceState* iState;
		
    };

#endif      //  __UT_CMceState_H__


// End of file
