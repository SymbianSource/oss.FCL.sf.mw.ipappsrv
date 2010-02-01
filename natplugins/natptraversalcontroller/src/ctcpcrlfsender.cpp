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


#include "ctcpcrlfsender.h"

// -----------------------------------------------------------------------------
// CTcpCRLFSender::CTcpCRLFSender
// -----------------------------------------------------------------------------
//
CTcpCRLFSender::CTcpCRLFSender(
    RSocket& aSocket,
    MNATBindingRefresherObserver& aObserver )
    : CCRLFSender( aObserver ),
      iSocket( aSocket )
    {
    }

// -----------------------------------------------------------------------------
// CTcpCRLFSender::~CTcpCRLFSender
// -----------------------------------------------------------------------------
//
CTcpCRLFSender::~CTcpCRLFSender()
    {
    Cancel();
    }

// -----------------------------------------------------------------------------
// CTcpCRLFSender::HasSocket
// -----------------------------------------------------------------------------
//
TBool CTcpCRLFSender::HasSocket(const RSocket& aSocket) const
    {
    return (aSocket.SubSessionHandle() == iSocket.SubSessionHandle());
    }

// -----------------------------------------------------------------------------
// CTcpCRLFSender::DoCancel
// From CActive
// -----------------------------------------------------------------------------
//
void CTcpCRLFSender::DoCancel()
    {
    iSocket.CancelSend();
    }

// -----------------------------------------------------------------------------
// CTcpCRLFSender::Send
// -----------------------------------------------------------------------------
//
void CTcpCRLFSender::Send( const TDesC8& aData )
    {
    iSocket.Send(aData,0,iStatus);
    }

// End of File
