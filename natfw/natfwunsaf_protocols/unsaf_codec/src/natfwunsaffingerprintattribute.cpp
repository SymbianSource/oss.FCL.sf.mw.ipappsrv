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




#include "natfwunsaffingerprintattribute.h"
#include "natfwunsafmessage.h"
#include "natfwunsafutils.h"
#include "natfwunsaflog.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFFingerprintAttribute::DecodeL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFFingerprintAttribute*
CNATFWUNSAFFingerprintAttribute::DecodeL(const TDesC8& aValue)
    {
    CNATFWUNSAFFingerprintAttribute* self =
        new (ELeave) CNATFWUNSAFFingerprintAttribute();
    CleanupStack::PushL(self);
    self->DecodeValueL(aValue);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFFingerprintAttribute::NewL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFFingerprintAttribute*
CNATFWUNSAFFingerprintAttribute::NewL(CBufBase& aNATFWUNSAFMessage)
    {
    CNATFWUNSAFFingerprintAttribute* self =
        CNATFWUNSAFFingerprintAttribute::NewLC(aNATFWUNSAFMessage);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFFingerprintAttribute::NewLC
// -----------------------------------------------------------------------------
//
CNATFWUNSAFFingerprintAttribute*
CNATFWUNSAFFingerprintAttribute::NewLC(CBufBase& aNATFWUNSAFMessage)
    {
    CNATFWUNSAFFingerprintAttribute* self =
        new (ELeave) CNATFWUNSAFFingerprintAttribute();
    CleanupStack::PushL(self);
    self->ConstructL(aNATFWUNSAFMessage);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFFingerprintAttribute::DecodeAttributeL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute*
CNATFWUNSAFFingerprintAttribute::DecodeAttributeL(
    const TDesC8& aByteStream)
    {
    CheckLengthL(aByteStream);

    return DecodeL(aByteStream.Mid(EValueOffset));
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFFingerprintAttribute::CNATFWUNSAFFingerprintAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFFingerprintAttribute::CNATFWUNSAFFingerprintAttribute()
    {

    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFFingerprintAttribute::ConstructL
// Update message header's length field to include the FINGERPRINT before
// computing the CRC-32.
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFFingerprintAttribute::ConstructL(CBufBase& aNATFWUNSAFMessage)
    {
    __ASSERT_ALWAYS(aNATFWUNSAFMessage.Size(), User::Leave(KErrArgument));
    TInt length = CNATFWUNSAFMessage::MessageLength(aNATFWUNSAFMessage.Ptr(0));
    CNATFWUNSAFMessage::SetMessageLength(aNATFWUNSAFMessage,
        length + EValueOffset + EAttributeValueSize);

    ComputeCRC32(aNATFWUNSAFMessage, iCRC);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFFingerprintAttribute::~CNATFWUNSAFFingerprintAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFFingerprintAttribute::~CNATFWUNSAFFingerprintAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFFingerprintAttribute::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::TType CNATFWUNSAFFingerprintAttribute::Type() const
    {
    return EFingerprint;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFFingerprintAttribute::EncodeValueL
// -----------------------------------------------------------------------------
//
HBufC8* CNATFWUNSAFFingerprintAttribute::EncodeValueL() const
    {
    HBufC8* encodedValue = HBufC8::NewLC(EAttributeValueSize);

    TPtr8 ptr = encodedValue->Des();
    ptr.FillZ(EAttributeValueSize);

    TUint32 value(iCRC);
    NATFWUNSAFUtils::WriteNetworkOrder32L(ptr, 0, value);

    CleanupStack::Pop(encodedValue);
    return encodedValue;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFFingerprintAttribute::CheckLengthL
// This attribute has a fixed length
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFFingerprintAttribute::CheckLengthL(
    const TDesC8& aByteStream)
    {
    __ASSERT_ALWAYS(ParseLengthL(aByteStream) == EAttributeValueSize,
                    User::Leave(KErrCorrupt));
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFFingerprintAttribute::CheckFingerprintL
// Exclude FINGERPRINT attribute from CRC-32 computing. It is located at the
// end of the message.
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CNATFWUNSAFFingerprintAttribute::CheckFingerprintL(
    const TDesC8& aNATFWUNSAFMessage) const
    {
    TInt KFingerprintSize = static_cast<TInt>(EValueOffset) +
        static_cast<TInt>(EAttributeValueSize);
    //Check there is enough data
    __ASSERT_ALWAYS(aNATFWUNSAFMessage.Length() >= KFingerprintSize,
                    User::Leave(KErrArgument));

    CBufBase* message = CBufFlat::NewL(
        aNATFWUNSAFMessage.Length() - KFingerprintSize);
    CleanupStack::PushL(message);
    message->InsertL(0,
                     aNATFWUNSAFMessage,
                     aNATFWUNSAFMessage.Length() - KFingerprintSize);

    TUint32 crc(0);
    ComputeCRC32(*message, crc);

    TBool match = (crc == iCRC);
    NATFWUNSAF_INTLOG("Checking FINGERPRINT, match", match)

    CleanupStack::PopAndDestroy(message);
    return match;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFFingerprintAttribute::DecodeValueL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFFingerprintAttribute::DecodeValueL(const TDesC8& aValue)
    {
    __ASSERT_ALWAYS(aValue.Length() == EAttributeValueSize,
        User::Leave(KErrCorrupt));
    iCRC = BigEndian::Get32(aValue.Ptr());
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFFingerprintAttribute::ComputeCRC32
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFFingerprintAttribute::ComputeCRC32(
    CBufBase& aNATFWUNSAFMessage,
    TUint32& aCRC) const
    {
    // Perform the CCITT CRC-32 checksum on the message
    Mem::Crc32(aCRC, aNATFWUNSAFMessage.Ptr(0).Ptr(),
        sizeof(aNATFWUNSAFMessage));
    // XOR according to RFC3489-bis05
    aCRC ^= EXORedValue;
    }
