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
* Description:    Connection multiplexer session abstraction.
*
*/




#ifndef C_CNCMSESSION_H
#define C_CNCMSESSION_H

#include <e32base.h>
#include <in_sock.h>
#include <commdbconnpref.h>

#include "mncmicmpobserver.h"

class CNcmStream;
class MNcmConnectionMultiplexerObserver;
class MNcmSessionObserver;
class CNcmPortStore;
class CNcmIcmpV4Receiver;
class CNcmIcmpV6Receiver;

/**
 *  Connection multiplexer session abstraction.
 *
 *  @lib connectionmux.lib
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CNcmSession ) : public CActive,
    public MNcmIcmpObserver
    {

public:

    /**
     * Two-phased constructor
     *
     * @param   aSessionId          The ID identifying session
     * @param   aIapId              IAP id
     * @param   aPortRangeStart     Port range start for session
     * @param   aPortRangeStop      Port range stop for session
     * @param   aObserver           Observer
     * @param   aSessionObserver    Session observer 
     */
    static CNcmSession* NewL( TUint aSessionId, TUint32 aIapId,
        TUint aPortRangeStart, TUint aPortRangeStop,
        MNcmConnectionMultiplexerObserver& aObserver,
        MNcmSessionObserver& aSessionObserver );

    /**
     * Two-phased constructor
     *
     * @param   aSessionId          The ID identifying session
     * @param   aIapId              IAP id
     * @param   aPortRangeStart     Port range start for session
     * @param   aPortRangeStop      Port range stop for session
     * @param   aObserver           Observer
     * @param   aSessionObserver    Session observer 
     */
    static CNcmSession* NewLC( TUint aSessionId, TUint32 aIapId,
        TUint aPortRangeStart, TUint aPortRangeStop,
        MNcmConnectionMultiplexerObserver& aObserver,
        MNcmSessionObserver& aSessionObserver );
        
    /**
     * Destructor
     */
    virtual ~CNcmSession();

    /**
     * Creates a new stream. On return the client receives a
     * stream ID. 
     *
     * @since   S60 3.2
     * @param   aQos            The desired quality of service.
     * @param   aProtocol       KProtocolInetUdp / KProtocolInetTcp
     * @return  The ID for the created stream
     */     
    TUint CreateStreamL( TInt aQos, TUint aProtocol );
    
    /**
     * Removes stream and all its connections. 
     *
     * @since   S60 3.2
     * @param   aStreamId       The ID identifying stream
     * @return  void
     */
    void RemoveStreamL( TUint aStreamId );
    
    /**
     * Get session Id
     *
     * @since   S60 v3.2
     * @return  Session ID
     */
    TUint SessionId() const;
    
    /**
     * Get IAP Id
     *
     * @since   S60 v3.2
     * @return  IAP ID
     */
    TUint32 IapId() const;
    
    /**
     * Gets RSocketServ used on this session
     *
     * @since   S60 v3.2
     * @return  RSocketServ used by this session
     */
    RSocketServ& SocketServer();
    
    /**
     * Gets RConnection used on this session
     *
     * @since S60 v3.2
     * @return RConnection used by this session
     */
    RConnection& Connection();
    
    /**
     * Gets the unique name of an RConnection used on this session
     *
     * @since   S60 v3.2
     * @param   aConnectionName     The unique name of an RConnection
     * @return  system-wide error code
     */
    TInt GetConnectionName( TName& aConnectionName );
    
    /**
     * Returns stream by identifier. Leaves with KErrNotFound if stream
     * isn't found.
     *
     * @since   S60 3.2
     * @param   aStreamId           The ID identifying stream
     * @return  Stream
     */
    CNcmStream* StreamByIdL( TUint aStreamId );
    
    /**
     * Receives port store
     *
     * @since   S60 v3.2
     * @return  port store used on this session
     */     
    CNcmPortStore& PortStore() const;
            
    /**
     * Return session initialization status
     *
     * @since   S60 v3.2
     * @return  Initialization state    ETrue if initialized,
     *                                  EFalse if not initialized
     */
    TBool Initialized() const;
    
    /**
     * Resolve the destination IP address from FQDN.
     * ( Address and port )
     *
     * @since   S60 v3.2
     * @param   aAddress       The Destination FQDN
     * @param   aPort          The Destination port
     * @param   aResult        On return contains the resolved IP address
     * @return  void
     */
    void ResolveDestinationAddressL( const TDesC8& aAddress, TUint aPort,
        TInetAddr& aResult );
    
    /**
     * Gets the local IPv4 address.
     *
     * @since   S60 v3.2
     * @return  Local IPv4 address
     */
    TInetAddr& LocalIPv4Address();

    /**
     * Gets the local IPv6 address.
     *
     * @since   S60 v3.2
     * @return  Local IPv6 address
     */
    TInetAddr& LocalIPv6Address();
    
    /**
     * From MNcmIcmpObserver
     *
     * Notifies that an ICMP error is occurred using
     * the specified remote address. Gets a local address and
     * a remote address to be used in the creation of the ICMP message.
     *
     * @since   S60 v3.2
     * @param   aAddress        Remote address on which the error occurred
     * @param   aLocalAddress   Reference to local address
     * @param   aRemoteAddress  Reference to remote address
     * @return  void
     */
    void IcmpError( const TInetAddr& aAddress, TInetAddr& aLocalAddress,
        TInetAddr& aRemoteAddress );
        
protected:

    // from base class CActive

    void RunL();
        
    void DoCancel();
    
    TInt RunError( TInt aError );
    
private:

    CNcmSession( TUint aSessionId, TUint32 aIapId,
        MNcmConnectionMultiplexerObserver& aObserver,
        MNcmSessionObserver& aSessionObserver );

    void ConstructL( TUint aPortRangeStart, TUint aPortRangeStop );
    
    TUint StreamIndexL( TUint aStreamId );
    
    TUint GenerateStreamId() const;
    
    void HandleIcmpErrorL( const TInetAddr& aAddress,
        TInetAddr& aLocalAddress, TInetAddr& aRemoteAddress );

private: // data

    /**
     * Unique session identifier
     */
     TUint iSessionId;
     
    /**
     * Internet access point identifier
     */
    TUint32 iIapId;
    
    /** 
     * Connection for socket
     * Own.
     */
    RConnection iConnection;

    /**
     * Socket server for this connection
     * Own.
     */
    RSocketServ iSocketServ;
    
    /**
     * Streams created into the session
     * Own.
     */
    RPointerArray<CNcmStream> iStreams;
       
    /**
     * Handler for connection multiplexer observer
     */
    MNcmConnectionMultiplexerObserver& iObserver; 
    
    /**
     * Handler for multiplexer session observer
     */
    MNcmSessionObserver& iSessionObserver;  
    
    /**
     * Store where local ports are obtained
     * Own.
     */
    CNcmPortStore* iPortStore;    
    
    /**
     * Flag to indicate whether session is initilized
     */
    TBool iInitialized;

    /**
     * Local IPv4 address
     */    
    TInetAddr iIpv4Addr;

    /**
     * Local IPv6 address
     */     
    TInetAddr iIpv6Addr;

    /**
     * Connection preferences 
     */      
    TCommDbConnPref iConnPref;

    /**
     * Receives and handles ICMPv4 errors.
     * Not own.
     */    
    CNcmIcmpV4Receiver* iIcmpV4receiver;

    /**
     * Receives and handles ICMPv6 errors.
     * Not own.
     */    
    CNcmIcmpV6Receiver* iIcmpV6receiver;    
    };


#endif // C_CNCMSESSION_H

