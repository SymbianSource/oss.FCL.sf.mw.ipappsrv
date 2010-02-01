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
#include "UT_CNATFWNatSettings.h"
#include "UT_CNATFWStunSettings.h"
#include "UT_CNATFWTurnSettings.h"
#include "UT_CNATFWIceSettings.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/CEUnitTestSuite.h>

/**
 * Test suite factory function.
 */
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC(_L("NATSettings Unit Tests"));

    rootSuite->AddL( UT_CNATFWNatSettings::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWStunSettings::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWTurnSettings::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CNATFWIceSettings::NewLC() );
    CleanupStack::Pop();

    CleanupStack::Pop( rootSuite );
    return rootSuite;
    }


//  END OF FILE






