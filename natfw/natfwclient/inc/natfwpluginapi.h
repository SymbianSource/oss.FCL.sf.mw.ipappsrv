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
* Description:    API for NAT protocol plug-ins
*
*/




#ifndef C_CNATFWPLUGINAPI_H
#define C_CNATFWPLUGINAPI_H


#include <e32base.h>
#include <ecom/ecom.h>
#include <in_sock.h>
#include "natfwconnectivityapidefs.h"


class MNATFWPluginObserver;
class CNATFWCandidate;
class CNATFWCredentials;
class MNcmConnectionMultiplexer;
class CNATFWCandidatePair;


/** NAT protocol plugin interface UID */
const TUid KInterfaceUid = { 0x10275444 };

/**
 *  Interface for the NAT protocol plug-ins.
 *
 *  @lib natfwclient.lib
 *  @since S60 v3.2 
 */
class CNATFWPluginApi : public CBase
    {

public:

    /**
     *  Class encapsulates initialization parameters needed in plugin
     *  instantiation.
     *
     *  @lib natfwclient.lib
     *  @since S60 v3.2 
     */
    class TNATFWPluginInitParams
        {
        public:

            /**
             * Constructor.
             * @param aObserver     The observer for NAT-plugin
             * @param aMultiplexer  The multiplexer
             * @param aDomain       The domain where to operate
             * @param iIapId        The Internet access point identifier
             */
            TNATFWPluginInitParams( MNATFWPluginObserver& aObserver,
                MNcmConnectionMultiplexer& aMultiplexer,
                const TDesC8& aDomain, TUint32 iIapId );

        public:
                    
            MNATFWPluginObserver& iObserver;
            MNcmConnectionMultiplexer& iMultiplexer;
            const TDesC8& iDomain;
            TUint32 iIapId;
        };


    /**
     * Two-phased constructor.
     * @param aImplementationUid    Implementation UID of the plugin to load
     * @param aInitParams           Initialization parameters
     */
    static CNATFWPluginApi* NewL( 
        TUid aImplementationUid,
        TNATFWPluginInitParams& aInitParams );

    /**
     * Two-phased constructor.
     * @param aPluginId             Identifier of the plugin to load
     * @param aInitParams           Initialization parameters
     */
    static CNATFWPluginApi* NewL( 
        const TDesC8& aPluginId,
        TNATFWPluginInitParams& aInitParams );

    /**
     * Destructor.
     */
    virtual ~CNATFWPluginApi();

    /**
     * Lists all available NAT-protocol plugin implementations.
     *
     * @since   S60 v3.2
     * @param   aImplInfoArray      Array of implementations
     */
    static void ListAllImplementationsL( RImplInfoPtrArray& aImplInfoArray );

    /**
     * Finds out a STUN Server and establishes a connection to it.
     * 
     * MNATFWPluginObserver::Notify is called with event code EServerConnected
     * when connecting has completed.
     *     
     * FetchCandidateL can be called after successful connecting.
     *
     * @since   S60 v3.2
     * @param   aSocketServ         The handle to socket server session
     * @param   aConnectionName     The unique name of the RConnection
     * @return  void
     */
    virtual void ConnectServerL( const RSocketServ& aSocketServ,
        const TName& aConnectionName ) = 0;
    
    /**
     * Fetches available candidates for a specified stream using given
     * retransmission timeout for STUN transactions. 
     * 
     * RTO value 0 indicates default retransmission timeout as specified
     * in STUN specification.
     * 
     * @since   S60 v3.2
     * @param   aStreamId           Stream identifier
     * @param   aRtoValue           Retransmission timeout in milliseconds
     * @param   aAddrFamily         KAFUnspec / KAfInet / KAfInet6
     * @leave   KErrNotSupported NAT Protocol plugin does not support
     * operation.
     */
    virtual inline void FetchCandidateL( TUint aStreamId, TUint aRtoValue,
        TUint aAddrFamily );
    
    /**
     * Fetches available candidates for a specified stream using given
     * retransmission timeout for STUN transactions. 
     * 
     * RTO value 0 indicates default retransmission timeout as specified
     * in STUN specification.
     * 
     * Candidate is fetched using given base address as a local endpoint for
     * a multiplexer connection.
     * 
     * @since   S60 v3.2
     * @param   aStreamId           Stream identifier
     * @param   aRtoValue           Retransmission timeout in milliseconds
     * @param   aBaseAddr           Address from which to create connection
     */
    virtual inline void FetchCandidateL( TUint aStreamId, TUint aRtoValue,
        const TInetAddr& aBaseAddr );
    
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
    virtual inline void FetchCandidatesL( TUint aStreamCollectionId,
        TUint aStreamId, TUint aComponentId, TUint aAddrFamily );
    
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
    virtual inline void SetRoleL( TNATFWIceRole aRole );

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
    virtual inline void SetCredentialsL(
        const CNATFWCandidate& aCandidate,
        const CNATFWCredentials& aCredentials );

    /**
     * ICE specific function.
     * Starts to perform connectivity checks.
     *
     * @since   S60 v3.2
     * @param   aRemoteCandidates   Remote candidate array
     * @leave   KErrNotSupported NAT Protocol plugin does not support
     * operation.
     */
    virtual inline void PerformConnectivityChecksL( 
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
     * @leave   KErrNotSupported NAT Protocol plugin does not support
     * operation.
     * @post    ICE processing is continued with new parameters
     */
    virtual inline void UpdateIceProcessingL( 
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
     * @leave   KErrNotSupported NAT Protocol plugin does not support
     * operation.
     * @post    ICE processing is continued with new parameters
     */
    virtual inline void UpdateIceProcessingL( 
        RPointerArray<CNATFWCandidate>& aRemoteCands );
    
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
     * @pre     FetchCandidate(/s) and possible ICE processing have completed
     * @param   aLocalCandidate The local end point for a media
     * @param   aState          The receiving state
     */
    virtual void SetReceivingStateL( const CNATFWCandidate& aLocalCandidate,
        TNATFWStreamingState aState ) = 0;
    
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
     * @pre     FetchCandidate(/s) and possible ICE processing have completed
     * @param   aLocalCandidate The local end point for a media
     * @param   aState          The sending state
     * @param   aDestAddr       The remote end point for a media
     */
    virtual void SetSendingStateL( const CNATFWCandidate& aLocalCandidate,
        TNATFWStreamingState aState, const TInetAddr& aDestAddr ) = 0;
    
    /**
     * Gets identifier for a connection that is associated with the
     * given local end point.
     * 
     * @since   S60 v3.2
     * @param   aLocalCandidate     The local end point for a media
     * @param   aConnectionId       Reference to found id
     */
    virtual void GetConnectionIdL( 
        const CNATFWCandidate& aLocalCandidate,
        TUint& aConnectionId ) = 0;
    
    /**
     * Get identifier of the plugin.
     * 
     * @since   S60 v3.2
     * @return  Identifier of the plugin
     */
    virtual const TDesC8& PluginIdentifier() const = 0;

protected:

    inline CNATFWPluginApi();

private:

    void ConstructL();

private: // data

    /**
     * Unique instance identifier key
     */
    TUid iDtor_ID_Key;
    };


#include "natfwpluginapi.inl"


#endif // C_CNATFWPLUGINAPI_H
