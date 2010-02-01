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




#ifndef __UT_CNATFWUNSAFMESSAGEINTEGRITYATTRIBUTE_H__
#define __UT_CNATFWUNSAFMESSAGEINTEGRITYATTRIBUTE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES

//  FORWARD DECLARATIONS
class CNATFWUNSAFMessageIntegrityAttribute;

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
NONSHARABLE_CLASS( UT_CNATFWUNSAFMessageIntegrityAttribute )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATFWUNSAFMessageIntegrityAttribute* NewL();
        static UT_CNATFWUNSAFMessageIntegrityAttribute* NewLC();
        /**
         * Destructor
         */
        ~UT_CNATFWUNSAFMessageIntegrityAttribute();

    private:    // Constructors and destructors

        UT_CNATFWUNSAFMessageIntegrityAttribute();
        void ConstructL();

    public:     // From observer interface

    private:    // New methods

        void SetupL();

        void Teardown();

        void TestShortTermNewL();
        void TestShortTermDecodeL();
        void TestShortTermEncodeAndDecodeL();
        void TestShortTermCheckMessageIntegrityL();
        void TestShortTermCheckMessageIntegrity2L();
        void TestShortTermCheckMessageIntegrity3L();
        void TestShortTermCheckMessageIntegrity4L();
        void TestShortTermCheckMessageIntegrity5L();

        void TestLongTermNewL();
        void TestLongTermDecodeL();
        void TestLongTermEncodeAndDecodeL();
        void TestLongTermCheckMessageIntegrityL();
        void TestLongTermCheckMessageIntegrity2L();
        void TestLongTermCheckMessageIntegrity3L();
        void TestLongTermCheckMessageIntegrity4L();
        void TestLongTermCheckMessageIntegrity5L();

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE;

        CNATFWUNSAFMessageIntegrityAttribute* iAttr;
    };

#endif // __UT_CNATFWUNSAFMESSAGEINTEGRITYATTRIBUTE_H__

// End of File
