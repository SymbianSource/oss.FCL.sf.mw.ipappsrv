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
* Description:    Abstract socket receiver
*
*/




#ifndef C_CNCMRECEIVER_H
#define C_CNCMRECEIVER_H

#include <e32base.h>
#include <in_sock.h>

#include "ncmconnectionmultiplexer.hrh"

const TUint KMaxReceivingBufferSize = 8192;// 65536 bits = 8192 bytes

class MNcmReceiverSenderObserver;

/**
 *  Abstract socket receiver
 *
 *  @lib connectionmux.lib
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CNcmReceiver ) : public CActive
    {

public:

    /**
     * Destructor
     */
    virtual ~CNcmReceiver();

    /**
     * The real receive action which MUST read data from iSocket to iBuffer.
     * Result is showed by iStatus. A new DoReceive() call will be done.
     *
     * @since S60 v3.2
     */
    virtual void DoReceive() = 0;

    /** 
     * Gets received message.
     * 
     * @since   S60 v3.2
     * @return  Received message.
     */
    TDesC8& Message();

    /** 
     * Gets message's from address.
     *
     * @since   S60 v3.2
     * @return  Message's from address
     */
    const TInetAddr& FromAddress() const;
    
    /**
     * Activate receiving.
     *
     * @since S60 v3.2
     * @return void
     */
    void Activate();
    
    /**
     * Deactivate receiving.
     *
     * @since S60 v3.2
     * @return void
     */
    void Deactivate();
        
protected:

    CNcmReceiver( RSocket& aSocket, MNcmReceiverSenderObserver& aObserver );
        
// from base class CActive

    void RunL();
    
    void DoCancel();

protected: // data

    /**
     * Buffer for a message to be received.
     */
    TBuf8<KMaxReceivingBufferSize> iBuffer;

    /**
     * Socket where to send (MUST be connected!)
     */
    RSocket& iSocket;
    
    /**
     * Destination observer ..
     */
    MNcmReceiverSenderObserver& iObserver;
    
    /**
     * Remote source address.
     */
    TInetAddr iFromAddr;
    
    /**
     * Data length
     */    
    TSockXfrLength iSockDataLength;
           
    /**
     * Notify type
     */
    TMultiplexerConnectionNotifyType iNotify;
    };


#endif // C_CNCMRECEIVER_H
