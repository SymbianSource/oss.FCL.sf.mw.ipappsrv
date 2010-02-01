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
* Description:   
*
*/




#ifndef C_NATFWTESTPLUGIN_H
#define C_NATFWTESTPLUGIN_H

#include <e32base.h>
#include "natfwpluginapi.h"

class CNATFWConnectionHandler;
//class MNATFWConnectionMultiplexerIF;
//class MNATFWPluginObserver;
//class CNATFWCandidate;

/**
 *  Implements NAT Plug-in interface
 *
 *  The client uses this interface to send STUN requests. Forwards
 *  client requests to STUN Connection Handler.
 *
 *  @lib stunplugin.lib
 *  @since S60 v3.2
 */
class CNATFWTestPlugin : public CNATFWPluginApi
    {

public:

    /**
     * Creates a new instance of NAT FW Plug-in class.
     *
     * @since S60 v3.2
     * @param aInitParams Initial parameters that will be given to
     *                    STUN Connection Handler class.
     * @return A new instance, ownership is transferred.
     */
    static CNATFWTestPlugin* NewL( TAny* aInitParams );

    virtual ~CNATFWTestPlugin();

    /**
     * ConnectServerL
     *
     * @since   S60 v3.2
     * @param   aSocketServ         The handle to socket server session
     * @param   aConnectionName     The unique name of the RConnection
     * @return  void
     */
     void ConnectServerL( const RSocketServ& aSocketServ,
        const TName& aConnectionName );
        
    /**
     * FetchCandidate
     *
     * @since S60 v3.2
     * @param aStreamId Stream ID
     * @return void
     */
    void FetchCandidateL( TUint aStreamId, TUint aRtoValue,
        TUint aAddrFamily  );
    
    /**
     * ICE specific function.
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
     * Connect
     *
     * @since S60 v3.2
     * @return void
     */
    void ConnectL();

    /**
     * MNATFWPluginObserver::Notify is called (returned) with stream's
     * current sending status.
     *
     * @since   S60 3.2
     * @param   aStreamId   The stream identifier
     * @param   aDestAddr   The address where to send media
     * @param   aState     The streaming status
     * @return  void
     */
    void SetSendingStateL( TUint aStreamId,
                            const TInetAddr& aDestAddr,
                            TNATFWStreamingState aState );
                            
    /**
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
     * @leave   KErrNotSupported NAT Protocol plugin does not support
     * operation.
     * @post    ICE processing is continued with new parameters
     */
    void UpdateIceProcessingL( 
        RPointerArray<CNATFWCandidatePair>& aPeerSelectedPairs );

    /**
     * ICE specific function.
     * Starts to perform connectivity checks.
     *
     * @since   S60 v3.2
     * @param   aRemoteCandidates   Remote candidate array
     * @leave   KErrNotSupported NAT Protocol plugin does not support
     * operation.
     */
    void PerformConnectivityChecksL( 
            RPointerArray<CNATFWCandidate>& aRemoteCandidates );
    
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
     * @leave   KErrNotSupported NAT Protocol plugin does not support
     * operation.
     * @post    ICE processing is continued with new parameters
     */
    void UpdateIceProcessingL( 
        RPointerArray<CNATFWCandidate>& aRemoteCands );

    
    /**
     * ICE specific function.
     * Sets the role for ICE. In role-conflict situation given role
     * will be silently changed.
     *
     * @since   S60 v3.2
     * @param   aRole               The role
     * @leave   KErrNotSupported NAT Protocol plugin does not support
     * operation.
     */
    void SetRoleL( TNATFWIceRole aRole );
    
    /**
     * ICE specific function. 
     * Sets username fragment and corresponding password for message signing
     * and authentication with the specified candidate.
     *
     * @since   S60 v3.2
     * @param   aCandidate          The candidate
     * @param   aCredentials        The credentials
     * @leave   KErrNotSupported NAT Protocol plugin does not support
     * operation.
     */
    void SetCredentialsL(
        const CNATFWCandidate& aCandidate,
        const CNATFWCredentials& aCredentials );
    
    /**
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
    
private:

    CNATFWTestPlugin();

    void ConstructL( TAny* aInitParams );
    

private: // data
    
    
    /**
     * Handles connection related actions.
     * Own.
     */
    CNATFWConnectionHandler* iConnectionHandler;
    
    /**
     * Plugin identifier.
     * Own.
     */
    HBufC8* iPluginId;
    
    /**
     * IapId
     */
    //TUint iIapId;
    
    /**
     * Domain name
     * own.
     */
    //HBufC8* iDomain;
    
    /**
     * Not own.
     */
    //MNATFWConnectionMultiplexerIF* iConnMux;
    
    /**
     * Sends notifications to NAT Plug-in observer.
     * Not own.
     */
    //MNATFWPluginObserver* iPluginObserver;
    
    /**
     * Candidate
     * own.
     */
    //CNATFWCandidate* iCandidate;
    
    /**
     * iStreamId
     */
    //TUint iStreamId;
    
    //TNATFWStreamingState iStreamingStatus;
    };

#endif // C_NATFWTESTPLUGIN_H
