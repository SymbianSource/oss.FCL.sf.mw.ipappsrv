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
#include "UT_CNATFWUNSAFLifetimeAttribute.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "natfwunsaflifetimeattribute.h"

const TInt KLifetime = 0x0ae1e234;

// CONSTRUCTION
UT_CNATFWUNSAFLifetimeAttribute* UT_CNATFWUNSAFLifetimeAttribute::NewL()
    {
    UT_CNATFWUNSAFLifetimeAttribute* self = UT_CNATFWUNSAFLifetimeAttribute::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFLifetimeAttribute* UT_CNATFWUNSAFLifetimeAttribute::NewLC()
    {
    UT_CNATFWUNSAFLifetimeAttribute* self = new( ELeave ) UT_CNATFWUNSAFLifetimeAttribute();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFLifetimeAttribute::~UT_CNATFWUNSAFLifetimeAttribute()
    {
    }

// Default constructor
UT_CNATFWUNSAFLifetimeAttribute::UT_CNATFWUNSAFLifetimeAttribute()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFLifetimeAttribute::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CNATFWUNSAFLifetimeAttribute::SetupL(  )
    {
    iAttribute = CNATFWUNSAFLifetimeAttribute::NewL(KLifetime);
    }

void UT_CNATFWUNSAFLifetimeAttribute::Teardown(  )
    {
    delete iAttribute;
    iAttribute = NULL;
    }

void UT_CNATFWUNSAFLifetimeAttribute::TestGettersL()
    {
    EUNIT_ASSERT(KLifetime == iAttribute->Lifetime());
    EUNIT_ASSERT(CNATFWUNSAFAttribute::ELifetime == iAttribute->Type());
    }

void UT_CNATFWUNSAFLifetimeAttribute::TestEncodeAndDecodeL()
    {
    HBufC8* encodedAttr = iAttribute->EncodeL();
    CleanupStack::PushL(encodedAttr);
    CNATFWUNSAFAttribute* clone =
        CNATFWUNSAFLifetimeAttribute::DecodeAttributeL(*encodedAttr);
    CleanupStack::PushL(clone);
    const CNATFWUNSAFLifetimeAttribute* castedClone =
        static_cast<CNATFWUNSAFLifetimeAttribute*>(clone);

    EUNIT_ASSERT(iAttribute->Lifetime() == castedClone->Lifetime());

    CleanupStack::PopAndDestroy(clone);
    CleanupStack::PopAndDestroy(encodedAttr);
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFLifetimeAttribute,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestGettersL - test ",
    "CNATFWUNSAFLifetimeAttribute",
    "TestGettersL",
    "FUNCTIONALITY",
    SetupL, TestGettersL, Teardown)

EUNIT_TEST(
    "TestEncodeAndDecodeL - test ",
    "CNATFWUNSAFLifetimeAttribute",
    "TestEncodeAndDecodeL",
    "FUNCTIONALITY",
    SetupL, TestEncodeAndDecodeL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
