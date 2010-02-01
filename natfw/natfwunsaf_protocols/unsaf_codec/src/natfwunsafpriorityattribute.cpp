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
#include "natfwunsafpriorityattribute.h"
#include "natfwunsafutils.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFPriorityAttribute::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFPriorityAttribute*
CNATFWUNSAFPriorityAttribute::NewL(TUint aPriority)
    {
    CNATFWUNSAFPriorityAttribute* self =
        CNATFWUNSAFPriorityAttribute::NewLC(aPriority);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFPriorityAttribute::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFPriorityAttribute*
CNATFWUNSAFPriorityAttribute::NewLC(TUint aPriority)
    {
    CNATFWUNSAFPriorityAttribute* self =
        new (ELeave) CNATFWUNSAFPriorityAttribute(aPriority);
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFPriorityAttribute::DecodeAttributeL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute*
CNATFWUNSAFPriorityAttribute::DecodeAttributeL(const TDesC8& aByteStream)
    {
    TUint16 valueLength = ParseLengthL(aByteStream);

    //Stream has to have enough data for the whole value element
    __ASSERT_ALWAYS(valueLength == EAttributeValueSize,
                    User::Leave(KErrCorrupt));
    __ASSERT_ALWAYS(aByteStream.Length() >= EValueOffset + valueLength,
                    User::Leave(KErrArgument));

    TUint32 value = BigEndian::Get32(aByteStream.Mid(EValueOffset).Ptr());
    return NewL(value);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFPriorityAttribute::CNATFWUNSAFPriorityAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFPriorityAttribute::CNATFWUNSAFPriorityAttribute(TUint aPriority) :
    CNATFWUNSAFAttribute(),
    iPriority(aPriority)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFPriorityAttribute::~CNATFWUNSAFPriorityAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFPriorityAttribute::~CNATFWUNSAFPriorityAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFPriorityAttribute::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::TType CNATFWUNSAFPriorityAttribute::Type() const
    {
    return EPriority;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFPriorityAttribute::EncodeValueL
// -----------------------------------------------------------------------------
//
HBufC8* CNATFWUNSAFPriorityAttribute::EncodeValueL() const
    {
    HBufC8* encodedValue = HBufC8::NewLC(EAttributeValueSize);

    TPtr8 ptr = encodedValue->Des();
    ptr.FillZ(EAttributeValueSize);

    TUint32 value(iPriority);
    NATFWUNSAFUtils::WriteNetworkOrder32L(ptr, 0, value);

    CleanupStack::Pop(encodedValue);
    return encodedValue;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFPriorityAttribute::Priority
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CNATFWUNSAFPriorityAttribute::Priority() const
    {
    return iPriority;
    }
