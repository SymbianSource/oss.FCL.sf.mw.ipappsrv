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
* Description:    UDP sender 
*
*/




#ifndef C_CNCMUDPSENDER_H
#define C_CNCMUDPSENDER_H

#include <in_sock.h>

#include "cncmsender.h"

/**
 *  UDP sender
 *
 *  @lib connectionmux.lib
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CNcmUdpSender ) : public CNcmSender
    {

public:

    /**
     * Two-phased constructor
     *
     * @param   aSocket             RSocket
     * @param   aObserver           Observer
     */
    static CNcmUdpSender *NewL( RSocket& aSocket,
        MNcmReceiverSenderObserver& aObserver );
    
    /**
     * Two-phased constructor
     *
     * @param   aSocket             RSocket
     * @param   aObserver           Observer
     */
    static CNcmUdpSender *NewLC( RSocket& aSocket,
        MNcmReceiverSenderObserver& aObserver );
    
    /**
     * Destructor
     */
    virtual ~CNcmUdpSender();
    
    /**
     * Set destination address
     *
     * @since   S60 v3.2
     * @param   aDestination        Destination address
     * @return void
     */
    void SetDestination( const TInetAddr& aDestination );

// from base class CNcmSender

    /**
     * Sends iBuffer to iSocket
     *
     * @since S60 v3.2
     * @return void
     */
    void DoSend();


private:

    CNcmUdpSender( RSocket& aSocket,
        MNcmReceiverSenderObserver& aObserver );

private: // data

    /**
     * Destination address for this sender
     */
    TInetAddr iDestination;
    };


#endif // C_CNCMUDPSENDER_H