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
* Description:    Provides services for client to traverse NATs
*
*/





#ifndef NATCONNECTIVITYFW_H
#define NATCONNECTIVITYFW_H

#include <e32base.h>
#include <in_sock.h>
#include "natfwconnectivityapidefs.h"

class CNATFWClient;
class MNATFWSocketMediaConnWrapper;
class CNATFWCandidate;
class CNATFWCredentials;
class MNATFWConnectivityObserver;
class CDesC8Array;
class CNATFWCandidatePair;

/**
 * Main API to the NAT Connectivity Framework. Provides services for client
 * to traverse through NATs and firewalls.
 *
 * Only one API instance per client is intended to create.
 *
 * @lib natconfw.lib
 * @since S60 v3.2
 */
class CNATFWConnectivityApi : public CBase
    {

public:

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CNATFWConnectivityApi* NewL();

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CNATFWConnectivityApi* NewLC();

    /**
     * Destructor.
     */
    virtual ~CNATFWConnectivityApi();

    /**
     * Registers an observer for the specified events. Same observer can be
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
    IMPORT_C void RegisterObserverForEventsL( MNATFWConnectivityObserver& aObserver,
        TUint16 aEvents );

    /**
     * Unregisters an observer for the specified events.
     *
     * Refer to MNATFWConnectivityObserver API for event definitions.
     *
     * @since   S60 v3.2
     * @param   aObserver       Observer for NAT Connectivity API events
     * @param   aEvents         Events for which to unregister
     * @post    Observer will not receive notifications for specified events
     *          any more.
     */
    IMPORT_C void UnregisterObserverForEvents( MNATFWConnectivityObserver& aObserver,
        TUint16 aEvents );

    /**
     * Creates a new NAT-session. On return the client receives identifier for
     * the session.
     *
     * The session is ready to be used when the client receives
     * MNATFWConnectivityObserver::Notify callback with event code
     * ESessionCreated.
     *
     * @since   S60 v3.2
     * @param   aIapId          The IAP used for a connection
     * @param   aDomain         Used domain for settings query
     * @return  The ID for the created session
     */
    IMPORT_C TUint CreateSessionL( TUint32 aIapId, const TDesC8& aDomain );

    /**
     * Loads a NAT protocol plugin to the session. Old plugin is destroyed if
     * it exists. In this case NAT operations for the session must be started
     * anew ( FetchCandidate(s) and possible ICE processing ).
     *
     * Given NAT protocol plugins are tried to load in order until working
     * one is found.
     *
     * @since   S60 v3.2
     * @pre     NAT session is created
     * @param   aSessionId      The ID identifying session
     * @param   aPlugins        Array containing identifiers for available
     *  NAT-protocol plugins in preferred order. E.g. "exampleprovider.stun".
     * @param   aLoadedPluginInd Index to the aPlugins array telling actually
     *  loaded plugin.
     * @post    NAT protocol plugin is loaded and session is ready for use
     * @leave   KErrNotFound NAT-plugin was not found
     */
    IMPORT_C void LoadPluginL( TUint aSessionId, const CDesC8Array& aPlugins,
        TInt& aLoadedPluginInd );

    /**
     * Creates a new NAT-stream to the session. On return the client receives
     * identifier for the stream.
     *
     * @since   S60 v3.2
     * @param   aSessionId      The ID identifying session
     * @param   aProtocol       KProtocolInetUdp / KProtocolInetTcp
     * @param   aQoS            The desired quality of service
     * @return  The ID for the created stream
     */
    IMPORT_C TUint CreateStreamL( TUint aSessionId,
        TUint aProtocol, TInt aQoS );

    /**
     * Creates a wrapper for the specified stream.
     *
     * @since   S60 v3.2
     * @param   aSessionId      The ID identifying session
     * @param   aStreamId       The ID identifying stream
     * @return  The interface through which the client can configure the wrapper
     */
    IMPORT_C MNATFWSocketMediaConnWrapper& CreateWrapperL(
        TUint aSessionId, TUint aStreamId );

    /**
     * Resolves public transport address to be used in the communication
     * between peers. 
     * 
     * MNATFWConnectivityObserver::NewLocalCandidateFound is called when
     * the candidate has been resolved.
     * 
     * MNATFWConnectivityObserver::Notify is called with event code 
     * EFetchingCompleted when resolving process has completed.
     * 
     * Address of given family is tried to resolve.
     * 
     * @since   S60 v3.2
     * @param   aSessionId      The ID identifying session
     * @param   aStreamId       The ID identifying stream
     * @param   aAddrFamily     KAFUnspec / KAfInet / KAfInet6
     * @leave   KErrNotSupported Loaded NAT Protocol plugin does not support
     * operation.
     */
    IMPORT_C void FetchCandidateL( TUint aSessionId, TUint aStreamId,
        TUint aAddrFamily );

    /**
     * ICE spesific function.
     * Fetches transport address candidates for the communication between
     * peers. Client is responsible for providing mapping between components
     * of media stream through collection identifier.
     *
     * MNATFWConnectivityObserver::NewLocalCandidateFound is called whenever
     * a new candidate has been found.
     *
     * MNATFWConnectivityObserver::Notify is called with event code
     * EFetchingCompleted when all available candidates are gathered.
     * 
     * Only candidates of specified family are tried to gather. With KAFUnspec
     * candidates of all address families are tried to resolve.
     *
     * @since   S60 v3.2
     * @param   aSessionId      The ID identifying session
     * @param   aStreamId       The ID identifying stream
     * @param   aCollectionId   The stream collection identifier
     * @param   aComponentId    The media component identifier
     * @param   aAddrFamily     KAFUnspec / KAfInet / KAfInet6
     * @leave   KErrNotSupported Loaded NAT Protocol plugin does not support 
     * operation.
     */
    IMPORT_C void FetchCandidatesL(
        TUint aSessionId, TUint aStreamId,
        TUint aCollectionId, TUint aComponentId, TUint aAddrFamily );
    
    /**
     * ICE specific function.
     * Sets the role of local ICE agent. In role-conflict situation given role
     * will be silently changed.
     *
     * @since   S60 v3.2
     * @param   aSessionId      The ID identifying session
     * @param   aRole           The role to take
     * @leave   KErrNotSupported Loaded NAT Protocol plugin does not support
     * operation.
     */
    IMPORT_C void SetRoleL( TUint aSessionId, TNATFWIceRole aRole );

    /**
     * ICE specific function. Sets username fragment and corresponding
     * password for message signing and authentication with the specified
     * candidate.
     *
     * Should a candidate use an FQDN as transport address, the corresponding
     * IP address shall be updated to it using A/AAAA queries.
     *
     * @since   S60 v3.2
     * @param   aCandidate      The candidate
     * @param   aCredentials    The credentials
     * @leave   KErrNotSupported Loaded NAT Protocol plugin does not support
     * operation.
     */
    IMPORT_C void SetCredentialsL(
        CNATFWCandidate& aCandidate,
        const CNATFWCredentials& aCredentials );

    /**
     * ICE specific function.
     * Performs connectivity checks between the local candidates and the
     * remote candidates.
     *
     * MNATFWConnectivityObserver::NewCandidatePairFound
     * is called once per NATFW stream when ICE has selected working
     * candidate pair to be used for media in that stream.
     *
     * MNATFWConnectivityObserver::Notify is called with event code
     * EConnChecksCompleted for each NATFW-stream when ICE process has
     * completed for that stream.
     *
     * Function can be used for starting initial connectivity tests and
     * ICE restarts for the stream(s). Restart requires updating of
     * role and credentials before calling this function.
     *
     * Should a candidate use an FQDN as transport address, the corresponding
     * IP address shall be updated to it using A/AAAA queries.
     *
     * @since   S60 v3.2
     * @pre     Role and credentials are set
     * @param   aSessionId      The ID identifying session
     * @param   aRemoteCands    The remote candidate array
     * @leave   KErrNotSupported Loaded NAT Protocol plugin does not support
     * operation.
     */
    IMPORT_C void PerformConnectivityChecksL( TUint aSessionId,
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
    IMPORT_C void UpdateIceProcessingL( TUint aSessionId,
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
    IMPORT_C void UpdateIceProcessingL( TUint aSessionId,
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
    IMPORT_C void SetReceivingStateL( const CNATFWCandidate& aLocalCandidate,
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
    IMPORT_C void SetSendingStateL( const CNATFWCandidate& aLocalCandidate,
        TNATFWStreamingState aState, const TInetAddr& aDestAddr );
    
    /**
     * Enables/disables media sending in the specified NAT-stream.
     * 
     * MNATFWConnectivityObserver::Notify is called with event code
     * ESendingActivated/ESendingDeactivated when state change has 
     * completed. While activated outgoing data from media engine is delivered
     * to the specified FQDN address and port.
     * 
     * @since   S60 v3.2
     * @pre     FetchCandidate(/s) and possible ICE processing have completed
     * @param   aLocalCandidate The local end point for a media
     * @param   aState          The streaming state
     * @param   aDestAddr       The destination address for the media as FQDN
     * @param   aPort           The destination port for the media
     */
    IMPORT_C void SetSendingStateL( const CNATFWCandidate& aLocalCandidate,
        TNATFWStreamingState aState, const TDesC8& aDestAddr, TUint aPort );

    /**
     * Closes the specified stream.
     *
     * @since   S60 v3.2
     * @pre     Streaming is disabled with SetReceivingStateL/SetSendingStateL
     * @param   aSessionId      The ID identifying session
     * @param   aStreamId       The ID identifying stream
     */
    IMPORT_C void CloseStreamL( TUint aSessionId, TUint aStreamId );

    /**
     * Closes the specified session. All streams in the session are
     * closed too.
     *
     * @since   S60 v3.2
     * @pre     Streaming is disabled in all NAT-streams belonging to
     *          the session
     * @param   aSessionId      The ID identifying session
     */
    IMPORT_C void CloseSessionL( TUint aSessionId );

private:

    CNATFWConnectivityApi();

    void ConstructL();

private: // data

    /**
     * Pointer to the implementor of the NAT Connectivity FW API
     * Own.
     */
    CNATFWClient* iClient;

    };

#endif // NATCONNECTIVITYFW_H
