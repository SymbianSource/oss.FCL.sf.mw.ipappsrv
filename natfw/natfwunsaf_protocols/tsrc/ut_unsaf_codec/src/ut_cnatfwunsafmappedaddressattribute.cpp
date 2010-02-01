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
#include "UT_CNATFWUNSAFMappedAddressAttribute.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "natfwunsafmappedaddressattribute.h"
#include "natfwunsafutils.h"

_LIT(KAddr, "1.2.3.4");

// CONSTRUCTION
UT_CNATFWUNSAFMappedAddressAttribute* UT_CNATFWUNSAFMappedAddressAttribute::NewL()
    {
    UT_CNATFWUNSAFMappedAddressAttribute* self = UT_CNATFWUNSAFMappedAddressAttribute::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFMappedAddressAttribute* UT_CNATFWUNSAFMappedAddressAttribute::NewLC()
    {
    UT_CNATFWUNSAFMappedAddressAttribute* self = new( ELeave ) UT_CNATFWUNSAFMappedAddressAttribute();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFMappedAddressAttribute::~UT_CNATFWUNSAFMappedAddressAttribute()
    {
    }

// Default constructor
UT_CNATFWUNSAFMappedAddressAttribute::UT_CNATFWUNSAFMappedAddressAttribute()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFMappedAddressAttribute::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CNATFWUNSAFMappedAddressAttribute::SetupL ()
    {
    TInetAddr addr;
    User::LeaveIfError(addr.Input(KAddr));
    iAttribute = CNATFWUNSAFMappedAddressAttribute::NewL(addr);


    const TInt KIPv6AddressSize = 16;
    TIp6Addr addrIPv6;
    for ( TInt i = 0; i < KIPv6AddressSize; ++ i )
        {
        addrIPv6.u.iAddr8[i] = i;
        }
    iIPv6Address.SetAddress(addrIPv6);
    iIPv6Address.SetPort(5060);
    }


void UT_CNATFWUNSAFMappedAddressAttribute::Teardown()
    {
    delete iAttribute;
    iAttribute = NULL;
    }


void UT_CNATFWUNSAFMappedAddressAttribute::TestEncodeAndDecodeL()
    {
    HBufC8* encodedAttr = iAttribute->EncodeL();
    CleanupStack::PushL(encodedAttr);
    CNATFWUNSAFAttribute* clone =
        CNATFWUNSAFMappedAddressAttribute::DecodeAttributeL(*encodedAttr);
    CleanupStack::PushL(clone);
    const CNATFWUNSAFMappedAddressAttribute* castedClone =
        static_cast<CNATFWUNSAFMappedAddressAttribute*>(clone);

    EUNIT_ASSERT(castedClone->Address().Match(iAttribute->Address()));

    CleanupStack::PopAndDestroy(clone);
    CleanupStack::PopAndDestroy(encodedAttr);
    }


void UT_CNATFWUNSAFMappedAddressAttribute::TestTypeL()
    {
    EUNIT_ASSERT(CNATFWUNSAFAttribute::EMappedAddress == iAttribute->Type());
    }

void UT_CNATFWUNSAFMappedAddressAttribute::TestEncodeAndDecodeIPv6L()
    {
    delete iAttribute;
    iAttribute = NULL;

    iAttribute = CNATFWUNSAFMappedAddressAttribute::NewL(iIPv6Address);
    HBufC8* encodedAttr = iAttribute->EncodeL();
    CleanupStack::PushL(encodedAttr);
    CNATFWUNSAFAttribute* clone =
        CNATFWUNSAFMappedAddressAttribute::DecodeAttributeL(*encodedAttr);
    CleanupStack::PushL(clone);
    const CNATFWUNSAFMappedAddressAttribute* castedClone =
        static_cast<CNATFWUNSAFMappedAddressAttribute*>(clone);

    EUNIT_ASSERT(castedClone->Address().Match(iAttribute->Address()));
    EUNIT_ASSERT(iAttribute->Address().CmpAddr(iIPv6Address));

    CleanupStack::PopAndDestroy(clone);
    CleanupStack::PopAndDestroy(encodedAttr);
    }

//Decode MAPPED-ADDRESS from a byte stream
void UT_CNATFWUNSAFMappedAddressAttribute::TestIPv6DecodeL()
    {
    delete iAttribute;
    iAttribute = NULL;

    TInt port = 5060;
    const TUint8 KStreamToDecode[] =
        {
        //MAPPED-ADDRESS attribute
        0, 1, //type
        //length of value element
        0, CNATFWUNSAFAddressAttribute::EAttributeValueSizeIPv6,
        0, //undefined
        CNATFWUNSAFAddressAttribute::KAddressFamilyIPv6,
        NATFWUNSAFUtils::HighByte(port), NATFWUNSAFUtils::LowByte(port), //port
        0, 1, 2, 3, //value of iIPv6Address
        4, 5, 6, 7,
        8, 9, 0xa, 0xb,
        0xc, 0xd, 0xe, 0xf
        };

    HBufC8* byteStream = HBufC8::NewLC(sizeof(KStreamToDecode));
    byteStream->Des().Append(KStreamToDecode, sizeof(KStreamToDecode));

    EUNIT_ASSERT(CNATFWUNSAFAddressAttribute::EAttributeValueSizeIPv6 ==
                      CNATFWUNSAFAttribute::ParseLengthL(*byteStream));

    CNATFWUNSAFAttribute* attr =
        CNATFWUNSAFMappedAddressAttribute::DecodeAttributeL(*byteStream);
    iAttribute = static_cast<CNATFWUNSAFMappedAddressAttribute*>(attr);
    CleanupStack::PopAndDestroy(byteStream);

    EUNIT_ASSERT(port == iAttribute->Address().Port());
    EUNIT_ASSERT(iAttribute->Address().CmpAddr(iIPv6Address));
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFMappedAddressAttribute,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestEncodeAndDecodeL - test ",
    "CNATFWUNSAFMappedAddressAttribute",
    "TestEncodeAndDecodeL",
    "FUNCTIONALITY",
    SetupL, TestEncodeAndDecodeL, Teardown)

EUNIT_TEST(
    "TestTypeL - test ",
    "CNATFWUNSAFMappedAddressAttribute",
    "TestTypeL",
    "FUNCTIONALITY",
    SetupL, TestTypeL, Teardown)

EUNIT_TEST(
    "TestEncodeAndDecodeIPv6L - test ",
    "CNATFWUNSAFMappedAddressAttribute",
    "TestEncodeAndDecodeIPv6L",
    "FUNCTIONALITY",
    SetupL, TestEncodeAndDecodeIPv6L, Teardown)

EUNIT_TEST(
    "TestIPv6DecodeL - test ",
    "CNATFWUNSAFMappedAddressAttribute",
    "TestIPv6DecodeL",
    "FUNCTIONALITY",
    SetupL, TestIPv6DecodeL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
