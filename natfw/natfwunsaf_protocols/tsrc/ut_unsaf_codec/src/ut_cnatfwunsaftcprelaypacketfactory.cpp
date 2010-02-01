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
#include "UT_CNATFWUNSAFTcpRelayPacketFactory.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "natfwunsaftcprelaypacketfactory.h"
#include "natfwunsafmessagefactory.h"
#include "natfwunsaftcprelaypacket.h"

// CONSTRUCTION
UT_CNATFWUNSAFTcpRelayPacketFactory* UT_CNATFWUNSAFTcpRelayPacketFactory::NewL()
    {
    UT_CNATFWUNSAFTcpRelayPacketFactory* self = UT_CNATFWUNSAFTcpRelayPacketFactory::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFTcpRelayPacketFactory* UT_CNATFWUNSAFTcpRelayPacketFactory::NewLC()
    {
    UT_CNATFWUNSAFTcpRelayPacketFactory* self = new( ELeave ) UT_CNATFWUNSAFTcpRelayPacketFactory();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFTcpRelayPacketFactory::~UT_CNATFWUNSAFTcpRelayPacketFactory()
    {
    }

// Default constructor
UT_CNATFWUNSAFTcpRelayPacketFactory::UT_CNATFWUNSAFTcpRelayPacketFactory()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFTcpRelayPacketFactory::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CNATFWUNSAFTcpRelayPacketFactory::SetupL(  )
    {
    iFactory = CNATFWUNSAFTcpRelayPacketFactory::NewL();
    }

void UT_CNATFWUNSAFTcpRelayPacketFactory::Teardown(  )
    {
    delete iFactory;
    iFactory = NULL;
    }


 void UT_CNATFWUNSAFTcpRelayPacketFactory::TestRelayLengthL()
    {
    EUNIT_ASSERT(KErrNotFound == iFactory->MessageLength(KNullDesC8))
    }

 // Decode: Unframed STUN Binding Request
 void UT_CNATFWUNSAFTcpRelayPacketFactory::TestDecodeL()
    {
    const TUint8 KMessageToDecode[] =
        {
        0, 1, //UNSAF message type: Binding Request
        0, 0, //Message length
        0x21, 0x12, 0xa4, 0x42, //Magic cookie
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb //TransactionID
        };


    HBufC8* byteStream = HBufC8::NewLC(sizeof(KMessageToDecode));
    byteStream->Des().Append(KMessageToDecode, sizeof(KMessageToDecode));
    TInt len = iFactory->MessageLength(*byteStream);
    //Length should be zero as there is no framing in the message
    EUNIT_ASSERT(0 == iFactory->MessageLength(*byteStream));
    //DecodeL should leave as there is no framing in the message
    EUNIT_ASSERT_LEAVE(iFactory->DecodeL(*byteStream));
    CleanupStack::PopAndDestroy(byteStream);
    }

// Decode: Framed Data
void UT_CNATFWUNSAFTcpRelayPacketFactory::TestDecode2L()
    {
    const TUint8 KMessageToDecode[] =
        {
        3, 0,    //Relay type: Data
        0, 0x14, //Data length 20 bytes
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa,
        0xb, 0xc, 0xf, 0, 1, 2, 3, 4, 5
        };


    HBufC8* byteStream = HBufC8::NewLC(sizeof(KMessageToDecode));
    byteStream->Des().Append(KMessageToDecode, sizeof(KMessageToDecode));
    TInt len = iFactory->MessageLength(*byteStream);
    EUNIT_ASSERT(0x14 == iFactory->MessageLength(*byteStream));

    CNATFWUNSAFTcpRelayPacket* packet = iFactory->DecodeL(*byteStream);
    CleanupStack::PopAndDestroy(byteStream);
    CleanupStack::PushL(packet);
    EUNIT_ASSERT(0x14 == packet->Data().Length())
    EUNIT_ASSERT(CNATFWUNSAFTcpRelayPacket::EFrameTypeData == packet->Type());
    CleanupStack::PopAndDestroy(packet);
    }

// Decode: Framed SEND-INDICATION message
void UT_CNATFWUNSAFTcpRelayPacketFactory::TestDecode3L()
    {
    const TUint8 KMessageToDecode[] =
        {
        2, 0,    //Relay type: STUN
        0, 0x14, //Data length 20 bytes
        0, 0x11, //STUN msg type: SEND-INDICATION (turn draft-03!!!)
        0, 0,    //STUN msg length: 0 bytes
        0x21, 0x12, 0xa4, 0x42, //Magic cookie
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb // TransactionID
        };


    HBufC8* byteStream = HBufC8::NewLC(sizeof(KMessageToDecode));
    byteStream->Des().Append(KMessageToDecode, sizeof(KMessageToDecode));
    EUNIT_ASSERT(0x14 == iFactory->MessageLength(*byteStream));

    CNATFWUNSAFTcpRelayPacket* packet = iFactory->DecodeL(*byteStream);
    CleanupStack::PopAndDestroy(byteStream);
    CleanupStack::PushL(packet);
    EUNIT_ASSERT(0x14 == packet->Data().Length() )
    EUNIT_ASSERT(CNATFWUNSAFTcpRelayPacket::EFrameTypeStun == packet->Type());

    HBufC8* stunMsgBuf = HBufC8::NewLC(packet->Data().Length());
    stunMsgBuf->Des().Copy(packet->Data());
    CNATFWUNSAFMessageFactory* msgFactory = CNATFWUNSAFMessageFactory::NewLC();
    CNATFWUNSAFMessage* msg = msgFactory->DecodeL(*stunMsgBuf);
    CleanupStack::PushL(msg);

    EUNIT_ASSERT(msg->Validate());
    EUNIT_ASSERT(CNATFWUNSAFMessage::ESendIndication == msg->Type());
    EUNIT_ASSERT(0 == msg->iAttributes.Count());

    TNATFWUNSAFTransactionID taID = msg->TransactionID();
    for (TInt i=0; i < KMaxNATFWUNSAFTransactionIdLength; ++i)
        {
        EUNIT_ASSERT(i == taID[i]);
        }
    CleanupStack::PopAndDestroy(msg);
    CleanupStack::PopAndDestroy(msgFactory);
    CleanupStack::PopAndDestroy(stunMsgBuf);
    CleanupStack::PopAndDestroy(packet);
    }

void UT_CNATFWUNSAFTcpRelayPacketFactory::TestCreateL()
    {
    // Create framed empty data relay packet
    CNATFWUNSAFTcpRelayPacket* packet = iFactory->CreateL(
        CNATFWUNSAFTcpRelayPacket::EFrameTypeData, KNullDesC8);
    CleanupStack::PushL(packet);
    EUNIT_ASSERT(CNATFWUNSAFTcpRelayPacket::EFrameTypeData == packet->Type());
    EUNIT_ASSERT(0 == packet->Data().Length());
    CleanupStack::PopAndDestroy(packet);

    // Create framed STUN relay packet containing a Send Indication
    const TUint8 KMessageToEncode[] =
        {
        0, 0x11, //UNSAF message type: SEND-INDICATION (turn draft-03!!!)
        0, 0,    //Message length
        0x21, 0x12, 0xa4, 0x42, //Magic cookie
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb //TransactionID
        };
    HBufC8* byteStream = HBufC8::NewLC(sizeof(KMessageToEncode));
    byteStream->Des().Append(KMessageToEncode,sizeof(KMessageToEncode));
    packet = iFactory->CreateL(CNATFWUNSAFTcpRelayPacket::EFrameTypeStun, *byteStream);
    CleanupStack::PopAndDestroy(byteStream);
    CleanupStack::PushL(packet);
    EUNIT_ASSERT(CNATFWUNSAFTcpRelayPacket::EFrameTypeStun == packet->Type());
    EUNIT_ASSERT(0x14 == packet->Data().Length());
    HBufC8* stunMsgBuf = HBufC8::NewLC(packet->Data().Length());
    stunMsgBuf->Des().Copy(packet->Data());
    CNATFWUNSAFMessageFactory* msgFactory = CNATFWUNSAFMessageFactory::NewLC();
    CNATFWUNSAFMessage* msg = msgFactory->DecodeL(*stunMsgBuf);
    CleanupStack::PushL(msg);

    EUNIT_ASSERT(msg->Validate());
    EUNIT_ASSERT(CNATFWUNSAFMessage::ESendIndication == msg->Type());
    EUNIT_ASSERT(0 == msg->iAttributes.Count());

    TNATFWUNSAFTransactionID taID = msg->TransactionID();
    for (TInt i=0; i < KMaxNATFWUNSAFTransactionIdLength; ++i)
        {
        EUNIT_ASSERT(i == taID[i]);
        }
    CleanupStack::PopAndDestroy(msg);
    CleanupStack::PopAndDestroy(msgFactory);
    CleanupStack::PopAndDestroy(stunMsgBuf);
    CleanupStack::PopAndDestroy(packet);
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFTcpRelayPacketFactory,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestRelayLengthL - test ",
    "CNATFWUNSAFTcpRelayPacketFactory",
    "TestRelayLengthL",
    "FUNCTIONALITY",
    SetupL, TestRelayLengthL, Teardown)

EUNIT_TEST(
    "TestDecodeL - test ",
    "CNATFWUNSAFTcpRelayPacketFactory",
    "TestDecodeL",
    "FUNCTIONALITY",
    SetupL, TestDecodeL, Teardown)

EUNIT_TEST(
    "TestDecode2L - test ",
    "CNATFWUNSAFTcpRelayPacketFactory",
    "TestDecode2L",
    "FUNCTIONALITY",
    SetupL, TestDecode2L, Teardown)

EUNIT_TEST(
    "TestDecode3L - test ",
    "CNATFWUNSAFTcpRelayPacketFactory",
    "TestDecode3L",
    "FUNCTIONALITY",
    SetupL, TestDecode3L, Teardown)

EUNIT_TEST(
    "TestCreateL - test ",
    "CNATFWUNSAFTcpRelayPacketFactory",
    "TestCreateL",
    "FUNCTIONALITY",
    SetupL, TestCreateL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
