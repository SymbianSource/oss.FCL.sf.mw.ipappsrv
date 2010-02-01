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




#ifndef C_CSTUNCLIENTRESOLVINGUDP_H
#define C_CSTUNCLIENTRESOLVINGUDP_H

// INCLUDES
#include "cstunclientstate.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
 *  In this state, STUN client waits to receive the resolved UDP addresses of
 *  the STUN server. 
 */
class CSTUNClientResolvingUDP : public CSTUNClientState
    {
    public: // Constructors and destructor

        CSTUNClientResolvingUDP( const CSTUNClientState& aGetSharedSecret,
                                 const CSTUNClientState& aReady );

        ~CSTUNClientResolvingUDP();

    public: // From CSTUNClientState     

        void ResolvingCompletedL( CSTUNClientImplementation& aContext,
                                  TBool aObtainSharedSecret ) const;
        
        void ResolvingFailed( CSTUNClientImplementation& aContext,
                              TInt aError ) const;        

    private:

        CSTUNClientResolvingUDP();

        CSTUNClientResolvingUDP( const CSTUNClientResolvingUDP& aResolvingUDP );

    private: // Data

        //Next possible states
        const CSTUNClientState& iGetSharedSecret;
        const CSTUNClientState& iReady;
    };

#endif // C_CSTUNCLIENTRESOLVINGTCP_H
