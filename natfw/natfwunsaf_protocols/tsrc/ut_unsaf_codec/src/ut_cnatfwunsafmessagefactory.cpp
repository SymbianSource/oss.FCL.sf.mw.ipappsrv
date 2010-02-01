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
#include "UT_CNATFWUNSAFMessageFactory.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <in_sock.h>
#include <hash.h>


//  INTERNAL INCLUDES
#include "natfwunsafcommontestdefs.h"
#include "natfwunsafmessagefactory.h"
#include "natfwunsafaddressattribute.h"
#include "natfwunsaferrorcodeattribute.h"
#include "natfwunsafunknownattributesattribute.h"


// CONSTRUCTION
UT_CNATFWUNSAFMessageFactory* UT_CNATFWUNSAFMessageFactory::NewL()
    {
    UT_CNATFWUNSAFMessageFactory* self = UT_CNATFWUNSAFMessageFactory::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFMessageFactory* UT_CNATFWUNSAFMessageFactory::NewLC()
    {
    UT_CNATFWUNSAFMessageFactory* self = new( ELeave ) UT_CNATFWUNSAFMessageFactory();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFMessageFactory::~UT_CNATFWUNSAFMessageFactory()
    {
    }

// Default constructor
UT_CNATFWUNSAFMessageFactory::UT_CNATFWUNSAFMessageFactory()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFMessageFactory::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CNATFWUNSAFMessageFactory::SetupL ()
    {
    iFactory = CNATFWUNSAFMessageFactory::NewL();
    }


void UT_CNATFWUNSAFMessageFactory::Teardown()
    {
    delete iFactory;
    iFactory = NULL;
    }


void UT_CNATFWUNSAFMessageFactory::TestMessageLengthL()
    {
    EUNIT_ASSERT(KErrNotFound == iFactory->MessageLength(KNullDesC8));
    }


//Decode UNSAF Binding request without any attributes
void UT_CNATFWUNSAFMessageFactory::TestDecodeL()
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
    EUNIT_ASSERT(0 == iFactory->MessageLength(*byteStream));

    CNATFWUNSAFMessage* msg = iFactory->DecodeL(*byteStream);
    CleanupStack::PopAndDestroy(byteStream);
    CleanupStack::PushL(msg);
    EUNIT_ASSERT(msg->Validate());
    EUNIT_ASSERT(msg->IsAllowed(CNATFWUNSAFAttribute::EResponseAddress));
    EUNIT_ASSERT(!msg->IsAllowed(CNATFWUNSAFAttribute::EErrorCode));

    EUNIT_ASSERT(CNATFWUNSAFMessage::EBindingRequest == msg->Type());
    EUNIT_ASSERT(0 == msg->iAttributes.Count());

    TNATFWUNSAFTransactionID taID = msg->TransactionID();
    for (TInt i=0; i < KMaxNATFWUNSAFTransactionIdLength; ++i)
        {
        EUNIT_ASSERT(i == taID[i]);
        }

    CleanupStack::PopAndDestroy(msg);
    }


//Decode UNSAF Binding response with all three mandatory attributes
void UT_CNATFWUNSAFMessageFactory::TestDecode2L()
    {
    //Message length with three address attributes
    const TInt KAttributeCount = 3;
    TUint8 msgLen = (CNATFWUNSAFAttribute::EValueOffset +
                     CNATFWUNSAFAddressAttribute::EAttributeValueSizeIPv4) *
                    KAttributeCount;

    const TUint8 KMessageToDecode[] =
        {
        1, 1, //UNSAF message type: Binding Response
        0, msgLen, //Message length
        0x21, 0x12, 0xa4, 0x42, //Magic cookie
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, //TransactionID

        //MAPPED-ADDRESS attribute
        0, 1, //type
        //length of value element
        0, CNATFWUNSAFAddressAttribute::EAttributeValueSizeIPv4,
        0, //undefined
        1, //family
        0x12, 0x34, //port
        0x50, 0x40, 0xff, 0xaa, //address

        //SOURCE-ADDRESS attribute
        0, 4, //type
        //length of value element
        0, CNATFWUNSAFAddressAttribute::EAttributeValueSizeIPv4,
        0, //undefined
        1, //family
        0x13, 0xC4, //port (5060 in decimal)
        1, 2, 3, 4, //address (1.2.3.4)

        //CHANGED-ADDRESS attribute
        0, 5, //type
        //length of value element
        0, CNATFWUNSAFAddressAttribute::EAttributeValueSizeIPv4,
        0, //undefined
        1, //family
        0x13, 0xC5, //port (5061 in decimal)
        2, 3, 4, 5  //address (2.3.4.5)
        };


    HBufC8* byteStream = HBufC8::NewLC(sizeof(KMessageToDecode));
    byteStream->Des().Append(KMessageToDecode, sizeof(KMessageToDecode));
    EUNIT_ASSERT(msgLen == iFactory->MessageLength(*byteStream));

    CNATFWUNSAFMessage* msg = iFactory->DecodeL(*byteStream);
    CleanupStack::PopAndDestroy(byteStream);
    CleanupStack::PushL(msg);
    EUNIT_ASSERT(msg->Validate());
    EUNIT_ASSERT(!msg->IsAllowed(CNATFWUNSAFAttribute::EErrorCode));

    //Verify UNSAF message is correctly decoded
    EUNIT_ASSERT(CNATFWUNSAFMessage::EBindingResponse == msg->Type());
    EUNIT_ASSERT(KAttributeCount == msg->iAttributes.Count());

    TNATFWUNSAFTransactionID taID = msg->TransactionID();
    for (TInt i=0; i < KMaxNATFWUNSAFTransactionIdLength; ++i)
        {
        EUNIT_ASSERT(i == taID[i]);
        }

    //Verify attributes are correctly decoded
    EUNIT_ASSERT(msg->IsAllowed(CNATFWUNSAFAttribute::EMappedAddress));
    CNATFWUNSAFAttribute* attr = msg->Attribute(
        CNATFWUNSAFAttribute::EMappedAddress);
    EUNIT_ASSERT(attr != NULL);

    const TInetAddr& addr =
        static_cast<CNATFWUNSAFAddressAttribute*>(attr)->Address();
    TUint32 expectedAddress = INET_ADDR(0x50, 0x40, 0xff, 0xaa);
    EUNIT_ASSERT(expectedAddress == addr.Address());
    EUNIT_ASSERT(0x1234 == addr.Port());

    EUNIT_ASSERT(msg->IsAllowed(CNATFWUNSAFAttribute::ESourceAddress));
    attr = msg->Attribute(CNATFWUNSAFAttribute::ESourceAddress);
    EUNIT_ASSERT(attr != NULL);

    const TInetAddr& addr2 =
        static_cast<CNATFWUNSAFAddressAttribute*>(attr)->Address();
    expectedAddress = INET_ADDR(1, 2, 3, 4);
    EUNIT_ASSERT(expectedAddress == addr2.Address());
    EUNIT_ASSERT(5060 == addr2.Port());

    EUNIT_ASSERT(msg->IsAllowed(CNATFWUNSAFAttribute::EChangedAddress));
    attr = msg->Attribute(CNATFWUNSAFAttribute::EChangedAddress);
    EUNIT_ASSERT(attr != NULL);

    const TInetAddr& addr3 =
        static_cast<CNATFWUNSAFAddressAttribute*>(attr)->Address();
    expectedAddress = INET_ADDR(2, 3, 4, 5);
    EUNIT_ASSERT(expectedAddress == addr3.Address());
    EUNIT_ASSERT(5061 == addr3.Port());

    EUNIT_ASSERT(EFalse == msg->HasUnknownMandatoryAttributes());

    CleanupStack::PopAndDestroy(msg);
    }


//Decode UNSAF Binding response with all three mandatory attributes and one
//unrecognized optional attribute.
void UT_CNATFWUNSAFMessageFactory::TestDecode3L()
    {
    //Message length with three address attributes
    const TInt KRecognizedAttributeCount = 3;
    const TInt KUnrecognizedAttributeLength = 4;
    TUint8 msgLen = ((CNATFWUNSAFAttribute::EValueOffset +
                      CNATFWUNSAFAddressAttribute::EAttributeValueSizeIPv4) *
                     KRecognizedAttributeCount)
                     +
                     //unrecognized attribute
                     CNATFWUNSAFAttribute::EValueOffset +
                     KUnrecognizedAttributeLength;

    const TUint8 KMessageToDecode[] =
        {
        1, 1, //UNSAF message type: Binding Response
        0, msgLen, //Message length
        0x21, 0x12, 0xa4, 0x42, //Magic cookie
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, //TransactionID

        //MAPPED-ADDRESS attribute
        0, 1, //type
        //length of value element
        0, CNATFWUNSAFAddressAttribute::EAttributeValueSizeIPv4,
        0, //undefined
        1, //family
        0x12, 0x34, //port
        0x50, 0x40, 0xff, 0xaa, //address

        //Unrecognized attribute
        0x80, 00, //unknown value, but it is optional (since larger than 0x7fff)
        0, KUnrecognizedAttributeLength, //length of value element
        0xaa, 0xbb, 0xcc, 0xdd, //attribute value

        //SOURCE-ADDRESS attribute
        0, 4, //type
        //length of value element
        0, CNATFWUNSAFAddressAttribute::EAttributeValueSizeIPv4,
        0, //undefined
        1, //family
        0x13, 0xC4, //port (5060 in decimal)
        1, 2, 3, 4, //address (1.2.3.4)

        //CHANGED-ADDRESS attribute
        0, 5, //type
        //length of value element
        0, CNATFWUNSAFAddressAttribute::EAttributeValueSizeIPv4,
        0, //undefined
        1, //family
        0x13, 0xC5, //port (5061 in decimal)
        2, 3, 4, 5  //address (2.3.4.5)
        };


    HBufC8* byteStream = HBufC8::NewLC(sizeof(KMessageToDecode));
    byteStream->Des().Append(KMessageToDecode, sizeof(KMessageToDecode));
    EUNIT_ASSERT(msgLen == iFactory->MessageLength(*byteStream));

    CNATFWUNSAFMessage* msg = iFactory->DecodeL(*byteStream);
    CleanupStack::PopAndDestroy(byteStream);
    CleanupStack::PushL(msg);
    EUNIT_ASSERT(msg->Validate());

    //Verify UNSAF message is correctly decoded
    EUNIT_ASSERT(CNATFWUNSAFMessage::EBindingResponse == msg->Type());
    //Unrecognized optional attribute must've been ignored
    EUNIT_ASSERT(KRecognizedAttributeCount == msg->iAttributes.Count());

    TNATFWUNSAFTransactionID taID = msg->TransactionID();
    for (TInt i=0; i < KMaxNATFWUNSAFTransactionIdLength; ++i)
        {
        EUNIT_ASSERT(i == taID[i]);
        }

    //Verify attributes are correctly decoded
    CNATFWUNSAFAttribute* attr = msg->Attribute(CNATFWUNSAFAttribute::EMappedAddress);
    EUNIT_ASSERT(attr != NULL);

    const TInetAddr& addr =
        static_cast<CNATFWUNSAFAddressAttribute*>(attr)->Address();
    TUint32 expectedAddress = INET_ADDR(0x50, 0x40, 0xff, 0xaa);
    EUNIT_ASSERT(expectedAddress == addr.Address());
    EUNIT_ASSERT(0x1234 == addr.Port());


    attr = msg->Attribute(CNATFWUNSAFAttribute::ESourceAddress);
    EUNIT_ASSERT(attr != NULL);

    const TInetAddr& addr2 =
        static_cast<CNATFWUNSAFAddressAttribute*>(attr)->Address();
    expectedAddress = INET_ADDR(1, 2, 3, 4);
    EUNIT_ASSERT(expectedAddress == addr2.Address());
    EUNIT_ASSERT(5060 == addr2.Port());


    attr = msg->Attribute(CNATFWUNSAFAttribute::EChangedAddress);
    EUNIT_ASSERT(attr != NULL);

    const TInetAddr& addr3 =
        static_cast<CNATFWUNSAFAddressAttribute*>(attr)->Address();
    expectedAddress = INET_ADDR(2, 3, 4, 5);
    EUNIT_ASSERT(expectedAddress == addr3.Address());
    EUNIT_ASSERT(5061 == addr3.Port());

    EUNIT_ASSERT(EFalse == msg->HasUnknownMandatoryAttributes());

    CleanupStack::PopAndDestroy(msg);
    }


//Decode UNSAF Binding Error response with unrecognized mandatory attribute.
//Decoding should succeed and should set the
//CNATFWUNSAFMessage::iUnknownMandatoryAttributes flag.
void UT_CNATFWUNSAFMessageFactory::TestDecode4L()
    {
    //Message length with three address attributes
    const TInt KUnrecognizedAttributeLength = 12;
    TUint8 msgLen = CNATFWUNSAFAttribute::EValueOffset +
                    KUnrecognizedAttributeLength;

    const TUint8 KMessageToDecode[] =
        {
        0x01, 0x11, //UNSAF message type: Binding Response
        0, msgLen, //Message length
        0x21, 0x12, 0xa4, 0x42, //Magic cookie
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, //TransactionID

        //Unrecognized attribute
        0x7f, 0xff, //unknown mandatory attribute
        0, KUnrecognizedAttributeLength, //length of value element
        0xaa, 0xbb, 0xcc, 0xdd,
        0xaa, 0xbb, 0xcc, 0xdd,
        0xaa, 0xbb, 0xcc, 0xdd //attribute value
        };

    HBufC8* byteStream = HBufC8::NewLC(sizeof(KMessageToDecode));
    byteStream->Des().Append(KMessageToDecode, sizeof(KMessageToDecode));
    EUNIT_ASSERT(msgLen == iFactory->MessageLength(*byteStream));

    CNATFWUNSAFMessage* msg = NULL;
    TRAPD(err, msg = iFactory->DecodeL(*byteStream));
    CleanupStack::PopAndDestroy(byteStream);
    CleanupStack::PushL(msg);

    //If alloc failure tool is "on", decoding can fail
    if (err == KErrNoMemory)
        {
        User::Leave(err);
        }
    EUNIT_ASSERT(err == KErrNone);
    EUNIT_ASSERT(ETrue == msg->HasUnknownMandatoryAttributes());

    CleanupStack::PopAndDestroy(msg);
    }


//Decode UNSAF Shared Secret error response with ERROR-CODE and
//UNKNOWN-ATTRIBUTES
void UT_CNATFWUNSAFMessageFactory::TestDecode5L()
    {
    const TUint8 KMessageToDecode[] =
        {
        0x01, 0x12, //UNSAF message type: Shared Secret error response
        0, 28, //Message length
        0x21, 0x12, 0xa4, 0x42, //Magic cookie
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, //TransactionID

        //ERROR-CODE attribute
        0, 9, //type
        0, 16, //length of value element
        0, 0, //zero
        4, //class
        8, //number
        'E', 'r', 'r', 'o',
        'r', ' ', 'r', 'e',
        's', 'u', 'l', 't',     //Reason Phrase

        //UNKNOWN-ATTRIBUTES attribute
        0, 0xa, //type
        0, 4, //length of value element
        0xab, 0xcd, //first unknown attribute
        0x10, 0x20  //second unknown attribute
        };

    HBufC8* byteStream = HBufC8::NewLC(sizeof(KMessageToDecode));
    byteStream->Des().Append(KMessageToDecode, sizeof(KMessageToDecode));

    CNATFWUNSAFMessage* msg = iFactory->DecodeL(*byteStream);
    CleanupStack::PopAndDestroy(byteStream);
    CleanupStack::PushL(msg);
    EUNIT_ASSERT(msg->Validate());
    EUNIT_ASSERT(!msg->IsAllowed(CNATFWUNSAFAttribute::EMappedAddress));

    //Verify UNSAF message is correctly decoded
    EUNIT_ASSERT(CNATFWUNSAFMessage::ESharedSecretErrorResponse == msg->Type());
    EUNIT_ASSERT(2 == msg->iAttributes.Count());

    //Verify attributes are correctly decoded
    EUNIT_ASSERT(msg->IsAllowed(CNATFWUNSAFAttribute::EErrorCode));
    CNATFWUNSAFAttribute* attr = msg->Attribute(CNATFWUNSAFAttribute::EErrorCode);
    EUNIT_ASSERT(attr != NULL);

    CNATFWUNSAFErrorCodeAttribute* ec =
        static_cast<CNATFWUNSAFErrorCodeAttribute*>(attr);

    EUNIT_ASSERT(408 == ec->ResponseCode());
    _LIT8(KExpectedPhrase, "Error result");
    EUNIT_ASSERT(!ec->ReasonPhrase().CompareF(KExpectedPhrase));

    EUNIT_ASSERT(msg->IsAllowed(CNATFWUNSAFAttribute::EUnknownAttributes));
    attr = msg->Attribute(CNATFWUNSAFAttribute::EUnknownAttributes);
    EUNIT_ASSERT(attr != NULL);

    CNATFWUNSAFUnknownAttributesAttribute* ua =
        static_cast<CNATFWUNSAFUnknownAttributesAttribute*>(attr);

    const RArray<TUint32>& array = ua->ContainedAttributes();
    EUNIT_ASSERT(2 == array.Count());
    EUNIT_ASSERT(0xabcd == array[0]);
    EUNIT_ASSERT(0x1020 == array[1]);

    CleanupStack::PopAndDestroy(msg);
    }


//Stream to be decoded does not begin with two zero bits
void UT_CNATFWUNSAFMessageFactory::TestDecodeWrongPrefixL()
    {
    const TUint8 KMessageToDecode[] =
        {
        4, 1, //Does not begin with two zeros
        0, 0, //Message length
        0x21, 0x12, 0xa4, 0x42, //Magic cookie
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb //TransactionID
        };


    HBufC8* byteStream = HBufC8::NewLC(sizeof(KMessageToDecode));
    byteStream->Des().Append(KMessageToDecode, sizeof(KMessageToDecode));
    EUNIT_ASSERT(0 == iFactory->MessageLength(*byteStream));

    CNATFWUNSAFMessage* msg = NULL;
    TRAPD(err, msg = iFactory->DecodeL(*byteStream));
    CleanupStack::PopAndDestroy(byteStream);
    delete msg;
    EUNIT_ASSERT(KErrArgument == err);
    }

//Stream to be decoded is missing the magic cookie
void UT_CNATFWUNSAFMessageFactory::TestDecodeNoCookieL()
    {
    const TUint8 KMessageToDecode[] =
        {
        0, 1, //UNSAF message type: Binding Request
        0, 0, //Message length
        0x21, 0x12, 0x42, 0xa4, //not the magic cookie
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb //TransactionID
        };


    HBufC8* byteStream = HBufC8::NewLC(sizeof(KMessageToDecode));
    byteStream->Des().Append(KMessageToDecode, sizeof(KMessageToDecode));
    EUNIT_ASSERT(0 == iFactory->MessageLength(*byteStream));

    CNATFWUNSAFMessage* msg = NULL;
    TRAPD(err, msg = iFactory->DecodeL(*byteStream));
    CleanupStack::PopAndDestroy(byteStream);
    delete msg;
    EUNIT_ASSERT(KErrArgument == err);
    }


//Decode an UNSAF message containing attributes having both IPv6 addresses and
//IPv4 addresses.
void UT_CNATFWUNSAFMessageFactory::TestDecodeIPv6AddressesL()
    {
    //Message length with three address attributes
    const TInt KRecognizedAttributeCount = 3;
    const TInt KUnrecognizedAttributeLength = 4;
    TUint8 msgLen = ((CNATFWUNSAFAttribute::EValueOffset +
                      CNATFWUNSAFAddressAttribute::EAttributeValueSizeIPv6) * 2)
                     +
                     CNATFWUNSAFAttribute::EValueOffset +
                     CNATFWUNSAFAddressAttribute::EAttributeValueSizeIPv4
                     +
                     //unrecognized attribute
                     CNATFWUNSAFAttribute::EValueOffset +
                     KUnrecognizedAttributeLength;

    const TUint8 KMessageToDecode[] =
        {
        1, 1, //UNSAF message type: Binding Response
        0, msgLen, //Message length
        0x21, 0x12, 0xa4, 0x42, //Magic cookie
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, //TransactionID

        //MAPPED-ADDRESS attribute
        0, 1, //type
        //length of value element
        0, CNATFWUNSAFAddressAttribute::EAttributeValueSizeIPv6,
        0, //undefined
        CNATFWUNSAFAddressAttribute::KAddressFamilyIPv6,
        0x12, 0x34, //port
        //IPv6 address: 5040 ffaa 0001 10fb e076 5800 19ab 300c
        0x50, 0x40, 0xff, 0xaa,
        0x00, 0x01, 0x10, 0xfb,
        0xe0, 0x76, 0x58, 0x00,
        0x19, 0xab, 0x30, 0x0c,

        //Unrecognized attribute
        0x90, 22, //unknown value, but it is optional (since larger than 0x7fff)
        0, KUnrecognizedAttributeLength, //length of value element
        0xaa, 0xbb, 0xcc, 0xdd, //attribute value

        //SOURCE-ADDRESS attribute
        0, 4, //type
        //length of value element
        0, CNATFWUNSAFAddressAttribute::EAttributeValueSizeIPv4,
        0, //undefined
        CNATFWUNSAFAddressAttribute::KAddressFamilyIPv4,
        0x13, 0xC4, //port (5060 in decimal)
        1, 2, 3, 4, //address (1.2.3.4)

        //CHANGED-ADDRESS attribute
        0, 5, //type
        //length of value element
        0, CNATFWUNSAFAddressAttribute::EAttributeValueSizeIPv6,
        0, //undefined
        CNATFWUNSAFAddressAttribute::KAddressFamilyIPv6,
        0x13, 0xC5, //port (5061 in decimal)
        0x00, 0x00, 0x00, 0xf0,
        0x20, 0x30, 0x20, 0x03,
        0x41, 0x00, 0xff, 0xff,
        0xbe, 0x10, 0x00, 0x54  //IPv6 address
        };


    HBufC8* byteStream = HBufC8::NewLC(sizeof(KMessageToDecode));
    byteStream->Des().Append(KMessageToDecode, sizeof(KMessageToDecode));
    EUNIT_ASSERT(msgLen == iFactory->MessageLength(*byteStream));

    CNATFWUNSAFMessage* msg = iFactory->DecodeL(*byteStream);
    CleanupStack::PopAndDestroy(byteStream);
    CleanupStack::PushL(msg);
    EUNIT_ASSERT(msg->Validate());

    //Verify UNSAF message is correctly decoded
    EUNIT_ASSERT(CNATFWUNSAFMessage::EBindingResponse == msg->Type());
    //Unrecognized optional attribute must've been ignored
    EUNIT_ASSERT(KRecognizedAttributeCount == msg->iAttributes.Count());

    //Verify attributes are correctly decoded
    CNATFWUNSAFAttribute* attr = msg->Attribute(CNATFWUNSAFAttribute::EMappedAddress);
    EUNIT_ASSERT(attr != NULL);

    const TInetAddr& addr =
        static_cast<CNATFWUNSAFAddressAttribute*>(attr)->Address();
    TInetAddr expectedAddr;
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
    expectedAddr.SetAddress(addrIPv6);
    expectedAddr.SetPort(0x1234);

    EUNIT_ASSERT(expectedAddr.CmpAddr(addr));
    EUNIT_ASSERT(0x1234 == addr.Port());


    attr = msg->Attribute(CNATFWUNSAFAttribute::ESourceAddress);
    EUNIT_ASSERT(attr != NULL);

    const TInetAddr& addr2 =
        static_cast<CNATFWUNSAFAddressAttribute*>(attr)->Address();
    expectedAddr.SetAddress(INET_ADDR(1, 2, 3, 4));
    expectedAddr.SetPort(5060);
    EUNIT_ASSERT(expectedAddr.CmpAddr(addr2));
    EUNIT_ASSERT(5060 == addr2.Port());


    attr = msg->Attribute(CNATFWUNSAFAttribute::EChangedAddress);
    EUNIT_ASSERT(attr != NULL);

    const TInetAddr& addr3 =
        static_cast<CNATFWUNSAFAddressAttribute*>(attr)->Address();
    addrIPv6.u.iAddr8[0] = 0x00;
    addrIPv6.u.iAddr8[1] = 0x00;
    addrIPv6.u.iAddr8[2] = 0x00;
    addrIPv6.u.iAddr8[3] = 0xf0;
    addrIPv6.u.iAddr8[4] = 0x20;
    addrIPv6.u.iAddr8[5] = 0x30;
    addrIPv6.u.iAddr8[6] = 0x20;
    addrIPv6.u.iAddr8[7] = 0x03;
    addrIPv6.u.iAddr8[8] = 0x41;
    addrIPv6.u.iAddr8[9] = 0x00;
    addrIPv6.u.iAddr8[10] = 0xff;
    addrIPv6.u.iAddr8[11] = 0xff;
    addrIPv6.u.iAddr8[12] = 0xbe;
    addrIPv6.u.iAddr8[13] = 0x10;
    addrIPv6.u.iAddr8[14] = 0x00;
    addrIPv6.u.iAddr8[15] = 0x54;
    expectedAddr.SetAddress(addrIPv6);
    expectedAddr.SetPort(5061);

    EUNIT_ASSERT(expectedAddr.CmpAddr(addr3));
    EUNIT_ASSERT(5061 == addr3.Port());

    EUNIT_ASSERT(EFalse == msg->HasUnknownMandatoryAttributes());

    CleanupStack::PopAndDestroy(msg);
    }


void UT_CNATFWUNSAFMessageFactory::TestCreateL()
    {
    CNATFWUNSAFMessage* msg = NULL;

    // Binding Request
    msg = CreateAndVerifyMsgLC(CNATFWUNSAFMessage::EBindingRequest);
    EUNIT_ASSERT(msg->Validate());
    EUNIT_ASSERT(!msg->IsAllowed(CNATFWUNSAFAttribute::EErrorCode));
    EUNIT_ASSERT(msg->IsAllowed(CNATFWUNSAFAttribute::EResponseAddress));
    EUNIT_ASSERT(msg->IsAllowed(CNATFWUNSAFAttribute::EChangeRequest));
    EUNIT_ASSERT(msg->IsAllowed(CNATFWUNSAFAttribute::EUsername));
    EUNIT_ASSERT(msg->IsAllowed(CNATFWUNSAFAttribute::EMessageIntegrity));
    CleanupStack::PopAndDestroy(msg);

    // Binding Response
    msg = CreateAndVerifyMsgLC(CNATFWUNSAFMessage::EBindingResponse);
    EUNIT_ASSERT(!msg->Validate());
    EUNIT_ASSERT(!msg->IsAllowed(CNATFWUNSAFAttribute::EErrorCode));
    EUNIT_ASSERT(msg->IsAllowed(CNATFWUNSAFAttribute::EMappedAddress));
    EUNIT_ASSERT(msg->IsAllowed(CNATFWUNSAFAttribute::ESourceAddress));
    EUNIT_ASSERT(msg->IsAllowed(CNATFWUNSAFAttribute::EChangedAddress));
    EUNIT_ASSERT(msg->IsAllowed(CNATFWUNSAFAttribute::EMessageIntegrity));
    EUNIT_ASSERT(msg->IsAllowed(CNATFWUNSAFAttribute::EReflectedFrom));
    CleanupStack::PopAndDestroy(msg);

    // Binding Error Response
    msg = CreateAndVerifyMsgLC(CNATFWUNSAFMessage::EBindingErrorResponse);
    EUNIT_ASSERT(!msg->Validate());
    EUNIT_ASSERT(!msg->IsAllowed(CNATFWUNSAFAttribute::EMappedAddress));
    EUNIT_ASSERT(msg->IsAllowed(CNATFWUNSAFAttribute::EErrorCode));
    EUNIT_ASSERT(msg->IsAllowed(CNATFWUNSAFAttribute::EUnknownAttributes));
    CleanupStack::PopAndDestroy(msg);

    // Shared Secret Request
    msg = CreateAndVerifyMsgLC(CNATFWUNSAFMessage::ESharedSecretRequest);
    EUNIT_ASSERT(msg->Validate());
    // No attributes allowed
    EUNIT_ASSERT(!msg->IsAllowed(CNATFWUNSAFAttribute::EMappedAddress));
    CleanupStack::PopAndDestroy(msg);

    // Shared Secret Response
    msg = CreateAndVerifyMsgLC(CNATFWUNSAFMessage::ESharedSecretResponse);
    EUNIT_ASSERT(!msg->Validate());
    EUNIT_ASSERT(!msg->IsAllowed(CNATFWUNSAFAttribute::EMappedAddress));
    EUNIT_ASSERT(msg->IsAllowed(CNATFWUNSAFAttribute::EUsername));
    EUNIT_ASSERT(msg->IsAllowed(CNATFWUNSAFAttribute::EPassword));
       CleanupStack::PopAndDestroy(msg);

       // Shared Secret Error Response
       msg = CreateAndVerifyMsgLC(CNATFWUNSAFMessage::ESharedSecretErrorResponse);
       EUNIT_ASSERT(!msg->Validate());
    EUNIT_ASSERT(!msg->IsAllowed(CNATFWUNSAFAttribute::EMappedAddress));
    EUNIT_ASSERT(msg->IsAllowed(CNATFWUNSAFAttribute::EErrorCode));
    EUNIT_ASSERT(msg->IsAllowed(CNATFWUNSAFAttribute::EUnknownAttributes));
    CleanupStack::PopAndDestroy(msg);
    }


CNATFWUNSAFMessage* UT_CNATFWUNSAFMessageFactory::CreateAndVerifyMsgLC(
    CNATFWUNSAFMessage::TType aType)
    {
    TNATFWUNSAFTransactionID taID = KDefaultTaId();
    CNATFWUNSAFMessage* msg = iFactory->CreateL(aType, taID);
    CleanupStack::PushL(msg);
    EUNIT_ASSERT(aType == msg->Type());
    return msg;
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFMessageFactory,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestMessageLengthL - test ",
    "CNATFWUNSAFMessageFactory",
    "TestMessageLengthL",
    "FUNCTIONALITY",
    SetupL, TestMessageLengthL, Teardown)

EUNIT_TEST(
    "TestDecodeL - test ",
    "CNATFWUNSAFMessageFactory",
    "TestDecodeL",
    "FUNCTIONALITY",
    SetupL, TestDecodeL, Teardown)

EUNIT_TEST(
    "TestDecode2L - test ",
    "CNATFWUNSAFMessageFactory",
    "TestDecode2L",
    "FUNCTIONALITY",
    SetupL, TestDecode2L, Teardown)

EUNIT_TEST(
    "TestDecode3L - test ",
    "CNATFWUNSAFMessageFactory",
    "TestDecode3L",
    "FUNCTIONALITY",
    SetupL, TestDecode3L, Teardown)

EUNIT_TEST(
    "TestDecode4L - test ",
    "CNATFWUNSAFMessageFactory",
    "TestDecode4L",
    "FUNCTIONALITY",
    SetupL, TestDecode4L, Teardown)

EUNIT_TEST(
    "TestDecode5L - test ",
    "CNATFWUNSAFMessageFactory",
    "TestDecode5L",
    "FUNCTIONALITY",
    SetupL, TestDecode5L, Teardown)

EUNIT_TEST(
    "TestDecodeWrongPrefixL - test ",
    "CNATFWUNSAFMessageFactory",
    "TestDecodeWrongPrefixL",
    "FUNCTIONALITY",
    SetupL, TestDecodeWrongPrefixL, Teardown)

EUNIT_TEST(
    "TestDecodeNoCookieL - test ",
    "CNATFWUNSAFMessageFactory",
    "TestDecodeNoCookieL",
    "FUNCTIONALITY",
    SetupL, TestDecodeNoCookieL, Teardown)

EUNIT_TEST(
    "TestDecodeIPv6AddressesL - test ",
    "CNATFWUNSAFMessageFactory",
    "TestDecodeIPv6AddressesL",
    "FUNCTIONALITY",
    SetupL, TestDecodeIPv6AddressesL, Teardown)

EUNIT_TEST(
    "TestCreateL - test ",
    "CNATFWUNSAFMessageFactory",
    "TestCreateL",
    "FUNCTIONALITY",
    SetupL, TestCreateL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
