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





#ifndef CNATFWUNSAFTLSSENDER_H
#define CNATFWUNSAFTLSSENDER_H

//  INCLUDES
#include <e32base.h>
#include <securesocket.h>

// FORWARD DECLARATIONS
class MNATFWUNSAFTlsTransportObserver;
class CNATFWUNSAFEncodedItem;
class CNATFWUNSAFMessage;


// CLASS DECLARATION
/**
* A class for sending UNSAF messages using TLS.
*
*  @lib natfwunsafprotocols.lib
*/
class CNATFWUNSAFTlsSender : public CActive
    {
    public:    // Constructors and destructor

        /**
        * Creates a new instance of CNATFWUNSAFTlsSender.
        * @param aSecureSocket a connected secure socket
        *        for which the handshake has been completed.
        * @param aObserver a callbacks for sending errors
        * @return a new instance of CNATFWUNSAFTlsSender
        */
        static CNATFWUNSAFTlsSender* NewL(
            CSecureSocket& aSecureSocket,
            MNATFWUNSAFTlsTransportObserver& aObserver );

        /**
        * Creates a new instance of CNATFWUNSAFTlsSender and
        * pushes in to CleanupStack.
        * @param aSecureSocket a connected secure socket
        *        for which the handshake has been completed.
        * @param aObserver a callbacks for sending errors
        * @return a new instance of CNATFWUNSAFTlsSender
        */
        static CNATFWUNSAFTlsSender* NewLC(
            CSecureSocket& aSecureSocket,
            MNATFWUNSAFTlsTransportObserver& aObserver );

        /**
        * Destructor.
        */
        ~CNATFWUNSAFTlsSender();

    public: // New functions

        /**
        * Sends an UNSAF message to the remote party.
        * Only one send operation can be outstanding at a time.
        * A send operation can be cancelled by deleting
        * this instance of CNATFWUNSAFTlsSender.
        * @param aMessage message to be sent
        * @param aCredentials credentials for long term shared secret
        * @param aUseFingerprint if user wishes to use a fingerprint, it should
        *        set this to a positive integer
        */
        void SendL( const CNATFWUNSAFMessage& aMessage,
                    const TDesC8& aCredentials,
                    const TUint aUseFingerprint = 0 );

    private: // From CActive

        void DoCancel();

        void RunL();

        TInt RunError( TInt aError );

    private: // Constructors

        CNATFWUNSAFTlsSender( CSecureSocket& aSecureSocket,
                         MNATFWUNSAFTlsTransportObserver& aObserver );


    private: // Private functions

        void EncodeL( const CNATFWUNSAFMessage& aMessage,
                      const TDesC8& aCredentials,
                      const TUint aUseFingerprint);

        void RemoveEncoded();

        void SendToSocketL();


    private: // Data

        CSecureSocket& iSecureSocket;

        MNATFWUNSAFTlsTransportObserver& iObserver;

        CNATFWUNSAFEncodedItem* iEncoded;

        TPtrC8 iCurrent;


#ifdef TEST_EUNIT
        friend class UT_CNATFWUNSAFTlsSender;
#endif
    };

#endif // CNATFWUNSAFTLSSENDER_H


