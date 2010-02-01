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




#ifndef __UT_CMCCCODECAMRWB_H__
#define __UT_CMCCCODECAMRWB_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccCodecAmrWb;

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
NONSHARABLE_CLASS( UT_CMccCodecAmrWb )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccCodecAmrWb* NewL();
        static UT_CMccCodecAmrWb* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccCodecAmrWb();

    private:    // Constructors and destructors

        UT_CMccCodecAmrWb();
        void ConstructL();

    public:     // From observer interface
        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMccCodecAmrWb_GetFmtpL();
        
         void UT_CMccCodecAmrWb_RequireSignallingL();
        
         void UT_CMccCodecAmrWb_CloneDefaultsLL();
        
         void UT_CMccCodecAmrWb_CloneDetailedLL();
        
         void UT_CMccCodecAmrWb_EnableVADL();
         
         void UT_CMccCodecAmrWb_GetBitrateModeSetL();
                 
         void UT_CMccCodecAmrWb_SetBitrateModeSetL();
         
         void UT_CMccCodecAmrWb_GetBitrateMaskFromModeL();

         void UT_CMccCodecAmrWb_CreateFmtpModeSetL();
         
         void UT_CMccCodecAmrWb_SetBitrateL();
         
         void UT_CMccCodecAmrWb_SetBitrateFromBitrateMaskL();
         
         void UT_CMccCodecAmrWb_SetSamplingFreqL();
         
         void UT_CMccCodecAmrWb_SetPTimeL();
         
         void UT_CMccCodecAmrWb_SetMaxPTimeL();
         
         void UT_CMccCodecAmrWb_SetSdpNameL();
         
         void UT_CMccCodecAmrWb_SetPayloadTypeL();
         
         void UT_CMccCodecAmrWb_SetCodecModeL();
         
         void UT_CMccCodecAmrWb_SetAllowedBitratesL();

         void UT_CMccCodecAmrWb_SetChannelsL();
        
         void UT_CMccCodecAmrWb_SetRedCountL();
        
         void UT_CMccCodecAmrWb_SetMaxRedL();

         void UT_CMccCodecAmrWb_GetChannelsL();        
        
         void UT_CMccCodecAmrWb_GetFmtpLL();        
        
         void UT_CMccCodecAmrWb_ParseFmtpAttrLL();
        
         void UT_CMccCodecAmrWb_CreateFmtpAttrListLL();

         void UT_CMccCodecAmrWb_CreateFmtpAttrListL_1L();
        
         void UT_CMccCodecAmrWb_SetModeChangePeriodL();        
        
         void UT_CMccCodecAmrWb_ModeChangePeriodL();        
        
         void UT_CMccCodecAmrWb_SetModeChangeNeighborL();        
        
         void UT_CMccCodecAmrWb_ModeChangeNeighborL();
         
         void UT_CMccCodecAmrWb_SetCrcL();
         
         void UT_CMccCodecAmrWb_GetCrcL();
         
         void UT_CMccCodecAmrWb_SetRobustSortingL();
         
         void UT_CMccCodecAmrWb_GetRobustSortingL();

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        CMccCodecAmrWb* iCodecAmr;

    };

#endif      //  __UT_CMCCCODECAMRWB_H__

// End of file
