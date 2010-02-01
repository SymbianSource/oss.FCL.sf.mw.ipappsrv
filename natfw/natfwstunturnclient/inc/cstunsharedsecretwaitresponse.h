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




#ifndef C_CSTUNSHAREDSECRETWAITRESPONSE_H
#define C_CSTUNSHAREDSECRETWAITRESPONSE_H

// INCLUDES
#include <e32base.h>
#include "cstunsharedsecretstate.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
 *  Waiting for Shared Secret Response or Shared Secret Error Response or to
 *  arrive from the STUN server. 
 */
class CSTUNSharedSecretWaitResponse : public CSTUNSharedSecretState
    {
    public: // Constructors and destructor

        CSTUNSharedSecretWaitResponse(
            const CSTUNSharedSecretState& aActive,
            const CSTUNSharedSecretState& aWaitToRetry );

          /**
        * Destructor
        */
        ~CSTUNSharedSecretWaitResponse();

    public: // From CSTUNSharedSecretState
    
        void IncomingMessageL( CSTUNSharedSecret& aContext,
                               CNATFWUNSAFMessage* aMessage ) const;
    
        void ErrorOccured( CSTUNSharedSecret& aContext, TInt aError ) const;

        void TimerExpiredL(    CSTUNSharedSecret& aContext ) const;

    private:

        CSTUNSharedSecretWaitResponse();

        CSTUNSharedSecretWaitResponse(
            const CSTUNSharedSecretWaitResponse& aWaitResponse );

    private: // Data

        //Next possible states
        const CSTUNSharedSecretState& iActive;
        const CSTUNSharedSecretState& iWaitToRetry;
    };

#endif // C_CSTUNSHAREDSECRETWAITRESPONSE_H
