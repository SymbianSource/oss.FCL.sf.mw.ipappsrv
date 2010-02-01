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
* Description: 
*
*/


#include "cnatbindingrefresher.h"
#include "mnatbindingrefresherobserver.h"


// -----------------------------------------------------------------------------
// CNATBindingRefresher::CNATBindingRefresher
// -----------------------------------------------------------------------------
//
CNATBindingRefresher::CNATBindingRefresher(
    CDeltaTimer& aDeltaTimer,
    MNATBindingRefresherObserver& aObserver)
    : iObserver(aObserver),
      iDeltaTimer(aDeltaTimer),
      iDeltaTimerCallBack(RefreshTimerExpired,this),
      iDeltaTimerEntry(iDeltaTimerCallBack),
      iIsRefreshed(EFalse),
      iTimerRunning(EFalse)
    {
    }

// -----------------------------------------------------------------------------
// CNATBindingRefresher::~CNATBindingRefresher
// -----------------------------------------------------------------------------
//
CNATBindingRefresher::~CNATBindingRefresher()
    {
    iDeltaTimer.Remove(iDeltaTimerEntry);
    }

// -----------------------------------------------------------------------------
// CNATBindingRefresher::PersistentConnectionRequired
// -----------------------------------------------------------------------------
//
TBool CNATBindingRefresher::PersistentConnectionRequired() const
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CNATBindingRefresher::HasSocket
// -----------------------------------------------------------------------------
//
TBool CNATBindingRefresher::HasSocket(const CSecureSocket& /*aSocket*/) const
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CNATBindingRefresher::SetRefresh
// -----------------------------------------------------------------------------
//
void CNATBindingRefresher::SetRefresh(TBool aRefresh)
    {
    iIsRefreshed = aRefresh;
    if (aRefresh)
        {
        StartTimer();
        }
    else
        {
        iDeltaTimer.Remove(iDeltaTimerEntry);
        CancelRefresh();
        iTimerRunning = EFalse;
        }
    }

// -----------------------------------------------------------------------------
// CNATBindingRefresher::ContinueRefreshing
// -----------------------------------------------------------------------------
//
void CNATBindingRefresher::ContinueRefreshing()
    {
    if (iIsRefreshed)
        {
        StartTimer();
        }
    }

// -----------------------------------------------------------------------------
// CNATBindingRefresher::IsRefreshed
// -----------------------------------------------------------------------------
//
TBool CNATBindingRefresher::IsRefreshed() const
    {
    return iIsRefreshed;
    }

// -----------------------------------------------------------------------------
// CNATBindingRefresher::RefreshTimerExpired
// A callback for CDeltaTimer
// -----------------------------------------------------------------------------
//
TInt CNATBindingRefresher::RefreshTimerExpired(TAny* aPtr)
    {
    _LIT(KPanicCategory,"NAT Traversal Controller");
    __ASSERT_ALWAYS(aPtr,User::Panic(KPanicCategory,KErrBadHandle));
    CNATBindingRefresher* self = reinterpret_cast<CNATBindingRefresher*>(aPtr);
    self->RefreshNow();
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CNATBindingRefresher::StartTimer
// -----------------------------------------------------------------------------
//
void CNATBindingRefresher::StartTimer()
    {
    if (!iTimerRunning)
        {
        iDeltaTimer.Remove(iDeltaTimerEntry);
        TTimeIntervalMicroSeconds32 interval(RefreshInterval());
        iDeltaTimer.Queue(interval,iDeltaTimerEntry);
        iTimerRunning = ETrue;
        }
    }

// -----------------------------------------------------------------------------
// CNATBindingRefresher::RefreshNow
// -----------------------------------------------------------------------------
//
void CNATBindingRefresher::RefreshNow()
    {
    iTimerRunning = EFalse;
    TRAPD(err, RefreshTimerExpiredL());
    if (err)
        {
        iObserver.NATBindingRefreshComplete(err);
        }
    }
