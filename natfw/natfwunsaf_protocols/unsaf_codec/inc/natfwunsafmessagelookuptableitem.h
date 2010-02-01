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




#ifndef TMESSAGELOOKUPTABLEITEM_H
#define TMESSAGELOOKUPTABLEITEM_H

// INCLUDES
#include <e32base.h>
#include "natfwunsafmessage.h"

// FORWARD DECLARATIONS

// TYPEDEFS
typedef CNATFWUNSAFMessage*
    (*TFactoryFnPtr)(const TNATFWUNSAFTransactionID& aTransactionID);

// CLASS DECLARATION
/**
* Lookup table item containing the mapping of UNSAF message types to
* factory functions.
*
*  @lib natfwunsafprotocols.lib
*/
NONSHARABLE_CLASS(TMessageLookupTableItem)
    {
    public: // Constructors and destructor
        /**
        * Constructor
        * @param aMessageType UNSAF message type
        * @param TFactoryFnPtr Function to instantiate a UNSAF message
        */
        TMessageLookupTableItem(CNATFWUNSAFMessage::TType aMessageType,
                                 TFactoryFnPtr aFactoryFn);

    public: // New functions

        CNATFWUNSAFMessage::TType MessageType() const;
        TFactoryFnPtr FactoryFunction() const;

    private: // Data

        CNATFWUNSAFMessage::TType iMessageType;
        TFactoryFnPtr iFactoryFn;
    };

#endif // TMESSAGELOOKUPTABLEITEM_H


