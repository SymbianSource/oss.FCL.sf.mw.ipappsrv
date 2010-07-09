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




#ifndef __UT_CMccJitterCalculator_H__
#define __UT_CMccJitterCalculator_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "mccjittercalculator.h"
#include "mmccevents.h"

//  FORWARD DECLARATIONS

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMccJitterCalculator
     : public CEUnitTestSuiteClass, public MRtpJitterObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccJitterCalculator* NewL();
        static UT_CMccJitterCalculator* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccJitterCalculator();

    private:    // Constructors and destructors

        UT_CMccJitterCalculator();
        void ConstructL();

	public: 	// From observer interface
	
	    void SendJitterEvent( TMccRtpEventDataExtended aEvent, TInt aError );
		
	public:     // Helpers

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMccJitterCalculator_StartObservingL();
         
         void UT_CMccJitterCalculator_JitterObservingL();
         
         void UT_CMccJitterCalculator_RtpPacketReceivedL();
         
         void UT_CMccJitterCalculator_CalculateFerL();
         
         void UT_CMccJitterCalculator_CountPacketLossPercentageL();
         
         void UT_CMccJitterCalculator_ResetCountersL();
        
        
    private:    // Data
    
        CMccJitterCalculator* iCalculator;
        TMccRtpEventDataExtended iLastEvent;
        TInt iLastError;

		EUNIT_DECLARE_TEST_TABLE; 
		
    };

#endif      //  __UT_CMccJitterCalculator_H__

// End of file
