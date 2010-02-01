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
#include "UT_CNATFWUNSAFIceControlledAttribute.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "natfwunsaficecontrolledattribute.h"

const TUint64 KIceControlled = 0x0001e2340001e234;

// CONSTRUCTION
UT_CNATFWUNSAFIceControlledAttribute* UT_CNATFWUNSAFIceControlledAttribute::NewL()
    {
    UT_CNATFWUNSAFIceControlledAttribute* self = UT_CNATFWUNSAFIceControlledAttribute::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFIceControlledAttribute* UT_CNATFWUNSAFIceControlledAttribute::NewLC()
    {
    UT_CNATFWUNSAFIceControlledAttribute* self = new( ELeave ) UT_CNATFWUNSAFIceControlledAttribute();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFIceControlledAttribute::~UT_CNATFWUNSAFIceControlledAttribute()
    {
    }

// Default constructor
UT_CNATFWUNSAFIceControlledAttribute::UT_CNATFWUNSAFIceControlledAttribute()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFIceControlledAttribute::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CNATFWUNSAFIceControlledAttribute::SetupL(  )
    {
    iAttribute = CNATFWUNSAFIceControlledAttribute::NewL(KIceControlled);
    }

void UT_CNATFWUNSAFIceControlledAttribute::Teardown(  )
    {
    delete iAttribute;
    iAttribute = NULL;
    }

void UT_CNATFWUNSAFIceControlledAttribute::TestGettersL()
    {
    EUNIT_ASSERT(KIceControlled == iAttribute->IceControlled());
    EUNIT_ASSERT(CNATFWUNSAFAttribute::EIceControlled == iAttribute->Type());
    }

void UT_CNATFWUNSAFIceControlledAttribute::TestEncodeAndDecodeL()
    {
    HBufC8* encodedAttr = iAttribute->EncodeL();
    CleanupStack::PushL(encodedAttr);
    CNATFWUNSAFAttribute* clone =
        CNATFWUNSAFIceControlledAttribute::DecodeAttributeL(*encodedAttr);
    CleanupStack::PushL(clone);
    const CNATFWUNSAFIceControlledAttribute* castedClone =
        static_cast<CNATFWUNSAFIceControlledAttribute*>(clone);

    EUNIT_ASSERT(iAttribute->IceControlled() == castedClone->IceControlled());

    CleanupStack::PopAndDestroy(clone);
    CleanupStack::PopAndDestroy(encodedAttr);
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFIceControlledAttribute,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestGettersL - test ",
    "CNATFWUNSAFIceControlledAttribute",
    "TestGettersL",
    "FUNCTIONALITY",
    SetupL, TestGettersL, Teardown)

EUNIT_TEST(
    "TestEncodeAndDecodeL - test ",
    "CNATFWUNSAFIceControlledAttribute",
    "TestEncodeAndDecodeL",
    "FUNCTIONALITY",
    SetupL, TestEncodeAndDecodeL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
