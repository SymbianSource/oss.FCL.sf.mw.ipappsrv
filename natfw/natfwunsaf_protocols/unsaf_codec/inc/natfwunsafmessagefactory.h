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





#ifndef CNATFWUNSAFMESSAGEFACTORY_H
#define CNATFWUNSAFMESSAGEFACTORY_H

//  INCLUDES
#include <e32base.h>
#include "natfwunsafmessage.h"
#include "natfwunsaftransactionid.h"

// FORWARD DECLARATIONS
class CAttributeLookupTable;
class CMessageLookupTable;

// CLASS DECLARATION
/**
* Class acts as a factory for NATFWUNSAF requests and responses
* hiding the actual message types from the user
*
*  @lib natfwunsafprotocols.lib
*/
class CNATFWUNSAFMessageFactory : public CBase
    {
    public: // Constructors and destructors

        /**
        * Creates a CNATFWUNSAFMessageFactory instance
        * @return A new instance of CNATFWUNSAFMessageFactory
        */
        IMPORT_C static CNATFWUNSAFMessageFactory* NewL();

        /**
        * Creates a CNATFWUNSAFMessageFactory instance and pushes it to
        * CleanupStack
        * @return A new instance of CNATFWUNSAFMessageFactory
        */
        IMPORT_C static CNATFWUNSAFMessageFactory* NewLC();

        /**
        * Destructor
        */
        IMPORT_C virtual ~CNATFWUNSAFMessageFactory();

    public: // New functions

        /**
        * Parses the UNSAF message length from a UNSAF message
        * (bytes 3 and 4) and returns the parsed length.
        * @param aNATFWUNSAFMessage a UNSAF message
        * @return KErrNotFound if length field cannot be parsed.
        *         Otherwise the value of the length field.
        */
        TInt MessageLength(const TDesC8& aNATFWUNSAFMessage) const;

        /**
        * Parses a UNSAF message including the 20 byte UNSAF
        * message header
        * @param aNATFWUNSAFMessage a UNSAF message
        * @return A new instance of CNATFWUNSAFMessage.
        *         The ownership is transferred.
        */
        IMPORT_C CNATFWUNSAFMessage* DecodeL(
            const TDesC8& aNATFWUNSAFMessage) const;

        /**
        * Creates a UNSAF message instance without any UNSAF
        * attributes
        * @param aType a UNSAF message type
        * @param aTransactionID a NATFWUNSAF transaction ID
        * @return A new instance of CNATFWUNSAFMessage.
        *         The ownership is transferred.
        */
        CNATFWUNSAFMessage* CreateL(const CNATFWUNSAFMessage::TType& aType,
            const TNATFWUNSAFTransactionID& aTransactionID) const;

    private:

        CNATFWUNSAFMessageFactory();

        void ConstructL();

        /**
        * Decodes a UNSAF message header, instantiates a UNSAF message of an
        * appropriate type and pushes it into CleanupStack.
        * @param aNATFWUNSAFMessage Byte stream to decode as a UNSAF message
        * @return A new instance of CNATFWUNSAFMessage.
        * The ownership is transferred.
        */
        CNATFWUNSAFMessage* DecodeHeaderLC(
            const TDesC8& aNATFWUNSAFMessage) const;

    private: // Data

        //NATFWUNSAF codec is only used by the NAT traversal-server,´
        //so TLS need not be used

        //Owned
        CAttributeLookupTable* iAttributeLookupTable;

        //Owned
        CMessageLookupTable* iMessageLookupTable;

    private: // For testing purposes

        __DECLARE_TEST;
    };

#endif // CNATFWUNSAFMESSAGEFACTORY_H


