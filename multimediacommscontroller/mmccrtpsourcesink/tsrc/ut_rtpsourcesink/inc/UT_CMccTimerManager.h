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




#ifndef UT_CMCCTIMERMANAGER_H
#define UT_CMCCTIMERMANAGER_H

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

#include "mccexpirationhandler.h"


//  FORWARD DECLARATIONS
class CMccTimerManager;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMccTimerManager
     : public CEUnitTestSuiteClass, public MMccExpirationHandler
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccTimerManager* NewL();
        static UT_CMccTimerManager* NewLC();

        /**
         * Destructor
         */
        ~UT_CMccTimerManager();

    private:    // Constructors and destructors

        UT_CMccTimerManager();
        void ConstructL();
        
    public: // From MMccExpirationHandler
    
        void TimerExpiredL( TMccTimerId aTimerId, TAny* aTimerParam );
   
    private:    // New methods

         void SetupL();
        
         void Teardown();        
        
         void TestIsRunningL();
         void TestIsRunning2L();
         void TestIsRunning3L();
         void TestStartL();
         void TestStart2L();
         void TestStart3L();
         void TestStart4L();
         void TestStart5L();
         void TestStart6L();
         void TestStart7L();
         void TestStart8L();
         void TestStart9L();
         void TestStopL();
         void TestStop2L();
         void TestStop3L();
         void TestNewTimerIdL();
         void TestTimerExpiredL();
         void TestTimerExpired2L();
         void TestTimerExpired3L();
    private:    // Data

        CMccTimerManager* iTimer;
        
        TMccTimerId iLastExpiredTimerId;
        TAny* iLastTimerParam;

		EUNIT_DECLARE_TEST_TABLE; 
		
    };

#endif      //  UT_CMCCTIMERMANAGER_H

// End of file
