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




//  CLASS HEADER
#include "ut_cnatfwunsafquerya_aaaa.h"
#include "ut_cnatfwunsafquerysrv.h"
#include "ut_cnatfwunsafquerydata.h"
#include "ut_cnatfwunsafa_aaaaorigdomain.h"
#include "ut_cnatfwunsafserverresolver.h"
#include "ut_cnatfwunsafrequestqueue.h"
#include "ut_cnatfwunsafhostresolver.h"
#include "ut_cnatfwunsafserverquery.h"
#include "ut_cnatfwunsafsrvorigdomain.h"
#include "ut_cnatfwunsafa_aaaasrvdomain.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/CEUnitTestSuite.h>

#define _DEBUG_

/**
 * Test suite factory function.
 */
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC(_L(
        "NATFW UNSAF Server Resolver unit tests"));

    rootSuite->AddL( UT_CNATFWUNSAFQueryA_AAAA::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFQuerySrv::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFQueryData::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFA_AAAAOrigDomain::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFServerResolver::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFRequestQueue::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFHostResolver::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFServerQuery::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFSrvOrigDomain::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFA_AAAASRVDomain::NewLC() );
    CleanupStack::Pop();

    CleanupStack::Pop( rootSuite );
    return rootSuite;
    }

#ifndef __SECURE_API__
/**
 * Standard Symbian DLL entry point function.
 */
GLDEF_C TInt E32Dll(TDllReason)
    {
    return KErrNone;
    }
#endif

//  END OF FILE
