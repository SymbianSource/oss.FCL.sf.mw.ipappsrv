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


#include "tnattraversalpendingrequest.h"
#include "sipnattraversalrequestobserver.h"
#include "mnattraversalpendingrequestcontainer.h"
#include "nattraversallog.h"

// -----------------------------------------------------------------------------
// TNATTraversalPendingRequest::TNATTraversalPendingRequest
// -----------------------------------------------------------------------------
//
TNATTraversalPendingRequest::TNATTraversalPendingRequest(
    const TUint32& aRequestId,
    MSIPNATTraversalRequestObserver* aObserver,
    MNATTraversalPendingRequestContainer& aContainer)
 : iRequestId(aRequestId),
   iObserver(aObserver),
   iContainer(aContainer),
   iDeltaTimerCallBack(TNATTraversalPendingRequest::TimerExpired,this),
   iDeltaTimerEntry(iDeltaTimerCallBack)
    {
    }

// -----------------------------------------------------------------------------
// TNATTraversalPendingRequest::RequestId
// -----------------------------------------------------------------------------
//
const TUint32& TNATTraversalPendingRequest::RequestId() const
    {
    return iRequestId;
    }

// -----------------------------------------------------------------------------
// TNATTraversalPendingRequest::DeltaTimerEntry
// -----------------------------------------------------------------------------
//
TDeltaTimerEntry& TNATTraversalPendingRequest::DeltaTimerEntry()
    {
    // If the original instance was created from stack and then copied,
    // the callback for the copy is not valid. Setting it correctly.
    TCallBack callback(TNATTraversalPendingRequest::TimerExpired,this);
    iDeltaTimerEntry.Set(callback);
    return iDeltaTimerEntry;
    }

// -----------------------------------------------------------------------------
// TNATTraversalPendingRequest::CompleteRequest
// -----------------------------------------------------------------------------
//
void TNATTraversalPendingRequest::CompleteRequest()
    {
    if (iObserver)
        {
        iObserver->RequestComplete(iRequestId,KErrNone);
        }
    }

// -----------------------------------------------------------------------------
// TNATTraversalPendingRequest::TimerExpired
// A callback for CDeltaTimer
// -----------------------------------------------------------------------------
//
TInt TNATTraversalPendingRequest::TimerExpired(TAny* aPtr)
    {
    NATTRAVERSAL_LOG("TNATTraversalPendingRequest::TimerExpired")
    _LIT(KPanicCategory,"NAT Traversal Controller");
    __ASSERT_ALWAYS(aPtr, User::Panic(KPanicCategory,KErrBadHandle));
    TNATTraversalPendingRequest* self =
        reinterpret_cast<TNATTraversalPendingRequest*>(aPtr);
    self->RequestCompleted();
    return ETrue;
    }

// -----------------------------------------------------------------------------
// TNATTraversalPendingRequest::RequestOrder
// -----------------------------------------------------------------------------
//
TInt TNATTraversalPendingRequest::RequestOrder(
    const TNATTraversalPendingRequest& aFirst,
    const TNATTraversalPendingRequest& aSecond)
    {
    if (aFirst.RequestId() < aSecond.RequestId())
        {
        return -1;
        }
    return (aFirst.RequestId() > aSecond.RequestId());
    }

// -----------------------------------------------------------------------------
// TNATTraversalPendingRequest::RequestCompleted
// -----------------------------------------------------------------------------
//
void TNATTraversalPendingRequest::RequestCompleted()
    {
    iContainer.RequestCompleted(iRequestId);
    }
