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


#include "ctlscrlfsender.h"

// -----------------------------------------------------------------------------
// CTlsCRLFSender::CTlsCRLFSender
// -----------------------------------------------------------------------------
//
CTlsCRLFSender::CTlsCRLFSender(
    CSecureSocket& aSocket,
    MNATBindingRefresherObserver& aObserver )
    : CCRLFSender( aObserver ),
      iSocket( aSocket )
    {
    }

// -----------------------------------------------------------------------------
// CTlsCRLFSender::~CTlsCRLFSender
// -----------------------------------------------------------------------------
//
CTlsCRLFSender::~CTlsCRLFSender()
    {
    Cancel();
    }

// -----------------------------------------------------------------------------
// CTlsCRLFSender::HasSocket
// -----------------------------------------------------------------------------
//
TBool CTlsCRLFSender::HasSocket(const CSecureSocket& aSocket) const
    {
    return (&iSocket == &aSocket);
    }

// -----------------------------------------------------------------------------
// CTlsCRLFSender::DoCancel
// From CActive
// -----------------------------------------------------------------------------
//
void CTlsCRLFSender::DoCancel()
    {
    iSocket.CancelSend();
    }

// -----------------------------------------------------------------------------
// CTlsCRLFSender::Send
// -----------------------------------------------------------------------------
//
void CTlsCRLFSender::Send( const TDesC8& aData )
    {
    iSocket.Send(aData,iStatus);
    }

// End of File
