/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef C_CSTUNBINDINGIMPLEMENTATION_H
#define C_CSTUNBINDINGIMPLEMENTATION_H

// INCLUDES
#include <e32base.h>
#include <es_sock.h>
#include <in_sock.h>
#include "mstuntransactionobserver.h"
#include "cstuntimeruser.h"
#include "natfwbindingimplementationbase.h"
#include "stunturnclientlogs.h"

// FORWARD DECLARATIONS
class MSTUNBindingObserver;
class MNcmConnectionMultiplexer;

// CLASS DECLARATION
/**
* Hides the implementation of STUN binding for the user of STUN client.
* @lib  natfwstunclient.dll
*/
class CSTUNBindingImplementation : public CBindingImplementation
    {
    friend class ut_cstunbindingimplementation;
    
    public:  // Constructors and destructor

        /**
        * Creates a new instance of CSTUNBindingImplementation.
        * Associates the local socket with the binding.
        * The public IP address is available when
        * MSTUNClientObserver::STUNBindingEventOccuredL gets called.
        *
        * @since    s60 3.2
        * @pre      aClient.IsInitialized()
        * @param    aBinding Associated binding interface
        * @param    aClient An initialised STUN client
        * @param    aSocket An opened socket that has the local port set.
        * @param    aStreamId stream id
        * @param    aSubstreamId substream id
        * @return a new instance, the ownership is transferred.
        */
        static CSTUNBindingImplementation* NewL( CBinding& aBinding,
                                                 MSTUNBindingObserver& aClient,
                                                 RSocket& aSocket );
        
        /**
         * Creates a new instance of CSTUNBindingImplementation.
         * Associates the local socket with the binding.
         * The public IP address is available when
         * MSTUNClientObserver::STUNBindingEventOccuredL gets called.
         *
         * @since    s60 3.2
         * @pre      aClient.IsInitialized()
         * @param    aBinding Associated binding interface
         * @param    aClient An initialised STUN client
         * @param    aStreamId stream id
         * @param    aSubstreamId substream id
         * @return a new instance, the ownership is transferred.
         */
        static CSTUNBindingImplementation* NewL( CBinding& aBinding,
                                         MSTUNBindingObserver& aClient,
                                         TUint aStreamId,
                                         TUint aSubstreamId,
                                         MNcmConnectionMultiplexer* aMux );
        /**
        * Destructor.
        */
        ~CSTUNBindingImplementation();

    public: // From CSTUNTimerUser

        void TimerExpiredL();

        void LeaveFromTimerExpired( TInt aError );

    public: // From MSTUNTransactionObserver

        void PublicAddressObtainedL( const TInetAddr& aAddress );

        void PublicAddressObtainedL( const TInetAddr& aReflexiveAddr,
                                     const TInetAddr& aRelayAddr );

        void TransactionError( TInt aError,
                               CNATFWUNSAFUnknownAttributesAttribute* aUnknownAttr );

        void TransactionEventOccurredL( TSTUNCallbackInfo::TFunction aEvent );
        
    public: // New functions

        /**
         * Refreshes the binding causing a new Binding Request to be sent
         * to the STUN server. The public IP address is available when 
         * MSTUNClientObserver gets called.
         *
         * @since    s60 3.2
         * @return   void
         */
        void SendRequestL();
        
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
        void SendRequestL( const TInetAddr& aDestAddr,
                           TBool aAddFingerprint,
                           TUint aRtoValue );
                
        /**
         * Cancels a previously issued call to SendRequestL.
         *
         * @since    s60 3.2
         * @return   void
         */
        void CancelRequest();

        /**
         * Gets the socket used for obtaining the public address.
         *
         * @since    s60 3.2
         * @return socket
         */
        const RSocket& Socket() const;    
    
        /**
         * Checks whether the public address has a value set
         *
         * @since    s60 3.2
         * @return ETrue if the public address has been obtained, 
         *         otherwise EFalse. 
         */
        TBool AddressResolved() const;    
    
        /**
         * Gets the public address for this binding as seen in public network.
         *
         * @since    s60 3.2
         * @return   public address
         */
        const TInetAddr& PublicAddr() const;
        
        /**
         * Returns alternate server transport address.
         *
         * @since   s60 3.2
         * @return alternate server address
         */
        const TInetAddr& AlternateServerAddr() const;
        
        /**
         * Returns realm value from error response.
         *
         * @since   s60 3.2
         * @return realm from error response         
         */
        const HBufC8* RealmFromResponse() const;        
        
        /**
         * The client should call this function when it has received data
         * from the socket shared with this binding and 
         * the client cannot itself handle the data.
         *
         * @since    s60 3.2
         * @param    aData data received
         * @param    aConsumed on return ETrue if the data was accepted,
         *           otherwise EFalse.
         * @param    aRemoteAddr KAFUnspec
         * @return   NULL.
         */
        HBufC8* HandleDataL( 
            const TDesC8& aData, TBool& aConsumed, TInetAddr& aRemoteAddr );


        /**
         * Returns the associated CSTUNClient instance.
         *
         * @since    s60 3.2
         * @return CSTUNClient, or NULL if the CSTUNClient has been deleted        
         */
        const CSTUNClient* STUNClient() const;

        /**
         * Get a STUN server address from STUN client
         *
         * @since    s60 3.2
         * @return   void
         */
        void GetServerAddressL();
        
        /**
         * Get a fresh a shared secret.
         *
         * @since    s60 3.2
         * @return   void  
         */
        void GetSharedSecretL();

        /**
         * Clears the pointer to CSTUNClient.
         *
         * @since    s60 3.2
         * @return   void
         * @post     iClient == NULL        
         */
        void DetachClient();

        /**
         * Store the obtained public address and inform application.
         *
         * @since    s60 3.2
         * @param    aPublicAddress Public address
         * @return   void
         */        
        void StoreAddressL( const TInetAddr& aPublicAddress );

        /**
         * Change the object's state.
         *
         * @since    s60 3.2
         * @param    aNewState State to enter.
         * @return   void
         */
        void ChangeState( CSTUNBindingState& aNewState );
        
        /**
         * Binding encountered an error and enters terminated state. The function
         * MSTUNClientObserver::BindingErrorL is used to inform application.
         *
         * @since    s60 3.2
         * @pre      aError != KErrNone
         * @param    aError Error code
         * @return   void
         */
        void Terminate( TInt aError );        
        
        /**
         * Checks if this binding is waiting for shared secret.
         *
         * @since    s60 3.2
         * @return ETrue BInding is waiting shared secret, EFalse otherwise
         */
        TBool IsWaitingSharedSecret() const;

        /**
         * STUN client uses this function to give the username and password, that
         * the binding requested earlier with function
         * MSTUNBindingObserver::ObtainSharedSecretL.
         *
         * @since    s60 3.2
         * @pre      aUsername and aPassword must either both have a descriptor that
         *           is not empty, or then both must have an empty descritor.
         * @param    aUsername Username or an empty descriptor if the shared secret
         *                    is not used.
         * @param    aPassword Password or an empty descriptor if the shared secret
         *                    is not used.
         * @return   void
         */
        void SharedSecretObtainedL( const TDesC8& aUsername,
                                    const TDesC8& aPassword );

        /**
         * Creates or updates a Binding Request message, using the given username
         * and password.
         *
         * @since    s60 3.2
         * @param    aUsername Username
         * @param    aPassword Password
         * @return   void
         */
        void CreateBindingRequestL( const TDesC8& aUsername,
                                    const TDesC8& aPassword );

        /**
         * Creates a transaction for sending Binding Request message
         *
         * @since    s60 3.2
         * @pre      iRequest != NULL
         * @return   void   
         */                          
        void SendBindingRequestL();
        
        /**
         * Take actions based on the reason why transaction failed. For certain
         * errors, it is possible to send the Binding Request again.
         * 
         * @since    s60 3.2
         * @pre      aError != KErrNone
         * @param    aError Error reason
         * @return   ETrue Request can be retried
         *           EFalse Binding enters Terminated-state
         */
        TBool HandleTransactionError( TInt aError );

        /**
         * Forward the ICMP error to transaction for processing.
         *
         * @since    s60 3.2
         * @param    aAddress Address where the ICMP error occurred
         * @return   void      
         */
        void IcmpError( const TInetAddr& aAddress );

        /**
         * Wait before retring to send a Binding Request
         *
         * @since    s60 3.2
         * @return   void      
         */
        void WaitBeforeRetrying();
        
        /**
         * ICE specific cancel for stopping ongoing message retranstmition.
         * Response is waited as long as sending timer expires. After that
         * MSTUNClientObserver::STUNBindingErrorOccurred method will be called.
         *
         * @since   s60 3.2
         * @return  void
         */
        void CancelRetransmission();
       
        /**
         * Method for sending binding indication. Indications are not
         * are not retransmitted and responses are not expected.
         *
         * @since   s60 3.2
         * @param   aRemoteAddr     address to send to (remote agent)
         * @param   aData           data to sent in indication
         * @param   aAddFingerprint add fingerprint attribute if true
         * @return  void
         */
        void SendIndicationL( const TInetAddr& aRemoteAddr,
            const TDesC8& aData, TBool aAddFingerprint );
            
        
        // From CBindingImplementationBase

        void SetICESpecificAttributes( const TICEAttributes& aAttributes );
        
        TUint StreamId() const;
        
        TUint ConnectionId() const;

        const CBinding& Binding();

    private: // Constructors, for internal use

        CSTUNBindingImplementation( CBinding& aBinding,        
                                    MSTUNBindingObserver& aClient,
                                    RSocket& aSocket );
        
        CSTUNBindingImplementation( CBinding& aBinding,
                                    MSTUNBindingObserver& aStunClient, 
                                    TUint aStreamId,
                                    TUint aSubstreamId,
                                    MNcmConnectionMultiplexer* aMux );

        void ConstructL();
        CSTUNBindingImplementation();

    private: // New functions, for internal use

        /**
         * Release the memory of the Binding Request related data. This data is
         * only needed for sending a Binding Request and receiving a response.
         *
         * @since    s60 3.2
         * @return   void
         */
        void FreeRequestData();

        /**
         * Return STUN client handle.
         *
         * @since   s60 3.2
         * @pre iClient != NULL
         * @return STUN client
         * @leave KErrNotFound If the STUN client has been deleted
         */
        MSTUNBindingObserver& ClientL() const;

        /**
         * Return the current username, or an empty descriptor if username
         * does not exist.
         *
         * @since   s60 3.2
         * @return  Username
         */
        const TDesC8& Username() const;

        /**
         * Checks if the STUN server has requested client to use a shared secret.
         *
         * @since   s60 3.2
         * @return ETrue if STUN server requires the use of a shared secret
         *          EFalse otherwise
         */
        TBool IsSharedSecretRequired() const;

        /**
         * Decodes a byte stream and return the decoded UNSAF message.
         *
         * @since   s60 3.2
         * @param   aData Byte stream to decode
         * @return  Decoded message, ownership is transferred.
         *          NULL if the stream didn't contain an UNSAF message.
         */
        CNATFWUNSAFMessage* DecodeMessageL( const TDesC8& aData ) const;
        
        /**
         * Validate message data.
         *
         * @since   s60 3.2
         * @param   aMsg pointer to unsaf message
         * @return  ETrue if valid message type, else EFalse
         */
        TBool ValidateMsgType( CNATFWUNSAFMessage* aMsg ) const;

    private: // Data
        
        TInetAddr iDestAddr;
        
    private: // For testing purposes

#ifdef TEST_EUNIT
        friend class CSTUNClientTest;
        friend class CSTUNClientImplementationTest;
        friend class CSTUNBindingTest;
        friend class CSTUNBindingImplementationTest;
#endif
    };

#endif // CSTUNBINDINGIMPLEMENTATION_H

// End of File
