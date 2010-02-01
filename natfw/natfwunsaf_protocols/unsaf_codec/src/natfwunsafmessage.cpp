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
#include "natfwunsafmessage.h"
#include "natfwunsaftransactionid.h"
#include "natfwunsafmessageintegrityattribute.h"
#include "natfwunsaffingerprintattribute.h"
#include "natfwunsafutils.h"


// -----------------------------------------------------------------------------
// CNATFWUNSAFMessage::CNATFWUNSAFMessage
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage::CNATFWUNSAFMessage(
    const TNATFWUNSAFTransactionID& aTransactionID)
#ifdef TEST_EUNIT
    : iAttributes(1)
#endif
    {
    iTransactionID = aTransactionID;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessage::~CNATFWUNSAFMessage
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage::~CNATFWUNSAFMessage()
    {
    iAttributes.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessage::TransactionID
// -----------------------------------------------------------------------------
//
EXPORT_C TNATFWUNSAFTransactionID CNATFWUNSAFMessage::TransactionID() const
    {
    return iTransactionID;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessage::HasAttribute
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CNATFWUNSAFMessage::HasAttribute(
    CNATFWUNSAFAttribute::TType aType) const
    {
    return Attribute(aType) ? ETrue : EFalse;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessage::Attribute
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFAttribute* CNATFWUNSAFMessage::Attribute(
    CNATFWUNSAFAttribute::TType aType) const
    {
    for (TInt i = 0; i < iAttributes.Count(); ++i)
        {
        if (iAttributes[i]->Type() == aType)
            {
            return iAttributes[i];
            }
        }

    return NULL;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessage::AddAttributeL
// -----------------------------------------------------------------------------
//
EXPORT_C void CNATFWUNSAFMessage::AddAttributeL(
    CNATFWUNSAFAttribute* aAttribute)
    {
    __ASSERT_ALWAYS(aAttribute, User::Leave(KErrArgument));

    iAttributes.AppendL(aAttribute);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessage::DetachAttribute
// -----------------------------------------------------------------------------
//
EXPORT_C void CNATFWUNSAFMessage::DetachAttribute(
    const CNATFWUNSAFAttribute* aAttribute)
    {
    TInt pos = iAttributes.Find(aAttribute);
    if (pos != KErrNotFound)
        {
        iAttributes.Remove(pos);
        }
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessage::DeleteAttribute
// Start search from the end of the array, so that deleting an attribute doesn't
// affect the positions of those attributes which haven't yet been checked.
// -----------------------------------------------------------------------------
//
EXPORT_C void CNATFWUNSAFMessage::DeleteAttribute(TUint16 aType)
    {
    for (TInt i = iAttributes.Count() - 1; i >= 0; --i)
        {
        if (iAttributes[i]->Type() == aType)
            {
            CNATFWUNSAFAttribute* attr = iAttributes[i];
            DetachAttribute(attr);
            delete attr;
            }
        }
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessage::EncodeL
// -----------------------------------------------------------------------------
//
EXPORT_C CBufBase* CNATFWUNSAFMessage::EncodeL() const
    {
    CBufBase* msg = EncodeMessageHeaderLC();

    for (TInt i = 0; i < iAttributes.Count(); ++i)
        {
        if (iAttributes[i]->Type() != CNATFWUNSAFAttribute::EMessageIntegrity &&
            iAttributes[i]->Type() != CNATFWUNSAFAttribute::EFingerprint)
            {
            HBufC8* attribute = iAttributes[i]->EncodeL();
            CleanupStack::PushL(attribute);
            msg->InsertL(msg->Size(), *attribute);
            CleanupStack::PopAndDestroy(attribute);
            }
        }
    CleanupStack::Pop(msg);
    msg->Compress();
    SetMessageLength(*msg);

    return msg;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessage::EncodeL
// 1. Encode the UNSAF message normally.
// 2. Create a MESSAGE-INTEGRITY attribute. This updates also the header's
//    length field to include the MESSAGE-INTEGRITY before computing the hash.
// 3. Encode it to the last attribute of the UNSAF message.
// 4. Create a FINGERPRINT attribute. This updates also the header's
//    length field to include the FINGERPRINT before computing the hash.
// 5. Encode it to the last attribute of the UNSAF message.
// -----------------------------------------------------------------------------
//
EXPORT_C CBufBase* CNATFWUNSAFMessage::EncodeL(
    const TDesC8& aSharedSecret,
    TBool aUseFingerprint) const
    {
    CBufBase* msg = EncodeL();
    CleanupStack::PushL(msg);
    if ( aSharedSecret.Length() )
        {
        //Long term credentials need be used if there's a REALM attribute
        CNATFWUNSAFMessageIntegrityAttribute* msgIntegrity =
            CNATFWUNSAFMessageIntegrityAttribute::NewLC(aSharedSecret, *msg,
                HasAttribute( CNATFWUNSAFAttribute::ERealm ));

        HBufC8* encodedMsgIntegrity = msgIntegrity->EncodeL();
        CleanupStack::PushL(encodedMsgIntegrity);
        msg->InsertL(msg->Size(), *encodedMsgIntegrity);
        CleanupStack::PopAndDestroy(encodedMsgIntegrity);
        CleanupStack::PopAndDestroy(msgIntegrity);
        }
    if ( aUseFingerprint )
        {
        CNATFWUNSAFFingerprintAttribute* fingerprint =
            CNATFWUNSAFFingerprintAttribute::NewLC(*msg);

        HBufC8* encodedFingerprint = fingerprint->EncodeL();
        CleanupStack::PushL(encodedFingerprint);
        msg->InsertL(msg->Size(), *encodedFingerprint);
        CleanupStack::PopAndDestroy(encodedFingerprint);
        CleanupStack::PopAndDestroy(fingerprint); 
        }

    CleanupStack::Pop(msg);
    msg->Compress();

    return msg;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessage::UnknownMandatoryAttributeFound
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFMessage::UnknownMandatoryAttributeFound()
    {
    iUnknownMandatoryAttributes = ETrue;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessage::HasUnknownMandatoryAttributes
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CNATFWUNSAFMessage::HasUnknownMandatoryAttributes() const
    {
    return iUnknownMandatoryAttributes;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessage::EncodeMessageHeaderLC
// -----------------------------------------------------------------------------
//
CBufBase* CNATFWUNSAFMessage::EncodeMessageHeaderLC() const
    {
    CBufBase* header = CBufFlat::NewL(EHeaderSize);
    CleanupStack::PushL(header);

    TUint16 messageType = Type();
    TUint16 bigEndianType(0);
    BigEndian::Put16(reinterpret_cast<TUint8*>(&bigEndianType), messageType);
    //First two bits of a STUN message are zero
    bigEndianType = bigEndianType & EMessageTypeMask;
    header->InsertL(EMessageTypeOffset, &bigEndianType, sizeof(bigEndianType));


    TUint16 msgLength = 0;
    //Value is zero, so it is same also in the big endian representation
    header->InsertL(EMessageLengthOffset, &msgLength, sizeof(msgLength));

    TUint32 bigEndianCookie(0);
    BigEndian::Put32(reinterpret_cast<TUint8*>(&bigEndianCookie), EMagicCookie);
    header->InsertL(EMagicCookieOffset,
                    &bigEndianCookie,
                    sizeof(bigEndianCookie));

    header->InsertL(ETransactionIDOffset,
                    iTransactionID,
                    KMaxNATFWUNSAFTransactionIdLength);
    return header;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessage::MessageLength
// TInt is guaranteed to be at least 32 bits.
// -----------------------------------------------------------------------------
//
TInt CNATFWUNSAFMessage::MessageLength(const TDesC8& aNATFWUNSAFMessage)
    {
    if (aNATFWUNSAFMessage.Length() < EHeaderSize)
        {
        return KErrNotFound;
        }

    return BigEndian::Get16(aNATFWUNSAFMessage.Mid(EMessageLengthOffset).Ptr());
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessage::SetMessageLength
// UNSAF message length does not include the NATFWUNSAF header
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFMessage::SetMessageLength(CBufBase& aNATFWUNSAFMessage)
    {
    SetMessageLength(aNATFWUNSAFMessage,
        aNATFWUNSAFMessage.Size() - EHeaderSize);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessage::SetMessageLength
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFMessage::SetMessageLength(CBufBase& aNATFWUNSAFMessage,
    TInt aLength)
    {
    __ASSERT_ALWAYS(aLength >= 0, User::Panic(KNullDesC, KErrUnderflow));

    TUint16 bigEndianLength(0);
    BigEndian::Put16(reinterpret_cast<TUint8*>(&bigEndianLength),
                     static_cast<TUint16>(aLength));

    aNATFWUNSAFMessage.Write(EMessageLengthOffset,
                        &bigEndianLength,
                        sizeof(bigEndianLength));
    }
