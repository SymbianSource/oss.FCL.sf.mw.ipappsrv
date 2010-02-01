/*
* Copyright (c) 2006 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Connection handler is responsible for creating connections
*                to Connection Multiplexer and TURN Client. It is responsible
*                to handle data received from different observers it
*                implements. Connection handler implements 3 Connection
*                Multiplexer's observers and STUNClientObserver.
*
*/




#ifndef C_NATFWTURNCONNECTIONHANDLER_H
#define C_NATFWTURNCONNECTIONHANDLER_H


#include <e32base.h>
#include <e32std.h>
#include <es_sock.h>
#include <in_sock.h>

#include "mncmconnectionobserver.h"
#include "mncmincomingconnectionobserver.h"
#include "mncmoutgoingconnectionobserver.h"
#include "mncmmessageobserver.h"

#include "natfwstunclientobserver.h"
#include "natfwrefreshobserver.h"
#include "natfwtimerobserver.h"

// forward declarations
class MNATFWPluginObserver;
class MNcmConnectionMultiplexer;
class CNATFWCandidate;
class CNATFWTurnRefreshTimer;
class CSTUNRelayBinding;
class CSTUNClient;
class TStreamData;
class TConnectionData;
class CNATFWNatSettingsApi;
class MNATFWTurnSettings;
class CNATFWTurnActiveDestinationTimer;
class CTurnAsyncCallback;
class CNATFWPluginApi;
class MNATFWServerSettings;
class CTurnServerSettings;

/**
 *  Connection handler for TURN Plug-in.
 *
 *  Connection handler is responsible for creating connections to Connection
 *  Multiplexer and TURN Client. It creates TURN Binding and it is responsible
 *  to handle data received from different observers it implements. Connection
 *  handler implements 3 Connection Multiplexer's observers and
 *  STUNClientObserver.
 *
 *  @lib turnplugin.lib
 *  @since S60 v3.2
 */
