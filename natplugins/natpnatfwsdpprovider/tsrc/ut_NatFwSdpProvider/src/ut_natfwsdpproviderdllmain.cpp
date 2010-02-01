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


//  CLASS HEADERS
#include "ut_nspcontroller.h"
#include "ut_nspcontentParser.h"
#include "ut_nspsession.h"
#include "ut_nspusecases.h"
#include "ut_nspusecases_fqdn.h"
#include "ut_nsputil.h"
#include "ut_nspinterface.h"
#include "ut_nspmediastreamcontainer.h"
#include "ut_nspmediastream.h"
#include "ut_nspmediastreamcomponent.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/CEUnitTestSuite.h>

_LIT( KNSPTester, "Root suite for NATFW SDP Provider" );

/**
 * Test suite factory function.
 */
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC( KNSPTester() );
    
    rootSuite->AddL( UT_CNSPController::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CNSPContentParser::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CNSPSession::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_NSPUtil::NewLC() );
    CleanupStack::Pop();    
    
    rootSuite->AddL( UT_CNSPMediaStreamContainer::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CNSPMediaStream::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CNSPMediaStreamComponent::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CNSPInterface::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CNSPUseCases::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CNSPUseCases_fqdn::NewLC() );
    CleanupStack::Pop();
    
    CleanupStack::Pop(); // rootSuite
    return rootSuite;
    }

//  END OF FILE
