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
#include "natfwunsafaddressattribute.h"
#include "natfwunsafutils.h"


// -----------------------------------------------------------------------------
// CNATFWUNSAFAddressAttribute::CNATFWUNSAFAddressAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAddressAttribute::CNATFWUNSAFAddressAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAddressAttribute::CNATFWUNSAFAddressAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAddressAttribute::CNATFWUNSAFAddressAttribute(
    const TInetAddr& aInetAddr) :
    CNATFWUNSAFAttribute(),
    iInetAddr(aInetAddr)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAddressAttribute::~CNATFWUNSAFAddressAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAddressAttribute::~CNATFWUNSAFAddressAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAddressAttribute::EncodeValueL
// -----------------------------------------------------------------------------
//
HBufC8* CNATFWUNSAFAddressAttribute::EncodeValueL() const
    {
    TInt size = IsIPv6Address() ? EAttributeValueSizeIPv6 :
                                  EAttributeValueSizeIPv4;
    HBufC8* encodedValue = HBufC8::NewLC(size);
    TPtr8 ptr = encodedValue->Des();
    ptr.FillZ(size);

    if (IsIPv6Address())
        {
        ptr[EFamilyOffset] = KAddressFamilyIPv6;
        }
    else
        {
        ptr[EFamilyOffset] = KAddressFamilyIPv4;
        }
    EncodeAddressL(ptr);

    CleanupStack::Pop(encodedValue);
    return encodedValue;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAddressAttribute::Address
// -----------------------------------------------------------------------------
//
EXPORT_C TInetAddr& CNATFWUNSAFAddressAttribute::Address()
    {
    return iInetAddr;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAddressAttribute::Address
// -----------------------------------------------------------------------------
//
EXPORT_C const TInetAddr& CNATFWUNSAFAddressAttribute::Address() const
    {
    return iInetAddr;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAddressAttribute::EncodeAddressL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFAddressAttribute::EncodeAddressL(TDes8& aResult) const
    {
    EncodeAddressL(iInetAddr, aResult);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAddressAttribute::EncodeAddressL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFAddressAttribute::EncodeAddressL(const TInetAddr& aInetAddr,
                                            TDes8& aResult) const
    {
    NATFWUNSAFUtils::WriteNetworkOrder16L(aResult,
                                        EPortOffset,
                                     static_cast<TUint16>
                                         (aInetAddr.Port()));
    if (IsIPv6Address(aInetAddr))
        {
        const TInt KIPv6AddressSize = EAttributeValueSizeIPv6 - EAddressOffset;
        const TIp6Addr& ipv6Addr = aInetAddr.Ip6Address();
        for ( TInt i = 0; i < KIPv6AddressSize; ++ i )
            {
            aResult[EAddressOffset + i] = ipv6Addr.u.iAddr8[i];
            }
        }
    else
        {
        NATFWUNSAFUtils::WriteNetworkOrder32L(aResult,
                                         EAddressOffset,
                                         aInetAddr.Address());
        }
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAddressAttribute::DecodeValueL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFAddressAttribute::DecodeValueL(const TDesC8& aValue)
    {
    //Byte stream must contain at least enough data for this attribute (at least
    //enough data to contain an IPv4 address). It may contain more data (other
    //attributes etc).
    __ASSERT_ALWAYS(aValue.Length() >= EAttributeValueSizeIPv4,
        User::Leave(KErrArgument));

    iInetAddr.SetPort(BigEndian::Get16(aValue.Mid(EPortOffset).Ptr()));

    TUint8 addressFamily = aValue[EFamilyOffset];
    __ASSERT_ALWAYS(addressFamily == KAddressFamilyIPv4 ||
                    addressFamily == KAddressFamilyIPv6,
                    User::Leave(KErrArgument));
    if (addressFamily == KAddressFamilyIPv6)
        {
        DecodeIPv6AddressL(aValue.Mid(EAddressOffset));
        }
    else
        {
        //SetAddress also sets address family to KAfInet, meaning IPv4 address
        iInetAddr.SetAddress(
            BigEndian::Get32(aValue.Mid(EAddressOffset).Ptr()));
        }
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAddressAttribute::DecodeIPv6AddressL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFAddressAttribute::DecodeIPv6AddressL(const TDesC8& aValue)
    {
    const TInt KIPv6AddressSize = EAttributeValueSizeIPv6 - EAddressOffset;
    __ASSERT_ALWAYS(aValue.Length() >= KIPv6AddressSize,
        User::Leave(KErrArgument));

    TIp6Addr addr;
    for ( TInt i = 0; i < KIPv6AddressSize; ++ i )
        {
        addr.u.iAddr8[i] = aValue[i];
        }
    iInetAddr.SetAddress(addr);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAddressAttribute::CheckLengthL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFAddressAttribute::CheckLengthL(const TDesC8& aByteStream)
    {
    TUint16 length = ParseLengthL(aByteStream);
    __ASSERT_ALWAYS(length == EAttributeValueSizeIPv4 ||
                     length == EAttributeValueSizeIPv6,
                    User::Leave(KErrCorrupt));
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAddressAttribute::IsIPv6Address
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFAddressAttribute::IsIPv6Address() const
    {
    return IsIPv6Address(iInetAddr);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAddressAttribute::IsIPv6Address
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFAddressAttribute::IsIPv6Address(
    const TInetAddr& aInetAddr) const
    {
    return aInetAddr.Address() == 0 || aInetAddr.IsV4Compat();
    }
