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




#include "natfwunsafserverattribute.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFServerAttribute::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFServerAttribute* CNATFWUNSAFServerAttribute::NewL(
    const TDesC8& aServer)
    {
    CNATFWUNSAFServerAttribute* self = CNATFWUNSAFServerAttribute::NewLC(
        aServer);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFServerAttribute::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFServerAttribute* CNATFWUNSAFServerAttribute::NewLC(
    const TDesC8& aServer)
    {
    CNATFWUNSAFServerAttribute* self =
        new (ELeave) CNATFWUNSAFServerAttribute();
    CleanupStack::PushL(self);
    self->CNATFWUNSAFTextAttribute::ConstructL(aServer);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFServerAttribute::DecodeAttributeL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute*
CNATFWUNSAFServerAttribute::DecodeAttributeL(const TDesC8& aByteStream)
    {
    CNATFWUNSAFServerAttribute* self =
        new (ELeave) CNATFWUNSAFServerAttribute();
    CleanupStack::PushL(self);
    self->DecodeL(aByteStream);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFServerAttribute::CNATFWUNSAFServerAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFServerAttribute::CNATFWUNSAFServerAttribute() :
    CNATFWUNSAFTextAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFServerAttribute::~CNATFWUNSAFServerAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFServerAttribute::~CNATFWUNSAFServerAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFServerAttribute::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::TType CNATFWUNSAFServerAttribute::Type() const
    {
    return EServer;
    }
