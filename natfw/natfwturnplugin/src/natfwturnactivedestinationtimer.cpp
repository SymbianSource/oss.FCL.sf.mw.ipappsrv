/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Is used to time actions related to setting Active
*                Destination request.
*
*/




#include "natfwturnactivedestinationtimer.h"
#include "natfwturnpluginlogs.h"
#include "natfwtimerobserver.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ default constructor
// ---------------------------------------------------------------------------
//
CNATFWTurnActiveDestinationTimer::CNATFWTurnActiveDestinationTimer(
    MNATFWTimerObserver& aObserver ) : CActive( EPriorityStandard ), 
    iObserver( aObserver )
    {
    __TURNPLUGIN( 
    "CNATFWTurnActiveDestinationTimer::CNATFWTurnActiveDestinationTimer" )
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------------------------
// Symbian constructor
// Creates timer service.
// ---------------------------------------------------------------------------
//
void CNATFWTurnActiveDestinationTimer::ConstructL()
    {
    __TURNPLUGIN( "CNATFWTurnActiveDestinationTimer::ConstructL" )

    User::LeaveIfError( iTimer.CreateLocal() );
    }


// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
CNATFWTurnActiveDestinationTimer* CNATFWTurnActiveDestinationTimer::NewL(
    MNATFWTimerObserver& aObserver )
    {
    __TURNPLUGIN( "CNATFWTurnActiveDestinationTimer::NewL" )
    CNATFWTurnActiveDestinationTimer* self = 
        CNATFWTurnActiveDestinationTimer::NewLC( aObserver );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
CNATFWTurnActiveDestinationTimer* CNATFWTurnActiveDestinationTimer::NewLC(
    MNATFWTimerObserver& aObserver )
    {
    __TURNPLUGIN( "CNATFWTurnActiveDestinationTimer::NewLC" )
    CNATFWTurnActiveDestinationTimer* self = 
         new( ELeave ) CNATFWTurnActiveDestinationTimer( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CNATFWTurnActiveDestinationTimer::~CNATFWTurnActiveDestinationTimer()
    {
    __TURNPLUGIN( 
    "CNATFWTurnActiveDestinationTimer::~CNATFWTurnActiveDestinationTimer" )
    Cancel();
    iTimer.Close();
    }


// ---------------------------------------------------------------------------
// CNATFWTurnActiveDestinationTimer::StartTimer
// ---------------------------------------------------------------------------
//
void CNATFWTurnActiveDestinationTimer::StartTimer( TUint32 aInterval,
                                                   TUint aStreamId,
                                                   TUint aConnectionId )
    {
    __TURNPLUGIN( "CNATFWTurnActiveDestinationTimer::StartTimer" )
    
    iStreamId = aStreamId;
    iConnectionId = aConnectionId;
    
    if ( !IsActive() )
        {
        iTimer.After( iStatus, aInterval );
        SetActive();
        }
    }


// ---------------------------------------------------------------------------
// From class CActive
// CNATFWTurnActiveDestinationTimer::RunL
// ---------------------------------------------------------------------------
//
void CNATFWTurnActiveDestinationTimer::RunL()
    {
    __TURNPLUGIN( "CNATFWTurnActiveDestinationTimer::RunL" )

    iObserver.TimerTriggeredL( iStreamId, iConnectionId );
    }


// ---------------------------------------------------------------------------
// From class CActive
// CNATFWTurnActiveDestinationTimer::DoCancel
// ---------------------------------------------------------------------------
//
void CNATFWTurnActiveDestinationTimer::DoCancel()
    {
    __TURNPLUGIN( "CNATFWTurnActiveDestinationTimer::DoCancel" )
    iTimer.Cancel();
    }

// End of file
