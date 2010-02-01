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
* Description:    Starts sending keepalive -requests to STUN Client.
*
*/




#include "natfwstunrefreshtimer.h"
#include "natfwstunpluginlogs.h"
#include "natfwrefreshobserver.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ default constructor
// ---------------------------------------------------------------------------
//
CNATFWStunRefreshTimer::CNATFWStunRefreshTimer( 
    MNATFWRefreshObserver& aObserver ) : 
    CActive( EPriorityStandard ), iObserver( aObserver )
    {
    __STUNPLUGIN( "CNATFWStunRefreshTimer::CNATFWStunRefreshTimer start" )
    CActiveScheduler::Add( this );
    __STUNPLUGIN( "CNATFWStunRefreshTimer::CNATFWStunRefreshTimer end" )
    }


// ---------------------------------------------------------------------------
// Symbian constructor
// Creates timer service.
// ---------------------------------------------------------------------------
//
void CNATFWStunRefreshTimer::ConstructL()
    {
    __STUNPLUGIN( "CNATFWStunRefreshTimer::ConstructL start" )

    User::LeaveIfError( iTimer.CreateLocal() );

    __STUNPLUGIN( "CNATFWStunRefreshTimer::ConstructL end" )
    }


// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
CNATFWStunRefreshTimer* CNATFWStunRefreshTimer::NewL( 
    MNATFWRefreshObserver& aObserver )
    {
    __STUNPLUGIN( "CNATFWStunRefreshTimer::NewL" )
    CNATFWStunRefreshTimer* self = 
        CNATFWStunRefreshTimer::NewLC( aObserver );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
CNATFWStunRefreshTimer* CNATFWStunRefreshTimer::NewLC( 
    MNATFWRefreshObserver& aObserver )
    {
    __STUNPLUGIN( "CNATFWStunRefreshTimer::NewLC" )
    CNATFWStunRefreshTimer* self = 
         new( ELeave ) CNATFWStunRefreshTimer( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CNATFWStunRefreshTimer::~CNATFWStunRefreshTimer()
    {
    __STUNPLUGIN( "CNATFWStunRefreshTimer::~CNATFWStunRefreshTimer" )

    Cancel();
    iTimer.Close();
    }


// ---------------------------------------------------------------------------
// Receives interval as a parameter and waits for that time before activation.
// ---------------------------------------------------------------------------
//
void CNATFWStunRefreshTimer::StartStunRefresh( TUint aInterval )
    {
    __STUNPLUGIN( "CNATFWStunRefreshTimer::StartStunRefresh" )
   
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
    }


// ---------------------------------------------------------------------------
// Checks refresher state
// ---------------------------------------------------------------------------
//
TBool CNATFWStunRefreshTimer::IsRunning() const
    {
    return IsActive();
    }


// ---------------------------------------------------------------------------
// Sends BindingRefresh request to CNATFWStunConnectionHandler through 
// MNATFWRefreshObserver after time in iInterval has passed.
// ---------------------------------------------------------------------------
//
void CNATFWStunRefreshTimer::RunL()
    {
    __STUNPLUGIN( "CNATFWStunRefreshTimer::RunL" )
    
    iObserver.BindingRefreshL();
    StartStunRefresh( iInterval );
    }
    
    
// ---------------------------------------------------------------------------
// Cancels the current timer.
// ---------------------------------------------------------------------------
//
void CNATFWStunRefreshTimer::DoCancel()
    {
    __STUNPLUGIN( "CNATFWStunRefreshTimer::DoCancel" )
    iTimer.Cancel();
    }

// End of file
