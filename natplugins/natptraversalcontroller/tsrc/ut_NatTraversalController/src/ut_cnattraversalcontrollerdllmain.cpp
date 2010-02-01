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
#include "ut_cnattraversalpendingrequest.h"
#include "ut_cnatbindingstunrefresher.h"
#include "ut_cnatbindingcrlfrefresher.h"
#include "ut_cnatbindingstunandcrlfrefresher.h"
#include "ut_cnatbinding.h"
#include "ut_cnattraversalsession.h"
#include "ut_cnattraversalcontroller.h"
#include "ut_cnattraversalconfig.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/CEUnitTestSuite.h>

#define _DEBUG_


/**
 * Test suite factory function.
 */
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    CEUnitTestSuite* rootSuite =
    	CEUnitTestSuite::NewLC( _L( "NAT Traversal Controller unit tests" ) );

	rootSuite->AddL( UT_CNATTraversalConfig::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATTraversalPendingRequest::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATBindingSTUNRefresher::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATBindingCRLFRefresher::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATBindingSTUNAndCRLFRefresher::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATBinding::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATTraversalSession::NewLC() );
    CleanupStack::Pop();

    /*rootSuite->AddL(UT_CNATTraversalController::NewLC() );
    CleanupStack::Pop();*/

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
