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




#include "natfwunsafremoteaddressattribute.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFRemoteAddressAttribute::DecodeAttributeL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute* CNATFWUNSAFRemoteAddressAttribute::DecodeAttributeL(
    const TDesC8& aByteStream)
    {
    CheckLengthL(aByteStream);
    TInetAddr emptyAddress;
    CNATFWUNSAFRemoteAddressAttribute* self =
        CNATFWUNSAFRemoteAddressAttribute::NewLC(emptyAddress);
    self->DecodeValueL(aByteStream.Mid(EValueOffset));
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRemoteAddressAttribute::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFRemoteAddressAttribute*
CNATFWUNSAFRemoteAddressAttribute::NewL(const TInetAddr& aAddress)
    {
    CNATFWUNSAFRemoteAddressAttribute* self =
        CNATFWUNSAFRemoteAddressAttribute::NewLC(aAddress);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRemoteAddressAttribute::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFRemoteAddressAttribute*
CNATFWUNSAFRemoteAddressAttribute::NewLC(const TInetAddr& aAddress)
    {
    CNATFWUNSAFRemoteAddressAttribute* self =
        new (ELeave) CNATFWUNSAFRemoteAddressAttribute(aAddress);
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRemoteAddressAttribute::CNATFWUNSAFRemoteAddressAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFRemoteAddressAttribute::CNATFWUNSAFRemoteAddressAttribute(
    const TInetAddr& aAddress) :
    CNATFWUNSAFAddressAttribute(aAddress)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRemoteAddressAttribute::~CNATFWUNSAFRemoteAddressAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFRemoteAddressAttribute::~CNATFWUNSAFRemoteAddressAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRemoteAddressAttribute::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::TType
CNATFWUNSAFRemoteAddressAttribute::Type() const
    {
    return ERemoteAddress;
    }
