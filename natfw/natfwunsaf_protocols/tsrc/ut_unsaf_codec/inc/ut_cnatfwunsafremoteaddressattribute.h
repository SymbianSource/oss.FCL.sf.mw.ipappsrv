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




#ifndef __UT_CNATFWUNSAFREMOTEADDRESSATTRIBUTE_H__
#define __UT_CNATFWUNSAFREMOTEADDRESSATTRIBUTE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <in_sock.h>
//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CNATFWUNSAFRemoteAddressAttribute;

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
NONSHARABLE_CLASS( UT_CNATFWUNSAFRemoteAddressAttribute )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATFWUNSAFRemoteAddressAttribute* NewL();
        static UT_CNATFWUNSAFRemoteAddressAttribute* NewLC();
        /**
         * Destructor
         */
        ~UT_CNATFWUNSAFRemoteAddressAttribute();

    private:    // Constructors and destructors

        UT_CNATFWUNSAFRemoteAddressAttribute();
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

        CNATFWUNSAFRemoteAddressAttribute* iAttribute;

        TInetAddr iIPv6Address;
    };

#endif      //  __UT_CNATFWUNSAFREMOTEADDRESSATTRIBUTE_H__

// End of file
