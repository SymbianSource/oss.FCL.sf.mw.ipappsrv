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
* Description:    TCP receiver
*
*/




#ifndef C_CNCMTCPRECEIVER_H
#define C_CNCMTCPRECEIVER_H

#include "cncmreceiver.h"

/**
 *  TCP receiver
 *
 *  @lib connectionmux.lib
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CNcmTcpReceiver ) : public CNcmReceiver
    {

public:

    /**
     * Two-phased constructor
     *
     * @param   aSocket             RSocket
     * @param   aObserver           Observer
     */
    static CNcmTcpReceiver *NewL( RSocket& aSocket,
        MNcmReceiverSenderObserver& aObserver );
    
    /**
     * Two-phased constructor
     *
     * @param   aSocket             RSocket
     * @param   aObserver           Observer
     */
    static CNcmTcpReceiver *NewLC( RSocket& aSocket,
        MNcmReceiverSenderObserver& aObserver );
    
    /**
     * Destructor
     */
    virtual ~CNcmTcpReceiver();

    /**
     * Sets the remote source address
     *
     * @since   S60 v3.2
     * @param   aRemoteAddress      A remote source address
     * @return  void
     */
    void SetDestination( const TInetAddr& aRemoteAddress );
    
// from base class CNcmReceiver

    /**
     * Sends iBuffer to iSocket
     *
     * @since S60 v3.2
     * return void
     */
    void DoReceive();


private:

    CNcmTcpReceiver( RSocket& aSocket,
        MNcmReceiverSenderObserver& aObserver );
    };


#endif // C_CNCMTCPRECEIVER_H
