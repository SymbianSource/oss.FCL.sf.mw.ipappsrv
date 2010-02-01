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
* Description:    Provides services for host candidate resolving.
*
*/




#ifndef C_ICEHOSTRESOLVER_H
#define C_ICEHOSTRESOLVER_H

#include "natfwpluginapi.h"
#include "natfwconnectivityapidefs.h"
#include "miceconnectionobserver.h"
#include "natfwinternaldefs.h"    //UNIT_TEST

class CIceCallBackExecuter;
class CIceConnection;

/**
 *  Provides services for host candidate resolving.
 *
 *  @lib icecandidatehandler.dll
 *  @since S60 v3.2
 */
class CIceHostResolver 
    : public CNATFWPluginApi, public MIceConnectionObserver
    {

    UNIT_TEST( UT_CIceHostResolver )
    UNIT_TEST( UT_CIceConnectivityCheck )
    UNIT_TEST( UT_CIceCheckList )
    
public:

    /**  CallBack types */
    enum TCallBackType
        {
        ECallBackServerConnected   = 1,
        ECallBackCandidateFound    = 2
        };

    /**
     * Two-phased constructor.
     * @param aInitParams       Initialization parameters
     */
    static CIceHostResolver* NewL( TAny* aInitParams );

    /**
     * Destructor.
     */
    virtual ~CIceHostResolver();
    
    /**
     * Static handler function for callbacks.
     *
     * @since   S60 3.2
     * @param   aObject         The object reference
     * @param   aCallBackType   The callback to be executed
     */
    static void HandleCallBack( CBase& aObject, TInt aCallBackType );
    
    /**
     * Returns local address for specified stream and family.
     *
     * @since   S60 3.2
     * @param   aStreamId       The stream identifier
     * @param   aAddrFamily     The address family
     * @return  Local address
     */
    TInetAddr LocalAddress( TUint aStreamId, TUint aAddrFamily ) const;
    
// from base class CNATFWPluginApi

    /**
     * From CNATFWPluginApi.
     *
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
     */
    void ConnectServerL( const RSocketServ& aSocketServ,
        const TName& aConnectionName );
        
    /**
     * From CNATFWPluginApi.
     *
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
    void FetchCandidateL( TUint aStreamId, TUint aRtoValue,
        TUint aAddrFamily );
    
    
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
     * @pre     FetchCandidate(/s) and possible ICE processing have completed
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
     * @pre     FetchCandidate(/s) and possible ICE processing have completed
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

// from base class MIceConnectionObserver

    /**
     * Notifies about connection state changes.
     *
     * @since   S60 v3.2
     * @param   aConnection         Notifying connection
     * @param   aType               Type of notify
     * @param   aError              Error code
     */
    void ConnectionNotify( CIceConnection& aConnection,
        TNotifyType aType, TInt aError );
    
private:

    CIceHostResolver();

    void ConstructL( TAny* aInitParams );
    
    void DoHandleCallBack( TInt aCallBackType );
    
    CIceConnection* FindConnection( const CNATFWCandidate& aLocalCandidate );
    
    CIceConnection* FindConnection( const TInetAddr& aLocalAddr );
    
    TBool IsOwnedCandidate( const CNATFWCandidate& aLocalCandidate ) const;

private: // data


    /**
     * Plugin identifier.
     * Own.
     */
    HBufC8* iPluginId;
    
    /**
     * Plugin observer.
     * Not Own.
     */
    MNATFWPluginObserver* iPluginObserver;
    
    /**
     * Interface to multiplexer services.
     * Not Own.
     */
    MNcmConnectionMultiplexer* iMultiplexer;
    
    /**
     * Host candidates pending posting.
     * Own.
     */
    RPointerArray<CNATFWCandidate> iHostCandidates;
    
    /**
     * CallBack executer for asynchronous callback generation.
     * Own.
     */
    CIceCallBackExecuter* iCallBackExecuter;
    
    /**
     * Host connections.
     * Own.
     */
    RPointerArray<CIceConnection> iConnections;
    };


#endif // C_ICEHOSTRESOLVER_H
