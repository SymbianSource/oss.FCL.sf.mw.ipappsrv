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




#include "natfwunsafpasswordattribute.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFPasswordAttribute::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFPasswordAttribute* CNATFWUNSAFPasswordAttribute::NewL(
    const TDesC8& aPassword)
    {
    CNATFWUNSAFPasswordAttribute* self = CNATFWUNSAFPasswordAttribute::NewLC(
        aPassword);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFPasswordAttribute::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFPasswordAttribute* CNATFWUNSAFPasswordAttribute::NewLC(
    const TDesC8& aPassword)
    {
    CNATFWUNSAFPasswordAttribute* self =
        new (ELeave) CNATFWUNSAFPasswordAttribute();
    CleanupStack::PushL(self);
    self->CNATFWUNSAFTextAttribute::ConstructL(aPassword);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFPasswordAttribute::DecodeAttributeL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute*
CNATFWUNSAFPasswordAttribute::DecodeAttributeL(const TDesC8& aByteStream)
    {
    CNATFWUNSAFPasswordAttribute* self =
        new (ELeave) CNATFWUNSAFPasswordAttribute();
    CleanupStack::PushL(self);
    self->DecodeL(aByteStream);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFPasswordAttribute::CNATFWUNSAFPasswordAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFPasswordAttribute::CNATFWUNSAFPasswordAttribute() :
    CNATFWUNSAFTextAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFPasswordAttribute::~CNATFWUNSAFPasswordAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFPasswordAttribute::~CNATFWUNSAFPasswordAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFPasswordAttribute::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::TType CNATFWUNSAFPasswordAttribute::Type() const
    {
    return EPassword;
    }
