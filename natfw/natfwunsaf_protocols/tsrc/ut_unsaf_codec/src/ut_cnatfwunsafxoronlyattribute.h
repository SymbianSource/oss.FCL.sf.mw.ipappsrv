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




#ifndef __UT_CNATFWUNSAFXORONLYATTRIBUTE_H__
#define __UT_CNATFWUNSAFXORONLYATTRIBUTE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES

//  FORWARD DECLARATIONS
class CNATFWUNSAFXorOnlyAttribute;

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
NONSHARABLE_CLASS( UT_CNATFWUNSAFXorOnlyAttribute )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATFWUNSAFXorOnlyAttribute* NewL();
        static UT_CNATFWUNSAFXorOnlyAttribute* NewLC();
        /**
         * Destructor
         */
        ~UT_CNATFWUNSAFXorOnlyAttribute();

    private:    // Constructors and destructors

        UT_CNATFWUNSAFXorOnlyAttribute();
        void ConstructL();

    public:     // From observer interface

    private:    // New methods

        void SetupL();
        void Teardown();
        void TestEncodeAndDecodeL();
        void TestTypeL();

private: // Data

    EUNIT_DECLARE_TEST_TABLE;

    CNATFWUNSAFXorOnlyAttribute* iAttribute;

    };

#endif // UT_CNATFWUNSAFXORONLYATTRIBUTE_H

// End of File
