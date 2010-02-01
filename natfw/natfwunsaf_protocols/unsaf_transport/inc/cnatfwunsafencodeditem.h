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





#ifndef CNATFWUNSAFENCODEDITEM_H
#define CNATFWUNSAFENCODEDITEM_H

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CNATFWUNSAFMessage;
class CNATFWUNSAFTcpRelayPacket;
class TInetAddr;

// CLASS DECLARATION
/**
* A class for storing details about single encoded UNSAF message.
*
*  @lib natfwunsafprotocols.lib
*/
NONSHARABLE_CLASS( CNATFWUNSAFEncodedItem ): public CBase
    {
    public:    // Constructors and destructor

        static CNATFWUNSAFEncodedItem* NewL(
            const CNATFWUNSAFMessage& aMessage,
            const TDesC8* aSharedSecret = 0,
            const TInetAddr* aAddr = 0,
            const TUint aStreamId = 0,
            const TUint aSubstreamId = 0,
            const TUint aUseFingerprint = 0 );

        static CNATFWUNSAFEncodedItem* NewLC(
            const CNATFWUNSAFMessage& aMessage,
            const TDesC8* aSharedSecret = 0,
            const TInetAddr* aAddr = 0,
            const TUint aStreamId = 0,
            const TUint aSubstreamId = 0,
            const TUint aUseFingerprint = 0 );

        static CNATFWUNSAFEncodedItem* NewL(
            const CNATFWUNSAFTcpRelayPacket& aMessage,
            const TUint aStreamId = 0,
            const TUint aSubstreamId = 0 );

        static CNATFWUNSAFEncodedItem* NewLC(
            const CNATFWUNSAFTcpRelayPacket& aMessage,
            const TUint aStreamId = 0,
            const TUint aSubstreamId = 0 );

        /**
        * Destructor.
        */
        ~CNATFWUNSAFEncodedItem();

    public: // New functions

        TPtr8 Ptr();

        TInetAddr& AddressL();

        TUint SubstreamId() const;

        TUint StreamId() const;

    private: // Private constructors

        void ConstructL(
            const CNATFWUNSAFMessage& aMessage,
            const TDesC8* aSharedSecret,
            const TInetAddr* aAddr,
            const TUint aStreamId,
            const TUint aSubstreamId,
            const TUint aUseFingerprint );

        void ConstructL(
            const CNATFWUNSAFTcpRelayPacket& aMessage,
            const TUint aStreamId,
            const TUint aSubstreamId );

        CNATFWUNSAFEncodedItem() {}

    private: // Data

        CBufBase* iEncoded;

        TInetAddr* iAddr;

        TUint iStreamId;

        TUint iSubstreamId;

        TUint iUseFingerprint;

    };

#endif // CNATFWUNSAFENCODEDITEM_H


