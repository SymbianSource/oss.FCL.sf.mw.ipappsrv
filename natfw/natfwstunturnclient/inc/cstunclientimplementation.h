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
* Description:    
*
*/




#ifndef C_STUNCLIENTIMPLEMENTATION_H
#define C_STUNCLIENTIMPLEMENTATION_H

// INCLUDES
#include <e32base.h>
#include <in_sock.h>
#include "natfwunsafserverresolver.h"
#include "natfwunsafserverresolverobserver.h"
#include "mnatfwunsaficmperrorobserver.h"
#include "msharedsecretobserver.h"
#include "mstunbindingobserver.h"
#include "cstuntimeruser.h"
#include "natfwstunclientdefs.h"


// FORWARD DECLARATIONS
class CDeltaTimer;
class CIcmpReceiver;
class MSTUNClientObserver;
class CSTUNClient;
class CBinding;
class CTransactionIDGenerator;
class CSTUNSharedSecret;
class CSTUNCredentials;
class CSTUNClientState;
class CSTUNClientResolvingTLS;
class CSTUNClientResolvingTCP;
class CSTUNClientResolvingUDP;
class CSTUNClientGetSharedSecret;
class CSTUNClientReady;
class CSTUNClientRenewSharedSecret;
class CAsyncCallback;
class MNcmConnectionMultiplexer;


