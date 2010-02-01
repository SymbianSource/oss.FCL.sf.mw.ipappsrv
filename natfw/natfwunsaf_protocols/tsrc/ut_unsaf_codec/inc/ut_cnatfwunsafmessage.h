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




#ifndef __UT_CNATFWUNSAFMESSAGE_H__
#define __UT_CNATFWUNSAFMESSAGE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "natfwunsaftransactionid.h"

//  FORWARD DECLARATIONS
class CNATFWUNSAFMessage;
class CNATFWUNSAFMappedAddressAttribute;

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
NONSHARABLE_CLASS( UT_CNATFWUNSAFMessage )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATFWUNSAFMessage* NewL();
        static UT_CNATFWUNSAFMessage* NewLC();
        /**
         * Destructor
         */
        ~UT_CNATFWUNSAFMessage();

    private:    // Constructors and destructors

        UT_CNATFWUNSAFMessage();
        void ConstructL();

    public:     // From observer interface

    private:    // New methods

        void SetupL();
        void Teardown();
        void TestTransactionIDL();
        void TestTransactionID2L();
        void TestTypeL();
        void TestAttributeL();
        void TestAttribute2L();
        void TestAttribute3L();
        void TestAttribute4L();
        void TestAddAttributeL();
        void TestDetachAttributeL();
        void TestDetachAttribute2L();
        void TestDeleteAttributeL();
        void TestDeleteAttribute2L();
        void TestDeleteAttribute3L();
        void TestDeleteAttribute4L();
        void TestEncodeL();
        void TestEncode2L();
        void TestEncode3L();
        void TestEncode4L();
        void TestEncodeWithHashL();
        void TestEncodeMessageWithXorMappedAddrAttrL();
        void TestSetMessageLengthL();
        void TestSetMessageLength2L();
        void TestSetMessageLength3L();
        void TestUnknownMandatoryAttributesL();

    private:    // Helper functions

        void CompareEncodedUNSAFMessageL(
            const TUint8* aExpectedResult,
            CBufBase& aUNSAFMessage,
            TUint aMaxBytesToCompare = 2000);

        TBool CompareEncodedUNSAFMessage(
            const TUint8* aExpectedResult,
             CBufBase& aUNSAFMessage,
            TUint aMaxBytesToCompare,
            TUint& aFirstDifference,
            TUint8& aExpected,
            TUint8& aActual);

        void WriteToCppUnitHtmlFile(
            const TPtrC8& aString, TInt aIntValue);

    private: // Data

        EUNIT_DECLARE_TEST_TABLE;

        CNATFWUNSAFMessage* iMsg;
        TNATFWUNSAFTransactionID iTaID;
        CNATFWUNSAFMappedAddressAttribute* iAttr;
    };

#endif // CNATFWUNSAFMESSAGETEST_H

// End of File
