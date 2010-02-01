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
#include "ut_cncmconnectionmultiplexer.h"
#include "ut_cncmconnection.h"
#include "ut_cncmlocaladdressresolver.h"
#include "ut_cncmicmpreceiver.h"
#include "ut_cncmmediasource.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/CEUnitTestSuite.h>

#define _DEBUG_


/**
 * Test suite factory function.
 */
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC(_L("Connection multiplexer Unit Tests"));

    rootSuite->AddL( UT_CNcmConnectionMultiplexer::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNcmConnection::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNcmLocalAddressResolver::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNcmIcmpReceiver::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CNcmMediaSource::NewLC() );
    CleanupStack::Pop();

    CleanupStack::Pop( rootSuite );    
    return rootSuite;
    }


//  END OF FILE






