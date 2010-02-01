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
#include "natfwunsafunknownattributesattribute.h"
#include "natfwunsafutils.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFUnknownAttributesAttribute::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFUnknownAttributesAttribute*
CNATFWUNSAFUnknownAttributesAttribute::NewL()
    {
    CNATFWUNSAFUnknownAttributesAttribute* self =
        CNATFWUNSAFUnknownAttributesAttribute::NewLC();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFUnknownAttributesAttribute::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFUnknownAttributesAttribute*
CNATFWUNSAFUnknownAttributesAttribute::NewLC()
    {
    CNATFWUNSAFUnknownAttributesAttribute* self =
        new (ELeave) CNATFWUNSAFUnknownAttributesAttribute();
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFUnknownAttributesAttribute::DecodeAttributeL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute*
CNATFWUNSAFUnknownAttributesAttribute::DecodeAttributeL(
    const TDesC8& aByteStream)
    {
    TUint16 valueLength = ParseLengthL(aByteStream);

    //Stream has to have enough data for the whole value element
    __ASSERT_ALWAYS(aByteStream.Length() >= EValueOffset + valueLength,
                    User::Leave(KErrArgument));

    //Check the value length is valid for this attribute
    CheckLengthL(valueLength);

    CNATFWUNSAFUnknownAttributesAttribute* self =
        new (ELeave) CNATFWUNSAFUnknownAttributesAttribute();
    CleanupStack::PushL(self);

    self->DecodeValueL(aByteStream.Mid(EValueOffset, valueLength));
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFUnknownAttributesAttribute::CNATFWUNSAFUnknownAttributesAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFUnknownAttributesAttribute::CNATFWUNSAFUnknownAttributesAttribute() :
    CNATFWUNSAFAttribute()
#ifdef TEST_EUNIT
    , iContainedAttributes(1)
#endif
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFUnknownAttributesAttribute::~CNATFWUNSAFUnknownAttributesAttribute
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFUnknownAttributesAttribute::
~CNATFWUNSAFUnknownAttributesAttribute()
    {
    iContainedAttributes.Close();
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFUnknownAttributesAttribute::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::TType CNATFWUNSAFUnknownAttributesAttribute::Type() const
    {
    __TEST_INVARIANT;

    return EUnknownAttributes;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFUnknownAttributesAttribute::EncodeValueL
// If the number of attributes is an odd number, one attribute is repeated.
// -----------------------------------------------------------------------------
//
HBufC8* CNATFWUNSAFUnknownAttributesAttribute::EncodeValueL() const
    {
    __TEST_INVARIANT;

    TInt count = iContainedAttributes.Count();
    if (count & 0x1)
        {
        count++;
        }

    HBufC8* encodedValue = HBufC8::NewLC(count * EElementSize);
    TPtr8 ptr = encodedValue->Des();
    ptr.FillZ(count * EElementSize);

    for (TInt i = 0; i < iContainedAttributes.Count(); ++i)
        {
        TUint16 value = static_cast<TUint16>(iContainedAttributes[i]);
        NATFWUNSAFUtils::WriteNetworkOrder16L(ptr,
                                         i * EElementSize,
                                         value);
        }
    if (count > iContainedAttributes.Count())
        {
        TUint16 value = static_cast<TUint16>(iContainedAttributes[0]);
        NATFWUNSAFUtils::WriteNetworkOrder16L(ptr,
                                 iContainedAttributes.Count() * EElementSize,
                                 value);
        }

    CleanupStack::Pop(encodedValue);
    return encodedValue;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFUnknownAttributesAttribute::AddContainedAttributeL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFUnknownAttributesAttribute::AddContainedAttributeL(
    TUint16 aAttributeType)
    {
    __ASSERT_ALWAYS(iContainedAttributes.Find(aAttributeType) == KErrNotFound,
                    User::Leave(KErrAlreadyExists));

    iContainedAttributes.AppendL(aAttributeType);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFUnknownAttributesAttribute::RemoveContainedAttribute
// -----------------------------------------------------------------------------
//
TInt CNATFWUNSAFUnknownAttributesAttribute::RemoveContainedAttribute(
    TUint16 aAttributeType)
    {
    TInt index = iContainedAttributes.Find(aAttributeType);

    if (index == KErrNotFound)
        {
        return KErrNotFound;
        }

    iContainedAttributes.Remove(index);
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFUnknownAttributesAttribute::ContainedAttributes
// -----------------------------------------------------------------------------
//
EXPORT_C const RArray<TUint32>& CNATFWUNSAFUnknownAttributesAttribute::
ContainedAttributes() const
    {
    __TEST_INVARIANT;
    return iContainedAttributes;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFUnknownAttributesAttribute::CheckLengthL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFUnknownAttributesAttribute::CheckLengthL(TInt aLength)
    {
    __ASSERT_ALWAYS((aLength % EGranularity) == 0, User::Leave(KErrCorrupt));
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFUnknownAttributesAttribute::DecodeValueL
// If the number of attributes would be an odd number, one attribute is
// repeated. The repeated attribute is not placed into iContainedAttributes
// array.
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFUnknownAttributesAttribute::DecodeValueL(const TDesC8& aValue)
    {
    for (TInt pos = 0; pos < aValue.Length(); pos = pos + EElementSize)
        {
        TUint attrType = BigEndian::Get16(aValue.Mid(pos).Ptr());

        if (iContainedAttributes.Find(attrType) == KErrNotFound)
            {
            iContainedAttributes.AppendL(attrType);
            }
        }
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFUnknownAttributesAttribute::__DbgTestInvariant
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFUnknownAttributesAttribute::__DbgTestInvariant() const
    {
#if defined(_DEBUG)
/*    if ( error )
        {
        User::Invariant();
        } */
#endif
    }
