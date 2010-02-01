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
#include "UT_CNATFWUNSAFAlternateServerAttribute.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "natfwunsafalternateserverattribute.h"
#include "natfwunsafutils.h"

_LIT(KAddr, "1.2.3.4");

// CONSTRUCTION
UT_CNATFWUNSAFAlternateServerAttribute* UT_CNATFWUNSAFAlternateServerAttribute::NewL()
    {
    UT_CNATFWUNSAFAlternateServerAttribute* self = UT_CNATFWUNSAFAlternateServerAttribute::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFAlternateServerAttribute* UT_CNATFWUNSAFAlternateServerAttribute::NewLC()
    {
    UT_CNATFWUNSAFAlternateServerAttribute* self = new( ELeave ) UT_CNATFWUNSAFAlternateServerAttribute();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFAlternateServerAttribute::~UT_CNATFWUNSAFAlternateServerAttribute()
    {
    }

// Default constructor
UT_CNATFWUNSAFAlternateServerAttribute::UT_CNATFWUNSAFAlternateServerAttribute()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFAlternateServerAttribute::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CNATFWUNSAFAlternateServerAttribute::SetupL(  )
    {
    TInetAddr addr;
    User::LeaveIfError(addr.Input(KAddr));
    iAttribute = CNATFWUNSAFAlternateServerAttribute::NewL(addr);

    const TInt KIPv6AddressSize = 16;
    TIp6Addr addrIPv6;
    for ( TInt i = 0; i < KIPv6AddressSize; ++ i )
        {
        addrIPv6.u.iAddr8[i] = i;
        }
    iIPv6Address.SetAddress(addrIPv6);
    iIPv6Address.SetPort(5060);

    }

void UT_CNATFWUNSAFAlternateServerAttribute::Teardown(  )
    {
    delete iAttribute;
    iAttribute = NULL;
    }

void UT_CNATFWUNSAFAlternateServerAttribute::TestEncodeAndDecodeL()
    {
    HBufC8* encodedAttr = iAttribute->EncodeL();
    CleanupStack::PushL(encodedAttr);
    CNATFWUNSAFAttribute* clone =
        CNATFWUNSAFAlternateServerAttribute::DecodeAttributeL(*encodedAttr);
    CleanupStack::PushL(clone);
    const CNATFWUNSAFAlternateServerAttribute* castedClone =
        static_cast<CNATFWUNSAFAlternateServerAttribute*>(clone);

    EUNIT_ASSERT(castedClone->Address().Match(iAttribute->Address()));

    CleanupStack::PopAndDestroy(clone);
    CleanupStack::PopAndDestroy(encodedAttr);
    }


void UT_CNATFWUNSAFAlternateServerAttribute::TestTypeL()
    {
    EUNIT_ASSERT(CNATFWUNSAFAttribute::EAlternateServer == iAttribute->Type());
    }

void UT_CNATFWUNSAFAlternateServerAttribute::TestEncodeAndDecodeIPv6L()
    {
    delete iAttribute;
    iAttribute = NULL;

    iAttribute = CNATFWUNSAFAlternateServerAttribute::NewL(iIPv6Address);
    HBufC8* encodedAttr = iAttribute->EncodeL();
    CleanupStack::PushL(encodedAttr);
    CNATFWUNSAFAttribute* clone =
        CNATFWUNSAFAlternateServerAttribute::DecodeAttributeL(*encodedAttr);
    CleanupStack::PushL(clone);
    const CNATFWUNSAFAlternateServerAttribute* castedClone =
        static_cast<CNATFWUNSAFAlternateServerAttribute*>(clone);

    EUNIT_ASSERT(castedClone->Address().Match(iAttribute->Address()));
    EUNIT_ASSERT(iAttribute->Address().CmpAddr(iIPv6Address));

    CleanupStack::PopAndDestroy(clone);
    CleanupStack::PopAndDestroy(encodedAttr);
    }

//Decode MAPPED-ADDRESS from a byte stream
void UT_CNATFWUNSAFAlternateServerAttribute::TestIPv6DecodeL()
    {
    delete iAttribute;
    iAttribute = NULL;

    TInt port = 5060;
    const TUint8 KStreamToDecode[] =
        {
        //ALTERNATE-SERVER attribute
        0, 0xe, //type
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
        CNATFWUNSAFAlternateServerAttribute::DecodeAttributeL(*byteStream);
    iAttribute = static_cast<CNATFWUNSAFAlternateServerAttribute*>(attr);
    CleanupStack::PopAndDestroy(byteStream);

    EUNIT_ASSERT(port == iAttribute->Address().Port());
    EUNIT_ASSERT(iAttribute->Address().CmpAddr(iIPv6Address));
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFAlternateServerAttribute,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestEncodeAndDecodeL - test ",
    "CNATFWUNSAFAlternateServerAttribute",
    "TestEncodeAndDecodeL",
    "FUNCTIONALITY",
    SetupL, TestEncodeAndDecodeL, Teardown)

EUNIT_TEST(
    "TestTypeL - test ",
    "CNATFWUNSAFAlternateServerAttribute",
    "TestTypeL",
    "FUNCTIONALITY",
    SetupL, TestTypeL, Teardown)

EUNIT_TEST(
    "TestEncodeAndDecodeIPv6L - test ",
    "CNATFWUNSAFAlternateServerAttribute",
    "TestEncodeAndDecodeIPv6L",
    "FUNCTIONALITY",
    SetupL, TestEncodeAndDecodeIPv6L, Teardown)

EUNIT_TEST(
    "TestIPv6DecodeL - test ",
    "CNATFWUNSAFAlternateServerAttribute",
    "TestIPv6DecodeL",
    "FUNCTIONALITY",
    SetupL, TestIPv6DecodeL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
