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
#include "natfwunsafusecandidateattribute.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFUseCandidateAttribute::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFUseCandidateAttribute*
CNATFWUNSAFUseCandidateAttribute::NewL()
    {
    CNATFWUNSAFUseCandidateAttribute* self =
        CNATFWUNSAFUseCandidateAttribute::NewLC();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFUseCandidateAttribute::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFUseCandidateAttribute*
CNATFWUNSAFUseCandidateAttribute::NewLC()
    {
    CNATFWUNSAFUseCandidateAttribute* self =
        new (ELeave) CNATFWUNSAFUseCandidateAttribute();
    CleanupStack::PushL(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFUseCandidateAttribute::DecodeAttributeL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute*
CNATFWUNSAFUseCandidateAttribute::DecodeAttributeL(const TDesC8& aByteStream)
    {
    TUint16 valueLength = ParseLengthL(aByteStream);

    //Stream has to have no data (length=0)
    __ASSERT_ALWAYS(valueLength == EAttributeValueSize,
                    User::Leave(KErrCorrupt));
    __ASSERT_ALWAYS(aByteStream.Length() >= EValueOffset + valueLength,
                    User::Leave(KErrArgument));
    return NewL();
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFUseCandidateAttribute::CNATFWUNSAFUseCandidateAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFUseCandidateAttribute::CNATFWUNSAFUseCandidateAttribute() :
    CNATFWUNSAFAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFUseCandidateAttribute::~CNATFWUNSAFUseCandidateAttribute
// -----------------------------------------------------------------------------
//
CNATFWUNSAFUseCandidateAttribute::~CNATFWUNSAFUseCandidateAttribute()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFUseCandidateAttribute::Type
// -----------------------------------------------------------------------------
//
CNATFWUNSAFAttribute::TType CNATFWUNSAFUseCandidateAttribute::Type() const
    {
    return EUseCandidate;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFUseCandidateAttribute::EncodeValueL
// -----------------------------------------------------------------------------
//
HBufC8* CNATFWUNSAFUseCandidateAttribute::EncodeValueL() const
    {
    //An empty buffer is returned since this attribute contains no data.
    return HBufC8::NewL(EAttributeValueSize);
    }
