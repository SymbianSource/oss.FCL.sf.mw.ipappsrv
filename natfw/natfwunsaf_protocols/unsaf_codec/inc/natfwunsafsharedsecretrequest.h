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




#ifndef CNATFWUNSAFSHAREDSECRETREQUEST_H
#define CNATFWUNSAFSHAREDSECRETREQUEST_H

// INCLUDES
#include <e32base.h>
#include "natfwunsafmessage.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
* NATFWUNSAF Shared Secret Request message
*
*  @lib natfwunsafprotocols.lib
*/
class CNATFWUNSAFSharedSecretRequest : public CNATFWUNSAFMessage
    {
    public: // Constructors and destructor

        /**
        * Creates a CNATFWUNSAFSharedSecretRequest instance
        * @param aTransactionID NATFWUNSAF transaction ID
        * @return A new instance of CNATFWUNSAFSharedSecretRequest
        */
        IMPORT_C static CNATFWUNSAFSharedSecretRequest*
                NewL(const TNATFWUNSAFTransactionID& aTransactionID);

        /**
        * Creates a CNATFWUNSAFSharedSecretRequest instance and pushes it to
        * CleanupStack
        * @param aTransactionID NATFWUNSAF transaction ID
        * @return A new instance of CNATFWUNSAFSharedSecretRequest
        */
        IMPORT_C static CNATFWUNSAFSharedSecretRequest*
                NewLC(const TNATFWUNSAFTransactionID& aTransactionID);

        /**
        * Creates a CNATFWUNSAFSharedSecretRequest instance.
        * Used from lookup table.
        * @param aTransactionID NATFWUNSAF transaction ID
        * @return A new instance of CNATFWUNSAFMessage,
        *         ownership is transferred.
        */
        static CNATFWUNSAFMessage*
            CreateL(const TNATFWUNSAFTransactionID& aTransactionID);

        /**
        * Destructor, deletes the resources of CNATFWUNSAFSharedSecretRequest.
        */
        ~CNATFWUNSAFSharedSecretRequest();

    public: // From CNATFWUNSAFMessage

        TType Type() const;

        TBool Validate() const;

        TBool IsAllowed(TUint16 aAttributeType) const;

    private: // Constructors

        CNATFWUNSAFSharedSecretRequest(
            const TNATFWUNSAFTransactionID& aTransactionID);
    };

#endif // CNATFWUNSAFSHAREDSECRETREQUEST_H


