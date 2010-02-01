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




#include <es_sock.h>
#include "natfwunsaftcprelaypacketfactory.h"
#include "natfwunsafutils.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFTcpRelayPacketFactory::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFTcpRelayPacketFactory*
    CNATFWUNSAFTcpRelayPacketFactory::NewL()
    {
    CNATFWUNSAFTcpRelayPacketFactory* self =
        CNATFWUNSAFTcpRelayPacketFactory::NewLC();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTcpRelayPacketFactory::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFTcpRelayPacketFactory*
    CNATFWUNSAFTcpRelayPacketFactory::NewLC()
    {
    CNATFWUNSAFTcpRelayPacketFactory* self =
        new (ELeave) CNATFWUNSAFTcpRelayPacketFactory();
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTcpRelayPacketFactory::CNATFWUNSAFTcpRelayPacketFactory
// -----------------------------------------------------------------------------
//
CNATFWUNSAFTcpRelayPacketFactory::CNATFWUNSAFTcpRelayPacketFactory()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTcpRelayPacketFactory::~CNATFWUNSAFTcpRelayPacketFactory
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFTcpRelayPacketFactory::~CNATFWUNSAFTcpRelayPacketFactory()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTcpRelayPacketFactory::MessageLength
// -----------------------------------------------------------------------------
//
TInt CNATFWUNSAFTcpRelayPacketFactory::MessageLength(
    const TDesC8& aNATFWUNSAFTcpRelayPacket) const
    {
    return CNATFWUNSAFTcpRelayPacket::RelayLength(aNATFWUNSAFTcpRelayPacket);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTcpRelayPacketFactory::DecodeHeaderLC
// Check that the first byte is 0x02 or 0x03. Otherwise this is not a relay
// packet.
// -----------------------------------------------------------------------------
//
CNATFWUNSAFTcpRelayPacket* CNATFWUNSAFTcpRelayPacketFactory::DecodeHeaderLC(
    const TDesC8& aNATFWUNSAFTcpRelayPacket) const
    {
    __ASSERT_ALWAYS(
        aNATFWUNSAFTcpRelayPacket.Length() >=
            CNATFWUNSAFTcpRelayPacket::EFrameSize,
                User::Leave(KErrArgument));
    //Last 8 bits are reserved as zeros
    TUint8 relayType = NATFWUNSAFUtils::HighByte( BigEndian::Get16(
        aNATFWUNSAFTcpRelayPacket.Mid(
            CNATFWUNSAFTcpRelayPacket::EFrameTypeOffset).Ptr()));

    __ASSERT_ALWAYS(relayType == CNATFWUNSAFTcpRelayPacket::EFrameTypeStun ||
        relayType == CNATFWUNSAFTcpRelayPacket::EFrameTypeData,
            User::Leave(KErrArgument));

    TUint16 dataLength = BigEndian::Get16(aNATFWUNSAFTcpRelayPacket.Mid(
        CNATFWUNSAFTcpRelayPacket::EFrameLengthOffset).Ptr());


    CNATFWUNSAFTcpRelayPacket* relay = CNATFWUNSAFTcpRelayPacket::CreateL(
        aNATFWUNSAFTcpRelayPacket.Mid(CNATFWUNSAFTcpRelayPacket::EFrameSize,
             dataLength),static_cast<CNATFWUNSAFTcpRelayPacket::TRelayType>
                (relayType));

    CleanupStack::PushL(relay);
    return relay;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTcpRelayPacketFactory::DecodeL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFTcpRelayPacket* CNATFWUNSAFTcpRelayPacketFactory::DecodeL(
    const TDesC8& aNATFWUNSAFTcpRelayPacket) const
    {
    //Decoding the header is all that is needed
    CNATFWUNSAFTcpRelayPacket* relay =
        DecodeHeaderLC(aNATFWUNSAFTcpRelayPacket);
    CleanupStack::Pop(relay);
    return relay;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTcpRelayPacketFactory::CreateL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFTcpRelayPacket* CNATFWUNSAFTcpRelayPacketFactory::CreateL(
    const CNATFWUNSAFTcpRelayPacket::TRelayType& aType,
    const TDesC8& aData) const
    {
    return CNATFWUNSAFTcpRelayPacket::CreateL(aData, aType);
    }

