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




#ifndef __UT_CNATFWUNSAFDESTINATIONADDRESSATTRIBUTE_H__
#define __UT_CNATFWUNSAFDESTINATIONADDRESSATTRIBUTE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <in_sock.h>
//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CNATFWUNSAFDestinationAddressAttribute;

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
NONSHARABLE_CLASS( UT_CNATFWUNSAFDestinationAddressAttribute )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATFWUNSAFDestinationAddressAttribute* NewL();
        static UT_CNATFWUNSAFDestinationAddressAttribute* NewLC();
        /**
         * Destructor
         */
        ~UT_CNATFWUNSAFDestinationAddressAttribute();

    private:    // Constructors and destructors

        UT_CNATFWUNSAFDestinationAddressAttribute();
        void ConstructL();

    public:     // From observer interface


    private:    // New methods

        void SetupL();

        void Teardown();

        void TestEncodeAndDecodeL();

        void TestTypeL();

        void TestEncodeAndDecodeIPv6L();

        void TestIPv6DecodeL();


    private:    // Data

        EUNIT_DECLARE_TEST_TABLE;

        CNATFWUNSAFDestinationAddressAttribute* iAttribute;

        TInetAddr iIPv6Address;
    };

#endif      //  __UT_CNATFWUNSAFDESTINATIONADDRESSATTRIBUTE_H__

// End of file
