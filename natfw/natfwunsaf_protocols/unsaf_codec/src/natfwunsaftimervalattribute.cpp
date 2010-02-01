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
#include "natfwunsaftimervalattribute.h"
#include "natfwunsafutils.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFTimerValAttribute::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFTimerValAttribute*
CNATFWUNSAFTimerValAttribute::NewL(TUint aTimerVal)
    {
    CNATFWUNSAFTimerValAttribute* self =
        CNATFWUNSAFTimerValAttribute::NewLC(aTimerVal);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTimerValAttribute::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFTimerValAttribute*
CNATFWUNSAFTimerValAttribute::NewLC(TUint aTimerVal)
    {
    CNATFWUNSAFTimerValAttribute* self =
        new (ELeave) CNATFWUNSAFTimerValAttribute(aTimerVal);
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTimerValAttribute::DecodeAttributeL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute*
CNATFWUNSAFTimerValAttribute::DecodeAttributeL(const TDesC8& aByteStream)
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
// CNATFWUNSAFTimerValAttribute::CNATFWUNSAFTimerValAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFTimerValAttribute::CNATFWUNSAFTimerValAttribute(TUint aTimerVal) :
    CNATFWUNSAFAttribute(),
    iTimerVal(aTimerVal)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTimerValAttribute::~CNATFWUNSAFTimerValAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFTimerValAttribute::~CNATFWUNSAFTimerValAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTimerValAttribute::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::TType CNATFWUNSAFTimerValAttribute::Type() const
    {
    return ETimerVal;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTimerValAttribute::EncodeValueL
// -----------------------------------------------------------------------------
//
HBufC8* CNATFWUNSAFTimerValAttribute::EncodeValueL() const
    {
    HBufC8* encodedValue = HBufC8::NewLC(EAttributeValueSize);

    TPtr8 ptr = encodedValue->Des();
    ptr.FillZ(EAttributeValueSize);

    TUint32 value(iTimerVal);
    NATFWUNSAFUtils::WriteNetworkOrder32L(ptr, 0, value);

    CleanupStack::Pop(encodedValue);
    return encodedValue;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTimerValAttribute::TimerVal
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CNATFWUNSAFTimerValAttribute::TimerVal() const
    {
    return iTimerVal;
    }
