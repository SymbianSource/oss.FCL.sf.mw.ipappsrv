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






#include <e32math.h>
#include "mceserverstarter.h"
#include "mceclientserver.h"

#define RETURN_IF_ERROR(err) {TInt _err=err; if(_err!=KErrNone) {return _err;}}

// -----------------------------------------------------------------------------
// MceServerStarter::Start
// -----------------------------------------------------------------------------
//
TInt MceServerStarter::Start ()
	{
	TInt err = KErrNone;

    TFindServer findServer( KMceServerName );
    TFullName name;
	if ( findServer.Next( name ) == KErrNone) 
        {
        return KErrNone; // Server already running
        }
		
	RSemaphore semaphore;
	err = semaphore.CreateGlobal( KMceServerSemaphoreName , 0 );
	if ( err != KErrNone )
		{
		return err;
		}

	err = CreateServerProcess( semaphore );
	semaphore.Close ();

    return err;
    
	}

// -----------------------------------------------------------------------------
// MceServerStarter::CreateServerProcess
// -----------------------------------------------------------------------------
//
TInt MceServerStarter::CreateServerProcess (RSemaphore& aSemaphore)
	{
	const TUidType serverUid (KNullUid, KNullUid, KServerUid3);


#if ((defined (__WINS__) || defined(__WINSCW__)) && !defined (EKA2))

	RLibrary lib;
    RETURN_IF_ERROR (lib.Load(KMceServerFilename,serverUid))

    //  Get the WinsMain function
	TLibraryFunction functionWinsMain = lib.Lookup (1);

    //  Call it and cast the result to a thread function
	TThreadFunction serverThreadFunction = 
        reinterpret_cast<TThreadFunction> (functionWinsMain());

	TName threadName (KSipServerName);

	// Append a random number to make it unique
	threadName.AppendNum (Math::Random(), EHex);

	RThread server;
    TInt err = server.Create (threadName,
                              serverThreadFunction, // thread's main function
                              KDefaultStackSize,
                              NULL,
                              &lib,
                              NULL,
                              KServerMinHeapSize,
                              KServerMaxHeapSize,
                              EOwnerProcess );

	lib.Close ();	// if successful, server thread has handle to library now
    RETURN_IF_ERROR (err)

	server.SetPriority (EPriorityMore);

#else // HW build

	RProcess server;
	RETURN_IF_ERROR (server.Create( KMceServerName, KNullDesC, serverUid ) )

#endif


	server.Resume ();
	aSemaphore.Wait();
	TInt exitReason = server.ExitReason();
	server.Close ();

    return exitReason;
	}
