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
**
*/




#include <e32svr.h>
#include "testconsole.h"


// ActiveScheduler trap harness
void startSchedulerL()
    {
	CActiveScheduler* testScheduler = new (ELeave)CActiveScheduler;
	CleanupStack::PushL( testScheduler );
	CActiveScheduler::Install( testScheduler );

    //Enter Active Object
	CTestAppConsole* appConsole = CTestAppConsole::NewLC();

	appConsole->StartTesting();
	CActiveScheduler::Start();
    
    
    CleanupStack::PopAndDestroy( appConsole );
    CleanupStack::PopAndDestroy( testScheduler );
    
    }


// Prog entry point
GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;
    RDebug::Print( _L("E32Main START Cells: %d"), User::CountAllocCells() );

	CTrapCleanup* stack = CTrapCleanup::New(); 
	TRAPD( err, startSchedulerL() );
	delete stack;
    
    RDebug::Print( _L("E32Main END Cells: %d"), User::CountAllocCells() );
    __UHEAP_MARKEND;
	
	return err;
	}
