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




#include "natfwunsafsourceaddressattribute.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFSourceAddressAttribute::DecodeAttributeL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute* CNATFWUNSAFSourceAddressAttribute::DecodeAttributeL(
    const TDesC8& aByteStream)
    {
    CheckLengthL(aByteStream);
    TInetAddr emptyAddress;
    CNATFWUNSAFSourceAddressAttribute* self =
        CNATFWUNSAFSourceAddressAttribute::NewLC(emptyAddress);
    self->DecodeValueL(aByteStream.Mid(EValueOffset));
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSourceAddressAttribute::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFSourceAddressAttribute*
CNATFWUNSAFSourceAddressAttribute::NewL(const TInetAddr& aAddress)
    {
    CNATFWUNSAFSourceAddressAttribute* self =
        CNATFWUNSAFSourceAddressAttribute::NewLC(aAddress);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSourceAddressAttribute::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFSourceAddressAttribute*
CNATFWUNSAFSourceAddressAttribute::NewLC(const TInetAddr& aAddress)
    {
    CNATFWUNSAFSourceAddressAttribute* self =
        new (ELeave) CNATFWUNSAFSourceAddressAttribute(aAddress);
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSourceAddressAttribute::CNATFWUNSAFSourceAddressAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFSourceAddressAttribute::CNATFWUNSAFSourceAddressAttribute(
    const TInetAddr& aAddress) :
    CNATFWUNSAFAddressAttribute(aAddress)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSourceAddressAttribute::~CNATFWUNSAFSourceAddressAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFSourceAddressAttribute::~CNATFWUNSAFSourceAddressAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFSourceAddressAttribute::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::TType CNATFWUNSAFSourceAddressAttribute::Type() const
    {
    return ESourceAddress;
    }
