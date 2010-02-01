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
#include "ut_cnatfwunsafxormappedaddressattribute.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "natfwunsafxormappedaddressattribute.h"

_LIT(KAddr, "1.2.3.4");
_LIT8(KTransactionId, "123456789012");


// CONSTRUCTION
UT_CNATFWUNSAFXorMappedAddressAttribute* UT_CNATFWUNSAFXorMappedAddressAttribute::NewL()
    {
    UT_CNATFWUNSAFXorMappedAddressAttribute* self = UT_CNATFWUNSAFXorMappedAddressAttribute::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFXorMappedAddressAttribute* UT_CNATFWUNSAFXorMappedAddressAttribute::NewLC()
    {
    UT_CNATFWUNSAFXorMappedAddressAttribute* self = new( ELeave ) UT_CNATFWUNSAFXorMappedAddressAttribute();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFXorMappedAddressAttribute::~UT_CNATFWUNSAFXorMappedAddressAttribute()
    {
    }

// Default constructor
UT_CNATFWUNSAFXorMappedAddressAttribute::UT_CNATFWUNSAFXorMappedAddressAttribute()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFXorMappedAddressAttribute::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CNATFWUNSAFXorMappedAddressAttribute::SetupL()
    {
    TInetAddr addr;
    User::LeaveIfError(addr.Input(KAddr));
    TNATFWUNSAFTransactionID taID(KTransactionId);
    iAttribute = CNATFWUNSAFXorMappedAddressAttribute::NewL(addr, taID);
    }


void UT_CNATFWUNSAFXorMappedAddressAttribute::Teardown()
    {
    delete iAttribute;
    iAttribute = NULL;
    }


void UT_CNATFWUNSAFXorMappedAddressAttribute::TestEncodeAndDecodeL()
    {
    HBufC8* encodedAttr = iAttribute->EncodeL();
    CleanupStack::PushL(encodedAttr);

    CNATFWUNSAFAttribute* clone =
        CNATFWUNSAFXorMappedAddressAttribute::DecodeAttributeL(*encodedAttr);
    CleanupStack::PushL(clone);
    CNATFWUNSAFXorMappedAddressAttribute* castedClone =
        static_cast<CNATFWUNSAFXorMappedAddressAttribute*>(clone);

    // Test before setting the transaction id
    EUNIT_ASSERT(castedClone->TransactionID().CompareF(KTransactionId) != 0);
    // Note this works only with IPv4 address
    EUNIT_ASSERT(castedClone->Address().Address() !=
            iAttribute->Address().Address());
    EUNIT_ASSERT(castedClone->Address().Port() !=
            iAttribute->Address().Port());

    // Test after setting the transaction id
    TNATFWUNSAFTransactionID taID(KTransactionId);
    castedClone->SetTransactionIDAndXorL(taID);
    EUNIT_ASSERT(!castedClone->TransactionID().CompareF(
                        iAttribute->TransactionID()));
    // Note this works only with IPv4 address
    EUNIT_ASSERT(castedClone->Address().Address() ==
                      iAttribute->Address().Address());
    EUNIT_ASSERT(castedClone->Address().Port() ==
                      iAttribute->Address().Port());

    CleanupStack::PopAndDestroy(clone);

    CleanupStack::PopAndDestroy(encodedAttr);
    }


void UT_CNATFWUNSAFXorMappedAddressAttribute::TestEncodeAndDecodeIPv6L()
    {
    //Create an IPv6 address
    TIp6Addr addrIPv6;
    addrIPv6.u.iAddr8[0] = 0x50;
    addrIPv6.u.iAddr8[1] = 0x40;
    addrIPv6.u.iAddr8[2] = 0xff;
    addrIPv6.u.iAddr8[3] = 0xaa;
    addrIPv6.u.iAddr8[4] = 0x00;
    addrIPv6.u.iAddr8[5] = 0x01;
    addrIPv6.u.iAddr8[6] = 0x10;
    addrIPv6.u.iAddr8[7] = 0xfb;
    addrIPv6.u.iAddr8[8] = 0xe0;
    addrIPv6.u.iAddr8[9] = 0x76;
    addrIPv6.u.iAddr8[10] = 0x58;
    addrIPv6.u.iAddr8[11] = 0x00;
    addrIPv6.u.iAddr8[12] = 0x19;
    addrIPv6.u.iAddr8[13] = 0xab;
    addrIPv6.u.iAddr8[14] = 0x30;
    addrIPv6.u.iAddr8[15] = 0x0c;
    TInetAddr addr;
    addr.SetAddress(addrIPv6);
    addr.SetPort(5060);

    TNATFWUNSAFTransactionID taID(KTransactionId);

    delete iAttribute;
    iAttribute = NULL;
    iAttribute = CNATFWUNSAFXorMappedAddressAttribute::NewL(addr, taID);



    HBufC8* encodedAttr = iAttribute->EncodeL();
    CleanupStack::PushL(encodedAttr);

    CNATFWUNSAFAttribute* clone =
        CNATFWUNSAFXorMappedAddressAttribute::DecodeAttributeL(*encodedAttr);
    CleanupStack::PushL(clone);
    CNATFWUNSAFXorMappedAddressAttribute* castedClone =
        static_cast<CNATFWUNSAFXorMappedAddressAttribute*>(clone);

    // Test before setting the transaction id
    EUNIT_ASSERT(castedClone->TransactionID().CompareF(KTransactionId) != 0);
    const TIp6Addr& origAddr = iAttribute->Address().Ip6Address();
    const TIp6Addr& cloneAddr = castedClone->Address().Ip6Address();
    const TInt KBitsInIPv6Address = 128;
    EUNIT_ASSERT(origAddr.Match(cloneAddr) < KBitsInIPv6Address);
    EUNIT_ASSERT(iAttribute->Address().Port() != castedClone->Address().Port());


    // Test after setting the transaction id
    castedClone->SetTransactionIDAndXorL(taID);
    EUNIT_ASSERT(!castedClone->TransactionID().CompareF(
                        iAttribute->TransactionID()));
    // Whole addresses must be same
    EUNIT_ASSERT(origAddr.Match(cloneAddr) == KBitsInIPv6Address);
    EUNIT_ASSERT(iAttribute->Address().Port() ==
                      castedClone->Address().Port());

    CleanupStack::PopAndDestroy(clone);

    CleanupStack::PopAndDestroy(encodedAttr);
    }


void UT_CNATFWUNSAFXorMappedAddressAttribute::TestTypeL()
    {
    EUNIT_ASSERT(CNATFWUNSAFAttribute::EXorMappedAddress == iAttribute->Type());
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFXorMappedAddressAttribute,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestEncodeAndDecodeL - test ",
    "CNATFWUNSAFXorMappedAddressAttribute",
    "TestEncodeAndDecodeL",
    "FUNCTIONALITY",
    SetupL, TestEncodeAndDecodeL, Teardown)

EUNIT_TEST(
    "TestEncodeAndDecodeIPv6L - test ",
    "CNATFWUNSAFXorMappedAddressAttribute",
    "TestEncodeAndDecodeIPv6L",
    "FUNCTIONALITY",
    SetupL, TestEncodeAndDecodeIPv6L, Teardown)

EUNIT_TEST(
    "TestTypeL - test ",
    "CNATFWUNSAFXorMappedAddressAttribute",
    "TestTypeL",
    "FUNCTIONALITY",
    SetupL, TestTypeL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
