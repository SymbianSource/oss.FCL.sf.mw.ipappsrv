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




#ifndef C_CSTUNCLIENTRENEWSHAREDSECRET_H
#define C_CSTUNCLIENTRENEWSHAREDSECRET_H

// INCLUDES
#include <e32base.h>
#include "cstunclientstate.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
 *  In this state, STUN client is obtaining a shared secret from the STUN
 *  server.
 */
class CSTUNClientRenewSharedSecret : public CSTUNClientState
    {
    public: // Constructors and destructor

        CSTUNClientRenewSharedSecret();

        ~CSTUNClientRenewSharedSecret();

    public: // From CSTUNClientState     

        void SharedSecretReceivedL( CSTUNClientImplementation& aContext ) const;
        
        void SharedSecretErrorL( CSTUNClientImplementation& aContext,
                                 TInt aError ) const;

        void ObtainSharedSecretL( CSTUNClientImplementation& aContext,
                                  CBinding& aBinding ) const;

        TBool SharedSecretRejectedL( CSTUNClientImplementation& aContext,
                                     const CBinding& aBinding,
                                     const TDesC8& aUsername,
                                        const TDesC8& aPassword ) const;

    public: // New functions
        
        void SetNeighbourStates( const CSTUNClientState& aReady );

    private:

        CSTUNClientRenewSharedSecret(
            const CSTUNClientRenewSharedSecret& aRenewSharedSecret );

    private: // Data

        //Next possible states
        const CSTUNClientState* iReady;
    };

#endif // C_CSTUNCLIENTRENEWSHAREDSECRET_H
