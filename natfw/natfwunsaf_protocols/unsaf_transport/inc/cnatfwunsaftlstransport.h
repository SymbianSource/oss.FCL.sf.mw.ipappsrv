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





#ifndef CNATFWUNSAFTLSTRANSPORT_H
#define CNATFWUNSAFTLSTRANSPORT_H

//  INCLUDES
#include <e32base.h>
#include <es_sock.h>
#include <in_sock.h>
#include <securesocket.h>

#include "mnatfwunsaftlstransportstateowner.h"

// FORWARD DECLARATIONS
class MNATFWUNSAFTlsTransportObserver;
class CNATFWUNSAFMessage;
class CNATFWUNSAFTlsSender;
class CNATFWUNSAFTlsReceiver;


// CLASS DECLARATION
/**
* A class for creating a TLS connection and sending UNSAF messages using TLS.
*
*  @lib natfwunsafprotocols.lib
*/
class CNATFWUNSAFTlsTransport : public CActive,
        public MNATFWUNSAFTlsTransportStateOwner
    {
    public:    // Constructors and destructor

        /**
        * Creates a new instance of CNATFWUNSAFTlsTransport.
        * Connects to the remote party using TCP and creates a TLS connection.
        * When the TLS connection has been established,
        * notification is received via MNATFWUNSAFTlsTransportObserver.
        * @param aSocketServer a connected socket server session
        * @param aConnection an active connection to be used
        * @param aAddr remote address of the TLS connection
        * @param aObserver an observer to receive callbacks
        *        about sending, receiving and error events
        * @return a new instance of CNATFWUNSAFTlsTransport
        */
        IMPORT_C static CNATFWUNSAFTlsTransport* NewL(
                    RSocketServ& aSocketServer,
                    RConnection& aConnection,
                    const TInetAddr& aAddr,
                    MNATFWUNSAFTlsTransportObserver& aObserver );

        /**
        * Creates a new instance of CNATFWUNSAFTlsTransport and
        * pushes in to CleanupStack.
        * Connects to the remote party using TCP and creates a TLS connection.
        * When the TLS connection has been established,
        * notification is received via MNATFWUNSAFTlsTransportObserver.
        * @param aSocketServer a connected socket server session
        * @param aConnection an active connection to be used
        * @param aAddr remote address of the TLS connection
        * @param aObserver an observer to receive callbacks
        *        about sending, receiving and error events
        * @return a new instance of CNATFWUNSAFTlsTransport
        */
        IMPORT_C static CNATFWUNSAFTlsTransport* NewLC(
                    RSocketServ& aSocketServer,
                    RConnection& aConnection,
                    const TInetAddr& aAddr,
                    MNATFWUNSAFTlsTransportObserver& aObserver );

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CNATFWUNSAFTlsTransport();

    public: // New functions

        /**
        * Checks whether the TLS connection is ready for sending messages.
        * @return ETrue if ready, EFalse otherwise.
        */
        IMPORT_C TBool IsConnected();

        /**
        * Sends an UNSAF message to the remote party.
        * Only one send operation can be outstanding at a time.
        * A send operation can be cancelled by deleting
        * this instance of CNATFWUNSAFTlsTransport.
        * @param aMessage message to be sent
        * @param aCredentials credentials for long term shared secret
        * @param aUseFingerprint flag for using fingerprint on message.
        */
        IMPORT_C void SendL( const CNATFWUNSAFMessage& aMessage,
                             const TDesC8& aCredentials,
                             const TUint aUseFingerprint = 0 );

        /**
        * By calling this method the client continues to receive
        * UNSAF messages over TLS even though the previous message was
        * completed
        */
        IMPORT_C void ContinueListeningL() const;

    public: // From MNATFWUNSAFTlsTransportStateOwner

        void EnterStateL( TNATFWUNSAFTlsTransportStateBase::TState aState );

        void ConnectTcpL();

        void ConnectTlsL();

        void TlsConnectedL();

        void SendOverTlsL( const CNATFWUNSAFMessage& aMessage,
                           const TDesC8& aCredentials,
                           const TUint aUsefingerprint = EFalse );


    private: // From CActive

        void DoCancel();

        void RunL();

        TInt RunError( TInt aError );

    private: // Constructors

        void ConstructL();

        CNATFWUNSAFTlsTransport( RSocketServ& aSocketServer,
                            RConnection& aConnection,
                            const TInetAddr& aAddr,
                            MNATFWUNSAFTlsTransportObserver& aObserver );

    private:

        void CheckCipherSuiteAvailabilityL();

        void InitializeStatesL();

        TNATFWUNSAFTlsTransportStateBase& CurrentState();

    private: // Data

        RSocketServ& iSocketServer;
        RConnection& iConnection;
        MNATFWUNSAFTlsTransportObserver& iObserver;
        CArrayVarFlat< TNATFWUNSAFTlsTransportStateBase > iStates;
        TNATFWUNSAFTlsTransportStateBase::TState iStateValue;

        TInetAddr iAddr;
        RSocket iSocket;

        CSecureSocket* iSecureSocket;
        CNATFWUNSAFTlsSender* iSender;
        CNATFWUNSAFTlsReceiver* iReceiver;


#ifdef TEST_EUNIT
        friend class UT_CNATFWUNSAFTlsTransport;
#endif
    };

#endif // CNATFWUNSAFTLSTRANSPORT_H


