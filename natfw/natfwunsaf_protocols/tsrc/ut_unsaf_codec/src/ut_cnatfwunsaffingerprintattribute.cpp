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
#include "UT_CNATFWUNSAFFingerprintAttribute.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "NATFWUNSAFCommonTestDefs.h"
#include "NATFWUNSAFFingerprintAttribute.h"
#include "NATFWUNSAFTransactionID.h"
#include "NATFWUNSAFBindingRequest.h"
#include "NATFWUNSAFMessageFactory.h"
#include "NATFWUNSAFUsernameAttribute.h"
#include "NATFWUNSAFPriorityAttribute.h"

// CONSTRUCTION
UT_CNATFWUNSAFFingerprintAttribute* UT_CNATFWUNSAFFingerprintAttribute::NewL()
    {
    UT_CNATFWUNSAFFingerprintAttribute* self = UT_CNATFWUNSAFFingerprintAttribute::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFFingerprintAttribute* UT_CNATFWUNSAFFingerprintAttribute::NewLC()
    {
    UT_CNATFWUNSAFFingerprintAttribute* self = new( ELeave ) UT_CNATFWUNSAFFingerprintAttribute();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFFingerprintAttribute::~UT_CNATFWUNSAFFingerprintAttribute()
    {
    }

// Default constructor
UT_CNATFWUNSAFFingerprintAttribute::UT_CNATFWUNSAFFingerprintAttribute()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFFingerprintAttribute::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS




void UT_CNATFWUNSAFFingerprintAttribute::SetupL ()
    {
    }


void UT_CNATFWUNSAFFingerprintAttribute::Teardown()
    {
    delete iAttr;
    iAttr = NULL;
    }


void UT_CNATFWUNSAFFingerprintAttribute::TestNewL()
    {
    TNATFWUNSAFTransactionID taID = KDefaultTaId();

    CNATFWUNSAFBindingRequest* msg = CNATFWUNSAFBindingRequest::NewLC(taID);
    CBufBase* buf = msg->EncodeL();
    CleanupStack::PushL(buf);

    iAttr = CNATFWUNSAFFingerprintAttribute::NewL(*buf);

    CleanupStack::PopAndDestroy(buf);
    CleanupStack::PopAndDestroy(msg);
    }


void UT_CNATFWUNSAFFingerprintAttribute::TestEncodeAndDecodeL()
    {
    TNATFWUNSAFTransactionID taID = KDefaultTaId();

    CNATFWUNSAFBindingRequest* msg = CNATFWUNSAFBindingRequest::NewLC(taID);
    CBufBase* buf = msg->EncodeL();
    CleanupStack::PushL(buf);

    iAttr = CNATFWUNSAFFingerprintAttribute::NewL(*buf);

    CleanupStack::PopAndDestroy(buf);
    CleanupStack::PopAndDestroy(msg);

    HBufC8* encodedAttr = iAttr->EncodeL();
    CleanupStack::PushL(encodedAttr);
    CNATFWUNSAFAttribute* clone =
        CNATFWUNSAFFingerprintAttribute::DecodeAttributeL(*encodedAttr);
    CleanupStack::PushL(clone);
    const CNATFWUNSAFFingerprintAttribute* castedClone =
        static_cast<CNATFWUNSAFFingerprintAttribute*>(clone);

    EUNIT_ASSERT(iAttr->iCRC == castedClone->iCRC);

    CleanupStack::PopAndDestroy(clone);
    CleanupStack::PopAndDestroy(encodedAttr);
    }

//Encode a message with FINGERPRINT
//Add USERNAME
//Test that CheckFingerprintL returns ETrue when same msg is used
void UT_CNATFWUNSAFFingerprintAttribute::TestCheckFingerprintL()
    {
    TNATFWUNSAFTransactionID taID = KDefaultTaId();
    CNATFWUNSAFBindingRequest* msg = CNATFWUNSAFBindingRequest::NewLC(taID);

    //Add USERNAME
    CNATFWUNSAFUsernameAttribute* uAttribute =
        CNATFWUNSAFUsernameAttribute::NewLC(_L8("username2"));
    msg->AddAttributeL(uAttribute);
    CleanupStack::Pop( uAttribute );
    
    //Add PRIORITY
    CNATFWUNSAFPriorityAttribute* pAttribute =
        CNATFWUNSAFPriorityAttribute::NewLC(0x0ee1e234);
    msg->AddAttributeL(pAttribute);
    CleanupStack::Pop( pAttribute );
    
    CBufBase* encodedBuf = msg->EncodeL(KNullDesC8, ETrue);
    CleanupStack::PushL(encodedBuf);

    //Decode message, to get the FINGERPRINT object
    CNATFWUNSAFMessageFactory* decoder = CNATFWUNSAFMessageFactory::NewLC();
    CNATFWUNSAFMessage* decodedMsg = decoder->DecodeL(encodedBuf->Ptr(0));
    CleanupStack::PushL(decodedMsg);

    CNATFWUNSAFFingerprintAttribute* fp =
        static_cast<CNATFWUNSAFFingerprintAttribute*>(
        decodedMsg->Attribute(CNATFWUNSAFAttribute::EFingerprint));

    EUNIT_ASSERT(fp);
    decodedMsg->DetachAttribute(fp);
    CleanupStack::PopAndDestroy(decodedMsg);
    CleanupStack::PopAndDestroy(decoder);
    CleanupStack::PushL(fp);


    EUNIT_ASSERT(
        fp->CheckFingerprintL(encodedBuf->Ptr(0)));

    CleanupStack::PopAndDestroy(fp);
    CleanupStack::PopAndDestroy(encodedBuf);
    CleanupStack::PopAndDestroy(msg);
    }

//Change the message type, fingerprint check must fail
void UT_CNATFWUNSAFFingerprintAttribute::TestCheckFingerprint2L()
    {
    TNATFWUNSAFTransactionID taID = KDefaultTaId();
    CNATFWUNSAFBindingRequest* msg = CNATFWUNSAFBindingRequest::NewLC(taID);

    CBufBase* encodedBuf = msg->EncodeL(KNullDesC8, ETrue);
    CleanupStack::PushL(encodedBuf);

    //Decode message, to get the FINGERPRINT object
    CNATFWUNSAFMessageFactory* decoder = CNATFWUNSAFMessageFactory::NewLC();
    CNATFWUNSAFMessage* decodedMsg = decoder->DecodeL(encodedBuf->Ptr(0));
    CleanupStack::PushL(decodedMsg);

    CNATFWUNSAFFingerprintAttribute* fp =
        static_cast<CNATFWUNSAFFingerprintAttribute*>(
        decodedMsg->Attribute(CNATFWUNSAFAttribute::EFingerprint));

    EUNIT_ASSERT(fp);
    decodedMsg->DetachAttribute(fp);
    CleanupStack::PopAndDestroy(decodedMsg);
    CleanupStack::PopAndDestroy(decoder);
    CleanupStack::PushL(fp);


    //Change the message type (EBindingRequest -> EBindingResponse)

    TUint16 msgType = CNATFWUNSAFMessage::EBindingResponse;
    TUint16 networkOrderMsgType = ByteOrder::Swap16(msgType);
    encodedBuf->Write(0, &networkOrderMsgType, 2);

    EUNIT_ASSERT(
        !fp->CheckFingerprintL(encodedBuf->Ptr(0)));

    CleanupStack::PopAndDestroy(fp);
    CleanupStack::PopAndDestroy(encodedBuf);
    CleanupStack::PopAndDestroy(msg);
    }

//Change the magic cookie, integrity check must fail
void UT_CNATFWUNSAFFingerprintAttribute::TestCheckFingerprint3L()
    {
    TNATFWUNSAFTransactionID taID = KDefaultTaId();
    CNATFWUNSAFBindingRequest* msg = CNATFWUNSAFBindingRequest::NewLC(taID);

    CBufBase* encodedBuf = msg->EncodeL(KNullDesC8, ETrue);
    CleanupStack::PushL(encodedBuf);

    //Decode message, to get the FINGERPRINT object
    CNATFWUNSAFMessageFactory* decoder = CNATFWUNSAFMessageFactory::NewLC();
    CNATFWUNSAFMessage* decodedMsg = decoder->DecodeL(encodedBuf->Ptr(0));
    CleanupStack::PushL(decodedMsg);

    CNATFWUNSAFFingerprintAttribute* fp =
        static_cast<CNATFWUNSAFFingerprintAttribute*>(
        decodedMsg->Attribute(CNATFWUNSAFAttribute::EFingerprint));

    EUNIT_ASSERT(fp);
    decodedMsg->DetachAttribute(fp);
    CleanupStack::PopAndDestroy(decodedMsg);
    CleanupStack::PopAndDestroy(decoder);
    CleanupStack::PushL(fp);


    //Change the magic cookie
    TUint32 mc = 0xaaaaaaaa;
    encodedBuf->Write(CNATFWUNSAFMessage::EMagicCookieOffset, &mc, sizeof(mc));

    EUNIT_ASSERT(
        !fp->CheckFingerprintL(encodedBuf->Ptr(0)));

    CleanupStack::PopAndDestroy(fp);
    CleanupStack::PopAndDestroy(encodedBuf);
    CleanupStack::PopAndDestroy(msg);
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFFingerprintAttribute,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestNewL - test ",
    "CNATFWUNSAFFingerprintAttribute",
    "TestNewL",
    "FUNCTIONALITY",
    SetupL, TestNewL, Teardown)

EUNIT_TEST(
    "TestEncodeAndDecodeL - test ",
    "CNATFWUNSAFFingerprintAttribute",
    "TestEncodeAndDecodeL",
    "FUNCTIONALITY",
    SetupL, TestEncodeAndDecodeL, Teardown)

EUNIT_TEST(
    "TestCheckFingerprintL - test ",
    "CNATFWUNSAFFingerprintAttribute",
    "TestCheckFingerprintL",
    "FUNCTIONALITY",
    SetupL, TestCheckFingerprintL, Teardown)

EUNIT_TEST(
    "TestCheckFingerprint2L - test ",
    "CNATFWUNSAFFingerprintAttribute",
    "TestCheckFingerprint2L",
    "FUNCTIONALITY",
    SetupL, TestCheckFingerprint2L, Teardown)

EUNIT_TEST(
    "TestCheckFingerprint3L - test ",
    "CNATFWUNSAFFingerprintAttribute",
    "TestCheckFingerprint3L",
    "FUNCTIONALITY",
    SetupL, TestCheckFingerprint3L, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
