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





#ifndef CNATFWUNSAFUDPSENDER_H
#define CNATFWUNSAFUDPSENDER_H

//  INCLUDES
#include <e32base.h>
#include <es_sock.h>
#include <in_sock.h>

// FORWARD DECLARATIONS
class MNATFWUNSAFUdpSenderObserver;
class CNATFWUNSAFEncodedItem;
class CNATFWUNSAFMessage;


// CLASS DECLARATION
/**
* A class for sending UNSAF messages to network using UDP socket.
*
*  @lib natfwunsafprotocols.lib
*/
class CNATFWUNSAFUdpSender : public CActive
    {
    public:    // Constructors and destructor

        /**
        * Creates a new instance of CNATFWUNSAFUdpSender
        * @param aSocket an opened socket handle used for sending
        * @param aObserver an observer to receive callbacks
        *        about sending status
        * @return a new instance of CNATFWUNSAFUdpSender
        */
        IMPORT_C static CNATFWUNSAFUdpSender* NewL( RSocket& aSocket,
                    MNATFWUNSAFUdpSenderObserver& aObserver );

        /**
        * Creates a new instance of CNATFWUNSAFUdpSender and
        * pushes in to CleanupStack.
        * @param aSocket an opened socket handle used for sending
        * @param aObserver an observer to receive callbacks
        *        about sending status
        * @return a new instance of CNATFWUNSAFUdpSender
        */
        IMPORT_C static CNATFWUNSAFUdpSender* NewLC( RSocket& aSocket,
                     MNATFWUNSAFUdpSenderObserver& aObserver );

        /**
        * Creates a new instance of CNATFWUNSAFUdpSender
        * @param    aStreamId natfw stream id
        * @param    aConnectionId   Connection id related to stream id
        * @param    aObserver an observer to receive callbacks
        *           about sending status
        * @return   a new instance of CNATFWUNSAFUdpSender
        */
        IMPORT_C static CNATFWUNSAFUdpSender* NewL( TUint aStreamId,
            TUint aConnectionId, MNATFWUNSAFUdpSenderObserver& aObserver );

        /**
        * Creates a new instance of CNATFWUNSAFUdpSender and
        * pushes in to CleanupStack.
        * @param    aStreamId natfw stream id
        * @param    aConnectionId   Connection id related to stream id
        * @param aObserver an observer to receive callbacks
        *        about sending status
        * @return a new instance of CNATFWUNSAFUdpSender
        */
        IMPORT_C static CNATFWUNSAFUdpSender* NewLC( TUint aStreamId,
            TUint aConnectionId, MNATFWUNSAFUdpSenderObserver& aObserver );

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CNATFWUNSAFUdpSender();

    public: // New functions

        /**
        * Sends an UNSAF message to the network.
        * Only one send operation can be outstanding at a time.
        * A send operation can be cancelled by deleting
        * this instance of CNATFWUNSAFUdpSender.
        * @param aAddr a remote address for the message
        * @param aMessage message to be sent
        * @param aSharedSecret a shared secret to be used to calculate
        *        a hash over the encoded UNSAF message.
        */
        IMPORT_C void SendL( const TInetAddr& aAddr,
                        const CNATFWUNSAFMessage& aMessage,
                        const TDesC8& aSharedSecret );

    private: // From CActive

        void DoCancel();

        void RunL();

        TInt RunError( TInt aError );

    private: // Constructors

        void ConstructL() const;

        CNATFWUNSAFUdpSender( RSocket& aSocket,
            MNATFWUNSAFUdpSenderObserver& aObserver );
        
        CNATFWUNSAFUdpSender( TUint aStreamId, TUint aConnectionId,
            MNATFWUNSAFUdpSenderObserver& aObserver );

    private: // Private methods

        void EncodeL( const CNATFWUNSAFMessage& aMessage,
                      const TDesC8& aSharedSecret,
                      const TInetAddr& aAddr );

        void RemoveEncoded();

        void SendToSocketL();


    private: // Data
        
        // Stream & connection IDs needed by multiplexer
        TUint iStreamId;
        TUint iConnectionId;
        
        RSocket& iSocket;
        MNATFWUNSAFUdpSenderObserver& iObserver;

        CNATFWUNSAFEncodedItem* iEncoded;

        TPtrC8 iCurrent;


#ifdef TEST_EUNIT
        friend class UT_CNATFWUNSAFUdpSender;
#endif

    };

#endif // CNATFWUNSAFUDPSENDER_H


