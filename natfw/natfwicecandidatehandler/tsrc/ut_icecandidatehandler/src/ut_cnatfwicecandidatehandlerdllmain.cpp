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
#include "UT_CIceSessionData.h"
#include "UT_CIceCheckList.h"
#include "UT_CIceCheckHandler.h"
#include "UT_CIceConnectivityCheck.h"
#include "UT_CIceCheckPrioritizer.h"
#include "UT_CIceCandidatePrioritizer.h"
#include "UT_CIceStreamCollection.h"
#include "ut_cicevalidlist.h"
#include "UT_CIcePlugin.h"
#include "ut_ciceconnectionhandler.h"
#include "UT_CIceLocalCandidateFinder.h"
#include "UT_CIceCallBackExecuter.h"
#include "UT_CIceHostResolver.h"
#include "UT_CIceNatPluginContainer.h"
#include "UT_CIceConnection.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/CEUnitTestSuite.h>

/**
 * Test suite factory function.
 */
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC(_L("Wizard generated"));
    
    rootSuite->AddL( UT_CIceSessionData::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CIceCheckList::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CIceCheckHandler::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CIceConnectivityCheck::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CIceCheckPrioritizer::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CIceCandidatePrioritizer::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CIceStreamCollection::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CIceValidList::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CIcePlugin::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CIceConnectionHandler::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CIceLocalCandidateFinder::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CIceCallBackExecuter::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CIceHostResolver::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CIceNatPluginContainer::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CIceConnection::NewLC() );
    CleanupStack::Pop();
    
    CleanupStack::Pop(); // rootSuite
    
    return rootSuite;
    }

//  END OF FILE






