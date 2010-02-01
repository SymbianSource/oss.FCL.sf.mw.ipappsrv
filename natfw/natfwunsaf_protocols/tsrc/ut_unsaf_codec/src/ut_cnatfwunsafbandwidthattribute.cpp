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
#include "UT_CNATFWUNSAFBandwidthAttribute.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "natfwunsafbandwidthattribute.h"

const TInt KBandwidth = 0x0001e234;

// CONSTRUCTION
UT_CNATFWUNSAFBandwidthAttribute* UT_CNATFWUNSAFBandwidthAttribute::NewL()
    {
    UT_CNATFWUNSAFBandwidthAttribute* self = UT_CNATFWUNSAFBandwidthAttribute::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFBandwidthAttribute* UT_CNATFWUNSAFBandwidthAttribute::NewLC()
    {
    UT_CNATFWUNSAFBandwidthAttribute* self = new( ELeave ) UT_CNATFWUNSAFBandwidthAttribute();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFBandwidthAttribute::~UT_CNATFWUNSAFBandwidthAttribute()
    {
    }

// Default constructor
UT_CNATFWUNSAFBandwidthAttribute::UT_CNATFWUNSAFBandwidthAttribute()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFBandwidthAttribute::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CNATFWUNSAFBandwidthAttribute::SetupL(  )
    {
    iAttribute = CNATFWUNSAFBandwidthAttribute::NewL(KBandwidth);
    }

void UT_CNATFWUNSAFBandwidthAttribute::Teardown(  )
    {
    delete iAttribute;
    iAttribute = NULL;
    }

void UT_CNATFWUNSAFBandwidthAttribute::TestGettersL()
    {
    EUNIT_ASSERT(KBandwidth == iAttribute->Bandwidth());
    EUNIT_ASSERT(CNATFWUNSAFAttribute::EBandwidth == iAttribute->Type());
    }

void UT_CNATFWUNSAFBandwidthAttribute::TestEncodeAndDecodeL()
    {
    HBufC8* encodedAttr = iAttribute->EncodeL();
    CleanupStack::PushL(encodedAttr);
    CNATFWUNSAFAttribute* clone =
        CNATFWUNSAFBandwidthAttribute::DecodeAttributeL(*encodedAttr);
    CleanupStack::PushL(clone);
    const CNATFWUNSAFBandwidthAttribute* castedClone =
        static_cast<CNATFWUNSAFBandwidthAttribute*>(clone);

    EUNIT_ASSERT(iAttribute->Bandwidth() == castedClone->Bandwidth());

    CleanupStack::PopAndDestroy(clone);
    CleanupStack::PopAndDestroy(encodedAttr);
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFBandwidthAttribute,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestGettersL - test ",
    "CNATFWUNSAFBandwidthAttribute",
    "TestGettersL",
    "FUNCTIONALITY",
    SetupL, TestGettersL, Teardown)

EUNIT_TEST(
    "TestEncodeAndDecodeL - test ",
    "CNATFWUNSAFBandwidthAttribute",
    "TestEncodeAndDecodeL",
    "FUNCTIONALITY",
    SetupL, TestEncodeAndDecodeL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
