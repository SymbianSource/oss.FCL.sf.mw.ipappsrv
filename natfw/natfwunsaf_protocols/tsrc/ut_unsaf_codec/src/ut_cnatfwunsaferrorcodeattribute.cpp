/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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




//  CLASS HEADER
#include "UT_CNATFWUNSAFErrorCodeAttribute.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "natfwunsaferrorcodeattribute.h"

const TInt KResponseCode = 100;
_LIT8(KReasonPhrase, "reason");

// CONSTRUCTION
UT_CNATFWUNSAFErrorCodeAttribute* UT_CNATFWUNSAFErrorCodeAttribute::NewL()
    {
    UT_CNATFWUNSAFErrorCodeAttribute* self = UT_CNATFWUNSAFErrorCodeAttribute::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFErrorCodeAttribute* UT_CNATFWUNSAFErrorCodeAttribute::NewLC()
    {
    UT_CNATFWUNSAFErrorCodeAttribute* self = new( ELeave ) UT_CNATFWUNSAFErrorCodeAttribute();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFErrorCodeAttribute::~UT_CNATFWUNSAFErrorCodeAttribute()
    {
    }

// Default constructor
UT_CNATFWUNSAFErrorCodeAttribute::UT_CNATFWUNSAFErrorCodeAttribute()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFErrorCodeAttribute::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }


//  METHODS



void UT_CNATFWUNSAFErrorCodeAttribute::SetupL ()
    {
    iAttribute = CNATFWUNSAFErrorCodeAttribute::NewL(KResponseCode,KReasonPhrase);
    }


void UT_CNATFWUNSAFErrorCodeAttribute::Teardown()
    {
    delete iAttribute;
    iAttribute = NULL;
    }


void UT_CNATFWUNSAFErrorCodeAttribute::TestGettersL()
    {
    EUNIT_ASSERT(KResponseCode == iAttribute->ResponseCode());
    EUNIT_ASSERT(!iAttribute->ReasonPhrase().CompareF(KReasonPhrase));
    EUNIT_ASSERT(CNATFWUNSAFAttribute::EErrorCode == iAttribute->Type());
    }


void UT_CNATFWUNSAFErrorCodeAttribute::TestEncodeAndDecodeL()
    {
    HBufC8* encodedAttr = iAttribute->EncodeL();
    CleanupStack::PushL(encodedAttr);
    CNATFWUNSAFAttribute* clone =
        CNATFWUNSAFErrorCodeAttribute::DecodeAttributeL(*encodedAttr);
    CleanupStack::PushL(clone);
    const CNATFWUNSAFErrorCodeAttribute* castedClone =
        static_cast<CNATFWUNSAFErrorCodeAttribute*>(clone);

    EUNIT_ASSERT(iAttribute->ResponseCode() == castedClone->ResponseCode());
    EUNIT_ASSERT(!iAttribute->ReasonPhrase().CompareF(
        castedClone->ReasonPhrase()));

    CleanupStack::PopAndDestroy(clone);
    CleanupStack::PopAndDestroy(encodedAttr);

    // Test with empty reason phrase
    delete iAttribute;
    iAttribute = NULL;
    iAttribute = CNATFWUNSAFErrorCodeAttribute::NewL(KResponseCode,KNullDesC8);
    encodedAttr = iAttribute->EncodeL();
    CleanupStack::PushL(encodedAttr);
    clone = CNATFWUNSAFErrorCodeAttribute::DecodeAttributeL(*encodedAttr);
    CleanupStack::PushL(clone);
    castedClone = static_cast<CNATFWUNSAFErrorCodeAttribute*>(clone);

    EUNIT_ASSERT(iAttribute->ResponseCode() == castedClone->ResponseCode());
    EUNIT_ASSERT(!iAttribute->ReasonPhrase().CompareF(
        castedClone->ReasonPhrase()));

    CleanupStack::PopAndDestroy(clone);
    CleanupStack::PopAndDestroy(encodedAttr);
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFErrorCodeAttribute,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestGettersL - test ",
    "CNATFWUNSAFErrorCodeAttribute",
    "TestGettersL",
    "FUNCTIONALITY",
    SetupL, TestGettersL, Teardown)

EUNIT_TEST(
    "TestEncodeAndDecodeL - test ",
    "CNATFWUNSAFErrorCodeAttribute",
    "TestEncodeAndDecodeL",
    "FUNCTIONALITY",
    SetupL, TestEncodeAndDecodeL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
