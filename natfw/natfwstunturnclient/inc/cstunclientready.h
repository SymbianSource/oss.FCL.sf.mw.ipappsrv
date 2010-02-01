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




#ifndef C_CSTUNCLIENTREADY_H
#define C_CSTUNCLIENTREADY_H

// INCLUDES
#include "cstunclientstate.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
 *  In this state, STUN client is ready to be used for obtaining bindings.  
 */
class CSTUNClientReady : public CSTUNClientState
    {
    public: // Constructors and destructor

        CSTUNClientReady( const CSTUNClientState& aRenewSharedSecret );

        ~CSTUNClientReady();

    public: // From CSTUNClientState

        void ObtainSharedSecretL( CSTUNClientImplementation& aContext,
                                  CBinding& aBinding ) const;

        TBool SharedSecretRejectedL( CSTUNClientImplementation& aContext,
                                     const CBinding& aBinding,
                                     const TDesC8& aUsername,
                                        const TDesC8& aPassword ) const;

    private:

        CSTUNClientReady();

        CSTUNClientReady( const CSTUNClientReady& aReady );
        
    private: // Data

        //Next possible states
        const CSTUNClientState& iRenewSharedSecret;
    };

#endif // C_CSTUNCLIENTREADY_H
