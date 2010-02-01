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




#include "natfwunsafxoronlyattribute.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFXorOnlyAttribute::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFXorOnlyAttribute* CNATFWUNSAFXorOnlyAttribute::NewL()
    {
    CNATFWUNSAFXorOnlyAttribute* self = CNATFWUNSAFXorOnlyAttribute::NewLC();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFXorOnlyAttribute::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFXorOnlyAttribute* CNATFWUNSAFXorOnlyAttribute::NewLC()
    {
    CNATFWUNSAFXorOnlyAttribute* self =
        new (ELeave) CNATFWUNSAFXorOnlyAttribute();
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFXorOnlyAttribute::DecodeAttributeL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute* CNATFWUNSAFXorOnlyAttribute::DecodeAttributeL(
    const TDesC8& aByteStream)
    {
    TUint16 valueLength = ParseLengthL(aByteStream);

    // XOR-ONLY-attribute's value length is always zero
    __ASSERT_ALWAYS(valueLength == 0, User::Leave(KErrArgument));

    return CNATFWUNSAFXorOnlyAttribute::NewL();
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFXorOnlyAttribute::CNATFWUNSAFXorOnlyAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFXorOnlyAttribute::CNATFWUNSAFXorOnlyAttribute() :
    CNATFWUNSAFAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFXorOnlyAttribute::~CNATFWUNSAFXorOnlyAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFXorOnlyAttribute::~CNATFWUNSAFXorOnlyAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFXorOnlyAttribute::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::TType CNATFWUNSAFXorOnlyAttribute::Type() const
    {
    return EXorOnly;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFXorOnlyAttribute::EncodeValueL
// -----------------------------------------------------------------------------
//
HBufC8* CNATFWUNSAFXorOnlyAttribute::EncodeValueL() const
    {
    return KNullDesC8().AllocL();
    }
