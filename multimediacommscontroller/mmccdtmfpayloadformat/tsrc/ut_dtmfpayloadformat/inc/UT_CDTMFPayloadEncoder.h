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




#ifndef __UT_CDTMFPAYLOADENCODER_H__
#define __UT_CDTMFPAYLOADENCODER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CDTMFPayloadEncoder;

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
NONSHARABLE_CLASS( UT_CDTMFPayloadEncoder )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CDTMFPayloadEncoder* NewL();
        static UT_CDTMFPayloadEncoder* NewLC();
        /**
         * Destructor
         */
        ~UT_CDTMFPayloadEncoder();

    private:    // Constructors and destructors

        UT_CDTMFPayloadEncoder();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CDTMFPayloadEncoder_EventAsInteger();
         
         void UT_CDTMFPayloadEncoder_EncodeEventL();
        
        
         void UT_CDTMFPayloadEncoder_EncodeToneL();
        
        
         void UT_CDTMFPayloadEncoder_UpdateEventPayloadL();
        
        
         void UT_CDTMFPayloadEncoder_UpdateTonePayloadL();
        
        
         void UT_CDTMFPayloadEncoder_PayloadFormatL();
        
        
         void UT_CDTMFPayloadEncoder_SetPayloadFormatL();
        
        
         void UT_CDTMFPayloadEncoder_PayloadTypeL();
        
        
         void UT_CDTMFPayloadEncoder_SetPayloadTypeL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        CDTMFPayloadEncoder* iEncoder;

    };

#endif      //  __UT_CDTMFPAYLOADENCODER_H__

// End of file
