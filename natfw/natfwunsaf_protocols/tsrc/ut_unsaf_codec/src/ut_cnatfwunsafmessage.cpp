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
#include "UT_CNATFWUNSAFMessage.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <in_sock.h>
#include <hash.h>


//  INTERNAL INCLUDES
#include "natfwunsafcommontestdefs.h"
#include "natfwunsafbindingrequest.h"
#include "natfwunsafmappedaddressattribute.h"
#include "natfwunsafresponseaddressattribute.h"
#include "natfwunsafchangedaddressattribute.h"
#include "natfwunsaferrorcodeattribute.h"
#include "natfwunsafunknownattributesattribute.h"
#include "natfwunsafxormappedAddressattribute.h"


// CONSTRUCTION
UT_CNATFWUNSAFMessage* UT_CNATFWUNSAFMessage::NewL()
    {
    UT_CNATFWUNSAFMessage* self = UT_CNATFWUNSAFMessage::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFMessage* UT_CNATFWUNSAFMessage::NewLC()
    {
    UT_CNATFWUNSAFMessage* self = new( ELeave ) UT_CNATFWUNSAFMessage();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFMessage::~UT_CNATFWUNSAFMessage()
    {
    }

// Default constructor
UT_CNATFWUNSAFMessage::UT_CNATFWUNSAFMessage()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFMessage::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CNATFWUNSAFMessage::SetupL ()
    {
    //Fill TransactionID with dummy values
    _LIT8(KTaId, "............");
    iTaID = KTaId;

    for (TInt i=0; i < KMaxNATFWUNSAFTransactionIdLength; ++i)
        {
        iTaID[i] = i;
        }

    iMsg = CNATFWUNSAFBindingRequest::NewL(iTaID);

    TInetAddr addr(0x10203040, 5060);
    iAttr = CNATFWUNSAFMappedAddressAttribute::NewL(addr);
    }

void UT_CNATFWUNSAFMessage::Teardown()
    {
    delete iMsg;
    iMsg = NULL;

    delete iAttr;
    iAttr = NULL;
    }

void UT_CNATFWUNSAFMessage::TestTransactionIDL()
    {
    TNATFWUNSAFTransactionID taID = iMsg->TransactionID();

    EUNIT_ASSERT(taID == iTaID);
    }

void UT_CNATFWUNSAFMessage::TestTransactionID2L()
    {
    TNATFWUNSAFTransactionID taID = iMsg->TransactionID();
    iTaID[10] = 0x55;

    EUNIT_ASSERT(!(taID == iTaID));
    }

void UT_CNATFWUNSAFMessage::TestTypeL()
    {
    EUNIT_ASSERT(CNATFWUNSAFMessage::EBindingRequest == iMsg->Type());
    }

void UT_CNATFWUNSAFMessage::TestAttributeL()
    {
    EUNIT_ASSERT(!iMsg->Attribute(CNATFWUNSAFAttribute::EMappedAddress));
    EUNIT_ASSERT(!iMsg->Attribute(CNATFWUNSAFAttribute::EResponseAddress));
    }

void UT_CNATFWUNSAFMessage::TestAttribute2L()
    {
    iMsg->AddAttributeL(iAttr);
    iAttr = NULL;

    EUNIT_ASSERT(!iMsg->Attribute(CNATFWUNSAFAttribute::EResponseAddress));
    EUNIT_ASSERT(iMsg->Attribute(CNATFWUNSAFAttribute::EMappedAddress) != NULL);
    }

void UT_CNATFWUNSAFMessage::TestAttribute3L()
    {
    iMsg->AddAttributeL(iAttr);
    iAttr = NULL;

    CNATFWUNSAFAttribute *attr = iMsg->Attribute(CNATFWUNSAFAttribute::EMappedAddress);
    iMsg->DetachAttribute(attr);
    CleanupStack::PushL(attr);

    EUNIT_ASSERT(!iMsg->Attribute(CNATFWUNSAFAttribute::EMappedAddress));

    CleanupStack::PopAndDestroy(attr);
    }

void UT_CNATFWUNSAFMessage::TestAttribute4L()
    {
    TInetAddr addr(0x1002009988, 1020);
    CNATFWUNSAFResponseAddressAttribute* ra =
        CNATFWUNSAFResponseAddressAttribute::NewLC(addr);
    iMsg->AddAttributeL(ra);
    CleanupStack::Pop(ra);

    iMsg->AddAttributeL(iAttr);
    iAttr = NULL;

    EUNIT_ASSERT(iMsg->Attribute(CNATFWUNSAFAttribute::EResponseAddress) != NULL);
    EUNIT_ASSERT(iMsg->Attribute(CNATFWUNSAFAttribute::EMappedAddress) != NULL);


    //Detach one attribute
    CNATFWUNSAFAttribute *attr = iMsg->Attribute(CNATFWUNSAFAttribute::EResponseAddress);
    iMsg->DetachAttribute(attr);
    CleanupStack::PushL(attr);

    EUNIT_ASSERT(!iMsg->Attribute(CNATFWUNSAFAttribute::EResponseAddress));
    EUNIT_ASSERT(iMsg->Attribute(CNATFWUNSAFAttribute::EMappedAddress) != NULL);

    CleanupStack::PopAndDestroy(attr);


    //Detach second attribute
    attr = iMsg->Attribute(CNATFWUNSAFAttribute::EMappedAddress);
    iMsg->DetachAttribute(attr);
    delete attr;

    EUNIT_ASSERT(!iMsg->Attribute(CNATFWUNSAFAttribute::EResponseAddress));
    EUNIT_ASSERT(!iMsg->Attribute(CNATFWUNSAFAttribute::EMappedAddress));
    }

void UT_CNATFWUNSAFMessage::TestAddAttributeL()
    {
    EUNIT_ASSERT(iMsg->iAttributes.Count() == 0);

    iMsg->AddAttributeL(iAttr);
    CNATFWUNSAFAttribute* attr = iAttr;
    iAttr = NULL;

    EUNIT_ASSERT(iMsg->iAttributes.Count() == 1);
    //Pointers much match
    EUNIT_ASSERT(iMsg->iAttributes[0] == attr);
    }

void UT_CNATFWUNSAFMessage::TestDetachAttributeL()
    {
    iMsg->AddAttributeL(iAttr);
    EUNIT_ASSERT(iMsg->iAttributes.Count() == 1);

    CNATFWUNSAFAttribute* attr = iAttr;
    iAttr = NULL;

    iMsg->DetachAttribute(attr);
    CleanupStack::PushL(attr);
    EUNIT_ASSERT(iMsg->iAttributes.Count() == 0);
    CleanupStack::PopAndDestroy(attr);
    }

//Try to detach a non-existing attribute
void UT_CNATFWUNSAFMessage::TestDetachAttribute2L()
    {
    iMsg->AddAttributeL(iAttr);
    EUNIT_ASSERT(iMsg->iAttributes.Count() == 1);
    iAttr = NULL;

    iMsg->DetachAttribute(NULL);
    EUNIT_ASSERT(iMsg->iAttributes.Count() == 1);
    }

//Try to delete attribute that doesn't exist in the message
void UT_CNATFWUNSAFMessage::TestDeleteAttributeL()
    {
    iMsg->AddAttributeL(iAttr);
    iAttr = NULL;
    EUNIT_ASSERT(iMsg->iAttributes.Count() == 1);

    iMsg->DeleteAttribute(CNATFWUNSAFAttribute::EMessageIntegrity);
    EUNIT_ASSERT(iMsg->iAttributes.Count() == 1);

    iMsg->DeleteAttribute(CNATFWUNSAFAttribute::EUsername);
    EUNIT_ASSERT(iMsg->iAttributes.Count() == 1);
    }

//Delete one attribute from the message
void UT_CNATFWUNSAFMessage::TestDeleteAttribute2L()
    {
    TInetAddr addr(0x1002009988, 1020);
    CNATFWUNSAFResponseAddressAttribute* ra =
        CNATFWUNSAFResponseAddressAttribute::NewLC(addr);
    iMsg->AddAttributeL(ra);
    CleanupStack::Pop(ra);

    iMsg->AddAttributeL(iAttr);
    iAttr = NULL;
    EUNIT_ASSERT(iMsg->iAttributes.Count() == 2);


    iMsg->DeleteAttribute(CNATFWUNSAFAttribute::EMappedAddress);
    EUNIT_ASSERT(iMsg->iAttributes.Count() == 1);
    EUNIT_ASSERT(iMsg->HasAttribute(CNATFWUNSAFAttribute::EResponseAddress));
    }

//Delete two attributes having the same type, from the message
void UT_CNATFWUNSAFMessage::TestDeleteAttribute3L()
    {
    TInetAddr addr(0x1002009988, 1020);
    CNATFWUNSAFMappedAddressAttribute* ma =
        CNATFWUNSAFMappedAddressAttribute::NewLC(addr);
    iMsg->AddAttributeL(ma);
    CleanupStack::Pop(ma);

    iMsg->AddAttributeL(iAttr);
    iAttr = NULL;
    EUNIT_ASSERT(iMsg->iAttributes.Count() == 2);


    iMsg->DeleteAttribute(CNATFWUNSAFAttribute::EMappedAddress);
    EUNIT_ASSERT(iMsg->iAttributes.Count() == 0);
    EUNIT_ASSERT(!iMsg->HasAttribute(CNATFWUNSAFAttribute::EMappedAddress));
    }

//Delete several attributes, one by one, from the message
void UT_CNATFWUNSAFMessage::TestDeleteAttribute4L()
    {
    TInetAddr addr(0x1002009988, 1020);

    CNATFWUNSAFChangedAddressAttribute* ca =
        CNATFWUNSAFChangedAddressAttribute::NewLC(addr);
    iMsg->AddAttributeL(ca);
    CleanupStack::Pop(ca);

    CNATFWUNSAFResponseAddressAttribute* ra =
        CNATFWUNSAFResponseAddressAttribute::NewLC(addr);
    iMsg->AddAttributeL(ra);
    CleanupStack::Pop(ra);

    iMsg->AddAttributeL(iAttr);
    iAttr = NULL;
    EUNIT_ASSERT(iMsg->iAttributes.Count() == 3);



    iMsg->DeleteAttribute(CNATFWUNSAFAttribute::EResponseAddress);
    EUNIT_ASSERT(iMsg->iAttributes.Count() == 2);
    EUNIT_ASSERT(iMsg->HasAttribute(CNATFWUNSAFAttribute::EChangedAddress));
    EUNIT_ASSERT(iMsg->HasAttribute(CNATFWUNSAFAttribute::EMappedAddress));


    iMsg->DeleteAttribute(CNATFWUNSAFAttribute::EMappedAddress);
    EUNIT_ASSERT(iMsg->iAttributes.Count() == 1);
    EUNIT_ASSERT(iMsg->HasAttribute(CNATFWUNSAFAttribute::EChangedAddress));

    //Try to delete MAPPED-ADDRESS again
    iMsg->DeleteAttribute(CNATFWUNSAFAttribute::EMappedAddress);
    EUNIT_ASSERT(iMsg->iAttributes.Count() == 1);
    EUNIT_ASSERT(iMsg->HasAttribute(CNATFWUNSAFAttribute::EChangedAddress));


    iMsg->DeleteAttribute(CNATFWUNSAFAttribute::EChangedAddress);
    EUNIT_ASSERT(iMsg->iAttributes.Count() == 0);
    EUNIT_ASSERT(!iMsg->HasAttribute(CNATFWUNSAFAttribute::EChangedAddress));
    }

//Encode UNSAF message without any attributes
void UT_CNATFWUNSAFMessage::TestEncodeL()
    {
    const TUint8 KExpectedResult[] =
        {
        0, 1, //UNSAF message type: Binding Request
        0, 0, //Message length
        0x21, 0x12, 0xa4, 0x42, //Magic cookie
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb //TransactionID
        };

    CBufBase* msg = iMsg->EncodeL();

    CleanupStack::PushL(msg);
    EUNIT_ASSERT(CNATFWUNSAFMessage::EHeaderSize == msg->Size());
    CompareEncodedUNSAFMessageL(KExpectedResult, *msg);
    CleanupStack::PopAndDestroy(msg);
    }

//Encode UNSAF message with one attribute
void UT_CNATFWUNSAFMessage::TestEncode2L()
    {
    const TUint8 KExpectedResult[] =
        {
        0, 1, //UNSAF message type: Binding Request
        0, 0x18, //Message length (one MAPPED-ADDRESS attribute)
        0x21, 0x12, 0xa4, 0x42, //Magic cookie
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, //TransactionID

        //MAPPED-ADDRESS attribute
        0, 1, //type
        0, 0x14, //length of value element
        0, //undefined
        2, //family IPv6
        0x00, 0x00, //port
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00 //address
        };

    TInetAddr addr(0x00000000, 0x0000);
    CNATFWUNSAFMappedAddressAttribute* attr =
        CNATFWUNSAFMappedAddressAttribute::NewLC(addr);

    iMsg->AddAttributeL(attr);
    CleanupStack::Pop(attr);


    CBufBase* msg = iMsg->EncodeL();

    CleanupStack::PushL(msg);
    EUNIT_ASSERT(CNATFWUNSAFMessage::EHeaderSize + 24 == msg->Size());
    CompareEncodedUNSAFMessageL(KExpectedResult, *msg);

    CleanupStack::PopAndDestroy(msg);
    }

//Encode UNSAF message with two attributes
void UT_CNATFWUNSAFMessage::TestEncode3L()
    {
    const TUint8 KExpectedResult[] =
        {
        0, 1, //UNSAF message type: Binding Request
        0, 24, //Message length
        0x21, 0x12, 0xa4, 0x42, //Magic cookie
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, //TransactionID

        //RESPONSE-ADDRESS attribute
        0, 2, //type
        0, 8, //length of value element
        0, //undefined
        1, //family
        0xab, 0xcd, //port
        5, 6, 7, 8, //address

        //MAPPED-ADDRESS attribute
        0, 1, //type
        0, 8, //length of value element
        0, //undefined
        1, //family
        0x12, 0x34, //port
        0x50, 0x40, 0xff, 0xaa //address
        };

    TInetAddr addr(INET_ADDR(5,6,7,8), 0xabcd);
    CNATFWUNSAFAttribute* attr =
        CNATFWUNSAFResponseAddressAttribute::NewLC(addr);
    iMsg->AddAttributeL(attr);
    CleanupStack::Pop(attr);
    attr = NULL;


    TInetAddr addr2(0x5040ffaa, 0x1234);
    attr = CNATFWUNSAFMappedAddressAttribute::NewLC(addr2);
    iMsg->AddAttributeL(attr);
    CleanupStack::Pop(attr);


    CBufBase* msg = iMsg->EncodeL();

    CleanupStack::PushL(msg);
    EUNIT_ASSERT(CNATFWUNSAFMessage::EHeaderSize + 24 == msg->Size());
    CompareEncodedUNSAFMessageL(KExpectedResult, *msg);

    CleanupStack::PopAndDestroy(msg);
    }

//Encode UNSAF message with two attributes
void UT_CNATFWUNSAFMessage::TestEncode4L()
    {
    const TUint8 KExpectedResult[] =
        {
        0, 1, //UNSAF message type: Binding Request
        0, 36, //Message length
        0x21, 0x12, 0xa4, 0x42, //Magic cookie
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, //TransactionID

        //ERROR-CODE attribute
        0, 9,  //type
        0, 20, //length of value element
        0, 0, //zero
        2,  //class
        25, //number
        'E', 'r', 'r', 'o', //Reason Phrase
        'r', ' ', 'r', 'e',
        's', 'u', 'l', 't',
        ' ', 'n', 'o', 'w',

        //UNKNOWN-ATTRIBUTES attribute
        0, 0xa, //type
        0, 8, //length of value element
        0x12, 0x34, //first unknown attribute
        0xab, 0x0c,
        0xff, 0x90,
        0x12, 0x34        //repeated attribute
        };

    _LIT8(KReason, "Error result now");
    TInt responseCode = 225;
    CNATFWUNSAFAttribute* attr =
        CNATFWUNSAFErrorCodeAttribute::NewLC(responseCode, KReason);
    iMsg->AddAttributeL(attr);
    CleanupStack::Pop(attr);
    attr = NULL;

    TUint attrType = 0x1234;
    TUint attrType2 = 0xab0c;
    TUint attrType3 = 0xff90;

    CNATFWUNSAFUnknownAttributesAttribute* uaAttr =
        CNATFWUNSAFUnknownAttributesAttribute::NewLC();
    uaAttr->AddContainedAttributeL(attrType);
    uaAttr->AddContainedAttributeL(attrType2);
    uaAttr->AddContainedAttributeL(attrType3);

    iMsg->AddAttributeL(uaAttr);
    CleanupStack::Pop(uaAttr);

    CBufBase* msg = iMsg->EncodeL();

    CleanupStack::PushL(msg);
    EUNIT_ASSERT(CNATFWUNSAFMessage::EHeaderSize + 36 == msg->Size());
    CompareEncodedUNSAFMessageL(KExpectedResult, *msg);
    CleanupStack::PopAndDestroy(msg);
    }

void UT_CNATFWUNSAFMessage::TestEncodeWithHashL()
    {
    const TUint8 KExpectedResult[] =
        {
        0, 1, //UNSAF message type: Binding Request
        0, 36, //Message length
        0x21, 0x12, 0xa4, 0x42, //Magic cookie
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, //TransactionID

        //RESPONSE-ADDRESS attribute
        0, 2, //type
        0, 8, //length of value element
        0, //undefined
        1, //family
        0xab, 0xcd, //port
        5, 6, 7, 8, //address

        //MESSAGE-INTEGRITY attribute
        0, 8, //type
        0, SHA1_HASH //length of value element
        //???? //HMAC: To be found out how to verify HMAC correctness
        };

    TInetAddr addr(INET_ADDR(5,6,7,8), 0xabcd);
    CNATFWUNSAFAttribute* attr =
        CNATFWUNSAFResponseAddressAttribute::NewLC(addr);
    iMsg->AddAttributeL(attr);
    CleanupStack::Pop(attr);
    attr = NULL;

    CBufBase* msg = iMsg->EncodeL(_L8("password"));

    CleanupStack::PushL(msg);
    EUNIT_ASSERT(CNATFWUNSAFMessage::EHeaderSize + 36 == msg->Size());

    //Compare up to the beginning of HMAC
    CompareEncodedUNSAFMessageL(KExpectedResult, *msg,
                                CNATFWUNSAFMessage::EHeaderSize + 16);

    CleanupStack::PopAndDestroy(msg);
    }

//Check that XOR-MAPPED-ADDRESS is encoded correctly.
void UT_CNATFWUNSAFMessage::TestEncodeMessageWithXorMappedAddrAttrL()
    {
    const TInt KMessageLength =
        2 * CNATFWUNSAFAttribute::EValueOffset +
        CNATFWUNSAFXorMappedAddressAttribute::EAttributeValueSizeIPv4 +
        CNATFWUNSAFXorMappedAddressAttribute::EAttributeValueSizeIPv6;

    const TUint8 KExpectedResult[] =
        {
        0, 1, //UNSAF message type: Binding Request
        0, KMessageLength, //Message length (one XOR-MAPPED-ADDRESS attribute)
        0x21, 0x12, 0xa4, 0x42, //Magic cookie
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, //TransactionID

        //XOR-MAPPED-ADDRESS attribute with IPv4 address
        0, 0x20, //type
        //length of value element
        0, CNATFWUNSAFXorMappedAddressAttribute::EAttributeValueSizeIPv4,
        0, //undefined
        CNATFWUNSAFAddressAttribute::KAddressFamilyIPv4, //family
        0x31, 0x0f, //x-port
        0x2b, 0x46, 0x23, 0xb3, //x-address

        //XOR-MAPPED-ADDRESS attribute with IPv6 address
        0, 0x20, //type
        //length of value element
        0, CNATFWUNSAFXorMappedAddressAttribute::EAttributeValueSizeIPv6,
        0, //undefined
        CNATFWUNSAFAddressAttribute::KAddressFamilyIPv6, //family
        0x32, 0xd6, //x-port
        0x8b, 0xa3, 0xaa, 0x32, //x-address
        0x60, 0x62, 0x28, 0x9b,
        0xff, 0x66, 0x67, 0x58,
        0x65, 0x7b, 0x85, 0x07
        };

    //Add first attribute
    _LIT(KAddr, "10.84.135.241");
    _LIT8(KTransactionId, "abcdefghijkl");
    TInetAddr addr;
    User::LeaveIfError(addr.Input(KAddr));
    addr.SetPort(4125);

    TNATFWUNSAFTransactionID taID(KTransactionId);
    CNATFWUNSAFAttribute* attr =
        CNATFWUNSAFXorMappedAddressAttribute::NewLC(addr, taID);
    iMsg->AddAttributeL(attr);
    CleanupStack::Pop(attr);
    attr = NULL;


    //Add second attribute
    TIp6Addr ip6addr;
    ip6addr.u.iAddr8[0] = 0xaa;
    ip6addr.u.iAddr8[1] = 0xb1;
    ip6addr.u.iAddr8[2] = 0x0e;
    ip6addr.u.iAddr8[3] = 0x70;

    ip6addr.u.iAddr8[4] = 0x01;
    ip6addr.u.iAddr8[5] = 0x00;
    ip6addr.u.iAddr8[6] = 0x4b;
    ip6addr.u.iAddr8[7] = 0xff;

    ip6addr.u.iAddr8[8] = 0x9a;
    ip6addr.u.iAddr8[9] = 0x00;
    ip6addr.u.iAddr8[10] = 0x00;
    ip6addr.u.iAddr8[11] = 0x30;

    ip6addr.u.iAddr8[12] = 0x0c;
    ip6addr.u.iAddr8[13] = 0x11;
    ip6addr.u.iAddr8[14] = 0xee;
    ip6addr.u.iAddr8[15] = 0x6b;
    TInetAddr addrIPv6;
    addrIPv6.SetAddress(ip6addr);
    addrIPv6.SetPort(5060);
    attr = CNATFWUNSAFXorMappedAddressAttribute::NewLC(addrIPv6, taID);
    iMsg->AddAttributeL(attr);
    CleanupStack::Pop(attr);
    attr = NULL;


    //Create encoded message
    CBufBase* msg = iMsg->EncodeL();

    CleanupStack::PushL(msg);
    EUNIT_ASSERT(CNATFWUNSAFMessage::EHeaderSize + KMessageLength ==
        msg->Size());
    CompareEncodedUNSAFMessageL(KExpectedResult, *msg);

    CleanupStack::PopAndDestroy(msg);
    }

void UT_CNATFWUNSAFMessage::TestSetMessageLengthL()
    {
    //UNSAF message, no attributes

    CBufBase* msg = iMsg->EncodeL();
    CleanupStack::PushL(msg);
    iMsg->SetMessageLength(*msg);

    TInt expected = 0; //just the header
    TInt actual =
        BigEndian::Get16(&msg->Ptr(CNATFWUNSAFMessage::EMessageLengthOffset)[0]);
    EUNIT_ASSERT(expected == actual);

    CleanupStack::PopAndDestroy(msg);
    }

void UT_CNATFWUNSAFMessage::TestSetMessageLength2L()
    {
    //UNSAF message, one attribute

    TInetAddr addr(0x5040ffaa, 0x1234);
    CNATFWUNSAFMappedAddressAttribute* attr =
        CNATFWUNSAFMappedAddressAttribute::NewLC(addr);

    iMsg->AddAttributeL(attr);
    CleanupStack::Pop(attr);

    CBufBase* msg = iMsg->EncodeL();
    CleanupStack::PushL(msg);
    iMsg->SetMessageLength(*msg);

    TInt expected = 4 + CNATFWUNSAFAddressAttribute::EAttributeValueSizeIPv4;
    TInt actual =
        BigEndian::Get16(&msg->Ptr(CNATFWUNSAFMessage::EMessageLengthOffset)[0]);
    EUNIT_ASSERT(expected == actual);

    CleanupStack::PopAndDestroy(msg);
    }

void UT_CNATFWUNSAFMessage::TestSetMessageLength3L()
    {
    const TInt KGranularity = 50;
    _LIT8(KBufferContents,
"some stuff to put into the buffer and then check the length is computed correctly");

    CBufBase* msg = CBufFlat::NewL(KGranularity);
    CleanupStack::PushL(msg);
    msg->InsertL(0, KBufferContents);
    iMsg->SetMessageLength(*msg);

    TInt expected = KBufferContents().Length() - CNATFWUNSAFMessage::EHeaderSize;
    TInt actual =
        BigEndian::Get16(&msg->Ptr(CNATFWUNSAFMessage::EMessageLengthOffset)[0]);
    EUNIT_ASSERT(expected == actual);

    CleanupStack::PopAndDestroy(msg);
    }

//Using setter/getter functions
void UT_CNATFWUNSAFMessage::TestUnknownMandatoryAttributesL()
    {
    EUNIT_ASSERT(!iMsg->HasUnknownMandatoryAttributes());
    EUNIT_ASSERT(!iMsg->iUnknownMandatoryAttributes);

    iMsg->UnknownMandatoryAttributeFound();
    EUNIT_ASSERT(iMsg->HasUnknownMandatoryAttributes());
    EUNIT_ASSERT(iMsg->iUnknownMandatoryAttributes);

    iMsg->UnknownMandatoryAttributeFound();
    EUNIT_ASSERT(iMsg->HasUnknownMandatoryAttributes());
    }

void UT_CNATFWUNSAFMessage::CompareEncodedUNSAFMessageL(
    const TUint8* aExpectedResult,
    CBufBase& aUNSAFMessage,
    TUint aMaxBytesToCompare)
    {
    TUint diffPos = 0;
    TUint8 expected = 0;
    TUint8 actual = 0;

    if (!CompareEncodedUNSAFMessage(aExpectedResult,
                                    aUNSAFMessage,
                                    aMaxBytesToCompare,
                                    diffPos,
                                    expected,
                                    actual))
        {
        EUNIT_ASSERT(expected == actual);
        }
    }

TBool UT_CNATFWUNSAFMessage::CompareEncodedUNSAFMessage(
    const TUint8* aExpectedResult,
    CBufBase& aUNSAFMessage,
    TUint aMaxBytesToCompare,
    TUint& aFirstDifference,
    TUint8& aExpected,
    TUint8& aActual)
    {
    TPtr8 unsafMessage = aUNSAFMessage.Ptr(0);

    for (TInt i=0; i < aUNSAFMessage.Size() && i < aMaxBytesToCompare; ++i)
        {
        if (aExpectedResult[i] != unsafMessage[i])
            {
            aFirstDifference = i;
            aExpected = aExpectedResult[i];
            aActual = unsafMessage[i];

            return EFalse;
            }
        }
    return ETrue;
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFMessage,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestTransactionIDL - test ",
    "CNATFWUNSAFMessage",
    "TestEncodeL",
    "FUNCTIONALITY",
    SetupL, TestTransactionIDL, Teardown)

EUNIT_TEST(
    "TestTransactionIDLL - test ",
    "CNATFWUNSAFMessage",
    "TestTransactionIDL2",
    "FUNCTIONALITY",
    SetupL, TestTransactionID2L, Teardown)

EUNIT_TEST(
    "TestTypeL - test ",
    "CNATFWUNSAFMessage",
    "TestTypeL",
    "FUNCTIONALITY",
    SetupL, TestTypeL, Teardown)

EUNIT_TEST(
    "TestAttributeL - test ",
    "CNATFWUNSAFMessage",
    "TestAttributeL",
    "FUNCTIONALITY",
    SetupL, TestAttributeL, Teardown)

EUNIT_TEST(
    "TestAttribute2L - test ",
    "CNATFWUNSAFMessage",
    "TestAttribute2L",
    "FUNCTIONALITY",
    SetupL, TestAttribute2L, Teardown)

EUNIT_TEST(
    "TestAttribute3L - test ",
    "CNATFWUNSAFMessage",
    "TestAttribute3L",
    "FUNCTIONALITY",
    SetupL, TestAttribute3L, Teardown)

EUNIT_TEST(
    "TestAttribute4L - test ",
    "CNATFWUNSAFMessage",
    "TestAttribute4L",
    "FUNCTIONALITY",
    SetupL, TestAttribute4L, Teardown)

EUNIT_TEST(
    "TestAddAttributeL - test ",
    "CNATFWUNSAFMessage",
    "TestAddAttributeL",
    "FUNCTIONALITY",
    SetupL, TestAddAttributeL, Teardown)

EUNIT_TEST(
    "TestDetachAttributeL - test ",
    "CNATFWUNSAFMessage",
    "TestDetachAttributeL",
    "FUNCTIONALITY",
    SetupL, TestDetachAttributeL, Teardown)

EUNIT_TEST(
    "TestDetachAttribute2L - test ",
    "CNATFWUNSAFMessage",
    "TestDetachAttribute2L",
    "FUNCTIONALITY",
    SetupL, TestDetachAttribute2L, Teardown)

EUNIT_TEST(
    "TestDeleteAttributeL - test ",
    "CNATFWUNSAFMessage",
    "TestDeleteAttributeL",
    "FUNCTIONALITY",
    SetupL, TestDeleteAttributeL, Teardown)

EUNIT_TEST(
    "TestDeleteAttribute2L - test ",
    "CNATFWUNSAFMessage",
    "TestDeleteAttribute2L",
    "FUNCTIONALITY",
    SetupL, TestDeleteAttribute2L, Teardown)

EUNIT_TEST(
    "TestDeleteAttribute3L - test ",
    "CNATFWUNSAFMessage",
    "TestDeleteAttribute3L",
    "FUNCTIONALITY",
    SetupL, TestDeleteAttribute3L, Teardown)

EUNIT_TEST(
    "TestDeleteAttribute4L - test ",
    "CNATFWUNSAFMessage",
    "TestDeleteAttributeL4",
    "FUNCTIONALITY",
    SetupL, TestDeleteAttribute4L, Teardown)

EUNIT_TEST(
    "TestEncodeL - test ",
    "CNATFWUNSAFMessage",
    "TestEncodeL",
    "FUNCTIONALITY",
    SetupL, TestEncodeL, Teardown)

EUNIT_TEST(
    "TestEncode2L - test ",
    "CNATFWUNSAFMessage",
    "TestEncode2L",
    "FUNCTIONALITY",
    SetupL, TestEncode2L, Teardown)

EUNIT_TEST(
    "TestEncode3L - test ",
    "CNATFWUNSAFMessage",
    "TestEncode3L",
    "FUNCTIONALITY",
    SetupL, TestEncode3L, Teardown)

EUNIT_TEST(
    "TestEncode4L - test ",
    "CNATFWUNSAFMessage",
    "TestEncode4L",
    "FUNCTIONALITY",
    SetupL, TestEncode4L, Teardown)

EUNIT_TEST(
    "TestEncodeWithHashL - test ",
    "CNATFWUNSAFMessage",
    "TestEncodeWithHashL",
    "FUNCTIONALITY",
    SetupL, TestEncodeWithHashL, Teardown)

EUNIT_TEST(
    "TestEncodeMessageWithXorMappedAddrAttrL - test ",
    "CNATFWUNSAFMessage",
    "TestEncodeMessageWithXorMappedAddrAttrL",
    "FUNCTIONALITY",
    SetupL, TestEncodeMessageWithXorMappedAddrAttrL, Teardown)

EUNIT_TEST(
    "TestSetMessageLengthL - test ",
    "CNATFWUNSAFMessage",
    "TestSetMessageLengthL",
    "FUNCTIONALITY",
    SetupL, TestSetMessageLengthL, Teardown)

EUNIT_TEST(
    "TestSetMessageLength2L - test ",
    "CNATFWUNSAFMessage",
    "TestSetMessageLength2L",
    "FUNCTIONALITY",
    SetupL, TestSetMessageLength2L, Teardown)

EUNIT_TEST(
    "TestSetMessageLength3L - test ",
    "CNATFWUNSAFMessage",
    "TestSetMessageLength3L",
    "FUNCTIONALITY",
    SetupL, TestSetMessageLength3L, Teardown)

EUNIT_TEST(
    "TestUnknownMandatoryAttributesL - test ",
    "CNATFWUNSAFMessage",
    "TestUnknownMandatoryAttributesL",
    "FUNCTIONALITY",
    SetupL, TestUnknownMandatoryAttributesL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
