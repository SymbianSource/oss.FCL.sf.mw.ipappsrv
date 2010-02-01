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
* Description:    NAT FW session abstraction
*
*/




#ifndef C_CNATFWSESSION_H
#define C_CNATFWSESSION_H

#include <e32base.h>
#include "natfwconnectivityapidefs.h"
#include "mnatfwpluginobserver.h"

class CNATFWCandidate;
class CNATFWCandidatePair;
class CNATFWStream;
class CNcmConnectionMultiplexer;
class MNATFWConnectivityObserver;
class MNATFWRegistrationController;
class CNATFWPluginApi;
class CNATFWNatSettingsApi;
class CNATFWPluginApi;
class CDesC8Array;
class CNatFwAsyncCallback;

/**
 *  NAT FW session abstraction.
 *
 *  Provides network connection layer and operation context for
 *  the NAT FW streams. Can be mapped e.g. to the SDP-session.
 *
 *  @lib natconfw.lib
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CNATFWSession ) : public CBase, public MNATFWPluginObserver
    {

    friend class UT_CNATFWClient;

public:

    static CNATFWSession* NewL(
        CNcmConnectionMultiplexer& aMultiplexer,
        MNATFWRegistrationController& aController,
        CNatFwAsyncCallback& aAsyncCallback,
        const TDesC8& aDomain,
        TUint32 aIapId );

    static CNATFWSession* NewLC(
        CNcmConnectionMultiplexer& aMultiplexer,
        MNATFWRegistrationController& aController,
        CNatFwAsyncCallback& aAsyncCallback,
        const TDesC8& aDomain,
        TUint32 aIapId );

    virtual ~CNATFWSession();

    /**
     * Returns the session identifier.
     *
     * @since   S60 v3.2
     * @return  The session identifier
     */
    TUint SessionId() const;

    /**
     * Loads a NAT protocol plugin to the session. Old plugin is destroyed if
     * it exists. In this case NAT operations for the session must be started
     * anew ( FetchCandidate(s) and possible ICE processing ).
     *
     * Given NAT protocol plugins are tried to be loaded in order until a
     * working one is found.
     *
     * @since   S60 v3.2
     * @param   aPlugins        Array containing identifiers for available
     *          NAT-protocol plugins in preferred order. E.g.
     *          "exampleprovider.stun".
     * @param   aLoadedPluginInd Index to the aPlugins array telling actually
     *          loaded plugin.
     * @leave   KErrNotFound NAT-plugin was not found
     */
    void LoadPluginL( const CDesC8Array& aPlugins, TInt& aLoadedPluginInd );


    /**
     * Resolves public IP address to be used in the communication between
     * peers. MNATFWConnectivityObserver::NewLocalCandidateFound
     * is called when a public IP has been resolved.
     *
     * @since   S60 v3.2
     * @param   aStreamId       The ID identifying stream
     * @param   aAddrFamily     KAFUnspec / KAfInet / KAfInet6
     */
    void FetchCandidateL( TUint aStreamId, TUint aAddrFamily );
    
    /**
     * ICE spesific function. Fetches IP address candidates for the
     * communication between peers. Client is responsible for providing a
     * mapping between components of media stream through the collection ID
     * parameter. MNATFWConnectivityObserver::NewLocalCandidateFound
     * is called whenever a new candidate has been found.
     *
     * @since   S60 v3.2
     * @param   aStreamId       The ID identifying stream
     * @param   aCollectionId   The stream collection identifier
     * @param   aComponentId    The media component identifier
     * @param   aAddrFamily     KAFUnspec / KAfInet / KAfInet6
     */
    void FetchCandidatesL( TUint aStreamId, TUint aCollectionId,
        TUint aComponentId, TUint aAddrFamily );
           
    /**
     * Creates a new stream for the client. On return the client receives a
     * stream identifier.
     *
     * @since   S60 v3.2
     * @param   aProtocol       KProtocolInetUdp / KProtocolInetTcp
     * @param   aQoS            The desired quality of service.
     * @return  The ID for the created stream
     */
    TUint CreateStreamL( TUint aProtocol, TInt aQoS );

    /**
     * ICE specific function.
     * Sets the role of local ICE agent. In role-conflict situation given role
     * will be silently changed.
     *
     * @since   S60 v3.2
     * @param   aRole           The role
     */
    void SetRoleL( TNATFWIceRole aRole );

    /**
     * ICE specific function.
     * Performs connectivity checks between the local candidates and the
     * remote candidates. MNATFWConnectivityObserver::NewCandidatePairFound
     * is called whenever working connection between local and remote
     * candidate is found.
     *
     * @since   S60 v3.2
     * @param   aRemoteCandidates   The remote candidate array
     */
    void PerformConnectivityChecksL(
        RPointerArray<CNATFWCandidate>& aRemoteCandidates );

    /**
     * ICE specific function.
     * Updates ICE processing for a session with the candidate pairs selected
     * by the controlling peer. If ICE processing for a stream is completed,
     * update for that is silently ignored.
     *
     * ICE restart is handled by setting new role and credentials and
     * re-starting connectivity checks with PerformCandidateChecksL.
     *
     * Adding new streams does not differ from initial operation.
     * Removing of streams is handled with CloseStreamL.
     *
     * @since   S60 v3.2
     * @pre     ICE processing is started with PerformConnectivityChecksL
     * @param   aPeerSelectedPairs  Peer selected candidate pairs
     * @leave   KErrNotSupported Loaded NAT Protocol plugin does not support
     * operation.
     * @post    ICE processing is continued with new parameters
     */
    void UpdateIceProcessingL(
        RPointerArray<CNATFWCandidatePair>& aPeerSelectedPairs );

    /**
     * ICE specific function.
     * Updates ICE processing for a session with an updated set of remote
     * candidates. If ICE processing for a stream is completed, update for
     * that is silently ignored. New remote candidates will be included in
     * connectivity tests from this point onwards.
     *
     * ICE restart is handled by setting new role and credentials and
     * re-starting connectivity checks with PerformCandidateChecksL.
     *
     * Adding new streams does not differ from initial operation.
     * Removing of streams is handled with CloseStreamL.
     *
     * @since   S60 v3.2
     * @pre     ICE processing is started with PerformConnectivityChecksL
     * @param   aRemoteCands        All remote candidates known currently
     * @leave   KErrNotSupported Loaded NAT Protocol plugin does not support
     * operation.
     * @post    ICE processing is continued with new parameters
     */
    void UpdateIceProcessingL(
        RPointerArray<CNATFWCandidate>& aRemoteCands );

    /**
     * Closes the stream from the session.
     *
     * @since   S60 v3.2
     * @pre     Streaming is disabled with SetReceivingStateL/SetSendingStateL
     * @param   aStreamId           The ID identifying stream
     */
    void CloseStreamL( TUint aStreamId );

    /**
     * Returns stream by identifier.
     *
     * @since   S60 v3.2
     * @param   aStreamId           The ID identifying stream
     * @return  Stream or NULL if not found
     */
    CNATFWStream* StreamById( TUint aStreamId );

    /**
     * Returns stream by identifier. Leaves with KErrNotFound if stream
     * isn't found.
     *
     * @since   S60 v3.2
     * @param   aStreamId           The ID identifying stream
     * @return  Stream
     */
    CNATFWStream* StreamByIdL( TUint aStreamId );

    /**
     * From MNATFWPluginObserver.
     * Called when an error within a stream has occured.
     *
     * @since   S60 v3.2
     * @param   aPlugin         The plugin raising event
     * @param   aStreamId       The ID identifying stream
     * @param   aErrorCode      Standard system wide error code
     */
    void Error( const CNATFWPluginApi& aPlugin,
        TUint aStreamId, TInt aErrorCode );

    /**
     * From MNATFWPluginObserver.
     * Notifies the client of plugin events.
     *
     * @since   S60 v3.2
     * @param   aPlugin         The plugin raising event
     * @param   aStreamId       The ID identifying stream
     * @param   aEvent          The event
     * @param   aErrCode        Standard system wide error code
     */
    void Notify( const CNATFWPluginApi& aPlugin,
        TUint aStreamId, TNATFWPluginEvent aEvent, TInt aErrCode );

    /**
     * From MNATFWPluginObserver.
     * Called when working candidate pair has been found. Ownership of
     * the candidate pair is trasferred.
     *
     * @since   S60 v3.2
     * @param   aPlugin         The plugin raising event
     * @param   aCandidatePair  The candidate pair which was found
     */
    void NewCandidatePairFound(
        const CNATFWPluginApi& aPlugin,
        CNATFWCandidatePair* aCandidatePair );

    /**
     * From MNATFWPluginObserver.
     * Called when a new local candidate has been found. Ownership of the
     * candidate is trasferred.
     *
     * @since   S60 v3.2
     * @param   aPlugin         The plugin raising event
     * @param   aCandidate      The local candidate that was found
     */
    void NewLocalCandidateFound(
        const CNATFWPluginApi& aPlugin,
        CNATFWCandidate* aCandidate );

