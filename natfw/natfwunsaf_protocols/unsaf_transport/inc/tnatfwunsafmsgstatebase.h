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




#ifndef TNATFWUNSAFMSGSTATEBASE_H
#define TNATFWUNSAFMSGSTATEBASE_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class MNATFWUNSAFMsgAssemblerCtx;


// CLASS DECLARATION
/**
* A base class for UNSAF message assembling states.
*
*  @lib natfwunsafprotocols.lib
*/
NONSHARABLE_CLASS( TNATFWUNSAFMsgStateBase )
    {
    public:    // State enums

        enum TNATFWUNSAFMsgStateValue
            {
            EMsgInit,
            EMsgHeaderStart,
            EMsgHeaderEnd,
            EMsgComplete,
            EMsgMaxStates
            };

    protected: // Constructor

        TNATFWUNSAFMsgStateBase( MNATFWUNSAFMsgAssemblerCtx& aMsgAssembler );

    public: // New functions

        /**
        * Handles assembling of the received data.
        *
        * @param aData received data to be assembled.
        * @param aNextLength will be filled with length of missing data.
        * @return ETrue if assembling completed, otherwise EFalse.
        */
        virtual TBool DataReceivedL( TPtr8 aData, TUint& aNextLength ) = 0;

    protected: // Data

        MNATFWUNSAFMsgAssemblerCtx& iMsgAssembler;

    };

#endif // end of TNATFWUNSAFMSGSTATEBASE_H


