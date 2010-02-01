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
#include "cnatbinding.h"
#include "sipnattraversalrequestobserver.h"
#include "sipnatbindingobserver.h"
#include "cnatbindingrefresher.h"
#include "nattraversalsocketmanager.h"

// -----------------------------------------------------------------------------
// CNATBinding::NewL
// -----------------------------------------------------------------------------
//
CNATBinding* CNATBinding::NewL(
    TUint32 aInitialRequestId,
    RSocket& aSTUNSocket,
    const TInetAddr& aLocalAddr,
    MSIPNATTraversalRequestObserver& aInitialRequestObserver,
    MSIPNATBindingObserver& aBindingObserver,
    MNATTraversalSocketManager& aSocketManager)
    {
    CNATBinding* self =
        CNATBinding::NewLC(aInitialRequestId,aSTUNSocket,aLocalAddr,
                           aInitialRequestObserver,aBindingObserver,
                           aSocketManager);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATBinding::NewLC
// -----------------------------------------------------------------------------
//
CNATBinding* CNATBinding::NewLC(
    TUint32 aInitialRequestId,
    RSocket& aSTUNSocket,
    const TInetAddr& aLocalAddr,
    MSIPNATTraversalRequestObserver& aInitialRequestObserver,
    MSIPNATBindingObserver& aBindingObserver,
    MNATTraversalSocketManager& aSocketManager)
    {
    CNATBinding* self =
        new(ELeave)CNATBinding(aInitialRequestId,aSTUNSocket,aLocalAddr,
                               aInitialRequestObserver,aBindingObserver,
                               aSocketManager);
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATBinding::CNATBinding
// -----------------------------------------------------------------------------
//
CNATBinding::CNATBinding(
    TUint32 aInitialRequestId,
    RSocket& aSTUNSocket,
    const TInetAddr& aLocalAddr,
    MSIPNATTraversalRequestObserver& aInitialRequestObserver,
    MSIPNATBindingObserver& aBindingObserver,
    MNATTraversalSocketManager& aSocketManager)
    : iInitialReqId(aInitialRequestId),
      iSTUNSocket(aSTUNSocket),
      iInitialReqObserver(aInitialRequestObserver),
      iBindingObserver(aBindingObserver),
      iSocketManager(aSocketManager),
      iPublicAddr(aLocalAddr)
    {
    }

// -----------------------------------------------------------------------------
// CNATBinding::~CNATBinding
// -----------------------------------------------------------------------------
//
CNATBinding::~CNATBinding()
    {
    iSocketManager.SendingCompleted(*this); // Make sure no requests are pending
    delete iRefresher;
    delete iBinding;
    }

// -----------------------------------------------------------------------------
// CNATBinding::PersistentConnectionRequired
// -----------------------------------------------------------------------------
//
TBool CNATBinding::PersistentConnectionRequired() const
    {
    TBool persistentConnection = EFalse;
    if (iRefresher)
        {
        persistentConnection = iRefresher->PersistentConnectionRequired();
        }
    return persistentConnection;
    }

// -----------------------------------------------------------------------------
// CNATBinding::STUNSocket
// -----------------------------------------------------------------------------
//
RSocket& CNATBinding::STUNSocket()
    {
    return iSTUNSocket;
    }

// -----------------------------------------------------------------------------
// CNATBinding::SetBinding
// -----------------------------------------------------------------------------
//
void CNATBinding::SetBinding(CSTUNBinding* aBinding)
    {
    delete iBinding;
    iBinding = aBinding;
    }

// -----------------------------------------------------------------------------
// CNATBinding::Binding
// -----------------------------------------------------------------------------
//
CSTUNBinding* CNATBinding::Binding()
    {
    return iBinding;
    }

// -----------------------------------------------------------------------------
// CNATBinding::SendSTUNRequestL
// -----------------------------------------------------------------------------
//
void CNATBinding::SendSTUNRequestL()
    {
    __ASSERT_ALWAYS(iBinding, User::Leave(KErrNotReady));
    if (!iSocketManager.AddToSendingQueueL(*this))
        {
        iBinding->SendRequestL();
        iIsSending = ETrue;
        }
    }

// -----------------------------------------------------------------------------
// CNATBinding::SetBindingFailed
// -----------------------------------------------------------------------------
//
void CNATBinding::SetBindingFailed(TInt aError)
    {
    SendingCompleted();
    delete iBinding;
    iBinding = NULL;
    if (iAddressResolved)
        {
        iAddressResolved = EFalse;
        iBindingObserver.FlowFailure(aError);
        }
    else
        {
        iInitialReqObserver.RequestComplete(iInitialReqId,KErrNone);
        }
    }

// -----------------------------------------------------------------------------
// CNATBinding::SetPublicAddr
// -----------------------------------------------------------------------------
//
TBool CNATBinding::SetPublicAddr(const TInetAddr& aPublicAddr)
    {
    SendingCompleted();
    TBool removeBinding = EFalse;
    if (iAddressResolved)
        {
        removeBinding = !aPublicAddr.CmpAddr(iPublicAddr);
        }
    else
        {
        iAddressResolved = ETrue;
        TBool localAddrMatchesToPublicAddr = iPublicAddr.Match(aPublicAddr);
        iPublicAddr = aPublicAddr;
        if (localAddrMatchesToPublicAddr)
            {
            // We are not behind a NAT
            iInitialReqObserver.RequestComplete(iInitialReqId,KErrNone);
            }
        else
            {
            iInitialReqObserver.PublicAddrResolved(iInitialReqId,aPublicAddr);
            }
        }
    return removeBinding;
    }

// -----------------------------------------------------------------------------
// CNATBinding::SetRefresher
// -----------------------------------------------------------------------------
//
void CNATBinding::SetRefresher(CNATBindingRefresher* aRefresher)
    {
    delete iRefresher;
    iRefresher = aRefresher;
    }

// -----------------------------------------------------------------------------
// CNATBinding::Refresher
// -----------------------------------------------------------------------------
//
CNATBindingRefresher* CNATBinding::Refresher()
    {
    return iRefresher;
    }

// -----------------------------------------------------------------------------
// CNATBinding::InitialRequestId
// -----------------------------------------------------------------------------
//
TUint32 CNATBinding::InitialRequestId() const
    {
    return iInitialReqId;
    }

// -----------------------------------------------------------------------------
// CNATBinding::NATBindingObserver
// -----------------------------------------------------------------------------
//
MSIPNATBindingObserver& CNATBinding::NATBindingObserver()
    {
    return iBindingObserver;
    }

// -----------------------------------------------------------------------------
// CNATBinding::AddressResolved
// -----------------------------------------------------------------------------
//
TBool CNATBinding::AddressResolved() const
    {
    return iAddressResolved;
    }

// -----------------------------------------------------------------------------
// CNATBinding::PublicAddr
// -----------------------------------------------------------------------------
//
const TInetAddr& CNATBinding::PublicAddr() const
    {
    return iPublicAddr;
    }

// -----------------------------------------------------------------------------
// CNATBinding::HandleDataL
// -----------------------------------------------------------------------------
//
void CNATBinding::HandleDataL(
    const TDesC8& aData,
    const RSocket& aSocket,
    TBool& aConsumed)
    {
    aConsumed = EFalse;
    if (iBinding &&
        aSocket.SubSessionHandle() == iBinding->Socket().SubSessionHandle())
        {
        // Set a dummy remote address as the data is guaranteed not to be a
        // TURN indication and on return the address is useless to us.
        TInetAddr addr(KAFUnspec);
        iBinding->HandleDataL(aData,aConsumed,addr);
        }
    }

// -----------------------------------------------------------------------------
// CNATBinding::ContinueRefreshing
// -----------------------------------------------------------------------------
//
void CNATBinding::ContinueRefreshing()
    {
    if (iRefresher)
        {
        iRefresher->ContinueRefreshing();
        }
    }

// -----------------------------------------------------------------------------
// CNATBinding::NATBindingRefreshComplete
// From MNATBindingRefresherObserver
// -----------------------------------------------------------------------------
//
void CNATBinding::NATBindingRefreshComplete(TInt aCompletionCode)
    {
    if (aCompletionCode != KErrNone)
        {
        iBindingObserver.FlowFailure(aCompletionCode);
        }
    else
        {
        ContinueRefreshing();
        }
    }

// -----------------------------------------------------------------------------
// CNATBinding::Socket
// From MNATTraversalSocketUser
// -----------------------------------------------------------------------------
//
const RSocket& CNATBinding::Socket() const
    {
    return iSTUNSocket;
    }

// -----------------------------------------------------------------------------
// CNATBinding::SendingAllowed
// From MNATTraversalSocketUser
// -----------------------------------------------------------------------------
//
void CNATBinding::SendingAllowed()
    {
    if (iBinding)
        {
        TRAPD(err, iBinding->SendRequestL());
        if (err)
            {
            SetBindingFailed(err);
            }
        else
            {
            iIsSending = ETrue;
            }
        }
    }

// -----------------------------------------------------------------------------
// CNATBinding::IsSending
// From MNATTraversalSocketUser
// -----------------------------------------------------------------------------
//
TBool CNATBinding::IsSending() const
    {
    return iIsSending;
    }

// -----------------------------------------------------------------------------
// CNATBinding::CancelSending
// From MNATTraversalSocketUser
// -----------------------------------------------------------------------------
//
void CNATBinding::CancelSending()
    {
    if (iBinding)
        {
        iBinding->CancelRequest();
        }
    }

// -----------------------------------------------------------------------------
// CNATBinding::SendingCompleted
// -----------------------------------------------------------------------------
//
void CNATBinding::SendingCompleted()
    {
    iIsSending = EFalse;
    iSocketManager.SendingCompleted(*this);
    }
