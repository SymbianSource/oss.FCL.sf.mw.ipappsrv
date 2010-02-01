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
#include "UT_CNATFWUNSAFUnknownAttributesAttribute.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "natfwunsafunknownattributesattribute.h"

const TUint16 KFirstAttribute = 123;
const TUint16 KSecondAttribute = 456;


// CONSTRUCTION
UT_CNATFWUNSAFUnknownAttributesAttribute* UT_CNATFWUNSAFUnknownAttributesAttribute::NewL()
    {
    UT_CNATFWUNSAFUnknownAttributesAttribute* self = UT_CNATFWUNSAFUnknownAttributesAttribute::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFUnknownAttributesAttribute* UT_CNATFWUNSAFUnknownAttributesAttribute::NewLC()
    {
    UT_CNATFWUNSAFUnknownAttributesAttribute* self = new( ELeave ) UT_CNATFWUNSAFUnknownAttributesAttribute();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFUnknownAttributesAttribute::~UT_CNATFWUNSAFUnknownAttributesAttribute()
    {
    }

// Default constructor
UT_CNATFWUNSAFUnknownAttributesAttribute::UT_CNATFWUNSAFUnknownAttributesAttribute()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFUnknownAttributesAttribute::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_CNATFWUNSAFUnknownAttributesAttribute::SetupL ()
    {
    iAttribute = CNATFWUNSAFUnknownAttributesAttribute::NewL();
    iAttribute->AddContainedAttributeL(KFirstAttribute);
    }


void UT_CNATFWUNSAFUnknownAttributesAttribute::Teardown()
    {
    delete iAttribute;
    iAttribute = NULL;
    }


void UT_CNATFWUNSAFUnknownAttributesAttribute::TestTypeL()
    {
    EUNIT_ASSERT(CNATFWUNSAFAttribute::EUnknownAttributes == iAttribute->Type());
    }


void UT_CNATFWUNSAFUnknownAttributesAttribute::TestAddAndRemoveAttributesL()
    {
    EUNIT_ASSERT(1 == iAttribute->ContainedAttributes().Count());
    EUNIT_ASSERT(KFirstAttribute == iAttribute->ContainedAttributes()[0]);

    iAttribute->AddContainedAttributeL(KSecondAttribute);
    EUNIT_ASSERT(2 == iAttribute->ContainedAttributes().Count());
    EUNIT_ASSERT(KFirstAttribute == iAttribute->ContainedAttributes()[0]);
    EUNIT_ASSERT(KSecondAttribute == iAttribute->ContainedAttributes()[1]);

    EUNIT_ASSERT(KErrNone == iAttribute->RemoveContainedAttribute(KFirstAttribute));
    EUNIT_ASSERT(1 == iAttribute->ContainedAttributes().Count());
    EUNIT_ASSERT(KSecondAttribute == iAttribute->ContainedAttributes()[0]);
    EUNIT_ASSERT(KErrNotFound == iAttribute->RemoveContainedAttribute(KFirstAttribute));

    EUNIT_ASSERT(KErrNone == iAttribute->RemoveContainedAttribute(KSecondAttribute));
    EUNIT_ASSERT(0 == iAttribute->ContainedAttributes().Count());
    EUNIT_ASSERT(KErrNotFound == iAttribute->RemoveContainedAttribute(KSecondAttribute));
    }


void UT_CNATFWUNSAFUnknownAttributesAttribute::TestEncodeAndDecodeL()
    {
    HBufC8* encodedAttr = iAttribute->EncodeL();
    CleanupStack::PushL(encodedAttr);
    CNATFWUNSAFAttribute* clone =
        CNATFWUNSAFUnknownAttributesAttribute::DecodeAttributeL(*encodedAttr);
    CleanupStack::PushL(clone);
    const CNATFWUNSAFUnknownAttributesAttribute* castedClone =
        static_cast<CNATFWUNSAFUnknownAttributesAttribute*>(clone);

    EUNIT_ASSERT(iAttribute->ContainedAttributes().Count() ==
                      castedClone->ContainedAttributes().Count());
    EUNIT_ASSERT(iAttribute->ContainedAttributes()[0] ==
                      castedClone->ContainedAttributes()[0]);

    CleanupStack::PopAndDestroy(clone);
    CleanupStack::PopAndDestroy(encodedAttr);

    // Encode empty
    EUNIT_ASSERT(KErrNone == iAttribute->RemoveContainedAttribute(KFirstAttribute));
    EUNIT_ASSERT(0 == iAttribute->ContainedAttributes().Count());
    encodedAttr = iAttribute->EncodeL();
    delete encodedAttr;
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFUnknownAttributesAttribute,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestTypeL - test ",
    "CNATFWUNSAFUnknownAttributesAttribute",
    "TestTypeL",
    "FUNCTIONALITY",
    SetupL, TestTypeL, Teardown)

EUNIT_TEST(
    "TestAddAndRemoveAttributesL - test ",
    "CNATFWUNSAFUnknownAttributesAttribute",
    "TestAddAndRemoveAttributesL",
    "FUNCTIONALITY",
    SetupL, TestAddAndRemoveAttributesL, Teardown)

EUNIT_TEST(
    "TestEncodeAndDecodeL - test ",
    "CNATFWUNSAFUnknownAttributesAttribute",
    "TestEncodeAndDecodeL",
    "FUNCTIONALITY",
    SetupL, TestEncodeAndDecodeL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