private:

    CNATFWSession(
        CNcmConnectionMultiplexer& aMultiplexer,
        MNATFWRegistrationController& aController,
        CNatFwAsyncCallback& aAsyncCallback,
        TUint32 aIapId );

    void ConstructL( const TDesC8& aDomain );

    void HandleQueuedItems();

    void DoNotify(
        TUint aStreamId,
        MNATFWConnectivityObserver::TNATFWConnectivityEvent aEvent,
        TInt aErrCode,
        TAny* aEventData );

private:

    /** Identifies states for Server connection  **/
    enum TServerConnectionState
        {
        EConnectionUnspecified    =   1,
        EConnectionInProgress     =   2,
        EConnectionConnected      =   3,
        EConnectionFailed         =   4
        };

    class TStatusCounter
        {
    public: // Constructors and destructor

    inline TStatusCounter() :
        iActivatedCount( 0 ),
        iStreamId( 0 ),
        iErrorOccurred( EFalse )
        {};

    public: // Data

        TUint iActivatedCount;

        TUint iStreamId;

        TBool iErrorOccurred;
        };


    class TFetchingData
        {
    public: // Constructors and destructor

    inline TFetchingData( TUint aStreamId, TUint aCollectionId,
            TUint aComponentId, TUint aAddrFamily, TBool aICESpecific ) :
        iStreamId( aStreamId ),
        iCollectionId( aCollectionId ),
        iComponentId( aComponentId ),
        iAddrFamily( aAddrFamily ),
        iICESpecific( aICESpecific )
        {};

    public: // Data

        TUint iStreamId;
        TUint iCollectionId;
        TUint iComponentId;
        TUint iAddrFamily;
        TBool iICESpecific;
        };

