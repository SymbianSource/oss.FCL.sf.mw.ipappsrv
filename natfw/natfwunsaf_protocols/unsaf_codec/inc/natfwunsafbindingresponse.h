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




#ifndef CNATFWUNSAFBINDINGRESPONSE_H
#define CNATFWUNSAFBINDINGRESPONSE_H

// INCLUDES
#include <e32base.h>
#include "natfwunsafmessage.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
* NATFWUNSAF Binding Response message
*
*  @lib natfwunsafprotocols.lib
*/
class CNATFWUNSAFBindingResponse : public CNATFWUNSAFMessage
    {
    public: // Constructors and destructor

        /**
        * Creates a CNATFWUNSAFBindingResponse instance
        * @param aTransactionID NATFWUNSAF transaction ID
        * @return A new instance of CNATFWUNSAFBindingResponse
        */
        IMPORT_C static CNATFWUNSAFBindingResponse*
            NewL(const TNATFWUNSAFTransactionID& aTransactionID);

        /**
        * Creates a CNATFWUNSAFBindingResponse instance and pushes it to
        * CleanupStack
        * @param aTransactionID NATFWUNSAF transaction ID
        * @return A new instance of CNATFWUNSAFBindingResponse
        */
        IMPORT_C static CNATFWUNSAFBindingResponse*
            NewLC(const TNATFWUNSAFTransactionID& aTransactionID);

        /**
        * Creates a CNATFWUNSAFBindingResponse instance. Used from lookup
        * table.
        * @param aTransactionID NATFWUNSAF transaction ID
        * @return A new instance of CNATFWUNSAFMessage,
        *         ownership is transferred.
        */
        static CNATFWUNSAFMessage*
            CreateL(const TNATFWUNSAFTransactionID& aTransactionID);

        /**
        * Destructor, deletes the resources of CNATFWUNSAFBindingResponse.
        */
        ~CNATFWUNSAFBindingResponse();

    public: // From CNATFWUNSAFMessage

        TType Type() const;

        TBool Validate() const;

        TBool IsAllowed(TUint16 aAttributeType) const;

    private: // Constructors

        CNATFWUNSAFBindingResponse(
            const TNATFWUNSAFTransactionID& aTransactionID);
    };

#endif // CNATFWUNSAFBINDINGRESPONSE_H


