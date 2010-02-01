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
#include "natfwunsafmagiccookieattribute.h"
#include "natfwunsafutils.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFMagicCookieAttribute::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFMagicCookieAttribute*
CNATFWUNSAFMagicCookieAttribute::NewL()
    {
    CNATFWUNSAFMagicCookieAttribute* self =
        CNATFWUNSAFMagicCookieAttribute::NewLC();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMagicCookieAttribute::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFMagicCookieAttribute*
CNATFWUNSAFMagicCookieAttribute::NewLC()
    {
    CNATFWUNSAFMagicCookieAttribute* self =
        new (ELeave) CNATFWUNSAFMagicCookieAttribute();
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMagicCookieAttribute::DecodeAttributeL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute*
CNATFWUNSAFMagicCookieAttribute::DecodeAttributeL(const TDesC8& aByteStream)
    {
    TUint16 valueLength = ParseLengthL(aByteStream);

    //Stream has to have enough data for the whole value element
    __ASSERT_ALWAYS(valueLength == EAttributeValueSize,
                    User::Leave(KErrCorrupt));
    __ASSERT_ALWAYS(aByteStream.Length() >= EValueOffset + valueLength,
                    User::Leave(KErrArgument));


    TUint32 value = BigEndian::Get32(aByteStream.Mid(EValueOffset).Ptr());
    __ASSERT_ALWAYS(EMagicCookieValue == value,
                    User::Leave(KErrArgument));
    return NewL();
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMagicCookieAttribute::CNATFWUNSAFMagicCookieAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMagicCookieAttribute::CNATFWUNSAFMagicCookieAttribute() :
    CNATFWUNSAFAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMagicCookieAttribute::~CNATFWUNSAFMagicCookieAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMagicCookieAttribute::~CNATFWUNSAFMagicCookieAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMagicCookieAttribute::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::TType CNATFWUNSAFMagicCookieAttribute::Type() const
    {
    return EMagicCookie;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMagicCookieAttribute::EncodeValueL
// -----------------------------------------------------------------------------
//
HBufC8* CNATFWUNSAFMagicCookieAttribute::EncodeValueL() const
    {
    HBufC8* encodedValue = HBufC8::NewLC(EAttributeValueSize);

    TPtr8 ptr = encodedValue->Des();
    ptr.FillZ(EAttributeValueSize);

    TUint32 value(EMagicCookieValue);
    NATFWUNSAFUtils::WriteNetworkOrder32L(ptr, 0, value);

    CleanupStack::Pop(encodedValue);
    return encodedValue;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMagicCookieAttribute::MagicCookie
// -----------------------------------------------------------------------------
//
EXPORT_C TUint32 CNATFWUNSAFMagicCookieAttribute::MagicCookie() const
    {
    return EMagicCookieValue;
    }
