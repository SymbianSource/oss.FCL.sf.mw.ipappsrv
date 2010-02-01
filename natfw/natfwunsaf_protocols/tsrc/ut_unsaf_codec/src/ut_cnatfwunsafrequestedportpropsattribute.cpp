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
#include "UT_CNATFWUNSAFRequestedPortPropsAttribute.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>

//  INTERNAL INCLUDES
#include "natfwunsafrequestedportpropsattribute.h"

// CONSTRUCTION
UT_CNATFWUNSAFRequestedPortPropsAttribute* UT_CNATFWUNSAFRequestedPortPropsAttribute::NewL()
    {
    UT_CNATFWUNSAFRequestedPortPropsAttribute* self = UT_CNATFWUNSAFRequestedPortPropsAttribute::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFRequestedPortPropsAttribute* UT_CNATFWUNSAFRequestedPortPropsAttribute::NewLC()
    {
    UT_CNATFWUNSAFRequestedPortPropsAttribute* self = new( ELeave ) UT_CNATFWUNSAFRequestedPortPropsAttribute();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFRequestedPortPropsAttribute::~UT_CNATFWUNSAFRequestedPortPropsAttribute()
    {
    }

// Default constructor
UT_CNATFWUNSAFRequestedPortPropsAttribute::UT_CNATFWUNSAFRequestedPortPropsAttribute()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFRequestedPortPropsAttribute::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CNATFWUNSAFRequestedPortPropsAttribute::SetupL(  )
    {
    iAttribute = CNATFWUNSAFRequestedPortPropsAttribute::NewL(
        CNATFWUNSAFRequestedPortPropsAttribute::EAlignmentPrefEvenAndNextOdd,
        5000);
    }

void UT_CNATFWUNSAFRequestedPortPropsAttribute::Teardown(  )
    {
    delete iAttribute;
    iAttribute = NULL;
    }

void UT_CNATFWUNSAFRequestedPortPropsAttribute::TestEncodeAndDecodeL()
    {
    EUNIT_ASSERT(CNATFWUNSAFRequestedPortPropsAttribute::EAlignmentPrefEvenAndNextOdd ==
        iAttribute->PortAlignmentPref());
    EUNIT_ASSERT(5000 == iAttribute->Port());
    HBufC8* encodedAttr = iAttribute->EncodeL();
    CleanupStack::PushL(encodedAttr);
    CNATFWUNSAFAttribute* clone =
        CNATFWUNSAFRequestedPortPropsAttribute::DecodeAttributeL(*encodedAttr);
    CleanupStack::PushL(clone);
    const CNATFWUNSAFRequestedPortPropsAttribute* castedClone =
        static_cast<CNATFWUNSAFRequestedPortPropsAttribute*>(clone);

    EUNIT_ASSERT(castedClone->PortAlignmentPref() == iAttribute->PortAlignmentPref());
    EUNIT_ASSERT(castedClone->Port() == iAttribute->Port());
    
    CleanupStack::PopAndDestroy(clone);
    CleanupStack::PopAndDestroy(encodedAttr);

    // Change port range and alignment preferences
    delete iAttribute;
    iAttribute = NULL;
    iAttribute = CNATFWUNSAFRequestedPortPropsAttribute::NewL(
        CNATFWUNSAFRequestedPortPropsAttribute::EAlignmentPrefNone,
        0);
    
    EUNIT_ASSERT(!iAttribute->PortAlignmentPref());
    EUNIT_ASSERT(!iAttribute->Port());
    
    encodedAttr = iAttribute->EncodeL();
    CleanupStack::PushL(encodedAttr);
    clone = CNATFWUNSAFRequestedPortPropsAttribute::DecodeAttributeL(*encodedAttr);
    CleanupStack::PushL(clone);
    castedClone = static_cast<CNATFWUNSAFRequestedPortPropsAttribute*>(clone);

    EUNIT_ASSERT(castedClone->PortAlignmentPref() == iAttribute->PortAlignmentPref());
    EUNIT_ASSERT(castedClone->Port() == iAttribute->Port());

    CleanupStack::PopAndDestroy(clone);
    CleanupStack::PopAndDestroy(encodedAttr);
    }


void UT_CNATFWUNSAFRequestedPortPropsAttribute::TestType()
    {
    EUNIT_ASSERT(CNATFWUNSAFAttribute::ERequestedPortProps == iAttribute->Type());
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFRequestedPortPropsAttribute,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestEncodeAndDecodeL - test ",
    "CNATFWUNSAFRequestedPortPropsAttribute",
    "TestEncodeAndDecodeL",
    "FUNCTIONALITY",
    SetupL, TestEncodeAndDecodeL, Teardown)

EUNIT_TEST(
    "TestType - test ",
    "CNATFWUNSAFRequestedPortPropsAttribute",
    "TestTypeL",
    "FUNCTIONALITY",
    SetupL, TestType, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
