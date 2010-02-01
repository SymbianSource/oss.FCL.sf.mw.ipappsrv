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




#ifndef __UT_CNATFWUNSAFMESSAGEFACTORY_H__
#define __UT_CNATFWUNSAFMESSAGEFACTORY_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "natfwunsafmessage.h"

//  FORWARD DECLARATIONS
class CNATFWUNSAFMessageFactory;

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
NONSHARABLE_CLASS( UT_CNATFWUNSAFMessageFactory )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATFWUNSAFMessageFactory* NewL();
        static UT_CNATFWUNSAFMessageFactory* NewLC();
        /**
         * Destructor
         */
        ~UT_CNATFWUNSAFMessageFactory();

    private:    // Constructors and destructors

        UT_CNATFWUNSAFMessageFactory();
        void ConstructL();

    public:     // From observer interface

    private:    // New methods

        void SetupL();
        void Teardown();
        void TestMessageLengthL();
        void TestDecodeL();
        void TestDecode2L();
        void TestDecode3L();
        void TestDecode4L();
        void TestDecode5L();
        void TestDecodeWrongPrefixL();
        void TestDecodeNoCookieL();
        void TestDecodeIPv6AddressesL();
        void TestCreateL();

    private: // Helper functions

        CNATFWUNSAFMessage* CreateAndVerifyMsgLC(
            CNATFWUNSAFMessage::TType aType);

    private: // Data

        EUNIT_DECLARE_TEST_TABLE;

        CNATFWUNSAFMessageFactory* iFactory;
    };

#endif // UT_CNATFWUNSAFMESSAGEFACTORY_H

// End of File
