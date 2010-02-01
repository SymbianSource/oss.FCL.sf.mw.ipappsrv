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




#ifndef C_CSTUNSHAREDSECRETWAITTORETRY_H
#define C_CSTUNSHAREDSECRETWAITTORETRY_H

// INCLUDES
#include <e32base.h>
#include "cstunsharedsecretstate.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
 *  When a Shared Secret Error Response with error code 5xx is received, wait
 *  a few seconds, then retry. 
 */
class CSTUNSharedSecretWaitToRetry : public CSTUNSharedSecretState
    {
    public: // Constructors and destructor

        CSTUNSharedSecretWaitToRetry();

          /**
        * Destructor
        */
        ~CSTUNSharedSecretWaitToRetry();
        
    public: // From CSTUNSharedSecretState

        void TimerExpiredL(    CSTUNSharedSecret& aContext ) const;

    public: // New functions
    
        /**
        * Set the neighbour states.
        * @param aWaitResponse WaitResponse state        
        */
        void SetNeighbourStates( const CSTUNSharedSecretState& aWaitResponse );

    private:

        CSTUNSharedSecretWaitToRetry(
            const CSTUNSharedSecretWaitToRetry& aWaitToRetry );

    private: // Data

        //Next possible states, not owned.
        const CSTUNSharedSecretState* iWaitResponse;
    };

#endif // C_CSTUNSHAREDSECRETWAITTORETRY_H
