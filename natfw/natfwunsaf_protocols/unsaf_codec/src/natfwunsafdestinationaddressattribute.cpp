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




#include "natfwunsafdestinationaddressattribute.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFDestinationAddressAttribute::DecodeAttributeL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute* CNATFWUNSAFDestinationAddressAttribute::DecodeAttributeL(
    const TDesC8& aByteStream)
    {
    CheckLengthL(aByteStream);
    TInetAddr emptyAddress;
    CNATFWUNSAFDestinationAddressAttribute* self =
        CNATFWUNSAFDestinationAddressAttribute::NewLC(emptyAddress);
    self->DecodeValueL(aByteStream.Mid(EValueOffset));
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFDestinationAddressAttribute::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFDestinationAddressAttribute*
CNATFWUNSAFDestinationAddressAttribute::NewL(const TInetAddr& aAddress)
    {
    CNATFWUNSAFDestinationAddressAttribute* self =
        CNATFWUNSAFDestinationAddressAttribute::NewLC(aAddress);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFDestinationAddressAttribute::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFDestinationAddressAttribute*
CNATFWUNSAFDestinationAddressAttribute::NewLC(const TInetAddr& aAddress)
    {
    CNATFWUNSAFDestinationAddressAttribute* self =
        new (ELeave) CNATFWUNSAFDestinationAddressAttribute(aAddress);
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFDestinationAddressAttribute::
// CNATFWUNSAFDestinationAddressAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFDestinationAddressAttribute::CNATFWUNSAFDestinationAddressAttribute(
    const TInetAddr& aAddress) :
    CNATFWUNSAFAddressAttribute(aAddress)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFDestinationAddressAttribute::
// ~CNATFWUNSAFDestinationAddressAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFDestinationAddressAttribute::
~CNATFWUNSAFDestinationAddressAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFDestinationAddressAttribute::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::TType
CNATFWUNSAFDestinationAddressAttribute::Type() const
    {
    return EDestinationAddress;
    }
