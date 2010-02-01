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
* Description:    Implements Socket Media Connection Wrapper IF and 
                 Socket Media Wrapper IF
*
*/




#ifndef C_SOCKET_MEDIA_CONN_WRAPPER_H
#define C_SOCKET_MEDIA_CONN_WRAPPER_H


#include <e32base.h>
#include <es_sock.h>

#include "mnatfwsocketmediaconnwrapper.h"
#include "natfwmediawrapper.h"
#include "natfwsockethandler.h"

class MNATFWMediaWrapperObserver;
class MNATFWSocketReceiverObserver;
class MNATFWSocketSenderObserver;

/**
 *  A basic media interface for attaching socket-based medias into nat 
 *  framework.
 *
 *  Socket Media Connection Wrapper implements a proxy pattern, between
 *  the NAT Framework user and internal use of the NAT Connectivity Framework.
 *
 *  @lib socketmediaconnwrapper.dll
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS ( CNATFWSocketMediaConnWrapper ) : public CBase, 
    public MNATFWSocketMediaConnWrapper, public MNATFWMediaWrapper,
    public MNATFWSocketReceiverObserver, public MNATFWSocketSenderObserver
  
    {
    friend class UT_MNATFWMediaWrapper;

public:
    
    /**
     * Two-phased constructor.
     * @param aObserver media observer
     * @param aPort     wrapper's local port
     * @param aProtocol used protocol
     * @param aStreamId stream id
     */    
    IMPORT_C static CNATFWSocketMediaConnWrapper* NewL( TUint aPort, 
        TUint aProtocol, TUint aStreamId );

    virtual ~CNATFWSocketMediaConnWrapper();

   
// from base class MNATFWSocketMediaConnWrapper

    /**
     * From MNATFWSocketMediaConnWrapper
     * Returns The stream ID which wrapper is associated with
     *
     * @since S60 v3.2
     * @return stream Id
     */
    IMPORT_C TUint StreamId();
    
    /**
     * From  MNATFWSocketMediaConnWrapper
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
     * From MNATFWSocketMediaConnWrapper
     * Closes TCP connection.
     *
     * MNsmcwTcpConnectionObserver::ConnectingCompleted is called with error
     * code KErrCancel if TCP connection establishment is ongoing.
     *
     * @since S60 v3.2
     */
    void CloseTcpConnection();
    
    /**
     * From MNATFWSocketMediaConnWrapper
     * User of NAT Connectivity Framework fetch the reserved local IP which
     * is used as destination address
     * 
     * @param aSocketToSendForPeer Address for receiving data
     * @since S60 v3.2
     * @return void
     */
    IMPORT_C void OutgoingAddr( TSockAddr& aSocketToSendForPeer );
    
    /**
     * From MNATFWSocketMediaConnWrapper
     * User of NAT Connectivity Framework sets local socket address for 
     * incoming packets. Wrapper connecting to remote address is started.
     *
     * @param aSocketToReceiveFromPeer Destination address for sending
     * @since S60 v3.2
     * @return void
     */
    IMPORT_C void SetIncomingAddrL( 
        const TSockAddr& aSocketToReceiveFromPeer );
        
    /**
     * From MNATFWSocketMediaConnWrapper
     * Enables/disables datapath between mediaengine and NATFW regarding
     * media receiving.
     * 
     * @since S60 v3.2
     * @param   aState          The receiving state
     * @return void
     */
    IMPORT_C void SetReceivingStateL( TNATFWStreamingState aState );
    
    /**
     * From MNATFWSocketMediaConnWrapper
     * Enables/disables datapath between mediaengine and NATFW regarding
     * media sending.
     * 
     * @since S60 v3.2
     * @param   aState          The sending state
     * @return void
     */
    IMPORT_C void SetSendingStateL( TNATFWStreamingState aState );
    
    
// from base class MNATFWMediaWrapper     
    
    
    /**
     * From MNATFWMediaWrapper
     * Enables wrapper to forward packets from mediaengine to connection
     * multiplexer.
     * 
     * @since S60 v3.2
     * @param   aState          The sending state
     * @return void
     */     
    IMPORT_C void SetReceivingStateForMuxWrapper( TNATFWStreamingState aState );      
       
    /**
     * From MNATFWMediaWrapper
     * Send data to connected media engine.
     *
     * @param aStreamPortion a reference to buffer to be sent
     * @since S60 v3.2
     * @return void
     */
    IMPORT_C void SendL( const TDesC8& aStreamPortion );
    
    /**
     * From MNATFWMediaWrapper
     * Register media observer.
     *
     * @since S60 v3.2
     * @param aObserver  media observer 
     * @param aServer    socket server handle
     * @param aLocalAddr wrapper's local address
     * @return void   
     */
    IMPORT_C void RegisterMediaWrapperObserverL( 
        MNATFWMediaWrapperObserver* aObserver,
        const RSocketServ& aServer,
        const TInetAddr& aLocalAddr );

    /**
     * Closes wrapper instance by deleting it.
     *
     * @since S60 v3.2
     * @return void
     */
    IMPORT_C void Close();
 
    /**
     * Gets wrapper's local and remote addresses
     *
     * @since S60 v3.2
     * @param aLocalAddr    wrapper's local address 
     * @param aRemoteAddr   wrapper's remote addresse
     * @return void   
     */
    IMPORT_C void GetAddresses( TInetAddr& aLocalAddr, TInetAddr& aRemoteAddr );
        
// from base class MNATFWSocketSenderObserver

    /**
     * From MNATFWSocketSenderObserver
     * Returns with completition code of sending operation
     *
     * @param aError system-wide error code
     * @since S60 v3.2
     * @return void
     */
    void SendingCompleted( TInt aError );
    
// from base class MNATFWSocketReceiverObserver
    
    /**
     * From MNATFWSocketReceiverObserver
     * Delivers buffer to multiplexer
     *
     * @param aStreamPortion stream portion which were sent by client
     * @since S60 v3.2
     * @return void
     */
    void DeliverBuffer( TDes8& aStreamPortion );
        
    
private:
    
    CNATFWSocketMediaConnWrapper( );
    
    void ConstructL( TUint aProtocol, TUint aStreamId, TUint aPort  );


private: // data

    /**
     * ID which identifies streams
     */
    TUint iStreamId;
    
    /**
     * Socket handler
     * Own.
     */    
    CNATFWSocketHandler* iHandler;
    
    /**
     * Observer
     * Not own.
     */
    MNATFWMediaWrapperObserver* iMediaObserver;
    
   };


#endif // C_SOCKET_MEDIA_CONN_WRAPPER_H
