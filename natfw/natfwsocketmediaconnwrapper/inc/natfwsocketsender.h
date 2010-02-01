/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Send data to User of NAT Connectivity Framework.
*
*/




#ifndef C_SOCKET_SENDER_H
#define C_SOCKET_SENDER_H

#include <e32base.h>
#include <es_sock.h> 
#include <in_sock.h>
#include "natfwsocketsenderobserver.h"
#include "cnsmcwsenditem.h"

/**
 *  Data sender
 *
 *  Send data to User of NAT Connectivity Framework.
 *
 *  @lib socketmediaconnwrapper.dll
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CNATFWSocketSender ) : public CActive
    {
    
    friend class ut_cnatfwsocketsender;
    
public:

    /**
     * Two-phased constructor.
     * @param aObserver sender observer
     * @param aProtocol used protocol
     */    
    static CNATFWSocketSender* NewL( 
        MNATFWSocketSenderObserver& aObserver, TUint aProtocol );

    virtual ~CNATFWSocketSender();
    
    /**
     * Set remote address
     *
     * @since S60 v3.2
     * @param aRemoteAddress remote address
     * @param aSocket        socket handle
     * @return void
     */      
    void SetRemoteAddress( const TSockAddr& aRemoteAddress, 
                           const RSocket& aSocket );
    
    /**
     * Send data to connected media engine.
     *
     * @since S60 v3.2
     * @param aStreamPortion a reference to buffer where received data put
     * @return void
     */   
    void SendL( const TDesC8& aStreamPortion );

private:

    CNATFWSocketSender( MNATFWSocketSenderObserver& aObserver, 
                        TUint aProtocol );
    
    /**
     * Send next packet from the queue
     *
     * @since S60 v3.2
     * @return void
     */  
    void SendNextPacket();
    
    /**
     * Remove item from queue
     *
     * @since S60 v3.2
     * @return void
     */  
    void RemoveFromQueue();
    
// from base class CActive
    
    /* From CActive
     * 
     *
     * @since S60 v3.2
     */
    void RunL();
    
    /* From CActive
     * 
     *
     * @since S60 v3.2
     */
    void DoCancel();

private: // data

    /**
     * Socket
     */
    RSocket iSocket;
    
    /**
     * Used protocol udp or tcp.
     */
    TUint iProtocol;
    
    /**
     * Remote address
     */
    TSockAddr iRemoteAddress;

    /**
     * Observer
     */
    MNATFWSocketSenderObserver& iObserver;
    
    /**
     * Send queue
     */
    TSglQue <CNsmcwSendItem> iSendQueue;
    
    /**
     * List iterator which is needed to delete all the items
     */
    TSglQueIter<CNsmcwSendItem> iQueueIter;
    
    };


#endif // C_SOCKET_SENDER_H
