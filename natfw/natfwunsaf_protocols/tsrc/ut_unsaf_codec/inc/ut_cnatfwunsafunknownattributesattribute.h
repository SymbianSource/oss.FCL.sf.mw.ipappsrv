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




#ifndef __UT_CNATFWUNSAFUNKNOWNATTRIBUTESATTRIBUTE_H__
#define __UT_CNATFWUNSAFUNKNOWNATTRIBUTESATTRIBUTE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES

//  FORWARD DECLARATIONS
class CNATFWUNSAFUnknownAttributesAttribute;

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
NONSHARABLE_CLASS( UT_CNATFWUNSAFUnknownAttributesAttribute )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATFWUNSAFUnknownAttributesAttribute* NewL();
        static UT_CNATFWUNSAFUnknownAttributesAttribute* NewLC();
        /**
         * Destructor
         */
        ~UT_CNATFWUNSAFUnknownAttributesAttribute();

    private:    // Constructors and destructors

        UT_CNATFWUNSAFUnknownAttributesAttribute();
        void ConstructL();

    public:     // From observer interface


    private:    // New methods

        void SetupL();
        void Teardown();
        void TestTypeL();
        void TestAddAndRemoveAttributesL();
        void TestEncodeAndDecodeL();

    private: // Data

        EUNIT_DECLARE_TEST_TABLE;

        CNATFWUNSAFUnknownAttributesAttribute* iAttribute;
    };

#endif // CNATFWUNSAFUNKNOWNATTRIBUTESATTRIBUTE_H

// End of File
