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
#include "ut_cnatfwsocketmediaconnwrapper.h"
#include "UT_MNATFWMediaWrapper.h"
#include "UT_MNATFWSocketMediaConnWrapper.h"
#include "ut_cnatfwsockethandler.h"
#include "ut_cnatfwsocketreceiver.h"
#include "ut_cnatfwsocketsender.h"


//  EXTERNAL INCLUDES
#include <digia/eunit/CEUnitTestSuite.h>

/**
 * Test suite factory function.
 */
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC(_L("Socket Media Connection Wrapper Unit Tests"));   
        
    rootSuite->AddL( ut_cnatfwsocketmediaconnwrapper::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_MNATFWMediaWrapper::NewLC() );
    CleanupStack::Pop(); // UT_MNATFWMediaWrapper instance
    
    rootSuite->AddL( UT_MNATFWSocketMediaConnWrapper::NewLC() );
    CleanupStack::Pop(); // UT_MNATFWSocketMediaConnWrapper instance

    rootSuite->AddL( ut_cnatfwsockethandler::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( ut_cnatfwsocketreceiver::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( ut_cnatfwsocketsender::NewLC() );
    CleanupStack::Pop();
    
    CleanupStack::Pop( rootSuite );
    
    return rootSuite;
    }


//  END OF FILE






