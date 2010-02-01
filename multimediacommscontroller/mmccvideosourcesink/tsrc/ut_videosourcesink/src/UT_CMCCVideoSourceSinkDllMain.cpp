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
#include "UT_CMccVideoSink.h"
#include "UT_CMccVideoSourceImpl.h"
#include "UT_CMccVideoSource.h"
#include "UT_CMccVideoSinkImpl.h"
#include "UT_CMccVideoSinkUser.h"
#include "UT_CMccVideoJitterBuffer.h"
#include "UT_CMccVideoSourceKeyRetriever.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/CEUnitTestSuite.h>

/**
 * Test suite factory function.
 */
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC(_L("Wizard generated"));
     
    rootSuite->AddL( UT_CMccVideoSink::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMccVideoSinkImpl::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMccVideoSinkUser::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMccVideoJitterBuffer::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMccVideoSourceImpl::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMccVideoSource::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMccVideoSourceKeyRetriever::NewLC() );
    CleanupStack::Pop();
    
    CleanupStack::Pop(); // rootSuite
    
    return rootSuite;
    }



//  END OF FILE






