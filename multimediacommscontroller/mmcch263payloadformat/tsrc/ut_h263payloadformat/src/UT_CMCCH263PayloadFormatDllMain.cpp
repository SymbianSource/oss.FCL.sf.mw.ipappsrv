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
#include "UT_CMccH263NewPayloadEncoder.h"
#include "UT_CMccH263OldPayloadEncoder.h"
#include "UT_CMccH263PayloadFormatWrite.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/CEUnitTestSuite.h>

/**
 * Test suite factory function.
 */
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC(_L("Wizard generated"));
    
    rootSuite->AddL( UT_CMccH263NewPayloadEncoder::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMccH263OldPayloadEncoder::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMccH263PayloadFormatWrite::NewLC() );
    CleanupStack::Pop();
    
    CleanupStack::Pop(); // rootSuite
    
    return rootSuite;
    }



//  END OF FILE






