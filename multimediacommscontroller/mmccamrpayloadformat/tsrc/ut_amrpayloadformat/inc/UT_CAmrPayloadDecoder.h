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




#ifndef __UT_CAMRPAYLOADDECODER_H__
#define __UT_CAMRPAYLOADDECODER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CAmrPayloadDecoder;

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
NONSHARABLE_CLASS( UT_CAmrPayloadDecoder )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CAmrPayloadDecoder* NewL();
        static UT_CAmrPayloadDecoder* NewLC();
        /**
         * Destructor
         */
        ~UT_CAmrPayloadDecoder();

    private:    // Constructors and destructors

        UT_CAmrPayloadDecoder();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void SetupWbL();
        
         void Teardown();
        
         void UT_CAmrPayloadDecoder_NewLL();
        
        
         void UT_CAmrPayloadDecoder_DecodePayloadL();
        
        
         void UT_CAmrPayloadDecoder_ModeRequestL();
        
        
         void UT_CAmrPayloadDecoder_FramesL();
        
         void UT_TStreamDecoder_CompareBuffersL();
         void UT_TStreamDecoder_CalculateFramePositionL();

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 

		CAmrPayloadDecoder* iDec;
    };

#endif      //  __UT_CAMRPAYLOADDECODER_H__

// End of file
