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
#include "UT_CNATFWUNSAFRequestedTransportAttribute.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "natfwunsafrequestedtransportattribute.h"

const TUint KTransportUdp = 0;
const TUint KTransportTcp = 1;
const TUint KTransportInvalid = 2;

// CONSTRUCTION
UT_CNATFWUNSAFRequestedTransportAttribute* UT_CNATFWUNSAFRequestedTransportAttribute::NewL()
    {
    UT_CNATFWUNSAFRequestedTransportAttribute* self = UT_CNATFWUNSAFRequestedTransportAttribute::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFRequestedTransportAttribute* UT_CNATFWUNSAFRequestedTransportAttribute::NewLC()
    {
    UT_CNATFWUNSAFRequestedTransportAttribute* self = new( ELeave ) UT_CNATFWUNSAFRequestedTransportAttribute();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFRequestedTransportAttribute::~UT_CNATFWUNSAFRequestedTransportAttribute()
    {
    }

// Default constructor
UT_CNATFWUNSAFRequestedTransportAttribute::UT_CNATFWUNSAFRequestedTransportAttribute()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFRequestedTransportAttribute::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CNATFWUNSAFRequestedTransportAttribute::SetupL()
    {
    iAttributeUdp = CNATFWUNSAFRequestedTransportAttribute::NewL(
        static_cast<CNATFWUNSAFRequestedTransportAttribute::TTransportProtocol>
            (KTransportUdp));
    iAttributeTcp = CNATFWUNSAFRequestedTransportAttribute::NewL(
        static_cast<CNATFWUNSAFRequestedTransportAttribute::TTransportProtocol>
            (KTransportTcp));
    TRAPD( err, iAttributeInv = CNATFWUNSAFRequestedTransportAttribute::NewL(
        static_cast<CNATFWUNSAFRequestedTransportAttribute::TTransportProtocol>
            (KTransportInvalid)));
    EUNIT_ASSERT(err != KErrNone );
    }

void UT_CNATFWUNSAFRequestedTransportAttribute::Teardown()
    {
    delete iAttributeUdp;
    iAttributeUdp = NULL;
    delete iAttributeTcp;
    iAttributeTcp = NULL;
    delete iAttributeInv;
    iAttributeInv = NULL;
    }

void UT_CNATFWUNSAFRequestedTransportAttribute::TestGettersL()
    {
    EUNIT_ASSERT(KTransportUdp == iAttributeUdp->TransportProtocol());
    EUNIT_ASSERT(KTransportTcp == iAttributeTcp->TransportProtocol());
    EUNIT_ASSERT(CNATFWUNSAFAttribute::ERequestedTransport == iAttributeUdp->Type());
    EUNIT_ASSERT(CNATFWUNSAFAttribute::ERequestedTransport == iAttributeTcp->Type());
    }

void UT_CNATFWUNSAFRequestedTransportAttribute::TestEncodeAndDecodeL()
    {
    HBufC8* encodedAttr = iAttributeUdp->EncodeL();
    CleanupStack::PushL(encodedAttr);
    CNATFWUNSAFAttribute* clone =
        CNATFWUNSAFRequestedTransportAttribute::DecodeAttributeL(*encodedAttr);
    CleanupStack::PushL(clone);
    const CNATFWUNSAFRequestedTransportAttribute* castedClone =
        static_cast<CNATFWUNSAFRequestedTransportAttribute*>(clone);

    EUNIT_ASSERT(iAttributeUdp->TransportProtocol() == castedClone->TransportProtocol());

    CleanupStack::PopAndDestroy(clone);
    CleanupStack::PopAndDestroy(encodedAttr);
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFRequestedTransportAttribute,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestGettersL - test ",
    "CNATFWUNSAFRequestedTransportAttribute",
    "TestGettersL",
    "FUNCTIONALITY",
    SetupL, TestGettersL, Teardown)

EUNIT_TEST(
    "TestEncodeAndDecodeL - test ",
    "CNATFWUNSAFRequestedTransportAttribute",
    "TestEncodeAndDecodeL",
    "FUNCTIONALITY",
    SetupL, TestEncodeAndDecodeL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
