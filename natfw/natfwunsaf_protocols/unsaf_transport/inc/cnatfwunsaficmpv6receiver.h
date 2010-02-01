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




#ifndef C_CICMPV6RECEIVER_H
#define C_CICMPV6RECEIVER_H

// INCLUDES
#include <e32base.h>
#include <in_sock.h>

// FORWARD DECLARATIONS
class MIcmpErrorObserver;

// CONSTANTS
const TInt KMaxUdpMessageSize = 1300;

// CLASS DECLARATION
NONSHARABLE_CLASS( CIcmpV6Receiver ): public CActive
    {
public: // Constructors and destructor

    static CIcmpV6Receiver* NewL( MIcmpErrorObserver& aObserver,
                                  RSocketServ& aServer );

    ~CIcmpV6Receiver();

public: // From CActive

    void RunL();

    void DoCancel();

private: // Constructors, for internal use

    CIcmpV6Receiver( MIcmpErrorObserver& aObserver, RSocketServ& aServer );

    void ConstructL();

private: // New functions, for internal use

    void Receive();

    void CheckError();

private: // Data

    RSocket iSocket;

    RSocketServ& iServer;

    TBuf8<KMaxUdpMessageSize> iData;

    TInetAddr iAddress;

    MIcmpErrorObserver& iObserver;

private: // For testing purposes

    friend class UT_CNATFWUNSAFIcmpRecv;
    };

#endif // end of C_CICMPV6RECEIVER_H


