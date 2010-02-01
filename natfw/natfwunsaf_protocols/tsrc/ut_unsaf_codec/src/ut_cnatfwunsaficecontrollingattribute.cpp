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
#include "UT_CNATFWUNSAFIceControllingAttribute.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "natfwunsaficecontrollingattribute.h"

const TUint64 KIceControlling = 0x0001e2340001e234;

// CONSTRUCTION
UT_CNATFWUNSAFIceControllingAttribute* UT_CNATFWUNSAFIceControllingAttribute::NewL()
    {
    UT_CNATFWUNSAFIceControllingAttribute* self = UT_CNATFWUNSAFIceControllingAttribute::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFIceControllingAttribute* UT_CNATFWUNSAFIceControllingAttribute::NewLC()
    {
    UT_CNATFWUNSAFIceControllingAttribute* self = new( ELeave ) UT_CNATFWUNSAFIceControllingAttribute();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFIceControllingAttribute::~UT_CNATFWUNSAFIceControllingAttribute()
    {
    }

// Default constructor
UT_CNATFWUNSAFIceControllingAttribute::UT_CNATFWUNSAFIceControllingAttribute()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFIceControllingAttribute::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CNATFWUNSAFIceControllingAttribute::SetupL(  )
    {
    iAttribute = CNATFWUNSAFIceControllingAttribute::NewL(KIceControlling);
    }

void UT_CNATFWUNSAFIceControllingAttribute::Teardown(  )
    {
    delete iAttribute;
    iAttribute = NULL;
    }

void UT_CNATFWUNSAFIceControllingAttribute::TestGettersL()
    {
    EUNIT_ASSERT(KIceControlling == iAttribute->IceControlling());
    EUNIT_ASSERT(CNATFWUNSAFAttribute::EIceControlling == iAttribute->Type());
    }

void UT_CNATFWUNSAFIceControllingAttribute::TestEncodeAndDecodeL()
    {
    HBufC8* encodedAttr = iAttribute->EncodeL();
    CleanupStack::PushL(encodedAttr);
    CNATFWUNSAFAttribute* clone =
        CNATFWUNSAFIceControllingAttribute::DecodeAttributeL(*encodedAttr);
    CleanupStack::PushL(clone);
    const CNATFWUNSAFIceControllingAttribute* castedClone =
        static_cast<CNATFWUNSAFIceControllingAttribute*>(clone);

    EUNIT_ASSERT(iAttribute->IceControlling() == castedClone->IceControlling());

    CleanupStack::PopAndDestroy(clone);
    CleanupStack::PopAndDestroy(encodedAttr);
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFIceControllingAttribute,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestGettersL - test ",
    "CNATFWUNSAFIceControllingAttribute",
    "TestGettersL",
    "FUNCTIONALITY",
    SetupL, TestGettersL, Teardown)

EUNIT_TEST(
    "TestEncodeAndDecodeL - test ",
    "CNATFWUNSAFIceControllingAttribute",
    "TestEncodeAndDecodeL",
    "FUNCTIONALITY",
    SetupL, TestEncodeAndDecodeL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
