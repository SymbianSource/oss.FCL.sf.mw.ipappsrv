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
#include "UT_CSTUNRelayBinding.h"
#include "ut_cstunbinding.h"
#include "ut_cstunclient.h"
#include "ut_cstunbindingimplementation.h"
#include "ut_cstunrelaybindingimplementation.h"
#include "ut_cstunsharedsecret.h"
#include "ut_cstuntransaction.h"
#include "ut_cstunclientimplementation.h"
#include "ut_cstunclientresolvingtcp.h"
#include "ut_cstunbindinggetaddress.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/CEUnitTestSuite.h>

/**
 * Test suite factory function.
 */
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC(_L("Wizard generated"));

    rootSuite->AddL( UT_CSTUNRelayBinding::NewLC() );
    CleanupStack::Pop();

    CleanupStack::Pop(); // rootSuite
    
    return rootSuite;
    }



//  END OF FILE






