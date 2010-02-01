/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    ICMPv4 receiver
*
*/




#ifndef C_CNCMICMPV4RECEIVER_H
#define C_CNCMICMPV4RECEIVER_H

// INCLUDES
#include <e32base.h>
#include <in_sock.h>

// FORWARD DECLARATIONS
class MNcmSessionObserver;
class CNcmIcmpSender;
class MNcmIcmpObserver;

/**
 *  Receives and handles ICMPv4 errors.
 *
 *  @lib connectionmux.lib
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CNcmIcmpV4Receiver ): public CActive
    {
    
    friend class UT_CNcmIcmpReceiver;
    
public: // Constructors and destructor

    /** 
     * Two-phased constructor
     * @param   aServer             Socket server
     * @param   aConnection         RConnection
     * @param   aObserver           ICMP error observer
     */
    static CNcmIcmpV4Receiver* NewL( RSocketServ& aServer,
        RConnection& aConnection, MNcmIcmpObserver& aObserver );

    virtual ~CNcmIcmpV4Receiver();

public: // From CActive

    void RunL();

    void DoCancel();

private: // Constructors, for internal use

    CNcmIcmpV4Receiver( RSocketServ& aServer,
        RConnection& aConnection, MNcmIcmpObserver& aObserver );

    void ConstructL();

private: // New functions, for internal use

    void Receive();

    void CheckError();

private: // Data

    /**
     * Socket server
     */
    RSocketServ& iServer;
    
    /**
     * RConnection
     */
    RConnection& iConnection;
    
    /**
     * Socket
     */
    RSocket iSocket;

    /**
     * ICMP sender
     * Own.
     */   
    CNcmIcmpSender* iIcmpSender;

    /**
     * Buffer where data is received from the socket
     */
    TBuf8<64> iData;

    /**
     * Address
     */
    TInetAddr iAddress;

    /**
     * Observer interface used to receive ICMP error notifies 
     */
    MNcmIcmpObserver& iObserver;

    };

#endif // end of C_CNCMICMPV4RECEIVER_H


