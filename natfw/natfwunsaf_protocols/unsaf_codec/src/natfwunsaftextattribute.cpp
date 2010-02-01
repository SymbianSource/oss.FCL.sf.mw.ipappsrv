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




#include "natfwunsaftextattribute.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFTextAttribute::CNATFWUNSAFTextAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFTextAttribute::CNATFWUNSAFTextAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTextAttribute::ConstructL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTextAttribute::ConstructL(const TDesC8& aValue)
    {
    iValue = aValue.AllocL();
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTextAttribute::~CNATFWUNSAFTextAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFTextAttribute::~CNATFWUNSAFTextAttribute()
    {
    delete iValue;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTextAttribute::EncodeValueL
// -----------------------------------------------------------------------------
//
HBufC8* CNATFWUNSAFTextAttribute::EncodeValueL() const
    {
    __TEST_INVARIANT;

    return iValue->AllocL();
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTextAttribute::Value
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC8& CNATFWUNSAFTextAttribute::Value() const
    {
    __TEST_INVARIANT;

    return *iValue;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTextAttribute::DecodeL
// ConstructL will check the value part's length is valid
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTextAttribute::DecodeL(const TDesC8& aByteStream)
    {
    TUint16 valueLength = ParseLengthL(aByteStream);

    //Stream has to have enough data for the whole value element
    __ASSERT_ALWAYS(aByteStream.Length() >= EValueOffset + valueLength,
                    User::Leave(KErrArgument));
    ConstructL(aByteStream.Mid(EValueOffset, valueLength));
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFTextAttribute::__DbgTestInvariant
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFTextAttribute::__DbgTestInvariant() const
    {
#if defined(_DEBUG)
    if (!iValue)
        {
        User::Invariant();
        }
#endif
    }
