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
* Description:    Provides STUN Client services.
*
*/




#ifndef CSTUNCLIENT_H
#define CSTUNCLIENT_H

#include <e32base.h>
#include <in_sock.h>
#include "natfwstunclientdefs.h"

class MSTUNClientObserver;
class CDeltaTimer;
class CSTUNClientImplementation;
class MNcmConnectionMultiplexer;

/**
 * A class for creating STUN bindings towards a particular STUN server.
 * During the initialization resolves the STUN server address using 
 * DNS queries and obtains a shared secret from the STUN server 
 * if required. Supports also STUN relay.
 * Client should set credentials if pre-provisioned ( also known as 
 * long term credentials ) just after NewL is called.
 * MSTUNClientObserver::STUNClientInitCompleted notifies when
 * STUN Client is initialized.
 *
 * @lib stunclient.lib
 */
class CSTUNClient : public CBase
    {
public:

    /**
     * Two-phased constructor.
     * Starts the initialization procedures for STUN Client.
     * MSTUNClientObserver::STUNClientInitCompleted gets called 
     * when the object is ready to be used.
     *
     * @param   aRetransmitInterval the initial retransmit interval 
     *          for STUN Binding Requests.
     * @param   aServerAddress FQDN or IP address of the STUN server.
     *          If FQDN, assumed to be a domain for which a SRV query 
     *          is performed. If SRV query fails a A/AAAA query is performed.
     * @param   aServerPort if not zero, 
     *          server port to be used instead of STUN default port. 
     * @param   aServiceName Client must tell used servicename
     *          "stun" if STUN used,
     *          "stun-relay" if STUN relay used.
     * @param   aSocketServ a connected socket server session
     * @param   aConnection an active connection started by the client
     *          with RConnection::Start()
     * @param   aTimer Timer services
     * @param   aObserver a callback for STUN Client event notifications
     * @param   aObtainSharedSecret if ETrue obtains a shared secret 
     *          which will be used for the STUN Binding Requests.
     *          If EFalse STUN Binding Requests will be sent without 
     *          the shared secret related attributes.
     * @param   aFailIfNoSRVRecordsFound if ETrue,
     *          MSTUNClientObserver::STUNClientInitCompleted 
     *          will be called with KErrNotFound, 
     *          if no SRV records for aSTUNServer with UDP were not found.
     * @param   aIcmpReceiverUsed Desides whether icmp receiver is
     *          instantiated in STUN client
     * @param   aMultiplexer Connection multiplexer.
     * @param   aTransportProtocol Transport protocol to use.
     */
    IMPORT_C static CSTUNClient* NewL( 
        TInt aRetransmitInterval,
        const TDesC8& aServerAddress,
        TUint aServerPort,
        const TDesC8& aServiceName,
        RSocketServ& aSocketServ,
        RConnection& aConnection,
        CDeltaTimer& aTimer,
        MSTUNClientObserver& aObserver,
        TBool aObtainSharedSecret=ETrue,
        TBool aFailIfNoSRVRecordsFound=EFalse,
        TBool aIcmpReceiverUsed=ETrue,
        MNcmConnectionMultiplexer* aMultiplexer=NULL,
        TTransportProtocol aTransportProtocol=EUdpProtocol );
    
    /**
     * Two-phased constructor.
     * Creates a new instance of CSTUNClient for ICE connectivity checks.
     * Starts the initialization procedures for STUN Client.
     * MSTUNClientObserver::STUNClientInitCompleted gets called 
     * when the object is ready to be used.
     *
     * @since   s60 v3.2
     * @param   aRetransmitInterval the initial retransmit interval 
     *          for STUN Binding Requests.
     * @param   aTimer Timer services
     * @param   aObserver a callback for STUN Client event notifications
     * @param   aMultiplexer Connection multiplexer.
     * @param   aTransportProtocol Used transport protocol.
     */
    IMPORT_C static CSTUNClient* NewL( 
        TInt aRetransmitInterval,
        CDeltaTimer& aTimer,
        MSTUNClientObserver& aObserver,
        MNcmConnectionMultiplexer* aMultiplexer=NULL,
        TTransportProtocol aTransportProtocol=EUdpProtocol );

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CSTUNClient* NewLC( 
        TInt aRetransmitInterval,
        const TDesC8& aServerAddress,
        TUint aServerPort,
        const TDesC8& aServiceName,
        RSocketServ& aSocketServ,
        RConnection& aConnection,
        CDeltaTimer& aTimer,
        MSTUNClientObserver& aObserver,
        TBool aObtainSharedSecret=ETrue,
        TBool aFailIfNoSRVRecordsFound=EFalse,
        TBool aIcmpReceiverUsed=ETrue,
        MNcmConnectionMultiplexer* aMultiplexer=NULL,
        TTransportProtocol aTransportProtocol=EUdpProtocol );
    
    /**
     * Two-phased constructor.
     */
    IMPORT_C static CSTUNClient* NewLC( 
        TInt aRetransmitInterval,
        CDeltaTimer& aTimer,
        MSTUNClientObserver& aObserver,
        MNcmConnectionMultiplexer* aMultiplexer=NULL,
        TTransportProtocol aTransportProtocol=EUdpProtocol );

    /**
     * Destructor.
     */
    IMPORT_C ~CSTUNClient();

    /**
     * Check whether this STUN Client has been properly initialized
     * and can be used to create STUN bindings.
     *    
     * @return   ETrue if the client is initialized, otherwise EFalse
     */
    IMPORT_C TBool IsInitialized() const;

    /**
     * Gets the STUN server address and port used for 
     * when sending Binding Requests.
     *
     * @pre     IsInitialized() == ETrue
     * @leave   KErrNotReady if IsInitialized() == EFalse
     * @leave   KErrNotFound if STUN client has no responding addresses left
     * @return  STUN server used to keep the NAT bindings alive
     */
    IMPORT_C const TInetAddr& STUNServerAddrL() const;
    
    /**
     * Sets credentials for the STUN server used.
     * These credentials override any existing ones. They are used to
     * obtain short term shared secret or act as credentials as they are.
     *
     * @pre     aUsername's length must be larger than zero and multiple of 4.
     * @pre     aPassword's length must be larger than zero and multiple of 4.
     * @param   aUsername username
     * @param   aPassword password
     * @return  void
     */
    IMPORT_C void SetCredentialsL( const TDesC8& aUsername,
        const TDesC8& aPassword );

    /**
     * Tells if the Binding Requests will be sent protected with the
     * MESSAGE-INTEGRITY attribute.
     *
     * @return   ETrue if message integrity is used, EFalse otherwise
     */
    IMPORT_C TBool SharedSecretObtained() const;

    /**
     * Requests a new UNSAF transaction ID. ID's type is
     * TNATFWUNSAFTransactionID but it is casted as a descriptor as
     * only TURN Plug-in needs to obtain the ID from STUN Client API.
     *
     * @since   s60 v3.2
     * @param   aTransactionID OUT: New Transaction ID
     * @return  void
     */
    IMPORT_C void ObtainTransactionIDL( TDesC8& aTransactionID );
    
    CSTUNClientImplementation& Implementation();

private:

    CSTUNClient();

    CSTUNClient( const CSTUNClient& aClient );
    
    void ConstructL( TInt aRetransmitInterval,
                     const TDesC8& aServerAddress,
                     TUint aServerPort,
                     const TDesC8& aServiceName,
                     RSocketServ& aSocketServ,
                     RConnection& aConnection,
                     CDeltaTimer& aTimer,
                     MSTUNClientObserver& aObserver,
                     TBool aObtainSharedSecret,
                     TBool aFailIfNoSRVRecordsFound,
                     TBool aIcmpReceiverUsed,
                     MNcmConnectionMultiplexer* aMultiplexer,
                     TTransportProtocol aTransportProtocol );

    void ConstructL( TInt aRetransmitInterval,
                     CDeltaTimer& aTimer,
                     MSTUNClientObserver& aObserver,
                     MNcmConnectionMultiplexer* aMultiplexer,
                     TTransportProtocol aTransportProtocol );

private: // Data

    /** 
     * Implementation.
     * Own.
     */ 
    CSTUNClientImplementation* iImplementation;

private:

    __DECLARE_TEST;
    };

#endif // CSTUNCLIENT_H

