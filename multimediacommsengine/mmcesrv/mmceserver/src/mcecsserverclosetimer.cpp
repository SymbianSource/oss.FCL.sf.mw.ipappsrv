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




#include "mcecsserverclosetimer.h"
#include "mcesip.h"

// -----------------------------------------------------------------------------
// CMceCsServerCloseTimer::NewL
// -----------------------------------------------------------------------------
//
CMceCsServerCloseTimer* CMceCsServerCloseTimer::NewL()
    {
    CMceCsServerCloseTimer* self = CMceCsServerCloseTimer::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceCsServerCloseTimer::NewLC
// -----------------------------------------------------------------------------
//
CMceCsServerCloseTimer* CMceCsServerCloseTimer::NewLC()
    {
    CMceCsServerCloseTimer* self = new (ELeave) CMceCsServerCloseTimer;
    CleanupStack::PushL( self );
    self->ConstructL ();
    return self;
    }

// -----------------------------------------------------------------------------
// CMceCsServerCloseTimer::ConstructL
// -----------------------------------------------------------------------------
//
void CMceCsServerCloseTimer::ConstructL()
    {
	User::LeaveIfError( iTimer.CreateLocal() );
    }

// -----------------------------------------------------------------------------
// CMceCsServerCloseTimer::CMceCsServerCloseTimer
// -----------------------------------------------------------------------------
//
CMceCsServerCloseTimer::CMceCsServerCloseTimer()
 : CActive( EPriorityStandard )
    {
	CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CMceCsServerCloseTimer::~CMceCsServerCloseTimer
// -----------------------------------------------------------------------------
//
CMceCsServerCloseTimer::~CMceCsServerCloseTimer()
    {
	CActive::Cancel();
	iTimer.Close();
    }

// -----------------------------------------------------------------------------
// CMceCsServerCloseTimer::StopActiveSchedulerAfter
// -----------------------------------------------------------------------------
//
void CMceCsServerCloseTimer::StopActiveSchedulerAfter( TUint aMilliSeconds )
    {
    if ( !IsActive() )
        {
	    TTimeIntervalMicroSeconds32 
	        timeInterval( KConversionMilliToMicro * aMilliSeconds );
        iTimer.After(iStatus, timeInterval);
        SetActive();
        }
    }

// -----------------------------------------------------------------------------
// CMceCsServerCloseTimer::RunL
// -----------------------------------------------------------------------------
//
void CMceCsServerCloseTimer::RunL()
    {
	CActiveScheduler::Stop();
    }

// -----------------------------------------------------------------------------
// CMceCsServerCloseTimer::DoCancel
// -----------------------------------------------------------------------------
//	
void CMceCsServerCloseTimer::DoCancel()
    {
	iTimer.Cancel();
    }
