/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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





#include "NATFWUNSAFTransportTestUtils.h"

#include "natfwunsafmessage.h"
#include "natfwunsafmessagefactory.h"

CNATFWUNSAFMessage* NATFWUNSAFTransportTestUtils::CreateUNSAFBindingRequestL()
    {
    CNATFWUNSAFMessage* msg = CreateUNSAFBindingRequestLC();
    CleanupStack::Pop( msg );
    return msg;
    }

CNATFWUNSAFMessage* NATFWUNSAFTransportTestUtils::CreateUNSAFBindingRequestLC()
    {
    const TUint8 KMessageToDecode[] =
        {
        0, 1, //NATFWUNSAF message type: Binding Request
        0, 0, //Message length
        0x21, 0x12, 0xa4, 0x42, //Magic cookie
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb //TransactionID
        };

    CNATFWUNSAFMessageFactory* factory = CNATFWUNSAFMessageFactory::NewLC();

    HBufC8* byteStream = HBufC8::NewLC( sizeof( KMessageToDecode ) );
    byteStream->Des().Append( KMessageToDecode, sizeof( KMessageToDecode ) );

    CNATFWUNSAFMessage* msg = factory->DecodeL( *byteStream );
    CleanupStack::PopAndDestroy( byteStream );
    CleanupStack::PopAndDestroy( factory );
    CleanupStack::PushL( msg );
    return msg;
    }

CNATFWUNSAFMessage* NATFWUNSAFTransportTestUtils::CreateUNSAFBindingResponseLC()
    {
    const TInt KAttributeCount = 3;

    // Value should be same as CNATFWUNSAFAddressAttribute::EAttributeValueSize
    const TInt KAttributeValueSize = 8;

    // Value should be same as CNATFWUNSAFAttribute::EValueOffset
    const TInt KValueOffset = 4;

    TUint8 msgLen = ( KValueOffset + KAttributeValueSize ) * KAttributeCount;

    const TUint8 KMessageToDecode[] =
        {
        1, 1, //NATFWUNSAF message type: Binding Response
        0, msgLen, //Message length
        0x21, 0x12, 0xa4, 0x42, //Magic cookie
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, //TransactionID

        //MAPPED-ADDRESS attribute
        0, 1, //type
        0, KAttributeValueSize, //length of value element
        0, //undefined
        1, //family
        0x12, 0x34, //port
        0x50, 0x40, 0xff, 0xaa, //address

        //SOURCE-ADDRESS attribute
        0, 4, //type
        0, KAttributeValueSize, //length of value element
        0, //undefined
        1, //family
        0x13, 0xC4, //port (5060 in decimal)
        1, 2, 3, 4, //address (1.2.3.4)

        //CHANGED-ADDRESS attribute
        0, 5, //type
        0, KAttributeValueSize, //length of value element
        0, //undefined
        1, //family
        0x13, 0xC5, //port (5061 in decimal)
        2, 3, 4, 5  //address (2.3.4.5)
        };

    CNATFWUNSAFMessageFactory* factory = CNATFWUNSAFMessageFactory::NewLC();

    HBufC8* byteStream = HBufC8::NewLC( sizeof( KMessageToDecode ) );
    byteStream->Des().Append( KMessageToDecode, sizeof( KMessageToDecode ) );

    CNATFWUNSAFMessage* msg = factory->DecodeL( *byteStream );
    CleanupStack::PopAndDestroy( byteStream );
    CleanupStack::PopAndDestroy( factory );

    CleanupStack::PushL( msg );
    return msg;
    }
