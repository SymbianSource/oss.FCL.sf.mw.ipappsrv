/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Exe entry point
*
*/

#include <e32svr.h>
#include <sdpcodecstringpool.h>
#include "nsptestconsoleapp.h"

void StartSchedulerL()
	{
	SdpCodecStringPool::OpenL();
	CActiveScheduler* testScheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL( testScheduler );
	CActiveScheduler::Install( testScheduler );
	
	CNSPTestConsoleApp* appConsole = CNSPTestConsoleApp::NewLC();
	appConsole->StartTesting();
	CActiveScheduler::Start();
	
    CleanupStack::PopAndDestroy( appConsole );
    CleanupStack::PopAndDestroy( testScheduler );
	SdpCodecStringPool::Close();
    }

// Entry point
GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;
    RDebug::Print( _L("E32Main START Cells: %d"), User::CountAllocCells() );
    
	CTrapCleanup* stack = CTrapCleanup::New();
	TRAPD( err, StartSchedulerL() );
	delete stack;
	
	RDebug::Print( _L("E32Main TRAPD returned with code: %d"), err );
    RDebug::Print( _L("E32Main END Cells: %d"), User::CountAllocCells() );
    
    __UHEAP_MARKEND;
	return err;
	}

