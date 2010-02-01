/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    state class to resolve tls address
*
*/





#ifndef C_CSTUNCLIENTRESOLVINGTLS_H
#define C_CSTUNCLIENTRESOLVINGTLS_H

#include "cstunclientstate.h"

/**
 *  State class for dns resolver
 *  In this state, STUN client waits to receive the resolved TLS addresses of
 *  the STUN server. Addresses are stored to iTcpAddresses -array.
 *
 *  @lib   natfwstunclient.lib
 *  @since S60 v3.2
 */

class CSTUNClientResolvingTLS : public CSTUNClientState
    {
    
public:

    CSTUNClientResolvingTLS( const CSTUNClientState& aResolvingUDP );

    /**
     * Destructor.
     */
    ~CSTUNClientResolvingTLS();

public: // From CSTUNClientState     
    
    /**
     * From CSTUNClientState
     * Resolving server address has succeeded.
     *
     * @since   S60 v3.2
     * @param   aContext reference to calling implementation
     * @param   aObtainSharedSecret indicates whether shared secret is used
     * @return  void
     */
    void ResolvingCompletedL( CSTUNClientImplementation& aContext,
                              TBool aObtainSharedSecret ) const;
    
    /**
     * From CSTUNClientState
     * Resolving server address has failed.
     *
     * @since   S60 v3.2
     * @param   aContext reference to calling implementation
     * @param   aError The reason for the failure
     * @return  void
     */
    void ResolvingFailed( CSTUNClientImplementation& aContext,
                          TInt aError ) const;

private:

    CSTUNClientResolvingTLS();

private: // data

    /**
     * Next possible state
     * not own.
     */
    const CSTUNClientState& iResolvingUDP;
    };

#endif // C_CSTUNCLIENTRESOLVINGTLS_H
