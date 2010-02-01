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




#ifndef CNATFWUNSAFMEDIACONNSENDER_H
#define CNATFWUNSAFMEDIACONNSENDER_H

#include <e32base.h>
#include <in_sock.h>

class CNATFWUNSAFEncodedItem;
class CNATFWUNSAFMessage;
class MNcmConnectionMultiplexer;
class MNcmSenderObserver;

/**
* A class for sending UNSAF messages to network using connection multiplexer.
*
*  @lib natfwunsafprotocols.lib
*  @since S60 3.2
*/
class CNATFWUNSAFMediaConnSender : public CBase
    {
public:    // Constructors and destructor

    /**
    * Creates a new instance of CNATFWUNSAFMediaConnSender
    * @param aMediaConnection Media connection used for sending
    * @return a new instance of CNATFWUNSAFMediaConnSender
    */
    IMPORT_C static CNATFWUNSAFMediaConnSender* NewL(
        MNcmConnectionMultiplexer& aMediaConnection );

    /**
    * Creates a new instance of CNATFWUNSAFMediaConnSender and
    * pushes in to CleanupStack.
    * @param aMediaConnection Media connection used for sending
    * @return a new instance of CNATFWUNSAFMediaConnSender
    */
    IMPORT_C static CNATFWUNSAFMediaConnSender* NewLC(
        MNcmConnectionMultiplexer& aMediaConnection );

    /**
    * Destructor.
    */
    IMPORT_C virtual ~CNATFWUNSAFMediaConnSender();

public: // New functions

    /**
    * Sends an UNSAF message to the network.
    * Only one send operation can be outstanding at a time.
    * @param aStreamId Id of the stream to be used for sending
    * @param aSubstreamId Id of the substream to be used for sending
    * @param aMessage message to be sent
    * @param aSharedSecret a shared secret to be used to calculate
    *        a hash over the encoded UNSAF message.
    * @param aObserver Observer for receiving sending related callbacks
    * @param aUseFingerprint If user requires to use a fingerprint
    *        it must set this to a positive integer.
    */
    IMPORT_C void SendL( const TUint aStreamId, 
                    const TUint aSubstreamId,
                    const CNATFWUNSAFMessage& aMessage,
                    const TDesC8& aSharedSecret,
                    MNcmSenderObserver* aObserver = NULL,
                    const TUint aUseFingerprint = 0 );

    /**
    * Overloaded SendL for sending to a specified destination.
    * @param aStreamId Id of the stream to be used for sending
    * @param aSubstreamId Id of the substream to be used for sending
    * @param aMessage message to be sent
    * @param aSharedSecret a shared secret to be used to calculate
    *        a hash over the encoded UNSAF message.
    * @param aDestinationAddress Destination address
    * @param aObserver Observer for receiving sending related callbacks
    * @param aUseFingerprint If user requires to use a fingerprint
    *        it must set this to a positive integer.
    */
    IMPORT_C void SendL( const TUint aStreamId, 
                    const TUint aSubstreamId,
                    const CNATFWUNSAFMessage& aMessage,
                    const TDesC8& aSharedSecret,
                    const TInetAddr& aDestinationAddress,
                    MNcmSenderObserver* aObserver = NULL,
                    const TUint aUseFingerprint = 0 );

    /**
    * Cancels sending of an UNSAF message to the network.
    * @param aStreamId Id of the stream to be used for sending
    * @param aSubstreamId Id of the substream to be used for sending
    * @param aObserver Observer for receiving canceling related callbacks
    */
    IMPORT_C void Cancel( const TUint aStreamId,
                    const TUint aSubstreamId,
                    MNcmSenderObserver* aObserver = NULL ) const;

private: // Constructors

    CNATFWUNSAFMediaConnSender(
        MNcmConnectionMultiplexer& aMediaConnection );

private: // Private methods

    void EncodeL( const CNATFWUNSAFMessage& aMessage,
                  const TDesC8& aSharedSecret,
                  const TUint aStreamId,
                  const TUint aSubstreamId,
                  const TUint aUseFingerprint );

    void RemoveEncoded();

    void SendToMediaConnectionL(
        MNcmSenderObserver* aObserver,
        const TInetAddr& aDestinationAddress );


private: // Data

    MNcmConnectionMultiplexer& iMediaConnection;
    CNATFWUNSAFEncodedItem* iEncoded;

    TPtrC8 iCurrent;

    };

#endif // CNATFWUNSAFMEDIACONNSENDER_H


