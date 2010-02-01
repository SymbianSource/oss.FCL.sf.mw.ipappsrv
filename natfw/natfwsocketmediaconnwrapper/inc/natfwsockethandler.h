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
* Description:    Handles socket connecting
*
*/




#ifndef C_NATFWSOCKETHANDLER_H
#define C_NATFWSOCKETHANDLER_H

#include <es_sock.h>
#include "natfwconnectivityapidefs.h"

#include "natfwsocketsender.h"
#include "natfwsocketreceiver.h"
#include "natfwmediawrapperobserver.h"
#include "mnsmcwtcpconnectionobserver.h"

class CNATFWSocketMediaConnWrapper;

/**
 *  Socket handler
 *  Handles socket connecting and own's socket
 *
 *  @lib socketmediaconnwrapper.dll
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CNATFWSocketHandler ) : public CActive
    {

    friend class ut_cnatfwsockethandler;
    friend class UT_MNATFWMediaWrapper; 

    enum TSenderState
        {
        EUnitialized = 1,
        EGettingConnection = 2,
        EConnected  = 3,
        ESending = 4
        };

    enum TReceiverState
        {
        ENotReceiving = 1,
        EReceiving = 2
        };

public:

    /**
     * Two-phased constructor.
     * @param aRecObs   receiver observer
     * @param aSendObs  sending observer
     * @param aProtocol used protocol
     * @param aPort     wrapper's local port    
     */
    static CNATFWSocketHandler* NewL( MNATFWSocketReceiverObserver& aRecObs,
        MNATFWSocketSenderObserver& aSendObs, TUint aProtocol, TUint aPort );
        
    /**
     * Two-phased constructor.
     * @param aRecObs   receiver observer
     * @param aSendObs  sending observer
     * @param aProtocol used protocol
     * @param aPort     wrapper's local port    
     */  
    static CNATFWSocketHandler* NewLC( MNATFWSocketReceiverObserver& aRecObs,
        MNATFWSocketSenderObserver& aSendObs, TUint aProtocol, TUint aPort );

    /**
    * Destructor.
    */
    virtual ~CNATFWSocketHandler();
        
    /**
     * Get local address
     *
     * @since S60 v3.2
     * @param aSocketToSendForPeer Address for send data
     * @return void
     */    
    void LocalAddress( TSockAddr& aSocketToSendForPeer );
 
    /**
     * Gets wrapper's remote address
     *
     * @since S60 v3.2
     * @param aRemoteAddress   wrapper's remote address
     * @return void   
     */  
    void GetRemoteAddress( TSockAddr& aRemoteAddress );
       
    /**
     * User of NAT Connectivity Framework sets address for incoming packets
     * Wrapper connecting to remote address is started.
     *
     * @since S60 v3.2
     * @param aSocketToSendForPeer Address for send data
     * @return void
     */
    void SetAddrL( const TSockAddr& aSocketToSendForPeer );
    
    /**
     * Enables/disables datapath between mediaengine and NATFW regarding
     * media receiving.
     * 
     * @since S60 v3.2
     * @return void
     */  
    void SetReceivingStateL( );

    /**
     * Enables/disables datapath between mediaengine and NATFW regarding
     * media sending.
     * 
     * @since S60 v3.2
     * @return void
     */
    void SetSendingStateL( );
    
    /**
     * Enables wrapper to forward packets from mediaengine to connection
     * multiplexer.
     * 
     * @since S60 v3.2
     * @param   aState          The sending state
     * @return void
     */     
    void SetReceivingStateForMuxWrapper( TNATFWStreamingState aState );    
            
    /**
     * Send data to connected media engine.
     *
     * @since S60 v3.2
     * @param aStreamPortion a reference to buffer where received data put
     * @return void
     */   
     void SendL( const TDesC8& aStreamPortion );
    
    /**
     * Setting function for local address.
     *
     * @since S60 v3.2
     * @param aSocketServer socket server to be set
     * @param aLocalAddress local address to be set
     * @return void
     */    
    void SetLocalAddress( const RSocketServ& aSocketServer,
                          const TSockAddr& aLocalAddress );

    /**
     * Sets media observer.
     *
     * @since S60 v3.2
     * @param aMediaObserver Media observer
     * @return void
     */ 
    void SetMediaObserverL( MNATFWMediaWrapperObserver* aMediaObserver );
    
    /**
     * Cancels wrapper's send operations.
     *
     * @since S60 v3.2
     * @return void
     */
    void DeactivateSending(); 
    
    /**
     * Cancels wrapper's receive operations.
     *
     * @since S60 v3.2
     * @return void
     */
    void DeactivateReceiving();

    /**
     * Establishes TCP connection between mediaengine and NATFW. Connection
     * will be established in specified direction.
     * 
     * MNsmcwTcpConnectionObserver::ConnectingCompleted will be called when
     * connection is established.
     * 
     * @since   S60 v3.2
     * @param   aSetup      TCP connection setup configuration
     * @param   aObserver   Client to notify when connected
     */
    void OpenTcpConnectionL( TNATFWTcpConnectionSetup aSetup,
                             MNsmcwTcpConnectionObserver& aObserver );
                              
    /**
     * Closes TCP connection.
     *
     * MNsmcwTcpConnectionObserver::ConnectingCompleted is called with error
     * code KErrCancel if TCP connection establishment is ongoing.
     *
     * @since S60 v3.2
     */
    void CloseTcpConnection();
    

private:

    CNATFWSocketHandler( TUint aPort );

    void ConstructL( MNATFWSocketReceiverObserver& aRecObs,
        MNATFWSocketSenderObserver& aSendObs, TUint aProtocol );
    
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
     * Data sender
     * Own.
     */
    CNATFWSocketSender* iSender;

    /**
     * Data receiver
     * Own.
     */
    CNATFWSocketReceiver* iReceiver;

    /**
     * Used protocol udp or tcp
     */
    TUint iProtocol;

    /**
     * Socket
     */
    RSocket iSocket;
    
    /**
     * Socket server
     */
    RSocketServ iSocketServer;
    
    /**
     * Remote address
     */
    TInetAddr iRemoteAddress;
        
    /**
     * Local address
     */
    TInetAddr iLocalAddress;
    
    /**
     * Socket's local port
     */
    TUint iPort;

    /**
     * Descripts sender states.
     */
    TSenderState iSendState;
    
    /**
     * Media observer.
     * Not own.
     */
    MNATFWMediaWrapperObserver* iMediaObserver;
    
    /**
     * Listening socket for a passive TCP connection
     */
    RSocket iListeningSocket;
    
    /**
     * TCP connection type
     */
    TNATFWTcpConnectionSetup iTcpConnectionType;
    
    /**
     * TCP connection observer
     * Not own.
     */
    MNsmcwTcpConnectionObserver* iTcpConnectionObserver;
    
    /**
     * Receiver state
     */
    TReceiverState iReceiveState;    
     
    };

#endif // C_NATFWSOCKETHANDLER_H
