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
* Description:    Connection multiplexer connection abstraction.
*
*/




#ifndef C_CNCMCONNECTION_H
#define C_CNCMCONNECTION_H

#include <e32base.h>
#include <in_sock.h>

#include "mncmsourceobserver.h"
#include "mncmreceiversenderobserver.h"
#include "natfwconnectivityapidefs.h"

class MNcmMultiplexerConnectionObserver;
class CNcmConnectionObserverHandler;
class CNcmSender;
class CNcmReceiver;

/**
 * Connection multiplexer connection abstraction.
 *
 *  @lib connectionmux.lib
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CNcmConnection ) : public CActive,
    public MNcmSourceObserver, public MNcmReceiverSenderObserver
    {

public:

    /**
     * Two-phased constructor
     *
     * @param   aSessionId                     The ID identifying session
     * @param   aStreamId                      The ID identifying stream
     * @param   aConnectionId                  The ID identifying connection
     * @param   aLocalAddr                     Local IP address
     * @param   aObserver                      Observer
     * @param   aConnectionObserverHandler     Observer handler
     * @param   aQos                           The desired quality of service
     * @param   aSocketServ                    Socket server
     * @param   aConnection                    RConnection
     * @param   aProtocol                      KProtocolInetUdp / KProtocolInetTcp
     * @param   aReuseAddress                  If ETrue reuse local address
     */
    static CNcmConnection* NewL( TUint aSessionId,
        TUint aStreamId,TUint aConnectionId,
        const TInetAddr& aLocalAddr,
        MNcmMultiplexerConnectionObserver& aObserver,
        CNcmConnectionObserverHandler& aConnectionObserverHandler,
        TInt aQos,
        RSocketServ& aSocketServ,
        RConnection& aConnection,
        TUint aProtocol,
        TBool aReuseAddress );

    /**
     * Two-phased constructor
     *
     * @param   aSessionId                     The ID identifying session
     * @param   aStreamId                      The ID identifying stream
     * @param   aConnectionId                  The ID identifying connection
     * @param   aLocalAddr                     Local IP address
     * @param   aObserver                      Observer
     * @param   aConnectionObserverHandler     Observer handler
     * @param   aQos                           The desired quality of service
     * @param   aSocketServ                    Socket server
     * @param   aConnection                    RConnection
     * @param   aProtocol                      KProtocolInetUdp / KProtocolInetTcp
     * @param   aReuseAddress                  If ETrue reuse local address
     */
    static CNcmConnection* NewLC( TUint aSessionId,
        TUint aStreamId,TUint aConnectionId,
        const TInetAddr& aLocalAddr,
        MNcmMultiplexerConnectionObserver& aObserver,
        CNcmConnectionObserverHandler& aConnectionObserverHandler,
        TInt aQos,
        RSocketServ& aSocketServ,
        RConnection& aConnection,
        TUint aProtocol,
        TBool aReuseAddress );

    /**
     * Destructor
     */
    virtual ~CNcmConnection();

    /**
     * Establishes TCP connection. Connection will be established in specified
     * direction.
     *
     * @since   S60 v3.2
     * @param   aConfig         TCP connection setup configuration
     * @param   aDestAddr       The destination address for active connection
     * @return  void
     */
    void OpenTcpConnectionL( TNATFWTcpConnectionSetup aConfig,
        const TInetAddr& aDestAddr );

    /**
     * Closes connection or cancels in-progress connection process.
     *
     * @since   S60 v3.2
     * @return  void
     */
    void CloseTcpConnectionL();
            
    /**
     * Set receiving state
     *
     * @since   S60 v3.2
     * @param   aState             Streaming status
     * @return  void
     */
    void SetReceivingStateL( TNATFWStreamingState aState );
    
    /**
     * Sets sending state
     *
     * @since   S60 v3.2
     * @param   aDestAddr           Destination address
     * @param   aState              Streaming state
     * @return  void
     */
    void SetSendingStateL( const TInetAddr& aDestAddr,
        TNATFWStreamingState aState );
    
    /** 
     * Check sender validity sending.
     *
     * @since   S60 v3.2
     * @param   aDestinationAddr    Destination address
     * @return  ETrue if ready to send,
     *          Efalse if not ready
     */  
    TBool CheckSenderValidityToSending( const TInetAddr& aDestinationAddr );
    
    /** 
     * Get destination address.
     *
     * @since   S60 v3.2  
     * @return  Destination address
     */   
    const TInetAddr& DestinationAddress() const;
    
    /**
     * Cancels the specified message send processes.
     *
     * @since   S60 v3.2       
     * @param   aSenderObserver         Observer
     * @return  void
     */
    void CancelMessageSendL(
        const MNcmSenderObserver* aSenderObserver );
    
    /**
     * Returns whether this destination can be used to send to arbitrary
     * remote address.
     *
     * @since   S60 v3.2       
     * @return  ETrue if generic destination, EFalse otherwise
     */
    TBool IsGenericDestination() const;

    /**
     * Sets address from which incoming data is accepted from this moment
     * onwards. Binds connection to the specified remote address. Unbinding
     * can be done by setting unspecified address.
     *
     * @since   S60 v3.2
     * @param   aAddress            The remote address
     */
    void SetAcceptedFromAddressL( const TInetAddr& aAddress );
    
    /**
     * Method to send from this connection.
     *
     * @since   S60 v3.2
     * @param   aMessage                Message to send
     * @param   aDestinationAddress     Destination address
     * @param   aSenderObserver         Sender observer for this send
     */
    void Send( const TDesC8& aMessage,
        const TInetAddr& aDestinationAddress,
        MNcmSenderObserver* aSenderObserver );

    /**
     * Send directly from this connection to network
     *
     * @since   S60 v3.2
     * @param   aMessage            Message is a reference to message which
     *                              is copied by caller
     * @param   aSenderObserver     Sender observer for this sending request
     * @return  void
     */
    void SendDirectlyToNetwork( const TDesC8& aMessage,
        MNcmSenderObserver* aSenderObserver  );
                
    /**
     * Returns connectionId ID
     *
     * @since   S60 v3.2
     * @return connection ID
     */
    TUint ConnectionId() const;

    /** 
     * Return the local IP address
     *
     * @since   S60 v3.2
     * @return  Local IP address.
     */
    TInetAddr& LocalAddress();
   
