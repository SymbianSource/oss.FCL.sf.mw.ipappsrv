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




#include <es_sock.h>
#include "natfwunsafattribute.h"
#include "natfwunsafutils.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFAttribute::CNATFWUNSAFAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::CNATFWUNSAFAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAttribute::~CNATFWUNSAFAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::~CNATFWUNSAFAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAttribute::EncodeL
// -----------------------------------------------------------------------------
//
HBufC8* CNATFWUNSAFAttribute::EncodeL() const
    {
    HBufC8* value = EncodeValueL();
    CleanupStack::PushL(value);
    TInt attrValLength = value->Length();

    //Pad non-DWORD-boundary aligned attributes with spaces if needed. Spaces
    //used instead of \0 in order to not mess up buggy C implementations.
    const TInt KGranularity = 4;
    TInt bytesInLastBlock = attrValLength % KGranularity;
    TInt bytesToAppend = KGranularity - bytesInLastBlock;
    if (0 < bytesInLastBlock && !IsWordBoundaryAligned(Type()))
        {
        CBufBase* valueBuf = CBufFlat::NewL(attrValLength + bytesToAppend);
        CleanupStack::PushL(valueBuf);
        valueBuf->InsertL(0, *value, attrValLength);
        const TChar KSpace(' ');
        for (TInt i = 0; i < bytesToAppend; ++i)
            {
            valueBuf->InsertL(valueBuf->Size(), &KSpace, 1);
            }
        // Store value pointer for proper cleanupstack handling
        HBufC8* oldValue = value;
        value = valueBuf->Ptr(0).AllocL();
        CleanupStack::PopAndDestroy(valueBuf);
        CleanupStack::PopAndDestroy( oldValue );
        CleanupStack::PushL( value );
        }

    HBufC8* attribute = HBufC8::NewLC(value->Length() + EValueOffset);
    TPtr8 ptr = attribute->Des();
    ptr.FillZ(EValueOffset);

    NATFWUNSAFUtils::WriteNetworkOrder16L(ptr, ETypeOffset, Type());
    NATFWUNSAFUtils::WriteNetworkOrder16L(ptr,
                                     ELengthOffset,
                                     static_cast<TUint16>(attrValLength));
    ptr.Append(*value);
    CleanupStack::Pop(attribute);
    CleanupStack::PopAndDestroy(value);

    return attribute;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAttribute::ParseLengthL
// -----------------------------------------------------------------------------
//
TUint16 CNATFWUNSAFAttribute::ParseLengthL(const TDesC8& aByteStream)
    {
    __ASSERT_ALWAYS(aByteStream.Length() >= EValueOffset,
                    User::Leave(KErrArgument));

    return BigEndian::Get16(aByteStream.Mid(ELengthOffset).Ptr());
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAttribute::ParseTypeAndLengthL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFAttribute::ParseTypeAndLengthL(const TDesC8& aByteStream,
                                           TUint16& aType,
                                           TUint16& aTotalLength)
    {
    //Stream must have enough data to contain at least an attribute with
    //zero-length value
    __ASSERT_ALWAYS(aByteStream.Length() >= EValueOffset,
                    User::Leave(KErrArgument));

    TUint16 totalLength = ParseLengthL(aByteStream) + EValueOffset;

    //Increase total length if it's not a multiple of 4 bytes.
    //This is because of possible padding in text attributes not visible from
    //attribute length field.
    const TUint16 KGranularity = 4;
    TUint16 bytesInLastBlock = totalLength % KGranularity;
    TUint16 bytesPadded = KGranularity - bytesInLastBlock;
    if (0 < bytesInLastBlock )
        {
        totalLength += bytesPadded;
        }

    __ASSERT_ALWAYS(aByteStream.Length() >= totalLength,
                    User::Leave(KErrArgument));

    aTotalLength = totalLength;
    aType = BigEndian::Get16(aByteStream.Mid(ETypeOffset).Ptr());
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAttribute::IsMandatory
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFAttribute::IsMandatory(TUint16 aType)
    {
    const TInt KLargestMandatoryValue = 0x7fff; //RFC 3489
    return aType <= KLargestMandatoryValue;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFAttribute::IsWordBoundaryAligned
// -----------------------------------------------------------------------------
//
TBool CNATFWUNSAFAttribute::IsWordBoundaryAligned(TUint16 aType)
    {
    return !(aType == EUsername ||
             aType == EPassword ||
             aType == EData     ||
             aType == EServer   ||
             aType == ENonce    ||
             aType == ERealm    ||
             aType == EErrorCode);
    }
