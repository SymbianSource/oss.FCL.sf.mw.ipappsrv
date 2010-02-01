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
#include "UT_CDTMFPayloadEncoder.h"
#include "UT_CDTMFPayloadDecoder.h"
#include "UT_CDTMFPayloadFormatRead.h"
#include "UT_CDTMFPayloadFormatWrite.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/CEUnitTestSuite.h>

/**
 * Test suite factory function.
 */
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC( _L("mcc/dtmf payload formatter") );

    rootSuite->AddL( UT_CDTMFPayloadDecoder::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CDTMFPayloadEncoder::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CDTMFPayloadFormatRead::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CDTMFPayloadFormatWrite::NewLC() );
    CleanupStack::Pop();
    
    CleanupStack::Pop( rootSuite );
    
    return rootSuite;
    }

/**
 * Standard Symbian DLL entry point function.
 */
/**
 *GLDEF_C TInt E32Dll(TDllReason)
 *   {
 *   return KErrNone;
 *   }
 */
//  END OF FILE
