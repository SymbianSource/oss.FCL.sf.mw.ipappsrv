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




#ifndef __UT_CMCCCODECAAC_H__
#define __UT_CMCCCODECAAC_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccCodecAAC;

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
NONSHARABLE_CLASS( UT_CMccCodecAAC )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccCodecAAC* NewL();
        static UT_CMccCodecAAC* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccCodecAAC();

    private:    // Constructors and destructors

        UT_CMccCodecAAC();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMccCodecAAC_RequireSignallingL();
        
        
         void UT_CMccCodecAAC_CloneDefaultsLL();
        
        
         void UT_CMccCodecAAC_CloneDetailedLL();
        
        
         void UT_CMccCodecAAC_EnableVADL();
        
        
         void UT_CMccCodecAAC_SetBitrateL();
        
        
         void UT_CMccCodecAAC_SetSamplingFreqL();
        
        
         void UT_CMccCodecAAC_SetPTimeL();
        
        
         void UT_CMccCodecAAC_SetMaxPTimeL();
        
        
         void UT_CMccCodecAAC_SetSdpNameL();
        
        
         void UT_CMccCodecAAC_SetPayloadTypeL();
        
        
         void UT_CMccCodecAAC_SetCodecModeL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        CMccCodecAAC* iCodec;
    };

#endif      //  __UT_CMCCCODECAAC_H__

// End of file
