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




#ifndef __UT_CMccSUBTHREADSHUTDOWNTIMER_H__
#define __UT_CMccSUBTHREADSHUTDOWNTIMER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccSubThreadShutdownTimer;

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
NONSHARABLE_CLASS( UT_CMccSubThreadShutdownTimer )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccSubThreadShutdownTimer* NewL();
        static UT_CMccSubThreadShutdownTimer* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccSubThreadShutdownTimer();

    private:    // Constructors and destructors

        UT_CMccSubThreadShutdownTimer();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMccSubThreadShutdownTimer_StartLL();
        
        
         void UT_CMccSubThreadShutdownTimer_ShutdownNowL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 

        CMccSubThreadShutdownTimer* iTimer;
    };

#endif      //  __UT_CMccSUBTHREADSHUTDOWNTIMER_H__

// End of file
