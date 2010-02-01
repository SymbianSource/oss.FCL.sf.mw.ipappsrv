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




#ifndef C_CICMPV4RECEIVER_H
#define C_CICMPV4RECEIVER_H

// INCLUDES
#include <e32base.h>
#include <in_sock.h>

// FORWARD DECLARATIONS
class MIcmpErrorObserver;

// CLASS DECLARATION
NONSHARABLE_CLASS( CIcmpV4Receiver ): public CActive
    {
public: // Constructors and destructor

    static CIcmpV4Receiver* NewL( MIcmpErrorObserver& aObserver,
                                  RSocketServ& aServer );

    ~CIcmpV4Receiver();

public: // From CActive

    void RunL();

    void DoCancel();

private: // Constructors, for internal use

    CIcmpV4Receiver( MIcmpErrorObserver& aObserver, RSocketServ& aServer );

    void ConstructL();

private: // New functions, for internal use

    void Receive();

    void CheckError();

private: // Data

    RSocket iSocket;

    RSocketServ& iServer;

    //Buffer where data is received from the socket
    TBuf8<64> iData;

    TInetAddr iAddress;

    MIcmpErrorObserver& iObserver;

private: // For testing purposes

    friend class UT_CNATFWUNSAFIcmpRecv;
    };

#endif // end of C_CICMPV4RECEIVER_H


