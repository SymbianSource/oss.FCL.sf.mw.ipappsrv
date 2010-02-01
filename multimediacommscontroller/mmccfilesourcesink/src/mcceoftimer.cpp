/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
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




// INCLUDES FILES
#include "mccfilesourcelogs.h"
#include "mcceoftimer.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CMccEofTimer::NewL
// ---------------------------------------------------------------------------
//
CMccEofTimer* CMccEofTimer::NewL(MMccEofTimerObserver* aOwner)
    {
    CMccEofTimer* self = new(ELeave) CMccEofTimer(aOwner);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CMccEofTimer::CMccEofTimer
// ---------------------------------------------------------------------------
//
CMccEofTimer::CMccEofTimer(MMccEofTimerObserver* aOwner)
: CTimer( CActive::EPriorityStandard),
  iOwner( aOwner )
    {
    CActiveScheduler::Add(this);
    }
        
// ---------------------------------------------------------------------------
// CMccEofTimer::~CMccEofTimer
// ---------------------------------------------------------------------------
//
CMccEofTimer::~CMccEofTimer()
    {
    }
        
// ---------------------------------------------------------------------------
// CMccEofTimer::RunL
// ---------------------------------------------------------------------------
//
void CMccEofTimer::RunL()
    {
    TRAPD(err, iOwner->EofTimerExpiredL() );
    if ( err )
        {
        __FILESOURCE_CONTROLL_INT1("CMccEofTimer::RunL, EofTimerExpiredL left with ", err )
        }
    }
