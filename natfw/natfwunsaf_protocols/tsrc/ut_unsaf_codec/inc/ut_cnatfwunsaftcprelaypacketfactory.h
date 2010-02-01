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




#ifndef __UT_CNATFWUNSAFRELAYFACTORY_H__
#define __UT_CNATFWUNSAFRELAYFACTORY_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS

class CNATFWUNSAFTcpRelayPacketFactory;

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
NONSHARABLE_CLASS( UT_CNATFWUNSAFTcpRelayPacketFactory )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATFWUNSAFTcpRelayPacketFactory* NewL();
        static UT_CNATFWUNSAFTcpRelayPacketFactory* NewLC();
        /**
         * Destructor
         */
        ~UT_CNATFWUNSAFTcpRelayPacketFactory();

    private:    // Constructors and destructors

        UT_CNATFWUNSAFTcpRelayPacketFactory();
        void ConstructL();

    public:     // From observer interface



    private:    // New methods

         void SetupL();

         void Teardown();

         void TestRelayLengthL();

         void TestDecodeL();

         void TestDecode2L();

         void TestDecode3L();

         void TestCreateL();

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE;

        CNATFWUNSAFTcpRelayPacketFactory* iFactory;
    };

#endif      //  __UT_CNATFWUNSAFRELAYFACTORY_H__

// End of file
