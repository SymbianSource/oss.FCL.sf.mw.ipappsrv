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




#include "natfwunsaficecontrollingattribute.h"
#include "natfwunsafutils.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFIceControllingAttribute::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFIceControllingAttribute*
CNATFWUNSAFIceControllingAttribute::NewL(TUint64 aIceControlling)
    {
    CNATFWUNSAFIceControllingAttribute* self =
        CNATFWUNSAFIceControllingAttribute::NewLC(aIceControlling);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFIceControllingAttribute::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFIceControllingAttribute*
CNATFWUNSAFIceControllingAttribute::NewLC(TUint64 aIceControlling)
    {
    CNATFWUNSAFIceControllingAttribute* self =
        new (ELeave) CNATFWUNSAFIceControllingAttribute(aIceControlling);
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFIceControllingAttribute::DecodeAttributeL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute*
CNATFWUNSAFIceControllingAttribute::DecodeAttributeL(const TDesC8& aByteStream)
    {
    TUint16 valueLength = ParseLengthL(aByteStream);

    //Stream has to have enough data for the whole value element
    __ASSERT_ALWAYS(valueLength == EAttributeValueSize,
                    User::Leave(KErrCorrupt));
    __ASSERT_ALWAYS(aByteStream.Length() >= EValueOffset + valueLength,
                    User::Leave(KErrArgument));

    TUint64 value = NATFWUNSAFUtils::GetQuadWordL(
        aByteStream.Mid(EValueOffset).Ptr());
    
    return NewL(value);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFIceControllingAttribute::CNATFWUNSAFIceControllingAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFIceControllingAttribute::CNATFWUNSAFIceControllingAttribute(
    TUint64 aIceControlling) :
    CNATFWUNSAFAttribute(),
    iIceControlling(aIceControlling)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFIceControllingAttribute::~CNATFWUNSAFIceControllingAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFIceControllingAttribute::~CNATFWUNSAFIceControllingAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFIceControllingAttribute::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::TType CNATFWUNSAFIceControllingAttribute::Type() const
    {
    return EIceControlling;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFIceControllingAttribute::EncodeValueL
// -----------------------------------------------------------------------------
//
HBufC8* CNATFWUNSAFIceControllingAttribute::EncodeValueL() const
    {
    HBufC8* encodedValue = HBufC8::NewLC(EAttributeValueSize);

    TPtr8 ptr = encodedValue->Des();
    ptr.FillZ(EAttributeValueSize);

    TUint64 value(iIceControlling);
    NATFWUNSAFUtils::WriteNetworkOrder64L(ptr, 0, value);

    CleanupStack::Pop(encodedValue);
    return encodedValue;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFIceControllingAttribute::IceControlling
// -----------------------------------------------------------------------------
//
EXPORT_C TUint64 CNATFWUNSAFIceControllingAttribute::IceControlling() const
    {
    return iIceControlling;
    }
