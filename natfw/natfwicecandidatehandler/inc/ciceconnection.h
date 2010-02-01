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
* Description:    Wrapper class to convert multiplexer connection into object.
*
*/




#ifndef C_CICECONNECTION_H
#define C_CICECONNECTION_H

#include <in_sock.h>
#include "natfwconnectivityapidefs.h"
#include "mncmconnectionobserver.h"
#include "miceconnectionobserver.h"
#include "natfwinternaldefs.h"    //UNIT_TEST

class MNcmConnectionMultiplexer;
class MIceConnectionObserver;
class CNATFWCandidate;


/**
 *  Wrapper class to convert multiplexer connection into object.
 *
 *  @lib icecandidatehandler.dll
 *  @since S60 v3.2
 */
class CIceConnection : public CBase, public MNcmConnectionObserver
    {

    UNIT_TEST( UT_CIceConnection )

public:

    /**  Current state of the connection */
    enum TConnectionState
        {
        EConnectionStateUninitialized   = 0,
        EConnectionStateRecv            = 1,
        EConnectionStateSend            = 2,
        EConnectionStateSendRecv        = 3
        };
    
    /** Connection notify state. When enabled connection generates callbacks
        when asynchronous request is completed. */
    enum TConnectionNotifyState
        {
        ENotifyStateEnabled             = 0,
        ENotifyStateDisabled            = 1
        };
    
    /**
     * Two-phased constructor.
     * @param   aStreamId       The stream identifier into which connection
     *                          is created
     * @param   aAddrFamily     KAfInet / KAfInet6
     * @param   aMultiplexer    The multiplexer
     * @param   aObserver       The observer for the connection
     */
    static CIceConnection* NewL( TUint aStreamId, 
        TUint aAddrFamily,
        MNcmConnectionMultiplexer& aMultiplexer, 
        MIceConnectionObserver& aObserver );
    
    /**
     * Two-phased constructor.
     * @param   aStreamId       The stream identifier into which connection
     *                          is created
     * @param   aAddrFamily     KAfInet / KAfInet6
     * @param   aMultiplexer    The multiplexer
     * @param   aObserver       The observer for the connection
     */
    static CIceConnection* NewLC( TUint aStreamId, 
        TUint aAddrFamily,
        MNcmConnectionMultiplexer& aMultiplexer, 
        MIceConnectionObserver& aObserver );
    
    /**
     * Destructor.
     */
    virtual ~CIceConnection();

    /**
     * Returns multiplexer stream identifier where connection is created.
     *
     * @since   S60 v3.2
     * @return  Stream identifier
     */
    TUint StreamId() const;
    
    /**
     * Returns current state of the connection.
     *
     * @since   S60 v3.2
     * @return  State
     */
    TConnectionState State() const;
    
    /**
     * Returns local transport address where connection is bound to.
     *
     * @since   S60 v3.2
     * @return  Local transport address
     */
    const TInetAddr& LocalAddress() const;
    
    /**
     * Returns transport protocol used with the connection.
     *
     * @since   S60 v3.2
     * @return  KProtocolInetUdp / KProtocolInetTcp
     */
    TUint TransportProtocol() const;
    
    /**
     * Returns connection id used with the connection.
     *
     * @since   S60 v3.2
     * @return  Connection identifier
     */
    TUint ConnectionId() const;    
    
    /**
     * Activates/deactivates receiving on a connection.
     *
     * @since   S60 v3.2
     * @param   aState          The streaming status
     * @param   aNotifyState    Indicates whether request should generate
     *                          callback.
     */
    void SetReceivingStateL( TNATFWStreamingState aState,
        TConnectionNotifyState aNotifyState );
    
    /**
     * Activates/deactivates sending to the specified destination address.
     *
     * @since   S60 v3.2
     * @param   aState          The streaming status
     * @param   aNotifyState    Indicates whether request should generate
     *                          callback.
     * @param   aDestAddr       The address where to send media
     */
    void SetSendingStateL( TNATFWStreamingState aState,
        TConnectionNotifyState aNotifyState,
        const TInetAddr& aDestAddr );
    
    /**
     * Binds given candidate with this connection.
     *
     * @since   S60 v3.2
     * @param   aCandidate       The candidate to associate with connection
     */
    void BindWithCandidateL( const CNATFWCandidate& aCandidate );
    
    /**
     * Returns boolean indicating whether specified candidate is associated
     * with this connection.
     *
     * @since   S60 v3.2
     * @param   aCandidate       The candidate
     */
    TBool IsBoundToCandidate( const CNATFWCandidate& aCandidate ) const;
    
// from base class MNcmConnectionObserver

    /**
     * From MNcmConnectionObserver.
     * Called by Connection Multiplexer when actions for connection completes.
     *
     * @since    S60 v3.2
     * @param    aStreamId           The ID identifying stream
     * @param    aConnectionId       The ID identifying connection
     * @param    aType               Type of notify
     * @param    aError              Error code
     */
    void ConnectionNotify( TUint aStreamId, TUint aConnectionId,
        TConnectionNotifyType aType, TInt aError );

private:

    CIceConnection( TUint aStreamId, 
        MNcmConnectionMultiplexer& aMultiplexer, 
        MIceConnectionObserver& aObserver );

    void ConstructL( TUint aAddrFamily );
    
    MIceConnectionObserver::TNotifyType ConvertToIceConnectionEvent( 
        MNcmConnectionObserver::TConnectionNotifyType aType ) const;

private: // data

    /**
     * Connection state.
     */
    TConnectionState iState;
    
    /**
     * Connection notify state.
     */
    TConnectionNotifyState iNotifyState;
    
    /**
     * Tranport protocol used with the connection.
     */
    TUint iProtocol;
    
    /**
     * Multiplexer stream identifier.
     */
    TUint iStreamId;
    
    /**
     * Multiplexer connection identifier.
     */
    TUint iConnectionId;
    
    /**
     * Local address where connection is bound to.
     */
    TInetAddr iLocalAddress;
    
    /**
     * The multiplexer instance to use.
     * Not own.
     */
    MNcmConnectionMultiplexer& iMultiplexer;

    /**
     * Observer for the connection.
     * Not own.
     */
    MIceConnectionObserver& iObserver;
    
    /**
     * Candidates which are associated with this connection.
     * Own.
     */
    RPointerArray<CNATFWCandidate> iBoundCandidates;
    };


#endif // C_CICECONNECTION_H
