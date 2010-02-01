/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    UDP receiver
*
*/




#ifndef C_CNCMUDPRECEIVER_H
#define C_CNCMUDPRECEIVER_H

#include "cncmreceiver.h"

/**
 *  UDP receiver
 *
 *  @lib connectionmux.lib
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CNcmUdpReceiver ) : public CNcmReceiver
    {

public:

    /**
     * Two-phased constructor
     *
     * @param   aSocket             RSocket
     * @param   aObserver           Observer
     */
    static CNcmUdpReceiver *NewL( RSocket& aSocket,
        MNcmReceiverSenderObserver& aObserver );
    
    /**
     * Two-phased constructor
     *
     * @param   aSocket             RSocket
     * @param   aObserver           Observer
     */
    static CNcmUdpReceiver *NewLC( RSocket& aSocket,
        MNcmReceiverSenderObserver& aObserver );
    
    /**
     * Destructor
     */
    virtual ~CNcmUdpReceiver();
    
// from base class CNcmReceiver

    /**
     * Sends iBuffer to iSocket
     *
     * @since   S60 v3.2
     * @return  void
     */
    void DoReceive();


private:

    CNcmUdpReceiver( RSocket& aSocket,
        MNcmReceiverSenderObserver& aObserver );
    };


#endif // C_CNCMUDPRECEIVER_H
