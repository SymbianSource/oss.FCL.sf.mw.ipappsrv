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
#include "natfwunsafbandwidthattribute.h"
#include "natfwunsafutils.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFBandwidthAttribute::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFBandwidthAttribute*
CNATFWUNSAFBandwidthAttribute::NewL(TInt aBandwidth)
    {
    CNATFWUNSAFBandwidthAttribute* self =
        CNATFWUNSAFBandwidthAttribute::NewLC(aBandwidth);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFBandwidthAttribute::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFBandwidthAttribute*
CNATFWUNSAFBandwidthAttribute::NewLC(TInt aBandwidth)
    {
    CNATFWUNSAFBandwidthAttribute* self =
        new (ELeave) CNATFWUNSAFBandwidthAttribute();
    CleanupStack::PushL(self);
    self->ConstructL(aBandwidth);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFBandwidthAttribute::DecodeAttributeL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute*
CNATFWUNSAFBandwidthAttribute::DecodeAttributeL(const TDesC8& aByteStream)
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
// CNATFWUNSAFBandwidthAttribute::CNATFWUNSAFBandwidthAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFBandwidthAttribute::CNATFWUNSAFBandwidthAttribute() :
    CNATFWUNSAFAttribute(),
    iBandwidth(0)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFBandwidthAttribute::ConstructL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFBandwidthAttribute::ConstructL(TInt aBandwidth)
    {
    __ASSERT_ALWAYS(0 <= aBandwidth, User::Leave(KErrArgument));

    iBandwidth = aBandwidth;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFBandwidthAttribute::~CNATFWUNSAFBandwidthAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFBandwidthAttribute::~CNATFWUNSAFBandwidthAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFBandwidthAttribute::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::TType CNATFWUNSAFBandwidthAttribute::Type() const
    {
    return EBandwidth;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFBandwidthAttribute::EncodeValueL
// -----------------------------------------------------------------------------
//
HBufC8* CNATFWUNSAFBandwidthAttribute::EncodeValueL() const
    {
    HBufC8* encodedValue = HBufC8::NewLC(EAttributeValueSize);

    TPtr8 ptr = encodedValue->Des();
    ptr.FillZ(EAttributeValueSize);

    TUint32 value(iBandwidth);
    NATFWUNSAFUtils::WriteNetworkOrder32L(ptr, 0, value);

    CleanupStack::Pop(encodedValue);
    return encodedValue;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFBandwidthAttribute::Bandwidth
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CNATFWUNSAFBandwidthAttribute::Bandwidth() const
    {
    return iBandwidth;
    }
