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




#include "natfwunsafchangedaddressattribute.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFChangedAddressAttribute::DecodeAttributeL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute* CNATFWUNSAFChangedAddressAttribute::DecodeAttributeL(
    const TDesC8& aByteStream)
    {
    CheckLengthL(aByteStream);
    TInetAddr emptyAddress;
    CNATFWUNSAFChangedAddressAttribute* self =
        CNATFWUNSAFChangedAddressAttribute::NewLC(emptyAddress);
    self->DecodeValueL(aByteStream.Mid(EValueOffset));
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFChangedAddressAttribute::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFChangedAddressAttribute*
CNATFWUNSAFChangedAddressAttribute::NewL(const TInetAddr& aAddress)
    {
    CNATFWUNSAFChangedAddressAttribute* self =
        CNATFWUNSAFChangedAddressAttribute::NewLC(aAddress);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFChangedAddressAttribute::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFChangedAddressAttribute*
CNATFWUNSAFChangedAddressAttribute::NewLC(const TInetAddr& aAddress)
    {
    CNATFWUNSAFChangedAddressAttribute* self =
        new (ELeave) CNATFWUNSAFChangedAddressAttribute(aAddress);
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFChangedAddressAttribute::CNATFWUNSAFChangedAddressAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFChangedAddressAttribute::CNATFWUNSAFChangedAddressAttribute(
    const TInetAddr& aAddress) :
    CNATFWUNSAFAddressAttribute(aAddress)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFChangedAddressAttribute::~CNATFWUNSAFChangedAddressAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFChangedAddressAttribute::~CNATFWUNSAFChangedAddressAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFChangedAddressAttribute::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::TType CNATFWUNSAFChangedAddressAttribute::Type() const
    {
    return EChangedAddress;
    }