class CNATFWTurnConnectionHandler : public CTimer,
                                    public MSTUNClientObserver,
                                    public MNcmConnectionObserver,
                                    public MNcmIncomingConnectionObserver,
                                    public MNcmOutgoingConnectionObserver,
                                    public MNcmMessageObserver,
                                    public MNATFWRefreshObserver,
                                    public MNATFWTimerObserver
    {
public:

    /**
     * Two-phased constructor.
     * @param aTurnPlugin The TURN plugin instance
     * @param aPluginObserver Observer for callbacks to STUN plug-in's client
     */
    static CNATFWTurnConnectionHandler* NewL(
        const CNATFWPluginApi& aTurnPlugin,
        MNATFWPluginObserver& aPluginObserver );
        
    /**
     * Two-phased constructor.
     * @param aTurnPlugin The TURN plugin instance
     * @param aPluginObserver Observer for callbacks to STUN plug-in's client
     */
    static CNATFWTurnConnectionHandler* NewLC(
        const CNATFWPluginApi& aTurnPlugin,
        MNATFWPluginObserver& aPluginObserver );


    ~CNATFWTurnConnectionHandler();

    /**
     * Initializes TURN Connection Handler.
     *
     * @since S60 v3.2
     * @param aIapId        IAP ID used in retrieving IAP settings from
     *                      NAT FW NAT Settings
     * @param aDomain       Domain name used in creating instance of NATFW
     *                      NAT Settings to get STUN server address and port.
     * @param aMultiplexer  Reference to the Connection Multiplexer
     */
    void PluginInitializeL( TUint32 aIapId,
                            const TDesC8& aDomain,
                            MNcmConnectionMultiplexer& aMultiplexer );

    /**
     * Finds out a STUN Server and establishes a connection to it.
     * 
     * MNATFWPluginObserver::Notify is called with event code EServerConnected
     * when connecting has completed.
     *
     * FetchCandidateL can be called after successful connecting.
     *
     * This is done asynchronously.
     * 
     * @since   S60 v3.2
     * @param   aSocketServ         The handle to socket server session
     * @param   aConnectionName     The unique name of the RConnection
     */
    void ConnectServerL( const RSocketServ& aSocketServ,
        const TName& aConnectionName );

    /**
     * Fetches available candidates for a specified stream using given
     * retransmission timeout for STUN transactions.
     * 
     * RTO value 0 indicates default retransmission timeout as specified
     * in STUN specification.
     * 
     * @since   S60 v3.2
     * @param   aStreamId       Stream identifier
     * @param   aRtoValue       Retransmission timeout in milliseconds
     * @param   aAddrFamily     KAFUnspec / KAfInet / KAfInet6
     * @param   aBaseAddr       Address from which to create connection
     */
    void FetchCandidateL( TUint aStreamId, TUint aRtoValue, TUint aAddrFamily,
        const TInetAddr& aBaseAddr );

    /**
     * Gets identifier for a connection that is associated with the
     * given local end point.
     * 
     * @since   S60 v3.2
     * @param   aLocalCandidate     The local end point for a media
     * @param   aStreamId           The stream identifier
     * @param   aConnectionId       Reference to found ID
     */
    void GetConnectionIdL( const CNATFWCandidate& aLocalCandidate,
        TUint aStreamId, TUint& aConnectionId );

    /**
     * Starts TURN Refresh timer. Sends periodically keepalive messages to
     * TURN client, which will keep the bindings to TURN Server active.
     *
     * @since   S60 v3.2
     */
    void StartTurnRefresh();

    /**
     * Creates TURN Binding and calls AllocateRequestL().
     *
     * @since S60 v3.2
     * @param aStreamId      Stream ID of current TURN Binding
     * @param aConnectionId  Connection ID of current TURN Binding
     */
    void CreateTURNBindingL( TUint aStreamId, TUint aConnectionId );

    /**
     * Enables/disables media receiving for the specified local candidate.
     * 
     * MNATFWPluginObserver::Notify is called with event code
     * EReceivingActivated/EReceivingDeactivated when status change has 
     * completed. While activated incoming data is delivered to the media
     * engine.
     * 
     * @since   S60 v3.2
     * @pre     FetchCandidate(/s) and possible ICE processing have completed
     * @param   aLocalCandidate The local end point for a media
     * @param   aState          The receiving state
     */
    void SetReceivingStateL( const CNATFWCandidate& aLocalCandidate,
        TNATFWStreamingState aState );

    /**
     * Once the client wants to primarily receive from one peer, it can send
     * a SetActiveDestination request.  All subsequent data received from
     * the active peer is forwarded directly to the client and vice versa,
     * except that it is wrapped or framed according to the protocol used
     * between the STUN relay client and STUN relay server.
     * The SetActiveDestination Request does not close other bindings. Data
     * to and from other peers is still wrapped in Send and Data indications
     * respectively.
     *
     * @since   S60 v3.2
     * @param   aLocalCandidate The local end point for a media
     * @param   aState          The sending state
     * @param   aDestAddr       The remote end point for a media
     */
    void SetActiveDestinationRequestL( const CNATFWCandidate& aLocalCandidate,
        TNATFWStreamingState aState, const TInetAddr& aDestAddr );

    /**
     * Validates that is outgoing message a TURN request or Send Indication.
     *
     * @since   S60 v3.2
     * @param   aMessage    Message from which will be validated if
     *                      it is TURN request or Send Indication
     * @return ETrue if the message is TURN request or Send Indication
     */
    TBool IsRequestOrIndicationL( const TDesC8& aMessage ) const;

    /**
     * Validates that is incoming message a TURN response.
     *
     * @since   S60 v3.2
     * @param   aMessage    Message from which will be validated if
     *                      it is TURN response
     * @return ETrue if the message is TURN response
     */
    TBool IsTurnResponseL( const TDesC8& aMessage ) const;

// From base class MStunClientObserver

    /**
     * Indicates that asynchronous initialization of the related
     * CSTUNClient has been completed and it can be used for 
     * creating CSTUNRelayBinding(s).
     *
     * @since S60 v3.2
     * @param aClient         Handle to STUN client
     * @param aCompletionCode Completion code
     */
    void STUNClientInitCompleted( const CSTUNClient& aClient,
                                  TInt aCompletionCode );

    /**
     * A TURN binding related event occurred. After this callback
     * the aBinding can be used again by calling its SendRequestL
     * function.
     *
     * @since S60 v3.2
     * @param aEvent   Binding event occurred
     * @param aBinding Handle to CBinding
     */
    void STUNBindingEventOccurredL( TSTUNBindingEvent aEvent,
                                    const CBinding& aBinding );

    /**
     * Error occurred during IP address resolution. This callback
     * will also be called if STUNBindingEventOccurredL leaves.
     *
     * @since S60 v3.2
     * @param aBinding Handle to CBinding
     * @param aError   Error code
     */
    void STUNBindingErrorOccurred( const CBinding& aBinding,TInt aError );

 // from base class MNcmConnectionObserver

    /**
     * Called by Connection Multiplexer when connection connecting completes.
     *
     * @since S60 v3.2
     * @param aStreamId     Stream of target of notify
     * @param aConnectionId Target of notify
     * @param aType         Type of notify
     * @param aError        Error code
     */
    void ConnectionNotify( TUint aStreamId,
                           TUint aConnectionId,
                           TConnectionNotifyType aType,
                           TInt aError );

// from base class MNcmIncomingConnectionObserver

    /**
     * Called by Connection Multiplexer when incoming message is found.
     *
     * @since   S60 v3.2
     * @param   aStreamId           The ID identifying stream
     * @param   aMessage            Message is a reference to constant message
     * @param   aConsumed           Flag to indicate is this message consumed
     * @param   aLocalAddr          A stream local address.
     * @param   aFromAddr           A remote source address. ( From socket )
     * @param   aPeerRemoteAddr     A peer remote address.
     * @param   aConsumed           A reference to a boolean variable which MUST
     *                              be set by observer: ETrue is message was 
     *                              consumed, EFalse otherwise.
     */
    void IncomingMessageL( TUint aStreamId, const TDesC8& aMessage,
        const TInetAddr& aLocalAddr, const TInetAddr& aFromAddr,
        const TInetAddr& aPeerRemoteAddr, TBool& aConsumed );

// from base class MNcmOutgoingConnectionObserver

    /**
     * Called by Connection Multiplexer when outgoing message is found.
     *
     * @since S60 v3.2
     * @param aStreamId             Stream where message was found
     * @param aConnectionId         Connection where message was found
     * @param aDestinationAddress   Destination address where the packet
     *                              is being sent to. If this is set then
     *                              connection ID is ignored       
     * @param aMessage              Message is a reference to constant message
     *                              which was found
     * @param aConsumed             Is reference to a boolean variable which
     *                              MUST be set by observer: ETrue is message
     *                              was consumed, EFalse otherwise.
     */
    void OutgoingMessageL( TUint aStreamId, TUint aConnectionId,
        const TInetAddr& aDestinationAddress, const TDesC8& aMessage,
        TBool& aConsumed );

// from base class MNcmMessageObserver

    /**
     * Called by Connection Multiplexer when incoming message is found.
     * Allows connection observer to observe and make modifications to found
     * message by offering an own copy of content with modification(s).
     *
     * TURN Plug-in must fill the peer address, because only TURN Plug-in can
     * know currently activated destination.
     *
     * @since   S60 v3.2
     * @param   aStreamId           The ID identifying stream
     * @param   aMessage            Message is a reference to constant message
     * @param   aLocalAddr          A stream local address.
     * @param   aFromAddr           A remote source address. ( From socket )
     * @param   aPeerRemoteAddr     A peer remote address.
     * @return  Pointer to a heap descriptor with new content for message,
     *          otherwise NULL is returned. Ownership of the heap descriptor
     *          is transferred to connection multiplexer ( from observer ).
     */
    HBufC8* IncomingMessageNotify( TUint aStreamId,
                                   const TDesC8& aMessage,
                                   const TInetAddr& aLocalAddr,
                                   const TInetAddr& aFromAddr,
                                   TInetAddr& aPeerRemoteAddr );

// from base class MNcmMessageObserver

    /**
     * Called by Connection Multiplexer when outgoing message is found.
     * Allows connection observer to observe and make modifications to found
     * message by offering an own copy of content with modification(s).
     *
     * @since S60 v3.2
     * @param aStreamId             The ID identifying stream
     * @param aConnectionId         The ID identifying connection
     * @param aDestinationAddress   Destination address where the packet
     *                              is being sent to.   
     * @param aMessage              Message is a reference to constant message
     *                              which was found
     * @return pointer to a heap descriptor with new content for message,
     *         otherwise NULL is returned. Ownership of the heap descriptor is
     *         transferred to multiplexer (from observer).
     */
    HBufC8* OutgoingMessageNotify( TUint aStreamId,
                                   TUint aConnectionId,
                                   const TInetAddr& aDestinationAddress,
                                   const TDesC8& aMessage );

 // from base class MNATFWRefreshObserver

    /**
     * Sends binding requests to TURN Bindings.
     *
     * @since S60 v3.2
     */
    void BindingRefreshL();

// from base class MNATFWTimerObserver

    /**
     * Notifies when active destination timer has triggered.
     *
     * @since S60 v3.2
     * @param  aStreamId        To identify the binding that the
     *                          triggering was related to
     * @param  aConnectionId    To identify the binding that the
     *                          triggering was related to
     */
    void TimerTriggeredL( TUint aStreamId, TUint aConnectionId );

protected:

    /**
     * from CActive
     */
    void DoCancel();

    /**
     * from CActive
     */
    void RunL();

private:

    CNATFWTurnConnectionHandler( const CNATFWPluginApi& aTurnPlugin,
        MNATFWPluginObserver& aPluginObserver );

    void ConstructL();

    void DeleteStream( TUint aStreamInd, TBool aRemoveMuxConn );

    HBufC8* HandleIncomingMessageL( TUint aStreamId, const TDesC8& aMessage, 
        const TInetAddr& aLocalAddr, const TInetAddr& aFromAddr, 
        TInetAddr& aPeerRemoteAddr );

    TInt IndexByStreamId( TUint aStreamId );

    TConnectionData* ConnectionById( TUint aStreamInd, TUint aConnectionId );

    TConnectionData* ConnectionByIndex( TUint aStreamInd,
        TUint aConnectionInd );

    void GenerateServerListL();

    void TryNextServerL();
    
    TConnectionData* FindConnection( TUint aStreamId,
        const TInetAddr& aLocalAddr );

    void ExecuteCallBack( );

private:

    /**
     * Flag defining whether TURN refresh has been started
     */
    TBool iTurnRefreshStarted;

    /**
     * IAP ID
     */
    TUint32 iIapId;

    /**
     * Currently selected index in list of servers ( iServerList )
     */    
    TInt iServerIndex;
    
    /**
     * Handle to the socket server.
     */
    RSocketServ iSocketServ;

    /**
     * Handle to the connection.
     * Own.
     */
    RConnection iConnection;
    
    /**
     * Pointer to domain name data.
     * Own.
     */
    HBufC8* iDomain;

    /**
     * Array for storing data for each stream
     */
    RArray<TStreamData> iStreamArray;

    
// Instances from other C -classes

    /**
     * STUN and TURN client handle.
     * Own.
     */
    CSTUNClient* iStunClient;

    /**
     * Set up stream for client.
     * Not own.
     */
    MNcmConnectionMultiplexer* iConnMux;

    /**
     * Sends keepalive messages periodically.
     * Own.
     */
    CNATFWTurnRefreshTimer* iTurnRefreshTimer;

    /**
     * Timer for setting Active Destination.
     * Own.
     */
    CNATFWTurnActiveDestinationTimer* iActiveDestinationTimer;

    /**
     * Pointer to timer for STUN Client.
     * Own.
     */
    CDeltaTimer* iTimerServ;

    /**
     * Pointer to NAT Settings.
     * Own.
     */
    CNATFWNatSettingsApi* iNatSettings;

    /**
     * Pointer to TURN related NAT Settings.
     * Not own.
     */
    MNATFWTurnSettings* iTurnSettings;

    /**
     * TURN UDP Refresh interval.
     */
    TUint iTurnRefreshInterval;

    /**
     * TURN Plug-in
     * Not own.
     */
    const CNATFWPluginApi& iTurnPlugin;

    /**
     * Async callback handler
     * own.
     */
    CTurnAsyncCallback* iAsyncCallback;

    /**
     * Array of server related settings
     * Own.
     */    
    RPointerArray<CTurnServerSettings> iServerList;
    
// Instances from other M classes

    /**
     * Sends notifications to NAT Plug-in observer.
     */
    MNATFWPluginObserver& iPluginObserver;
    
    enum TCallBackCmdType
        {
        EConnectServer,
        EFetchCandidate,
        ESetSendingStatus
        };
    
    class TCallBackCmd
        {
        public:
            TCallBackCmd( TUint aStreamId, TCallBackCmdType aType, 
                    TInt aData, TAny* aDataPtr = NULL )
                :
                iStreamId( aStreamId ),
                iType( aType ),
                iDataPtr( aDataPtr ),
                iData( aData ) 
                    {
                    
                    }
        public:
            
            TUint iStreamId;
            TCallBackCmdType iType;
            TAny* iDataPtr;
            TInt iData;
        
        private:
            TCallBackCmd();
        };
    
    RArray<TCallBackCmd> iCallBackCmds;

    };

#endif // C_NATFWTURNCONNECTIONHANDLER_H
