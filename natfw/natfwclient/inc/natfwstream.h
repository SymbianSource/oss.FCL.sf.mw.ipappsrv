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
* Description:    NAT FW stream abstraction
*
*/




#ifndef C_CNATFWSTREAMIMPL_H
#define C_CNATFWSTREAMIMPL_H

#include <e32base.h>
#include <in_sock.h>
#include "natfwconnectivityapidefs.h"
#include "mncmconnectionmultiplexerobserver.h"

class MNATFWSocketMediaConnWrapper;
class CNATFWCredentials;
class CNATFWCandidate;
class CNcmConnectionMultiplexer;
class MNATFWConnectivityObserver;
class MNATFWRegistrationController;
class CNATFWPluginApi;
class CNATFWSession;
class CNatFwAsyncCallback;

/**
 *  NAT FW stream abstraction.
 *
 *  Provides NAT-traversal capable communication channel between local and
 *  remote sockets.
 *
 *  @lib natconfw.dll
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CNATFWStream ) : public CBase
    {

public:

    static CNATFWStream* NewL(
        MNATFWRegistrationController& aController,
        CNcmConnectionMultiplexer& aMultiplexer,
        CNatFwAsyncCallback& aAsyncCallback,
        CNATFWSession& aContext,
        TUint aProtocol, TInt aQoS );

    static CNATFWStream* NewLC(
        MNATFWRegistrationController& aController,
        CNcmConnectionMultiplexer& aMultiplexer,
        CNatFwAsyncCallback& aAsyncCallback,
        CNATFWSession& aContext,
        TUint aProtocol, TInt aQoS );

    virtual ~CNATFWStream();

    /**
     * Return unique identifier of the stream.
     *
     * @since   S60 v3.2
     * @return  Stream identifier
     */
    TUint StreamId() const;

    /**
     * Creates a wrapper for the stream.
     *
     * @since   S60 v3.2
     * @return  The interface through which the client can configure the wrapper.
     */
    MNATFWSocketMediaConnWrapper& CreateWrapperL();

    /**
     * Sets a new NAT-protocol plugin to be used or updates a stored
     * protocol plugin.
     *
     * @since   S60 v3.2
     * @param   aPlugin      The protocol plugin
     */
    void SetProtocolPlugin( CNATFWPluginApi* aPlugin );

    /**
     * Resolves public IP address to be used in the communication between
     * peers. MNATFWConnectivityObserver::NewLocalCandidateFound
     * is called when a public IP has been resolved.
     *
     * @since   S60 v3.2
     * @param   aAddrFamily     KAFUnspec / KAfInet / KAfInet6
     */
    void FetchCandidateL( TUint aAddrFamily );
    
    /**
     * ICE spesific function. Fetches IP address candidates for the
     * communication between peers. Client is responsible for providing
     * mapping between components of media stream through collection ID 
     * parameter. MNATFWConnectivityObserver::NewLocalCandidateFound
     * is called whenever a new candidate has been found.
     *
     * @since   S60 v3.2
     * @param   aCollectionId   The stream collection id
     * @param   aComponentId    The component identifier
     * @param   aAddrFamily     KAFUnspec / KAfInet / KAfInet6
     */
    void FetchCandidatesL( TUint aCollectionId, TUint aComponentId, 
        TUint aAddrFamily );

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
        const CNATFWCandidate& aCandidate,
        const CNATFWCredentials& aCredentials );

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
     * @param   aLocalCandidate The local end point for a media
     * @param   aState          The sending state
     * @param   aDestAddr       The remote end point for a media
     */
    void SetSendingStateL( const CNATFWCandidate& aLocalCandidate,
        TNATFWStreamingState aState, const TInetAddr& aDestAddr );
    
    /**
     * Call initiated by the Multiplexer of stream events.
     *
     * @since   S60 v3.2
     * @param   aSessionId      The ID identifying session
     * @param   aStreamId       The ID identifying stream
     * @param   aType           The type of the event
     * @param   aError          Error code.
     */
    void Notify( TUint aSessionId, TUint aStreamId,
        MNcmConnectionMultiplexerObserver::TNotifyType aType, TInt aError );

private:

    CNATFWStream(
        MNATFWRegistrationController& aController,
        CNcmConnectionMultiplexer& aMultiplexer,
        CNatFwAsyncCallback& aAsyncCallback,
        CNATFWSession& aContext,
        TUint aProtocol,
        TInt aQoS );

    void ConstructL();

    void DoNotify( TUint aSessionId, TUint aStreamId,
        MNATFWConnectivityObserver::TNATFWConnectivityEvent aEvent,
        TInt aError );

private: // data

    /**
     * Unique identifier of the stream
     */
    TUint iStreamId;

    /**
     * Desired quality of service for the stream
     */
    TInt iQoS;

    /**
     * Remote IP of the other peer
     */
    TInetAddr iDestAddr;

    /**
     * Transport protocol used with the stream
     */
    TUint iProtocol;

    /**
     * NAT FW Registration controller
     * Not own
     */
    MNATFWRegistrationController& iController;

    /**
     * Multiplexer
     * Not own.
     */
    CNcmConnectionMultiplexer& iMultiplexer;

    /**
     * Async callback handler
     * Not own.
     */
    CNatFwAsyncCallback& iAsyncCallback;

    /**
     * NAT-protocol plugin to be used
     * Not own.
     */
    CNATFWPluginApi* iPlugin;

    /**
     * Session where stream belongs to
     * Not own.
     */
    CNATFWSession& iContext;

    };

#endif // C_CNATFWSTREAMIMPL_H
