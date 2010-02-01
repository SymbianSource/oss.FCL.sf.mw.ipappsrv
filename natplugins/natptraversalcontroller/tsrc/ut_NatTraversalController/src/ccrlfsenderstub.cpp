/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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


#include "CCRLFSenderStub.h"

// -----------------------------------------------------------------------------
// CCRLFSenderStub::CCRLFSenderStub
// -----------------------------------------------------------------------------
//
CCRLFSenderStub::CCRLFSenderStub(
    MNATBindingRefresherObserver& aObserver )
    : CCRLFSender( aObserver )
    {
    }

// -----------------------------------------------------------------------------
// CCRLFSenderStub::~CCRLFSenderStub
// -----------------------------------------------------------------------------
//
CCRLFSenderStub::~CCRLFSenderStub()
    {
    Cancel();
    }

// -----------------------------------------------------------------------------
// CCRLFSenderStub::HasSocket
// -----------------------------------------------------------------------------
//
TBool CCRLFSenderStub::HasSocket(const RSocket& /*aSocket*/) const
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CCRLFSenderStub::DoCancel
// From CActive
// -----------------------------------------------------------------------------
//
void CCRLFSenderStub::DoCancel()
    {
    if (iStatus == KRequestPending)
        {
        TRequestStatus* status = &iStatus;
        User::RequestComplete(status,KErrCancel);
        }
    }

// -----------------------------------------------------------------------------
// CCRLFSenderStub::Send
// -----------------------------------------------------------------------------
//
void CCRLFSenderStub::Send(const TDesC8& /*aData*/)
    {
    iStatus = KRequestPending;
    }

// End of File
