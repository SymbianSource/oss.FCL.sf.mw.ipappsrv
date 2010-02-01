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
* Description:  Implementation
*
*/

#include "TCmdTerminateProcess.h"
#include "SIPConstants.h"
#include <flogger.h>
#include <f32file.h>

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

_LIT(KFileDir, "Sip");
_LIT(KFileName, "Tester.txt");
// Literals used for action name
_LIT8( KIsRunning, "running" );
_LIT8( KTerminate, "terminate" );

void TCmdTerminateProcess::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	TPtrC8 action = ExtractTextL( KParamActionName );
    // Get action name pattern for the process to be found
    TPtrC8 processPattern = ExtractTextL( KParamProcess );

    TFullName procName;
    procName.Copy(processPattern);  

	// -- Execution -----------------------------------------------------------

    if( !action.CompareC( KTerminate ) ) 
		{
		TerminateProcessL(procName);
		}
    else if( !action.CompareC( KIsRunning ) )
    	{
    	if(IsProcessRunning(procName))
    		{
    		AddBooleanResponseL(_L8("IsRunning"), ETrue);
    		} 
    	else
			{
			AddBooleanResponseL(_L8("IsRunning"), EFalse);
			}
    	}
    else {}
    
	// -- Response creation ---------------------------------------------------
	}

TBool TCmdTerminateProcess::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("TerminateProcess") );
	}

TTcCommandBase* TCmdTerminateProcess::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdTerminateProcess( aContext );
	}

void TCmdTerminateProcess::TerminateProcessL(const TDesC& aName)
	{

#if ( !defined(EKA2) && ( defined(__WINS__) || defined(__WINSCW__) ) ) 
	// WINS doesn't have a concept of process (in EKA1)
	RThread process;
	TFindThread processFinder (aName);
#else
	RProcess process;
	TFindProcess processFinder (aName);
#endif

	// Find the process
	TFullName processName;
	while ( processFinder.Next( processName ) == KErrNone )
		{
		User::InfoPrint(_L("Found"));
		RFileLogger::Write (KFileDir,KFileName,EFileLoggingModeAppend,processName);
		User::LeaveIfError( process.Open( processFinder ) );
			// ..and terminate it
		// process.Terminate( KErrNone );
		process.Kill( KErrNone );
		process.Close();
		
		AddTextResponseL( KParamClientInfo, _L8("Process terminated") );
		}
	}

TBool TCmdTerminateProcess::IsProcessRunning(const TDesC& aName)
	{
	TBool Ret(EFalse);
	
	TFullName result;
	TFindProcess find(aName);
	TInt count = 0;
  	while(find.Next(result) == KErrNone)
    {
		++count;
		if(count >= 1)
			{
			Ret = ETrue;
			break;
			}
    }
    return Ret;
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