// from base class MNcmSourceObserver

    /**
     * From MNcmrSourceObserver.
     *
     * @since S60 v3.2
     * @param aMessage          The outgoing message
     * @param aSenderObserver   Observer for this send
     * @param aDestAddr         Destination address
     */
    void OutgoingMessageAvailable( const TDesC8& aMessage,
        MNcmSenderObserver* aSenderObserver, const TInetAddr& aDestAddr );

// from base class MNcmReceiverSenderObserver

    /**
     * Called by socket receiver while message is found
     *
     * @since S60 v3.2
     * @return void
     */
    void MessageAvailable();
    
    /**
     * Called by socket receiver/sender while error occurs on receiving
     *
     * @since   S60 v3.2
     * @param   aError              Error code
     * @return  void
     */
    void Error( TInt aError );

protected:
    
// from base class CActive

    void RunL();
    
    void DoCancel();
            
private: // Private constructors

    /** Identifies states for TCP connection  **/
    enum TTcpConnectionState
        {
        ETcpConnectionUnspecifiedState            =   1,
        ETcpConnectionConnectingState             =   2,
        ETcpConnectionConnectedState              =   3
        };
        
    CNcmConnection( TUint aSessionId, TUint aStreamId,
        TUint aConnectionId,
        const TInetAddr& aLocalAddr,
        MNcmMultiplexerConnectionObserver& aObserver,
        CNcmConnectionObserverHandler& 
        aConnectionObserverHandler,
        TInt aQos,
        RSocketServ& aSocketServ,
        RConnection& aConnection,
        TUint aProtocol,
        TBool aReuseAddress );

    void ConstructL();

    void InitializeUdpL();
    
    void InitializeTcpL();

    void ActivateReceivingL();
    
    void DeactivateReceivingL();
    
    void ActivateSendingL( const TInetAddr& aDestAddr );
    
    void DeactivateSendingL();
    
private: // data

    /**
     * Session ID 
     */
    TUint iSessionId;
    
    /**
     * Stream ID
     */
    TUint iStreamId;
    
    /**
     * Connection ID associated with this sub stream
     */
    TUint iConnectionId;
    
    /**
     * Local address
     */
    TInetAddr iLocalAddr;
    
    /**
     * Observer for this connection
     * Not own.
     */
    MNcmMultiplexerConnectionObserver& iObserver;
    
    /**
     * Handler for connection observers
     * Not own.
     */
    CNcmConnectionObserverHandler& iConnectionObserverHandler;
    
    /**
     * TOS bit setting
     */
    TInt iIpTOS;
 
    /**
     * Protocol, KProtocolInetUdp / KProtocolInetTcp
     */
    TUint iProtocol;
    
    /**
     * ETrue if local address should be reuse.
     */
    TBool iReuseAddress;
    
    /**
     * Socket for this connection (Multiplexer destination)
     * Own.
     */
    RSocket iSocket;
    
    /**
     * Socket for listening incoming TCP connection requests
     * Own.
     */
    RSocket iListenSocket;  
   
    /**
     * Socket for this connection (Multiplexer destination)
     * Not own.
     */
    RSocketServ& iSocketServ;
    
    /**
     * Socket for this connection (Multiplexer destination)
     * Not own.
     */
    RConnection& iConnection;
    
    /**
     * Sender for connection
     * Own.
     */
    CNcmSender* iSender;

    /**
     * Receiver for connection
     * Own.
     */
    CNcmReceiver* iReceiver;
    
    /**
     * Destiantion address.
     */    
    TInetAddr iDestinationAddress;
              
    /**
     * Connection receiver state
     */
    TNATFWStreamingState iReceiverState;
    
    /**
     * Connection sender state
     */
    TNATFWStreamingState iSenderState;
    
    /**
     * TCP connection state
     */
    TTcpConnectionState iTcpConnectionState;
    
    /**
     * TCP connecting type
     */
    TNATFWTcpConnectionSetup iConnSetupConf;
    
    /**
     * Boolean variable, which indicates is listening socket in listening state
     */        
    TBool iIsListening;
    
    /**
     * Indicates whether this destination can be used to send to the arbitrary
     * remote address.
     */
    TBool iIsGenericDestination;
    
    /**
     * Remote address from which to accept data.
     */
    TInetAddr iAcceptedFromAddr;
    
    /**
     * Message
     */
    HBufC8* iMessage;

    };


#endif // C_CNCMCONNECTION_H

