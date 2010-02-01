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
#include "UT_CNATFWUNSAFMagicCookieAttribute.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "natfwunsafmagiccookieattribute.h"

const TInt KMagicCookie = 0x72c64bc6;

// CONSTRUCTION
UT_CNATFWUNSAFMagicCookieAttribute* UT_CNATFWUNSAFMagicCookieAttribute::NewL()
    {
    UT_CNATFWUNSAFMagicCookieAttribute* self = UT_CNATFWUNSAFMagicCookieAttribute::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFMagicCookieAttribute* UT_CNATFWUNSAFMagicCookieAttribute::NewLC()
    {
    UT_CNATFWUNSAFMagicCookieAttribute* self = new( ELeave ) UT_CNATFWUNSAFMagicCookieAttribute();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFMagicCookieAttribute::~UT_CNATFWUNSAFMagicCookieAttribute()
    {
    }

// Default constructor
UT_CNATFWUNSAFMagicCookieAttribute::UT_CNATFWUNSAFMagicCookieAttribute()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFMagicCookieAttribute::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CNATFWUNSAFMagicCookieAttribute::SetupL(  )
    {
    iAttribute = CNATFWUNSAFMagicCookieAttribute::NewL();
    }

void UT_CNATFWUNSAFMagicCookieAttribute::Teardown(  )
    {
    delete iAttribute;
    iAttribute = NULL;
    }

void UT_CNATFWUNSAFMagicCookieAttribute::TestGettersL()
    {
    EUNIT_ASSERT(KMagicCookie == iAttribute->MagicCookie());
    EUNIT_ASSERT(CNATFWUNSAFAttribute::EMagicCookie == iAttribute->Type());
    }

void UT_CNATFWUNSAFMagicCookieAttribute::TestEncodeAndDecodeL()
    {
    HBufC8* encodedAttr = iAttribute->EncodeL();
    CleanupStack::PushL(encodedAttr);
    CNATFWUNSAFAttribute* clone =
        CNATFWUNSAFMagicCookieAttribute::DecodeAttributeL(*encodedAttr);
    CleanupStack::PushL(clone);
    const CNATFWUNSAFMagicCookieAttribute* castedClone =
        static_cast<CNATFWUNSAFMagicCookieAttribute*>(clone);

    EUNIT_ASSERT(iAttribute->MagicCookie() == castedClone->MagicCookie());

    CleanupStack::PopAndDestroy(clone);
    CleanupStack::PopAndDestroy(encodedAttr);
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFMagicCookieAttribute,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestGettersL - test ",
    "CNATFWUNSAFMagicCookieAttribute",
    "TestGettersL",
    "FUNCTIONALITY",
    SetupL, TestGettersL, Teardown)

EUNIT_TEST(
    "TestEncodeAndDecodeL - test ",
    "CNATFWUNSAFMagicCookieAttribute",
    "TestEncodeAndDecodeL",
    "FUNCTIONALITY",
    SetupL, TestEncodeAndDecodeL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
