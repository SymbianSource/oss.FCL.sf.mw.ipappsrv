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




#include "natfwunsafxormappedaddressattribute.h"
#include "natfwunsafmessage.h"
#include "natfwunsafutils.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFXorMappedAddressAttribute::DecodeAttributeL
// After decoding, iInetAddr has the XOR'd address. To get the real address,
// SetTransactionIDAndXorL must be used.
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute* CNATFWUNSAFXorMappedAddressAttribute::DecodeAttributeL(
    const TDesC8& aByteStream)
    {
    CheckLengthL(aByteStream);
    CNATFWUNSAFXorMappedAddressAttribute* self =
        new(ELeave)CNATFWUNSAFXorMappedAddressAttribute;
    CleanupStack::PushL(self);
    self->DecodeValueL(aByteStream.Mid(EValueOffset));
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFXorMappedAddressAttribute::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFXorMappedAddressAttribute*
CNATFWUNSAFXorMappedAddressAttribute::NewL(
    const TInetAddr& aAddress,
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFXorMappedAddressAttribute* self =
        CNATFWUNSAFXorMappedAddressAttribute::NewLC(aAddress, aTransactionID);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFXorMappedAddressAttribute::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFXorMappedAddressAttribute*
CNATFWUNSAFXorMappedAddressAttribute::NewLC(
    const TInetAddr& aAddress,
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    CNATFWUNSAFXorMappedAddressAttribute* self =
        new (ELeave) CNATFWUNSAFXorMappedAddressAttribute(aAddress);
    CleanupStack::PushL(self);
    self->SetTransactionIdL(aTransactionID);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFXorMappedAddressAttribute::CNATFWUNSAFXorMappedAddressAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFXorMappedAddressAttribute::CNATFWUNSAFXorMappedAddressAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFXorMappedAddressAttribute::CNATFWUNSAFXorMappedAddressAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFXorMappedAddressAttribute::CNATFWUNSAFXorMappedAddressAttribute(
    const TInetAddr& aAddress) :
    CNATFWUNSAFAddressAttribute(aAddress)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFXorMappedAddressAttribute::~CNATFWUNSAFXorMappedAddressAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFXorMappedAddressAttribute::~CNATFWUNSAFXorMappedAddressAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFXorMappedAddressAttribute::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::TType CNATFWUNSAFXorMappedAddressAttribute::Type() const
    {
    return EXorMappedAddress;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFXorMappedAddressAttribute::SetTransactionID
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFXorMappedAddressAttribute::SetTransactionIdL(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    __ASSERT_ALWAYS(
        aTransactionID.Length() == KMaxNATFWUNSAFTransactionIdLength,
            User::Leave(KErrArgument));
    iTransactionID.Copy(aTransactionID);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFXorMappedAddressAttribute::TransactionID
// -----------------------------------------------------------------------------
//
const TNATFWUNSAFTransactionID&
CNATFWUNSAFXorMappedAddressAttribute::TransactionID() const
    {
    return iTransactionID;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFXorMappedAddressAttribute::SetTransactionIDAndXorL
// -----------------------------------------------------------------------------
//
EXPORT_C void CNATFWUNSAFXorMappedAddressAttribute::SetTransactionIDAndXorL(
    const TNATFWUNSAFTransactionID& aTransactionID)
    {
    SetTransactionIdL(aTransactionID);
    iInetAddr = XorAddress();
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFXorMappedAddressAttribute::EncodeAddressL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFXorMappedAddressAttribute::EncodeAddressL(TDes8& aResult) const
    {
    CNATFWUNSAFAddressAttribute::EncodeAddressL(XorAddress(), aResult);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFXorMappedAddressAttribute::XorPort
// Port is XOR'd with most significant 16 bits of the magic cookie.
// -----------------------------------------------------------------------------
//
TUint16 CNATFWUNSAFXorMappedAddressAttribute::XorPort() const
    {
    TUint16 result = 0;
    if (iTransactionID.Length() == KMaxNATFWUNSAFTransactionIdLength)
        {
           const TUint KPortMask = 0xffff;
           result = ((CNATFWUNSAFMessage::EMagicCookie >> 16) & KPortMask) ^
                    static_cast<TUint16>(iInetAddr.Port());
        }
    return result;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFXorMappedAddressAttribute::XorAddress
// Sets both address and port, and returns a copy of the XOR'd address.
// -----------------------------------------------------------------------------
//
TInetAddr CNATFWUNSAFXorMappedAddressAttribute::XorAddress() const
    {
    TInetAddr addr;

    if (iTransactionID.Length() == KMaxNATFWUNSAFTransactionIdLength)
        {
        if (IsIPv6Address())
            {
            TIp6Addr xorAddr;
            const TIp6Addr& origAddr = iInetAddr.Ip6Address();

            TUint16 highWord =
                NATFWUNSAFUtils::HighWord(CNATFWUNSAFMessage::EMagicCookie);
            xorAddr.u.iAddr8[0] =
                origAddr.u.iAddr8[0] ^ NATFWUNSAFUtils::HighByte(highWord);
            xorAddr.u.iAddr8[1] =
                origAddr.u.iAddr8[1] ^ NATFWUNSAFUtils::LowByte(highWord);

            TUint16 lowWord = NATFWUNSAFUtils::LowWord(
                CNATFWUNSAFMessage::EMagicCookie);
            xorAddr.u.iAddr8[2] =
                origAddr.u.iAddr8[2] ^ NATFWUNSAFUtils::HighByte(lowWord);
            xorAddr.u.iAddr8[3] =
                origAddr.u.iAddr8[3] ^ NATFWUNSAFUtils::LowByte(lowWord);

            const TInt cookieLength =
                CNATFWUNSAFMessage::ETransactionIDOffset -
                    CNATFWUNSAFMessage::EMagicCookieOffset;
            for (TInt i = 0; i < KMaxNATFWUNSAFTransactionIdLength; ++i)
                {
                xorAddr.u.iAddr8[i + cookieLength] =
                    origAddr.u.iAddr8[i + cookieLength] ^ iTransactionID[i];
                }
            addr.SetAddress(xorAddr);
            }
        else
            {
            addr.SetAddress(
                CNATFWUNSAFMessage::EMagicCookie ^ iInetAddr.Address());
            }
        }

    addr.SetPort(XorPort());
    return addr;
    }
