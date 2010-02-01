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





#ifndef TNATFWUNSAFTLSTRANSPORTSTATEBASE_H
#define TNATFWUNSAFTLSTRANSPORTSTATEBASE_H

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class MNATFWUNSAFTlsTransportStateOwner;
class CNATFWUNSAFMessage;


// CLASS DECLARATION
/**
* A base class for TLS transport states.
*
*  @lib natfwunsafprotocols.lib
*/
NONSHARABLE_CLASS( TNATFWUNSAFTlsTransportStateBase )
    {
    public:    // State enums

        enum TState
            {
            EInit,
            ETcpConnecting,
            ETlsConnecting,
            ETlsConnected,
            EMaxStates
            };

    public: // New functions

        virtual TNATFWUNSAFTlsTransportStateBase::TState State() const = 0;

        virtual void EnterL();

        virtual void StateChangedL( TInt aStatus );

        virtual void SendL( const CNATFWUNSAFMessage& aMessage,
                            const TDesC8& aCredentials,
                            const TUint aUsefingerprint = 0 );

    protected: // Constructor

        TNATFWUNSAFTlsTransportStateBase(
            MNATFWUNSAFTlsTransportStateOwner& aOwner );

    protected: // Data

        MNATFWUNSAFTlsTransportStateOwner& iOwner;
    };

#endif // TNATFWUNSAFTLSTRANSPORTSTATEBASE_H


