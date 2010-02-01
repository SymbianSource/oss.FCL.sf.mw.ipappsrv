/*
* Copyright (c) 2006 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Starts sending keepalive -requests to TURN Client.
*
*/




#include "natfwturnrefreshtimer.h"
#include "natfwturnpluginlogs.h"
#include "natfwrefreshobserver.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ default constructor
// ---------------------------------------------------------------------------
//
CNATFWTurnRefreshTimer::CNATFWTurnRefreshTimer( 
    MNATFWRefreshObserver& aObserver ) :
    CActive( EPriorityStandard ), iObserver( aObserver )
    {
    __TURNPLUGIN( "CNATFWTurnRefreshTimer::CNATFWTurnRefreshTimer" )
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------------------------
// Symbian constructor
// Creates timer service.
// ---------------------------------------------------------------------------
//
void CNATFWTurnRefreshTimer::ConstructL()
    {
    __TURNPLUGIN( "CNATFWTurnRefreshTimer::ConstructL start" )

    User::LeaveIfError( iTimer.CreateLocal() );

    __TURNPLUGIN( "CNATFWTurnRefreshTimer::ConstructL end" )
    }


// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
CNATFWTurnRefreshTimer* CNATFWTurnRefreshTimer::NewL( 
    MNATFWRefreshObserver&aObserver )
    {
    __TURNPLUGIN( "CNATFWTurnRefreshTimer::NewL" )
    CNATFWTurnRefreshTimer* self = 
        CNATFWTurnRefreshTimer::NewLC( aObserver );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
CNATFWTurnRefreshTimer* CNATFWTurnRefreshTimer::NewLC( 
    MNATFWRefreshObserver&aObserver )
    {
    __TURNPLUGIN( "CNATFWTurnRefreshTimer::NewLC" )
    CNATFWTurnRefreshTimer* self = 
         new( ELeave ) CNATFWTurnRefreshTimer( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CNATFWTurnRefreshTimer::~CNATFWTurnRefreshTimer()
    {
    __TURNPLUGIN( "CNATFWTurnRefreshTimer::~CNATFWTurnRefreshTimer" )

    Cancel();
    iTimer.Close();
    }


// ---------------------------------------------------------------------------
// Receives interval as a parameter and waits for that time before activation.
// ---------------------------------------------------------------------------
//
void CNATFWTurnRefreshTimer::StartTurnRefresh( TUint aInterval )
    {
    __TURNPLUGIN( "CNATFWTurnRefreshTimer::StartTurnRefresh start" )
    
    iInterval = aInterval;  
    
    if ( iInterval > 0 )
        {
        if ( !IsActive() )
            {
            iTimer.After( iStatus, aInterval );
            SetActive();
            }
        }
    else
        {
        Cancel();
        }

    __TURNPLUGIN( "CNATFWTurnRefreshTimer::StartTurnRefresh end" )
    }


// ---------------------------------------------------------------------------
// Checks refresher state.
// ---------------------------------------------------------------------------
//
TBool CNATFWTurnRefreshTimer::IsRunning() const
    {
    return IsActive();
    }


// ---------------------------------------------------------------------------
// Sends BindingRefresh request to CNATFWTurnConnectionHandler through
// MNATFWRefreshObserver after time in iInterval has passed.
// ---------------------------------------------------------------------------
//
void CNATFWTurnRefreshTimer::RunL()
    {
    __TURNPLUGIN( "CNATFWTurnRefreshTimer::RunL start" )

    iObserver.BindingRefreshL();
    StartTurnRefresh( iInterval );

    __TURNPLUGIN( "CNATFWTurnRefreshTimer::RunL end" )
    }


// ---------------------------------------------------------------------------
// Cancels the current timer.
// ---------------------------------------------------------------------------
//
void CNATFWTurnRefreshTimer::DoCancel()
    {
    __TURNPLUGIN( "CNATFWTurnRefreshTimer::DoCancel" )
    iTimer.Cancel();
    }
    
// End of file
