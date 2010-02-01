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




#include "natfwunsaficecontrolledattribute.h"
#include "natfwunsafutils.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFIceControlledAttribute::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFIceControlledAttribute*
CNATFWUNSAFIceControlledAttribute::NewL(TUint64 aIceControlled)
    {
    CNATFWUNSAFIceControlledAttribute* self =
        CNATFWUNSAFIceControlledAttribute::NewLC(aIceControlled);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFIceControlledAttribute::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFIceControlledAttribute*
CNATFWUNSAFIceControlledAttribute::NewLC(TUint64 aIceControlled)
    {
    CNATFWUNSAFIceControlledAttribute* self =
        new (ELeave) CNATFWUNSAFIceControlledAttribute(aIceControlled);
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFIceControlledAttribute::DecodeAttributeL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute*
CNATFWUNSAFIceControlledAttribute::DecodeAttributeL(const TDesC8& aByteStream)
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
// CNATFWUNSAFIceControlledAttribute::CNATFWUNSAFIceControlledAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFIceControlledAttribute::CNATFWUNSAFIceControlledAttribute(
    TUint64 aIceControlled) :
    CNATFWUNSAFAttribute(),
    iIceControlled(aIceControlled)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFIceControlledAttribute::~CNATFWUNSAFIceControlledAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFIceControlledAttribute::~CNATFWUNSAFIceControlledAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFIceControlledAttribute::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::TType CNATFWUNSAFIceControlledAttribute::Type() const
    {
    return EIceControlled;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFIceControlledAttribute::EncodeValueL
// -----------------------------------------------------------------------------
//
HBufC8* CNATFWUNSAFIceControlledAttribute::EncodeValueL() const
    {
    HBufC8* encodedValue = HBufC8::NewLC(EAttributeValueSize);

    TPtr8 ptr = encodedValue->Des();
    ptr.FillZ(EAttributeValueSize);

    TUint64 value(iIceControlled);
    NATFWUNSAFUtils::WriteNetworkOrder64L(ptr, 0, value);

    CleanupStack::Pop(encodedValue);
    return encodedValue;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFIceControlledAttribute::IceControlled
// -----------------------------------------------------------------------------
//
EXPORT_C TUint64 CNATFWUNSAFIceControlledAttribute::IceControlled() const
    {
    return iIceControlled;
    }
