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
* Description:    Implementation
*
*/




#include "CDelayedProcess.h"
#include <MceMediaStream.h>
#include <MceMediaSource.h>
#include <e32debug.h>

/**
 * Standard Symbian OS static function for creating an instance of the object.
 * If creation fails, a leave will be sent.
 * @param aDtmfSource pointer to the CMceMediaSource containing the dtmf source.
 */
CDelayedProcess* CDelayedProcess::NewL(CMceMediaSource* aDtmfSource)
{
	CDelayedProcess* self = new (ELeave) CDelayedProcess(aDtmfSource);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
}

CDelayedProcess::CDelayedProcess(CMceMediaSource* aDtmfSource)
: CActive(CActive::EPriorityStandard),
iDtmfSrc(aDtmfSource)
{
	// Call base constructor of CActive with the standard priority
	// Add this active object instance to the Active Scheduler:
	CActiveScheduler::Add(this);
}

void CDelayedProcess::ConstructL()
{
	// Create a thread-relative timer.
	User::LeaveIfError(iTimer.CreateLocal());
}

CDelayedProcess::~CDelayedProcess()
{
	// Call cancel-function of CActive base class, which will call DoCancel()
	Cancel();
	// Close the handle to the timer.
	iTimer.Close();
}

/**
 * Request from the application to start an asynchronous process.
 * @param aDelay delay until the asynch. timer event occurs, in microseconds.
 */
void CDelayedProcess::StartProcess(TTimeIntervalMicroSeconds32 aDelay)
{
	// Make sure that the active object is currently inactive
	_LIT(KDelayedProcessPanic, "CDelayedProcess");
	__ASSERT_ALWAYS(!IsActive(), User::Panic(KDelayedProcessPanic, KErrInUse));


	iTimer.After(iStatus, aDelay);

	// Indicates that the active object has issued a request and that it is now outstanding.
	SetActive();
}

/**
 * Handles an active object’s request completion event.
 */
void CDelayedProcess::RunL()
{
	
	_LIT(KTimerExpired, "Timer Expired\n");
	RDebug::Print(KTimerExpired);

	// Stop sending the tone
	if( iDtmfSrc->DtmfActive() )
	{
		iDtmfSrc->StopDtmfToneL();	
		
	}
	
	TRequestStatus* status = &iStatus;
	// Generates event on itself
//	User::RequestComplete(status, KErrNone);
	
}

/**
 * Implements cancellation of an outstanding request.
 */
void CDelayedProcess::DoCancel()
{
	iTimer.Cancel();
}
