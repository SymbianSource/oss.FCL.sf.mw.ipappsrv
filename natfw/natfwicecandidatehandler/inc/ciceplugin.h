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
* Description:    Realizes NAT protocol plugin API for ICE.
*
*/




#ifndef CNATFWICEPLUGIN_H
#define CNATFWICEPLUGIN_H

#include <e32base.h>
#include <in_sock.h>
#include "natfwpluginapi.h"
#include "mnatfwpluginobserver.h"
#include "natfwconnectivityapidefs.h"
#include "miceconnhandlerobserver.h"
#include "micenatplugineventobs.h"
#include "natfwinternaldefs.h"    //UNIT_TEST

class MNATFWPluginObserver;
class CIceSessionData;
class CIceLocalCandidateFinder;
class CIceConnectionHandler;
class CIceCheckHandler;
class CIceNatPluginContainer;

/**
 *  Implements ICE protocol for NAT traversal.
 *
 *  @lib icecandidatehandler.dll
 *  @since S60 v3.2
 */
class CIcePlugin 
    : 
    public CNATFWPluginApi,
    public MIceConnHandlerObserver,
    public MIceNatPluginEventObs
    {
    
    UNIT_TEST( UT_CIcePlugin )
    
public:

    /**
     * Two-phased constructor.
     * @param aInitParams           Initialization parameters
     */
    static CIcePlugin* NewL( TAny* aInitParams );

    /**
     * Destructor.
     */
    virtual ~CIcePlugin();

    /**
     * From CNATFWPluginApi.
     *
     * Finds out a STUN Server and establishes a connection to it.
     * 
     * MNATFWPluginObserver::Notify is called with event code EServerConnected
     * when connecting has completed.
     *     
     * FetchCandidatesL can be called after successful connecting.
     *
     * @since   S60 v3.2
     * @param   aSocketServ         The handle to socket server session
     * @param   aConnectionName     The unique name of the RConnection
     */
    void ConnectServerL( const RSocketServ& aSocketServ,
        const TName& aConnectionName );
    
    /**
     * Fetches all available candidates for a specified stream in a 
     * stream collection.
     *
     * @since   S60 v3.2
     * @param   aStreamCollectionId Stream collection identifier
     * @param   aStreamId           Stream identifier
     * @param   aComponentId        Component identifier
     * @param   aAddrFamily         KAFUnspec / KAfInet / KAfInet6
     * @leave   KErrNotSupported NAT Protocol plugin does not support
     * operation.
     */
    void FetchCandidatesL( TUint aStreamCollectionId,
        TUint aStreamId, TUint aComponentId, TUint aAddrFamily );
    
    /**
     * From CNATFWPluginApi.
     * Sets the role for ICE. In role-conflict situation given role
     * will be silently changed.
     *
     * @since   S60 v3.2
     * @param   aRole               The role
     */
    void SetRoleL( TNATFWIceRole aRole );
    
    /**
     * From CNATFWPluginApi.
     * Sets username fragment and corresponding password for message signing
     * and authentication with the specified candidate.
     *
     * @since   S60 3.2
     * @param   aCandidate          The candidate
     * @param   aCredentials        The credentials
     */
    void SetCredentialsL(
        const CNATFWCandidate& aCandidate,
        const CNATFWCredentials& aCredentials );
    
    /**
     * From CNATFWPluginApi.
     * Starts to perform connectivity checks.
     *
     * @since   S60 3.2
     * @param   aRemoteCandidates Remote candidates
     */
    void PerformConnectivityChecksL( 
        RPointerArray<CNATFWCandidate>& aRemoteCandidates );

    /**
     * From CNATFWPluginApi.
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
     * @leave   KErrNotSupported NAT Protocol plugin does not support
     * operation.
     * @post    ICE processing is continued with new parameters
     */
    void UpdateIceProcessingL( 
        RPointerArray<CNATFWCandidatePair>& aPeerSelectedPairs );
    
    /**
     * From CNATFWPluginApi.
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
     * @leave   KErrNotSupported NAT Protocol plugin does not support
     * operation.
     * @post    ICE processing is continued with new parameters
     */
    void UpdateIceProcessingL( 
        RPointerArray<CNATFWCandidate>& aRemoteCands );
    
    /**
     * From CNATFWPluginApi.
     * Enables/disables media receiving for the specified local candidate.
     * 
     * Only those candidates given with MNATFWPluginObserver callbacks
     * can be controlled. NATFW stream can have only one active candidate
     * at a time. In other words, exactly one local candidate can be used
     * for media delivery in receiving, sending or both directions.
     * 
     * MNATFWPluginObserver::Notify is called with event code
     * EReceivingActivated/EReceivingDeactivated when status change has 
     * completed. While activated incoming data is delivered to the media
     * engine.
     * 
     * Receiving must be disabled prior to deleting plugin so that all
     * resources can be freed.
     * 
     * Note: If TURN is used media cannot be delivered until remote address
     * is known.
     * 
     * @since   S60 v3.2
     * @param   aLocalCandidate The local end point for a media
     * @param   aState          The receiving state
     */
    void SetReceivingStateL( const CNATFWCandidate& aLocalCandidate,
        TNATFWStreamingState aState );
    
    /**
     * From CNATFWPluginApi.
     * Enables/disables media sending from the specified local candidate.
     * 
     * Only those candidates given with MNATFWPluginObserver callbacks
     * can be controlled. NATFW stream can have only one active candidate
     * at a time. In other words, exactly one local candidate can be used
     * for media delivery in receiving, sending or both directions.
     * 
     * Sending must be disabled prior to deleting plugin so that all
     * resources can be freed.
     * 
     * MNATFWPluginObserver::Notify is called with event code
     * ESendingActivated/ESendingDeactivated when status change has 
     * completed. While activated outgoing data from media engine is delivered
     * to the specified remote address.
     * 
     * @since   S60 v3.2
     * @param   aLocalCandidate The local end point for a media
     * @param   aState          The sending state
     * @param   aDestAddr       The remote end point for a media
     */
    void SetSendingStateL( const CNATFWCandidate& aLocalCandidate,
        TNATFWStreamingState aState, const TInetAddr& aDestAddr );
    
    /**
     * From CNATFWPluginApi.
     * Gets identifier for a connection that is associated with the
     * given local end point.
     * 
     * @since   S60 v3.2
     * @param   aLocalCandidate     The local end point for a media
     * @param   aConnectionId       Reference to found id
     */
    void GetConnectionIdL( 
        const CNATFWCandidate& aLocalCandidate,
        TUint& aConnectionId );
    
    /**
     * From CNATFWPluginApi.
     * Get identifier of the plugin.
     * 
     * @since   S60 3.2
     * @return  Identifier of the plugin
     */
    const TDesC8& PluginIdentifier() const;

// From base class MIceConnHandlerObserver
    
    /**
     * Notifies that stream has been closed.
     * 
     * @since   S60 3.2
     * @param   aStreamId       The stream identifier
     */
    void StreamClosed( TUint aStreamId );

// from base class MIceNatPluginEventObs

    void PluginEventOccured( const CNATFWPluginApi* aPlugin, 
        TUint aStreamId, TNatPluginEvent aEventCode, 
        TInt aErrorCode, TAny* aEventData, TUint aStreamConnectionId = 0,
        TUint aComponentId = 0 , TBool aIPv6After = 0 );

private:

    CIcePlugin();

    void ConstructL( TAny* aInitParams );
    
private: // data

    /**
     * Used to keep track of ongoing connect server requests.
     */
    TInt iConnectedServerCount;
    
    /**
     * Plugin identifier.
     * Own.
     */
    HBufC8* iPluginId;
    
    /**
     * Used to store candidates, credentials etc.
     * Own.
     */
    CIceSessionData* iSessionData;
    
    /**
     * Candidate finder for local candidate resolving.
     * Own.
     */
    CIceLocalCandidateFinder* iCandidateFinder;
    
    /**
     * Handles connections.
     * Own.
     */
    CIceConnectionHandler* iConnectionHandler;

    /**
     * Handles all actions relating to connectivity checks.
     * Own.
     */
    CIceCheckHandler* iCheckHandler;

    /**
     * NAT protocol plugin container.
     * Own.
     */
    CIceNatPluginContainer* iPluginContainer;

    /**
     * Observer for ICE plugin.
     * Not Own.
     */
    MNATFWPluginObserver* iPluginObserver;

    /**
     * True if ipv4 candidate fetching succeeded.
     */
    TBool iIpv4Succeeded;

    };


#endif // CNATFWICEPLUGIN_H
