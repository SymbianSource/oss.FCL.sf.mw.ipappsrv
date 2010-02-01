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




#ifndef __UT_CMCCCODECILBC_H__
#define __UT_CMCCCODECILBC_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMCCCodecILBC;

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
NONSHARABLE_CLASS( UT_CMCCCodecILBC )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMCCCodecILBC* NewL();
        static UT_CMCCCodecILBC* NewLC();
        /**
         * Destructor
         */
        ~UT_CMCCCodecILBC();

    private:    // Constructors and destructors

        UT_CMCCCodecILBC();
        void ConstructL();

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMCCCodecILBC_NewLL();
        
         void UT_CMCCCodecILBC_CloneDefaultsLL();
        
         void UT_CMCCCodecILBC_CloneDetailedLL();
        
         void UT_CMCCCodecILBC_RequireSignallingL();
        
         void UT_CMCCCodecILBC_EnableVADL();
        
         void UT_CMCCCodecILBC_SetBitrateL();
        
         void UT_CMCCCodecILBC_SetAllowedBitratesL();
         
         void UT_CMCCCodecILBC_SetSamplingFreqL();
        
         void UT_CMCCCodecILBC_SetSdpNameL();
        
         void UT_CMCCCodecILBC_SetPayloadTypeL();
        
         void UT_CMCCCodecILBC_SetCodecModeL();
        
         void UT_CMCCCodecILBC_CreateFmtpAttrListLL();
        
         void UT_CMCCCodecILBC_ParseFmtpAttrLL();
        
         void UT_CMCCCodecILBC_GetFmtpLL();
        
         void UT_CMCCCodecILBC_SetMaxPTimeL();
        
         void UT_CMCCCodecILBC_SetPTimeL();
        
         void UT_CMCCCodecILBC_SetComfortNoiseGenerationL();
        
    private:    // Data

        EUNIT_DECLARE_TEST_TABLE;
        
        CMCCCodecILBC* iCodec; 

    };

#endif      //  __UT_CMCCCODECILBC_H__

// End of file
