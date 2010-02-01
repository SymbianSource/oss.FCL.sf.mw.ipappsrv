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
#include "natfwunsafrequestedtransportattribute.h"
#include "natfwunsafutils.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFRequestedTransportAttribute::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFRequestedTransportAttribute*
CNATFWUNSAFRequestedTransportAttribute::NewL(TTransportProtocol aProtocol)
    {
    CNATFWUNSAFRequestedTransportAttribute* self =
        CNATFWUNSAFRequestedTransportAttribute::NewLC(aProtocol);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRequestedTransportAttribute::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFRequestedTransportAttribute*
CNATFWUNSAFRequestedTransportAttribute::NewLC(TTransportProtocol aProtocol)
    {
    CNATFWUNSAFRequestedTransportAttribute* self =
        new (ELeave) CNATFWUNSAFRequestedTransportAttribute();
    CleanupStack::PushL(self);
    self->ConstructL(aProtocol);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRequestedTransportAttribute::DecodeAttributeL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute*
CNATFWUNSAFRequestedTransportAttribute::DecodeAttributeL(
    const TDesC8& aByteStream)
    {
    TUint16 valueLength = ParseLengthL(aByteStream);

    //Stream has to have enough data for the whole value element
    __ASSERT_ALWAYS(valueLength == EAttributeValueSize,
                    User::Leave(KErrCorrupt));
    __ASSERT_ALWAYS(aByteStream.Length() >= EValueOffset + valueLength,
                    User::Leave(KErrArgument));

    TUint32 value = BigEndian::Get32(aByteStream.Mid(EValueOffset).Ptr());
    __ASSERT_ALWAYS(EProtocolUdp == value || EProtocolTcp == value,
        User::Leave(KErrArgument));
        
    return NewL(static_cast<TTransportProtocol>(value));
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRequestedTransportAttribute::
// CNATFWUNSAFRequestedTransportAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFRequestedTransportAttribute::
CNATFWUNSAFRequestedTransportAttribute() :
    CNATFWUNSAFAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRequestedTransportAttribute::ConstructL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFRequestedTransportAttribute::ConstructL(
    TTransportProtocol aProtocol)
    {
     __ASSERT_ALWAYS(EProtocolUdp == aProtocol || EProtocolTcp == aProtocol,
        User::Leave(KErrArgument));
    iProtocol = aProtocol;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRequestedTransportAttribute::
// ~CNATFWUNSAFRequestedTransportAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFRequestedTransportAttribute::
~CNATFWUNSAFRequestedTransportAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRequestedTransportAttribute::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::TType CNATFWUNSAFRequestedTransportAttribute::Type() const
    {
    return ERequestedTransport;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRequestedTransportAttribute::TransportProtocol
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFRequestedTransportAttribute::TTransportProtocol
    CNATFWUNSAFRequestedTransportAttribute::TransportProtocol() const
    {
    return iProtocol;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRequestedTransportAttribute::EncodeValueL
// -----------------------------------------------------------------------------
//
HBufC8* CNATFWUNSAFRequestedTransportAttribute::EncodeValueL() const
    {
    HBufC8* encodedValue = HBufC8::NewLC(EAttributeValueSize);

    TPtr8 ptr = encodedValue->Des();
    ptr.FillZ(EAttributeValueSize);

    TUint32 value(iProtocol);
    NATFWUNSAFUtils::WriteNetworkOrder32L(ptr, 0, value);

    CleanupStack::Pop(encodedValue);
    return encodedValue;
    }
