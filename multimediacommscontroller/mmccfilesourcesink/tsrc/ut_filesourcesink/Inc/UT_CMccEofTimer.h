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




#ifndef __UT_CMCCEOFTIMER_H__
#define __UT_CMCCEOFTIMER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "mcceoftimer.h"

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

      
//  CLASS DEFINITION

NONSHARABLE_CLASS( UT_CMccEofTimer )
     : public CEUnitTestSuiteClass, public MMccEofTimerObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccEofTimer* NewL();
        static UT_CMccEofTimer* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccEofTimer();
        
    public:	// from MCCEorTimerObserver interface
    	virtual void EofTimerExpiredL();

    private:    // Constructors and destructors

        UT_CMccEofTimer();
        void ConstructL();

    private:    // New methods

        void SetupL();
        
        void Teardown();
        
        void UT_CMccEofTimer_NewLL();
        
    private: // test methods
    	void UT_CMccEofTimer_RunLL();

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        CMccEofTimer* iTimer;
        TBool iEofTimerExpiredCalled;
    };

#endif      //  __UT_CMCCEOFTIMER_H__

// End of file
