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
*                to Connection Multiplexer and STUN Client. It is responsible
*                to handle data received from different observers it
*                implements. Connection handler implements 2 Connection
*                Multiplexer's observers and STUNClientObserver.
*
*/




#ifndef C_NATFWSTUNCONNECTIONHANDLER_H
#define C_NATFWSTUNCONNECTIONHANDLER_H


#include <e32base.h>
#include <e32std.h>
#include <in_sock.h>

#include "mncmconnectionobserver.h"
#include "mncmincomingconnectionobserver.h"

#include "natfwstunclientobserver.h"
#include "natfwrefreshobserver.h"
#include "natfwconnectivityapidefs.h"

// forward declarations
class MNATFWPluginObserver;
class MNcmConnectionMultiplexer;
class CNATFWCandidate;
class CNATFWStunRefreshTimer;
class CSTUNBinding;
class CSTUNClient;
class CBinding;
class TStreamData;
class TConnectionData;
class CNATFWNatSettingsApi;
class MNATFWStunSettings;
class CPeriodic;
class CStunAsyncCallback;
class CNATFWPluginApi;
class CStunServerSettings;

/**
 *  Connection handler for STUN Plug-in
 *
 *  Connection handler is responsible for creating connections to Connection
 *  Multiplexer and STUN Client. It creates STUN Binding and it is responsible
 *  to handle data received from different observers it implements. Connection
 *  handler implements 2 Connection Multiplexer's observers and
 *  STUNClientObserver.
 *  
 *  @lib stunplugin.lib
 *  @since S60 v3.2
 */
 
