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
#include "natfwunsaflifetimeattribute.h"
#include "natfwunsafutils.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFLifetimeAttribute::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFLifetimeAttribute*
CNATFWUNSAFLifetimeAttribute::NewL(TInt aLifetime)
    {
    CNATFWUNSAFLifetimeAttribute* self =
        CNATFWUNSAFLifetimeAttribute::NewLC(aLifetime);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFLifetimeAttribute::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFLifetimeAttribute*
CNATFWUNSAFLifetimeAttribute::NewLC(TInt aLifetime)
    {
    CNATFWUNSAFLifetimeAttribute* self =
        new (ELeave) CNATFWUNSAFLifetimeAttribute();
    CleanupStack::PushL(self);
    self->ConstructL(aLifetime);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFLifetimeAttribute::DecodeAttributeL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute*
CNATFWUNSAFLifetimeAttribute::DecodeAttributeL(const TDesC8& aByteStream)
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
// CNATFWUNSAFLifetimeAttribute::CNATFWUNSAFLifetimeAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFLifetimeAttribute::CNATFWUNSAFLifetimeAttribute() :
    CNATFWUNSAFAttribute(),
    iLifetime(0)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFLifetimeAttribute::ConstructL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFLifetimeAttribute::ConstructL(TInt aLifetime)
    {
    __ASSERT_ALWAYS(0 <= aLifetime, User::Leave(KErrArgument));

    iLifetime = aLifetime;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFLifetimeAttribute::~CNATFWUNSAFLifetimeAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFLifetimeAttribute::~CNATFWUNSAFLifetimeAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFLifetimeAttribute::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::TType CNATFWUNSAFLifetimeAttribute::Type() const
    {
    return ELifetime;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFLifetimeAttribute::EncodeValueL
// -----------------------------------------------------------------------------
//
HBufC8* CNATFWUNSAFLifetimeAttribute::EncodeValueL() const
    {
    HBufC8* encodedValue = HBufC8::NewLC(EAttributeValueSize);

    TPtr8 ptr = encodedValue->Des();
    ptr.FillZ(EAttributeValueSize);

    TUint32 value(iLifetime);
    NATFWUNSAFUtils::WriteNetworkOrder32L(ptr, 0, value);

    CleanupStack::Pop(encodedValue);
    return encodedValue;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFLifetimeAttribute::Lifetime
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CNATFWUNSAFLifetimeAttribute::Lifetime() const
    {
    return iLifetime;
    }
