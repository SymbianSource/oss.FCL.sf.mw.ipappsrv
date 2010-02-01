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




#ifndef CNATFWUNSAFALLOCATEREQUEST_H
#define CNATFWUNSAFALLOCATEREQUEST_H

#include <e32base.h>
#include "natfwunsafmessage.h"

/**
* NATFWUNSAF Allocate Request message
*
*  @lib natfwunsafprotocols.lib
*  @since S60 3.2
*/
class CNATFWUNSAFAllocateRequest : public CNATFWUNSAFMessage
    {
public: // Constructors and destructor

    /**
    * Creates a CNATFWUNSAFAllocateRequest instance
    * @param aTransactionID NATFWUNSAF transaction ID
    * @return A new instance of CNATFWUNSAFAllocateRequest
    */
    IMPORT_C static CNATFWUNSAFAllocateRequest*
                NewL(const TNATFWUNSAFTransactionID& aTransactionID);

    /**
    * Creates a CNATFWUNSAFAllocateRequest instance and pushes it to
    * CleanupStack
    * @param aTransactionID NATFWUNSAF transaction ID
    * @return A new instance of CNATFWUNSAFAllocateRequest
    */
    IMPORT_C static CNATFWUNSAFAllocateRequest*
                NewLC(const TNATFWUNSAFTransactionID& aTransactionID);

    /**
    * Creates a CNATFWUNSAFAllocateRequest instance. Used from lookup table.
    * @param aTransactionID NATFWUNSAF transaction ID
    * @return A new instance of CNATFWUNSAFMessage,
    *         ownership is transferred.
    */
    static CNATFWUNSAFMessage*
        CreateL(const TNATFWUNSAFTransactionID& aTransactionID);

    /**
    * Destructor, deletes the resources of CNATFWUNSAFAllocateRequest.
    */
    ~CNATFWUNSAFAllocateRequest();

public: // From CNATFWUNSAFMessage

    TType Type() const;

    TBool Validate() const;

    TBool IsAllowed(TUint16 aAttributeType) const;

private: // Constructors

    CNATFWUNSAFAllocateRequest(
        const TNATFWUNSAFTransactionID& aTransactionID);
    };

#endif // CNATFWUNSAFALLOCATEREQUEST_H


