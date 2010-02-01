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




#include "natfwunsafrelayaddressattribute.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFRelayAddressAttribute::DecodeAttributeL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute* CNATFWUNSAFRelayAddressAttribute::DecodeAttributeL(
    const TDesC8& aByteStream)
    {
    CheckLengthL(aByteStream);
    TInetAddr emptyAddress;
    CNATFWUNSAFRelayAddressAttribute* self =
        CNATFWUNSAFRelayAddressAttribute::NewLC(emptyAddress);
    self->DecodeValueL(aByteStream.Mid(EValueOffset));
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRelayAddressAttribute::NewL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFRelayAddressAttribute*
CNATFWUNSAFRelayAddressAttribute::NewL(const TInetAddr& aAddress)
    {
    CNATFWUNSAFRelayAddressAttribute* self =
        CNATFWUNSAFRelayAddressAttribute::NewLC(aAddress);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRelayAddressAttribute::NewLC
// -----------------------------------------------------------------------------
//
CNATFWUNSAFRelayAddressAttribute*
CNATFWUNSAFRelayAddressAttribute::NewLC(const TInetAddr& aAddress)
    {
    CNATFWUNSAFRelayAddressAttribute* self =
        new (ELeave) CNATFWUNSAFRelayAddressAttribute(aAddress);
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRelayAddressAttribute::CNATFWUNSAFRelayAddressAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFRelayAddressAttribute::CNATFWUNSAFRelayAddressAttribute(
    const TInetAddr& aAddress) :
    CNATFWUNSAFAddressAttribute(aAddress)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRelayAddressAttribute::~CNATFWUNSAFRelayAddressAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFRelayAddressAttribute::~CNATFWUNSAFRelayAddressAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRelalyAddressAttribute::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::TType CNATFWUNSAFRelayAddressAttribute::Type() const
    {
    return ERelayAddress;
    }
