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
#include "UT_CNATFWUNSAFNonceAttribute.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "natfwunsafnonceattribute.h"

_LIT8(KNonceVal,"mynonce");
_LIT8(KNonceInvalidVal,"my\\ \"nonce");

// CONSTRUCTION
UT_CNATFWUNSAFNonceAttribute* UT_CNATFWUNSAFNonceAttribute::NewL()
    {
    UT_CNATFWUNSAFNonceAttribute* self = UT_CNATFWUNSAFNonceAttribute::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFNonceAttribute* UT_CNATFWUNSAFNonceAttribute::NewLC()
    {
    UT_CNATFWUNSAFNonceAttribute* self = new( ELeave ) UT_CNATFWUNSAFNonceAttribute();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFNonceAttribute::~UT_CNATFWUNSAFNonceAttribute()
    {
    }

// Default constructor
UT_CNATFWUNSAFNonceAttribute::UT_CNATFWUNSAFNonceAttribute()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFNonceAttribute::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CNATFWUNSAFNonceAttribute::SetupL()
    {
    const TDesC8& dataDesc(KNonceVal);
    iNonce = dataDesc.AllocL();

    }

void UT_CNATFWUNSAFNonceAttribute::Setup2L()
    {
    const TDesC8& dataDesc(KNonceInvalidVal);
    iNonce = dataDesc.AllocL();
    }

void UT_CNATFWUNSAFNonceAttribute::Teardown()
    {
    delete iAttribute;
    iAttribute = NULL;
    delete iNonce;
    iNonce = NULL;
    }

void UT_CNATFWUNSAFNonceAttribute::TestGettersL()
    {
    iAttribute = CNATFWUNSAFNonceAttribute::NewL(iNonce->Des());
    EUNIT_ASSERT(!iNonce->Des().Compare(iAttribute->Value()));
    EUNIT_ASSERT(CNATFWUNSAFAttribute::ENonce == iAttribute->Type());
    }

void UT_CNATFWUNSAFNonceAttribute::TestEncodeAndDecodeL()
    {
    iAttribute = CNATFWUNSAFNonceAttribute::NewL(iNonce->Des());
    HBufC8* encodedAttr = iAttribute->EncodeL();
    CleanupStack::PushL(encodedAttr);
    CNATFWUNSAFAttribute* clone =
        CNATFWUNSAFNonceAttribute::DecodeAttributeL(*encodedAttr);
    CleanupStack::PushL(clone);
    const CNATFWUNSAFNonceAttribute* castedClone =
        static_cast<CNATFWUNSAFNonceAttribute*>(clone);

    EUNIT_ASSERT(!iAttribute->Value().Compare(castedClone->Value()));

    CleanupStack::PopAndDestroy(clone);
    CleanupStack::PopAndDestroy(encodedAttr);
    }

void UT_CNATFWUNSAFNonceAttribute::TestInvalidNonceL()
    {
    TRAPD( err, iAttribute = CNATFWUNSAFNonceAttribute::NewL(iNonce->Des()));
    if ( KErrNoMemory == err )
        {
        User::Leave( err );
        }
    EUNIT_ASSERT(KErrCorrupt == err || KErrArgument == err);
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFNonceAttribute,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestGettersL - test ",
    "CNATFWUNSAFNonceAttribute",
    "TestGettersL",
    "FUNCTIONALITY",
    SetupL, TestGettersL, Teardown)

EUNIT_TEST(
    "TestEncodeAndDecodeL - test ",
    "CNATFWUNSAFNonceAttribute",
    "TestEncodeAndDecodeL",
    "FUNCTIONALITY",
    SetupL, TestEncodeAndDecodeL, Teardown)

EUNIT_TEST(
    "TestInvalidNonceL - test ",
    "CNATFWUNSAFNonceAttribute",
    "TestInvalidNonceL",
    "FUNCTIONALITY",
    Setup2L, TestInvalidNonceL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
