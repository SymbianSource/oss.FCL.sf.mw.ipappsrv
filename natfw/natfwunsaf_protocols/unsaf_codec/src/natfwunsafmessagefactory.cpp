/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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




#include <es_sock.h>
#include "natfwunsafmessagefactory.h"
#include "natfwunsafattributelookuptable.h"
#include "natfwunsafmessagelookuptable.h"
#include "natfwunsaflog.h"

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessageFactory::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFMessageFactory* CNATFWUNSAFMessageFactory::NewL()
    {
    CNATFWUNSAFMessageFactory* self = CNATFWUNSAFMessageFactory::NewLC();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessageFactory::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFMessageFactory* CNATFWUNSAFMessageFactory::NewLC()
    {
    CNATFWUNSAFMessageFactory* self = new (ELeave) CNATFWUNSAFMessageFactory();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessageFactory::ConstructL
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFMessageFactory::ConstructL()
    {
    iAttributeLookupTable = CAttributeLookupTable::NewL();
    iMessageLookupTable = CMessageLookupTable::NewL();
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessageFactory::CNATFWUNSAFMessageFactory
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessageFactory::CNATFWUNSAFMessageFactory()
    {
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessageFactory::~CNATFWUNSAFMessageFactory
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFMessageFactory::~CNATFWUNSAFMessageFactory()
    {
    delete iAttributeLookupTable;
    delete iMessageLookupTable;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessageFactory::MessageLength
// -----------------------------------------------------------------------------
//
TInt CNATFWUNSAFMessageFactory::MessageLength(
    const TDesC8& aNATFWUNSAFMessage) const
    {
    __TEST_INVARIANT;

    return CNATFWUNSAFMessage::MessageLength(aNATFWUNSAFMessage);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessageFactory::DecodeHeaderLC
// Check the two MSBs are zero and magic cookie exists. Otherwise this is not a
// STUN message. As the codec is currently used only by the STUN client (not by
// any server implementation), all responses must have the magic cookie, as it
// was put into requests.
// Can't use CreateL here, as msgType is not necessarily any of the
// enumerations.
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage* CNATFWUNSAFMessageFactory::DecodeHeaderLC(
    const TDesC8& aNATFWUNSAFMessage) const
    {
    __TEST_INVARIANT;
    __ASSERT_ALWAYS(
        aNATFWUNSAFMessage.Length() >= CNATFWUNSAFMessage::EHeaderSize,
            User::Leave(KErrArgument));

    TUint16 msgType = BigEndian::Get16(
        aNATFWUNSAFMessage.Mid(CNATFWUNSAFMessage::EMessageTypeOffset).Ptr());

    __ASSERT_ALWAYS(((
        msgType & CNATFWUNSAFMessage::EMessageTypePrefixMask) == 0) &&
            (BigEndian::Get32(aNATFWUNSAFMessage.Mid(
                CNATFWUNSAFMessage::EMagicCookieOffset).Ptr()) ==
                    CNATFWUNSAFMessage::EMagicCookie),
                        User::Leave(KErrArgument));


    TFactoryFnPtr factoryFn = iMessageLookupTable->Find(msgType);
    __ASSERT_ALWAYS(factoryFn, User::Leave(KErrArgument));

    TPtrC8 transactionID =
        aNATFWUNSAFMessage.Mid(CNATFWUNSAFMessage::ETransactionIDOffset,
                          KMaxNATFWUNSAFTransactionIdLength);

    CNATFWUNSAFMessage* msg = (*factoryFn)(transactionID);
    CleanupStack::PushL(msg);
    return msg;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessageFactory::DecodeL
// Mandatory UNSAF attributes must be understood. Optional attributes are
// ignored when not understood.
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWUNSAFMessage* CNATFWUNSAFMessageFactory::DecodeL(
    const TDesC8& aNATFWUNSAFMessage) const
    {
    __TEST_INVARIANT;

    CNATFWUNSAFMessage* msg = DecodeHeaderLC(aNATFWUNSAFMessage);

    NATFWUNSAF_BYTESTREAMLOG("CNATFWUNSAFMessageFactory::DecodeL data",
        aNATFWUNSAFMessage);

    TInt msgLengthExcludingHeader = MessageLength(aNATFWUNSAFMessage);
    TInt pos(0);

    while (pos < msgLengthExcludingHeader)
        {
        TPtrC8 msgBody = aNATFWUNSAFMessage.Mid(
            CNATFWUNSAFMessage::EHeaderSize + pos);

        //Can't use the enum, as attribute type could have an unrecognized
        //value
        TUint16 attrType(0);
        TUint16 attrTotalLength(0); //includes type, length, value

        CNATFWUNSAFAttribute::ParseTypeAndLengthL(msgBody, attrType,
            attrTotalLength);
        TDecodeFnPtr decodeFn = iAttributeLookupTable->Find(attrType);

        if (decodeFn)
            {
            //After detecting this is a known attribute, check it is allowed to
            //be present in this msgType, if not, ignore it w/o decoding.
            if (msg->IsAllowed(attrType))
                {
                CNATFWUNSAFAttribute* attr = (*decodeFn)(msgBody);
                CleanupStack::PushL(attr);
                msg->AddAttributeL(attr);
                CleanupStack::Pop(attr);
                }
            }
        else
            {
            if (CNATFWUNSAFAttribute::IsMandatory(attrType))
                {
                msg->UnknownMandatoryAttributeFound();
                }
            }

        pos += attrTotalLength;
        }

    CleanupStack::Pop(msg);
    return msg;
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessageFactory::CreateL
// -----------------------------------------------------------------------------
//
CNATFWUNSAFMessage* CNATFWUNSAFMessageFactory::CreateL(
    const CNATFWUNSAFMessage::TType& aType,
    const TNATFWUNSAFTransactionID& aTransactionID) const
    {
    __TEST_INVARIANT;

    TFactoryFnPtr factoryFn = iMessageLookupTable->Find(aType);
    __ASSERT_ALWAYS(factoryFn, User::Leave(KErrArgument));

    return (*factoryFn)(aTransactionID);
    }

// -----------------------------------------------------------------------------
// CNATFWUNSAFMessageFactory::__DbgTestInvariant
// -----------------------------------------------------------------------------
//
void CNATFWUNSAFMessageFactory::__DbgTestInvariant() const
    {
#if defined(_DEBUG)
    if (!iAttributeLookupTable || !iMessageLookupTable)
        {
        User::Invariant();
        }
#endif
    }
