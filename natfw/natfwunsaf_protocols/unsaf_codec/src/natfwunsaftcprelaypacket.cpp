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




/*
TCP Relay packet header:

    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   | Relay type    | Reserved=0x00 |           Data length         |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/

#include <es_sock.h>
#include "natfwunsaftcprelaypacket.h"
#include "natfwunsafutils.h"

// CONSTANTS
const TInt KByteSize = 8;

// -----------------------------------------------------------------------------
// CNATFWUNSAFTcpRelayPacket::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFTcpRelayPacket* CNATFWUNSAFTcpRelayPacket::NewL(
    const TDesC8& aData,
    const TRelayType aType)
    {
    CNATFWUNSAFTcpRelayPacket* self =
        CNATFWUNSAFTcpRelayPacket::NewLC(aData, aType);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTcpRelayPacket::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFTcpRelayPacket* CNATFWUNSAFTcpRelayPacket::NewLC(
    const TDesC8& aData,
    const TRelayType aType)
    {
    CNATFWUNSAFTcpRelayPacket* self =
        new (ELeave) CNATFWUNSAFTcpRelayPacket();
    CleanupStack::PushL(self);
    self->ConstructL(aData, aType);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTcpRelayPacket::CreateL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFTcpRelayPacket* CNATFWUNSAFTcpRelayPacket::CreateL(
    const TDesC8& aData,
    const TRelayType aType )
    {
    return NewL(aData, aType);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTcpRelayPacket::CNATFWUNSAFTcpRelayPacket
// -----------------------------------------------------------------------------
//
CNATFWUNSAFTcpRelayPacket::CNATFWUNSAFTcpRelayPacket()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTcpRelayPacket::~CNATFWUNSAFTcpRelayPacket
// -----------------------------------------------------------------------------
//
CNATFWUNSAFTcpRelayPacket::~CNATFWUNSAFTcpRelayPacket()
    {
    delete iData;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTcpRelayPacket::ConstructL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTcpRelayPacket::ConstructL(
    const TDesC8& aData,
    const TRelayType aType )
    {
    CheckLengthL(aData.Length());
    iData = aData.AllocL();
    iType = aType;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTcpRelayPacket::EncodeL
// -----------------------------------------------------------------------------
//
EXPORT_C CBufBase* CNATFWUNSAFTcpRelayPacket::EncodeL() const
    {
    CBufBase* relay = EncodeRelayHeaderLC();
    relay->InsertL(relay->Size(), *iData);
    CleanupStack::Pop(relay);
    SetRelayLength(*relay);
    return relay;
    }
    
// -----------------------------------------------------------------------------
// CNATFWUNSAFTcpRelayPacket::EncodeRelayHeaderLC
// -----------------------------------------------------------------------------
//
CBufBase* CNATFWUNSAFTcpRelayPacket::EncodeRelayHeaderLC() const
    {
    CBufBase* header = CBufFlat::NewL(EFrameSize);
    CleanupStack::PushL(header);
    TUint16 relayType = Type();
     // Last eight bits of frame type are reserved as zeros
    relayType <<= KByteSize;
    TUint16 bigEndianType(0);
    BigEndian::Put16(reinterpret_cast<TUint8*>(&bigEndianType), relayType);
    header->InsertL(EFrameTypeOffset, &bigEndianType, sizeof(bigEndianType));
    //Value is zero, so it is same also in the big endian representation
    TUint16 frameLength(0);
    header->InsertL(EFrameLengthOffset, &frameLength, sizeof(frameLength));
    return header;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTcpRelayPacket::RelayLength
// TInt is guaranteed to be at least 32 bits.
// -----------------------------------------------------------------------------
//
TInt CNATFWUNSAFTcpRelayPacket::RelayLength(const TDesC8& aRelay)
    {
    if (aRelay.Length() < EFrameSize)
        {
        return KErrNotFound;
        }

    return BigEndian::Get16(aRelay.Mid(EFrameLengthOffset).Ptr());
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTcpRelayPacket::SetRelayLength
// UNSAF Relay length does not include the NATFWUNSAF header
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTcpRelayPacket::SetRelayLength(CBufBase& aRelay)
    {
    SetRelayLength(aRelay, aRelay.Size() - EFrameSize);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTcpRelayPacket::SetRelayLength
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTcpRelayPacket::SetRelayLength(CBufBase& aRelay, TInt aLength)
    {
    __ASSERT_ALWAYS(aLength >= 0, User::Panic(KNullDesC, KErrUnderflow));

    TUint16 bigEndianLength(0);
    BigEndian::Put16(reinterpret_cast<TUint8*>(&bigEndianLength),
                 static_cast<TUint16>(aLength));

    aRelay.Write(EFrameLengthOffset, &bigEndianLength,
        sizeof(bigEndianLength));
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTcpRelayPacket::CheckLengthL
// Check the granularity of attribute's length.
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTcpRelayPacket::CheckLengthL(TInt aLength) const
    {
    __ASSERT_ALWAYS((aLength % EGranularity) == 0, User::Leave(KErrCorrupt));
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTcpRelayPacket::Data
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC8& CNATFWUNSAFTcpRelayPacket::Data() const
    {
    return *iData;
    }
    
// -----------------------------------------------------------------------------
// CNATFWUNSAFTcpRelayPacket::Type
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFTcpRelayPacket::TRelayType
    CNATFWUNSAFTcpRelayPacket::Type() const
    {
    return iType;
    }
