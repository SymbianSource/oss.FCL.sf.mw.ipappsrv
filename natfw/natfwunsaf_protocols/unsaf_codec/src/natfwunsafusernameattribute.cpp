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




#include "natfwunsafusernameattribute.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFUsernameAttribute::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFUsernameAttribute* CNATFWUNSAFUsernameAttribute::NewL(
    const TDesC8& aUsername)
    {
    CNATFWUNSAFUsernameAttribute* self = CNATFWUNSAFUsernameAttribute::NewLC(
        aUsername);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFUsernameAttribute::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFUsernameAttribute* CNATFWUNSAFUsernameAttribute::NewLC(
    const TDesC8& aUsername)
    {
    CNATFWUNSAFUsernameAttribute* self =
        new (ELeave) CNATFWUNSAFUsernameAttribute();
    CleanupStack::PushL(self);
    self->CNATFWUNSAFTextAttribute::ConstructL(aUsername);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFUsernameAttribute::DecodeAttributeL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute*
CNATFWUNSAFUsernameAttribute::DecodeAttributeL(const TDesC8& aByteStream)
    {
    CNATFWUNSAFUsernameAttribute* self =
        new (ELeave) CNATFWUNSAFUsernameAttribute();
    CleanupStack::PushL(self);
    self->DecodeL(aByteStream);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFUsernameAttribute::CNATFWUNSAFUsernameAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFUsernameAttribute::CNATFWUNSAFUsernameAttribute() :
    CNATFWUNSAFTextAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFUsernameAttribute::~CNATFWUNSAFUsernameAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFUsernameAttribute::~CNATFWUNSAFUsernameAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFUsernameAttribute::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::TType CNATFWUNSAFUsernameAttribute::Type() const
    {
    return EUsername;
    }
