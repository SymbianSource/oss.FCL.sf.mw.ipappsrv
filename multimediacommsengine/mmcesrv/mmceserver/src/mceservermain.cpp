/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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





#include "mceclientserver.h"
#include "mceservermain.h"
#include "mceservercore.h"
#include "mcesrvlogs.h"

// -----------------------------------------------------------------------------
// MceServerMain::ThreadFunction
// -----------------------------------------------------------------------------
//
TInt MceServerMain::ThreadFunction (TAny* /*aNone*/)
	{
    TInt err = KErrNone;
    CTrapCleanup* cleanupStack = CTrapCleanup::New();
	if (!cleanupStack) 
        {
        PanicServer(ECreateTrapCleanup);
        }
	RSemaphore semaphore;
	err = semaphore.OpenGlobal(KMceServerSemaphoreName);
    if (err != KErrNone) 
        {
        PanicServer(ESrvCreateServer);
        }
    TRAP(err, ThreadFunctionL(semaphore));
    if (err != KErrNone)
        {
        semaphore.Signal();
        semaphore.Close();
        }

    delete cleanupStack;
    return err;
	}

// -----------------------------------------------------------------------------
// MceServerMain::ThreadFunctionL
// -----------------------------------------------------------------------------
//
void MceServerMain::ThreadFunctionL (RSemaphore& aSemaphore)
	{
	// Give a name to this thread
    User::LeaveIfError(User::RenameThread(KMceServerName));

    // Construct server
    CMceServerCore::NewLC();

	// Server created ok
	aSemaphore.Signal();
	aSemaphore.Close();

	// Start handling requests
	CActiveScheduler::Start();

    // This will be executed after the active scheduler has been stopped:
    CleanupStack::PopAndDestroy(1); // server
	}

// -----------------------------------------------------------------------------
// MceServerMain::PanicServer
// -----------------------------------------------------------------------------
//
void MceServerMain::PanicServer (TMceServerPanic aPanic)
	{
    User::Panic(KMceServerPanic, aPanic);
	}
 

#if ((defined (__WINS__) || defined(__WINSCW__)) && !defined (EKA2))

EXPORT_C TInt WinsMain()
    {
    MCESRV_DEBUG("MCE Server process started");
	return reinterpret_cast<TInt>(&MceServerMain::ThreadFunction);
    }

TInt E32Dll(TDllReason) 
    {
    return KErrNone; 
    }

#else // HW build

TInt E32Main() 
    {
    MCESRV_DEBUG("MCE Server process started");
    return MceServerMain::ThreadFunction(NULL);
    }

#endif

