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




#ifndef __UT_CNATFWUNSAFUSECANDIDATEATTRIBUTE_H__
#define __UT_CNATFWUNSAFUSECANDIDATEATTRIBUTE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES

//  FORWARD DECLARATIONS
class CNATFWUNSAFUseCandidateAttribute;

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
NONSHARABLE_CLASS( UT_CNATFWUNSAFUseCandidateAttribute )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATFWUNSAFUseCandidateAttribute* NewL();
        static UT_CNATFWUNSAFUseCandidateAttribute* NewLC();
        /**
         * Destructor
         */
        ~UT_CNATFWUNSAFUseCandidateAttribute();

    private:    // Constructors and destructors

        UT_CNATFWUNSAFUseCandidateAttribute();
        void ConstructL();

    public:     // From observer interface

    private:    // New methods

        void SetupL();

        void Teardown();

        void TestGettersL();

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE;

        CNATFWUNSAFUseCandidateAttribute* iAttribute;

    };

#endif      //  __UT_CNATFWUNSAFUSECANDIDATEATTRIBUTE_H__

// End of file
