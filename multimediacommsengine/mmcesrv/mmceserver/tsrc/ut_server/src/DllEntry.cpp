/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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

#include <cppunit/TestSuite.h>
#include "CMceTimerManagerTest.h"

// Bullseye reguires following entry point
#ifdef _BullseyeCoverage
GLDEF_C int __stdcall __E32Dll(void*, unsigned, void*)
	{
	return 0;
	}
#else
#ifndef EKA2
// Starting point of the DLL (Removed in EKA2) 
GLDEF_C TInt E32Dll(TDllReason)
	{
	return(KErrNone);
	}
#endif
#endif _BullseyeCoverage


// Exactly one exported function returning the suite of 
// test functions for the test runner in the framework.
// (Always needed)
//
EXPORT_C MTest* CreateTestL ()
	{
    CTestSuite *suite = CTestSuite::NewL(_L8("MceTimerManagerTest"));

    suite->addTestL(CMceTimerManagerTest::suiteL());
/*
	suite->addTestL(CMceCsReceiveQueueTest::suiteL());
    suite->addTestL(CMceCsReceiverBaseTest::suiteL());
    suite->addTestL(CMceCsSessionReceiverTest::suiteL());
    suite->addTestL(CMceCsSubSessionReceiverTest::suiteL());
    suite->addTestL(CMceCsSessionTest::suiteL());
    suite->addTestL(CMceCsSubSessionTest::suiteL());
*/
    return suite;
	}
