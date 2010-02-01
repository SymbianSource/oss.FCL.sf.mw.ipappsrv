/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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


#include "TestSuite.h"
#include "CNATTraversalPendingRequestTest.h"
#include "CNATBindingSTUNRefresherTest.h"
#include "CNATBindingCRLFRefresherTest.h"
#include "CNATBindingSTUNAndCRLFRefresherTest.h"
#include "CNATBindingTest.h"
#include "CNATTraversalSessionTest.h"
#include "CNATTraversalControllerTest.h"


// Exactly one exported function returning the suite of
// test functions for the test runner in the framework.
// (Always needed)
//
EXPORT_C MTest* CreateTestL ()
    {
    CTestSuite *suite = CTestSuite::NewL(_L8("NATTraversalControllerTest"));

    suite->addTestL(CNATTraversalPendingRequestTest::suiteL());
    suite->addTestL(CNATBindingSTUNRefresherTest::suiteL());
    suite->addTestL(CNATBindingCRLFRefresherTest::suiteL());
    suite->addTestL(CNATBindingSTUNAndCRLFRefresherTest::suiteL());
    suite->addTestL(CNATBindingTest::suiteL());
    suite->addTestL(CNATTraversalSessionTest::suiteL());
    suite->addTestL(CNATTraversalControllerTest::suiteL());

    return suite;
    }
