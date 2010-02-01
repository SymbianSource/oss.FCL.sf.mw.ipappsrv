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
* Description:    Abstract socket sender
*
*/




#ifndef C_CNCMSENDER_H
#define C_CNCMSENDER_H

#include <e32base.h>

#include "ncmconnectionmultiplexer.hrh"

class MNcmReceiverSenderObserver;
class MNcmSenderObserver;
class RSocket;
class CNcmSendItem;

/**
 *  Abstract socket sender
 *
 *  @lib connectionmux.lib
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CNcmSender ) : public CActive
    {

public:

    /**
     * Destructor
     */
    virtual ~CNcmSender();

    /**
     * Send action which forwards real sending operation to DoSend()
     *
     * @since S60 v3.2
     * @param aMessage          Message to be send. It will be copied
     *                          to iBuffer.
     * @param aSenderObserver   Sender observer for this sending request
     */
    void Send( const TDesC8& aMessage, MNcmSenderObserver* aSenderObserver );

    /**
     * Cancels the specified message send processes.
     *
     * @since   S60 v3.2       
     * @param   aSenderObserver         Observer
     * @return  void
     */
    void CancelMessageSendL( const MNcmSenderObserver* aSenderObserver );
        
    /**
     * The real send action which should write iBuffer to iSocket
     *
     * @since S60 v3.2
     */
    virtual void DoSend() = 0;
    
    /**
     * Activate sending.
     *
     * @since S60 v3.2
     * @return void
     */
    void Activate() const;
    
    /**
     * Deactivate sending.
     *
     * @since S60 v3.2
     * @return void
     */
    void Deactivate();

protected:

    CNcmSender( RSocket& aSocket,
        MNcmReceiverSenderObserver& aObserver );
    
// from base class CActive

    void RunL();
    
    void DoCancel();

protected: // data

    /**
     * Socket where to send (MUST be connected!)
     */
    RSocket& iSocket;
    
    /**
     * Destination observer.
     */
    MNcmReceiverSenderObserver& iObserver;
    
    /**
     * Notify type
     */
    TMultiplexerConnectionNotifyType iNotify;
    
    /**
     * Send item array.
     * Holds packets in array if socket is busy.
     * Own.
     */
    RPointerArray<CNcmSendItem> iSendItemArray;
    };


#endif // C_CNCMSENDER_H
