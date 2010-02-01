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




#ifndef __UT_CMccPERIODICRUNNER_H__
#define __UT_CMccPERIODICRUNNER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccPeriodicRunner;

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif
/*
static enum TTstCallbackType
            {
            ETstCompleted,
            ETstCancelled,
            ETstRunError,
            ETstNone
            };
  */          
//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
NONSHARABLE_CLASS( UT_CMccPeriodicRunner )
     : public CEUnitTestSuiteClass
    {
    
    public:     // Type definitions

     
            
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccPeriodicRunner* NewL();
        static UT_CMccPeriodicRunner* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccPeriodicRunner();

    private:    // Constructors and destructors

        UT_CMccPeriodicRunner();
        void ConstructL();

    public:     // From observer interface
   
		static TInt RequestCompletedL( TAny* aObject );

    private:    // New methods

        void SetupL();
        
        void Teardown();
        
        void UT_CMccPeriodicRunner_NewLL();
        
    private: // test methods
    	void UT_CMccPeriodicRunner_RunL();
    	void UT_CMccPeriodicRunner_CancelL();
    	void UT_CMccPeriodicRunner_ErrorL();

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        CMccPeriodicRunner* iTimerRunner;
        
    };

#endif      //  __UT_CMccPERIODICRUNNER_H__

// End of file
