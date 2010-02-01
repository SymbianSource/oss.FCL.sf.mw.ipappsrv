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
#include "UT_CNATFWUNSAFTimerValAttribute.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "natfwunsaftimervalattribute.h"

const TInt KTimerVal = 3000;

// CONSTRUCTION
UT_CNATFWUNSAFTimerValAttribute* UT_CNATFWUNSAFTimerValAttribute::NewL()
    {
    UT_CNATFWUNSAFTimerValAttribute* self = UT_CNATFWUNSAFTimerValAttribute::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFTimerValAttribute* UT_CNATFWUNSAFTimerValAttribute::NewLC()
    {
    UT_CNATFWUNSAFTimerValAttribute* self = new( ELeave ) UT_CNATFWUNSAFTimerValAttribute();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFTimerValAttribute::~UT_CNATFWUNSAFTimerValAttribute()
    {
    }

// Default constructor
UT_CNATFWUNSAFTimerValAttribute::UT_CNATFWUNSAFTimerValAttribute()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFTimerValAttribute::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CNATFWUNSAFTimerValAttribute::SetupL(  )
    {
    iAttribute = CNATFWUNSAFTimerValAttribute::NewL(KTimerVal);
    }

void UT_CNATFWUNSAFTimerValAttribute::Teardown(  )
    {
    delete iAttribute;
    iAttribute = NULL;
    }

void UT_CNATFWUNSAFTimerValAttribute::TestGettersL()
    {
    EUNIT_ASSERT(KTimerVal == iAttribute->TimerVal());
    EUNIT_ASSERT(CNATFWUNSAFAttribute::ETimerVal == iAttribute->Type());
    }

void UT_CNATFWUNSAFTimerValAttribute::TestEncodeAndDecodeL()
    {
    HBufC8* encodedAttr = iAttribute->EncodeL();
    CleanupStack::PushL(encodedAttr);
    CNATFWUNSAFAttribute* clone =
        CNATFWUNSAFTimerValAttribute::DecodeAttributeL(*encodedAttr);
    CleanupStack::PushL(clone);
    const CNATFWUNSAFTimerValAttribute* castedClone =
        static_cast<CNATFWUNSAFTimerValAttribute*>(clone);

    EUNIT_ASSERT(iAttribute->TimerVal() == castedClone->TimerVal());

    CleanupStack::PopAndDestroy(clone);
    CleanupStack::PopAndDestroy(encodedAttr);
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFTimerValAttribute,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestGettersL - test ",
    "CNATFWUNSAFTimerValAttribute",
    "TestGettersL",
    "FUNCTIONALITY",
    SetupL, TestGettersL, Teardown)

EUNIT_TEST(
    "TestEncodeAndDecodeL - test ",
    "CNATFWUNSAFTimerValAttribute",
    "TestEncodeAndDecodeL",
    "FUNCTIONALITY",
    SetupL, TestEncodeAndDecodeL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
