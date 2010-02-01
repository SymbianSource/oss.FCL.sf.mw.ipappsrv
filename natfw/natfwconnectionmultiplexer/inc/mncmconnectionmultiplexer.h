/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    A basic interface for using connection multiplexer.
*
*/




#ifndef M_MNCMCONNECTIONMULTIPLEXER_H
#define M_MNCMCONNECTIONMULTIPLEXER_H

#include <e32base.h>

#include "natfwconnectivityapidefs.h"

class MNcmIncomingConnectionObserver;
class MNcmOutgoingConnectionObserver;
class MNcmConnectionObserver;
class MNcmMessageObserver;
class MNcmSenderObserver;
class RSocketServ;
class RSocket;
class RConnection;
class TInetAddr;

/**
 *  A basic interface for using connection multiplexer. 
 *
 *  
 *  @lib connectionmux.lib
 *  @since S60 v3.2
 */
class MNcmConnectionMultiplexer
    {

public:

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
    virtual TUint CreateConnectionL( TUint aStreamId, TUint aAddrFamily ) = 0;

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
    virtual TUint CreateConnectionL( TUint aStreamId,
        const TInetAddr& aLocalAddr ) = 0;
        
    /**
     * Removes the specified connection.
     * 
     * @since   S60 v3.2
     * @param   aStreamId       The ID identifying stream
     * @param   aConnectionnId  The ID identifying connection
     */
    virtual void RemoveConnectionL( TUint aStreamId,
        TUint aConnectionnId ) = 0;
                
    /**
     * Fetches stream specific information which is formerly stored, while
     * stream was created
     *
     * @since   S60 v3.2
     * @param   aStreamId       The ID identifying stream
     * @param   aIapId          IAP Id for session
     * @param   aQos            The quality of service
     * @param   aProtocol       KProtocolInetUdp / KProtocolInetTcp
     */
    virtual void GetStreamInfoL( TUint aStreamId, TUint32& aIapId,
        TInt& aQos, TUint& aProtocol ) = 0;
        
    /**
     * Return the local IP address and port of specified connection.
     *
     * @since   S60 v3.2
     * @param   aStreamId       The ID identifying stream
     * @param   aConnectionId   The ID identifying connection
     * @return  Local IP address
     */
    virtual TInetAddr& LocalIPAddressL( TUint aStreamId,
        TUint aConnectionId ) = 0;

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
     */      
    virtual void OpenTcpConnectionL( TUint aStreamId, TUint aConnectionId,
        TNATFWTcpConnectionSetup aConfig, const TInetAddr& aDestAddr ) = 0;
         
    /**
     * Closes TCP connection or cancel in-progress connection process.
     *
     * If TCP connection establishment is in-progress 
     * MNcmConnectionObserver::Notify is called with event code 
     * ETcpConnSetupCompleted and error code KErrCancel.
     *
     * After successful connection closing ConnectTcpConnectionL can be
     * called again with different destination address.
     * 
     * @since   S60 3.2
     * @param   aStreamId       The ID identifying stream
     * @param   aConnectionId   The ID identifying connection
     */
    virtual void CloseTcpConnection( TUint aStreamId,
        TUint aConnectionId ) = 0;
    
    /**
     * Registers an observer for observing incoming messages in
     * specific stream. This method won't register observer again if
     * client have allready registered.
     *
     * @since   S60 v3.2
     * @param   aStreamId       The ID identifying stream
     * @param   aObserver       Observer to be registered
     */
    virtual void RegisterIncomingConnectionObserverL( TUint aStreamId,
        MNcmIncomingConnectionObserver& aObserver ) = 0;

    /**
     * Registers an observer for outgoing messages in specific
     * stream. This method won't register observer again if
     * client have allready registered.
     *
     * @since   S60 v3.2
     * @param   aStreamId       The ID identifying stream
     * @param   aObserver       Observer to be registered
     */
    virtual void RegisterOutgoingConnectionObserverL( TUint aStreamId,
        MNcmOutgoingConnectionObserver& aObserver ) = 0;

    /**
     * Unregisters specified observer.
     *
     * @since   S60 v3.2
     * @param   aStreamId       The ID identifying stream
     * @param   aObserver       Observer to be unregistered
     */
    virtual void UnregisterIncomingConnectionObserverL( TUint aStreamId,
        MNcmIncomingConnectionObserver& aObserver ) = 0;

    /**
     * Unregisters specified observer.
     *
     * @since   S60 v3.2
     * @param   aStreamId       The ID identifying stream
     * @param   aObserver       Observer to be unregistered
     */
    virtual void UnregisterOutgoingConnectionObserverL( TUint aStreamId,
        MNcmOutgoingConnectionObserver& aObserver ) = 0;
     
    /**
     * Registers an observer for connection notifies.
     * This method won't register observer again if
     * client have allready registered.
     *
     * @since   S60 v3.2
     * @param   aStreamId       The ID identifying stream
     * @param   aObserver       Observer to be registered
     */
    virtual void RegisterConnectionObserverL( TUint aStreamId,
        MNcmConnectionObserver& aObserver ) = 0;

    /**
     * Unregisters an observer for connection notifies.
     *
     * @since   S60 v3.2
     * @param   aStreamId       A stream ID
     * @param   aObserver       Observer to be unregistered
     */
    virtual void UnregisterConnectionObserverL( TUint aStreamId,
        MNcmConnectionObserver& aObserver ) = 0;

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
     */
    virtual void RegisterMessageObserverL( TUint aStreamId,
        MNcmMessageObserver& aObserver ) = 0;

    /**
     * Unregisters an observer for receiving incoming and outgoing message
     * notifications.
     *
     * @since   S60 v3.2
     * @param   aStreamId       A stream ID
     * @param   aObserver       Observer to be unregistered
     */
    virtual void UnregisterMessageObserverL( TUint aStreamId,
        MNcmMessageObserver& aObserver ) = 0;
                       
    /**
     * Sends a message from specified connection to network.
     *
     * @since   S60 v3.2
     * @param   aStreamId           The ID identifying stream
     * @param   aConnectionId       The ID identifying connection
     * @param   aMessage            A message to be copied and then sent asynchronously.
     * @param   aSenderObserver     Observer for this send.
     */
    virtual void SendL( TUint aStreamId, TUint aConnectionId,
        const TDesC8& aMessage,
        MNcmSenderObserver* aSenderObserver = NULL ) = 0;
 
    /**
     * Overloaded SendL
     * Sends a message from specified connection to network. 
     * Destination address parameter tells the destination address for the message. 
     *
     * @since   S60 v3.2
     * @param   aStreamId               The ID identifying stream
     * @param   aConnectionId           The ID identifying conenction
     * @param   aMessage                A message to be copied and
     *                                  then sent asynchronously.
     * @param   aDestinationAddress     Destination address
     * @param   aSenderObserver         Observer for this send.
     */
    virtual void SendL( TUint aStreamId, TUint aConnectionId,
        const TDesC8& aMessage, const TInetAddr& aDestinationAddress,
        MNcmSenderObserver* aSenderObserver = NULL ) = 0;

    /**
     * Overloaded SendL
     * Sends a message from connection which is found by nex hop address.
     * Destination address parameter tells the destination address for
     * the message. 
     *
     * @since   S60 v3.2
     * @param   aStreamId               The ID identifying stream
     * @param   aMessage                A message
     * @param   aNextHopAddress         Specifies the IP address of the
     *                                  next hop in the route
     *                                  will be used to find connection
     *                                  which will be used for sending.
     * @param   aDestinationAddress     Destination address.
     */
    virtual void SendL( TUint aStreamId, const TDesC8& aMessage,
        const TInetAddr& aNextHopAddress,
        const TInetAddr& aDestinationAddress ) = 0;
        
    /**
     * Overloaded SendL
     * Sends a message directly from connection. Will not offer this
     * message to any registered observers if aSendDirectly is set to
     * ETrue
     *
     * @since   S60 v3.2
     * @param   aStreamId               The ID identifying stream
     * @param   aConnectionId           The ID identifying conenction
     * @param   aMessage                A message
     * @param   aSendDirectly           if ETrue sends a message directly.
     *                                  Will not offer this message to any
     *                                  registered observers.
     *                                  normally and will be offered registered
     *                                  observers.
     * @param   aSenderObserver         Observer for this send.
     */
    virtual void SendL( TUint aStreamId, TUint aConnectionId,
        const TDesC8& aMessage, TBool aSendDirectly,
        MNcmSenderObserver* aSenderObserver = NULL ) = 0;
        
    /**
     * Cancels sender observer's message send processes.
     *
     * Client MUST call its destructor on this method and cancel all send
     * processes which it is observing( Client have used SendL with 
     * sender observer param ), otherwise there can be panic.
     *
     * @since   S60 v3.2
     * @param   aStreamId               The ID identifying stream
     * @param   aConnectionId           The ID identifying connection         
     * @param   aSenderObserver         Observer which observing message send processes 
     */
     virtual void CancelMessageSend( TUint aStreamId, TUint aConnectionId,
        const MNcmSenderObserver* aSenderObserver ) = 0;
        
    /**
     * Enables/disables data receiving for the specified connection.
     *
     * MNcmConnectionObserver::ConnectionNotify is called with event code
     * EReceivingActivated/EReceivingDeactivated when status change
     * has completed. While incoming data is received from network.
     *
     * Connection receives can be use for sending after successful sending activation.
     *
     * @since   S60 v3.2
     * @param   aStreamId           The ID identifying stream
     * @param   aConnectionId       The ID identifying connection
     * @param   aState              The receiving state
     */
    virtual void SetReceivingStateL( TUint aStreamId, TUint aConnectionId,
        TNATFWStreamingState aState ) = 0;
    
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
     */
    virtual void SetSendingStateL( TUint aStreamId, TUint aConnectionId,
        const TInetAddr& aDestAddr, TNATFWStreamingState aState ) = 0;
        
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
    virtual void SetAcceptedFromAddressL( TUint aStreamId, 
        TUint aConnectionId, const TInetAddr& aAddress ) = 0;
    };


#endif // M_MNCMCONNECTIONMULTIPLEXER_H
