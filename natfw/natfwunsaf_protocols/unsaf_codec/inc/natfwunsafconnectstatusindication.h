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




#ifndef CNATFWUNSAFCONNECTSTATUSINDICATION_H
#define CNATFWUNSAFCONNECTSTATUSINDICATION_H

#include <e32base.h>
#include "natfwunsafmessage.h"

/**
* UNSAF Connect Status Indication
* !!!! The implementation is still unclear as no spec available !!!!
*
*  @lib natfwunsafprotocols.lib
*  @since S60 3.2
*/
class CNATFWUNSAFConnectStatusIndication : public CNATFWUNSAFMessage
    {
public: // Constructors and destructor

    /**
    * Creates a CNATFWUNSAFConnectStatusIndication instance
    * @param aTransactionID NATFWUNSAF transaction ID
    * @return A new instance of CNATFWUNSAFConnectStatusIndication
    */
    static CNATFWUNSAFConnectStatusIndication*
        NewL(const TNATFWUNSAFTransactionID& aTransactionID);

    /**
    * Creates a CNATFWUNSAFConnectStatusIndication instance and pushes
    * it to CleanupStack
    * @param aTransactionID NATFWUNSAF transaction ID
    * @return A new instance of CNATFWUNSAFConnectStatusIndication
    */
    static CNATFWUNSAFConnectStatusIndication*
        NewLC(const TNATFWUNSAFTransactionID& aTransactionID);

    /**
    * Creates a CNATFWUNSAFConnectStatusIndication instance. Used from
    * lookup table.
    * @param aTransactionID NATFWUNSAF transaction ID
    * @return A new instance of CNATFWUNSAFMessage,
    *         ownership is transferred.
    */
    static CNATFWUNSAFMessage*
        CreateL(const TNATFWUNSAFTransactionID& aTransactionID);

    /**
    * Destructor, deletes the resources of
    * CNATFWUNSAFConnectStatusIndication.
    */
    ~CNATFWUNSAFConnectStatusIndication();

public: // From CNATFWUNSAFMessage

    TType Type() const;

    TBool Validate() const;

    TBool IsAllowed(TUint16 aAttributeType) const;

private: // Constructors

    CNATFWUNSAFConnectStatusIndication(
        const TNATFWUNSAFTransactionID& aTransactionID);
    };

#endif // CNATFWUNSAFCONNECTSTATUSINDICATION_H


