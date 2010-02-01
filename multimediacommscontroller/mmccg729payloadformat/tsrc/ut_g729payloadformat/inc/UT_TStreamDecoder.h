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




#ifndef __UT_TSTREAMDECODER_H__
#define __UT_TSTREAMDECODER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class TStreamDecoder;

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
NONSHARABLE_CLASS( UT_TStreamDecoder )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_TStreamDecoder* NewL();
        static UT_TStreamDecoder* NewLC();
        /**
         * Destructor
         */
        ~UT_TStreamDecoder();

    private:    // Constructors and destructors

        UT_TStreamDecoder();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_TStreamDecoder_InitializeL();
        
        
         void UT_TStreamDecoder_DecodeL();
        
        
         void UT_TStreamDecoder_Decode_1L();
        
        
         void UT_TStreamDecoder_ByteIndexL();
        
        
         void UT_TStreamDecoder_BitIndexL();
        
        
         void UT_TStreamDecoder_SetByteIndexL();
        
        
         void UT_TStreamDecoder_SetBitIndexL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 

    };

#endif      //  __UT_TSTREAMDECODER_H__

// End of file
