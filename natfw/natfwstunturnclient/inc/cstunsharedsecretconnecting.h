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




#ifndef C_CSTUNSHAREDSECRETCONNECTING_H
#define C_CSTUNSHAREDSECRETCONNECTING_H

// INCLUDES
#include <e32base.h>
#include "cstunsharedsecretstate.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
 *  Waiting for TLS connection to be formed. 
 */
class CSTUNSharedSecretConnecting : public CSTUNSharedSecretState
    {
    public: // Constructors and destructor

        CSTUNSharedSecretConnecting(
            const CSTUNSharedSecretState& aWaitResponse );

          /**
        * Destructor
        */
        ~CSTUNSharedSecretConnecting();

    public: // From CSTUNSharedSecretState
    
        void TlsConnectedL( CSTUNSharedSecret& aContext ) const;
    
        void ErrorOccured( CSTUNSharedSecret& aContext, TInt aError )  const;

        void TimerExpiredL( CSTUNSharedSecret& aContext ) const;

    private:

        CSTUNSharedSecretConnecting();

        CSTUNSharedSecretConnecting(
            const CSTUNSharedSecretConnecting& aConnecting );

    private: // Data

        //Next possible states
        const CSTUNSharedSecretState& iWaitResponse;
    };

#endif // C_CSTUNSHAREDSECRETCONNECTING_H
