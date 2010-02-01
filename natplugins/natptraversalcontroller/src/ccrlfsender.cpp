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


#include "ccrlfsender.h"
#include "mnatbindingrefresherobserver.h"

_LIT8(KDoubleCRLF,"\r\n\r\n");

// -----------------------------------------------------------------------------
// CCRLFSender::CCRLFSender
// -----------------------------------------------------------------------------
//
CCRLFSender::CCRLFSender( MNATBindingRefresherObserver& aObserver )
    : CActive( CActive::EPriorityStandard ),
      iObserver( aObserver )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CCRLFSender::~CCRLFSender
// -----------------------------------------------------------------------------
//
CCRLFSender::~CCRLFSender()
    {
    }

// -----------------------------------------------------------------------------
// CCRLFSender::PersistentConnectionRequired
// -----------------------------------------------------------------------------
//
TBool CCRLFSender::PersistentConnectionRequired() const
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCRLFSender::HasSocket
// -----------------------------------------------------------------------------
//
TBool CCRLFSender::HasSocket(const RSocket& /*aSocket*/) const
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CCRLFSender::HasSocket
// -----------------------------------------------------------------------------
//
TBool CCRLFSender::HasSocket(const CSecureSocket& /*aSocket*/) const
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CCRLFSender::Send
// -----------------------------------------------------------------------------
//
void CCRLFSender::Send()
    {
    if (ReadyToSend())
        {
        Send(KDoubleCRLF);
        SetActive();
        }
    }

// -----------------------------------------------------------------------------
// CCRLFSender::RunL
// From CActive
// -----------------------------------------------------------------------------
//
void CCRLFSender::RunL()
    {
    SendingCompleted();
    iObserver.NATBindingRefreshComplete(iStatus.Int());
    }

// -----------------------------------------------------------------------------
// CCRLFSender::ReadyToSend
// -----------------------------------------------------------------------------
//
TBool CCRLFSender::ReadyToSend()
    {
    // Default implementation
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCRLFSender::SendingCompleted
// -----------------------------------------------------------------------------
//
void CCRLFSender::SendingCompleted()
    {
    // Default implementation empty
    }

// End of File
