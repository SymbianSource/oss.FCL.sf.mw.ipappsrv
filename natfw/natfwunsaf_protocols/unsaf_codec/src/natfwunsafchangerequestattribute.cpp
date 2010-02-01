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
#include "natfwunsafchangerequestattribute.h"
#include "natfwunsafutils.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFChangeRequestAttribute::NewL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFChangeRequestAttribute*
CNATFWUNSAFChangeRequestAttribute::NewL(TBool aChangeIP, TBool aChangePort)
    {
    CNATFWUNSAFChangeRequestAttribute* self =
        CNATFWUNSAFChangeRequestAttribute::NewLC(aChangeIP, aChangePort);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFChangeRequestAttribute::NewLC
// -----------------------------------------------------------------------------
//
CNATFWUNSAFChangeRequestAttribute*
CNATFWUNSAFChangeRequestAttribute::NewLC(TBool aChangeIP, TBool aChangePort)
    {
    CNATFWUNSAFChangeRequestAttribute* self =
        new (ELeave) CNATFWUNSAFChangeRequestAttribute(aChangeIP, aChangePort);
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFChangeRequestAttribute::DecodeAttributeL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute*
CNATFWUNSAFChangeRequestAttribute::DecodeAttributeL(const TDesC8& aByteStream)
    {
    TUint16 valueLength = ParseLengthL(aByteStream);
    __ASSERT_ALWAYS(valueLength == EAttributeValueSize,
                    User::Leave(KErrCorrupt));
    __ASSERT_ALWAYS(aByteStream.Length() >= EValueOffset + valueLength,
                    User::Leave(KErrArgument));

    TUint32 value = BigEndian::Get32(aByteStream.Mid(EValueOffset).Ptr());

    return NewL((value & EChangeIPMask) > 0, (value & EChangePortMask) > 0);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFChangeRequestAttribute::CNATFWUNSAFChangeRequestAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFChangeRequestAttribute::CNATFWUNSAFChangeRequestAttribute(
    TBool aChangeIP, TBool aChangePort) :
    CNATFWUNSAFAttribute(),
    iChangeIP(aChangeIP),
    iChangePort(aChangePort)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFChangeRequestAttribute::~CNATFWUNSAFChangeRequestAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFChangeRequestAttribute::~CNATFWUNSAFChangeRequestAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFChangeRequestAttribute::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::TType CNATFWUNSAFChangeRequestAttribute::Type() const
    {
    return EChangeRequest;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFChangeRequestAttribute::EncodeValueL
// -----------------------------------------------------------------------------
//
HBufC8* CNATFWUNSAFChangeRequestAttribute::EncodeValueL() const
    {
    HBufC8* encodedValue = HBufC8::NewLC(EAttributeValueSize);

    TPtr8 ptr = encodedValue->Des();
    ptr.FillZ(EAttributeValueSize);

    TUint32 value(0);
    if (ChangeIP())
        {
        value = value | EChangeIPMask;
        }
    if (ChangePort())
        {
        value = value | EChangePortMask;
        }

    NATFWUNSAFUtils::WriteNetworkOrder32L(ptr, 0, value);

    CleanupStack::Pop(encodedValue);
    return encodedValue;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFChangeRequestAttribute::ChangeIP
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFChangeRequestAttribute::ChangeIP() const
    {
    return iChangeIP;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFChangeRequestAttribute::ChangePort
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFChangeRequestAttribute::ChangePort() const
    {
    return iChangePort;
    }
