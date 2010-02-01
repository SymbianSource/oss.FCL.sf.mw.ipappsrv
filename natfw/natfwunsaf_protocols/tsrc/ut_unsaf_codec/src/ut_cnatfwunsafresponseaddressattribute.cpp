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
#include "UT_CNATFWUNSAFResponseAddressAttribute.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>

//  INTERNAL INCLUDES
#include "natfwunsafresponseaddressattribute.h"

_LIT(KAddr, "1.2.3.4");

// CONSTRUCTION
UT_CNATFWUNSAFResponseAddressAttribute* UT_CNATFWUNSAFResponseAddressAttribute::NewL()
    {
    UT_CNATFWUNSAFResponseAddressAttribute* self = UT_CNATFWUNSAFResponseAddressAttribute::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFResponseAddressAttribute* UT_CNATFWUNSAFResponseAddressAttribute::NewLC()
    {
    UT_CNATFWUNSAFResponseAddressAttribute* self = new( ELeave ) UT_CNATFWUNSAFResponseAddressAttribute();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFResponseAddressAttribute::~UT_CNATFWUNSAFResponseAddressAttribute()
    {
    }

// Default constructor
UT_CNATFWUNSAFResponseAddressAttribute::UT_CNATFWUNSAFResponseAddressAttribute()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFResponseAddressAttribute::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CNATFWUNSAFResponseAddressAttribute::SetupL ()
    {
    TInetAddr addr;
    User::LeaveIfError(addr.Input(KAddr));
    iAttribute = CNATFWUNSAFResponseAddressAttribute::NewL(addr);
    }


void UT_CNATFWUNSAFResponseAddressAttribute::Teardown()
    {
    delete iAttribute;
    iAttribute = NULL;
    }


void UT_CNATFWUNSAFResponseAddressAttribute::TestEncodeAndDecodeL()
    {
    HBufC8* encodedAttr = iAttribute->EncodeL();
    CleanupStack::PushL(encodedAttr);
    CNATFWUNSAFAttribute* clone =
        CNATFWUNSAFResponseAddressAttribute::DecodeAttributeL(*encodedAttr);
    CleanupStack::PushL(clone);
    const CNATFWUNSAFResponseAddressAttribute* castedClone =
        static_cast<CNATFWUNSAFResponseAddressAttribute*>(clone);

    EUNIT_ASSERT(castedClone->Address().Match(iAttribute->Address()));

    CleanupStack::PopAndDestroy(clone);
    CleanupStack::PopAndDestroy(encodedAttr);
    }


void UT_CNATFWUNSAFResponseAddressAttribute::TestTypeL()
    {
    EUNIT_ASSERT(CNATFWUNSAFAttribute::EResponseAddress == iAttribute->Type());
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFResponseAddressAttribute,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestEncodeAndDecodeL - test ",
    "CNATFWUNSAFResponseAddressAttribute",
    "TestEncodeAndDecodeL",
    "FUNCTIONALITY",
    SetupL, TestEncodeAndDecodeL, Teardown)

EUNIT_TEST(
    "TestTypeL - test ",
    "CNATFWUNSAFResponseAddressAttribute",
    "TestTypeL",
    "FUNCTIONALITY",
    SetupL, TestTypeL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
