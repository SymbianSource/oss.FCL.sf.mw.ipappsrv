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




#ifndef C_CSTUNCLIENTRESOLVINGTCP_H
#define C_CSTUNCLIENTRESOLVINGTCP_H

// INCLUDES
#include "cstunclientstate.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
 *  In this state, STUN client waits to receive the resolved TCP addresses of
 *  the STUN server. 
 */
class CSTUNClientResolvingTCP : public CSTUNClientState
    {
    public: // Constructors and destructor

        CSTUNClientResolvingTCP( const CSTUNClientState& aResolvingTLS,
                                 const CSTUNClientState& aResolvingUDP );
        
        CSTUNClientResolvingTCP( const CSTUNClientState& aResolvingUDP );

        ~CSTUNClientResolvingTCP();

    public: // From CSTUNClientState     

        void ResolvingCompletedL( CSTUNClientImplementation& aContext,
                                  TBool aObtainSharedSecret ) const;
        
        void ResolvingFailed( CSTUNClientImplementation& aContext,
                              TInt aError ) const;

    private:

        CSTUNClientResolvingTCP();


    private: // Data

        //Next possible states
        const CSTUNClientState& iResolvingTLS;
        const CSTUNClientState& iResolvingUDP;
    };

#endif // C_CSTUNCLIENTRESOLVINGTCP_H
