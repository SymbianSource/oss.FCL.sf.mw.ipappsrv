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




#include "natfwunsafrequestedipattribute.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFRequestedIpAttribute::DecodeAttributeL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute* CNATFWUNSAFRequestedIpAttribute::DecodeAttributeL(
    const TDesC8& aByteStream)
    {
    CheckLengthL(aByteStream);
    TInetAddr emptyAddress;
    CNATFWUNSAFRequestedIpAttribute* self =
        CNATFWUNSAFRequestedIpAttribute::NewLC(emptyAddress);
    self->DecodeValueL(aByteStream.Mid(EValueOffset));
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRequestedIpAttribute::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFRequestedIpAttribute*
CNATFWUNSAFRequestedIpAttribute::NewL(const TInetAddr& aAddress)
    {
    CNATFWUNSAFRequestedIpAttribute* self =
        CNATFWUNSAFRequestedIpAttribute::NewLC(aAddress);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRequestedIpAttribute::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFRequestedIpAttribute*
CNATFWUNSAFRequestedIpAttribute::NewLC(const TInetAddr& aAddress)
    {
    CNATFWUNSAFRequestedIpAttribute* self =
        new (ELeave) CNATFWUNSAFRequestedIpAttribute(aAddress);
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRequestedIpAttribute::CNATFWUNSAFRequestedIpAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFRequestedIpAttribute::CNATFWUNSAFRequestedIpAttribute(
    const TInetAddr& aAddress) :
    CNATFWUNSAFAddressAttribute(aAddress)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRequestedIpAttribute::~CNATFWUNSAFRequestedIpAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFRequestedIpAttribute::~CNATFWUNSAFRequestedIpAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRelalyAddressAttribute::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::TType CNATFWUNSAFRequestedIpAttribute::Type() const
    {
    return ERequestedIp;
    }
