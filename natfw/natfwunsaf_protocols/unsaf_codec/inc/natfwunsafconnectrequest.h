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




#ifndef CNATFWUNSACONNECTREQUEST_H
#define CNATFWUNSACONNECTREQUEST_H

#include <e32base.h>
#include "natfwunsafmessage.h"

/**
* UNSAF Connect Request message
*
*  @lib natfwunsafprotocols.lib
*  @since S60 3.2
*/
class CNATFWUNSAFConnectRequest : public CNATFWUNSAFMessage
    {
public: // Constructors and destructor

    /**
    * Creates a CNATFWUNSAFConnectRequest instance
    * @param aTransactionID NATFWUNSAF transaction ID
    * @return A new instance of CNATFWUNSAFConnectRequest
    */
    IMPORT_C static CNATFWUNSAFConnectRequest*
                NewL(const TNATFWUNSAFTransactionID& aTransactionID);

    /**
    * Creates a CNATFWUNSAFConnectRequest instance and pushes it to
    * CleanupStack
    * @param aTransactionID NATFWUNSAF transaction ID
    * @return A new instance of CNATFWUNSAFConnectRequest
    */
    IMPORT_C static CNATFWUNSAFConnectRequest*
                NewLC(const TNATFWUNSAFTransactionID& aTransactionID);

    /**
    * Creates a CNATFWUNSAFConnectRequest instance. Used from lookup table.
    * @param aTransactionID NATFWUNSAF transaction ID
    * @return A new instance of CNATFWUNSAFMessage,
    *         ownership is transferred.
    */
    static CNATFWUNSAFMessage*
        CreateL(const TNATFWUNSAFTransactionID& aTransactionID);

    /**
    * Destructor, deletes the resources of CNATFWUNSAFConnectRequest.
    */
    ~CNATFWUNSAFConnectRequest();

public: // From CNATFWUNSAFMessage

    TType Type() const;

    TBool Validate() const;

    TBool IsAllowed(TUint16 aAttributeType) const;

private: // Constructors

    CNATFWUNSAFConnectRequest(
        const TNATFWUNSAFTransactionID& aTransactionID);
    };

#endif // CNATFWUNSAFCONNECTREQUEST_H


