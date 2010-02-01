/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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



        
// INCLUDE FILES
#include "mccperiodicrunner.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccPeriodicRunner::CMccPeriodicRunner
// -----------------------------------------------------------------------------
//
CMccPeriodicRunner::CMccPeriodicRunner( )
	: CActive( EPriorityStandard )
	{
	CActiveScheduler::Add(this);
	}

// -----------------------------------------------------------------------------
// CMccPeriodicRunner::ConstructL
// -----------------------------------------------------------------------------
//
void CMccPeriodicRunner::ConstructL()
    {
	User::LeaveIfError(iTimer.CreateLocal());
    }

// -----------------------------------------------------------------------------
// CMccPeriodicRunner::NewL
// -----------------------------------------------------------------------------
//
CMccPeriodicRunner* CMccPeriodicRunner::NewL( )
    {
    CMccPeriodicRunner* self = 
    	new ( ELeave ) CMccPeriodicRunner();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMccPeriodicRunner::Start
// -----------------------------------------------------------------------------
//
void CMccPeriodicRunner::Start( 
	TTimeIntervalMicroSeconds32 aInterval,
	TCallBack aCallBack )
	{	
	Cancel();
	
	iInterval = aInterval.Int();
	iCallBack = aCallBack;
	
	iTimer.After( iStatus, iInterval );
	SetActive();
	}

// -----------------------------------------------------------------------------
// CMccPeriodicRunner::Stop
// -----------------------------------------------------------------------------
//
void CMccPeriodicRunner::Stop()
	{
	Cancel();
	}

// -----------------------------------------------------------------------------
// CMccPeriodicRunner::Interval
// -----------------------------------------------------------------------------
//
TUint32 CMccPeriodicRunner::Interval()
    {
    return iInterval.Int();
    }
    
// -----------------------------------------------------------------------------
// CMccPeriodicRunner::~CMccPeriodicRunner
// -----------------------------------------------------------------------------
//
CMccPeriodicRunner::~CMccPeriodicRunner()
    {
    Cancel();
	iTimer.Close();
    }

// -----------------------------------------------------------------------------
// CMccPeriodicRunner::DoCancel
// -----------------------------------------------------------------------------
//
void CMccPeriodicRunner::DoCancel()
	{
	iTimer.Cancel();	
	}

// -----------------------------------------------------------------------------
// CMccPeriodicRunner::RunL
// -----------------------------------------------------------------------------
//	
void CMccPeriodicRunner::RunL()
	{
	iTimer.After( iStatus, iInterval );
	SetActive();
	iCallBack.CallBack();
	}

// -----------------------------------------------------------------------------
// CMccPeriodicRunner::RunError
// -----------------------------------------------------------------------------
//	
TInt CMccPeriodicRunner::RunError(TInt /*aError*/)
	{
	return KErrNone;
	}
