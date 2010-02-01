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
#include "UT_CMccController.h"
#include "UT_CMccResourceItem.h"
#include "UT_CMccResourceContainer.h"
#include "UT_CMccResourcePool.h"
#include "UT_CMccCameraHandler.h"
#include "UT_CMccResourceIterators.h"
#include "UT_CMccScreen.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/CEUnitTestSuite.h>





/**
 * Test suite factory function.
 */
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
	CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC(_L("Wizard generated"));
		
	rootSuite->AddL( UT_CMccResourceItem::NewLC() );
	CleanupStack::Pop();

	rootSuite->AddL( UT_CMccResourceContainer::NewLC() );
	CleanupStack::Pop();

	rootSuite->AddL( UT_CMccResourcePool::NewLC() );
	CleanupStack::Pop();

	rootSuite->AddL( UT_CMccResourceIterators::NewLC() );
	CleanupStack::Pop();
	
	rootSuite->AddL( UT_CMccController::NewLC() );
	CleanupStack::Pop();
	
	rootSuite->AddL( UT_CMccCameraHandler::NewLC() );
	CleanupStack::Pop();
	
	rootSuite->AddL( UT_CMccScreen::NewLC() );
	CleanupStack::Pop();
	    
	CleanupStack::Pop(); // rootSuite
	
	return rootSuite;
    }

/**
 * Standard Symbian DLL entry point function.
 */
 /*
GLDEF_C TInt E32Dll(TDllReason)
    {
    return KErrNone;
    }
*/
//  END OF FILE
