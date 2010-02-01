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
#include "UT_CNATFWUNSAFPasswordAttribute.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>

//  INTERNAL INCLUDES
#include "natfwunsafpasswordattribute.h"

_LIT8(KPassword, "passwordXX");

// CONSTRUCTION
UT_CNATFWUNSAFPasswordAttribute* UT_CNATFWUNSAFPasswordAttribute::NewL()
    {
    UT_CNATFWUNSAFPasswordAttribute* self = UT_CNATFWUNSAFPasswordAttribute::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFPasswordAttribute* UT_CNATFWUNSAFPasswordAttribute::NewLC()
    {
    UT_CNATFWUNSAFPasswordAttribute* self = new( ELeave ) UT_CNATFWUNSAFPasswordAttribute();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFPasswordAttribute::~UT_CNATFWUNSAFPasswordAttribute()
    {
    }

// Default constructor
UT_CNATFWUNSAFPasswordAttribute::UT_CNATFWUNSAFPasswordAttribute()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFPasswordAttribute::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CNATFWUNSAFPasswordAttribute::SetupL ()
    {
    iAttribute = CNATFWUNSAFPasswordAttribute::NewL(KPassword);
    }


void UT_CNATFWUNSAFPasswordAttribute::Teardown()
    {
    delete iAttribute;
    iAttribute = NULL;
    }


void UT_CNATFWUNSAFPasswordAttribute::TestEncodeAndDecodeL()
    {
    HBufC8* encodedAttr = iAttribute->EncodeL();
    CleanupStack::PushL(encodedAttr);
    CNATFWUNSAFAttribute* clone =
        CNATFWUNSAFPasswordAttribute::DecodeAttributeL(*encodedAttr);
    CleanupStack::PushL(clone);
    const CNATFWUNSAFPasswordAttribute* castedClone =
        static_cast<CNATFWUNSAFPasswordAttribute*>(clone);

    EUNIT_ASSERT(!iAttribute->Value().CompareF(castedClone->Value()));

    CleanupStack::PopAndDestroy(clone);
    CleanupStack::PopAndDestroy(encodedAttr);
    }


void UT_CNATFWUNSAFPasswordAttribute::TestTypeL()
    {
    EUNIT_ASSERT(CNATFWUNSAFAttribute::EPassword == iAttribute->Type());
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFPasswordAttribute,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestEncodeAndDecodeL - test ",
    "CNATFWUNSAFPasswordAttribute",
    "TestEncodeAndDecodeL",
    "FUNCTIONALITY",
    SetupL, TestEncodeAndDecodeL, Teardown)

EUNIT_TEST(
    "TestTypeL - test ",
    "CNATFWUNSAFPasswordAttribute",
    "TestTypeL",
    "FUNCTIONALITY",
    SetupL, TestTypeL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
