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




#include "natfwunsafresponseaddressattribute.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFResponseAddressAttribute::DecodeAttributeL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute* CNATFWUNSAFResponseAddressAttribute::DecodeAttributeL(
    const TDesC8& aByteStream)
    {
    CheckLengthL(aByteStream);
    TInetAddr emptyAddress;
    CNATFWUNSAFResponseAddressAttribute* self =
        CNATFWUNSAFResponseAddressAttribute::NewLC(emptyAddress);
    self->DecodeValueL(aByteStream.Mid(EValueOffset));
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFResponseAddressAttribute::NewL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFResponseAddressAttribute*
CNATFWUNSAFResponseAddressAttribute::NewL(const TInetAddr& aAddress)
    {
    CNATFWUNSAFResponseAddressAttribute* self =
        CNATFWUNSAFResponseAddressAttribute::NewLC(aAddress);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFResponseAddressAttribute::NewLC
// -----------------------------------------------------------------------------
//
CNATFWUNSAFResponseAddressAttribute*
CNATFWUNSAFResponseAddressAttribute::NewLC(const TInetAddr& aAddress)
    {
    CNATFWUNSAFResponseAddressAttribute* self =
        new (ELeave) CNATFWUNSAFResponseAddressAttribute(aAddress);
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFResponseAddressAttribute::CNATFWUNSAFResponseAddressAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFResponseAddressAttribute::CNATFWUNSAFResponseAddressAttribute(
    const TInetAddr& aAddress) :
    CNATFWUNSAFAddressAttribute(aAddress)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFResponseAddressAttribute::~CNATFWUNSAFResponseAddressAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFResponseAddressAttribute::~CNATFWUNSAFResponseAddressAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFResponseAddressAttribute::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::TType CNATFWUNSAFResponseAddressAttribute::Type() const
    {
    return EResponseAddress;
    }
