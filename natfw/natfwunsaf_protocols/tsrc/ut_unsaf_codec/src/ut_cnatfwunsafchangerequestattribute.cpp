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
#include "UT_CNATFWUNSAFChangeRequestAttribute.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "natfwunsafchangerequestattribute.h"


// CONSTRUCTION
UT_CNATFWUNSAFChangeRequestAttribute* UT_CNATFWUNSAFChangeRequestAttribute::NewL()
    {
    UT_CNATFWUNSAFChangeRequestAttribute* self = UT_CNATFWUNSAFChangeRequestAttribute::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFChangeRequestAttribute* UT_CNATFWUNSAFChangeRequestAttribute::NewLC()
    {
    UT_CNATFWUNSAFChangeRequestAttribute* self = new( ELeave ) UT_CNATFWUNSAFChangeRequestAttribute();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFChangeRequestAttribute::~UT_CNATFWUNSAFChangeRequestAttribute()
    {
    }

// Default constructor
UT_CNATFWUNSAFChangeRequestAttribute::UT_CNATFWUNSAFChangeRequestAttribute()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFChangeRequestAttribute::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }


//  METHODS

void UT_CNATFWUNSAFChangeRequestAttribute::SetupL ()
    {
    iAttribute = CNATFWUNSAFChangeRequestAttribute::NewL(ETrue,ETrue);
    }


void UT_CNATFWUNSAFChangeRequestAttribute::Teardown()
    {
    delete iAttribute;
    iAttribute = NULL;
    }


void UT_CNATFWUNSAFChangeRequestAttribute::TestEncodeAndDecodeL()
    {
    EUNIT_ASSERT(iAttribute->ChangeIP());
    EUNIT_ASSERT(iAttribute->ChangePort());

    HBufC8* encodedAttr = iAttribute->EncodeL();
    CleanupStack::PushL(encodedAttr);
    CNATFWUNSAFAttribute* clone =
        CNATFWUNSAFChangeRequestAttribute::DecodeAttributeL(*encodedAttr);
    CleanupStack::PushL(clone);
    const CNATFWUNSAFChangeRequestAttribute* castedClone =
        static_cast<CNATFWUNSAFChangeRequestAttribute*>(clone);

    EUNIT_ASSERT(castedClone->ChangeIP() == iAttribute->ChangeIP());
    EUNIT_ASSERT(castedClone->ChangePort() == iAttribute->ChangePort());

    CleanupStack::PopAndDestroy(clone);
    CleanupStack::PopAndDestroy(encodedAttr);

    // Set change IP and port to EFalse
    delete iAttribute;
    iAttribute = NULL;
    iAttribute = CNATFWUNSAFChangeRequestAttribute::NewL(EFalse,EFalse);
    EUNIT_ASSERT(!iAttribute->ChangeIP());
    EUNIT_ASSERT(!iAttribute->ChangePort());
    encodedAttr = iAttribute->EncodeL();
    CleanupStack::PushL(encodedAttr);
    clone = CNATFWUNSAFChangeRequestAttribute::DecodeAttributeL(*encodedAttr);
    CleanupStack::PushL(clone);
    castedClone = static_cast<CNATFWUNSAFChangeRequestAttribute*>(clone);

    EUNIT_ASSERT(castedClone->ChangeIP() == iAttribute->ChangeIP());
    EUNIT_ASSERT(castedClone->ChangePort() == iAttribute->ChangePort());

    CleanupStack::PopAndDestroy(clone);
    CleanupStack::PopAndDestroy(encodedAttr);
    }


void UT_CNATFWUNSAFChangeRequestAttribute::TestTypeL()
    {
    EUNIT_ASSERT(CNATFWUNSAFAttribute::EChangeRequest == iAttribute->Type());
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFChangeRequestAttribute,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestEncodeAndDecodeL - test ",
    "CNATFWUNSAFChangeRequestAttribute",
    "TestEncodeAndDecodeL",
    "FUNCTIONALITY",
    SetupL, TestEncodeAndDecodeL, Teardown)

EUNIT_TEST(
    "TestTypeL - test ",
    "CNATFWUNSAFChangeRequestAttribute",
    "TestTypeL",
    "FUNCTIONALITY",
    SetupL, TestTypeL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
