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




#ifndef TATTRIBUTELOOKUPTABLEITEM_H
#define TATTRIBUTELOOKUPTABLEITEM_H

// INCLUDES
#include <e32base.h>
#include "natfwunsafattribute.h"

// FORWARD DECLARATIONS

// TYPEDEFS
typedef CNATFWUNSAFAttribute* (*TDecodeFnPtr)(const TDesC8& aValue);

// CLASS DECLARATION
/**
* Lookup table item containing the mapping of UNSAF attribute types to
* decode functions.
*
*  @lib natfwunsafprotocols.lib
*/
NONSHARABLE_CLASS(TAttributeLookupTableItem)
    {
    public: // Constructors and destructor
        /**
        * Constructor
        * @param aAttributeType UNSAF attribute type
        * @param aDecodeFn Decode function for the attribute type
        */
        TAttributeLookupTableItem(CNATFWUNSAFAttribute::TType aAttributeType,
                                   TDecodeFnPtr aDecodeFn);

    public: // New functions

        CNATFWUNSAFAttribute::TType AttributeType() const;

        TDecodeFnPtr DecodeFunction() const;

    private: // Data

        CNATFWUNSAFAttribute::TType iAttributeType;
        TDecodeFnPtr iDecodeFn;
    };

#endif // TATTRIBUTELOOKUPTABLEITEM_H


