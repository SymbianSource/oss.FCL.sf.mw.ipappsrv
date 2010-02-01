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
#include "UT_CNATFWUNSAFTcpRelayPacket.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "natfwunsaftcprelaypacket.h"
#include "natfwunsafsendindication.h"
#include "natfwunsaftransactionid.h"

// CONSTRUCTION
UT_CNATFWUNSAFTcpRelayPacket* UT_CNATFWUNSAFTcpRelayPacket::NewL()
    {
    UT_CNATFWUNSAFTcpRelayPacket* self = UT_CNATFWUNSAFTcpRelayPacket::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFTcpRelayPacket* UT_CNATFWUNSAFTcpRelayPacket::NewLC()
    {
    UT_CNATFWUNSAFTcpRelayPacket* self = new( ELeave ) UT_CNATFWUNSAFTcpRelayPacket();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFTcpRelayPacket::~UT_CNATFWUNSAFTcpRelayPacket()
    {
    }

// Default constructor
UT_CNATFWUNSAFTcpRelayPacket::UT_CNATFWUNSAFTcpRelayPacket()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFTcpRelayPacket::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_CNATFWUNSAFTcpRelayPacket::SetupL()
    {
    // Create a relay packet containing a SEND-INDICATION
    // Fill transaction id with dummy values
    _LIT8(KTaId, "............");
    TNATFWUNSAFTransactionID tID(KTaId);

    for (TInt i=0; i < KMaxNATFWUNSAFTransactionIdLength; ++i)
        {
        tID[i] = i;
        }

    CNATFWUNSAFSendIndication* msg =
        CNATFWUNSAFSendIndication::NewL(tID);
    CleanupStack::PushL(msg);
    CBufBase* data = msg->EncodeL();
    iPacket = CNATFWUNSAFTcpRelayPacket::NewL(
        data->Ptr(0), CNATFWUNSAFTcpRelayPacket::EFrameTypeStun);
    delete data;
    CleanupStack::PopAndDestroy(msg);
    }

void UT_CNATFWUNSAFTcpRelayPacket::Setup2L()
    {
    // Create a relay packet containing random data
    const TUint8 KRelayData[] =
        {
        0xff, 0xee, 0xff, 0xee, 0xff, 0xee, 0xff, 0xee, 0xff,
        0xee, 0xff, 0xee, 0xff, 0xee, 0xff, 0xee, 0xff, 0xee,
        0xff, 0xee, 0xff, 0xee, 0xff, 0xee, 0xff, 0xee, 0xff,
        0xee, 0xff, 0xee, 0xff, 0xee, 0xff, 0xee, 0xff, 0xee,
        0xff, 0xee, 0xff, 0xee, 0xff, 0xee, 0xff, 0xee, 0xff,
        0xee, 0xff, 0xee
        };

    HBufC8* byteStream = HBufC8::NewLC(sizeof(KRelayData));
    byteStream->Des().Append(KRelayData,sizeof(KRelayData));
    iPacket = CNATFWUNSAFTcpRelayPacket::NewL(
        *byteStream, CNATFWUNSAFTcpRelayPacket::EFrameTypeData);
    CleanupStack::PopAndDestroy(byteStream);
    }

void UT_CNATFWUNSAFTcpRelayPacket::Teardown()
    {
    delete iPacket;
    iPacket = NULL;
    }

void UT_CNATFWUNSAFTcpRelayPacket::TestEncodeL()
    {
    //Encode the relay packet created in SetupL
    const TUint8 KExpectedResult[] =
        {
        2, 0,    //UNSAF relay packet type: STUN
        0, 0x14, //Data length 20 bytes
        0, 0x11, //STUN msg type: SEND-INDICATION (turn draft-03!!!)
        0, 0,    //STUN msg length: 0 bytes
        0x21, 0x12, 0xa4, 0x42, //Magic cookie
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb // TransactionID
        };

    CBufBase* encodedPacket = iPacket->EncodeL();
    CleanupStack::PushL(encodedPacket);
    TPtr8 packetPtr = encodedPacket->Ptr(0);
    for (TInt i = 0; i < encodedPacket->Size(); ++i)
        {
        EUNIT_ASSERT(KExpectedResult[i] == packetPtr[i]);
        }
    CleanupStack::PopAndDestroy(encodedPacket);
    }

void UT_CNATFWUNSAFTcpRelayPacket::TestEncode2L()
    {
    //Encode the relay packet created in Setup2L
    const TUint8 KExpectedResult[] =
        {
        3, 0,     //UNSAF relay packet type: Data
        0, 0x30,  //Data length: 48 bytes
        0xff, 0xee, 0xff, 0xee, 0xff, 0xee, 0xff, 0xee, 0xff,
        0xee, 0xff, 0xee, 0xff, 0xee, 0xff, 0xee, 0xff, 0xee,
        0xff, 0xee, 0xff, 0xee, 0xff, 0xee, 0xff, 0xee, 0xff,
        0xee, 0xff, 0xee, 0xff, 0xee, 0xff, 0xee, 0xff, 0xee,
        0xff, 0xee, 0xff, 0xee, 0xff, 0xee, 0xff, 0xee, 0xff,
        0xee, 0xff, 0xee
        };

    CBufBase* encodedPacket = iPacket->EncodeL();
    CleanupStack::PushL(encodedPacket);
    TPtr8 packetPtr = encodedPacket->Ptr(0);
    for (TInt i = 0; i < encodedPacket->Size(); ++i)
        {
        EUNIT_ASSERT(KExpectedResult[i] == packetPtr[i]);
        }
    CleanupStack::PopAndDestroy(encodedPacket);
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFTcpRelayPacket,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestEncodeL - test ",
    "CNATFWUNSAFTcpRelayPacket",
    "TestEncodeL",
    "FUNCTIONALITY",
    SetupL, TestEncodeL, Teardown)

EUNIT_TEST(
    "TestEncode2L - test ",
    "CNATFWUNSAFTcpRelayPacket",
    "TestEncode2L",
    "FUNCTIONALITY",
    Setup2L, TestEncode2L, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
