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
* Description:    Candidate
*
*/




#ifndef CNATFWCANDIDATE_H
#define CNATFWCANDIDATE_H

#include <e32base.h>
#include <in_sock.h>
#include "natfwconnectivityapidefs.h"

/**
 *  Represents candidate transport address (local/remote) for media receival.
 *
 *  @lib natconfw.lib
 *  @since S60 v3.2
 */
class CNATFWCandidate : public CBase
    {

public:

    /** Candidate type based on interface from which candidate is obtained. */
    enum TCandidateType {
        /** Candidate representing private address from local interface. */
        EHost                   = 1,

        /** Candidate representing public address allocated by a NAT as seen
        by used STUN-server. */
        EServerReflexive        = 2,

        /** Candidate representing public address allocated by a NAT as seen
        by the peer. */
        EPeerReflexive          = 3,

        /** Candidate representing public address allocated from STUN-relay
        server. */
        ERelay                  = 4
        };

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CNATFWCandidate* NewL();

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CNATFWCandidate* NewLC();

    /**
     * Two-phased copy constructor.
     * @param aCandidate        The candidate to copy
     */
    IMPORT_C static CNATFWCandidate* NewL(
        const CNATFWCandidate& aCandidate );

    /**
     * Two-phased copy constructor.
     * @param aCandidate        The candidate to copy
     */
    IMPORT_C static CNATFWCandidate* NewLC(
        const CNATFWCandidate& aCandidate );

    /**
     * Destructor.
     */
    virtual ~CNATFWCandidate();

    /**
     * Returns identifier for the session where candidate belongs to.
     *
     * @since   S60 v3.2
     * @return  The session identifier
     */
    IMPORT_C TUint SessionId() const;

    /**
     * Sets identifier of the session where candidate belongs to.
     *
     * @since   S60 v3.2
     * @param   aId                 The session identifier
     */
    IMPORT_C void SetSessionId( TUint aId );

    /**
     * Returns candidate stream collection identifier.
     *
     * @since   S60 v3.2
     * @return  Stream collection identifier
     */
    IMPORT_C TUint StreamCollectionId() const;

    /**
     * Sets candidate stream collection identifier.
     *
     * @since   S60 v3.2
     * @param   aId                 The stream collection identifier
     */
    IMPORT_C void SetStreamCollectionId( TUint aId );

    /**
     * Returns candidate stream identifier.
     *
     * @since   S60 v3.2
     * @return  Stream identifier
     */
    IMPORT_C TUint StreamId() const;

    /**
     * Sets candidate stream identifier.
     *
     * @since   S60 v3.2
     * @param   aId                 Stream identifier
     */
    IMPORT_C void SetStreamId( TUint aId );

    /**
     * Returns candidate priority.
     *
     * @since   S60 v3.2
     * @return  Candidate priority
     */
    IMPORT_C TUint Priority() const;

    /**
     * Sets candidate prioritity.
     *
     * @since   S60 v3.2
     * @param   aPriority           Candidate priority
     */
    IMPORT_C void SetPriority( TUint aPriority );

    /**
     * Returns candidate transport address. If address is not set or it is
     * available only in FQDN format, address of family KAFUnspec is returned.
     *
     * Addresses in FQDN format can be accessed using TransportDomainAddr()
     * and TransportDomainPort().
     *
     * @since   S60 v3.2
     * @return  Transport address
     */
    IMPORT_C const TInetAddr& TransportAddr() const;

    /**
     * Sets candidate transport address. If the domain address is not set, it
     * will be updated as well.
     *
     * @since   S60 v3.2
     * @param   aAddr               The transport address
     */
    IMPORT_C void SetTransportAddrL( const TInetAddr& aAddr );

     /**
     * Gets transport address in FQDN format. If address is not set or it is
     * available only as an IP-address, an empty descriptor is returned.
     *
     * IP addresses can be accessed using TransportAddr().
     *
     * @since   S60 v3.2
     * @return  Transport address in FQDN format
     */
    IMPORT_C const TDesC8& TransportDomainAddr() const;

    /**
     * Gets transport address port that is associated with the FQDN
     *
     * Port associated with an IP-address can be accessed using
     * TransportAddr().
     *
     * @since   S60 v3.2
     * @return  Port associated with the FQDN
     */
    IMPORT_C TUint TransportDomainPort() const;

    /**
     * Sets candidate transport domain address either in FQDN or IP address
     * format. If the address provided represents a valid IP address, the
     * candidate's transport IP address is updated as well.
     *
     * @since   S60 v3.2
     * @param   aAddr               The transport address
     * @param   aPort               The transport address port
     */
    IMPORT_C void SetTransportDomainAddrL( const TDesC8& aAddr, TUint aPort );

    /**
     * Returns candidate transportprotocol.
     *
     * @since   S60 v3.2
     * @return  Candidate transportprotocol
     */
    IMPORT_C TUint TransportProtocol() const;

    /**
     * Sets candidate transport protocol.
     *
     * @since   S60 v3.2
     * @param   aTransportProtocol  The transport protocol
     */
    IMPORT_C void SetTransportProtocol(
        TUint aTransportProtocol );

    /**
     * Returns base transport address of the candidate.
     *
     * @since   S60 v3.2
     * @return  Base of the candidate
     */
    IMPORT_C const TInetAddr& Base() const;

    /**
     * Sets base transport address of the candidate.
     *
     * @since   S60 v3.2
     * @param   aBase               The base
     */
    IMPORT_C void SetBase( const TInetAddr& aBase );

    /**
     * Returns candidate foundation.
     *
     * @since   S60 v3.2
     * @return  Foundation of the candidate
     */
    IMPORT_C const TDesC8& Foundation() const;

    /**
     * Sets candidate foundation.
     *
     * @since   S60 v3.2
     * @param   aFoundation         The foundation
     */
    IMPORT_C void SetFoundationL( const TDesC8& aFoundation );

    /**
     * Returns type of the candidate.
     *
     * @since   S60 v3.2
     * @return  The type of the candidate
     */
    IMPORT_C CNATFWCandidate::TCandidateType Type() const;

    /**
     * Sets type of the candidate.
     *
     * @since   S60 v3.2
     * @param   aType               The candidate type
     */
    IMPORT_C void SetType( CNATFWCandidate::TCandidateType aType );

    /**
     * Returns component identifier associated with the candidate.
     *
     * @since   S60 v3.2
     * @return  Component identifier
     */
    IMPORT_C TUint ComponentId() const;

    /**
     * Sets component identifier for the candidate.
     *
     * @since   S60 v3.2
     * @param   aComponentId        The component identifier
     */
    IMPORT_C void SetComponentId( TUint aComponentId );

    /**
     * Compares foundations between candidates.
     *
     * @since   S60 v3.2
     * @param   aSource             Source candidate for comparison
     * @param   aTarget             Target candidate for comparison
     * @return  ETrue if canditates are same
     */
    IMPORT_C static TBool CompareFoundations( const CNATFWCandidate& aSource,
        const CNATFWCandidate& aTarget );

    /**
     * Implements TLinearOrder using priority value in comparison.
     *
     * @since   S60 v3.2
     * @param   aC1                 Candidate one
     * @param   aC2                 Candidate two
     * @return  aC1.Priority - aC2.Priority
     */
    IMPORT_C static TInt PriorityOrder( const CNATFWCandidate& aC1,
        const CNATFWCandidate& aC2 );

    /**
     * Makes a copy of given candidate.
     *
     * @since   S60 v3.2
     * @param   aCandidate          The candidate to copy
     */
    IMPORT_C void CopyL( const CNATFWCandidate& aCandidate );

    IMPORT_C TBool operator==(const CNATFWCandidate& aCandidate) const;

    IMPORT_C TBool operator!=(const CNATFWCandidate& aCandidate) const;

private:

    CNATFWCandidate();
    void ConstructL();
    void ConstructL( const CNATFWCandidate& aCa );

private: // data

    /**
     * Session identifier
     */
    TUint iSessionId;

    /**
     * Stream identifier
     */
    TUint iStreamId;

    /**
     * Stream collection identifier
     */
    TUint iStreamCollectionId;

    /**
     * Component identifier
     */
    TUint iComponentId;

    /**
     * Type of the candidate
     */
    TCandidateType iType;

    /**
     * Candidate priority
     */
    TUint iPriority;

    /**
     * Candidate transport address
     */
    TInetAddr iTransportAddr;

    /**
     * Candidate transport domain address
     */
    RBuf8 iTransportDomainAddr;

    /**
     * Candidate transport domain port
     */
    TUint iTransportDomainPort;

    /**
     * Transport protocol
     */
    TUint iTransportProtocol;

    /**
     * Foundation of the candidate
     */
    RBuf8 iFoundation;

    /**
     * Base of the candidate
     */
    TInetAddr iBase;
    };

#endif // CNATFWCANDIDATE_H
