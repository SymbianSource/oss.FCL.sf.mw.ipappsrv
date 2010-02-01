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





#ifndef C_CBINDINGIMPLEMENTATION_H
#define C_CBINDINGIMPLEMENTATION_H

// INCLUDES
#include <e32base.h>
#include <es_sock.h>
#include <in_sock.h>
#include "mstuntransactionobserver.h"
#include "cstuntimeruser.h"
#include "mstunbindingobserver.h"
#include "natfwstunclientdefs.h"

// FORWARD DECLARATIONS
class CNATFWUNSAFMessage;
class CNATFWUNSAFUnknownAttributesAttribute;
class CBinding;
class CSTUNBindingState;
class CSTUNBindingInit;
class CSTUNBindingGetSharedSecret;
class CSTUNBindingGetAddress;
class CSTUNBindingActive;
class CSTUNBindingWaitToRetry;
class CSTUNClient;
class MSTUNBindingObserver;
class CSTUNTransaction;
class CStunIndicationTransmitter;
class MNcmConnectionMultiplexer;



// CLASS DECLARATION
/**
 * Hides the implementation of bindings for the user of STUNClient.
 *
 * @lib     natfwstunclient.lib
 * @since   s60 v3.2
 */
class CBindingImplementation : public CSTUNTimerUser,
                               public MSTUNTransactionObserver
    {
        
public: // default constructor

    CBindingImplementation( MSTUNBindingObserver& aClient,
                            CBinding& aBinding,                                
                            RSocket& aSocket );
    
    // overloaded default constructor
    CBindingImplementation( MSTUNBindingObserver& aClient,
                            CBinding& aBinding,
                            TUint aStreamId,
                            TUint aConnectionId,
                            MNcmConnectionMultiplexer* aMultiplexer );
                        
public: // From CSTUNTimerUser

    virtual void TimerExpiredL() = 0;

    virtual void LeaveFromTimerExpired( TInt aError ) = 0;

public: // From MSTUNTransactionObserver

    virtual void PublicAddressObtainedL( const TInetAddr& aAddress ) = 0;
    
    // for stun relay functionality
    virtual void PublicAddressObtainedL( const TInetAddr& aReflexiveAddr,
                                         const TInetAddr& aRelayAddr ) = 0;

    virtual void TransactionError( TInt aError,
                CNATFWUNSAFUnknownAttributesAttribute* aUnknownAttr ) = 0;
    
    virtual void TransactionEventOccurredL( 
        TSTUNCallbackInfo::TFunction aEvent ) = 0;
        
    virtual void NewRTTSampleMeasured( TInt64& aRTTSample,
        TBool aRetransmitted );
    

public: // New functions

    /**
     * Refreshes the binding causing a new Binding Request to be sent
     * to the STUN server. The public IP address is available when 
     * MSTUNClientObserver gets called.
     *
     * @since   s60 v3.2
     * @return  void
     */
    virtual void SendRequestL();
    
    /**
     * Overloaded version of SendRequestL().
     *
     * @since   S60 v3.2
     * @param   aDestAddr       Destination where to send request
                                (KAFUnspec=default address)
     * @param   aAddFingerprint Whether to include fingerprint attribute
                                to request
     * @param   aRtoValue       Retransmission timeout
                                (=0 if default to be used)
     * @return  void
     */
    virtual void SendRequestL( const TInetAddr& aDestAddr,
                               TBool aAddFingerprint,
                               TUint aRtoValue );    
    
    /**
     * Refreshes the binding causing a new Binding Request to be sent
     * to the STUN server. The public IP address is available when 
     * MSTUNClientObserver gets called.
     *
     * @since   s60 v3.2
     * @param   aRtoValue Retransmission timeout(=0 if default to be used)
     * @return  void
     */
    virtual void AllocateRequestL( TUint aRtoValue );

    /**
     * Cancels a previously issued call to SendRequestL.
     *
     * @since   s60 v3.2
     * @return  void
     */
    virtual void CancelRequest() = 0;

    /**
     * Gets the socket used for obtaining the public address.
     *
     * @since   s60 v3.2
     * @return  socket
     */
    virtual const RSocket& Socket() const = 0;

    /**
     * Checks whether the public address has a value set
     *
     * @since   s60 v3.2
     * @return  ETrue if the public address has been obtained, 
     *          otherwise EFalse. 
     */
    virtual TBool AddressResolved() const = 0;

    /**
     * Gets the public address for this binding as seen in public network.
     *
     * @since   s60 v3.2
     * @return public address
     */
    virtual const TInetAddr& PublicAddr() const = 0;
    
    /**
     * Returns alternate server transport address.
     *
     * @since   s60 v3.2
     * @return alternate server address
     */
    virtual const TInetAddr& AlternateServerAddr() const = 0;
    
    /**
     * Returns realm value from error response.
     *
     * @since   s60 v3.2
     * @return realm from error response         
     */
    virtual const HBufC8* RealmFromResponse() const = 0; 
            
    /**
     * The client should call this function when it has received data
     * from the socket shared with this binding and 
     * the client cannot itself handle the data.
     *
     * @since   s60 v3.2  
     * @param   aData data received
     * @param   aConsumed on return ETrue if the data was accepted,
     *          otherwise EFalse.
     * @param   aRemoteAddr remote address from remote address attribute
     * @return  Decoded data if data is indication and not consumed,
     *          otherwise NULL. Ownership is transferred.
     */
    virtual HBufC8* HandleDataL( const TDesC8& aData, 
                                 TBool& aConsumed,
                                 TInetAddr& aRemoteAddr ) = 0;

    /**
     * Returns the associated CSTUNClient instance.
     *
     * @since   s60 v3.2
     * @return CSTUNClient, or NULL if the CSTUNClient has been deleted        
     */
    virtual const CSTUNClient* STUNClient() const = 0;

    /**
     * Get a STUN server address from STUN client
     *
     * @since   s60 v3.2
     * @return  void
     */
    virtual void GetServerAddressL() = 0;

    /**
     * Get a fresh a shared secret.
     *
     * @since   s60 v3.2
     * @return  void  
     */
    virtual void GetSharedSecretL() = 0;

    /**
     * Clears the pointer to CSTUNClient.
     *
     * @since   s60 v3.2
     * @post    iClient == NULL
     * @return  void   
     */
    virtual void DetachClient() = 0;

    /**
     * Store the obtained public address and inform application.
     *
     * @since   s60 v3.2
     * @param   aPublicAddress Public address
     * @return  void
     */
    virtual void StoreAddressL( const TInetAddr& aPublicAddress ) = 0;

    /**
     * Store the obtained addresses and inform application.
     *
     * @since   s60 v3.2
     * @param   aReflexiveAddr  public IP address (server reflexive)
     * @param   aRelayAddr      public relay IP address
     * return   void
     */
    virtual void StoreAddressL( const TInetAddr& aReflexiveAddr,
                                const TInetAddr& aRelayAddr );

    /**
     * Change the object's state.
     *
     * @since   s60 v3.2
     * @param   aNewState State to enter.
     * @return  void
     */
    virtual void ChangeState( CSTUNBindingState& aNewState ) = 0;

    /**
     * Binding encountered an error and enters terminated state. The function
     * MSTUNClientObserver::BindingErrorL is used to inform application.
     *
     * @since   s60 v3.2
     * @pre     aError != KErrNone
     * @param   aError Error code
     * @return  void
     */
    virtual void Terminate( TInt aError ) = 0;

    /**
     * Checks if this binding is waiting for shared secret.
     *
     * @since   s60 v3.2
     * @return  ETrue BInding is waiting shared secret, EFalse otherwise
     */
    virtual TBool IsWaitingSharedSecret() const = 0;

    /**
     * STUN client uses this function to give the username and password, that
     * the binding requested earlier with function
     * MSTUNBindingObserver::ObtainSharedSecretL.
     *
     * @since   s60 v3.2
     * @pre     aUsername and aPassword must either both have a descriptor that
     *          is not empty, or then both must have an empty descritor.
     * @param   aUsername Username or an empty descriptor if the shared secret
     *          is not used.
     * @param   aPassword Password or an empty descriptor if the shared secret
     *          is not used.
     * @param   void
     */
    virtual void SharedSecretObtainedL( const TDesC8& aUsername,
        const TDesC8& aPassword ) = 0;

    /**
     * Creates or updates a Binding Request message, using the given username
     * and password.
     *
     * @since   s60 v3.2
     * @param   aUsername Username
     * @param   aPassword Password
     * @return  void
     */
    virtual void CreateBindingRequestL( const TDesC8& aUsername,
        const TDesC8& aPassword ) = 0;

    /**
     * Creates a transaction for sending Binding Request message
     *
     * @since   s60 v3.2
     * @pre     iRequest != NULL
     * @return  void
     */      
    virtual void SendBindingRequestL() = 0;

    /**
     * Take actions based on the reason why transaction failed. For certain
     * errors, it is possible to send the Binding Request again.
     *
     * @since   s60 v3.2
     * @pre     aError != KErrNone
     * @param   aError Error reason
     * @return  ETrue Request can be retried
     *          EFalse Binding enters Terminated-state
     */
    virtual TBool HandleTransactionError( TInt aError ) = 0;

    /**
     * Forward the ICMP error to transaction for processing.
     *
     * @since   s60 v3.2
     * @param   aAddress Address where the ICMP error occurred
     * @return  void
     */
    virtual void IcmpError( const TInetAddr& aAddress ) = 0;

    /**
     * Wait before retring to send a Binding Request
     *
     * @since   s60 v3.2
     * @return  void
     */
    virtual void WaitBeforeRetrying() = 0;
    
    /**
     * ICE specific attributes can be set for the request.
     *  
     * @since   s60 v3.2
     * @param   aAttributes     Contains ICE specific attributes.
     * @return  void
     */
    virtual void SetICESpecificAttributes( 
        const TICEAttributes& aAttributes ) = 0;
    
    /**
     * Returns the relay address allocated for this binding.
     *
     * @since   s60 v3.2
     * @return  STUN Relay public IP address
     */               
    virtual const TInetAddr& RelayAddr() const;
    
    /**
     * Once the client wants to primarily receive from one peer, it can send
     * a SetActiveDestination request.  All subsequent data received from
     * the active peer is forwarded directly to the client and vice versa,
     * except that it is wrapped or framed according to the protocol used
     * between the STUN relay client and STUN relay server.
     * The SetActiveDestination Request does not close other bindings. Data
     * to and from other peers is still wrapped in Send and Data indications
     * respectively.
     *
     * @since   s60 v3.2
     * @param   aRemoteAddr Client can specify remote address
     * @since   aTimerValue Client gets timer value after response
     * @return  void
     */
    virtual void SetActiveDestinationRequestL( 
        const TInetAddr& aRemoteAddr, TUint32& aTimerValue );
    
    /**
     * Send indication to remote address. Indications are not
     * retransmitted and responses are not expected.
     *
     * @since   s60 v3.2
     * @param   aRemoteAddr     Address to send to (remote agent)
     * @param   aData           Data to send to remote agent. Use KNullDesC8
     *                          if there is no data available.
     * @param   aAddFingerprint Add fingerprint attribute if true
     * @return  void
     */
    virtual void SendIndicationL( const TInetAddr& aRemoteAddr, 
        const TDesC8& aData, TBool aAddFingerprint ) = 0;
    
    /**
     * The Connect Request is used by a client when it has obtained an
     * allocated transport address that is TCP.  The client can use the
     * Connect Request to ask the server to open a TCP connection to a
     * specified destination address included in the request.
     *
     * @since   s60 v3.2
     * @param   aRemoteAddr remote address
     * @return  void
     */
    virtual void ConnectRequestL( const TInetAddr& aRemoteAddr );
    
    /**
     * client can query used stream id
     *
     * @since   s60 v3.2
     * @return  stream Id
     */
    virtual TUint StreamId( ) const = 0;

    /**
     * client can query used connection id
     *
     * @since   s60 v3.2
     * @return  connection Id
     */
    virtual TUint ConnectionId( ) const = 0;
    

public: // New functions, for internal use

    /**
     * Release the memory of the Binding Request related data. This data is
     * only needed for sending a Binding Request and receiving a response.
     *
     * @since   s60 v3.2
     * @return  void
     */
    virtual void FreeRequestData() = 0;

    /**
     * Return STUN client handle.
     *
     * @since   s60 v3.2
     * @pre     iClient != NULL
     * @return  STUN client
     * @leave   KErrNotFound If the STUN client has been deleted
     */
    virtual MSTUNBindingObserver& ClientL() const = 0;

    /**
     * Return the current username, or an empty descriptor if username
     * does not exist.
     *
     * @since   s60 v3.2
     * @return  Username
     */
    virtual const TDesC8& Username() const = 0;

    /**
     * Checks if the STUN server has requested client to use a shared secret.
     * @return  ETrue if STUN server requires the use of a shared secret
     *          EFalse otherwise
     */
    virtual TBool IsSharedSecretRequired() const = 0;

    /**
     * Decodes a byte stream and return the decoded UNSAF message.
     * @param   aData Byte stream to decode
     * @return  Decoded message, ownership is transferred.
     *          NULL if the stream didn't contain an UNSAF message.
     */
    virtual CNATFWUNSAFMessage* DecodeMessageL( const TDesC8& aData ) const = 0;
    
    /**
     * Used binding
     *
     * @since   s60 v3.2
     * @return  void
     */
    virtual const CBinding& Binding() = 0;
    
    /**
     * Cancels retransmission process after transaction expires.
     *
     * @since   s60 v3.2
     * @return  void
     */
    virtual void CancelRetransmission();


private:

     void CalculateNewRTO( TInt64& aRTTSample );
     
protected: // Data
    
    /**
     * Associated STUN client instance
     * Not own.
     */
    MSTUNBindingObserver* iClient;

    /**
     * Associated binding interface
     */
    CBinding& iBinding;

    /**
     * The user of NAT Traversal FW provides the UDP RSocket to be used in
     * Binding Requests and responses. It is passed within CSTUNBinding::NewL
     * Each CSTUNBinding instance uses a separate RSocket.
     *
     * Not own. Must be NULLed in derived classes destructor.
     */
    RSocket* iSocket;
    
    /**
     * Used natfw stream id
     */
    TUint iStreamId;
    
    /**
     * Used natfw connection id
     */
    TUint iConnectionId;
    
    /**
     * Used for transaction by unsaf protocols
     * Not own.
     */
    MNcmConnectionMultiplexer* iMux;
    
    /**
     * STUN server's address. If the server does not respond, binding tries
     * to obtain another address from the STUN client.
     */
    TInetAddr iServerAddress;

    /**
     *The public server reflexive address, obtained from STUN server
     */
    TInetAddr iPublicAddr;
    
    /**
     *The public stun server relay address
     */
    TInetAddr iRelayAddr;

    /**
     * Controls whether to insert XOR-ONLY into Binding Request
     */
    TBool iAddXorOnly;

    /**
     * If binding couldn't be obtained because transaction ended with an
     * Error, the reason is stored here.
     */
    TInt iTransactionError;

    /**
     * Amount of successive error (4xx-6xx) responses. Binding may retry to
     * send the request upon receiving a certain error response, but only
     * does so for a limited amount of responses. This counter is used to
     * prevent infinite loops, when e.g. server always responds with a 5xx.
     */
    TInt iErrorResponseCount;

    /**
     * Binding Request
     * owned.
     */
    CNATFWUNSAFMessage* iRequest;
    
    /**
     * Shared secret, owned. Can't be a reference to the password stored in
     * CSTUNSharedSecret as CBindingImplementation wouldn't know if it
     * expired and STUN client has created a different CSTUNSharedSecret.
     * Can be an empty descriptor.
     * Own.
     */
    HBufC8* iSharedSecret;

    /**
     * UNKNOWN-ATTRIBUTES received from transaction. '
     * Owned.
     */
    CNATFWUNSAFUnknownAttributesAttribute* iUnknownAttr;

    /**
     * instance of CSTUNTransaction
     * Owned 
     */
    CSTUNTransaction* iTransaction;

    /**
     * Current state
     * Not own.
     */
    CSTUNBindingState* iState;
    
    /**
     * ICE specific attributes
     */
    TICEAttributes iICEAttributes;
    
    /**
     * STUN binding states
     * Own.
     */
    CSTUNBindingInit* iInit;
    CSTUNBindingGetSharedSecret* iGetSharedSecret;
    CSTUNBindingGetAddress* iGetAddress;
    CSTUNBindingActive* iActive;
    CSTUNBindingWaitToRetry* iWaitToRetry;
    
    /**
     * Retransmit interval
     */
    TInt iRetransmitInterval;

    /**
     * Realm value in error response
     * Own.
     */
    HBufC8* iRealmFromResponse;
    
    /**
     * Nonce value from error response
     * Own.
     */
    HBufC8* iNonce; 
    
    /**
     * Realm value shuold be used from response
     */
    TBool iUseRealmFromResponse;
    
    /**
     * ETrue if fingerprint attributes will be added to requests.
     */
    TBool iAddFingerprint;
    
    /**
     * Indication transmitter
     * Own.
     */
    CStunIndicationTransmitter* iIndicationTx;
    
    
private:
    
    /**
     * Indicates if we have calculated the first RTO value
     */
    TBool iFirstRTOCalculated;
    
    /**
     * Smoothed round-trip time
     */
    TInt64 iSRTT;
    
    /**
     * Round-trip time variation
     */
    TInt64 iRTTVAR;
    
    /**
     * Retransmission timeout
     */
    TInt64 iRTO;
    
    /**
     * Dummy IP address
     */
    TInetAddr iDummyIP;
    };

#endif // CBINDINGIMPLEMENTATION_H

// End of File
