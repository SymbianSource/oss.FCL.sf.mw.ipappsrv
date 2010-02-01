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
#include "ut_cnatfwunsafudpsender.h"
#include "ut_cnatfwunsaftlssender.h"
#include "ut_cnatfwunsaftlsreceiver.h"
#include "ut_cnatfwunsaftlstransport.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/CEUnitTestSuite.h>

#define _DEBUG_

/**
 * Test suite factory function.
 */
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC(_L("NATFW UNSAF Transport unit tests"));

    rootSuite->AddL( UT_CNATFWUNSAFUdpSender::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFTlsSender::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFTlsReceiver::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWUNSAFTlsTransport::NewLC() );
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
