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
* Description:    Receive data from User of NAT Connectivity Framework.
*
*/




#ifndef C_SOCKET_RECEIVER_H
#define C_SOCKET_RECEIVER_H

#include <e32base.h>
#include <es_sock.h> 
#include <in_sock.h>
#include "natfwconnectivityapidefs.h"
#include "natfwsocketreceiverobserver.h"

const TUint KReceiveBuffer = 8192;
 
/**
 *  Data receiver
 *
 *  Receive data from User of NAT Connectivity Framework.
 *
 *  @lib socketmediaconnwrapper.dll
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CNATFWSocketReceiver ) : public CActive
    {
    
    friend class ut_cnatfwsocketreceiver;

public:

    /**
     * Two-phased constructor.
     * @param aRecObs reveive observer
     * @param aProtocol used protocol
     */       
    static CNATFWSocketReceiver* NewL( MNATFWSocketReceiverObserver& aRecObs,
                                       TUint aProtocol );

    virtual ~CNATFWSocketReceiver();
    
    /**
     * Starts listening data from remote address.
     * 
     * @since S60 v3.2
     * @param aSocket socket handle
     * @return void
     */  
    void StartListening( const RSocket& aSocket );

    /**
     * Enables wrapper to forward packets from mediaengine to connection
     * multiplexer.
     * 
     * @since S60 v3.2
     * @param   aState          The sending state
     * @return void
     */     
    void SetReceivingStateForMuxWrapper( TNATFWStreamingState aState );    

    
private:

    CNATFWSocketReceiver( MNATFWSocketReceiverObserver& aRecObs, 
                          TUint aProtocol );

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
    
    /* From CActive
     * 
     *
     * @since S60 v3.2
     */
    TInt RunError( TInt aError );

private: // data
    
    /**
     * Used protocol udp or tcp
     */
    TUint iProtocol;

    /**
     * Socket
     */
    RSocket iSocket;
        
    /**
     * Observer
     * Not own.
     */
    MNATFWSocketReceiverObserver& iObserver;
    
    /**
     * Data buffer
     */
    TBuf8<KReceiveBuffer> iBuffer;
    
    /**
     * Remote address
     */
    TSockAddr iRemoteAddress;
     
    /**
     * Data length
     */    
    TSockXfrLength iSockDataLength;

    /**
     * Receiving state for wrapper mux datapath
     */
    TNATFWStreamingState iWrapperMuxReceiveState; 

    };


#endif // C_SOCKET_RECEIVER_H