class CNATFWStunConnectionHandler : public CTimer,
                                    public MSTUNClientObserver,
                                    public MNcmIncomingConnectionObserver,
                                    public MNcmConnectionObserver,
                                    public MNATFWRefreshObserver
    {
public:

    /**
     * Two-phased constructor.
     * @param aStunPlugin The STUN plugin instance
     * @param aPluginObserver Observer for callbacks to STUN plug-in's client
     */
    static CNATFWStunConnectionHandler* NewL(
        const CNATFWPluginApi& aStunPlugin,
        MNATFWPluginObserver& aPluginObserver );

    /**
     * Two-phased constructor.
     * @param aStunPlugin The STUN plugin instance
     * @param aPluginObserver Observer for callbacks to STUN plug-in's client
     */
    static CNATFWStunConnectionHandler* NewLC(
        const CNATFWPluginApi& aStunPlugin,
        MNATFWPluginObserver& aPluginObserver );

    /**
     * Destructor.
     */
    ~CNATFWStunConnectionHandler();

    /**
     * Initializes STUN Connection Handler.
     *
     * @since S60 v3.2
     * @param aIapId        IAP ID used in retrieving IAP settings from
     *                      NAT FW NAT Settings
     * @param aDomain       Domain name used in creating instance of NATFW
     *                      NAT Settings to get STUN server address and port.
     * @param aMultiplexer  Reference to the Connection Multiplexer
     * @return void
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
     * @return  void
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
     * @return void
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
     * Starts STUN Refresh timer. Sends periodically keepalive messages to
     * STUN client, which will keep the bindings to STUN Server active.
     *
     * @since S60 v3.2
     * @return void
     */
    void StartStunRefresh();

    /**
     * Creates STUN Binding and calls SendRequestL().
     *
     * @since S60 v3.2
     * @param aStreamId     Stream ID of current STUN Binding
     * @param aConnectionId Connection ID of current STUN Binding
     * @return void
     */
    void CreateSTUNBindingL( TUint aStreamId, TUint aConnectionId );

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
     * Enables/disables media sending from the specified local candidate.
     * 
     * MNATFWPluginObserver::Notify is called with event code
     * ESendingActivated/ESendingDeactivated when status change has
     * completed. While activated outgoing data from media engine is delivered
     * to the specified remote address.
     * 
     * @since   S60 v3.2
     * @pre     FetchCandidate(/s) and possible ICE processing have completed
     * @param   aLocalCandidate The local end point for a media
     * @param   aState          The sending state
     * @param   aDestAddr       The remote end point for a media
     */
    void SetSendingStateL( const CNATFWCandidate& aLocalCandidate,
        TNATFWStreamingState aState, const TInetAddr& aDestAddr );

// From base class MStunClientObserver

    /**
     * Indicates that asynchronous initialization of the related
     * CSTUNClient has been completed and it can be used for
     * creating CSTUNBinding(s).
     *
     * @since S60 v3.2
     * @param aClient         Handle to STUN client
     * @param aCompletionCode Completion code
     * @return void
     */
    void STUNClientInitCompleted( const CSTUNClient& aClient,
                                  TInt aCompletionCode );

    /**
     * A STUN binding related event occurred. After this callback
     * the aBinding can be used again by calling its SendRequestL
     * function.
     *
     * @since S60 v3.2
     * @param aEvent   Binding event occurred
     * @param aBinding Handle to CBinding
     * @return void
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
     * @return void
     */
    void STUNBindingErrorOccurred( const CBinding& aBinding,TInt aError );

// from base class MNcmIncomingConnectionObserver

    /**
     * Called by Connection Multiplexer when incoming message is found.
     *
     * @since   S60 v3.2
     * @param   aStreamId           The ID identifying stream
     * @param   aMessage            Message is a reference to constant message
     * @param   aLocalAddr          A stream local address.
     * @param   aFromAddr           A remote source address. ( From socket )
     * @param   aPeerRemoteAddr     A peer remote address.
     * @param   aConsumed           Flag to indicate is this message consumed
     * @return  void
     */
    void IncomingMessageL( TUint aStreamId,
                              const TDesC8& aMessage,
                              const TInetAddr& aLocalAddr,
                              const TInetAddr& aFromAddr,
                              const TInetAddr& aPeerRemoteAddress,
                              TBool& aConsumed );

 // from base class MNcmConnectionObserver

    /**
     * Called by Connection Multiplexer when connecting connection completes.
     *
     * @since S60 v3.2
     * @param aStreamId     Stream of target of notify
     * @param aConnectionId Target of notify
     * @param aType         Type of notify
     * @param aError        Error code
     * @return void
     */
    void ConnectionNotify( TUint aStreamId,
                           TUint aConnectionId,
                           TConnectionNotifyType aType,
                           TInt aError );

 // from base class MNATFWRefreshObserver

    /**
     * Sends binding request to STUN Binding.
     *
     * @since S60 v3.2
     * @return void
     */
    void BindingRefreshL();

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

    CNATFWStunConnectionHandler( const CNATFWPluginApi& aStunPlugin,
        MNATFWPluginObserver& aPluginObserver );

    void ConstructL( MNATFWPluginObserver& aPluginObserver );

    void DeleteStream( TUint aStreamInd, TBool aRemoveMuxConn );

    TInt IndexByStreamId( TUint aStreamId );

    void GenerateServerListL();

    void TryNextServerL();

    TConnectionData* ConnectionById( TUint aStreamInd, TUint aConnectionId );

    TConnectionData* ConnectionByIndex( TUint aStreamInd,
        TUint aConnectionInd );

    void ExecuteCallBack( );
    
private:

    /**
     * IAP ID
     */
    TUint32 iIapId;

    /**
     * Flag defining whether STUN refresh has been started.
     */
    TBool iStunRefreshStarted;

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
     * Array for storing data for each stream.
     */
    RArray<TStreamData> iStreamArray;

    /**
     * Pointer to domain name data.
     * Own.
     */
    HBufC8* iDomain;

// Instances from other C classes

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
    CNATFWStunRefreshTimer* iStunRefreshTimer;

    /**
     * Sends notifications to NAT Plug-in observer.
     */
    MNATFWPluginObserver& iPluginObserver;

    /**
     * Pointer to timer for STUN Client
     * Own.
     */
    CDeltaTimer* iTimerServ;

    /**
     * Pointer to NAT Settings.
     * Own.
     */
    CNATFWNatSettingsApi* iNatSettings;
    
    /**
     * Pointer to NAT Settings.
     * Not own.
     */
    MNATFWStunSettings* iStunSettings;
    
    /**
     * STUN UDP Refresh interval.
     */
    TUint iStunRefreshInterval;

    /**
     * Async callback handler
     * own.
     */
    CStunAsyncCallback* iAsyncCallback;

    /**
     * STUN plugin
     * Not own.
     */
    const CNATFWPluginApi& iStunPlugin;

    /**
     * Array of STUN server related settings
     * Own.
     */
    RPointerArray<CStunServerSettings> iServerList;

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

#endif // C_NATFWSTUNCONNECTIONHANDLER_H
