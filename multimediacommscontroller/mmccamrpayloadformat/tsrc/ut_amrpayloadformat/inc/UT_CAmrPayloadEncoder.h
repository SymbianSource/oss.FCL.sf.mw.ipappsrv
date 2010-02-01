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




#ifndef __UT_CAMRPAYLOADENCODER_H__
#define __UT_CAMRPAYLOADENCODER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CAmrPayloadEncoder;

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
NONSHARABLE_CLASS( UT_CAmrPayloadEncoder )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CAmrPayloadEncoder* NewL();
        static UT_CAmrPayloadEncoder* NewLC();
        /**
         * Destructor
         */
        ~UT_CAmrPayloadEncoder();

    private:    // Constructors and destructors

        UT_CAmrPayloadEncoder();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void SetupWbL();
        
         void Teardown();
        
         void UT_CAmrPayloadEncoder_NewLL();
        
        
         void UT_CAmrPayloadEncoder_SetModeRequestL();
        
        
         void UT_CAmrPayloadEncoder_EncodeFrameL();
        
        
         void UT_CAmrPayloadEncoder_ResetPayloadBufferL();
        
        
         void UT_CAmrPayloadEncoder_ReEncodeFrameLL();
        
        
         void UT_CAmrPayloadEncoder_ReEncodeFrameL_1L();
        
        
         void UT_CAmrPayloadEncoder_SetChannelCountL();
        
        
         void UT_CAmrPayloadEncoder_SetFrameBlockCountL();
        
        
         void UT_CAmrPayloadEncoder_SetRedFrameBlockCountL();
        
         void UT_TStreamEncoder_SaveRedundantFrameL();

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
		CAmrPayloadEncoder* iEnc;

    };

#endif      //  __UT_CAMRPAYLOADENCODER_H__

// End of file
