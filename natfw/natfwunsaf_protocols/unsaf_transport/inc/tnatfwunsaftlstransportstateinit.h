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





#ifndef TNATFWUNSAFTLSTRANSPORTSTATEINIT_H
#define TNATFWUNSAFTLSTRANSPORTSTATEINIT_H

//  INCLUDES
#include <e32base.h>

#include "tnatfwunsaftlstransportstatebase.h"

// FORWARD DECLARATIONS


// CLASS DECLARATION
/**
* A class for TLS transport initial state.
*
*  @lib natfwunsafprotocols.lib
*/
NONSHARABLE_CLASS( TNATFWUNSAFTlsTransportStateInit ):
        public TNATFWUNSAFTlsTransportStateBase
    {
    public: // Contructor

        TNATFWUNSAFTlsTransportStateInit(
            MNATFWUNSAFTlsTransportStateOwner& aOwner );

    public: // From TNATFWUNSAFTlsTransportStateBase

        TNATFWUNSAFTlsTransportStateBase::TState State() const;

        void EnterL();

    };

#endif // TNATFWUNSAFTLSTRANSPORTSTATEINIT_H


