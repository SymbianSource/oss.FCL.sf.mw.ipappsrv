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




#ifndef __UT_CNATFWUNSAFQUERYSRV_H__
#define __UT_CNATFWUNSAFQUERYSRV_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <in_sock.h>

//  FORWARD DECLARATIONS
class CNATFWUNSAFQuerySrv;

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
NONSHARABLE_CLASS( UT_CNATFWUNSAFQuerySrv )
     : public CEUnitTestSuiteClass
    {

    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATFWUNSAFQuerySrv* NewL();
        static UT_CNATFWUNSAFQuerySrv* NewLC();
        /**
         * Destructor
         */
        ~UT_CNATFWUNSAFQuerySrv();

    private:    // Constructors and destructors

        UT_CNATFWUNSAFQuerySrv();
        void ConstructL();

    private:    // New methods

        void SetupL();
        void Teardown();
        void QuerySrvNewLTestL ();
        void QueryResultBufTestL ();
        void SetTargetProtocolLTestL ();
        void AddLTestL ();

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE;

        CNATFWUNSAFQuerySrv* iQuery;

        };

#endif      //  __UT_CNATFWUNSAQUERYSRV_H__

// End of file

