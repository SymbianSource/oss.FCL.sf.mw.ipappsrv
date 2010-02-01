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




#ifndef __UT_CMCCCODECH263_H__
#define __UT_CMCCCODECH263_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccCodecH263;

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
NONSHARABLE_CLASS( UT_CMccCodecH263 )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccCodecH263* NewL();
        static UT_CMccCodecH263* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccCodecH263();

    private:    // Constructors and destructors

        UT_CMccCodecH263();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMccCodecH263_RequireSignallingL();
        
        
         void UT_CMccCodecH263_CloneDefaultsLL();
        
        
         void UT_CMccCodecH263_CloneDetailedLL();
        
        
         void UT_CMccCodecH263_EnableVADL();
        
        
         void UT_CMccCodecH263_EnableInbandL();
        
        
         void UT_CMccCodecH263_EnableOutbandL();
        
        
         void UT_CMccCodecH263_SetBitrateL();
        
        
         void UT_CMccCodecH263_SetMaxBitrateL();
        
        
         void UT_CMccCodecH263_SetSamplingFreqL();
        
        
         void UT_CMccCodecH263_SetPTimeL();
        
        
         void UT_CMccCodecH263_SetMaxPTimeL();
        
        
         void UT_CMccCodecH263_SetSdpNameL();
        
        
         void UT_CMccCodecH263_SetPayloadTypeL();
        
        
         void UT_CMccCodecH263_SetCodecModeL();
        
        
         void UT_CMccCodecH263_GetFmtpLL();
        
        
         void UT_CMccCodecH263_SetFmtpAttrLL();
        
        
         void UT_CMccCodecH263_SetAllowedBitratesL();
         
         void UT_CMccCodecH263_SetConfigKeyL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        CMccCodecH263* iCodec;

    };

#endif      //  __UT_CMCCCODECH263_H__

// End of file
