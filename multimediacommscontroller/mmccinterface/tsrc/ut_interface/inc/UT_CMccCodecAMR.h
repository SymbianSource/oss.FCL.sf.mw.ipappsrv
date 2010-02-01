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




#ifndef __UT_CMCCCODECAMR_H__
#define __UT_CMCCCODECAMR_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccCodecAMR;

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
NONSHARABLE_CLASS( UT_CMccCodecAMR )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccCodecAMR* NewL();
        static UT_CMccCodecAMR* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccCodecAMR();

    private:    // Constructors and destructors

        UT_CMccCodecAMR();
        void ConstructL();

    public:     // From observer interface
        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMccCodecAMR_GetFmtpL();
        
         void UT_CMccCodecAMR_RequireSignallingL();
        
         void UT_CMccCodecAMR_CloneDefaultsLL();
        
         void UT_CMccCodecAMR_CloneDetailedLL();
        
         void UT_CMccCodecAMR_EnableVADL();
         
         void UT_CMccCodecAMR_GetBitrateModeSetL();
                 
         void UT_CMccCodecAMR_SetBitrateModeSetL();
         
         void UT_CMccCodecAMR_GetBitrateMaskFromModeL();

         void UT_CMccCodecAMR_CreateFmtpModeSetL();
         
         void UT_CMccCodecAMR_SetBitrateL();
         
         void UT_CMccCodecAMR_SetBitrateFromBitrateMaskL();
         
         void UT_CMccCodecAMR_SetSamplingFreqL();
         
         void UT_CMccCodecAMR_SetPTimeL();
         
         void UT_CMccCodecAMR_SetMaxPTimeL();
         
         void UT_CMccCodecAMR_SetSdpNameL();
         
         void UT_CMccCodecAMR_SetPayloadTypeL();
         
         void UT_CMccCodecAMR_SetCodecModeL();
         
         void UT_CMccCodecAMR_SetAllowedBitratesL();

         void UT_CMccCodecAMR_SetChannelsL();
        
         void UT_CMccCodecAMR_SetRedCountL();
        
         void UT_CMccCodecAMR_SetMaxRedL();

         void UT_CMccCodecAMR_GetChannelsL();        
        
         void UT_CMccCodecAMR_GetFmtpLL();        
        
         void UT_CMccCodecAMR_ParseFmtpAttrLL();
        
         void UT_CMccCodecAMR_CreateFmtpAttrListLL();

         void UT_CMccCodecAMR_CreateFmtpAttrListL_1L();
        
         void UT_CMccCodecAMR_SetModeChangePeriodL();        
        
         void UT_CMccCodecAMR_ModeChangePeriodL();        
        
         void UT_CMccCodecAMR_SetModeChangeNeighborL();        
        
         void UT_CMccCodecAMR_ModeChangeNeighborL();
         
         void UT_CMccCodecAMR_SetCrcL();
         
         void UT_CMccCodecAMR_GetCrcL();
         
         void UT_CMccCodecAMR_SetRobustSortingL();
         
         void UT_CMccCodecAMR_GetRobustSortingL();
         

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        CMccCodecAMR* iCodecAmr;

    };

#endif      //  __UT_CMCCCODECAMR_H__

// End of file
