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




#ifndef CNATFWUNSAFSETACTIVEDESTINATIONERRORRESPONSE_H
#define CNATFWUNSAFSETACTIVEDESTINATIONERRORRESPONSE_H

#include <e32base.h>
#include "natfwunsafmessage.h"

/**
* UNSAF SetActiveDestinationErrorResponse message
*
*  @lib natfwunsafprotocols.lib
*  @since S60 3.2
*/
class CNATFWUNSAFSetActiveDestinationErrorResponse : public CNATFWUNSAFMessage
    {
public: // Constructors and destructor

    /**
    * Creates a CNATFWUNSAFSetActiveDestinationErrorResponse instance
    * @param aTransactionID NATFWUNSAF transaction ID
    * @return A new instance of
    *         CNATFWUNSAFSetActiveDestinationErrorResponse
    */
    static CNATFWUNSAFSetActiveDestinationErrorResponse*
        NewL(const TNATFWUNSAFTransactionID& aTransactionID);

    /**
    * Creates a CNATFWUNSAFSetActiveDestinationErrorResponse instance and
    * pushes it to CleanupStack
    * @param aTransactionID NATFWUNSAF transaction ID
    * @return A new instance of
    *         CNATFWUNSAFSetActiveDestinationErrorResponse
    */
    static CNATFWUNSAFSetActiveDestinationErrorResponse*
        NewLC(const TNATFWUNSAFTransactionID& aTransactionID);

    /**
    * Creates a CNATFWUNSAFSetActiveDestinationErrorResponse instance.
    * Used from lookup table.
    * @param aTransactionID NATFWUNSAF transaction ID
    * @return A new instance of CNATFWUNSAFMessage,
    *         ownership is transferred.
    */
    static CNATFWUNSAFMessage*
        CreateL(const TNATFWUNSAFTransactionID& aTransactionID);

    /**
    * Destructor, deletes the resources of
    * CNATFWUNSAFSetActiveDestinationErrorResponse.
    */
    ~CNATFWUNSAFSetActiveDestinationErrorResponse();

public: // From CNATFWUNSAFMessage

    TType Type() const;

    TBool Validate() const;

    TBool IsAllowed(TUint16 aAttributeType) const;

private: // Constructors

    CNATFWUNSAFSetActiveDestinationErrorResponse(
        const TNATFWUNSAFTransactionID& aTransactionID);
    };

#endif // CNATFWUNSAFSETACTIVEDESTINATIONERRORRESPONSE_H


