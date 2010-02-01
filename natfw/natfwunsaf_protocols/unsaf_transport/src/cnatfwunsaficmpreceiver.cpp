/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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




#include "cnatfwunsaficmpreceiver.h"
#include "cnatfwunsaficmpv4receiver.h"
#include "cnatfwunsaficmpv6receiver.h"


// -----------------------------------------------------------------------------
// CIcmpReceiver::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CIcmpReceiver* CIcmpReceiver::NewL( MIcmpErrorObserver& aObserver,
                                             RSocketServ& aServer )
    {
    CIcmpReceiver* self = new ( ELeave ) CIcmpReceiver();
    CleanupStack::PushL( self );
    self->ConstructL( aObserver, aServer );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CIcmpReceiver::CIcmpReceiver
// -----------------------------------------------------------------------------
//
CIcmpReceiver::CIcmpReceiver()
    {
    }

// -----------------------------------------------------------------------------
// CIcmpReceiver::ConstructL
// -----------------------------------------------------------------------------
//
void CIcmpReceiver::ConstructL( MIcmpErrorObserver& aObserver,
                                RSocketServ& aServer )
    {
    iIcmpV4Receiver = CIcmpV4Receiver::NewL( aObserver, aServer );
    iIcmpV6Receiver = CIcmpV6Receiver::NewL( aObserver, aServer );
    }

// -----------------------------------------------------------------------------
// CIcmpReceiver::~CIcmpReceiver
// -----------------------------------------------------------------------------
//
CIcmpReceiver::~CIcmpReceiver()
    {
    delete iIcmpV4Receiver;
    delete iIcmpV6Receiver;
    }
