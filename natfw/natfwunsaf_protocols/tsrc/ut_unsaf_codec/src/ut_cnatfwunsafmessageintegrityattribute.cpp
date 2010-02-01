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
#include "UT_CNATFWUNSAFMessageIntegrityAttribute.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "NATFWUNSAFCommonTestDefs.h"
#include "NATFWUNSAFMessageIntegrityAttribute.h"
#include "NATFWUNSAFUsernameAttribute.h"
#include "NATFWUNSAFTransactionID.h"
#include "NATFWUNSAFBindingRequest.h"
#include "NATFWUNSAFMessageFactory.h"
#include "NATFWUNSAFRealmAttribute.h"

// CONSTRUCTION
UT_CNATFWUNSAFMessageIntegrityAttribute* UT_CNATFWUNSAFMessageIntegrityAttribute::NewL()
    {
    UT_CNATFWUNSAFMessageIntegrityAttribute* self = UT_CNATFWUNSAFMessageIntegrityAttribute::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFMessageIntegrityAttribute* UT_CNATFWUNSAFMessageIntegrityAttribute::NewLC()
    {
    UT_CNATFWUNSAFMessageIntegrityAttribute* self = new( ELeave ) UT_CNATFWUNSAFMessageIntegrityAttribute();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFMessageIntegrityAttribute::~UT_CNATFWUNSAFMessageIntegrityAttribute()
    {
    }

// Default constructor
UT_CNATFWUNSAFMessageIntegrityAttribute::UT_CNATFWUNSAFMessageIntegrityAttribute()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFMessageIntegrityAttribute::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS




void UT_CNATFWUNSAFMessageIntegrityAttribute::SetupL ()
    {
    }


void UT_CNATFWUNSAFMessageIntegrityAttribute::Teardown()
    {
    delete iAttr;
    iAttr = NULL;
    }


void UT_CNATFWUNSAFMessageIntegrityAttribute::TestShortTermNewL()
    {
    TNATFWUNSAFTransactionID taID = KDefaultTaId();

    CNATFWUNSAFBindingRequest* msg = CNATFWUNSAFBindingRequest::NewLC(taID);
    CBufBase* buf = msg->EncodeL();
    CleanupStack::PushL(buf);

    _LIT8(KSecret, "SharedSecretValue");
    iAttr = CNATFWUNSAFMessageIntegrityAttribute::NewL(KSecret, *buf);

    CleanupStack::PopAndDestroy(buf);
    CleanupStack::PopAndDestroy(msg);
    }


void UT_CNATFWUNSAFMessageIntegrityAttribute::TestShortTermDecodeL()
    {
    _LIT8(KStreamToDecode, "abcdefghijklmnopqrst");
    //Decode buffer has to as long enough
    EUNIT_ASSERT(KStreamToDecode().Length() >= SHA1_HASH);

    iAttr = CNATFWUNSAFMessageIntegrityAttribute::DecodeL(KStreamToDecode);

    EUNIT_ASSERT(!KStreamToDecode().Left(SHA1_HASH).CompareF(iAttr->iHash));
    }


void UT_CNATFWUNSAFMessageIntegrityAttribute::TestShortTermEncodeAndDecodeL()
    {
    TNATFWUNSAFTransactionID taID = KDefaultTaId();

    CNATFWUNSAFBindingRequest* msg = CNATFWUNSAFBindingRequest::NewLC(taID);
    CBufBase* buf = msg->EncodeL();
    CleanupStack::PushL(buf);

    _LIT8(KSecret, "SharedSecret");
    iAttr = CNATFWUNSAFMessageIntegrityAttribute::NewL(KSecret, *buf);

    CleanupStack::PopAndDestroy(buf);
    CleanupStack::PopAndDestroy(msg);

    HBufC8* encodedAttr = iAttr->EncodeL();
    CleanupStack::PushL(encodedAttr);
    CNATFWUNSAFAttribute* clone =
        CNATFWUNSAFMessageIntegrityAttribute::DecodeAttributeL(*encodedAttr);
    CleanupStack::PushL(clone);
    const CNATFWUNSAFMessageIntegrityAttribute* castedClone =
        static_cast<CNATFWUNSAFMessageIntegrityAttribute*>(clone);

    EUNIT_ASSERT(!iAttr->iHash.Compare(castedClone->iHash));

    CleanupStack::PopAndDestroy(clone);
    CleanupStack::PopAndDestroy(encodedAttr);
    }

//Encode a message with shared secret, so that encoded byte stream contains a
//MESSAGE-INTEGRITY.
//Test that CheckMessageIntegrityL returns ETrue when the stream and same
//secret as used.
void UT_CNATFWUNSAFMessageIntegrityAttribute::TestShortTermCheckMessageIntegrityL()
    {
    TNATFWUNSAFTransactionID taID = KDefaultTaId();
    CNATFWUNSAFBindingRequest* msg = CNATFWUNSAFBindingRequest::NewLC(taID);

    _LIT8(KSecret, "MySecret");
    CBufBase* encodedBuf = msg->EncodeL(KSecret);
    CleanupStack::PushL(encodedBuf);

    //Decode message, to get the MESSAGE-INTEGRITY object
    CNATFWUNSAFMessageFactory* decoder = CNATFWUNSAFMessageFactory::NewLC();
    CNATFWUNSAFMessage* decodedMsg = decoder->DecodeL(encodedBuf->Ptr(0));
    CleanupStack::PushL(decodedMsg);

    CNATFWUNSAFMessageIntegrityAttribute* mi =
        static_cast<CNATFWUNSAFMessageIntegrityAttribute*>(
        decodedMsg->Attribute(CNATFWUNSAFAttribute::EMessageIntegrity));

    EUNIT_ASSERT(mi);
    decodedMsg->DetachAttribute(mi);
    CleanupStack::PopAndDestroy(decodedMsg);
    CleanupStack::PopAndDestroy(decoder);
    CleanupStack::PushL(mi);


    EUNIT_ASSERT(
        mi->CheckMessageIntegrityL(encodedBuf->Ptr(0), KSecret));

    CleanupStack::PopAndDestroy(mi);
    CleanupStack::PopAndDestroy(encodedBuf);
    CleanupStack::PopAndDestroy(msg);
    }

//Change the message type, integrity check must fail
void UT_CNATFWUNSAFMessageIntegrityAttribute::TestShortTermCheckMessageIntegrity2L()
    {
    TNATFWUNSAFTransactionID taID = KDefaultTaId();
    CNATFWUNSAFBindingRequest* msg = CNATFWUNSAFBindingRequest::NewLC(taID);

    _LIT8(KSecret, "MySecret");
    CBufBase* encodedBuf = msg->EncodeL(KSecret);
    CleanupStack::PushL(encodedBuf);

    //Decode message, to get the MESSAGE-INTEGRITY object
    CNATFWUNSAFMessageFactory* decoder = CNATFWUNSAFMessageFactory::NewLC();
    CNATFWUNSAFMessage* decodedMsg = decoder->DecodeL(encodedBuf->Ptr(0));
    CleanupStack::PushL(decodedMsg);

    CNATFWUNSAFMessageIntegrityAttribute* mi =
        static_cast<CNATFWUNSAFMessageIntegrityAttribute*>(
        decodedMsg->Attribute(CNATFWUNSAFAttribute::EMessageIntegrity));

    EUNIT_ASSERT(mi);
    decodedMsg->DetachAttribute(mi);
    CleanupStack::PopAndDestroy(decodedMsg);
    CleanupStack::PopAndDestroy(decoder);
    CleanupStack::PushL(mi);


    //Change the message type (EBindingRequest -> EBindingResponse)

    TUint16 msgType = CNATFWUNSAFMessage::EBindingResponse;
    TUint16 networkOrderMsgType = ByteOrder::Swap16(msgType);
    encodedBuf->Write(0, &networkOrderMsgType, 2);

    EUNIT_ASSERT(
        !mi->CheckMessageIntegrityL(encodedBuf->Ptr(0), KSecret));

    CleanupStack::PopAndDestroy(mi);
    CleanupStack::PopAndDestroy(encodedBuf);
    CleanupStack::PopAndDestroy(msg);
    }

//Change the transactionID, integrity check must fail
void UT_CNATFWUNSAFMessageIntegrityAttribute::TestShortTermCheckMessageIntegrity3L()
    {
    TNATFWUNSAFTransactionID taID = KDefaultTaId();
    CNATFWUNSAFBindingRequest* msg = CNATFWUNSAFBindingRequest::NewLC(taID);

    _LIT8(KSecret, "MySecret");
    CBufBase* encodedBuf = msg->EncodeL(KSecret);
    CleanupStack::PushL(encodedBuf);

    //Decode message, to get the MESSAGE-INTEGRITY object
    CNATFWUNSAFMessageFactory* decoder = CNATFWUNSAFMessageFactory::NewLC();
    CNATFWUNSAFMessage* decodedMsg = decoder->DecodeL(encodedBuf->Ptr(0));
    CleanupStack::PushL(decodedMsg);

    CNATFWUNSAFMessageIntegrityAttribute* mi =
        static_cast<CNATFWUNSAFMessageIntegrityAttribute*>(
        decodedMsg->Attribute(CNATFWUNSAFAttribute::EMessageIntegrity));

    EUNIT_ASSERT(mi);
    decodedMsg->DetachAttribute(mi);
    CleanupStack::PopAndDestroy(decodedMsg);
    CleanupStack::PopAndDestroy(decoder);
    CleanupStack::PushL(mi);


    //Change the transactionID
    TUint8 byteValue = 8;
    encodedBuf->Write(CNATFWUNSAFMessage::ETransactionIDOffset + 5, &byteValue, 1);

    EUNIT_ASSERT(
        !mi->CheckMessageIntegrityL(encodedBuf->Ptr(0), KSecret));

    CleanupStack::PopAndDestroy(mi);
    CleanupStack::PopAndDestroy(encodedBuf);
    CleanupStack::PopAndDestroy(msg);
    }

//Encode with wrong value in message header's length field. Use value that does
//NOT include the MESSAGE-INTEGRITY.
//The updated verification should succeed also in this case.
void UT_CNATFWUNSAFMessageIntegrityAttribute::TestShortTermCheckMessageIntegrity4L()
    {
    TNATFWUNSAFTransactionID taID = KDefaultTaId();
    CNATFWUNSAFBindingRequest* msg = CNATFWUNSAFBindingRequest::NewLC(taID);

    _LIT8(KUsername, "aaaaBBBBccccDDDDeeeeFFFF");
    CNATFWUNSAFUsernameAttribute* username =
        CNATFWUNSAFUsernameAttribute::NewLC(KUsername);
    msg->AddAttributeL(username);
    CleanupStack::Pop(username);

    CBufBase* encodedBuf = msg->EncodeL();
    CleanupStack::PushL(encodedBuf);


    //Manually create CUNSAFMessageIntegrityAttribute, instead of using
    //CUNSAFMessageIntegrityAttribute::NewLC, because NewLC updates the message
    //header's length field.
    _LIT8(KSecret, "123456789012");
    CNATFWUNSAFMessageIntegrityAttribute* mi =
        new (ELeave) CNATFWUNSAFMessageIntegrityAttribute();
    CleanupStack::PushL(mi);
    //Do NOT add the MESSAGE-INTEGRITY's size into message header's length field
    mi->ComputeShortTermHMACL(KSecret, *encodedBuf, mi->iHash);


    HBufC8* encodedMsgIntegrity = mi->EncodeL();
    CleanupStack::PushL(encodedMsgIntegrity);
    encodedBuf->InsertL(encodedBuf->Size(), *encodedMsgIntegrity);
    CleanupStack::PopAndDestroy(encodedMsgIntegrity);
    CleanupStack::PopAndDestroy(mi);
    mi = NULL;

    //Now that MESSAGE-INTEGRITY's HMAC has been computed, update the message's
    //length to correct value so that decoding it succeeds.
    CNATFWUNSAFMessage::SetMessageLength(*encodedBuf,
        CNATFWUNSAFMessage::MessageLength(encodedBuf->Ptr(0)) +
        CNATFWUNSAFAttribute::EValueOffset +
        SHA1_HASH);
    encodedBuf->Compress();


    //Decode message, to get the MESSAGE-INTEGRITY object
    CNATFWUNSAFMessageFactory* decoder = CNATFWUNSAFMessageFactory::NewLC();
    CNATFWUNSAFMessage* decodedMsg = decoder->DecodeL(encodedBuf->Ptr(0));
    CleanupStack::PushL(decodedMsg);

    mi = static_cast<CNATFWUNSAFMessageIntegrityAttribute*>(
        decodedMsg->Attribute(CNATFWUNSAFAttribute::EMessageIntegrity));

    EUNIT_ASSERT(mi);
    decodedMsg->DetachAttribute(mi);
    CleanupStack::PopAndDestroy(decodedMsg);
    CleanupStack::PopAndDestroy(decoder);
    CleanupStack::PushL(mi);


    EUNIT_ASSERT(
        mi->CheckMessageIntegrityL(encodedBuf->Ptr(0), KSecret));

    CleanupStack::PopAndDestroy(mi);
    CleanupStack::PopAndDestroy(encodedBuf);
    CleanupStack::PopAndDestroy(msg);
    }

//Encode a message with shared secret, so that encoded byte stream contains a
//MESSAGE-INTEGRITY and after that a FINGERPRINT attribute.
//Test that CheckMessageIntegrityL returns ETrue when the stream and same
//secret as used.
void UT_CNATFWUNSAFMessageIntegrityAttribute::TestShortTermCheckMessageIntegrity5L()
    {
    TNATFWUNSAFTransactionID taID = KDefaultTaId();
    CNATFWUNSAFBindingRequest* msg = CNATFWUNSAFBindingRequest::NewLC(taID);

    _LIT8(KSecret, "MySecret");
    //Encode using a FINGERPRINT
    CBufBase* encodedBuf = msg->EncodeL(KSecret, ETrue);
    CleanupStack::PushL(encodedBuf);

    //Decode message, to get the MESSAGE-INTEGRITY object
    CNATFWUNSAFMessageFactory* decoder = CNATFWUNSAFMessageFactory::NewLC();
    CNATFWUNSAFMessage* decodedMsg = decoder->DecodeL(encodedBuf->Ptr(0));
    CleanupStack::PushL(decodedMsg);
    CNATFWUNSAFMessageIntegrityAttribute* mi =
        static_cast<CNATFWUNSAFMessageIntegrityAttribute*>(
        decodedMsg->Attribute(CNATFWUNSAFAttribute::EMessageIntegrity));

    EUNIT_ASSERT(mi);
    decodedMsg->DetachAttribute(mi);
    CleanupStack::PopAndDestroy(decodedMsg);
    CleanupStack::PopAndDestroy(decoder);
    CleanupStack::PushL(mi);


    EUNIT_ASSERT(
        mi->CheckMessageIntegrityL(encodedBuf->Ptr(0), KSecret));

    CleanupStack::PopAndDestroy(mi);
    CleanupStack::PopAndDestroy(encodedBuf);
    CleanupStack::PopAndDestroy(msg);
    }


void UT_CNATFWUNSAFMessageIntegrityAttribute::TestLongTermNewL()
    {
    TNATFWUNSAFTransactionID taID = KDefaultTaId();

    CNATFWUNSAFBindingRequest* msg = CNATFWUNSAFBindingRequest::NewLC(taID);
    CBufBase* buf = msg->EncodeL();
    CleanupStack::PushL(buf);

    _LIT8(KSecret, "SharedSecretValue");
    iAttr = CNATFWUNSAFMessageIntegrityAttribute::NewL(KSecret, *buf, ETrue);

    CleanupStack::PopAndDestroy(buf);
    CleanupStack::PopAndDestroy(msg);
    }


void UT_CNATFWUNSAFMessageIntegrityAttribute::TestLongTermDecodeL()
    {
    _LIT8(KStreamToDecode, "abcdefghijklmnopqrst");
    //Decode buffer has to as long enough
    EUNIT_ASSERT(KStreamToDecode().Length() >= SHA1_HASH);

    iAttr = CNATFWUNSAFMessageIntegrityAttribute::DecodeL(KStreamToDecode);

    EUNIT_ASSERT(!KStreamToDecode().Left(SHA1_HASH).CompareF(iAttr->iHash));
    }


void UT_CNATFWUNSAFMessageIntegrityAttribute::TestLongTermEncodeAndDecodeL()
    {
    TNATFWUNSAFTransactionID taID = KDefaultTaId();

    CNATFWUNSAFBindingRequest* msg = CNATFWUNSAFBindingRequest::NewLC(taID);
    CBufBase* buf = msg->EncodeL();
    CleanupStack::PushL(buf);

    _LIT8(KSecret, "SharedSecret");
    iAttr = CNATFWUNSAFMessageIntegrityAttribute::NewL(KSecret, *buf, ETrue);

    CleanupStack::PopAndDestroy(buf);
    CleanupStack::PopAndDestroy(msg);

    HBufC8* encodedAttr = iAttr->EncodeL();
    CleanupStack::PushL(encodedAttr);
    CNATFWUNSAFAttribute* clone =
        CNATFWUNSAFMessageIntegrityAttribute::DecodeAttributeL(*encodedAttr);
    CleanupStack::PushL(clone);
    const CNATFWUNSAFMessageIntegrityAttribute* castedClone =
        static_cast<CNATFWUNSAFMessageIntegrityAttribute*>(clone);

    EUNIT_ASSERT(!iAttr->iHash.Compare(castedClone->iHash));

    CleanupStack::PopAndDestroy(clone);
    CleanupStack::PopAndDestroy(encodedAttr);
    }

//Encode a message with shared secret, so that encoded byte stream contains a
//MESSAGE-INTEGRITY.
//Test that CheckMessageIntegrityL returns ETrue when the stream and same
//secret as used.
void UT_CNATFWUNSAFMessageIntegrityAttribute::TestLongTermCheckMessageIntegrityL()
    {
    TNATFWUNSAFTransactionID taID = KDefaultTaId();
    CNATFWUNSAFBindingRequest* msg = CNATFWUNSAFBindingRequest::NewLC(taID);

    CNATFWUNSAFAttribute* attr =
        CNATFWUNSAFRealmAttribute::NewLC(_L8("\"myrealm\""));
    msg->AddAttributeL(attr);
    CleanupStack::Pop(attr);
    _LIT8(KSecret, "user:realm:pass");
    CBufBase* encodedBuf = msg->EncodeL(KSecret);
    CleanupStack::PushL(encodedBuf);

    //Decode message, to get the MESSAGE-INTEGRITY object
    CNATFWUNSAFMessageFactory* decoder = CNATFWUNSAFMessageFactory::NewLC();
    CNATFWUNSAFMessage* decodedMsg = decoder->DecodeL(encodedBuf->Ptr(0));
    CleanupStack::PushL(decodedMsg);
    
    CNATFWUNSAFMessageIntegrityAttribute* mi =
        static_cast<CNATFWUNSAFMessageIntegrityAttribute*>(
        decodedMsg->Attribute(CNATFWUNSAFAttribute::EMessageIntegrity));

    EUNIT_ASSERT(mi);
    decodedMsg->DetachAttribute(mi);
    if (decodedMsg->HasAttribute(CNATFWUNSAFAttribute::ERealm))
        {
        mi->UseLongTermCredentials(ETrue);
        }
    else
        {
        mi->UseLongTermCredentials(EFalse);
        }
    
    CleanupStack::PopAndDestroy(decodedMsg);
    CleanupStack::PopAndDestroy(decoder);
    CleanupStack::PushL(mi);


    EUNIT_ASSERT(
        mi->CheckMessageIntegrityL(encodedBuf->Ptr(0), KSecret));

    CleanupStack::PopAndDestroy(mi);
    CleanupStack::PopAndDestroy(encodedBuf);
    CleanupStack::PopAndDestroy(msg);
    }

//Change the message type, integrity check must fail
void UT_CNATFWUNSAFMessageIntegrityAttribute::TestLongTermCheckMessageIntegrity2L()
    {
    TNATFWUNSAFTransactionID taID = KDefaultTaId();
    CNATFWUNSAFBindingRequest* msg = CNATFWUNSAFBindingRequest::NewLC(taID);
    CNATFWUNSAFAttribute* attr =
        CNATFWUNSAFRealmAttribute::NewLC(_L8("\"myrealm\""));
    msg->AddAttributeL(attr);
    CleanupStack::Pop(attr);
    _LIT8(KSecret, "user:realm:pass");
    CBufBase* encodedBuf = msg->EncodeL(KSecret);
    CleanupStack::PushL(encodedBuf);

    //Decode message, to get the MESSAGE-INTEGRITY object
    CNATFWUNSAFMessageFactory* decoder = CNATFWUNSAFMessageFactory::NewLC();
    CNATFWUNSAFMessage* decodedMsg = decoder->DecodeL(encodedBuf->Ptr(0));
    CleanupStack::PushL(decodedMsg);

    CNATFWUNSAFMessageIntegrityAttribute* mi =
        static_cast<CNATFWUNSAFMessageIntegrityAttribute*>(
        decodedMsg->Attribute(CNATFWUNSAFAttribute::EMessageIntegrity));

    EUNIT_ASSERT(mi);
    decodedMsg->DetachAttribute(mi);
    if (decodedMsg->HasAttribute(CNATFWUNSAFAttribute::ERealm))
        {
        mi->UseLongTermCredentials(ETrue);
        }
    else
        {
        mi->UseLongTermCredentials(EFalse);
        }
    CleanupStack::PopAndDestroy(decodedMsg);
    CleanupStack::PopAndDestroy(decoder);
    CleanupStack::PushL(mi);


    //Change the message type (EBindingRequest -> EBindingResponse)

    TUint16 msgType = CNATFWUNSAFMessage::EBindingResponse;
    TUint16 networkOrderMsgType = ByteOrder::Swap16(msgType);
    encodedBuf->Write(0, &networkOrderMsgType, 2);

    EUNIT_ASSERT(
        !mi->CheckMessageIntegrityL(encodedBuf->Ptr(0), KSecret));

    CleanupStack::PopAndDestroy(mi);
    CleanupStack::PopAndDestroy(encodedBuf);
    CleanupStack::PopAndDestroy(msg);
    }

//Change the transactionID, integrity check must fail
void UT_CNATFWUNSAFMessageIntegrityAttribute::TestLongTermCheckMessageIntegrity3L()
    {
    TNATFWUNSAFTransactionID taID = KDefaultTaId();
    CNATFWUNSAFBindingRequest* msg = CNATFWUNSAFBindingRequest::NewLC(taID);
    CNATFWUNSAFAttribute* attr =
        CNATFWUNSAFRealmAttribute::NewLC(_L8("\"myrealm\""));
    msg->AddAttributeL(attr);
    CleanupStack::Pop(attr);
    _LIT8(KSecret, "user:realm:pass");
    CBufBase* encodedBuf = msg->EncodeL(KSecret);
    CleanupStack::PushL(encodedBuf);

    //Decode message, to get the MESSAGE-INTEGRITY object
    CNATFWUNSAFMessageFactory* decoder = CNATFWUNSAFMessageFactory::NewLC();
    CNATFWUNSAFMessage* decodedMsg = decoder->DecodeL(encodedBuf->Ptr(0));
    CleanupStack::PushL(decodedMsg);

    CNATFWUNSAFMessageIntegrityAttribute* mi =
        static_cast<CNATFWUNSAFMessageIntegrityAttribute*>(
        decodedMsg->Attribute(CNATFWUNSAFAttribute::EMessageIntegrity));

    EUNIT_ASSERT(mi);
    decodedMsg->DetachAttribute(mi);
    if (decodedMsg->HasAttribute(CNATFWUNSAFAttribute::ERealm))
        {
        mi->UseLongTermCredentials(ETrue);
        }
    else
        {
        mi->UseLongTermCredentials(EFalse);
        }
    CleanupStack::PopAndDestroy(decodedMsg);
    CleanupStack::PopAndDestroy(decoder);
    CleanupStack::PushL(mi);


    //Change the transactionID
    TUint8 byteValue = 8;
    encodedBuf->Write(CNATFWUNSAFMessage::ETransactionIDOffset + 5, &byteValue, 1);

    EUNIT_ASSERT(
        !mi->CheckMessageIntegrityL(encodedBuf->Ptr(0), KSecret));

    CleanupStack::PopAndDestroy(mi);
    CleanupStack::PopAndDestroy(encodedBuf);
    CleanupStack::PopAndDestroy(msg);
    }

//Encode with wrong value in message header's length field. Use value that does
//NOT include the MESSAGE-INTEGRITY.
//The updated verification should succeed also in this case.
void UT_CNATFWUNSAFMessageIntegrityAttribute::TestLongTermCheckMessageIntegrity4L()
    {
    TNATFWUNSAFTransactionID taID = KDefaultTaId();
    CNATFWUNSAFBindingRequest* msg = CNATFWUNSAFBindingRequest::NewLC(taID);
    CNATFWUNSAFAttribute* attr =
        CNATFWUNSAFRealmAttribute::NewLC(_L8("\"myrealm\""));
    msg->AddAttributeL(attr);
    CleanupStack::Pop(attr);
    _LIT8(KUsername, "aaaaBBBBccccDDDDeeeeFFFF");
    CNATFWUNSAFUsernameAttribute* username =
        CNATFWUNSAFUsernameAttribute::NewLC(KUsername);
    msg->AddAttributeL(username);
    CleanupStack::Pop(username);

    CBufBase* encodedBuf = msg->EncodeL();
    CleanupStack::PushL(encodedBuf);


    //Manually create CUNSAFMessageIntegrityAttribute, instead of using
    //CUNSAFMessageIntegrityAttribute::NewLC, because NewLC updates the message
    //header's length field.
    _LIT8(KSecret, "user:realm:pass");
    CNATFWUNSAFMessageIntegrityAttribute* mi =
        new (ELeave) CNATFWUNSAFMessageIntegrityAttribute();
    mi->UseLongTermCredentials(ETrue);
    CleanupStack::PushL(mi);
    //Do NOT add the MESSAGE-INTEGRITY's size into message header's length field
    mi->ComputeLongTermHMACL(KSecret, *encodedBuf, mi->iHash);


    HBufC8* encodedMsgIntegrity = mi->EncodeL();
    CleanupStack::PushL(encodedMsgIntegrity);
    encodedBuf->InsertL(encodedBuf->Size(), *encodedMsgIntegrity);
    CleanupStack::PopAndDestroy(encodedMsgIntegrity);
    CleanupStack::PopAndDestroy(mi);
    mi = NULL;

    //Now that MESSAGE-INTEGRITY's HMAC has been computed, update the message's
    //length to correct value so that decoding it succeeds.
    CNATFWUNSAFMessage::SetMessageLength(*encodedBuf,
        CNATFWUNSAFMessage::MessageLength(encodedBuf->Ptr(0)) +
        CNATFWUNSAFAttribute::EValueOffset +
        MD5_HASH);
    encodedBuf->Compress();


    //Decode message, to get the MESSAGE-INTEGRITY object
    CNATFWUNSAFMessageFactory* decoder = CNATFWUNSAFMessageFactory::NewLC();
    CNATFWUNSAFMessage* decodedMsg = decoder->DecodeL(encodedBuf->Ptr(0));
    CleanupStack::PushL(decodedMsg);

    mi = static_cast<CNATFWUNSAFMessageIntegrityAttribute*>(
        decodedMsg->Attribute(CNATFWUNSAFAttribute::EMessageIntegrity));

    EUNIT_ASSERT(mi);
    decodedMsg->DetachAttribute(mi);
    if (decodedMsg->HasAttribute(CNATFWUNSAFAttribute::ERealm))
        {
        mi->UseLongTermCredentials(ETrue);
        }
    else
        {
        mi->UseLongTermCredentials(EFalse);
        }
    CleanupStack::PopAndDestroy(decodedMsg);
    CleanupStack::PopAndDestroy(decoder);
    CleanupStack::PushL(mi);


    EUNIT_ASSERT(
        mi->CheckMessageIntegrityL(encodedBuf->Ptr(0), KSecret));

    CleanupStack::PopAndDestroy(mi);
    CleanupStack::PopAndDestroy(encodedBuf);
    CleanupStack::PopAndDestroy(msg);
    }

//Encode a message with shared secret, so that encoded byte stream contains a
//MESSAGE-INTEGRITY and after that a FINGERPRINT attribute.
//Test that CheckMessageIntegrityL returns ETrue when the stream and same
//secret as used.
void UT_CNATFWUNSAFMessageIntegrityAttribute::TestLongTermCheckMessageIntegrity5L()
    {
    TNATFWUNSAFTransactionID taID = KDefaultTaId();
    CNATFWUNSAFBindingRequest* msg = CNATFWUNSAFBindingRequest::NewLC(taID);

    _LIT8(KSecret, "MySecret");
    //Encode using a FINGERPRINT
    CBufBase* encodedBuf = msg->EncodeL(KSecret, ETrue);
    CleanupStack::PushL(encodedBuf);

    //Decode message, to get the MESSAGE-INTEGRITY object
    CNATFWUNSAFMessageFactory* decoder = CNATFWUNSAFMessageFactory::NewLC();
    CNATFWUNSAFMessage* decodedMsg = decoder->DecodeL(encodedBuf->Ptr(0));
    CleanupStack::PushL(decodedMsg);

    CNATFWUNSAFMessageIntegrityAttribute* mi =
        static_cast<CNATFWUNSAFMessageIntegrityAttribute*>(
        decodedMsg->Attribute(CNATFWUNSAFAttribute::EMessageIntegrity));

    EUNIT_ASSERT(mi);
    decodedMsg->DetachAttribute(mi);
    CleanupStack::PopAndDestroy(decodedMsg);
    CleanupStack::PopAndDestroy(decoder);
    CleanupStack::PushL(mi);


    EUNIT_ASSERT(
        mi->CheckMessageIntegrityL(encodedBuf->Ptr(0), KSecret));

    CleanupStack::PopAndDestroy(mi);
    CleanupStack::PopAndDestroy(encodedBuf);
    CleanupStack::PopAndDestroy(msg);
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFMessageIntegrityAttribute,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestShortTermNewL - test ",
    "CNATFWUNSAFMessageIntegrityAttribute",
    "TestShortTermNewL",
    "FUNCTIONALITY",
    SetupL, TestShortTermNewL, Teardown)

EUNIT_TEST(
    "TestShortTermDecodeL - test ",
    "CNATFWUNSAFMessageIntegrityAttribute",
    "TestShortTermDecodeL",
    "FUNCTIONALITY",
    SetupL, TestShortTermDecodeL, Teardown)

EUNIT_TEST(
    "TestShortTermEncodeAndDecodeL - test ",
    "CNATFWUNSAFMessageIntegrityAttribute",
    "TestShortTermEncodeAndDecodeL",
    "FUNCTIONALITY",
    SetupL, TestShortTermEncodeAndDecodeL, Teardown)

EUNIT_TEST(
    "TestShortTermCheckMessageIntegrityL - test ",
    "CNATFWUNSAFMessageIntegrityAttribute",
    "TestShortTermCheckMessageIntegrityL",
    "FUNCTIONALITY",
    SetupL, TestShortTermCheckMessageIntegrityL, Teardown)

EUNIT_TEST(
    "TestShortTermCheckMessageIntegrity2L - test ",
    "CNATFWUNSAFMessageIntegrityAttribute",
    "TestShortTermCheckMessageIntegrity2L",
    "FUNCTIONALITY",
    SetupL, TestShortTermCheckMessageIntegrity2L, Teardown)

EUNIT_TEST(
    "TestShortTermCheckMessageIntegrity3L - test ",
    "CNATFWUNSAFMessageIntegrityAttribute",
    "TestShortTermCheckMessageIntegrity3L",
    "FUNCTIONALITY",
    SetupL, TestShortTermCheckMessageIntegrity3L, Teardown)

EUNIT_TEST(
    "TestShortTermCheckMessageIntegrity4L - test ",
    "CNATFWUNSAFMessageIntegrityAttribute",
    "TestShortTermCheckMessageIntegrity4L",
    "FUNCTIONALITY",
    SetupL, TestShortTermCheckMessageIntegrity4L, Teardown)

EUNIT_TEST(
    "TestShortTermCheckMessageIntegrity5L - test ",
    "CNATFWUNSAFMessageIntegrityAttribute",
    "TestShortTermCheckMessageIntegrity5L",
    "FUNCTIONALITY",
    SetupL, TestShortTermCheckMessageIntegrity5L, Teardown)

EUNIT_TEST(
    "TestLongTermNewL - test ",
    "CNATFWUNSAFMessageIntegrityAttribute",
    "TestLongTermNewL",
    "FUNCTIONALITY",
    SetupL, TestLongTermNewL, Teardown)

EUNIT_TEST(
    "TestLongTermDecodeL - test ",
    "CNATFWUNSAFMessageIntegrityAttribute",
    "TestLongTermDecodeL",
    "FUNCTIONALITY",
    SetupL, TestLongTermDecodeL, Teardown)

EUNIT_TEST(
    "TestLongTermEncodeAndDecodeL - test ",
    "CNATFWUNSAFMessageIntegrityAttribute",
    "TestLongTermEncodeAndDecodeL",
    "FUNCTIONALITY",
    SetupL, TestLongTermEncodeAndDecodeL, Teardown)

EUNIT_TEST(
    "TestLongTermCheckMessageIntegrityL - test ",
    "CNATFWUNSAFMessageIntegrityAttribute",
    "TestLongTermCheckMessageIntegrityL",
    "FUNCTIONALITY",
    SetupL, TestLongTermCheckMessageIntegrityL, Teardown)

EUNIT_TEST(
    "TestLongTermCheckMessageIntegrity2L - test ",
    "CNATFWUNSAFMessageIntegrityAttribute",
    "TestLongTermCheckMessageIntegrity2L",
    "FUNCTIONALITY",
    SetupL, TestLongTermCheckMessageIntegrity2L, Teardown)

EUNIT_TEST(
    "TestLongTermCheckMessageIntegrity3L - test ",
    "CNATFWUNSAFMessageIntegrityAttribute",
    "TestLongTermCheckMessageIntegrity3L",
    "FUNCTIONALITY",
    SetupL, TestLongTermCheckMessageIntegrity3L, Teardown)

EUNIT_TEST(
    "TestLongTermCheckMessageIntegrity4L - test ",
    "CNATFWUNSAFMessageIntegrityAttribute",
    "TestLongTermCheckMessageIntegrity4L",
    "FUNCTIONALITY",
    SetupL, TestLongTermCheckMessageIntegrity4L, Teardown)

EUNIT_TEST(
    "TestLongTermCheckMessageIntegrity5L - test ",
    "CNATFWUNSAFMessageIntegrityAttribute",
    "TestLongTermCheckMessageIntegrity5L",
    "FUNCTIONALITY",
    SetupL, TestLongTermCheckMessageIntegrity5L, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
