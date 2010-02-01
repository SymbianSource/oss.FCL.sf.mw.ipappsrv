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
#include "natfwunsafconnectstatattribute.h"
#include "natfwunsafutils.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectStatAttribute::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFConnectStatAttribute*
CNATFWUNSAFConnectStatAttribute::NewL(TStatus aStatus)
    {
    CNATFWUNSAFConnectStatAttribute* self =
        CNATFWUNSAFConnectStatAttribute::NewLC(aStatus);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectStatAttribute::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFConnectStatAttribute*
CNATFWUNSAFConnectStatAttribute::NewLC(TStatus aStatus)
    {
    CNATFWUNSAFConnectStatAttribute* self =
        new (ELeave) CNATFWUNSAFConnectStatAttribute();
    CleanupStack::PushL(self);
    self->ConstructL(aStatus);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectStatAttribute::DecodeAttributeL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute* CNATFWUNSAFConnectStatAttribute::DecodeAttributeL(
    const TDesC8& aByteStream)
    {
    TUint16 valueLength = ParseLengthL(aByteStream);

    //Stream has to have enough data for the whole value element
    __ASSERT_ALWAYS(valueLength == EAttributeValueSize,
                    User::Leave(KErrCorrupt));
    __ASSERT_ALWAYS(aByteStream.Length() >= EValueOffset + valueLength,
                    User::Leave(KErrArgument));

    TUint32 value = BigEndian::Get32(aByteStream.Mid(EValueOffset).Ptr());
    __ASSERT_ALWAYS(EClosed >= value, User::Leave(KErrArgument));

    return NewL(static_cast<TStatus>(value));
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectStatAttribute::CNATFWUNSAFConnectStatAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFConnectStatAttribute::CNATFWUNSAFConnectStatAttribute() :
    CNATFWUNSAFAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectStatAttribute::ConstructL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFConnectStatAttribute::ConstructL(
    TStatus aStatus)
    {
    __ASSERT_ALWAYS(EClosed >= aStatus, User::Leave(KErrArgument));
    iStatus = aStatus;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectStatAttribute::~CNATFWUNSAFConnectStatAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFConnectStatAttribute::~CNATFWUNSAFConnectStatAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectStatAttribute::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::TType CNATFWUNSAFConnectStatAttribute::Type() const
    {
    return EConnectStat;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectStatAttribute::TransportProtocol
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFConnectStatAttribute::TStatus
    CNATFWUNSAFConnectStatAttribute::ConnectionStatus() const
    {
    return iStatus;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFConnectStatAttribute::EncodeValueL
// -----------------------------------------------------------------------------
//
HBufC8* CNATFWUNSAFConnectStatAttribute::EncodeValueL() const
    {
    HBufC8* encodedValue = HBufC8::NewLC(EAttributeValueSize);

    TPtr8 ptr = encodedValue->Des();
    ptr.FillZ(EAttributeValueSize);

    TUint32 value(iStatus);
    NATFWUNSAFUtils::WriteNetworkOrder32L(ptr, 0, value);

    CleanupStack::Pop(encodedValue);
    return encodedValue;
    }

