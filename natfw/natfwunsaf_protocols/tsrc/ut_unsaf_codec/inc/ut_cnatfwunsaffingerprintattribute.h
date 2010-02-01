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




#ifndef __UT_CNATFWUNSAFFINGERPRINTATTRIBUTE_H__
#define __UT_CNATFWUNSAFFINGERPRINTATTRIBUTE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES

//  FORWARD DECLARATIONS
class CNATFWUNSAFFingerprintAttribute;

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
NONSHARABLE_CLASS( UT_CNATFWUNSAFFingerprintAttribute )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATFWUNSAFFingerprintAttribute* NewL();
        static UT_CNATFWUNSAFFingerprintAttribute* NewLC();
        /**
         * Destructor
         */
        ~UT_CNATFWUNSAFFingerprintAttribute();

    private:    // Constructors and destructors

        UT_CNATFWUNSAFFingerprintAttribute();
        void ConstructL();

    public:     // From observer interface

    private:    // New methods

        void SetupL();
        void Teardown();

        void TestNewL();
        void TestEncodeAndDecodeL();
        void TestCheckFingerprintL();
        void TestCheckFingerprint2L();
        void TestCheckFingerprint3L();

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE;

        CNATFWUNSAFFingerprintAttribute* iAttr;
    };

#endif // __UT_CNATFWUNSAFFINGERPRINTATTRIBUTE_H__

// End of File