private: // data

    /**
     * Used to keep track ongoing sending status requests on stream basis.
     * Own.
     */
    RArray<TStatusCounter> iSendingStatusCounts;

    /**
     * Used to keep track ongoing receiving status requests on stream basis.
     * Own.
     */
    RArray<TStatusCounter> iReceivingStatusCounts;

    /**
     * Unique session identifier
     */
    TUint iSessionId;

    /**
     * Internet access point identifier
     */
    TUint32 iIapId;

    /**
     * Domain for settings querying
     * Own.
     */
    HBufC8* iDomain;

    /**
     * NAT FW Registration controller
     * Not own.
     */
    MNATFWRegistrationController& iController;

    /**
     * The multiplexer
     * Not own.
     */
    CNcmConnectionMultiplexer& iMultiplexer;

    /**
     * Async callback handler
     * Not own.
     */
    CNatFwAsyncCallback& iAsyncCallback;

    /**
     * NAT-settings
     * Own.
     */
    CNATFWNatSettingsApi* iNatSettings;

    /**
     * NAT-protocol plugin used in the session
     * Own.
     */
    CNATFWPluginApi* iPlugin;

    /**
     * Streams created into the session
     * Own.
     */
    RPointerArray<CNATFWStream> iStreams;

    /**
     * Server connection process state
     */
    TServerConnectionState iServerConnectionState;

    /**
     * FetchCandidate(s) queue
     * Own.
     */
    RArray<TFetchingData> iFetchCandidateQueue;

    };

#endif // C_CNATFWSESSION_H
