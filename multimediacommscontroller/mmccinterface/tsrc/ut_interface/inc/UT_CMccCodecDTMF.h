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




#ifndef __UT_CMCCCODECDTMF_H__
#define __UT_CMCCCODECDTMF_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccCodecDTMF;

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
NONSHARABLE_CLASS( UT_CMccCodecDTMF )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccCodecDTMF* NewL();
        static UT_CMccCodecDTMF* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccCodecDTMF();

    private:    // Constructors and destructors

        UT_CMccCodecDTMF();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMccCodecDTMF_RequireSignallingL();
        
        
         void UT_CMccCodecDTMF_CloneDefaultsLL();
        
        
         void UT_CMccCodecDTMF_CloneDetailedLL();
        
        
         void UT_CMccCodecDTMF_EnableVADL();
               
        
         void UT_CMccCodecDTMF_SetBitrateL();
        
        
         void UT_CMccCodecDTMF_SetSamplingFreqL();
        
        
         void UT_CMccCodecDTMF_SetPTimeL();
        
        
         void UT_CMccCodecDTMF_SetMaxPTimeL();
        
        
         void UT_CMccCodecDTMF_SetSdpNameL();
        
        
         void UT_CMccCodecDTMF_SetPayloadTypeL();
        
        
         void UT_CMccCodecDTMF_SetCodecModeL();
        
         void UT_CMccCodecDTMF_AddAllowedBit();
         
         void UT_CMccCodecDTMF_GetAllowedToneNumbersArray();
         
         void UT_CMccCodecDTMF_FindContinuusIntegers();
         
         void UT_CMccCodecDTMF_CreateFmtpAttrListL();
         
         void UT_CMccCodecDTMF_AddToneNumbersL();
         void UT_CMccCodecDTMF_ParserToneNumberL();
         void UT_CMccCodecDTMF_ParserToneNumbersL();
         void UT_CMccCodecDTMF_ParseTokenL();
         void UT_CMccCodecDTMF_EncodeTokensL();
         void UT_CMccCodecDTMF_AddTokenL();
         void UT_CMccCodecDTMF_TokenizeL();
         void UT_CMccCodecDTMF_ParseFmtpAttrL();
         void UT_CMccCodecDTMF_GetFmtpL();

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        CMccCodecDTMF* iCodec;

    };

#endif      //  __UT_CMCCCODECDTMF_H__

// End of file
