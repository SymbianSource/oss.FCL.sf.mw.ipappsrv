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




#ifndef __UT_CMCCCODECAVC_H__
#define __UT_CMCCCODECAVC_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccCodecAVC;

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
NONSHARABLE_CLASS( UT_CMccCodecAVC )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccCodecAVC* NewL();
        static UT_CMccCodecAVC* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccCodecAVC();

    private:    // Constructors and destructors

        UT_CMccCodecAVC();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMccCodecAVC_RequireSignallingL();
        
        
         void UT_CMccCodecAVC_CloneDefaultsLL();
        
        
         void UT_CMccCodecAVC_CloneDetailedLL();
        
        
         void UT_CMccCodecAVC_EnableVADL();
              
        
         void UT_CMccCodecAVC_SetBitrateL();
        
        
         void UT_CMccCodecAVC_SetSamplingFreqL();
        
        
         void UT_CMccCodecAVC_SetPTimeL();
        
        
         void UT_CMccCodecAVC_SetMaxPTimeL();
        
        
         void UT_CMccCodecAVC_SetSdpNameL();
        
        
         void UT_CMccCodecAVC_SetPayloadTypeL();
        
        
         void UT_CMccCodecAVC_SetCodecModeL();
         
         void UT_CMccCodecAVC_SetAllowedBitratesL();
         
         void UT_CMccCodecAVC_SetGetConfigKeyL();
         
         void UT_CMccCodecAVC_GetFmtpL();
        
         void UT_CMccCodecAVC_ParseFmtpL();
         
         void UT_CMccCodecAVC_SetPreferredEncodingDecodingDeviceL();

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        CMccCodecAVC* iCodec;
    };

#endif      //  __UT_CMCCCODECAVC_H__

// End of file
