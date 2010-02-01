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




#ifndef __UT_CMCCCODECCN_H__
#define __UT_CMCCCODECCN_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccCodecCn;

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
NONSHARABLE_CLASS( UT_CMccCodecCn )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccCodecCn* NewL();
        static UT_CMccCodecCn* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccCodecCn();

    private:    // Constructors and destructors

        UT_CMccCodecCn();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMccCodecCn_NewLL();
        
        
         void UT_CMccCodecCn_CloneDefaultsLL();
        
        
         void UT_CMccCodecCn_CloneDetailedLL();
        
        
         void UT_CMccCodecCn_RequireSignallingL();
        
        
         void UT_CMccCodecCn_EnableVADL();
        
        
         void UT_CMccCodecCn_SetBitrateL();
        
        
         void UT_CMccCodecCn_SetSamplingFreqL();
        
        
         void UT_CMccCodecCn_SetPTimeL();
        
        
         void UT_CMccCodecCn_SetMaxPTimeL();
        
        
         void UT_CMccCodecCn_SetSdpNameL();
        
        
         void UT_CMccCodecCn_SetPayloadTypeL();
        
        
         void UT_CMccCodecCn_SetCodecModeL();
        
        
         void UT_CMccCodecCn_SetComfortNoiseGenerationL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        CMccCodecCn* iCodec;

    };

#endif      //  __UT_CMCCCODECCN_H__

// End of file
