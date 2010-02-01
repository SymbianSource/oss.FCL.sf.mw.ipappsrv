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


#include "cudpcrlfsender.h"
#include "nattraversalsocketmanager.h"
#include "mnatbindingrefresherobserver.h"

// -----------------------------------------------------------------------------
// CUdpCRLFSender::CUdpCRLFSender
// -----------------------------------------------------------------------------
//
CUdpCRLFSender::CUdpCRLFSender(
    RSocket& aSocket,
    const TInetAddr& aAddr,
    MNATBindingRefresherObserver& aObserver,
    MNATTraversalSocketManager& aSocketManager )
    : CCRLFSender( aObserver ),
      iSocket( aSocket ),
      iAddr( aAddr ),
      iSocketManager ( aSocketManager )
    {
    }

// -----------------------------------------------------------------------------
// CUdpCRLFSender::~CUdpCRLFSender
// -----------------------------------------------------------------------------
//
CUdpCRLFSender::~CUdpCRLFSender()
    {
    Cancel();
    iSocketManager.SendingCompleted(*this);
    }

// -----------------------------------------------------------------------------
// CUdpCRLFSender::PersistentConnectionRequired
// -----------------------------------------------------------------------------
//
TBool CUdpCRLFSender::PersistentConnectionRequired() const
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CUdpCRLFSender::HasSocket
// From CCRLFSender
// -----------------------------------------------------------------------------
//
TBool CUdpCRLFSender::HasSocket(const RSocket& aSocket) const
    {
    return (aSocket.SubSessionHandle() == iSocket.SubSessionHandle());
    }

// -----------------------------------------------------------------------------
// CUdpCRLFSender::Socket
// From MNATTraversalSocketUser
// -----------------------------------------------------------------------------
//
const RSocket& CUdpCRLFSender::Socket() const
    {
    return iSocket;
    }

// -----------------------------------------------------------------------------
// CUdpCRLFSender::SendingAllowed
// From MNATTraversalSocketUser
// -----------------------------------------------------------------------------
//
void CUdpCRLFSender::SendingAllowed()
    {
    iAllowedToSend = ETrue;
    CCRLFSender::Send();
    iAllowedToSend = EFalse;
    }

// -----------------------------------------------------------------------------
// CUdpCRLFSender::IsSending
// From MNATTraversalSocketUser
// -----------------------------------------------------------------------------
//
TBool CUdpCRLFSender::IsSending() const
    {
    return IsActive();
    }

// -----------------------------------------------------------------------------
// CUdpCRLFSender::CancelSending
// From MNATTraversalSocketUser
// -----------------------------------------------------------------------------
//
void CUdpCRLFSender::CancelSending()
    {
    Cancel();
    }

// -----------------------------------------------------------------------------
// CUdpCRLFSender::DoCancel
// From CActive
// -----------------------------------------------------------------------------
//
void CUdpCRLFSender::DoCancel()
    {
    iSocket.CancelSend();
    }

// -----------------------------------------------------------------------------
// CUdpCRLFSender::Send
// -----------------------------------------------------------------------------
//
void CUdpCRLFSender::Send(const TDesC8& aData)
    {
    iSocket.SendTo(aData,iAddr,0,iStatus);
    }

// -----------------------------------------------------------------------------
// CUdpCRLFSender::ReadyToSend
// From CCRLFSender
// -----------------------------------------------------------------------------
//
TBool CUdpCRLFSender::ReadyToSend()
    {
    if (iAllowedToSend)
        {
        return ETrue;
        }
    TBool addedToQueue = ETrue;
    TRAPD(err, addedToQueue = iSocketManager.AddToSendingQueueL(*this))
    if (err)
        {
        iObserver.NATBindingRefreshComplete(err);
        }
    return !addedToQueue;
    }

// -----------------------------------------------------------------------------
// CUdpCRLFSender::SendingCompleted
// From CCRLFSender
// -----------------------------------------------------------------------------
//
void CUdpCRLFSender::SendingCompleted()
    {
    iSocketManager.SendingCompleted(*this);
    }

// End of File
