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




#ifndef __UT_CMCCCODECG711_H__
#define __UT_CMCCCODECG711_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMCCCodecG711;

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
NONSHARABLE_CLASS( UT_CMCCCodecG711 )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMCCCodecG711* NewL();
        static UT_CMCCCodecG711* NewLC();
        /**
         * Destructor
         */
        ~UT_CMCCCodecG711();

    private:    // Constructors and destructors

        UT_CMCCCodecG711();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMCCCodecG711_NewLL();
        
        
         void UT_CMCCCodecG711_CloneDefaultsLL();
        
        
         void UT_CMCCCodecG711_CloneDetailedLL();
        
        
         void UT_CMCCCodecG711_RequireSignallingL();
        
        
         void UT_CMCCCodecG711_EnableVADL();
        
        
         void UT_CMCCCodecG711_SetBitrateL();
        
        
         void UT_CMCCCodecG711_SetSamplingFreqL();
        
        
         void UT_CMCCCodecG711_SetPTimeL();
        
        
         void UT_CMCCCodecG711_SetMaxPTimeL();
        
        
         void UT_CMCCCodecG711_SetSdpNameL();
        
        
         void UT_CMCCCodecG711_SetPayloadTypeL();
        
        
         void UT_CMCCCodecG711_SetCodecModeL();
        
        
         void UT_CMCCCodecG711_SetComfortNoiseGenerationL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        CMCCCodecG711* iCodec;

    };

#endif      //  __UT_CMCCCODECG711_H__

// End of file
