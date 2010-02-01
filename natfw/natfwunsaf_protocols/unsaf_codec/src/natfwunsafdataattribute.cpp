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




#include "natfwunsafdataattribute.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFDataAttribute::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFDataAttribute* CNATFWUNSAFDataAttribute::NewL(
    const TDesC8& aData)
    {
    CNATFWUNSAFDataAttribute* self = CNATFWUNSAFDataAttribute::NewLC(
        aData);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFDataAttribute::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFDataAttribute* CNATFWUNSAFDataAttribute::NewLC(
    const TDesC8& aData)
    {
    CNATFWUNSAFDataAttribute* self =
        new (ELeave) CNATFWUNSAFDataAttribute();
    CleanupStack::PushL(self);
    self->CNATFWUNSAFTextAttribute::ConstructL(aData);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFDataAttribute::DecodeAttributeL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute*
CNATFWUNSAFDataAttribute::DecodeAttributeL(const TDesC8& aByteStream)
    {
    CNATFWUNSAFDataAttribute* self =
        new (ELeave) CNATFWUNSAFDataAttribute();
    CleanupStack::PushL(self);
    self->DecodeL(aByteStream);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFDataAttribute::CNATFWUNSAFDataAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFDataAttribute::CNATFWUNSAFDataAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFDataAttribute::~CNATFWUNSAFDataAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFDataAttribute::~CNATFWUNSAFDataAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFDataAttribute::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::TType CNATFWUNSAFDataAttribute::Type() const
    {
    return EData;
    }
