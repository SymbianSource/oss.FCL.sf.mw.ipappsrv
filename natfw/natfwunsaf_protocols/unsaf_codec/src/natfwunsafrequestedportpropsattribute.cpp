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
#include "natfwunsafrequestedportpropsattribute.h"
#include "natfwunsafutils.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFRequestedPortPropsAttribute::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFRequestedPortPropsAttribute*
CNATFWUNSAFRequestedPortPropsAttribute::NewL(TPortAlignmentPref aAlignmentPref,
    TUint16 aPort)
    {
    CNATFWUNSAFRequestedPortPropsAttribute* self =
        CNATFWUNSAFRequestedPortPropsAttribute::NewLC(aAlignmentPref,
            aPort);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRequestedPortPropsAttribute::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFRequestedPortPropsAttribute*
CNATFWUNSAFRequestedPortPropsAttribute::NewLC(
    TPortAlignmentPref aAlignmentPref, TUint16 aPort)
    {
    CNATFWUNSAFRequestedPortPropsAttribute* self =
        new (ELeave) CNATFWUNSAFRequestedPortPropsAttribute(aAlignmentPref,
            aPort);
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRequestedPortPropsAttribute::DecodeAttributeL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute*
CNATFWUNSAFRequestedPortPropsAttribute::DecodeAttributeL(
    const TDesC8& aByteStream)
    {
    TUint16 valueLength = ParseLengthL(aByteStream);

    //Stream has to have enough data for the whole value element
    __ASSERT_ALWAYS(valueLength == EAttributeValueSize,
                    User::Leave(KErrCorrupt));
    __ASSERT_ALWAYS(aByteStream.Length() >= EValueOffset + valueLength,
                    User::Leave(KErrArgument));

    TUint16 alignment = BigEndian::Get16(aByteStream.Mid(EValueOffset).Ptr());

    TUint16 port = BigEndian::Get16(aByteStream.Mid(
        (TInt)EValueOffset + (TInt)EPortOffset).Ptr());

    __ASSERT_ALWAYS(alignment <= EPortAlignmentMask,
        User::Leave(KErrArgument));

    return NewL(static_cast<TPortAlignmentPref>(alignment), port);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRequestedPortPropsAttribute::CNATFWUNSAFRequestedPortPropsAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFRequestedPortPropsAttribute::
CNATFWUNSAFRequestedPortPropsAttribute(TPortAlignmentPref aAlignmentPref,
    TUint16 aPort) :
    CNATFWUNSAFAttribute(),
    iPortAlignmentPref(aAlignmentPref),
    iPort(aPort)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRequestedPortPropsAttribute::~CNATFWUNSAFRequestedPortPropsAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFRequestedPortPropsAttribute::~CNATFWUNSAFRequestedPortPropsAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRequestedPortPropsAttribute::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::TType CNATFWUNSAFRequestedPortPropsAttribute::Type() const
    {
    return ERequestedPortProps;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRequestedPortPropsAttribute::PortRangePref
// -----------------------------------------------------------------------------
//
EXPORT_C TUint16 CNATFWUNSAFRequestedPortPropsAttribute::Port() const
    {
    return iPort;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRequestedPortPropsAttribute::PortAlignmentPref
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFRequestedPortPropsAttribute::TPortAlignmentPref
    CNATFWUNSAFRequestedPortPropsAttribute::PortAlignmentPref() const
    {
    return iPortAlignmentPref;
    }
    
// -----------------------------------------------------------------------------
// CNATFWUNSAFRequestedPortPropsAttribute::EncodeValueL
// -----------------------------------------------------------------------------
//
HBufC8* CNATFWUNSAFRequestedPortPropsAttribute::EncodeValueL() const
    {
    HBufC8* encodedValue = HBufC8::NewLC(EAttributeValueSize);

    TPtr8 ptr = encodedValue->Des();
    ptr.FillZ(EAttributeValueSize);

    NATFWUNSAFUtils::WriteNetworkOrder16L(ptr, 0,
        static_cast<TUint16>(iPortAlignmentPref));
    NATFWUNSAFUtils::WriteNetworkOrder16L(ptr, EPortOffset, iPort);

    CleanupStack::Pop(encodedValue);
    return encodedValue;
    }
