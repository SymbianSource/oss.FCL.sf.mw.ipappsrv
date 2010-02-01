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




#ifndef CNATFWUNSAFSHAREDSECRETERRORRESPONSE_H
#define CNATFWUNSAFSHAREDSECRETERRORRESPONSE_H

// INCLUDES
#include <e32base.h>
#include "natfwunsafmessage.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
* NATFWUNSAF Shared Secret Error Response message
*
*  @lib natfwunsafprotocols.lib
*/
class CNATFWUNSAFSharedSecretErrorResponse : public CNATFWUNSAFMessage
    {
    public: // Constructors and destructor

        /**
        * Creates a CNATFWUNSAFSharedSecretErrorResponse instance
        * @param aTransactionID NATFWUNSAF transaction ID
        * @return A new instance of CNATFWUNSAFSharedSecretErrorResponse
        */
        static CNATFWUNSAFSharedSecretErrorResponse*
            NewL(const TNATFWUNSAFTransactionID& aTransactionID);

        /**
        * Creates a CNATFWUNSAFSharedSecretErrorResponse instance
        * and pushes it to CleanupStack
        * @param aTransactionID NATFWUNSAF transaction ID
        * @return A new instance of CNATFWUNSAFSharedSecretErrorResponse
        */
        static CNATFWUNSAFSharedSecretErrorResponse*
            NewLC(const TNATFWUNSAFTransactionID& aTransactionID);

        /**
        * Creates a CNATFWUNSAFSharedSecretErrorResponse instance.
        * Used from lookup table.
        * @param aTransactionID NATFWUNSAF transaction ID
        * @return A new instance of CNATFWUNSAFMessage,
        *         ownership is transferred.
        */
        static CNATFWUNSAFMessage*
            CreateL(const TNATFWUNSAFTransactionID& aTransactionID);

        /**
        * Destructor, deletes the resources of
        * CNATFWUNSAFSharedSecretErrorResponse.
        */
        ~CNATFWUNSAFSharedSecretErrorResponse();

    public: // From CNATFWUNSAFMessage

        TType Type() const;

        TBool Validate() const;

        TBool IsAllowed(TUint16 aAttributeType) const;

    private: // Constructors

        CNATFWUNSAFSharedSecretErrorResponse(
            const TNATFWUNSAFTransactionID& aTransactionID);
    };

#endif // CNATFWUNSAFSHAREDSECRETERRORRESPONSE_H


