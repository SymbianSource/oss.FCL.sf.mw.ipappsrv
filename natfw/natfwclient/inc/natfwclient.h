/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Implementation classs of the NAT Connectivity FW
*
*/




#ifndef NATFWCLIENT_H
#define NATFWCLIENT_H

#include "natfwconnectivityapi.h"
#include "mncmconnectionmultiplexerobserver.h"
#include "mnatfwregistrationcontroller.h"

class CNcmConnectionMultiplexer;
class MNATFWSocketMediaConnWrapper;
class CNATFWSession;
class CNATFWStream;
class CNatFwAsyncCallback;

/**
 *  The implementation class of the NAT Connectivity FW
 *
 *  Implemets the NAT connectivity FW API
 *
 *  @lib natconfw.lib
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CNATFWClient ) : public CBase,
    public MNcmConnectionMultiplexerObserver,
    public MNATFWRegistrationController
    {

    friend class UT_CNATFWClient;

public:

    static CNATFWClient* NewL();

    static CNATFWClient* NewLC();

    virtual ~CNATFWClient();

    /**
     * Registers given observer for the specified events. Same observer can be
     * registered for several events. Also several observers can be registered
     * for the same event. In that case each observer receives own copy of
     * eventdata.
     *
     * Refer to MNATFWConnectivityObserver API for event definitions.
     *
     * @since   S60 v3.2
     * @param   aObserver       Observer for NAT Connectivity API events
     * @param   aEvents         Events for which to register
     * @leave   KErrAlreadyExists Observer is already registered for some of
     *  the events
     */
    void RegisterObserverForEventsL( MNATFWConnectivityObserver& aObserver,
        TUint16 aEvents );

    /**
     * Unregisters given observer for the specified events.
     *
     * Refer to MNATFWConnectivityObserver API for event definitions.
     *
     * @since   S60 v3.2
     * @param   aObserver       Observer for NAT Connectivity API events
     * @param   aEvents         Events for which to unregister
     * @post    Observer will not receive notifications for specified events
     *          any more.
     */
    void UnregisterObserverForEvents( MNATFWConnectivityObserver& aObserver,
        TUint16 aEvents );

    /**
     * Creates a new session for the client. On return the client receives
     * identifier for the session. The session is ready to be used when the
     * client receives a callback to MNATFWConnectivityObserver::Notify
     * function with event code ESessionCreated.
     *
     * @since   S60 v3.2
     * @param   aIapId          The IAP used for a connection
     * @param   aDomain         Used domain for settings query
     * @return  The ID for the created session
     */
    TUint CreateSessionL( TUint32 aIapId, const TDesC8& aDomain );

    /**
     * Loads a NAT protocol plugin to the session. Old plugin is destroyed if
     * it exists. In this case NAT operations for the session must be started
     * anew ( FetchCandidate(s) and possible ICE processing ).
     *
     * Given NAT protocol plugins are tried to be loaded in order until a
     * working one is found.
     *
     * @since   S60 v3.2
     * @pre     NAT session is created
     * @param   aSessionId      The ID identifying session
     * @param   aPlugins        Array containing identifiers for available
     *          NAT-protocol plugins in preferred order. E.g.
     *          "exampleprovider.stun".
     * @param   aLoadedPluginInd Index to the aPlugins array telling actually
     *  loaded plugin.
     * @post    NAT protocol plugin is loaded and session is ready for use
     * @leave   KErrNotFound NAT-plugin was not found
     */
    void LoadPluginL( TUint aSessionId, const CDesC8Array& aPlugins,
        TInt& aLoadedPluginInd );

    /**
     * Creates a new stream for the client. On return the client receives a
     * stream ID.
     *
     * @since   S60 v3.2
     * @param   aSessionId      The ID identifying session
     * @param   aProtocol       KProtocolInetUdp / KProtocolInetTcp
     * @param   aQoS            The desired quality of service.
     * @return  The ID for the created stream
     */
    TUint CreateStreamL( TUint aSessionId,
        TUint aProtocol, TInt aQoS );

    /**
     * Creates a wrapper for the specified stream.
     *
     * @since   S60 v3.2
     * @param   aSessionId      The ID identifying session
     * @param   aStreamId       The ID identifying stream
     * @return  The interface through which the client can configure the wrapper.
     */
    MNATFWSocketMediaConnWrapper& CreateWrapperL(
        TUint aSessionId, TUint aStreamId );

    /**
     * Resolves public IP address to be used in the communication between
     * peers. MNATFWConnectivityObserver::NewLocalCandidateFound
     * is called when a public IP has been resolved.
     *
     * @since   S60 v3.2
     * @param   aSessionId      The ID identifying session
     * @param   aStreamId       The ID identifying stream
     * @param   aAddrFamily     KAFUnspec / KAfInet / KAfInet6
     */
    void FetchCandidateL( TUint aSessionId, TUint aStreamId,
        TUint aAddrFamily );

    /**
     * ICE spesific function. Fetches IP address candidates for the
     * communication between peers. Client is responsible for providing
     * mapping between components of media stream through collection ID
     * parameter. MNATFWConnectivityObserver::NewLocalCandidateFound
     * is called whenever a new candidate has been found.
     *
     * @since   S60 v3.2
     * @param   aSessionId      The ID identifying session
     * @param   aStreamId       The ID identifying stream
     * @param   aCollectionId   The stream collection identifier
     * @param   aComponentId    The media component identifier
     * @param   aAddrFamily     KAFUnspec / KAfInet / KAfInet6
     */
    void FetchCandidatesL(
        TUint aSessionId, TUint aStreamId,
        TUint aCollectionId, TUint aComponentId, TUint aAddrFamily );

    /**
     * ICE specific function.
     * Sets the role of local ICE agent. In role-conflict situation given role
     * will be silently changed.
     *
     * @since   S60 v3.2
     * @param   aSessionId      The ID identifying session
     * @param   aRole           The role
     */
    void SetRoleL( TUint aSessionId, TNATFWIceRole aRole );

    /**
     * ICE specific function. Set username fragment and corresponding
     * password for message signing and authentication with the specified
     * candidate.
     *
     * @since   S60 v3.2
     * @param   aCandidate      The candidate
     * @param   aCredentials    The credentials
     */
    void SetCredentialsL(
        CNATFWCandidate& aCandidate,
        const CNATFWCredentials& aCredentials );

    /**
     * ICE specific function.
     * Performs connectivity checks between the local candidates and the
     * remote candidates. MNATFWConnectivityObserver::NewCandidatePairFound
     * is called once per NATFW stream when ICE has selected working
     * candidate pair to be used for media in that stream.
     *
     * @since   S60 v3.2
     * @param   aSessionId      The ID identifying session
     * @param   aRemoteCands    The remote candidate array
     */
    void PerformConnectivityChecksL( TUint aSessionId,
        RPointerArray<CNATFWCandidate>& aRemoteCands );

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
     * Should a candidate use an FQDN as transport address, the corresponding
     * IP address shall be updated to it using A/AAAA queries.
     *
     * @since   S60 v3.2
     * @pre     ICE processing is started with PerformConnectivityChecksL
     * @param   aSessionId          The ID identifying session
     * @param   aPeerSelectedPairs  Peer selected candidate pairs
     * @leave   KErrNotSupported Loaded NAT Protocol plugin does not support
     * operation.
     * @post    ICE processing is continued with new parameters
     */
    void UpdateIceProcessingL( TUint aSessionId,
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
     * Should a candidate use an FQDN as transport address, the corresponding
     * IP address shall be updated to it using A/AAAA queries.
     *
     * @since   S60 v3.2
     * @pre     ICE processing is started with PerformConnectivityChecksL
     * @param   aSessionId          The ID identifying session
     * @param   aRemoteCands        All remote candidates known currently
     * @leave   KErrNotSupported Loaded NAT Protocol plugin does not support
     * operation.
     * @post    ICE processing is continued with new parameters
     */
    void UpdateIceProcessingL( TUint aSessionId,
        RPointerArray<CNATFWCandidate>& aRemoteCands );

    /**
     * Enables/disables media receiving for the specified local candidate.
     * 
     * Only those candidates given with MNATFWConnectivityObserver callbacks
     * can be controlled. NATFW stream can have only one active candidate
     * at a time. In other words, exactly one local candidate can be used
     * for media delivery in receiving, sending or both directions.
     * 
     * Receiving must be disabled prior to closing stream so that all
     * resources can be freed.
     * 
     * MNATFWConnectivityObserver::Notify is called with event code
     * EReceivingActivated/EReceivingDeactivated when state change has 
     * completed. While activated incoming data is delivered to the media
     * engine.
     * 
     * Note: If TURN is used media cannot be delivered until remote address
     * is known.
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
     * Only those candidates given with MNATFWConnectivityObserver callbacks
     * can be controlled. NATFW stream can have only one active candidate
     * at a time. In other words, exactly one local candidate can be used
     * for media delivery in receiving, sending or both directions.
     * 
     * Sending must be disabled prior to closing stream so that all
     * resources can be freed.
     * 
     * MNATFWConnectivityObserver::Notify is called with event code
     * ESendingActivated/ESendingDeactivated when state change has 
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

    /**
     * Enables/disables media sending in the specified NAT-stream.
     * 
     * MNATFWConnectivityObserver::Notify is called with event code
     * ESendingActivated/ESendingDeactivated when state change has 
     * completed. 
     * 
     * While activated outgoing data from media engine is delivered
     * to the specified FQDN and port.
     * 
     * @since   S60 v3.2
     * @pre     FetchCandidate(/s) and possible ICE processing have completed
     * @param   aLocalCandidate The local end point for a media
     * @param   aState          The streaming state
     * @param   aDestAddr       The destination address for the media as FQDN
     * @param   aPort           The destination port for the media
     */
    void SetSendingStateL( const CNATFWCandidate& aLocalCandidate,
        TNATFWStreamingState aState, const TDesC8& aDestAddr, TUint aPort );

    /**
     * Closes the specified stream.
     *
     * @since   S60 v3.2
     * @pre     Streaming is disabled with SetReceivingStateL/SetSendingStateL
     * @param   aSessionId      The ID identifying session
     * @param   aStreamId       The ID identifying stream
     */
    void CloseStreamL( TUint aSessionId, TUint aStreamId );

    /**
     * Closes the specified session. All streams in the session are
     * closed too.
     *
     * @since   S60 v3.2
     * @pre     Streaming is disabled in all NAT-streams belonging to
     *          the session
     * @param   aSessionId      The ID identifying session
     */
    void CloseSessionL( TUint aSessionId );

    /**
     * From MNcmConnectionMultiplexerObserver.
     * Called by the Multiplexer when actions completes.
     *
     * @since S60 v3.2
     * @param aSessionId    The session identifier
     * @param aStreamId     The stream id where the event occured
     * @param aType         The type of the event
     * @param aError        Error code
     */
    void Notify( TUint aSessionId, TUint aStreamId, TNotifyType aType,
        TInt aError );

    /**
     * From MNATFWRegistrationController.
     * Provides access to the event registry.
     *
     * @since   S60 v3.2
     * @return  The event registry
     */
    const RArray<TNATFWEventRegistration>& Registry();

private:

    CNATFWClient();

    void ConstructL();

    CNATFWSession* SessionByIdL( TUint aSessionId );

    CNATFWStream* FindStreamById( TUint aStreamId );

    void ResolveFQDNAddrL( CNATFWCandidate& aCandidate );

    void DoNotify(
        MNATFWConnectivityObserver::TNATFWConnectivityEvent aEvent,
        TUint aSessionId, TUint aStreamId, TInt aErrCode,
        TAny* aEventData );

private: // data

    /**
     * Pointer to Connection Multiplexer.
     * Own.
     */
    CNcmConnectionMultiplexer* iConnMux;

    /**
     * Events registered by the client
     * Own.
     */
    RArray<TNATFWEventRegistration> iEventRegistry;

    /**
     * Sessions owned by the client
     * Own.
     */
    RPointerArray<CNATFWSession> iSessions;

    /**
     * Async callback handler
     * Own.
     */
    CNatFwAsyncCallback* iAsyncCallback;

    };

#endif // NATFWCLIENT_H
