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


#include <e32math.h>
#include "natfwstunbinding.h"
#include "cnatbindingstunrefresher.h"
#include "cnatbinding.h"

const TInt KRefreshBaseValue = 900000; // microseconds
const TInt KRefreshVariationRange = 200000; // microseconds

// -----------------------------------------------------------------------------
// CNATBindingSTUNRefresher::CNATBindingSTUNRefresher
// -----------------------------------------------------------------------------
//
CNATBindingSTUNRefresher::CNATBindingSTUNRefresher(
    CDeltaTimer& aDeltaTimer,
    TInt aRefreshInterval,
    MNATBindingRefresherObserver& aObserver,
    CNATBinding& aNATBinding)
    : CNATBindingRefresher(aDeltaTimer,aObserver),
      iNATBinding(aNATBinding),
      iRefreshInterval(aRefreshInterval)
    {
    TTime time;
    time.HomeTime();
    iRandomSeed = time.Int64();
    }

// -----------------------------------------------------------------------------
// CNATBindingSTUNRefresher::~CNATBindingSTUNRefresher
// -----------------------------------------------------------------------------
//
CNATBindingSTUNRefresher::~CNATBindingSTUNRefresher()
    {
    CSTUNBinding* stunBinding = iNATBinding.Binding();
    if (stunBinding)
        {
        stunBinding->CancelRequest();
        }
    }

// -----------------------------------------------------------------------------
// CNATBindingSTUNRefresher::HasSocket
// -----------------------------------------------------------------------------
//
TBool CNATBindingSTUNRefresher::HasSocket(const RSocket& aSocket) const
    {
    CSTUNBinding* stunBinding = iNATBinding.Binding();
    return stunBinding &&
           (aSocket.SubSessionHandle() ==
            stunBinding->Socket().SubSessionHandle());
    }

// -----------------------------------------------------------------------------
// CNATBindingSTUNRefresher::RefreshInterval
// -----------------------------------------------------------------------------
//
TInt CNATBindingSTUNRefresher::RefreshInterval()
    {
    TInt interval = KErrUnderflow;
    if (iRefreshInterval > 0)
        {
        const TInt minValue = iRefreshInterval*KRefreshBaseValue;
        const TInt variantPart = iRefreshInterval*KRefreshVariationRange;
        const TInt rand = Math::Rand(iRandomSeed);
        interval = (minValue+(rand%variantPart));
        }
    return interval;
    }

// -----------------------------------------------------------------------------
// CNATBindingSTUNRefresher::RefreshTimerExpiredL
// -----------------------------------------------------------------------------
//
void CNATBindingSTUNRefresher::RefreshTimerExpiredL()
    {
    __ASSERT_ALWAYS(iNATBinding.Binding(), User::Leave(KErrNotFound));

    iNATBinding.SendSTUNRequestL();
    }

// -----------------------------------------------------------------------------
// CNATBindingSTUNRefresher::CancelRefresh
// -----------------------------------------------------------------------------
//
void CNATBindingSTUNRefresher::CancelRefresh()
    {
    CSTUNBinding* stunBinding = iNATBinding.Binding();
    if (stunBinding)
        {
        stunBinding->CancelRequest();
        }
    }
