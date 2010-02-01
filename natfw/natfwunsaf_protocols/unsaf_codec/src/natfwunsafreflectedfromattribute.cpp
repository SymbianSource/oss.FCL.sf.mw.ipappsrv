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




#include "natfwunsafreflectedfromattribute.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFReflectedFromAttribute::DecodeAttributeL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute* CNATFWUNSAFReflectedFromAttribute::DecodeAttributeL(
    const TDesC8& aByteStream)
    {
    CheckLengthL(aByteStream);
    TInetAddr emptyAddress;
    CNATFWUNSAFReflectedFromAttribute* self =
        CNATFWUNSAFReflectedFromAttribute::NewLC(emptyAddress);
    self->DecodeValueL(aByteStream.Mid(EValueOffset));
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFReflectedFromAttribute::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFReflectedFromAttribute*
CNATFWUNSAFReflectedFromAttribute::NewL(const TInetAddr& aAddress)
    {
    CNATFWUNSAFReflectedFromAttribute* self =
        CNATFWUNSAFReflectedFromAttribute::NewLC(aAddress);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFReflectedFromAttribute::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFReflectedFromAttribute*
CNATFWUNSAFReflectedFromAttribute::NewLC(const TInetAddr& aAddress)
    {
    CNATFWUNSAFReflectedFromAttribute* self =
        new (ELeave) CNATFWUNSAFReflectedFromAttribute(aAddress);
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFReflectedFromAttribute::CNATFWUNSAFReflectedFromAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFReflectedFromAttribute::CNATFWUNSAFReflectedFromAttribute(
    const TInetAddr& aAddress) :
    CNATFWUNSAFAddressAttribute(aAddress)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFReflectedFromAttribute::~CNATFWUNSAFReflectedFromAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFReflectedFromAttribute::~CNATFWUNSAFReflectedFromAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFReflectedFromAttribute::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::TType CNATFWUNSAFReflectedFromAttribute::Type() const
    {
    return EReflectedFrom;
    }
