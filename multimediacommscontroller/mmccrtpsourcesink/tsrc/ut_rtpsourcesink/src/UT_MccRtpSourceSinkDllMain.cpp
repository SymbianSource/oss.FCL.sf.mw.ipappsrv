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
#include "UT_CMccRtpDataSink.h"
#include "UT_CMccRtpDataSource.h"
#include "UT_CMccRtpInterface.h"
#include "UT_CMccTimerManager.h"
#include "UT_CMccJitterCalculator.h"
#include "UT_CMccRtpSender.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/CEUnitTestSuite.h>





/**
 * Test suite factory function.
 */
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC(_L("Wizard generated"));

    rootSuite->AddL( UT_CMccRtpDataSink::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMccRtpDataSource::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMccRtpInterface::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMccTimerManager::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMccJitterCalculator::NewLC() );
    CleanupStack::Pop();

rootSuite->AddL( UT_CMccRtpSender::NewLC() );
CleanupStack::Pop();

    CleanupStack::Pop(); // rootSuite

    return rootSuite;
    }

#ifndef EKA2
/**
 * Standard Symbian DLL entry point function.
 */
GLDEF_C TInt E32Dll(TDllReason)
    {
    return KErrNone;
    }
#endif // EKA2

//  END OF FILE
