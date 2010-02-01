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




#include "stunassert.h"
#include "cstuntimeruser.h"
#include "stunutils.h"
#include "stunturnclientlogs.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSTUNTimerUser::CSTUNTimerUser
// ---------------------------------------------------------------------------
//
CSTUNTimerUser::CSTUNTimerUser( CDeltaTimer& aDeltaTimer ) :
    iDeltaTimer( aDeltaTimer )
    {    
    }

// ---------------------------------------------------------------------------
// CSTUNTimerUser::CSTUNTimerUser
// Dummy implementation. Default constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CSTUNTimerUser::CSTUNTimerUser() : iDeltaTimer( *( CDeltaTimer* )0x1 )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNTimerUser::CSTUNTimerUser
// Dummy implementation, as copy constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CSTUNTimerUser::CSTUNTimerUser( const CSTUNTimerUser& aTimerUser ) :
    CBase(),
    iDeltaTimer( aTimerUser.iDeltaTimer )
    {    
    }

// ---------------------------------------------------------------------------
// CSTUNTimerUser::~CSTUNTimerUser
// ---------------------------------------------------------------------------
//
CSTUNTimerUser::~CSTUNTimerUser()
    {
    StopTimer();
    }

// ---------------------------------------------------------------------------
// CSTUNTimerUser::StartTimer
// ---------------------------------------------------------------------------
//
void CSTUNTimerUser::StartTimer( TInt aDuration )
    {
    __STUNTURNCLIENT_INT1( "CSTUNTimerUser::StartTimer, duration = ", aDuration )
    __STUN_ASSERT_RETURN( !iIsRunning, KErrNotReady );

    TCallBack callback( TimerExpired, this );
    iTimerEntry.Set( callback );

    iDeltaTimer.Queue( TimerDuration( aDuration ), iTimerEntry );
    iIsRunning = ETrue;
    }

// -----------------------------------------------------------------------------
// CSTUNTimerUser::StopTimer
// -----------------------------------------------------------------------------
//
void CSTUNTimerUser::StopTimer()
    {
    __STUNTURNCLIENT( "CSTUNTimerUser::StopTimer" )
    iDeltaTimer.Remove( iTimerEntry );
    iIsRunning = EFalse;
    }

// -----------------------------------------------------------------------------
// CSTUNTimerUser::TimerExpired
// -----------------------------------------------------------------------------
//
TInt CSTUNTimerUser::TimerExpired( TAny *aPtr )
    {
    __STUNTURNCLIENT( "CSTUNTimerUser::TimerExpired" )
    __STUN_ASSERT_RETURN_VALUE( aPtr, KErrArgument );

    CSTUNTimerUser* handler = reinterpret_cast<CSTUNTimerUser*>( aPtr );
    handler->iIsRunning = EFalse;

    TRAPD( err, handler->TimerExpiredL() );
    
    //Even if TimerExpiredL made an asynchronous callback, the actual callback
    //function has not been called yet, so application can't have deleted
    //handler yet.
    //Binding must be implemented so that it if has deleted the transaction,
    //TimerExpiredL will not leave after that!
    if ( err != KErrNone )
        {
        handler->LeaveFromTimerExpired( err );
        }

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CSTUNTimerUser::DeltaTimer
// -----------------------------------------------------------------------------
//
CDeltaTimer& CSTUNTimerUser::DeltaTimer()
    {
    return iDeltaTimer;
    }
    
// -----------------------------------------------------------------------------
// CSTUNTimerUser::IsRunning
// -----------------------------------------------------------------------------
//
TBool CSTUNTimerUser::IsRunning() const
    {
    __STUNTURNCLIENT_INT1( "CSTUNTimerUser::IsRunning, isRunning = ", iIsRunning )
    return iIsRunning;
    }

// -----------------------------------------------------------------------------
// CSTUNTimerUser::TimerDuration
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds32 CSTUNTimerUser::TimerDuration( TInt aMilliseconds )
    {
    __STUNTURNCLIENT( "CSTUNTimerUser::TimerDuration" )
    const TInt KMicrosecInMillisec = 1000;
    const TInt KMaxMilliseconds = KMaxTInt32 / KMicrosecInMillisec;

    __ASSERT_DEBUG( aMilliseconds <= KMaxMilliseconds,
                    User::Panic( _L( "STUN:Timer overflow" ), KErrOverflow ) );
    __ASSERT_ALWAYS( aMilliseconds <= KMaxMilliseconds,
                     aMilliseconds = KMaxMilliseconds );

    TTimeIntervalMicroSeconds32 microSec( aMilliseconds * KMicrosecInMillisec );
    return microSec;
    }
