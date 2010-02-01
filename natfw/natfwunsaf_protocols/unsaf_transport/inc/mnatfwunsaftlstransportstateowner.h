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





#ifndef MNATFWUNSAFTLSTRANSPORTSTATEOWNER_H
#define MNATFWUNSAFTLSTRANSPORTSTATEOWNER_H

//  INCLUDES
#include <e32std.h>

#include "tnatfwunsaftlstransportstatebase.h"

// FORWARD DECLARATIONS
class CNATFWUNSAFMessage;

// CLASS DECLARATION
/**
*  Defines an interface of CNATFWUNSAFTlsTransport used by state classes.
*
*  @lib natfwunsafprotocols.lib
*/
class MNATFWUNSAFTlsTransportStateOwner
    {
    public:    // New functions

        virtual void EnterStateL(
            TNATFWUNSAFTlsTransportStateBase::TState aState ) = 0;

        virtual void ConnectTcpL() = 0;

        virtual void ConnectTlsL() = 0;

        virtual void TlsConnectedL() = 0;

        virtual void SendOverTlsL( const CNATFWUNSAFMessage& aMessage,
                                   const TDesC8& aCredentials,
                                   const TUint aUseFingerprint = 0 ) = 0;

    };

#endif // MNATFWUNSAFTLSTRANSPORTSTATEOWNER_H



