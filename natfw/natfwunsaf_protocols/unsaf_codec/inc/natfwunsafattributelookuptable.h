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




#ifndef CATTRIBUTELOOKUPTABLE_H
#define CATTRIBUTELOOKUPTABLE_H

// INCLUDES
#include <e32base.h>
#include "natfwunsafattributelookuptableitem.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
* Lookup table for mapping a key value to a function pointer.
*
*  @lib natfwunsafprotocols.lib
*/
NONSHARABLE_CLASS(CAttributeLookupTable) : public CBase
    {
    public: // Constructors and destructor

        static CAttributeLookupTable* NewL();

        /**
        * Destructor, deletes the resources of CAttributeLookupTable.
        */
        virtual ~CAttributeLookupTable();

    public: // New functions

        /**
        * Searches for the decode function of the UNSAF attribute type
        * identified by aAttributeType.
        * @param aAttributeType UNSAF attribute type
        * @return TDecodeFnPtr Pointer to the decode function, or NULL if match
        *    was not found.
        */
        TDecodeFnPtr Find(TUint16 aAttributeType);

    private:

        CAttributeLookupTable();

        void ConstructL();

        /**
        * Adds a new item into the table.
        * @param aAttributeType UNSAF attribute type
        * @param aDecodeFn Decode function for the attribute type
        */
        void AddItemL(CNATFWUNSAFAttribute::TType aAttributeType,
                      TDecodeFnPtr aDecodeFn);

    private: // Data

        RArray<TAttributeLookupTableItem> iTable;
    };

#endif // CATTRIBUTELOOKUPTABLE_H


