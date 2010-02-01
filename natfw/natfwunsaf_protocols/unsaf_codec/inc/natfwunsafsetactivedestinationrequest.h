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




#ifndef CNATFWUNSAFSETACTIVEDESTINATIONREQUEST_H
#define CNATFWUNSAFSETACTIVEDESTINATIONREQUEST_H

#include <e32base.h>
#include "natfwunsafmessage.h"

/**
* UNSAF SetActiveDestination Request message
*
*  @lib natfwunsafprotocols.lib
*  @since S60 3.2
*/
class CNATFWUNSAFSetActiveDestinationRequest : public CNATFWUNSAFMessage
    {
public: // Constructors and destructor

    /**
    * Creates a CNATFWUNSAFSetActiveDestinationRequest instance
    * @param aTransactionID NATFWUNSAF transaction ID
    * @return A new instance of CNATFWUNSAFSetActiveDestinationRequest
    */
    IMPORT_C static CNATFWUNSAFSetActiveDestinationRequest*
                NewL(const TNATFWUNSAFTransactionID& aTransactionID);

    /**
    * Creates a CNATFWUNSAFSetActiveDestinationRequest instance and
    * pushes it to CleanupStack
    * @param aTransactionID NATFWUNSAF transaction ID
    * @return A new instance of CNATFWUNSAFSetActiveDestinationRequest
    */
    IMPORT_C static CNATFWUNSAFSetActiveDestinationRequest*
                NewLC(const TNATFWUNSAFTransactionID& aTransactionID);

    /**
    * Creates a CNATFWUNSAFSetActiveDestinationRequest instance.
    * Used from lookup table.
    * @param aTransactionID NATFWUNSAF transaction ID
    * @return A new instance of CNATFWUNSAFMessage,
    *         ownership is transferred.
    */
    static CNATFWUNSAFMessage*
        CreateL(const TNATFWUNSAFTransactionID& aTransactionID);

    /**
    * Destructor, deletes the resources of
    * CNATFWUNSAFSetActiveDestinationRequest.
    */
    ~CNATFWUNSAFSetActiveDestinationRequest();

public: // From CNATFWUNSAFMessage

    TType Type() const;

    TBool Validate() const;

    TBool IsAllowed(TUint16 aAttributeType) const;

private: // Constructors

    CNATFWUNSAFSetActiveDestinationRequest(
        const TNATFWUNSAFTransactionID& aTransactionID);
    };

#endif // CNATFWUNSAFSETACTIVEDESTINATIONREQUEST_H


