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




#include "natfwunsafmessageintegrityattribute.h"
#include "natfwunsafmessage.h"
#include "natfwunsaflog.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessageIntegrityAttribute::DecodeL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessageIntegrityAttribute*
CNATFWUNSAFMessageIntegrityAttribute::DecodeL(const TDesC8& aValue)
    {
    CNATFWUNSAFMessageIntegrityAttribute* self =
        new (ELeave) CNATFWUNSAFMessageIntegrityAttribute();
    CleanupStack::PushL(self);
    self->DecodeValueL(aValue);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessageIntegrityAttribute::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFMessageIntegrityAttribute*
CNATFWUNSAFMessageIntegrityAttribute::NewL(const TDesC8& aSharedSecret,
                                        CBufBase& aNATFWUNSAFMessage,
                                        TBool aLongTerm)
    {
    CNATFWUNSAFMessageIntegrityAttribute* self =
        CNATFWUNSAFMessageIntegrityAttribute::NewLC(aSharedSecret,
            aNATFWUNSAFMessage, aLongTerm);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessageIntegrityAttribute::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFMessageIntegrityAttribute*
CNATFWUNSAFMessageIntegrityAttribute::NewLC(const TDesC8& aSharedSecret,
                                         CBufBase& aNATFWUNSAFMessage,
                                         TBool aLongTerm)
    {
    CNATFWUNSAFMessageIntegrityAttribute* self =
        new (ELeave) CNATFWUNSAFMessageIntegrityAttribute();
    CleanupStack::PushL(self);
    self->ConstructL(aSharedSecret,aNATFWUNSAFMessage,aLongTerm);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessageIntegrityAttribute::DecodeAttributeL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute*
CNATFWUNSAFMessageIntegrityAttribute::DecodeAttributeL(
    const TDesC8& aByteStream)
    {
    CheckLengthL(aByteStream);

    return DecodeL(aByteStream.Mid(EValueOffset));
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessageIntegrityAttribute::CNATFWUNSAFMessageIntegrityAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessageIntegrityAttribute::CNATFWUNSAFMessageIntegrityAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessageIntegrityAttribute::ConstructL
// Update message header's length field to include the MESSAGE-INTEGRITY before
// computing the HMAC.
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFMessageIntegrityAttribute::ConstructL(
    const TDesC8& aSharedSecret, CBufBase& aNATFWUNSAFMessage, TBool aLongTerm)
    {
    iLongTerm = aLongTerm;
    TInt length = CNATFWUNSAFMessage::MessageLength(aNATFWUNSAFMessage.Ptr(0));
    CNATFWUNSAFMessage::SetMessageLength(aNATFWUNSAFMessage,
                                    length + EValueOffset + SHA1_HASH);
    iLongTerm ? ComputeLongTermHMACL(aSharedSecret, aNATFWUNSAFMessage, iHash)
              : ComputeShortTermHMACL(aSharedSecret, aNATFWUNSAFMessage, iHash);

    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessageIntegrityAttribute::~CNATFWUNSAFMessageIntegrityAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessageIntegrityAttribute::~CNATFWUNSAFMessageIntegrityAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessageIntegrityAttribute::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::TType CNATFWUNSAFMessageIntegrityAttribute::Type() const
    {
    return EMessageIntegrity;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessageIntegrityAttribute::EncodeValueL
// -----------------------------------------------------------------------------
//
HBufC8* CNATFWUNSAFMessageIntegrityAttribute::EncodeValueL() const
    {
    HBufC8* encodedValue = HBufC8::NewL(SHA1_HASH);
    *encodedValue = iHash;
    return encodedValue;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessageIntegrityAttribute::CheckMessageIntegrityL
// Exclude MESSAGE-INTEGRITY attribute from HMAC computing. It is located at the
// end of the message.
// If computed hash doesn't match, retry by excluding a possible FINGERPRINT
// attribute.
// If computed hash still doesn't match, retry so that the message header's
// length field doesn't include the MESSAGE-INTEGRITY.
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CNATFWUNSAFMessageIntegrityAttribute::CheckMessageIntegrityL(
    const TDesC8& aNATFWUNSAFMessage,
    const TDesC8& aSharedSecret) const
    {
    const TInt KMessageIntegritySize = EValueOffset + SHA1_HASH;
    //Check there is enough data
    __ASSERT_ALWAYS(aNATFWUNSAFMessage.Length() >= KMessageIntegritySize,
                    User::Leave(KErrArgument));

    CBufBase* message = CBufFlat::NewL(EInputBlockSize);
    CleanupStack::PushL(message);

    message->InsertL(0,
                     aNATFWUNSAFMessage,
                     aNATFWUNSAFMessage.Length() - KMessageIntegritySize);

    //This buffer is for possible FINGERPRINT removal
    CBufBase* message2 = CBufFlat::NewL(EInputBlockSize);
    CleanupStack::PushL(message2);
    const TInt KFingerprintSize = EValueOffset + 4;
    message2->InsertL(0,
                     aNATFWUNSAFMessage,
                     aNATFWUNSAFMessage.Length() - KMessageIntegritySize -
                        KFingerprintSize);

    TSHA1_Hash hash;
    iLongTerm ? ComputeLongTermHMACL(aSharedSecret, *message, hash)
              : ComputeShortTermHMACL(aSharedSecret, *message, hash);

    TBool match = (hash == iHash);
    NATFWUNSAF_INTLOG("Checking MSG-INT, match", match)

    if (!match)
        {
        // Try excluding a possible FINGERPRINT attribute
        NATFWUNSAF_LOG("MSG-INT didn't match, try removing FINGERPRINT")

        //Message length changed since calculating the HMAC, decrease it
        TInt newLength = aNATFWUNSAFMessage.Length() -
            CNATFWUNSAFMessage::EHeaderSize - KFingerprintSize;

        CNATFWUNSAFMessage::SetMessageLength(*message2, newLength);

        iLongTerm ? ComputeLongTermHMACL(aSharedSecret, *message2, hash)
                  : ComputeShortTermHMACL(aSharedSecret, *message2, hash);

        match = (hash == iHash);

        NATFWUNSAF_INTLOG("Checking MSG-INT 2nd try, match", match)
        }

    if (!match)
        {
        TInt newLength = aNATFWUNSAFMessage.Length() -
            (KMessageIntegritySize + CNATFWUNSAFMessage::EHeaderSize);
        NATFWUNSAF_INTLOG("MSG-INT didn't match, try with header length",
            newLength)
        CNATFWUNSAFMessage::SetMessageLength(*message, newLength);

        iLongTerm ? ComputeLongTermHMACL(aSharedSecret, *message, hash)
                  : ComputeShortTermHMACL(aSharedSecret, *message, hash);

        match = (hash == iHash);
        NATFWUNSAF_INTLOG("Checking MSG-INT 3rd try, match", match)
        }
    CleanupStack::PopAndDestroy(message2);
    CleanupStack::PopAndDestroy(message);
    return hash == iHash;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessageIntegrityAttribute::UseLongTermCredentials
// Sets credential lifetime.
// -----------------------------------------------------------------------------
//
EXPORT_C void CNATFWUNSAFMessageIntegrityAttribute::UseLongTermCredentials(
    TBool aLongTerm)
    {
    iLongTerm = aLongTerm;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessageIntegrityAttribute::CheckLengthL
// This attribute has a fixed length
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFMessageIntegrityAttribute::CheckLengthL(
    const TDesC8& aByteStream)
    {
    __ASSERT_ALWAYS(ParseLengthL(aByteStream) == SHA1_HASH,
                    User::Leave(KErrCorrupt));
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessageIntegrityAttribute::DecodeValueL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFMessageIntegrityAttribute::DecodeValueL(const TDesC8& aValue)
    {
    __ASSERT_ALWAYS(aValue.Length() >= static_cast<TInt>(SHA1_HASH),
                    User::Leave(KErrArgument));

    TPtrC8 ptrToHash = aValue.Left(SHA1_HASH);
    iHash = ptrToHash;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessageIntegrityAttribute::ComputeShortTermHMACL
// Pad the aNATFWUNSAFMessage with zeroes to make its length a multiple of
// EInputBlockSize bytes.
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFMessageIntegrityAttribute::ComputeShortTermHMACL(
    const TDesC8& aSharedSecret,
    CBufBase& aNATFWUNSAFMessage,
    TSHA1_Hash& aHash) const
    {
    CSHA1* sha1 = CSHA1::NewL();
    CleanupStack::PushL(sha1);
    CHMAC* hmac = CHMAC::NewL(aSharedSecret, sha1);
    CleanupStack::Pop(sha1);
    CleanupStack::PushL(hmac);

    DoComputeSHA1L(aHash, hmac, aNATFWUNSAFMessage);

    CleanupStack::PopAndDestroy(hmac);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessageIntegrityAttribute::ComputeLongTermHMACL
// Pad the aNATFWUNSAFMessage with zeroes to make its length a multiple of
// EInputBlockSize bytes.
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFMessageIntegrityAttribute::ComputeLongTermHMACL(
    const TDesC8& aSharedSecret,
    CBufBase& aNATFWUNSAFMessage,
    TSHA1_Hash& aHash) const
    {
    CMD5* md5 = CMD5::NewL();
    CleanupStack::PushL(md5);
    CSHA1* sha1 = CSHA1::NewL();
    CleanupStack::PushL(sha1);

    // Pass the 16 byte MD5-hash as key for HMAC
    CHMAC* hmac = CHMAC::NewL(md5->Hash(aSharedSecret), sha1);
    CleanupStack::Pop(sha1);
    CleanupStack::PopAndDestroy(md5);
    CleanupStack::PushL(hmac);

    DoComputeSHA1L(aHash, hmac, aNATFWUNSAFMessage);

    CleanupStack::PopAndDestroy(hmac);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessageIntegrityAttribute::DoComputeSHA1L
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFMessageIntegrityAttribute::DoComputeSHA1L(TSHA1_Hash& aHash,
    CHMAC* aHmac, CBufBase& aNATFWUNSAFMessage) const
    {
    __ASSERT_ALWAYS( aHmac, User::Leave( KErrArgument ) );
    
    // Pad msg with zeroes
    TInt bytesInLastBlock = aNATFWUNSAFMessage.Size() % EInputBlockSize;
    TInt bytesToAppend = 0;

    if (bytesInLastBlock > 0)
        {
        bytesToAppend = EInputBlockSize - bytesInLastBlock;
        const TUint8 KZero = 0;
        for (TInt i = 0; i < bytesToAppend; ++i)
            {
            aNATFWUNSAFMessage.InsertL(aNATFWUNSAFMessage.Size(), &KZero, 1);
            }
        }

    TPtrC8 hash = aHmac->Hash(aNATFWUNSAFMessage.Ptr(0));

    //Remove appended zeros
    aNATFWUNSAFMessage.Delete(aNATFWUNSAFMessage.Size() - bytesToAppend,
        bytesToAppend);

    //SHA-1 hash is a fixed length
    __ASSERT_ALWAYS(aHmac->HashSize() == SHA1_HASH, User::Leave(KErrGeneral));

    aHash = hash;
    }
