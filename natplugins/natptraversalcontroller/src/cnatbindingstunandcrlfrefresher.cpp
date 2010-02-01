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


#include "natfwstunbinding.h"
#include "cnatbindingstunandcrlfrefresher.h"
#include "cudpcrlfsender.h"
#include "cnatbinding.h"

// -----------------------------------------------------------------------------
// CNATBindingSTUNAndCRLFRefresher::NewL
// -----------------------------------------------------------------------------
//
CNATBindingSTUNAndCRLFRefresher* CNATBindingSTUNAndCRLFRefresher::NewL(
    CDeltaTimer& aDeltaTimer,
    TInt aRefreshInterval,
    MNATBindingRefresherObserver& aObserver,
    CNATBinding& aNATBinding,
    RSocket& aSocket,
    const TInetAddr& aAddr,
    MNATTraversalSocketManager& aSocketManager)
    {
    CNATBindingSTUNAndCRLFRefresher* self =
        CNATBindingSTUNAndCRLFRefresher::NewLC(
            aDeltaTimer,aRefreshInterval,aObserver,aNATBinding,aSocket,
            aAddr,aSocketManager);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATBindingSTUNAndCRLFRefresher::NewLC
// -----------------------------------------------------------------------------
//
CNATBindingSTUNAndCRLFRefresher* CNATBindingSTUNAndCRLFRefresher::NewLC(
    CDeltaTimer& aDeltaTimer,
    TInt aRefreshInterval,
    MNATBindingRefresherObserver& aObserver,
    CNATBinding& aNATBinding,
    RSocket& aSocket,
    const TInetAddr& aAddr,
    MNATTraversalSocketManager& aSocketManager)
    {
    CNATBindingSTUNAndCRLFRefresher* self =
        new(ELeave)CNATBindingSTUNAndCRLFRefresher(aDeltaTimer,
                                                   aRefreshInterval,
                                                   aObserver,
                                                   aNATBinding);
    CleanupStack::PushL(self);
    self->ConstructL(aSocket,aAddr,aSocketManager);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATBindingSTUNAndCRLFRefresher::CNATBindingSTUNAndCRLFRefresher
// -----------------------------------------------------------------------------
//
CNATBindingSTUNAndCRLFRefresher::CNATBindingSTUNAndCRLFRefresher(
    CDeltaTimer& aDeltaTimer,
    TInt aRefreshInterval,
    MNATBindingRefresherObserver& aObserver,
    CNATBinding& aNATBinding)
    : CNATBindingSTUNRefresher(aDeltaTimer,aRefreshInterval,
                               aObserver,aNATBinding)
    {
    }

// -----------------------------------------------------------------------------
// CNATBindingSTUNAndCRLFRefresher::ConstructL
// -----------------------------------------------------------------------------
//
void CNATBindingSTUNAndCRLFRefresher::ConstructL(
    RSocket& aSocket,
    const TInetAddr& aAddr,
    MNATTraversalSocketManager& aSocketManager)
    {
    iCRLFSender = new(ELeave)CUdpCRLFSender(aSocket,aAddr,*this,aSocketManager);
    }

// -----------------------------------------------------------------------------
// CNATBindingSTUNAndCRLFRefresher::~CNATBindingSTUNAndCRLFRefresher
// -----------------------------------------------------------------------------
//
CNATBindingSTUNAndCRLFRefresher::~CNATBindingSTUNAndCRLFRefresher()
    {
    delete iCRLFSender;
    }

// -----------------------------------------------------------------------------
// CNATBindingSTUNAndCRLFRefresher::NATBindingRefreshComplete
// From MNATBindingRefresherObserver
// -----------------------------------------------------------------------------
//
void CNATBindingSTUNAndCRLFRefresher::NATBindingRefreshComplete(
    TInt aCompletionCode)
    {
    if (aCompletionCode == KErrNone)
        {
        CSTUNBinding* stunBinding = iNATBinding.Binding();
        if (stunBinding)
            {
            TRAPD(err,iNATBinding.SendSTUNRequestL())
            if (err)
                {
                iObserver.NATBindingRefreshComplete(err);
                }
            }
        else
            {
            iObserver.NATBindingRefreshComplete(KErrNotFound);
            }
        }
    else
        {
        iObserver.NATBindingRefreshComplete(aCompletionCode);
        }
    }

// -----------------------------------------------------------------------------
// CNATBindingSTUNAndCRLFRefresher::RefreshTimerExpiredL
// From CNATBindingRefresher
// -----------------------------------------------------------------------------
//
void CNATBindingSTUNAndCRLFRefresher::RefreshTimerExpiredL()
    {
    // First send the CRLF burst and when that completes
    // send the STUN request (see NATBindingRefreshComplete).
    iCRLFSender->Cancel();
    iCRLFSender->CCRLFSender::Send();
    }

// -----------------------------------------------------------------------------
// CNATBindingSTUNAndCRLFRefresher::CancelRefresh
// From CNATBindingRefresher
// -----------------------------------------------------------------------------
//
void CNATBindingSTUNAndCRLFRefresher::CancelRefresh()
    {
    iCRLFSender->Cancel();

    CSTUNBinding* stunBinding = iNATBinding.Binding();
    if (stunBinding)
        {
        stunBinding->CancelRequest();
        }
    }
