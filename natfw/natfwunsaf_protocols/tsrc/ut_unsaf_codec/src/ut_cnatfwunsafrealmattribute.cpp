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
#include "UT_CNATFWUNSAFRealmAttribute.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "natfwunsafrealmattribute.h"

_LIT8(KQuotedRealmVal,"\"myrealm\"");
_LIT8(KUnQuotedRealmVal,"myrealm");

// CONSTRUCTION
UT_CNATFWUNSAFRealmAttribute* UT_CNATFWUNSAFRealmAttribute::NewL()
    {
    UT_CNATFWUNSAFRealmAttribute* self = UT_CNATFWUNSAFRealmAttribute::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFRealmAttribute* UT_CNATFWUNSAFRealmAttribute::NewLC()
    {
    UT_CNATFWUNSAFRealmAttribute* self = new( ELeave ) UT_CNATFWUNSAFRealmAttribute();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFRealmAttribute::~UT_CNATFWUNSAFRealmAttribute()
    {
    }

// Default constructor
UT_CNATFWUNSAFRealmAttribute::UT_CNATFWUNSAFRealmAttribute()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFRealmAttribute::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CNATFWUNSAFRealmAttribute::SetupL()
    {
    const TDesC8& dataDesc(KQuotedRealmVal);
    iRealm = dataDesc.AllocL();
    }

void UT_CNATFWUNSAFRealmAttribute::Teardown()
    {
    delete iAttribute;
    iAttribute = NULL;
    delete iRealm;
    iRealm = NULL;
    }

void UT_CNATFWUNSAFRealmAttribute::TestGettersL()
    {
    iAttribute = CNATFWUNSAFRealmAttribute::NewL(iRealm->Des());
    // Now the double quotes should be removed
    const TDesC8& dataDesc(KUnQuotedRealmVal);
    EUNIT_ASSERT(!dataDesc.Compare(iAttribute->Value()));
    EUNIT_ASSERT(CNATFWUNSAFAttribute::ERealm == iAttribute->Type());
    }

void UT_CNATFWUNSAFRealmAttribute::TestEncodeAndDecodeL()
    {
    iAttribute = CNATFWUNSAFRealmAttribute::NewL(iRealm->Des());
    HBufC8* encodedAttr = iAttribute->EncodeL();
    CleanupStack::PushL(encodedAttr);
    CNATFWUNSAFAttribute* clone =
        CNATFWUNSAFRealmAttribute::DecodeAttributeL(*encodedAttr);
    CleanupStack::PushL(clone);
    const CNATFWUNSAFRealmAttribute* castedClone =
        static_cast<CNATFWUNSAFRealmAttribute*>(clone);

    EUNIT_ASSERT(!iAttribute->Value().Compare(castedClone->Value()));

    CleanupStack::PopAndDestroy(clone);
    CleanupStack::PopAndDestroy(encodedAttr);
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFRealmAttribute,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestGettersL - test ",
    "CNATFWUNSAFRealmAttribute",
    "TestGettersL",
    "FUNCTIONALITY",
    SetupL, TestGettersL, Teardown)

EUNIT_TEST(
    "TestEncodeAndDecodeL - test ",
    "CNATFWUNSAFRealmAttribute",
    "TestEncodeAndDecodeL",
    "FUNCTIONALITY",
    SetupL, TestEncodeAndDecodeL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
