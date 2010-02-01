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
* Description:    Connection multiplexer is responsible for handling network
*                connections, sending and receiving data from the
*                network and allows client to start media flow between
*                connection multiplexer and socket media connection wrapper. 
*
*/




#ifndef C_CNCMCONNECTIONMULTIPLEXER_H
#define C_CNCMCONNECTIONMULTIPLEXER_H

#include <e32base.h>

#include "natfwconnectivityapidefs.h"
#include "mncmmultiplexerconnectionobserver.h"
#include "mncmsessionobserver.h"
#include "mncmconnectionmultiplexer.h"
#include "mncmmediasourceobserver.h"

class TInetAddr;
class MNcmConnectionMultiplexerObserver;
class MNcmMultiplexerConnectionObserver;
class MNcmIncomingConnectionObserver;
class MNcmOutgoingConnectionObserver;
class MNATFWMediaWrapper;
class CNcmPortStore;
class CNcmConnectionObserverHandler;
class RSocket;
class RConnection;
class RSocketServ;
class CNcmSession;
class CNcmStream;
class MNATFWMediaWrapper;
class MNcmSenderObserver;
class CNcmCallBackExecuter;

/**
 *  Connection multiplexer is responsible for handling network
 *  connections, send and receive data from the network. It allows client
 *  to start data flow between connection multiplexer and socket media
 *  connection wrapper.
 *
 *  Connection Multiplexer have one session per IAP and in session can be
 *  multiple streams and in streams can be multiple connections.
 *
 *  @lib connectionmux.lib
 *  @since S60 v3.2
 */
