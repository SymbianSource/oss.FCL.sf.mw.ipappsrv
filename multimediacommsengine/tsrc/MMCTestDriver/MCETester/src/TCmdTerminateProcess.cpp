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
* Description:    Implementation
*
*/




#include <flogger.h>
#include "TCmdTerminateProcess.h"
#include "MCEConstants.h"


/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	Process
 *   IDs:			-
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			-
 */

//_LIT(KFileDir, "Sip");
//_LIT(KFileName, "Tester.txt");

void TCmdTerminateProcess::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------


	// -- Execution -----------------------------------------------------------
/*
	// Terminate Ssc Server

#if ( !defined(EKA2) && ( defined(__WINS__) || defined(__WINSCW__) ) ) 
	// WINS doesn't have a concept of process (in EKA1)
	RThread process;
	TFindThread processFinder (_L("*MceServer*"));
#else
	RProcess process;
	TFindProcess processFinder (_L("*MceServer*"));
#endif

	// Find the process
	TFullName processName;
	while ( processFinder.Next( processName ) == KErrNone )
		{
		User::InfoPrint(_L("Found Ssc server"));
		RFileLogger::Write (KFileDir,KFileName,EFileLoggingModeAppend,processName);
		User::LeaveIfError( process.Open( processFinder ) );
			// ..and terminate it
		// process.Terminate( KErrNone );
		process.Kill( KErrNone );
		process.Close();
		}
*/
	// Terminate Sip profile server

#if ( !defined(EKA2) && ( defined(__WINS__) || defined(__WINSCW__) ) ) 
	// WINS doesn't have a concept of process (in EKA1)
	RThread sipProcess;
	TFindThread sipProcessFinder (_L("*SipProfile*"));
#else
	RProcess sipProcess;
	TFindProcess sipProcessFinder (_L("*SipProfile*"));
#endif

	// Find the process
	TFullName sipProcessName;
	while ( sipProcessFinder.Next( sipProcessName ) == KErrNone )
		{
		User::InfoPrint(_L("Found profile server"));
		//RFileLogger::Write (KFileDir,KFileName,EFileLoggingModeAppend,sipProcessName);
		/*User::LeaveIfError( sipProcess.Open( sipProcessFinder ) );
			// ..and terminate it
		// process.Terminate( KErrNone );
		sipProcess.Kill( KErrNone );
		sipProcess.Close();*/
		}


	// -- Response creation ---------------------------------------------------
	}

TBool TCmdTerminateProcess::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("TerminateProcess") );
	}

TTcCommandBase* TCmdTerminateProcess::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdTerminateProcess( aContext );
	}
/*



This should be
      _LIT(KProcess,"A[");

Match expects wildcards. The 0 means it only considers it found if it is
the start of the string.

>         TInt perr = User::LeaveIfError (processHandle.Open
> (processFinder,EOwnerThread));
>         RProcess processHandle;
>          if ( perr == KErrNone ) processHandle.Kill(0);
>         CleanupClosePushL (processHandle);

The CleanupClosePushL should be immediatly after the Open - least chance
of a leaving statment being added before it is on the cleanup stack.


>         CleanupStack::PopAndDestroy (&processHandle);
*/
