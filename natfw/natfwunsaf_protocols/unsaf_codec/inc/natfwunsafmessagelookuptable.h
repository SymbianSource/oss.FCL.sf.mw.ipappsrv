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




#ifndef CMESSAGELOOKUPTABLE_H
#define CMESSAGELOOKUPTABLE_H

// INCLUDES
#include <e32base.h>
#include "natfwunsafmessagelookuptableitem.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
* Lookup table for mapping UNSAF message types to factory functions.
*
*  @lib natfwunsafprotocols.lib
*/
NONSHARABLE_CLASS(CMessageLookupTable) : public CBase
    {
    public: // Constructors and destructor

        static CMessageLookupTable* NewL();

        /**
        * Destructor, deletes the resources of CMessageLookupTable.
        */
        virtual ~CMessageLookupTable();

    public: // New functions

        /**
        * Searches for a factory function to instantiate an UNSAF message of
        * the indicated type.
        * @param aMessageType UNSAF message type
        * @return TFactoryFnPtr Function to instantiate a UNSAF message,
        *    or NULL if match was not found.
        */
        TFactoryFnPtr Find(TUint16 aMessageType);

    private:

        CMessageLookupTable();

        void ConstructL();

        /**
        * Adds a new item into the table.
        * @param aMessageType UNSAF message type
        * @param aFactoryFn Function to instantiate a UNSAF message
        */
        void AddItemL(CNATFWUNSAFMessage::TType aMessageType,
                         TFactoryFnPtr aFactoryFn);

    private: // Data

        RArray<TMessageLookupTableItem> iTable;
    };

#endif // CMESSAGELOOKUPTABLE_H


