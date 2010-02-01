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




#ifndef __UT_CDTMFPAYLOADDECODER_H__
#define __UT_CDTMFPAYLOADDECODER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CDTMFPayloadDecoder;

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
NONSHARABLE_CLASS( UT_CDTMFPayloadDecoder )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CDTMFPayloadDecoder* NewL();
        static UT_CDTMFPayloadDecoder* NewLC();
        /**
         * Destructor
         */
        ~UT_CDTMFPayloadDecoder();

    private:    // Constructors and destructors

        UT_CDTMFPayloadDecoder();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CDTMFPayloadDecoder_ConvertToChar();
        
         void UT_CDTMFPayloadDecoder_DecodeEventPayloadL();
        
        
         void UT_CDTMFPayloadDecoder_DecodeTonePayloadL();
        
        
         void UT_CDTMFPayloadDecoder_PayloadFormatL();
        
        
         void UT_CDTMFPayloadDecoder_SetPayloadFormatL();
        
        
         void UT_CDTMFPayloadDecoder_SetCurTimeStampL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        CDTMFPayloadDecoder* iDecoder;

    };

#endif      //  __UT_CDTMFPAYLOADDECODER_H__

// End of file
