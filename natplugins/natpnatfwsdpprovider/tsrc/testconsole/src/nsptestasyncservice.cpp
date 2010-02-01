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
* Description:  
*
*/

#include <e32std.h>
#include <e32base.h>
#include "nsptestasyncservice.h"

#define __PANIC( aError ) \
	User::Panic( _L( "NSP Test console test async service" ), aError )

const TTimeIntervalMicroSeconds32 KTimeoutInterval = 10000000; // 10s

// -----------------------------------------------------------------------------
// CNSPTestAsyncService::NewL
// -----------------------------------------------------------------------------
// 
CNSPTestAsyncService* CNSPTestAsyncService::NewL( TInt& aTimerStatus )
    {
    CNSPTestAsyncService* self = CNSPTestAsyncService::NewLC( aTimerStatus );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CNSPTestAsyncService::NewLC
// -----------------------------------------------------------------------------
// 
CNSPTestAsyncService* CNSPTestAsyncService::NewLC( TInt& aTimerStatus )
    {
    CNSPTestAsyncService* self = new ( ELeave ) CNSPTestAsyncService( aTimerStatus );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CNSPTestAsyncService::~CNSPTestAsyncService
// -----------------------------------------------------------------------------
// 
CNSPTestAsyncService::~CNSPTestAsyncService()
    {
	if ( iWait && iWait->IsStarted() )
		{
		iWait->AsyncStop();
		}
	
    delete iWait;
    iTimer.Close();
    }

// -----------------------------------------------------------------------------
// CNSPTestAsyncService::CNSPTestAsyncService
// -----------------------------------------------------------------------------
// 
CNSPTestAsyncService::CNSPTestAsyncService( TInt& aTimerStatus )
    : CActive( EPriorityStandard ),
      iTimerStatus( aTimerStatus )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CNSPTestAsyncService::ConstructL
// -----------------------------------------------------------------------------
// 
void CNSPTestAsyncService::ConstructL()
    {
	User::LeaveIfError( iTimer.CreateLocal() );
    iWait = new (ELeave) CActiveSchedulerWait;
	}

// -----------------------------------------------------------------------------
// CNSPTestAsyncService::StartScheduler
// -----------------------------------------------------------------------------
// 
void CNSPTestAsyncService::RunL()
	{
	iTimerStatus = KErrTimedOut;
	
	if ( iWait && iWait->IsStarted() )
		{
		iWait->AsyncStop();	
		}
	}

// -----------------------------------------------------------------------------
// CNSPTestAsyncService::StartScheduler
// -----------------------------------------------------------------------------
// 
void CNSPTestAsyncService::DoCancel()
	{
	iTimerStatus = KErrCancel;
	iTimer.Cancel();
	
	if ( iWait && iWait->IsStarted() )
		{
		iWait->AsyncStop();	
		}
	}

// -----------------------------------------------------------------------------
// CNSPTestAsyncService::StartScheduler
// -----------------------------------------------------------------------------
// 
void CNSPTestAsyncService::StartScheduler()
	{
	if ( IsActive() )
		{
		Cancel();
		}
	
    iTimer.After( iStatus, KTimeoutInterval );
	SetActive();
	iWait->Start();
	}

// -----------------------------------------------------------------------------
// CNSPTestAsyncService::Stop
// -----------------------------------------------------------------------------
// 
void CNSPTestAsyncService::Stop()
	{
	iTimerStatus = KErrNone;
	iTimer.Cancel();
	
	if ( iWait && iWait->IsStarted() )
		{
		iWait->AsyncStop();	
		}
	}

