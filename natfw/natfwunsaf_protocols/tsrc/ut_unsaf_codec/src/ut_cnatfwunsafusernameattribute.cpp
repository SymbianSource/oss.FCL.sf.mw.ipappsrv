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
#include "ut_cnatfwunsafusernameattribute.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "natfwunsafusernameattribute.h"

_LIT8(KUsername, "username");
_LIT8(KUsername2, "username2");

// CONSTRUCTION
UT_CNATFWUNSAFUsernameAttribute* UT_CNATFWUNSAFUsernameAttribute::NewL()
    {
    UT_CNATFWUNSAFUsernameAttribute* self = UT_CNATFWUNSAFUsernameAttribute::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFUsernameAttribute* UT_CNATFWUNSAFUsernameAttribute::NewLC()
    {
    UT_CNATFWUNSAFUsernameAttribute* self = new( ELeave ) UT_CNATFWUNSAFUsernameAttribute();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFUsernameAttribute::~UT_CNATFWUNSAFUsernameAttribute()
    {
    }

// Default constructor
UT_CNATFWUNSAFUsernameAttribute::UT_CNATFWUNSAFUsernameAttribute()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFUsernameAttribute::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CNATFWUNSAFUsernameAttribute::SetupL ()
    {
    iAttribute = CNATFWUNSAFUsernameAttribute::NewL(KUsername);
    iAttribute2 = CNATFWUNSAFUsernameAttribute::NewL(KUsername2);
    }


void UT_CNATFWUNSAFUsernameAttribute::Teardown()
    {
    delete iAttribute;
    iAttribute = NULL;
    delete iAttribute2;
    iAttribute2 = NULL;
    }


void UT_CNATFWUNSAFUsernameAttribute::TestEncodeAndDecodeL()
    {
    HBufC8* encodedAttr = iAttribute->EncodeL();
    CleanupStack::PushL(encodedAttr);
    CNATFWUNSAFAttribute* clone =
        CNATFWUNSAFUsernameAttribute::DecodeAttributeL(*encodedAttr);
    CleanupStack::PushL(clone);
    const CNATFWUNSAFUsernameAttribute* castedClone =
        static_cast<CNATFWUNSAFUsernameAttribute*>(clone);

    EUNIT_ASSERT(!iAttribute->Value().CompareF(castedClone->Value()));

    CleanupStack::PopAndDestroy(clone);
    CleanupStack::PopAndDestroy(encodedAttr);

    // Non-word-aligned parameter to the constructor > padding needed
    encodedAttr = iAttribute2->EncodeL();
    CleanupStack::PushL(encodedAttr);
    clone =
        CNATFWUNSAFUsernameAttribute::DecodeAttributeL(*encodedAttr);
    CleanupStack::PushL(clone);
    castedClone =
        static_cast<CNATFWUNSAFUsernameAttribute*>(clone);

    EUNIT_ASSERT(!iAttribute2->Value().CompareF(castedClone->Value()));

    CleanupStack::PopAndDestroy(clone);
    CleanupStack::PopAndDestroy(encodedAttr);
    }


void UT_CNATFWUNSAFUsernameAttribute::TestTypeL()
    {
    EUNIT_ASSERT(CNATFWUNSAFAttribute::EUsername == iAttribute->Type());
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFUsernameAttribute,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestEncodeAndDecodeL - test ",
    "CNATFWUNSAFUsernameAttribute",
    "TestEncodeAndDecodeL",
    "FUNCTIONALITY",
    SetupL, TestEncodeAndDecodeL, Teardown)

EUNIT_TEST(
    "TestTypeL - test ",
    "CNATFWUNSAFUsernameAttribute",
    "TestTypeL",
    "FUNCTIONALITY",
    SetupL, TestTypeL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
