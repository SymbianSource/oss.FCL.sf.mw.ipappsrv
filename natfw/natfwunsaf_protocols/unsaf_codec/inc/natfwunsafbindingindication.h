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




#ifndef CNATFWUNSAFBINDINGINDICATION_H
#define CNATFWUNSAFBINDINGINDICATION_H

// INCLUDES
#include <e32base.h>
#include "natfwinternaldefs.h"    //UNIT_TEST
#include "natfwunsafmessage.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
* NATFWUNSAF Binding Indication message
*
*  @lib natfwunsafprotocols.lib
*/
class CNATFWUNSAFBindingIndication : public CNATFWUNSAFMessage
    {
    public: // Constructors and destructor

        /**
        * Creates a CNATFWUNSAFBindingIndication instance
        * @param aTransactionID NATFWUNSAF transaction ID
        * @return A new instance of CNATFWUNSAFBindingIndication
        */
        IMPORT_C static CNATFWUNSAFBindingIndication*
                    NewL(const TNATFWUNSAFTransactionID& aTransactionID);

        /**
        * Creates a CNATFWUNSAFBindingIndication instance and pushes it to
        * CleanupStack
        * @param aTransactionID NATFWUNSAF transaction ID
        * @return A new instance of CNATFWUNSAFBindingIndication
        */
        IMPORT_C static CNATFWUNSAFBindingIndication*
                    NewLC(const TNATFWUNSAFTransactionID& aTransactionID);

        /**
        * Creates a CNATFWUNSAFBindingIndication instance. Used from lookup
        * table.
        * @param aTransactionID NATFWUNSAF transaction ID
        * @return A new instance of CNATFWUNSAFMessage,
        *         ownership is transferred.
        */
        static CNATFWUNSAFMessage*
            CreateL(const TNATFWUNSAFTransactionID& aTransactionID);

        /**
        * Destructor, deletes the resources of CNATFWUNSAFBindingIndication.
        */
        ~CNATFWUNSAFBindingIndication();

    public: // From CNATFWUNSAFMessage

        TType Type() const;

        TBool Validate() const;

        TBool IsAllowed(TUint16 aAttributeType) const;

    private: // Constructors

        CNATFWUNSAFBindingIndication(
            const TNATFWUNSAFTransactionID& aTransactionID);
    };

#endif // CNATFWUNSAFBINDINGINDICATION_H


