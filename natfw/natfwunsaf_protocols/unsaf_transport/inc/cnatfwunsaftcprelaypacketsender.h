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




#ifndef CNATFWUNSAFTCPRELAYPACKETSENDER_H
#define CNATFWUNSAFTCPRELAYPACKETSENDER_H

#include <e32base.h>

class CNATFWUNSAFEncodedItem;
class CNATFWUNSAFTcpRelayPacket;
class MNcmConnectionMultiplexer;
class MNcmSenderObserver;

/**
* A class for sending UNSAF messages to network using connection multiplexer.
*
*  @lib natfwunsafprotocols.lib
*  @since S60 3.2
*/
class CNATFWUNSAFTcpRelayPacketSender : public CBase
    {
public:    // Constructors and destructor

    /**
    * Creates a new instance of CNATFWUNSAFTcpRelayPacketSender
    * @param aMediaConnection Media connection used for sending
    * @return a new instance of CNATFWUNSAFTcpRelayPacketSender
    */
    IMPORT_C static CNATFWUNSAFTcpRelayPacketSender* NewL(
        MNcmConnectionMultiplexer& aMediaConnection );

    /**
    * Creates a new instance of CNATFWUNSAFTcpRelayPacketSender and
    * pushes in to CleanupStack.
    * @param aMediaConnection media connection used for sending
    * @return a new instance of CNATFWUNSAFTcpRelayPacketSender
    */
    IMPORT_C static CNATFWUNSAFTcpRelayPacketSender* NewLC(
        MNcmConnectionMultiplexer& aMediaConnection );

    /**
    * Destructor.
    */
    IMPORT_C virtual ~CNATFWUNSAFTcpRelayPacketSender();

public: // New functions

    /**
    * Sends an UNSAF message to the network.
    * Only one send operation can be outstanding at a time.
    * @param aStreamId Id of the stream to be used for sending
    * @param aSubstreamId Id of the substream to be used for sending
    * @param aMessage message to be sent
    * @param aObserver Observer for receiving sending related callbacks
    */
    IMPORT_C void SendL( const TUint aStreamId,
                    const TUint aSubstreamId,
                    const CNATFWUNSAFTcpRelayPacket& aMessage,
                    MNcmSenderObserver* aObserver = NULL );

private: // Constructors

    CNATFWUNSAFTcpRelayPacketSender(
        MNcmConnectionMultiplexer& aMediaConnection);

private: // Private methods

    void EncodeL( const CNATFWUNSAFTcpRelayPacket& aMessage,
                  const TUint aStreamId,
                  const TUint aSubstreamId );
    
    void RemoveEncoded();

    void SendToMediaConnectionL( MNcmSenderObserver* aObserver );


private: // Data
    
    MNcmConnectionMultiplexer& iMediaConnection;
    CNATFWUNSAFEncodedItem* iEncoded;

    TPtrC8 iCurrent;

    };

#endif // CNATFWUNSAFTCPRELAYPACKETSENDER_H


