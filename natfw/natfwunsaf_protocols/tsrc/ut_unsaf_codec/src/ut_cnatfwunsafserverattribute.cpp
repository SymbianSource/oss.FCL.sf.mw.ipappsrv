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
#include "UT_CNATFWUNSAFServerAttribute.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>

//  INTERNAL INCLUDES
#include "natfwunsafserverattribute.h"

_LIT8(KServer, "myserver2");

// CONSTRUCTION
UT_CNATFWUNSAFServerAttribute* UT_CNATFWUNSAFServerAttribute::NewL()
    {
    UT_CNATFWUNSAFServerAttribute* self = UT_CNATFWUNSAFServerAttribute::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFServerAttribute* UT_CNATFWUNSAFServerAttribute::NewLC()
    {
    UT_CNATFWUNSAFServerAttribute* self = new( ELeave ) UT_CNATFWUNSAFServerAttribute();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFServerAttribute::~UT_CNATFWUNSAFServerAttribute()
    {
    }

// Default constructor
UT_CNATFWUNSAFServerAttribute::UT_CNATFWUNSAFServerAttribute()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFServerAttribute::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CNATFWUNSAFServerAttribute::SetupL ()
    {
    iAttribute = CNATFWUNSAFServerAttribute::NewL(KServer);
    }


void UT_CNATFWUNSAFServerAttribute::Teardown()
    {
    delete iAttribute;
    iAttribute = NULL;
    }


void UT_CNATFWUNSAFServerAttribute::TestEncodeAndDecodeL()
    {
    HBufC8* encodedAttr = iAttribute->EncodeL();
    CleanupStack::PushL(encodedAttr);
    CNATFWUNSAFAttribute* clone =
        CNATFWUNSAFServerAttribute::DecodeAttributeL(*encodedAttr);
    CleanupStack::PushL(clone);
    const CNATFWUNSAFServerAttribute* castedClone =
        static_cast<CNATFWUNSAFServerAttribute*>(clone);

    EUNIT_ASSERT(!iAttribute->Value().CompareF(castedClone->Value()));

    CleanupStack::PopAndDestroy(clone);
    CleanupStack::PopAndDestroy(encodedAttr);
    }


void UT_CNATFWUNSAFServerAttribute::TestTypeL()
    {
    EUNIT_ASSERT(CNATFWUNSAFAttribute::EServer == iAttribute->Type());
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFServerAttribute,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestEncodeAndDecodeL - test ",
    "CNATFWUNSAFServerAttribute",
    "TestEncodeAndDecodeL",
    "FUNCTIONALITY",
    SetupL, TestEncodeAndDecodeL, Teardown)

EUNIT_TEST(
    "TestTypeL - test ",
    "CNATFWUNSAFServerAttribute",
    "TestTypeL",
    "FUNCTIONALITY",
    SetupL, TestTypeL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
