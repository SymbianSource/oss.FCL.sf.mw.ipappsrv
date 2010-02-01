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




#ifndef __UT_TSTREAMENCODER_H__
#define __UT_TSTREAMENCODER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class TStreamEncoder;

#include <e32def.h>
#include <e32base.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
NONSHARABLE_CLASS( UT_TStreamEncoder )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_TStreamEncoder* NewL();
        static UT_TStreamEncoder* NewLC();
        /**
         * Destructor
         */
        ~UT_TStreamEncoder();

    private:    // Constructors and destructors

        UT_TStreamEncoder();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_TStreamEncoder_InitializeL();
        
        
         void UT_TStreamEncoder_EncodeL();
        
        
         void UT_TStreamEncoder_Encode_1L();
        
        
         void UT_TStreamEncoder_ByteIndexL();
        
        
         void UT_TStreamEncoder_BitIndexL();
        
        
         void UT_TStreamEncoder_SetByteIndexL();
        
        
         void UT_TStreamEncoder_SetBitIndexL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 

    };

#endif      //  __UT_TSTREAMENCODER_H__

// End of file
