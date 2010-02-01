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




#ifndef __UT_CNATFWUNSAFQUERYA_AAAA_H__
#define __UT_CNATFWUNSAFQUERYA_AAAA_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <in_sock.h>

//  FORWARD DECLARATIONS
class CNATFWUNSAFQueryA_AAAA;

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
NONSHARABLE_CLASS( UT_CNATFWUNSAFQueryA_AAAA )
     : public CEUnitTestSuiteClass
    {

    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATFWUNSAFQueryA_AAAA* NewL();
        static UT_CNATFWUNSAFQueryA_AAAA* NewLC();
        /**
         * Destructor
         */
        ~UT_CNATFWUNSAFQueryA_AAAA();

    private:    // Constructors and destructors

        UT_CNATFWUNSAFQueryA_AAAA();
        void ConstructL();

    private:    // New methods

        void SetupL();
        void Teardown();
        void NewLTestL ();
        void QueryResultBufTestL ();

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE;

        CNATFWUNSAFQueryA_AAAA*    iQuery;
    };

#endif      //  __UT_CNATFWUNSAFQUERYA_AAAA_H__

// End of file
