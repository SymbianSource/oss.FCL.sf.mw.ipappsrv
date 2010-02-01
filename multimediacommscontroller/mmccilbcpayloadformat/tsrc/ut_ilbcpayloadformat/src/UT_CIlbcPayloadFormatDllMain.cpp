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
#include "UT_CIlbcPayloadFormatRead.h"
#include "UT_CIlbcPayloadFormatWrite.h"
#include "UT_TStreamDecoder.h"
#include "UT_TStreamEncoder.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/CEUnitTestSuite.h>


/**
 * Test suite factory function.
 */
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
	//return UT_CAmrCommonUtility::NewL();
    CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC(_L("Wizard generated"));

    rootSuite->AddL(  UT_CIlbcPayloadFormatRead::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL(  UT_CIlbcPayloadFormatWrite::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_TStreamDecoder::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_TStreamEncoder::NewLC() );
    CleanupStack::Pop();    
    
    CleanupStack::Pop(); // rootSuite
    
	return rootSuite;
    }

//  END OF FILE