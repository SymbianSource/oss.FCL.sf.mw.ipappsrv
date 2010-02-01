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
    }

// ---------------------------------------------------------------------------
// CSTUNTimerUser::StartTimer
// ---------------------------------------------------------------------------
//
void CSTUNTimerUser::StartTimer( TInt aDuration )
    {


    }

// -----------------------------------------------------------------------------
// CSTUNTimerUser::StopTimer
// -----------------------------------------------------------------------------
//
void CSTUNTimerUser::StopTimer()
    {
    }

// -----------------------------------------------------------------------------
// CSTUNTimerUser::TimerExpired
// -----------------------------------------------------------------------------
//
TInt CSTUNTimerUser::TimerExpired( TAny *aPtr )
    {

    }

// -----------------------------------------------------------------------------
// CSTUNTimerUser::DeltaTimer
// -----------------------------------------------------------------------------
//
CDeltaTimer& CSTUNTimerUser::DeltaTimer()
    {
    }
    
// -----------------------------------------------------------------------------
// CSTUNTimerUser::IsRunning
// -----------------------------------------------------------------------------
//
TBool CSTUNTimerUser::IsRunning() const
    {
    }

// -----------------------------------------------------------------------------
// CSTUNTimerUser::TimerDuration
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds32 CSTUNTimerUser::TimerDuration( TInt aMilliseconds )
    {
    }
