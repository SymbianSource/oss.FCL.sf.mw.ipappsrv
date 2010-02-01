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




#include "natfwunsafalternateserverattribute.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFAlternateServerAttribute::DecodeAttributeL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute* CNATFWUNSAFAlternateServerAttribute::DecodeAttributeL(
    const TDesC8& aByteStream)
    {
    CheckLengthL(aByteStream);
    TInetAddr emptyAddress;
    CNATFWUNSAFAlternateServerAttribute* self =
        CNATFWUNSAFAlternateServerAttribute::NewLC(emptyAddress);
    self->DecodeValueL(aByteStream.Mid(EValueOffset));
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAlternateServerAttribute::NewL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAlternateServerAttribute*
CNATFWUNSAFAlternateServerAttribute::NewL(const TInetAddr& aAddress)
    {
    CNATFWUNSAFAlternateServerAttribute* self =
        CNATFWUNSAFAlternateServerAttribute::NewLC(aAddress);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAlternateServerAttribute::NewLC
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAlternateServerAttribute*
CNATFWUNSAFAlternateServerAttribute::NewLC(const TInetAddr& aAddress)
    {
    CNATFWUNSAFAlternateServerAttribute* self =
        new (ELeave) CNATFWUNSAFAlternateServerAttribute(aAddress);
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAlternateServerAttribute::CNATFWUNSAFAlternateServerAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAlternateServerAttribute::CNATFWUNSAFAlternateServerAttribute(
    const TInetAddr& aAddress) :
    CNATFWUNSAFAddressAttribute(aAddress)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAlternateServerAttribute::~CNATFWUNSAFAlternateServerAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAlternateServerAttribute::~CNATFWUNSAFAlternateServerAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAlternateServerAttribute::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::TType CNATFWUNSAFAlternateServerAttribute::Type() const
    {
    return EAlternateServer;
    }
