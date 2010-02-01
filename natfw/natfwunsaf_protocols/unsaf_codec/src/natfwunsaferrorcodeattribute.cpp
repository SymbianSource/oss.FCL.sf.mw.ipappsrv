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




#include "natfwunsaferrorcodeattribute.h"

const TInt KMinResponseCode = 100;
const TInt KMaxResponseCode = 699;

const TInt KMinResponseClass = 1;
const TInt KMaxResponseClass = 6;

// -----------------------------------------------------------------------------
// CNATFWUNSAFErrorCodeAttribute::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFErrorCodeAttribute*
CNATFWUNSAFErrorCodeAttribute::NewL(TInt aResponseCode,
    const TDesC8& aReasonPhrase)
    {
    CNATFWUNSAFErrorCodeAttribute* self =
        CNATFWUNSAFErrorCodeAttribute::NewLC(aResponseCode, aReasonPhrase);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFErrorCodeAttribute::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFErrorCodeAttribute*
CNATFWUNSAFErrorCodeAttribute::NewLC(TInt aResponseCode,
    const TDesC8& aReasonPhrase)
    {
    CNATFWUNSAFErrorCodeAttribute* self =
        new (ELeave) CNATFWUNSAFErrorCodeAttribute();
    CleanupStack::PushL(self);
    self->ConstructL(aResponseCode, aReasonPhrase);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFErrorCodeAttribute::DecodeAttributeL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute*
CNATFWUNSAFErrorCodeAttribute::DecodeAttributeL(const TDesC8& aByteStream)
    {
    TUint16 valueLength = ParseLengthL(aByteStream);

    //Stream has to have enough data for the whole value element
    __ASSERT_ALWAYS(aByteStream.Length() >= EValueOffset + valueLength,
                    User::Leave(KErrArgument));

    //Check that the length of the whole attribute is DWORD aligned
    //Attribute value length (prior to padding) found in header no longer needs
    //to be aligned
    CheckLengthL(aByteStream.Length());

    //Check the value length is valid for this attribute
    __ASSERT_ALWAYS(valueLength >= EReasonPhraseOffset,
                    User::Leave(KErrCorrupt));
    TInt reasonPhraseLength = valueLength - EReasonPhraseOffset;

    TPtrC8 attrValue = aByteStream.Mid(EValueOffset);

    TInt classValue = attrValue[EClassOffset] & EClassMask;
    __ASSERT_ALWAYS(classValue >= KMinResponseClass &&
                    classValue <= KMaxResponseClass,
                    User::Leave(KErrArgument));

    TInt numberValue = attrValue[ENumberOffset];
    __ASSERT_ALWAYS(numberValue >= 0 && numberValue < E100,
                    User::Leave(KErrArgument));

    HBufC8* reasonPhrase =
        attrValue.Mid(EReasonPhraseOffset, reasonPhraseLength).AllocLC();
    reasonPhrase->Des().Trim();

    CNATFWUNSAFAttribute* attr = NewL(classValue*E100 + numberValue,
        *reasonPhrase);

    CleanupStack::PopAndDestroy(reasonPhrase);

    return attr;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFErrorCodeAttribute::CNATFWUNSAFErrorCodeAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFErrorCodeAttribute::CNATFWUNSAFErrorCodeAttribute() :
    CNATFWUNSAFAttribute(),
    iResponseCode(0)
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFErrorCodeAttribute::ConstructL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFErrorCodeAttribute::ConstructL(TInt aResponseCode,
                                          const TDesC8& aReasonPhrase)
    {
    __ASSERT_ALWAYS(CheckResponseCode(aResponseCode),
        User::Leave(KErrArgument));

    iReasonPhrase = aReasonPhrase.AllocL();
    iResponseCode = aResponseCode;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFErrorCodeAttribute::~CNATFWUNSAFErrorCodeAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFErrorCodeAttribute::~CNATFWUNSAFErrorCodeAttribute()
    {
    delete iReasonPhrase;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFErrorCodeAttribute::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::TType CNATFWUNSAFErrorCodeAttribute::Type() const
    {
    __TEST_INVARIANT;

    return EErrorCode;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFErrorCodeAttribute::EncodeValueL
// -----------------------------------------------------------------------------
//
HBufC8* CNATFWUNSAFErrorCodeAttribute::EncodeValueL() const
    {
    __TEST_INVARIANT;

    TInt encodedReasonPhraseLength = EncodedReasonPhraseLength();
    HBufC8* encodedValue =
        HBufC8::NewLC(EReasonPhraseOffset + encodedReasonPhraseLength);
    TPtr8 ptr = encodedValue->Des();
    ptr.FillZ(EReasonPhraseOffset);


    ptr[EClassOffset] = (iResponseCode / E100) & EClassMask;
    ptr[ENumberOffset] = iResponseCode % E100;


    ptr.Append(*iReasonPhrase);

    TInt spacesToAppend = encodedReasonPhraseLength - iReasonPhrase->Length();
    const TChar KSpace(' ');
    for (TInt i = 0; i < spacesToAppend; ++i)
        {
        ptr.Append(KSpace);
        }

    CleanupStack::Pop(encodedValue);
    return encodedValue;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFErrorCodeAttribute::EncodedReasonPhraseLength
// -----------------------------------------------------------------------------
//
TInt CNATFWUNSAFErrorCodeAttribute::EncodedReasonPhraseLength() const
    {
    __TEST_INVARIANT;

    TInt length = iReasonPhrase->Length();
    TInt charsInLastBlock = length % EGranularity;

    if (charsInLastBlock > 0)
        {
        length = length + EGranularity - charsInLastBlock;
        }
    return length;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFErrorCodeAttribute::ResponseCode
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CNATFWUNSAFErrorCodeAttribute::ResponseCode() const
    {
    __TEST_INVARIANT;
    return iResponseCode;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFErrorCodeAttribute::ReasonPhrase
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC8& CNATFWUNSAFErrorCodeAttribute::ReasonPhrase() const
    {
    __TEST_INVARIANT;
    return *iReasonPhrase;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFErrorCodeAttribute::CheckLengthL
// Check the granularity of attribute's length.
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFErrorCodeAttribute::CheckLengthL(TInt aLength)
    {
    __ASSERT_ALWAYS((aLength % EGranularity) == 0, User::Leave(KErrCorrupt));
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFErrorCodeAttribute::CheckResponseCode
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFErrorCodeAttribute::CheckResponseCode(TInt aResponseCode) const
    {
    return (aResponseCode >= KMinResponseCode &&
            aResponseCode <= KMaxResponseCode);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFErrorCodeAttribute::__DbgTestInvariant
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFErrorCodeAttribute::__DbgTestInvariant() const
    {
#if defined(_DEBUG)
    if (!iReasonPhrase || !CheckResponseCode(iResponseCode))
        {
        User::Invariant();
        }
#endif
    }
