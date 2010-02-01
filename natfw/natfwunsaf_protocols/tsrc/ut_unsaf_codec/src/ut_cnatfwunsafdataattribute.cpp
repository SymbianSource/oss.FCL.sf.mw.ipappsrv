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
#include "UT_CNATFWUNSAFDataAttribute.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "natfwunsafdataattribute.h"
#include "natfwunittestmacros.h"

_LIT8(KDataVal,"asdfaj^^lkasdisdan@@@@@*");
_LIT8(KDataInvalidVal,"asdfaj^^lkasdisdan@@*");

// CONSTRUCTION
UT_CNATFWUNSAFDataAttribute* UT_CNATFWUNSAFDataAttribute::NewL()
    {
    UT_CNATFWUNSAFDataAttribute* self = UT_CNATFWUNSAFDataAttribute::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFDataAttribute* UT_CNATFWUNSAFDataAttribute::NewLC()
    {
    UT_CNATFWUNSAFDataAttribute* self = new( ELeave ) UT_CNATFWUNSAFDataAttribute();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFDataAttribute::~UT_CNATFWUNSAFDataAttribute()
    {
    }

// Default constructor
UT_CNATFWUNSAFDataAttribute::UT_CNATFWUNSAFDataAttribute()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFDataAttribute::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CNATFWUNSAFDataAttribute::SetupL()
    {
    const TDesC8& dataDesc(KDataVal);
    iData = dataDesc.AllocL();

    }

void UT_CNATFWUNSAFDataAttribute::Setup2L()
    {
    const TDesC8& dataDesc(KDataInvalidVal);
    iData = dataDesc.AllocL();
    }

void UT_CNATFWUNSAFDataAttribute::Teardown()
    {
    delete iAttribute;
    iAttribute = NULL;
    delete iData;
    iData = NULL;
    }

void UT_CNATFWUNSAFDataAttribute::TestGettersL()
    {
    iAttribute = CNATFWUNSAFDataAttribute::NewL(iData->Des());
    EUNIT_ASSERT(!iData->Des().Compare(iAttribute->Value()));
    EUNIT_ASSERT(CNATFWUNSAFAttribute::EData == iAttribute->Type());
    }

void UT_CNATFWUNSAFDataAttribute::TestEncodeAndDecodeL()
    {
    iAttribute = CNATFWUNSAFDataAttribute::NewL(iData->Des());
    HBufC8* encodedAttr = iAttribute->EncodeL();
    CleanupStack::PushL(encodedAttr);
    CNATFWUNSAFAttribute* clone =
        CNATFWUNSAFDataAttribute::DecodeAttributeL(*encodedAttr);
    CleanupStack::PushL(clone);
    const CNATFWUNSAFDataAttribute* castedClone =
        static_cast<CNATFWUNSAFDataAttribute*>(clone);

    EUNIT_ASSERT(!iAttribute->Value().Compare(castedClone->Value()));

    CleanupStack::PopAndDestroy(clone);
    CleanupStack::PopAndDestroy(encodedAttr);
    }

// Check if padding out succeeded
void UT_CNATFWUNSAFDataAttribute::TestInvalidDataL()
    {
    NATFW_EUNIT_ASSERT_SPECIFIC_LEAVE( iAttribute =
        CNATFWUNSAFDataAttribute::NewL(iData->Des()), KErrNone );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFDataAttribute,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestGettersL - test ",
    "CNATFWUNSAFDataAttribute",
    "TestGettersL",
    "FUNCTIONALITY",
    SetupL, TestGettersL, Teardown)

EUNIT_TEST(
    "TestEncodeAndDecodeL - test ",
    "CNATFWUNSAFDataAttribute",
    "TestEncodeAndDecodeL",
    "FUNCTIONALITY",
    SetupL, TestEncodeAndDecodeL, Teardown)

EUNIT_TEST(
    "TestInvalidDataL - test ",
    "CNATFWUNSAFDataAttribute",
    "TestInvalidDataL",
    "FUNCTIONALITY",
    Setup2L, TestInvalidDataL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
