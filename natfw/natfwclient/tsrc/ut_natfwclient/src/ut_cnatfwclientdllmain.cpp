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
#include "UT_CNATFWClient.h"
#include "ut_cnatfwcandidate.h"
#include "ut_cnatfwcandidatepair.h"
#include "ut_cnatfwidentification.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/CEUnitTestSuite.h>

/**
 * Test suite factory function.
 */
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC( _L("NATFW Client Unit Tests" ));
    
    rootSuite->AddL( UT_CNATFWClient::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWCandidate::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CNATFWCandidatePair::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWIdentification::NewLC() );
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






