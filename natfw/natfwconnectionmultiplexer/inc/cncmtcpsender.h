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
* Description:    TCP sender
*
*/




#ifndef C_CNCMTCPSENDER_H
#define C_CNCMTCPSENDER_H

#include "cncmsender.h"


/**
 *  TCP sender
 *
 *  @lib connectionmux.lib
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CNcmTcpSender ) : public CNcmSender
    {

public:

    /**
     * Two-phased constructor
     *
     * @param   aSocket             RSocket
     * @param   aObserver           Observer
     */
    static CNcmTcpSender *NewL( RSocket& aSocket,
        MNcmReceiverSenderObserver& aObserver );
    
    /**
     * Two-phased constructor
     *
     * @param   aSocket             RSocket
     * @param   aObserver           Observer
     */
    static CNcmTcpSender *NewLC( RSocket& aSocket,
        MNcmReceiverSenderObserver& aObserver );
    
    /**
     * Destructor
     */
    virtual ~CNcmTcpSender();

// from base class CNcmSender

    /**
     * Sends iBuffer to iSocket
     *
     * @since S60 v3.2
     * @return void
     */
    void DoSend();

private:

    CNcmTcpSender( RSocket& aSocket, MNcmReceiverSenderObserver& aObserver );

    };


#endif // C_CNCMTCPSENDER_H