class CNcmConnectionMultiplexer : public CBase,
    public MNcmConnectionMultiplexer,
    public MNcmMultiplexerConnectionObserver,
    public MNcmSessionObserver,
    public MNcmMediaSourceObserver
    {

    friend class UT_CNcmConnectionMultiplexer;
    
public:

    /** 
     * Two-phased constructor
     *
     * @param   aObserver      Observer for Connection Multiplexer
     */
    IMPORT_C static CNcmConnectionMultiplexer* NewL(
        MNcmConnectionMultiplexerObserver& aObserver );

    /** 
     * Destructor
     */
    virtual ~CNcmConnectionMultiplexer();

    /** 
     * Creates a new session. On return the client received identifier for
     * the session. The session is ready to be used when the client receives
     * MNcmConnectionMultiplexerObserver::Notify callback with event code 
     * ESessionCreated.
     *
     * @since   S60 v3.2
     * @param   aIapId              The IAP used for a conection
     * @param   aPortRangeStart     Port range start for session
     * @param   aPortRangeStop      Port range stop for session
     * @return  The ID for the created session.
     */
    IMPORT_C TUint CreateSessionL( TUint32 aIapId, TUint aPortRangeStart,
        TUint aPortRangeStop );
    
    /**
     * Creates a new stream.
     *
     * This is done synchronously.
     *
     * @since   S60 v3.2
     * @param   aSessionId      The ID identifying session
     * @param   aQos            The desired quality of service
     * @param   aProtocol       The protocol to be used,
     *                          KProtocolInetUdp / KProtocolInetTcp
     * @return  The ID for the created stream.
     */
    IMPORT_C TUint CreateStreamL( TUint aSessionId, TInt aQos,
        TUint aProtocol );
    
    /** 
     * Removes session and all its streams and streams connections
     *
     * This is done synchronously.  
     *
     * @since   S60 v3.2
     * @param   aSessionId      The ID identifying session
     * @return  void
     */
    IMPORT_C void RemoveSessionL( TUint aSessionId );
    
    /** 
     * Removes stream and all its connections.
     *
     * This is done synchronously.
     *
     * @since   S60 v3.2
     * @param   aStreamId     The ID identifying stream
     * @return  void
     */
    IMPORT_C void RemoveStreamL( TUint aStreamId );

    /**
     * Registers a media wrapper into stream. Ownership of wrapper is
     * also transferred to Connection Multiplexer.
     *
     * @since   S60 v3.2
     * @param   aMediaWrapper   Pointer to Socket Media Connection Wrapper
     * @return  void
     */
    IMPORT_C void RegisterMediaWrapperL( MNATFWMediaWrapper* aMediaWrapper );

    /**
     * Receives port store associated to session
     *
     * @since   S60 v3.2
     * @param   aSessionId    The ID identifying session
     * @return  Port store used on session
     */    
    IMPORT_C CNcmPortStore& PortStoreL( TUint aSessionId );
    
    /**
     * Gets the socket server handle and the unique name of
     * the RConnection used by the connection multiplexer session.
     *
     * @since   S60 v3.2
     * @param   aSessionId              The ID identifying session
     * @param   aConnectionName         The unique name of the RConnection
     * @return  handle to socket server   
     */
    IMPORT_C RSocketServ* GetSessionInfoL( TUint aSessionId, 
        TName& aConnectionName );  
                      
    /**
     * Enables/disables media flow from Connection Multiplexer to 
     * Socket Media Connection Wrapper for the specified connection.
     *
     * The receiving is enabled/disabled when the client receives
     * MNcmConnectionMultiplexerObserver::Notify callback with event code 
     * EReceivingActivated/EReceivingDeactivated.
     * While incoming data from network is passed to Socket Media Connection
     * Wrapper if receiving for media is activated.
     *
     * @since   S60 v3.2
     * @param   aStreamId           The ID identifying stream
     * @param   aConnectionId       The ID identifying connection
     * @param   aState              The receiving state
     * @return  void
     */
    IMPORT_C void SetReceivingStateForMediaL( TUint aStreamId,
        TUint aConnectionId, TNATFWStreamingState aState );
    
    /**
     * Enables/disables media flow from Socket Media Connection Wrapper to 
     * Connection Multiplexer for the specified connection.
     *
     * The sending is enabled/disabled when the client receives
     * MNcmConnectionMultiplexerObserver::Notify callback with event code 
     * ESendingActivated/ESendingDeactivated. While incoming data from
     * Socket Media Connection Wrapper is delivered to specified connection
     * which is used to send media to network if sending for media is
     * activated.
     *
     * @since   S60 3.2
     * @param   aStreamId           The ID identifying stream
     * @param   aConnectionId       The ID identifying connection
     * @param   aState              The sending state
     * @return  void
     */
    IMPORT_C void SetSendingStateForMediaL( TUint aStreamId,
        TUint aConnectionId, TNATFWStreamingState aState );

    /**
     * Resolves the destination IP address from FQDN.
     * ( Address and port )

     * @since   S60 v3.2
     * @param   aStreamId      The ID identifying stream
     * @param   aAddress       The Destination FQDN
     * @param   aPort          The Destination port
     * @param   aResult        On return contains the resolved IP address
     * @return  void
     */
    IMPORT_C void ResolveDestinationAddressL( TUint aStreamId,
        const TDesC8& aAddress, TUint aPort, TInetAddr& aResult );
                
// from base class MNcmConnectionMultiplexer

    /**
     * Creates a new connection for stream. Will use same protocol as
     * used in stream. Address family specifies address family
     * which is used to choose whether IPv4 or IPv6 connection will be made.
     *
     * This is done synchronously.
     * 
     * @since   S60 v3.2
     * @param   aStreamId       The ID identifying stream
     * @param   aAddrFamily     The address family(KAfInet/KAfInet6)
     *                          KAfInet, IPv4 local address is used
     *                          KAfInet6, IPv6 local address is used
     * @return  The ID for the created connection
     * @leave KErrNotSupported If specified local address hasn't found
     */
    IMPORT_C TUint CreateConnectionL( TUint aStreamId, TUint aAddrFamily );

    /**
     * Overloaded CreateConnectionL
     * Creates a new connection for stream. Will use same protocol as
     * used in stream. Connection is made by using specified local address.
     *
     * This is done synchronously.
     * 
     * @since   S60 v3.2
     * @param   aStreamId       The ID identifying stream
     * @param   aLocalAddr      The specified local address.
     * @return  The ID for the created connection
     */
    IMPORT_C TUint CreateConnectionL( TUint aStreamId,
        const TInetAddr& aLocalAddr );
        
    /**
     * Removes the specified connection.
     * 
     * @since   S60 v3.2
     * @param   aStreamId       The ID identifying stream
     * @param   aConnectionnId  The ID identifying connection
     * @return  void
     */
    IMPORT_C void RemoveConnectionL( TUint aStreamId, TUint aConnectionnId );
        
    /**
     * Fetches stream specific information which is formerly stored, while
     * stream was created
     *
     * @since   S60 v3.2
     * @param   aStreamId       The ID identifying stream
     * @param   aIapId          IAP Id for session
     * @param   aQos            The quality of service
     * @param   aProtocol       KProtocolInetUdp / KProtocolInetTcp
     * @return  void
     */
    IMPORT_C void GetStreamInfoL( TUint aStreamId, TUint32& aIapId,
        TInt& aQos, TUint& aProtocol );

    /**
     * Returns the local IP address and port of specified connection.
     *
     * @since   S60 v3.2
     * @param   aStreamId       The ID identifying stream
     * @param   aConnectionId   The ID identifying connection
     * @return  Local IP address
     */
    IMPORT_C TInetAddr& LocalIPAddressL( TUint aStreamId,
        TUint aConnectionId );
            
    /**
     * Establishes TCP connection. Connection will be established in specified
     * direction using given connection setup.
     *
     * MNcmConnectionObserver::ConnectionNotify is called with event code
     * ETcpSetupCompleted when connection setup is completed.
     * 
     * SetReceivingStateL/SetSendingStateL can be called normally after
     * successfull connection initialization.
     * 
     * @since   S60 3.2
     * @param   aStreamId       The ID identifying stream
     * @param   aConnectionId   The ID identifying connection
     * @param   aConfig         TCP connection setup configuration
     * @param   aDestAddr       The destination address for active connection
     * @return  void
     */      
    IMPORT_C void OpenTcpConnectionL( TUint aStreamId, TUint aConnectionId,
        TNATFWTcpConnectionSetup aConfig, const TInetAddr& aDestAddr );

    /**
     * Closes TCP connection or cancels in-progress connection process.
     *
     * If TCP connection establishment is in-progress 
     * MNcmConnectionObserver::Notify is called with event code 
     * ETcpConnSetupCompleted and error code KErrCancel. 
     *
     * After successful connection closing SetupTcpConnectionL can be
     * called again with different destination address.
     * 
     * @since   S60 3.2
     * @param   aStreamId       The ID identifying stream
     * @param   aConnectionId   The ID identifying connection
     * @return  void
     */
    IMPORT_C void CloseTcpConnection( TUint aStreamId, TUint aConnectionId );
                
    /**
     * Registers an observer for observing incoming messages in
     * specific stream. This method won't register observer again if
     * client have allready registered.
     *
     * @since   S60 v3.2
     * @param   aStreamId       The ID identifying stream
     * @param   aObserver       Observer to be registered
     * @return  void
     */
    IMPORT_C void RegisterIncomingConnectionObserverL( TUint aStreamId,
        MNcmIncomingConnectionObserver& aObserver );

    /**
     * Registers an observer for outgoing messages in specific
     * stream. This method won't register observer again if
     * client have allready registered.
     *
     * @since   S60 v3.2
     * @param   aStreamId       The ID identifying stream
     * @param   aObserver       Observer to be registered
     * @return  void
     */
    IMPORT_C void RegisterOutgoingConnectionObserverL( TUint aStreamId,
        MNcmOutgoingConnectionObserver& aObserver );

    /**
     * Unregisters specified observer.
     *
     * @since   S60 v3.2
     * @param   aStreamId       The ID identifying stream
     * @param   aObserver       Observer to be unregistered
     * @return  void
     */
    IMPORT_C void UnregisterIncomingConnectionObserverL( TUint aStreamId,
        MNcmIncomingConnectionObserver& aObserver );

    /**
     *  Unregisters specified observer.
     *
     * @since   S60 v3.2
     * @param   aStreamId       The ID identifying stream
     * @param   aObserver       Observer to be unregistered
     * @return  void
     */
    IMPORT_C void UnregisterOutgoingConnectionObserverL( TUint aStreamId,
        MNcmOutgoingConnectionObserver& aObserver );
      
    /**
     * Registers an observer for connection notifies.
     * This method won't register observer again if
     * client have allready registered.
     *
     * @since   S60 v3.2
     * @param   aStreamId       The ID identifying stream
     * @param   aObserver       Observer to be registered
     * @return  void
     */
    IMPORT_C void RegisterConnectionObserverL( TUint aStreamId,
        MNcmConnectionObserver& aObserver );

    /**
     *  Unregisters specified observer.
     *
     * @since S60 v3.2
     * @param   aStreamId       The ID identifying stream
     * @param   aObserver       Observer to be unregistered
     * @return void
     */
    IMPORT_C void UnregisterConnectionObserverL( TUint aStreamId,
        MNcmConnectionObserver& aObserver );

    /**
     * Registers an observer for receive incoming and outgoing message
     * notifications. All notifications are offered first to this observer.
     * There can be only one registered observer at a time.
     * Message observer is allowed to make modifications to offered
     * messages.
     *
     * @since   S60 v3.2
     * @param   aStreamId       The ID identifying stream
     * @param   aObserver       Observer to be registered
     * @return  void
     */
    IMPORT_C void RegisterMessageObserverL( TUint aStreamId,
        MNcmMessageObserver& aObserver );

    /**
     * Unregisters an observer for receiving incoming and outgoing message
     * notifications.
     *
     * @since S60 v3.2
     * @param aStreamId A stream ID
     * @param aObserver to be unregistered
     * @return void
     */
    IMPORT_C void UnregisterMessageObserverL( TUint aStreamId,
        MNcmMessageObserver& aObserver );
        
    /**
     * Sends a message from specified connection to network.
     *
     * @since   S60 v3.2
     * @param   aStreamId           The ID identifying stream
     * @param   aConnectionId       The ID identifying connection
     * @param   aMessage            A message to be copied and
     *                              then sent asynchronously.
     * @param   aSenderObserver     Observer for this send.
     * @return  void
     */
    IMPORT_C void SendL( TUint aStreamId, TUint aConnectionId,
        const TDesC8& aMessage, MNcmSenderObserver* aSenderObserver = NULL );
    
    /**
     * Overloaded SendL
     * Sends a message from specified connection to network. 
     * Destination address parameter tells
     * the destination address for the message. 
     *
     * @since   S60 v3.2
     * @param   aStreamId               The ID identifying stream
     * @param   aConnectionId           The ID identifying conenction
     * @param   aMessage                A message to be copied and
     *                                  then sent asynchronously.
     * @param   aDestinationAddress     Destination address
     * @param   aSenderObserver         Observer for this send.
     * @return  void
     */
    IMPORT_C void SendL( TUint aStreamId, TUint aConnectionId,
        const TDesC8& aMessage, const TInetAddr& aDestinationAddress,
        MNcmSenderObserver* aSenderObserver = NULL );

    /**
     * Overloaded SendL
     * Sends a message from connection which is found by next hop address.
     * Destination address parameter tells the destination address for
     * the message. 
     *
     * @since   S60 v3.2
     * @param   aStreamId               The ID identifying stream
     * @param   aMessage                A message
     * @param   aNextHopAddress         Specifies the IP address of the next
     *                                  hop in the route
     *                                  will be used to find connection which
     *                                  will be used for sending.
     * @param   aDestinationAddress     Destination address.
     * @return  void
     */
    IMPORT_C void SendL( TUint aStreamId, const TDesC8& aMessage,
        const TInetAddr& aNextHopAddress,
        const TInetAddr& aDestinationAddress );
        
    /**
     * Overloaded SendL
     * Sends a message directly from connection. Will not offer this
     * message to any registered observers if aSendDirectly is set to
     * ETrue
     *
     * @since   S60 v3.2
     * @param   aStreamId           The ID identifying stream
     * @param   aConnectionId       The ID identifying conenction
     * @param   aMessage            A message
     * @param   aSendDirectly       if ETrue sends a message directly.
     *                              Will not offer this message to any
     *                              registered observers.
     *                              normally and will be offered registered
     *                              observers.
     * @param   aSenderObserver     Observer for this send.
     * @return  void
     */
    IMPORT_C void SendL( TUint aStreamId, TUint aConnectionId,
        const TDesC8& aMessage, TBool aSendDirectly,
        MNcmSenderObserver* aSenderObserver = NULL );
    
    /**
     * Cancels sender observer's message send processes.
     *
     * Client MUST call its destructor on this method and cancel all send
     * processes which it is observing( Client have used SendL with 
     * sender observer param ), otherwise there can be panic.
     *
     * @since   S60 v3.2
     * @param   aStreamId           The ID identifying stream
     * @param   aConnectionId       The ID identifying connection         
     * @param   aSenderObserver     Observer which observing
     *                              message send processes 
     * @return  void
     */
     IMPORT_C void CancelMessageSend( TUint aStreamId, TUint aConnectionId,
        const MNcmSenderObserver* aSenderObserver );
        

    /**
     * Enables/disables data receiving for the specified connection.
     *
     * MNcmConnectionObserver::ConnectionNotify is called with event code
     * EReceivingActivated/EReceivingDeactivated when status change
     * has completed. While incoming data is received from network.
     *
     * Connection receives can be use for sending
     * after successful sending activation.
     *
     * @since   S60 v3.2
     * @param   aStreamId           The ID identifying stream
     * @param   aConnectionId       The ID identifying connection
     * @param   aState              The receiving state
     * @return  void
     */
    IMPORT_C void SetReceivingStateL( TUint aStreamId, TUint aConnectionId,
        TNATFWStreamingState aState );
    
    /**
     * Enables/disables data sending for the specified connection.
     * Function can be called again with new destination address while
     * sending is in activated state.
     *
     * MNcmConnectionObserver::ConnectionNotify is called with event code
     * ESendingActivated/ESendingDeactivated when status change
     * has completed.
     *
     * Connection can be use for sending after successful sending activation.
     *
     * Destination address for TCP-connection is given in 
     * active TCP-connection setup.
     *
     * @since   S60 3.2
     * @param   aStreamId           The ID identifying stream
     * @param   aConnectionId       The ID identifying connection
     * @param   aDestAddr           The destination address where
     *                              the client wants to connect to
     * @param   aState              The sending state
     * @return  void
     */
    IMPORT_C void SetSendingStateL( TUint aStreamId, TUint aConnectionId,
        const TInetAddr& aDestAddr, TNATFWStreamingState aState );
    
    /**
     * Sets address from which incoming data is accepted from this moment
     * onwards. Binds connection to the specified remote address. Unbinding
     * can be done by setting unspecified address.
     *
     * @since   S60 v3.2
     * @param   aStreamId           The ID identifying stream
     * @param   aConnectionId       The ID identifying connection
     * @param   aAddress            The remote address
     */
    IMPORT_C void SetAcceptedFromAddressL( TUint aStreamId, 
        TUint aConnectionId, const TInetAddr& aAddress );
    
    /**
     * Static handler function for callbacks.
     *
     * @since   S60 3.2
     * @param   aObject         The object reference
     * @param   aStreamId       The ID identifying stream
     * @param   aConnectionId   The ID identifying connection     
     * @param   aType           The notify type
     * @param   aError          The error code
     * @return  void
     */    
    static void HandleCallBack( CBase& aObject, TUint aStreamId,
        TUint aConnectionId, TMultiplexerConnectionNotifyType aType,
        TInt aError );
            
// from base class MNcmMultiplexerConnectionObserver

    /**
     * From MNcmMultiplexerConnectionObserver
     *
     * Called by connection when transfer error occurs on it
     *
     * @since   S60 v3.2
     * @param   aSessionId          The ID identifying session
     * @param   aStreamId           The ID identifying stream
     * @param   aConnectionId       The ID identifying connection
     * @param   aNotifyType         Type of notify
     * @param   aError              The error code
     * @return void
     */
    void ConnectionError( TUint aSessionId, TUint aStreamId,
        TUint aConnectionId,
        TMultiplexerConnectionNotifyType aNotifyType, TInt aError );

    /**
     * From MNcmMultiplexerConnectionObserver
     *
     * Called by socket destination object when action completes
     * (or error is reached)
     *
     * @since   S60 v3.2
     * @param   aStreamId           The ID identifying stream
     * @param   aConnectionId       The ID identifying connection
     * @param   aType               Type of notify
     * @param   aError              Error code
     * @return  void
     */
    void ConnectionNotify( TUint aStreamId, TUint aConnectionId,
                           TMultiplexerConnectionNotifyType aType,
                           TInt aError );

    /**
     * From MNcmMultiplexerConnectionObserver
     *
     * Called by socket destination object when current
     * local port is already in use.
     * Gets new free port.
     *
     * @since   S60 v3.2
     * @param   aStreamId           The ID identifying stream
     * @param   aConnectionId       The ID identifying connection     
     * @param   aPort               New free port
     * @return  void
     */
    void GetNewFreePort( TUint aStreamId, TUint aConnectionId, TUint& aPort );
        
// from base class MNcmMediaSourceObserver

    /**
     * From MNcmMediaSourceObserver
     *
     * Called by Media Source after wrapper has had sending error
     *
     * @since   S60 v3.2
     * @param   aStreamId           The ID identifying stream   
     * @param   aError              The error code
     * @return  void
     */
    void WrapperSenderError( TUint aStreamId, TInt aError );
    
    /**
     * From MNcmMediaSourceObserver
     *
     * Called by Media Source if current local port is already in use.
     * Gets new free port.
     *
     * @since   S60 v3.2
     * @param   aStreamId           The ID identifying stream   
     * @param   aPort               New free port
     * @return  void
     */ 
    void GetFreePort( TUint aStreamId, TUint& aPort );

    /**
     * From MNcmMediaSourceObserver
     *
     * Notifies that the first media packet is sent to
     * the remote address.
     *
     * @since   S60 v3.2
     * @param   aStreamId           The ID identifying stream
     * @return  void
     */     
    void FirstMediaPacketSent( TUint aStreamId );
        
// from base class MNcmSessionObserver

    /**
     * From MNcmSessionObserver
     *
     * Notifies that session creation is failed.
     *
     * @since   S60 v3.2
     * @param   aSessionId      The ID identifying session
     * @return  void
     */
    void SessionCreationFailed( TUint aSessionId );

private:

    void ConstructL();

    CNcmConnectionMultiplexer(
        MNcmConnectionMultiplexerObserver& aObserver );
    
    CNcmSession* SessionByIdL( TUint aSessionId );
    
    CNcmStream* StreamByIdL( TUint aStreamId );
    
    CNcmStream* StreamById( TUint aStreamId );
    
    CNcmSession* SessionByStreamIdL( TUint aStreamId );
 
    void ConnectionNotifyL( TUint aStreamId, TUint aConnectionId,
        TMultiplexerConnectionNotifyType aType, TInt aError );
    
    void GetNewFreePortL( TUint aStreamId, TUint aConnectionId,
        TUint& aPort );

    void HandleCallBackL( TUint aStreamId, TUint aConnectionId,
        TMultiplexerConnectionNotifyType aType, TInt aError );
        
    void CloseTcpConnectionL( TUint aStreamId, TUint aConnectionId );

    void CancelMessageSendL( TUint aStreamId, TUint aConnectionId,
        const MNcmSenderObserver* aSenderObserver );
            
private: // data

    /**
     * Observer for this connection multiplexer
     * Not own.
     */
    MNcmConnectionMultiplexerObserver& iObserver;
    
    /**
     *  Next unused session ÍD 
     */
    TUint iNextSessionId;
    
    /**
     * Created sessions.
     * Own.
     */
    RPointerArray<CNcmSession> iSessions;

    /**
     * Async callback executer
     * Own.
     */    
    CNcmCallBackExecuter* iAsyncCallback;   
    };


#endif // C_CNCMCONNECTIONMULTIPLEXER_H
