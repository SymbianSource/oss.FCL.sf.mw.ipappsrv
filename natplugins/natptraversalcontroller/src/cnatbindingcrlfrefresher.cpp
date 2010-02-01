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


#include "cnatbindingcrlfrefresher.h"
#include "ccrlfsender.h"

const TInt KMicroSecondsInSecond = 1000000;

// -----------------------------------------------------------------------------
// CNATBindingCRLFRefresher::CNATBindingCRLFRefresher
// -----------------------------------------------------------------------------
//
CNATBindingCRLFRefresher::CNATBindingCRLFRefresher(
    CDeltaTimer& aDeltaTimer,
    TInt aRefreshInterval,
    MNATBindingRefresherObserver& aObserver,
    CCRLFSender* aSender)
    : CNATBindingRefresher(aDeltaTimer,aObserver),
      iRefreshInterval(aRefreshInterval),
      iSender(aSender)
    {
    }

// -----------------------------------------------------------------------------
// CNATBindingCRLFRefresher::~CNATBindingCRLFRefresher
// -----------------------------------------------------------------------------
//
CNATBindingCRLFRefresher::~CNATBindingCRLFRefresher()
    {
    delete iSender;
    }

// -----------------------------------------------------------------------------
// CNATBindingCRLFRefresher::PersistentConnectionRequired
// -----------------------------------------------------------------------------
//
TBool CNATBindingCRLFRefresher::PersistentConnectionRequired() const
    {
    return iSender->PersistentConnectionRequired();
    }

// -----------------------------------------------------------------------------
// CNATBindingCRLFRefresher::HasSocket
// -----------------------------------------------------------------------------
//
TBool CNATBindingCRLFRefresher::HasSocket(const RSocket& aSocket) const
    {
    return iSender->HasSocket(aSocket);
    }

// -----------------------------------------------------------------------------
// CNATBindingCRLFRefresher::HasSocket
// -----------------------------------------------------------------------------
//
TBool CNATBindingCRLFRefresher::HasSocket(const CSecureSocket& aSocket) const
    {
    return iSender->HasSocket(aSocket);
    }

// -----------------------------------------------------------------------------
// CNATBindingCRLFRefresher::RefreshInterval
// -----------------------------------------------------------------------------
//
TInt CNATBindingCRLFRefresher::RefreshInterval()
    {
    return (iRefreshInterval*KMicroSecondsInSecond);
    }

// -----------------------------------------------------------------------------
// CNATBindingCRLFRefresher::RefreshTimerExpiredL
// -----------------------------------------------------------------------------
//
void CNATBindingCRLFRefresher::RefreshTimerExpiredL()
    {
    iSender->Cancel();
    iSender->Send();
    }

// -----------------------------------------------------------------------------
// CNATBindingCRLFRefresher::CancelRefresh
// -----------------------------------------------------------------------------
//
void CNATBindingCRLFRefresher::CancelRefresh()
    {
    iSender->Cancel();
    }
