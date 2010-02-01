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




#ifndef __UT_CMccCNGENERATOR_H__
#define __UT_CMccCNGENERATOR_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <mmf/common/mmfutilities.h>

//  FORWARD DECLARATIONS
class CMccCnGenerator;


//  CLASS DEFINITION

/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMccCnGenerator : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccCnGenerator* NewL();
        
        static UT_CMccCnGenerator* NewLC();
        
        /**
         * Destructor
         */
        ~UT_CMccCnGenerator();
        

    private:    // Constructors and destructors

        UT_CMccCnGenerator();
        
        void ConstructL();
        

    private:    // New methods

        void SetupL();

        void ReSetupL( TUint32 aFourCC );
        
        void Teardown();
        
        void UT_CMccCnGenerator_GenerateSidPacketL();


        void UT_CMccCnGenerator_ConcealErrorForNextFrameL();
        
        
        void UT_CMccCnGenerator_DtxPeriodStatusL();
        
        
        void UT_CMccCnGenerator_DoDtxDecisionL();
        
        
        void UT_CMccCnGenerator_IsSidBufferL();
        

    private:    // Data

		EUNIT_DECLARE_TEST_TABLE; 
		TFourCC iFourCC;
        CMccCnGenerator* iCnGenerator;
    };

#endif      //  __UT_CMccCNGENERATOR_H__

// End of file
