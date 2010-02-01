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




//  EXTERNAL INCLUDES
#include <digia/eunit/CEUnitTestSuite.h>

//  CLASS HEADER
#include "UT_CMccJitterBuffer.h"
#include "UT_CMccJitterBufferImpl.h"
#include "UT_CMccCnGenerator.h"


/**
 * Test suite factory function.
 */
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC(_L("Wizard generated"));

    rootSuite->AddL( UT_CMccCnGenerator::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMccJitterBufferImpl::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMccJitterBuffer::NewLC() );
    CleanupStack::Pop();

    CleanupStack::Pop(); // rootSuite

    return rootSuite;
    }

//  END OF FILE
