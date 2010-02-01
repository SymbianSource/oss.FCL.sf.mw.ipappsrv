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




#include "natfwunsafmappedaddressattribute.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFMappedAddressAttribute::DecodeAttributeL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute* CNATFWUNSAFMappedAddressAttribute::DecodeAttributeL(
    const TDesC8& aByteStream)
    {
    CheckLengthL(aByteStream);
    TInetAddr emptyAddress;
    CNATFWUNSAFMappedAddressAttribute* self =
        CNATFWUNSAFMappedAddressAttribute::NewLC(emptyAddress);
    self->DecodeValueL(aByteStream.Mid(EValueOffset));
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMappedAddressAttribute::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFMappedAddressAttribute*
CNATFWUNSAFMappedAddressAttribute::NewL(const TInetAddr& aAddress)
    {
    CNATFWUNSAFMappedAddressAttribute* self =
        CNATFWUNSAFMappedAddressAttribute::NewLC(aAddress);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMappedAddressAttribute::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFMappedAddressAttribute*
CNATFWUNSAFMappedAddressAttribute::NewLC(const TInetAddr& aAddress)
    {
    CNATFWUNSAFMappedAddressAttribute* self =
        new (ELeave) CNATFWUNSAFMappedAddressAttribute(aAddress);
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMappedAddressAttribute::CNATFWUNSAFMappedAddressAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMappedAddressAttribute::CNATFWUNSAFMappedAddressAttribute(
    const TInetAddr& aAddress) :
    CNATFWUNSAFAddressAttribute(aAddress)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMappedAddressAttribute::~CNATFWUNSAFMappedAddressAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMappedAddressAttribute::~CNATFWUNSAFMappedAddressAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMappedAddressAttribute::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::TType CNATFWUNSAFMappedAddressAttribute::Type() const
    {
    return EMappedAddress;
    }
