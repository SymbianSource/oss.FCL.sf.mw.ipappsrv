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
* Description:    Connection multiplexer stream abstraction.
*
*/




#ifndef C_CNCMSTREAM_H
#define C_CNCMSTREAM_H

#include <e32base.h>
#include <in_sock.h>


class MNATFWMediaWrapper;
class CNcmConnection;
class MNcmMultiplexerConnectionObserver;
class CNcmConnectionObserverHandler;
class CNcmMediaSource;

/**
 *  Connection multiplexer stream abstraction.
 *
 *  @lib connectionmux.lib
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CNcmStream ) : public CBase
    {

public:

    /**
     * Two-phased constructor
     *
     * @param   aSessionId          The ID identifying session
     * @param   aStreamId           The ID identifying stream
     * @param   aQos                The quality of service
     * @param   aProtocol           KProtocolInetUdp / KProtocolInetTcp
     */
    static CNcmStream* NewL( TUint aSessionId, TUint aStreamId,
        TInt aQos, TUint aProtocol );

    /**
     * Two-phased constructor
     *
     * @param   aSessionId          The ID identifying session
     * @param   aStreamId           The ID identifying stream
     * @param   aQos                The quality of service
     * @param   aProtocol           KProtocolInetUdp / KProtocolInetTcp
     */
    static CNcmStream* NewLC( TUint aSessionId, TUint aStreamId,
        TInt aQos, TUint aProtocol );

    /**
     * Destructor
     */
    virtual ~CNcmStream();

    /**
     * Creates a new connection for stream. Will use same protocol as
     * used in stream.
     *
     * This is done synchronously.
     * 
     * @since   S60 v3.2
     * @param   aObserver                   Observer
     * @param   aConnectionObserverHandler  Connection observer handler
     * @param   aSocketServ                 Socket server
     * @param   aConnection                 RConnection
     * @param   aLocalAddress               Local address    
     * @param   aReuseAddress               If ETrue reuse the local address             
     * @return  The ID for the created connection
     */
    TUint CreateConnectionL( MNcmMultiplexerConnectionObserver& aObserver,
        CNcmConnectionObserverHandler& aConnectionObserverHandler,
        RSocketServ& aSocketServ, RConnection& aConnection,
        const TInetAddr& aLocalAddress, TBool aReuseAddress );
    
    /**
     * Removes connection
     *
     * @since   S60 v3.2
     * @param   aConnectionId       The ID identifying stream.
     * @return  void
     */ 
    void RemoveConnectionL( TUint aConnectionId );
    
    /**
     * Gets the stream ID
     *
     * @since   S60 v3.2
     * @return  The ID identifying stream.
     */ 
    TUint StreamId() const;
    
    /**
     * Gets the quality of service
     *
     * @since   S60 v3.2
     * @return  Quality of service.
     */ 
    TInt Qos() const;
    
    /**
     * Sets the qualiy of service
     *
     * @since   S60 v3.2
     * @param   aQos Qualiy of service
     * @return  void
     */ 
    void SetQos( TInt aQos );
    
    /**
     * Gets the protocol used in stream.
     *
     * @since   S60 v3.2
     * @return  The protocol, KProtocolInetUdp / KProtocolInetTcp
     */ 
    TUint StreamProtocol() const;
    
    /**
     * Sets the protocol used in stream.
     *
     * @since   S60 v3.2
     * @param   aProtocol       KProtocolInetUdp / KProtocolInetTcp  
     * @return  void
     */ 
    void SetStreamProtocol( TUint aProtocol );
   
    /**
     * Returns pointer to connection. Leaves with KErrNotFound if connection
     * isn't found.
     *
     * @since   S60 v3.2
     * @param   aConnectionId       The ID identifying sub stream.    
     * @return  Connection
     */   
    CNcmConnection* ConnectionL( TUint aConnectionId ); 
       
    /**
     * Registers a media wrapper into stream. Ownership of wrapper is
     * also tranferred. Leaves with KErrArgument if pointer to
     * media wrapper is NULL.
     *
     * @since   S60 v3.2
     * @param   aWrapper         Pointer to Media Wrapper
     * @return  void
     */
    void RegisterWrapperL( MNATFWMediaWrapper* aWrapper );
    
    /** 
     * Returns Media wrapper. Leaves with KErrNotFound if media wrapper 
     * isn't found.
     *
     * @since   S60 v3.2
     * @return  Media wrapper
     */
    MNATFWMediaWrapper* WrapperL();   
    
    /** 
     * Return the MediaConnectionId connection id.
     *
     * @since   S60 v3.2
     * @return  Media connection ID( 0 if not set )
     */
    TUint MediaConnectionId() const;

    /** 
     * Sets the media connection ID
     *
     * @since   S60 v3.2
     * @param   aMediaConnectionId      Media connection ID
     * @return  void
     */
    void SetMediaConnectionId( TUint aMediaConnectionId );
      
    /**
     * Returns pointer to connection observer handler.
     *
     * @since   S60 v3.2   
     * @return  Connection observer handler.
     */
    CNcmConnectionObserverHandler& ConnectionObserverHandler();
    
    /**
     * Find and return the connection ID by destination address.
     * Connection sender must be active.
     *
     * @since   S60 v3.2
     * @param   aDestinationAddress Destination address   
     * @return  Connection.
     */
    CNcmConnection* ConnectionByDestinationAddressL(
        const TInetAddr& aDestinationAddress );
        
    /**
     * Register a media source for stream. Ownership of media source is transferred.
     *
     * @since   S60 v3.2
     * @param   aMediaSource             The media source
     * @return  void
     */
    void RegisterMediaSourceL( CNcmMediaSource* aMediaSource );
    
private: //Private constructors

    CNcmStream( TUint aSessionId, TUint aStreamId,
        TInt aQos, TUint aProtocol );
    
    void ConstructL();

private: // data

    /**
     * Session ID
     */
    TUint iSessionId;
    
    /**
     * Stream ID 
     */
    TUint iStreamId;

    /**
     * QoS for connections
     */
    TInt iQos;
    
    /**
     * Stream protocol, KProtocolInetUdp / KProtocolInetTcp
     */
    TUint iStreamProtocol;
    
    /**
     * Registered wrapper
     * Own.
     */
    MNATFWMediaWrapper* iWrapper;
            
    /**
     * Connections within stream, implemented by multiplexers
     * Own.
     */
    RPointerArray<CNcmConnection> iConnections;
    
    /**
     * Next connection ID
     */
    TUint iNextConnectionId;
    
    /**
     * Media connection ID
     * This is zero if media connection is unspecified.
     * otherwise connection ID will be returned.
     */
    TUint iMediaConnectionId;
    
    /**
     * Handler for connection observers
     * Own.
     */
    CNcmConnectionObserverHandler* iConnectionObserverHandler;
    };
    
#endif // C_CNCMSTREAM_H
