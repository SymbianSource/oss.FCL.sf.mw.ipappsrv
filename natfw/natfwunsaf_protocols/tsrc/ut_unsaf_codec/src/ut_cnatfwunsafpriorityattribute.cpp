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
#include "UT_CNATFWUNSAFPriorityAttribute.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "natfwunsafpriorityattribute.h"

const TInt KPriority = 0x0ee1e234;

// CONSTRUCTION
UT_CNATFWUNSAFPriorityAttribute* UT_CNATFWUNSAFPriorityAttribute::NewL()
    {
    UT_CNATFWUNSAFPriorityAttribute* self = UT_CNATFWUNSAFPriorityAttribute::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFPriorityAttribute* UT_CNATFWUNSAFPriorityAttribute::NewLC()
    {
    UT_CNATFWUNSAFPriorityAttribute* self = new( ELeave ) UT_CNATFWUNSAFPriorityAttribute();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFPriorityAttribute::~UT_CNATFWUNSAFPriorityAttribute()
    {
    }

// Default constructor
UT_CNATFWUNSAFPriorityAttribute::UT_CNATFWUNSAFPriorityAttribute()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFPriorityAttribute::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CNATFWUNSAFPriorityAttribute::SetupL(  )
    {
    iAttribute = CNATFWUNSAFPriorityAttribute::NewL(KPriority);
    }

void UT_CNATFWUNSAFPriorityAttribute::Teardown(  )
    {
    delete iAttribute;
    iAttribute = NULL;
    }

void UT_CNATFWUNSAFPriorityAttribute::TestGettersL()
    {
    EUNIT_ASSERT(KPriority == iAttribute->Priority());
    EUNIT_ASSERT(CNATFWUNSAFAttribute::EPriority == iAttribute->Type());
    }

void UT_CNATFWUNSAFPriorityAttribute::TestEncodeAndDecodeL()
    {
    HBufC8* encodedAttr = iAttribute->EncodeL();
    CleanupStack::PushL(encodedAttr);
    CNATFWUNSAFAttribute* clone =
        CNATFWUNSAFPriorityAttribute::DecodeAttributeL(*encodedAttr);
    CleanupStack::PushL(clone);
    const CNATFWUNSAFPriorityAttribute* castedClone =
        static_cast<CNATFWUNSAFPriorityAttribute*>(clone);

    EUNIT_ASSERT(iAttribute->Priority() == castedClone->Priority());

    CleanupStack::PopAndDestroy(clone);
    CleanupStack::PopAndDestroy(encodedAttr);
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFPriorityAttribute,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestGettersL - test ",
    "CNATFWUNSAFPriorityAttribute",
    "TestGettersL",
    "FUNCTIONALITY",
    SetupL, TestGettersL, Teardown)

EUNIT_TEST(
    "TestEncodeAndDecodeL - test ",
    "CNATFWUNSAFPriorityAttribute",
    "TestEncodeAndDecodeL",
    "FUNCTIONALITY",
    SetupL, TestEncodeAndDecodeL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
