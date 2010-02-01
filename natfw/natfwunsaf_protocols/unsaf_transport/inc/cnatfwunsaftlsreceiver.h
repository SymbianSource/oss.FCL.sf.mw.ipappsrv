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





#ifndef CNATFWUNSAFTLSRECEIVER_H
#define CNATFWUNSAFTLSRECEIVER_H

//  INCLUDES
#include <e32base.h>
#include <securesocket.h>
#include <es_sock.h>

#include "mnatfwunsafmsgassemblerobserver.h"

// FORWARD DECLARATIONS
class MNATFWUNSAFTlsTransportObserver;
class CNATFWUNSAFMsgAssembler;


// CLASS DECLARATION
/**
* A class for receiving UNSAF messages using TLS.
*
*  @lib natfwunsafprotocols.lib
*/
class CNATFWUNSAFTlsReceiver : public CActive,
        public MNATFWUNSAFMsgAssemblerObserver
    {
    public:    // Constructors and destructor

        /**
        * Creates a new instance of CNATFWUNSAFTlsReceiver.
        * Starts receiving from the secure socket.
        * @param aSecureSocket a connected secure socket
        *        for which the handshake has been completed.
        * @param aObserver a callback for receiving UNSAF messages
        *        and error notifications
        * @return a new instance of CNATFWUNSAFTlsReceiver
        */
        static CNATFWUNSAFTlsReceiver* NewL(
            CSecureSocket& aSecureSocket,
            MNATFWUNSAFTlsTransportObserver& aObserver );

        /**
        * Creates a new instance of CNATFWUNSAFTlsReceiver and
        * pushes in to CleanupStack.
        * Starts receiving from the secure socket.
        * @param aSecureSocket a connected secure socket
        *        for which the handshake has been completed.
        * @param aObserver a callback for receiving UNSAF messages
        *        and error notifications
        * @return a new instance of CNATFWUNSAFTlsReceiver
        */
        static CNATFWUNSAFTlsReceiver* NewLC(
            CSecureSocket& aSecureSocket,
            MNATFWUNSAFTlsTransportObserver& aObserver );

        /**
        * By calling this method, the receiver continues to receive
        * UNSAF messages using TLS even though the previous message was
        * completed
        */
        void ContinueRecvL();

        /**
        * Destructor.
        */
        ~CNATFWUNSAFTlsReceiver();

    public: // From MNATFWUNSAFMsgAssemblerObserver

        void NATFWUNSAFMsgCompleteL(
            CNATFWUNSAFMessage* aMessage, TInt aStatus );


    private: // From CActive

        void DoCancel();

        void RunL();

        TInt RunError( TInt aError );

    private: // Constructors

        void ConstructL();

        CNATFWUNSAFTlsReceiver( CSecureSocket& aSecureSocket,
                           MNATFWUNSAFTlsTransportObserver& aObserver );

    private: // New functions

        void AssembleL();

        void ReceiveL( TUint aReceiveBufLength );

        void CreateReceiveBufferL( TUint aReceiveBufLength );

    private: // Data

        CSecureSocket& iSecureSocket;

        MNATFWUNSAFTlsTransportObserver& iObserver;

        CNATFWUNSAFMsgAssembler* iMsgAssembler;

        HBufC8* iReceiveBuffer;

        TPtr8 iReceiveBufferPtr;

        TUint iNextLength;

        TSockXfrLength iSockLength;


#ifdef TEST_EUNIT
        friend class UT_CNATFWUNSAFTlsReceiver;
#endif

    };

#endif // CNATFWUNSAFTLSRECEIVER_H