// CLASS DECLARATION
/**
* This class hides the implementation STUN client from the user.
* @lib natfwstunclient.lib
*/
class CSTUNClientImplementation :    
    public CSTUNTimerUser,
    public MNATFWUNSAFServerResolverObserver,
    public MIcmpErrorObserver,
    public MSharedSecretObserver,
    public MSTUNBindingObserver
    {
    friend class ut_cstunrelaybindingimplementation;
    friend class ut_cstunclientimplementation;
    
    public: // Constructors and destructor
    
        /**
        * Creates a new instance of CSTUNClientImplementation.
        * Starts the initialization procedures for STUN Client.
        *     
        * @param aClient STUN client
        * @param aRetransmitInterval Retransmit interval for STUN binding
        *         requests
        * @param aServerAddress FQDN or IP address of the STUN or TURN 
        *        server to be used.
        * @param aServerPort if not zero, 
        *        server port to be used instead of STUN or TURN default port 
        * @param aServiceName Client must tell used servicename
        *        ("stun" or "stun-relay") because of discovery of server
        *        functionality
        * @param aSocketServ A connected socket server session
        * @param aConnection An active connection started by the client
        *        with RConnection::Start()
        * @param aTimer Timer services
        * @param aObserver A callback for STUN Client event notifications
        * @param aObtainSharedSecret If ETrue obtains a shared secret 
        *        which will be used for the STUN Binding Requests.
        *        If EFalse STUN Binding Requests will be sent without 
        *        the shared secret related attributes.
        * @param aFailIfNoSRVRecordsFound If ETrue, resolving fails if no SRV
        *         records for STUN server were found.
        * @param aIcmpReceiverUsed Desides whether icmp receiver is
        *        instantiated in STUN client
        * @param aMultiplexer instance of multiplexer.
        * @param aTransportProtocol Used transport protocol.
        *
        * @return A new instance, ownership is transferred.
        */
        static CSTUNClientImplementation* NewL(            
            CSTUNClient& aClient,
            TInt aRetransmitInterval,            
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
        
        /**
         * Creates a new instance of CSTUNClientImplementation (for 
         * ICE connectivity checks).
         * Starts the initialization procedures for STUN Client.
         *     
         * @param aClient STUN client
         * @param aRetransmitInterval Retransmit interval for STUN binding
         *        requests
         * @param aTimer Timer services
         * @param aObserver A callback for STUN Client event notifications
         * @param aMultiplexer instance of multiplexer.
         * @param aTransportProtocol Used transport protocol.
         *
         * @return A new instance, ownership is transferred.
         */
        static CSTUNClientImplementation* NewL(            
            CSTUNClient& aClient,
            TInt aRetransmitInterval,
            CDeltaTimer& aTimer,          
            MSTUNClientObserver& aObserver,
            MNcmConnectionMultiplexer* aMultiplexer,
            TTransportProtocol aTransportProtocol );
            
        /**
         * Destructor.
         */
        ~CSTUNClientImplementation();

    public: // From CSTUNTimerUser

        void TimerExpiredL();

        void LeaveFromTimerExpired( TInt aError );

    public: // From MUNSAFServerResolverObserver
    
        void CompletedL();

        void ErrorOccured( TInt aError );

    public: // From MIcmpErrorObserver

        void IcmpError( const TInetAddr& aAddress );

    public: // From MSharedSecretObserver

        void SharedSecretObtainedL();

        void SharedSecretErrorL( TInt aError );

    public: // From MSTUNBindingObserver

        const CSTUNClient& STUNClient() const;

        CDeltaTimer& TimerProvider();
        
        TInt RetransmitInterval() const;

        TTransportProtocol TransportProtocol() const;
        
        void AddressResolvedL( const CBinding& aBinding ) const;

        void ObtainSharedSecretL( CBinding& aBinding );

        TBool SharedSecretRejectedL( const CBinding& aBinding,
                                     const TDesC8& aUsername,
                                     const TDesC8& aPassword );

        TBool ObtainServerAddress( TInetAddr& aAddress );
        
        void ObtainTransactionIDL( TNATFWUNSAFTransactionID& aTransactionID );

        void AttachBindingL( const CBinding& aBinding );

        void DetachBinding( const CBinding& aBinding );
        
        void BindingErrorL( const CBinding& aBinding, TInt aError, 
                            TBool aIsFatal );
        
        void BindingEventOccurred( const CBinding& aBinding, 
                                   TSTUNCallbackInfo::TFunction aEvent );
        
        void RenewSharedSecretL();
        
        const TDesC8& UsernameForIndication();
        
        const TDesC8& PasswordForIndication();
        

    public: // New functions
    
        /**
        * Check whether this STUN Client has been properly initialized
        * and can be used to create STUN bindings.    
        * @return ETrue if the client is initialized, otherwise EFalse
        */
        TBool IsInitialized() const;
        
        /**
        * Gets the STUN server address and port used for this client when
        * sending Binding Requests.
        * @pre IsInitialized() == ETrue
        * @return STUN server used to keep the NAT bindings alive
        * @leave KErrNotReady if IsInitialized() == EFalse
        * @leave KErrNotFound if STUN client has no responding addresses left
        */
        const TInetAddr& STUNServerAddrL() const;

        /**
        * Sets credentials for the STUN server used.
        * These credentials override any existing ones and are used for all
        * the binding requests.
        * @pre aUsername's length must be larger than zero and a multiple of 4
        * @pre aPasswd's length must be larger than zero and a multiple of 4
        * @param aUsername username
        * @param aPasswd password
        */
        void SetCredentialsL( const TDesC8& aUsername, 
            const TDesC8& aPassword );

        /**
        * Tells if the Binding Requests will be sent protected with the
        * MESSAGE-INTEGRITY attribute.
        * @return ETrue if message integrity is used, EFalse otherwise
        */
        TBool SharedSecretObtained() const;

        /**
        * Returns information telling if application has set credentials.
        * @return ETrue  Application has provided credentials
        *          EFalse Otherwise
        */
        TBool HasPresetCredentials() const;

        /**
        * Change the object's state.        
        * @param aNewState State to enter, can be NULL. Ownership is not
        *    transferred.
        */
        void ChangeState( const CSTUNClientState* aNewState );

        /**
        * STUN client has encountered an error which it cannot ignore.
        * Pass the error to NAT Traversal FW and enter "terminated" state.        
        * @pre aError != KErrNone
        * @param aError Reason for the failure
        */
        void Terminate( TInt aError );
        
        
        /**
         * TLS resolving has been done (either successfully or failing).
         *
         * @since   s60 v3.2
         * @return  void        
         */
        void TlsResolvedL( );
                
        /**
        * TCP resolving has been done (either successfully or failing).
        * @param aObtainSharedSecret ETrue if shared secret should be obtained.        
        *                             EFalse if shared secret will not be used.
        */
        void TcpResolvedL( TBool aObtainSharedSecret );

        /**
        * Release resources allocated for address resolving.        
        */
        void FreeResolverResources();
        
        /**
        * Retrieve a shared secret from a STUN server, by sending a Shared
        * Secret Request.
        */
        void GetSharedSecretFromServerL();
        
        /**
        * Give the shared secret information the binding.
        * @param aBinding STUN binding requesting a shared secret.
        */
        void PassSharedSecretToBindingL( CBinding& aBinding ) const;

        /**
        * Pass the shared secret to all bindings that are waiting it.        
        */
        void InformWaitingBindingsL() const;

        /**
        * Error occurred while trying to obtain a shared secret. In case of a
        * connection failure or timeout, check if there are more TCP addresses
        * left to try.
        * @pre aError != KErrNone
        * @param aError Reason for the failure        
        */
        void HandleSharedSecretErrorL( TInt aError );

        /**
        * Check if the username and password match the values in currently used
        * shared secret.
        * @param aUsername Username to compare
         * @param aPassword Password to compare
         * @return ETrue Values match, EFalse otherwise    
        */
        TBool DoesSharedSecretMatch( const TDesC8& aUsername,
                                           const TDesC8& aPassword ) const;

        /**
        * Sends STUNClientInitCompleted event to upper layer, together with the
        * aError status code.
        * @param aError Initialization completion status
        */
        void PassInitCompletedL( TInt aError ) const;

        /**
        * Inform application that server rejected the provided credentials.
        * @param aBinding STUN binding that used the credentials
        */
        void PassCredentialsRejectedL( const CBinding& aBinding ) const;

        /**
        * Start a timer to control when the shared secret expires.
        */
        void StartSharedSecretTimer();
        
        /**
         * Multiplexer instance can be queried.
         *
         * @since   s60 v3.2
         * @return  multiplexer instance
         */
        MNcmConnectionMultiplexer* MultiplexerInstance();
        

    private: // Enumerations
    
        enum TSTUNConstants
            {
            EDefaultSTUNPort = 3478
            };

    private: // Constructors
    
        CSTUNClientImplementation( CSTUNClient& aClient,
                                   TInt aRetransmitInterval,
                                   TUint aServerPort,
                                   RSocketServ& aSocketServ,
                                   RConnection& aConnection,
                                   CDeltaTimer& aTimer,
                                   MSTUNClientObserver& aObserver,
                                   TBool aObtainSharedSecret,
                                   MNcmConnectionMultiplexer* aMultiplexer,
                                   TTransportProtocol aTransportProtocol );
                                   
        CSTUNClientImplementation( CSTUNClient& aClient,
                                   TInt aRetransmitInterval,
                                   CDeltaTimer& aTimer,
                                   MSTUNClientObserver& aObserver,
                                   MNcmConnectionMultiplexer* aMultiplexer,
                                   TTransportProtocol aTransportProtocol );

        CSTUNClientImplementation();
        

        CSTUNClientImplementation(
            const CSTUNClientImplementation& aImplementation );

        void ConstructL( const TDesC8& aServerAddress,
                         const TDesC8& aServiceName,
                         TBool aFailIfNoSRVRecordsFound,
                         TBool aIcmpReceiverUsed );
        
        void ConstructL();
        

    private: // New functions, for internal use        

        void ResolveAddressL( const TDesC8& aProtocol,
                              RArray<TInetAddr>& aResult );
        
        void ResolveAddressL( const TDesC8& aProtocol,
                              RArray<TInetAddr>& aResult,
                              TUint aPort,
                              const TDesC8& aServiceName );
        

        /**
        * Obtain STUN server's UDP addresses.
        */
        void ResolveUdpL();

        /**
        * Removes the specified address from the list of STUN server's UDP
        * addresses.
        * @param aAddress Address to remove
        */
        void RemoveAddress( const TInetAddr& aAddress );

    private: // Data

        CSTUNClient& iClient;
        
        /**
         * Retransmit interval
         */
        TInt iRetransmitInterval;
        
        /** 
         * STUN or TURN server's port to use. After resolving, 
         * the correct port to use is also in the TInetAddrs stored in 
         * iTcpAddresses and iUdpAddresses.
         */
        TUint iServerPort;
        
        /**
         * Reference to RSocketServ
         */
        RSocketServ& iSocketServer;
        
        /**
         * Reference to RConnection
         */
        RConnection& iConnection;

        /** 
         * Callback to upper layer
         * Not owned.
         */
        MSTUNClientObserver& iObserver;

        /**
         * If ETrue obtains a shared secret, which will be used for the STUN
         * Binding Requests. If EFalse STUN Binding Requests will be sent without
         * the shared secret related attributes.
         */
        TBool iObtainSharedSecret;
        
        /**
         * Pointer to multiplexer
         * Not own.
         */ 
        MNcmConnectionMultiplexer* iMultiplexer;

        /**
         * Callback dispatcher
         * Own.
         */
        CAsyncCallback* iAsyncCallback;

        /**
         * STUN or TURN server address (FQDN or IP address).
         * Exists only for the initialization phase.
         * Own.
         */
        HBufC8* iServerAddress;

        /**
         * For resolving the STUN server address, owned.
         * Exists only for the initialization phase.
         * Own.
         */
        CNATFWUNSAFServerResolver* iResolver;


        /** 
         * The first address in the array is the currently used one. If it no
         * longer responds, causing timeout, it will be removed from the array
         * and the next address will be the one to use. When addresses run out,
         * STUN client terminates and an error is reported to application.
         */

        /**
         * Resolved STUN server TCP addresses.
         */
        RArray<TInetAddr> iTcpAddresses;

        /**
         * Resolved STUN server UDP addresses.
         */
        RArray<TInetAddr> iUdpAddresses;

        /**
         * Transaction id generator
         * Own.
         */
        CTransactionIDGenerator* iTransactionIDGenerator;

        /**
         * Shared secret retriever. 
         * Own.
         */
        CSTUNSharedSecret* iSharedSecret;
        
        /**
         * Credentials set by application, can be NULL.
         * Own.
         */
        CSTUNCredentials* iCredentials;

        /**
         * Bindings associated with this STUN client instance. The bindings are
         * not owned by CSTUNClientImplementation.
         */
        RPointerArray<CBinding> iBindings;

        /**
         * icmp receiver
         * Own.
         */
        CIcmpReceiver* iIcmpReceiver;

        /**
         * The current state of the CSTUNClientImplementation, not owned.
         * If the pointer is NULL, it indicates the CSTUNClientImplementation
         * has "terminated".
         * Own.
         */
        const CSTUNClientState* iState;
        
        /**
         * STUN client's states
         * Own.
         */
        CSTUNClientResolvingTLS* iResolvingTLS;
        CSTUNClientResolvingTCP* iResolvingTCP;
        CSTUNClientResolvingUDP* iResolvingUDP;
        CSTUNClientGetSharedSecret* iGetSharedSecret;
        CSTUNClientReady* iReady;
        CSTUNClientRenewSharedSecret* iRenewSharedSecret;
        
        /**
         * Deltatimer, for shared timer services
         * Own.
         */
        CDeltaTimer* iTimer;
        
        /**
         * Service name contains NAT protocol, e.g. "STUN" or "STUN-RELAY"
         * Own.
         */
        HBufC8* iServiceName;
        
        /**
         * Transport protocol
         */
        TTransportProtocol iTransportProtocol;
        

    private: // For testing purposes

#ifdef TEST_EUNIT
        friend class CSTUNClientTest;
        friend class CSTUNClientImplementationTest;
        friend class CSTUNBindingTest;
        friend class CAsyncCallbackTest;
        friend class STUNTestUtils;
#endif
        __DECLARE_TEST;
    };

#endif // C_STUNCLIENTIMPLEMENTATION_H

// End of File
