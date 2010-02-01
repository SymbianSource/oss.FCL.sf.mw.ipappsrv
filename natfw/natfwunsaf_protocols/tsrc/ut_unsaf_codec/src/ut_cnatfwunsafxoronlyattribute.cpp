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
#include "ut_cnatfwunsafxoronlyattribute.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "natfwunsafxoronlyattribute.h"


// CONSTRUCTION
UT_CNATFWUNSAFXorOnlyAttribute* UT_CNATFWUNSAFXorOnlyAttribute::NewL()
    {
    UT_CNATFWUNSAFXorOnlyAttribute* self = UT_CNATFWUNSAFXorOnlyAttribute::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFXorOnlyAttribute* UT_CNATFWUNSAFXorOnlyAttribute::NewLC()
    {
    UT_CNATFWUNSAFXorOnlyAttribute* self = new( ELeave ) UT_CNATFWUNSAFXorOnlyAttribute();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFXorOnlyAttribute::~UT_CNATFWUNSAFXorOnlyAttribute()
    {
    }

// Default constructor
UT_CNATFWUNSAFXorOnlyAttribute::UT_CNATFWUNSAFXorOnlyAttribute()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFXorOnlyAttribute::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CNATFWUNSAFXorOnlyAttribute::SetupL ()
    {
    iAttribute = CNATFWUNSAFXorOnlyAttribute::NewL();
    }


void UT_CNATFWUNSAFXorOnlyAttribute::Teardown()
    {
    delete iAttribute;
    iAttribute = NULL;
    }


void UT_CNATFWUNSAFXorOnlyAttribute::TestEncodeAndDecodeL()
    {
    HBufC8* encodedAttr = iAttribute->EncodeL();
    CleanupStack::PushL(encodedAttr);
    CNATFWUNSAFAttribute* clone =
        CNATFWUNSAFXorOnlyAttribute::DecodeAttributeL(*encodedAttr);
    delete clone;
    CleanupStack::PopAndDestroy(encodedAttr);
    }


void UT_CNATFWUNSAFXorOnlyAttribute::TestTypeL()
    {
    EUNIT_ASSERT(CNATFWUNSAFAttribute::EXorOnly == iAttribute->Type());
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFXorOnlyAttribute,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestEncodeAndDecodeL - test ",
    "CNATFWUNSAFXorOnlyAttribute",
    "TestEncodeAndDecodeL",
    "FUNCTIONALITY",
    SetupL, TestEncodeAndDecodeL, Teardown)

EUNIT_TEST(
    "TestTypeL - test ",
    "CNATFWUNSAFXorOnlyAttribute",
    "TestTypeL",
    "FUNCTIONALITY",
    SetupL, TestTypeL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
