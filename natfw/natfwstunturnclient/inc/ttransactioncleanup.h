/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef C_TRANSACTIONCLEANUP_H
#define C_TRANSACTIONCLEANUP_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CSTUNTransaction;

// CLASS DECLARATION
/**
 * This class is used to terminate the STUN transaction, if a leave occurs
 * while the transaction was processing a received STUN message.
 */
class TTransactionCleanup
    {
public: // Constructor

    /**
     * Constructor
     *
     * @param aTransaction STUN transaction, that will be terminated.
     */
    TTransactionCleanup( CSTUNTransaction& aTransaction );

public: // New functions

    /**
     * Returns a TCleanupItem which can be pushed into CleanupStack.
     *
     * @return TCleanupItem
     */
    TCleanupItem CleanupItem();

    /**
     * When CleanupStack destroys the TCleanupItem, this function is called.
     * The iTransaction is terminated.
     *
     * @pre aCleanup != NULL
     *
     * @param aCleanup Pointer to a TTransactionCleanup. Ownership is not
     *          transferred.
     */
    static void TerminateTransaction( TAny* aCleanup );

private: // Data

    CSTUNTransaction& iTransaction;
    };

#endif // C_TRANSACTIONCLEANUP_H
