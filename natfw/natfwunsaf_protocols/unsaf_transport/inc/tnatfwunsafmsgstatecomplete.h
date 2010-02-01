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




#ifndef TNATFWUNSAFMSGSTATECOMPLETE_H
#define TNATFWUNSAFMSGSTATECOMPLETE_H

// INCLUDES
#include "tnatfwunsafmsgstatebase.h"

// FORWARD DECLARATIONS
class MNATFWUNSAFMsgAssemblerCtx;


// CLASS DECLARATION
/**
* A class for UNSAF message assembling completed state.
*
*  @lib natfwunsafprotocols.lib
*/
NONSHARABLE_CLASS( TNATFWUNSAFMsgStateComplete ):
    public TNATFWUNSAFMsgStateBase
    {
    public:

        TNATFWUNSAFMsgStateComplete(
            MNATFWUNSAFMsgAssemblerCtx& aMsgAssembler )
            : TNATFWUNSAFMsgStateBase( aMsgAssembler ) {};

    public:

        TBool DataReceivedL( TPtr8 aData, TUint&  aNextLength );

    };

#endif // end of TNATFWUNSAFMSGSTATECOMPLETE_H



