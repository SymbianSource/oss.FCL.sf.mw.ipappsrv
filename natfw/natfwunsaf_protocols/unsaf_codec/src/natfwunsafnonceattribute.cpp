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




#include "natfwunsafnonceattribute.h"
#include "natfwunsafutils.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFNonceAttribute::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFNonceAttribute* CNATFWUNSAFNonceAttribute::NewL(
    const TDesC8& aNonce)
    {
    CNATFWUNSAFNonceAttribute* self = CNATFWUNSAFNonceAttribute::NewLC(
        aNonce);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFNonceAttribute::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFNonceAttribute* CNATFWUNSAFNonceAttribute::NewLC(
    const TDesC8& aNonce)
    {
    CNATFWUNSAFNonceAttribute* self =
        new (ELeave) CNATFWUNSAFNonceAttribute();
    CleanupStack::PushL(self);
    TBool valid = ETrue;
    for ( TInt i = 0; i < aNonce.Length(); i++ )
        {
        valid = NATFWUNSAFUtils::IsQdTextChar(aNonce[i]);
        if ( !valid )
            {
            break;
            }
        }
    if (!valid)
        {
        if(!NATFWUNSAFUtils::IsQuotedString(aNonce))
            {
            User::Leave(KErrArgument);
            }
        }
    self->ConstructL(aNonce);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFNonceAttribute::DecodeAttributeL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute*
CNATFWUNSAFNonceAttribute::DecodeAttributeL(const TDesC8& aByteStream)
    {
    TUint16 valueLength = ParseLengthL(aByteStream);

    //Stream has to have enough data for the whole value element
    __ASSERT_ALWAYS(aByteStream.Length() >= EValueOffset + valueLength,
                    User::Leave(KErrArgument));
    return NewL(aByteStream.Mid(EValueOffset, valueLength));
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFNonceAttribute::CNATFWUNSAFNonceAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFNonceAttribute::CNATFWUNSAFNonceAttribute() :
    CNATFWUNSAFAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFNonceAttribute::ConstructL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFNonceAttribute::ConstructL(const TDesC8& aNonce)
    {
    iNonce = aNonce.AllocL();
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFNonceAttribute::~CNATFWUNSAFNonceAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFNonceAttribute::~CNATFWUNSAFNonceAttribute()
    {
    delete iNonce;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFNonceAttribute::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::TType CNATFWUNSAFNonceAttribute::Type() const
    {
    return ENonce;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFNonceAttribute::EncodeValueL
// -----------------------------------------------------------------------------
//
HBufC8* CNATFWUNSAFNonceAttribute::EncodeValueL() const
    {
    return iNonce->AllocL();
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFNonceAttribute::Value
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC8& CNATFWUNSAFNonceAttribute::Value() const
    {
    return *iNonce;
    }

