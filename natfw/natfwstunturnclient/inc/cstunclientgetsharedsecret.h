/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef C_CSTUNCLIENTGETSHAREDSECRET_H
#define C_CSTUNCLIENTGETSHAREDSECRET_H

// INCLUDES
#include <e32base.h>
#include "cstunclientstate.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
 *  In this state, STUN client is obtaining a shared secret from the STUN
 *  server.
 */
class CSTUNClientGetSharedSecret : public CSTUNClientState
    {
    public: // Constructors and destructor

        CSTUNClientGetSharedSecret( const CSTUNClientState& aReady );

        ~CSTUNClientGetSharedSecret();

    public: // From CSTUNClientState     

        void SharedSecretReceivedL( CSTUNClientImplementation& aContext ) const;

        void SharedSecretErrorL( CSTUNClientImplementation& aContext,
                                 TInt aError ) const;

        //ObtainSharedSecretL event should never be received in this state,
        //since bindings cannot be created before STUN client has initialized
        //itself, and reached Ready state.

    private:

        CSTUNClientGetSharedSecret();

        CSTUNClientGetSharedSecret(
            const CSTUNClientGetSharedSecret& aGetSharedSecret );

    private: // Data

        //Next possible states
        const CSTUNClientState& iReady;
    };

#endif // C_CSTUNCLIENTGETSHAREDSECRET_H
