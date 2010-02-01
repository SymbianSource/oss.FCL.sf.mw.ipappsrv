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




#include "natfwunsafrealmattribute.h"
#include "natfwunsafutils.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFRealmAttribute::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFRealmAttribute* CNATFWUNSAFRealmAttribute::NewL(
    const TDesC8& aRealm)
    {
    CNATFWUNSAFRealmAttribute* self = CNATFWUNSAFRealmAttribute::NewLC(
        aRealm);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRealmAttribute::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFRealmAttribute* CNATFWUNSAFRealmAttribute::NewLC(
    const TDesC8& aRealm)
    {
    CNATFWUNSAFRealmAttribute* self =
        new (ELeave) CNATFWUNSAFRealmAttribute();
    CleanupStack::PushL(self);
    //Validate realm-value
    TPtrC8 checkedRealm = NATFWUNSAFUtils::RealmValueFromQuotedStringL(aRealm);
    self->ConstructL(checkedRealm);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRealmAttribute::DecodeAttributeL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute*
CNATFWUNSAFRealmAttribute::DecodeAttributeL(const TDesC8& aByteStream)
    {
    TUint16 valueLength = ParseLengthL(aByteStream);

    //Stream has to have enough data for the whole value element
    __ASSERT_ALWAYS(aByteStream.Length() >= EValueOffset + valueLength,
                    User::Leave(KErrArgument));
    return NewL(aByteStream.Mid(EValueOffset, valueLength));
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRealmAttribute::CNATFWUNSAFRealmAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFRealmAttribute::CNATFWUNSAFRealmAttribute() :
    CNATFWUNSAFAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRealmAttribute::ConstructL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFRealmAttribute::ConstructL(const TDesC8& aRealm)
    {
    iRealm = aRealm.AllocL();
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRealmAttribute::~CNATFWUNSAFRealmAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFRealmAttribute::~CNATFWUNSAFRealmAttribute()
    {
    delete iRealm;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRealmAttribute::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::TType CNATFWUNSAFRealmAttribute::Type() const
    {
    return ERealm;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFRealmAttribute::EncodeValueL
// -----------------------------------------------------------------------------
//
HBufC8* CNATFWUNSAFRealmAttribute::EncodeValueL() const
    {
    return iRealm->AllocL();
    }
    
// -----------------------------------------------------------------------------
// CNATFWUNSAFRealmAttribute::Value
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC8& CNATFWUNSAFRealmAttribute::Value() const
    {
    return *iRealm;
    }

