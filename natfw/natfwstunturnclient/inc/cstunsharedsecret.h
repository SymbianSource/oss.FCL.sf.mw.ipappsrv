/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef C_CSTUNSHAREDSECRET_H
#define C_CSTUNSHAREDSECRET_H

// INCLUDES
#include <e32base.h>
#include "mnatfwunsaftlstransportobserver.h"
#include "cstuntimeruser.h"

// FORWARD DECLARATIONS
class RSocketServ;
class RConnection;
class TInetAddr;
class CNATFWUNSAFTlsTransport;
class CNATFWUNSAFSharedSecretRequest;
class CNATFWUNSAFAttribute;
class MSharedSecretObserver;
class CSTUNSharedSecretState;
class CSTUNSharedSecretConnecting;
class CSTUNSharedSecretWaitResponse;
class CSTUNSharedSecretWaitToRetry;
class CSTUNSharedSecretActive;
class CTransactionIDGenerator;

// CLASS DECLARATION

/**
 *  This class handles the obtaining of a shared secret.
 *
 *  @lib ?library
 *  @since S60 ?S60_version *** for example, S60 v3.0
 */
class CSTUNSharedSecret :    
    public CSTUNTimerUser,
    public MNATFWUNSAFTlsTransportObserver
    {
    	
    friend class ut_cstunsharedsecret;
    	
    public: // Constructors and destructor

        /**
        * Creates a new instance of CSTUNSharedSecret, and initiates the
        * procedures needed to obtain a shared secret.        
        * @param aSocketServer a connected socket server session        
        * @param aConnection an active connection to be used
        * @param aTimer Timer services
        * @param aAddr remote address of the TLS connection
        * @param aTransactionIDGenerator Provides unique TransactionIDs
        * @param aObserver An observer to receive callbacks
        * @param aLtUsername long term username
        * @param aLtPassword long term password
        * @return A new instance of CSTUNSharedSecret, ownership is transferred.
        */
        static CSTUNSharedSecret*
            NewL( RSocketServ& aSocketServer,
                  RConnection& aConnection,
                  CDeltaTimer& aTimer,
                  const TInetAddr& aAddr,
                  CTransactionIDGenerator& aTransactionIDGenerator,
                  MSharedSecretObserver& aObserver,
                  const TDesC8& aLtUsername,
                  const TDesC8& aLtPassword );

        ~CSTUNSharedSecret();

    public: // From CSTUNTimerUser

        void TimerExpiredL();

        void LeaveFromTimerExpired( TInt aError );
    
    public: // From MUNSAFTlsTransportObserver
    
        void TlsConnectedL();

        void IncomingMessageL( CNATFWUNSAFMessage* aMessage );

        void ErrorOccured( TInt aError );

    public: // New functions

        /**
        * Get the username.
        * @return Username, or an empty descriptor if username has not been
        *          obtained.
        */
        const TDesC8& Username() const;

        /**
        * Get the password.        
        * @return Password, or an empty descriptor if password has not been
        *          obtained.
        */
        const TDesC8& Password() const;

        /**
        * Change the object's state.        
        * @param aNewState State to enter, can be NULL.
        *                    Ownership is not transferred.        
        */
        void ChangeState( const CSTUNSharedSecretState* aNewState );

        /**
        * Terminate the state machine as an error has occurred.
        * @pre aError != KErrNone
        * @param aError Reason for the failure.
        */
        void Terminate( TInt aError );

        void SendRequestL();
        
        /**
        * Check if the received UNSAF message is correct and should be
        * processed.
        * @param aMessage UNSAF message
        * @return ETrue message is valid, EFalse otherwise
        */
        TBool CheckMessage( const CNATFWUNSAFMessage& aMessage ) const;        

        /**
        * Process a Shared Secret Response.        
        * @pre aResponse.Type() == CUNSAFMessage::ESharedSecretResponse
        * @param aResponse Shared Secret Response        
        */
        void ResponseReceivedL( const CNATFWUNSAFMessage& aResponse );
        
        /**
        * Process a Shared Secret Error Response.        
        * @pre aResponse.Type() == CUNSAFMessage::ESharedSecretErrorResponse
        * @param aResponse Shared Secret Error Response
        * @return ETrue  Wait a while, then retry
        *          EFalse Terminate immediately
        */
        TBool ErrorResponseReceivedL( const CNATFWUNSAFMessage& aResponse );

    private: // Enumerations
        
        enum TTimerDuration
            {
            // How long implementation waits for a TLS connection to be formed.
            KTlsHandshakeTimeout = 8000,

            // How long implementation waits for a Shared Secret (Error)
            // Response, after having sent a Shared Secret Request.
            KSharedSecretRequestTimeout = 10000
            };
        
        TBool IsExpectedError( TInt aError );
        
        void SaveRealmAndNonceL( const CNATFWUNSAFMessage& aResponse );

    private: // Constructors
    
        CSTUNSharedSecret( CDeltaTimer& aTimer,
                           CTransactionIDGenerator& aTransactionIDGenerator,
                           MSharedSecretObserver& aObserver );

        CSTUNSharedSecret();

        CSTUNSharedSecret( const CSTUNSharedSecret& aSharedSecret );

        void ConstructL( RSocketServ& aSocketServer,
                         RConnection& aConnection,
                         const TInetAddr& aAddr,
                         const TDesC8& aLtUsername,
                         const TDesC8& aLtPassword );        

    private: // New functions, for internal use

        void StoreValueL( CNATFWUNSAFAttribute* aAttribute, HBufC8** aDest ) const;

    private: // Data
        
        /**
         * not own.
         */
        MSharedSecretObserver& iObserver;
        
        /**
         * not own.
         */
        CTransactionIDGenerator& iTransactionIDGenerator;

        /**
         * TLS transport
         * own.
         */
        CNATFWUNSAFTlsTransport* iTlsTransport;

        /**
         * Kept until response has been received.
         * own.
         */
        CNATFWUNSAFSharedSecretRequest* iRequest;

        /**
         * Username to use in the STUN Binding Request
         * own.
         */
        HBufC8* iUsername;
        
        /**
         * Password is the shared secret
         * own.
         */
        HBufC8* iPassword;
        
        /**
         * Current state
         * not own.
         */
        const CSTUNSharedSecretState* iState;
        
        /**
         * Username to use in shared secret request authentication
         * own.
         */
        HBufC8* iLtUsername;
        
        /**
         * Password to use in shared secret request authentication
         * own.
         */
        HBufC8* iLtPassword;
        
        /**
         * Nonce value from error response
         * own.
         */
        HBufC8* iNonce;
        
        /**
         * Realm value from error response
         * own.
         */
        HBufC8* iRealm;
        
        // Indicates that first error response has arrived (401)
        TBool iAddUsernameAttr;
        
        /**
         * Keeps count of errors
         * own.
         */
        RArray<TInt> iErrorBuffer;

        //Shared secret states, owned
        CSTUNSharedSecretConnecting*     iConnecting;
        CSTUNSharedSecretWaitResponse*    iWaitResponse;
        CSTUNSharedSecretWaitToRetry*    iWaitToRetry;
        CSTUNSharedSecretActive*         iActive;
        
    private: // For testing purposes

#ifdef TEST_EUNIT
        friend class CSTUNSharedSecretTest;
        friend class STUNTestUtils;
        friend class CSTUNClientTest;
        friend class CSTUNClientImplementationTest;
#endif
        __DECLARE_TEST;
    };

#endif // C_CSTUNSHAREDSECRET_H
