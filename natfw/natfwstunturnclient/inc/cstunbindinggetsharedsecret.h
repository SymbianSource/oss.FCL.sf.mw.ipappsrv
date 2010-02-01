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




#ifndef C_CSTUNBINDINGGETSHAREDSECRET_H
#define C_CSTUNBINDINGGETSHAREDSECRET_H

// INCLUDES
#include "cstunbindingstate.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
 *  In this state, the binding is waiting to get a shared secret.
 *
 *  @lib ?library
 *  @since S60 ?S60_version *** for example, S60 v3.0
 */
class CSTUNBindingGetSharedSecret : public CSTUNBindingState
    {
    public: // Constructors and destructor

        /**
        * Constructor
        * @param aGetAddress GetAddress state
        */
        CSTUNBindingGetSharedSecret( CSTUNBindingState& aGetAddress );

          /**
        * Destructor
        */
        virtual ~CSTUNBindingGetSharedSecret();

    public: // From CSTUNBindingState

        void SharedSecretObtainedL( CBindingImplementation& aContext,
                                    const TDesC8& aUsername,
                                    const TDesC8& aPassword ) const;

        void SendRequestL( CBindingImplementation& aContext ) const;

    private:

        CSTUNBindingGetSharedSecret();

        CSTUNBindingGetSharedSecret(
            const CSTUNBindingGetSharedSecret& aGetSharedSecret );

    private: // Data

        //Next possible states
        CSTUNBindingState& iGetPublicAddress;
    };

#endif // C_CSTUNBINDINGGETSHAREDSECRET_H
